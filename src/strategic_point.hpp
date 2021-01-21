
#pragma once

#include <cstdint>

namespace hbx
{

class strategic_point
{
public:
    strategic_point() = default;
    ~strategic_point() = default;

    int32_t	side = 0;
    int32_t value = 0;
    int32_t x = 0;
    int32_t y = 0;
};

}
