

#include "GServer.h"
#include "Client.h"
#include <memory>

class GServer;

class ChatHandler
{
public:
	ChatHandler(GServer * server);
	~ChatHandler();

	void ParseChat(const std::shared_ptr<Client> & client, std::string message);

	GServer * server;
};