
#include "lserver.hpp"
#include "net/net_handler.hpp"
#include "streams.hpp"
#include "defines.hpp"
#include "client.hpp"
#include "server.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/async.h>

namespace hbx
{

lserver::lserver(server * _server)
    : server_(*_server)
{
    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(stdout_sink);

    auto rotating = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("log/lserver.log", 1024 * 1024 * 5, 20);
    sinks.push_back(rotating);

    log = std::make_shared<spdlog::async_logger>("hbx-lserver", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
    spdlog::register_logger(log);

#ifndef _NDEBUG
    log->set_pattern("%^%Y-%m-%d %H:%M:%S.%e [%L] [th#%5t]%$ : %v");
#else
    log->set_pattern("%^%Y-%m-%d %H:%M:%S.%e [%L]%$ : %v");
#endif
    log->set_level(spdlog::level::level_enum::trace);
}

lserver::~lserver()
{

}

void lserver::handle_message(const message_entry & msg, std::shared_ptr<client> _client)
{
    stream_read sr(msg.data, msg.size);
    message_id msg_id = sr.read_message_id();

    switch (msg_id)
    {
        case message_id::REQUEST_LOGIN:
            return handle_login(_client, sr);
    }
}

void lserver::handle_login(std::shared_ptr<client> _client, stream_read & sr)
{
    stream_write sw;

    if (get_status() != server_status::online)
    {
        sw.write_int32(0);
        sw.write_enum(log_res_msg::SERVICENOTAVAILABLE);
        _client->write(sw);
        return;
    }

    auto wat = sr.read_string(12);
    auto username = sr.read_string(60);
    auto password = sr.read_string(60);
    auto world_server = sr.read_string(30);

    _client->screenwidth_v = sr.read_int32();
    _client->screenheight_v = sr.read_int32();

    if (username.length() == 0 || password.length() == 0)
    {
        log->warn("Invalid login - {}", username);
        
    }
}

}
