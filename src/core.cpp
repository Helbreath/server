//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "core.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/async.h>
#include <spdlog/fmt/fmt.h>
#include "Client.h"
#include "connection_state_hb.h"
#include "netmessages.h"
#include "Map.h"

core::core()
{
    load_config();

    spdlog::init_thread_pool(8192, 2);

    std::vector<spdlog::sink_ptr> sinks;

    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt >();
    sinks.push_back(stdout_sink);

    // daily rotation max 30 days
    auto rotating = std::make_shared<spdlog::sinks::daily_file_sink_mt>("log/core.log", 0, 0, false, 30);
    sinks.push_back(rotating);

    log = std::make_shared<spdlog::async_logger>("core", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
    spdlog::register_logger(log);

    log->set_pattern("%^%Y-%m-%d %H:%M:%S.%e [%L] [th#%t]%$ : %v");
    log->set_level(spdlog::level::level_enum::info);
}

core::~core()
{
    if (fw)
        fw->stop();
}

void core::load_config()
{
    bool has_log = false;
    if (log.get() != nullptr)
        has_log = true;

    try
    {
        std::ifstream config_file("config.json");


        if (!config_file.is_open())
        {
            std::perror("File opening failed");
            throw std::runtime_error("config.json does not exist");
        }

        json cfg;
        config_file >> cfg;

        if (!cfg["log-level"].is_null())
        {
            if (cfg["log-level"].is_number_integer())
            {
                auto l = static_cast<spdlog::level::level_enum>(cfg["log-level"].get<int32_t>());
                if (cfg["log-level"].get<int32_t>() > 6 || cfg["log-level"].get<int32_t>() < 0)
                {
                    if (has_log) log->error("Unknown \"log-level\" in config.json. Default: info");
                    else std::cout << "Unknown \"log-level\" in config.json. Default: info\n";
                    l = spdlog::level::level_enum::info;
                }
                loglevel = l;
            }
            else if (cfg["log-level"].is_string())
            {
                std::string s = cfg["log-level"].get<std::string>();
                if (s == "trace")
                    loglevel = spdlog::level::level_enum::trace;
                else if (s == "debug")
                    loglevel = spdlog::level::level_enum::debug;
                else if (s == "info")
                    loglevel = spdlog::level::level_enum::info;
                else if (s == "warn")
                    loglevel = spdlog::level::level_enum::warn;
                else if (s == "err")
                    loglevel = spdlog::level::level_enum::err;
                else if (s == "critical")
                    loglevel = spdlog::level::level_enum::critical;
                else if (s == "off")
                    loglevel = spdlog::level::level_enum::off;
                else
                {
                    if (has_log) log->error("Unknown \"log-level\" in config.json. Default: info");
                    else std::cout << "Unknown \"log-level\" in config.json. Default: info\n";
                    loglevel = spdlog::level::level_enum::info;
                }
            }
            else
            {
                if (has_log) log->error("Unknown \"log-level\" in config.json. Default: info");
                else std::cout << "Unknown \"log-level\" in config.json. Default: info\n";
                loglevel = spdlog::level::level_enum::info;
            }
        }
        else
            loglevel = spdlog::level::level_enum::info;

        if (!cfg["log-format"].is_null())
            log_formatting = cfg["log-format"].get<std::string>();
        else
            log_formatting = "%^%Y-%m-%d %H:%M:%S.%e [%L] [th#%t]%$ : %v";

        if (!cfg["filewatcher"].is_null())
        {
            bool fwcheck = cfg["filewatcher"].get<bool>();
            if (fwcheck)
            {
                if (!fw)
                {
                    fw = std::make_unique<filewatcher>(std::vector<std::string>{ "config.json" }, std::chrono::milliseconds(2000));
                }
                fw->start([&](std::string path_to_watch, file_status status) -> void
                {
                    if (!std::filesystem::is_regular_file(std::filesystem::path(path_to_watch)) && status != file_status::deleted)
                        return;

                    switch (status)
                    {
                        case file_status::created:
                            file_created(path_to_watch);
                            break;
                        case file_status::modified:
                            file_modified(path_to_watch);
                            break;
                        case file_status::deleted:
                            file_deleted(path_to_watch);
                            break;
                        default:
                            log->error("Unknown file status for {}", path_to_watch);
                    }
                });
                if (has_log) log->info("File watcher started");
                else std::cout << "File watcher started\n";
            }
            else
            {
                if (fw)
                {
                    fw->stop();
                }
            }
        }

        if (!cfg["world-name"].is_null())
            world_name = cfg["world-name"].get<std::string>();
        else
            throw std::runtime_error("No world-name set");

        if (!cfg["maps"].is_null())
            maps_loaded = cfg["maps"].get<std::vector<std::string>>();
        else
            throw std::runtime_error("No maps set");

        if (!cfg["sqluser"].is_null())
            sqluser = cfg["sqluser"].get<std::string>();
        else
            throw std::runtime_error("No sqluser set");

        if (!cfg["sqlpass"].is_null())
            sqlpass = cfg["sqlpass"].get<std::string>();
        else
            throw std::runtime_error("No sqlpass set");

        if (!cfg["sqldb"].is_null())
            sqldb = cfg["sqldb"].get<std::string>();
        else
            throw std::runtime_error("No sqldb set");

        if (!cfg["sqlhost"].is_null())
            sqlhost = cfg["sqlhost"].get<std::string>();
        else
            throw std::runtime_error("No sqlhost set");

        if (!cfg["sqlport"].is_null())
            sqlport = cfg["sqlport"].get<uint16_t>();
        else
            throw std::runtime_error("No sqlport set");

        if (!cfg["bind-ip"].is_null())
            bindip = cfg["bind-ip"].get<std::string>();
        else
            throw std::runtime_error("No bind-ip set");

        if (!cfg["port"].is_null())
            bindport = cfg["port"].get<uint16_t>();
        else
            throw std::runtime_error("No port set");

    }
    catch (std::exception & e)
    {
        if (has_log) log->info("Error loading config.json : {}", e.what());
        else std::cout << "Error loading config.json : " << e.what() << '\n';

        // allows reloading of configurations to not break state since there should already be a valid value from a prior
        // successful load
        if (get_server_state() == server_status::uninitialized)
            state_valid = false;
    }
}

void core::run()
{
    if (!state_valid)
    {
        std::cout << "Server state invalid. Errors occurred on creation. Exiting core::run()\n";
        return;
    }

    try
    {
        pq_login = std::make_unique<pqxx::connection>(fmt::format("postgresql://{}:{}@{}:{}/{}", sqluser, sqlpass, sqlhost, sqlport, sqldb));
        pq_game = std::make_unique<pqxx::connection>(fmt::format("postgresql://{}:{}@{}:{}/{}", sqluser, sqlpass, sqlhost, sqlport, sqldb));
    }
    catch (pqxx::broken_connection & ex)
    {
        throw std::runtime_error(fmt::format("Unable to connect to SQL - {}", ex.what()));
    }

    log->info("Connected to SQL");

    // load map data
    for (auto & mapname : maps_loaded)
    {
        // load map data into maps
        maps.emplace(std::make_unique<Map>(this));
    }

    set_server_state(server_status::running);

    // todo - make a way to change these externally - maybe by admin client?
    set_login_server_state(login_server_status::running);
    set_game_server_state(game_server_status::running);

    server = std::make_unique<ix::WebSocketServer>(bindport, bindip);

    auto res = server->listen();
    if (!res.first)
        throw std::runtime_error(res.second);

    log->info(fmt::format("Listening on {}:{}", bindip, bindport));

    server->setConnectionStateFactory([&]()
    {
        return std::make_shared<connection_state_hb>();
    });

    server->setOnClientMessageCallback(
        [&](std::shared_ptr<ix::ConnectionState> connection_state, ix::WebSocket & websocket, const ix::WebSocketMessagePtr & message)
        {
            on_message(connection_state, websocket, message);
        }
    );

    server->start();
    log->info("Server started");
    //std::thread th{ [&]() { server->wait(); } };

    using time_point = std::chrono::time_point<std::chrono::steady_clock>;
    using milliseconds = std::chrono::duration<int, std::ratio<1, 1000>>;
    using seconds = std::chrono::duration<int, std::ratio<1, 1>>;
    using minutes = std::chrono::duration<int, std::ratio<60, 1>>;
    using hours = std::chrono::duration<int, std::ratio<3600, 1>>;
    using days = std::chrono::duration<int, std::ratio<24 * 3600, 1>>;

    time_point current_time = std::chrono::steady_clock::now();
    time_point player_timeout_timer = std::chrono::steady_clock::now();
    time_point player_count_report_timer = std::chrono::steady_clock::now();

    while (get_server_state() == server_status::running)
    {
        // todo - better handle timers
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1ms);
        current_time = std::chrono::steady_clock::now();

        if (std::chrono::duration_cast<seconds>(current_time - player_count_report_timer).count() > 5)
        {
            log->info(fmt::format("Online - players: {} - sockets: {}", client_list.size(), websocket_clients.size()));
            player_count_report_timer = std::chrono::steady_clock::now();
        }

        // check the client list every 10ms for timed out users - if this becomes a performance issue (unlikely) then change to a regular for
        // so that it can do a full set iteration at once rather than break out of it when a single change is made. unfortunately ranged-for has
        // iteration limitations
        if (std::chrono::duration_cast<milliseconds>(current_time - player_timeout_timer).count() > 10)
        {
            std::lock_guard<std::recursive_mutex> l(websocket_list);
            // why can't anyone make a good ws library
            auto wsclients = server->getClients();
            for (auto & wspair : websocket_clients)
            {
                auto & ws = wspair.first;
                auto & connstate = wspair.second;
                connection_state_hb * connection_state = dynamic_cast<connection_state_hb *>(connstate.get());
                auto & player = connection_state->client;

                if (!player) continue;

                if (std::chrono::duration_cast<seconds>(current_time - connection_state->lastpackettime).count() > 20)
                {
                    log->info(fmt::format("Disconnecting player packet timed out [{}] account [{}]", player->name, player->account));
                    DeleteClient(player, true, true);
                    break;
                }
                if (player->disconnected && std::chrono::duration_cast<seconds>(current_time - player->disconnecttime).count() > 10)
                {
                    log->info(fmt::format("Disconnecting player disconnected timed out [{}] account [{}]", player->name, player->account));
                    DeleteClient(player, true, true);
                    break;
                }
            }
            for (auto & player : client_list)
            {
                auto connection_state = player->get_connection_state();
                if (connection_state && std::chrono::duration_cast<seconds>(current_time - connection_state->lastpackettime).count() > 20)
                {
                    log->info(fmt::format("Disconnecting player packet timed out [{}] account [{}]", player->name, player->account));
                    DeleteClient(player, true, true);
                    break;
                }
                if (player->disconnected && std::chrono::duration_cast<seconds>(current_time - player->disconnecttime).count() > 10)
                {
                    log->info(fmt::format("Disconnecting player disconnected timed out [{}] account [{}]", player->name, player->account));
                    DeleteClient(player, true, true);
                    break;
                }
            }
            player_timeout_timer = std::chrono::steady_clock::now();
        }

    }
}

void core::on_message(std::shared_ptr<ix::ConnectionState> ixconnstate, ix::WebSocket & websocket, const ix::WebSocketMessagePtr & message)
{
    connection_state_hb * connection_state = dynamic_cast<connection_state_hb*>(ixconnstate.get());
    connection_state->lastpackettime = std::chrono::steady_clock::now();

    if (message->type == ix::WebSocketMessageType::Open)
    {
        log->info("Connection opened");
        // initial client object setup on fresh connect

        // track websocket list locally to match websocket to state iteratively
        std::lock_guard<std::recursive_mutex> l(websocket_list);
        for (auto & ws : server->getClients())
            if (ws.get() == &websocket)
            {
                websocket_clients.insert(std::make_pair(ws, ixconnstate));
                connection_state->websocket = ws;
                log->info("Pair added to internal list");
                break;
            }
    }
    else if (message->type == ix::WebSocketMessageType::Close)
    {
        log->info("Connection closed");
        std::lock_guard<std::recursive_mutex> l(websocket_list);
        for (auto & wspair : websocket_clients)
            if (wspair.first.get() == &websocket)
            {
                websocket_clients.erase(wspair);
                log->info("Pair cleared from internal list");
                break;
            }
    }
    else if (message->type == ix::WebSocketMessageType::Error)
    {
        log->error("Error on websocket");
        std::lock_guard<std::recursive_mutex> l(websocket_list);
        for (auto & wspair : websocket_clients)
            if (wspair.first.get() == &websocket)
            {
                websocket_clients.erase(wspair);
                log->info("Pair cleared from internal list");
                break;
            }
    }
    else if (message->type == ix::WebSocketMessageType::Message)
    {
        log->info(fmt::format("Message received - {} bytes", message->str.length()));
        if (message->binary)
        {
            // handle legacy binary packets

            try
            {

                StreamRead sr = StreamRead(message->str.c_str(), static_cast<uint32_t>(message->str.length()));
                StreamWrite sw = StreamWrite();

                std::shared_ptr<Client> client = connection_state->client;

                int32_t msgid = sr.ReadInt();

                log->info(fmt::format("Event [{}]", msgid));

                if (msgid == MSGID_COMMAND_CHECKCONNECTION)
                {
                    // todo - handle
                    return;
                }

                auto check_login = [&](const std::shared_ptr<Client> & client) -> bool
                {
                    if (!client->logged_in)
                    {
                        sw.WriteInt(MSGID_RESPONSE_LOG);
                        sw.WriteShort(LOGRESMSGTYPE_REJECT);
                        sw.WriteString("Not logged in");
                        client->SWrite(sw);
                        log->info("Player trying to send messages prior to login <{}> for account <{}>", connection_state->getRemoteIp(), client->account);
                        if (client) DeleteClient(client);
                        return false;
                    }
                    return true;
                };

                auto check_login_status = [&]() -> bool
                {
                    if (get_login_server_state() != login_server_status::running && get_login_server_state() != login_server_status::running_queue)
                    {
                        sw.WriteInt(MSGID_RESPONSE_LOG);
                        sw.WriteShort(LOGRESMSGTYPE_REJECT);
                        sw.WriteString("Login server not online");
                        auto data = ix::IXWebSocketSendData{ sw.data, sw.position };
                        websocket.sendBinary(data);
                        return false;
                    }
                    return true;
                };

                auto check_game_status = [&]() -> bool
                {
                    if (get_game_server_state() != game_server_status::running)
                    {
                        sw.WriteInt(MSGID_RESPONSE_LOG);
                        sw.WriteShort(LOGRESMSGTYPE_REJECT);
                        sw.WriteString("Login server not online");
                        auto data = ix::IXWebSocketSendData{ sw.data, sw.position };
                        websocket.sendBinary(data);
                        return false;
                    }
                    return true;
                };

                int64_t account_id;

                switch (msgid)
                {
                    // game related events


                    // login related events
                    case MSGID_REQUEST_LOGIN:
                        {
                            if (!check_login_status()) return;

                            std::string account = sr.ReadString(60);
                            std::string password = sr.ReadString(60);
                            std::string worldname = sr.ReadString(30);
                            if (world_name != worldname)
                            {
                                // invalid world name
                                sw.WriteInt(MSGID_RESPONSE_LOG);
                                sw.WriteShort(LOGRESMSGTYPE_SERVICENOTAVAILABLE);
                                auto data = ix::IXWebSocketSendData{ sw.data, sw.position };
                                websocket.sendBinary(data);
                                return;
                            }

                            if (!CheckLogin(client.get(), account, password, account_id))
                            {
                                // login failed
                                log->info("Failed login from <{}> for account <{}>", connection_state->getRemoteIp(), account);
                                // todo - add login spam protection

                                sw.WriteInt(MSGID_RESPONSE_LOG);
                                sw.WriteShort(LOGRESMSGTYPE_PASSWORDMISMATCH);
                                auto data = ix::IXWebSocketSendData{ sw.data, sw.position };
                                websocket.sendBinary(data);
                                return;
                            }

                            connection_state->client = std::make_shared<Client>();

                            client = connection_state->client;
                            client->connection_state = ixconnstate;
                            client->account_id = account_id;
                            client->account = account;
                            client_list.insert(connection_state->client);
                            client->logged_in = true;

                            sw.WriteInt(MSGID_RESPONSE_LOG);
                            sw.WriteShort(MSGTYPE_CONFIRM);
                            sw.WriteShort(UPPER_VERSION);
                            sw.WriteShort(LOWER_VERSION);
                            sw.WriteShort(PATCH_VERSION);
                            sw.WriteByte(0x01);
                            sw.WriteShort(0);//dates \/
                            sw.WriteShort(0);
                            sw.WriteShort(0);
                            sw.WriteShort(0);
                            sw.WriteShort(0);
                            sw.WriteShort(0);//dates /\

                            SendCharList(client.get(), sw);
                            sw.WriteInt(500);
                            sw.WriteInt(500);
                            sw.WriteString("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 32);
                            client->SWrite(sw);
                        }
                        break;
                    case MSGID_REQUEST_CREATENEWCHARACTER:
                        if (!check_login_status() || !check_login(client)) return;
                        CreateCharacter(client.get(), sr);
                        break;
                    case MSGID_REQUEST_DELETECHARACTER:
                        if (!check_login_status() || !check_login(client)) return;
                        DeleteCharacter(client.get(), sr);
                        break;
                    case MSGID_REQUEST_ENTERGAME:
                        if (!check_login_status() || !check_login(client)) return;
                        EnterGame(client, sr);
                        break;
                    default:
                        log->error("Unknown packet received from client - {:X}", msgid);
                        break;
                }
            }
            catch (int32_t reason)
            {
                if (reason == -192)
                {
                    log->error("(data == 0)");
                }
                else if (reason == -193)
                {
                    log->error("LServer: (position+a > size)");
                }
            }
        }
        else
        {
            // handle new json objects
            json obj = json::parse(message->str);
        }
    }
}

void core::file_created(std::string path)
{
    // generally unused
}

void core::file_modified(std::string path)
{
    if (path == "config.json")
    {
        load_config();
    }
}

void core::file_deleted(std::string path)
{
    // generally unused
}

Map * core::GetMap(std::string name)
{
    for (auto & map : maps)
    {
        if (map->name == name)
            return map.get();
    }
    return nullptr;
}

