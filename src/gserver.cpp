
#include "gserver.hpp"
#include "net/net_handler.hpp"
#include "streams.hpp"
#include "defines.hpp"
#include "client.hpp"
#include "npc.hpp"
#include "server.hpp"
#include "map.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/async.h>

namespace hbx
{

using namespace std::chrono_literals;

gserver::gserver(server * _server, const std::string & _config_file)
    : server_(*_server)
{
    std::ifstream config_file(fmt::format("config/{}", _config_file));
    config_file >> config;

    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(stdout_sink);

    auto rotating = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("log/gserver.log", 1024 * 1024 * 5, 20);
    sinks.push_back(rotating);

    log = std::make_shared<spdlog::async_logger>("hbx-gserver", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
    spdlog::register_logger(log);

    if (config.count("name") == 0)
    {
        std::cout << "No name set for gserver\n";
    }

#ifndef _NDEBUG
    log->set_pattern(fmt::format("%^%Y-%m-%d %H:%M:%S.%e [%L] [th#%5t] [{}]%$ : %v", config["name"]));
#else
    log->set_pattern(fmt::format("%^%Y-%m-%d %H:%M:%S.%e [%L] [{}]%$ : %v", config["name"]));
#endif
    log->set_level(spdlog::level::level_enum::trace);

    if (config.count("maps") == 0 || config["maps"].size() == 0)
    {
        log->error("No maps configured to be loaded");
        return;
    }

    server_name = config["name"].get<std::string>();
    int map_count = 0;
    for (auto & m : config["maps"])
    {
        auto & ret = maps.emplace_back(std::make_unique<map>(this));
        map & newmap = *ret;
        if (!newmap.bInit(m.get<std::string>()))
        {
            log->error("Map [{}] failed to load.", newmap.name);
            return;
        }
    }
    log->info("All maps loaded.");

    moveLocationX["640x480"]   = 21; moveLocationY["640x480"]   = 15;
    moveLocationX["800x600"]   = 25; moveLocationY["800x600"]   = 17;
    moveLocationX["1024x768"]  = 33; moveLocationY["1024x768"]  = 23;
    moveLocationX["1280x1024"] = 41; moveLocationY["1280x1024"] = 31;
    moveLocationX["1920x1200"] = 61; moveLocationY["1920x1200"] = 37;
    moveLocationX["1280x720"]  = 41; moveLocationY["1280x720"]  = 21;
    moveLocationX["1366x768"]  = 43; moveLocationY["1366x768"]  = 23;
    moveLocationX["1600x900"]  = 51; moveLocationY["1600x900"]  = 27;
    moveLocationX["1920x1080"] = 61; moveLocationY["1920x1080"] = 33;


    server_.set_interval([&]
    {
        std::unique_lock<std::mutex> l(cl_m);
        std::vector<std::shared_ptr<client>> templist = clients;
        for (const std::shared_ptr<client> & clnt : templist)
        {
            client * client_ = clnt.get();
            //check clients for disconnections that need deleting
            if (client_->disconnecttime != 0 && /*(*iter)->currentstatus == 2 &&*/ client_->disconnecttime + 10000 < utility::current_time())
            {
                //client persists for 10 seconds
                log->info("Client Object Deletion! <{}>", client_->socket_->address);
                {
                    if (client_->currentstatus == 2)
                    {
                        DeleteClient(clnt, true, true);//in game
                    }
                    else
                    {
                        DeleteClient(clnt, false, true);
                    }
                }
                continue;
            }

            //5 mins to sit at char select screen
            if ((client_->currentstatus == 1) && (client_->lastpackettime + 5 * 60 * 1000 < utility::current_time()))
            {
                log->info("Client Char Select Timeout! <{}>", client_->socket_->address);
                DeleteClient(clnt, false, true);
                continue;
            }

            //check connections for recent data (ghost sockets)
// 					if (client->disconnecttime == 0 && client->lastpackettime + 30000 < ltime && (client->currentstatus != 1))
// 					{
// 						//socket idle for 30 seconds (should never happen unless disconnected)
// 						poco_information(*logger, Poco::format("Client Timeout! <%s>", client->address));
// 						DeleteClient(clnt, true, true);
// 						mutclientlist.unlock_upgrade_and_lock();
// 						iter = clientlist.erase(iter);
// 						mutclientlist.unlock_and_lock_upgrade();
// 						continue;
// 					}
        }
    }, 1s);
}

gserver::~gserver()
{

}

map * gserver::get_map(std::string map_name)
{
    for (auto & m : maps)
    {
        if (m->name == map_name)
            return m.get();
    }
    return nullptr;
}

n_client gserver::get_client(uint64_t id)
{
    auto c = std::find_if(clients.begin(), clients.end(), [&](const std::shared_ptr<client> & c)
    {
        if (c->char_id == id)
            return true;
        return false;
    });

    if (c == clients.end())
        return {};
    
    return { *c };
}

void gserver::handle_message(const message_entry & msg)
{
    stream_read sr(msg.data, msg.size);
    message_id msg_id = sr.read_enum();

    map * m = get_map(msg.client_->map_name);
    try
    {
        m->handle_message(msg.client_, sr);
    }
    catch (std::exception & ex)
    {
        log->error("Error processing msg [{}] from client account id [{}]", int32_t(msg_id), msg.client_->account_id);
    }
    return;

//     switch (msg_id)
//     {
//         case message_id::REQUEST_INITPLAYER:
//             return handle_initplayer(msg.client_, sr);
//         case message_id::REQUEST_INITDATA:
//             return handle_initdata(msg.client_, sr);
//         case message_id::REQUEST_FULLOBJECTDATA:
//             return handle_fullobjectdata(msg.client_, sr);
//     }
}

void gserver::handle_new_client(std::shared_ptr<client> _client)
{
    std::unique_lock<std::mutex> l(cl_m);
    clients.push_back(_client);

    _client->socket_mode_ = gameserver;





    _client->m_dwSPTime = _client->m_dwMPTime =
        _client->m_dwHPTime = _client->m_dwAutoSaveTime =
        _client->m_dwTime = _client->m_dwHungerTime = _client->m_dwExpStockTime =
        _client->m_dwRecentAttackTime = _client->m_dwAutoExpTime = _client->lastpackettime = utility::current_time();











    stream_write sw;
    sw.write_enum(log_rsp_message_id::enter_game);
    sw.write_int16((int16_t)enter_game_msg::CONFIRM);
    _client->write(sw);
}

void gserver::handle_close_client(std::shared_ptr<client> _client)
{
    std::unique_lock<std::mutex> l(cl_m);

    auto c = std::find(clients.begin(), clients.end(), _client);

    if (c == clients.end())
        log->error("handle_close_client client is not in list");
    else
        clients.erase(c);
}

void gserver::handle_initplayer(std::shared_ptr<client> _client, stream_read & sr)
{
    std::string name = sr.read_string(10);
    uint8_t observer_mode = sr.read_int8();
    std::string gameserver_name = sr.read_string(20);

    stream_write sw;
    sw.write_enum(message_id::RESPONSE_INITPLAYER);
    sw.write_int16(MSGTYPE_CONFIRM);
    _client->write(sw);

}

void gserver::handle_initdata(std::shared_ptr<client> _client, stream_read & sr)
{
    sr.position = 6;
    std::string name = sr.read_string(10);
    uint8_t observer_mode = sr.read_int8();
    std::string gameserver_name = sr.read_string(20);

    map * m = get_map(_client->map_name);
    if (!m)
    {
        // invalid map
        stream_write sw;
        sw.write_enum(message_id::RESPONSE_INITDATA);
        sw.write_int16(MSGTYPE_CONFIRM);
        _client->write(sw);
        return;
    }

    //init coords
    if (_client->x == -1 || _client->y == -1)
    {
        _client->x = 50;
        _client->y = 50;
    }

    _client->currentstatus = 2;

    stream_write sw;
    sw.write_enum(message_id::PLAYERCHARACTERCONTENTS);
    sw.write_int16(MSGTYPE_CONFIRM);
    sw.write_int32(_client->health);
    sw.write_int32(_client->mana);
    sw.write_int32(_client->stamina);
    sw.write_int32(_client->m_iDefenseRatio);
    sw.write_int32(0); // m_iTHAC0
    sw.write_int32(_client->level);
    sw.write_int32(_client->GetStr());
    sw.write_int32(_client->GetInt());
    sw.write_int32(_client->GetVit());
    sw.write_int32(_client->GetDex());
    sw.write_int32(_client->GetMag());
    sw.write_int32(_client->GetChr());
    sw.write_int32(0); // m_iLU_Point
    sw.write_int64(_client->experience);
    sw.write_int32(_client->enemyKillCount);
    sw.write_int32(_client->playerKillCount);
    sw.write_int32(_client->m_iRewardGold);
    sw.write_string(_client->nation, 10);
    sw.write_string(_client->guild_name, 10);
    sw.write_int32(_client->m_iGuildRank);
    sw.write_int8(_client->superAttack);
    sw.write_int32(_client->arenaNumber);
    sw.write_int32(_client->total_ek);
    _client->write(sw);


    sw.clear();
    sw.write_enum(message_id::RESPONSE_INITDATA);
    sw.write_int16(MSGTYPE_CONFIRM);
    sw.write_uint64(_client->handle + 30000);
    sw.write_int16(_client->x);
    sw.write_int16(_client->y);
    sw.write_int16(/*_client->m_sType*/ 1);
    sw.write_uint16(_client->m_sAppr1);
    sw.write_uint16(_client->m_sAppr2);
    sw.write_uint16(_client->m_sAppr3);
    sw.write_uint16(_client->m_sAppr4);
    sw.write_uint32(_client->m_iApprColor);
    sw.write_uint16(_client->head_appr);
    sw.write_uint16(_client->body_appr);
    sw.write_uint16(_client->arm_appr);
    sw.write_uint16(_client->leg_appr);
    sw.write_uint32(_client->status);
    sw.write_string(_client->map_name, 10);
    sw.write_string(_client->nation, 10);
    sw.write_int8(m->m_bIsFixedDayMode ? 1 : m->day_or_night);
    sw.write_int8(m->m_cWhetherStatus);
    sw.write_int32(_client->contribution);
    sw.write_int8(0); // observer mode
    sw.write_int32(_client->m_iRating);
    sw.write_int64(_client->health);
    sw.write_int32(_client->m_iLuck);
    sw.write_int8(0); // discount - negative number for greater discount

    _client->write(sw);

    send_event_near_type_a(_client, MOTION_EVENT_CONFIRM, 0, 0, 0);
}

void gserver::handle_fullobjectdata(std::shared_ptr<client> _client, std::shared_ptr<unit> target)
{
    stream_write sw;
    sw.write_enum(message_id::RESPONSE_MOTION);
    sw.write_int16(motion_id::MOTION_STOP);

    {
        uint32_t temp;

        if (_client == nullptr) return;
        if (_client->m_bIsInitComplete == false) return;

        if (target != 0)
        {
            if (target->is_player())
            {
                stream_write sw(50);

                std::shared_ptr<client> object = std::static_pointer_cast<client>(target);

                sw.write_int32(MSGID_MOTION_STOP);
                sw.write_int16(uint16_t(object->handle));
                sw.write_int8(object->get_owner_type());
                sw.write_int16(object->x);
                sw.write_int16(object->y);
                sw.write_int16(object->get_type());
                sw.write_int8(object->direction);
                sw.write_string(object->name, 10);
                sw.write_int16(object->m_sAppr1);
                sw.write_int16(object->m_sAppr2);
                sw.write_int16(object->m_sAppr3);
                sw.write_int16(object->m_sAppr4);
                sw.write_int32(object->m_iApprColor);
                sw.write_int16(object->m_sHeadApprValue);
                sw.write_int16(object->m_sBodyApprValue);
                sw.write_int16(object->m_sArmApprValue);
                sw.write_int16(object->m_sLegApprValue);


                temp = object->status;
                if (_bGetIsPlayerHostile(_client, object) && _client->m_iAdminUserLevel == 0)
                    temp &= STATUS_ENEMYFLAGS;

                sw.write_int32(temp);
                sw.write_int8(uint8_t(object->dead_ ? true : false));

                _client->write(sw);
            }
            else
            {
                if (target->handle == 0) return;

                std::shared_ptr<npc> object = std::static_pointer_cast<npc>(target);
                if (!object)
                    return;

                stream_write sw(50);

                sw.write_int32(MSGID_MOTION_STOP);
                sw.write_int16(uint16_t(object->handle + 10000));
                sw.write_int8(object->get_owner_type());
                sw.write_int16(object->x);
                sw.write_int16(object->y);
                sw.write_int16(object->get_type());
                sw.write_int8(object->direction);
                sw.write_int16(object->appr2);
                sw.write_int32(object->status);
                sw.write_int8(uint8_t(object->dead_ ? true : false));

                _client->write(sw);
            }
        }
    }
}

void gserver::send_event_near_type_a(std::shared_ptr<unit> owner, uint32_t msg_id, int16_t sV1, int16_t sV2, int16_t sV3)
{


}


// DeleteClient only closes the socket with all intents and purposes of leaving
// their character in game unless they are not fully logged in, in which case this
// would kill the client object as well unless param is passed to also delete the object
// a proper logout would get the deleteobj flag passed
void gserver::DeleteClient(std::shared_ptr<client> client_, bool save, bool deleteobj)
{

    if (!client_) return;

    // 	upgrade_lock<shared_mutex> upgradelist(mutclientlist);
    // 	mutclientlist.unlock_and_lock_upgrade();
        //upgrade_lock<shared_mutex> upgradelist(mutclientlist);

    if (client_->socket_)
    {
        //lock_guard<mutex> lock(client->socket->mtx);
        client_->socket_->stop();
    }
    client_->currentstatus = 0;
    client_->disconnecttime = utility::current_time();

    if (deleteobj)
    {
        if (!client_->gs)
        {
            //__debugbreak();
            //client disconnects/times out before login or at char select screen
            return;
        }
        //delete the object
        //might already have ownership of gatelock
        // 

        //need to perform client removal
        //remove from map
        if (client_->is_dead())
            client_->map_->ClearDeadOwner(client_->x, client_->y);
        else
            client_->map_->ClearOwner(client_->x, client_->y);

        //let npcs know it's an invalid target
        client_->m_bActive = false;

        client_->gs->SendEventToNearClient_TypeA(client_, MSGID_MOTION_EVENT_REJECT, 0, 0, 0);


        if (save)
        {
            if (client_->is_dead())
            {
                client_->x = -1;
                client_->y = -1;

                if (client_->is_neutral())
                {
                    client_->map_name = sideMap[faction::neutral];
                }
                else
                {
                    if (client_->gs->m_bIsCrusadeMode)
                    {
                        if (client_->deadPenaltyTime > 0)
                        {
                            client_->lockedMapName = sideMap[client_->side];
                            client_->deadPenaltyTime = 60 * 10;//10 minutes
                            client_->LockMap(sideMap[client_->side], 300);
                        }
                        else
                            client_->deadPenaltyTime = 60 * 10;

                    }

                    if (client_->side == faction::aresden)
                    {
                        if ((client_->map_name == sideMap[faction::elvine]) && !client_->IsGM())
                        {
                            client_->LockMap(sideMapJail[faction::elvine], 180);
                        }
                        else if (client_->level > 80)
                        {
                            client_->map_name = "resurr1";
                        }
                        else
                        {
                            client_->map_name = "aresden";
                        }
                    }
                    else// if (player->m_side == ELVINE)
                    {
                        if ((client_->map_name == sideMap[faction::aresden]) && !client_->IsGM())
                        {
                            client_->LockMap(sideMapJail[faction::aresden], 180);
                        }
                        else if (client_->level > 80)
                        {
                            client_->map_name = "resurr2";
                        }
                        else
                        {
                            client_->map_name = "elvine";
                        }
                    }
                }
            }


            if (client_->observerMode)
            {
                client_->x = -1;
                client_->y = -1;
                client_->map_name = sideMap[client_->side];
            }

            if (client_->m_bIsInitComplete)
            {
                //TODO: savedata here
            }
        }

        //any non-save specific stuff goes here

        //Update party status and stuff (notify party of disconnect and set status)
        //player->gserver->partyMgr

        //Update friends of disconnect
        //player->gserver->friends
    }
}

}
