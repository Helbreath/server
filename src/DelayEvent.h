// DelayEvent.h: interface for the CDelayEvent class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <stdint.h>
#include "funcs.h"

#define DELAYEVENTTYPE_DAMAGEOBJECT				1
#define DELAYEVENTTYPE_MAGICRELEASE				2
#define DELAYEVENTTYPE_USEITEM_SKILL			3
#define DELAYEVENTTYPE_METEORSTRIKE				4
#define DELAYEVENTTYPE_DOMETEORSTRIKEDAMAGE		5
#define DELAYEVENTTYPE_CALCMETEORSTRIKEEFFECT	6
#define DELAYEVENTTYPE_ANCIENT_TABLET			7

#define DELAYEVENTTYPE_END_APOCALYPSE			8
#define DELAYEVENTTYPE_KILL_ABADDON				9

class Map;
class Unit;

class DelayEvent  
{										  
public:

	DelayEvent();
	virtual ~DelayEvent();

	int32_t m_iDelayType;
	int32_t m_iEffectType;

	Map * m_pMap;
	int32_t m_dX, m_dY;

	Unit * target;
	
	int32_t m_iV1, m_iV2, m_iV3;

	uint32_t m_dwTriggerTime;
};

