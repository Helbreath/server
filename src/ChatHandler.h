

#include "GServer.h"

#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/shared_lock_guard.hpp>
#include <boost/algorithm/string.hpp>

class GServer;

class ChatHandler
{
public:
	ChatHandler(GServer * server);
	~ChatHandler();

	void ChatHandler::ParseChat(const shared_ptr<Client> & client, string message);

	GServer * server;
};