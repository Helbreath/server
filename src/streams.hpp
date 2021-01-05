/*
* File:   streams.h
* Author: zero
*
* Created on June 23, 2012, 7:25 AM
*/

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <climits>
#include "defines.hpp"

using std::ofstream;
using std::string;

/**
** Exceptions:
** -191 = string too long
** -192 = data variable invalid
** -193 = Stream read out of bounds
**/

namespace hbx
{

class file_write
{
public:
    file_write(ofstream & input)
        : file(input) {};
    ~file_write()
    {
        file.close();
    }
    ofstream & file;

    void write_byte(char value)
    {
        file.write(&value, 1);
    };

    void write_bytes(char * value, int sz)
    {
        file.write(value, sz);
    };

    void write_short(int16_t value)
    {
        char sc[2];
        int16_t * sp = (int16_t *)sc;
        *sp = value;
        file.write(sc, 2);
    };

    void write_int(int32_t value)
    {
        char ic[4];
        int * ip = (int *)ic;
        *ip = value;
        file.write(ic, 4);
    };

    void write_int64(int64_t value)
    {
        char ic[8];
        int64_t * ip = (int64_t *)ic;
        *ip = value;
        file.write(ic, 8);
    };

    void write_float(float value)
    {
        write_bytes((char *)&value, 4);
    };

    void write_double(double value)
    {
        write_bytes((char *)&value, 8);
    };

    void write_string(std::string value)
    {
        write_int(uint32_t(value.length()));
        file.write(value.c_str(), value.length());
    };
};

#define VerifyData \
    if (data == 0) \
    throw -192
#define VerifySize(a)                                     \
    if (position + a > size)                              \
    {                                                     \
        uint64_t oldsize = size;                          \
        if ((uint64_t)(float(size) * 1.15) < oldsize + a) \
        {                                                 \
            size = (uint64_t)(float(size) * 1.15) + a;    \
        }                                                 \
        else                                              \
        {                                                 \
            size = (uint64_t)(float(size) * 1.15);        \
        }                                                 \
        char *temp = new char[size];                      \
        memset(temp, 0, size);                            \
        memcpy(temp, data, oldsize);                      \
        delete[] data;                                    \
        data = temp;                                      \
    }
#define VerifyLength(a)      \
    if (position + a > size) \
    {                        \
        throw -193;          \
    }
class stream_write
{
public:
    stream_write(uint32_t initsize = 100)
    {
        size = initsize;
        position = 0;
        data = new char[size];
        std::memset(data, 0, size);
    };
    ~stream_write()
    {
        if (data)
            delete[] data;
    }
    char * data;
    uint64_t position;
    uint64_t size;

    void clear()
    {
        if (data)
            delete[] data;
        size = 100;
        data = new char[size];
        position = 0;
    }

    uint64_t write_size()
    {
        VerifyData;
        std::memcpy(data, &position, 2);
        return position;
    };

    void write_bytes(const char * value, int sz)
    {
        VerifyData;
        VerifySize(sz);
        std::memcpy(data + position, value, sz);
        position += sz;
    };

    void write_array(const char * value, int sz)
    {
        VerifyData;
        write_int16(sz);
        VerifySize(sz);
        std::memcpy(data + position, value, sz);
        position += sz;
    };

    void write_int8(const int8_t value)
    {
        VerifyData;
        VerifySize(1);
        std::memcpy(data + position, &value, 1);
        position++;
    };

    void write_uint8(const uint8_t value)
    {
        VerifyData;
        VerifySize(1);
        std::memcpy(data + position, &value, 1);
        position++;
    };

    void write_int16(const int16_t value)
    {
        VerifyData;
        VerifySize(2);
        std::memcpy(data + position, &value, 2);
        position += 2;
    };

    void write_uint16(const uint16_t value)
    {
        VerifyData;
        VerifySize(2);
        std::memcpy(data + position, &value, 2);
        position += 2;
    };

    void write_int32(const int32_t value)
    {
        VerifyData;
        VerifySize(4);
        std::memcpy(data + position, &value, 4);
        position += 4;
    };

    void write_uint32(const uint32_t value)
    {
        VerifyData;
        VerifySize(4);
        std::memcpy(data + position, &value, 4);
        position += 4;
    };

    void write_int64(const int64_t value)
    {
        VerifyData;
        VerifySize(8);
        std::memcpy(data + position, &value, 8);
        position += 8;
    };

    void write_uint64(const uint64_t value)
    {
        VerifyData;
        VerifySize(8);
        std::memcpy(data + position, &value, 8);
        position += 8;
    };

    void write_float(const float value)
    {
        VerifyData;
        VerifySize(4);
        std::memcpy(data + position, &value, 4);
        position += 4;
    };

    void write_double(const double value)
    {
        VerifyData;
        VerifySize(8);
        std::memcpy(data + position, &value, 8);
        position += 8;
    };

    void write_message_id(message_id id)
    {
        write_int32(static_cast<int32_t>(id));
    }

    void write_string(const std::string & value)
    {
        VerifyData;
        if (value.length() >= SHRT_MAX)
            throw - 191;
        write_int16(char(value.length()));
        VerifySize(value.length());
        std::memcpy(data + position, value.c_str(), value.length());
        position += value.length();
    };

    void write_string(const std::string & value, uint64_t length)
    {
        VerifyData;
        VerifySize(length);
        if (value.length() > length)
            std::memcpy(data + position, value.c_str(), length);
        else
            std::memcpy(data + position, value.c_str(), value.length());
        position += length;
    };
};

class file_read
{
public:
    file_read(char * input, uint32_t in)
        : data(input), position(0), size(in) {};
    ~file_read()
    {
    }

    char * data;
    uint64_t position;
    uint64_t size;

    char read_byte()
    {
        VerifyData;
        VerifyLength(1);
        char * cp = (char *)(data + position);
        position++;
        return *cp;
    };

    void read_bytes(char * p, uint16_t sz)
    {
        VerifyData;
        VerifyLength(sz);
        memcpy(p, (char *)(data + position), sz);
        position += sz;
    }

    uint16_t read_short()
    {
        VerifyData;
        VerifyLength(2);
        uint16_t * sp = (uint16_t *)(data + position);
        position += 2;
        return *sp;
    };

    int read_int()
    {
        VerifyData;
        VerifyLength(4);
        uint32_t * ip = (uint32_t *)(data + position);
        position += 4;
        return *ip;
    };

    int64_t read_int64()
    {
        VerifyData;
        VerifyLength(8);
        int64_t * ip = (int64_t *)(data + position);
        position += 8;
        return *ip;
    };

    float read_float()
    {
        VerifyData;
        VerifyLength(4);
        float * fp = (float *)(data + position);
        position += 4;
        return *fp;
    };

    double read_double()
    {
        VerifyData;
        VerifyLength(8);
        double * fp = (double *)(data + position);
        position += 8;
        return *fp;
    };

    std::string read_string()
    {
        uint32_t sz = read_int();
        if (sz == 0)
            return "";
        VerifyData;
        VerifyLength(sz);
        char * c = new char[sz + 1];
        memset(c, 0, sz + 1);
        memcpy(c, data + position, sz);
        position += sz;
        std::string str;
        str = c;
        delete[] c;
        return str;
    };
};

class stream_read
{
public:
    stream_read(char * input, uint32_t in)
        : data(input), position(0), size(in) {
        //size += 4;
    };
    ~stream_read()
    {
    }

    char * data;
    uint64_t position;
    uint64_t size;

    uint16_t read_size()
    {
        VerifyData;
        return *(uint16_t *)(data);
    };

    char * read_array()
    {
        VerifyData;
        uint16_t sz = read_int16();
        VerifyLength(sz);
        char * temp = new char[sz];
        std::memcpy(temp, (char *)(data + position), sz);
        position += sz;
        return temp;
    };

    void read_bytes(char * p, uint16_t sz)
    {
        VerifyData;
        VerifyLength(sz);
        std::memcpy(p, (char *)(data + position), sz);
        position += sz;
    }

    int8_t read_int8()
    {
        VerifyData;
        VerifyLength(1);
        int8_t cp = (int8_t) * (data + position);
        position++;
        return cp;
    };

    uint8_t read_uint8()
    {
        VerifyData;
        VerifyLength(1);
        uint8_t cp = (uint8_t) * (data + position);
        position++;
        return cp;
    };

    int16_t read_int16()
    {
        VerifyData;
        VerifyLength(2);
        int16_t * sp = (int16_t *)(data + position);
        position += 2;
        return *sp;
    };

    uint16_t read_uint16()
    {
        VerifyData;
        VerifyLength(2);
        uint16_t * sp = (uint16_t *)(data + position);
        position += 2;
        return *sp;
    };

    int read_int32()
    {
        VerifyData;
        VerifyLength(4);
        int32_t * ip = (int32_t *)(data + position);
        position += 4;
        return *ip;
    };

    int read_uint32()
    {
        VerifyData;
        VerifyLength(4);
        uint32_t * ip = (uint32_t *)(data + position);
        position += 4;
        return *ip;
    };

    int64_t read_int64()
    {
        VerifyData;
        VerifyLength(8);
        int64_t * ip = (int64_t *)(data + position);
        position += 8;
        return *ip;
    };

    uint64_t read_uint64()
    {
        VerifyData;
        VerifyLength(8);
        uint64_t * ip = (uint64_t *)(data + position);
        position += 8;
        return *ip;
    };

    float read_float()
    {
        VerifyData;
        VerifyLength(4);
        float * fp = (float *)(data + position);
        position += 4;
        return *fp;
    };

    double read_double()
    {
        VerifyData;
        VerifyLength(8);
        double * fp = (double *)(data + position);
        position += 8;
        return *fp;
    };

    message_id read_message_id()
    {
        return static_cast<message_id>(read_int32());
    }

    std::string read_string()
    {
        uint32_t sz = read_int16();
        if (sz == 0)
            return "";
        VerifyData;
        VerifyLength(sz);
        char * c = new char[sz + 1];
        std::memset(c, 0, sz + 1);
        std::memcpy(c, data + position, sz);
        position += sz;
        std::string str;
        str = c;
        delete[] c;
        return str;
    };
    std::string read_string(uint32_t sz)
    {
        VerifyData;
        VerifyLength(sz);
        char * c = new char[sz + 1];
        std::memset(c, 0, sz + 1);
        std::memcpy(c, data + position, sz);
        position += sz;
        std::string str;
        str = c;
        delete[] c;
        return str;
    };
};

}
