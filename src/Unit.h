#ifndef UNIT_H
#define UNIT_H
#pragma once

#include "Magic.h"
#include "common.h"

class Client;
class Npc;
class Map;
class GServer;

enum OwnerType{
	OWNERTYPE_NONE,
	OWNERTYPE_PLAYER,
	OWNERTYPE_NPC,
	OWNERTYPE_PLAYER_INDIRECT,
	OWNERTYPE_NPC_INDIRECT
};

class Unit
{
public:
	Unit(void);
	virtual ~Unit(void);
	void SetStatusFlag(long flag, bool enabled);
	void ToggleStatusFlag(long flag);
	bool GetStatusFlag(long flag) const;
	void SetMagicFlag(int16_t magicType, bool enabled);
	bool AddMagicEffect(int16_t magicType, uint64_t effectTime, int8_t kind = 1);
	bool RemoveMagicEffect(int8_t magicType);
	void SetSideFlag(Side side);

	bool IsDead()			const { return (m_iHP > 0) ? false : true; }
	bool IsBerserked()	const { return m_cMagicEffectStatus[MAGICTYPE_BERSERK] ? true : false; }
	bool IsInvisible()	const { return m_cMagicEffectStatus[MAGICTYPE_INVISIBILITY] ? true : false; }

	bool IsPlayer()	const	{ return (m_ownerType == OWNERTYPE_PLAYER) ? true : false; }//TODO: go through old code and update
	bool IsNPC() 		const	{ return (m_ownerType == OWNERTYPE_NPC) ? true : false; }
	bool IsNeutral()	const	{ return (m_side == NEUTRAL) ? true : false; }
	bool IsAres()		const	{ return (m_side == ARESDEN) ? true : false; }
	bool IsElv()		const	{ return (m_side == ELVINE) ? true : false; }

	virtual float GetDropFactor()		const { return 1.0f; }


	GServer * gserver;
	uint64_t m_handle;
	
	int16_t m_sX, m_sY;
	uint64_t m_iHP;
	uint64_t m_iMP;
	uint64_t  m_iExp;
	Side m_side;
	uint32_t m_iStatus;
	bool m_bIsKilled;
	shared_ptr<Unit> m_killer;
	Map * pMap;

	int8_t  m_cDir;
	int8_t m_sType;
	int8_t m_sOriginalType;
	int8_t m_ownerType;

	string name;

	int8_t  m_cMagicEffectStatus[MAXMAGICEFFECTS];

	uint64_t m_iGuildGUID;

	uint64_t m_uid;
	static uint64_t GenUID(){
		static uint64_t uids = 0;
		return uids++;
	}
};
#endif
