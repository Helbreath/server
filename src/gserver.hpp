
#pragma once

#include <memory>
#include <vector>
#include <optional>
#include <mutex>
#include <list>
#include <queue>
#include <nlohmann/json.hpp>
#include "utility.hpp"
#include "streams.hpp"
#include "teleport_loc.hpp"

namespace spdlog { class logger; }

namespace hbx
{

class server;
class client;
class npc;
class map;
class magic;
class guild;
class build_item;
struct message_entry;
class chat_handler;
class mineral;
class Fish;
class Potion;
class skill;
class Crafting;
class Quest;
class teleport;
class delay_event;
class item;
class unit;

using namespace nlohmann;

using n_client = std::optional<std::shared_ptr<client>>;

class gserver
{
public:
    gserver(server * _server, const std::string & _config_file);
    ~gserver();

    server_status get_status() const noexcept { return status_; }
    map * get_map(std::string map_name);
    // By character id
    n_client get_client(uint64_t id);
    void handle_message(const message_entry & msg);
    void handle_new_client(std::shared_ptr<client> _client);
    void handle_close_client(std::shared_ptr<client> _client);
    void handle_initplayer(std::shared_ptr<client> _client, stream_read & sr);
    void handle_initdata(std::shared_ptr<client> _client, stream_read & sr);
    void handle_fullobjectdata(std::shared_ptr<client> _client, std::shared_ptr<unit> target);

    void send_event_near_type_a(std::shared_ptr<unit> owner, uint32_t msg_id, int16_t sV1, int16_t sV2, int16_t sV3);

    uint64_t object_counter = 0;

    std::shared_ptr<spdlog::logger> log;
    server & server_;
    server_status status_ = server_status::uninitialized;
    std::string server_name;
    uint64_t id = 0;
    std::mutex cl_m;
    std::vector<std::shared_ptr<client>> clients;
    std::vector<std::unique_ptr<map>> maps;
    json config;

    std::map<string, int16_t> moveLocationX;
    std::map<string, int16_t> moveLocationY;

    chat_handler * chathandler;

    void ChatThread();
    void SocketThread();
    void TimerThread();

    void DeleteClient(std::shared_ptr<client> client_, bool save = true, bool deleteobj = false);
    bool RegisterDelayEvent(int iDelayType, int iEffectType, uint64_t dwLastTime, std::shared_ptr<unit> unit_, map * pmap, int dX, int dY, int iV1, int iV2, int iV3);
    void RemoveFromDelayEventList(std::shared_ptr<unit> unit_, int32_t iEffectType);
    void DelayEventProcessor();

    bool Init(std::string config);
    bool LoadCharacterData(std::shared_ptr<client> client_);
    void InitPlayerData(std::shared_ptr<client> client_);
    void ClientCommonHandler(std::shared_ptr<client> client_, stream_read & sr);
    void ClientMotionHandler(std::shared_ptr<client> client_, stream_read & sr);
    int iClientMotion_Stop_Handler(std::shared_ptr<client> client_, uint16_t sX, uint16_t sY, uint8_t cDir);
    int iClientMotion_GetItem_Handler(std::shared_ptr<client> client_, uint16_t sX, uint16_t sY, uint8_t cDir);
    int iClientMotion_Attack_Handler(std::shared_ptr<client> client_, uint16_t sX, uint16_t sY, uint16_t dX, uint16_t dY, int16_t wType, uint8_t cDir, uint64_t wTargetObjectID, bool bResponse = true, bool bIsDash = false);
    int iClientMotion_Move_Handler(std::shared_ptr<client> client_, uint16_t sX, uint16_t sY, uint8_t cDir, bool bIsRun);
    int iClientMotion_Magic_Handler(std::shared_ptr<client> client_, uint16_t sX, uint16_t sY, uint8_t cDir);
    void SendEventToNearClient_TypeA(std::shared_ptr<unit> owner, uint32_t msgid, uint32_t sV1, uint32_t sV2, uint32_t sV3);
    //void SendEventToNearClient_TypeA(shared_ptr<Npc> npc, uint16_t msgid, int16_t sV1, int16_t sV2, int16_t sV3);
    void SendEventToNearClient_TypeB(uint32_t msgid, uint16_t msgtype, map * mapIndex, uint16_t sX, uint16_t sY, uint32_t sV1 = 0, uint32_t sV2 = 0, uint32_t sV3 = 0, uint32_t sV4 = 0);
    void SendObjectMotionRejectMsg(std::shared_ptr<client> client_);
    bool _bAddClientItemList(std::shared_ptr<client> client_, item * pItem, int * pDelReq);
    void RequestFullObjectData(std::shared_ptr<client> client_, std::shared_ptr<unit> target);
    bool _bGetIsPlayerHostile(std::shared_ptr<client> player, std::shared_ptr<client> target);
    bool WriteTileData(stream_write & sw, std::shared_ptr<client> player, uint16_t ix, uint16_t iy);
    void AddGroundItem(item * pItem, uint16_t x, uint16_t y, map * mapIndex, uint64_t dwTime);
    uint8_t iSetSide(client * client_);
    void RequestHuntmode(std::shared_ptr<client> client_);
    void RequestRestartHandler(std::shared_ptr<client> player);
    void SendNotifyMsg(std::shared_ptr<client> from, std::shared_ptr<client> to, uint16_t wMsgType, uint64_t sV1 = 0, uint64_t sV2 = 0, uint64_t sV3 = 0, std::string pString = "",
        uint64_t sV4 = 0, uint64_t sV5 = 0, uint64_t sV6 = 0, uint64_t sV7 = 0, uint64_t sV8 = 0,
        uint64_t sV9 = 0, std::string pString2 = "");
    uint64_t iGetLevelExp(uint16_t iLevel);
    item * _iGetArrowItemIndex(client * client_);
    int _iCalcMaxLoad(client * client_);
    int iCalcTotalWeight(std::shared_ptr<client> client_);
    void WriteItemData(stream_write & sw, item * pItem) const;
    void SendItemNotifyMsg(std::shared_ptr<client> client_, uint16_t msgtype, item * pItem, int iV1);
    void DropItemHandler(std::shared_ptr<client> client_, short sItemIndex, int iAmount, std::string pItemName, bool bByPlayer);
    void ItemDepleteHandler(std::shared_ptr<client> client_, item * pItem, bool bIsUseItemResult, bool bIsLog = true, bool notify = true);
    void RequestTeleportHandler(std::shared_ptr<client> client_, char teleportType, std::string cMapName = "", int dX = -1, int dY = -1);
    int iGetMapLocationSide(std::string MapName);

    void PlayerMapEntry(std::shared_ptr<client> client_, bool setRecallTime);
    void ToggleCombatModeHandler(std::shared_ptr<client> client_);
    void RequestTitleHandler(std::shared_ptr<client> client_, stream_read & sr);
    bool bCheckClientAttackFrequency(std::shared_ptr<client> client_);
    bool bCheckClientMagicFrequency(client * client_);
    bool bCheckClientMoveFrequency(std::shared_ptr<client> client_, bool running);
    void _CheckAttackType(client * client_, int16_t & spType);
    void ClearSkillUsingStatus(std::shared_ptr<client> client_);
    void UseItemHandler(client * client_, short sItemIndex, short dX, short dY, short sDestItemID);

    int getPlayerNum(map * pMap, short dX, short dY, char cRadius);
    void CalculateSSN_ItemIndex(client * client_, item * Weapon, int iValue);
    int32_t CalculateAttackEffect(std::shared_ptr<unit> target, std::shared_ptr<unit> attacker, int tdX, int tdY, int iAttackMode, bool bNearAttack = false, bool bIsDash = false);
    int32_t CalculateUseSkillItemEffect(std::shared_ptr<client> player, int16_t skillvalue, int skillnum, map * map_, int32_t x, int32_t y);
    void EnduStrippingDamage(std::shared_ptr<unit> target, std::shared_ptr<unit> attacker, int item_, int higherStripEndu, bool shield = false);
    void Effect_Damage_Spot(std::shared_ptr<unit> attacker, std::shared_ptr<unit> target, short sV1, short sV2, short sV3, bool exp, Element element = ELEMENT_NONE, magic * spell = nullptr);
    void Effect_HpUp_Spot(std::shared_ptr<unit> caster, std::shared_ptr<unit> target, short sV1, short sV2, short sV3);
    void Effect_SpDown_Spot(std::shared_ptr<unit> caster, std::shared_ptr<unit> target, short sV1, short sV2, short sV3);
    void Effect_SpUp_Spot(std::shared_ptr<unit> caster, std::shared_ptr<unit> target, short sV1, short sV2, short sV3);
    void Effect_Damage_Spot_DamageMove(std::shared_ptr<unit> attacker, std::shared_ptr<unit> target, short sV1, short sV2, short sV3, bool exp, Element element = ELEMENT_NONE, magic * spell = nullptr);
    void PlayerMagicHandler(std::shared_ptr<client> client_, uint16_t dX, uint16_t dY, int16_t sType, bool bItemEffect = false, int32_t iV1 = 0);

    int _iCalcSkillSSNpoint(int iLevel);

    void _CheckMiningAction(client * client_, int dX, int dY);
    void GetExp(std::shared_ptr<client> client_, uint64_t iExp, bool bIsAttackerOwn = false);

    int iAddDynamicObjectList(unit * owner, short sType, map * pmap, short sX, short sY, uint32_t dwLastTime, int iV1);
    void CheckDynamicObjectList();
    int _iGetWeaponSkillType(std::shared_ptr<client> client_);
    int16_t iGetComboAttackBonus(int16_t iSkill, int16_t iComboCount);

    bool bAnalyzeCriminalAction(std::shared_ptr<client> client_, short dX, short dY, bool bIsCheck = false);

    std::shared_ptr<client> GetClient(uint64_t ObjectID);
    npc * GetNpcByName(std::string name);
    item * GetItemByName(std::string name);

    void ParseChat(client * client_, std::string message);









    std::vector<std::shared_ptr<delay_event>> DelayEventList;
    std::mutex delayMutex;

    //	DropManager dropManager;


    std::queue<std::unique_ptr<message_entry>> chatpipe;
    std::mutex mutchat;
    std::mutex mutobjectlist;

    //TODO: guilds - load all the guilds from the db and keep them in memory
    //Give guilds more purpose than just another chat channel in game

    std::list<guild *> guildlist;


    //need a config array for variables moving forward, temporary solution for like a day (/cough week)
    uint16_t lockedMapTimeDefault;
    uint16_t farmRestartLimit;//level limit where players can no longer respawn in the farm

    string servername;
    uint64_t m_iLimitedUserExp, m_iLevelExp51;

    std::unordered_map<string, uint8_t> m_mapNameList;

    int m_iTotalMaps;
    bool m_dropsInitiated;
    bool m_bIsGameStarted;


    std::vector<item *> m_pItemConfigList;
    std::vector<npc *> m_npcConfigList;
    std::vector<magic *> m_pMagicConfigList;
    std::vector<skill *> m_pSkillConfigList;
    std::vector<Quest *> m_pQuestConfigList;
    std::vector<Crafting *> m_pCraftingConfigList;
    std::vector<npc *> m_pNpcList;




    teleport * m_pTeleportConfigList[MAXTELEPORTLIST];

    int m_iTotalClients, m_iMaxClients, m_iTotalGameServerClients, m_iTotalGameServerMaxClients;

    bool m_bOnExitProcess;
    uint64_t m_dwExitProcessTime;

    uint64_t m_dwWeatherTime, m_dwGameTime1, m_dwGameTime2, m_dwGameTime3, m_dwGameTime4, m_dwGameTime5, m_dwGameTime6, m_dwFishTime;

    char m_cDayOrNight;
    int m_iSkillSSNpoint[102];

    int m_iTotalNoticeMsg, m_iPrevSendNoticeMsg;
    uint64_t m_dwNoticeTime, m_dwSpecialEventTime, m_startTime;
    bool m_bIsSpecialEventTime;
    char m_cSpecialEventType;

    uint64_t m_iLevelExpTable[300];
    Fish * m_pFish[MAXFISHS];
    Potion * m_pPortionConfigList[MAXPOTIONTYPES];

    bool m_bIsServerShutdowned;
    char m_cShutDownCode;
    mineral * m_pMineral[MAXMINERALS];

    int m_iMiddlelandMapIndex;
    int m_iAresdenMapIndex;
    int m_iElvineMapIndex;
    faction m_astoriaBasePos[3];
    faction m_eventWinner[ET_MAX];

    bool m_bIsApocalypseMode;
    bool m_bIsApocalypseGateOpen;

    bool m_bHeldenianMode;
    uint32_t m_dwHeldenianGUID;
    int	m_iHeldenianType1Winner, m_iHeldenianType2Winner;
    int	m_iHeldenianType;
    int	m_iLastHeldenianType;
    int m_iHeldenianAresdenDead, m_iHeldenianElvineDead, m_iHeldenianAresdenKill, m_iHeldenianElvineKill, m_iHeldenianAresdenFlags, m_iHeldenianElvineFlags,
        m_iHeldenianAresdenLeftTower, m_iHeldenianElvineLeftTower;
    int	m_iBtFieldMapIndex;
    int	m_iRampartMapIndex;
    int	m_iGodHMapIndex;

    int m_iMaxGMGMana;
    int m_iAresdenOccupyTiles;
    int m_iElvineOccupyTiles;
    int m_iCurMsgs, m_iMaxMsgs;


    int iDMGCount; // New monster event xRisenx


    uint64_t m_dwCanFightzoneReserveTime;

    int  m_iFightZoneReserve[MAXFIGHTZONE];
    uint64_t m_donateEventHolder;
    uint32_t m_donateEventPot;

    struct {
        uint64_t iFunds;
        uint64_t iCrimes;
        uint64_t iWins;
    } m_stCityStatus[MAXSIDES];

    int	  m_iStrategicStatus;

    build_item * m_pBuildItemList[MAXBUILDITEMS];
    item * m_pDupItemIDList[MAXDUPITEMID];

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

    teleport_loc m_pGuildTeleportLoc[MAXGUILDS];
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

    int m_iClientShortCut[MAXCLIENTS + 1];

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

    item * m_pGold;

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

}
