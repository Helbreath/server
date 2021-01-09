
#pragma once

#include <memory>

namespace hbx
{

class server;
class client;
struct message_entry;
class stream_read;

class lserver
{
public:
    lserver(server * _server);
    ~lserver();

    server_status get_status() const noexcept { return status_; }
    void handle_message(const message_entry & msg, std::shared_ptr<client> _client);
    void handle_login(std::shared_ptr<client> _client, stream_read & sr);

    std::shared_ptr<spdlog::logger> log;
    server & server_;
    server_status status_ = server_status::uninitialized;

};

}
