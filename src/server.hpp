/*!
 * \file server.h
 * \date 2021/01/03 4:42
 *
 * \author Sharon Fox
 * Contact: sharon@sharonfox.dev
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#pragma once

#include "net/net_handler.hpp"
#include <asio/bind_executor.hpp>
#include <asio/executor_work_guard.hpp>
#include <asio/steady_timer.hpp>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/async.h>

#include <thread>
#include <mutex>
#include <random>
#include <condition_variable>
#include "async++.hpp"

#include "redisclient/redissyncclient.h"
#include "redisclient/redisasyncclient.h"

namespace pqxx { class connection; }

namespace hbx
{

class client;
class lserver;
class gserver;

enum RequestMethod
{
    Get,
    Post,
    Put,
    Patch,
    Delete,
    MAX_METHODS
};

struct request_params
{
    std::string path;
    RequestMethod method = Post;
    std::string body;
    std::string host;
    std::string port = "443";
    std::vector<std::string> headers;
    std::string _path_ex;
};

using asio_exec = asio::executor_work_guard<asio::io_context::executor_type>;
using work_ptr = std::unique_ptr<asio_exec>;
using namespace nlohmann;

class server
{
public:
    server(std::shared_ptr<asio::io_context> io_);
    ~server();

    struct hbx_timer
    {
        hbx_timer(asio::io_context & _io) : timer(_io) {}
        uint64_t id;
        asio::steady_timer timer;
        std::function <void(void)> fn;
    };

    void update_stats();

    std::string execute(request_params && params);

    std::string get_method(RequestMethod method) const noexcept
    {
        switch (method)
        {
            case Post:
                return "POST";
            case Put:
                return "PUT";
            case Patch:
                return "PATCH";
            case Delete:
                return "DELETE";
            case Get:
            default:
                return "GET";
        }
    }

    server_status get_status() const noexcept { return status_; }
    void set_status(server_status _status) noexcept;
    void run();
    void stop();

    void start_lserver();
    void start_gservers();
    void start_gserver(const std::string & name);

    void transfer_client(std::shared_ptr<client> _client, std::string server_name, std::string map_name);

    /// Socket is not closed in this function. It is primarily called from net_handler after a socket closure
    void close_client(std::shared_ptr<client> _client);

    void handle_message(const message_entry & msg, std::shared_ptr<client> _client);

    gserver * find_gserver(uint64_t server_id);
    gserver * find_gserver(std::string name);
    gserver * find_gserver(std::string name, std::string map_name);

    template <typename int_type, typename ratio = std::nano>
    uint64_t set_timeout(std::function <void(void)> fn, const std::chrono::duration<int_type, ratio> & t)
    {
        std::unique_lock<std::mutex> l(timer_m);
        std::shared_ptr<hbx_timer> tmr = std::make_shared<hbx_timer>(*io_context_);
        tmr->id = timer_count++;
        tmr->fn = fn;
        tmr->timer.async_wait(std::bind(&server::timer_cb, this, tmr));
        timers.insert(tmr);
        tmr->timer.expires_after(t);
        return tmr->id;
    }

    template <typename int_type, typename ratio = std::nano>
    uint64_t set_interval(std::function <void(void)> fn, const std::chrono::duration<int_type, ratio> & t)
    {
        std::unique_lock<std::mutex> l(timer_m);
        std::shared_ptr<hbx_timer> tmr = std::make_shared<hbx_timer>(*io_context_);
        tmr->id = timer_count++;
        tmr->fn = fn;
        tmr->timer.async_wait(std::bind(&server::interval_cb, this, tmr));
        timers.insert(tmr);
        tmr->timer.expires_after(t);
        return tmr->id;
    }

    void clear_timeout(uint64_t t)
    {
        std::unique_lock<std::mutex> l(timer_m);
        for (auto timer : timers)
        {
            if (timer->id == t)
            {
                timers.erase(timer);
                return;
            }
        }
    }

    void timer_cb(std::shared_ptr<hbx_timer> t)
    {
        std::unique_lock<std::mutex> l(timer_m);
        timers.erase(t);
        t->fn();
    }

    void interval_cb(std::shared_ptr<hbx_timer> t)
    {
        std::unique_lock<std::mutex> l(timer_m);
        t->fn();
    }

    std::shared_ptr<spdlog::logger> log;

    std::mutex cl_m;
    std::set<std::shared_ptr<client>> clients;

    std::condition_variable cv;

    std::unique_ptr<net_handler> nh;

    std::set<std::shared_ptr<hbx_timer>> timers;
    uint64_t timer_count = 1;

    std::mutex timer_m;

    std::unique_ptr<pqxx::connection> pg;

    uint16_t upper_version;
    uint16_t lower_version;
    uint16_t patch_version;

private:
    int64_t threadcount;
    std::unique_ptr<lserver> lserver_;
    std::set<std::unique_ptr<gserver>> gservers_;
    std::shared_ptr<asio::io_context> io_context_;
    work_ptr wrk = nullptr;
    std::vector<std::thread> threads;
    redisclient::RedisSyncClient redis;
    redisclient::RedisAsyncClient redis_async;
    asio::steady_timer web_stats_timer;
    std::unordered_map<std::string, asio::ip::basic_resolver<asio::ip::tcp>::results_type> resolver_cache_;
    server_status status_ = server_status::uninitialized;

    inline int32_t gen_rand(int32_t _min = 0, int32_t _max = INT32_MAX)
    {
        std::uniform_int_distribution<int32_t> distribution(_min, _max);
        std::mt19937 engine;
        return distribution(engine);
    }

    json config;

#ifdef NDEBUG
    bool is_production = true;
#else
    bool is_production = false;
#endif
};

}
