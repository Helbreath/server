
#pragma once

#include <cstdint>

namespace hbx
{

class occupy_flag
{
public:
    occupy_flag(int dx, int dy, char side, int ek_num, int doi)
        : side(side)
        , ek_count(ek_num)
        , x(dx)
        , y(dy)
        , dynamic_object_index(doi)
    { }
    ~occupy_flag() = default;

    char side;
    int32_t ek_count;
    int32_t x;
    int32_t y;
    int32_t dynamic_object_index;
};

}
