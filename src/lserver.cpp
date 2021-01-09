
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
    log_message_id msg_id = sr.read_enum();

    switch (msg_id)
    {
        case log_message_id::login:
            return handle_login(_client, sr);
        case log_message_id::create_new_character:
            return handle_create_new_character(_client, sr);
        case log_message_id::delete_character:
            return handle_delete_character(_client, sr);
        case log_message_id::enter_game:
            return handle_enter_game(_client, sr);
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
    _client->screenwidth = sr.read_int32();
    _client->screenheight = sr.read_int32();

    if (username.length() == 0 || password.length() == 0)
    {
        log->warn("Invalid login - {}", username);
        _client->disconnect();
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

    send_login_success(_client);
}

void lserver::handle_enter_game(std::shared_ptr<client> _client, stream_read & sr)
{
    //server_.transfer_client();
    stream_write sw;
    sw.write_enum(log_rsp_message_id::log);
    sw.write_enum(enter_game_msg::CONFIRM);
    _client->write(sw);
}

void lserver::handle_create_new_character(std::shared_ptr<client> _client, stream_read & sr)
{
    stream_write sw;
  
    std::string wat = sr.read_string(12);
    std::string username = sr.read_string(60);
    std::string password = sr.read_string(60);
    std::string player_name = sr.read_string(10);
    std::string world_name = sr.read_string(30);

    uint32_t gender = sr.read_int8();
    uint32_t skin_color = sr.read_int8();
    uint32_t hair_style = sr.read_int8();
    uint32_t hair_color = sr.read_int8();
    uint32_t underwear_color = sr.read_int8();
    uint32_t strength = sr.read_int8();
    uint32_t vitality = sr.read_int8();
    uint32_t dexterity = sr.read_int8();
    uint32_t intelligence = sr.read_int8();
    uint32_t magic = sr.read_int8();
    uint32_t charisma = sr.read_int8();

    if (
        strength + vitality + dexterity + intelligence + magic + charisma != 70 ||
        strength < 10 || strength > 14 ||
        vitality < 10 || vitality > 14 ||
        dexterity < 10 || dexterity > 14 ||
        intelligence < 10 || intelligence > 14 ||
        magic < 10 || magic > 14 ||
        player_name.length() == 0 || player_name.length() > 10 ||
        hair_style > 15 || hair_style < 0 ||
        hair_color > 15 || hair_color < 0 ||
        underwear_color > 15 || underwear_color < 0 ||
        skin_color > 3 || skin_color < 0
        ) {
        sw.write_enum(log_rsp_message_id::log);
        sw.write_enum(log_res_msg::NEWCHARACTERFAILED);
        return _client->write(sw);
    }

    uint32_t appr1;
    appr1 = (hair_style << 8) | (hair_color << 4) | underwear_color;
    uint32_t HP = vitality * 8 + strength * 2 + intelligence * 2 + 8;
    uint32_t MP = magic * 3 + intelligence * 2 + 2;
    uint32_t SP = strength + 17;


    // check if too many characters
    {
        work W{ *server_.pg };
        result R{ W.exec_params("select count(*) from characters where account_id = $1", _client->account_id) };

        auto field = R.at(0)[0];

        if (field.get<int>().value_or(0) > 10)
        {
            sw.write_enum(log_rsp_message_id::log);
            sw.write_enum(log_res_msg::NEWCHARACTERFAILED);
            return _client->write(sw);
        }
    }

    // check if name exists already
    {
        work W{ *server_.pg };
        result R{ W.exec_params("select count(*) from characters where name = $1", player_name) };

        auto field = R.at(0)[0];

        if (field.get<int>().value_or(0) > 0)
        {
            sw.write_enum(log_rsp_message_id::log);
            sw.write_enum(log_res_msg::ALREADYEXISTINGCHARACTER);
            return _client->write(sw);
        }
    }

    {
        work W{ *server_.pg };
        result R{ W.exec_params("insert into characters(account_id, name, strength, vitality, dexterity, intelligence, magic, charisma, appr1, gender, skin, hairstyle, haircolor, underwear, hp, mp, sp) \
            values($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17) returning id;",
            _client->account_id, player_name, strength, vitality, dexterity, intelligence, magic, charisma, appr1, gender, skin_color, hair_style, hair_color, underwear_color, HP, MP, SP) };

        if (R.empty() || R.at(0).empty())
        {
            sw.write_enum(log_rsp_message_id::log);
            sw.write_enum(log_res_msg::NEWCHARACTERFAILED);
            return _client->write(sw);
        }

        auto field = R.at(0)[0];

        uint64_t char_id = field.get<int>().value_or(0);
        if (char_id > 0)
        {
            for (int i = 0; i < 24; ++i)
            {
                switch (i)
                {
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 14:
                    case 19:
                    case 21:
                        W.exec_params("insert into skills (char_id, skill_id, mastery, experience) values ($1, $2, 20, 0)", char_id, i);
                        break;
                    default:
                        break;
                }
            }
        }
        W.commit();
    }
    fetch_character_list(_client);
    sw.write_enum(log_rsp_message_id::log);
    sw.write_enum(log_res_msg::NEWCHARACTERCREATED);
    sw.write_string(player_name, 10);
    build_character_list(_client, sw);
    _client->write(sw);
}

void lserver::handle_delete_character(std::shared_ptr<client> _client, stream_read & sr)
{
    std::string wat = sr.read_string(12);
    std::string username = sr.read_string(60);
    std::string password = sr.read_string(60);
    std::string player_name = sr.read_string(10);
    std::string world_name = sr.read_string(30);

    uint64_t account_id = _client->account_id;

    {
        work W{ *server_.pg };
        result R{ W.exec_params("select count(*) from characters where account_id = $1 and name = $2", account_id, player_name) };

        if (R.empty() || R.at(0).empty())
        {
            stream_write sw;
            sw.write_enum(log_rsp_message_id::log);
            sw.write_enum(log_res_msg::REJECT);
            sw.write_string("Unable to delete character. Please contact support.");
            sw.write_int32(0); // block year
            sw.write_int32(0); // block month
            sw.write_int32(0); // block day

            _client->write(sw);
            return;
        }

        auto field = R.at(0)[0];

        if (field.get<int>().value_or(0) > 0)
        {
            result R2{ W.exec_params("delete from characters where account_id = $1 and name = $2", account_id, player_name) };
            W.commit();
            for (auto c = _client->characters.begin(); c != _client->characters.end(); c++)
            {
                if ((*c)["name"] == player_name)
                {
                    _client->characters.erase(c);
                    send_login_success(_client);
                    return;
                }
            }
        }
    }


    stream_write sw;
    sw.write_enum(log_rsp_message_id::log);
    sw.write_enum(log_res_msg::REJECT);
    sw.write_string("Test string to write");
    sw.write_int32(0); // block year
    sw.write_int32(0); // block month
    sw.write_int32(0); // block day

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
        sw.write_uint16(static_cast<uint16_t>(std::stoul(c["appr1"])));
        sw.write_uint16(static_cast<uint16_t>(std::stoul(c["appr2"])));
        sw.write_uint16(static_cast<uint16_t>(std::stoul(c["appr3"])));
        sw.write_uint16(static_cast<uint16_t>(std::stoul(c["appr4"])));
        sw.write_uint16(static_cast<uint16_t>(std::stoul(c["head_appr"])));
        sw.write_uint16(static_cast<uint16_t>(std::stoul(c["body_appr"])));
        sw.write_uint16(static_cast<uint16_t>(std::stoul(c["arm_appr"])));
        sw.write_uint16(static_cast<uint16_t>(std::stoul(c["leg_appr"])));
        sw.write_uint16(static_cast<uint16_t>(std::stoul(c["gender"])));
        sw.write_uint16(static_cast<uint16_t>(std::stoul(c["skin"])));
        sw.write_uint16(static_cast<uint16_t>(std::stoul(c["level"])));
        sw.write_uint64(std::stoull(c["experience"]));
        sw.write_uint16(static_cast<uint16_t>(std::stoul(c["strength"])));
        sw.write_uint16(static_cast<uint16_t>(std::stoul(c["vitality"])));
        sw.write_uint16(static_cast<uint16_t>(std::stoul(c["dexterity"])));
        sw.write_uint16(static_cast<uint16_t>(std::stoul(c["intelligence"])));
        sw.write_uint16(static_cast<uint16_t>(std::stoul(c["magic"])));
        sw.write_uint16(static_cast<uint16_t>(std::stoul(c["charisma"])));
        sw.write_uint32(static_cast<uint32_t>(std::stoul(c["apprcolor"])));
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

void lserver::send_login_success(std::shared_ptr<client> _client)
{
    stream_write sw;
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

}
