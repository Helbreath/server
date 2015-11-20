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

	bool IsDead() const { return _dead; }
	bool IsBerserked()	const { return magicEffectStatus[MAGICTYPE_BERSERK] ? true : false; }
	bool IsInvisible()	const { return magicEffectStatus[MAGICTYPE_INVISIBILITY] ? true : false; }

	bool IsPlayer()	const	{ return (_ownerType == OWNERTYPE_PLAYER) ? true : false; }//TODO: go through old code and update
	bool IsNPC() 		const	{ return (_ownerType == OWNERTYPE_NPC) ? true : false; }
	bool IsNeutral()	const	{ return (side == NEUTRAL) ? true : false; }
	bool IsAres()		const	{ return (side == ARESDEN) ? true : false; }
	bool IsElv()		const	{ return (side == ELVINE) ? true : false; }

	void SetOwnerType(int8_t type) { _ownerType = type; }
	void SetType(int8_t type) { _type = type; }
	void SetTypeOriginal(int8_t type) { _typeOriginal = type; }
	int8_t OwnerType() { return _ownerType; }
	int8_t Type() { return _type; }
	int8_t TypeOriginal() { return _typeOriginal; }

	virtual float GetDropFactor()		const { return 1.0f; }


	Map * map;
	uint64_t handle;
	
	//stick in stats struct or something?
	int16_t x, y;
	int64_t health;
	int64_t mana;
	uint64_t experience;
	Side side;
	uint32_t status;
	bool _dead;
	shared_ptr<Unit> killer;
	int8_t direction;



	string name;

	int8_t  magicEffectStatus[MAXMAGICEFFECTS];

	uint64_t guildGUID;

	uint64_t m_uid;
	static uint64_t GenUID(){
		static uint64_t uids = 0;
		return uids++;
	}

private:
	//confusing Helbreath names amuck!
	int8_t _type;//model type
	int8_t _typeOriginal;//original model type
	int8_t _ownerType;//whether is player or npc
};
#endif
