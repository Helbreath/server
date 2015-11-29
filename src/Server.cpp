#include "Server.h"
#include "Client.h"
#include "streams.h"
#include "netmessages.h"
#include "Gate.h"

Server::Server()
{
	currentplayersonline = 0;
	currentconnections = 0;

	sqlpool = 0;

	L = luaL_newstate();
	luaL_openlibs(L);
	TimerThreadRunning = false;
}

Server::~Server()
{
}

void Server::run()
{
#ifndef WIN32
	struct timespec req={0};
	req.tv_sec = 0;
	req.tv_nsec = 50000000L;//50ms
#else
	_tzset();
#endif
	try
	{
		serverstatus = SERVERSTATUS_ONLINE;
		logger->information("Creating Server threads.");

		while (serverstatus == SERVERSTATUS_ONLINE)
#ifdef WIN32
			Sleep(50);
#else
			nanosleep(&req,0);
#endif

		lua_close(L);


	}
	catch (std::exception& e)
	{
		logger->fatal(Poco::format("Server::run() Exception: %s", e.what() ));
	}
}

void Server::handle_stop()
{
	logger->information("handle_stop()");
	this->serverstatus = SERVERSTATUS_SHUTDOWN;
}

bool Server::Init()
{
	try
	{
		logger->information("Loading Config.");
		if (luaL_dofile(L, configfile.c_str()) != 0)
		{
			logger->fatal(Poco::format("%s", (string)lua_tostring(L, -1)));
			return false;
		}
		lua_getglobal(L, "config");

		char * temp;

		//ip address
		{
			lua_getfield(L, -1, "bindip");
			temp = (char*)lua_tostring(L, -1);
			if (temp == 0) { logger->fatal("Invalid bindip setting."); return false; }
			bindaddress = temp;
			logger->information(Poco::format("bindip: %s", bindaddress));
			if (bindaddress.length() == 0 || bindaddress.length() > 15)
			{
				logger->fatal("Invalid bindip setting.");
				return false;
			}
			lua_pop(L, 1);
		}

		//TODO: something to consider: make players connect to a new port for the game server, or let them reuse the login connection?
		//player connect port
		{
			uint16_t tbp = 0;
			lua_getfield(L, -1, "bindport");
			temp = (char*)lua_tostring(L, -1);
			if (temp == 0) { logger->fatal("Invalid bindport setting."); return false; }
			tbp = atoi(temp);
			logger->information(Poco::format("bindport: %?d", tbp));
			if (tbp < 1 || tbp > 65534)
			{
				logger->fatal("Invalid bindport setting.");
				return false;
			}
			bindport = temp;
			lua_pop(L, 1);
		}

		//sql host
		{
			lua_getfield(L, -1, "sqlhost");
			temp = (char*)lua_tostring(L, -1);
			if (temp == 0) { logger->fatal("Invalid sqlhost setting."); return false; }
			sqlhost = temp;
			logger->information(Poco::format("sqlhost: %s", sqlhost));
			lua_pop(L, 1);
		}

		//sql user
		{
			lua_getfield(L, -1, "sqluser");
			temp = (char*)lua_tostring(L, -1);
			if (temp == 0) { logger->fatal("Invalid sqluser setting."); return false; }
			sqluser = temp;
			logger->information(Poco::format("sqluser: %s", sqluser));
			lua_pop(L, 1);
		}

		//sql pass
		{
			lua_getfield(L, -1, "sqlpass");
			temp = (char*)lua_tostring(L, -1);
			if (temp == 0) { logger->fatal("Invalid sqlpass setting."); return false; }
			logger->information("sqlpass set");
			sqlpass = temp;
			lua_pop(L, 1);
		}

		lua_pop(L, 1);

		return true;
	}
	catch (std::exception& e)
	{
		logger->fatal(Poco::format("Init() Exception: %s", (string)e.what()));
		system("pause");
		return false;
	}
	catch(...)
	{
		logger->fatal("Unspecified Init() Exception.");
		system("pause");
		return false;
	}
}

bool Server::ConnectSQL()
{
	try
	{
		sqlpool = new SessionPool("MySQL", "host="+sqlhost+";port=3306;db=server_db;user="+sqluser+";password="+sqlpass+";compress=true;auto-reconnect=true");
	}
	catch (Poco::Exception& exc)
	{
		logger->fatal(Poco::format("ConnectSQL() Exception: %s", exc.displayText()));
		return false;
	}
	return true;
}

//Base DeleteClient
void Server::DeleteClient(shared_ptr<Client> client, bool save, bool deleteobj)
{

	if (!client) return;

}

void Server::PutMsgQueue(shared_ptr<Client> client, MsgQueue & q, char * data, uint32_t size)
{
	lock_guard<mutex> lock(Gate::GetSingleton().mutpacketlist);
	//poco_information(*logger, "PutMsgQueue()");
	shared_ptr<MsgQueueEntry> msg(new MsgQueueEntry);

	msg->data = data;
	msg->size = size;
	msg->client = client;

	q.push_back(msg);
}

void Server::PutMsgQueue(shared_ptr<Server::MsgQueueEntry> msg, MsgQueue & q)
{
	q.push_back(msg);
}


shared_ptr<Server::MsgQueueEntry> Server::GetMsgQueue(MsgQueue & q)
{
    shared_ptr<Server::MsgQueueEntry> msg = q.front();
    q.pop_front();
	return msg;
}

