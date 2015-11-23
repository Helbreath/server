
#pragma once

#include "common.h"
#include "Magic.h"
#include "Unit.h"

#define MAXWAYPOINTS			10

#define MOVETYPE_STOP			0
#define MOVETYPE_SEQWAYPOINT	1
#define MOVETYPE_RANDOMWAYPOINT	2
#define MOVETYPE_FOLLOW			3
#define MOVETYPE_RANDOMAREA		4
#define MOVETYPE_RANDOM			5
#define MOVETYPE_GUARD			6

#define BEHAVIOR_STOP			0
#define BEHAVIOR_MOVE			1
#define BEHAVIOR_ATTACK			2
#define BEHAVIOR_FLEE			3
#define BEHAVIOR_DEAD			4


enum NPCType{
	NPC_INVALID = -1,
	NPC_SLIME = 10,
	NPC_RABBIT = 55,
	NPC_CAT = 56,
	NPC_GIANT_ANT = 16,
	NPC_AMPHIS = 22,
	NPC_ORC = 14,
	NPC_ZOMBIE = 18,
	NPC_SCORPION = 17,
	NPC_SKELETON = 11,
	NPC_ORC_MAGE = 14,
	NPC_CLAY_GOLEM = 23,
	NPC_STONE_GOLEM = 12,
	NPC_HELLBOUND = 27,
	NPC_GIANT_FROG = 57,
	NPC_CYCLOPS = 13,
	NPC_TROLL = 28,
	NPC_BEHOLDER = 53,
	NPC_CANNIBAL_PLANT = 60,
	NPC_OGRE = 29,
	NPC_MOUNTAIN_GIANT = 58,
	NPC_LICHE = 30,
	NPC_STALKER = 48,
	NPC_WEREWOLF = 33,
	NPC_DARK_ELF = 54,
	NPC_ETTIN = 59,
	NPC_DEMON = 31,
	NPC_UNICORN = 32,
	NPC_GAGOYLE = 52,
	NPC_HELLCLAW = 49,
	NPC_TIGERWORM = 50,
	NPC_RUDOLPH = 61,
	NPC_DIREBOAR = 62,
	NPC_FROST = 63,
	NPC_ICE_GOLEM = 65,
	NPC_WYVERN = 66,
	NPC_BARLOG = 70,
	NPC_CENTAURUS = 71,
	NPC_CLAW_TURTLE = 72,
	NPC_FIRE_WYVERN = 73,
	NPC_GIANT_CRAYFISH = 74,
	NPC_GIANT_LIZARD = 75,
	NPC_GIANT_PLANT = 76,
	NPC_MASTERMAGE_ORC = 77,
	NPC_MINOTAURS = 78,
	NPC_NIZIE = 79,
	NPC_TENTOCLE = 80,
	NPC_ABADDON = 81,
	NPC_CROPS = 64,

	NPC_MCGAFFIN = 67,
	NPC_PERRY = 68,
	NPC_DEVLIN = 69,

	NPC_DUMMY = 34,
	NPC_ATTACK_DUMMY = 34,
	NPC_ENERGY_SPHERE = 35,

	NPC_SHOPKEEPER_W = 15,
	NPC_GANDLF = 19,
	NPC_HOWARD = 20,

	NPC_GUARD = 21,

	NPC_TOM = 24,
	NPC_WILLIAM = 25,
	NPC_KENNEDY = 26,

	NPC_AGT = 36,
	NPC_CGT = 37,
	NPC_MS = 38,
	NPC_DT = 39,
	NPC_ESG = 40,
	NPC_GMG = 41,
	NPC_MANASTONE = 42,

	NPC_LWB = 43,
	NPC_GHK = 44,
	NPC_GHKABS = 45,
	NPC_TK = 46,
	NPC_BG = 47,

	NPC_YB = 1,
	NPC_CP = 51,


	NPC_GAIL = 90,

	NPC_SOR = 82,
	NPC_ATK = 83,
	NPC_ELF = 84,
	NPC_DSK = 85,
	NPC_HBT = 86,
	NPC_CT = 87,
	NPC_BAR = 88,
	NPC_AGC = 89,
	
	NPC_GATE = 91,
	NPC_GATE_A = 91,
	NPC_GATE_E = 91,

	NPC_SCARECROW = 92,

	NPC_GHOST = 93,
	NPC_VAMPIRE = 94,

	NPC_FIRE_PIXIE = 95,
	NPC_ICE_PIXIE = 96,
	NPC_EARTH_PIXIE = 97,
	NPC_AIR_PIXIE = 98,

	NPC_GUARDB_ARESDEN = 99,
	NPC_GUARDB_ELVINE = 100,
	NPC_WILLIAMB = 101,
	NPC_PRINCESS = 102,
	NPC_AS = 103,
	//NPC_ORCLORD = 103,
	NPC_DARKUNICORN = 104,
	NPC_BLACK_DEMON = 105,
	NPC_EARTH_WYVERN = 106,
	NPC_WIND_WYVERN = 107,
	NPC_DARK_WYVERN = 108,
	NPC_GRASS_WYVERN = 109,
	NPC_SKY_WYVERN = 110,
	NPC_SEA_WYVERN = 111,
	NPC_METAL_WYVERN = 112,
	NPC_VOID_WYVERN = 113,
	NPC_ETERNAL_DRAGON = 114,
	/*NPC_MOTHER_DRAGON = 115,
	NPC_HELL_DRAGON = 116,
	NPC_ALLIGATOR = 117,
	NPC_PUMA = 118,
	NPC_ZOMBIE_KING = 119,
	NPC_SLIME_KING = 120,
	NPC_SLIME_PRINESS = 121,*/
	NPC_ENRAGED_TROLL = 122,
	NPC_ENRAGED_CYCLOPS = 123,
	NPC_ENRAGED_STALKER = 124,
	NPC_ENRAGED_GAGOYLE = 125,
	NPC_ENRAGED_HELLCLAW = 126,
	NPC_ENRAGED_TIGERWORM = 127
};



class Npc : public Unit
{
public:

	Npc(uint64_t NpcH, Map * map);
	~Npc();

	weak_ptr<Npc> self;

	bool initNpcAttr(Npc * npcTemplate, char cSA);
	void Behave();
	bool behavior_searchMaster();

	virtual void behavior_move();
	void behavior_stop();
	virtual void behavior_attack();
	virtual void behavior_death(shared_ptr<Unit> attacker, int16_t dmg);
	void behavior_dead();
	bool behavior_manaCollector();
	void behavior_grandMagicGenerator();
	bool behavior_detector();
	void behavior_flee();
	void nextWaypointDest();
	uint8_t	GetNextMoveDir(short sX, short sY, short dstX, short dstY, Map* pMap, char cTurn, int * pError, short * DOType);
	uint8_t GetNextMoveDir(short sX, short sY, short dstX, short dstY, Map* pMap, char cTurn, int * pError);
	//Unit * TargetSearch(uint8_t dX, uint8_t dY, uint8_t Radius);
	shared_ptr<Unit> TargetSearch();
	int getDangerValue(short dX, short dY);
	void magicHandler(Unit * unit, short dX, short dY, short magicType);
	int GetGenLevel();

	void Cast(shared_ptr<Unit> target, short spell);
	void Cast(short x, short y, short spell);

	bool Follow(shared_ptr<Unit> master);
	void ReleaseFollowMode(shared_ptr<Unit> owner);
	void RemoveFromTarget(shared_ptr<Unit> target, int iCode = 0);
	void SetTarget(shared_ptr<Unit> target, bool isperm = true);

	virtual void RegenHP();
	virtual void RegenMP();
	
	bool IsHighlyTrained()	const { return (specialAbility == 9); }
	bool IsElite()				const { return (specialAbility == 10); }
	
	void AddHP(uint64_t value);
	void ReduceHP(uint64_t value);

	short m_dX, m_dY;
	short m_vX, m_vY;
	int   m_tmp_iError;
	rect  roamArea;

	char  action;
	char  turn;

	short appr2;

	uint64_t timeLastAction;
	uint64_t timeActionInterval;
	uint64_t timeHealth, timeMana;//TODO: remove hp regen during combat and instead make it only regen outside combat?
	uint64_t timeDead, timeRegen;

	//TODO: completely revamp dice system on NPCs. Instead give ranges like actually makes sense
	// int minDamage = 50; int maxDamage = 80; easier to read than 5D20+30 - HB is not D&D.
	int  m_iHitDice;//hitpoint regen dice
	int  m_iDefenseRatio;
	int  m_iHitRatio;//attack hit chance
	int  m_iMagicHitRatio;
	int  m_iMinBravery;
	int  m_iExpDice;
	uint64_t goldDropValue;

	char m_cActionLimit;

	char m_cSize;
	char m_cAttackDiceThrow;
	char m_cAttackDiceRange;
	char m_cAttackBonus;
	char m_cBravery;
	uint16_t m_cResistMagic;
	char m_cMagicLevel;
	char m_cDayOfWeekLimit;		    
	char m_cChatMsgPresence;     
	int  m_iMaxMana;
																    
	char  m_cMoveType;
	char  m_cBehavior;
	short m_sBehaviorTurnCount;
	char  m_cTargetSearchRange;

	shared_ptr<Unit> follow;
	bool  summoned;            
	uint64_t timeSummoned;
	char  targetType;
	char  followOwnerType;
	shared_ptr<Unit> target;
	uint8_t  m_cCurWaypoint;
	uint8_t  m_cTotalWaypoint;

	int   m_iSpotMobIndex;
	int   m_iWayPointIndex[MAXWAYPOINTS+1];

	bool  permAttackMode;		   
	int   m_iNoDieRemainExp;
	int   m_iAttackStrategy; 
	int   m_iAILevel;

	int   attackRange;
	/*
		AI-Level 
			1: 보통 행동 
			2: 에너지가 가장 낮은 목표물을 추적 
			3: 마법 보호중인 목표물 마법 공격대상에서 제외 
	*/
	int   m_iAttackCount;


	int   m_iLastDamage;
	int   summonControlMode;
	Element element;
	int   absDamage;

	int   m_iItemRatio;
	int   m_iAssignedItem;
	char  specialAbility;

									/*
case 0: break;
case 1:  "Penetrating Invisibility"
case 2:  "Breaking Magic Protection"
case 3:  "Absorbing Physical Damage"
case 4:  "Absorbing Magical Damage"
case 5:  "Poisonous"
case 6:  "Extremely Poisonous"
case 7:  "Explosive"
case 8:  "Hi-Explosive" 

									*/

	int	  buildCount;

	bool  isLeader;

	//TODO: turn this into something more readable and perhaps useful
	int   m_iV1;//Grand Magic Generator damage count and ManaStone mana count

	// 2002-09-17 #1
	int		m_iNpcitemType;

	int		npcItemMax;

	int		m_iNpcCrops;
	int		m_iCropsSkillLV;
};

