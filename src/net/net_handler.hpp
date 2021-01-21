#pragma once

#include "netmessages.hpp"
#include "socket.hpp"
#include <set>

namespace hbx
{

class server;

struct message_entry
{
    message_entry() = default;
    ~message_entry() { if (data) delete[] data; }
    char * data = nullptr;
    size_t size = 0;
    std::shared_ptr<client> client_;
};
class net_handler
{
public:
    net_handler(std::shared_ptr<asio::io_context> io_, server * _server);
    ~net_handler();

    void start(std::shared_ptr<socket> c);
    void stop(std::shared_ptr<socket> c);
    void stop();
    void write();
    void handle_request(const request & req);
    void handle_accept(const asio::error_code & e);



    std::shared_ptr<asio::io_context> io_context_;
    asio::ssl::context ctx;
    std::shared_ptr<socket> new_socket_;
    std::set<std::shared_ptr<socket>> connections;
    asio::ip::tcp::acceptor acceptor_;
    server * server_;
};

}
