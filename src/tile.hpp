
#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include "defines.hpp"

namespace hbx
{

class item;
class unit;

class tile
{
public:
    tile();
    ~tile();


    bool IsBuild()		const { return (m_attribute & 0x10) ? true : false; }
    bool IsFarm()		const { return (m_attribute & 0x20) ? true : false; }
    bool IsTele()		const { return (m_attribute & 0x40) ? true : false; }
    bool IsWater()		const { return (m_attribute & 0x08) ? true : false; }
    bool IsMovable()	const { return (m_attribute & 0x80 || !m_bIsTempMoveAllowed) ? false : true; }
    void IsMovable(bool b) { m_attribute &= 0x80 | (b << 7); }
    bool IsTempMovable()	const { return m_bIsTempMoveAllowed; }
    void IsTempMovable(bool b) { m_bIsTempMoveAllowed = b; }

    owner_type  m_cOwnerType;		// OT_PLAYER / OT_NPC
    owner_type  m_cDeadOwnerType;
    std::shared_ptr<unit> owner;
    std::shared_ptr<unit> deadowner;

    std::vector<item *> m_pItem;

    uint16_t  m_wDynamicObjectID;
    uint16_t m_sDynamicObjectType;
    uint64_t m_dwDynamicObjectRegisterTime;

    uint8_t  m_bIsMoveAllowed, m_bIsTeleport, m_bIsWater, m_bIsTempMoveAllowed;
    uint8_t  m_bIsFarmingAllowed;


    uint8_t   m_iOccupyStatus;
    uint8_t   m_iOccupyFlagIndex;
    // Crusade
    uint8_t	  m_attribute;

    int8_t m_cOwnerClass;
    int16_t m_sOwner;

    int8_t m_cDeadOwnerClass;
    int16_t m_sDeadOwner;

    std::vector<item *> items;

/*
    bool move_allowed;
    bool teleport;
    bool water;
    bool temp_move_allowed;
    bool farming_allowed;

    int32_t occupy_status;
    int32_t occupy_flag_index;

    int32_t	attribute;
*/
};

}
