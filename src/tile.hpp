
#pragma once

#include "item.hpp"
#include <cstdint>
#include <vector>

namespace hbx
{

class tile
{
public:
    tile() = default;
    ~tile() = default;

    int8_t m_cOwnerClass;
    int16_t m_sOwner;

    int8_t m_cDeadOwnerClass;
    int16_t m_sDeadOwner;

    std::vector<item*> items;

    uint16_t m_wDynamicObjectID;
    int16_t m_sDynamicObjectType;
    uint32_t m_dwDynamicObjectTime;

    bool move_allowed;
    bool teleport;
    bool water;
    bool temp_move_allowed;
    bool farming_allowed;

    int32_t occupy_status;
    int32_t occupy_flag_index;

    int32_t	attribute;
};

}
