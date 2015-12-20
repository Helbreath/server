#include "Unit.h"
#include "GServer.h"
#include "netmessages.h"
#include "Map.h"
#include "Npc.h"
// #include "ActionID.h"
// #include "DelayEvent.h"
// #include "maths.h"

Unit::Unit(void)
{
	m_uid = GenUID();
	_dead   = false;
	killer = 0;
	_typeOriginal      = 0;
	status    = 0;
	map = 0;
}

Unit::~Unit(void)
{
}

void Unit::SetStatusFlag(long flag, bool enabled)
{
	if (enabled) 
		status |= flag;
	else 
		status &= ~flag;

	map->gserver->SendEventToNearClient_TypeA(this, MSGID_MOTION_NULL, 0, 0, 0);
}

void Unit::ToggleStatusFlag(long flag)
{
	status ^= flag;	
	
	map->gserver->SendEventToNearClient_TypeA(this, MSGID_MOTION_NULL, 0, 0, 0);
}

bool Unit::GetStatusFlag(long flag) const
{
	return (status & flag) ? true : false;
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
			switch(magicEffectStatus[magicType])
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
			switch (magicEffectStatus[magicType])
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

	if(magicEffectStatus[magicType])
		return false;

	magicEffectStatus[magicType] = kind;

// 	if (effectTime > 0)
// 		gs->RegisterDelayEvent(DELAYEVENTTYPE_MAGICRELEASE, magicType, dwTime + (effectTime)_s, 
// 			this, 0, 0, 0, kind, 0, 0);
	
	if (IsPlayer())
		map->gserver->SendNotifyMsg(nullptr, GetPlayer(), NOTIFY_MAGICEFFECTON, magicType, kind, 0);

	SetMagicFlag(magicType, true);
 	return true;
}

bool Unit::RemoveMagicEffect(int8_t magicType)
{
	if (!magicEffectStatus[magicType])
		return false;

	if (IsPlayer())
		map->gserver->SendNotifyMsg(nullptr, GetPlayer(), NOTIFY_MAGICEFFECTOFF, magicType, magicEffectStatus[magicType], 0);

	SetMagicFlag(magicType, false);
	magicEffectStatus[magicType] = 0;
	map->gserver->RemoveFromDelayEventList(this, magicType);
 	return true;
}

void Unit::SetSideFlag(Side side)
{
//	SetNibble(m_iStatus, 7, side);
}

void Unit::SetPoison(uint32_t level, uint64_t time)
{
	if (_poisonLevel == 0 || _poisonTime == 0)
	{
		SetStatusFlag(STATUS_POISON, false);
		_poisonLevel = 0;
		_poisonTime = 0;
		return;
	}
	SetStatusFlag(STATUS_POISON, true);
	_poisonLevel = level;
	_poisonTime = time;
}

bool Unit::CheckResistMagic(char direction, int hitratio)
{
	int targetMagicResistRatio = 0;
	if (IsPlayer())
	{
		Client * player = GetPlayer();
		if (player->IsInvincible()) return true;

		targetMagicResistRatio = player->m_cSkillMastery[SKILL_MAGICRES] + player->m_iAddMR;
		if (player->GetMag() > 50)
			targetMagicResistRatio += (player->GetMag() - 50);

		targetMagicResistRatio += player->m_iAddResistMagic;
		if ((player->status & STATUS_REDSLATE) != 0) return true;
	}
	else
	{
		targetMagicResistRatio = GetNpc()->m_cResistMagic;
	}

	if (magicEffectStatus[MAGICTYPE_PROTECT] == MAGICPROTECT_AMP) return true;

	if (magicEffectStatus[MAGICTYPE_PROTECT] == MAGICPROTECT_PFM && hitratio < 1000) return true;

	if (hitratio >= 10000) hitratio -= 10000;

	if (targetMagicResistRatio < 1) targetMagicResistRatio = 1;

	hitratio = (hitratio / targetMagicResistRatio) / 50.0f;

	if (hitratio < MINIMUMHITRATIO) hitratio = MINIMUMHITRATIO;
	if (hitratio > MAXIMUMHITRATIO) hitratio = MAXIMUMHITRATIO;

	if (dice(1, 100) <= hitratio) return false;

	if (IsPlayer()) GetPlayer()->CalculateSSN_SkillIndex(SKILL_MAGICRES, 1);
	return true;
}

bool Unit::CheckResistPoison()
{
	int iResist, iResult;

	if (IsPlayer())
	{
		Client * client = GetPlayer();
		iResist = client->m_cSkillMastery[SKILL_POISONRES] + client->m_iAddPR;
	}
	else if (IsNPC())
	{
		iResist = 0;
	}
	else
	{
		return true;
	}

	iResult = dice(1, 100);
	if (iResult >= iResist)
		return false;

	if (IsPlayer())
		GetPlayer()->CalculateSSN_SkillIndex(SKILL_POISONRES, 1);

	return true;
}

bool Unit::CheckResistIce(char direction, int iHitRatio)
{
	int    iTargetIceResistRatio, iResult;

	if (IsPlayer())
	{
		Client * client = GetPlayer();

		if (client->IsInvincible()) return true;

		iTargetIceResistRatio = client->IceResist();

		if (client->m_dwWarmEffectTime == 0) {
		}
		else if ((unixtime() - client->m_dwWarmEffectTime) < 1000 * 30) return true;
	}
	else if (IsNPC())
	{
		Npc * npc = GetNpc();
		if (npc->element == ELEMENT_WATER) return true;
		iTargetIceResistRatio = (npc->m_cResistMagic) - (npc->m_cResistMagic / 3);
	}
	else
	{
		return true;
	}

	if (iTargetIceResistRatio < 1) iTargetIceResistRatio = 1;

	iResult = dice(1, 100);
	if (iResult <= iTargetIceResistRatio) return true;

	return false;
}
