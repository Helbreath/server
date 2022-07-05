
#include "Npc.h"
//#include "HTNpc.h"
#include "GServer.h"
#include "Map.h"
//#include "misc.h"

#include "Client.h"
//#include "ActionID.h"
//#include "DelayEvent.h"
//#include "..\map\DynamicObjectID.h"
//#include "..\astoria.h"
//#include "Quest.h"
#include "funcs.h"
#include "netmessages.h"
#include "Tile.h"
#include "Misc.h"

using std::endl;

extern char _tmp_cTmpDirX[9];
extern char _tmp_cTmpDirY[9];

Npc::Npc(uint64_t NpcH, Map * map)
{
	int i;

	this->map = map;

	handle = NpcH;
	SetOwnerType(OWNERTYPE_NPC);

	for (i = 0; i < MAXWAYPOINTS; i++)
		m_iWayPointIndex[i] = -1;

	for (i = 0; i < MAXMAGICEFFECTS; i++)
		magicEffectStatus[i]	= 0;

	summoned       = false;
	permAttackMode = false;

	timeRegen = 0;

	summonControlMode = 0;

	element = ELEMENT_NONE;
	absDamage = 0;
	appr2     = 0;

	attackRange    = 1;
	specialAbility = 0;

	buildCount = 0;

	isLeader  = false;
	guildGUID = 0;

	m_iV1 = 0;

	m_iNpcCrops = 0;
	m_iCropsSkillLV = 0;
	// added - acidx
	targetType = 0;
	followOwnerType = 0;
	// 2002-09-17 #1
	npcItemMax = 0;

	goldDropValue = 0;
}

Npc::~Npc()
{

}

//TODO: have the server pass the config struct or something.
bool Npc::initNpcAttr(Npc * npcTemplate, char cSA)
{
	int i, iTemp;
	double dV1, dV2, dV3;
	uint32_t mult = 1;

	if (npcTemplate != 0)
	{
		name = npcTemplate->name;

		SetType(npcTemplate->Type());

		experience = dice(npcTemplate->m_iExpDice, 4) + npcTemplate->m_iExpDice;
		//
		m_iHitDice = npcTemplate->m_iHitDice;
		m_iExpDice = npcTemplate->m_iExpDice;
		m_iDefenseRatio = npcTemplate->m_iDefenseRatio;
		m_iHitRatio = npcTemplate->m_iHitRatio;
		m_iMinBravery = npcTemplate->m_iMinBravery;
		m_cAttackDiceThrow = npcTemplate->m_cAttackDiceThrow;
		m_cAttackDiceRange = npcTemplate->m_cAttackDiceRange;
		m_cSize = npcTemplate->m_cSize;
		side = npcTemplate->side;
		m_cActionLimit = npcTemplate->m_cActionLimit;
		timeActionInterval = npcTemplate->timeActionInterval;
		timeRegen = npcTemplate->timeRegen;
		m_cResistMagic = npcTemplate->m_cResistMagic;
		m_cMagicLevel = npcTemplate->m_cMagicLevel;
		m_iMaxMana = npcTemplate->m_iMaxMana;
		mana = npcTemplate->m_iMaxMana;
		m_cChatMsgPresence = npcTemplate->m_cChatMsgPresence;
		m_cDayOfWeekLimit = npcTemplate->m_cDayOfWeekLimit;
		m_cTargetSearchRange = npcTemplate->m_cTargetSearchRange;
		m_iAttackStrategy = dice(1,10);
		m_iAILevel		   = dice(1,3);
		absDamage = npcTemplate->absDamage;
		m_iMagicHitRatio = npcTemplate->m_iMagicHitRatio;
		attackRange = npcTemplate->attackRange;
		specialAbility    = cSA;
		buildCount = npcTemplate->m_iMinBravery;
		element = npcTemplate->element;
		goldDropValue	   = npcTemplate->goldDropValue;

		switch (specialAbility)
		{
		case 1: // Clairvoyant
			dV2 = (double)experience;
			dV3 = 25.0f/100.0f;
			dV1 = dV2 * dV3;
			experience += (int)dV1;
			break;

		case 2:
			dV2 = (double)experience;
			dV3 = 30.0f/100.0f;
			dV1 = dV2 * dV3;
			experience += (int)dV1;
			break;

		case 3: // Absorbing Physical Damage
			if (absDamage > 0) {
				specialAbility = 0;
				cSA = 0;
			}
			else {
				iTemp = 20 + dice(1, 60);
				absDamage -= iTemp;
				if (absDamage < -90) absDamage = -90;
			}

			dV2 = (double)experience;
			dV3 = (double)abs(absDamage)/100.0f;
			dV1 = dV2 * dV3;
			experience += (int)dV1;
			break;

		case 4: // Absorbing Magical Damage
			if (absDamage < 0) {
				specialAbility = 0;
				cSA = 0;
			}
			else {
				iTemp = 20 + dice(1, 60);
				absDamage += iTemp;
				if (absDamage > 90) absDamage = 90;
			}

			dV2 = (double)experience;
			dV3 = (double)(absDamage)/100.0f;
			dV1 = dV2 * dV3;
			experience += (int)dV1;
			break;

		case 5://??
			dV2 = (double)experience;
			dV3 = 15.0f/100.0f;
			dV1 = dV2 * dV3;
			experience += (int)dV1;
			break;

		case 6://??
		case 7://??
			dV2 = (double)experience;
			dV3 = 20.0f/100.0f;
			dV1 = dV2 * dV3;
			experience += (int)dV1;
			break;

		case 8://??
			dV2 = (double)experience;
			dV3 = 25.0f/100.0f;
			dV1 = dV2 * dV3;
			experience += (int)dV1;
			break;
// New Abilitys xRisenx

		case 9://?? is this 'highly trained'?

			switch (Type())
			{
				case NPC_LICHE:
					mult = 4;
					break;
				case NPC_SLIME:
					mult = 4;
					break;
			}
			experience *= 4*mult;
			m_iHitDice *= 2*mult;
			m_iDefenseRatio *= 3;
			m_iHitRatio *= 2;
			m_iMinBravery = 100;
			m_cAttackDiceThrow += 10; // 2->10
			m_cAttackDiceRange += 15; // 3->15
			timeActionInterval = (uint64_t)(timeActionInterval * 0.85);
			m_cResistMagic *= 1.5 + (mult/10);
			if(m_cResistMagic > 100) m_cResistMagic = 100;
			m_iMaxMana *= 1.5 * mult;
			mana = m_iMaxMana;
			m_iAttackStrategy = ATTACKAI_NORMAL;
			m_iAILevel = 3;
			if(absDamage > 70 && absDamage < 90)
				absDamage += 10;
			else if(absDamage > 50 && absDamage < 70)
				absDamage += 20;
			else if(absDamage < 50)
				absDamage += 30;
			m_iMagicHitRatio *= 2 * mult/2;
			goldDropValue *= 4/**mult*mult*/;
			break;

		//case 10: Elite

		case 11: // Mighty
			m_iHitDice		 += (m_iHitDice/4);  // No HP increase, but will regen faster & higher
			//m_cAttackDiceRange += (m_cAttackDiceRange/2);
			m_cAttackDiceRange *= 1.25;
			m_cAttackDiceThrow *= 1.65;
			attackRange     += 1;
			experience			 += (experience / 2);
			break;

		case 12: // Crippled
			m_iHitDice         = m_iHitDice/4;  // No HP diminution, so crippled mob will have hard time regen....
			m_iDefenseRatio    = m_iDefenseRatio/2;
			m_iHitRatio        = m_iHitRatio/2;
			experience			   = experience / 4;
			timeActionInterval     += 600;
			break;

		case 13: // Shaman
			m_cResistMagic     *= 2;
			m_iMaxMana         += 2500;
			m_iMagicHitRatio   *= 2;
			m_iHitDice		   *= 3;
			if ((m_iHitDice >= 1) && (m_iHitDice <= 50)) m_cMagicLevel = 5; // Same as Cyclops
			if ((m_iHitDice >= 51) && (m_iHitDice <= 150)) m_cMagicLevel = 7; // Same as Demon
			if ((m_iHitDice >= 151) && (m_iHitDice <= 500)) m_cMagicLevel = 8; // Same as Centaurs
			if ((m_iHitDice >= 501) && (m_iHitDice <= 10000)) m_cMagicLevel = 17; // Same as Barlog
			experience			 += (experience / 4);
			break;

		case 14: // Swift
			if (m_iHitRatio <= 200)
				m_iHitRatio += (m_iHitRatio/2);
			else
				m_iHitRatio     += 100;
			if (m_iDefenseRatio <= 200)
				m_iDefenseRatio += (m_iDefenseRatio/2);
			else
				m_iDefenseRatio += 100;
				timeActionInterval     = timeActionInterval/4;
			if (timeActionInterval < 200) timeActionInterval = 200;
				experience			 += (experience / 2);
			break;

		// New Abilitys xRisenx

		}

		if(m_iHitDice <= 5)
			health  = abs(dice(m_iHitDice, 4) + m_iHitDice);
		else health  = abs(m_iHitDice * 5 + dice(1, m_iHitDice));

		if (health == 0) health = 1;

		m_iNoDieRemainExp  = (experience) - (experience/3);

		SetNibble(status, 2, cSA);

		if(side == 10) // temporary code
			side = HOSTILE;
		SetNibble(status, 7, side);

		return true;
	}

	return false;
}

void Npc::RegenHP()
{
	if(IsDead())
		return;

	uint64_t now = unixtime();

	if ((now - timeHealth) > HPUPTIME)
	{
		timeHealth = now;

		//AddHP( dice(2, m_iHitDice/2) );
	}
}

void Npc::RegenMP()
{
	if (m_cMagicLevel == 0)
		return;

	uint64_t now = unixtime();

	if(now - timeMana > MPUPTIME) {
		timeMana = now;

//		m_iMP += dice(1, (m_iMaxMana/5));

		if (mana > m_iMaxMana)
			mana = m_iMaxMana;
	}
}

void Npc::Behave()
{
	switch (m_cBehavior)
	{
	case BEHAVIOR_DEAD:
		behavior_dead();
		break;
	case BEHAVIOR_STOP:
		behavior_stop();
		break;
	case BEHAVIOR_MOVE:
		behavior_move();
		break;
	case BEHAVIOR_ATTACK:
		behavior_attack();
		break;
	case BEHAVIOR_FLEE:
		behavior_flee();
		break;
	}
}

bool Npc::behavior_searchMaster()
{
// 	int ix, iy, rX, rY;
// 	short sMasterH;
// 	char cOwnerType;
//
// 	rX = m_sX - m_cTargetSearchRange;
// 	rY = m_sY - m_cTargetSearchRange;
//
// 	sMasterH = NULL;
//
// 	for (ix = rX; ix < rX + m_cTargetSearchRange*2 + 1; ix++)
// 		for (iy = rY; iy < rY + m_cTargetSearchRange*2 + 1; iy++) {
// 			g_mapList[m_cMapIndex]->GetOwner(&sMasterH, &cOwnerType, ix, iy);
// 			pMap->GetOwner(&sMasterH, &cOwnerType, ix, iy);
// 			switch (cOwnerType)
// 			{
// 			case OWNERTYPE_NPC:
// 				if ((sMasterH != m_handle) && (g_npcList[sMasterH] != NULL) && (g_npcList[sMasterH]->m_sType == m_sType) &&
// 					(g_npcList[sMasterH]->m_cMoveType == MOVETYPE_RANDOM)) {
// 						g_npcList[sMasterH]->m_bIsMaster = true;
// 						m_cMoveType = MOVETYPE_FOLLOW;
// 						m_cFollowOwnerType = OWNERTYPE_NPC;
// 						m_iFollowOwnerIndex = sMasterH;
// 						return true;
// 				}
// 				break;
// 			}
// 		}

		return false;
}


void Npc::behavior_move()
{
// 
//  	char  cDir;
//  	short sX, sY, dX, dY, absX, absY;
//  	short sDistance;
//  	shared_ptr<Unit> sTarget;
// 	dX = 0; dY = 0;
//  	if (_dead == true) return;
// 
//  	if ((summoned == true) &&
//  		(summonControlMode == 1)) return;
//  	if (magicEffectStatus[ MAGICTYPE_HOLDOBJECT ] != 0) return;
// 	if ((target != nullptr) && (target->map->name != map->name))
// 	{
// 		map->RemoveFromTarget(this->shared_from_this());
// 		return;
// 	}
//  	switch (m_cActionLimit) {
//  	case 2:
//  	case 3:
//  	case 5:
//  		case 8: // Heldenian gates
//  		m_cBehavior          = BEHAVIOR_STOP;
//  		m_sBehaviorTurnCount = 0;
//  		return;
// 	}
// 
//  	int iStX, iStY;
//  	if (map != nullptr) {
//  		iStX = x / 20;
//  		iStY = y / 20;
// 		/*for (int i = 0; i < MAXSECTORS*MAXSECTORS; i++)
// 		{
// 			std::cout << i << pMap->m_stSectorInfo[i].iPlayerActivity << endl;
// 		}*/
// 	//pMap->m_stTempSectorInfo[iStX + iStY*MAXSECTORS].iMonsterActivity++; //AcidX - TODO Figure out a way to pull from the Vectors!
//  	}
// 
//  	m_sBehaviorTurnCount++;
//  	if (m_sBehaviorTurnCount > 5)
//  	{
//  		absX = abs(m_vX - x);
//  		absY = abs(m_vY - y);
// 
//  		if ((absX <= 2)	&& (absY <= 2)) {
//  			m_sBehaviorTurnCount = 0;
//  			nextWaypointDest();
//  		}
// 
//  		m_vX = x;
//  		m_vY = y;
//  	}
// 
//  	if (m_sBehaviorTurnCount > 20)
//  	{
//  			nextWaypointDest();
//  	}
// 
// 	sTarget = TargetSearch();
// 	if (sTarget!=nullptr) {
// 		
//  		if (timeActionInterval < 1000) {//Changed to 500 for quicker re-action time.. 1000 seems too slow
//  			if (dice(1,3) == 3) {
//  				m_cBehavior          = BEHAVIOR_ATTACK;
//  				m_sBehaviorTurnCount = 0;
//  				target = sTarget;
// 				
//  				targetType  = sTarget->OwnerType();
// 				
//  				return;
//  			}
// 		}
//  		else {//I don't think this was even needed? Either way it's kind of stupid whoever did this.
//  			m_cBehavior          = BEHAVIOR_ATTACK;
//  			m_sBehaviorTurnCount = 0;
//  			target = sTarget;
// 			targetType  = sTarget->OwnerType();
//  			return;
//  		}
//  	}
// 
//  	if ((isLeader == true) && (dice(1,3) == 2)) return;
// 
//  	if (m_cMoveType == MOVETYPE_FOLLOW) {
//  		sX = x;
//  		sY = y;
//  		switch (followOwnerType) {
//  		case OWNERTYPE_PLAYER:
//  			if (follow == nullptr) {
//  				m_cMoveType = MOVETYPE_RANDOM;
//  				return;
//  			}
// 
//  			dX = follow->x;
//  			dY = follow->y;
//  			break;
//  		case OWNERTYPE_NPC:
// 			if (follow == nullptr) {
//  				m_cMoveType = MOVETYPE_RANDOM;
// 				follow = nullptr;
//  				//searchMaster(m_handle);
//  				return;
//  			}
// 
//  			dX = follow->x;
//  			dY = follow->y;
//  			break;
//  		}
// 		if (dX < 0 || dY < 0)return;
//  		if (abs(sX - dX) >= abs(sY - dY))
//  			sDistance = abs(sX - dX);
//  		else sDistance = abs(sY - dY);
// 
//  		if (sDistance >= 3) {
//  			short DOType = 0;
// 			cDir =GetNextMoveDir(sX, sY, dX, dY, map, turn, &m_tmp_iError, &DOType);
// 
//  			if (cDir == 0)
// 			{
//  			}
//  			else
// 			{
//  				if(DOType == DYNAMICOBJECT_SPIKE)
//  				{
//  					//uint32_t dmg = dice(2,4);
//  					uint32_t dmg = 100; // Npc Damage Spike Field xRisenx
//  					ReduceHP(dmg);
// 
//  					if(IsDead())
//  					{
// 						map->NpcKilledHandler(nullptr, shared_from_this(), 0);
//  						return;
//  					}
// 					else
// 					{
//  						map->gserver->SendEventToNearClient_TypeA(this, MSGID_MOTION_DAMAGE, dmg, 1, 0);
//  					}
//  				}
// 				//Clear tile and set next tile owner
//  				dX = x + _tmp_cTmpDirX[cDir];
//  				dY = y + _tmp_cTmpDirY[cDir];
// 				map->ClearOwner(x, y);
// 				map->SetOwner(shared_from_this(), dX,dY);
// 	
//  				x   = dX;
//  				y   = dY;
//  				direction = cDir;
//  				map->gserver->SendEventToNearClient_TypeA(this, MSGID_MOTION_MOVE, 0, 0, 0);
//  			}
//  		}
//  	}
//  	else
//  	{
//  		short DOType = 0;
//  		cDir = GetNextMoveDir(x, y, m_dX, m_dY,
//  			map, turn, &m_tmp_iError, &DOType);
// 			
//  		if (cDir == 0) {
//  			if (dice(1,10) == 3) nextWaypointDest();
//  		}
//  		else {
//  			if(DOType == DYNAMICOBJECT_SPIKE)
//  			{
//  				//uint32_t dmg = dice(2,4);
//  				uint32_t dmg = 100; // Npc Damage Spike Field xRisenx
//  				ReduceHP(dmg);
// 
//  				if(IsDead())
//  				{
//  					//gserver->NpcKilledHandler(NULL, NULL, m_handle, 0);
//  					return;
//  				} else {
//  					map->gserver->SendEventToNearClient_TypeA(this, MSGID_MOTION_DAMAGE, dmg, 1, 0);
//  				}
//  			}
//  			dX = x + _tmp_cTmpDirX[cDir];
//  			dY = y + _tmp_cTmpDirY[cDir];
// 			//Clear tile and set next tile owner
// 			map->ClearOwner(x, y);
// 			map->SetOwner(shared_from_this(), dX, dY);
//  			x   = dX;
//  			y   = dY;
//  			direction = cDir;
//  			map->gserver->SendEventToNearClient_TypeA(this, MSGID_MOTION_MOVE, 0, 0, 0);
// 			
//  		}
// 	
//  	}
}
//Removed from GS and added here.. 
void Npc::behavior_flee()
{
// 	char cDir;
// 	short sX, sY, dX, dY;
// 	shared_ptr<Unit> sTarget;
// 	char  cTargetType;
// 
// 	if (this->_dead == true) return;
// 
// 	this->m_sBehaviorTurnCount++;
// 
// 
// 	switch (this->m_iAttackStrategy) {
// 	case ATTACKAI_EXCHANGEATTACK:
// 	case ATTACKAI_TWOBYONEATTACK:
// 		if (this->m_sBehaviorTurnCount >= 2) {
// 
// 			this->m_cBehavior = BEHAVIOR_ATTACK;
// 			this->m_sBehaviorTurnCount = 0;
// 			return;
// 		}
// 		break;
// 
// 	default:
// 		//TODO add this mobs support master/slaves
// 		//if (dice(1, 2) == 1) NpcRequestAssistance(iNpcH);
// 		break;
// 	}
// 
// 	if (this->m_sBehaviorTurnCount > 10) {
// 
// 		this->m_sBehaviorTurnCount = 0;
// 		this->m_cBehavior = BEHAVIOR_MOVE;
// 		this->m_tmp_iError = 0;
// 		if (this->health <= 3) {
// 			this->health += dice(1, this->m_iHitDice);
// 			if (this->health <= 0) this->health = 1;
// 		}
// 		return;
// 	}
// 
// 	sTarget = this->TargetSearch();
// 
// 	if (sTarget == nullptr)
// 	{
// 		return;
// 	}
// 
// 	cTargetType = sTarget->OwnerType();
// 	if (sTarget != nullptr) {
// 		this->target = sTarget;
// 		this->targetType = cTargetType;
// 	}
// 
// 	sX = this->x;
// 	sY = this->y;
// 	switch (this->targetType) {
// 	case OWNERTYPE_PLAYER:
// 		dX = this->target->x;
// 		dY = this->target->y;
// 		break;
// 	case OWNERTYPE_NPC:
// 		dX = this->target->x;
// 		dY = this->target->y;
// 		break;
// 	}
// 	dX = sX - (dX - sX);
// 	dY = sY - (dY - sY);
// 
// 	cDir = GetNextMoveDir(sX, sY, dX, dY, this->map, this->turn, &this->m_tmp_iError);
// 	if (cDir == 0) {
// 	}
// 	else {
// 		dX = this->x + _tmp_cTmpDirX[cDir];
// 		dY = this->y + _tmp_cTmpDirY[cDir];
// 		this->map->ClearOwner(this->x, this->y);
// 
// 		this->map->SetOwner(shared_from_this(), dX, dY);
// 		this->x = dX;
// 		this->y = dY;
// 		this->direction = cDir;
// 		map->gserver->SendEventToNearClient_TypeA(this, MSGID_MOTION_MOVE, 0, 0, 0);
// 	}
}

void Npc::behavior_stop()
{
// 	std::cout << "NPC::Stop" << std::endl;
// 	char  cTargetType;
// 	shared_ptr<Unit> sTarget;
// 	bool  bFlag;
// 
// 	m_sBehaviorTurnCount++;
// 
// 	switch (m_cActionLimit)
// 	{
// 	case 5:
// 		switch (Type())
// 		{
// 		case 38: // Mana Collector
// 			if (m_sBehaviorTurnCount >= 3) {
// 				m_sBehaviorTurnCount = 0;
// 				bFlag = behavior_manaCollector();
// 
// 				if (bFlag == true) {
// 					map->gserver->SendEventToNearClient_TypeA(this, MSGID_MOTION_ATTACK, x, y, 1);
// 				}
// 			}
// 			break;
// 
// 		case 39: // Detector
// 			if (m_sBehaviorTurnCount >= 3) {
// 				m_sBehaviorTurnCount = 0;
// 				bFlag = behavior_detector();
// 				if (bFlag == true) {
// 					map->gserver->SendEventToNearClient_TypeA(this, MSGID_MOTION_ATTACK, x, y, 1);
// 				}
// 			}
// 			break;
// 		case 40: // Energy Shield Generator
// 			break;
// 
// 		case 41: // Grand Magic Generator
// 			if (m_sBehaviorTurnCount >= 3) {
// 				m_sBehaviorTurnCount = 0;
// 				behavior_grandMagicGenerator();
// 			}
// 			break;
// 
// 		case 42: // ManaStone
// 			m_sBehaviorTurnCount = 0;
// 			m_iV1 += 5;
// 			if (m_iV1 >= 5) m_iV1 = 5;
// 			break;
// 		default:
// 			sTarget = TargetSearch();
// 			break;
// 		}
// 		break;
// 	}
// 
// 	if (sTarget != nullptr) {
// 
// 		m_cBehavior = BEHAVIOR_ATTACK;
// 		m_sBehaviorTurnCount = 0;
// 		target = sTarget;
// 		targetType = sTarget->Type();
// 		return;
// 	}
}

void Npc::behavior_attack()
{
//  	int   iMagicType;
//  	short sX, sY, dX, dY;
//  	uint8_t cDir;
// 	sX= sY= dX= dY = 0;
//  	uint32_t dwTime = unixtime();
// 
//  	if (magicEffectStatus[ MAGICTYPE_HOLDOBJECT ] != 0) return;
//  	if (_dead == true) return;
// 
//  	switch (m_cActionLimit) {
//  	case 1:
//  	case 2:
//  	case 3:
//  	case 4:
//  			case 6: // moving merchant
//  	case 8: // Heldenian gate
//  		return;
// 
//  	case 5:
//  		if (buildCount > 0) return;
//  	}
// 
//  	int iStX, iStY;
//  	if (map != nullptr) {
//  		iStX = x / 20;
//  		iStY = y / 20;
// // 		pMap->m_stTempSectorInfo[iStX+iStY].iMonsterActivity++;
//  	}
// 
//  	if (m_sBehaviorTurnCount == 0)
//  		m_iAttackCount = 0;
// 
//  	m_sBehaviorTurnCount++;
//  	if (m_sBehaviorTurnCount > 20) {
//  		m_sBehaviorTurnCount = 0;
// 
//  		if (permAttackMode == false)
//  			m_cBehavior    = BEHAVIOR_MOVE;
// 
//  		return;
//  	}
// 
//  	sX = x;
//  	sY = y;
// 
//  	if (target == nullptr)
// 	{
//  		m_sBehaviorTurnCount = 0;
//  		m_cBehavior    = BEHAVIOR_MOVE;
//  		return;
// 	}
// 	dX = target->x;
// 	dY = target->y;
// 
//  	if ( (getDangerValue(dX, dY) > m_cBravery) &&
//  		(permAttackMode == false) &&
//  		(m_cActionLimit != 5)) {
// 
//  			m_sBehaviorTurnCount = 0;
//  			m_cBehavior          = BEHAVIOR_FLEE;
//  			return;
//  	}
// 
//  	if ( (health <= 2) && (dice(1,m_cBravery) <= 3) &&
//  		(permAttackMode == false) &&
//  		(m_cActionLimit != 5)) {
// 
//  			m_sBehaviorTurnCount = 0;
//  			m_cBehavior          = BEHAVIOR_FLEE;
//  			return;
//  	}
// 
//  	if ((abs(sX - dX) <= 1) && (abs(sY - dY) <= 1)) {
// 
// 		cDir = CMisc::cGetNextMoveDir(sX, sY, dX, dY);//gserver->cGetNextMoveDir(sX, sY, dX, dY, pMap, m_cTurn, &m_tmp_iError);//gserver->cGetNextMoveDir(sX, sY, dX, dY);
// 		//m_cDir=GetNextMoveDir(sX, sY, dX, dY,pMap,m_cTurn,&m_tmp_iError);
//  		if (cDir == 0) return;
//  		direction = cDir;
// 
//  		if (m_cActionLimit == 5) {
// 			switch (Type()) {
//  			case 89: // AGT
//  				if (target) {
//  				if(targetType == OWNERTYPE_PLAYER) {
// 					map->gserver->SendEventToNearClient_TypeA(this, MSGID_MOTION_ATTACK, dX, dY, 1);
//  				m_iMagicHitRatio = 1000;
//  				magicHandler(target.get(), dX, dY, 61);
//  					}
//  				}
//  				break;
//  			case 87: // CT
//  				if (target) {
//  				if(targetType == OWNERTYPE_PLAYER) {
// 					map->gserver->SendEventToNearClient_TypeA(this, MSGID_MOTION_ATTACK, x + _tmp_cTmpDirX[cDir], y + _tmp_cTmpDirY[cDir], 2);
// 					map->gserver->CalculateAttackEffect(target.get(), this, handle, OWNERTYPE_NPC, dX, dY, 2);
//  					}
//  				}
//  				break;
//  			case 36: // Crossbow Guard Tower
// 				map->gserver->SendEventToNearClient_TypeA(this, MSGID_MOTION_ATTACK, x + _tmp_cTmpDirX[cDir], y + _tmp_cTmpDirY[cDir], 2);
// 				map->gserver->CalculateAttackEffect(target.get(), this, handle, OWNERTYPE_NPC, dX, dY, 2);
//  				break;
// 
//  			case 37: // Cannon Guard Tower:
// 				map->gserver->SendEventToNearClient_TypeA(this, MSGID_MOTION_ATTACK, dX, dY, 1);
//  				m_iMagicHitRatio = 1000;
// 				magicHandler(target.get(), dX, dY, 61);
//  				break;
// 			}
//  		}
// 
//  		else {
//  			if (m_cMagicLevel == 11) {
// 				map->gserver->SendEventToNearClient_TypeA(this, MSGID_MOTION_ATTACK, x, y, 1);
//  				m_iMagicHitRatio = 1000;
// 				magicHandler(target.get(), x, y, 75);
//  			} else
//  			{
// 				map->gserver->SendEventToNearClient_TypeA(this, MSGID_MOTION_ATTACK, 0, 0, 1);
// 				map->gserver->CalculateAttackEffect(target.get(), this, handle, OWNERTYPE_NPC, dX, dY, 1);
//  			}
//  		}
//  		m_iAttackCount++;
// 
//  		if ((permAttackMode == false) && (m_cActionLimit == 0)) {
//  			switch (m_iAttackStrategy) {
//  			case ATTACKAI_EXCHANGEATTACK:
//  				m_sBehaviorTurnCount = 0;
//  				m_cBehavior          = BEHAVIOR_FLEE;
//  				break;
// 
//  			case ATTACKAI_TWOBYONEATTACK:
//  				if (m_iAttackCount >= 2) {
//  					m_sBehaviorTurnCount = 0;
//  					m_cBehavior          = BEHAVIOR_FLEE;
//  				}
//  				break;
//  			}
//  		}
//  	}
//  	else {
// 		cDir = CMisc::cGetNextMoveDir(sX, sY, dX, dY);
// 		//cDir = GetNextMoveDir(sX, sY, dX, dY, pMap, m_cTurn, &m_tmp_iError);//gserver->cGetNextMoveDir(sX, sY, dX, dY);
//  		if (cDir == 0) return;
//  		direction = cDir;
// 
//  		if ((m_cMagicLevel > 0) && (dice(1,2) == 1) &&
//  			(abs(sX - dX) <= 9) && (abs(sY - dY) <= 7)) {
//  				iMagicType = -1;
// 				//return;//TODO : Fix because Magic is currently broken;
// 				//also fix this nasty switch
// 				switch (m_cMagicLevel)
//  				{
//  				case 1:
// 					if (map->gserver->m_pMagicConfigList[0]->manaCost <= mana)
//  						iMagicType = 0;
//  					break;
//  				case 2:
// 					if (map->gserver->m_pMagicConfigList[10]->manaCost <= mana)
//  						iMagicType = 10;
// 					else if (map->gserver->m_pMagicConfigList[0]->manaCost <= mana)
//  						iMagicType = 0;
//  					break;
//  				case 3:
// 					if (map->gserver->m_pMagicConfigList[20]->manaCost <= mana)
//  						iMagicType = 20;
// 					else if (map->gserver->m_pMagicConfigList[10]->manaCost <= mana)
//  						iMagicType = 10;
//  					break;
//  				case 4:
// 					if (map->gserver->m_pMagicConfigList[30]->manaCost <= mana)
//  						iMagicType = 30;
// 					else if (map->gserver->m_pMagicConfigList[37]->manaCost <= mana)
//  						iMagicType = 37;
// 					else if (map->gserver->m_pMagicConfigList[20]->manaCost <= mana)
//  						iMagicType = 20;
// 					else if (map->gserver->m_pMagicConfigList[10]->manaCost <= mana)
//  						iMagicType = 10;
//  					break;
//  				case 5:
// 					if (map->gserver->m_pMagicConfigList[43]->manaCost <= mana)
//  						iMagicType = 43;
// 					else if (map->gserver->m_pMagicConfigList[30]->manaCost <= mana)
//  						iMagicType = 30;
// 					else if (map->gserver->m_pMagicConfigList[37]->manaCost <= mana)
//  						iMagicType = 37;
// 					else if (map->gserver->m_pMagicConfigList[20]->manaCost <= mana)
//  						iMagicType = 20;
// 					else if (map->gserver->m_pMagicConfigList[10]->manaCost <= mana)
//  						iMagicType = 10;
//  					break;
//  				case 6:
// 					if (map->gserver->m_pMagicConfigList[51]->manaCost <= mana)
//  						iMagicType = 51;
// 					else if (map->gserver->m_pMagicConfigList[43]->manaCost <= mana)
//  						iMagicType = 43;
// 					else if (map->gserver->m_pMagicConfigList[30]->manaCost <= mana)
//  						iMagicType = 30;
// 					else if (map->gserver->m_pMagicConfigList[37]->manaCost <= mana)
//  						iMagicType = 37;
// 					else if (map->gserver->m_pMagicConfigList[20]->manaCost <= mana)
//  						iMagicType = 20;
// 					else if (map->gserver->m_pMagicConfigList[10]->manaCost <= mana)
//  						iMagicType = 10;
//  					break;
//  				case 7:
// 					if ((map->gserver->m_pMagicConfigList[70]->manaCost <= mana) && (dice(1, 5) == 3))
//  						iMagicType = 70;
// 					else if (map->gserver->m_pMagicConfigList[61]->manaCost <= mana)
//  						iMagicType = 61;
// 					else if (map->gserver->m_pMagicConfigList[60]->manaCost <= mana)
//  						iMagicType = 60;
// 					else if (map->gserver->m_pMagicConfigList[51]->manaCost <= mana)
//  						iMagicType = 51;
// 					else if (map->gserver->m_pMagicConfigList[43]->manaCost <= mana)
//  						iMagicType = 43;
// 					break;
//  				case 8:
// 					if ((map->gserver->m_pMagicConfigList[35]->manaCost <= mana) && (dice(1, 3) == 2))
// 						iMagicType = 35;
// 					else if (map->gserver->m_pMagicConfigList[60]->manaCost <= mana)
//  						iMagicType = 60;
// 					else if (map->gserver->m_pMagicConfigList[51]->manaCost <= mana)
//  						iMagicType = 51;
// 					else if (map->gserver->m_pMagicConfigList[43]->manaCost <= mana)
//  						iMagicType = 43;
//  					break;
// 
//  				case 9:
// 					if ((map->gserver->m_pMagicConfigList[74]->manaCost <= mana) && (dice(1, 3) == 2))
//  						iMagicType = 74;
//  					break;
//  				case 10:
// 					if ((map->gserver->m_pMagicConfigList[57]->manaCost <= mana) && (dice(1, 3) == 2))
//  						iMagicType = 57;
//  					break;
//  				case 11:
//  					goto NBA_CHASE;
//  					break;
//  				case 12: // Wyvern
// 					if ((map->gserver->m_pMagicConfigList[91]->manaCost <= mana) && (dice(1, 3) == 2))
//  						iMagicType = 91;//blizzard
// 					else if (map->gserver->m_pMagicConfigList[63]->manaCost <= mana)
//  						iMagicType = 63;
//  					break;
//  				case 13: // Abaddon
// 					if ((map->gserver->m_pMagicConfigList[96]->manaCost <= mana) && (dice(1, 3) == 2))
//  						iMagicType = 96;
// 					else if (map->gserver->m_pMagicConfigList[81]->manaCost <= mana)
//  						iMagicType = 81;
//  					break;
//  				case 14: // Earth Wyvern
// 					if ((map->gserver->m_pMagicConfigList[64]->manaCost <= mana) && (dice(1, 3) == 2))
//  						iMagicType = 64;
// 					else if (map->gserver->m_pMagicConfigList[96]->manaCost <= mana)
//  						iMagicType = 96;
//  					break;
//  				case 15: // Black Demon
// 					if (map->gserver->m_pMagicConfigList[92]->manaCost <= mana)
//  						iMagicType = 92;
// 					else if ((map->gserver->m_pMagicConfigList[73]->manaCost <= mana) && (dice(1, 30) == 2))
//  						iMagicType = 73;
// 					else if ((map->gserver->m_pMagicConfigList[83]->manaCost <= mana) && (dice(1, 10) == 2))
//  						iMagicType = 83;
//  					break;
//  				case 16: // Fire Wyvern
// 					if ((map->gserver->m_pMagicConfigList[61]->manaCost <= mana) && (dice(1, 3) == 2))
//  						iMagicType = 61;
// 					else if (map->gserver->m_pMagicConfigList[81]->manaCost <= mana)
//  						iMagicType = 81;
//  					break;
//  				case 17: // Barlog
// 					if (map->gserver->m_pMagicConfigList[92]->manaCost <= mana)
//  						iMagicType = 92;
// 					else if (map->gserver->m_pMagicConfigList[63]->manaCost <= mana)
//  						iMagicType = 63;
//  					break;
//  				case 18: // Enraged Troll / Enraged Stalker
// 					if ((map->gserver->m_pMagicConfigList[35]->manaCost <= mana) && (dice(1, 3) == 2))
// 						iMagicType = 35;
//  					break;
//  				case 19: // Enraged Cyclops
// 					if (map->gserver->m_pMagicConfigList[74]->manaCost <= mana)
//  						iMagicType = 74;
// 					else if (map->gserver->m_pMagicConfigList[35]->manaCost <= mana)
// 						iMagicType = 35;
// 					break;
//  				case 20: // Enraged Gargoyle
// 					if ((map->gserver->m_pMagicConfigList[41]->manaCost <= mana) && (dice(1, 3) == 2))
// 						iMagicType = 41;
// 					else if (map->gserver->m_pMagicConfigList[81]->manaCost <= mana)
// 						iMagicType = 81;
// 				break;
//  				case 21: // Enraged Hellclaw
// 					if ((map->gserver->m_pMagicConfigList[55]->manaCost <= mana) && (dice(1, 3) == 2))
// 						iMagicType = 55;
// 					else if (map->gserver->m_pMagicConfigList[35]->manaCost <= mana)
//  						iMagicType = 35;
// 					break;
// 				case 22: // Enraged Tigerworm
// 					if (map->gserver->m_pMagicConfigList[88]->manaCost <= mana)
//  						iMagicType = 88;
// 					else if (map->gserver->m_pMagicConfigList[35]->manaCost <= mana)
//  						iMagicType = 35;
//  					break;
//  				case 23: // Wind Wyvern
// 					if (map->gserver->m_pMagicConfigList[63]->manaCost <= mana)
//  						iMagicType = 63;
// 					else if (map->gserver->m_pMagicConfigList[93]->manaCost <= mana)
//  						iMagicType = 93;
//  					break;
//  				}
// 
//  				if (iMagicType != -1) {
// 
//  					if (m_iAILevel >= 2) {
//  						switch (targetType) {
//  						case OWNERTYPE_PLAYER:
//  							if (target->magicEffectStatus[MAGICTYPE_PROTECT] == MAGICPROTECT_PFM) {
//  								if ((abs(sX - dX) > attackRange) || (abs(sY - dY) > attackRange)) {
//  									m_sBehaviorTurnCount = 0;
//  									m_cBehavior    = BEHAVIOR_MOVE;
//  									return;
//  								}
//  								else goto NBA_CHASE;
//  							}
//  							if ((iMagicType == 35) && (target->magicEffectStatus[ MAGICTYPE_HOLDOBJECT ] != 0)) goto NBA_CHASE;
//  							break;
// 
//  						case OWNERTYPE_NPC:
//  							if (target->magicEffectStatus[MAGICTYPE_PROTECT] == MAGICPROTECT_PFM) {
//  								if ((abs(sX - dX) > attackRange) || (abs(sY - dY) > attackRange)) {
//  									m_sBehaviorTurnCount = 0;
//  									m_cBehavior    = BEHAVIOR_MOVE;
//  									return;
//  								}
//  								else goto NBA_CHASE;
//  							}
//  							if ((iMagicType == 35) && (target->magicEffectStatus[ MAGICTYPE_HOLDOBJECT ] != 0)) goto NBA_CHASE;
//  							break;
//  						}
//  					}
// 
// 					map->gserver->SendEventToNearClient_TypeA(this, MSGID_MOTION_ATTACK, x + _tmp_cTmpDirX[cDir], y + _tmp_cTmpDirY[cDir], 1);
// 					magicHandler(this, dX, dY, iMagicType);
//  					timeLastAction = dwTime;
//  					return;
//  				}
//  		}
// 
//  		if ((m_cMagicLevel < 0) && (dice(1,2) == 1) &&
//  			(abs(sX - dX) <= 9) && (abs(sY - dY) <= 7)) {
//  				iMagicType = -1;
// 				if (map->gserver->m_pMagicConfigList[43]->manaCost <= mana)
//  					iMagicType = 43;
// 				else if (map->gserver->m_pMagicConfigList[37]->manaCost <= mana)
//  					iMagicType = 37;
// 				else if (map->gserver->m_pMagicConfigList[0]->manaCost <= mana)
// 					iMagicType = 0;
//  				if (iMagicType != -1) {
// 					map->gserver->SendEventToNearClient_TypeA(this, MSGID_MOTION_ATTACK, x + _tmp_cTmpDirX[cDir], y + _tmp_cTmpDirY[cDir], 1);
// 					magicHandler(target.get(), dX, dY, iMagicType);
//  					timeLastAction = dwTime;
//  					return;
//  				}
//  		}
// 
//  		if ((attackRange > 1) &&
//  			(abs(sX - dX) <= attackRange) && (abs(sY - dY) <= attackRange)) {
// 
// 			cDir = CMisc::cGetNextMoveDir(sX, sY, dX, dY);//GetNextMoveDir(sX, sY, dX, dY, pMap, m_cTurn, &m_tmp_iError);//gserver->cGetNextMoveDir(sX, sY, dX, dY);
//  				if (cDir == 0) return;
//  				direction = cDir;
// 
//  				if (m_cActionLimit == 5)
// 				{
// 					switch (Type()) {
//  						case 89:  //AGT
// 							if (target && target->IsPlayer())
// 							{
// 								map->gserver->SendEventToNearClient_TypeA(this, MSGID_MOTION_ATTACK, dX, dY, 1);
// 								m_iMagicHitRatio = 1000;
// 								magicHandler(target.get(), dX, dY, 61);
// 							}
//  							break;
//  						case 87: // CT
//  							if (target && target->IsPlayer())
// 							{
// 								map->gserver->SendEventToNearClient_TypeA(this, MSGID_MOTION_ATTACK, dX, dY, 2);
// 								map->gserver->CalculateAttackEffect(target.get(), this, handle, OWNERTYPE_NPC, dX, dY, 2);
//  							}
//  							break;
//  						case 36: // Crossbow Guard Tower
// 							map->gserver->SendEventToNearClient_TypeA(this, MSGID_MOTION_ATTACK, dX, dY, 2);
// 							map->gserver->CalculateAttackEffect(target.get(), this, handle, OWNERTYPE_NPC, dX, dY, 2);
//  						break;
// 
//  						case 37://  Cannon Guard Tower
// 							map->gserver->SendEventToNearClient_TypeA(this, MSGID_MOTION_ATTACK, dX, dY, 1);
//  							m_iMagicHitRatio = 1000;
//  							magicHandler(target.get(), dX, dY, 61);
//  						break;
//  					}
//  				}
//  				else {
// 					switch (Type()) {
//  					case 51:
// 						map->gserver->SendEventToNearClient_TypeA(this, MSGID_MOTION_ATTACK, dX, dY, 1);
//  						m_iMagicHitRatio = 1000;
//  						magicHandler(target.get(), dX, dY, 61);
//  						break;
// 
//  					case 54:  //Dark Elf
// 						map->gserver->SendEventToNearClient_TypeA(this, MSGID_MOTION_ATTACK, dX, dY, 2);
// 						map->gserver->CalculateAttackEffect(target.get(), this, handle, OWNERTYPE_NPC, dX, dY, 2);
//  						break;
//  					case 63: //v2.20 2002-12-20 frost
//  					case 53:// Beholder
//  					case 79:
//  						if (target)
//  						{
// 							if (!target->IsDead() && target->CheckResistIce(direction, m_iMagicHitRatio) == false)
//  							{
//  								if (target->magicEffectStatus[ MAGICTYPE_ICE ] == 0)
//  								{
//  									target->magicEffectStatus[ MAGICTYPE_ICE ] = 1;
//  									target->SetStatusFlag(STATUS_FROZEN, true);
// 									map->RegisterDelayEvent(DELAYEVENTTYPE_MAGICRELEASE, MAGICTYPE_ICE, dwTime + (5 * 1000),
// 										target.get(), 0, 0, 1, 0, 0);
//  								}
//  							}
// 							map->gserver->SendEventToNearClient_TypeA(this, MSGID_MOTION_ATTACK, dX, dY, 20);
// 							map->gserver->CalculateAttackEffect(target.get(), this, handle, OWNERTYPE_NPC, dX, dY, 1);
//  						}
//  						break;
// 
//  					default:
// 						map->gserver->SendEventToNearClient_TypeA(this, MSGID_MOTION_ATTACK, dX, dY, 20);
// 						//gserver->CalculateAttackEffect(m_iTargetIndex.get(), this, m_handle, OWNERTYPE_NPC, dX, dY, 1);
// 						magicHandler(target.get(), dX, dY, 0);//test
// 						break;
//  					}
//  				}
//  				m_iAttackCount++;
// 
//  				if ((permAttackMode == false) && (m_cActionLimit == 0)) {
//  					switch (m_iAttackStrategy) {
//  					case ATTACKAI_EXCHANGEATTACK:
//  						m_sBehaviorTurnCount = 0;
//  						m_cBehavior          = BEHAVIOR_FLEE;
//  						break;
// 
//  					case ATTACKAI_TWOBYONEATTACK:
//  						if (m_iAttackCount >= 2) {
//  							m_sBehaviorTurnCount = 0;
//  							m_cBehavior          = BEHAVIOR_FLEE;
//  						}
//  						break;
//  					}
//  				}
//  				return;
//  		}
// 
//  NBA_CHASE:;
// 
//  		if (m_cActionLimit != 0) return;
// 
//  		m_iAttackCount = 0;
// 
//  		{
// 			cDir = CMisc::cGetNextMoveDir(sX, sY, dX, dY);// GetNextMoveDir(sX, sY, dX, dY, pMap, m_cTurn, &m_tmp_iError);
//  			if (cDir == 0) {
//  				return;
//  			}
//  			dX = x + _tmp_cTmpDirX[cDir];
//  			dY = y + _tmp_cTmpDirY[cDir];
//  			map->ClearOwner( x, y);
// 			map->SetOwner(shared_from_this(), dX, dY);
//  			x   = dX;
//  			y   = dY;
//  			direction = cDir;
// 			map->gserver->SendEventToNearClient_TypeA(this, MSGID_MOTION_MOVE, 0, 0, 0);
//  		}
//  	}
}

bool Npc::behavior_manaCollector()
{
// 	int dX, dY, iMaxMP, iTotal;
// 	short sOwnerH;
// 	char  cOwnerType;
// 	double dV1, dV2, dV3;
// 	bool bRet;
//
// 	if (m_sAppr2 != 0) return false;
//
// 	bRet = false;
// 	for (dX = m_sX-5; dX <= m_sX+5; dX++)
// 		for (dY = m_sY-5; dY <= m_sY+5; dY++) {
// 			pMap->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
//
// 			if (sOwnerH != NULL) {
// 				switch (cOwnerType) {
// 				case OWNERTYPE_PLAYER:
// 					if (m_side == g_clientList[sOwnerH]->m_side) {
// 						iMaxMP = g_clientList[sOwnerH]->GetMaxMP();
// 						if (g_clientList[sOwnerH]->m_iMP < iMaxMP) {
// 							iTotal = dice(1, (g_clientList[sOwnerH]->GetMag()) ); // Mana Point´Â 1D(Magic)
// 							if (g_clientList[sOwnerH]->m_iAddMP != 0) {
// 								dV2 = (double)iTotal;
// 								dV3 = (double)g_clientList[sOwnerH]->m_iAddMP;
// 								dV1 = (dV3 / 100.0f)*dV2;
// 								iTotal += (int)dV1;
// 							}
//
// 							g_clientList[sOwnerH]->m_iMP += iTotal;
//
// 							if (g_clientList[sOwnerH]->m_iMP > iMaxMP)
// 								g_clientList[sOwnerH]->m_iMP = iMaxMP;
//
// 							gserver->SendNotifyMsg(NULL, sOwnerH, NOTIFY_MP, NULL, NULL, NULL, NULL);
// 						}
// 					}
// 					break;
//
// 				case OWNERTYPE_NPC:
// 					if ((g_npcList[sOwnerH]->m_sType == NPC_MANASTONE) && (g_npcList[sOwnerH]->m_iV1 > 0)) {
// 						if (g_npcList[sOwnerH]->m_iV1 >= 3) {
// 							gserver->m_iCollectedMana[m_side] += 3;
// 							g_npcList[sOwnerH]->m_iV1 -= 3;
// 							bRet = true;
// 						} else {
// 							gserver->m_iCollectedMana[m_side] += g_npcList[sOwnerH]->m_iV1;
// 							g_npcList[sOwnerH]->m_iV1 = 0;
// 							bRet = true;
// 						}
// 					}
// 					break;
// 				}
// 			}
// 		}
//
// 	return bRet;
	return false;
}


void Npc::behavior_grandMagicGenerator()
{
// 	if(IsNeutral()) return;
//
// 	if (gserver->m_mana[m_side] > gserver->m_iMaxGMGMana) {
// 		gserver->_GrandMagicLaunchMsgSend(1, m_side);
// 		gserver->MeteorStrikeMsgHandler(m_side);
// 		gserver->m_mana[m_side] = 0;
// 	}
}



bool Npc::behavior_detector()
{
//	int dX, dY;
	bool  bFlag = false;

// 	if (m_sAppr2 != 0) return false;
//
// 	Unit * owner = NULL;
//
// 	for (dX = m_sX-10; dX <= m_sX+10; dX++)
// 		for (dY = m_sY-10; dY <= m_sY+10; dY++) {
// 			owner = pMap->GetOwner(dX, dY);
//
// 			if(!owner)	continue;
//
// 			if (!owner->IsNeutral() && (owner->m_side != m_side)) {
// 				if (owner->m_cMagicEffectStatus[ MAGICTYPE_INVISIBILITY ] != NULL) {
// 					owner->m_cMagicEffectStatus[ MAGICTYPE_INVISIBILITY ] = NULL;
// 					owner->SetMagicFlag(MAGICTYPE_INVISIBILITY, false);
// 				}
// 				bFlag = true;
// 			}
// 		}

	return bFlag;
}

//TODO: let the npc reference the map its on for this data
void Npc::nextWaypointDest()
{
//  	short sRange, sX, sY;
//  	int i, j, iMapIndex;
//  	bool bFlag;
// 
//  	switch (m_cMoveType)
//  	{
//  	case MOVETYPE_GUARD:
//  		break;
// 
//  	case MOVETYPE_SEQWAYPOINT:
// 
//  		m_cCurWaypoint++;
//  		if (m_cCurWaypoint >= m_cTotalWaypoint)
//  			m_cCurWaypoint = 1;
// 		m_dX = (short)(map->waypointList[m_iWayPointIndex[m_cCurWaypoint]].x);
// 		m_dY = (short)(map->waypointList[m_iWayPointIndex[m_cCurWaypoint]].y);
//  		break;
// 
//  	case MOVETYPE_RANDOMWAYPOINT:
// 
//  		m_cCurWaypoint = ((rand() % (m_cTotalWaypoint - 1)) + 1);
// 		m_dX = (short)(map->waypointList[m_iWayPointIndex[m_cCurWaypoint]].x);
// 		m_dY = (short)(map->waypointList[m_iWayPointIndex[m_cCurWaypoint]].y);
//  		break;
// 
//  	case MOVETYPE_RANDOMAREA:
// 
//  		sRange = (short)(roamArea.right - roamArea.left);
//  		m_dX = (short)((rand() % sRange) + roamArea.left);
//  		sRange = (short)(roamArea.bottom - roamArea.top);
//  		m_dY = (short)((rand() % sRange) + roamArea.top);
//  		break;
// 
//  	case MOVETYPE_RANDOM:
// 		m_dX = (rand() % (map->sizeX - 50)) + 15;
// 		m_dY = (rand() % (map->sizeY - 50)) + 15;
//  	//	iMapIndex = m_cMapIndex;
// //
//  		for ( i = 0; i <= 30; i++) {
// 			sX = (rand() % (map->sizeX - 50)) + 15;
// 			sY = (rand() % (map->sizeY - 50)) + 15;
// //
//  			bFlag = true;
// 			/*
// 			for (k = 0; k < MAXMGAR; k++)
// 			{
// 				if (mapIndex->m_rcMobGenAvoidRect[k].left != -1)
// 				{
// 					if ((sX >= mapIndex->m_rcMobGenAvoidRect[k].left) &&
// 						(sX <= mapIndex->m_rcMobGenAvoidRect[k].right) &&
// 						(sY >= mapIndex->m_rcMobGenAvoidRect[k].top) &&
// 						(sY <= mapIndex->m_rcMobGenAvoidRect[k].bottom))
// 					{*/
// 		
// 			for (j = 0; j < map->mobGeneratorAvoidList.size(); j++)
// 				if (map->mobGeneratorAvoidList[j].left != -1) {
//  					if ((sX >= map->mobGeneratorAvoidList[j].left) &&
// 						(sX <= map->mobGeneratorAvoidList[j].right) &&
// 						(sY >= map->mobGeneratorAvoidList[j].top) &&
// 						(sY <= map->mobGeneratorAvoidList[j].bottom)) {
//  							bFlag = false;
//  					}
//  				}
//  				if (bFlag == true) goto CNW_GET_VALIDLOC_SUCCESS;
//  		}
// // 		// Fail!
//  		m_tmp_iError  = 0;
//  		return;
// //
//  CNW_GET_VALIDLOC_SUCCESS:;
//  		m_dX = sX;
//  		m_dY = sY;
//  		break;
//  	}
// //
//  	m_tmp_iError  = 0;
}
/*
Unit * Npc::TargetSearch(uint8_t dX,uint8_t dY,uint8_t Radius)
{
	int x, y, ret;
	class Tile * pTile;

	//if ((cMapIndex < 0) || (cMapIndex > MAXMAPS)) return 0;
	//if (maplist[cMapIndex] == NULL) return 0;

	ret = 0;
	for (x = dX - Radius; x <= dX + Radius; x++)
		for (y = dY - Radius; y <= dY + Radius; y++) {
			if ((x < 0) || (x >= pMap->m_sSizeX) ||
				(y < 0) || (y >= pMap->m_sSizeY)) {
			}
			else {
				pTile = (class Tile *)(pMap->m_pTile + x + y*pMap->m_sSizeY);
				if ((pTile->owner != NULL) && (pTile->m_cOwnerClass == OWNERTYPE_PLAYER))
					return pTile->owner;
			}
		}

	return 0;
}*/
std::shared_ptr<Unit> Npc::TargetSearch()
{

//  	int ix, iy, iPKCount;
//  	short sX, sY, rX, rY;
//  	short sDistance, sTempDistance;
//  	int   iSearchType;
// 
//  	sDistance    = 100;
// 
//  	iSearchType = 0;
// 	switch (Type())
//  	{
//  	case NPC_CP: iSearchType = 1; break;
//  	}
// 
//  	std::list<std::shared_ptr<Unit>> owners = map->GetOwners(
//  		x - m_cTargetSearchRange, x + m_cTargetSearchRange,
//  		y - m_cTargetSearchRange, y + m_cTargetSearchRange);
// 	if (owners.size() <= 0)return 0;
// 	std::cout << "Owners Size = " << owners.size() << endl;
// 	std::cout << "HP = " << this->health << endl;
// 	
// 	std::shared_ptr<Unit> target;
// 	for (std::shared_ptr<Unit> owner : owners)
// 	{
// 	
// 		if (owner == nullptr) continue;
//  		if (owner == shared_from_this())
//  			continue;
// 
//  		iPKCount = 0;
// 		switch (owner->OwnerType())
//  		{
//  		case OWNERTYPE_PLAYER:
//  //			if(gserver->GetClient(owner->m_handle)->IsNoAggro() || iSearchType == 1)
//  //				continue;
// 
// 			iPKCount = map->gserver->GetClient(owner->handle)->playerKillCount;
//  			break;
// 
//  		case OWNERTYPE_NPC:
//  			switch (iSearchType)
//  			{
//  			case 1:
//  				switch (owner->Type())
//  				{
//  				case 36:
//  				case 37:
//  				case 38:
//  				case 39:
//  				case 40:
//  				case 41:
// 					break;
//  				default:
//  					continue;
//  				}
//  				break;
//  			case 0:
// 				switch (owner->Type())
//  				{
//  				case NPC_CT:
//  				case NPC_AGC:
//  					continue;
//  				}
//  				break;
// 
//  			}
// 
//  			iPKCount    = 0;
// 
// 			if (Type() == 21) {
// 				if (map->gserver->getPlayerNum(this->map, owner->x, owner->y, 2) != 0) {
//  					continue;
//  				}
//  			}
//  			break;
//  		}
// 
//  		/*if (m_side < 10) {
//  			if (owner->IsNeutral()) {
//  				if (iPKCount == 0) continue;
//  			}*/
//  			/*else {
//  				if ((iPKCount == 0 && owner->m_side == m_side) || IsNeutral())
//  					//continue;
//  			}*/
//  		// }
// /* 		else {
//  			if ((owner->m_ownerType == OWNERTYPE_NPC && owner->IsNeutral()) || owner->m_side == m_side)
//  				continue;
//  		}*/
// 
//  		// INVISIBILITY
//  		if (owner->IsInvisible() && specialAbility != 1) continue;
// 
//  		if (abs(x - owner->x) >= abs(y - owner->y))
//  			sTempDistance = abs(x - owner->x);
//  		else sTempDistance = abs(y - owner->y);
// 
//  		if (sTempDistance <	sDistance) {
//  			sDistance = sTempDistance;
//  			target = owner;
//  		}
//  	}
// 
//  	if(target)
//  	{
// 		return target;//pTarget     = target;
//  		//pTargetType = *target;
//  	} else {
//  		return 0;
//  	}
return 0;
}

int Npc::getDangerValue(short dX, short dY)
{
	return 1;
// 	int ix, iy, iDangerValue;
// 	short sDOType;
// 	uint32_t dwRegisterTime;
// 	Unit * owner;
//
// 	iDangerValue = 0;
//
// 	for (ix = dX - 2; ix <= dX + 2; ix++){
// 		for (iy = dY - 2; iy <= dY + 2; iy++) {
// 			owner = pMap->GetOwner(ix, iy);
// 			pMap->bGetDynamicObject( ix, iy, &sDOType, &dwRegisterTime );
//
// 			if (sDOType == 1) iDangerValue++;
//
// 			if (!owner) continue;
//
// 			if (m_side != owner->m_side)
// 				iDangerValue++;
// 			else iDangerValue--;
// 		}
// 	}
//
// 	return iDangerValue;
}

void Npc::magicHandler(Unit * unit, short dX, short dY, short magicType)
{
// 	Magic * spell;
// 
// 	if ((dX < 0) || (dX >= map->sizeX) ||
// 		(dY < 0) || (dY >= map->sizeY)) return;
// 
//  	const int crossPnts[5][2] = {{0,0},{-1,0},{1,0},{0,-1},{0,1}};
//  	uint64_t  dwTime = unixtime();
// 	int32_t damage = 0, magicResist = 0;
// // 	int i, iErr, ix, iy, sX, sY, tX, tY, iResult, iWeatherBonus;
// 
// 	if ((magicType < 0) || (magicType >= 100))     return;
// 	if (map->gserver->m_pMagicConfigList[magicType] == nullptr) return;
// 
// 	if (unit->IsPlayer())
// 	{
// 		if (unit->map->flags.attackEnabled == false) return;
// 		//TODO: Calculate player magic hit ratio (or go ahead as planned and remove it early)
// 	}
// 	else
// 	{
// 		magicResist = ((Npc*)unit)->m_iMagicHitRatio;
// 	}
// 
// 	//TODO: Implement old weather effects?
// 	// 	iWeatherBonus = gserver->iGetWeatherMagicBonusEffect(spell, pMap->m_weather);
// 
// 	spell = map->gserver->m_pMagicConfigList[magicType];
// 
// 	Unit * target = map->GetOwner(dX, dY).get();
// 
// 
// 
// 	if (spell->delayTime == 0) {
// 		switch (spell->magicType)
// 		{
// 		case MAGICTYPE_CANCELLATION:
// 			if (target){
// 				target->RemoveMagicEffect(MAGICTYPE_INVISIBILITY);
// 				target->RemoveMagicEffect(MAGICTYPE_PROTECT);
// 				target->RemoveMagicEffect(MAGICTYPE_HOLDOBJECT);
// 				target->RemoveMagicEffect(MAGICTYPE_CONFUSE);
// 				target->RemoveMagicEffect(MAGICTYPE_BERSERK);
// 				target->RemoveMagicEffect(MAGICTYPE_ICE);
// 				target->RemoveMagicEffect(MAGICTYPE_POLYMORPH);
// 				target->RemoveMagicEffect(MAGICTYPE_INHIBITION);
// 			}
// 			break;
// 
// 		case MAGICTYPE_INVISIBILITY:
// 			switch (spell->m_sValue[MAGICV_TYPE])
// 			{
// 			case 1: // Invis
// 				//if (!target || !target->AddMagicEffect(spell->m_sType, spell->m_dwLastTime)) goto NMH_NOEFFECT;
// 				break;
// 
// 			case 2: // Detect Invis
// 				//TODO: increase or decrease detect invis radius to compensate larger resolution
// 				for (short ix = dX - 8; ix <= dX + 8; ix++)
// 				{
// 					for (short iy = dY - 8; iy <= dY + 8; iy++){
// 						target = map->GetOwner(ix, iy).get();
// 						if (target){
// 							target->RemoveMagicEffect(spell->magicType);
// 							map->RemoveFromTarget(shared_ptr<Unit>(target));
// 						}
// 					}
// 				}
// 				break;
// 			}
// 			break;
// 
// 		case MAGICTYPE_HOLDOBJECT:
// 			if (target && target->CheckResistMagic(direction, magicResist) == false) {
// 				if (target->IsNPC() && ((Npc*)target)->m_cMagicLevel >= 6)
// 					break;
// 				if (!target->AddMagicEffect(spell->magicType, spell->lastTime))
// 					break;
// 			}
// 			break;
// 
// 
// // 		case MAGICTYPE_DAMAGE_LINEAR:
// // 			sX = m_sX;
// // 			sY = m_sY;
// //
// // 			for (i = 2; i < 13; i++) {
// // 				iErr = 0;
// // 				CMisc::GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);
// //
// // 				for(int pNo=0; pNo < 5; pNo++){
// // 					pMap->GetOwner(&sOwnerH, &cOwnerType, tX+crossPnts[pNo][0], tY+crossPnts[pNo][1]);
// // 					if (CheckResistingMagicSuccess(m_cDir, sOwnerH, cOwnerType, iResult) == false)
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element);
// //
// // 					pMap->GetDeadOwner(&sOwnerH, &cOwnerType, tX+crossPnts[pNo][0], tY+crossPnts[pNo][1]);
// // 					if ( (cOwnerType == OWNERTYPE_PLAYER) && (g_clientList[sOwnerH] != NULL) &&
// // 						(g_clientList[sOwnerH]->m_iHP > 0) ) {
// // 							if (CheckResistingMagicSuccess(m_cDir, sOwnerH, cOwnerType, iResult) == false)
// // 								gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARRANGE] + iWeatherBonus, false, spell->m_element);
// // 					}
// // 				}
// // 				if ( (abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
// // 			}
// //
// // 			for (iy = dY - spell->m_vRange; iy <= dY + spell->m_vRange; iy++)
// // 				for (ix = dX - spell->m_hRange; ix <= dX + spell->m_hRange; ix++) {
// // 					pMap->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
// // 					if (CheckResistingMagicSuccess(m_cDir, sOwnerH, cOwnerType, iResult) == false)
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARRANGE] + iWeatherBonus, false, spell->m_element);
// //
// // 					pMap->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
// // 					if ( (cOwnerType == OWNERTYPE_PLAYER) && (g_clientList[sOwnerH] != NULL) &&
// // 						(g_clientList[sOwnerH]->m_iHP > 0) ) {
// // 							if (CheckResistingMagicSuccess(m_cDir, sOwnerH, cOwnerType, iResult) == false)
// // 								gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARRANGE] + iWeatherBonus, false, spell->m_element);
// // 					}
// // 				}
// //
// // 			// dX, dY
// // 			pMap->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
// // 			if (CheckResistingMagicSuccess(m_cDir, sOwnerH, cOwnerType, iResult) == false)
// // 				gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, false, spell->m_element);
// //
// // 			pMap->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
// // 			if ( (cOwnerType == OWNERTYPE_PLAYER) && (g_clientList[sOwnerH] != NULL) &&
// // 				(g_clientList[sOwnerH]->m_iHP > 0) ) {
// // 					if (CheckResistingMagicSuccess(m_cDir, sOwnerH, cOwnerType, iResult) == false)
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, false, spell->m_element);
// // 			}
// // 			break;
// //
// // 		case MAGICTYPE_ICE_LINEAR:
// // 			sX = m_sX;
// // 			sY = m_sY;
// //
// // 			for (i = 2; i < 13; i++) {
// // 				iErr = 0;
// // 				CMisc::GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);
// //
// // 				for(int pNo=0; pNo < 5; pNo++){
// // 					target = pMap->GetOwner(tX+crossPnts[pNo][0], tY+crossPnts[pNo][1]);
// // 					if (target && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						//for (i = 0; i < 2; i++)
// // 						gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, sX, sY, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element);
// // 						if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 							target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 					}
// //
// // 					target = pMap->GetDeadOwner(tX+crossPnts[pNo][0], tY+crossPnts[pNo][1]);
// // 					if (target && !target->IsDead() && target->IsPlayer() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						//for (i = 0; i < 2; i++)
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element);
// // 						if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 							target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 					}
// // 				}
// // 				if ((abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
// // 			}
// //
// // 			for (iy = dY - spell->m_vRange; iy <= dY + spell->m_vRange; iy++)
// // 				for (ix = dX - spell->m_hRange; ix <= dX + spell->m_hRange; ix++) {
// // 					target = pMap->GetOwner(ix, iy);
// // 					if (target && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						//for (i = 0; i < 2; i++)
// // 						gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, sX, sY, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element);
// // 						if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 							target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 					}
// //
// // 					target = pMap->GetDeadOwner(ix, iy);
// // 					if (target && !target->IsDead() && target->IsPlayer() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						//for (i = 0; i < 2; i++)
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element);
// // 						if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 							target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 					}
// // 				}
// //
// // 			for (iy = dY - spell->m_vRange; iy <= dY + spell->m_vRange; iy++)
// // 				for (ix = dX - spell->m_hRange; ix <= dX + spell->m_hRange; ix++) {
// // 					target = pMap->GetOwner(ix, iy);
// // 					if (target && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						//for (i = 0; i < 2; i++)
// // 						gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, sX, sY, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element);
// // 						if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 							target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 					}
// //
// // 					target = pMap->GetDeadOwner(ix, iy);
// // 					if (target && !target->IsDead() && target->IsPlayer() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						//for (i = 0; i < 2; i++)
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element);
// // 						if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 							target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 					}
// // 				}
// //
// // 			target = pMap->GetOwner(dX, dY);
// // 			if (target && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 				//for (i = 0; i < 2; i++)
// // 				gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element);
// // 				if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 					target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 			}
// //
// // 			target = pMap->GetDeadOwner(dX, dY);
// // 			if (target && !target->IsDead() && target->IsPlayer()  && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false){
// // 				//for (i = 0; i < 2; i++)
// // 				gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element);
// // 				if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 					target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 			}
// //
// // 			target = pMap->GetDeadOwner(dX, dY);
// // 			if (target && !target->IsDead() && target->IsPlayer()  && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false){
// // 				//for (i = 0; i < 2; i++)
// // 				gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element);
// // 				if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 					target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 			}
// //
// // 			break;
// //
// //
// 		case MAGICTYPE_DAMAGE_SPOT:
// 			//test
// 			target = map->GetOwner(dX, dY).get();
// 			if (target && target->IsPlayer()) map->gserver->SendNotifyMsg(0, (Client*)target, NOTIFY_NOTICEMSG, 0, 0, 0, "Magic cast");
// 			if (target && target->CheckResistMagic(direction, magicResist) == false)
// 				map->gserver->Effect_Damage_Spot(unit, target, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS]/* + iWeatherBonus*/, true, spell->element);
// 
// 			target = map->GetDeadOwner(dX, dY).get();
// 			if (target && target->IsPlayer() && target->health > 0 ) {
// 				if (target->CheckResistMagic(direction, magicResist) == false)
// 					map->gserver->Effect_Damage_Spot(unit, target, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS]/* + iWeatherBonus*/, true, spell->element);
// 			}
// 			break;
// 
// 		}
// 	}
// //
// // 		case MAGICTYPE_HPUP_SPOT:
// // 			pMap->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
// // 			gserver->Effect_HpUp_Spot(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS]);
// // 			break;
// //
// // 		case MAGICTYPE_DAMAGE_AREA:
// // 			pMap->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
// // 			if (CheckResistingMagicSuccess(m_cDir, sOwnerH, cOwnerType, iResult) == false)
// // 				gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element);
// //
// // 			pMap->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
// // 			if ( (cOwnerType == OWNERTYPE_PLAYER) && (g_clientList[sOwnerH] != NULL) &&
// // 				(g_clientList[sOwnerH]->m_iHP > 0) ) {
// // 					if (CheckResistingMagicSuccess(m_cDir, sOwnerH, cOwnerType, iResult) == false)
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element);
// // 			}
// //
// // 			for (iy = dY - spell->m_vRange; iy <= dY + spell->m_vRange; iy++)
// // 				for (ix = dX - spell->m_hRange; ix <= dX + spell->m_hRange; ix++) {
// // 					pMap->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
// // 					if (CheckResistingMagicSuccess(m_cDir, sOwnerH, cOwnerType, iResult) == false)
// // 						gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, dX, dY, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element);
// //
// // 					pMap->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
// // 					if ( (cOwnerType == OWNERTYPE_PLAYER) && (g_clientList[sOwnerH] != NULL) &&
// // 						(g_clientList[sOwnerH]->m_iHP > 0) ) {
// // 							if (CheckResistingMagicSuccess(m_cDir, sOwnerH, cOwnerType, iResult) == false)
// // 								gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, dX, dY, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element);
// // 					}
// // 				}
// // 			break;
// //
// // 		case MAGICTYPE_DAMAGE_AREA_NOSPOT:
// // 			for (iy = dY - spell->m_vRange; iy <= dY + spell->m_vRange; iy++)
// // 				for (ix = dX - spell->m_hRange; ix <= dX + spell->m_hRange; ix++) {
// // 					pMap->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
// // 					if (CheckResistingMagicSuccess(m_cDir, sOwnerH, cOwnerType, iResult) == false)
// // 						gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, dX, dY, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element);
// //
// // 					pMap->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
// // 					if ( (cOwnerType == OWNERTYPE_PLAYER) && (g_clientList[sOwnerH] != NULL) &&
// // 						(g_clientList[sOwnerH]->m_iHP > 0) ) {
// // 							if (CheckResistingMagicSuccess(m_cDir, sOwnerH, cOwnerType, iResult) == false)
// // 								gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, dX, dY, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element);
// // 					}
// // 				}
// // 			break;
// //
// // 		case MAGICTYPE_SPDOWN_AREA:
// // 			pMap->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
// // 			if (CheckResistingMagicSuccess(m_cDir, sOwnerH, cOwnerType, iResult) == false)
// // 				gserver->Effect_SpDown_Spot(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS]);
// // 			for (iy = dY - spell->m_vRange; iy <= dY + spell->m_vRange; iy++)
// // 				for (ix = dX - spell->m_hRange; ix <= dX + spell->m_hRange; ix++) {
// // 					pMap->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
// // 					if (CheckResistingMagicSuccess(m_cDir, sOwnerH, cOwnerType, iResult) == false)
// // 						gserver->Effect_SpDown_Spot(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
// // 				}
// // 			break;
// //
// // 		case MAGICTYPE_SPUP_AREA:
// // 			spell->Effect_RecSP(this, target, spell->m_sValue[MAGICV_THROW],
// // 				spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS]);
// //
// // 			for (iy = dY - spell->m_vRange; iy <= dY + spell->m_vRange; iy++)
// // 			{
// // 				for (ix = dX - spell->m_hRange; ix <= dX + spell->m_hRange; ix++) {
// // 					target = pMap->GetOwner(ix, iy);
// // 					spell->Effect_RecSP(this, target, spell->m_sValue[MAGICV_LINEARTHROW],
// // 						spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
// // 				}
// // 			}
// // 			break;
// //
// // 		case MAGICTYPE_DAMAGE_AREA_NOSPOT_SPDOWN:
// // 			for (iy = dY - spell->m_vRange; iy <= dY + spell->m_vRange; iy++)
// // 			{
// // 				for (ix = dX - spell->m_hRange; ix <= dX + spell->m_hRange; ix++)
// // 				{
// // 					pMap->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
// // 					if (CheckResistingMagicSuccess(m_cDir, sOwnerH, cOwnerType, iResult) == false) {
// // 						gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, dX, dY, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, false, spell->m_element, spell);
// // 						gserver->Effect_SpDown_Spot(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
// // 					}
// //
// // 					pMap->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
// // 					if ( (cOwnerType == OWNERTYPE_PLAYER) && (g_clientList[sOwnerH] != NULL) &&
// // 						(g_clientList[sOwnerH]->m_iHP > 0) ) {
// //
// // 							if (CheckResistingMagicSuccess(m_cDir, sOwnerH, cOwnerType, iResult) == false) {
// // 								gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, dX, dY, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, false, spell->m_element, spell);
// // 								gserver->Effect_SpDown_Spot(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
// // 							}
// // 					}
// // 				}
// // 			}
// // 			break;
// //
// // 		case MAGICTYPE_ICE:
// //
// // 			for (iy = dY - spell->m_vRange; iy <= dY + spell->m_vRange; iy++)
// // 				for (ix = dX - spell->m_hRange; ix <= dX + spell->m_hRange; ix++) {
// //
// // 					target = pMap->GetOwner(ix, iy);
// // 					if(target && !target->IsDead() && ((target->IsPlayer() && !target->IsNeutral()) ||
// // 						(target->IsNPC() && g_npcList[target->m_handle]->m_bIsSummoned))) {
// // 						if ((CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 							&& (checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)) {
// // 							//g_game->Effect_Damage_Spot(iClientH, OWNERTYPE_PLAYER, sOwnerH, cOwnerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element);
// //
// // 							gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC,  target->m_handle, target->m_ownerType, dX, dY, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element);
// // 							if (target && !target->IsDead())
// // 								target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 						}
// // 					} // if
// //
// // 					target = pMap->GetDeadOwner(ix, iy);
// // 					if(target && !target->IsDead() && target->IsPlayer() && !target->IsNeutral()){
// // 						if ((CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 							&& (checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)) {
// //
// // 								//g_game->Effect_Damage_Spot(iClientH, OWNERTYPE_PLAYER, sOwnerH, cOwnerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element);
// // 								gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element);
// // 								if (target && !target->IsDead())
// // 									target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 						}
// // 					} // if
// // 				} // for
// // 			break;
// //
// // 		case MAGICTYPE_CREATE_DYNAMIC:
// // 			switch (spell->m_sValue[MAGICV_DYNAMICOBJ])
// // 			{
// // 			case DYNAMICOBJECT_PCLOUD_BEGIN:
// // 			case DYNAMICOBJECT_FIRE:
// // 			case DYNAMICOBJECT_SPIKE:
// // 				switch (spell->m_sValue[MAGICV_DYNAMICTYPE])
// // 				{
// // 				case 1:
// // 					// wall - type
// // 					m_cDir = CMisc::cGetNextMoveDir(m_sX, m_sY, dX, dY);
// //
// // 					short rx, ry;
// // 					switch (m_cDir)
// // 					{
// // 					case 1:	rx = 1; ry = 0;   break;
// // 					case 2: rx = 1; ry = 1;   break;
// // 					case 3: rx = 0; ry = 1;   break;
// // 					case 4: rx = -1; ry = 1;  break;
// // 					case 5: rx = 1; ry = 0;   break;
// // 					case 6: rx = -1; ry = -1; break;
// // 					case 7: rx = 0; ry = -1;  break;
// // 					case 8: rx = 1; ry = -1;  break;
// // 					}
// //
// // 					gserver->iAddDynamicObjectList(m_handle, OWNERTYPE_NPC_INDIRECT, spell->m_sValue[MAGICV_DYNAMICOBJ], m_cMapIndex,
// // 						dX, dY, spell->m_dwLastTime*1000);
// //
// // 					for (i = 1; i <= spell->m_hRange; i++) {
// // 						gserver->iAddDynamicObjectList(m_handle, OWNERTYPE_NPC_INDIRECT, spell->m_sValue[MAGICV_DYNAMICOBJ], m_cMapIndex,
// // 							dX + i*rx, dY + i*ry, spell->m_dwLastTime*1000);
// //
// // 						gserver->iAddDynamicObjectList(m_handle, OWNERTYPE_NPC_INDIRECT, spell->m_sValue[MAGICV_DYNAMICOBJ], m_cMapIndex,
// // 							dX - i*rx, dY - i*ry, spell->m_dwLastTime*1000);
// // 					}
// // 					break;
// //
// // 				case 2:
// // 					// Field - Type
// // 					bool bFlag = false;
// // 					int cx, cy;
// // 					for (iy = dY - spell->m_vRange; iy <= dY + spell->m_vRange; iy++) {
// // 						for (ix = dX - spell->m_hRange; ix <= dX + spell->m_hRange; ix++)
// // 							gserver->iAddDynamicObjectList(m_handle, OWNERTYPE_NPC_INDIRECT, spell->m_sValue[MAGICV_DYNAMICOBJ], m_cMapIndex,
// // 							ix, iy, spell->m_dwLastTime*1000, spell->m_sValue[MAGICV_THROW]);
// // 					}
// // 					break;
// // 				}
// // 				break;
// //
// // 			case DYNAMICOBJECT_ICESTORM:
// // 				gserver->iAddDynamicObjectList(m_handle, OWNERTYPE_NPC_INDIRECT, spell->m_sValue[MAGICV_DYNAMICOBJ], m_cMapIndex,
// // 					dX, dY, spell->m_dwLastTime*1000, 100);
// // 				break;
// // 			}
// // 			break;
// //
// // 		case MAGICTYPE_DAMAGE_LINEAR_SPDOWN:
// // 			sX = m_sX;
// // 			sY = m_sY;
// //
// // 			for (i = 2; i < 13; i++) {
// // 				iErr = 0;
// // 				CMisc::GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);
// // 				for(int pNo = 0; pNo < 5; pNo++){
// // 					target = pMap->GetOwner(tX+crossPnts[pNo][0], tY+crossPnts[pNo][1]);
// // 					if (target && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// //
// // 						gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, sX, sY, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element, spell);
// // 						if (target && !target->IsDead() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 							gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element, spell);
// // 							gserver->Effect_SpDown_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
// // 						}
// // 					}
// //
// // 					target = pMap->GetDeadOwner(tX+crossPnts[pNo][0], tY+crossPnts[pNo][1]);
// // 					if (target && !target->IsDead() && target->IsPlayer() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false){
// //
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element, spell);
// // 						if (target && !target->IsDead() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 							gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element, spell);
// // 							gserver->Effect_SpDown_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
// // 						}
// // 					}
// // 				}
// // 				if ((abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
// // 			}
// //
// //
// // 			for (iy = dY - spell->m_vRange; iy <= dY + spell->m_vRange; iy++)
// // 				for (ix = dX - spell->m_hRange; ix <= dX + spell->m_hRange; ix++) {
// // 					target = pMap->GetOwner(ix, iy);
// // 					if (target && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// //
// // 						gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, dX, dY, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element, spell);
// // 						if (target && !target->IsDead() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 							gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element, spell);
// // 							gserver->Effect_SpDown_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
// // 						}
// // 					}
// //
// // 					target = pMap->GetDeadOwner(ix, iy);
// // 					if (target && !target->IsDead() && target->IsPlayer() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false){
// //
// // 						gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, dX, dY, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element, spell);
// // 						if (target && !target->IsDead() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 							gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element, spell);
// // 							gserver->Effect_SpDown_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
// // 						}
// // 					}
// // 				}
// //
// // 				target = pMap->GetOwner(dX, dY);
// // 				if (target && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 					gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element, spell);
// //
// // 					if (target && !target->IsDead() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element, spell);
// // 						gserver->Effect_SpDown_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
// // 					}
// // 				}
// //
// // 				target = pMap->GetDeadOwner(dX, dY);
// // 				if (target && !target->IsDead() && target->IsPlayer() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false){
// //
// // 					gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, false, spell->m_element, spell);
// // 					if (target && !target->IsDead() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element, spell);
// // 						gserver->Effect_SpDown_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
// // 					}
// // 				}
// //
// // 			break;
// //
// // 		case MAGICTYPE_SOTG_LINEAR: // Strike of the ghosts (SP DOWN)
// // 			sX = m_sX;
// // 			sY = m_sY;
// //
// // 			for (i = 2; i < 13; i++) {
// // 				iErr = 0;
// // 				CMisc::GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);
// // 				for(int pNo = 0; pNo < 5; pNo++){
// // 					target = pMap->GetOwner(tX+crossPnts[pNo][0], tY+crossPnts[pNo][1]);
// // 					if (target && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// //
// // 						gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, sX, sY, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element, spell);
// // 						if (target && !target->IsDead() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 							gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element, spell);
// // 							gserver->Effect_SpDown_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
// // 						}
// // 					}
// //
// // 					target = pMap->GetDeadOwner(tX+crossPnts[pNo][0], tY+crossPnts[pNo][1]);
// // 					if (target && !target->IsDead() && target->IsPlayer() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false){
// //
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element, spell);
// // 						if (target && !target->IsDead() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 							gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element, spell);
// // 							gserver->Effect_SpDown_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
// // 						}
// // 					}
// // 				}
// // 				if ((abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
// // 			}
// //
// //
// // 			for (iy = dY - spell->m_vRange; iy <= dY + spell->m_vRange; iy++)
// // 				for (ix = dX - spell->m_hRange; ix <= dX + spell->m_hRange; ix++) {
// // 					target = pMap->GetOwner(ix, iy);
// // 					if (target && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// //
// // 						gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, dX, dY, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element, spell);
// // 						if (target && !target->IsDead() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 							gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element, spell);
// // 							gserver->Effect_SpDown_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
// // 						}
// // 					}
// //
// // 					target = pMap->GetDeadOwner(ix, iy);
// // 					if (target && !target->IsDead() && target->IsPlayer() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false){
// //
// // 						gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, dX, dY, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element, spell);
// // 						if (target && !target->IsDead() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 							gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element, spell);
// // 							gserver->Effect_SpDown_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
// // 						}
// // 					}
// // 				}
// //
// // 				for (iy = dY - spell->m_vRange; iy <= dY + spell->m_vRange; iy++)
// // 				for (ix = dX - spell->m_hRange; ix <= dX + spell->m_hRange; ix++) {
// // 					target = pMap->GetOwner(ix, iy);
// // 					if (target && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// //
// // 						gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, dX, dY, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element, spell);
// // 						if (target && !target->IsDead() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 							gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element, spell);
// // 							gserver->Effect_SpDown_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
// // 						}
// // 					}
// //
// // 					target = pMap->GetDeadOwner(ix, iy);
// // 					if (target && !target->IsDead() && target->IsPlayer() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false){
// //
// // 						gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, dX, dY, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element, spell);
// // 						if (target && !target->IsDead() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 							gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element, spell);
// // 							gserver->Effect_SpDown_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
// // 						}
// // 					}
// // 				}
// //
// // 				for (iy = dY - spell->m_vRange; iy <= dY + spell->m_vRange; iy++)
// // 				for (ix = dX - spell->m_hRange; ix <= dX + spell->m_hRange; ix++) {
// // 					target = pMap->GetOwner(ix, iy);
// // 					if (target && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// //
// // 						gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, dX, dY, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element, spell);
// // 						if (target && !target->IsDead() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 							gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element, spell);
// // 							gserver->Effect_SpDown_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
// // 						}
// // 					}
// //
// // 					target = pMap->GetDeadOwner(ix, iy);
// // 					if (target && !target->IsDead() && target->IsPlayer() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false){
// //
// // 						gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, dX, dY, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element, spell);
// // 						if (target && !target->IsDead() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 							gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element, spell);
// // 							gserver->Effect_SpDown_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
// // 						}
// // 					}
// // 				}
// //
// // 				for (iy = dY - spell->m_vRange; iy <= dY + spell->m_vRange; iy++)
// // 				for (ix = dX - spell->m_hRange; ix <= dX + spell->m_hRange; ix++) {
// // 					target = pMap->GetOwner(ix, iy);
// // 					if (target && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// //
// // 						gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, dX, dY, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element, spell);
// // 						if (target && !target->IsDead() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 							gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element, spell);
// // 							gserver->Effect_SpDown_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
// // 						}
// // 					}
// //
// // 					target = pMap->GetDeadOwner(ix, iy);
// // 					if (target && !target->IsDead() && target->IsPlayer() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false){
// //
// // 						gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, dX, dY, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element, spell);
// // 						if (target && !target->IsDead() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 							gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element, spell);
// // 							gserver->Effect_SpDown_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
// // 						}
// // 					}
// // 				}
// //
// // 				target = pMap->GetOwner(dX, dY);
// // 				if (target && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 					gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element, spell);
// //
// // 					if (target && !target->IsDead() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element, spell);
// // 						gserver->Effect_SpDown_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
// // 					}
// // 				}
// //
// // 				target = pMap->GetDeadOwner(dX, dY);
// // 				if (target && !target->IsDead() && target->IsPlayer() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false){
// //
// // 					gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, false, spell->m_element, spell);
// // 					if (target && !target->IsDead() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element, spell);
// // 						gserver->Effect_SpDown_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
// // 					}
// // 				}
// //
// // 				target = pMap->GetDeadOwner(dX, dY);
// // 				if (target && !target->IsDead() && target->IsPlayer() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false){
// //
// // 					gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, false, spell->m_element, spell);
// // 					if (target && !target->IsDead() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element, spell);
// // 						gserver->Effect_SpDown_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
// // 					}
// // 				}
// //
// // 				target = pMap->GetDeadOwner(dX, dY);
// // 				if (target && !target->IsDead() && target->IsPlayer() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false){
// //
// // 					gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, false, spell->m_element, spell);
// // 					if (target && !target->IsDead() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element, spell);
// // 						gserver->Effect_SpDown_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
// // 					}
// // 				}
// //
// // 				target = pMap->GetDeadOwner(dX, dY);
// // 				if (target && !target->IsDead() && target->IsPlayer() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false){
// //
// // 					gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, false, spell->m_element, spell);
// // 					if (target && !target->IsDead() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element, spell);
// // 						gserver->Effect_SpDown_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
// // 					}
// // 				}
// // 			break;
// //
// // 		case MAGICTYPE_COTG_LINEAR:
// // 			sX = m_sX;
// // 			sY = m_sY;
// //
// // 			for (i = 2; i < 13; i++) {
// // 				iErr = 0;
// // 				CMisc::GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);
// //
// // 				for(int pNo=0; pNo < 5; pNo++){
// // 					target = pMap->GetOwner(tX+crossPnts[pNo][0], tY+crossPnts[pNo][1]);
// // 					if (target && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						//for (i = 0; i < 2; i++)
// // 						gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, sX, sY, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element);
// // 						if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 							target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 					}
// //
// // 					target = pMap->GetDeadOwner(tX+crossPnts[pNo][0], tY+crossPnts[pNo][1]);
// // 					if (target && !target->IsDead() && target->IsPlayer() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						//for (i = 0; i < 2; i++)
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element);
// // 						if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 							target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 					}
// // 				}
// // 				if ((abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
// // 			}
// //
// // 			for (iy = dY - spell->m_vRange; iy <= dY + spell->m_vRange; iy++)
// // 				for (ix = dX - spell->m_hRange; ix <= dX + spell->m_hRange; ix++) {
// // 					target = pMap->GetOwner(ix, iy);
// // 					if (target && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						//for (i = 0; i < 2; i++)
// // 						gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, sX, sY, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element);
// // 						if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 							target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 					}
// //
// // 					target = pMap->GetDeadOwner(ix, iy);
// // 					if (target && !target->IsDead() && target->IsPlayer() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						//for (i = 0; i < 2; i++)
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element);
// // 						if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 							target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 					}
// // 				}
// //
// // 			for (iy = dY - spell->m_vRange; iy <= dY + spell->m_vRange; iy++)
// // 				for (ix = dX - spell->m_hRange; ix <= dX + spell->m_hRange; ix++) {
// // 					target = pMap->GetOwner(ix, iy);
// // 					if (target && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						//for (i = 0; i < 2; i++)
// // 						gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, sX, sY, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element);
// // 						if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 							target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 					}
// //
// // 					target = pMap->GetDeadOwner(ix, iy);
// // 					if (target && !target->IsDead() && target->IsPlayer() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						//for (i = 0; i < 2; i++)
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element);
// // 						if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 							target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 					}
// // 				}
// //
// // 			for (iy = dY - spell->m_vRange; iy <= dY + spell->m_vRange; iy++)
// // 				for (ix = dX - spell->m_hRange; ix <= dX + spell->m_hRange; ix++) {
// // 					target = pMap->GetOwner(ix, iy);
// // 					if (target && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						//for (i = 0; i < 2; i++)
// // 						gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, sX, sY, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element);
// // 						if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 							target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 					}
// //
// // 					target = pMap->GetDeadOwner(ix, iy);
// // 					if (target && !target->IsDead() && target->IsPlayer() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						//for (i = 0; i < 2; i++)
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element);
// // 						if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 							target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 					}
// // 				}
// //
// // 			for (iy = dY - spell->m_vRange; iy <= dY + spell->m_vRange; iy++)
// // 				for (ix = dX - spell->m_hRange; ix <= dX + spell->m_hRange; ix++) {
// // 					target = pMap->GetOwner(ix, iy);
// // 					if (target && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						//for (i = 0; i < 2; i++)
// // 						gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, sX, sY, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element);
// // 						if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 							target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 					}
// //
// // 					target = pMap->GetDeadOwner(ix, iy);
// // 					if (target && !target->IsDead() && target->IsPlayer() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						//for (i = 0; i < 2; i++)
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element);
// // 						if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 							target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 					}
// // 				}
// //
// // 			target = pMap->GetOwner(dX, dY);
// // 			if (target && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 				//for (i = 0; i < 2; i++)
// // 				gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element);
// // 				if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 					target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 			}
// //
// // 			target = pMap->GetDeadOwner(dX, dY);
// // 			if (target && !target->IsDead() && target->IsPlayer()  && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false){
// // 				//for (i = 0; i < 2; i++)
// // 				gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element);
// // 				if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 					target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 			}
// //
// // 			target = pMap->GetDeadOwner(dX, dY);
// // 			if (target && !target->IsDead() && target->IsPlayer()  && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false){
// // 				//for (i = 0; i < 2; i++)
// // 				gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element);
// // 				if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 					target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 			}
// //
// // 			target = pMap->GetDeadOwner(dX, dY);
// // 			if (target && !target->IsDead() && target->IsPlayer()  && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false){
// // 				//for (i = 0; i < 2; i++)
// // 				gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element);
// // 				if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 					target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 			}
// //
// // 			target = pMap->GetDeadOwner(dX, dY);
// // 			if (target && !target->IsDead() && target->IsPlayer()  && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false){
// // 				//for (i = 0; i < 2; i++)
// // 				gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element);
// // 				if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 					target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 			}
// //
// // 			break;
// //
// // 		case MAGICTYPE_FSW_LINEAR:
// // 			sX = m_sX;
// // 			sY = m_sY;
// //
// // 			for (i = 2; i < 13; i++) {
// // 				iErr = 0;
// // 				CMisc::GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);
// //
// // 				for(int pNo=0; pNo < 5; pNo++){
// // 					pMap->GetOwner(&sOwnerH, &cOwnerType, tX+crossPnts[pNo][0], tY+crossPnts[pNo][1]);
// // 					if (CheckResistingMagicSuccess(m_cDir, sOwnerH, cOwnerType, iResult) == false)
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element);
// //
// // 					pMap->GetDeadOwner(&sOwnerH, &cOwnerType, tX+crossPnts[pNo][0], tY+crossPnts[pNo][1]);
// // 					if ( (cOwnerType == OWNERTYPE_PLAYER) && (g_clientList[sOwnerH] != NULL) &&
// // 						(g_clientList[sOwnerH]->m_iHP > 0) ) {
// // 							if (CheckResistingMagicSuccess(m_cDir, sOwnerH, cOwnerType, iResult) == false)
// // 								gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARRANGE] + iWeatherBonus, false, spell->m_element);
// // 					}
// // 				}
// // 				if ( (abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
// // 			}
// //
// // 			for (iy = dY - spell->m_vRange; iy <= dY + spell->m_vRange; iy++)
// // 				for (ix = dX - spell->m_hRange; ix <= dX + spell->m_hRange; ix++) {
// // 					pMap->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
// // 					if (CheckResistingMagicSuccess(m_cDir, sOwnerH, cOwnerType, iResult) == false)
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARRANGE] + iWeatherBonus, false, spell->m_element);
// //
// // 					pMap->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
// // 					if ( (cOwnerType == OWNERTYPE_PLAYER) && (g_clientList[sOwnerH] != NULL) &&
// // 						(g_clientList[sOwnerH]->m_iHP > 0) ) {
// // 							if (CheckResistingMagicSuccess(m_cDir, sOwnerH, cOwnerType, iResult) == false)
// // 								gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARRANGE] + iWeatherBonus, false, spell->m_element);
// // 					}
// // 				}
// //
// // 			for (iy = dY - spell->m_vRange; iy <= dY + spell->m_vRange; iy++)
// // 				for (ix = dX - spell->m_hRange; ix <= dX + spell->m_hRange; ix++) {
// // 					pMap->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
// // 					if (CheckResistingMagicSuccess(m_cDir, sOwnerH, cOwnerType, iResult) == false)
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARRANGE] + iWeatherBonus, false, spell->m_element);
// //
// // 					pMap->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
// // 					if ( (cOwnerType == OWNERTYPE_PLAYER) && (g_clientList[sOwnerH] != NULL) &&
// // 						(g_clientList[sOwnerH]->m_iHP > 0) ) {
// // 							if (CheckResistingMagicSuccess(m_cDir, sOwnerH, cOwnerType, iResult) == false)
// // 								gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARRANGE] + iWeatherBonus, false, spell->m_element);
// // 					}
// // 				}
// //
// // 			for (iy = dY - spell->m_vRange; iy <= dY + spell->m_vRange; iy++)
// // 				for (ix = dX - spell->m_hRange; ix <= dX + spell->m_hRange; ix++) {
// // 					pMap->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
// // 					if (CheckResistingMagicSuccess(m_cDir, sOwnerH, cOwnerType, iResult) == false)
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARRANGE] + iWeatherBonus, false, spell->m_element);
// //
// // 					pMap->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
// // 					if ( (cOwnerType == OWNERTYPE_PLAYER) && (g_clientList[sOwnerH] != NULL) &&
// // 						(g_clientList[sOwnerH]->m_iHP > 0) ) {
// // 							if (CheckResistingMagicSuccess(m_cDir, sOwnerH, cOwnerType, iResult) == false)
// // 								gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARRANGE] + iWeatherBonus, false, spell->m_element);
// // 					}
// // 				}
// //
// // 			// dX, dY
// // 			pMap->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
// // 			if (CheckResistingMagicSuccess(m_cDir, sOwnerH, cOwnerType, iResult) == false)
// // 				gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, false, spell->m_element);
// //
// // 			pMap->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
// // 			if ( (cOwnerType == OWNERTYPE_PLAYER) && (g_clientList[sOwnerH] != NULL) &&
// // 				(g_clientList[sOwnerH]->m_iHP > 0) ) {
// // 					if (CheckResistingMagicSuccess(m_cDir, sOwnerH, cOwnerType, iResult) == false)
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, false, spell->m_element);
// // 			}
// //
// // 			pMap->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
// // 			if ( (cOwnerType == OWNERTYPE_PLAYER) && (g_clientList[sOwnerH] != NULL) &&
// // 				(g_clientList[sOwnerH]->m_iHP > 0) ) {
// // 					if (CheckResistingMagicSuccess(m_cDir, sOwnerH, cOwnerType, iResult) == false)
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, false, spell->m_element);
// // 			}
// //
// // 			pMap->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
// // 			if ( (cOwnerType == OWNERTYPE_PLAYER) && (g_clientList[sOwnerH] != NULL) &&
// // 				(g_clientList[sOwnerH]->m_iHP > 0) ) {
// // 					if (CheckResistingMagicSuccess(m_cDir, sOwnerH, cOwnerType, iResult) == false)
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, sOwnerH, cOwnerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, false, spell->m_element);
// // 			}
// //
// // 			break;
// //
// // 		case MAGICTYPE_MB_LINEAR:
// // 			sX = m_sX;
// // 			sY = m_sY;
// //
// // 			for (i = 2; i < 13; i++) {
// // 				iErr = 0;
// // 				CMisc::GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);
// //
// // 				for(int pNo=0; pNo < 5; pNo++){
// // 					target = pMap->GetOwner(tX+crossPnts[pNo][0], tY+crossPnts[pNo][1]);
// // 					if (target && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						//for (i = 0; i < 2; i++)
// // 						gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, sX, sY, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element);
// // 						if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 							target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 					}
// //
// // 					target = pMap->GetDeadOwner(tX+crossPnts[pNo][0], tY+crossPnts[pNo][1]);
// // 					if (target && !target->IsDead() && target->IsPlayer() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						//for (i = 0; i < 2; i++)
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element);
// // 						if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 							target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 					}
// // 				}
// // 				if ((abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
// // 			}
// //
// // 			for (iy = dY - spell->m_vRange; iy <= dY + spell->m_vRange; iy++)
// // 				for (ix = dX - spell->m_hRange; ix <= dX + spell->m_hRange; ix++) {
// // 					target = pMap->GetOwner(ix, iy);
// // 					if (target && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						//for (i = 0; i < 2; i++)
// // 						gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, sX, sY, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element);
// // 						if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 							target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 					}
// //
// // 					target = pMap->GetDeadOwner(ix, iy);
// // 					if (target && !target->IsDead() && target->IsPlayer() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						//for (i = 0; i < 2; i++)
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element);
// // 						if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 							target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 					}
// // 				}
// //
// // 			for (iy = dY - spell->m_vRange; iy <= dY + spell->m_vRange; iy++)
// // 				for (ix = dX - spell->m_hRange; ix <= dX + spell->m_hRange; ix++) {
// // 					target = pMap->GetOwner(ix, iy);
// // 					if (target && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						//for (i = 0; i < 2; i++)
// // 						gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, sX, sY, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element);
// // 						if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 							target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 					}
// //
// // 					target = pMap->GetDeadOwner(ix, iy);
// // 					if (target && !target->IsDead() && target->IsPlayer() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						//for (i = 0; i < 2; i++)
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element);
// // 						if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 							target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 					}
// // 				}
// //
// // 			for (iy = dY - spell->m_vRange; iy <= dY + spell->m_vRange; iy++)
// // 				for (ix = dX - spell->m_hRange; ix <= dX + spell->m_hRange; ix++) {
// // 					target = pMap->GetOwner(ix, iy);
// // 					if (target && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						//for (i = 0; i < 2; i++)
// // 						gserver->Effect_Damage_Spot_DamageMove(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, sX, sY, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element);
// // 						if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 							target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 					}
// //
// // 					target = pMap->GetDeadOwner(ix, iy);
// // 					if (target && !target->IsDead() && target->IsPlayer() && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 						//for (i = 0; i < 2; i++)
// // 						gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + iWeatherBonus, false, spell->m_element);
// // 						if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 							target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 					}
// // 				}
// //
// // 			target = pMap->GetOwner(dX, dY);
// // 			if (target && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false) {
// // 				//for (i = 0; i < 2; i++)
// // 				gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element);
// // 				if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 					target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 			}
// //
// // 			target = pMap->GetDeadOwner(dX, dY);
// // 			if (target && !target->IsDead() && target->IsPlayer()  && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false){
// // 				//for (i = 0; i < 2; i++)
// // 				gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element);
// // 				if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 					target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 			}
// //
// // 			target = pMap->GetDeadOwner(dX, dY);
// // 			if (target && !target->IsDead() && target->IsPlayer()  && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false){
// // 				//for (i = 0; i < 2; i++)
// // 				gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element);
// // 				if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 					target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 			}
// //
// // 			target = pMap->GetDeadOwner(dX, dY);
// // 			if (target && !target->IsDead() && target->IsPlayer()  && CheckResistingMagicSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false){
// // 				//for (i = 0; i < 2; i++)
// // 				gserver->Effect_Damage_Spot(m_handle, OWNERTYPE_NPC, target->m_handle, target->m_ownerType, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + iWeatherBonus, true, spell->m_element);
// // 				if(target && !target->IsDead() && checkResistingIceSuccess(m_cDir, target->m_handle, target->m_ownerType, iResult) == false)
// // 					target->AddMagicEffect(spell->m_sType, spell->m_sValue[MAGICV_ICEDURATION]);
// // 			}
// //
// // 			break;
// // 		}
// // 	}
// // 	else {
// //
// //
// // 	}
// //
// // NMH_NOEFFECT:;
// //
// 	mana -= spell->manaCost;
// 	if (mana < 0)
// 		mana = 0;
// 
// 	map->gserver->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, COMMONTYPE_MAGIC, map, x, y, dX, dY, (magicType + 100), Type());

}

int Npc::GetGenLevel()
{
	switch (Type())
	{
	case 10: // Slime
	case 16: // Giant-Ant
	case 22: // Amphis
	case 55: // Rabbit
	case 56: // Cat
		return 1;

	case 14: // Orc
	case 18: // Zombie
	case 17: // Scorpion
	case 11: // Skeleton
		return 2;

	case 23: // clay-golem
	case 12: // stone-golem
		return 3;

	case 61: // rudolph
	case 27: // hellhound
	case 57: // Giant Frog
	case 80: // Tentocle
		return 4;

	case 60: // Cannibal-Plant
	case 28: // Troll
	case 13: // Cyclops
	case 53: // Beholder
	case 33: // WereWolf
	case 48: // Stalker
		return 5;

	case 54: // Dark-Elf
	case 65: // Ice-Golem
	case 62: // Direboar
	case 29: // Orge
	case 72: // Claw Turtle
		return 6;


	case 76: // Giant-Plant
	case 58: // Mountain-Giant
		return 7;

	case 74: // Giant-Crayfish
	case 63: // Frost
	case 79: // Nizie
	case 30: // Liche
	case 75: // Giant Lizard
	case 77: // MasterMage-Orc
	case 59: // Ettin
	case 78: // Minotaurus
		return 8;

	case 31: // Demon
	case 32: // Unicorn
	case 52: // Gagoyle
	case 71: // Centaurus
	case 70: // Barlog
		return 9;

	case 49: // Hellclaw
	case 50: // Tigerworm
	case 122: // Enraged Troll
	case 123: // Enraged Cyclops
	case 124: // Enraged Stalker
	case 125: // Enraged Gargoyle
	case 126: // Enraged Hellclaw
	case 127: // Enraged Tigerworm
	//case 103: // Orc Lord
		return 10;
	}

	return 0;
}

void Npc::AddHP(uint64_t value)
{
	int iMaxHP = 0;// = dice(m_iHitDice, 8) + m_iHitDice;

	if (health < iMaxHP)
	{
		health += value;
		if (health > iMaxHP) health = iMaxHP;
		if (health <= 0)     health = 1;
	}

// 	if(m_sType == NPC_AS && m_cMapIndex == gserver->m_iAstoriaMapIndex &&
// 		gserver->m_astoria.get() && gserver->m_astoria->GetEventType() == ET_DESTROY_SHIELD)
// 	{
// 		gserver->m_astoria->NotifyShieldHP( this );
// 	}
}

void Npc::ReduceHP(uint64_t value)
{
	switch (m_cActionLimit)
	{
	case 0:
	case 3:
	case 5:
		if (health <= value)
			health = 0;
		else
			health -= value;
		break;
	}

// 	if(m_sType == NPC_AS && m_cMapIndex == gserver->m_iAstoriaMapIndex &&
// 		gserver->m_astoria.get() && gserver->m_astoria->GetEventType() == ET_DESTROY_SHIELD)
// 	{
// 		gserver->m_astoria->NotifyShieldHP( this );
// 	}
}

void Npc::behavior_death(std::shared_ptr<Unit> attacker, int16_t dmg)
{
//  	short  sAttackerWeapon = 1;
// 
//  	if(attacker && attacker->IsPlayer())
//  	{
// 		killer = attacker;
// 		sAttackerWeapon = ((static_cast<Client*>(attacker.get()))->m_sAppr2 & 0x0FF0) >> 4;
// 		map->NpcDeadItemGenerator(attacker, shared_from_this());
//  	}
// 
//  	_dead = true;
//  	health = 0;
//  	m_iLastDamage = dmg;
// 
//  	map->totalAliveObject--;
// 
// 	map->RemoveFromTarget(shared_from_this(), OWNERTYPE_NPC);
// 
// 	ReleaseFollowMode(shared_from_this());
// 
//  	target = 0;
//  	targetType  = 0;
// 
// 	map->gserver->SendEventToNearClient_TypeA(this, MSGID_MOTION_DYING, dmg, sAttackerWeapon, 0);
// 
//  	map->ClearOwner(x, y);
// 
// 	map->SetDeadOwner(shared_from_this(), x, y);
// 
//  	m_cBehavior = BEHAVIOR_DEAD;
// 
//  	m_sBehaviorTurnCount = 0;
// 
//  	timeDead = unixtime();
// 
// 
//  	if (map->type == MAPTYPE_NOPENALTY_NOREWARD) return;
// 
//  	// Monster kill event xRisenx
//  	/*if (OWNERTYPE_PLAYER)
//      {    if (player != NULL)
//          {    if (g_game->m_bNpcHunt)
//              {    g_game->NpcHuntPointsAdder(attacker);
//      }    }    }*/
//  	// Monster kill event xRisenx
// 	/*
//  	gserver->NpcDeadItemGenerator(m_handle, (attacker) ? attacker->m_handle : NULL, (attacker) ? attacker->m_ownerType : 0 );
// 
// 
//  	if(!m_bIsSummoned && attacker && attacker->IsPlayer())
//  	{
//  		uint64_t iExp = (m_iExp/3);
//  		if (m_iNoDieRemainExp > 0)
//  			iExp += m_iNoDieRemainExp;
// 
// 
//  		if (player->m_iAddExp != NULL) {
//  			double dTmp1 = (double)player->m_iAddExp;
//  			double dTmp2 = (double)iExp;
//  			double dTmp3 = (dTmp1/100.0f)*dTmp2;
//  			iExp += (int)dTmp3;
//  		}
// 
// 
//  		if (gserver->m_bIsCrusadeMode) {
//  			if (  (strcmp(player->m_cMapName, sideMap[NEUTRAL]) != 0)
//  				|| (strcmp(player->m_cMapName, "arefarm") != 0)
//  				|| (strcmp(player-++>m_cMapName, "elvfarm") != 0) )
//  				if (iExp > 10) iExp = iExp/4;
//  		}
// 
//  		if (player->m_iLevel > 100 )
//  		{
//  			switch (m_sType)
//  			{
//  			case NPC_RABBIT:
//  			case NPC_CAT:
//  				iExp = 0 ;
//  				break ;
//  			}
//  		}
// 
//  		gserver->GetExp(player->m_handle, iExp);
// 
//  		/*int iQuestIndex = player->m_iQuest;
//  		if (iQuestIndex)
//  		{
//  			if (gserver->m_pQuestConfigList[iQuestIndex])
//  			{
//  				switch (gserver->m_pQuestConfigList[iQuestIndex]->m_iType)
//  				{
//  				case QUESTTYPE_MONSTERHUNT:
//  					if ( (player->m_bQuestMatchFlag_Loc == true) &&
//  						(gserver->m_pQuestConfigList[iQuestIndex]->m_iTargetType == m_sType) ) {
//  							player->m_iCurQuestCount++;
//  							//char cQuestRemain = (g_game->m_pQuestConfigList[player->m_iQuest]->m_iMaxCount - player->m_iCurQuestCount);
//  							int cQuestRemain = (gserver->m_pQuestConfigList[player->m_iQuest]->m_iMaxCount - player->m_iCurQuestCount);
//  							player->Notify(NULL, NOTIFY_QUESTCOUNTER, cQuestRemain, NULL, NULL, NULL);
//  							gserver->_bCheckIsQuestCompleted(player->m_handle);
//  					}
//  					break;
//  				}
//  			}
//  		}
//  	}
// 	*/
// 	/*
//  	if(gserver->m_bHeldenianMode && pMap->m_bIsHeldenianMap)
//  	{
//  		if(attacker->IsNPC()	&& npc && npc->m_bIsSummoned && npc->m_cFollowOwnerType == OWNERTYPE_PLAYER &&
//  			!npc->IsDead()	&& g_clientList[npc->m_iFollowOwnerIndex] && !g_clientList[npc->m_iFollowOwnerIndex]->IsDead())
//  		{
//  			g_clientList[npc->m_iFollowOwnerIndex]->m_iWarContribution += m_iHitDice;
//  		}
//  	}*/
// 
//  /*	if(m_cFollowOwnerType == OWNERTYPE_NPC && g_npcList[m_iFollowOwnerIndex] &&
//  		g_npcList[m_iFollowOwnerIndex]->IsHighlyTrained())
//  	{
//  		CHTNpc * htnpc = (CHTNpc*)g_npcList[m_iFollowOwnerIndex];
//  		htnpc->Behavior_MinionDeath(this);
//  	}*/
// 
//  	/*if(gserver->m_bIsCrusadeMode)
//  	{
//  		int iConstructionPoint = 0;
//  		int iWarContribution = 0;
// 
//  		switch (m_sType)
//  		{
//  		case 1:  iConstructionPoint = 50; iWarContribution = 100; break;
//  		case 2:  iConstructionPoint = 50; iWarContribution = 100; break;
//  		case 3:  iConstructionPoint = 50; iWarContribution = 100; break;
//  		case 4:  iConstructionPoint = 50; iWarContribution = 100; break;
//  		case 5:  iConstructionPoint = 50; iWarContribution = 100; break;
//  		case 6:  iConstructionPoint = 50; iWarContribution = 100; break;
//  		case 36: iConstructionPoint = 700; iWarContribution = 4000; break;
//  		case 37: iConstructionPoint = 700; iWarContribution = 4000; break;
// 		case 38: iConstructionPoint = 500; iWarContribution = 2000; break;
//  		case 39: iConstructionPoint = 500; iWarContribution = 2000; break;
//  		case 40: iConstructionPoint = 1500; iWarContribution = 5000; break;
//  		case 41: iConstructionPoint = 5000; iWarContribution = 10000; break;
//  		case 43: iConstructionPoint =  500; iWarContribution = 1000; break;
//  		case 44: iConstructionPoint = 1000; iWarContribution = 2000; break;
//  		case 45: iConstructionPoint = 1500; iWarContribution = 3000; break;
//  		case 46: iConstructionPoint = 1000; iWarContribution = 2000; break;
//  		case 47: iConstructionPoint = 1500; iWarContribution = 3000; break;
//  		case 51: iConstructionPoint = 800; iWarContribution = 1500; break;
// 
//  		case 64:
//  			pMap->bRemoveCropsTotalSum();
//  			break;
//  		}
// 
//  		if(iConstructionPoint && attacker)
//  		{
//  			if(attacker->IsPlayer())
//  			{
//  				if (player->m_side != m_side)
//  				{
//  					player->m_iConstructionPoint += iConstructionPoint;
// 
//  					if (player->m_iConstructionPoint > MAXCONSTRUCTIONPOINT)
//  						player->m_iConstructionPoint = MAXCONSTRUCTIONPOINT;
// 
//  					player->m_iWarContribution   += iWarContribution;
//  					if (player->m_iWarContribution > MAXWARCONTRIBUTION)
//  						player->m_iWarContribution = MAXWARCONTRIBUTION;
//  					player->Notify(NULL, NOTIFY_CONSTRUCTIONPOINT, player->m_iConstructionPoint, player->m_iWarContribution, NULL, NULL);
//  				}
//  				else
//  				{
//  					player->m_iWarContribution = 0;
// 
//  					wsprintf(g_cTxt, "WarContribution: Friendly Npc Killed by player(%s)! ", player->m_cAccountName);
//  					PutLogFileList(g_cTxt, EVENT_LOGFILE);
// 
//  					player->Notify(NULL, NOTIFY_CONSTRUCTIONPOINT, player->m_iConstructionPoint, player->m_iWarContribution, NULL, NULL);
//  				}
//  			}
//  			else if(attacker->IsNPC() && npc->m_iGuildGUID)
//  			{
//  				if(npc->m_side != m_side)
//  				{
//  					for(int i = 1; i < MAXCLIENTS; i++)
//  					{
//  						if(g_clientList[i] && g_clientList[i]->m_iGuildGUID == npc->m_iGuildGUID &&
//  							g_clientList[i]->m_iCrusadeDuty == 3)
// 						{
//  							g_clientList[i]->m_iConstructionPoint += iConstructionPoint;
//  							if (g_clientList[i]->m_iConstructionPoint > MAXCONSTRUCTIONPOINT)
//  								g_clientList[i]->m_iConstructionPoint = MAXCONSTRUCTIONPOINT;
// 
//  							gserver->SendNotifyMsg(NULL, i, NOTIFY_CONSTRUCTIONPOINT, g_clientList[i]->m_iConstructionPoint, g_clientList[i]->m_iWarContribution, NULL, NULL);
// 							goto NKH_GOTOPOINT1;
// 						}
//  					}
// 
//  					char cData[120];
//  					ZeroMemory(cData, sizeof(cData));
//  					char * cp = (char *)cData;
//  					*cp = GSM_CONSTRUCTIONPOINT;
//  					cp++;
// 					int * ip = (int*)cp;
//  					*ip = npc->m_iGuildGUID;
//  					cp += 4;
//  					ip = (int*)cp;
//  					*ip = iConstructionPoint;
//  					cp += 4;
//  					gserver->bStockMsgToGateServer(cData, 9);
//  				}
//  			}
//  		}
//  	}
// 
//  NKH_GOTOPOINT1:;
// 
//  	if (m_cSpecialAbility == 7) {
//  		m_iMP = 100;
//  		m_iMagicHitRatio = 100;
//  		magicHandler(m_sX, m_sY, 30);
//  	}
//  	else if (m_cSpecialAbility == 8) {
//  		m_iMP = 100;
//  		m_iMagicHitRatio = 100;
//  		magicHandler(m_sX, m_sY, 61);
//  	}
// 
//  	if(gserver->m_bHeldenianMode && (gserver->m_iHeldenianType == 1) && pMap->m_bIsHeldenianMap)
//  	{
//  		if ((m_sType == NPC_CT) || (m_sType == NPC_AGC))
//  		{
//  			if (m_side == 1)
//  			{
//  				gserver->m_iHeldenianAresdenLeftTower--;
//  				wsprintf(g_cTxt, "Aresden Tower Broken, Left TOWER %d", gserver->m_iHeldenianAresdenLeftTower);
//  				PutLogList(g_cTxt);
//  				gserver->UpdateHeldenianStatus();
//  				if (gserver->m_iHeldenianAresdenLeftTower == 0)	gserver->HeldenianEndWarNow(gserver->m_iHeldenianType, ELVINE);
//  			}
//  			else if (m_side == 2)
//  			{
//  				gserver->m_iHeldenianElvineLeftTower--;
//  				wsprintf(g_cTxt, "Elvine Tower Broken, Left TOWER %d", gserver->m_iHeldenianElvineLeftTower);
//  				PutLogList(g_cTxt);
//  				gserver->UpdateHeldenianStatus();
//  				if (gserver->m_iHeldenianElvineLeftTower == 0)	gserver->HeldenianEndWarNow(gserver->m_iHeldenianType, ARESDEN);
// 
//  			}
//  		}*/
}

void Npc::Cast(std::shared_ptr<Unit> target, short spell)
{
	Cast(target->x, target->y, spell);
}

void Npc::Cast(short x, short y, short spell)
{
// 	magicHandler(x, y, spell);
// 	m_cDir = CMisc::cGetNextMoveDir(m_sX, m_sY, x, y);
// 	gserver->SendEventToNearClient_TypeA(m_handle, OWNERTYPE_NPC, MSGID_MOTION_ATTACK,
// 		x + _tmp_cTmpDirX[m_cDir], y + _tmp_cTmpDirY[m_cDir], 1);
}


bool Npc::Follow(std::shared_ptr<Unit> master)
{
	if(!master || master->map != map)
		return false;

	//TODO: set follow target to Unit* type
	m_cMoveType = MOVETYPE_FOLLOW;
	follow = master;
	side = master->side;

	return true;
}
void Npc::ReleaseFollowMode(std::shared_ptr<Unit> owner)
{
// 	for (std::shared_ptr<Npc> npcs : map->npclist)
// 		if (npcs != nullptr)  {
// 			if ((npcs->m_cMoveType == MOVETYPE_FOLLOW) &&
// 				(npcs->follow == owner) &&
// 				(npcs->followOwnerType == owner->OwnerType())) {
// 
// 				npcs->m_cMoveType = MOVETYPE_RANDOMWAYPOINT;
// 			}
// 		}
}
void Npc::SetTarget(std::shared_ptr<Unit> target, bool isperm)
{
	//TODO: set attack target to Unit* type
	m_cBehavior = BEHAVIOR_ATTACK;
	m_sBehaviorTurnCount = 0;
	target = target;

	permAttackMode = isperm;
}
void Npc::behavior_dead()
{
	uint64_t dwTime;

	dwTime = unixtime();
	m_sBehaviorTurnCount++;
	if (m_sBehaviorTurnCount > 5) {
		m_sBehaviorTurnCount = 0;
	}

// 	if ((dwTime - timeDead) > timeRegen)
// 		map->DeleteNpc(shared_from_this());
}
uint8_t Npc::GetNextMoveDir(short sX, short sY, short dstX, short dstY, Map* pMap, char cTurn, int * pError)
{
// 	uint8_t  cDir, cTmpDir;
// 	int   aX, aY, dX, dY;
// 	int   i, iResX, iResY;
// 
// 	if ((sX == dstX) && (sY == dstY)) return 0;
// 
// 	dX = sX;
// 	dY = sY;
// 
// 	if ((abs(dX - dstX) <= 1) && (abs(dY - dstY) <= 1)) {
// 		iResX = dstX;
// 		iResY = dstY;
// 	}
// 	else CMisc::GetPoint(dX, dY, dstX, dstY, &iResX, &iResY, pError);
// 
// 	cDir = CMisc::cGetNextMoveDir(dX, dY, iResX, iResY);
// 
// 	if (cTurn == 0)
// 		for (i = cDir; i <= cDir + 7; i++) {
// 			cTmpDir = i;
// 			if (cTmpDir > 8) cTmpDir -= 8;
// 			aX = _tmp_cTmpDirX[cTmpDir];
// 			aY = _tmp_cTmpDirY[cTmpDir];
// 			if (pMap->bGetMoveable(dX + aX, dY + aY) == true) return cTmpDir;
// 		}
// 
// 	if (cTurn == 1)
// 		for (i = cDir; i >= cDir - 7; i--) {
// 			cTmpDir = i;
// 			if (cTmpDir < 1) cTmpDir += 8;
// 			aX = _tmp_cTmpDirX[cTmpDir];
// 			aY = _tmp_cTmpDirY[cTmpDir];
// 			if (pMap->bGetMoveable(dX + aX, dY + aY) == true) return cTmpDir;
// 		}
// 
	return 0;
}

uint8_t Npc::GetNextMoveDir(short sX, short sY, short dstX, short dstY, Map* pMap, char cTurn, int * pError, short * DOType)
{
// 	uint8_t  cDir, cTmpDir;
// 	int   aX, aY, dX, dY;
// 	int   i, iResX, iResY;
// 
// 	if ((sX == dstX) && (sY == dstY)) return 0;
// 
// 	dX = sX;
// 	dY = sY;
// 
// 	if ((abs(dX - dstX) <= 1) && (abs(dY - dstY) <= 1)) {
// 		iResX = dstX;
// 		iResY = dstY;
// 	}
// 	else CMisc::GetPoint(dX, dY, dstX, dstY, &iResX, &iResY, pError);
// 
// 	cDir = CMisc::cGetNextMoveDir(dX, dY, iResX, iResY);
// 
// 	if (cTurn == 0)
// 		for (i = cDir; i <= cDir + 7; i++) {
// 			cTmpDir = i;
// 			if (cTmpDir > 8) cTmpDir -= 8;
// 			aX = _tmp_cTmpDirX[cTmpDir];
// 			aY = _tmp_cTmpDirY[cTmpDir];
// 			if (pMap->bGetMoveable(dX + aX, dY + aY, DOType) == true) return cTmpDir;
// 		}
// 
// 	if (cTurn == 1)
// 		for (i = cDir; i >= cDir - 7; i--) {
// 			cTmpDir = i;
// 			if (cTmpDir < 1) cTmpDir += 8;
// 			aX = _tmp_cTmpDirX[cTmpDir];
// 			aY = _tmp_cTmpDirY[cTmpDir];
// 			if (pMap->bGetMoveable(dX + aX, dY + aY, DOType) == true) return cTmpDir;
// 		}

	return 0;
}
