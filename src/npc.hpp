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

class npc : public unit
{
public:

	npc(std::string pName)
		: name(pName)
	{
        for (int i = 0; i < DEF_MAXWAYPOINTS; i++)
            m_iWayPointIndex[i] = -1;

        for (int i = 0; i < DEF_MAXMAGICEFFECTS; i++)
            m_cMagicEffectStatus[i] = 0;
	}
	~npc() = default;

	std::string name;
	short m_dX = 0, m_dY = 0;
	short m_vX = 0, m_vY = 0;
	int   m_tmp_iError = 0;
	rect  m_rcRandomArea;

	char  m_cAction = 0;
	char  m_cTurn = 0;

	short m_sType = 0;
	short m_sOriginalType = 0;
	short m_sAppr2 = 0;
	short m_sStatus = 0;

	uint32_t m_dwActionTime = 0;
	uint32_t m_dwHPupTime = 0, m_dwMPupTime = 0;
	uint32_t m_dwDeadTime = 0, m_dwRegenTime = 0;

	int  m_iHitDice = 0;
	int  m_iMagicHitRatio = 0;
	int  m_iMinBravery = 0;
	int  m_iExpDice = 0;

	char m_cActionLimit = 0;

	char m_cSize = 0;
	char m_cAttackDiceThrow = 0;
	char m_cAttackDiceRange = 0;
	char m_cAttackBonus = 0;
	char m_cBravery = 0;
	char m_cResistMagic = 0;
	char m_cMagicLevel = 0;
	char m_cDayOfWeekLimit = 0;
	char m_cChatMsgPresence = 0;

	char  m_cMoveType = 0;
	char  m_cBehavior = 0;
	short m_sBehaviorTurnCount = 0;
	char  m_cTargetSearchRange = 0;

	int   m_iFollowOwnerIndex = 0;
	char  m_cFollowOwnerType = 0;
	bool  m_bIsSummoned = false;
	uint32_t m_dwSummonedTime = 0;

	int   m_iTargetIndex = 0;
	char  m_cTargetType = 0;
	char  m_cCurWaypoint = 0;
	char  m_cTotalWaypoint = 0;

	int   m_iSpotMobIndex = 0;
	int   m_iWayPointIndex[DEF_MAXWAYPOINTS + 1];
	char  m_cMagicEffectStatus[DEF_MAXMAGICEFFECTS];

	bool  m_bIsPermAttackMode = false;
	int   m_iNoDieRemainExp = 0;
	int   m_iAttackStrategy = 0;
	int   m_iAILevel = 1;

	int   m_iAttackRange = 1;
	/*
        AI-Level
        1: normal behavior
        2: Track the lowest energy target
        3: Targets under magic protection are excluded from magic attack targets
	*/
	int   m_iAttackCount = 0;

	int   m_iLastDamage = 0;
	int   m_iSummonControlMode = 0;
	char  m_cAttribute = 0;
	int   m_iAbsDamage = 0;

	int   m_iItemRatio = 0;
	int   m_iAssignedItem = 0;

	char  m_cSpecialAbility = 0;

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

	int	  m_iBuildCount = 0;
	int   m_iManaStock = 0;
	bool  m_bIsMaster = false;

	int   m_iV1 = 0;

	std::vector<npc_item> m_vNpcItem;

	int		m_iNpcitemType = 0;
	int		m_iNpcitemMax = 0;
	int		m_iNpcCrops = 0;
	int		m_iCropsSkillLV = 0;
};

}
