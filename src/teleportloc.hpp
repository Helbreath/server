
#pragma once

#include <cstdint>
#include <string>

namespace hbx
{

class teleport_loc
{
public:
    teleport_loc() = default;
    ~teleport_loc() = default;

    int16_t source_x = -1;
    int16_t source_y = -1;

    std::string dest_map_name;
    std::string dest_map_name2;

    uint8_t direction = 0;

    int16_t dest_x = -1;
    int16_t dest_y = -1;
    int16_t dest_x2 = -1;
    int16_t dest_y2 = -1;
    int32_t v1 = 0;
    int32_t	v2 = 0;
    int32_t num_summon_npc = 0;
    uint32_t time = 0;
    uint32_t time2 = 0;

};

}
