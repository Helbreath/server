/*
 * Copyright (C) Alex Nekipelov (alex@nekipelov.net)
 * License: MIT
 */


#ifndef REDISSYNCCLIENT_REDISBUFFER_H
#define REDISSYNCCLIENT_REDISBUFFER_H

#include <string>
#include <list>
#include <vector>
#include <string.h>

#include "config.h"

namespace redisclient {

class RedisBuffer
{
public:
    inline RedisBuffer();
    inline RedisBuffer(const char *ptr, int64_t dataSize);
    inline RedisBuffer(const char *s);
    inline RedisBuffer(const std::string &s);
    inline RedisBuffer(const std::vector<char> &buf);

    inline int64_t size() const;
    inline const char *data() const;

private:
    const char *ptr_;
    int64_t size_;
};


RedisBuffer::RedisBuffer()
    : ptr_(NULL), size_(0)
{
}

RedisBuffer::RedisBuffer(const char *ptr, int64_t dataSize)
    : ptr_(ptr), size_(dataSize)
{
}

RedisBuffer::RedisBuffer(const char *s)
    : ptr_(s), size_(s == NULL ? 0 : strlen(s))
{
}

RedisBuffer::RedisBuffer(const std::string &s)
    : ptr_(s.c_str()), size_(s.length())
{
}

RedisBuffer::RedisBuffer(const std::vector<char> &buf)
    : ptr_(buf.empty() ? NULL : &buf[0]), size_(buf.size())
{
}

int64_t RedisBuffer::size() const
{
    return size_;
}

const char *RedisBuffer::data() const
{
    return ptr_;
}

}

#endif //REDISSYNCCLIENT_REDISBUFFER_H

