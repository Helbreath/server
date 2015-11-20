
#include "connection.h"
#include <vector>
#include <boost/bind.hpp>
#include "request_handler.h"
#include "Client.h"
#include "funcs.h"

#include "Poco/Logger.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/Message.h"

using Poco::Logger;
using Poco::PatternFormatter;
using Poco::FormattingChannel;
using Poco::ConsoleChannel;
using Poco::FileChannel;
using Poco::Message;

#include "Server.h"
#include "Gate.h"
#include "GServer.h"
#include "LServer.h"

extern uint64_t unixtime();

connection::connection(boost::asio::io_service& io_service,
					   Gate& gserver, request_handler& handler)
					   : socket_(io_service),
					   server(gserver),
					   request_handler_(handler),
					   uid(0)
{
	size = 0;
}

boost::asio::ip::tcp::socket& connection::socket()
{
	return socket_;
}

void connection::start()
{
	uid = rand()*rand()*rand();

	boost::asio::async_read(socket_, boost::asio::buffer(buffer_, 3), boost::bind(&connection::handle_read_header, shared_from_this(),
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));
}

void connection::stop()
{
	shared_ptr<Client> client = client_.lock();
	//TODO: find out where client is emptied improperly - temp "fix" (likely involving DeleteClient via invalid DB access)
	if (client == nullptr)
		return;
	client->disconnecttime = unixtime();
	socket_.close();
	client->socket.reset();
	if (client->handle > 0)
	{
		server.currentplayersonline--;
		server.logger->information(Poco::format("Client disconnected: %s", client->name));
	}
	//TODO: record last online time
	server.currentconnections--;
}

void connection::write(const char * data, const uint64_t size)
{
    char ckey[1] = {0};
	char csize[2];
	try {
	    uint16_t tsize = uint16_t(size) + 3;
		*(int16_t*)csize = tsize;
		boost::asio::write(socket_, boost::asio::buffer(ckey, 1));
		boost::asio::write(socket_, boost::asio::buffer(csize, 2));
		boost::asio::write(socket_, boost::asio::buffer(data, size));
	}
	catch (std::exception& e)
	{
		server.consoleLogger->error(Poco::format("asio::write_some() exception: %s", (string)e.what()));
	}
}

void connection::write(StreamWrite & sw)
{
	char ckey[1] = {0};
	char csize[2];
	try {
		uint16_t tsize = uint16_t(sw.size) + 3;
		*(int16_t*)csize = tsize;
		boost::asio::write(socket_, boost::asio::buffer(ckey, 1));
		boost::asio::write(socket_, boost::asio::buffer(csize, 2));
		boost::asio::write(socket_, boost::asio::buffer(sw.data, sw.size));
	}
	catch (std::exception& e)
	{
		server.consoleLogger->error(Poco::format("asio::write_some() exception: %s", (string)e.what()));
	}
}

void connection::handle_read_header(const boost::system::error_code& e,
									std::size_t bytes_transferred)
{
	if (!e)
	{
		if (bytes_transferred == 3)
		{
			//TODO: helbreath's ghetto xor key needs to go. first bytes = size or gtfo
			size = *(int16_t*)(((char*)buffer_.data())+1);
			if (size > 2048)//temporary set size .. shouldn't really be more than this anyway
			{
				server.consoleLogger->error(Poco::format("Invalid packet size : %?d", size));
				server.stop(shared_from_this());
				return;
			}
			boost::asio::async_read(socket_, boost::asio::buffer(buffer_, size-3), boost::bind(&connection::handle_read, shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
		}
	}
	else if (e != boost::asio::error::operation_aborted)
	{
		server.stop(shared_from_this());
		return;
	}

}

void connection::handle_read(const boost::system::error_code& e,
							 std::size_t bytes_transferred)
{
	if (!e)
	{
		if (bytes_transferred != size-3)
		{
			server.consoleLogger->error(Poco::format("Did not receive proper amount of bytes : rcv: %?d needed: %?d", bytes_transferred, size));
			server.stop(shared_from_this());
			return;
		}
		//printf("uid("XI64")\n", uid);
		// read object size
		if ((size > MAXPACKETSIZE) || (size <= 0))
		{
			//ERROR - object too large - close connection
			server.consoleLogger->error(Poco::format("Invalid packet size : %?d", size));
			server.stop(shared_from_this());
			return;
		}

		// parse packet
		request_.size = size;
		request_.data = buffer_.data();
		request_.connection = this;
		try {
			request_handler_.handle_request(request_);
		}
		catch (std::exception& e)
		{
			server.consoleLogger->error(Logger::format("handle_request() exception: ", e.what()));
		}

		boost::asio::async_read(socket_, boost::asio::buffer(buffer_, 3), boost::bind(&connection::handle_read_header, shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}
	else if (e != boost::asio::error::operation_aborted)
	{
		server.stop(shared_from_this());
		return;
	}
}

void connection::handle_write(const boost::system::error_code& e)
{
	if (!e)
	{
		// 		// Initiate graceful connection closure.
		// 		asio::error_code ignored_ec;
		// 		socket_.shutdown(asio::ip::tcp::socket::shutdown_both, ignored_ec);
	}

	if (e != boost::asio::error::operation_aborted)
	{
		server.stop(shared_from_this());
	}
}
