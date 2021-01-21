/**
* @file Map.h
* @date 2018/03/01 3:16
*
* @author Zero (zero@xandium.net)
*/

#pragma once

#include <cstdint>
#include <mutex>
#include <vector>
#include <list>
#include <queue>
#include "utility.hpp"
#include "occupy_flag.hpp"
#include "tile.hpp"
#include "strategic_point.hpp"
#include "teleport_loc.hpp"
#include "structs.hpp"
#include "defines.hpp"
#include "net/net_handler.hpp"

namespace hbx
{

class unit;
class npc;
class item;
class teleport_loc;
class delay_event;
class gserver;
struct message_entry;

class map
{
public:
    map(gserver * gs_);
    ~map();

    tile * m_pTile;
    std::string name;
    std::string location_name;
    int32_t m_sSizeX, m_sSizeY, m_sTileDataSize;
    teleport_loc * m_pTeleportLoc[DEF_MAXTELEPORTLOC];

    point m_pInitialPoint[DEF_MAXINITIALPOINT];

    bool  m_bNamingValueUsingStatus[1000];
    bool  m_bRandomMobGenerator;
    char  m_cRandomMobGeneratorLevel;
    int32_t   m_iTotalActiveObject;
    int32_t   m_iTotalAliveObject;
    int32_t   m_iMaximumObject;

    char  m_cType;

    bool  m_bIsFixedDayMode;
    bool  m_bIsFixedSnowMode;
    uint8_t day_or_night = 0;

    struct spot_mob_generator_st 
    {
        bool bDefined;
        char cType;

        char cWaypoint[10];
        rect rcRect;

        int32_t  iTotalActiveMob;
        int32_t  iMobType;
        int32_t  iMaxMobs;
        int32_t  iCurMobs;

    };
    spot_mob_generator_st spot_mob_generator[DEF_MAXSPOTMOBGENERATOR];

    point m_WaypointList[DEF_MAXWAYPOINTCFG];
    rect  m_rcMobGenAvoidRect[DEF_MAXMGAR];
    rect  m_rcNoAttackRect[DEF_MAXNMR];

    point m_FishPointList[DEF_MAXFISHPOINT];
    int32_t   m_iTotalFishPoint, m_iMaxFish, m_iCurFish;

    bool  m_bMineralGenerator;
    char  m_cMineralGeneratorLevel;
    point m_MineralPointList[DEF_MAXMINERALPOINT];
    int32_t   m_iTotalMineralPoint, m_iMaxMineral, m_iCurMineral;

    char  m_cWhetherStatus;
    uint32_t m_dwWhetherLastTime, m_dwWhetherStartTime;

    int32_t   m_iLevelLimit;
    int32_t   m_iUpperLevelLimit;

    occupy_flag * m_pOccupyFlag[DEF_MAXOCCUPYFLAG];
    int32_t   m_iTotalOccupyFlags;

    strategic_point * m_pStrategicPointList[DEF_MAXSTRATEGICPOINTS];
    bool  m_bIsAttackEnabled;

    bool  m_bIsFightZone;

    struct energy_sphere_creation_list_st {
        char cType;
        int32_t sX, sY;

    };
    energy_sphere_creation_list_st energy_sphere_creation_list[DEF_MAXENERGYSPHERES];

    int32_t m_iTotalEnergySphereCreationPoint;

    struct energy_sphere_goal_list_st {
        char cResult;
        int32_t aresdenX, aresdenY, elvineX, elvineY;
    };
    energy_sphere_goal_list_st energy_sphere_goal_list[DEF_MAXENERGYSPHERES];

    int32_t m_iTotalEnergySphereGoalPoint;

    bool m_bIsEnergySphereGoalEnabled;
    int32_t m_iCurEnergySphereGoalPointIndex;

    struct sector_info_st {
        int32_t iPlayerActivity = 0;
        int32_t iNeutralActivity = 0;
        int32_t iAresdenActivity = 0;
        int32_t iElvineActivity = 0;
        int32_t iMonsterActivity = 0;
    };
    
    sector_info_st sector_info[DEF_MAXSECTORS][DEF_MAXSECTORS];
    sector_info_st temp_sector_info[DEF_MAXSECTORS][DEF_MAXSECTORS];

    int32_t m_iMaxNx, m_iMaxNy, m_iMaxAx, m_iMaxAy, m_iMaxEx, m_iMaxEy, m_iMaxMx, m_iMaxMy, m_iMaxPx, m_iMaxPy;

    struct strike_point_st {
        std::string related_map_name;
        int32_t map_index = -1;
        int32_t dx = 0;
        int32_t dy = 0;
        int32_t hp = 0;
        int32_t init_hp = 0;

        int32_t effect_x[5] = { 0, 0, 0, 0, 0 };
        int32_t effect_y[5] = { 0, 0, 0, 0, 0 };

    };
    strike_point_st strike_point[DEF_MAXSTRIKEPOINTS];
    int32_t m_iTotalStrikePoints;

    bool m_bIsDisabled;

    crusade_structure_info_st crusade_structure_info[DEF_MAXCRUSADESTRUCTURES];
    int32_t m_iTotalCrusadeStructures;
    int32_t m_iTotalAgriculture;

    struct item_event_list_st {
        std::string item_name;
        int32_t amount;
        int32_t total_num;
        int32_t month;
        int32_t day;
        int32_t type;
        char * mob[5];

        int32_t current_num;
        int32_t	num_mob;
    };
    item_event_list_st item_event_list[DEF_MAXITEMEVENTS];
    int32_t m_iTotalItemEvents;

    bool m_bIsEnergySphereAutoCreation;

    int16_t sMobEventAmount;

    std::mutex m;

    void run();

    std::thread * actionthread;
    std::thread * timerthread;
    std::queue<std::unique_ptr<message_entry>> actionpipe;
    std::mutex mutaction;
    bool TimerThreadRunning;

    void handle_message(std::shared_ptr<client> client_, stream_read & sr);

    void TimerThread();
    void SocketThread();
    void put_msg_queue(message_entry * msg);



    void NpcProcess();

    void RemoveFromTarget(std::shared_ptr<unit> target, int iCode = 0);
    void NpcKilledHandler(std::shared_ptr<unit> attacker, std::shared_ptr<npc> npc_, int16_t damage) const;
    void NpcBehavior_Flee(std::shared_ptr<npc> npc_) const;
    void NpcBehavior_Dead(std::shared_ptr<npc> npc_);
    void NpcDeadItemGenerator(std::shared_ptr<unit> attacker, std::shared_ptr<npc> npc_) const;

    std::shared_ptr<npc> CreateNpc(string & pNpcName, char cSA, char cMoveType, uint16_t * poX, uint16_t * poY, faction changeSide, char * pWaypointList, rect * pArea, int iSpotMobIndex, bool bHideGenMode = false, bool bIsSummoned = false, bool bFirmBerserk = false, bool bIsMaster = false, int iGuildGUID = 0);
    void DeleteNpc(std::shared_ptr<npc> npc_);

    char cGetNextMoveDir(short sX, short sY, short dstX, short dstY, map * map_, char cTurn, int * pError) const;
    char cGetNextMoveDir(short sX, short sY, short dstX, short dstY, map * map_, char cTurn, int * pError, short * DOType) const;

    void RemoveFromDelayEventList(unit * unit_, int32_t iEffectType);
    bool RegisterDelayEvent(int iDelayType, int iEffectType, uint64_t dwLastTime, std::shared_ptr<unit> unit_, int dX, int dY, int iV1, int iV2, int iV3);
    void DelayEventProcessor();
    bool bGetEmptyPosition(int16_t & pX, int16_t & pY, std::shared_ptr<unit> client_);

    std::shared_ptr<npc> GetNpc(uint64_t ObjectID);

    std::list<std::shared_ptr<npc>> npclist;

    uint64_t npchandle;

    //	DropManager	drops;

    struct
    {
        item * item_;
        time_t dropTime;
        uint16_t sx, sy;
        map * cMapIndex;
        bool bEmpty;
    } m_stGroundNpcItem[MAXGROUNDITEMS];

    std::list<std::shared_ptr<delay_event>> DelayEventList;
    std::mutex delayMutex;



    void RestoreStrikePoints();
    bool bRemoveCrusadeStructureInfo(short sX, short sY);
    bool bAddCrusadeStructureInfo(char cType, short sX, short sY, char cSide);
    int iGetAttribute(int dX, int dY, int iBitMask) const;
    void _SetupNoAttackArea();
    void ClearTempSectorInfo();
    void ClearSectorInfo();
    uint64_t iCheckItem(short sX, short sY) const;
    void SetTempMoveAllowedFlag(int dX, int dY, bool bFlag) const;
    int iAnalyze(char cType, int * pX, int * pY, int * pV1, int * pV2, int * pV3) const;
    bool bGetIsWater(short dX, short dY) const;
    bool bGetIsFarm(short dX, short dY) const;
    bool IsBuild(short dX, short dY) const;
    bool bAddCropsTotalSum();
    bool bRemoveCropsTotalSum();
    bool bGetIsMoveAllowedTile(short dX, short dY) const;
    bool bGetIsMoveAllowedTile(Point p) const;
    bool bGetDynamicObject(short sX, short sY, short * pType, uint64_t * pRegisterTime, int * pIndex = 0) const;
    void SetDynamicObject(uint16_t wID, short sType, short sX, short sY, uint64_t dwRegisterTime) const;
    bool bGetIsTeleport(short dX, short dY) const;
    bool bSearchTeleportDest(int sX, int sY, std::string & pMapName, uint16_t * pDx, uint16_t * pDy, uint8_t * pDir);
    bool bInit(std::string pName);
    bool bIsValidLoc(short sX, short sY) const;
    item * pGetItem(short sX, short sY, short * pRemainItemSprite, short * pRemainItemSpriteFrame, uint32_t * pRemainItemColor) const;
    bool bSetItem(short sX, short sY, item * pItem) const;
    void ClearDeadOwner(short sX, short sY) const;
    void ClearOwner(short sX, short sY) const;
    bool bGetMoveable(short dX, short dY, short * pDOtype = nullptr, item * pTopItem = nullptr) const;
    std::shared_ptr<unit> GetOwner(short sX, short sY) const;
    void SetOwner(std::shared_ptr<unit> sOwner, short sX, short sY);
    std::shared_ptr<unit> GetDeadOwner(short sX, short sY) const;
    void SetDeadOwner(std::shared_ptr<unit> sOwner, short sX, short sY);
    std::list<std::shared_ptr<unit>>GetOwners(short x1, short x2, short y1, short y2) const;
    void IncPlayerActivity(client * player);
    bool bDecodeMapConfig();
    bool GetInitialPoint(int16_t & pX, int16_t & pY, std::string & pPlayerLocation);
    tile * GetTile(int16_t x, int16_t y) const;

    gserver * gs;
    std::string factionName;
    short sizeX, sizeY, tileDataSize;
    std::vector<teleport_loc *> teleportLocationList;

    //short m_sInitialPointX, m_sInitialPointY;
    std::vector<point> initialPointList;


    //map flags
    struct stMapFlags {
        bool apocalypseMap;
        bool heldenianMap;
        bool mineralGenerator;
        bool randomMobGenerator;
        bool attackEnabled;
        bool partyDisabled;
        bool shieldDisabled;
        bool armorDisabled;
        bool chatDisabled;
        bool magicLimited[MAXMAGICTYPE];
        bool permIllusionOn;
        bool disabled;
        bool fightZone;
    } flags;

    uint8_t mineralGenLevel;
    std::vector<point> mineralPointList;
    uint16_t   mineralTotalPoint, mineralMax, mineralCurrent;


    int8_t  heldenianModeMap;
    int16_t heldenianWinningZoneX;
    int16_t heldenianWinningZoneY;

    ApocMobGenType apocalypseMobGenType;
    int32_t apocalypseBossMobNpcID;
    rect	apocalypseBossMobRect;

    uint16_t mobGenLevel;
    uint16_t totalActiveObject;
    uint16_t totalAliveObject;
    uint16_t maximumObject;//redundant?

    char    dynamicGateType;
    int16_t dynamicGateCoordRectX1, dynamicGateCoordRectY1, dynamicGateCoordRectX2, dynamicGateCoordRectY2;
    std::string  dynamicGateCoordDestMap;
    int16_t dynamicGateCoordTgtX, dynamicGateCoordTgtY;



    struct heldgatedoor {
        char  cDir;
        short dX;
        short dY;
    };
    std::vector<heldgatedoor> heldenianGateDoor;

    struct heldtower {
        short sTypeID;
        short dX;
        short dY;
        char  cSide;
    };
    std::vector<heldtower> heldenianTower;



    int8_t type;
    bool   fixedDay;
    bool   fixedSnow;

    struct spotmobgen {
        bool defined;
        char genType;				// 1:RANDOMAREA   2:RANDOMWAYPOINT

        char waypoints[10];
        rect rcRect;

        int  active;//??
        int  type;
        int  max;
        int  current;

    };
    std::vector<spotmobgen> spotMobGenerator;

    std::vector<point> waypointList;
    std::vector<rect>  mobGeneratorAvoidList;
    std::vector<rect>  safeZoneList;

    //TODO: figure out something with fishing. no one ever did it for 16 years, why should they do it now?
    std::vector<point> fishingPointList;
    uint16_t   totalFishPoints, fishMax, fishCurrent;




    Weather weather;
    uint32_t weatherEndTime, weatherStartTime;

    uint16_t   levelLimitLower;
    uint16_t   levelLimitUpper;


    struct OccupyFlag
    {
        OccupyFlag(int16_t dX, int16_t dY, char cSide, int32_t iEKNum, int32_t iDOI)
        {
            x = dX;
            y = dY;

            side = cSide;
            EKCount = iEKNum;

            dynamicObjectIndex = iDOI;
        }

        char side;
        int  EKCount;
        int  x, y;

        int  dynamicObjectIndex;
    };

    struct StrategicPoint
    {
        StrategicPoint()
        {
            side = value = x = y = 0;
        }
        int8_t		side;
        int32_t     value;
        int16_t		x, y;
    };
    std::vector<StrategicPoint *> strategicPointList;

    std::vector<OccupyFlag *> occupyFlag;
    int   occupyFlagTotal;

    struct sectorinfo {
        int playerActivity;
        int neutralActivity;
        int aresdenActivity;
        int elvineActivity;
        int mobActivity;
    };

    //use vectors or maps?
    std::vector<sectorinfo> sectorInfo;
    std::vector<sectorinfo> sectorInfoTemp;

    struct strikepoint {
        //map * map_;
        int16_t x, y;
        int64_t hp, hpInit;

        int32_t effectX[5];
        int32_t effectY[5];

    };
    std::vector<strikepoint> strikePointList;

    struct crusadestructure {
        char cType;
        char cSide;
        short sX, sY;
    };
    std::vector<crusadestructure> m_stCrusadeStructureInfo;

    struct itemeventlist {
        std::string cItemName;
        int iAmount;
        int iTotalNum;
        int iMonth;
        int iDay;
        int iType;
        std::string cMob[5];

        int iCurNum;
        int	iNumMob;
    };
    std::vector<itemeventlist> itemEventList;

    short mobEventAmount;

    int chatZone;//?

    map::OccupyFlag * iRegisterOccupyFlag(int dX, int dY, int iSide, int iEKNum, int iDOI);

private:
    bool _bDecodeMapDataFileContents();
};

}
