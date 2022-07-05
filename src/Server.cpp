#include "Server.h"
#include "Client.h"
#include "streams.h"
#include "netmessages.h"

Server::Server(std::shared_ptr<spdlog::logger> log)
	: log(log)
{
	currentplayersonline = 0;
	currentconnections = 0;
	timer_thread_running = false;
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
		log->info("Creating Server threads.");

		while (serverstatus == SERVERSTATUS_ONLINE)
#ifdef WIN32
			Sleep(50);
#else
			nanosleep(&req,0);
#endif
	}
	catch (std::exception& e)
	{
		log->critical(fmt::format("Server::run() Exception: {}", e.what() ));
	}
}

void Server::handle_stop()
{
	log->info("handle_stop()");
	this->serverstatus = SERVERSTATUS_SHUTDOWN;
}

//Base DeleteClient
void Server::DeleteClient(std::shared_ptr<Client> client, bool save, bool deleteobj)
{

// 	if (!client) return;

}

void Server::PutMsgQueue(std::shared_ptr<Client> client, MsgQueue & q, char * data, uint32_t size)
{
// 	std::lock_guard<std::mutex> lock(Gate::GetSingleton().mutpacketlist);
// 	//poco_information(*log, "PutMsgQueue()");
// 	std::shared_ptr<MsgQueueEntry> msg(new MsgQueueEntry);
// 
// 	msg->data = new char[size + 1];
// 	memcpy(msg->data, data, size);
// 	msg->size = size;
// 	msg->client = client;
// 
// 	q.push_back(msg);
}

void Server::PutMsgQueue(std::shared_ptr<Server::MsgQueueEntry> msg, MsgQueue & q)
{
// 	q.push_back(msg);
}


std::shared_ptr<Server::MsgQueueEntry> Server::GetMsgQueue(MsgQueue & q)
{
// 	std::shared_ptr<Server::MsgQueueEntry> msg = q.front();
//     q.pop_front();
// 	return msg;
	return nullptr;
}

