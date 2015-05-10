
#include "Tile.h"
#include "Item.h"


Tile::Tile()
	//: m_pItem(ITEMS_PER_TILE)
{ 
	m_bIsMoveAllowed = true;
	m_bIsTeleport    = false;
	m_bIsWater       = false;
	m_bIsFarmingAllowed = false; 

	owner = nullptr;
	m_cOwnerType = 0;

	deadowner = nullptr;
	m_cDeadOwnerType = 0;

	m_wDynamicObjectID   = 0;
	m_sDynamicObjectType = 0;

	m_bIsTempMoveAllowed = true;

	m_iOccupyStatus    = 0;
	m_iOccupyFlagIndex = 0;

	m_attribute  = 0;
}

Tile::~Tile()
{
	for (Item * item : m_pItem)
	{
		delete item;
	}
}
