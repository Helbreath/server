
#pragma once

#include <stdint.h>
#include <string>
#include "defines.hpp"

namespace hbx
{

class CMagic
{
public:
    CMagic() = default;
    ~CMagic() = default;

    //char m_cName[31];
    std::string name;

    int16_t type = 0;
    uint32_t delay_time = 0;
    uint32_t last_time = 0;
    int16_t value1 = 0;
    int16_t value2 = 0;
    int16_t value3 = 0;
    int16_t value4 = 0;
    int16_t value5 = 0;
    int16_t value6 = 0;
    int16_t value7 = 0;
    int16_t value8 = 0;
    int16_t value9 = 0;
    int16_t value10 = 0;
    int16_t value11 = 0;
    int16_t value12 = 0;
    int16_t int_limit = 0;
    int32_t gold_cost = 0;

    int8_t category = 0;
    int32_t attribute = 0;
};

}
