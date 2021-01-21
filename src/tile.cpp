
#include "tile.hpp"
#include "item.hpp"
#include "utility.hpp"

namespace hbx
{

tile::tile()
//: m_pItem(ITEMS_PER_TILE)
{
    m_bIsMoveAllowed = true;
    m_bIsTeleport = false;
    m_bIsWater = false;
    m_bIsFarmingAllowed = false;

    owner = nullptr;
    m_cOwnerType = ot_none;

    deadowner = nullptr;
    m_cDeadOwnerType = ot_none;

    m_wDynamicObjectID = 0;
    m_sDynamicObjectType = 0;

    m_bIsTempMoveAllowed = true;

    m_iOccupyStatus = 0;
    m_iOccupyFlagIndex = 0;

    m_attribute = 0;
    tilecount++;
}

tile::~tile()
{
    for (item * item_ : m_pItem)
    {
        delete item_;
    }
}

}
