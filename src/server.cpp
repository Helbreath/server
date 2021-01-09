
#include "utility.hpp"
#include <string>
#include "server.hpp"
#include <climits>
#include <iostream>
#include <async++.h>
#include <nlohmann/json.hpp>
#include <asio/connect.hpp>
#include <asio/write.hpp>
#include <asio/read_until.hpp>
#include <asio/read.hpp>
#include <asio/streambuf.hpp>
#include <pqxx/pqxx>
#include "client.hpp"
#include "lserver.hpp"
#include "gserver.hpp"

#ifdef WIN32
#define PATH_MAX 512
#endif

using namespace std::chrono_literals;
using namespace nlohmann;

namespace hbx
{

server::server(std::shared_ptr<asio::io_context> io_)
    : io_context_(io_)
    , redis(*io_)
    , redis_async(*io_)
    , web_stats_timer(*io_)
{
    std::ifstream config_file("config/config.json");
    config_file >> config;

    spdlog::init_thread_pool(8192, 2);
    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    std::vector<spdlog::sink_ptr> sinks;// { stdout_sink, rotating_sink };
    sinks.push_back(stdout_sink);

    // 5MB max filesize and 20 max log files
    char cwd[PATH_MAX];
#ifdef WIN32
    if (_getcwd(cwd, sizeof cwd) == NULL)
        throw 1;
    int mkdirres = _mkdir(fmt::format("{}/{}", cwd, "./log").c_str());
#else
    if (getcwd(cwd, sizeof cwd) == NULL)
        throw 1;
    mkdir(fmt::format("{}/{}", cwd, "./log").c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
    auto rotating = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("log/server.log", 1024 * 1024 * 5, 20);
    sinks.push_back(rotating);

    log = std::make_shared<spdlog::async_logger>("hbx", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
    spdlog::register_logger(log);

#ifndef _NDEBUG
    log->set_pattern("%^%Y-%m-%d %H:%M:%S.%e [%L] [th#%5t]%$ : %v");
#else
    log->set_pattern("%^%Y-%m-%d %H:%M:%S.%e [%L]%$ : %v");
#endif
    log->set_level(spdlog::level::level_enum::trace);

    io_context_ = std::make_shared<asio::io_context>();
    wrk = std::make_unique<asio_exec>(asio::make_work_guard(*io_context_));
    threadcount = std::thread::hardware_concurrency();
    if (config.contains("threads"))
        threadcount = config["threads"].get<int64_t>();
    for (int64_t i = 0; i < threadcount; ++i)
        threads.emplace_back(std::bind(static_cast<asio::io_context::count_type(asio::io_context:: *)()>(&asio::io_context::run), io_context_.get()));

    web_stats_timer.expires_after(5s);
    web_stats_timer.async_wait(std::bind(&server::update_stats, this));

    std::string errmsg;
    asio::ip::address connectaddress = asio::ip::make_address(config["redis"]["host"].get<std::string>().c_str());
    if (!redis.connect(connectaddress, config["redis"]["port"].get<uint16_t>(), errmsg))
    {
        log->critical("Can't connect to redis: {}", errmsg);
        throw std::runtime_error("Can't connect to redis.");
    }

}

server::~server()
{
    status_ = server_status::closed;
    wrk.reset();
    if (io_context_)
        io_context_->stop();
    for (auto & t : threads)
        t.join();
}

std::string server::execute(request_params && params)
{
    std::chrono::system_clock::time_point http_date;

    try
    {
        asio::ip::basic_resolver<asio::ip::tcp>::results_type r;

        //TODO: make cache expire?
        auto it = resolver_cache_.find(params.host);
        if (it == resolver_cache_.end())
        {
            asio::ip::tcp::resolver resolver(*io_context_);
            r = resolver.resolve(params.host, params.port);
            resolver_cache_.emplace(params.host, r);
        }
        else
            r = it->second;


        if (params.port == "443")
        {
            asio::ssl::context ctx(asio::ssl::context::tlsv12);

            ctx.set_options(
                asio::ssl::context::default_workarounds
                | asio::ssl::context::no_sslv2
                | asio::ssl::context::no_sslv3);

            asio::ssl::stream<asio::ip::tcp::socket> socket(*io_context_, ctx);
            SSL_set_tlsext_host_name(socket.native_handle(), params.host.data());

            asio::connect(socket.lowest_layer(), r);

            asio::error_code handshake_ec;
            socket.handshake(asio::ssl::stream_base::client, handshake_ec);

            asio::streambuf request;
            std::ostream request_stream(&request);
            request_stream << get_method(params.method) << " " << (!params.path.empty() ? params.path : "/") << " HTTP/1.0\r\n";
            request_stream << "Host: " << params.host << "\r\n";
            request_stream << "Accept: */*\r\n";
            for (auto & h : params.headers)
                request_stream << h << "\r\n";

            if (!params.body.empty() || params.method == Post || params.method == Patch || params.method == Put)
            {
                request_stream << "Content-Length: " << params.body.size() << "\r\n";
                request_stream << "Content-Type: application/json\r\n";
                request_stream << "Connection: close\r\n\r\n";
                request_stream << params.body;
            }
            else
                request_stream << "Connection: close\r\n\r\n";

            asio::write(socket, request);
            asio::streambuf response;
            asio::read_until(socket, response, "\r\n");
            std::stringstream response_content;
            response_content << &response;

            asio::error_code error;
            while (asio::read(socket, response, asio::transfer_at_least(1), error))
                response_content << &response;

            if (error != asio::error::eof && error != asio::ssl::error::stream_truncated)
                throw asio::system_error(error);

            std::string s = response_content.str();
            if (std::size_t index = s.find("\r\n\r\n"); index != std::string::npos)
            {
                return s.substr(index + 4, s.length() - index - 4);
            }
            return "";
        }
        else
        {
            asio::ip::tcp::socket socket(*io_context_);
            asio::connect(socket, r);

            asio::streambuf request;
            std::ostream request_stream(&request);
            request_stream << get_method(params.method) << " " << (!params.path.empty() ? params.path : "/") << " HTTP/1.0\r\n";
            request_stream << "Host: " << params.host << "\r\n";
            request_stream << "Accept: */*\r\n";
            for (auto & h : params.headers)
                request_stream << h << "\r\n";

            if (!params.body.empty() || params.method == Post || params.method == Patch || params.method == Put)
            {
                request_stream << "Content-Length: " << params.body.size() << "\r\n";
                request_stream << "Content-Type: application/json\r\n";
                request_stream << "Connection: close\r\n\r\n";
                request_stream << params.body;
            }
            else
                request_stream << "Connection: close\r\n\r\n";

            asio::write(socket, request);
            asio::streambuf response;
            asio::read_until(socket, response, "\r\n");
            std::stringstream response_content;
            response_content << &response;

            std::string s = response_content.str();
            if (std::size_t index = s.find("\r\n\r\n"); index != std::string::npos)
            {
                return s.substr(index + 4, s.length() - index - 4);
            }
            return "";
        }
    }
    catch (std::exception & e)
    {
        log->error("Exception: {}", e.what());
    }
    return "";
}

void server::update_stats()
{
    if (get_status() >= server_status::closing)
        return;
    web_stats_timer.expires_at(std::chrono::steady_clock::now() + std::chrono::seconds(10));
    try
    {
        request_params rp;
        rp.host = "127.0.0.1";
        rp.port = "9998";
        if (is_production)
            rp.path = "/bot";
        else
            rp.path = "/test/bot";

        json m;
        {
            json j;
            j["memory"] = (uint64_t)utility::getCurrentRSS();

            m.push_back(j);
        }

        rp.body = m.dump();
        execute(std::forward<request_params>(rp));
    }
    catch (...)
    {
    }
    web_stats_timer.async_wait(std::bind(&server::update_stats, this));
}

void server::set_status(server_status _status) noexcept
{
    status_ = _status;
    lserver_->status_ = _status;
}

void server::run()
{
    nh = std::make_unique<net_handler>(io_context_, this);
    status_ = server_status::online;

    try
    {
        const json & sql = config["sql"];
        std::string host = sql["host"];
        std::string user = sql["user"];
        std::string pass = sql["pass"];
        std::string db = sql["db"];
        int32_t port = sql["port"];
        pg = std::make_unique<pqxx::connection>(fmt::format("host={} port={} dbname={} user={} password={}", host, port, db, user, pass));
        std::cout << "Connected to " << pg->dbname() << "\n";
        pqxx::work W{ *pg };

        pqxx::result R{ W.exec("select * from characters where account_id = 4") };

        std::cout << "Found " << R.size() << " characters:\n";
        for (auto row : R)
            std::cout << row[0].c_str() << '\n';

        W.commit();
    }
    catch (pqxx::broken_connection & e)
    {
        std::cerr << e.what() << '\n';
        return;
    }
    catch (const std::exception & e)
    {
        std::cerr << e.what() << '\n';
        return;
    }


//     request_params rp;
//     rp.method = Get;
//     rp.host = "discord.com";
//     rp.headers.push_back("Authorization: Bot Mjg4MDYzMTYzNzI5OTY5MTUy.XrDe-Q.yESPUJMzEzIQkJuDWT-maHb9D24");
//     rp.path = "/gateway/bot";
// 
//     execute(std::forward<request_params>(rp));

//     {
//         request_params rp;
//         rp.method = Delete;
//         rp.host = "discord.com";
//         rp.headers.push_back("Authorization: Bot Mjg4MDYzMTYzNzI5OTY5MTUy.XrDe-Q.yESPUJMzEzIQkJuDWT-maHb9D24");
//         rp.path = "/api/guilds/287048029524066334/bans/553478921870508061";
// 
//         execute(std::forward<request_params>(rp));
//     }
//     using namespace std::chrono_literals;
//     std::this_thread::sleep_for(10s);
//     {
//         request_params rp;
//         rp.method = Put;
//         rp.host = "discord.com";
//         rp.headers.push_back("Authorization: Bot Mjg4MDYzMTYzNzI5OTY5MTUy.XrDe-Q.yESPUJMzEzIQkJuDWT-maHb9D24");
//         rp.path = "/api/guilds/287048029524066334/bans/553478921870508061";
// 
//         execute(std::forward<request_params>(rp));
//     }
    //redis.command("PUBLISH", { "thing", "thang thong" });
    /*
    redisclient::RedisValue result;
    std::deque<redisclient::RedisBuffer> v;
    for (auto & k : value)
    v.emplace_back(k.data());
    result = redis.command(std::string{ action }, v);
    if (result.isOk())
    return result.toString();
    else if (result.isError())
    {
    std::stringstream ss;
    for (const auto & a : value)
        ss << a << ' ';
    throw aegis::exception(fmt::format("result_action({}) failure: {} || {}", action, result.toString(), ss.str()), aegis::make_error_code(aegis::error::bad_redis_request));
    }
    throw aegis::exception(fmt::format("result_action({}) failure (no error, no ok)", action), aegis::make_error_code(aegis::error::bad_redis_request));
    */
}

void server::stop()
{
    status_ = server_status::closing;
    cv.notify_all();
}

void server::start_lserver()
{
    lserver_ = std::make_unique<lserver>(this);
}

void server::start_gservers()
{
    // start all up
}

void server::start_gserver(const std::string & name)
{
    auto res = gservers_.insert(std::make_unique<gserver>(this));
    gserver * gs = (*res.first).get();
    static uint64_t gserver_id = 0;

    gs->id = ++gserver_id;
}

void server::transfer_client(std::shared_ptr<client> _client, std::string server_name, std::string map_name)
{
    for (auto & g : gservers_)
    {
        gserver & gs = g.get();
        if (gs.server_name != server_name)
            continue;

        _client->server_id = gs.id;
        gs.handle_new_client(_client);
    }
}

void server::close_client(std::shared_ptr<client> _client)
{
    nh->stop(_client->socket_);
    if (_client->server_id != 0)
    {
        gserver * gs = find_gserver(_client->server_id);
        if (gs)
            gs->handle_close_client(_client);
    }

    std::unique_lock<std::mutex> l(cl_m);
    clients.erase(_client);
}

void server::handle_message(const message_entry & msg, std::shared_ptr<client> _client)
{
    skt_mode mode = _client->socket_mode();
    if (mode == gameserver)
    {
        gserver * gs = find_gserver(_client->server_id);
        if (gs) lserver_->handle_message(msg, _client);
        else log->error("No gserver attached to client gs:{} client id:{}", _client->server_id, _client->account_id);
    }

    if (mode == loginserver)
    {
        lserver_->handle_message(msg, _client);
    }
}

gserver * server::find_gserver(int8_t server_id)
{
    for (auto & g : gservers_)
    {
        gserver & gs = g.get();
        if (gs.id != server_id)
            continue;
        return *g;
    }
    return nullptr;
}

}
