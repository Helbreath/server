/**
* @file Map.h
* @date 2018/03/01 3:16
*
* @author Zero (zero@xandium.net)
*/

#pragma once

#include <cstdint>
#include "utility.hpp"
#include "occupyflag.hpp"
#include "tile.hpp"
#include "strategicpoint.hpp"
#include "teleportloc.hpp"
#include "structs.hpp"
#include "defines.hpp"

namespace hbx
{

class map
{
public:
    map();
    ~map();

    void restore_strike_points();
    bool remove_crusade_structure_info(int16_t sX, int16_t sY);
    bool add_crusade_structure_info(char cType, int16_t sX, int16_t sY, char cSide);
    int32_t get_attribute(int32_t dX, int32_t dY, int32_t iBitMask);
    void setup_no_attack_area();
    void clear_temp_sector_info();
    void clear_sector_info();
    int32_t get_occupy_flag(int32_t dX, int32_t dY, int32_t iSide, int32_t iEKNum, int32_t iDOI);
    int32_t  check_item(int16_t sX, int16_t sY);
    void set_temp_move_allowed_flag(int32_t dX, int32_t dY, bool bFlag);
    int32_t iAnalyze(char cType, int32_t * pX, int32_t * pY, int32_t * pV1, int32_t * pV2, int32_t * pV3);
    bool bGetIsWater(int16_t dX, int16_t dY);
    bool bGetIsFarm(int16_t dX, int16_t dY);
    bool bAddCropsTotalSum();
    bool bRemoveCropsTotalSum();
    void GetDeadOwner(int16_t * pOwner, char * pOwnerClass, int16_t sX, int16_t sY);
    bool bGetIsMoveAllowedTile(int16_t dX, int16_t dY);
    void SetNamingValueEmpty(int32_t iValue);
    int32_t iGetEmptyNamingValue();
    bool bGetDynamicObject(int16_t sX, int16_t sY, int16_t * pType, uint32_t * pTime, int32_t * pIndex = 0);
    void SetDynamicObject(uint16_t wID, int16_t sType, int16_t sX, int16_t sY, uint32_t dwTime);
    bool bGetIsTeleport(int16_t dX, int16_t dY);
    bool bSearchTeleportDest(int32_t sX, int32_t sY, std::string & pMapName, int32_t * pDx, int32_t * pDy, char * pDir);
    bool bInit(const std::string & map_name);
    bool bIsValidLoc(int16_t sX, int16_t sY);
    item * pGetItem(int16_t sX, int16_t sY, int16_t * pRemainItemSprite, int16_t * pRemainItemSpriteFrame, char * pRemainItemColor);
    bool bSetItem(int16_t sX, int16_t sY, item * pItem);
    void ClearDeadOwner(int16_t sX, int16_t sY);
    void ClearOwner(int32_t iDebugCode, int16_t sOwnerH, char cOwnerType, int16_t sX, int16_t sY);
    bool bGetMoveable(int16_t dX, int16_t dY, int16_t * pDOtype = 0, item * pTopItem = 0); // v2.172
    void GetOwner(int16_t * pOwner, char * pOwnerClass, int16_t sX, int16_t sY);
    void SetOwner(int16_t sOwner, char cOwnerClass, int16_t sX, int16_t sY);
    void SetDeadOwner(int16_t sOwner, char cOwnerClass, int16_t sX, int16_t sY);
    tile * m_pTile;
    std::string name;
    std::string location_name;
    int16_t m_sSizeX, m_sSizeY, m_sTileDataSize;
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

private:
    bool _bDecodeMapDataFileContents();
};

}
