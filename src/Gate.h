#pragma once


#include "Server.h"

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

using namespace Poco::Data;

class Gate : public Server
{
public:
	Gate(void);
	~Gate(void);

	//temporary solution
	void PutLogFileList(char * str);
	void PutLogList(char * str);

	// Initialize Server
	bool Init();

	void TimerThread();
	void ChatThread();
	void SocketThread();

	void DeleteClient(shared_ptr<Client> client, bool save = true, bool deleteobj = false);

	LServer * loginserver;
	std::list<GServer*> gameserver;

	//READ access instances use shared_lock()
	//WRITE access instances use lock()
	// Gate has the only mutex to prevent lockups
	shared_mutex mutclientlist;

	uint8_t thread_pool_size_;


	/// Run the server's io_service loop.
	void run();

	/// Stop the server.
	void stop();

	/// Start the first asynchronous operation for the connection.
	void start(connection_ptr c);

	/// Stop the specified connection.
	void stop(connection_ptr c);

	/// Stop all connections.
	void stop_all();

	/// Handle completion of an asynchronous accept operation.
	void handle_accept(const boost::system::error_code& e);

	/// Handle a request to stop the server.
	void handle_stop();

	/// The io_service used to perform asynchronous operations.
	boost::asio::io_service io_service_;

	/// The signal_set is used to register for process termination notifications.
	boost::asio::signal_set signals_;

	/// Acceptor used to listen for incoming connections.
	boost::asio::ip::tcp::acceptor acceptor_;

	/// The connection list which owns all live connections.
	std::set<connection_ptr> connections_;

	/// The next connection to be accepted.
	connection_ptr new_connection_;

	/// The handler for all incoming requests.
	request_handler request_handler_;


	std::thread *socketthread;

	bool InitSockets();

	bool AccountInUse(std::shared_ptr<Client> client);
};

