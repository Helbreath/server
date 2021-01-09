
#pragma once

#include <memory>
#include <array>
#include "streams.hpp"
#include "utility.hpp"

namespace spdlog { class logger; }

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
    void handle_create_new_character(std::shared_ptr<client> _client, stream_read & sr);
    void handle_delete_character(std::shared_ptr<client> _client, stream_read & sr);
    void build_character_list(std::shared_ptr<client> _client, stream_write & sw);
    void fetch_character_list(std::shared_ptr<client> _client);

    std::shared_ptr<spdlog::logger> log;
    server & server_;
    server_status status_ = server_status::uninitialized;

    std::array<uint8_t, 32> enc_key = {
        0xf4, 0xb7, 0xdd, 0x28, 0xc5, 0xaf, 0xff, 0x3f,
        0xdf, 0xd5, 0xa2, 0x9d, 0x68, 0x58, 0xde, 0x91,
        0xd7, 0x4c, 0x4c, 0xf5, 0xbe, 0x74, 0xc3, 0xa7,
        0xc9, 0xf3, 0x05, 0x81, 0x43, 0xee, 0x6f, 0x71
    };
};

}
