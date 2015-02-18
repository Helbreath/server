#include "Unit.h"
#include "GServer.h"
#include "netmessages.h"
// #include "ActionID.h"
// #include "DelayEvent.h"
// #include "maths.h"

Unit::Unit(void)
{
	m_uid = GenUID();
	m_bIsKilled   = false;
	m_killer = 0;
	m_sOriginalType      = 0;
	m_iStatus    = 0;
	gserver = 0;
}

Unit::~Unit(void)
{
}

void Unit::SetStatusFlag(long flag, bool enabled)
{
	if (enabled) 
		m_iStatus |= flag;
	else 
		m_iStatus &= ~flag;

	gserver->SendEventToNearClient_TypeA(this, MSGID_MOTION_NULL, 0, 0, 0);
}

void Unit::ToggleStatusFlag(long flag)
{
	m_iStatus ^= flag;	
	
	gserver->SendEventToNearClient_TypeA(this, MSGID_MOTION_NULL, 0, 0, 0);
}

bool Unit::GetStatusFlag(long flag) const
{
	return (m_iStatus & flag) ? true : false;
}

void Unit::SetMagicFlag(int16_t magicType, bool enabled)
{
	long flag = 0;

	switch(magicType)
	{
		case MAGICTYPE_INVISIBILITY:
			flag = STATUS_INVISIBILITY;
			break;
		case MAGICTYPE_ICE:
		case MAGICTYPE_ICE_LINEAR:
		case MAGICTYPE_MB_LINEAR:
		case MAGICTYPE_COTG_LINEAR:
			flag = STATUS_FROZEN;
			break;
		case MAGICTYPE_BERSERK:
			flag = STATUS_BERSERK;
			break;
		case MAGICTYPE_PROTECT:
			switch(m_cMagicEffectStatus[magicType])
			{
			case MAGICPROTECT_PFA:
				flag = STATUS_PFA;
				break;
			case MAGICPROTECT_PFM:
			//case MAGICPROTECT_GPFM:
			case MAGICPROTECT_AMP:
				flag = STATUS_PFM;
				break;
			case MAGICPROTECT_DS:
			case MAGICPROTECT_GDS:
				flag = STATUS_DEFENSESHIELD;
				break;
			}
			break;
		case MAGICTYPE_CONFUSE:
			switch (m_cMagicEffectStatus[magicType])
			{
			case 1:	// Language confuse
				break;
			case 2:	// Confusion
				break;
			case 3:
				flag = STATUS_ILLUSION;
				break;
			case 4:	
				flag = STATUS_ILLUSIONMOVEMENT;
				break;
			}
			break;
		default:
			// No flag
			return;
	}

	SetStatusFlag(flag, enabled);
}

bool Unit::AddMagicEffect(int16_t magicType, uint64_t effectTime, int8_t kind)
{
	//char magicType = spell->m_sType;
	uint64_t  dwTime = unixtime();

	if(m_cMagicEffectStatus[magicType])
		return false;

	m_cMagicEffectStatus[magicType] = kind;

// 	if (effectTime > 0)
// 		gs->RegisterDelayEvent(DELAYEVENTTYPE_MAGICRELEASE, magicType, dwTime + (effectTime)_s, 
// 			this, 0, 0, 0, kind, 0, 0);
	
	if (IsPlayer())
		gserver->SendNotifyMsg(0, static_cast<Client*>(this), NOTIFY_MAGICEFFECTON, magicType, kind, 0, 0);

	SetMagicFlag(magicType, true);
 	return true;
}

bool Unit::RemoveMagicEffect(int8_t magicType)
{
// 	if(!m_cMagicEffectStatus[magicType])
// 		return false;
// 
// 	if (IsPlayer())
// 		gserver->SendNotifyMsg(NULL, m_handle, NOTIFY_MAGICEFFECTOFF, magicType, m_cMagicEffectStatus[magicType], NULL, NULL);
// 
// 	SetMagicFlag(magicType, false);
// 	m_cMagicEffectStatus[magicType] = NULL;
// 	gserver->RemoveFromDelayEventList(this, magicType);
 	return true;
}

void Unit::SetSideFlag(Side side)
{
//	SetNibble(m_iStatus, 7, side);
}
