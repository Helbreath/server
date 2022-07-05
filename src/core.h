//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include <vector>
#include <memory>
#include <string>
#include <spdlog/spdlog.h>
#include "GServer.h"
#include "LServer.h"
#include <nlohmann/json.hpp>
#include "utility.h"
#include "filewatcher.h"
#include <ixwebsocket/IXWebSocketServer.h>
#include <pqxx/pqxx>
#include <pqxx/except>
#include <shared_mutex>

using json = nlohmann::json;

class core
{
public:
    core();
    ~core();

    std::vector<spdlog::sink_ptr> sinks;
    std::shared_ptr<spdlog::logger> log;

    std::unique_ptr<ix::WebSocketServer> server;

    spdlog::level::level_enum loglevel = spdlog::level::level_enum::info;
    std::string log_formatting;
    bool state_valid = true;

    std::unique_ptr<filewatcher> fw;

    std::vector<std::string> maps_loaded;

    std::set<std::unique_ptr<Map>> maps;

    std::string world_name;

    std::string sqluser;
    std::string sqlpass;
    std::string sqldb;
    std::string sqlhost;
    uint16_t sqlport;
    std::string bindip;
    uint16_t bindport;

    // a single class will handle all connections and functionality - you log into the same object you also play the game with
    // the reason they were separated in 1997 is because servers back then didn't have the processing power per thread to handle more than
    // a few functions at once and every Helbreath binary was 100% single-threaded
    // this new design will have a thread per map, and a dedicated thread for top level processes, sockets, and log-server tasks
    // multiple actual game servers though will be spread across multiple binaries to keep things simple and because
    // chances are different "servers" will also have different clients
    // end goal is the server sends clients full configurations to change how they calculate and do things letting one universal client
    // function between many significantly different servers - UI changes would still need client changes though
    std::set<std::shared_ptr<Client>> client_list;
    std::shared_mutex client_list_mtx;

    std::set<std::pair<std::shared_ptr<ix::WebSocket>, std::shared_ptr<ix::ConnectionState>>> websocket_clients;
    std::recursive_mutex websocket_list;

    void load_config();
    void run();

    void on_message(std::shared_ptr<ix::ConnectionState> connection_state, ix::WebSocket & websocket, const ix::WebSocketMessagePtr & message);

    server_status get_server_state() const noexcept { return server_status_; }
    void set_server_state(server_status s) noexcept { server_status_ = s; }

    game_server_status get_game_server_state() const noexcept { return game_status_; }
    void set_game_server_state(game_server_status s) noexcept { game_status_ = s; }

    login_server_status get_login_server_state() const noexcept { return login_status_; }
    void set_login_server_state(login_server_status s) noexcept { login_status_ = s; }

    void file_created(std::string path);
    void file_modified(std::string path);
    void file_deleted(std::string path);

private:

    std::unique_ptr<pqxx::connection> pq_login;
    std::unique_ptr<pqxx::connection> pq_game;


    server_status server_status_ = server_status::uninitialized;
    game_server_status game_status_ = game_server_status::uninitialized;
    login_server_status login_status_ = login_server_status::uninitialized;

    // login server functionality
    void DeleteClient(std::shared_ptr<Client> client, bool save = false, bool deleteobj = false);

    bool CheckLogin(Client * client, std::string & account, std::string & pass, int64_t & account_id);
    void SendCharList(Client * client, StreamWrite & sw);

    bool RequestLogin(std::string account, std::string password);
    void CreateCharacter(Client * client, StreamRead & sr);
    void DeleteCharacter(Client * client, StreamRead & sr);
    void EnterGame(std::shared_ptr<Client> client, StreamRead & sr);

    void process_binary_message(StreamRead sr);
    void process_json_message(json obj);

public:
    // game server functionality

    Map * GetMap(std::string name);
};