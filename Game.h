// Game.h: interface for the CGame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAME_H__C3D29FC5_755B_11D2_A8E6_00001C7030A6__INCLUDED_)
#define AFX_GAME_H__C3D29FC5_755B_11D2_A8E6_00001C7030A6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
//#pragma warning( disable: 4700 4305 4309)
/*
warning C4305: '=' : truncation from 'const int' to 'short'
warning C4309: '=' : truncation of constant value
warning C4700: local variable 'i' used without having been initialized
*/
#endif

#include <windows.h>
#include <winbase.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <memory.h>
#include <direct.h>
#include <vector>

#include "winmain.h"
#include "StrTok.h"
#include "Xsocket.h"
#include "Client.h"
#include "Npc.h"
#include "Map.h"
#include "ActionID.h"
#include "UserMessages.h"
#include "NetMessages.h"
#include "MessageIndex.h"
#include "Misc.h"
#include "Msg.h"
#include "Magic.h"
#include "Skill.h"
#include "DynamicObject.h"
#include "DelayEvent.h"
#include "Version.h"
#include "Fish.h"
#include "DynamicObject.h"
#include "DynamicObjectID.h"
#include "Potion.h"
#include "Mineral.h"
#include "Quest.h"
#include "BuildItem.h"
#include "Teleport.h"
#include "TeleportLoc.h"
#include "GlobalDef.h"
#include "TempNpcItem.h"
#include "GuildsMan.h"
#include "Party.h"
#include "Heldenian.h"
#include "Apocalypse.h"
#include "Crusade.h"

// Time
#define _s					* CLOCKS_PER_SEC
#define _ms					_s / 1000
#define _m					_s * 60
#define _h					_m * 60
#define _d					_h * 24

class CGame
{

public:

	CGame(HWND hWnd);
	virtual ~CGame();

	bool bAccept(class XSocket* pXSock);
	void DisplayInfo(HDC hdc);
	bool bInit();
	void Quit();
	void OnTimer(char cType);
	
	void OnKeyUp(WPARAM wParam, LPARAM lParam);
	void OnKeyDown(WPARAM wParam, LPARAM lParam);
	bool bOnClose();
	
	void OnGateSocketEvent(UINT message, WPARAM wParam, LPARAM lParam);
	void OnMainLogSocketEvent(UINT message, WPARAM wParam, LPARAM lParam);
	void OnSubLogSocketEvent(UINT message, WPARAM wParam, LPARAM lParam);
	void OnClientSocketEvent(UINT message, WPARAM wParam, LPARAM lParam);
	void OnStartGameSignal();
	
	void SendNotifyMsg(int iFromH, int iToH, WORD wMsgType, DWORD sV1, DWORD sV2, DWORD sV3, char* pString, DWORD sV4 = 0, DWORD sV5 = 0, DWORD sV6 = 0, DWORD sV7 = 0, DWORD sV8 = 0, DWORD sV9 = 0, char* pString2 = 0, DWORD sV10 = 0, DWORD sV11 = 0, DWORD sV12 = 0, DWORD sV13 = 0, DWORD sV14 = 0);
	void RequestTeleportHandler(int iClientH, char* pData, char* cMapName = 0, int dX = -1, int dY = -1, bool deleteteam = false);
	void ShowClientMsg(int iClientH, char* pMsg);
	void calcularTop15HB(int iClientH);
	void SendAlertMsg(int client, char* pMsg);
	bool _bInitItemAttr(class CItem* pItem, char* pItemName);
	bool _bAddClientItemList(int iClientH, class CItem* pItem, int* pDelReq);
	bool bEquipItemHandler(int iClientH, short sItemIndex, bool bNotify = true);
	void ItemDepleteHandler(int iClientH, short sItemIndex, bool bIsUseItemResult, bool bIsItemUsed);

	void ParseCommand(char* pMsg);

	char m_cGameServerAddrInternal[16];
	char m_cGameServerAddrExternal[16];
	int  m_iGameServerMode;
	char m_cGameServerAddr[16];
	int  m_iGameServerPort;

	class CClient* m_pClientList[DEF_MAXCLIENTS];
	class CMap* m_pMapList[DEF_MAXMAPS];

private:

	bool m_bSQLMode, m_bAntiHackMode, m_bBlockLocalConn;

	int srvHour, srvMinute;

	void ResetStatsHandler(int iClientH, char* pData, DWORD dwMsgSize);

	short m_sJailTime;

	void ShowSpellEffect2(int iClientH, short sType, int ownertype);

	void GenerateMonsterSlaves(int MapIndex, int iX, int iY, int iOrder);

	void StormBringer(int iClientH, short dX, short dY);

	void _CreateNewGuildFile(char* cGuildName);
	int _iComposeGuildDataFileContents(int iGuildH, char* pData);
	int ObtenerNuevoID();

	struct {
		int iGuildLevel;
		char cGuildName[21];
		class CItem* m_pItemInBankList[DEF_MAXBANKITEMS];
	}m_stGuild[DEF_MAXGUILDS];

	int iCityMaxHP[2], iCityHP[2];

	void minimap_clear_apoc(int client);
	void minimap_update_apoc(int client);

	void SetClass(int iClientH);
	bool m_bHappyHour, m_bFuryHour;

	void ManageHappyHour();
	void ManageFuryHour();

	void GlobalStartGladiatorArena();
	void GlobalEndGladiatorArena();
	void ManualStartGladiatorArena(int iClientH, char* pData, DWORD dwMsgSize);
	void ManualEndGladiatorArena(int iClientH, char* pData, DWORD dwMsgSize);

	void RequestLeaveArena(int iClientH);
	void RequestArenaStatus(int iSubjectH, bool bUpdate);
	void RequestArenaStatus(int iClientH);

	void CheckDenialServiceAttack(int iClientH, DWORD dwClientTime);
	bool iUpgradeHeroItemRequirements(int iClientH, int iItemIndex);

	void PlayerOrder_Criticals(int iClientH);

	int m_iCTFEventFlagHolder[2], m_iCTFEventFlag[2];
	char m_cCFTEventCount[2];
	void UpdateEventStatus();
	void AdminOrder_SetEvent();

	
	void SendLoginData(int client);

	void NotifyEvents();
	bool _candy_boost = false, _revelation = false, _city_teleport = false, _drop_inhib = false;

	void RequestSetTrapHandler(int iClientH, char* pData);

	void AdminOrder_SetObserverMode(int iClientH);

	void CalcExpStock(int iClientH);

	//50Cent - Repair All
	void RequestRepairAllItemsHandler(int iClientH);
	void RequestRepairAllItemsDeleteHandler(int iClientH, int index);
	void RequestRepairAllItemsConfirmHandler(int iClientH);

	void AdminOrder_BanPj(int iClientH, char* pData, DWORD dwMsgSize);

	void HeldenianStartWarNow();
	void HeldenianEndWarNow();
	void HeldenianVictoryNow(int iSide);

	//wanted system
	void SetWantedFlag(short sOwnerH, char cOwnerType, bool iStatus);

	bool  iUpgradeHeroCapeRequirements(int iClientH, int iItemIndex);
	int  HeroItemChecker(int iItemID, short sEnemyKill, char cContribution, char cSide);

	// KLKS clean tiles

	void ClearMap();
	void RequestHeldenianScroll(int iClientH, char* pData, DWORD dwMsgSize);
	char cGetNextMoveArea(short sOwnerH, short sX, short sY, short dstX, short dstY, char cMapIndex, char cTurn, int* pError, char cArea);
	bool bGetNpcMovementArea(short sOwnerH, short pX, short pY, char cMapIndex, char cArea);
	bool bGetEmptyArea(short sOwnerH, char cMapIndex, short pX, short pY, char cArea);
	void CheckHeldenianResultCalculation(int iClientH);
	bool MobBossGenerator(int iMapIndex);

	void DoAbaddonThunderDamageHandler(char cMapIndex);
	void SendThunder(int iClient, short sX, short sY, short sV3, short sV4);


	void bReadScheduleConfigFile(char* pFn);
	void bReadApocalypseGUIDFile(char* cFn);

	void _CreateApocalypseGUID(DWORD dwApocalypseGUID);
	void LocalEndApocalypse();
	void LocalStartApocalypse(DWORD dwApocalypseGUID);
	void GlobalEndApocalypseMode(int iClientH);
	void GlobalStartApocalypseMode(int iClientH, int iMode); // Changed by SNOOPY

	// SNOOPY: Added Apocalypse functions
	void GenerateApocalypseBoss(int MapIndex);
	void OpenCloseApocalypseGate();
	void Notify_ApocalypseGateState(int iClientH);
	void Use_ApocalypseGate(int iClientH);
	void ForceOpen_ApocalypseGate();
	void ForceClose_ApocalypseGate();
	void Open_EmptyMap_Gate(int MapIndex);
	void GenerateSlime(int MapIndex);

	int iGetPlayerStatus(int iClientH, short sOwnerH);

	

	void LocalStartHeldenianMode(short sV1, short sV2, DWORD dwHeldenianGUID);
	void GlobalStartHeldenianMode();

	void HeldenianWarStarter();
	bool UpdateHeldenianStatus(int iClientH);
	void _CreateHeldenianGUID(DWORD dwHeldenianGUID, int iWinnerSide);
	void ManualStartHeldenianMode(int iClientH, char* pData, DWORD dwMsgSize);
	void ManualEndHeldenianMode(int iClientH, char* pData, DWORD dwMsgSize);

	bool _bCheckCharacterData(int iClientH);
	bool _bDecodeNpcItemConfigFileContents(char* cFn);
	void GlobalUpdateConfigs(char cConfigType);
	void LocalUpdateConfigs(char cConfigType);
	void GlobalEndHeldenianMode();
	void LocalEndHeldenianMode(DWORD dwHeldenianGUID, int iWinner);

	void BattleMageMagicHandler(int iClientH, int dX, int dY, short sType, bool bItemEffect, int iV1);

	void RemoveEventNpc(int iNpcH);
	void RemoveOccupyFlags(int iMapIndex);

	void RequestHeldenianTeleportList(int iClientH, char* pData, DWORD dwMsgSize);
	void RequestHeldenianTeleportNow(int iClientH, char* pData, DWORD dwMsgSize);

	bool bCheckHeldenianMap(int sAttackerH, char cType);
	void SetHeroFlag(short sOwnerH, char cOwnerType, bool bStatus);
	void SetInhibitionCastingFlag(short sOwnerH, char cOwnerType, bool bStatus);
	void bCalculateEnduranceDecrement(short sTargetH, short sAttackerH, char cTargetType, int iArmorType);
	int _cCheckHeroItemEquipped(int iClientH);
	bool bPlantSeedBag(int iMapIndex, int dX, int dY, int iItemEffectValue1, int iItemEffectValue2, int iClientH);
	void _CheckFarmingAction(short sAttackerH, short sTargetH, bool bType);

	void bReadHeldenianGUIDFile(char* cFn);

	// KLKS clean tiles
	void AdminOrder_CleanMap(int iClientH, char* pData, DWORD dwMsgSize);

	// Lists
	bool bReadBannedListConfigFile(char* pFn);
	bool bReadAdminListConfigFile(char* pFn);

	// Crusade
	void ManualEndCrusadeMode(int iWinnerSide); // 2.17 (x) 2.14 ( )
	void CrusadeWarStarter();
	void bReadCrusadeGUIDFile(char* cFn);
	void _CreateCrusadeGUID(DWORD dwCrusadeGUID, int iWinnerSide);
	void GlobalStartCrusadeMode();
	void GSM_SetGuildTeleportLoc(int iGuildGUID, int dX, int dY, char* pMapName);
	void SyncMiddlelandMapInfo();
	void _SendMapStatus(int iClientH);
	void MapStatusHandler(int iClientH, int iMode, char* pMapName);
	void SelectCrusadeDutyHandler(int iClientH, int iDuty);
	void RequestSummonWarUnitHandler(int iClientH, int dX, int dY, char cType, char cNum, char cMode);
	void RequestGuildTeleportHandler(int iClientH);
	void RequestSetGuildTeleportLocHandler(int iClientH, int dX, int dY, int iGuildGUID, char* pMapName);
	void MeteorStrikeHandler(int iMapIndex);
	void _LinkStrikePointMapIndex();
	void MeteorStrikeMsgHandler(char cAttackerSide);
	void _NpcBehavior_GrandMagicGenerator(int iNpcH);
	void CollectedManaHandler(WORD wAresdenMana, WORD wElvineMana);
	void SendCollectedMana();
	void _GrandMagicLaunchMsgSend(int iType, char cAttackerSide);
	void GrandMagicResultHandler(char* cMapName, int iCrashedStructureNum, int iStructureDamageAmount, int iCasualities, int iActiveStructure, int iTotalStrikePoints, char* cData);
	void CalcMeteorStrikeEffectHandler(int iMapIndex);
	void DoMeteorStrikeDamageHandler(int iMapIndex);
	void RequestSetGuildConstructLocHandler(int iClientH, int dX, int dY, int iGuildGUID, char* pMapName);
	void GSM_SetGuildConstructLoc(int iGuildGUID, int dX, int dY, char* pMapName);
	void GSM_ConstructionPoint(int iGuildGUID, int iPoint);
	void CheckCommanderConstructionPoint(int iClientH);
	void bReadCrusadeStructureConfigFile(char* cFn);

	void LocalEndCrusadeMode(int iWinnerSide);
	void LocalStartCrusadeMode(DWORD dwGuildGUID);
	void CheckCrusadeResultCalculation(int iClientH);
	bool _bNpcBehavior_Detector(int iNpcH);
	bool _bNpcBehavior_ManaCollector(int iNpcH);
	bool __bSetConstructionKit(int iMapIndex, int dX, int dY, int iType, int iTimeCost, int iClientH);

	bool isLeap(int y);

	// Acidx commands

	void AdminOrder_Pushplayer(int iClientH, char* pData, DWORD dwMsgSize);

	void AdminOrder_CheckRep(int iClientH, char* pData, DWORD dwMsgSize);


	void ApplyCombatKilledPenalty(int iClientH, int cPenaltyLevel, bool bIsSAattacked);


	void AdminOrder_ClearNpc(int iClientH);

	// Settings.cfg
	bool bReadSettingsConfigFile(char* cFn);



	// Daryl - AdminSettings.cfg
	bool bReadAdminSetConfigFile(char* cFn);


	// Hack Checks
	void bCheckClientMoveFrequency(int iClientH, DWORD dwClientTime);
	void bCheckClientMagicFrequency(int iClientH, DWORD dwClientTime);
	void bCheckClientAttackFrequency(int iClientH, DWORD dwClientTime);



	//Hypnotoad functions
	void SetDefenseShieldFlag(short sOwnerH, char cOwnerType, bool bStatus);
	void SetMagicProtectionFlag(short sOwnerH, char cOwnerType, bool bStatus);
	void SetProtectionFromArrowFlag(short sOwnerH, char cOwnerType, bool bStatus);
	void SetIllusionMovementFlag(short sOwnerH, char cOwnerType, bool bStatus);
	void SetIllusionFlag(short sOwnerH, char cOwnerType, bool bStatus);


	void RequestChangePlayMode(int iClientH);
	void GetHeroMantleHandler(int iClientH, int iItemID, char* pString);

	void bCheckMagicInt(int iClientH);
	bool bChangeState(char cStateChange, char* cStr, char* cVit, char* cDex, char* cInt, char* cMag, char* cChar);
	void StateChangeHandler(int iClientH, char* pData, DWORD dwMsgSize);

	void SetPoisonFlag(short sOwnerH, char cOwnerType, bool bStatus);

	void AdminOrder_EnableAdminCommand(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_CreateItem(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_Summon(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_SummonAll(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_SummonPlayer(int iClientH, char* pData, DWORD dwMsgSize);

	void AdminOrder_UnsummonAll(int iClientH);
	void AdminOrder_SetAttackMode(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_SummonDemon(int iClientH);
	void AdminOrder_SetInvi(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_Polymorph(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_GetNpcStatus(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_CheckIP(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_CreateFish(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_Teleport(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_ReserveFightzone(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_CloseConn(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_CallGuard(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_DisconnectAll(int iClientH, char* pData, DWORD dwMsgSize);

	bool bCopyItemContents(class CItem* pOriginal, class CItem* pCopy);
	int  iGetMapLocationSide(char* pMapName);
	void ChatMsgHandlerGSM(int iMsgType, int iV1, char* pName, char* pData, DWORD dwMsgSize);
	void RemoveClientShortCut(int iClientH);
	bool bAddClientShortCut(int iClientH);

	void GSM_RequestFindCharacter(WORD wReqServerID, WORD wReqClientH, char* pName, char* pFinder); // New 16/05/2001 Changed
	void ServerStockMsgHandler(char* pData);
	void SendStockMsgToGateServer();
	bool bStockMsgToGateServer(char* pData, DWORD dwSize);
	void RequestHelpHandler(int iClientH);

	void CheckConnectionHandler(int iClientH, char* pData);

	void SetAngelFlag(short sOwnerH, char cOwnerType, int iStatus, int iTemp);
	void GetAngelHandler(int iClientH, char* pData, DWORD dwMsgSize);
	void ChangeClassHandler(int iClientH, char* pData, DWORD dwMsgSize);

	void ReqCreateCraftingHandler(int iClientH, char* pData);

	void RequestPurchaseItemHandler2(int iClientH, char* pItemName, int iNum, int iOption);

	void PlayerOrder_ChangeCity(int iClientH, bool bChange);
	void RequestRango(int iClientH, int iObjectID);

	void AgingMapSectorInfo();
	void UpdateMapSectorInfo();
	bool bGetItemNameWhenDeleteNpc(int& iItemID, short sNpcType);
	int iGetItemWeight(class CItem* pItem, int iCount);
	void CancelQuestHandler(int iClientH, int iQuest);
	void ActivateSpecialAbilityHandler(int iClientH);
	void EnergySphereProcessor(bool bIsAdminCreate = false, int iClientH = 0);
	
	void JoinPartyHandler(int iClientH, int iV1, char* pMemberName);
	void CreateNewPartyHandler(int iClientH);
	void _DeleteRandomOccupyFlag(int iMapIndex);
	void RequestSellItemListHandler(int iClientH, char* pData);
	void RequestRestartHandler(int iClientH);
	int iRequestPanningMapDataRequest(int iClientH, char* pData);
	void GetMagicAbilityHandler(int iClientH);
	void Effect_Damage_Spot_DamageMove(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sAtkX, short sAtkY, short sV1, short sV2, short sV3, bool bExp, int iAttr);
	void _TamingHandler(int iClientH, int iSkillNum, char cMapIndex, int dX, int dY);
	void RequestCheckAccountPasswordHandler(char* pData, DWORD dwMsgSize);
	int _iTalkToNpcResult_Guard(int iClientH, int* pQuestType, int* pMode, int* pRewardType, int* pRewardAmount, int* pContribution, char* pTargetName, int* pTargetType, int* pTargetCount, int* pX, int* pY, int* pRange);
	void SetIceFlag(short sOwnerH, char cOwnerType, bool bStatus);
	void _bDecodeNoticementFileContents(char* pData, DWORD dwMsgSize);
	void RequestNoticementHandler(int iClientH, char* pData);
	void _AdjustRareItemValue(class CItem* pItem);
	bool _bCheckDupItemID(class CItem* pItem);
	void _bDecodeDupItemIDFileContents(char* pData, DWORD dwMsgSize);
	void NpcDeadItemGenerator(int iNpcH, short sAttackerH, char cAttackerType);
	int  iGetPlayerABSStatus(int iWhatH, int iRecvH);

	char _cGetSpecialAbility(int iKindSA);
	void BuildItemHandler(int iClientH, char* pData);
	bool _bDecodeBuildItemConfigFileContents(char* pData, DWORD dwMsgSize);
	bool _bCheckSubLogSocketIndex();
	void _CheckGateSockConnection();
	void OnSubLogRead(int iIndex);

	void _CheckStrategicPointOccupyStatus(char cMapIndex);
	void GetMapInitialPoint(int iMapIndex, short* pX, short* pY, char* pPlayerLocation = 0);
	int  iGetMaxHP(int iClientH, bool bBloodEffect = false);
	int  iGetMaxMP(int iClientH);
	int  iGetMaxSP(int iClientH);
	void _ClearQuestStatus(int iClientH, int iQuest);
	bool _bCheckItemReceiveCondition(int iClientH, class CItem* pItem);
	void SendItemNotifyMsg(int iClientH, WORD wMsgType, class CItem* pItem, int iV1);

	int _iTalkToNpcResult_WTower(int iClientH, int* pQuestType, int* pMode, int* pRewardType, int* pRewardAmount, int* pContribution, char* pTargetName, int* pTargetType, int* pTargetCount, int* pX, int* pY, int* pRange);
	int _iTalkToNpcResult_WHouse(int iClientH, int* pQuestType, int* pMode, int* pRewardType, int* pRewardAmount, int* pContribution, char* pTargetName, int* pTargetType, int* pTargetCount, int* pX, int* pY, int* pRange);
	int _iTalkToNpcResult_BSmith(int iClientH, int* pQuestType, int* pMode, int* pRewardType, int* pRewardAmount, int* pContribution, char* pTargetName, int* pTargetType, int* pTargetCount, int* pX, int* pY, int* pRange);
	int _iTalkToNpcResult_GShop(int iClientH, int* pQuestType, int* pMode, int* pRewardType, int* pRewardAmount, int* pContribution, char* pTargetName, int* pTargetType, int* pTargetCount, int* pX, int* pY, int* pRange);
	int _iTalkToNpcResult_GuildHall(int iClientH, int* pQuestType, int* pMode, int* pRewardType, int* pRewardAmount, int* pContribution, char* pTargetName, int* pTargetType, int* pTargetCount, int* pX, int* pY, int* pRange);
	bool _bCheckIsQuestCompleted(int iClientH, int iQuest);
	void _CheckQuestEnvironment(int iClientH);
	void _SendQuestContents(int iClientH);
	void QuestAcceptedHandler(int iClientH);
	bool _bDecodeQuestConfigFileContents(char* pData, DWORD dwMsgSize);

	void CancelExchangeItem(int iClientH);
	bool bAddItem(int iClientH, class CItem* pItem, char cMode);
	void ConfirmExchangeItem(int iClientH);
	void SetExchangeItem(int iClientH, int iItemIndex, int iAmount);
	void ExchangeItemHandler(int iClientH, short sItemIndex, int iAmount, short dX, short dY, WORD wObjectID, char* pItemName);

	void _BWM_Command_Shutup(char* pData);
	void _BWM_Init(int iClientH, char* pData);
	void CheckUniqueItemEquipment(int iClientH);
	void _SetItemPos(int iClientH, char* pData);

	bool _bDecodeOccupyFlagSaveFileContents(char* pData, DWORD dwMsgSize);
	void GetOccupyFlagHandler(int iClientH);
	int  _iComposeFlagStatusContents(char* pData);
	void SetSummonMobAction(int iClientH, int iMode, DWORD dwMsgSize, char* pData = 0);
	bool __bSetOccupyFlag(char cMapIndex, int dX, int dY, int iSide, int iEKNum, int iClientH, bool bAdminFlag);
	bool _bDepleteDestTypeItemUseEffect(int iClientH, int dX, int dY, short sItemIndex, short sDestItemID);
	void SetDownSkillIndexHandler(int iClientH, int iSkillIndex);
	int iGetComboAttackBonus(int iSkill, int iComboCount);
	int  _iGetWeaponSkillType(int iClientH);
	void CheckFireBluring(char cMapIndex, int sX, int sY);
	void NpcTalkHandler(int iClientH, int iWho, int iQuest); // Magn0S:: Quest List
	void bDeleteMineral(int iIndex);
	void _CheckMiningAction(int iClientH, int dX, int dY);
	int iCreateMineral(char cMapIndex, int tX, int tY, char cLevel);
	void MineralGenerator();
	void LocalSavePlayerData(int iClientH);
	bool _bDecodePotionConfigFileContents(char* pData, DWORD dwMsgSize);
	void ReqCreatePotionHandler(int iClientH, char* pData);
	void _CheckAttackType(int iClientH, short* spType);

	void ForceDisconnectAccount(char* pAccountName, WORD wCount);
	void NpcRequestAssistance(int iNpcH);
	void ToggleSafeAttackModeHandler(int iClientH);
	void SetBerserkFlag(short sOwnerH, char cOwnerType, bool bStatus);
	void SpecialEventHandler();
	int iGetNpcRelationship_SendEvent(int iNpcH, int iOpponentH);
	int _iForcePlayerDisconect(int iNum);
	int iGetMapIndex(char* pMapName);
	int iGetNpcRelationship(int iWhatH, int iRecvH);
	int iGetPlayerRelationship(int iClientH, int iOpponentH);
	int iGetWhetherMagicBonusEffect(short sType, char cWheatherStatus);
	void WeatherProcessor();
	int _iCalcPlayerNum(char cMapIndex, short dX, short dY, char cRadius);
	void FishGenerator();
	void ReqGetFishThisTimeHandler(int iClientH);
	void FishProcessor();
	int iCheckFish(int iClientH, char cMapIndex, short dX, short dY);
	bool bDeleteFish(int iHandle, int iDelMode);
	int  iCreateFish(char cMapIndex, short sX, short sY, short sDifficulty, class CItem* pItem, int iDifficulty, DWORD dwLastTime);

	// v1.4311-3 º¯°æ ÇÔ¼ö ¼±¾ð  Expire -> Ban À¸·Î 
	void UserCommand_BanGuildsman(int iClientH, char* pData, DWORD dwMsgSize);
	// v1.4311-3 Ãß°¡ ÇÔ¼ö ¼±¾ð ÀÌº¥Æ®¸¦ ´ëºñÇÏ¿© ¿î¿µÀÚ°¡ »çÅõÀåÀ» ¿¹¾àÇØ³õ´Â´Ù.
	int iGetExpLevel(DWORD iExp);
	
	void ResponseSavePlayerDataReplyHandler(char* pData, DWORD dwMsgSize);
	void NoticeHandler();
	bool bReadNotifyMsgListFile(char* cFn);
	void SetPlayerReputation(int iClientH, char* pMsg, char cValue, DWORD dwMsgSize);
	void ShutUpPlayer(int iClientH, char* pMsg, DWORD dwMsgSize);
	void CheckDayOrNightMode();
	
	bool bCheckResistingPoisonSuccess(short sOwnerH, char cOwnerType);
	void PoisonEffect(int iClientH, int iV1);
	void bSetNpcAttackMode(char* cName, int iTargetH, char cTargetType, bool bIsPermAttack);
	bool _bGetIsPlayerHostile(int iClientH, int sOwnerH);
	bool bAnalyzeCriminalAction(int iClientH, short dX, short dY, bool bIsCheck = false);

	int _iGetPlayerNumberOnSpot(short dX, short dY, char cMapIndex, char cRange);
	void CalcTotalItemEffect(int iClientH, int iEquipItemID, bool bNotify = true);
	
	void GetPlayerProfile(int iClientH, char* pMsg, DWORD dwMsgSize);
	void SetPlayerProfile(int iClientH, char* pMsg, DWORD dwMsgSize);
	void ToggleWhisperPlayer(int iClientH, char* pMsg, DWORD dwMsgSize);
	void CheckAndNotifyPlayerConnection(int iClientH, char* pMsg, DWORD dwSize);
	int iCalcTotalWeight(int iClientH);
	void ReqRepairItemCofirmHandler(int iClientH, char cItemID, char* pString);
	void ReqRepairItemHandler(int iClientH, char cItemID, char cRepairWhom, char* pString);
	void ReqSellItemConfirmHandler(int iClientH, char cItemID, int iNum, char* pString);
	void ReqSellItemHandler(int iClientH, char cItemID, char cSellToWhom, int iNum, char* pItemName);
	void UseSkillHandler(int iClientH, int iV1, int iV2, int iV3);
	int  iCalculateUseSkillItemEffect(int iOwnerH, char cOwnerType, char cOwnerSkill, int iSkillNum, char cMapIndex, int dX, int dY);
	void ClearSkillUsingStatus(int iClientH);
	void DynamicObjectEffectProcessor();
	int _iGetTotalClients();
	void SendObjectMotionRejectMsg(int iClientH);
	void SetInvisibilityFlag(short sOwnerH, char cOwnerType, bool bStatus);
	bool bRemoveFromDelayEventList(int iH, char cType, int iEffectType);
	void DelayEventProcessor();
	bool bRegisterDelayEvent(int iDelayType, int iEffectType, DWORD dwLastTime, int iTargetH, char cTargetType, char cMapIndex, int dX, int dY, int iV1, int iV2, int iV3);
	int iGetFollowerNumber(short sOwnerH, char cOwnerType);
	int  _iCalcSkillSSNpoint(int iLevel);

	void bCheckTotalSkillMasteryPoints(int iClientH, int iSkill);
	bool bSetItemToBankItem(int iClientH, class CItem* pItem);
	void NpcMagicHandler(int iNpcH, short dX, short dY, short sType);
	bool bCheckResistingTremorSuccess(short sTargetH, char cTargetType);
	bool bCheckResistingIceSuccess(char cAttackerDir, short sTargetH, char cTargetType, int iHitRatio);
	bool bCheckResistingMagicSuccess(char cAttackerDir, short sTargetH, char cTargetType, int iHitRatio);
	void Effect_SpUp_Spot(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3);
	void Effect_SpDown_Spot(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3);
	void Effect_HpUp_Spot(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3);
	void Effect_Damage_Spot(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3, bool bExp, int iAttr = 0);
	void Effect_Damage_Spot_Type2(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sAtkX, short sAtkY, short sV1, short sV2, short sV3, bool bExp, int iAttr);
	void UseItemHandler(int iClientH, short sItemIndex, short dX, short dY, short sDestItemID);
	void NpcBehavior_Stop(int iNpcH);
	int _iGetArrowItemIndex(int iClientH);
	void RequestFullObjectData(int iClientH, char* pData);
	void DeleteNpc(int iNpcH);
	void CalcNextWayPointDestination(int iNpcH);
	void MobGenerator();
	void CalculateSSN_SkillIndex(int iClientH, short sSkillIndex, int iValue);
	void CalculateSSN_ItemIndex(int iClientH, short sWeaponIndex, int iValue);
	void CheckDynamicObjectList();
	int  iAddDynamicObjectList(short sOwner, char cOwnerType, short sType, char cMapIndex, short sX, short sY, DWORD dwLastTime, int iV1 = 0, int iV2 = 0, int iV3 = 0);
	int _iCalcMaxLoad(int iClientH);
	void GetRewardMoneyHandler(int iClientH);
	void _PenaltyItemDrop(int iClientH, int iTotal, bool bIsSAattacked = false);
	void EnemyKillRewardHandler(int iAttackerH, int iClientH);
	void PK_KillRewardHandler(short sAttackerH, short sVictumH);
	void ApplyPKpenalty(short sAttackerH, short sVictumH);
	bool bSetItemToBankItem(int iClientH, short sItemIndex);
	void RequestRetrieveItemHandler(int iClientH, char* pData);
	void RequestCivilRightHandler(int iClientH, char* pData);
	bool bCheckLimitedUser(int iClientH);
	void LevelUpSettingsHandler(int iClientH, char* pData, DWORD dwMsgSize);
	// v1.4311-3 ¼±¾ð ÇÔ¼ö  »çÅõÀå ¿¹¾à ÇÔ¼ö ¼±¾ð FightzoneReserveHandler
	void FightzoneReserveHandler(int iClientH, char* pData, DWORD dwMsgSize);
	void bCheckLevelUp(int iClientH);

	DWORD iGetLevelExp(int iLevel);
	void TimeManaPointsUp(int iClientH);
	void TimeStaminarPointsUp(int iClientH);

	bool SpecialWeapon_DS(int iClientH);
	bool __bReadMapInfo(int iMapIndex);
	bool bBankItemToPlayer(int iClientH, short sItemIndex);
	bool bPlayerItemToBank(int iClientH, short sItemIndex);
	int  _iGetSkillNumber(char* pSkillName);
	void TrainSkillResponse(bool bSuccess, int iClientH, int iSkillNum, int iSkillLevel);
	int _iGetMagicNumber(char* pMagicName, int* pReqInt, int* pCost);
	bool RequestStudyMagicHandler(int iClientH, char* pName, bool bSucces = false, bool bIsPurchase = true);
	bool _bDecodeSkillConfigFileContents(char* pData, DWORD dwMsgSize);
	bool _bDecodeMagicConfigFileContents(char* pData, DWORD dwMsgSize);
	void ReleaseFollowMode(short sOwnerH);
	bool bSetNpcFollowMode(char* pName, char* pFollowName, char cFollowOwnerType);
	void PlayerMagicHandler(int iClientH, int dX, int dY, short sType, bool bItemEffect = false, int iV1 = 0, bool bIgnoreOwnerLimits = false);
	int  iClientMotion_Magic_Handler(int iClientH, short sX, short sY, char cDir);
	void SendMsgToGateServer(DWORD dwMsg, int iClientH, char* pData = 0);
	void OnGateRead();

	void ToggleCombatModeHandler(int iClientH);
	void GuildNotifyHandler(char* pData, DWORD dwMsgSize);
	void SendGuildMsg(int iClientH, WORD wNotifyMsgType, short sV1, short sV2, char* pString);

	void TimeHitPointsUp(int iClientH);

	int iDice(int iThrow, int iRange);
	bool _bInitNpcAttr(class CNpc* pNpc, char* pNpcName, short sClass, char cSA);
	bool _bDecodeNpcConfigFileContents(char* pData, DWORD dwMsgSize);
	void ReleaseItemHandler(int iClientH, short sItemIndex, bool bNotice);
	void ClientKilledHandler(int iClientH, int iAttackerH, char cAttackerType, short sDamage);
	int  SetItemCount(int iClientH, char* pItemName, DWORD dwCount);
	int  SetItemCount(int iClientH, int iItemIndex, DWORD dwCount);
	DWORD dwGetItemCount(int iClientH, char* pName);
	void DismissGuildRejectHandler(int iClientH, char* pName);
	void DismissGuildApproveHandler(int iClientH, char* pName);
	void JoinGuildRejectHandler(int iClientH, char* pName);
	void JoinGuildApproveHandler(int iClientH, char* pName);
	//void SendNotifyMsg(int iFromH, int iToH, WORD wMsgType, DWORD sV1, DWORD sV2, DWORD sV3, char* pString, DWORD sV4 = 0, DWORD sV5 = 0, DWORD sV6 = 0, DWORD sV7 = 0, DWORD sV8 = 0, DWORD sV9 = 0, char* pString2 = 0, DWORD sV10 = 0);
	void GiveItemHandler(int iClientH, short sItemIndex, int iAmount, short dX, short dY, WORD wObjectID, char* pItemName);
	void RequestPurchaseItemHandler(int iClientH, char* pItemName, int iNum);
	void ResponseDisbandGuildHandler(char* pData, DWORD dwMsgSize);
	void RequestDisbandGuildHandler(int iClientH, char* pData, DWORD dwMsgSize);
	void RequestCreateNewGuildHandler(int iClientH, char* pData, DWORD dwMsgSize);
	void ResponseCreateNewGuildHandler(char* pData, DWORD dwMsgSize);
	int  iClientMotion_Stop_Handler(int iClientH, short sX, short sY, char cDir);

	void RequestOnlines(int iClientH);

	void MultiplicadorExp(int Client, DWORD Exp);

	int  iClientMotion_GetItem_Handler(int iClientH, short sX, short sY, char cDir);
	void DropItemHandler(int iClientH, short sItemIndex, int iAmount, char* pItemName, bool bByPlayer = true);
	void ClientCommonHandler(int iClientH, char* pData);
	bool __fastcall bGetMsgQuene(char* pFrom, char* pData, DWORD* pMsgSize, int* pIndex, char* pKey);
	void MsgProcess();
	bool __fastcall bPutMsgQuene(char cFrom, char* pData, DWORD dwMsgSize, int iIndex, char cKey);
	void NpcBehavior_Flee(int iNpcH);
	int iGetDangerValue(int iNpcH, short dX, short dY);
	void NpcBehavior_Dead(int iNpcH);
	void NpcKilledHandler(short sAttackerH, char cAttackerType, int iNpcH, short sDamage);
	int iCalculateAttackEffect(short sTargetH, char cTargetType, short sAttackerH, char cAttackerType, int tdX, int tdY, int iAttackMode, bool bNearAttack = false, bool bIsDash = false, bool bArrowUse = false, bool bMainGaucheAttack = false);

	void RemoveFromTarget(short sTargetH, char cTargetType, int iCode = 0, int iAttackerH = 0, char cAttackerType = 0);

	void NpcBehavior_Attack(int iNpcH);
	void TargetSearch(int iNpcH, short* pTarget, char* pTargetType);
	void NpcBehavior_Move(int iNpcH);
	bool bGetEmptyPosition(short* pX, short* pY, char cMapIndex);
	char cGetNextMoveDir(short sX, short sY, short dstX, short dstY, char cMapIndex, char cTurn, int* pError);
	int  iClientMotion_Attack_Handler(int iClientH, short sX, short sY, short dX, short dY, short wType, char cDir, WORD wTargetObjectID, bool bResponse = true, bool bIsDash = false);
	void ChatMsgHandler(int iClientH, char* pData, DWORD dwMsgSize);
	void NpcProcess();
	int bCreateNewNpc(char* pNpcName, char* pName, char* pMapName, short sClass, char cSA, char cMoveType, int* poX, int* poY, char* pWaypointList, RECT* pArea, int iSpotMobIndex, char cChangeSide, bool bHideGenMode, bool bIsSummoned = false, bool bFirmBerserk = false, bool bIsMaster = false, int iGuildGUID = 0);


	bool _bGetIsStringIsNumber(char* pStr);
	bool bReadProgramConfigFile(char* cFn);

	void InitPlayerData(int iClientH, char* pData, DWORD dwSize);
	void ResponsePlayerDataHandler(char* pData, DWORD dwSize);
	bool bSendMsgToLS(DWORD dwMsg, int iClientH, bool bFlag = true, char* pData = 0);
	void OnMainLogRead();

	void CheckClientResponseTime();

	int iComposeMoveMapData(short sX, short sY, int iClientH, char cDir, char* pData);
	void SendEventToNearClient_TypeB(DWORD dwMsgID, WORD wMsgType, char cMapIndex, short sX, short sY, short sV1 = 0, short sV2 = 0, short sV3 = 0, short sV4 = 0);
	void SendEventToNearClient_TypeB(DWORD dwMsgID, WORD wMsgType, char cMapIndex, short sX, short sY, short sV1 = 0, short sV2 = 0, short sV3 = 0, DWORD sV4 = 0);
	void SendEventToNearClient_TypeA(short sOwnerH, char cOwnerType, DWORD dwMsgID, WORD wMsgType, short sV1, short sV2, short sV3);
	void DeleteClient(int iClientH, bool bSave, bool bNotify, bool bCountLogout = true, bool bForceCloseConn = false);
	int  iComposeInitMapData(short sX, short sY, int iClientH, char* pData);
	void RequestInitDataHandler(int iClientH, char* pData, char cKey, bool bIsResurrected);
	void RequestInitPlayerHandler(int iClientH, char* pData, char cKey);
	int iClientMotion_Move_Handler(int iClientH, short sX, short sY, char cDir, char cMoveType);
	void ClientMotionHandler(int iClientH, char* pData);

	void OnClientRead(int iClientH);



	// v1.4311-3 ÇÔ¼ö ¼±¾ð ÀÔÀå±Ç ¹Þ´Â ÇÔ¼ö .. 
	void GetFightzoneTicketHandler(int iClientH);
	// v1.4311-3 2 ½Ã°£¸¶´Ù »çÅõÀå ¿¹¾àÀ» ÃÊ±âÈ­ ½ÃÅ²´Ù.



	void RefreshPartyStatus(int iClientH);
	void RefreshPartyCoords(int iClientH);


	void AdminOrder_BanIP(int iClientH, char* pData, DWORD dwMsgSize);

	// New 06/05/2004
	// Upgrades
	bool bCheckIsItemUpgradeSuccess(int iClientH, int iItemIndex, int iSomH, bool bBonus = false);
	void RequestItemUpgradeHandler(int iClientH, int iItemIndex);

	void RequestItemEnchantHandler(int iClientH, int iItemIndex, int iAttributeType);

	//Party Codes
	void RequestCreatePartyHandler(int iClientH);
	void PartyOperationResultHandler(char* pData);
	void PartyOperationResult_Create(int iClientH, char* pName, int iResult, int iPartyID);
	void PartyOperationResult_Join(int iClientH, char* pName, int iResult, int iPartyID);
	void PartyOperationResult_Dismiss(int iClientH, char* pName, int iResult, int iPartyID);
	void PartyOperationResult_Delete(int iPartyID);
	void RequestJoinPartyHandler(int iClientH, char* pData, DWORD dwMsgSize);
	void RequestDismissPartyHandler(int iClientH);
	void GetPartyInfoHandler(int iClientH);
	void PartyOperationResult_Info(int iClientH, char* pName, int iTotal, char* pNameList);
	void RequestDeletePartyHandler(int iClientH);
	void RequestAcceptJoinPartyHandler(int iClientH, int iResult);
	void GetExp(int iClientH, DWORD iExp, bool bIsAttackerOwn = false);

	// New 07/05/2004
	// Guild Codes
	void RequestGuildNameHandler(int iClientH, int iObjectID, int iIndex);

	// Item Logs
	bool _bItemLog(int iAction, int iClientH, char* cName, class CItem* pItem);
	bool _bItemLog(int iAction, int iGiveH, int iRecvH, class CItem* pItem, bool bForceItemLog = false);
	bool _bCheckGoodItem(class CItem* pItem);

	bool bCheckAndConvertPlusWeaponItem(int iClientH, int iItemIndex);
	void ArmorLifeDecrement(int iAttackerH, int iTargetH, char cOwnerType, int iValue);

	// MultiDrops
	bool bGetMultipleItemNamesWhenDeleteNpc(short sNpcType, int iProbability, int iMin, int iMax, short sBaseX, short sBaseY,
		int iItemSpreadType, int iSpreadRange,
		int* iItemIDs, POINT* BasePos, int* iNumItem);

	// Player shutup
	void GSM_RequestShutupPlayer(char* pGMName, WORD wReqServerID, WORD wReqClientH, WORD wTime, char* pPlayer);

	// PK Logs
	bool _bPKLog(int iAction, int iAttackerH, int iVictumH, char* pNPC);

	bool _bDecodeItemConfigFileContents(char* pData, DWORD dwMsgSize);
	int _iComposePlayerDataFileContents(int iClientH, char* pData);
	bool _bDecodePlayerDatafileContents(int iClientH, char* pData, DWORD dwSize);
	bool _bRegisterMap(char* pName);



	int __iSearchForQuest(int iClientH, int iWho, int* pQuestType, int* pMode, int* pRewardType, int* pRewardAmount, int* pContribution, char* pTargetName, int* pTargetType, int* pTargetCount, int* pX, int* pY, int* pRange);
	int _iTalkToNpcResult_Cityhall(int iClientH, int* pQuestType, int* pMode, int* pRewardType, int* pRewardAmount, int* pContribution, char* pTargetName, int* pTargetType, int* pTargetCount, int* pX, int* pY, int* pRange);
	void _ClearExchangeStatus(int iToH);
	int _iGetItemSpaceLeft(int iClientH);

	void AdminOrder_GoTo(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_MonsterCount(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_SetForceRecallTime(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_UnsummonBoss(int iClientH);

	bool _bCrusadeLog(int iAction, int iClientH, int iData, char* cName);
	int iGetPlayerABSStatus(int iClientH);
	bool _bInitItemAttr(class CItem* pItem, int iItemID);
	void ReqCreateSlateHandler(int iClientH, char* pData);
	void SetSlateFlag(int iClientH, short sType, bool bFlag);
	void CheckForceRecallTime(int iClientH);
	void SetPlayingStatus(int iClientH);
	void ForceChangePlayMode(int iClientH);

	void RequestResurrectPlayer(int iClientH, bool bResurrect);

	void AdminOrder_SummonGuild(int iClientH);

	void GetDkSet(int iClientH);

	void AFKChecker();

	void RequestGuildMemberRank(int iClientH, char* pName, int iIndex);


	void iRecoverFollowers(int  iClientH, bool bControlAll);
	void PlayerCommandAddRank(int iClientH, char* pData, DWORD dwMsgSize, int iRank);
	void PlayerOrder_DeleteRank(int iClientH, char* pData, DWORD dwMsgSize);

	void ApocalypseStarter();


	void CheckSpecialEvent(int iClientH);

	DWORD m_iMaxAbs = 0, m_iMaxResist = 0, m_iMaxRec = 0, m_iMaxHPCrit = 0;
	int m_iFragileDropRate = 0;

	bool m_bIsGameServerRegistered;
	bool m_bIsSocketConnected[DEF_MAXSUBLOGSOCK];
	bool ReceivedAllConfig;

	bool m_iNotifyCleanMap;	
	DWORD m_dwCleanTime;

	char m_cServerName[12];

	int m_iBuildDate;

	char m_cLogServerAddr[16];
	char m_cGateServerAddr[16];

	int  m_iLogServerPort;
	int  m_iGateServerPort;
	int  m_iWorldServerPort;

	DWORD  m_iLimitedUserExp, m_iLevelExp20;

	class CNpc* m_pNpcList[DEF_MAXNPCS];
	
	class CNpcItem* m_pTempNpcItem[DEF_MAXNPCITEMS];
	class CDynamicObject* m_pDynamicObjectList[DEF_MAXDYNAMICOBJECTS];
	class CDelayEvent* m_pDelayEventList[DEF_MAXDELAYEVENTS];

	class CMsg* m_pMsgQuene[DEF_MSGQUENESIZE];
	int             m_iQueneHead, m_iQueneTail;
	int             m_iTotalMaps;
	class XSocket* m_pMainLogSock, * m_pGateSock;
	int				m_iGateSockConnRetryTimes;
	class CMisc     m_Misc;
	bool			m_bIsGameStarted;
	bool            m_bIsLogSockAvailable, m_bIsGateSockAvailable;
	bool			m_bIsItemAvailable, m_bIsBuildItemAvailable, m_bIsNpcAvailable, m_bIsMagicAvailable;
	bool			m_bIsSkillAvailable, m_bIsPotionAvailable, m_bIsQuestAvailable, m_bIsTeleportAvailable, m_bIsWLServerAvailable;
	class CItem* m_pItemConfigList[DEF_MAXITEMTYPES];
	class CNpc* m_pNpcConfigList[DEF_MAXNPCTYPES];
	class CMagic* m_pMagicConfigList[DEF_MAXMAGICTYPE];
	class CSkill* m_pSkillConfigList[DEF_MAXSKILLTYPE];
	class CQuest* m_pQuestConfigList[DEF_MAXQUESTTYPE];

	char            m_pMsgBuffer[DEF_MSGBUFFERSIZE + 1];

	HWND  m_hWnd;
	int   m_iTotalClients, m_iMaxClients, m_iTotalGameServerClients, m_iTotalGameServerMaxClients;
	int   m_iTotalBots, m_iMaxBots, m_iTotalGameServerBots, m_iTotalGameServerMaxBots;
	SYSTEMTIME m_MaxUserSysTime;

	bool  m_bF1pressed, m_bF4pressed, m_bF12pressed, m_bF5pressed;
	bool  m_bOnExitProcess;
	DWORD m_dwExitProcessTime;

	DWORD m_dwWhetherTime, m_dwGameTime1, m_dwGameTime2, m_dwGameTime3, m_dwGameTime4, m_dwGameTime5, m_dwGameTime6, m_dwFishTime;

	// Crusade Schedule
	bool m_bIsCrusadeWarStarter;
	bool m_bIsApocalypseStarter;
	int m_iLatestCrusadeDayOfWeek;

	// Centuu - scheduled events
	void DeathmatchStarter();
	void CaptureTheFlagStarter();
	void InvasionStarter();
	void CandyStarter();
	void BeholderStarter();
	void BagProtectionStarter();
	void TeamArenaStarter();
	void ShinningStarter();
	void HappyHourStarter();
	void FuryHourStarter();

	int  m_cDayOrNight;		// 1ÀÌ¸é ÁÖ°£, 2¸é ¾ß°£ 
	int   m_iSkillSSNpoint[102];

	class CMsg* m_pNoticeMsgList[DEF_MAXNOTIFYMSGS];
	int   m_iTotalNoticeMsg, m_iPrevSendNoticeMsg;
	DWORD m_dwNoticeTime, m_dwSpecialEventTime;
	bool  m_bIsSpecialEventTime;
	char  m_cSpecialEventType;
	//LifeX Auto Skills
    void AutoSkill(int iClientH);

	DWORD m_iLevelExpTable[DEF_MAXEXPTABLE];	//New 22/10/14 - centu: int -> unsigned long

	class CFish* m_pFish[DEF_MAXFISHS];

	class CPotion* m_pPotionConfigList[DEF_MAXPOTIONTYPES];
	class CPotion* m_pCraftingConfigList[DEF_MAXPOTIONTYPES];

	class CTeleportDest* m_pTeleportDestination[DEF_MAXTELEPORTDESTINATION];

	// 2002-12-6  Teleport  
	class CTeleport* m_pTeleportConfigList[DEF_MAXTELEPORTLIST];

	bool  m_bIsServerShutdowned;
	char  m_cShutDownCode;
	class CMineral* m_pMineral[DEF_MAXMINERALS];

	int   m_iMiddlelandMapIndex;	// ÀüÀïÅÍÀÎ ¹Ìµé·£µå ¸Ê ÀÎµ¦½º: ÀÌ °ªÀÌ -1ÀÌ¸é ¹Ìµé·£µå ¸ÊÀÌ ¾øÀ½À» ÀÇ¹Ì 
	int   m_iAresdenMapIndex;		// ¾Æ·¹½ºµ§ ¸Ê ÀÎµ¦½º 
	int	  m_iElvineMapIndex;		// ¿¤¹ÙÀÎ ¸Ê ÀÎµ¦½º
	int   m_iBTFieldMapIndex;
	int   m_iGodHMapIndex;
	int		m_iRampartMapIndex;
	int   m_iAresdenOccupyTiles;
	int   m_iElvineOccupyTiles;
	int   m_iCurMsgs, m_iMaxMsgs;

	// v1.4311-3 º¯¼ö ¼±¾ð  ¿¹¾àµÈ ½Ã°£À» °¡Áö´Â º¯¼ö 
	DWORD m_dwCanFightzoneReserveTime;

	// v1.4311-3 º¯¼ö ¼±¾ð °¢ »çÅõÀåÀÇ ¿¹¾à ¿©ºÎ¸¦ ÀúÀå 
	int  m_iFightZoneReserve[DEF_MAXFIGHTZONE];
	// v1.4311-3 º¯¼ö ¼±¾ð »çÅõÀå¿¡ µé¾î°¬À»¶§ ¿î¿µÀÚ ¸í·É¾î·Î °­ÄÝ ¾ÊµÇ°Ô ÇÔ.
	int  m_iFightzoneNoForceRecall;

	struct {
		int markX, markY;
		char cGuildName[21], cMap[11];
		DWORD dwMarkTime;
	}m_stGuildInfo[DEF_MAXGUILDS];

	int ObtenerX(char* pGuild);
	int ObtenerY(char* pGuild);
	int ObtenerID(char* pGuild);

	struct {
		int sX, sY;
		char cGuildName[21], cMap[11];
	}m_stSummonGuild[DEF_MAXGUILDS];

	int	  m_iStrategicStatus;

	class XSocket* m_pSubLogSock[DEF_MAXSUBLOGSOCK];
	int   m_iSubLogSockInitIndex;
	bool  m_bIsSubLogSockAvailable[DEF_MAXSUBLOGSOCK];
	int	  m_iCurSubLogSockIndex;
	int   m_iSubLogSockFailCount;
	int   m_iSubLogSockActiveCount;	// v1.5 ÇöÀç ¿¬°á »óÅÂ°¡ À¯ÁöµÇ°í ÀÖ´Â sub-log-socket °¹¼ö
	int   m_iAutoRebootingCount;	// v1.5 ÀÚµ¿ ÀçºÎÆÃ µÈ È½¼ö 

	class CBuildItem* m_pBuildItemList[DEF_MAXBUILDITEMS];
	class CItem* m_pDupItemIDList[DEF_MAXDUPITEMID];

	char* m_pNoticementData;
	DWORD  m_dwNoticementDataSize;

	DWORD  m_dwMapSectorInfoTime;
	int    m_iMapSectorInfoUpdateCount;

	// Crusade Ã³¸®¿ë
	int	   m_iCrusadeCount;			// 0ÀÌ¸é Àü¸éÀü ´ë±â»óÅÂ. 1ÀÌ¸é Ä«¿îÆ® ´Ù¿î ½ÃÀÛ. 
	bool   m_bIsCrusadeMode;		// Å©·ç¼¼ÀÌµå ¸ðµå
	bool   m_bIsApocalypseMode;
	// Daryl - Chat logging option
	char m_bLogChatOption;

	struct {
		char cMapName[11];			// ¼³Ä¡µÇ´Â ¸Ê À§Ä¡
		char cType;					// °ÇÃà¹° Á¾·ù. »ç½Ç NPC Á¾·ùÀÓ.
		int  dX, dY;				// ¼³Ä¡µÇ´Â ¸Ê»óÀÇ À§Ä¡

	} m_stCrusadeStructures[DEF_MAXCRUSADESTRUCTURES];

	int m_iCollectedMana[3];
	int m_iAresdenMana, m_iElvineMana;

	class CTeleportLoc m_pGuildTeleportLoc[DEF_MAXGUILDS];
	//

	WORD  m_wServerID_GSS;
	char  m_cGateServerStockMsg[DEF_MAXGATESERVERSTOCKMSGSIZE];
	int   m_iIndexGSS;
	char m_cApocalypseMapName[11];
	char m_cHeldenianMapName[11];
	int m_iLastCrusadeWinner; 	// New 13/05/2004
	struct {
		int iCrashedStructureNum;
		int iStructureDamageAmount;
		int iCasualties;
	} m_stMeteorStrikeResult;

	// ¹Ìµé·£µåÀÇ Á¤º¸¸¦ ÀúÀåÇÏ´Â ±¸Á¶Ã¼. ¹Ìµé·£µå¸¦ ¿î¿ëÇÏÁö ¾Ê´Â ¼­¹ö¿¡¼­´Â ÀÌ Á¤º¸¸¦ ¹Þ¾Æ °»½ÅÇÑ´Ù.
	struct {
		char cType;			// ÀÌ°Ô NULLÀÌ¸é Á¤ÀÇµÇÁö ¾ÊÀº°ÍÀ» ÀÇ¹Ì.
		char cSide;			// »çÀÌµå
		short sX, sY;		// ¼³Ä¡µÈ À§Ä¡ 
	} m_stMiddleCrusadeStructureInfo[DEF_MAXCRUSADESTRUCTURES];

	struct {
		char m_cBannedIPaddress[21];		 // Å¬¶óÀÌ¾ðÆ®ÀÇ IP address
	} m_stBannedList[DEF_MAXBANNED];

	struct {
		char m_cGMName[11];
	} m_stAdminList[DEF_MAXADMINS];

	// Crusade Scheduler
	struct {
		int iDay;
		int iHour;
		int iMinute;
	} m_stCrusadeWarSchedule[DEF_MAXSCHEDULE];

	struct {
		int iDay;
		int iHour;
		int iMinute;
	} m_stApocalypseScheduleStart[DEF_MAXSCHEDULE];

	struct {
		int iDay;
		int StartiHour;
		int StartiMinute;
	} m_stHeldenianSchedule[DEF_MAXSCHEDULE];

	//Magn0S:: Added Schedules for events
	struct {
		int iDay;
		int iHour;
		int iMinute;
	} m_stGladArenaSchedule[DEF_MAXSCHEDULE];

	struct {
		int iDay;
		int iHour;
		int iMinute;
	} m_stCTFSchedule[DEF_MAXSCHEDULE];

	struct {
		int iDay;
		int iHour;
		int iMinute;
	} m_stInvasionSchedule[DEF_MAXSCHEDULE];

	struct {
		int iDay;
		int iHour;
		int iMinute;
	} m_stCandyFurySchedule[DEF_MAXSCHEDULE];

	struct {
		int iDay;
		int iHour;
		int iMinute;
	} m_stBeholderSchedule[DEF_MAXSCHEDULE];

	struct {
		int iDay;
		int iHour;
		int iMinute;
	} m_stBagProtectSchedule[DEF_MAXSCHEDULE];

	struct {
		int iDay;
		int iHour;
		int iMinute;
	} m_stTeamArenaSchedule[DEF_MAXSCHEDULE];

	struct {
		int iDay;
		int iHour;
		int iMinute;
	} m_stShinningSchedule[DEF_MAXSCHEDULE];

	struct {
		int iDay;
		int iHour;
		int iMinute;
	} m_stHappyHourSchedule[DEF_MAXSCHEDULE];

	struct {
		int iDay;
		int iHour;
		int iMinute;
	} m_stFuryHourSchedule[DEF_MAXSCHEDULE];

	int m_iTotalMiddleCrusadeStructures;

	// ÀÌ º¯¼ö ¾È¿¡´Â ÇöÀç Á¢¼ÓÇÑ Å¬¶óÀÌ¾ðÆ®ÀÇ ÀÎµ¦½º°¡ °ø¹é¾øÀÌ ÀúÀåµÇ¾î ÀÖ´Ù. 0ÀÌ ³ª¿À¸é ¸®½ºÆ®ÀÇ ³¡ÀÌ¶ó´Â ÀÇ¹Ì
	int m_iClientShortCut[DEF_MAXCLIENTS + 1];

	int m_iNpcConstructionPoint[DEF_MAXNPCTYPES];
	DWORD m_dwCrusadeGUID;
	int m_sLastCrusadeDate;
	int   m_iCrusadeWinnerSide;
	int   m_iPlayerMaxLevel;

	struct {
		int iTotalMembers;
		int iIndex[9];
	}m_stPartyInfo[DEF_MAXCLIENTS];

	// Daryl - Admin level adjustments
	int m_iAdminLevelWho;
	int m_iAdminLevelGMKill;
	int m_iAdminLevelGMRevive;
	int m_iAdminLevelGMCloseconn;
	int m_iAdminLevelGMCheckRep;
	int m_iAdminLevelEnergySphere;
	int m_iAdminLevelShutdown;
	int m_iAdminLevelObserver;
	int m_iAdminLevelShutup;
	int m_iAdminLevelCallGaurd;
	int m_iAdminLevelSummonDemon;
	int m_iAdminLevelSummonDeath;
	int m_iAdminLevelReserveFightzone;
	int m_iAdminLevelCreateFish;
	int m_iAdminLevelTeleport;
	int m_iAdminLevelCheckIP;
	int m_iAdminLevelPolymorph;
	int m_iAdminLevelSetInvis;
	int m_iAdminLevelSetZerk;
	int m_iAdminLevelSetIce;
	int m_iAdminLevelGetNpcStatus;
	int m_iAdminLevelSetAttackMode;
	int m_iAdminLevelUnsummonAll;
	int m_iAdminLevelUnsummonDemon;
	int m_iAdminLevelSummon;
	int m_iAdminLevelSummonAll;
	int m_iAdminLevelSummonPlayer;
	int m_iAdminLevelDisconnectAll;
	int m_iAdminLevelEnableCreateItem;
	int m_iAdminLevelCreateItem;
	int m_iAdminLevelStorm;
	int m_iAdminLevelWeather;
	int m_iAdminLevelSetStatus;
	int m_iAdminLevelGoto;
	int m_iAdminLevelMonsterCount;
	int m_iAdminLevelSetRecallTime;
	int m_iAdminLevelUnsummonBoss;
	int m_iAdminLevelClearNpc;
	int m_iAdminLevelTime;
	int m_iAdminLevelPushPlayer;
	int m_iAdminLevelSummonGuild;
	int m_iAdminLevelCheckStatus;
	int m_iAdminLevelCleanMap;

	// 09/26/2004
	short m_sSlateSuccessRate;

	// 17/05/2004
	short m_sForceRecallTime;

	// 22/05/2004
	int	 m_iPrimaryDropRate, m_iSecondaryDropRate;
	int  m_iRareDropRate;

	// 25/05/2004
	int m_iFinalShutdownCount;

	// New 06/07/2004
	bool m_bEnemyKillMode;
	int m_iEnemyKillAdjust, m_iRatingAdjust;
	bool m_bAdminSecurity;

	bool bShinning = false;
	void ManageShinning();
	void minimap_clear(int client);
	void minimap_update(int client);
	
	// Configurable Raid Time 
	short m_sRaidTimeMonday; 
	short m_sRaidTimeTuesday; 
	short m_sRaidTimeWednesday; 
	short m_sRaidTimeThursday; 
	short m_sRaidTimeFriday; 
	short m_sRaidTimeSaturday; 
	short m_sRaidTimeSunday; 

	bool m_bManualTime;
	int m_iSummonGuildCost;
	
	// Apocalypse
	bool	m_bIsHeldenianTeleport;

	DWORD m_dwApocalypseGUID;
	
	// Slate exploit
	int m_sCharPointLimit;

	int m_iDKCost;

	bool m_bIsCTFEvent = false;

	// Limit Checks
	short m_sCharStatLimit;
	bool m_bAllow100AllSkill;
	short m_sCharSkillLimit;
	char m_cRepDropModifier;
	char  m_cSecurityNumber[16];
	short m_sMaxPlayerLevel;
	
	bool var_89C, var_8A0;
	bool m_bReceivedItemList;

	void minimap_update_mark(int client);
	void minimap_clear_mark(int client);
	void DeleteMark();

	// MORLA 2.2 - Deathmach Game
	int iDGtop1,iDGtop2,iDGtop3,iDGtop4,iDGtop5,iDGtop6,iDGtop7,iDGtop8,iDGtop9,iDGtop10;
	int iLastKill;
	int iTotalKills;
	bool bDeathmatch;
	DWORD dwTimeLastKill;

	// 	Heldenian
	bool	m_bIsHeldenianMode;			// Event has started	-> can TP on maps
	bool	m_bHeldenianWarInitiated;	// War has begun		-> can Fight 
	short	m_cHeldenianType;			// Type 1: (BtField) 
										// Type 2: (Casttle siege) Previous winner will defend.
	short    m_sLastHeldenianWinner;		// Last war result, allows to TP on BtField out off war time
										// Tells defender for next Type 2 Heldenian
	short	m_cHeldenianWinner;			// If victory before Event timeOut, tell who wins
	DWORD	m_dwHeldenianGUID;			// identify this event
	DWORD	m_dwHeldenianWarStartTime;	// 15min delay to start the real war
	DWORD	m_dwHeldenianFinishTime;	// 1h30 after, finish the event and compute result...	
	short	m_cHeldenianFirstDestroyedTowerWinner;
	int m_iHeldenianAresdenDead, m_iHeldenianElvineDead;
	int m_iHeldenianAresdenFlags, m_iHeldenianElvineFlags;
	int m_iHeldenianAresdenLeftTower, m_iHeldenianElvineLeftTower;

	DWORD dwCrusadeFinishTime;
	void CrusadeEnd();

	DWORD dwEventFinishTime;
	void EventEnd();

	short m_sHeldenianFinish, m_sCrusadeFinish, m_sApocalypseFinish, m_sDeathmatchFinish, m_sCTF_Finish,
		m_sDropFinish, m_sCityTeleportFinish, m_sShinningFinish, m_sCandyFinish, m_sTeamArenaFinish,
		m_sRevelationFinish;

	void CityTeleport();

	DWORD m_dwHeldenianWarTime;
	DWORD m_dwHeldenianEndTime;
	char  m_cHeldenianSummonMode;

	DWORD m_dwApocalypseGateOpenTime;
	DWORD m_dwApocalypseGateCloseTime;

	// new March 13, 2005
	bool m_bIsApocalypseGateOpen;
	bool m_bHeldinianDuty;
	bool m_bHeldenianDutyMultiplyer;
	bool m_bIsHeldenianReady;
	bool m_bF10pressed;
	bool m_bIsApocalypseScheduleLoaded;
	bool m_bIsApocalypseSchedule;
	bool m_bIsHeldenianScheduleLoaded;
	bool m_bIsHeldenianSchedule;
	DWORD m_dwApocalypseStartHour;
	DWORD m_dwApocalypseStartMinute;

	// centu - settings
	int m_iClearMapTime, m_iGoldRate, m_iGuildCost;
	int m_iExpSetting;

	bool  m_bNpcItemConfig;

	//LifeX Fix Revive 14/10/2019
	void AdminOrder_Revive(int iClientH, char * pData, DWORD dwMsgSize);

	//Top ek
	int  aHBTopKills[16];
	char  aHBTopClientH[16][11];
	bool bReadTopPlayersFile(char* cFn);
	void _CreateTopPlayers();
	void ordenarTop15HB(int iClientH);
	
	void PlayerCommandCheckAdmins(int iClientH);

	//Magn0S:: Teleport List
	bool bDecodeTeleportList(char* pFn);
	void RequestTeleportListHandler(int iClientH, char* pData, DWORD dwMsgSize);
	void RequestChargedTeleportHandler(int iClientH, char* pData, DWORD dwMsgSize);

	//Magn0S:: Quest List
	void RequestQuestList(int iClientH, char* pData, DWORD dwMsgSize);

	void ReceivedClientOrder(int iClientH, int iOption1, int iOption2, int iOption3, char* cName);
	void AdminOrder_AddGM(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_SetRep(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_SetMAJ(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_SetContrib(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_SetEk(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_SetCoin(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_SetAdminLvl(int iClientH, char* pData, DWORD dwMsgSize);

	//Magn0S:: Multi Quest
	void RequestQuestPrize(int iClientH, int iQuest);
	void NotifyMapRestrictions(int iClientH, bool bNotify);

	//Magn0S:: Fragiles
	void CheckDestroyFragileItem(int iClientH);
	void AdminOrder_CreateFragileItem(int iClientH, char* pData, DWORD dwMsgSize);

	void LearnAllMagics(int iClientH);

	void NotifyPlayerAttributes(int iClientH);

	//Magn0S:: Drop Manager
	bool bDecodeDropManagerFile(char* cFn);
	int m_iMaxArmorDrop, m_iMaxWeaponDrop, m_iMaxAttrWeaponDrop, m_iMaxStatedArmor, m_iMaxStatedWeapon;
	int m_iArmorDrop[13];
	int m_iWeaponDrop[13];
	int m_iAttrWeaponDrop[9];
	int m_iStatedArmorDrop[8];
	int m_iStatedWeaponDrop[4];

	int m_iServerPhyDmg, m_iServerMagDmg;

	bool m_bNullDrop[10];
	//Magn0S End--------------------------------------------------------------------------------------------------------
};

#endif // !defined(AFX_GAME_H__C3D29FC5_755B_11D2_A8E6_00001C7030A6__INCLUDED_)
