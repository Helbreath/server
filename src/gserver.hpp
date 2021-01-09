
#pragma once

#include <memory>
#include <set>
#include <mutex>
#include <nlohmann/json.hpp>
#include "utility.hpp"
#include "streams.hpp"
#include "map.hpp"

namespace spdlog { class logger; }

namespace hbx
{

class server;
class client;
struct message_entry;

using namespace nlohmann;

class gserver
{
public:
    gserver(server * _server, const std::string & _config_file);
    ~gserver();

    server_status get_status() const noexcept { return status_; }
    map * get_map(std::string map_name);
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
    json config;
};

}
