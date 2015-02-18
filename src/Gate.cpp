#include "Gate.h"
#include "LServer.h"
#include "GServer.h"
#include "Client.h"
#include "Map.h"
#include "streams.h"
#include "netmessages.h"

Gate::Gate()
	: io_service_(),
	signals_(io_service_),
	acceptor_(io_service_),
	new_connection_(new connection(io_service_,
	*this, request_handler_)),
	Server(string("config.lua"))
{
	loginserver = 0;

	//asio socket thread count
	thread_pool_size_ = 1;

	currentplayersonline = 0;
	currentconnections = 0;

	sqlpool = 0;

	pFCConsole = new FormattingChannel(new PatternFormatter("Gate: %p: %t"));
	pFCConsole->setChannel(new ConsoleChannel);
	pFCConsole->open();

	pFCDefault= new FormattingChannel(new PatternFormatter("Gate: %t"));
	pFCDefault->setChannel(new ConsoleChannel);
	pFCDefault->open();

	pFCFile = new FormattingChannel(new PatternFormatter("%Y-%m-%d %H:%M:%S.%c | %s:%q:%t"));
	pFCFile->setChannel(new FileChannel("gateserver.log"));
	pFCFile->setProperty("rotation", "daily");
	pFCFile->setProperty("times", "local");
	pFCFile->open();

	consoleLogger = &Poco::Logger::create("ConsoleLogger", pFCConsole, Message::PRIO_TRACE);
	logger		  = &Poco::Logger::create("DefaultLogger", pFCDefault, Message::PRIO_TRACE);
	fileLogger    = &Poco::Logger::create("FileLogger", pFCFile, Message::PRIO_TRACE);
	poco_information(*logger, "");
// 	consoleLogger->error("An error message");
// 	consoleLogger->warning("A warning message");
// 	consoleLogger->information("An information message");
// 	fileLogger->warning("An information message");
// 	if ((consoleLogger)->information())
// 		(consoleLogger)->information("Another informational message", __FILE__, __LINE__);
// 	Poco::Logger::get("ConsoleLogger").error("Another error message");


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
	loginserver->runthread->join();
	delete loginserver;
	for (GServer * server : gameserver)
	{
		server->runthread->join();
		delete server;
	}
	delete sqlpool;
	MySQL::Connector::unregisterConnector();
	SQLite::Connector::unregisterConnector();
	Poco::Logger::destroy("ConsoleLogger");
	Poco::Logger::destroy("FileLogger");
	pFCConsole->close();
	pFCDefault->close();
	pFCFile->close();
}
void Gate::run()
{
	try
	{
		serverstatus = SERVERSTATUS_ONLINE;
		logger->information("Creating Gate threads.");


		// Create a pool of threads to run all of the io_services.
		std::vector<boost::shared_ptr<boost::thread> > threads;
		for (std::size_t i = 0; i < thread_pool_size_; ++i)
		{
			boost::shared_ptr<boost::thread> thread(new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_)));
			threads.push_back(thread);
		}

		socketthread = new std::thread(std::bind(std::mem_fun(&Gate::SocketThread), this));
		timerthread = new std::thread(std::bind(std::mem_fun(&Gate::TimerThread), this));

		// Wait for all threads in the pool to exit.
		for (std::size_t i = 0; i < threads.size(); ++i)
			threads[i]->join();
		//io_service_.run();

		socketthread->join();
		timerthread->join();

		delete socketthread;
		delete timerthread;

		lua_close(L);


	}
	catch (std::exception& e)
	{
		consoleLogger->fatal(Poco::format("Gate::run() Exception: %s", e.what() ));
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
		consoleLogger->information(Poco::format("Client connected [%?d]: %s:%?d", currentconnections, address.to_string(), (int)endp.port()));


		shared_ptr<Client> client(new Client());
		client->self = client;
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
	loginserver->serverstatus = SERVERSTATUS_SHUTDOWN;
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
	shared_ptr<Client> client = c->client_.lock();
	if (!client)
	{
		consoleLogger->trace(Poco::format("Socket with no client attached closed <%s>", c->address));
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
		if (client->m_handle > 0)
		{
			consoleLogger->error(Poco::format("<%?d> Client Disconnected! (%s) (%s)", client->m_handle, client->m_cCharName, c->address));
			if ((unixtime() - client->m_dwLogoutHackCheck) < 10000)
			{
				consoleLogger->error(Poco::format("Logout Hack: (%s) Player: (%s) - disconnected within 10 seconds of most recent damage. Hack? Lag?", c->address, client->m_cCharName));
			}
		}
		else
		{
			consoleLogger->error(Poco::format("Client Disconnected! (%s)", c->address));
		}
		//TODO: force player entities to stay in game for a few seconds after a disconnect? - guarantees no logout hack occurs. 10 seconds? resets on being attacked in case they pre-emptively logout?

		connections_.erase(c);
		c->stop();
		//post socket closing

	}
	catch (std::exception& e)
	{
		consoleLogger->error(Poco::format("exception: ", e.what()));
	}
}

void Gate::stop_all()
{
	for (boost::shared_ptr<connection> c : connections_)
	{
		c->stop();
	}
	// \/ this = that /\
// 	std::for_each(connections_.begin(), connections_.end(),
// 		boost::bind(&connection::stop, _1));
	connections_.clear();
}
void stack_dump(lua_State *stack);

bool Gate::Init()
{
	try
	{
		consoleLogger->information("Loading Config.");
		if (luaL_dofile(L, "config.lua") != 0)
		{
			consoleLogger->fatal(Poco::format("%s", (string)lua_tostring(L,-1)));
			return false;
		}
		lua_getglobal(L, "config");

		char * temp;

		//ip address
		{
			lua_getfield(L, -1, "bindip");
			temp = (char*)lua_tostring(L, -1);
			if (temp == 0) { consoleLogger->fatal("Invalid bindip setting."); return false; }
			bindaddress = temp;
			logger->information(Poco::format("bindip: %s", bindaddress));
			if (bindaddress.length() == 0 || bindaddress.length() > 15)
			{
				consoleLogger->fatal("Invalid bindip setting.");
				return false;
			}
			lua_pop(L, 1);
		}

		//player connect port
		{
			uint16_t tbp = 0;
			lua_getfield(L, -1, "bindport");
			temp = (char*)lua_tostring(L, -1);
			if (temp == 0) { consoleLogger->fatal("Invalid bindport setting."); return false; }
			tbp = atoi(temp);
			logger->information(Poco::format("bindport: %?d", tbp));
			if (tbp < 1 || tbp > 65534)
			{
				consoleLogger->fatal("Invalid bindport setting.");
				return false;
			}
			bindport = temp;
			lua_pop(L, 1);
		}

		//sql host
		{
			lua_getfield(L, -1, "sqlhost");
			temp = (char*)lua_tostring(L, -1);
			if (temp == 0) { consoleLogger->fatal("Invalid sqlhost setting."); return false; }
			sqlhost = temp;
			logger->information(Poco::format("sqlhost: %s", sqlhost));
			lua_pop(L, 1);
		}

		//sql user
		{
			lua_getfield(L, -1, "sqluser");
			temp = (char*)lua_tostring(L, -1);
			if (temp == 0) { consoleLogger->fatal("Invalid sqluser setting."); return false; }
			sqluser = temp;
			logger->information(Poco::format("sqluser: %s", sqluser));
			lua_pop(L, 1);
		}

		//sql pass
		{
			lua_getfield(L, -1, "sqlpass");
			temp = (char*)lua_tostring(L, -1);
			if (temp == 0) { consoleLogger->fatal("Invalid sqlpass setting."); return false; }
			logger->information("sqlpass set");
			sqlpass = temp;
			lua_pop(L, 1);
		}

		//server configs
		{
			lua_getfield(L, -1, "servers");
			if(lua_istable(L,-1))
			{
				uint8_t tableSize = uint8_t(lua_rawlen(L, -1));
				for (uint8_t i = 1; i <= tableSize; ++i)
				{
					lua_pushinteger(L, i);
					lua_gettable(L, -2);

					lua_pushinteger(L, 1);
					lua_gettable(L, -2);
					string servername = (char*)lua_tostring(L, -1);
					lua_pop(L, 1);

					lua_pushinteger(L, 2);
					lua_gettable(L, -2);
					string serverconfig = (char*)lua_tostring(L, -1);
					lua_pop(L, 1);

					lua_pop(L, 1);

					GServer * hg = new GServer(servername, serverconfig);
					if (!hg->Init())
					{
						lua_pop(L,1);
						lua_pop(L,1);
						lua_pop(L,1);
						return false;
					}
					hg->gate = this;
					hg->runthread = new std::thread(std::bind(std::mem_fun(&GServer::run), hg));
					gameserver.push_back(hg);
				}
				lua_pop(L,1);
			}
			lua_pop(L, 1);
		}

		lua_pop(L, 1);

		loginserver = new LServer(string("loginserver.lua"));
		loginserver->Init();
		loginserver->gate = this;
		loginserver->runthread = new std::thread(std::bind(std::mem_fun(&LServer::run), loginserver));

		return true;
	}
	catch (std::exception& e)
	{
		consoleLogger->fatal(Poco::format("Init() Exception: %s", (string)e.what()));
		return false;
	}
	catch(...)
	{
		consoleLogger->fatal("Unspecified Init() Exception.");
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
		catch (std::exception& e)
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
		consoleLogger->fatal(Poco::format("Init() Exception: %s", (string)e.what()));
		return 0;
	}
	catch(...)
	{
		consoleLogger->fatal("Unspecified Init() Exception.");
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
			shared_ptr<MsgQueueEntry> msg = GetMsgQueue(socketpipe);

			StreamRead sr = StreamRead(msg->data, msg->size);
			StreamWrite sw = StreamWrite();
            
            shared_ptr<Client> client = msg->client;

			uint32_t msgid = sr.ReadInt();
			client->lastpackettime = unixtime();

			if (client->currentstatus == 1)
			{
				loginserver->PutMsgQueue(msg, loginserver->socketpipe);
			}
			else if (client->currentstatus == 2)
			{
				if (client->gserver)
				{
					if (msgid == MSGID_COMMAND_CHATMSG || msgid == MSGID_COMMAND_CHECKCONNECTION)
					{
						client->gserver->mutchat.lock();
						client->gserver->PutMsgQueue(msg, client->gserver->chatpipe);
						client->gserver->mutchat.unlock();
					}
					else
					{
						client->gserver->mutaction.lock();
						client->gserver->PutMsgQueue(msg, client->gserver->actionpipe);
						client->gserver->mutaction.unlock();
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

			if (t100msectimer < ltime)
			{
				t100msectimer += 100;
			}
			if (t1sectimer < ltime)
			{
				t1sectimer += 1000;
				mutclientlist.lock_upgrade();
				for (std::list<shared_ptr<Client>>::iterator iter = clientlist.begin(); iter != clientlist.end();)
				{
					shared_ptr<Client> clnt = (*iter);
					Client * client = (*iter).get();
					//check clients for disconnections that need deleting
					if (client->disconnecttime != 0 && /*(*iter)->currentstatus == 2 &&*/ client->disconnecttime + 10000 < ltime)
					{
						//client persists for 10 seconds
						poco_information(*logger, Poco::format("Client Object Deletion! <%s>", client->address));
						if (client->currentstatus == 2)
						{
							DeleteClient(clnt, true, true);//in game
						}
						else
						{
							DeleteClient(clnt, false, true);
						}
						mutclientlist.unlock_upgrade_and_lock();
						iter = clientlist.erase(iter);
						if (client->gserver)
							client->gserver->clientlist.remove(clnt);
						mutclientlist.unlock_and_lock_upgrade();
						continue;
					}

					//5 mins to sit at char select screen
					if ((client->currentstatus == 1) && (client->lastpackettime + 5*60*1000 < ltime))
					{
						poco_information(*logger, Poco::format("Client Char Select Timeout! <%s>", client->address));
						DeleteClient(clnt, false, true);
						mutclientlist.unlock_upgrade_and_lock();
						iter = clientlist.erase(iter);
						mutclientlist.unlock_and_lock_upgrade();
						continue;
					}

					//check connections for recent data (ghost sockets)
					if (client->disconnecttime == 0 && client->lastpackettime + 30000 < ltime && (client->currentstatus != 1))
					{
						//socket idle for 30 seconds (should never happen unless disconnected)
						poco_information(*logger, Poco::format("Client Timeout! <%s>", client->address));
						DeleteClient(clnt, false, true);
						mutclientlist.unlock_upgrade_and_lock();
						iter = clientlist.erase(iter);
						mutclientlist.unlock_and_lock_upgrade();
						continue;
					}
					++iter;
				}
				mutclientlist.unlock_upgrade();
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
			consoleLogger->critical(Poco::format("TimerThread() SQL Exception: %s", e->displayText() ));
		}
		catch (std::exception & e)
		{
			consoleLogger->fatal(Poco::format("TimerThread() Exception: %s", string(e.what())));
			TimerThreadRunning = false;
			//TODO: invoke server shutdown/restart?
			//timer thread should never exit - needs some sort of something here
			return;
		}
		catch (...)
		{
			consoleLogger->fatal("Unspecified TimerThread() Exception.");
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
		client->mutsocket.lock();
		stop(client->socket);
		client->mutsocket.unlock();
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
		if (client->m_bIsKilled)
			client->pMap->ClearDeadOwner(client->m_sX, client->m_sY);
		else
			client->pMap->ClearOwner(client->m_sX, client->m_sY);

		client->gserver->SendEventToNearClient_TypeA(client.get(), MSGID_MOTION_EVENT_REJECT, 0, 0, 0);

		Client * player = client.get();

		if (save)
		{
			if (player->m_bIsKilled)
			{
				player->m_sX = -1;
				player->m_sY = -1;

				if (player->IsNeutral())
				{
					player->m_cMapName = sideMap[NEUTRAL];
				}
				else
				{
					if (player->gserver->m_bIsCrusadeMode)
					{
						if (player->m_iDeadPenaltyTime > 0)
						{
							player->m_cLockedMapName = sideMap[player->m_side];
							player->m_iLockedMapTime = 60*5;
							player->m_iDeadPenaltyTime = 60*10;
						}
						else
							player->m_iDeadPenaltyTime = 60*10;

					}

					if (player->m_side == ARESDEN)
					{
						if ((player->m_cMapName == sideMap[ELVINE]) && !player->IsGM())
						{
							player->m_cLockedMapName = sideMapJail[ELVINE];
							player->m_iLockedMapTime = 60*3;
						}
						else if (player->m_iLevel > 80)
						{
							player->m_cMapName = "resurr1";
						}
						else
						{
							player->m_cMapName = "aresden";
						}
					}
					else// if (player->m_side == ELVINE)
					{
						if ((player->m_cMapName == sideMap[ARESDEN]) && !player->IsGM())
						{
							player->m_cLockedMapName = sideMapJail[ARESDEN];
							player->m_iLockedMapTime = 60*3;
						}
						else if (player->m_iLevel > 80)
						{
							player->m_cMapName = "resurr2";
						}
						else
						{
							player->m_cMapName = "elvine";
						}
					}
				}
			}


			if (player->m_bIsObserverMode)
			{
				player->m_sX = -1;
				player->m_sY = -1;
				player->m_cMapName = sideMap[player->m_side];
			}

			if (player->m_bIsInitComplete)
			{
				//savedata here
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
