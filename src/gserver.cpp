
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

bool gserver::has_map(std::string map_name)
{
    for (auto & m : maps)
    {
        if (m->name == map_name)
            return true;
    }
    return false;
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

    stream_write sw;
    sw.write_enum(log_rsp_message_id::enter_game);
    sw.write_enum(enter_game_msg::CONFIRM);
    _client->write(sw);
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

    map * m = nullptr;

    stream_write sw;
    sw.write_enum(message_id::RESPONSE_INITDATA);
    sw.write_enum(msg_type::CONFIRM);
    sw.write_uint64(_client->internal_id);
    sw.write_int16(_client->m_sX);
    sw.write_int16(_client->m_sY);
    sw.write_int16(/*_client->m_sType*/ 1);
    sw.write_uint16(_client->m_sAppr1);
    sw.write_uint16(_client->m_sAppr2);
    sw.write_uint16(_client->m_sAppr3);
    sw.write_uint16(_client->m_sAppr4);
    sw.write_uint16(_client->m_iApprColor);
    sw.write_uint16(_client->head_appr);
    sw.write_uint16(_client->body_appr);
    sw.write_uint16(_client->arm_appr);
    sw.write_uint16(_client->leg_appr);
    sw.write_uint32(_client->m_sStatus);
    sw.write_string(_client->map_name, 10);
    sw.write_string(_client->nation, 10);
    sw.write_int8(m->m_bIsFixedDayMode ? 1 : m->day_or_night);
    sw.write_int8(m->m_cWhetherStatus);
    sw.write_int32(_client->m_iContribution);
    sw.write_int8(0); // observer mode
    sw.write_int32(_client->m_iRating);
    sw.write_int64(_client->m_iHP);
    sw.write_int32(_client->m_iLuck);
    sw.write_int8(0); // discount

    _client->write(sw);
}

}
