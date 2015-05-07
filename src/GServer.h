#pragma once


#include "Server.h"
#include "Gate.h"
#include "Party.h"
#include "streams.h"
#include "DelayEvent.h"

#pragma region wtfdefines

#define MAXCLIENTS			10000
#define MAXNPCS				15000
#define MAXMAPS				120
#define MAXBUILDITEMS		300
#define CLIENTTIMEOUT		18000
#define AUTOSAVETIME		3*60000
#define HPUPTIME			8000
#define MPUPTIME			15000 // Changed 9->15 Sec
#define SPUPTIME			10000

#define HUNGERTIME			1*60000
#define POISONTIME			12000
#define SUMMONTIME			5*60000
#define NOTICETIME			80000
#define PLANTTIME			5*60000

#define EXPSTOCKTIME		10000
#define AUTOEXPTIME			6*60000



#define TOTALLEVELUPPOINT	7 // Level Stat Bonus xRisenx

#define MAXDYNAMICOBJECTS	60000
#define MAXDELAYEVENTS		60000

#define SSN_LIMIT_MULTIPLY_VALUE	2

#define MAXNOTIFYMSGS		1000
#define NIGHTTIME			40

#define CHARPOINTLIMIT		700 // Max Stats xRisenx
#define RAGPROTECTIONTIME	7000
#define MAXREWARDGOLD		99999999

#define MAXSUPERATTACK		10000 // Max Critical Limit xRisenx
#define MAXLUCKY			5000 // Max Lucky Limit xRisenx
#define MAXREP				10000 // Max Rep Limit xRisenx
#define MAXNEGAREP			-10000 // Max Negative Rep Limit xRisenx

#define ATTACKAI_NORMAL				1
#define ATTACKAI_EXCHANGEATTACK		2
#define ATTACKAI_TWOBYONEATTACK		3

#define MAXFISHS					200
#define MAXMINERALS					200
#define MAXENGAGINGFISH				30
#define MAXPOTIONTYPES				500
#define MAXCRAFTING					120

#define MOBEVENTTIME				5*60000
#define MAXQUESTTYPE				200

#define MAXSUBLOGSOCK				5

#define ITEMLOG_GIVE				1
#define ITEMLOG_DROP				2
#define ITEMLOG_GET					3
#define ITEMLOG_DEPLETE				4
#define ITEMLOG_NEWGENDROP			5
#define ITEMLOG_DUPITEMID			6
#define ITEMLOG_BUY					7
#define ITEMLOG_SELL				8
#define ITEMLOG_RETRIEVE			9
#define ITEMLOG_DEPOSIT				10
#define ITEMLOG_EXCHANGE			11
#define ITEMLOG_MAGICLEARN			12
#define ITEMLOG_MAKE				13
#define ITEMLOG_SUMMONMONSTER		14
#define ITEMLOG_POISONED			15
#define ITEMLOG_SKILLLEARN			16
#define ITEMLOG_REPAIR				17
#define ITEMLOG_JOINGUILD           18
#define ITEMLOG_BANGUILD            19
#define ITEMLOG_RESERVEFIGZONE      20	//  "
#define ITEMLOG_APPLY               21	//  "
#define ITEMLOG_SHUTUP              22	//  "
#define ITEMLOG_CLOSECONN			23	//  "
#define ITEMLOG_SPELLFIELD			24	//  "
#define ITEMLOG_CREATEGUILD			25	//  "
#define ITEMLOG_GUILDDISMISS		26	//  "
#define ITEMLOG_SUMMONPLAYER        27	//  "
#define ITEMLOG_CREATE				28	//  "
#define ITEMLOG_UPGRADEFAIL         29
#define ITEMLOG_UPGRADESUCCESS      30
#define ITEMLOG_MAILRETRIEVE			31
#define ITEMLOG_MAILSEND				32
#define ITEMLOG_GWHDEPOSIT				33
#define ITEMLOG_GWHRETRIEVE			34


#define ITEMSPREAD_RANDOM			1
#define ITEMSPREAD_FIXED			2
#define MAX_NPCITEMDROP				95


#define CRUSADELOG_ENDCRUSADE       1
#define CRUSADELOG_STARTCRUSADE     2
#define CRUSADELOG_SELECTDUTY       3
#define CRUSADELOG_GETEXP           4
#define CRUSADELOG_APOCALYPSE		5

#define PKLOG_BYPLAYER				1
#define PKLOG_BYPK					2
#define PKLOG_BYENERMY				3
#define PKLOG_BYNPC					4
#define PKLOG_BYOTHER				5
#define PKLOG_REDUCECRIMINAL        6


#define MAXDUPITEMID				100

#define MAXGUILDS					1000
#define MAXONESERVERUSERS			800
#define MAXGATESERVERSTOCKMSGSIZE	30000


#define MAXCONSTRUCTNUM			20 //raised from 10 to 20 to in corporate 4mana stones

#define MAXSCHEDULE				15 // Raised from 10 to 15



#define MAXFIGHTZONE 10

//============================
// #define LEVELLIMIT		130
#define LEVELLIMIT		50
//============================

//============================
#define MAGICHITRATIO 85.5f // xRisenx
//#define MAGICHITRATIO 204.5f // xRisenx
//============================

//============================
#define MINIMUMHITRATIO 15
//============================

//============================
#define MAXIMUMHITRATIO	99
//============================

//============================
#define PLAYERMAXLEVEL	205
//============================

//============================
#define GMGMANACONSUMEUNIT	15
//============================

#define MAXCONSTRUCTIONPOINT 30000

#define MAXWARCONTRIBUTION	 500000

#define MAXPARTYNUM			200
//#define MAXARENAPLAYERS     200 // Gladiator Arena xRisenx

#define ALLSIDES			100
#define ATTACKER			101
#define DEFENDER			102

//v2.19 2002-11-19
#define PK					0
#define NONPK				1
#define NEVERNONPK			2

#define MAX_CRUSADESUMMONMOB	100

#define CRUSADELOG_ENDHELDENIAN		11
#define CRUSADELOG_STARTHELDENIAN	12
#define CRUSADELOG_GETHELDENIANEXP	14
#define CRUSADELOG_HELDENIANVICTORY	15
#define CRUSADELOG_HELDENIANWARNOW	16

#define MAX_HELDENIANTOWER			200
#define MAXHELDENIAN				10
#define MAXHELDENIANSUMMONPOINT		12000
#ifdef RestatScroll
#define STR 0x01 //Correct
#define DEX 0x02 //Correct
#define INT	0x03 //Correct
#define VIT 0x04 //Correct
#define MAG	0x05 //Correct
#define CHR 0x06 //Correct
#endif

#define DEF_SOCKETTHREADS 2
#pragma endregion

class Map;

class GServer : public Server
{
public:
	GServer(string servername, string config);
	~GServer(void);

	//temporary solution
	void PutLogFileList(char * str);
	void PutLogList(char * str);

	// Initialize Server
	bool Init();

	void run();
	void handle_stop();

	std::thread * chatthread;
	std::thread * actionthread;
	std::thread * runthread;

	Gate * gate;

	void TimerThread();
	void ChatThread();
	void SocketThread();
	void ActionThread();

	void DeleteClient(shared_ptr<Client> client, bool save = true, bool deleteobj = false);

	bool LoadCharacterData(shared_ptr<Client> client);
	void InitPlayerData(shared_ptr<Client> client);
	void ClientCommonHandler(shared_ptr<Client> client, shared_ptr<MsgQueueEntry> msg);
	void ClientMotionHandler(shared_ptr<Client> client, shared_ptr<MsgQueueEntry> msg);
	int iClientMotion_Stop_Handler(shared_ptr<Client> client, uint16_t sX, uint16_t sY, uint8_t cDir);
	int iClientMotion_GetItem_Handler(shared_ptr<Client> client, uint16_t sX, uint16_t sY, uint8_t cDir);
	int iClientMotion_Attack_Handler(shared_ptr<Client> client, uint16_t sX, uint16_t sY, uint16_t dX, uint16_t dY, int16_t wType, uint8_t cDir, uint64_t wTargetObjectID, bool bResponse = true, bool bIsDash = false);
	int iClientMotion_Move_Handler(shared_ptr<Client> client, uint16_t sX, uint16_t sY, uint8_t cDir, bool bIsRun);
	int iClientMotion_Magic_Handler(shared_ptr<Client> client, uint16_t sX, uint16_t sY, uint8_t cDir);
	void SendEventToNearClient_TypeA(Unit * owner, uint32_t msgid, uint32_t sV1, uint32_t sV2, uint32_t sV3);
	//void SendEventToNearClient_TypeA(shared_ptr<Npc> npc, uint16_t msgid, int16_t sV1, int16_t sV2, int16_t sV3);
	void SendEventToNearClient_TypeB(uint32_t msgid, uint16_t msgtype, Map * mapIndex, uint16_t sX, uint16_t sY, uint32_t sV1 = 0, uint32_t sV2 = 0, uint32_t sV3 = 0, uint32_t sV4 = 0);
	void SendObjectMotionRejectMsg(Client * client);
	bool _bAddClientItemList(shared_ptr<Client> client, Item * pItem, int * pDelReq);
	void RequestFullObjectData(shared_ptr<Client> client, Unit * target);
	bool _bGetIsPlayerHostile(Client * player, Client * target);
	bool WriteTileData(StreamWrite & sw, Client * player, Tile * srcTile, uint16_t ix, uint16_t iy);
	void AddGroundItem(Item * pItem, uint16_t x, uint16_t y, Map * mapIndex, uint32_t dwTime);
	Map * GetMap(string name);
	uint8_t iSetSide(Client * client);
	void RequestHuntmode(Client * client);
	void SendNotifyMsg(Client * from, Client * to, uint16_t wMsgType, uint64_t sV1 = 0, uint64_t sV2 = 0, uint64_t sV3 = 0, string pString = "", 
		uint64_t sV4 = 0, uint64_t sV5 = 0, uint64_t sV6 = 0, uint64_t sV7 = 0, uint64_t sV8 = 0, 
		uint64_t sV9 = 0, string pString2 = "");
	uint64_t iGetLevelExp(uint16_t iLevel);
	Item * _iGetArrowItemIndex(Client * client);
	int _iCalcMaxLoad(shared_ptr<Client> client);
	int iCalcTotalWeight(shared_ptr<Client> client);
	int SetItemCount(shared_ptr<Client> client, string pItemName, uint32_t dwCount);
	int SetItemCount(shared_ptr<Client> client, int32_t iItemID, uint32_t dwCount);
	void WriteItemData(StreamWrite & sw, Item * pItem) const;
	void SendItemNotifyMsg(shared_ptr<Client> client, uint16_t msgtype, Item *pItem, int iV1);
	void DropItemHandler(shared_ptr<Client> client, short sItemIndex, int iAmount, string pItemName, bool bByPlayer);
	void ItemDepleteHandler(shared_ptr<Client> client, Item * pItem, bool bIsUseItemResult, bool bIsLog = true, bool notify = true);
	bool bGetEmptyPosition(short * pX, short * pY, shared_ptr<Unit> client);
	void RequestTeleportHandler(Client * client, char teleportType, string cMapName = "", int dX = -1, int dY = -1);
	int iGetMapLocationSide(string MapName);
	
	void PlayerMapEntry(shared_ptr<Client> client, bool setRecallTime);
	void ToggleCombatModeHandler(shared_ptr<Client> client);
	shared_ptr<Npc> CreateNpc(string & pNpcName, Map * mapIndex, char cSA, char cMoveType, uint16_t * poX, uint16_t * poY, Side changeSide, char * pWaypointList, rect * pArea, int iSpotMobIndex, bool bHideGenMode = false, bool bIsSummoned = false, bool bFirmBerserk = false, bool bIsMaster = false, int iGuildGUID = 0);
	void RequestTitleHandler(Client * client, StreamRead & sr);
	bool bCheckClientAttackFrequency(Client * client);
	bool bCheckClientMagicFrequency(Client * client);
	bool bCheckClientMoveFrequency(Client * client, bool running);
	void _CheckAttackType(Client * client, int16_t & spType);
	void ClearSkillUsingStatus(Client * client);

	void NpcProcess();

	void RemoveFromTarget(shared_ptr<Unit> target, int iCode = 0);
	void NpcKilledHandler(shared_ptr<Unit> attacker, shared_ptr<Npc> npc, int16_t damage);
	void NpcBehavior_Flee(shared_ptr<Npc> npc);
	void NpcBehavior_Dead(shared_ptr<Npc> npc);

	void DeleteNpc(shared_ptr<Npc> NpcH);
	
	char cGetNextMoveDir(short sX, short sY, short dstX, short dstY, Map * map, char cTurn, int * pError);
	char cGetNextMoveDir(short sX, short sY, short dstX, short dstY, Map * map, char cTurn, int * pError, short * DOType);


	int getPlayerNum(Map * pMap, short dX, short dY, char cRadius);
	bool CheckResistingMagicSuccess(char cAttackerDir, Unit * target, int iHitRatio);
	bool CheckResistingPoisonSuccess(Unit * owner);
	bool CheckResistingIceSuccess(char cAttackerDir, Unit * target, int iHitRatio);
	void CalculateSSN_SkillIndex(Client * client, short sSkillIndex, int iValue);
	void CalculateSSN_ItemIndex(Client * client, Item * Weapon, int iValue);
	int CalculateAttackEffect(Unit * target, Unit * attacker, int tdX, int tdY, int iAttackMode, bool bNearAttack = false, bool bIsDash = false);
	void EnduStrippingDamage(Unit * target, Unit * attacker, int item, int higherStripEndu, bool shield = false);

	int _iCalcSkillSSNpoint(int iLevel);

	void RemoveFromDelayEventList(Unit * unit, int32_t iEffectType);
	bool RegisterDelayEvent(int iDelayType, int iEffectType, uint32_t dwLastTime, Unit * unit, Map * pmap, int dX, int dY, int iV1, int iV2, int iV3);

	void _CheckMiningAction(Client * client, int dX, int dY);
	void GetExp(Client * client, uint64_t iExp, bool bIsAttackerOwn);

	int iAddDynamicObjectList(Unit * owner, short sType, Map * pmap, short sX, short sY, uint32_t dwLastTime, int iV1);
	void CheckDynamicObjectList();
	int _iGetWeaponSkillType(Client * client);
	int16_t iGetComboAttackBonus(int16_t iSkill, int16_t iComboCount);

	bool bAnalyzeCriminalAction(Client * client, short dX, short dY, bool bIsCheck = false);

	uint64_t npchandle;

	shared_ptr<Client> GetClient(uint64_t ObjectID);
	shared_ptr<Npc> GetNpc(uint64_t ObjectID);

	void ParseChat(Client * client, string message);

	MsgQueue chatpipe;
	MsgQueue actionpipe;
	std::mutex mutchat;
	std::mutex mutaction;
	shared_mutex mutnpclist;

	//TODO: guilds - load all the guilds from the db and keep them in memory
	//Give guilds more purpose than just another chat channel in game


	std::list<shared_ptr<Npc>> npclist;

	struct
	{
		Item * item;
		time_t dropTime;
		uint16_t sx, sy;
		Map * cMapIndex;
		bool bEmpty;
	} m_stGroundNpcItem[MAXGROUNDITEMS];

	string servername;
	uint64_t  m_iLimitedUserExp, m_iLevelExp51;

	std::unordered_map<string, uint8_t> m_mapNameList;

	int             m_iTotalMaps;
	bool			m_dropsInitiated;
	bool			m_bIsGameStarted;
	Item   * m_pItemConfigList[MAXITEMTYPES];
	Npc    * m_npcConfigList[MAXNPCTYPES];
	Magic  * m_pMagicConfigList[MAXMAGICTYPE];
	Skill  * m_pSkillConfigList[MAXSKILLTYPE];
	Quest  * m_pQuestConfigList[MAXQUESTTYPE];
	char            m_pMsgBuffer[2048];
	Crafting * m_pCraftingConfigList[MAXCRAFTING];

	Teleport * m_pTeleportConfigList[MAXTELEPORTLIST];

	int   m_iTotalClients, m_iMaxClients, m_iTotalGameServerClients, m_iTotalGameServerMaxClients;

	bool  m_bOnExitProcess;
	uint64_t m_dwExitProcessTime;

	uint64_t m_dwWeatherTime, m_dwGameTime1, m_dwGameTime2, m_dwGameTime3, m_dwGameTime4, m_dwGameTime5, m_dwGameTime6, m_dwFishTime;

	char  m_cDayOrNight;
 	int   m_iSkillSSNpoint[102];

	int   m_iTotalNoticeMsg, m_iPrevSendNoticeMsg;
	uint64_t m_dwNoticeTime, m_dwSpecialEventTime, m_startTime;
	bool  m_bIsSpecialEventTime;
	char  m_cSpecialEventType;

	uint64_t   m_iLevelExpTable[300];
 	Fish * m_pFish[MAXFISHS];
	Potion * m_pPortionConfigList[MAXPOTIONTYPES];

	bool  m_bIsServerShutdowned;
	char  m_cShutDownCode;
	Mineral * m_pMineral[MAXMINERALS];

	int m_iMiddlelandMapIndex;
	int m_iAresdenMapIndex;
	int m_iElvineMapIndex;
	Side m_astoriaBasePos[3];
	Side m_eventWinner[ET_MAX];
	Ini * m_eventsIni;

	bool	m_bIsApocalypseMode;
	bool	m_bIsApocalypseGateOpen;

	bool	m_bHeldenianMode;
	uint32_t	m_dwHeldenianGUID;
	int	m_iHeldenianType1Winner, m_iHeldenianType2Winner;
	int	m_iHeldenianType;
	int	m_iLastHeldenianType;
	int m_iHeldenianAresdenDead, m_iHeldenianElvineDead, m_iHeldenianAresdenKill, m_iHeldenianElvineKill, m_iHeldenianAresdenFlags, m_iHeldenianElvineFlags,
		m_iHeldenianAresdenLeftTower, m_iHeldenianElvineLeftTower;
	int	m_iBtFieldMapIndex;
	int	m_iRampartMapIndex;
	int	m_iGodHMapIndex;

	int	 m_iMaxGMGMana;
	int m_iAresdenOccupyTiles;
	int m_iElvineOccupyTiles;
	int m_iCurMsgs, m_iMaxMsgs;

	std::list<shared_ptr<DelayEvent>> DelayEventList;
	std::mutex delayMutex;


	int iDMGCount; // New monster event xRisenx


	uint64_t m_dwCanFightzoneReserveTime ;

	int  m_iFightZoneReserve[MAXFIGHTZONE] ;
	uint64_t m_donateEventHolder;
	uint32_t m_donateEventPot;

	struct {
		uint64_t iFunds;
		uint64_t iCrimes;
		uint64_t iWins;
	} m_stCityStatus[MAXSIDES];

	int	  m_iStrategicStatus;

	BuildItem * m_pBuildItemList[MAXBUILDITEMS];
	Item * m_pDupItemIDList[MAXDUPITEMID];

	char * m_pNoticementData;
	uint32_t  m_dwNoticementDataSize;

	uint32_t  m_dwMapSectorInfoTime;
	int    m_iMapSectorInfoUpdateCount;



	bool   m_bIsCrusadeMode;
	struct {
		string cMapName;
		char cType;
		int  dX, dY;
	} m_stCrusadeStructures[MAXCRUSADESTRUCTURES];


	// 2 variables? hmm
	int m_iCollectedMana[MAXSIDES];
	int m_mana[MAXSIDES];
	// int m_iAresdenMana, m_iElvineMana;

	TeleportLoc m_pGuildTeleportLoc[MAXGUILDS];
	//

	int8_t GSID;

	struct {
		int iCrashedStructureNum;
		int iStructureDamageAmount;
		int iCasualties;
	} m_stMeteorStrikeResult;

	struct {
		char cType;
		char cSide;
		short sX, sY;
	} m_stMiddleCrusadeStructureInfo[MAXCRUSADESTRUCTURES];
	int m_iTotalMiddleCrusadeStructures;

		int m_iClientShortCut[MAXCLIENTS+1];

	int m_iNpcConstructionPoint[MAXNPCTYPES];
	uint64_t m_dwCrusadeGUID;
	int   m_iCrusadeWinnerSide;

	int   m_iPlayerMaxLevel;

	int   m_iWorldMaxUser;

	short m_sForceRecallTime;
	short m_sSlateSuccessRate;

	int   m_iFinalShutdownCount;

	uint32_t  m_schedulesCnt;

	/*enum EventStatus{
		ES_ANNOUNCED_ONCE,
		ES_ANNOUNCED_TWICE,
		ES_ANNOUNCED_THRICE,
		ES_STARTED,
		ES_ENDED
	};
	struct {
		int iDay;
		int iHour;
		int iMinute;
		EventStatus evStatus;
		EventType evType;
		bool operator==(SYSTEMTIME sysTime)
		{
			return (iDay == sysTime.wDayOfWeek &&
				iHour == sysTime.wHour &&
				iMinute == sysTime.wMinute)
				? true : false;
		}
	} m_schedules[MAXSCHEDULE];*/

	Item * m_pGold;

	std::vector<Map*> maplist;

	//XSocket * m_webSocket;
	char m_websiteAddr[61];
	char m_websiteScriptAddr[61];
	int m_websitePort;

	bool	m_bReceivedItemList;

	bool m_SoccerMode;
	int m_SoccerAresdenGoals, m_SoccerElvineGoals, m_SoccerWinner;

	bool m_bNpcHunt;

	bool bDeleteMineral(int iIndex);
};

