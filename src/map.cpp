
#include "map.hpp"
#include "server.hpp"
#include <fstream>

namespace hbx
{

map::map()
{
    for (int i = 0; i < DEF_MAXTELEPORTLOC; i++)
        m_pTeleportLoc[i] = nullptr;

    for (int i = 0; i < DEF_MAXWAYPOINTCFG; i++)
    {
        m_WaypointList[i].x = -1;
        m_WaypointList[i].y = -1;
    }

    for (int i = 0; i < DEF_MAXMGAR; i++)
    {
        m_rcMobGenAvoidRect[i].top = -1;
        m_rcMobGenAvoidRect[i].left = -1;
    }

    for (int i = 0; i < DEF_MAXNMR; i++)
    {
        m_rcNoAttackRect[i].top = -1;
        m_rcNoAttackRect[i].left = -1;
    }

    for (int i = 0; i < DEF_MAXSPOTMOBGENERATOR; i++)
    {
        spot_mob_generator[i].bDefined = false;
        spot_mob_generator[i].iTotalActiveMob = 0;
    }

    for (int i = 0; i < DEF_MAXFISHPOINT; i++)
    {
        m_FishPointList[i].x = -1;
        m_FishPointList[i].y = -1;
    }

    for (int i = 0; i < DEF_MAXMINERALPOINT; i++)
    {
        m_MineralPointList[i].x = -1;
        m_MineralPointList[i].y = -1;
    }

    for (int i = 0; i < DEF_MAXINITIALPOINT; i++)
    {
        m_pInitialPoint[i].x = -1;
        m_pInitialPoint[i].y = -1;
    }

    for (int i = 0; i < 1000; i++)
        m_bNamingValueUsingStatus[i] = false;

    for (int i = 0; i < DEF_MAXOCCUPYFLAG; i++)
        m_pOccupyFlag[i] = nullptr;

    for (int i = 0; i < DEF_MAXSTRATEGICPOINTS; i++)
        m_pStrategicPointList[i] = nullptr;

    for (int i = 0; i < DEF_MAXENERGYSPHERES; i++)
    {
        energy_sphere_creation_list[i].cType = 0;
        energy_sphere_goal_list[i].cResult = 0;
    }

    for (int i = 0; i < DEF_MAXITEMEVENTS; i++)
    {
        item_event_list[i].amount = 0;
        item_event_list[i].total_num = 0;
        item_event_list[i].current_num = 0;
        item_event_list[i].month = 0;
        item_event_list[i].day = 0;
    }
    m_iTotalItemEvents = 0;

    m_iTotalActiveObject = 0;
    m_iTotalAliveObject = 0;

    m_bIsFixedDayMode = false;
    m_bIsFixedSnowMode = false;

    m_iTotalFishPoint = 0;
    m_iMaxFish = 0;
    m_iCurFish = 0;

    m_iTotalMineralPoint = 0;
    m_iMaxMineral = 0;
    m_iCurMineral = 0;

    m_pTile = nullptr;

    m_cWhetherStatus = 0;
    m_cType = DEF_MAPTYPE_NORMAL;

    m_iLevelLimit = 0;
    m_iUpperLevelLimit = 0;
    m_bMineralGenerator = false;

    m_iTotalOccupyFlags = 0;

    m_bIsAttackEnabled = true;
    m_cRandomMobGeneratorLevel = 0;

    m_bIsFightZone = false;

    m_iTotalEnergySphereCreationPoint = 0;
    m_iTotalEnergySphereGoalPoint = 0;

    m_bIsEnergySphereGoalEnabled = false;
    m_iCurEnergySphereGoalPointIndex = -1;

    m_iMaxNx = m_iMaxNy = m_iMaxAx = m_iMaxAy = m_iMaxEx = m_iMaxEy = m_iMaxMx = m_iMaxMy = m_iMaxPx = m_iMaxPy = 0;

    m_iTotalStrikePoints = 0;
    m_bIsDisabled = false;

    m_iTotalCrusadeStructures = 0;
    m_iTotalAgriculture = 0;

    m_bIsEnergySphereAutoCreation = false;

    sMobEventAmount = 15;
}

map::~map()
{
    if (m_pTile != nullptr)
        delete[]m_pTile;

    for (auto & teleport : m_pTeleportLoc)
        if (teleport != nullptr) delete teleport;

    for (auto & occupy : m_pOccupyFlag)
        if (occupy != nullptr) delete occupy;

    for (auto & strat : m_pStrategicPointList)
        if (strat != nullptr) delete strat;
}

void map::SetOwner(int16_t  sOwner, char cOwnerClass, int16_t  sX, int16_t  sY)
{
    tile * pTile;

    if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return;

    pTile = (tile *)(m_pTile + sX + sY * m_sSizeY);
    pTile->m_sOwner = sOwner;
    pTile->m_cOwnerClass = cOwnerClass;
}


void map::SetDeadOwner(int16_t  sOwner, char cOwnerClass, int16_t  sX, int16_t  sY)
{
    tile * pTile;

    if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return;

    pTile = (tile *)(m_pTile + sX + sY * m_sSizeY);
    pTile->m_sDeadOwner = sOwner;
    pTile->m_cDeadOwnerClass = cOwnerClass;
}


void map::GetOwner(int16_t * pOwner, char * pOwnerClass, int16_t  sX, int16_t  sY)
{
    tile * pTile;

    if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY))
    {
        *pOwner = 0;
        *pOwnerClass = 0;
        return;
    }

    pTile = (tile *)(m_pTile + sX + sY * m_sSizeY);
    *pOwner = pTile->m_sOwner;
    *pOwnerClass = pTile->m_cOwnerClass;

    if (pTile->m_sOwner == 0) *pOwnerClass = 0;
}


void map::GetDeadOwner(int16_t * pOwner, char * pOwnerClass, int16_t  sX, int16_t  sY)
{
    tile * pTile;

    if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY))
    {
        *pOwner = 0;
        *pOwnerClass = 0;
        return;
    }

    pTile = (tile *)(m_pTile + sX + sY * m_sSizeY);
    *pOwner = pTile->m_sDeadOwner;
    *pOwnerClass = pTile->m_cDeadOwnerClass;
}


bool map::bGetMoveable(int16_t  dX, int16_t  dY, int16_t * pDOtype, item * pTopItem)
{
    tile * pTile;

    if ((dX < 20) || (dX >= m_sSizeX - 20) || (dY < 20) || (dY >= m_sSizeY - 20)) return false;

    pTile = (tile *)(m_pTile + dX + dY * m_sSizeY);

    if (pDOtype != nullptr) *pDOtype = pTile->m_sDynamicObjectType;
    if (pTopItem != nullptr)
        if (!pTile->items.empty())
            pTopItem = pTile->items.back();
    //if (pTopItem != nullptr) pTopItem = pTile->m_pItem[0];

    if (pTile->m_sOwner != 0) return false;
    if (pTile->move_allowed == false) return false;
    if (pTile->temp_move_allowed == false) return false;

    return true;
}

bool map::bGetIsMoveAllowedTile(int16_t  dX, int16_t  dY)
{
    tile * pTile;

    if ((dX < 20) || (dX >= m_sSizeX - 20) || (dY < 20) || (dY >= m_sSizeY - 20)) return false;

    pTile = (tile *)(m_pTile + dX + dY * m_sSizeY);

    if (pTile->move_allowed == false) return false;
    if (pTile->temp_move_allowed == false) return false;

    return true;
}

bool map::bGetIsTeleport(int16_t  dX, int16_t  dY)
{
    tile * pTile;

    if ((dX < 14) || (dX >= m_sSizeX - 16) || (dY < 12) || (dY >= m_sSizeY - 14)) return false;

    pTile = (tile *)(m_pTile + dX + dY * m_sSizeY);

    if (pTile->teleport == false) return false;

    return true;
}

void map::ClearOwner(int32_t iDebugCode, int16_t  sOwnerH, char cOwnerType, int16_t  sX, int16_t  sY)
{
    tile * pTile;

    if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return;

    pTile = (tile *)(m_pTile + sX + sY * m_sSizeY);

    if ((pTile->m_sOwner == sOwnerH) && (pTile->m_cOwnerClass == cOwnerType))
    {
        pTile->m_sOwner = 0;
        pTile->m_cOwnerClass = 0;
    }

    if ((pTile->m_sDeadOwner == sOwnerH) && (pTile->m_cDeadOwnerClass == cOwnerType))
    {
        pTile->m_sDeadOwner = 0;
        pTile->m_cDeadOwnerClass = 0;
    }
}

void map::ClearDeadOwner(int16_t  sX, int16_t  sY)
{
    tile * pTile;

    if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return;

    pTile = (tile *)(m_pTile + sX + sY * m_sSizeY);
    pTile->m_sDeadOwner = 0;
    pTile->m_cDeadOwnerClass = 0;
}

bool map::bSetItem(int16_t  sX, int16_t  sY, item * pItem)
{
    tile * pTile;

    if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return false;

    pTile = (tile *)(m_pTile + sX + sY * m_sSizeY);

    pTile->items.push_back(pItem);

    return true;
}


item * map::pGetItem(int16_t  sX, int16_t  sY, int16_t * pRemainItemSprite, int16_t * pRemainItemSpriteFrame, uint32_t * pRemainItemColor) //v1.4 color
{
    tile * pTile;
    item * pItem;

    if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return nullptr;

    pTile = (tile *)(m_pTile + sX + sY * m_sSizeY);
    if (pTile->items.empty()) return nullptr;

    pItem = pTile->items.back();
    pTile->items.pop_back();

    *pRemainItemSprite = pItem->sprite;
    *pRemainItemSpriteFrame = pItem->sprite_frame;
    *pRemainItemColor = pItem->color;

    return pItem;
}


int32_t map::check_item(int16_t  sX, int16_t  sY)
{
    tile * pTile;
    item * pItem;

    if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return 0;

    pTile = (tile *)(m_pTile + sX + sY * m_sSizeY);
    if (pTile->items.empty()) return 0;
    pItem = pTile->items.back();

    return pItem->id;
}


bool map::bIsValidLoc(int16_t  sX, int16_t  sY)
{
    if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return false;
    return true;
}

bool map::bInit(const std::string & map_name)
{
    name = map_name;

    if (_bDecodeMapDataFileContents() == false)
        return false;

    for (auto & teleport : m_pTeleportLoc)
        teleport = nullptr;

    return true;
}

bool map::_bDecodeMapDataFileContents()
{
    std::string filename;
    char  cHeader[260], cTemp[100];
    std::string token;
    char cReadMode;
    char seps[] = "= \t\n";
    tile * pTile;
    int16_t * sp;

    filename = fmt::format("mapdata\\{}.amd", name);

    std::ifstream istrm(filename.c_str(), std::ios::binary);

    if (!istrm.is_open())
        return false;

    memset(cHeader, 0, sizeof(cHeader));

    istrm.read(cHeader, 256);
    if (!istrm.good())
        return false;

    for (int i = 0; i < 256; i++)
        if (cHeader[i] == 0) cHeader[i] = ' ';

    cReadMode = 0;

    auto strs = utility::tokenizer(cHeader, seps);
    token = strs.get();
    try
    {
        while (true)
        {

            if (cReadMode != 0)
            {
                switch (cReadMode)
                {
                    case 1:
                        m_sSizeX = atoi(token.c_str());
                        cReadMode = 0;
                        break;
                    case 2:
                        m_sSizeY = atoi(token.c_str());
                        cReadMode = 0;
                        break;
                    case 3:
                        m_sTileDataSize = atoi(token.c_str());
                        cReadMode = 0;
                        break;
                }
            }
            else
            {
                if (token == "MAPSIZEX") cReadMode = 1;
                if (token == "MAPSIZEY") cReadMode = 2;
                if (token == "TILESIZE") cReadMode = 3;
            }

            token = strs.get();
        }
    }
    catch (...)
    {

    }

    m_pTile = (tile *)new tile[m_sSizeX * m_sSizeY];

    for (int iy = 0; iy < m_sSizeY; iy++)
        for (int ix = 0; ix < m_sSizeX; ix++)
        {
            istrm.read(cTemp, m_sTileDataSize);
            if (!istrm.good())
                return false;
            pTile = (tile *)(m_pTile + ix + iy * m_sSizeY);
            if ((cTemp[8] & 0x80) != 0)
            {
                pTile->move_allowed = false;
            }
            else pTile->move_allowed = true;

            if ((cTemp[8] & 0x40) != 0)
            {
                pTile->teleport = true;
            }
            else pTile->teleport = false;

            if ((cTemp[8] & 0x20) != 0)
            {
                pTile->farming_allowed = true;
            }
            else pTile->farming_allowed = false;

            sp = (int16_t *)&cTemp[0];
            if (*sp == 19)
            {
                pTile->water = true;
            }
            else pTile->water = false;
        }

    if (istrm.is_open())
        istrm.close();

    return true;
}


bool map::bSearchTeleportDest(int32_t sX, int32_t sY, std::string & pMapName, int32_t * pDx, int32_t * pDy, char * pDir)
{
    for (auto & teleport : m_pTeleportLoc)
        if ((teleport != nullptr) && (teleport->source_x == sX) && (teleport->source_y == sY))
        {
            pMapName = teleport->dest_map_name;
            *pDx = teleport->dest_x;
            *pDy = teleport->dest_y;
            *pDir = teleport->direction;
            return true;
        }

    return false;
}

void map::SetDynamicObject(uint16_t wID, int16_t sType, int16_t sX, int16_t sY, uint32_t dwTime)
{
    tile * pTile;

    if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return;

    pTile = (tile *)(m_pTile + sX + sY * m_sSizeY);

    pTile->m_wDynamicObjectID = wID;
    pTile->m_sDynamicObjectType = sType;
    pTile->m_dwDynamicObjectTime = dwTime;
}

bool map::bGetDynamicObject(int16_t  sX, int16_t  sY, int16_t * pType, uint32_t * pTime, int32_t * pIndex)
{
    tile * pTile;

    if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return false;

    pTile = (tile *)(m_pTile + sX + sY * m_sSizeY);

    *pType = pTile->m_sDynamicObjectType;
    *pTime = pTile->m_dwDynamicObjectTime;
    if (pIndex != nullptr) *pIndex = pTile->m_wDynamicObjectID;

    return true;
}

int32_t map::iGetEmptyNamingValue()
{
    for (int32_t i = 0; i < 1000; i++)
        if (m_bNamingValueUsingStatus[i] == false)
        {

            m_bNamingValueUsingStatus[i] = true;
            return i;
        }

    return -1;
}

void map::SetNamingValueEmpty(int32_t iValue)
{
    m_bNamingValueUsingStatus[iValue] = false;
}

bool map::bGetIsWater(int16_t  dX, int16_t  dY)
{
    tile * pTile;

    if ((dX < 14) || (dX >= m_sSizeX - 16) || (dY < 12) || (dY >= m_sSizeY - 14)) return false;

    pTile = (tile *)(m_pTile + dX + dY * m_sSizeY);

    if (pTile->water == false) return false;

    return true;
}

bool map::bGetIsFarm(int16_t  dX, int16_t  dY)
{
    tile * pTile;

    if ((dX < 14) || (dX >= m_sSizeX - 16) || (dY < 12) || (dY >= m_sSizeY - 14)) return false;

    pTile = (tile *)(m_pTile + dX + dY * m_sSizeY);

    if (pTile->farming_allowed == false) return false;

    return true;
}

int32_t map::iAnalyze(char cType, int32_t * pX, int32_t * pY, int32_t * pV1, int32_t * pV2, int32_t * pV3)
{
    switch (cType)
    {
        case 1:

            break;


    }

    return 0;
}

void map::set_temp_move_allowed_flag(int32_t dX, int32_t dY, bool bFlag)
{
    tile * pTile;

    if ((dX < 20) || (dX >= m_sSizeX - 20) || (dY < 20) || (dY >= m_sSizeY - 20)) return;

    pTile = (tile *)(m_pTile + dX + dY * m_sSizeY);
    pTile->temp_move_allowed = bFlag;
}

int32_t map::get_occupy_flag(int32_t dX, int32_t dY, int8_t iSide, int32_t iEKNum, int32_t iDOI)
{
    if ((dX < 20) || (dX >= m_sSizeX - 20) || (dY < 20) || (dY >= m_sSizeY - 20)) return -1;

    for (int32_t i = 1; i < DEF_MAXOCCUPYFLAG; i++)
        if (m_pOccupyFlag[i] == nullptr)
        {
            m_pOccupyFlag[i] = new occupy_flag(dX, dY, iSide, iEKNum, iDOI);
            if (m_pOccupyFlag == nullptr) return -1;
            else return i;
        }

    return -1;
}



void map::clear_sector_info()
{
    for (int32_t ix = 0; ix < DEF_MAXSECTORS; ix++)
        for (int32_t iy = 0; iy < DEF_MAXSECTORS; iy++)
            sector_info[ix][iy] = sector_info_st();
}

void map::clear_temp_sector_info()
{
    for (int ix = 0; ix < DEF_MAXSECTORS; ix++)
        for (int iy = 0; iy < DEF_MAXSECTORS; iy++)
            temp_sector_info[ix][iy] = sector_info_st();
}

void map::setup_no_attack_area()
{
    tile * pTile;

    for (int32_t i = 0; i < DEF_MAXNMR; i++)
    {
        if ((m_rcNoAttackRect[i].top > 0))
        {
            for (int32_t ix = m_rcNoAttackRect[i].left; ix <= m_rcNoAttackRect[i].right; ix++)
                for (int32_t iy = m_rcNoAttackRect[i].top; iy <= m_rcNoAttackRect[i].bottom; iy++)
                {
                    pTile = (tile *)(m_pTile + ix + iy * m_sSizeY);
                    pTile->attribute = pTile->attribute | 0x00000004;
                }
        }
        else if (m_rcNoAttackRect[i].top == -10)
        {
            for (int32_t ix = 0; ix < m_sSizeX; ix++)
                for (int32_t iy = 0; iy < m_sSizeY; iy++)
                {
                    pTile = (tile *)(m_pTile + ix + iy * m_sSizeY);
                    pTile->attribute = pTile->attribute | 0x00000004;
                }
        }
    }
}

int32_t map::get_attribute(int32_t dX, int32_t dY, int32_t iBitMask)
{
    tile * pTile;

    if ((dX < 20) || (dX >= m_sSizeX - 20) || (dY < 20) || (dY >= m_sSizeY - 20)) return -1;

    pTile = (tile *)(m_pTile + dX + dY * m_sSizeY);
    return (pTile->attribute & iBitMask);
}

bool map::add_crusade_structure_info(char cType, int16_t  sX, int16_t  sY, char cSide)
{
    for (auto & structure : crusade_structure_info)
        if (structure.type == 0)
        {
            structure.type = cType;
            structure.side = cSide;
            structure.x = sX;
            structure.y = sY;

            m_iTotalCrusadeStructures++;
            return true;
        }

    return false;
}

bool map::remove_crusade_structure_info(int16_t  sX, int16_t  sY)
{
    for (auto & structure : crusade_structure_info)
        if ((structure.x == sX) && (structure.y == sY))
        {
            structure.type = 0;
            structure.side = 0;
            structure.x = 0;
            structure.y = 0;
            goto RCSI_REARRANGE;
        }

    return false;

RCSI_REARRANGE:;

    for (int32_t i = 0; i < DEF_MAXCRUSADESTRUCTURES - 1; i++)
        if ((crusade_structure_info[i].type == 0) && (crusade_structure_info[i + 1].type != 0))
        {
            crusade_structure_info[i].type = crusade_structure_info[i + 1].type;
            crusade_structure_info[i].side = crusade_structure_info[i + 1].side;
            crusade_structure_info[i].x = crusade_structure_info[i + 1].x;
            crusade_structure_info[i].y = crusade_structure_info[i + 1].y;

            crusade_structure_info[i + 1].type = 0;
            crusade_structure_info[i + 1].side = 0;
            crusade_structure_info[i + 1].x = 0;
            crusade_structure_info[i + 1].y = 0;
        }

    m_iTotalCrusadeStructures--;
    return true;
}


bool map::bAddCropsTotalSum()
{
    if (m_iTotalAgriculture < DEF_MAXAGRICULTURE)
    {
        m_iTotalAgriculture++;
        return true;
    }
    return false;
}

bool map::bRemoveCropsTotalSum()
{
    if (m_iTotalAgriculture < DEF_MAXAGRICULTURE)
    {
        m_iTotalAgriculture--;
        if (m_iTotalAgriculture < 0)
        {
            m_iTotalAgriculture = 0;
        }
        return true;
    }
    return false;
}

void map::restore_strike_points()
{
    for (auto & strike_point : strike_point)
    {
        strike_point.hp = strike_point.init_hp;
    }
}

}
