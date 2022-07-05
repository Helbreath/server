#pragma once


#include "funcs.h"
#include "common.h"
#include <thread>
#include <mutex>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#ifndef WIN32
#include <mysql/mysql.h>
#endif

#include "TeleportLoc.h"

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

class Server
{
public:
	Server(std::shared_ptr<spdlog::logger> log);
	~Server(void);

	std::string configfile;

	virtual void run();
	virtual void handle_stop();

	// MySQL
	std::string sqlhost, sqluser, sqlpass, bindaddress, bindport;
	std::string sqldb;

	uint32_t serverstatus;

	// Initialize Server
	virtual bool init() = 0;

	// Max players allowed connected
	uint32_t maxplayers;

	// Current players connected
	uint32_t currentplayersonline;
	uint32_t currentconnections;

	// Whether the timer thread is running or not
	bool timer_thread_running;

	std::thread timerthread;

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
	using MsgQueue = std::list<std::shared_ptr<MsgQueueEntry>>;
	MsgQueue socketpipe;
	std::mutex mutsocket;
	void PutMsgQueue(std::shared_ptr<Client> client, MsgQueue & q, char * data, uint32_t size);
	void PutMsgQueue(std::shared_ptr<MsgQueueEntry>, MsgQueue & q);
	std::shared_ptr<MsgQueueEntry> GetMsgQueue(MsgQueue & q);

    std::shared_ptr<spdlog::logger> log;
};

