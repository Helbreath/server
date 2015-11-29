
#pragma once

#include "funcs.h"
#include "connection.h"
#include "common.h"


class Item;
class Unit;

class Tile  
{												  
public:
	Tile();
	~Tile();

	bool IsBuild()		const { return (m_attribute & 0x10) ? true : false; }
	bool IsFarm()		const { return (m_attribute & 0x20) ? true : false; }
	bool IsTele()		const { return (m_attribute & 0x40) ? true : false; }
	bool IsMovable()	const { return (m_attribute & 0x80 || !m_bIsTempMoveAllowed) ? false : true; }

	uint8_t  m_cOwnerType;		// OT_PLAYER / OT_NPC
	uint8_t  m_cDeadOwnerType;	
	boost::shared_ptr<Unit> owner;
	boost::shared_ptr<Unit> deadowner;

	std::vector<Item *> m_pItem;

	uint16_t  m_wDynamicObjectID;
	short m_sDynamicObjectType;
	uint64_t m_dwDynamicObjectRegisterTime;

	uint8_t  m_bIsMoveAllowed, m_bIsTeleport, m_bIsWater, m_bIsTempMoveAllowed;
	bool  m_bIsFarmingAllowed; 


	int   m_iOccupyStatus;    
	int   m_iOccupyFlagIndex; 
	// Crusade
	uint8_t	  m_attribute;		  

};

