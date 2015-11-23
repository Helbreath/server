

#include "GServer.h"

class GServer;

class ChatHandler
{
public:
	ChatHandler(GServer * server);
	~ChatHandler();

	void ChatHandler::ParseChat(Client * client, string message);

	GServer * server;
};