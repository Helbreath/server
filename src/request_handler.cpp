
#include "request_handler.h"
#include <boost/lexical_cast.hpp>
#include "request.h"
#include "Gate.h"
#include "Server.h"
#include "GServer.h"
#include "LServer.h"
#include "Client.h"
#include "streams.h"
#include "netmessages.h"

request_handler::request_handler()
{
}

void request_handler::handle_request(const request& req)
{
	//req.object
	//object received - process
// 	asio::async_write(socket_, reply_.to_buffers(),
// 		boost::bind(&connection::handle_write, shared_from_this(),
// 		asio::placeholders::error));
	//amf3object obj = req.object;
	//rep.objects.push_back(amf3object());

	uint64_t timestamp = unixtime();


	connection & c = *req.connection;
	shared_ptr<Client> client = c.client_.lock();
	if (client == nullptr)
	{
		//client has been deleted since, ignore message
		return;
	}
	//gateserver->logger->information("data received");
	client->lastpackettime = timestamp;
	//req.data
	//TODO: decide on how to handle data - handle packets like my other projects within this function, or handle them directly within the server class?
	//also to consider - asio is threaded so this function would also have multiple threads
	//shove message into a shitty queue for the "Server thread" to handle, or let the 
	//socket threads process or multithread the queue
	//solution: multithreaded multiqueue

	if (client->disconnecttime > 0)
	{
		//client already disconnected - old message
	}
	Gate::GetSingleton().PutMsgQueue(client, Gate::GetSingleton().socketpipe, req.data, req.size);
}
