/*
 * Copyright (C) Alex Nekipelov (alex@nekipelov.net)
 * License: MIT
 */

#ifndef REDISASYNCCLIENT_REDISASYNCCLIENT_CPP
#define REDISASYNCCLIENT_REDISASYNCCLIENT_CPP

#include <memory>
#include <functional>

#include "../redisasyncclient.h"

namespace redisclient {

RedisAsyncClient::RedisAsyncClient(asio::io_context &ioService)
    : pimpl(std::make_shared<RedisClientImpl>(ioService))
{
    pimpl->errorHandler = std::bind(&RedisClientImpl::defaulErrorHandler, std::placeholders::_1);
}

RedisAsyncClient::~RedisAsyncClient()
{
    pimpl->close();
}

void RedisAsyncClient::connect(const ::asio::ip::address &address,
                               unsigned short port,
                               std::function<void(bool, const std::string &)> handler)
{
    asio::ip::tcp::endpoint endpoint(address, port);
    connect(endpoint, std::move(handler));
}

void RedisAsyncClient::connect(const asio::ip::tcp::endpoint &endpoint,
                               std::function<void(bool, const std::string &)> handler)
{
    if( pimpl->state == State::Unconnected || pimpl->state == State::Closed )
    {
        pimpl->state = State::Connecting;
        pimpl->socket.async_connect(endpoint, std::bind(&RedisClientImpl::handleAsyncConnect,
                    pimpl, std::placeholders::_1, std::move(handler)));
    }
    else
    {
        std::stringstream ss;

        ss << "RedisAsyncClient::connect called on socket with state " << to_string(pimpl->state);
        handler(false, ss.str());
    }
}

bool RedisAsyncClient::isConnected() const
{
    return pimpl->getState() == State::Connected ||
            pimpl->getState() == State::Subscribed;
}

void RedisAsyncClient::disconnect()
{
    pimpl->close();
}

void RedisAsyncClient::installErrorHandler(std::function<void(const std::string &)> handler)
{
    pimpl->errorHandler = std::move(handler);
}

void RedisAsyncClient::command(const std::string &cmd, std::deque<RedisBuffer> args,
                          std::function<void(RedisValue)> handler)
{
    if(stateValid())
    {
        args.emplace_front(cmd);

        pimpl->post(std::bind(&RedisClientImpl::doAsyncCommand, pimpl,
                    std::move(pimpl->makeCommand(args)), std::move(handler)));
    }
}

RedisAsyncClient::Handle RedisAsyncClient::subscribe(
        const std::string &channel,
        std::function<void(std::vector<char> msg)> msgHandler,
        std::function<void(RedisValue)> handler)
{
    auto handleId = pimpl->subscribe("subscribe", channel, msgHandler, handler);    
    return { handleId , channel };
}

RedisAsyncClient::Handle RedisAsyncClient::psubscribe(
    const std::string &pattern,
    std::function<void(std::vector<char> msg)> msgHandler,
    std::function<void(RedisValue)> handler)
{
    auto handleId = pimpl->subscribe("psubscribe", pattern, msgHandler, handler);
    return{ handleId , pattern };
}

void RedisAsyncClient::unsubscribe(const Handle &handle)
{
    pimpl->unsubscribe("unsubscribe", handle.id, handle.channel, dummyHandler);
}

void RedisAsyncClient::punsubscribe(const Handle &handle)
{
    pimpl->unsubscribe("punsubscribe", handle.id, handle.channel, dummyHandler);
}

void RedisAsyncClient::singleShotSubscribe(const std::string &channel,
                                           std::function<void(std::vector<char> msg)> msgHandler,
                                           std::function<void(RedisValue)> handler)
{
    pimpl->singleShotSubscribe("subscribe", channel, msgHandler, handler);
}

void RedisAsyncClient::singleShotPSubscribe(const std::string &pattern,
    std::function<void(std::vector<char> msg)> msgHandler,
    std::function<void(RedisValue)> handler)
{
    pimpl->singleShotSubscribe("psubscribe", pattern, msgHandler, handler);
}

void RedisAsyncClient::publish(const std::string &channel, const RedisBuffer &msg,
                          std::function<void(RedisValue)> handler)
{
    assert( pimpl->state == State::Connected );

    static const std::string publishStr = "PUBLISH";

    if( pimpl->state == State::Connected )
    {
        std::deque<RedisBuffer> items(3);

        items[0] = publishStr;
        items[1] = channel;
        items[2] = msg;

        pimpl->post(std::bind(&RedisClientImpl::doAsyncCommand, pimpl,
                    pimpl->makeCommand(items), std::move(handler)));
    }
    else
    {
        std::stringstream ss;

        ss << "RedisAsyncClient::command called with invalid state "
           << to_string(pimpl->state);

        pimpl->errorHandler(ss.str());
    }
}

RedisAsyncClient::State RedisAsyncClient::state() const
{
    return pimpl->getState();
}

bool RedisAsyncClient::stateValid() const
{
    assert( pimpl->state == State::Connected );

    if( pimpl->state != State::Connected )
    {
        std::stringstream ss;

        ss << "RedisAsyncClient::command called with invalid state "
           << to_string(pimpl->state);

        pimpl->errorHandler(ss.str());
        return false;
    }

    return true;
}

}

#endif // REDISASYNCCLIENT_REDISASYNCCLIENT_CPP
