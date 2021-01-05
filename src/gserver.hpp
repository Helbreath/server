
#pragma once

namespace hbx
{

class server;
class client;
struct message_entry;

class gserver
{
public:
    gserver(server * _server);
    ~gserver();

    void handle_message(const message_entry & msg, std::shared_ptr<client> _client);

    std::shared_ptr<spdlog::logger> log;
    server & server_;

};

}
