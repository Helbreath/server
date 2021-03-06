/*
 * Copyright (C) Alex Nekipelov (alex@nekipelov.net)
 * License: MIT
 */

#ifndef REDISASYNCCLIENT_REDISCLIENT_H
#define REDISASYNCCLIENT_REDISCLIENT_H

#include <asio/io_service.hpp>

#include <string>
#include <list>
#include <type_traits>
#include <functional>

#include "impl/redisclientimpl.h"
#include "redisvalue.h"
#include "redisbuffer.h"
#include "config.h"

namespace redisclient {

class RedisClientImpl;

class RedisAsyncClient {
public:
    // Subscribe handle.
    struct Handle {
        int64_t id;
        std::string channel;
    };

    typedef RedisClientImpl::State State;

    REDIS_CLIENT_DECL RedisAsyncClient(asio::io_context &ioService);
    REDIS_CLIENT_DECL ~RedisAsyncClient();

    // Connect to redis server
    REDIS_CLIENT_DECL void connect(
            const asio::ip::address &address,
            unsigned short port,
            std::function<void(bool, const std::string &)> handler);

    // Connect to redis server
    REDIS_CLIENT_DECL void connect(
            const asio::ip::tcp::endpoint &endpoint,
            std::function<void(bool, const std::string &)> handler);

    // backward compatibility
    inline void asyncConnect(
            const asio::ip::address &address,
            unsigned short port,
            std::function<void(bool, const std::string &)> handler)
    {
        connect(address, port, std::move(handler));
    }

    // backward compatibility
    inline void asyncConnect(
            const asio::ip::tcp::endpoint &endpoint,
            std::function<void(bool, const std::string &)> handler)
    {
        connect(endpoint, handler);
    }

    // Return true if is connected to redis.
    // Deprecated: use state() == RedisAsyncClisend::State::Connected.
    REDIS_CLIENT_DECL bool isConnected() const;

    // Return connection state. See RedisClientImpl::State.
    REDIS_CLIENT_DECL State state() const;

    // disconnect from redis and clear command queue
    REDIS_CLIENT_DECL void disconnect();

    // Set custom error handler.
    REDIS_CLIENT_DECL void installErrorHandler(
            std::function<void(const std::string &)> handler);

    // Execute command on Redis server with the list of arguments.
    REDIS_CLIENT_DECL void command(
            const std::string &cmd, std::deque<RedisBuffer> args,
            std::function<void(RedisValue)> handler = dummyHandler);

    // Subscribe to channel. Handler msgHandler will be called
    // when someone publish message on channel. Call unsubscribe 
    // to stop the subscription.
    REDIS_CLIENT_DECL Handle subscribe(const std::string &channelName,
                                       std::function<void(std::vector<char> msg)> msgHandler,
                                       std::function<void(RedisValue)> handler = &dummyHandler);


    REDIS_CLIENT_DECL Handle psubscribe(const std::string &pattern,
                                        std::function<void(std::vector<char> msg)> msgHandler,
                                        std::function<void(RedisValue)> handler = &dummyHandler);

    // Unsubscribe
    REDIS_CLIENT_DECL void unsubscribe(const Handle &handle);
    REDIS_CLIENT_DECL void punsubscribe(const Handle &handle);

    // Subscribe to channel. Handler msgHandler will be called
    // when someone publish message on channel; it will be 
    // unsubscribed after call.
    REDIS_CLIENT_DECL void singleShotSubscribe(
            const std::string &channel,
            std::function<void(std::vector<char> msg)> msgHandler,
            std::function<void(RedisValue)> handler = &dummyHandler);

    REDIS_CLIENT_DECL void singleShotPSubscribe(
            const std::string &channel,
            std::function<void(std::vector<char> msg)> msgHandler,
            std::function<void(RedisValue)> handler = &dummyHandler);

    // Publish message on channel.
    REDIS_CLIENT_DECL void publish(
            const std::string &channel, const RedisBuffer &msg,
            std::function<void(RedisValue)> handler = &dummyHandler);

    REDIS_CLIENT_DECL static void dummyHandler(RedisValue) {}

protected:
    REDIS_CLIENT_DECL bool stateValid() const;

private:
    std::shared_ptr<RedisClientImpl> pimpl;
};

}

#ifdef REDIS_CLIENT_HEADER_ONLY
#include "impl/redisasyncclient.cpp"
#endif

#endif // REDISASYNCCLIENT_REDISCLIENT_H
