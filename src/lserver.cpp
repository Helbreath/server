
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

void lserver::handle_message(const message_entry & msg)
{
    stream_read sr(msg.data, msg.size);
    log_message_id msg_id = sr.read_enum();

    switch (msg_id)
    {
        case log_message_id::login:
            return handle_login(msg.client_, sr);
        case log_message_id::create_new_character:
            return handle_create_new_character(msg.client_, sr);
        case log_message_id::delete_character:
            return handle_delete_character(msg.client_, sr);
        case log_message_id::enter_game:
            return handle_enter_game(msg.client_, sr);
    }
}

void lserver::handle_login(std::shared_ptr<client> _client, stream_read & sr)
{
    stream_write sw;

    if (get_status() != server_status::online)
    {
        sw.write_int32(0);
        sw.write_int16((int16_t)log_res_msg::SERVICENOTAVAILABLE);
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

#if defined(HELBREATHX)
    request_params rp;
    rp.method = Get;
    rp.host = "helbreathx.net";
    rp.body = nlohmann::json({ { "email", username }, { "pass", password }, { "key", server_.config["web-login-key"].get<std::string>() }}).dump();
    rp.path = "/serverlogin.php";

    std::string str = server_.execute(std::forward<request_params>(rp));
#else
    // todo - do sql login (and hash password)
    {
        work W{ *server_.pg };
        result R{ W.exec_params("SELECT * FROM accounts WHERE username = $1 AND password = $2", username, password) };

        for (auto row : R)
            for (auto column : row)
                res[column.name()] = column.c_str();
    }
#endif

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
        sw.write_int16((int16_t)log_res_msg::SERVICENOTAVAILABLE);
        _client->write(sw);
        return;
    }

    std::map<std::string, std::string> res;

    {
        work W{ *server_.pg };
#if defined(HELBREATHX)
        result R{ W.exec_params("SELECT * FROM accounts WHERE forum_member_id = $1", str.c_str()) };
#else
        // todo
        result R{ W.exec_params("SELECT * FROM accounts WHERE forum_member_id = $1", str.c_str()) };
#endif

        for (auto row : R)
            for (auto column : row)
                res[column.name()] = column.c_str();
    }

    _client->account_id = std::stoull(res["id"]);
    _client->xcoins = std::stoull(res["xcoins"]);

    fetch_character_list(_client);

    send_login_success(_client);
}

void lserver::handle_enter_game(std::shared_ptr<client> _client, stream_read & sr)
{
    std::string wat = sr.read_string(12);
    std::string username = sr.read_string(60);
    std::string password = sr.read_string(60);
    std::string player_name = sr.read_string(10);
    uint16_t enter_game_type = sr.read_uint16();
    std::string world_name = sr.read_string(30);

    // find and apply character info
    for (auto & c : _client->characters)
    {
        if (c["name"] == player_name)
        {
            // found character

            gserver * gs = server_.find_gserver(world_name, c["maploc"]);
            if (gs == nullptr)
            {
                stream_write sw;
                sw.write_enum(log_rsp_message_id::log);
                sw.write_int16((int16_t)enter_game_msg::REJECT);
                sw.write_string("Game server or map not online. Please contact support.");
                sw.write_int32(0); // block year
                sw.write_int32(0); // block month
                sw.write_int32(0); // block day
                _client->write(sw);
                return;
            }

            _client->handle = std::stoull(c["id"]);
            _client->char_id = std::stoull(c["id"]);
            _client->account_id = std::stoull(c["account_id"]);
            _client->name = c["id"];
            //_client->id1 = std::stoull(c["id1"]);
            //_client->id2 = std::stoull(c["id2"]);
            //_client->id3 = std::stoull(c["id3"]);
            _client->level = std::stoi(c["level"]);
            _client->SetStr(std::stoi(c["strength"]));
            _client->SetVit(std::stoi(c["vitality"]));
            _client->SetDex(std::stoi(c["dexterity"]));
            _client->SetInt(std::stoi(c["intelligence"]));
            _client->SetMag(std::stoi(c["magic"]));
            _client->SetChr(std::stoi(c["charisma"]));
            _client->experience = std::stoll(c["experience"]);


            _client->gender = std::stoi(c["gender"]);
            _client->colorSkin = std::stoi(c["skin"]);
            _client->m_cHairStyle = std::stoi(c["hairstyle"]);
            _client->colorHair = std::stoi(c["haircolor"]);
            _client->colorUnderwear = std::stoi(c["underwear"]);
            _client->m_iApprColor = std::stoi(c["apprcolor"]);
            _client->m_sAppr1 = std::stoi(c["appr1"]);
            _client->m_sAppr2 = std::stoi(c["appr2"]);
            _client->m_sAppr3 = std::stoi(c["appr3"]);
            _client->m_sAppr4 = std::stoi(c["appr4"]);
            _client->head_appr = std::stoi(c["head_appr"]);
            _client->body_appr = std::stoi(c["body_appr"]);
            _client->arm_appr = std::stoi(c["arm_appr"]);
            _client->leg_appr = std::stoi(c["leg_appr"]);

            _client->nation = c["nation"];
            _client->map_name = c["maploc"];

            _client->x = std::stoi(c["locx"]);
            _client->y = std::stoi(c["locy"]);

            _client->profile = c["profile"];

            _client->admin_level = std::stoi(c["adminlevel"]);
            _client->contribution = std::stoi(c["contribution"]);
            _client->m_iSpecialAbilityTime = std::stoll(c["leftspectime"]);
            _client->locked_map_name = c["lockmapname"];
            _client->m_iLockedMapTime = std::stoi(c["lockmaptime"]);
            //_client->admin_level = std::stoll(c["blockdate"]); // check this and reject?
            //_client->m_iGuildGUID = std::stoll(c["guild_id"]);
            _client->arenaNumber = std::stoi(c["fightnum"]);
            _client->arenaReserveTime = std::stoi(c["fightdate"]);
            _client->arenaTicketNumber = std::stoi(c["fightticket"]);
            _client->m_iQuest = std::stoi(c["questnum"]);
            _client->m_iQuestID = std::stoi(c["questid"]);
            _client->m_iCurQuestCount = std::stoi(c["questcount"]);
            _client->m_iQuestRewardType = std::stoi(c["questrewardtype"]);
            _client->m_iQuestRewardAmount = std::stoi(c["questrewardamount"]);
            _client->m_bIsQuestCompleted = c["questcompleted"] == "t";
            _client->m_iSpecialEventID = std::stoi(c["eventid"]);
            _client->crusadeContribution = std::stoi(c["warcon"]);
            _client->m_iCrusadeDuty = std::stoi(c["crusadejob"]);
            _client->crusade_construction_point = std::stoll(c["crusadeconstructpoint"]);
            _client->m_dwCrusadeGUID = std::stoll(c["crusadeid"]);
            _client->reputation = std::stoll(c["reputation"]);
            _client->health = std::stoll(c["hp"]);
            _client->mana = std::stoll(c["mp"]);
            _client->stamina = std::stoll(c["sp"]);
            _client->enemyKillCount = std::stoll(c["ek"]);
            _client->playerKillCount = std::stoll(c["pk"]);
            _client->m_iRewardGold = std::stoll(c["rewardgold"]);
            _client->m_iDownSkillIndex = std::stoll(c["downskillid"]);
            _client->m_iHungerStatus = std::stoll(c["hunger"]);
            _client->superAttack = std::stoll(c["leftsac"]);
            _client->m_iTimeLeft_ShutUp = std::stoll(c["leftshutuptime"]);
            _client->m_iTimeLeft_Rating = std::stoll(c["leftreptime"]);
            _client->m_iTimeLeft_ForceRecall = std::stoll(c["leftforcerecalltime"]);
            _client->m_iTimeLeft_FirmStamina = std::stoll(c["leftfirmstaminatime"]);
            _client->m_iDeadPenaltyTime = std::stoll(c["leftdeadpenaltytime"]);
            _client->admin_level = std::stoll(c["adminlevel"]);
            _client->m_iPartyID = std::stoll(c["party_id"]);
            _client->m_iGizonItemUpgradeLeft = std::stoll(c["itemupgradeleft"]);
            _client->total_ek = std::stoll(c["totalek"]);
            _client->total_pk = std::stoll(c["totalpk"]);
            _client->mmr = std::stoll(c["mmr"]);
            _client->altmmr = std::stoll(c["altmmr"]);
            _client->m_iLuck = std::stoll(c["luck"]);
            _client->gold = std::stoll(c["gold"]);
            // _client->xcoins = std::stoll(c["xcoins"]); // comes from account data instead

            std::string temp = c["magicmastery"];
            for (int i = 0; i < temp.length(); ++i)
            {
                _client->magicMastery[i] = (temp[i] == '1') ? 1 : 0;
            }

            if (_client->gender == MALE) _client->set_type(1);
            else if (_client->gender == FEMALE) _client->set_type(4);
            _client->set_type(_client->get_type() + _client->colorSkin - 1);
            _client->appr1 = (uint64_t(_client->m_cHairStyle) << 8) | (uint64_t(_client->colorHair) << 4) | uint64_t(_client->colorUnderwear);

            //todo: load items here

            _client->m_bIsInitComplete = true;

            server_.transfer_client(_client, world_name, _client->map_name);
            return;
        }
    }

    stream_write sw;
    sw.write_enum(log_rsp_message_id::log);
    sw.write_int16((int16_t)enter_game_msg::REJECT);
    sw.write_string("Unable to enter game. Please contact support.");
    sw.write_int32(0); // block year
    sw.write_int32(0); // block month
    sw.write_int32(0); // block day
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
        sw.write_int16((int16_t)log_res_msg::NEWCHARACTERFAILED);
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
            sw.write_int16((int16_t)log_res_msg::NEWCHARACTERFAILED);
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
            sw.write_int16((int16_t)log_res_msg::ALREADYEXISTINGCHARACTER);
            return _client->write(sw);
        }
    }

    {
        work W{ *server_.pg };
        result R{ W.exec_params("insert into characters(account_id, name, strength, vitality, dexterity, intelligence, magic, charisma, appr1, gender, skin, hairstyle, haircolor, underwear, hp, mp, sp) \
            values($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17) returning id;",
            _client->account_id, player_name, strength, vitality, dexterity, intelligence, magic, charisma, appr1, gender, skin_color, hair_style, hair_color, underwear_color, HP, MP, SP) };

        if (R.empty())
        {
            sw.write_enum(log_rsp_message_id::log);
            sw.write_int16((int16_t)log_res_msg::NEWCHARACTERFAILED);
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
    sw.write_int16((int16_t)log_res_msg::NEWCHARACTERCREATED);
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

        if (R.empty())
        {
            stream_write sw;
            sw.write_enum(log_rsp_message_id::log);
            sw.write_int16(MSGTYPE_REJECT);
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
    sw.write_int16(MSGTYPE_REJECT);
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
    sw.write_int16(MSGTYPE_CONFIRM);
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
