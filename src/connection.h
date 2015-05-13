#ifndef HTTP_CONNECTION_HPP
#define HTTP_CONNECTION_HPP

#define MAXPACKETSIZE 2048

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio/write.hpp>
#include "streams.h"
#include "request.h"
#include "request_handler.h"

class Gate;
class GServer;
class LServer;
class Server;
class Client;

/// Represents a single connection from a client.
class connection
	: public boost::enable_shared_from_this<connection>,
	private boost::noncopyable
{
public:
	/// Construct a connection with the given io_service.
	explicit connection(boost::asio::io_service& io_service,
		Gate & gserver, request_handler& handler);

	/// Get the socket associated with the connection.
	boost::asio::ip::tcp::socket& socket();

	/// Start the first asynchronous operation for the connection.
	void start();

	/// Stop all asynchronous operations associated with the connection.
	void stop();

	void write(const char * data, const uint64_t size);
	void write(StreamWrite & sw);

private:
	/// Handle completion of a read operation.
	void handle_read_header(const boost::system::error_code& e,
		std::size_t bytes_transferred);
	void handle_read(const boost::system::error_code& e,
		std::size_t bytes_transferred);
	/// Handle completion of a write operation.
	void handle_write(const boost::system::error_code& e);

	/// Socket for the connection.
	boost::asio::ip::tcp::socket socket_;

	/// The manager for this connection.
	Gate & server;

	/// The handler used to process the incoming request.
	request_handler& request_handler_;

	/// Buffer for incoming data.
	boost::array<char, MAXPACKETSIZE> buffer_;

	/// The incoming request.
	request request_;

	int32_t size;

public:
	uint64_t uid;

	std::weak_ptr<Client> client_;

	std::string address;
};

typedef boost::shared_ptr<connection> connection_ptr;

#endif // HTTP_CONNECTION_HPP
