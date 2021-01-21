#pragma once

#include <cstdint>
#include <string>
#include "defines.hpp"
#include "utility.hpp"
#include "unit.hpp"

namespace hbx
{

class npc_item
{
public:
	npc_item() = default;
	~npc_item() = default;

	std::string name;
	int16_t m_sItemID;
	int16_t m_sFirstProbability;
	int8_t m_cFirstTargetValue;
	int16_t m_sSecondProbability;
	int8_t m_cSecondTargetValue;
};

class guild;
class item;

class npc : public unit
{
public:

	npc(uint64_t NpcH, map * map_);
	~npc() = default;


    std::shared_ptr<npc> sft() { return std::static_pointer_cast<npc>(shared_from_this()); }

    bool initNpcAttr(npc * npcTemplate, char cSA);
    void Behave();
    bool behavior_searchMaster();

    void behavior_death(std::shared_ptr<unit> attacker, int16_t dmg);
    void behavior_move();
    void behavior_attack();
    void behavior_stop();
    void behavior_dead();
    bool behavior_manaCollector();
    void behavior_grandMagicGenerator();
    bool behavior_detector();
    void behavior_flee();
    void nextWaypointDest();
    uint8_t	GetNextMoveDir(short sX, short sY, short dstX, short dstY, map * pMap, char cTurn, int * pError, short * DOType);
    uint8_t GetNextMoveDir(short sX, short sY, short dstX, short dstY, map * pMap, char cTurn, int * pError);
    //Unit * TargetSearch(uint8_t dX, uint8_t dY, uint8_t Radius);
    std::shared_ptr<unit> TargetSearch();
    int getDangerValue(short dX, short dY);
    void magicHandler(std::shared_ptr<unit> unit, short dX, short dY, short magicType);
    int GetGenLevel();

    void Cast(std::shared_ptr<unit> target, short spell);
    void Cast(short x, short y, short spell);

    bool Follow(std::shared_ptr<unit> master);
    void ReleaseFollowMode(std::shared_ptr<unit> owner);
    void RemoveFromTarget(std::shared_ptr<unit> target, int iCode = 0);
    void SetTarget(std::shared_ptr<unit> target, bool isperm = true);

    virtual void RegenHP();
    virtual void RegenMP();

    bool IsHighlyTrained() const { return (specialAbility == 9); }
    bool IsElite() const { return (specialAbility == 10); }
    bool IsBoss() const { return (specialAbility == 11); }

    void AddHP(uint64_t value);
    void ReduceHP(uint64_t value);

	std::string name;
	map * map_ = nullptr;
	short m_dX = 0, m_dY = 0;
	short m_vX = 0, m_vY = 0;
	int32_t m_tmp_iError = 0;
	rect roamArea;

	int8_t action = 0;
	int8_t turn = 0;

    uint64_t timeLastAction;
    uint64_t timeActionInterval;
    uint64_t timeHealth, timeMana;//TODO: remove hp regen during combat and instead make it only regen outside combat?
    uint64_t timeDead, timeRegen;
    
	//TODO: completely revamp dice system on NPCs. Instead give ranges like actually makes sense
	// int minDamage = 50; int maxDamage = 80; easier to read than 5D20+30 - HB is not D&D.
	int32_t m_iHitDice = 0;//hitpoint regen dice
	int32_t m_iDefenseRatio = 0;
	int32_t m_iHitRatio = 0;//attack hit chance
	int32_t m_iMagicHitRatio = 0;
	int32_t m_iMinBravery = 0;
	int32_t m_iExpDice = 0;
    uint64_t goldDropValue;

	int8_t m_cActionLimit = 0;

	int8_t m_cSize = 0;
	int8_t m_cAttackDiceThrow = 0;
	int8_t m_cAttackDiceRange = 0;
	int8_t m_cAttackBonus = 0;
	int8_t m_cBravery = 0;
	uint16_t m_cResistMagic = 0;
	int8_t m_cMagicLevel = 0;
	int8_t m_cDayOfWeekLimit = 0;
	int8_t m_cChatMsgPresence = 0;

    int32_t m_iMaxMana;

	int8_t m_cMoveType = 0;
	int8_t m_cBehavior = 0;
	int16_t m_sBehaviorTurnCount = 0;
	int8_t m_cTargetSearchRange = 0;

    std::shared_ptr<unit> follow;

	int32_t m_iFollowOwnerIndex = 0;
	int8_t m_cFollowOwnerType = 0;
	bool m_bIsSummoned = false;
	uint32_t m_dwSummonedTime = 0;

    bool summoned = false;
    uint64_t timeSummoned = 0;
	int8_t targetType = 0;
	int8_t followOwnerType = 0;
    std::shared_ptr<unit> target;
    uint8_t m_cCurWaypoint = 0;
    uint8_t m_cTotalWaypoint = 0;

	int32_t m_iSpotMobIndex = 0;
	int32_t m_iWayPointIndex[MAXWAYPOINTS + 1];

	bool permAttackMode = false;
	int32_t m_iNoDieRemainExp = 0;
	int32_t m_iAttackStrategy = 0;
	int32_t m_iAILevel = 1;

	int32_t attackRange = 1;
	/*
        AI-Level
        1: normal behavior
        2: Track the lowest energy target
        3: Targets under magic protection are excluded from magic attack targets
	*/
	int32_t m_iAttackCount = 0;

	int32_t m_iLastDamage = 0;
	int32_t summonControlMode = 0;
	Element element = ELEMENT_NONE;
	int32_t absDamage = 0;

	int32_t m_iItemRatio = 0;
	int32_t m_iAssignedItem = 0;

	int8_t specialAbility = 0;

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

	int32_t	  buildCount = 0;
	int32_t   m_iManaStock = 0;
	bool  isLeader = false;

    //TODO: turn this into something more readable and perhaps useful
    int32_t m_iV1 = 0;//Grand Magic Generator damage count and ManaStone mana count

	std::vector<npc_item> m_vNpcItem;

	int32_t m_iNpcitemType = 0;
	int32_t npcItemMax = 0;
	int32_t m_iNpcCrops = 0;
	int32_t m_iCropsSkillLV = 0;

    std::shared_ptr<guild> guild_;

};

}
