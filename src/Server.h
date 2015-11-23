#pragma once


#include "funcs.h"
#include "connection.h"
#include "common.h"
#include "XLogger.h"
#include <set>
#include <unordered_map>
#include <mutex>

#include <lua5.2/lua.hpp>
#include <lua5.2/lauxlib.h>

#ifndef WIN32
#include <mysql/mysql.h>
#endif

#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/shared_lock_guard.hpp>
#include <boost/algorithm/string.hpp>


#include "TeleportLoc.h"

#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/MySQL/Connector.h"
#include "Poco/Data/MySQL/MySQLException.h"
#include "Poco/Data/SessionPool.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/Exception.h"

#include "Poco/Logger.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/Message.h"

using Poco::PatternFormatter;
using Poco::FormattingChannel;
using Poco::ConsoleChannel;
using Poco::FileChannel;
using Poco::Message;
using Poco::Data::RecordSet;
using namespace Poco::Data::Keywords;

using boost::unique_lock;
using boost::shared_lock;
using boost::shared_mutex;
using boost::lock;
using boost::defer_lock;
using boost::mutex;
using boost::upgrade_to_unique_lock;
using boost::upgrade_lock;

class Map;
class BuildItem;
class Item;
class Tile;
class Magic;
class Npc;
class Client;
class Unit;
//class DropManager;
class DynamicObject;
class DelayEvent;
class Msg;
class Skill;
class Quest;
class Crafting;
class Teleport;
class Fish;
class Potion;
class Mineral;
class Ini;

using namespace std;
using namespace Poco::Data;

class Server
{
public:
	Server();
	~Server(void);

	string configfile;

	//temporary solution
	void PutLogFileList(char * str);
	void PutLogList(char * str);

	virtual void run();
	virtual void handle_stop();

	// MySQL
	string sqlhost, sqluser, sqlpass, bindaddress, bindport;
	string sqldb;

	uint32_t serverstatus;

	// MySQL connection pools
	SessionPool * sqlpool;

	// Lua Handle
	lua_State *L;

	// Initialize Server
	virtual bool Init();

	// Max players allowed connected
	uint32_t maxplayers;

	// Current players connected
	uint32_t currentplayersonline;
	uint32_t currentconnections;

	// Whether the timer thread is running or not
	bool TimerThreadRunning;

	std::thread *timerthread;

	bool ConnectSQL();

	virtual void DeleteClient(std::shared_ptr<Client> client, bool save = true, bool deleteobj = false);


	//TODO: dynamic List or static array?
	//list is easier to manage while array is less cpu intensive to access/control
	//array gave static numbers to reference clients by (iClientH, etc)
	//list would need another way to reference clients. incremental connection count? random number? - would also need some recoding client side
	//solution: leave it as list server side, replace clientshortcut in old hg with newer version that will simulate old clientid handles and such
	//old clientshortcut is rendered obsolete with list iterating
	// better solution: iClientH will refer to rowid within sqldb (of some sort) ensuring no duplicates, giving the client something to differentiate clients with
	// and being similar to the old system - clientshortcut removed
	// 
	// or. use map<>
	// another way to further decrease access time would be to keep a clientlist in the map class for actions that affect only clients local to the same map
	std::list<std::shared_ptr<Client>> clientlist;

	struct MsgQueueEntry
	{
		std::shared_ptr<Client> client;
		char * data;
		uint32_t size;
	};
	typedef std::list<std::shared_ptr<MsgQueueEntry>> MsgQueue;
	MsgQueue socketpipe;
	std::mutex mutsocket;
	void PutMsgQueue(std::shared_ptr<Client> client, MsgQueue & q, char * data, uint32_t size);
	void PutMsgQueue(std::shared_ptr<MsgQueueEntry>, MsgQueue & q);
	std::shared_ptr<MsgQueueEntry> GetMsgQueue(MsgQueue & q);

	XLogger * logger;
};

