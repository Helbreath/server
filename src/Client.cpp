// Client.cpp: implementation of the Client class.
//
//////////////////////////////////////////////////////////////////////

#include "Client.h"
#include "netmessages.h"
#include "GServer.h"
//#include "skilltypes.h"
//#include "..\mgrs\GuildMgr.h"
//#include "..\mgrs\PartyMgr.h"
//#include "..\map\Map.h"
//#include "..\map\OccupyFlag.h"
//#include "combat.h"
//#include "ActionID.h"
//#include "Guild.h"
#include "Map.h"
#include "Npc.h"

extern int * g_skillSSNpoint;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Client::Client()
{
	int i;

	gserver = 0;

	m_iAddChargeCritical = 0;

	m_iAddTransMana = 0;

	_IceResist = _ParalyzeResist = _FireResist = _PoisonResist = _LightningResist = _ArcaneResist = 0;

	m_timeHackDif = 0;

	m_iVit = m_iCharisma = m_iLuck = m_iLU_Pool = 0;

	memset(m_iSkillSSN, 0, sizeof(m_iSkillSSN));

	m_dwLogoutHackCheck = m_dwWarBeginTime = 0;
	m_dwPoisonTime = 0;

	iReturnID = iNumPoints = iRank = iCrusadeJob = iTitleIndex = iNextRankPoints = 0;

	m_iCustomItemValue_Attack = m_iCustomItemValue_Defense = 0;

	m_iMinAP_L = m_iMinAP_SM = m_iMaxAP_L = m_iMaxAP_SM = 0;

	m_nextRecallPoint = 0;
	m_iAddMR = 0;
	m_iAddTransMana = 0;
	m_iAddChargeCritical = 0;
	m_iLastDamage = 0;
	m_dwAttackLAT = 0;
	m_specialAbilityLastSec = m_specialAbilityStartTime = 0;

	m_bIsWarLocation = false;

	m_bIsHunter = false;

	currentstatus = 1;

	socknum = 0;
	disconnecttime = 0;
	lastpackettime = 0;

	m_bActive = true;

	for (int i = 0; i < DEFAULTBAGSIZE; ++i)
	{
		m_pItemList.push_back(new ItemWrap);
	}
	for (int i = 0; i < DEFAULTBANKSIZE; ++i)
	{
		m_pItemInBankList.push_back(new ItemWrap);
	}

	//handle of 0 assumed. 0 = invalid and is not usable for an object in game - this value would be set when obtaining character data from the db assuming login is successful
	m_handle = 0;
	m_ownerType = OWNERTYPE_PLAYER;
	m_bIsBeingResurrected = false;

	m_cProfile = "__________";
	skincolor = haircolor = underwearcolor = 0xFFFFFFFF;

	m_cLocation = "NONE";
	m_iGuildRank = GUILDRANK_NONE;
	m_iGuildGUID = -1;
	m_guild = 0;
	m_gldSummonsTime = 0;

	m_bIsInitComplete = false;

	m_charID = 0;

	_str = _int = _dex = _mag = _vit = _agi = 0;

	//m_iHitRatio_ItemEffect_SM = 0;
	//m_iHitRatio_ItemEffect_L  = 0;

	m_iEnemyKillCount = 0;
	m_iEnemyKillTotalCount = 0;
	m_iPKCount = 0;
	m_iRewardGold = 0;
	m_iCurWeightLoad = 0;
#ifdef Showdmg
	iDmgShowon = 0;
#endif

	m_bIsSafeAttackMode  = false;


	for (i = 0; i < MAXITEMEQUIPPOS; i++) 
		m_sItemEquipmentStatus[i] = -1;

	Equipped.Head = nullptr;
	Equipped.Chest = nullptr;
	Equipped.Hauberk = nullptr;
	Equipped.Leggings = nullptr;
	Equipped.Boots = nullptr;
	Equipped.Cape = nullptr;
	Equipped.Angel = nullptr;
	Equipped.Ring1 = nullptr;
	Equipped.Ring2 = nullptr;
	Equipped.LeftHand = nullptr;
	Equipped.RightHand = nullptr;
	Equipped.Necklace = nullptr;
	Equipped.pArrow = nullptr;
	Equipped.TwoHand = nullptr;
	for (i = 0; i < MAXMAGICTYPE; i++)
		m_cMagicMastery[i] = 0;

	for (i = 0; i < MAXSKILLTYPE; i++)
		m_cSkillMastery[i] = 0;

	for (i = 0; i < MAXSKILLTYPE; i++) {
		m_bSkillUsingStatus[i] = false;
		m_iSkillUsingTimeID[i] = 0;
	}

	m_sX = -1;
	m_sY = -1;
	m_cDir = 5;
	m_sType   = 0;
	m_sAppr1  = 0;
	m_sAppr2  = 0;
	m_sAppr3  = 0;
	m_sAppr4  = 0;
	m_iApprColor = 0; 
	m_sHeadApprValue = 0;
	m_sBodyApprValue = 0;
	m_sArmApprValue  = 0;
	m_sLegApprValue  = 0;

	m_cSex  = NONE;
	m_cSkin = 0;
	m_cHairStyle  = 0;
	m_cHairColor  = 0;
	m_cUnderwear  = 0;

	m_cAttackDiceThrow_SM = 0;
	m_cAttackDiceRange_SM = 0;
	m_cAttackDiceThrow_L = 0;
	m_cAttackDiceRange_L = 0;
	m_cAttackBonus_SM    = 0;
	m_cAttackBonus_L     = 0;

	m_side = NEUTRAL;

	m_iHitRatio = 0;
	m_iDefenseRatio = 0;

	for (i = 0; i < MAXITEMEQUIPPOS; i++) m_iDamageAbsorption_Armor[i]  = 0;
	m_iDamageAbsorption_Shield = 0;


	m_iHPstock = 0;
	m_iHPStatic_stock = 0 ;

	for (i = 0; i < MAXMAGICEFFECTS; i++) 
		m_cMagicEffectStatus[i]	= 0;

	m_iWhisperPlayerIndex = 0;

	m_iHungerStatus  = 100;  

	m_bIsWarLocation = false;

	m_bIsPoisoned    = false;
	m_iPoisonLevel   = 0;

	m_iAdminUserLevel  = 0;
	m_GMFlags = 0;

	m_reputation          = 0;
	m_iTimeLeft_ShutUp = 0;
	m_iTimeLeft_Rating = 0;
	m_iTimeLeft_ForceRecall  = 0;
	m_iTimeLeft_FirmStamina = 0;
	m_dwLogoutHackCheck = 0;
	m_lastDamageTime = 0;
	m_iLucky = 0;

	m_bIsOnServerChange  = false;

	m_iExpStock = 0;

	m_iAllocatedFish = 0;
	m_iFishChance    = 0;

	m_bIsOnWaitingProcess = false;

	m_iSuperAttackLeft  = 0;
	m_iSuperAttackCount = 0;

	m_sUsingWeaponSkill = SKILL_HANDATTACK; 
	m_iMPSaveRatio   = 0;
	m_iAddResistMagic  = 0;
	m_iAddPhysicalDamage = 0;
	m_iAddMagicalDamage  = 0;
	m_addMagicDmgPct  = 0;
	m_bIsLuckyEffect     = false;
	m_iSideEffect_MaxHPdown = 0;

	m_iAddAbsAir   = 0;
	m_iAddAbsEarth = 0;
	m_iAddAbsFire  = 0;
	m_iAddAbsWater = 0;
	m_iComboAttackCount = 0;
	m_iDownSkillIndex   = -1;

	m_iMagicDamageSaveItemIndex = -1;

	m_sCharIDnum1 = m_sCharIDnum2 = m_sCharIDnum3 = 0;

	m_partyCoordSteps = 0;
	m_pinguid = 0;

	m_iAbuseCount     = 0;
	m_iAddMining = 0;
	m_iAddCrafting = 0;
	m_iAddFishing = 0;

	m_iReserveTime = m_iFightZoneTicketNumber = m_iFightzoneNumber = 0 ;

	m_iPenaltyBlockYear = m_iPenaltyBlockMonth = m_iPenaltyBlockDay = 0; 

	m_isExchangeMode = false;
	m_isExchangeConfirm = false;
	m_exchangeH = 0;
	m_exchangeCount = 0;
	
	for(i = 0; i < 4; i++){
		m_exchangeItems[i].itemAmount = 0;
		m_exchangeItems[i].itemIndex  = -1;		
	}	

	m_iQuest		 = 0;
	m_iQuestID       = 0;
	m_iAskedQuest	 = 0;
	m_iCurQuestCount = 0;
	m_iQuestRewardType	 = 0;
	m_iQuestRewardAmount = 0;

	m_iContribution = 0;
	m_bQuestMatchFlag_Loc = false;  
	m_bIsQuestCompleted   = false;

	m_bIsNeutral      = false;
	m_bIsObserverMode = false;


	m_iSpecialEventID = 200081;

	m_iSpecialWeaponEffectType  = 0;
	m_iSpecialWeaponEffectValue = 0;
	m_iAddHP = m_iAddSP = m_iAddMP = 0; 
	m_iAddAR = m_iAddPR = m_iAddDR = 0;
	m_iAddAbsPD = m_iAddAbsMD = 0;
	m_iAddCD = m_iAddExp = m_iAddGold = 0;

	m_iSpecialAbilityTime = SPECABLTYTIMESEC;
	m_iSpecialAbilityType = 0;
	m_bIsSpecialAbilityEnabled = false;
	m_specialAbilityLastSec   = 0;

	m_iSpecialAbilityEquipPos  = 0;

	m_iMoveMsgRecvCount   = 0;
	m_iAttackMsgRecvCount = 0;
	m_iRunMsgRecvCount    = 0;
	m_iSkillMsgRecvCount  = 0;
 
	m_iAlterItemDropIndex = -1;

	m_iAutoExpAmount = 0;
	m_iWarContribution = 0;

	m_dwMoveLAT = m_dwRunLAT = m_dwAttackLAT = 0;

	m_dwInitCCTimeRcv = 0;
	m_dwInitCCTime = 0;

	m_cLockedMapName = "NONE";
	m_iLockedMapTime   = 0;
	m_iDeadPenaltyTime = 0;

	m_iCrusadeDuty  = 0;
	m_dwCrusadeGUID = 0;

	for (i = 0; i < MAXCRUSADESTRUCTURES; i++) {
		m_stCrusadeStructureInfo[i].cType = 0;
		m_stCrusadeStructureInfo[i].cSide = 0;
		m_stCrusadeStructureInfo[i].sX = 0;
		m_stCrusadeStructureInfo[i].sY = 0;
	}
	m_iCSIsendPoint = 0;

	m_bIsSendingMapStatus = false;

	m_iConstructionPoint = 0;

	m_iConstructLocX = m_iConstructLocY = -1;

	m_dwFightzoneDeadTime = 0;

	_party = 0;
	_partyStatus = PS_NOTINPARTY;

	m_iReqJoinPartyClientH = 0;

	m_dwLastActionTime = 0;
	m_bIsCheckingWhisperPlayer = false;

	m_hasPrecasted = false;
	m_timeHackTurn = 0;
	m_timeHack = 0; 
	for(int i=0; i < SPEEDCHECKTURNS; i++)
		m_moveTime[i] = 540; 
	m_moveTurn = 0;
	for(int i=0; i < SPEEDCHECKTURNS; i++)
		m_runTime[i] = 300; 
	m_runTurn = 0; 

	m_bIsBankModified = false ;

	m_iGizonItemUpgradeLeft = 0;

	m_dwAttackFreqTime = m_dwMagicFreqTime = m_dwMoveFreqTime = 0;
	m_resetMoveFreq = false;
	m_rejectedMove = false; 
	m_bIsAttackModeChange = false; 
	m_bIsOnTown = false;
	m_bIsOnShop = false ; 
	m_bIsOnTower = false ; 
	m_bIsOnWarehouse = false ; 
	m_bIsOnGuildHall = false; 
	m_bIsOnCath  = false;
	m_dwWarmEffectTime = 0;
	m_bIsInBuilding = false ;

	m_heroArmourBonus = 0;
	m_AheroArmourBonus = 0;
	m_LheroArmourBonus = 0;

	_angelStr = _angelMag = _angelInt = _angelDex = _angelAgi = _angelVit = 0;

	ActiveTitle = "";
	TitleType = 0;
}

Client::~Client()
{
	for(int i = 0; i < m_pItemList.size(); i++)
	{
		delete m_pItemList[i];
	}
	for (int i = 0; i < m_pItemInBankList.size(); i++)
	{
		delete m_pItemInBankList[i];
	}
}

//TODO: do we want clients invoking packets being sent? or should the server know to send them when this function is called?
//these functions aren't called that often - only on level up stat change or resetting of stats
// ultimately, will the client class itself be accessing the server class?
void Client::SetStr(int str, bool check) 
{
// 	_str = str;
// 	if(check)
// 	{
// 		if(m_iHP > GetMaxHP())
// 		{
// 			m_iHP = GetMaxHP();
// 			Notify(NULL, NOTIFY_HP, NULL, NULL, NULL, NULL);
// 		}
// 		if(m_iSP > GetMaxSP())
// 		{
// 			m_iSP = GetMaxSP();
// 			Notify(NULL, NOTIFY_SP, NULL, NULL, NULL, NULL);
// 		}
// 	}
}

void Client::SetMag(int mag)
{
//	_mag = mag;
// 	if(m_iMP > GetMaxMP())
// 	{
// 		m_iMP = GetMaxMP();
// 		Notify(NULL, NOTIFY_MP, NULL, NULL, NULL, NULL);
// 	}
}

void Client::SetInt(int __int, bool check)
{
// 	_int = __int;
// 	if(check)
// 	{
// 		if(m_iMP > GetMaxMP())
// 		{
// 			m_iMP = GetMaxMP();
// 			Notify(NULL, NOTIFY_MP, NULL, NULL, NULL, NULL);
// 		}
// 	}
}

void Client::SetDex(int dex)
{
	_dex = dex;
}

void Client::SetAngelStr(int str)
{
// 	_angelStr = str;
// 
// 	if(m_iHP > GetMaxHP())
// 	{
// 		m_iHP = GetMaxHP();
// 		Notify(NULL, NOTIFY_HP, NULL, NULL, NULL, NULL);
// 	}
// 	if(m_iSP > GetMaxSP())
// 	{
// 		m_iSP = GetMaxSP();
// 		Notify(NULL, NOTIFY_SP, NULL, NULL, NULL, NULL);
// 	}
/*	Done client-side.
	for (short sItemIndex = 0; sItemIndex < MAXITEMS; sItemIndex++)
		if  ((m_pItemList[sItemIndex] != NULL) && 
			(m_bIsItemEquipped[sItemIndex] == true))
		{
			if(m_pItemList[sItemIndex]->m_wWeight/100 > GetStr())
				g_game->ReleaseItemHandler(m_handle, sItemIndex, true);
		}*/
}
void Client::SetAngelDex(int dex)
{
	_angelDex = dex;
}
void Client::SetAngelInt(int __int)
{
// 	_angelInt = __int;
// 	if(m_iMP > GetMaxMP())
// 	{
// 		m_iMP = GetMaxMP();
// 		Notify(NULL, NOTIFY_MP, NULL, NULL, NULL, NULL);
// 	}
}
void Client::SetAngelMag(int mag)
{
// 	_angelMag = mag ;
// 	if(m_iMP > GetMaxMP())
// 	{
// 		m_iMP = GetMaxMP();
// 		Notify(NULL, NOTIFY_MP, NULL, NULL, NULL, NULL);
// 	}
}

int Client::GetEffectiveMA() const
{
	//TODO: Fix MA and PA altogether - how they interact, how they stack, etc
	int MA = m_iAddAbsMD;

	//if(MA > 25)
		//MA = 0.5*MA + logb((double)MA, (double)1.3 );

	return (MA > 80) ? 80 : MA;
}

bool Client::CheckTotalSkillMasteryPoints(int iSkill)
{
// 	int i;
// 	int iRemainPoint, iTotalPoints, iWeaponIndex, iDownSkillSSN, iDownPoint;
// 	short sDownSkillIndex;
// 
// 	iTotalPoints = 0;
// 	for (i = 0; i <	MAXSKILLTYPE; i++) 
// 		iTotalPoints += m_cSkillMastery[i];
// 
// 	iRemainPoint = iTotalPoints - MAXSKILLPOINTS;
// 
// 	if (iRemainPoint > 0) {
// 		while (iRemainPoint > 0) {
// 			sDownSkillIndex = -1; 
// 			if (m_iDownSkillIndex != -1) {
// 				if (m_cSkillMastery[m_iDownSkillIndex] > 0) {
// 					sDownSkillIndex = m_iDownSkillIndex;
// 				}
// 				else {
// 					iDownSkillSSN = 99999999;
// 					for (i = 0; i < MAXSKILLTYPE; i++)
// 						if ((m_cSkillMastery[i] >= 21) && (i != iSkill) && 
// 							(m_iSkillSSN[i] <= iDownSkillSSN)) {
// 								iDownSkillSSN = m_iSkillSSN[i];
// 								sDownSkillIndex = i;
// 						}
// 				}
// 			}
// 
// 			if (sDownSkillIndex != -1) {
// 
// 				if (m_cSkillMastery[sDownSkillIndex] <= 20)
// 					iDownPoint = m_cSkillMastery[sDownSkillIndex];
// 				else iDownPoint = 1;
// 
// 				m_cSkillMastery[sDownSkillIndex] -= iDownPoint;
// 				m_iSkillSSN[sDownSkillIndex] = g_skillSSNpoint[m_cSkillMastery[sDownSkillIndex]+1] - 1;
// 				iRemainPoint -= iDownPoint;
// 
// 				if (m_sItemEquipmentStatus[ EQUIPPOS_TWOHAND ] != -1) {
// 					iWeaponIndex = m_sItemEquipmentStatus[ EQUIPPOS_TWOHAND ];
// 					if (m_pItemList[iWeaponIndex]->m_sRelatedSkill == sDownSkillIndex) {
// 						m_iHitRatio -= iDownPoint;
// 						if (m_iHitRatio < 0) m_iHitRatio = 0;
// 					}
// 				}
// 
// 				if (m_sItemEquipmentStatus[ EQUIPPOS_RHAND ] != -1) {
// 					iWeaponIndex = m_sItemEquipmentStatus[ EQUIPPOS_RHAND ];
// 					if (m_pItemList[iWeaponIndex]->m_sRelatedSkill == sDownSkillIndex) {
// 						m_iHitRatio -= iDownPoint;
// 						if (m_iHitRatio < 0) m_iHitRatio = 0;
// 					}
// 				}
// 				Notify(NULL, NOTIFY_SKILL, sDownSkillIndex, m_cSkillMastery[sDownSkillIndex], NULL, NULL);
// 			}
// 			else {
// 				return false;
// 			}
// 		}
// 		return true;
// 	}

	return false;
}

void Client::ValidateSkills(bool logInvalidSkills)
{
// 	if(IsGM()) return;
// 
// 	int invalidSkills = 0;
// 	char logMessage[100];
// 	for (int skillIndex = 0; skillIndex < MAXSKILLTYPE; skillIndex++)
// 	{	
// 		switch (skillIndex) {
// 		case SKILL_MINING:  
// 		case SKILL_MANUFACTURING:
// 		case SKILL_HANDATTACK:
// 			if (m_cSkillMastery[skillIndex] > (GetStr() * 2)) 
// 			{	
// 				invalidSkills += m_cSkillMastery[skillIndex] - (GetStr() * 2);
// 				m_cSkillMastery[skillIndex] = GetStr() * 2;
// 				m_iSkillSSN[skillIndex] = 0;
// 				Notify(NULL, NOTIFY_SKILL, skillIndex, m_cSkillMastery[skillIndex], NULL, NULL);
// 			}
// 			break;
// 
// 		case SKILL_MAGICRES:
// 			if (m_cSkillMastery[skillIndex] > (m_iLevel * 2)) 
// 			{	
// 				invalidSkills += m_cSkillMastery[skillIndex] - (m_iLevel * 2);
// 				m_cSkillMastery[skillIndex] = m_iLevel * 2;
// 				m_iSkillSSN[skillIndex] = 0;
// 				Notify(NULL, NOTIFY_SKILL, skillIndex, m_cSkillMastery[skillIndex], NULL, NULL);
// 			}
// 			break;
// 
// 		case SKILL_MAGIC:
// 		case SKILL_STAFF:
// 		case SKILL_CRAFTING:
// 			if (m_cSkillMastery[skillIndex] > (GetMag() * 2))
// 			{	
// 				invalidSkills += m_cSkillMastery[skillIndex] - (GetMag() * 2);
// 				m_cSkillMastery[skillIndex] = GetMag() * 2;
// 				m_iSkillSSN[skillIndex] = 0;
// 				Notify(NULL, NOTIFY_SKILL, skillIndex, m_cSkillMastery[skillIndex], NULL, NULL);
// 			}
// 			break;
// 
// 		case SKILL_FISHING:
// 		case SKILL_ARCHERY:
// 		case SKILL_SHORTSWORD:
// 		case SKILL_LONGSWORD:
// 		case SKILL_FENCING:
// 		case SKILL_AXE:
// 		case SKILL_SHIELD:
// 		case SKILL_HAMMER:
// 			if (m_cSkillMastery[skillIndex] > (GetDex() * 2)) 
// 			{	
// 				invalidSkills += m_cSkillMastery[skillIndex] - (GetDex() * 2);
// 				m_cSkillMastery[skillIndex] = GetDex() * 2;
// 				m_iSkillSSN[skillIndex] = 0;
// 				Notify(NULL, NOTIFY_SKILL, skillIndex, m_cSkillMastery[skillIndex], NULL, NULL);
// 			}
// 			break;
// 
// 		case SKILL_ALCHEMY:
// 		case SKILL_FARMING:
// 		case SKILL_PRETENDCORPSE:
// 			if (m_cSkillMastery[skillIndex] > (GetInt() * 2)) 
// 			{	
// 				invalidSkills += m_cSkillMastery[skillIndex] - (GetInt() * 2);
// 				m_cSkillMastery[skillIndex] = GetInt() * 2;
// 				m_iSkillSSN[skillIndex] = 0;
// 				Notify(NULL, NOTIFY_SKILL, skillIndex, m_cSkillMastery[skillIndex], NULL, NULL);
// 			}
// 			break;
// 
// 		case SKILL_POISONRES:
// 			if (m_cSkillMastery[skillIndex] > (m_iVit * 2)) 
// 			{	
// 				invalidSkills += m_cSkillMastery[skillIndex] - (m_iVit * 2);
// 				m_cSkillMastery[skillIndex] = m_iVit * 2;
// 				m_iSkillSSN[skillIndex] = 0;
// 				Notify(NULL, NOTIFY_SKILL, skillIndex, m_cSkillMastery[skillIndex], NULL, NULL);
// 			}
// 			break;
// 		default:
// 			m_iSkillSSN[skillIndex] = 0;
// 			break;
// 		}
// 	}
// 
// 	if(logInvalidSkills && (invalidSkills > 0))
// 	{
// 		gserver->logger->information(Poco::format("PC(%s) A skill was too high, reduced by (%?d) points", (string)m_cCharName, invalidSkills));
// 	}
}

//TODO: when checking relationship, SEND THE OTHER CLIENT OBJECT AS A PARAM. why have the client object seek it out when it doesn't have to?
int Client::GetPlayerRelationship(Client * target) const
{
	int iRet;

	if (m_bIsInitComplete == false) return 0;

	if (target == nullptr) return 0;
	if (target->m_bIsInitComplete == false) return 0;

	iRet = 0;

	if (m_iPKCount != 0) {
		if ((m_side == target->m_side) &&
			(m_side != NEUTRAL) && (target->m_side != NEUTRAL))
			iRet = 7;
		else iRet = 2; 
	}
	else if (target->m_iPKCount != 0) {
		if ((m_side == target->m_side) &&
			(m_side != NEUTRAL) && (target->m_side != NEUTRAL))
			iRet = 6;
		else iRet = 2;
	}
	else {
		if (m_side != target->m_side) {
			if ((m_side != NEUTRAL) && (target->m_side != NEUTRAL)) {
				// (Traveler)
				iRet = 2;
			}
			else {
				iRet = 0;
			}
		}
		else {
			if ((m_cGuildName == target->m_cGuildName) &&
				(m_cGuildName != "NONE")) {
				if (target->m_iGuildRank == GUILDRANK_MASTER)
						iRet = 5;
					else iRet = 3;
			}
			else 
				if ((m_side == target->m_side) &&
					(m_cGuildName != "NONE") &&
					(target->m_cGuildName != "NONE") &&
					(m_cGuildName != target->m_cGuildName)) {
						iRet = 4;
				}
				else iRet = 1;
		}
	}

	return iRet;
}
int Client::GetMaxHP() const
{
	//TODO: Fix max hp.
	int iRet;

	//iRet = m_iVit*3 + m_iLevel*2 + (GetStr())/2;
	iRet = m_iVit*8 + m_iLevel*8 + (GetStr())*2 + (GetInt())*2; // HP System xRisenx
	/*iRet =  (m_pClientList[iClientH]->m_iVit * 12) + (m_pClientList[iClientH]->m_iLevel * 5) 
		+ ((m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iAngelicStr) * 6
		+ (m_pClientList[iClientH]->m_iMag * 8));*/

	if (m_iSideEffect_MaxHPdown != 0)
		iRet = iRet - (iRet/m_iSideEffect_MaxHPdown);

	return iRet;
}

int Client::GetMaxMP() const
{
	int iRet;

	iRet = (GetMag())*3  + m_iLevel*2 + (GetInt())*2; // MP System xRisenx

	return iRet;
}

//TODO: remove SP? Make SP do something else? SP serves as a mostly useless stat. With a PG potion it's always capped and almost impossible to drain from enemy casts
//Maybe make SP the alternative resource to new classes? (energy class anyone?) - goes hand in hand with why the fuck warriors have MP.
int Client::GetMaxSP() const
{
	int iRet;

	iRet = (GetStr()) + m_iLevel*17; // SP System xRisenx

	return iRet;
}

void Client::AddHP(long hp)
{
// 	int maxHP = GetMaxHP();
// 	m_iHP += hp; 
// 	if (m_iHP > maxHP) m_iHP = maxHP;
// 	Notify(NULL, NOTIFY_HP, NULL, NULL, NULL, NULL);
}

void Client::KilledHandler(Unit * attacker, int32_t sDamage)
{
	string attackername;
#ifdef EKMessage
	//char EKMessage1[100], EKMessage2[100], EKMessage3[100], EKMessage4[100], EKMessage5[100], EKMessage6[100];
	int KilledEK, RangeEK;
#endif
	short sAttackerWeapon;
	int   * ip, i, iExH;
	bool  bIsSAattacked = false;

	if (m_bIsInitComplete == false) return;
	if (m_bIsKilled == true) return;

	if (pMap->m_cName.find("fight") != -1)
	{
		m_dwFightzoneDeadTime = unixtime();
		gserver->logger->information(Poco::format("Fightzone Dead Time: %?d", m_dwFightzoneDeadTime));
	}

	m_bIsKilled = true;
	m_iHP = 0;

// 	if (m_isExchangeMode == true) {
// 		iExH = m_exchangeH;
// 		gserver->_ClearExchangeStatus(iExH);
// 		gserver->_ClearExchangeStatus(m_handle);
// 	}

	gserver->RemoveFromTarget(self.lock());

	attackername = attacker->name;

	Notify(nullptr, NOTIFY_HP, 0, 0, 0);
	Notify(nullptr, NOTIFY_KILLED, 0, 0, 0, attackername);
	if (attacker->m_ownerType == OWNERTYPE_PLAYER) {
		sAttackerWeapon = ((static_cast<Client*>(attacker)->m_sAppr2 & 0x0FF0) >> 4);
	}
	else sAttackerWeapon = 1;
	gserver->SendEventToNearClient_TypeA(this, MSGID_MOTION_DYING, sDamage, sAttackerWeapon, 0);
	pMap->ClearOwner(m_sX, m_sY);
	pMap->SetDeadOwner(self.lock(), m_sX, m_sY);

// 	int itemInd;
// 	if(gserver->m_astoria.get() && gserver->m_astoria->IsCapture() && (itemInd = HasItem(ITEM_RELIC)))
// 	{
// 		gserver->DropItemHandler(m_handle, itemInd, 1, m_pItemList[itemInd]->m_cName, false);
// 	}

	if (pMap->m_cType == MAPTYPE_NOPENALTY_NOREWARD) return;
	// Monster kill event xRisenx
	/*if (cAttackerType == OWNERTYPE_PLAYER)
    {    if (g_clientList[iAttackerH] != NULL)
        {    if (g_game->m_bNpcHunt)
            {    g_game->NpcHuntPointsAdder(iAttackerH);
    }    }    }*/
	// Monster kill event xRisenx
	if (attacker->m_ownerType == OWNERTYPE_PLAYER || attacker->m_ownerType == OWNERTYPE_PLAYER_INDIRECT) {
		if (attacker->m_ownerType == OWNERTYPE_PLAYER_INDIRECT) {
			//gserver->_bPKLog(PKLOG_BYOTHER,(int) -1,m_handle,NULL) ;
			// m_iExp -= dice(1, 50);
			// if (m_iExp < 0) m_iExp = 0;

			// Notify(NULL, NOTIFY_EXP, NULL, NULL, NULL, NULL);
		}

		switch (static_cast<Client*>(attacker)->m_iSpecialAbilityType) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			bIsSAattacked = true;
			break;
		}

		if (attacker == this) return;
		if (IsNeutral()) {
			if (m_iPKCount == 0) {

			//	g_clientList[iAttackerH]->ApplyPKPenalty(m_handle);
				//gserver->EnemyKillRewardHandler(iAttackerH, m_handle);
			}
			else {
				//gserver->PK_KillRewardHandler(iAttackerH, m_handle);
			}
		} 
		else {
			if (static_cast<Client*>(attacker)->IsNeutral()) {
				if (m_iPKCount == 0) {
					//	g_clientList[iAttackerH]->ApplyPKPenalty(m_handle);
					//gserver->EnemyKillRewardHandler(iAttackerH, m_handle);
				}
				else {
					//g_clientList[iAttackerH]->ApplyPKPenalty(m_handle);
				}
			}
			else {
// 				if(gserver->m_astoria.get())
// 					gserver->m_astoria->PlayerKill(g_clientList[iAttackerH], this);
// 				
// 				if (gserver->m_bHeldenianMode && g_mapList[m_cMapIndex]->m_bIsHeldenianMap) 
// 					gserver->HeldenianPlayerKill(g_clientList[iAttackerH], this);

				if (m_side == static_cast<Client*>(attacker)->m_side) {
					if (m_iPKCount == 0) {
						static_cast<Client*>(attacker)->ApplyPKPenalty(m_handle);
					}
					else {
						//gserver->PK_KillRewardHandler(iAttackerH, m_handle);
					}
				}
				else {
					//gserver->EnemyKillRewardHandler(iAttackerH, m_handle);
				}
			}
		}

		if (m_iPKCount == 0) {
			// Innocent
			if (static_cast<Client*>(attacker)->IsNeutral()) {
			}
			else {
				if (m_side == static_cast<Client*>(attacker)->m_side) {
				}
				else {
					ApplyCombatKilledPenalty(2, bIsSAattacked);
				}
			}
		}
		else if ((m_iPKCount >= 1) && (m_iPKCount <= 3)) {
			// Criminal 
			ApplyCombatKilledPenalty(3, bIsSAattacked,true);
		}
		else if ((m_iPKCount >= 4) && (m_iPKCount <= 11)) {
			// Murderer 
			ApplyCombatKilledPenalty(6, bIsSAattacked,true);
		}
		else if (m_iPKCount >= 12) {
			// Slaughterer 
			ApplyCombatKilledPenalty(12, bIsSAattacked,true);
		}
	}
	else if (attacker->m_ownerType == OWNERTYPE_NPC) {

		//gserver->_bPKLog(PKLOG_BYNPC,(int) -1,m_handle, attacker->name) ;

		if (m_iPKCount == 0) {
			// Innocent
			ApplyCombatKilledPenalty(1, bIsSAattacked,true);
		}
		else if ((m_iPKCount >= 1) && (m_iPKCount <= 3)) {
			// Criminal 
			ApplyCombatKilledPenalty(3, bIsSAattacked , true);
		}
		else if ((m_iPKCount >= 4) && (m_iPKCount <= 11)) {
			// Murderer 
			ApplyCombatKilledPenalty(6, bIsSAattacked , true);
		}
		else if (m_iPKCount >= 12) {
			// Slaughterer 
			ApplyCombatKilledPenalty(12, bIsSAattacked , true);
		}

		if (static_cast<Npc*>(attacker)->m_iGuildGUID != 0) {

			if (static_cast<Npc*>(attacker)->m_side != m_side) {
				for (auto c : gserver->clientlist)
				{
					if ((c->m_iGuildGUID == static_cast<Npc*>(attacker)->m_iGuildGUID) && (c->m_iCrusadeDuty == 0))
					{
						c->m_iConstructionPoint += (m_iLevel / 2);
						if (c->m_iConstructionPoint > MAXCONSTRUCTIONPOINT)
							c->m_iConstructionPoint = MAXCONSTRUCTIONPOINT;
						gserver->logger->information(Poco::format("Enemy Player Killed by Npc! Construction +%?d", (m_iLevel / 2)));
						gserver->SendNotifyMsg(nullptr, c.get(), NOTIFY_CONSTRUCTIONPOINT, c->m_iConstructionPoint, c->m_iWarContribution, 0);
					}
				}
			}
		}
	}
#ifdef EKMessage
	for (KilledEK = 1; KilledEK < MAXCLIENTS; KilledEK++)
	{
		if ((gserver->m_pClientList[KilledEK] != NULL))
		{
			ZeroMemory(g_cTxt, sizeof(g_cTxt));
			RangeEK = rand()%6;
			switch(RangeEK)
			{
			case 0:
				wsprintf(g_cTxt, "[%s] laid [%s] to rest!", cAttackerName, name);
				break;
			case 1:
				wsprintf(g_cTxt, "[%s] smashed [%s] face into the ground!", cAttackerName, name);
				break;
			case 2:
				wsprintf(g_cTxt, "[%s] was no match for [%s]!", name, cAttackerName);
				break;
			case 3:
				wsprintf(g_cTxt, "[%s] says LAG LAG!! but gets PWNED by [%s]!", name, cAttackerName);
				break;
			case 4:
				wsprintf(g_cTxt, "[%s] sent [%s] off to pie heaven!", cAttackerName, name);
				break;
			case 5:
				wsprintf(g_cTxt, "[%s] got beat by [%s\'s] ugly stick!", name, cAttackerName);
				break;
			}
			gserver->SendNotifyMsg(NULL, KilledEK, NOTIFY_NOTICEMSG, NULL, NULL, NULL, g_cTxt);
			wsprintf(g_cTxt, "[EK] %s killed %s.", cAttackerName, name);
			gserver->logger->information(Poco::format("[EK] %s killed %s.", (string)cAttackerName, (string)name));
		}
	}
#endif
	// Gladiator Arena xRisenx
	//if (strcmp(g_game->m_pMapList[g_game->m_pClientList[g_game->iClientH]->m_cMapIndex]->m_cName, g_game->cArenaMap) == 0)
// 	if (strcmp(g_mapList[m_cMapIndex]->m_cName, g_game->cArenaMap) == 0)
// {
//   m_iArenaDeaths++;
//   m_iArenaKills = 0; // reset dead guy's kill count
//   //g_game->RequestArenaStatus(iClientH, true);
//   g_game->RequestArenaStatus(true);
//   g_game->m_pClientList[iAttackerH]->m_iArenaKills++;
// 
//     if(g_game->m_pClientList[iAttackerH]->m_iArenaKills % 25 == 0)
//     {
//          //m_pClientList[i]->m_iGladiatorCount += 1;
// 		 g_clientList[i]->m_iGladiatorCount += 1;
//        //CheckTitleLevelUp(i, "Gladiator", m_pClientList[i]->m_iGladiatorCount);
// 		  //g_game->increaseTitlePoints(iClientH, TITLE_INDEX_GLADIATOR, m_iGladiatorCount);
// 		 g_game->increaseTitlePoints(TITLE_INDEX_GLADIATOR, m_iGladiatorCount, true);
//     }
// 
//   g_game->RequestArenaStatus(iAttackerH, true);
// }
	// Gladiator Arena xRisenx
}

void Client::ApplyCombatKilledPenalty(char cPenaltyLevel, bool bIsSAattacked, bool bItemDrop)
{  
// 	int iExp;
// 
// 	if (m_bIsInitComplete == false) return;
// 
// 	if (gserver->m_bIsCrusadeMode) 
// 	{
// 		DecPKCount();
// 		return;
// 	}
// 	else if (gserver->m_bIsApocalypseMode && g_mapList[m_cMapIndex]->m_bIsApocalypseMap) 
// 	{
// 		DecPKCount();
// 		return;
// 	}
// 	else if (gserver->m_bHeldenianMode && g_mapList[m_cMapIndex]->m_bIsHeldenianMap) 
// 	{
// 		DecPKCount();
// 		return;
// 	}
// 	else if(gserver->m_astoria.get() && m_cMapIndex == gserver->m_iAstoriaMapIndex)
// 	{
// 		DecPKCount();	
// 		return;
// 	}
// 	else {
// 		DecPKCount();
// 
// 		iExp = dice(1, (5*cPenaltyLevel*m_iLevel));
// 
// 		if (m_bIsNeutral == true) iExp = iExp / 3;
// 
// 		// if (m_iLevel == PLAYERMAXLEVEL) iExp = 0;
// 
// 		m_iExp -= iExp;
// 		if (m_iExp < 0) m_iExp = 0;
// 
// 		Notify(NULL, NOTIFY_EXP, NULL, NULL, NULL, NULL);
// 
// 		if (m_bIsNeutral != true) {
// #ifdef ITEMDROP
// 			if (m_iLevel < 80) {
// 				cPenaltyLevel--;
// 				if (cPenaltyLevel <= 0) cPenaltyLevel = 1;
// 			}
// 			PenaltyItemDrop(cPenaltyLevel, bIsSAattacked,bItemDrop); 
// #else
// 			if (m_iLevel < 80) {
// 				cPenaltyLevel--;
// 				if (cPenaltyLevel <= 0) cPenaltyLevel = 1;
// 			}
// 			PenaltyItemDrop(cPenaltyLevel, bIsSAattacked);
// #endif
// 		}
// 	}
}


void Client::PenaltyItemDrop(int iTotal, bool bIsSAattacked , bool bItemDrop)
{
// 	int i, j, iRemainItem;
// 	char cItemIndexList[MAXITEMS], cItemIndex;
// 
// 	if (m_bIsInitComplete == false) return;
// 
// 	if(m_iAlterItemDropIndex == -1)
// 	{
// 		for (i = 0; i < MAXITEMS; i++) 
// 		{
// 			if(m_pItemList[i] && m_pItemList[i]->m_sItemEffectType == ITEMEFFECTTYPE_ALTERITEMDROP &&
// 				m_pItemList[i]->m_wCurLifeSpan > 0)
// 			{
// 				m_iAlterItemDropIndex = i;
// 			}
// 		}
// 	}
// 
// 	if(m_iAlterItemDropIndex != -1)
// 	{
// 		if(m_pItemList[m_iAlterItemDropIndex])
// 		{
// 			// Testcode
// 			if (m_pItemList[m_iAlterItemDropIndex]->m_sItemEffectType == ITEMEFFECTTYPE_ALTERITEMDROP) {
// 				if (m_pItemList[m_iAlterItemDropIndex]->m_wCurLifeSpan > 0)
// 					m_pItemList[m_iAlterItemDropIndex]->m_wCurLifeSpan--;
// 
// 				gserver->DropItemHandler(m_handle, m_iAlterItemDropIndex, -1, m_pItemList[m_iAlterItemDropIndex]->m_cName);
// 
// 				m_iAlterItemDropIndex = -1;
// 			}
// 			else {
// 				// testcode
// 				gserver->logger->information("Alter Drop Item Index Error1", __FILE__, __LINE__);
// 				for (i = 0; i < MAXITEMS; i++) 
// 				{
// 					if(m_pItemList[i] && m_pItemList[i]->m_sItemEffectType == ITEMEFFECTTYPE_ALTERITEMDROP)
// 					{
// 						m_iAlterItemDropIndex = i;
// 						if (m_pItemList[m_iAlterItemDropIndex]->m_wCurLifeSpan > 0)
// 							m_pItemList[m_iAlterItemDropIndex]->m_wCurLifeSpan--;
// 
// 						gserver->DropItemHandler(m_handle, m_iAlterItemDropIndex, -1, m_pItemList[m_iAlterItemDropIndex]->m_cName);
// 						m_iAlterItemDropIndex = -1;
// 						return;
// 					}
// 				}
// 
// 				goto PID_DROP;
// 			}
// 		}
// 		return;
// 	}
// 
// PID_DROP:;
// 
// 	for (i = 1; i <= iTotal; i++) {
// 		iRemainItem = 0;
// 		ZeroMemory(cItemIndexList, sizeof(cItemIndexList));
// 
// #ifdef ITEMDROP
// 		if (bItemDrop)
// 		{
// 			for (j = 0; j < MAXITEMS; j++) 
// 				if (m_pItemList[j] != NULL )
// 				{
// 					cItemIndexList[iRemainItem] = j;
// 					iRemainItem++;
// 				}
// 		}
// 		else 
// 		{
// 			//
// 			for (j = 0; j < MAXITEMS; j++) 
// 				if (m_pItemList[j] != NULL 
// 					&& ( (m_pItemList[j]->m_cItemType == ITEMTYPE_CONSUME) 
// 					|| (m_pItemList[j]->m_cItemType == ITEMTYPE_EAT) ))
// 				{
// 					cItemIndexList[iRemainItem] = j;
// 					iRemainItem++;
// 				}
// 		}
// #else
// 		for (j = 0; j < MAXITEMS; j++) 
// 			if (m_pItemList[j] != NULL )
// 			{
// 				cItemIndexList[iRemainItem] = j;
// 				iRemainItem++;
// 			}
// #endif
// 			if (iRemainItem == 0) return;
// 			cItemIndex = cItemIndexList[ dice(1, iRemainItem) - 1 ];
// 
// 			if ((m_pItemList[cItemIndex]->m_sTouchEffectType != 0) &&
// 				(m_pItemList[cItemIndex]->m_sTouchEffectValue1 == m_sCharIDnum1) &&
// 				(m_pItemList[cItemIndex]->m_sTouchEffectValue2 == m_sCharIDnum2) &&
// 				(m_pItemList[cItemIndex]->m_sTouchEffectValue3 == m_sCharIDnum3)) {
// 			} 
// 			else if (m_pItemList[cItemIndex]->m_sIDnum == 1100)  {	//LegionTokens Non Dropable
// 			}
// 			else if (((m_pItemList[cItemIndex]->m_sItemEffectType == ITEMEFFECTTYPE_ATTACK_SPECABLTY) ||
// 				(m_pItemList[cItemIndex]->m_sItemEffectType == ITEMEFFECTTYPE_DEFENSE_SPECABLTY)) &&
// 				(bIsSAattacked == false)) {
// 			}
// 			else if ((m_bIsLuckyEffect == true) && (dice(1,10) == 5)) {
// 			}
// 			else gserver->DropItemHandler(m_handle, cItemIndex, -1, m_pItemList[cItemIndex]->m_cName);
// 	}
}

void Client::ApplyPKPenalty(short sVictimH)
{
// 	int iV1, iV2;
// 
// 	if (g_clientList[sVictimH] == NULL) return;
// 	if ((m_bIsSafeAttackMode == true) && (m_iPKCount == 0)) return; 
// 
// 	iV1 = dice((g_clientList[sVictimH]->m_iLevel/2)+1, 50); 
// 	iV2 = dice((m_iLevel/2)+1, 50);
// 
// 	m_iExp -= iV1; 
// 	m_iExp -= iV2;
// 	if (m_iExp < 0) m_iExp = 0;
// 
// 
// 	IncPKCount();
// 
// 	gserver->_bPKLog(PKLOG_BYPK,m_handle,sVictimH,NULL) ;
// 	gserver->m_stCityStatus[m_side].iCrimes++;
// 
// 	//m_reputation -= 0;
// 
// 	if(m_side != NEUTRAL && !IsGM())
// 	{
// 		#ifndef PKPENALTY               
// 			if (strcmp(g_mapList[m_cMapIndex]->m_cLocationName, sideMap[m_side]) != 0 ) return ;
// 		#endif
// 		ZeroMemory(m_cLockedMapName, sizeof(m_cLockedMapName));
// 		strcpy(m_cLockedMapName, sideMap[m_side]);
// 		m_iLockedMapTime = 60*3;
// 		gserver->RequestTeleportHandler(m_handle, 2, m_cLockedMapName, -1, -1);
// 	}
}

bool Client::IsInFoeMap()
{
// 	switch(m_side)
// 	{
// 	case ARESDEN:
// 		if(strcmp(g_mapList[ m_cMapIndex ]->m_cLocationName, sideMap[ELVINE]) == 0)
// 		{
// 			return true;
// 		}
// 		break;
// 	case ELVINE:
// 		if(strcmp(g_mapList[ m_cMapIndex ]->m_cLocationName, sideMap[ARESDEN]) == 0)
// 		{
// 			return true;
// 		}
// 		break;
// 	}

	return false;
}
//TODO: ... on being sent to jail, set a bool var "injail" or some shit and check against that. why do a memory compare?
bool Client::IsInJail()
{ 
// 	return (memcmp( g_mapList[m_cMapIndex]->m_cLocationName + 3, "jail", 4) == 0) 
// 		? true : false; 
	return false;
}

void Client::ApplyElo(Client * foe)
{
// 	float E; // Expected score
// 
// 	float lvlFactor = 1.0f;
// 	if(m_iLevel > foe->m_iLevel)
// 		lvlFactor = 1 - logb( (float)(m_iLevel - foe->m_iLevel), 180.0);
// 	else if(m_iLevel < foe->m_iLevel)
// 		lvlFactor = logb( (float)(foe->m_iLevel - m_iLevel), 180.0);
// 
// 	E = 1 / ( 1 + pow((float)10,  (float)((foe->m_elo - m_elo)/400.0))  );
// 
// 	m_elo += lvlFactor * K * (1 - E);
// 
// // 	ek += 0.5 + pow((float)(( (foeElo-elo) / 1000.0) + 1), (float)2.5)
// // 		* ((foeElo / 1000.0) * 1.5);
// 
// 	E = 1 / ( 1 + pow((float)10,  (float)((m_elo - foe->m_elo)/400.0))  );
// 
// 	foe->m_elo += lvlFactor * K * (0 - E);;
// 
// 	// 	float ekA;
// 	// 	ekA = 0.75 * pow((float)(( (elo-foeElo) / 1000.0) + 1), (float)2.5)
// 	// 		* ((foeElo / 1000.0) * 1.5);
// 	// 	ek -= ekA;
// 	// 	if(ek < 0) ek = 0;
}

void Client::DecPKCount()
{
// 	if (m_iPKCount > 0) {
// 		m_iPKCount--;
// 		Notify(NULL, NOTIFY_PKPENALTY, NULL, NULL, NULL, NULL);
// 		gserver->_bPKLog(PKLOG_REDUCECRIMINAL,(int) -1,m_handle,NULL);
// 		if(m_iPKCount == 0)
// 		{	
// 			SetStatusFlag(STATUS_PK, false);
// 			gserver->SendEventToNearClient_TypeA(m_handle, OWNERTYPE_PLAYER, MSGID_MOTION_NULL, NULL, NULL, NULL);
// 		}
// 
// 	}
}

void Client::IncPKCount()
{
// 	m_iPKCount++;
// 	SetStatusFlag(STATUS_PK, true);
// 
// 	Notify(NULL, NOTIFY_PKPENALTY, NULL, NULL, NULL, NULL);
// 	gserver->SendEventToNearClient_TypeA(m_handle, OWNERTYPE_PLAYER, MSGID_MOTION_NULL, NULL, NULL, NULL);
}
void Client::Save()
{
// 	gserver->bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATA, m_handle);
// 	m_dwAutoSaveTime = unixtime();
}
void Client::SWrite(StreamWrite & sw)
{
	if (socket == nullptr)
		return;
	socket->write(sw);
}
//TODO: let client class send packets directly?
void Client::Notify(Client * from, uint16_t wMsgType, uint32_t sV1, uint32_t sV2, uint32_t sV3, string pString, 
	uint32_t sV4, uint32_t sV5, uint32_t sV6, uint32_t sV7, uint32_t sV8, uint32_t sV9, string pString2)
{
	gserver->SendNotifyMsg(from, this, wMsgType, sV1, sV2, sV3, pString,
 		sV4, sV5, sV6, sV7, sV8, sV9, pString2);
}

void Client::NotifyGuildInfo(bool memberList) const
{
// 	char * index = packet;
// 
// 	Push(index, (uint32_t)MSGID_GUILDINFO);
// 
// 	m_guild->WriteInfo(index);
// 
// 	if(memberList)
// 	{
// 		Push(index, m_guild->GetOnlineCnt() - 1);
// 		m_guild->WriteMemberList(index, this);
// 	}
// 	else
// 		Push(index, (uint32_t)0);
// 
// 	socket->write(packet, index - packet);
// 	//socket->write(packet, index - packet);
}

void Client::NotifyGuildsmanStatus(Client const * const player, bool online) const
{
// 	char * index = packet;
// 
// 	Push(index, (uint32_t)MSGID_GUILDSMANSTATUS);
// 	Push(index, (char*)player->m_cCharName, 10);
// 	Push(index, (uint8_t)online);
// 
// 	if(online)
// 	{
// 		Push(index, (uint8_t)player->m_iGuildRank);
// 		Push(index, gserver->m_mapNameList[ player->m_cMapName ]);
// 	}
// 
// 	socket->write(packet, index - packet);
// 	//socket->write(packet, index - packet);
}

void Client::NotifyGuildSummons(Client const * const player) const
{
// 	char * index = packet;
// 
// 	Push(index, (uint32_t)MSGID_REQGUILDSUMMONS);
// 	Push(index, gserver->m_mapNameList[ player->m_cMapName ]);
// 	
// 	socket->write(packet, index - packet);
// 	//socket->write(packet, index - packet);
}

void Client::UpdateWeight()
{
	m_iCurWeightLoad = 0;

	for(int i = 0; i < m_pItemList.size(); i++)
	{
		if(m_pItemList[i]->_item)
		{
			m_iCurWeightLoad += m_pItemList[i]->_item->GetWeight();
		}
	}
}

uint32_t Client::HasItem(string name) const
{
	for (int i = 0; i < m_pItemList.size(); i++)
	{
		if (m_pItemList[i]->_item && m_pItemList[i]->_item->m_cName == name) 
		{
			return i;
		}
	}

	return ITEM_NONE;
}

uint32_t Client::HasItem(uint64_t id) const
{
	for (int i = 0; i < m_pItemList.size(); i++)
	{
		if (m_pItemList[i]->_item && m_pItemList[i]->_item->m_sIDnum == id) 
		{
			return i;
		}
	}

	return ITEM_NONE;
}

uint32_t Client::GetItemCount(uint64_t id) const
{
	for (int i = 0; i < m_pItemList.size(); i++)
	{
		if (m_pItemList[i]->_item && m_pItemList[i]->_item->m_sIDnum == id) 
		{
			return m_pItemList[i]->_item->m_dwCount;
		}
	}

	return 0;
}

void Client::SetItemCount(uint64_t id, uint32_t val, bool notify)
{
	for(int i = 0; i < m_pItemList.size(); i++)
	{
		if(m_pItemList[i]->_item && m_pItemList[i]->_item->m_sIDnum == id) 
		{
			if(val == 0) {
				gserver->ItemDepleteHandler(self.lock(), m_pItemList[i]->_item, false);
			}
			else {
				m_pItemList[i]->_item->m_dwCount = val;
				if(notify)
					Notify(0, NOTIFY_SETITEMCOUNT, i, val, (char)true, 0);
			}

			UpdateWeight();
			return;
		}
	}
}

float Client::GetDropFactor() const
{
// 	/*int rep = m_reputation;
// 	if(rep < 0)
// 		rep = 0;
// 	log10((float)rep+10) + */
// 	//float factor = 1 / ((3*log10((float)m_iContribution+10)) / 5);
// 	float factor = 1 / ((2*log10((float)m_iLucky+10)) / 5);
// 	//float factor = 1 / m_iLucky*5 / 10;
// 
// 	if(IsHeldWinner())
// 		factor *= 0.90f; // 10% Higher drop rate on first nothing/item 
// 	if(gserver->m_eventWinner[ET_DESTROY_SHIELD] == m_side)
// 		factor *= 0.90f; // 10% Higher drop rate on first nothing/item 
// 	if(HasPartyHuntBonus())
// 		factor *= 0.90f; // 10% Higher drop rate on first nothing/item 

//	return factor;
	return 0.0f;
}
 bool Client::IsHeldWinner() const
{
// 	switch(gserver->m_iLastHeldenianType)
// 	{
// 	case 1:
// 		return  (m_side == gserver->m_iHeldenianType1Winner);
// 
// 	case 2:
// 		return  (m_side == gserver->m_iHeldenianType2Winner);
// 	}
	return false;
}
 bool Client::IsHeldLoser() const
{
// 	switch(gserver->m_iLastHeldenianType)
// 	{
// 	case 1:
// 		return  (m_side != gserver->m_iHeldenianType1Winner);
// 
// 	case 2:
// 		return  (m_side != gserver->m_iHeldenianType2Winner);
// 	}
	return false;
}
bool Client::CheckNearbyFlags()
{
// 	if (!gserver->m_bHeldenianMode || gserver->m_iHeldenianType != 1 ||
// 		m_cMapIndex != gserver->m_iBtFieldMapIndex)
// 		return false;
// 
// 	OccupyFlag ** flagList = g_mapList[gserver->m_iBtFieldMapIndex]->m_pOccupyFlag;
// 	OccupyFlag * flag;
// 	for (int i = 1; i < MAXOCCUPYFLAG; i++)
// 	{
// 		flag = flagList[i];
// 
// 		if (flag && flag->m_side == m_side && 
// 			abs(flag->m_sX - m_sX) < FLAGRANGE_X &&
// 			abs(flag->m_sY - m_sY) < FLAGRANGE_Y)
// 		{
// 			return true;
// 		}
// 	}

	return false;
}

bool Client::HasPartyRaidBonus() const
{
	// Instead of doing constant checks if a player has the bonus, keep track of when players grant or remove the bonus from joining or leaving
	// then just check for a bool or something on whether it's set which would be updated on players joining/leaving party and player themself
	// changing maps and such that would affect the granting of it - if we're to implement ranges on "abilities" like set below, there should
	// be a better system for handling this instead of functions that check for existence of it (such as a buff tracking system like real mmos)
// 	if(!GetParty())
// 		return false;
// 
// 	while(Client * raidmaster = GetParty()->GetRaidmasters( m_guild ))
// 	{
// 		if(m_cMapIndex == raidmaster->m_cMapIndex &&
// 			abs(m_sX - raidmaster->m_sX) < GUILDPARTYBONUS_RANGE_X &&
// 			abs(m_sY - raidmaster->m_sY) < GUILDPARTYBONUS_RANGE_Y)
// 		{
// 			while(GetParty()->GetRaidmasters( m_guild ));
// 			return true;
// 		}
// 	}

	return false;
}

bool Client::HasPartyHuntBonus() const
{
// 	if(!GetParty())
// 		return false;
// 
// 	while(Client * huntmaster = GetParty()->GetHuntmasters( m_guild ))
// 	{
// 		if(m_cMapIndex == huntmaster->m_cMapIndex &&
// 			abs(m_sX - huntmaster->m_sX) < GUILDPARTYBONUS_RANGE_X &&
// 			abs(m_sY - huntmaster->m_sY) < GUILDPARTYBONUS_RANGE_Y)
// 		{
// 			while(GetParty()->GetHuntmasters( m_guild ));
// 			return true;
// 		}
// 	}

	return false;
}

//TODO: do we care?
bool Client::IsFlooding(uint32_t sensitivity)
{
// 	static uint32_t floodFreq = 0;
// 	static uint32_t floodTime = unixtime();
// 	
// 	uint32_t curTime = unixtime();
// 
// 	if(curTime - floodTime > 3 _s)
// 	{
// 		floodFreq = 0;
// 		floodTime = curTime;
// 	}else{
// 		if(++floodFreq >= sensitivity)
// 			return true;
// 	}

	return false;
}

void Client::WithdrawFromGuild()
{
	//TODO: Client does not withdraw from guild. guild removes client. client should not be accessing guild or party functions
	//whoever added the party/guild managers decided to not let them really manage anything and if we keep the manager code
	//they need to have purpose
// 	if(m_iGuildRank == GUILDRANK_NONE)
// 		return;
// 
// 	gserver->bSendMsgToLS(MSGID_REQUEST_UPDATEGUILDINFO_DELGUILDSMAN, m_handle);
// 
// 	gserver->SendGuildMsg(m_handle, NOTIFY_DISMISSGUILDSMAN, NULL, NULL, NULL);
// 
// 	if(m_guild)
// 	{
// 		m_guild->MemberWithdrawal( this );
// 		m_guild->MemberLogout( this );
// 	}
// 	
// 	if(m_iGuildRank == GUILDRANK_MASTER)
// 	{
// 		gserver->bSendMsgToLS(MSGID_REQUEST_DISBANDGUILD, m_handle);
// 		
// 		if(m_guild)
// 		{
// 			gserver->SendGuildMsg(m_handle, NOTIFY_GUILDDISBANDED, NULL, NULL, NULL);
// 			guildMgr.Remove( m_guild );
// 		}
// 	}
// 
// 	ZeroMemory(m_cGuildName, sizeof(m_cGuildName));
// 	strcpy(m_cGuildName, "NONE");
// 	m_iGuildRank = GUILDRANK_NONE; 
// 	m_iGuildGUID = -1;
// 	m_guild = NULL;
// 
// 	if(GetParty())
// 		partyMgr.RemoveFromParty( this );
// 
// 	gserver->SendEventToNearClient_TypeA(m_handle, OWNERTYPE_PLAYER, MSGID_MOTION_NULL, NULL, NULL, NULL);
// 	gserver->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, COMMONTYPE_CLEARGUILDNAME,
// 		m_cMapIndex, m_sX, m_sY, 0, 0, 0);

}

//TODO: instead of check by ID, when an item is equipped, enable a flag that sets the resist amount
uint16_t Client::IceResist()
{
	if ((Equipped.Chest->m_sIDnum == 770) || (Equipped.Chest->m_sIDnum == 771))
		return true;

	return _IceResist;
}

uint16_t Client::ParalyzeResist()
{

	return _ParalyzeResist;
}

uint16_t Client::FireResist()
{

	return _FireResist;
}

uint16_t Client::PoisonResist()
{

	return _PoisonResist;
}

uint16_t Client::LightningResist()
{

	return _LightningResist;
}

uint16_t Client::ArcaneResist()
{

	return _ArcaneResist;
}

int8_t Client::ChangeCurrency(int8_t id, int64_t amount)
{
	for (stCurrency & c : _currency)
	{
		if (c._id == id)
		{
			if (c._count + amount > c._max)
			{
				c._count = c._max;
				return 2;
			}
			else if (c._count + amount < c._min)
			{
				c._count = c._min;
				return 2;
			}
			else
			{
				c._count += amount;
				return 1;
			}
		}
	}
	return 0;
}

int8_t Client::SetCurrency(int8_t id, int64_t amount)
{
	for (stCurrency & c : _currency)
	{
		if (c._id == id)
		{
			if (amount > c._max)
			{
				c._count = c._max;
				return 2;
			}
			else if (amount < c._min)
			{
				c._count = c._min;
				return 2;
			}
			else
			{
				c._count = amount;
				return 1;
			}
		}
	}
	return 0;
}
