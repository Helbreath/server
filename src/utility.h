//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

enum class server_status
{
    uninitialized = 0,
    running,
    shutdown,
    offline
};

enum class game_server_status
{
    uninitialized = 0,
    running,
    running_full,
    shutdown,
    offline
};

enum class login_server_status
{
    uninitialized = 0,
    running,
    running_queue,
    shutdown,
    offline
};
