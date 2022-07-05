//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include <ixwebsocket/IXConnectionState.h>
#include <ixwebsocket/IXWebSocket.h>

class Client;

class connection_state_hb : public ix::ConnectionState
{
public:
    std::shared_ptr<Client> client;
    std::weak_ptr<ix::WebSocket> websocket;
    bool disconnected = false;
    std::chrono::time_point<std::chrono::steady_clock> disconnecttime = std::chrono::steady_clock::now();
    std::chrono::time_point<std::chrono::steady_clock> lastpackettime = std::chrono::steady_clock::now();
};
