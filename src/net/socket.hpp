#pragma once

#include "../utility.hpp"
#include "../streams.hpp"

#include <asio/ssl.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/io_context.hpp>
#include <array>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <memory>

#define MAX_PACKET_SIZE 4096

namespace hbx
{

class server;
class client;
class socket;
class request_handler;
class net_handler;

struct request
{
    std::size_t size;
    std::string cmd;
    std::string uri;
    char * data;
    socket * socket_;
};

/// Represents a single connection from a client.
class socket : public std::enable_shared_from_this<socket>
{
public:
    /// Construct a connection with the given io_service.
    explicit socket(asio::io_context & io_service, net_handler & handler, asio::ssl::context & context);

    /// Get the socket associated with the connection.
    asio::ssl::stream<asio::ip::tcp::socket>::lowest_layer_type & get_socket()
    {
        return socket_.lowest_layer();
    }
    //boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& socket();

    /// Start the first asynchronous operation for the connection.
    void start();

    /// Stop all asynchronous operations associated with the connection.
    void stop();

    void write(const char * data, const uint64_t size);
    void write(stream_write & sw);

    /// Handle completion of a read operation.
    void handle_read_header(const asio::error_code & e, std::size_t bytes_transferred);
    void handle_read(const asio::error_code & e, std::size_t bytes_transferred);
    /// Handle completion of a write operation.
    void handle_write(const asio::error_code & e);

    asio::ssl::stream<asio::ip::tcp::socket> socket_;

    /// The manager for this connection.
    net_handler & nh;

    /// Buffer for incoming data.
    std::array<char, 8192> buffer_; //2048

    /// The incoming request.
    request request_;

    std::size_t size;
    std::string address;
    client * client_ = nullptr;

    bool handshake_complete;
};

using socket_ptr = std::shared_ptr<socket>;

}
