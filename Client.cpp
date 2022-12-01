// Client.cpp: implementation of the CClient class.
//
//////////////////////////////////////////////////////////////////////

#include "Client.h"
#include <string>
#include "team.h"
using namespace std;
extern class Team* c_team;

#pragma warning (disable : 4996)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClient::CClient(HWND hWnd)
{
 int i;	

	m_dwCharID = 0;

	m_pXSock = 0;
	m_pXSock = new class XSocket(hWnd, DEF_CLIENTSOCKETBLOCKLIMIT);
	m_pXSock->bInitBufferSize(DEF_MSGBUFFERSIZE);
	ZeroMemory(m_cProfile, sizeof(m_cProfile));
	strcpy(m_cProfile, "__________");
	ZeroMemory(m_cCharName, sizeof(m_cCharName));
	ZeroMemory(m_cAccountName, sizeof(m_cAccountName));
	ZeroMemory(m_cAccountPassword, sizeof(m_cAccountPassword));
	ZeroMemory(m_cGuildName, sizeof(m_cGuildName));
	ZeroMemory(m_cLocation, sizeof(m_cLocation));
	ZeroMemory(m_cMapName, sizeof(m_cMapName)); // new
	strcpy(m_cLocation, "NONE");
	m_iGuildRank = -1;
	m_iGuildGUID = -1;
	m_bIsInitComplete = false;
	m_iLU_Pool = 0; // removed
	iteam = stoptime = 0;
	m_cWarType = 0;
	m_iEnemyKillCount = 0;
	m_iMaxEK = 0;
	m_iPKCount = 0;
	m_iRewardGold = 0;
	m_iCurWeightLoad = 0;
	m_bIsSafeAttackMode  = false;
	m_bIsSelfSafe = false;
	for (i = 0; i < DEF_MAXITEMEQUIPPOS; i++) {
		m_sItemEquipmentStatus[i] = -1;
	}
	for (i = 0; i < DEF_MAXITEMS; i++) {
		m_pItemList[i]       = 0;
		m_ItemPosList[i].x   = 40;
		m_ItemPosList[i].y   = 30;
		m_bIsItemEquipped[i] = false;
	}
	m_cArrowIndex = -1;
	for (i = 0; i < DEF_MAXBANKITEMS; i++) {
		m_pItemInBankList[i] = 0;
	}
	for (i = 0; i < DEF_MAXMAGICTYPE; i++) {
		m_cMagicMastery[i] = 0;
	}
	for (i = 0; i < DEF_MAXSKILLTYPE; i++) {
		m_cSkillMastery[i] = 0;
	}
	for (i = 0; i < DEF_MAXSKILLTYPE; i++) {
		m_bSkillUsingStatus[i] = false;
		m_iSkillUsingTimeID[i] = 0;
	}
	m_cMapIndex = -1;
	m_sX = -1;
	m_sY = -1;
	m_cDir = 5; 
	m_sType = 0;
	m_sOriginalType = 0;
	m_sAppr1 = 0;
	m_sAppr2 = 0;
	m_sAppr3 = 0;
	m_sAppr4 = 0;
	m_iApprColor = 0;
	m_iStatus = 0;
	m_iStatus2 = 0;
	m_cSex = 0;
	m_cSkin = 0;
	m_cHairStyle = 0;
	m_cHairColor = 0;
	m_cUnderwear = 0;
	m_cAttackDiceThrow_SM = 0;
	m_cAttackDiceRange_SM = 0;
	m_cAttackDiceThrow_L = 0;
	m_cAttackDiceRange_L = 0;
	m_cAttackBonus_SM = 0;
	m_cAttackBonus_L = 0;
	m_cSide = 0;
	m_iHitRatio = 0;
	m_iDefenseRatio = 0;
	m_iClass = 0;
	for (i = 0; i < DEF_MAXITEMEQUIPPOS; i++) {
		m_iDamageAbsorption_Armor[i]  = 0;
	}
	m_iDamageAbsorption_Shield = 0;
	m_iHPstock = 0;
	// <MISSING_VARIABLE_HERE> 00B4h 
	m_bIsKilled = false;
	for (i = 0; i < DEF_MAXMAGICEFFECTS; i++) {
		m_cMagicEffectStatus[i]	= 0;
	}
	m_iWhisperPlayerIndex = -1;
	ZeroMemory(m_cWhisperPlayerName, sizeof(m_cWhisperPlayerName));
	m_iHungerStatus = 100;
	m_bIsWarLocation = false;
	m_bIsPoisoned = false;
	m_iPoisonLevel = 0;
	m_iAdminUserLevel = 0;
	m_iRating = 0;
	m_iTimeLeft_ShutUp = 0;
	m_iTimeLeft_Rating = 0;
	m_iTimeLeft_ForceRecall = 0;
	m_iTimeLeft_FirmStaminar = 0;
	m_iRecentRunTime = 0;
	m_iRecentWalkTime = 0;
	// <MISSING_VARIABLE_HERE> 0BC0h 
	m_sV1 = 0;
	m_bIsOnServerChange = false;
	m_iExpStock = 0;
	m_iAllocatedFish = 0;
	m_iFishChance = 0;

	m_dwGold = 0;

	m_iAngelicStr = 0; // By Snoopy81
	m_iAngelicInt = 0;
	m_iAngelicDex = 0;
	m_iAngelicMag = 0;

	//MORLA 2.2 - Nuevos Datos en el PJ para el Deathmach Game
	m_iDeaths = 0;
	m_iDGPoints = 0;
	m_iDGDeaths = 0;
	m_iDGKills = 0;
	m_iTotalDGDeaths = 0;
	m_iTotalDGKills = 0;

	m_iWantedLevel = 0; // Wanted System

	m_bIsPolymorph = false;

	m_dwDSLAT = m_dwDSLATOld = m_dwDSLATS = 0;
	m_iDSCount = 0;

	// REINTRODUCED by SNOOPY
	m_iHitRatio_ItemEffect_SM = 0;
	m_iHitRatio_ItemEffect_L  = 0;
	m_iMagicHitRatio_ItemEffect = 0;

	ZeroMemory(m_cIPaddress, sizeof(m_cIPaddress)); 
	ZeroMemory(m_cIP2, sizeof(m_cIP2)); 

	
	m_dwAFKCheckTime = 0;

	m_bIsOnWaitingProcess = false;
	m_iSuperAttackLeft = 0;
	m_iSuperAttackCount = 0;
	m_sUsingWeaponSkill = 5;
	m_iManaSaveRatio = 0;
	m_iAddResistMagic = 0;
	m_iAddPhysicalDamage = 0;
	m_iAddMagicalDamage = 0;
	m_bIsLuckyEffect = false;
	m_iSideEffect_MaxHPdown = 0;

	m_iAddAbsLight = 0;
	m_iAddAbsEarth = 0;
	m_iAddAbsFire = 0;
	m_iAddAbsWater = 0;
	m_iAddAbsHoly = 0;
	m_iAddAbsUnholy = 0;
	m_iAddElementAbs = 0;

	m_iComboAttackCount = 0;
	m_iDownSkillIndex = -1;
	m_iMagicDamageSaveItemIndex = -1;
	m_sCharIDnum1 = m_sCharIDnum2 = m_sCharIDnum3 = 0;
	m_iPartyRank = -1; // uncommented new
	m_iPartyMemberCount = 0; // uncommented new
	m_iPartyGUID = 0; // new
	for (i = 0; i < DEF_MAXPARTYMEMBERS; i++) {
		m_stPartyMemberName[i].iIndex = 0;
		ZeroMemory(m_stPartyMemberName[i].cName, sizeof(m_stPartyMemberName[i].cName));
	}
	m_iAbuseCount = 0;
	m_bIsBWMonitor = false;
	m_bIsForceDisconnect = false; // new
	// <MISSING_VARIABLE_HERE> 0CD0h 
	m_bIsBeingResurrected = false;
	m_cExchangeMode = 0;
	m_iFightZoneTicketNumber = m_iFightzoneNumber = m_iReserveTime =  0;
	// <MISSING_VARIABLE_HERE> 0B8Ch 
	// <MISSING_VARIABLE_HERE> 0B88h 
	// <MISSING_VARIABLE_HERE> 0B84h
	m_iExchangeH = 0;
    for (i = 0; i < DEF_MAXSKILLTYPE; i++) m_iSkillSSN[i] = 0;
	for (i = 0; i < 8; i++) {
		m_cExchangeItemIndex[i]  = 0;
		m_iExchangeItemAmount[i] = 0; 
		m_iExchangeAlterIndex[i] = 0;
		ZeroMemory(m_cExchangeItemName[i], sizeof(m_cExchangeItemName[i]));
	}
	m_iExchangeComplete = 0;
	m_bIsExchangeConfirm = false;
	m_iExchangeCount = 0;
	m_iAlterItemDropIndex = -1;

	//Magn0S:: Multi Quest
	for (i = 0; i < DEF_MAXQUEST; i++) m_iQuest[i] = 0;
	for (i = 0; i < DEF_MAXQUEST; i++) m_iQuestID[i] = 0;
	for (i = 0; i < DEF_MAXQUEST; i++) m_iCurQuestCount[i] = 0;
	for (i = 0; i < DEF_MAXQUEST; i++) m_iQuestRewardType[i] = 0;
	for (i = 0; i < DEF_MAXQUEST; i++) m_iQuestRewardAmount[i] = 0;
	for (i = 0; i < DEF_MAXQUEST; i++) m_bIsQuestCompleted[i] = false;

	//m_iQuest = 0;
	//m_iQuestID = 0;
	m_iAskedQuest = 0;
	//m_iCurQuestCount = 0;
	//m_iQuestRewardType = 0;
	//m_iQuestRewardAmount = 0;
	m_iContribution = 0;
	m_bQuestMatchFlag_Loc = false;
	//m_bIsQuestCompleted = false;
	m_cHeroArmourBonus = 0;
	m_bIsNeutral = false;
	m_bIsObserverMode = false;
	m_iSpecialEventID = 200081;
	m_iSpecialWeaponEffectType = 0;
	m_iSpecialWeaponEffectValue = 0;
	m_iAddHP = m_iAddSP = m_iAddMP = 0;
	m_iAddAR = m_iAddPR = m_iAddDR = 0;
	m_iAddAbsPD = m_iAddAbsMD = 0;
	m_iAddCD = m_iAddExp = m_iAddGold = 0;
	m_iSpecialAbilityTime = DEF_SPECABLTYTIMESEC;
	m_iSpecialAbilityType = 0;
	m_bIsSpecialAbilityEnabled = false;
	m_iSpecialAbilityLastSec = 0;
	m_iSpecialAbilityEquipPos = 0;
	m_iMoveMsgRecvCount = 0;
	m_iAttackMsgRecvCount = 0;
	m_iRunMsgRecvCount = 0;
	m_iSkillMsgRecvCount = 0;
	m_bIsAdminCreateItemEnabled = false; // new
	m_bIsAdminCommandEnabled = true;
	m_iAutoExpAmount = 0;
	m_iWarContribution = 0;
	m_dwMoveLAT = m_dwRunLAT = m_dwAttackLAT = 0;
	m_dwInitCCTimeRcv = 0;
	m_dwInitCCTime = 0;
	ZeroMemory(m_cLockedMapName, sizeof(m_cLockedMapName));
	strcpy(m_cLockedMapName, "NONE");
	m_iLockedMapTime = 0;
	// <MISSING_VARIABLE_HERE> 0F1Ch
	m_iCrusadeDuty  = 0;
	m_dwCrusadeGUID = 0;
	for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++) {
		m_stCrusadeStructureInfo[i].cType = 0;
		m_stCrusadeStructureInfo[i].cSide = 0;
		m_stCrusadeStructureInfo[i].sX = 0;
		m_stCrusadeStructureInfo[i].sY = 0;
	}	
	m_iCSIsendPoint = 0;
	m_bIsSendingMapStatus = false;
	ZeroMemory(m_cSendingMapName, sizeof(m_cSendingMapName));
	m_iConstructionPoint = 0;
	ZeroMemory(m_cConstructMapName, sizeof(m_cConstructMapName));
	m_iConstructLocX = m_iConstructLocY = -1;
	// <MISSING_VARIABLE_HERE> 165Ch	
	m_iPartyID = 0;
	m_iPartyStatus = 0;
	m_iReqJoinPartyClientH = 0;	
	ZeroMemory(m_cReqJoinPartyName,sizeof(m_cReqJoinPartyName));
	m_dwLastActionTime = 0; // new
	m_bIsCheckingWhisperPlayer = false;
	// <MISSING_VARIABLE_HERE> 1660h
	m_iGizonItemUpgradeLeft = 0;
	// <MISSING_VARIABLE_HERE> 168Ch
	// <MISSING_VARIABLE_HERE> 1684h
	// <MISSING_VARIABLE_HERE> 1688h
	m_bIsMoveBlocked = false;
	// <MISSING_VARIABLE_HERE> 1694h
	m_bIsOwnLocation = false;
	m_pIsProcessingAllowed = false;
	// <MISSING_VARIABLE_HERE> 16A4h
	m_bIsInsideWarehouse = false;
	// <MISSING_VARIABLE_HERE> 16B0h	
	m_bIsAdminOrderGoto = false;
	m_bIsInsideEnemyBuilding = false; // new
	m_bIsImpossibleZone = false; // rename

	///////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////Modifications////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	//m_iHitRatio_ItemEffect_SM = 0;
	//m_iHitRatio_ItemEffect_L  = 0;
	m_cAura = 0;
	m_dwLogoutHackCheck = 0;
	m_iAddTransMana = 0;
	m_iAddChargeCritical = 0;
	m_bInhibition = false;
	m_iPenaltyBlockYear = m_iPenaltyBlockMonth = m_iPenaltyBlockDay = 0; // v1.4
	ZeroMemory(m_cExchangeName, sizeof(m_cExchangeName));			// ±³È¯ÇÒ ´ë»óÀÇ ÀÌ¸§ 
	ZeroMemory(m_cExchangeItemName, sizeof(m_cExchangeItemName));	// ±³È¯ÇÒ ¾ÆÀÌÅÛ ÀÌ¸§ 	
	m_dwHeldenianGUID = 0;
	m_bIsInsideWizardTower = false;
	m_cHeroArmorBonus = 0;
	m_bMagicConfirm = false;
	m_bMagicItem = false;
	m_iSpellCount = 0;
	m_bMagicPauseTime = false;
	m_bIsClientConnected = false;

	m_bIsExchangeMode = false;

	//50Cent - Repair All
    totalItemRepair = 0;
	for (i = 0; i < DEF_MAXITEMS; i++) {
		m_stRepairAll[i].index = 0;
		m_stRepairAll[i].price = 0;
	}

	//Magn0S:: New Variables
	m_iCoinPoints = 0;
	bShowDmg = false;

	m_bIsOnApocMap = false;
	///////////////////////////////////////////////////////////////////////////////////////
}

CClient::~CClient()
{
 int i;
	
	if (m_pXSock != 0) delete m_pXSock;
	for (i = 0; i < DEF_MAXITEMS; i++)
		if (m_pItemList[i] != 0) {
			delete m_pItemList[i];
			m_pItemList[i] = 0;
		}
	for(i = 0; i < DEF_MAXBANKITEMS; i++)
		if (m_pItemInBankList[i] != 0) {
			delete m_pItemInBankList[i];
			m_pItemInBankList[i]=0;
		}
}

int CClient::bCreateNewParty()
{
 int i;

	if (m_iPartyRank != -1) return 0;
	m_iPartyRank = 0;
	m_iPartyMemberCount = 0;
	m_iPartyGUID = (rand() % 999999) + timeGetTime();
	for (i = 0; i < DEF_MAXPARTYMEMBERS; i++) {
		m_stPartyMemberName[i].iIndex = 0;
		ZeroMemory(m_stPartyMemberName[i].cName, sizeof(m_stPartyMemberName[i].cName));
	}
	return 1;
}


bool CClient::IsInMap(char* mapname)
{
	return string(m_cMapName) == mapname;
}



bool CClient::IsLocation(char* location)
{
	return string(m_cLocation) == location;
}

bool CClient::IsInsideTeam()
{
	return string(m_cMapName) == "team" && c_team->bteam;
}

bool CClient::IsTeamPlayer()
{
	return std::find(c_team->m_team.begin(), c_team->m_team.end(), m_cCharName) != c_team->m_team.end();
}


void CClient::Send(int h, WORD msg, DWORD sV1, DWORD sV2, DWORD sV3,
	char* pString, DWORD sV4, DWORD sV5,
	DWORD sV6, DWORD sV7, DWORD sV8, DWORD sV9, char* pString2)
{

	G_pGame->SendNotifyMsg(0, h, msg, sV1, sV2, sV3, pString,
		sV4, sV5, sV6, sV7, sV8, sV9, pString2);
}
