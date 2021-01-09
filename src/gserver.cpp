
#include "gserver.hpp"
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

gserver::gserver(server * _server)
    : server_(*_server)
{
    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(stdout_sink);

    auto rotating = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("log/gserver.log", 1024 * 1024 * 5, 20);
    sinks.push_back(rotating);

    log = std::make_shared<spdlog::async_logger>("hbx-gserver", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
    spdlog::register_logger(log);

#ifndef _NDEBUG
    log->set_pattern("%^%Y-%m-%d %H:%M:%S.%e [%L] [th#%5t]%$ : %v");
#else
    log->set_pattern("%^%Y-%m-%d %H:%M:%S.%e [%L]%$ : %v");
#endif
    log->set_level(spdlog::level::level_enum::trace);
}

gserver::~gserver()
{

}

void gserver::handle_message(const message_entry & msg, std::shared_ptr<client> _client)
{
    stream_read sr(msg.data, msg.size);
    message_id msg_id = sr.read_enum();

    switch (msg_id)
    {
        case message_id::REQUEST_INITPLAYER:
            return handle_initplayer(_client, sr);
        case message_id::REQUEST_INITDATA:
            return handle_initdata(_client, sr);
    }
}

void gserver::handle_new_client(std::shared_ptr<client> _client)
{
    std::unique_lock<std::mutex> l(cl_m);
    clients.insert(_client);
}

void gserver::handle_close_client(std::shared_ptr<client> _client)
{
    std::unique_lock<std::mutex> l(cl_m);
    clients.erase(_client);
}

void gserver::handle_initplayer(std::shared_ptr<client> _client, stream_read & sr)
{
    std::string name = sr.read_string(10);
    uint8_t observer_mode = sr.read_int8();
    std::string gameserver_name = sr.read_string(20);

    stream_write sw;
    sw.write_enum(message_id::RESPONSE_INITPLAYER);
    sw.write_enum(msg_type::CONFIRM);
    _client->write(sw);

}

void gserver::handle_initdata(std::shared_ptr<client> _client, stream_read & sr)
{
    std::string name = sr.read_string(10);
    uint8_t observer_mode = sr.read_int8();
    std::string gameserver_name = sr.read_string(20);



    _client->internal_id = ++object_counter;

    stream_write sw;
    sw.write_enum(message_id::RESPONSE_INITDATA);
    sw.write_enum(msg_type::CONFIRM);
    _client->write(sw);
}

}
