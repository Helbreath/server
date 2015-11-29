#include "Gate.h"
#include "LServer.h"
#include "GServer.h"
#include "Client.h"
#include "Map.h"
#include "streams.h"
#include "netmessages.h"

Gate * Gate::_instance = nullptr;

Gate::Gate()
	: io_service_(),
	signals_(io_service_),
	acceptor_(io_service_),
	new_connection_(new connection(io_service_,
	*this, request_handler_))
{
	//asio socket thread count
	thread_pool_size_ = 1;

	currentplayersonline = 0;
	currentconnections = 0;

	sqlpool = 0;

	MySQL::Connector::registerConnector();
	SQLite::Connector::registerConnector();


	L = luaL_newstate();
	luaL_openlibs(L);
	TimerThreadRunning = false;

	// Register to handle the signals that indicate when the server should exit.
	// It is safe to register for the same signal multiple times in a program,
	// provided all registration for the specified signal is made through Asio.
	signals_.add(SIGINT);
	signals_.add(SIGTERM);
#if defined(SIGQUIT)
	signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)
	signals_.async_wait(boost::bind(&Gate::handle_stop, this));

}

Gate::~Gate()
{
	for (GServer * server : gameserver)
	{
		if (!server)
			continue;
		server->runthread->join();
		delete server;
	}
	if (sqlpool)
		delete sqlpool;
	MySQL::Connector::unregisterConnector();
	SQLite::Connector::unregisterConnector();
}

void Gate::run()
{
	try
	{
		serverstatus = SERVERSTATUS_ONLINE;
		logger->information("Creating Gate threads.");


		// Create a pool of threads to run all of the io_services.
		std::vector<shared_ptr<boost::thread> > threads;
		for (std::size_t i = 0; i < thread_pool_size_; ++i)
		{
			boost::shared_ptr<boost::thread> thread(new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_)));
			threads.push_back(thread);
		}

		socketthread = thread(std::bind(std::mem_fun(&Gate::SocketThread), this));
		timerthread = thread(std::bind(std::mem_fun(&Gate::TimerThread), this));

		// Wait for all threads in the pool to exit.
		for (std::size_t i = 0; i < threads.size(); ++i)
			threads[i]->join();
		//io_service_.run();

		socketthread.join();
		timerthread.join();

		lua_close(L);


	}
	catch (std::exception& e)
	{
		logger->fatal(Poco::format("Gate::run() Exception: %s", e.what() ));
	}
}

void Gate::stop()
{
	// Post a call to the stop function so that server::stop() is safe to call
	// from any thread.
	io_service_.post(boost::bind(&Gate::handle_stop, this));
}

void Gate::handle_accept(const boost::system::error_code& e)
{
	if (!e)
	{
		connection_ptr clientsocket = new_connection_;

		currentconnections++;

		boost::asio::ip::tcp::endpoint endp = clientsocket->socket().remote_endpoint();
		boost::asio::ip::address address = endp.address();
		clientsocket->address = address.to_string();
		logger->information(Poco::format("Client connected [%?d]: %s:%?d", currentconnections, address.to_string(), (int)endp.port()));


		shared_ptr<Client> client(new Client());
		client->socket = (clientsocket);
		client->address = clientsocket->address;
		clientsocket->client_ = client;

		client->m_dwSPTime = client->m_dwMPTime =
			client->m_dwHPTime = client->m_dwAutoSaveTime =
			client->m_dwTime = client->m_dwHungerTime = client->m_dwExpStockTime =
			client->m_dwRecentAttackTime = client->m_dwAutoExpTime = client->lastpackettime = unixtime();

		clientlist.push_back(client);

		start(new_connection_);
		new_connection_.reset(new connection(io_service_,
			*this, request_handler_));
		acceptor_.async_accept(new_connection_->socket(),
			boost::bind(&Gate::handle_accept, this,
			boost::asio::placeholders::error));


// 		m_iTotalClients++;
//
// 		if (m_iTotalClients > m_iMaxClients)
// 		{
// 			m_iMaxClients = m_iTotalClients;
// 		}
	}
}

void Gate::handle_stop()
{
	// The server is stopped by cancelling all outstanding asynchronous
	// operations. Once all operations have finished the io_service::run() call
	// will exit.
	serverstatus = SERVERSTATUS_SHUTDOWN;
	LServer::GetSingleton().serverstatus = SERVERSTATUS_SHUTDOWN;
	for (GServer * server : gameserver)
	{
		server->serverstatus = SERVERSTATUS_SHUTDOWN;
	}
	acceptor_.close();
	stop_all();
}

void Gate::start(connection_ptr c)
{
	connections_.insert(c);
	c->start();
}

void Gate::stop(connection_ptr c)
{
	if (c == nullptr)
	{
		logger->error("Invalid socket being closed");
		return;
	}
	shared_ptr<Client> client = c->client_.lock();
	if (!client)
	{
		logger->error(Poco::format("Socket with no client attached closed <%s>", c->address));
		return;
	}
	if (client->disconnecttime > 0)
	{
		//socket is already disconnected (re-call from connection::handle_read after a server invoked socket close)
		return;
	}
	try
	{
		//fyi, client_ should always be set, the question is whether m_handle is set (indicates actually made it in game)
		if (client->handle > 0)
		{
			logger->error(Poco::format("<%?d> Client Disconnected! (%s) (%s)", client->handle, client->name, c->address));
			if ((unixtime() - client->logoutHackCheck) < 10000)
			{
				logger->error(Poco::format("Logout Hack: (%s) Player: (%s) - disconnected within 10 seconds of most recent damage. Hack? Lag?", c->address, client->name));
			}
		}
		else
		{
			logger->error(Poco::format("Client Disconnected! (%s)", c->address));
		}
		//TODO: force player entities to stay in game for a few seconds after a disconnect? - guarantees no logout hack occurs. 10 seconds? resets on being attacked in case they pre-emptively logout?

		connections_.erase(c);
		c->stop();
		//post socket closing

	}
	catch (std::exception& e)
	{
		logger->error(Poco::format("exception: ", e.what()));
	}
}

void Gate::stop_all()
{
	for (shared_ptr<connection> c : connections_)
	{
		c->stop();
	}
	// \/ this = that /\
// 	std::for_each(connections_.begin(), connections_.end(),
// 		boost::bind(&connection::stop, _1));
	connections_.clear();
}

void stack_dump(lua_State *stack);

bool Gate::Init(string config)
{
	configfile = config;
	try
	{
		using Poco::JSON::Parser;
		using Poco::Dynamic::Var;
		using Poco::JSON::Object;
		using Poco::JSON::Array;

		Parser * parser = new Parser();
		std::ifstream config(configfile.c_str());
		Var parsed = parser->parse(config);
		Var parsedResult = parser->result();

		logger->information(Poco::format("Loading %s", configfile));

		Object::Ptr configOptions = parsedResult.extract<Object::Ptr>();
		if (configOptions)
		{
			bindaddress = configOptions->get("bindaddress").convert<string>();
			logger->information(Poco::format("bindaddress: %s", bindaddress));
			
			bindport = configOptions->get("bindport").convert<string>();
			logger->information(Poco::format("bindport: %s", bindport));
			
			sqlhost = configOptions->get("sqlhost").convert<string>();
			logger->information(Poco::format("sqlhost: %s", sqlhost));
			
			sqluser = configOptions->get("sqluser").convert<string>();
			logger->information(Poco::format("sqluser: %s", sqluser));
			
			sqlpass = configOptions->get("sqlpass").convert<string>();
			logger->information("sqlpass set");
			
			sqldb = configOptions->get("sqldb").convert<string>();
			logger->information(Poco::format("sqldb: %s", sqldb));

			if (configOptions->has("servers"))
			{
				Array::Ptr servers = configOptions->getArray("servers");
				for (int i = 0; i < servers->size(); ++i)
				{
					Object::Ptr server = servers->getObject(i);
					string servername;
					string serverconfig;
					if (server->has("name"))
						servername = server->get("name").convert<string>();
					if (server->has("config"))
						serverconfig = server->get("config").convert<string>();
					GServer * hg = new GServer(servername);
					hg->logger = XLogger::GetSingleton();
					if (!hg->Init(serverconfig))
					{
						return false;
					}
					hg->runthread = new std::thread(std::bind(std::mem_fun(&GServer::run), hg));
					gameserver.push_back(hg);
				}
			}
		}

		LServer::CreateInstance();
		LServer::GetSingleton().logger = XLogger::GetSingleton();
		LServer::GetSingleton().Init("loginserver.lua");
		LServer::GetSingleton().runthread = thread(std::bind(std::mem_fun(&LServer::run), &LServer::GetSingleton()));

		return true;
	}
	catch (exception& e)
	{
		logger->fatal(Poco::format("Init() Exception: %s", (string)e.what()));
		return false;
	}
	catch(...)
	{
		logger->fatal("Unspecified Init() Exception.");
		return false;
	}
}

void stack_dump(lua_State *stack)
{
	int stackSize = lua_gettop(stack);

	std::cout << "Stack Dump: " << std::endl;

	for (int i = stackSize; i > 0; --i)
	{
		int absoluteIndex = i;
		int relativeIndex = stackSize - i + 1;

		std::cout << "Index[" << absoluteIndex << " / -" << relativeIndex << "] = ";

		int type = lua_type(stack, i);
		switch (type)
		{
		case LUA_TSTRING:
			std::cout << lua_tostring(stack, i);
			break;
		case LUA_TBOOLEAN:
			std::cout << lua_toboolean(stack, i);
			break;
		case LUA_TNUMBER:
			std::cout << lua_tonumber(stack, i);
			break;
		case LUA_TUSERDATA:
		case LUA_TLIGHTUSERDATA:
			std::cout << lua_touserdata(stack, i);
			break;
		default:
			std::cout << lua_typename(stack, type);
		}

		std::cout << std::endl;
	}

	std::cout << std::endl;
}

bool Gate::InitSockets()
{
	try
	{
		// Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
		boost::asio::ip::tcp::resolver resolver(io_service_);
		boost::asio::ip::tcp::resolver::query query(bindaddress, bindport);
		boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
		acceptor_.open(endpoint.protocol());
		acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		try {
			acceptor_.bind(endpoint);
		}
		catch (std::exception&)
		{
			throw std::runtime_error("Invalid bind address or port already in use! Exiting.");
		}

		// Finally listen on the socket and start accepting connections
		acceptor_.listen();
		acceptor_.async_accept(new_connection_->socket(),
			boost::bind(&Gate::handle_accept, this,
			boost::asio::placeholders::error));

		return true;
	}
	catch (std::exception& e)
	{
		logger->fatal(Poco::format("Init() Exception: %s", (string)e.what()));
		return 0;
	}
	catch(...)
	{
		logger->fatal("Unspecified Init() Exception.");
		return 0;
	}
}

void Gate::SocketThread()
{
#ifndef WIN32
	struct timespec req={0};
	req.tv_sec = 0;
	req.tv_nsec = 1000000L;//1ms
#else
	_tzset();
#endif

	while (serverstatus == SERVERSTATUS_ONLINE)
	{
	    if (socketpipe.size() > 0)
        {
			lock_guard<mutex> lock(Gate::GetSingleton().mutpacketlist);
			shared_ptr<MsgQueueEntry> msg = GetMsgQueue(socketpipe);

			StreamRead sr = StreamRead(msg->data, msg->size);
			StreamWrite sw = StreamWrite();
            
			shared_ptr<Client> client = msg->client;

			uint32_t msgid = sr.ReadInt();
			client->lastpackettime = unixtime();

			if (client->currentstatus == 1)
			{
				LServer::GetSingleton().PutMsgQueue(msg, LServer::GetSingleton().socketpipe);
			}
			else if (client->currentstatus == 2)
			{
				if (client->gserver)
				{
					if (msgid == MSGID_COMMAND_CHATMSG || msgid == MSGID_COMMAND_CHECKCONNECTION)
					{
						lock_guard<mutex> lock(client->gserver->mutchat);
						client->gserver->PutMsgQueue(msg, client->gserver->chatpipe);
					}
					else
					{
						lock_guard<mutex> lock(client->map->mutaction);
						client->gserver->PutMsgQueue(msg, msg->client->map->actionpipe);
					}
				}
			}

        }

#ifdef WIN32
		Sleep(1);
#else
		nanosleep(&req,0);
#endif
	}
	logger->information("ActionThread() complete.");
}

void Gate::TimerThread()
{
#ifndef WIN32
	struct timespec req={0};
	req.tv_sec = 0;
	req.tv_nsec = 1000000L;//1ms
#else
	_tzset();
#endif

	TimerThreadRunning = true;

	uint64_t t1htimer;
	uint64_t t30mintimer;
	uint64_t t6mintimer;
	uint64_t t5mintimer;
	uint64_t t3mintimer;
	uint64_t t1mintimer;
	uint64_t t30sectimer;
	uint64_t t5sectimer;
	uint64_t t1sectimer;
	uint64_t t100msectimer;
	uint64_t ltime;

	t1htimer = t30mintimer = t6mintimer = t5mintimer = t3mintimer = t1mintimer = t30sectimer = t5sectimer = t1sectimer = t100msectimer = unixtime();

	while (serverstatus == SERVERSTATUS_ONLINE)
	{
		try
		{
			ltime = ltime = unixtime();

			//for ()

			if (t100msectimer < ltime)
			{
				t100msectimer += 100;
			}
			if (t1sectimer < ltime)
			{
				t1sectimer += 1000;

				std::list<shared_ptr<Client>> templist = clientlist;
				for (shared_ptr<Client> clnt : templist)
				{
					Client * client = clnt.get();
					//check clients for disconnections that need deleting
					if (client->disconnecttime != 0 && /*(*iter)->currentstatus == 2 &&*/ client->disconnecttime + 10000 < ltime)
					{
						//client persists for 10 seconds
						logger->information(Poco::format("Client Object Deletion! <%s>", client->address));
						{
							shared_lock_guard<shared_mutex> lock(mutclientlist);
							if (client->currentstatus == 2)
							{
								DeleteClient(clnt, true, true);//in game
							}
							else
							{
								DeleteClient(clnt, false, true);
							}
						}
						{
							lock_guard<shared_mutex> lock(mutclientlist);
							clientlist.remove(clnt);
							if (client->gserver)
								client->gserver->clientlist.remove(clnt);
						}
						continue;
					}

					//5 mins to sit at char select screen
					if ((client->currentstatus == 1) && (client->lastpackettime + 5*60*1000 < ltime))
					{
						logger->information(Poco::format("Client Char Select Timeout! <%s>", client->address));
						DeleteClient(clnt, false, true);
						clientlist.remove(clnt);
						continue;
					}

					//check connections for recent data (ghost sockets)
// 					if (client->disconnecttime == 0 && client->lastpackettime + 30000 < ltime && (client->currentstatus != 1))
// 					{
// 						//socket idle for 30 seconds (should never happen unless disconnected)
// 						poco_information(*logger, Poco::format("Client Timeout! <%s>", client->address));
// 						DeleteClient(clnt, true, true);
// 						mutclientlist.unlock_upgrade_and_lock();
// 						iter = clientlist.erase(iter);
// 						mutclientlist.unlock_and_lock_upgrade();
// 						continue;
// 					}
				}
			}
			if (t5sectimer < ltime)
			{
				t5sectimer += 5000;
			}
			if (t30sectimer < ltime)
			{
				t30sectimer += 30000;
			}
			if (t1mintimer < ltime)
			{
				t1mintimer += 60000;
			}
			if (t3mintimer < ltime)
			{
				t3mintimer += 180000;
			}
			if (t6mintimer < ltime)
			{
				t6mintimer += 360000;
			}
			if (t1htimer < ltime)
			{

				t1htimer += 3600000;
			}
	#ifdef WIN32
			Sleep(1);
	#else
			nanosleep(&req,0);
	#endif
		}
		catch (Poco::Data::MySQL::MySQLException * e)
		{
			logger->critical(Poco::format("TimerThread() SQL Exception: %s", e->displayText() ));
		}
		catch (std::exception & e)
		{
			logger->fatal(Poco::format("TimerThread() Exception: %s", string(e.what())));
			TimerThreadRunning = false;
			//TODO: invoke server shutdown/restart?
			//timer thread should never exit - needs some sort of something here
			return;
		}
		catch (...)
		{
			logger->fatal("Unspecified Gate::TimerThread() Exception.");
			TimerThreadRunning = false;
			//TODO: invoke server shutdown/restart?
			//timer thread should never exit - needs some sort of something here
			return;
		}
	}
	TimerThreadRunning = false;
	logger->information("TimerThread() complete.");

	return;
}

//Base DeleteClient
void Gate::DeleteClient(shared_ptr<Client> client, bool save, bool deleteobj)
{

	if (!client) return;

// 	upgrade_lock<shared_mutex> upgradelist(mutclientlist);
// 	mutclientlist.unlock_and_lock_upgrade();
	//upgrade_lock<shared_mutex> upgradelist(mutclientlist);

	if (client->socket)
	{
		//lock_guard<mutex> lock(client->socket->mtx);
		stop(client->socket);
	}
	client->currentstatus = 0;
	client->disconnecttime = unixtime();

	if (deleteobj)
	{
		if (!client->gserver)
		{
			//__debugbreak();
			//client disconnects/times out before login or at char select screen
			return;
		}
		//delete the object
		//might already have ownership of gatelock
		// 

		//need to perform client removal
		//remove from map
		Client * player = client.get();
	
		if (player->IsDead())
			player->map->ClearDeadOwner(player->x, player->y);
		else
			player->map->ClearOwner(player->x, player->y);

		//let npcs know it's an invalid target
		player->m_bActive = false;

		player->gserver->SendEventToNearClient_TypeA(player, MSGID_MOTION_EVENT_REJECT, 0, 0, 0);


		if (save)
		{
			if (player->IsDead())
			{
				player->x = -1;
				player->y = -1;

				if (player->IsNeutral())
				{
					player->mapName = sideMap[NEUTRAL];
				}
				else
				{
					if (player->gserver->m_bIsCrusadeMode)
					{
						if (player->deadPenaltyTime > 0)
						{
							player->lockedMapName = sideMap[player->side];
							player->deadPenaltyTime = 60*10;//10 minutes
							player->LockMap(sideMap[player->side], 300);
						}
						else
							player->deadPenaltyTime = 60*10;

					}

					if (player->side == ARESDEN)
					{
						if ((player->mapName == sideMap[ELVINE]) && !player->IsGM())
						{
							player->LockMap(sideMapJail[ELVINE], 180);
						}
						else if (player->level > 80)
						{
							player->mapName = "resurr1";
						}
						else
						{
							player->mapName = "aresden";
						}
					}
					else// if (player->m_side == ELVINE)
					{
						if ((player->mapName == sideMap[ARESDEN]) && !player->IsGM())
						{
							player->lockedMapName = sideMapJail[ARESDEN];
							player->lockedMapTime = 60*3;
						}
						else if (player->level > 80)
						{
							player->mapName = "resurr2";
						}
						else
						{
							player->mapName = "elvine";
						}
					}
				}
			}


			if (player->observerMode)
			{
				player->x = -1;
				player->y = -1;
				player->mapName = sideMap[player->side];
			}

			if (player->m_bIsInitComplete)
			{
				//TODO: savedata here
			}
		}

		//any non-save specific stuff goes here

		//Update party status and stuff (notify party of disconnect and set status)
		//player->gserver->partyMgr
		 
		//Update friends of disconnect
		//player->gserver->friends
	}
}

bool Gate::AccountInUse(shared_ptr<Client> client)
{
	bool test = false;
	for (GServer * gserver : gameserver)
	{
		for (shared_ptr<Client> clnt : gserver->clientlist)
		{
			if (clnt->account == client->account)
			{
				test = true;
			}
		}
	}
	return test;
}
