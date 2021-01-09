
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

#include <pqxx/pqxx>

namespace hbx
{

using namespace pqxx;

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
    //log_message_id msg_id = sr.read_enum<log_message_id>();
    log_message_id msg_id = sr.read_enum();

    switch (msg_id)
    {
        case log_message_id::login:
            return handle_login(_client, sr);
        case log_message_id::create_new_character:
            return handle_create_new_character(_client, sr);
        case log_message_id::delete_character:
            return handle_delete_character(_client, sr);
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
        server_.close_client(_client);
    }

    request_params rp;
    rp.method = Get;
    rp.host = "helbreathx.net";
    rp.body = json({ { "email", username }, { "pass", password }, { "key", "06c2a11ba3375c31ad674df2ce512eda6fcb6cd9c7e6cec429486bb81493f0eff2b691d7e97fa1878f7fc425d5d46f3f09c4456d53c035fa953b388505853c8f" } }).dump();
    rp.path = "/serverlogin.php";

    std::string str = server_.execute(std::forward<request_params>(rp));

    if (str.empty() || str.length() > 10)
    {
        sw.write_int32(0);
        sw.write_enum(log_res_msg::PASSWORDMISMATCH);
        _client->write(sw);
        return;
    }

    // login successful

    client & c = *_client;

    c.account_name = username;
    c.account_password = password;
    try
    {
        c.forum_id = std::stoull(str);
    }
    catch (...)
    {
        sw.write_int32(0);
        sw.write_enum(log_res_msg::SERVICENOTAVAILABLE);
        _client->write(sw);
        return;
    }

    std::map<std::string, std::string> res;

    {
        connection & pg = *server_.pg;
        work W{ *server_.pg };
        result R{ W.exec_params("SELECT * FROM accounts WHERE forum_member_id = $1", str.c_str()) };


        for (auto row : R)
            for (auto column : row)
                res[column.name()] = column.c_str();
                //std::cout << column.name() << " - " << column.c_str() << '\n';
    }

    _client->account_id = std::stoull(res["id"]);
    _client->xcoins = std::stoull(res["xcoins"]);

    fetch_character_list(_client);

    sw.write_enum(log_rsp_message_id::log);
    sw.write_enum(log_res_msg::CONFIRM);
    sw.write_uint16(server_.upper_version);
    sw.write_uint16(server_.lower_version);
    sw.write_uint16(server_.patch_version);
    sw.write_int8(0);
    sw.write_int16(0); // account year
    sw.write_int16(0); // account month
    sw.write_int16(0); // account day
    sw.write_int16(0); // ip year
    sw.write_int16(0); // ip month
    sw.write_int16(0); // ip day
    build_character_list(_client, sw);
    sw.write_int32(0); // timeleftsecaccount
    sw.write_int32(0); // timeleftsecip
    sw.write_bytes(enc_key.data(), sizeof enc_key);
    _client->write(sw);
}

void lserver::handle_create_new_character(std::shared_ptr<client> _client, stream_read & sr)
{
    stream_write sw;
    sw.write_enum(log_rsp_message_id::log);
    sw.write_enum(log_res_msg::NEWCHARACTERCREATED);

    _client->write(sw);
}

void lserver::handle_delete_character(std::shared_ptr<client> _client, stream_read & sr)
{
    stream_write sw;
    sw.write_enum(log_rsp_message_id::log);
    sw.write_enum(log_res_msg::CHARACTERDELETED);

    _client->write(sw);
}

void lserver::build_character_list(std::shared_ptr<client> _client, stream_write & sw)
{
    std::unique_lock<std::mutex> l(_client->char_list_m);
    sw.write_int8(static_cast<uint8_t>(_client->characters.size()));

    for (auto & c : _client->characters)
    {
        sw.write_string(c["name"], 10);
        sw.write_int8(1);
        sw.write_uint64(std::stoull(c["id"]));
        sw.write_uint16(std::stoi(c["appr1"]));
        sw.write_uint16(std::stoi(c["appr2"]));
        sw.write_uint16(std::stoi(c["appr3"]));
        sw.write_uint16(std::stoi(c["appr4"]));
        sw.write_uint16(std::stoi(c["head_appr"]));
        sw.write_uint16(std::stoi(c["body_appr"]));
        sw.write_uint16(std::stoi(c["arm_appr"]));
        sw.write_uint16(std::stoi(c["leg_appr"]));
        sw.write_uint16(std::stoi(c["gender"]));
        sw.write_uint16(std::stoi(c["skin"]));
        sw.write_uint16(std::stoi(c["level"]));
        sw.write_uint64(std::stoull(c["experience"]));
        sw.write_uint16(std::stoi(c["strength"]));
        sw.write_uint16(std::stoi(c["vitality"]));
        sw.write_uint16(std::stoi(c["dexterity"]));
        sw.write_uint16(std::stoi(c["intelligence"]));
        sw.write_uint16(std::stoi(c["magic"]));
        sw.write_uint16(std::stoi(c["charisma"]));
        sw.write_uint32(std::stoi(c["apprcolor"]));
        sw.write_uint16(0);
        sw.write_uint16(0);
        sw.write_uint16(0);
        sw.write_uint16(0);
        sw.write_uint16(0);
        sw.write_string(c["maploc"], 10);
    }
}

void lserver::fetch_character_list(std::shared_ptr<client> _client)
{
    connection & pg = *server_.pg;
    work W{ *server_.pg };
    result R{ W.exec_params("select * from characters where account_id = $1", _client->account_id) };

    std::unique_lock<std::mutex> l(_client->char_list_m);
 
    _client->characters.clear();
    for (const_result_iterator::reference row : R)
    {
        std::map<std::string, std::string> character;
        for (const_row_iterator::reference column : row)
            character[column.name()] = column.c_str();
        _client->characters.push_back(character);
    }
}

}
