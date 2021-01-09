
#pragma once

#include <memory>
#include <set>
#include "utility.hpp"
#include "streams.hpp"

namespace spdlog { class logger; }

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

    server_status get_status() const noexcept { return status_; }
    void handle_message(const message_entry & msg, std::shared_ptr<client> _client);
    void handle_new_client(std::shared_ptr<client> _client);
    void handle_close_client(std::shared_ptr<client> _client);
    void handle_initplayer(std::shared_ptr<client> _client, stream_read & sr);
    void handle_initdata(std::shared_ptr<client> _client, stream_read & sr);

    uint64_t object_counter = 0;

    std::shared_ptr<spdlog::logger> log;
    server & server_;
    server_status status_ = server_status::uninitialized;
    std::string server_name;
    uint64_t id = 0;
    std::mutex cl_m;
    std::set<std::shared_ptr<client>> clients;
    std::set<std::unique_ptr<map>> maps;

};

}
