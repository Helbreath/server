
#include <fstream>
#include "map.hpp"
#include "server.hpp"
#include "gserver.hpp"
#include "npc.hpp"
#include "client.hpp"
#include "unit.hpp"
#include "guild.hpp"
#include "defines.hpp"
#include "delay_event.hpp"
#include "strategic_point.hpp"
#include <chrono>
#include <thread>
#include <string>
#include <string.h>
#include <nlohmann/json.hpp>

namespace hbx
{

using namespace std::chrono_literals;

map::map(gserver * gs_)
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











    npchandle = 0x100000000;//base handle value of 4,294,967,296 to 'guarantee' uniqueness between clients and npcs 

    TimerThreadRunning = false;
    sizeX = sizeY = 0;

    totalActiveObject = 0;
    totalAliveObject = 0;

    flags.apocalypseMap = false;
    apocalypseMobGenType = AMGT_NONE;
    apocalypseBossMobNpcID = 0;

    dynamicGateType = 0;

    dynamicGateCoordRectX1 = -2;
    dynamicGateCoordRectY1 = -2;
    dynamicGateCoordRectX2 = -2;
    dynamicGateCoordRectY2 = -2;
    dynamicGateCoordTgtX = 0;
    dynamicGateCoordTgtY = 0;

    //m_sInitialPointX = 0;
    //m_sInitialPointY = 0;

    fixedDay = false;
    fixedSnow = false;

    totalFishPoints = 0;
    fishMax = 0;
    fishCurrent = 0;

    mineralTotalPoint = 0;
    mineralMax = 0;
    mineralCurrent = 0;

    m_pTile = nullptr;

    weather = WEATHER_SUNNY;
    type = MAPTYPE_NORMAL;

    gs = gs_;

    levelLimitLower = 0;
    levelLimitUpper = 0;
    flags.mineralGenerator = false;

    occupyFlagTotal = 0;

    flags.attackEnabled = true;
    mobGenLevel = 0;

    for (int i = 0; i < MAXMAGICTYPE; i++)
        flags.magicLimited[i] = false;

    flags.partyDisabled = false;
    flags.shieldDisabled = false;
    flags.armorDisabled = false;
    flags.chatDisabled = false;
    flags.permIllusionOn = false;

    flags.fightZone = false;

    chatZone = 0;

    m_iMaxNx = m_iMaxNy = m_iMaxAx = m_iMaxAy = m_iMaxEx = m_iMaxEy = m_iMaxMx = m_iMaxMy = m_iMaxPx = m_iMaxPy = 0;

    flags.disabled = false;

    flags.heldenianMap = false;

    flags.randomMobGenerator = false;

    mobEventAmount = 15;

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

    if (actionthread)
    {
        actionthread->join();
        delete actionthread;
    }

    if (timerthread)
    {
        timerthread->join();
        delete timerthread;
    }
}

void map::run()
{
    TimerThreadRunning = true;
    actionthread = new std::thread(std::bind(&map::SocketThread, this));
    timerthread = new std::thread(std::bind(&map::TimerThread, this));
}

void map::put_msg_queue(message_entry * msg)
{
    std::unique_lock<std::mutex> lock(mutaction);
    actionpipe.push(std::make_unique<message_entry>(*msg));
}

void map::handle_message(std::shared_ptr<client> client_, stream_read & sr)
{
    stream_write sw;
    sr.position = 0;
    /*message_id*/ tagMSGID msgid = sr.read_enum();

    uint32_t cmd = sr.read_int16();

    gs->log->info("Packet processing - 0x{:X} - 0x{:X}", msgid, cmd);

    client_->last_packet_time = utility::current_time();

    if (msgid & MSGIDTYPE_MOTION)
    {
        gs->ClientMotionHandler(client_, sr);
    }
    else if (msgid == MSGID_COMMAND_COMMON)
    {
        //this first as it will be the most often sent packet type
        try
        {
            gs->ClientCommonHandler(client_, sr);
        }
        catch (int test)
        {
            //-193 data length error
            gs->log->error("Error: {}", test);
        }
    }
    else switch (msgid)
    {
        case MSGID_REQUEST_SETITEMPOS:
            break;

        case MSGID_REQUEST_TELEPORT:
            gs->RequestTeleportHandler(client_, 4);
            break;

        case MSGID_REQUEST_INITPLAYER:

            if (!client_->m_bIsInitComplete)
            {
                gs->DeleteClient(client_, false, true);
                return;
            }
//            gs->InitPlayerData(client_);
            gs->handle_initdata(client_, sr);
            break;

        case MSGID_REQUEST_INITDATA:
            return;

        case MSGID_REQUEST_RESTART:
            gs->RequestRestartHandler(client_);
            break;

        case MSGID_REQUEST_FULLOBJECTDATA:
        {
            uint32_t cmd = sr.read_int16();

//             std::shared_ptr<unit> target = client_->map_->GetOwner(dX, dY);
//             if (target)
//             {
//                 if (target->is_player())
//                     gs->handle_fullobjectdata(client_, target);
//             }
        }
        break;

        case MSGID_COMMAND_CHECKCONNECTION:
        {
            sw.write_int32(MSGID_COMMAND_CHECKCONNECTION);
            sw.write_int16(MSGTYPE_CONFIRM);
            client_->last_ping_time = utility::current_time();
            int64_t t = sr.read_int64();
            sw.write_int64(t);
            client_->write(sw);
            return;
        }

        default:
            gs->log->error("Unknown packet received from client - 0x{:X}", msgid);
            //DeleteClient(msg->client, false, true);
            break;
    }
}

void map::SocketThread()
{
    timespec req = { 0 };
    req.tv_sec = 0;
    req.tv_nsec = 1000000L;//1ms

    std::unique_lock<std::mutex> lock(mutaction, std::defer_lock);

    while (gs->get_status() == server_status::online)
    {
        if (!actionpipe.empty())
        {
            lock.lock();
            message_entry * pmsg = actionpipe.front().get();

            stream_read sr(pmsg->data, pmsg->size, true);
            stream_write sw;

            std::shared_ptr<client> client_ = pmsg->client_;
            pmsg = nullptr;

            actionpipe.pop();
            lock.unlock();


            uint32_t msgid = sr.read_int32();
            uint32_t cmd = sr.read_int16();

            gs->log->info("Packet processing - 0x{:X} - 0x{:X}", msgid, cmd);

            try
            {
                if (msgid & MSGIDTYPE_MOTION)
                {
                    gs->ClientMotionHandler(client_, sr);
                }
                else if (msgid == MSGID_COMMAND_COMMON)
                {
                    //this first as it will be the most often sent packet type
                    try
                    {
                        gs->ClientCommonHandler(client_, sr);
                    }
                    catch (int test)
                    {
                        //-193 data length error
                        gs->log->error("Error: {}", test);
                    }
                }
                else switch (msgid)
                {
                    case MSGID_REQUEST_SETITEMPOS:
                        break;

                    case MSGID_REQUEST_TELEPORT:
                        gs->RequestTeleportHandler(client_, 4);
                        break;

                    case MSGID_REQUEST_INITPLAYER:

                        if (!client_->m_bIsInitComplete)
                        {
                            gs->DeleteClient(client_, false, true);
                            continue;
                        }
                        gs->InitPlayerData(client_);
                        break;

                    case MSGID_REQUEST_RESTART:
                        gs->RequestRestartHandler(client_);
                        break;

                    default:
                        gs->log->error("Unknown packet received from client - 0x%.4X", msgid);
                        //DeleteClient(msg->client, false, true);
                        break;
                }
            }
            catch (int a)
            {
                gs->log->error("({}) Invalid packet received from client - 0x{:X} X:0x{:X} - {}", a, msgid, cmd, sr.size);
            }
        }

        std::this_thread::sleep_for(1ms);
    }
    gs->log->info("ActionThread() complete.");
    //mysql_thread_end();
}

void map::TimerThread()
{
//     struct timespec req = { 0 };
//     req.tv_sec = 0;
//     req.tv_nsec = 1000000L;//1ms
// 
//     TimerThreadRunning = true;
// 
//     uint64_t t1htimer;
//     uint64_t t30mintimer;
//     uint64_t t6mintimer;
//     uint64_t t5mintimer;
//     uint64_t t3mintimer;
//     uint64_t t1mintimer;
//     uint64_t t30sectimer;
//     uint64_t t5sectimer;
//     uint64_t t1sectimer;
//     uint64_t t100msectimer;
//     uint64_t ltime;
// 
//     t1htimer = t30mintimer = t6mintimer = t5mintimer = t3mintimer = t1mintimer = t30sectimer = t5sectimer = t1sectimer = t100msectimer = utility::unixtime();
// 
//     while (gs->get_status() == server_status::online)
//     {
//         try
//         {
//             ltime = ltime = unixtime();
// 
//             if (t100msectimer < ltime)
//             {
//                 NpcProcess();
//                 DelayEventProcessor();
// 
//                 t100msectimer += 100;
//             }
//             if (t1sectimer < ltime)
//             {
//                 t1sectimer += 1000;
//             }
//             if (t5sectimer < ltime)
//             {
//                 //test code - keep this until beta?
// 
//                 //regenerates stamina at a drastically increased rate
//                 {
//                     std::unique_lock<std::mutex> l(gs->server_.cl_m);
//                     // std::shared_lock_guard<std::shared_mutex> lock(Gate::GetSingleton().mutclientlist);
//                     for (const std::shared_ptr<client> & client : gs->server_.clients)
//                     {
//                         if (client->map_ == this)
//                         {
//                             client->stamina = client->GetStr() + 17;
//                             gs->SendNotifyMsg(0, client, NOTIFY_SP);
//                         }
//                     }
//                 }
// 
//                 //check connections for recent data (ghost sockets)
//                 //                 for (shared_ptr<Client> client : clientlist)
//                 //                 {
//                 //                     if (client->disconnecttime == 0 && client->lastpackettime + 30000 < ltime)
//                 //                     {
//                 //                         //socket idle for 30 seconds (should never happen unless disconnected)
//                 //                         poco_information(*logger, Poco::format("Client Timeout! <%s>", client->socket->address));
//                 //                         gate->stop(client->socket);
//                 //                         //when to delete client object...... ?
//                 //                         //force a ~10 second delay on object deletion to prevent logout hacking/plug pulling/etc? or destroy instantly?
//                 //                     }
//                 //                 }
//                 t5sectimer += 5000;
//             }
//             if (t30sectimer < ltime)
//             {
//                 t30sectimer += 30000;
//             }
//             if (t1mintimer < ltime)
//             {
//                 t1mintimer += 60000;
//             }
//             if (t3mintimer < ltime)
//             {
//                 t3mintimer += 180000;
//             }
//             if (t6mintimer < ltime)
//             {
//                 t6mintimer += 360000;
//             }
//             if (t1htimer < ltime)
//             {
// 
//                 t1htimer += 3600000;
//             }
//             std::this_thread::sleep_for(1ms);
//         }
//         catch (Poco::Data::MySQL::MySQLException * e)
//         {
//             gs->log->critical("TimerThread() SQL Exception: {}", e->displayText());
//         }
//         catch (std::system_error & e)
//         {
//             gs->log->critical("Map::TimerThread() std::system_error: {}", e.what());
//         }
//         catch (std::exception & e)
//         {
//             gs->log->critical("Map::TimerThread() std::Exception: {}", e.what());
//         }
//         catch (...)
//         {
//             gs->log->critical("Unspecified Map::TimerThread() Exception.");
//         }
//     }
//     TimerThreadRunning = false;
//     gs->log->information("TimerThread() complete.");
// 
//     //mysql_thread_end();
}

void map::SetOwner(std::shared_ptr<unit> sOwner, short sX, short sY)
{
    tile * pTile;

    if ((sX < 0) || (sX >= sizeX) || (sY < 0) || (sY >= sizeY)) return;

    pTile = (tile *)(m_pTile + sX + sY * sizeY);
    pTile->owner = sOwner;
    pTile->m_cOwnerType = sOwner->is_player() ? ot_player : ot_npc;
}

void map::SetDeadOwner(std::shared_ptr<unit> sOwner, short sX, short sY)
{
    tile * pTile;

    if ((sX < 0) || (sX >= sizeX) || (sY < 0) || (sY >= sizeY)) return;

    pTile = (tile *)(m_pTile + sX + sY * sizeY);
    pTile->deadowner = sOwner;
    pTile->m_cDeadOwnerType = sOwner->is_player() ? ot_player : ot_npc;
}

std::shared_ptr<unit> map::GetOwner(short sX, short sY) const
{
    tile * pTile;

    if ((sX < 0) || (sX >= sizeX) || (sY < 0) || (sY >= sizeY)) {
        return nullptr;
    }

    pTile = (tile *)(m_pTile + sX + sY * sizeY);
    return pTile->owner;

}

std::shared_ptr<unit> map::GetDeadOwner(short sX, short sY) const
{
    tile * pTile;

    if ((sX < 0) || (sX >= sizeX) || (sY < 0) || (sY >= sizeY)) {
        return nullptr;
    }

    pTile = (tile *)(m_pTile + sX + sY * sizeY);
    return pTile->deadowner;

}

std::list<std::shared_ptr<unit>> map::GetOwners(short x1, short x2, short y1, short y2) const
{
    tile * pTile;
    //TODO: this. shoot whoever wrote it and rewrite it before an alpha test
    //return 0;
    std::list<std::shared_ptr<unit>> owners;

    if (x1 < 0)
        x1 = 0;

    if (x2 >= sizeX)
        x2 = sizeX - 1;

    if (y1 < 0)
        y1 = 0;

    if (y2 >= sizeY)
        y2 = sizeY - 1;

    auto n = 0;

    for (int y = y1; y <= y2; y++)
    {
        for (int x = x1; x <= x2; x++)
        {
            pTile = static_cast<tile *>(m_pTile + x + y * sizeY);

            switch (pTile->m_cOwnerType)
            {
                case ot_player:
                    owners.push_back(pTile->owner);
                    n++;
                    break;;

                case ot_npc:
                    owners.push_back(pTile->owner);
                    n++;
                    break;
                default:
                    //owners.push_back(nullptr);
                    //n++;
                    break;
            }
        }
    }

    return owners;
}

bool map::bGetMoveable(short dX, short dY, short * pDOtype, item * pTopItem) const
{
    tile * pTile;

    if ((dX < 20) || (dX >= sizeX - 20) || (dY < 20) || (dY >= sizeY - 20)) return false;

    pTile = (tile *)(m_pTile + dX + dY * sizeY);

    if (pDOtype != nullptr) *pDOtype = pTile->m_sDynamicObjectType;
    if ((pTopItem != nullptr) && (!pTile->m_pItem.empty())) pTopItem = pTile->m_pItem.front();

    if (pTile->owner != nullptr) return false;
    if (pTile->IsMovable() == false) return false;
    if (pTile->IsTempMovable() == false) return false;

    return true;
}

bool map::bGetIsMoveAllowedTile(short dX, short dY) const
{
    if ((dX < 20) || (dX >= sizeX - 20) || (dY < 20) || (dY >= sizeY - 20)) return false;

    auto pTile = static_cast<tile *>(m_pTile + dX + dY * sizeY);

    if (pTile->IsMovable() == false) return false;
    if (pTile->IsTempMovable() == false) return false;

    return true;
}

bool map::bGetIsMoveAllowedTile(Point p) const
{
    if (p.x < 20 || p.x >= sizeX - 20
        || p.y < 20 || p.y >= sizeY - 20
        ) return false;

    auto pTile = static_cast<tile *>(m_pTile + p.x + p.y * sizeY);

    if (!pTile->IsMovable() || !pTile->IsTempMovable())
        return false;

    return true;
}

bool map::bGetIsTeleport(short dX, short dY) const
{
    if ((dX < 14) || (dX >= sizeX - 16) || (dY < 12) || (dY >= sizeY - 14)) return false;

    auto pTile = static_cast<tile *>(m_pTile + dX + dY * sizeY);

    if (pTile->IsTele() == false) return false;

    return true;
}

void map::ClearOwner(short sX, short sY) const
{
    if ((sX < 0) || (sX >= sizeX) || (sY < 0) || (sY >= sizeY)) return;

    auto pTile = static_cast<tile *>(m_pTile + sX + sY * sizeY);
    pTile->owner = nullptr;
    pTile->m_cOwnerType = ot_none;
}

void map::ClearDeadOwner(short sX, short sY) const
{
    if ((sX < 0) || (sX >= sizeX) || (sY < 0) || (sY >= sizeY)) return;

    auto pTile = static_cast<tile *>(m_pTile + sX + sY * sizeY);
    pTile->deadowner = nullptr;
    pTile->m_cDeadOwnerType = ot_none;
}

bool map::bSetItem(short sX, short sY, item * pItem) const
{
    if ((sX < 0) || (sX >= sizeX) || (sY < 0) || (sY >= sizeY)) return false;

    auto pTile = static_cast<tile *>(m_pTile + sX + sY * sizeY);

    if (!pTile->m_pItem.empty())
    {
        if (pTile->m_pItem.front()->m_sIDnum == ITEM_RELIC) return false;
    }
    pTile->m_pItem.push_back(pItem);

    //    if(pTile->m_pItem[0] && pTile->m_pItem[0]->m_sIDnum == ITEM_RELIC) return false;

    //     if (pTile->m_pItem[ITEMS_PER_TILE-1] != NULL) 
    //         delete pTile->m_pItem[ITEMS_PER_TILE-1];
    //     else pTile->m_cTotalItem++;

    //     for (i = ITEMS_PER_TILE-2; i >= 0; i--) 
    //         pTile->m_pItem[i+1] = pTile->m_pItem[i];

    //    pTile->m_pItem[0] = pItem;
        //pTile->m_cTotalItem++;
    return true;
}


item * map::pGetItem(short sX, short sY, short * pRemainItemSprite, short * pRemainItemSpriteFrame, uint32_t * pRemainItemColor) const
{
    if ((sX < 0) || (sX >= sizeX) || (sY < 0) || (sY >= sizeY)) return 0;

    auto pTile = static_cast<tile *>(m_pTile + sX + sY * sizeY);
    if (pTile->m_pItem.empty()) return nullptr;
    auto pItem = pTile->m_pItem.front();
    pTile->m_pItem.erase(pTile->m_pItem.begin());

    if (pTile->m_pItem.empty())
    {
        *pRemainItemSprite = 0;
        *pRemainItemSpriteFrame = 0;
        *pRemainItemColor = 0;
    }
    else
    {
        *pRemainItemSprite = pItem->m_sSprite;
        *pRemainItemSpriteFrame = pItem->m_sSpriteFrame;
        *pRemainItemColor = pItem->m_cItemColor;
    }

    return pItem;
}


uint64_t map::iCheckItem(short sX, short sY) const
{
    if ((sX < 0) || (sX >= sizeX) || (sY < 0) || (sY >= sizeY)) return 0;

    auto pTile = static_cast<tile *>(m_pTile + sX + sY * sizeY);
    if (pTile->m_pItem.empty()) return 0;
    auto pItem = pTile->m_pItem.front();

    return pItem->m_sIDnum;
}


bool map::bIsValidLoc(short sX, short sY) const
{
    if ((sX < 0) || (sX >= sizeX) || (sY < 0) || (sY >= sizeY)) return false;
    return true;
}

bool map::bInit(string pName)
{
    name = pName;

    if (_bDecodeMapDataFileContents() == false)
        return false;

    if (bDecodeMapConfig() == false)
        return false;

    return true;
}

/*
bool map::bDecodeMapConfig()
{
    std::string mapfile = fmt::format("mapdata/{}.json", name);

    using namespace nlohmann;
    
    json obj;
   
    std::ifstream config_file(mapfile);
    config_file >> obj;

    gs->log->info("Loading {}", mapfile);








    lua_State * L = gserver->L;
    try
    {
        if (luaL_dofile(L, mapfile.c_str()) != 0)
        {
            gserver->logger->fatal(Poco::format("%s", (string)lua_tostring(L, -1)));
            return false;
        }
        lua_getglobal(L, "mapdata");

        if (lua_istable(L, -1))
        {
            lua_pushnil(L);
            while (lua_next(L, -2))
            {
                string option = (char *)lua_tostring(L, -2);
                if (option == "teleportloc")
                {
                    if (lua_istable(L, -1))
                    {
                        auto tableSize = uint8_t(lua_rawlen(L, -1));
                        for (uint8_t i = 1; i <= tableSize; ++i)
                        {
                            lua_pushinteger(L, i);
                            lua_gettable(L, -2);

                            lua_pushinteger(L, 1);
                            lua_gettable(L, -2);
                            auto sX = uint16_t(lua_tointeger(L, -1));
                            lua_pop(L, 1);
                            lua_pushinteger(L, 2);
                            lua_gettable(L, -2);
                            auto sY = uint16_t(lua_tointeger(L, -1));
                            lua_pop(L, 1);
                            lua_pushinteger(L, 3);
                            lua_gettable(L, -2);
                            string dest = lua_tostring(L, -1);
                            lua_pop(L, 1);
                            lua_pushinteger(L, 4);
                            lua_gettable(L, -2);
                            auto dX = uint16_t(lua_tointeger(L, -1));
                            lua_pop(L, 1);
                            lua_pushinteger(L, 5);
                            lua_gettable(L, -2);
                            auto dY = uint16_t(lua_tointeger(L, -1));
                            lua_pop(L, 1);
                            lua_pushinteger(L, 6);
                            lua_gettable(L, -2);
                            auto dir = uint8_t(lua_tointeger(L, -1));
                            lua_pop(L, 1);

                            auto loc = new TeleportLoc();
                            loc->m_sSrcX = sX;
                            loc->m_sSrcY = sY;
                            loc->m_sDestX = dX;
                            loc->m_sDestY = dY;
                            loc->m_cDir = dir;
                            loc->m_cDestMapName = dest;

                            teleportLocationList.push_back(loc);

                            lua_pop(L, 1);
                        }
                    }
                }
                else if (option == "waypoint")
                {
                    if (lua_istable(L, -1))
                    {
                        auto tableSize = uint8_t(lua_rawlen(L, -1));
                        for (uint8_t i = 1; i <= tableSize; ++i)
                        {
                            lua_pushinteger(L, i);
                            lua_gettable(L, -2);

                            lua_pushinteger(L, 1);
                            lua_gettable(L, -2);
                            auto sX = uint16_t(lua_tointeger(L, -1));
                            lua_pop(L, 1);
                            lua_pushinteger(L, 2);
                            lua_gettable(L, -2);
                            auto sY = uint16_t(lua_tointeger(L, -1));
                            lua_pop(L, 1);

                            waypointList.push_back(point(sX, sY));

                            lua_pop(L, 1);
                        }
                    }
                }
                else if (option == "npc")
                {

                }
                else if (option == "randommobgenerator")
                {
                    lua_pushinteger(L, 1);
                    lua_gettable(L, -2);
                    auto gen = uint16_t(lua_tointeger(L, -1));
                    lua_pop(L, 1);
                    lua_pushinteger(L, 2);
                    lua_gettable(L, -2);
                    auto level = uint16_t(lua_tointeger(L, -1));
                    lua_pop(L, 1);

                    flags.randomMobGenerator = (gen > 0) ? true : false;
                    mobGenLevel = level;
                }
                else if (option == "maximumobject")
                {
                    maximumObject = uint16_t(lua_tointeger(L, -1));
                }
                else if (option == "npcavoidrect")
                {

                }
                else if (option == "spotmobgenerator")
                {

                }
                else if (option == "name")
                {
                    this->factionName = lua_tostring(L, -1);
                }
                else if (option == "initialpoint")
                {
                    if (lua_istable(L, -1))
                    {
                        auto tableSize = uint8_t(lua_rawlen(L, -1));
                        for (uint8_t i = 1; i <= tableSize; ++i)
                        {
                            lua_pushinteger(L, i);
                            lua_gettable(L, -2);

                            lua_pushinteger(L, 1);
                            lua_gettable(L, -2);
                            auto sX = uint16_t(lua_tointeger(L, -1));
                            lua_pop(L, 1);
                            lua_pushinteger(L, 2);
                            lua_gettable(L, -2);
                            auto sY = uint16_t(lua_tointeger(L, -1));
                            lua_pop(L, 1);

                            initialPointList.push_back(point(sX, sY));

                            lua_pop(L, 1);
                        }
                    }
                }
                else if (option == "noattackarea")
                {
                    if (lua_istable(L, -1))
                    {
                        auto tableSize = uint8_t(lua_rawlen(L, -1));
                        for (auto i = 1; i <= tableSize; ++i)
                        {
                            lua_pushinteger(L, i);
                            lua_gettable(L, -2);

                            lua_pushinteger(L, 1);
                            lua_gettable(L, -2);
                            auto sX = uint16_t(lua_tointeger(L, -1));
                            lua_pop(L, 1);
                            lua_pushinteger(L, 2);
                            lua_gettable(L, -2);
                            auto sY = uint16_t(lua_tointeger(L, -1));
                            lua_pop(L, 1);
                            lua_pushinteger(L, 3);
                            lua_gettable(L, -2);
                            auto dX = uint16_t(lua_tointeger(L, -1));
                            lua_pop(L, 1);
                            lua_pushinteger(L, 4);
                            lua_gettable(L, -2);
                            auto dY = uint16_t(lua_tointeger(L, -1));
                            lua_pop(L, 1);

                            safeZoneList.push_back(rect(sX, sY, dX, dY));

                            lua_pop(L, 1);
                        }
                    }
                }
                else if (option == "fixeddayornightmode")
                {

                }
                else if (option == "fishpoint")
                {

                }
                else if (option == "maxfish")
                {
                    fishMax = (uint32_t)lua_tointeger(L, -1);
                }
                else if (option == "type")
                {

                }
                else if (option == "levellimit")
                {
                    levelLimitLower = (uint16_t)lua_tointeger(L, -1);
                }
                else if (option == "mineralgenerator")
                {

                }
                else if (option == "mineralpoint")
                {

                }
                else if (option == "maxmineral")
                {
                    mineralMax = (uint16_t)lua_tointeger(L, -1);
                }
                else if (option == "upperlevellimit")
                {
                    levelLimitUpper = (uint16_t)lua_tointeger(L, -1);
                }
                else if (option == "strategicpoint")
                {

                }
                else if (option == "energyspherecreationpoint")
                {

                }
                else if (option == "energyspheregoalpoint")
                {

                }
                else if (option == "strikepoint")
                {

                }
                else if (option == "mobeventamount")
                {

                }
                else if (option == "ApocalypseMobGenType")
                {

                }
                else if (option == "ApocalypseBossMob")
                {

                }
                else if (option == "DynamicGateType")
                {

                }
                else if (option == "DynamicGateCoord")
                {

                }
                else if (option == "ApocalypseMap")
                {

                }
                else if (option == "HeldenianMap")
                {

                }
                else if (option == "HeldenianTower")
                {

                }
                else if (option == "HeldenianModeMap")
                {

                }
                else if (option == "HeldenianWinningZone")
                {

                }
                else if (option == "HeldenianGateDoor")
                {

                }
                else if (option == "chatzone")
                {

                }
                else if (option == "itemevent")
                {

                }

                lua_pop(L, 1);
            }
        }
        lua_pop(L, 1);

        return true;
    }
    catch (std::exception & e)
    {
        gserver->logger->fatal(Poco::format("bDecodeMapConfig() Exception: %s", (string)e.what()));
        return false;
    }
    catch (...)
    {
        gserver->logger->fatal("Unspecified bDecodeMapConfig() Exception.");
        return false;
    }


    return true;
}*/

bool _bGetIsStringIsNumber(std::string pStr)
{
    for (int i = 0; i < pStr.length(); i++)
        if ((pStr[i] != '-') && ((pStr[i] < (char)'0') || (pStr[i] > (char)'9'))) return false;

    return true;

}
bool map::bDecodeMapConfig()
{
    char * pContents;
    std::string token;
    char seps[] = "= \t\n";
    char cReadModeA = 0;
    char cReadModeB = 0;
    int  iTeleportLocIndex = 0;
    int  iWayPointCfgIndex = 0;
    int  iTotalNpcSetting = 0;
    int  iMGARCfgIndex = 0;
    int  iSMGRCfgIndex = 0;
    int  iNMRCfgIndex = 0;
    int  iFishPointIndex = 0;
    int  iMineralPointIndex = 0;
    int  iStrategicPointIndex = 0;
    int  iIndex = 0;
    int  iNumMob = 0;
    uint32_t dwFileSize;


    std::string mapfile = fmt::format("mapdata/{}.txt", name);


    std::ifstream config_file(mapfile);

    if (!config_file.is_open())
    {
        gs->log->critical("Unable to open map config: {}", name);
        return false;
    }

    config_file.seekg(0, std::ios::end);
    dwFileSize = config_file.tellg();
    config_file.seekg(0, std::ios::beg);

    std::string cNpcName;
    char cNpcMoveType, cNpcWaypointIndex[10] = {};
    short sIPindex;
    //SYSTEMTIME SysTime;


    if (name == "fight")
        m_bIsFightZone = true;

    if (name == "icebound")
        m_bIsFixedSnowMode = true;

    return true;

//     GetLocalTime(&SysTime);
//     if (SysTime.wYear == 2002 && SysTime.wMonth == 12 && SysTime.wDay == 25)
//     {
//         m_bIsFixedSnowMode = true;
//     }

    pContents = new char[dwFileSize + 1];
    memset(pContents, 0, uint64_t(dwFileSize) + 1);

    config_file.read(pContents, dwFileSize);

    auto toks = utility::tokenizer(pContents, seps);
    token = toks.get();
    //token = strtok(pContents, seps);
    while (token.length())
    {
        if (cReadModeA != 0)
        {
            switch (cReadModeA)
            {
                case 1:
                    switch (cReadModeB)
                    {
                        case 1:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 1 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }

                            m_pTeleportLoc[iTeleportLocIndex]->source_x = std::stoi(token);
                            cReadModeB = 2;
                            break;

                        case 2:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 2 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }

                            m_pTeleportLoc[iTeleportLocIndex]->source_y = std::stoi(token);
                            cReadModeB = 3;
                            break;

                        case 3:
                            m_pTeleportLoc[iTeleportLocIndex]->dest_map_name = token;
                            cReadModeB = 4;
                            break;

                        case 4:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 3 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }

                            m_pTeleportLoc[iTeleportLocIndex]->dest_x = std::stoi(token);
                            cReadModeB = 5;
                            break;

                        case 5:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 4 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }

                            m_pTeleportLoc[iTeleportLocIndex]->dest_y = std::stoi(token);
                            cReadModeB = 6;
                            break;

                        case 6:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 5 - Wrong Data format.");
                                delete[] pContents;

                                return false;
                            }

                            m_pTeleportLoc[iTeleportLocIndex]->direction = std::stoi(token);
                            iTeleportLocIndex++;
                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 2:
                    switch (cReadModeB)
                    {
                        case 1:

                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 6 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            iWayPointCfgIndex = std::stoi(token);

                            if (m_WaypointList[iWayPointCfgIndex].x != -1)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 7 - Duplicated waypoint");
                                delete[] pContents;
                                return false;
                            }
                            cReadModeB = 2;
                            break;

                        case 2:

                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 8 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            m_WaypointList[iWayPointCfgIndex].x = std::stoi(token);
                            cReadModeB = 3;
                            break;

                        case 3:

                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 9 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            m_WaypointList[iWayPointCfgIndex].y = std::stoi(token);
                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 3:

                    switch (cReadModeB)
                    {
                        case 1:
                            cNpcName = token;
                            cReadModeB = 2;
                            break;
                        case 2:
                            // NpcMoveType
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 10 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            cNpcMoveType = std::stoi(token);
                            cReadModeB = 3;
                            break;
                        default:
                            // WayPoint0~waypoint9
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 11 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            cNpcWaypointIndex[cReadModeB - 3] = std::stoi(token);
                            cReadModeA = 0;
                            cReadModeB = 0;

                            if (!CreateNpc(cNpcName, 0, cNpcMoveType, NULL, NULL, (faction)3, cNpcWaypointIndex, NULL, NULL, false))
                            {
                                gs->log->critical("Failed to create default NPC!");
                                break;
                            }

                            iTotalNpcSetting++;
                            break;
                    }
                    break;

                case 4:
                    switch (cReadModeB)
                    {
                        case 1:

                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 12 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            m_bRandomMobGenerator = std::stoi(token) ? true : false;
                            cReadModeB = 2;
                            break;

                        case 2:
                            // Mob- Level
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 13 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            m_cRandomMobGeneratorLevel = std::stoi(token);
                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 5:
                    // Maximum object
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 14 - Wrong Data format.");
                        delete[] pContents;
                        return false;
                    }

                    m_iMaximumObject = std::stoi(token);
                    cReadModeA = 0;
                    cReadModeB = 0;
                    break;

                case 6:

                    switch (cReadModeB)
                    {
                        case 1:

                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 15 - Wrong Data format(MGAR num).");
                                delete[] pContents;
                                return false;
                            }
                            iMGARCfgIndex = std::stoi(token);

                            if (m_rcMobGenAvoidRect[iMGARCfgIndex].left != -1)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 16 - Duplicated Mob Gen Rect Number!");
                                delete[] pContents;
                                return false;
                            }

                            cReadModeB = 2;
                            break;

                        case 2:
                            // left
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 17 - Wrong Data format(MGAR num).");
                                delete[] pContents;
                                return false;
                            }
                            m_rcMobGenAvoidRect[iMGARCfgIndex].left = std::stoi(token);
                            cReadModeB = 3;
                            break;

                        case 3:
                            // top
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 18 - Wrong Data format(MGAR num).");
                                delete[] pContents;
                                return false;
                            }
                            m_rcMobGenAvoidRect[iMGARCfgIndex].top = std::stoi(token);
                            cReadModeB = 4;
                            break;

                        case 4:
                            // right
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 19 - Wrong Data format(MGAR num).");
                                delete[] pContents;
                                return false;
                            }
                            m_rcMobGenAvoidRect[iMGARCfgIndex].right = std::stoi(token);
                            cReadModeB = 5;
                            break;

                        case 5:
                            // bottom
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 20 - Wrong Data format(MGAR num).");
                                delete[] pContents;
                                return false;
                            }
                            m_rcMobGenAvoidRect[iMGARCfgIndex].bottom = std::stoi(token);
                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 7:

                    switch (cReadModeB)
                    {
                        case 1:

                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 21 - Wrong Data format(MGAR num).");
                                delete[] pContents;
                                return false;
                            }
                            iSMGRCfgIndex = std::stoi(token);

                            if (spotMobGenerator[iSMGRCfgIndex].defined == true)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error - ");
                                delete[] pContents;
                                return false;
                            }
                            cReadModeB = 2;
                            spotMobGenerator[iSMGRCfgIndex].defined = true;
                            break;

                        case 2:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 22 - Wrong Data format(SMGAR num).");
                                delete[] pContents;
                                return false;
                            }
                            spotMobGenerator[iSMGRCfgIndex].type = std::stoi(token);

                            if (spotMobGenerator[iSMGRCfgIndex].type == 1)
                                cReadModeB = 3;
                            else if (spotMobGenerator[iSMGRCfgIndex].type == 2)
                                cReadModeB = 9;
                            break;

                        case 3:
                            // left
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 23 - Wrong Data format(SMGAR num).");
                                delete[] pContents;
                                return false;
                            }
                            spotMobGenerator[iSMGRCfgIndex].rcRect.left = std::stoi(token);
                            cReadModeB = 4;
                            break;

                        case 4:
                            // top
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 24 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            spotMobGenerator[iSMGRCfgIndex].rcRect.top = std::stoi(token);
                            cReadModeB = 5;
                            break;

                        case 5:
                            // right
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 25 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            spotMobGenerator[iSMGRCfgIndex].rcRect.right = std::stoi(token);
                            cReadModeB = 6;
                            break;

                        case 6:
                            // bottom
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 26 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            spotMobGenerator[iSMGRCfgIndex].rcRect.bottom = std::stoi(token);
                            cReadModeB = 7;
                            break;

                        case 7:
                            // spot mob type
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 27 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            spotMobGenerator[iSMGRCfgIndex].genType = std::stoi(token);
                            cReadModeB = 8;
                            break;

                        case 8:
                            // Max Mobs
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 28 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            spotMobGenerator[iSMGRCfgIndex].max = std::stoi(token);
                            spotMobGenerator[iSMGRCfgIndex].current = 0;
                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;

                        default:
                            // WayPoint0~waypoint9
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 29 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            spotMobGenerator[iSMGRCfgIndex].waypoints[cReadModeB - 9] = std::stoi(token);
                            cReadModeB++;
                            break;

                        case 19:
                            // spot mob type
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 30 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            spotMobGenerator[iSMGRCfgIndex].genType = std::stoi(token);
                            cReadModeB = 20;
                            break;

                        case 20:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 31 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            spotMobGenerator[iSMGRCfgIndex].max = std::stoi(token);
                            spotMobGenerator[iSMGRCfgIndex].current = 0;
                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 8:
                    location_name = token;

                    /*if(!chatZone){
                        if( strcmp(m_pMapList[iMapIndex]->m_cLocationName, sideMap[ARESDEN]) == 0)
                            chatZone = ARESDEN;
                        else if( strcmp(m_pMapList[iMapIndex]->m_cLocationName, sideMap[ELVINE]) == 0)
                            chatZone = ELVINE;
                    }*/
                    cReadModeA = 0;
                    cReadModeB = 0;
                    break;

                case 9:
                    switch (cReadModeB)
                    {
                        case 1:
                            // Initial-Point Index
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 32:1 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            sIPindex = std::stoi(token);
                            if (m_pInitialPoint[sIPindex].x != -1)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Duplicate Initial Point Index!");
                                delete[] pContents;
                                return false;
                            }
                            cReadModeB = 2;
                            break;

                        case 2:
                            // Initial-Point X
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 32 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            m_pInitialPoint[sIPindex].x = std::stoi(token);
                            cReadModeB = 3;
                            break;

                        case 3:
                            // Initial-Point Y
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 33 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            m_pInitialPoint[sIPindex].y = std::stoi(token);
                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 10:

                    switch (cReadModeB)
                    {
                        case 1:
                            // 
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 34 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            iNMRCfgIndex = std::stoi(token);

                            if (m_rcNoAttackRect[iNMRCfgIndex].top != -1)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 35 - Duplicate No-Magic-Rect number");
                                delete[] pContents;
                                return false;
                            }
                            cReadModeB = 2;
                            break;

                        case 2:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 36 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            m_rcNoAttackRect[iNMRCfgIndex].left = std::stoi(token);
                            cReadModeB = 3;
                            break;

                        case 3:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 37 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            m_rcNoAttackRect[iNMRCfgIndex].top = std::stoi(token);
                            cReadModeB = 4;
                            break;

                        case 4:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 38 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            m_rcNoAttackRect[iNMRCfgIndex].right = std::stoi(token);
                            cReadModeB = 5;
                            break;

                        case 5:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 39 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            m_rcNoAttackRect[iNMRCfgIndex].bottom = std::stoi(token);
                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 11:
                    m_bIsFixedDayMode = std::stoi(token) ? true : false;


                    if (m_bIsFixedDayMode == true)
                        m_bIsFixedSnowMode = false;
                    cReadModeA = 0;
                    break;

                case 12:
                    switch (cReadModeB)
                    {
                        case 1:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 40 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            iFishPointIndex = std::stoi(token);

                            if (m_FishPointList[iFishPointIndex].x != -1)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 41 - Duplicate FishPoint number");
                                delete[] pContents;
                                return false;
                            }

                            m_iTotalFishPoint++;
                            cReadModeB = 2;
                            break;

                        case 2:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 42 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            m_FishPointList[iFishPointIndex].x = std::stoi(token);

                            cReadModeB = 3;
                            break;

                        case 3:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 43 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            m_FishPointList[iFishPointIndex].y = std::stoi(token);

                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 13:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 44 - Wrong Data format.");
                        delete[] pContents;
                        return false;
                    }
                    m_iMaxFish = std::stoi(token);

                    cReadModeA = 0;
                    break;

                case 14:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 45 - Wrong Data format.");
                        delete[] pContents;
                        return false;
                    }
                    m_cType = std::stoi(token);

                    cReadModeA = 0;
                    break;

                case 15:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 46 - Wrong Data format.");
                        delete[] pContents;
                        return false;
                    }
                    m_iLevelLimit = std::stoi(token);

                    cReadModeA = 0;
                    break;

                case 16:
                    switch (cReadModeB)
                    {
                        case 1:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 47 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            m_bMineralGenerator = std::stoi(token) ? true : false;
                            cReadModeB = 2;
                            break;

                        case 2:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 48 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            m_cMineralGeneratorLevel = std::stoi(token);
                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 17:
                    switch (cReadModeB)
                    {
                        case 1:
                            if (_bGetIsStringIsNumber(token) == false) {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 49 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            iMineralPointIndex = std::stoi(token);

                            if (m_MineralPointList[iMineralPointIndex].x != -1) {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 50 - Duplicate MineralPoint number");
                                delete[] pContents;
                                return false;
                            }

                            m_iTotalMineralPoint++;
                            cReadModeB = 2;
                            break;

                        case 2:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 51 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            m_MineralPointList[iMineralPointIndex].x = std::stoi(token);

                            cReadModeB = 3;
                            break;

                        case 3:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 52 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            m_MineralPointList[iMineralPointIndex].y = std::stoi(token);

                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 18:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 53 - Wrong Data format.");
                        delete[] pContents;
                        return false;
                    }
                    m_iMaxMineral = std::stoi(token);

                    cReadModeA = 0;
                    break;

                case 19:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 54 - Wrong Data format.");
                        delete[] pContents;
                        return false;
                    }
                    m_iUpperLevelLimit = std::stoi(token);

                    cReadModeA = 0;
                    break;

                case 20:
                    switch (cReadModeB)
                    {
                        case 1:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 55 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            iStrategicPointIndex = std::stoi(token);

                            if (m_pStrategicPointList[iStrategicPointIndex] != nullptr)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 56 - Duplicate Strategic Point number");
                                delete[] pContents;
                                return false;
                            }

                            m_pStrategicPointList[iStrategicPointIndex] = new strategic_point();
                            cReadModeB = 2;
                            break;

                        case 2:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 57 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            m_pStrategicPointList[iStrategicPointIndex]->side = std::stoi(token);

                            cReadModeB = 3;
                            break;

                        case 3:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 58 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            m_pStrategicPointList[iStrategicPointIndex]->value = std::stoi(token);

                            cReadModeB = 4;
                            break;

                        case 4:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 59 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            m_pStrategicPointList[iStrategicPointIndex]->x = std::stoi(token);

                            cReadModeB = 5;
                            break;

                        case 5:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 60 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            m_pStrategicPointList[iStrategicPointIndex]->y = std::stoi(token);

                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 21:
                    switch (cReadModeB)
                    {
                        case 1:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 61 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            iIndex = std::stoi(token);

                            if (energy_sphere_creation_list[iIndex].cType != NULL) {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 62 - Duplicate EnergySphereCreation number");
                                delete[] pContents;
                                return false;
                            }

                            m_iTotalEnergySphereCreationPoint++;
                            cReadModeB = 2;
                            break;

                        case 2:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 63 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            energy_sphere_creation_list[iIndex].cType = std::stoi(token);

                            cReadModeB = 3;
                            break;

                        case 3:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 64 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            energy_sphere_creation_list[iIndex].sX = std::stoi(token);

                            cReadModeB = 4;
                            break;

                        case 4:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 65 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            energy_sphere_creation_list[iIndex].sY = std::stoi(token);

                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 22:
//                     switch (cReadModeB)
//                     {
//                         case 1:
//                             if (_bGetIsStringIsNumber(token) == false)
//                             {
//                                 gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 69 - Wrong Data format.");
//                                 delete[] pContents;
//                                 return false;
//                             }
//                             m_stEnergySphereGoalList_aresdenX = std::stoi(token);
// 
//                             cReadModeB = 2;
//                             break;
// 
//                         case 2:
//                             if (_bGetIsStringIsNumber(token) == false)
//                             {
//                                 gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 70 - Wrong Data format.");
//                                 delete[] pContents;
//                                 return false;
//                             }
//                             m_stEnergySphereGoalList_aresdenY = std::stoi(token);
// 
//                             cReadModeB = 3;
//                             break;
// 
//                         case 3:
//                             if (_bGetIsStringIsNumber(token) == false)
//                             {
//                                 gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 71 - Wrong Data format.");
//                                 delete[] pContents;
//                                 return false;
//                             }
//                             m_stEnergySphereGoalList_elvineX = std::stoi(token);
// 
//                             cReadModeB = 4;
//                             break;
// 
//                         case 4:
//                             if (_bGetIsStringIsNumber(token) == false)
//                             {
//                                 gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 72 - Wrong Data format.");
//                                 delete[] pContents;
//                                 return false;
//                             }
//                             m_stEnergySphereGoalList_elvineY = std::stoi(token);
// 
//                             cReadModeA = 0;
//                             cReadModeB = 0;
//                             break;
//                     }
//                     break;
// 
//                 case 23:
//                     switch (cReadModeB)
//                     {
//                         case 1:
//                             if (_bGetIsStringIsNumber(token) == false)
//                             {
//                                 gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 73 - Wrong Data format.");
//                                 delete[] pContents;
//                                 return false;
//                             }
//                             iIndex = std::stoi(token);
// 
//                             if (strlen(strikePointList[iIndex].cRelatedMapName) != NULL)
//                             {
//                                 wsprintf(g_cTxt, "(!!!) CRITICAL ERROR! Map Info file error 74 - Duplicate Strike Point number(%d)", iIndex);
//                                 gs->log->critical(g_cTxt);
//                                 delete[] pContents;
//                                 return false;
//                             }
// 
//                             cReadModeB = 2;
//                             break;
// 
//                         case 2:
//                             if (_bGetIsStringIsNumber(token) == false)
//                             {
//                                 gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 75 - Wrong Data format.");
//                                 delete[] pContents;
//                                 return false;
//                             }
//                             strikePointList[iIndex].x = std::stoi(token);
// 
//                             cReadModeB = 3;
//                             break;
// 
//                         case 3:
//                             if (_bGetIsStringIsNumber(token) == false) {
//                                 gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 76 - Wrong Data format.");
//                                 delete[] pContents;
//                                 return false;
//                             }
//                             strikePointList[iIndex].y = std::stoi(token);
// 
//                             cReadModeB = 4;
//                             break;
// 
//                         case 4:
//                             if (_bGetIsStringIsNumber(token) == false) {
//                                 gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
//                                 delete[] pContents;
//                                 return false;
//                             }
//                             strikePointList[iIndex].iHP = std::stoi(token);
//                             strikePointList[iIndex].iInitHP = std::stoi(token);
//                             cReadModeB = 5;
//                             break;
// 
// 
//                         case 5:
//                             if (_bGetIsStringIsNumber(token) == false) {
//                                 gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
//                                 delete[] pContents;
//                                 return false;
//                             }
//                             strikePointList[iIndex].effectX[0] = std::stoi(token);
// 
//                             cReadModeB = 6;
//                             break;
// 
//                         case 6:
//                             if (_bGetIsStringIsNumber(token) == false) {
//                                 gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
//                                 delete[] pContents;
//                                 return false;
//                             }
//                             strikePointList[iIndex].effectY[0] = std::stoi(token);
// 
//                             cReadModeB = 7;
//                             break;
// 
//                         case 7:
//                             if (_bGetIsStringIsNumber(token) == false) {
//                                 gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
//                                 delete[] pContents;
//                                 return false;
//                             }
//                             strikePointList[iIndex].effectX[1] = std::stoi(token);
// 
//                             cReadModeB = 8;
//                             break;
// 
//                         case 8:
//                             if (_bGetIsStringIsNumber(token) == false) {
//                                 gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
//                                 delete[] pContents;
//                                 return false;
//                             }
//                             strikePointList[iIndex].effectY[1] = std::stoi(token);
// 
//                             cReadModeB = 9;
//                             break;
// 
//                         case 9:
//                             if (_bGetIsStringIsNumber(token) == false) {
//                                 gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
//                                 delete[] pContents;
//                                 return false;
//                             }
//                             strikePointList[iIndex].effectX[2] = std::stoi(token);
// 
//                             cReadModeB = 10;
//                             break;
// 
//                         case 10:
//                             if (_bGetIsStringIsNumber(token) == false)
//                             {
//                                 gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
//                                 delete[] pContents;
//                                 return false;
//                             }
//                             strikePointList[iIndex].effectY[2] = std::stoi(token);
// 
//                             cReadModeB = 11;
//                             break;
// 
//                         case 11:
//                             if (_bGetIsStringIsNumber(token) == false)
//                             {
//                                 gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
//                                 delete[] pContents;
//                                 return false;
//                             }
//                             strikePointList[iIndex].effectX[3] = std::stoi(token);
// 
//                             cReadModeB = 12;
//                             break;
// 
//                         case 12:
//                             if (_bGetIsStringIsNumber(token) == false)
//                             {
//                                 gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
//                                 delete[] pContents;
//                                 return false;
//                             }
//                             strikePointList[iIndex].effectY[3] = std::stoi(token);
// 
//                             cReadModeB = 13;
//                             break;
// 
//                         case 13:
//                             if (_bGetIsStringIsNumber(token) == false)
//                             {
//                                 gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
//                                 delete[] pContents;
//                                 return false;
//                             }
//                             strikePointList[iIndex].effectX[4] = std::stoi(token);
// 
//                             cReadModeB = 14;
//                             break;
// 
//                         case 14:
//                             if (_bGetIsStringIsNumber(token) == false) {
//                                 gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
//                                 delete[] pContents;
//                                 return false;
//                             }
//                             strikePointList[iIndex].effectY[4] = std::stoi(token);
// 
//                             cReadModeB = 15;
//                             break;
// 
//                         case 15:
//                             //strikePointList[iIndex].cRelatedMapName = token;
// 
//                             m_iTotalStrikePoints++;
// 
//                             cReadModeA = 0;
//                             cReadModeB = 0;
//                             break;
// 
//                     }
//                     break;

                case 24: // item event
                    switch (cReadModeB)
                    {
                        case 1: //  index
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 78 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            iIndex = std::stoi(token);

                            if (itemEventList[iIndex].cItemName.length() == 0)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 79 - Duplicate Item-Event number(%d:%s)", iIndex, itemEventList[iIndex].cItemName);
                                delete[] pContents;
                                return false;
                            }

                            m_iTotalItemEvents++;
                            cReadModeB = 2;
                            break;

                        case 2: // item_name
                            itemEventList[iIndex].cItemName = token;
                            cReadModeB = 3;
                            break;

                        case 3: // amount
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 81 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            itemEventList[iIndex].iAmount = std::stoi(token);

                            cReadModeB = 4;
                            break;

                        case 4: // total number
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 82 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            itemEventList[iIndex].iTotalNum = std::stoi(token);

                            cReadModeB = 5;
                            break;

                        case 5: // month
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 83 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            itemEventList[iIndex].iMonth = std::stoi(token);

                            cReadModeB = 6;
                            break;

                        case 6: // day
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 83 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            itemEventList[iIndex].iDay = std::stoi(token);

                            cReadModeB = 7;
                            break;

                        case 7: // type
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 83 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            itemEventList[iIndex].iType = std::stoi(token);

                            itemEventList[iIndex].iNumMob = 0;

                            cReadModeB = 8;
                            break;

                        case 8: // monster list
                            if (token == "EOL")
                            {
                                cReadModeA = 0;
                                cReadModeB = 0;
                            }
                            else
                            {
                                iNumMob = itemEventList[iIndex].iNumMob;

                                if (iNumMob >= 5)
                                {
                                    cReadModeA = 0;
                                    cReadModeB = 0;
                                    gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 83 - Wrong Data format.");
                                    break;
                                }

                                itemEventList[iIndex].cMob[iNumMob] = token;

                                itemEventList[iIndex].iNumMob++;

                                cReadModeB = 8;
                            }
                            break;
                    }
                    break;
                case 25:
                    switch (cReadModeB)
                    {
                        case 1:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error 78 - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            mobEventAmount = std::stoi(token);
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 26:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error ApocalypseMobGenType - Wrong Data format.");
                        delete[] pContents;
                        return false;
                    }
                    apocalypseMobGenType = (ApocMobGenType)std::stoi(token);
                    cReadModeA = 0;
                    cReadModeB = 0;
                    break;

                case 27:
                    switch (cReadModeB)
                    {
                        case 1:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            apocalypseBossMobNpcID = std::stoi(token);
                            cReadModeB = 2;
                            break;
                        case 2:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            apocalypseBossMobRect.left = std::stoi(token);
                            cReadModeB = 3;
                            break;
                        case 3:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            apocalypseBossMobRect.top = std::stoi(token);
                            cReadModeB = 4;
                            break;
                        case 4:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            apocalypseBossMobRect.right = std::stoi(token);
                            cReadModeB = 5;
                            break;
                        case 5:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            apocalypseBossMobRect.bottom = std::stoi(token);
                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 28:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error DynamicGateType - Wrong Data format.");
                        delete[] pContents;
                        return false;
                        return false;
                    }
                    dynamicGateType = std::stoi(token);
                    // DynamicGateType = 1: Opens Gate when Apoc begins
                    // DynamicGateType = 2: Open the gate when map is empty
                    // DynamicGateType = 3: Gate stays closed will change to 4 value to open it, along with a boss spawn.
                    // DynamicGateType = 4: Gate is openned. (set by server when Abaddon is spawning)
                    // DynamicGateType = 5: Gate is used by GM command
                    cReadModeA = 0;
                    cReadModeB = 0;
                    break;

                case 29:
                    switch (cReadModeB) {
                        case 1:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            dynamicGateCoordRectX1 = std::stoi(token);
                            cReadModeB = 2;
                            break;

                        case 2:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            dynamicGateCoordRectY1 = std::stoi(token);
                            cReadModeB = 3;
                            break;

                        case 3:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            dynamicGateCoordRectX2 = std::stoi(token);
                            cReadModeB = 4;
                            break;

                        case 4:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            dynamicGateCoordRectY2 = std::stoi(token);
                            cReadModeB = 5;
                            break;

                        case 5:
                            dynamicGateCoordDestMap = token;
                            cReadModeB = 6;
                            break;

                        case 6:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            dynamicGateCoordTgtX = std::stoi(token);
                            cReadModeB = 7;
                            break;

                        case 7:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            dynamicGateCoordTgtY = std::stoi(token);
                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 30:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error ApocalypseMap -  Wrong Data format.");
                        delete[] pContents;
                        return false;
                    }
                    flags.apocalypseMap = (bool)std::stoi(token);
                    cReadModeA = 0;
                    cReadModeB = 0;
                    break;
                case 31:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error HeldenianMap -  Wrong Data format.");
                        delete[] pContents;
                        return false;
                    }
                    flags.heldenianMap = std::stoi(token);
                    cReadModeA = 0;
                    cReadModeB = 0;
                    break;

                case 32:
                    switch (cReadModeB) {
                        case 1: // NpcID
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error Hedenian tower type id - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            heldenianTower[iIndex].sTypeID = std::stoi(token);
                            cReadModeB = 2;
                            break;
                        case 2: // side 
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error Hedenian Tower Side - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            heldenianTower[iIndex].cSide = std::stoi(token);
                            cReadModeB = 3;
                            break;
                        case 3: // sX
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error Hedenian Tower X pos - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            heldenianTower[iIndex].dX = std::stoi(token);
                            cReadModeB = 4;
                            break;
                        case 4: // sY
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error Hedenian Tower Y pos - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            heldenianTower[iIndex].dY = std::stoi(token);
                            iIndex++;
                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 33:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error Hedenian Map Mode - Wrong Data format.");
                        delete[] pContents;
                        return false;
                    }
                    heldenianModeMap = std::stoi(token);
                    cReadModeA = 0;
                    cReadModeB = 0;
                    break;

                case 34:
                    switch (cReadModeB) {
                        case 1:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error Hedenian Map Mode - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            heldenianWinningZoneX = std::stoi(token);
                            cReadModeB = 2;
                            break;
                        case 2:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error Hedenian Map Mode - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            heldenianWinningZoneY = std::stoi(token);
                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 35:
                    switch (cReadModeB) {
                        case 1:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error Heldenian Door Direction - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            heldenianGateDoor[iIndex].cDir = std::stoi(token);
                            cReadModeB = 2;
                            break;
                        case 2:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error Heldenian Door X pos - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            heldenianGateDoor[iIndex].dX = std::stoi(token);
                            cReadModeB = 3;
                            break;
                        case 3:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error Heldenian Door Y pos - Wrong Data format.");
                                delete[] pContents;
                                return false;
                            }
                            heldenianGateDoor[iIndex].dY = std::stoi(token);
                            iIndex++;
                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 36:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        gs->log->critical("(!!!) CRITICAL ERROR! Map Info file error - Chat-zone - Wrong Data format.");
                        delete[] pContents;
                        return false;
                    }
                    chatZone = std::stoi(token);
                    cReadModeA = 0;
                    cReadModeB = 0;
                    break;

                default:
                    break;
            }
        }
        else
        {
            if (token == "teleport-loc")
            {
                m_pTeleportLoc[iTeleportLocIndex] = new teleport_loc();
                cReadModeA = 1;
                cReadModeB = 1;
            }

            if (token == "waypoint")
            {
                cReadModeA = 2;
                cReadModeB = 1;
            }

            if (token == "npc")
            {
                cReadModeA = 3;
                cReadModeB = 1;
            }

            if (token == "random-mob-generator")
            {
                cReadModeA = 4;
                cReadModeB = 1;
            }

            if (token == "maximum-object")
                cReadModeA = 5;

            if (token == "npc-avoidrect")
            {
                cReadModeA = 6;
                cReadModeB = 1;
            }

            if (token == "spot-mob-generator")
            {
                cReadModeA = 7;
                cReadModeB = 1;
            }

            if (token == "map-location")
                cReadModeA = 8;

            if (token == "initial-point")
            {
                cReadModeA = 9;
                cReadModeB = 1;
            }

            if (token == "chat-zono-attack-areane")
            {
                cReadModeA = 10;
                cReadModeB = 1;
            }

            if (token == "fixed-dayornight-mode")
                cReadModeA = 11;

            if (token == "fish-point")
            {
                cReadModeA = 12;
                cReadModeB = 1;
            }

            if (token == "max-fish")
                cReadModeA = 13;

            if (token == "type")
                cReadModeA = 14;

            if (token == "level-limit")
                cReadModeA = 15;

            if (token == "mineral-generator")
            {
                cReadModeA = 16;
                cReadModeB = 1;
            }

            if (token == "mineral-point")
            {
                cReadModeA = 17;
                cReadModeB = 1;
            }

            if (token == "max-mineral")
                cReadModeA = 18;

            if (token == "upper-level-limit")
                cReadModeA = 19;

            if (token == "strategic-point")
            {
                cReadModeA = 20;
                cReadModeB = 1;
            }

            if (token == "energy-sphere-creation-point")
            {
                cReadModeA = 21;
                cReadModeB = 1;
            }

            if (token == "energy-sphere-goal-point")
            {
                cReadModeA = 22;
                cReadModeB = 1;
            }

            if (token == "strike-point")
            {
                cReadModeA = 23;
                cReadModeB = 1;
            }

            if (token == "item-event")
            {
                cReadModeA = 24;
                cReadModeB = 1;
            }

            if (token == "mobevent-amount")
            {
                cReadModeA = 25;
                cReadModeB = 1;
            }

            if (token == "ApocalypseMobGenType")
            {
                cReadModeA = 26;
                cReadModeB = 1;
            }

            if (token == "chat-zone")
            {
                cReadModeA = 27;
                cReadModeB = 1;
            }

            if (token == "DynamicGateType")
            {
                cReadModeA = 28;
                cReadModeB = 1;
            }

            if (token == "DynamicGateCoord")
            {
                cReadModeA = 29;
                cReadModeB = 1;
            }

            if (token == "ApocalypseMap")
            {
                cReadModeA = 30;
                cReadModeB = 1;
            }

            if (token == "HeldenianMap")
            {
                cReadModeA = 31;
                cReadModeB = 1;
            }

            if (token == "HeldenianTower")
            {
                cReadModeA = 32;
                cReadModeB = 1;
            }

            if (token == "HeldenianModeMap")
            {
                cReadModeA = 33;
                cReadModeB = 1;
            }

            if (token == "HeldenianWinningZone")
            {
                cReadModeA = 34;
                cReadModeB = 1;
            }

            if (token == "HeldenianGateDoor")
            {
                cReadModeA = 35;
                cReadModeB = 1;
            }

            if (token == "chat-zone")
            {
                cReadModeA = 36;
                cReadModeB = 1;
            }


            if (token == "[END-MAP-INFO]")
            {
                cReadModeA = 0;
                cReadModeB = 0;
                goto RMI_SKIPDECODING;
            }
        }
        token = toks.get();
    }

RMI_SKIPDECODING:;

    delete[] pContents;

    if ((cReadModeA != 0) || (cReadModeB != 0))
    {
        gs->log->critical("(!!!) CRITICAL ERROR! map info file contents error!");
        return false;
    }

    gs->log->critical("(!) Map info file decoding({}) - success! TL({}) WP({}) LNPC({}) MXO({}) RMG({} / {})", iTeleportLocIndex, iWayPointCfgIndex, iTotalNpcSetting, m_iMaximumObject, m_bRandomMobGenerator, m_cRandomMobGeneratorLevel);

    _SetupNoAttackArea();

    return true;

}

bool map::_bDecodeMapDataFileContents()
{
    std::string mapfile = "mapdata/";
    mapfile += name + ".amd";
    FILE * file;
    char * buffer;
    file = fopen(mapfile.c_str(), "r");

    if (!file)
    {
        //not existing
        gs->log->critical("Loaded map does not exist: {}", name);
        return false;
    }

    uint32_t filesize;

    fseek(file, 0, SEEK_END);
    filesize = ftell(file);
    fseek(file, 0, SEEK_SET);

    buffer = new char[filesize + 1];

    fread(buffer, 1, filesize, file);

    file_read fr(buffer, filesize);
    char cHeader[260];
    memset(cHeader, 0, 260);

    fr.read_bytes(cHeader, 256);

    for (int i = 0; i < 256; ++i)
        if (cHeader[i] == 0) cHeader[i] = ' ';

    utility::tokenizer toks(cHeader, "\t\n =");
    std::string token = toks.get();

    if (token == "MAPSIZEX")
    {
        token = toks.get();
        sizeX = std::stoi(token);
    }
    token = toks.get();
    if (token == "MAPSIZEY")
    {
        token = toks.get();
        sizeY = std::stoi(token);
    }
    token = toks.get();
    if (token == "TILESIZE")
    {
        token = toks.get();
        tileDataSize = std::stoi(token);
    }

    m_pTile = new tile[sizeX * sizeY];

    auto tileData = new char[tileDataSize];
    tile * pTile;

    for (auto iy = 0; iy < sizeY; iy++)
    {
        for (auto ix = 0; ix < sizeX; ix++)
        {
            fr.read_bytes(tileData, tileDataSize);
            pTile = (m_pTile + ix + iy * sizeY);
            pTile->m_attribute = tileData[8];
            if (*reinterpret_cast<short *>(&tileData[0]) == 19)
                pTile->m_attribute &= 0x08;
            //             pTile->m_bIsMoveAllowed = (tileData[8] & 0x80) ? false : true;
            //             pTile->m_bIsTeleport = (tileData[8] & 0x40) ? true : false;
            //             pTile->m_bIsFarmingAllowed = (tileData[8] & 0x20) ? true : false;
            //            pTile->m_bIsWater = (*reinterpret_cast<short *>(&tileData[0])== 19) ? true : false;
        }
    }
    delete[] tileData;

    return true;

    //     HANDLE hFile;
    //     char  cMapFileName[64], cHeader[260];
    //     uint32_t dwFileSize, nRead;
    //     int i, ix, iy;
    //     char * token, cReadMode, *mapData, *tileData;
    //     char seps[] = "= \t\n"; 
    //     class CStrTok * pStrTok = NULL;
    //     class CTile * pTile;
    // 
    //     wsprintf(cMapFileName, "mapdata\\%s.amd",m_cName);
    // 
    //     hFile = CreateFile(cMapFileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
    //     if (hFile == INVALID_HANDLE_VALUE) return false;
    //     dwFileSize = GetFileSize(hFile, NULL);
    // 
    //     ZeroMemory(cHeader, sizeof(cHeader));
    //     ReadFile(hFile, (char *)cHeader, 256, &nRead, NULL);
    // 
    //     for (i = 0; i < 256; i++) 
    //         if (cHeader[i] == NULL) cHeader[i] = ' ';
    // 
    //     cReadMode = 0;
    // 
    //     pStrTok = new StrTok(cHeader, seps);
    //     token = pStrTok->pGet();
    //     //token = strtok( cHeader, seps );   
    //     while( token != NULL )   {
    // 
    //         if (cReadMode != 0) {
    //             switch (cReadMode) {
    //             case 1:
    //                 m_sSizeX = atoi(token);
    //                 cReadMode = 0;
    //                 break;
    //             case 2:
    //                 m_sSizeY = atoi(token);
    //                 cReadMode = 0;
    //                 break;
    //             case 3:
    //                 m_sTileDataSize = atoi(token);
    //                 cReadMode = 0;
    //                 break;
    //             }
    //         }
    //         else {
    //             if (memcmp(token, "MAPSIZEX",8) == 0) cReadMode = 1;
    //             if (memcmp(token, "MAPSIZEY",8) == 0) cReadMode = 2;
    //             if (memcmp(token, "TILESIZE",8) == 0) cReadMode = 3; 
    //         }
    // 
    //         //token = strtok( NULL, seps );
    //         token = pStrTok->pGet();
    //     }
    // 
    //     m_pTile = (Tile *)new Tile[m_sSizeX * m_sSizeY];
    // 
    //     mapData = new char[dwFileSize];
    //     tileData = mapData;
    // 
    //     ReadFile(hFile, mapData, dwFileSize, &nRead, NULL);
    //     CloseHandle(hFile);
    // 
    //     for (iy = 0; iy < m_sSizeY; iy++)
    //         for (ix = 0; ix < m_sSizeX; ix++, tileData += m_sTileDataSize) {
    //             pTile = (Tile *)(m_pTile + ix + iy*m_sSizeY);
    //             pTile->m_attribute = tileData[8];
    //             pTile->m_bIsMoveAllowed = (tileData[8] & 0x80) ? false : true;
    //             pTile->m_bIsTeleport = (tileData[8] & 0x40) ? true : false;
    //             pTile->m_bIsFarmingAllowed = (tileData[8] & 0x20) ? true : false;
    //             pTile->m_bIsWater = (*((short *)&tileData[0])== 19) ? true : false;
    //         }
    // 
    //         delete [] mapData;
    // 
    //         if (pStrTok != NULL) delete pStrTok;
    return true;
}


bool map::bSearchTeleportDest(int sX, int sY, std::string & pMapName, uint16_t * pDx, uint16_t * pDy, uint8_t * pDir)
{
    int i;

    for (i = 0; i < teleportLocationList.size(); i++)
        if ((teleportLocationList[i]->source_x == sX) && (teleportLocationList[i]->source_y == sY))
        {
            pMapName = teleportLocationList[i]->dest_map_name;
            *pDx = teleportLocationList[i]->dest_x;
            *pDy = teleportLocationList[i]->dest_y;
            *pDir = teleportLocationList[i]->direction;
            return true;
        }

    return false;
}

void map::SetDynamicObject(uint16_t wID, short sType, short sX, short sY, uint64_t dwRegisterTime) const
{
    if ((sX < 0) || (sX >= sizeX) || (sY < 0) || (sY >= sizeY)) return;

    auto pTile = (m_pTile + sX + sY * sizeY);

    pTile->m_wDynamicObjectID = wID;
    pTile->m_sDynamicObjectType = sType;
    pTile->m_dwDynamicObjectRegisterTime = dwRegisterTime;
}

bool map::bGetDynamicObject(short sX, short sY, short * pType, uint64_t * pRegisterTime, int * pIndex) const
{
    if ((sX < 0) || (sX >= sizeX) || (sY < 0) || (sY >= sizeY)) return false;

    auto pTile = (m_pTile + sX + sY * sizeY);

    *pType = pTile->m_sDynamicObjectType;
    *pRegisterTime = pTile->m_dwDynamicObjectRegisterTime;
    if (pIndex != nullptr) *pIndex = pTile->m_wDynamicObjectID;

    return true;
}

bool map::bGetIsWater(short dX, short dY) const
{
    if ((dX < 14) || (dX >= sizeX - 16) || (dY < 12) || (dY >= sizeY - 14)) return false;

    auto pTile = (tile *)(m_pTile + dX + dY * sizeY);

    return pTile->IsWater();
}



bool map::bGetIsFarm(short dX, short dY) const
{
    if ((dX < 14) || (dX >= sizeX - 16) || (dY < 12) || (dY >= sizeY - 14)) return false;

    auto pTile = (m_pTile + dX + dY * sizeY);

    return pTile->IsFarm();
}

bool map::IsBuild(short dX, short dY) const
{
    if ((dX < 14) || (dX >= sizeX - 16) || (dY < 12) || (dY >= sizeY - 14)) return false;

    auto pTile = (m_pTile + dX + dY * sizeY);

    return pTile->IsBuild();
}


int map::iAnalyze(char cType, int * pX, int * pY, int * pV1, int * pV2, int * pV3) const
{
    switch (cType) {
        case 1:

            break;
    }

    return 0;
}

void map::SetTempMoveAllowedFlag(int dX, int dY, bool bFlag) const
{
    if ((dX < 20) || (dX >= sizeX - 20) || (dY < 20) || (dY >= sizeY - 20)) return;

    auto pTile = (m_pTile + dX + dY * sizeY);
    pTile->IsTempMovable(bFlag);
}

map::OccupyFlag * map::iRegisterOccupyFlag(int dX, int dY, int iSide, int iEKNum, int iDOI)
{
    if ((dX < 20) || (dX >= sizeX - 20) || (dY < 20) || (dY >= sizeY - 20)) return nullptr;

    OccupyFlag * oflag = new OccupyFlag(dX, dY, iSide, iEKNum, iDOI);

    occupyFlag.push_back(oflag);

    return oflag;
}



void map::ClearSectorInfo()
{
    for (int i = 0; i < MAXSECTORS * MAXSECTORS; ++i)
    {
        sectorInfo[i].neutralActivity = 0;
        sectorInfo[i].aresdenActivity = 0;
        sectorInfo[i].elvineActivity = 0;
        sectorInfo[i].mobActivity = 0;
        sectorInfo[i].playerActivity = 0;
    }
}

void map::ClearTempSectorInfo()
{
    for (int i = 0; i < MAXSECTORS * MAXSECTORS; ++i)
    {
        sectorInfoTemp[i].neutralActivity = 0;
        sectorInfoTemp[i].aresdenActivity = 0;
        sectorInfoTemp[i].elvineActivity = 0;
        sectorInfoTemp[i].mobActivity = 0;
        sectorInfoTemp[i].playerActivity = 0;
    }
}

void map::_SetupNoAttackArea()
{
    int i, ix, iy;
    tile * pTile;

    for (i = 0; i < safeZoneList.size(); i++) {
        if (safeZoneList[i].top > 0) {

            for (ix = safeZoneList[i].left; ix <= safeZoneList[i].right; ix++)
                for (iy = safeZoneList[i].top; iy <= safeZoneList[i].bottom; iy++) {
                    pTile = (m_pTile + ix + iy * sizeY);
                    pTile->m_attribute |= 0x00000004;
                }
        }

        else if (safeZoneList[i].top == -10) {
            for (ix = 0; ix < sizeX; ix++)
                for (iy = 0; iy < sizeY; iy++) {
                    pTile = (m_pTile + ix + iy * sizeY);
                    pTile->m_attribute |= 0x00000004;
                }
        }
    }
}

int map::iGetAttribute(int dX, int dY, int iBitMask) const
{
    if ((dX < 20) || (dX >= sizeX - 20) || (dY < 20) || (dY >= sizeY - 20)) return -1;

    tile * pTile = (m_pTile + dX + dY * sizeY);
    return (pTile->m_attribute & iBitMask);
}

bool map::bAddCrusadeStructureInfo(char cType, short sX, short sY, char cSide)
{
    crusadestructure tempstructure = crusadestructure();
    tempstructure.cType = cType;
    tempstructure.cSide = cSide;
    tempstructure.sX = sX;
    tempstructure.sY = sY;
    m_stCrusadeStructureInfo.push_back(tempstructure);
    return true;
    // useful? \/
    //if(cType == NPC_MANASTONE)
    //{
    //    pTile = (class CTile *)(m_pTile + sX + (sY-1)*m_sSizeY);
    //    pTile->m_bIsMoveAllowed = false;
    //}
}

bool map::bRemoveCrusadeStructureInfo(short sX, short sY)
{
    for (std::vector<crusadestructure>::iterator iter = m_stCrusadeStructureInfo.begin(); iter != m_stCrusadeStructureInfo.end(); ++iter)
    {
        if ((iter->sX == sX) && (iter->sY == sY))
        {
            m_stCrusadeStructureInfo.erase(iter);
            return true;
        }
    }
    return false;
}



bool map::bAddCropsTotalSum()
{
    if (m_iTotalAgriculture < MAXAGRICULTURE)
    {
        m_iTotalAgriculture++;
        return true;
    }
    return false;
}


bool map::bRemoveCropsTotalSum()
{
    if (m_iTotalAgriculture == 0)
        return true;
    if (m_iTotalAgriculture < MAXAGRICULTURE)
    {
        m_iTotalAgriculture--;
        return true;
    }
    return false;
}

void map::RestoreStrikePoints()
{
    for (int i = 0; i < strikePointList.size(); i++)
        strikePointList[i].hp = strikePointList[i].hpInit;
}

void map::IncPlayerActivity(client * player)
{
    int iStX, iStY;
    return;

    //TODO: vector or 2d array? need to initialize vector

    iStX = player->x / 20;
    iStY = player->y / 20;
    sectorInfoTemp[int64_t(iStX + iStY) * MAXSECTORS].playerActivity++;

    switch (player->side)
    {
        case faction::neutral: sectorInfoTemp[int64_t(iStX + iStY) * MAXSECTORS].neutralActivity++; break;
        case faction::aresden: sectorInfoTemp[int64_t(iStX + iStY) * MAXSECTORS].aresdenActivity++; break;
        case faction::elvine: sectorInfoTemp[int64_t(iStX + iStY) * MAXSECTORS].elvineActivity++;  break;
    }
}

bool map::GetInitialPoint(int16_t & pX, int16_t & pY, string & pPlayerLocation)
{
    //every map has to have an initial point as a "fail safe". if none exists by some stupid error, send back to town

    if (initialPointList.empty())
    {
        map * tempmap = nullptr;
        if (pPlayerLocation == "NONE")
            tempmap = gs->get_map("default");//hardcoded. trololo. -- could use changes if attempting to ever make an additional side... doubtful though
        else if (pPlayerLocation.substr(0, 3) == "are")
            tempmap = gs->get_map("aresden");
        else if (pPlayerLocation.substr(0, 3) == "elv")
            tempmap = gs->get_map("elvine");

        if (!tempmap)
        {
            //map not found. this is the critical failure point as player cannot load into current map, and their "town" map cannot be found loaded.
            //worst case, pick a random teleport spot. If that fails, return a false to indicate to disconnect the client
            if (teleportLocationList.empty())
            {
                return false;
            }
            else
            {
                //this is incredibly crude and will result in player most likely instant teleporting to where the spot leads to, but is a fail safe to not being able to be
                //placed. this should typically never be trigger, but HAS had a use before but has always resulted in the inability of the player to login, so this should exist
                uint16_t pos = uint16_t(rand() % teleportLocationList.size());
                pX = teleportLocationList[pos]->source_x;
                pY = teleportLocationList[pos]->source_y;
                return true;
            }
        }
    }

    if (initialPointList.empty())
    {
        gs->log->error("Map {} has empty initial spawn coords list. defaulting to half of map size.", name);
        pX = sizeX / 2;
        pY = sizeY / 2;
        return true;
    }

    if (pPlayerLocation == "NONE")
    {
        //apparently travelers always go to the first initial coordinate
        pX = initialPointList[0].x;
        pY = initialPointList[0].y;
        return true;
    }

    uint16_t pos = uint16_t(rand() % initialPointList.size());

    pX = initialPointList[pos].x;
    pY = initialPointList[pos].y;
    return true;
}

tile * map::GetTile(int16_t x, int16_t y) const
{
    return (m_pTile + x + y * sizeY);
}

void map::NpcProcess()
{
    int iMaxHP = 0;
    uint64_t dwTime, dwActionTime;

    dwTime = utility::current_time();

    //TODO: either acquire a lock on every npc in list or --> add a mutex for npc deletion
    std::list<std::shared_ptr<npc>> tempnpclist = npclist;

    for (const std::shared_ptr<npc> & npc_ : tempnpclist)
    {
        if (npc_ == nullptr)
            continue;
        switch (npc_->m_cBehavior)
        {
            case BEHAVIOR_ATTACK:
                dwActionTime = npc_->timeActionInterval + 50 * utility::gen_rand(1, 7);
                break;
            case BEHAVIOR_MOVE:
            case BEHAVIOR_FLEE:
                dwActionTime = npc_->timeActionInterval + 400;
                break;
            default:
                dwActionTime = npc_->timeActionInterval;
                break;
        }

        if (npc_->magic_effect_status[MAGICTYPE_ICE] != 0)
            dwActionTime += (dwActionTime / 2);

        if ((dwTime - npc_->timeLastAction) > dwActionTime) {
            npc_->timeLastAction = dwTime;

            npc_->RegenHP();
            npc_->RegenMP();
            npc_->Behave();

            if ((npc_->health != 0) && (npc_->summoned == true))
            {
                switch (npc_->get_type()) {
                    case 29:
                        if ((dwTime - npc_->timeSummoned) > 1000 * 90)
                            NpcKilledHandler(nullptr, npc_, 0);
                        break;
                    case 64:
                        if ((dwTime - npc_->timeSummoned) > PLANTTIME)
                            DeleteNpc(npc_);
                        break;
                    default:
                        if ((dwTime - npc_->timeSummoned) > SUMMONTIME)
                            NpcKilledHandler(nullptr, npc_, 0);
                        break;
                }
            }


            if (npc_->is_dead() && ((dwTime - npc_->timeDead) > npc_->timeRegen))
                DeleteNpc(npc_);
        }
    }
}

void map::DeleteNpc(std::shared_ptr<npc> npc_)
{
    int  i, iNumItem, iItemID, iItemIDs[MAX_NPCITEMDROP], iSlateID = 0;
    char cItemName[21];
    item * pItem = nullptr, * pItem2 = nullptr;
    uint32_t dwCount = 0, dwTime;
    Point ItemPositions[MAX_NPCITEMDROP];
    char cTemp[256];
    memset(iItemIDs, 0, sizeof iItemIDs);
    memset(cTemp, 0, sizeof cTemp);
    memset(ItemPositions, 0, sizeof ItemPositions);
    memset(cItemName, 0, sizeof cItemName);

    dwTime = utility::current_time();

    //Init number of items to 1 unless its a multidrop;
    iNumItem = 0;
    iItemID = 0; // No current item

    gs->SendEventToNearClient_TypeA(npc_, MSGID_MOTION_EVENT_REJECT, 0, 0, 0);
    ClearDeadOwner(npc_->x, npc_->y);

    totalActiveObject--;

    if (flags.apocalypseMap)
    {
        if (npc_->get_type() == apocalypseBossMobNpcID)
        {
            RegisterDelayEvent(DELAYEVENTTYPE_END_APOCALYPSE, 0, utility::current_time() + 300 * 1000,
                nullptr, 0, 0, 0, 0, 0);
            // open gate back to town
            if (npc_->map_ != nullptr)
            {
                npc_->map_->dynamicGateType = 4;

                int iShortCutIndex = 0;
                //while (int i = m_iClientShortCut[iShortCutIndex++])
                {
                    //Notify_ApocalypseGateState(i);
                }
            }
        }
        else if (totalActiveObject == 0)
        {
            if (apocalypseMobGenType == AMGT_OPENGATE)
            {
                int iShortCutIndex = 0;
                //while (int i = m_iClientShortCut[iShortCutIndex++])
                {
                    //Notify_ApocalypseGateState(i);
                }
            }
            else if (npc_->map_->apocalypseMobGenType == AMGT_SPAWNBOSS)
            {
                //GenerateApocalypseBoss(maplist[iNpcH]->pMap);
            }
        }
    }


    if (npc_->m_iSpotMobIndex != 0)
        spotMobGenerator[npc_->m_iSpotMobIndex].current--;

    RemoveFromTarget(npc_);

    switch (npc_->get_type())
    {
        case NPC_AGT:
        case NPC_CGT:
        case NPC_MS:
        case NPC_DT:
        case NPC_MANASTONE:
            npc_->map_->bRemoveCrusadeStructureInfo(npc_->x, npc_->y);

            for (i = 0; i < MAXGUILDS; i++) {
                if (gs->m_pGuildTeleportLoc[i].v1 == npc_->guild_->uid) {
                    gs->m_pGuildTeleportLoc[i].time = dwTime;
                    gs->m_pGuildTeleportLoc[i].v2--;
                    if (gs->m_pGuildTeleportLoc[i].v2 < 0) gs->m_pGuildTeleportLoc[i].v2 = 0;
                    break;
                }
            }
            break;

        case NPC_LWB:
        case NPC_GHK:
        case NPC_GHKABS:
        case NPC_TK:
        case NPC_BG:
        case NPC_CP:
            for (i = 0; i < MAXGUILDS; i++) {
                if (gs->m_pGuildTeleportLoc[i].v1 == npc_->guild_->uid) {
                    gs->m_pGuildTeleportLoc[i].num_summon_npc--;
                    if (gs->m_pGuildTeleportLoc[i].num_summon_npc < 0) gs->m_pGuildTeleportLoc[i].num_summon_npc = 0;
                    break;
                }
            }
            break;

        case NPC_CROPS:
            npc_->map_->bRemoveCropsTotalSum();
            break;
    }


    RemoveFromDelayEventList(npc_.get(), 0);

#ifdef NO_SECONDDROP
    return;
#endif

    if (npc_->summoned)
        return;

    npclist.remove(npc_);
    //     for (std::list<shared_ptr<Npc>>::const_iterator iter = npclist.cbegin(); iter != npclist.cend(); ++iter)
    //     {
    //         if (*iter == npc)
    //         {
    //             npclist.remove(iter);
    //             break;
    //         }
    //     }

    //     pItem = new Item;
    //     ZeroMemory(cItemName, sizeof(cItemName));
    // 
    //     if (drops.HasSecondaryDrop(npc.get()))
    //     {
    //         if (drops.GetSecDropNum(npc->m_sType) == 1)
    //             iItemID = drops.Roll(npc.get(), ONNPCDELETE);
    //         else
    //             iNumItem = RollMultiple(npc.get(), ITEMSPREAD_FIXED, 4, iItemIDs, ItemPositions);
    //     }
    // 
    //     dwCount = 1;
    // 
    // 
    //     if (iNumItem > 0) {
    //         GetLocalTime(&SysTime);
    //         wsprintf(cTemp, "%d%02d%", SysTime.wMonth, SysTime.wDay);
    //         for (int j = 0; j < iNumItem; j++){
    //             if (pItem == NULL) {
    //                 pItem = new class CItem;
    //             }
    //             if (pItem->InitItemAttr(iItemIDs[j]) == false ||
    //                 maplist[npc->pMap]->bGetIsMoveAllowedTile(ItemPositions[j].x, ItemPositions[j].y) == false) {
    //                 delete pItem;
    //                 pItem = NULL;
    //             }
    //             else {
    //                 if (iItemIDs[j] == ITEM_GOLD)
    //                 {
    //                     if (npc->dwGoldDropValue <= 4) pItem->m_dwCount = dice(1, npc->dwGoldDropValue);
    //                     else
    //                         switch (dice(1, 2))
    //                     {
    //                         case 1:
    //                             pItem->m_dwCount = (uint32_t)(npc->dwGoldDropValue + dice(1, npc->dwGoldDropValue / 5));
    //                             break;
    // 
    //                         case 2:
    //                             pItem->m_dwCount = (uint32_t)(npc->dwGoldDropValue - dice(1, npc->dwGoldDropValue / 5));
    //                             break;
    //                     }
    //                 }
    //                 else
    //                     pItem->m_dwCount = dwCount;
    // 
    //                 pItem->m_sTouchEffectType = ITET_ID;
    //                 pItem->m_sTouchEffectValue1 = dice(1, 100000);
    //                 pItem->m_sTouchEffectValue2 = dice(1, 100000);
    //                 pItem->m_sTouchEffectValue3 = (short)dwTime;
    //                 if (!maplist[npc->pMap]->bSetItem(
    //                     ItemPositions[j].x, ItemPositions[j].y, pItem))
    //                 {
    //                     delete pItem;
    //                 }
    //                 else
    //                 {
    //                     SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, COMMONTYPE_ITEMDROP, npc->pMap,
    //                         ItemPositions[j].x, ItemPositions[j].y, pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor);
    //                     _bItemLog(ITEMLOG_NEWGENDROP, NULL, npc->m_cNpcName, pItem);
    //                     AddGroundItem(pItem, ItemPositions[j].x, ItemPositions[j].y, npc->pMap, TILECLEANTIMEPLAYER);
    //                 }
    //                 pItem = NULL;
    //             }
    //         }
    //     }
    //     else{
    //         if (iItemID == 0 && npc->dwGoldDropValue > 0 && dice(1, 50) == 13) {
    //             iItemID = ITEM_GOLD;
    //             if (npc->dwGoldDropValue <= 4) dwCount = dice(1, npc->dwGoldDropValue);
    //             else
    //                 switch (dice(1, 2))
    //             {
    //                 case 1:
    //                     dwCount = (uint32_t)(npc->dwGoldDropValue + dice(1, npc->dwGoldDropValue / 5));
    //                     break;
    // 
    //                 case 2:
    //                     dwCount = (uint32_t)(npc->dwGoldDropValue - dice(1, npc->dwGoldDropValue / 5));
    //                     break;
    //             }
    //         }
    //         if (pItem->InitItemAttr(iItemID) == false) {
    //             delete pItem;
    //             pItem = NULL;
    //         }
    //         else {
    // 
    //             pItem->m_dwCount = dwCount;
    // 
    //             pItem->m_sTouchEffectType = ITET_ID;
    //             pItem->m_sTouchEffectValue1 = dice(1, 100000);
    //             pItem->m_sTouchEffectValue2 = dice(1, 100000);
    // #ifdef LOGTIME
    //             pItem->m_sTouchEffectValue3 = dwTime;
    // #else 
    //             SYSTEMTIME SysTime;
    //             char cTemp[256];
    //             GetLocalTime(&SysTime);
    //             ZeroMemory(cTemp, sizeof(cTemp));
    //             //            wsprintf(cTemp, "%d%02d%02d",  (short)SysTime.wMonth, (short)SysTime.wDay,(short) SysTime.wHour);
    //             wsprintf(cTemp, "%d%02d%", (short)SysTime.wMonth, (short)SysTime.wDay);
    // 
    //             pItem->m_sTouchEffectValue3 = atoi(cTemp);
    // #endif
    //             if (!maplist[npc->pMap]->bSetItem(
    //                 npc->m_sX, npc->m_sY, pItem))
    //             {
    //                 delete pItem;
    //             }
    //             else
    //             {
    //                 SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, COMMONTYPE_ITEMDROP, npc->pMap,
    //                     npc->m_sX, npc->m_sY,
    //                     pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor);
    // 
    //                 _bItemLog(ITEMLOG_NEWGENDROP, 0, npc->m_cNpcName, pItem);
    //                 AddGroundItem(pItem, npc->m_sX, npc->m_sY, npc->pMap, TILECLEANTIMEPLAYER);
    //             }
    //             pItem = NULL;
    //         }
    //     }
    // 
    //     if (dice(1, 100000) < 60) {
    //         pItem2 = new class Item;
    //         switch (dice(1, 4)){
    //         case 1:    iSlateID = 868; break;
    //         case 2: iSlateID = 869; break;
    //         case 3: iSlateID = 870; break;
    //         case 4: iSlateID = 871; break;
    //         }
    //         if (pItem2->InitItemAttr(iSlateID) == false) {
    //             delete pItem2;
    //             pItem2 = NULL;
    //         }
    //         else {
    //             pItem2->m_dwCount = 1;
    // 
    //             pItem2->m_sTouchEffectType = ITET_ID;
    //             pItem2->m_sTouchEffectValue1 = dice(1, 100000);
    //             pItem2->m_sTouchEffectValue2 = dice(1, 100000);
    //             pItem2->m_sTouchEffectValue3 = (short)dwTime;
    // 
    //             if (!maplist[npc->pMap]->bSetItem(
    //                 npc->m_sX, npc->m_sY, pItem2))
    //             {
    //                 delete pItem2;
    //             }
    //             else
    //             {
    //                 SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, COMMONTYPE_ITEMDROP, npc->pMap,
    //                     npc->m_sX, npc->m_sY, pItem2->m_sSprite, pItem2->m_sSpriteFrame, pItem2->m_cItemColor);
    //                 //_bItemLog(ITEMLOG_NEWGENDROP, NULL, npc->m_cNpcName, pItem2);
    //                 AddGroundItem(pItem, npc->m_sX, npc->m_sY, npc->pMap, TILECLEANTIMEPLAYER);
    //             }
    //             pItem2 = NULL;
    //         }
    //     }
}
std::shared_ptr<npc> map::CreateNpc(std::string & pNpcName, char cSA, char cMoveType, uint16_t * poX, uint16_t * poY, faction changeSide, char * pWaypointList, rect * pArea, int iSpotMobIndex, bool bHideGenMode, bool bIsSummoned, bool bFirmBerserk, bool bIsMaster, int iGuildGUID)
{
    int i = 0, t, j, k;
    char cTxt[120];
    short sX, sY, sRange;
    bool  bFlag;

    memset(cTxt, 0, sizeof cTxt);

    if (pNpcName.length() == 0) return nullptr;

    std::shared_ptr<npc> newnpc = std::make_shared<npc>(++npchandle, this);

    switch (cSA)
    {
        //     case 9:
        //         newnpc = (Npc*)(new HTNpc(i));
        //         break;
        //     case 10:
        //         newnpc = (Npc*)(new Elite(i));
        //         break;
        //    default:
        //        newnpc = new Npc(++npchandle, this);
        //        break;
    }

    for (auto npc : gs->m_npcConfigList)
    {
        if (npc && npc->name == pNpcName)
        {
            if (!newnpc->initNpcAttr(npc, cSA))
            {
                gs->log ->error("(!) Error in Npc Creation ({}) Ignored.", pNpcName);
                return nullptr;
            }
            break;
        }
    }
    if (newnpc->name.length() == 0)
    {
        gs->log->error("(!) Non-existing NPC creation request! ({}) Ignored.", pNpcName);
        return nullptr;
    }

    newnpc->map_ = this;

    //     if (newnpc->m_cDayOfWeekLimit < 10) 
    //     {
    //         if (newnpc->m_cDayOfWeekLimit != SysTime.wDayOfWeek) 
    //         {
    //             delete newnpc;
    //             return false;
    //         }
    //     }

    switch (cMoveType)
    {
        case MOVETYPE_GUARD:
        case MOVETYPE_RANDOM:
            if ((poX != nullptr) && (poY != nullptr) && (*poX != 0) && (*poY != 0)) {
                sX = *poX;
                sY = *poY;
            }
            else {
                for (j = 0; j <= 30; j++)
                {
                    sX = (rand() % (sizeX - 50)) + 15;
                    sY = (rand() % (sizeY - 50)) + 15;

                    bFlag = true;
                    for (k = 0; k < mobGeneratorAvoidList.size(); k++)
                    {
                        if (mobGeneratorAvoidList[k].left != -1)
                        {
                            if ((sX >= mobGeneratorAvoidList[k].left) &&
                                (sX <= mobGeneratorAvoidList[k].right) &&
                                (sY >= mobGeneratorAvoidList[k].top) &&
                                (sY <= mobGeneratorAvoidList[k].bottom))
                            {
                                return nullptr;
                            }
                        }
                    }
                }
            }
            break;

        case MOVETYPE_RANDOMAREA:
            sRange = short(pArea->right - pArea->left);
            sX = short((rand() % sRange) + pArea->left);
            sRange = short(pArea->bottom - pArea->top);
            sY = short((rand() % sRange) + pArea->top);
            break;

        case MOVETYPE_RANDOMWAYPOINT:
            sX = short(waypointList[pWaypointList[utility::gen_rand(1, 10) - 1]].x);
            sY = short(waypointList[pWaypointList[utility::gen_rand(1, 10) - 1]].y);
            break;

        default:
            if ((poX != nullptr) && (poY != nullptr) && (*poX != 0) && (*poY != 0)) {
                sX = *poX;
                sY = *poY;
            }
            else {
                sX = short(waypointList[pWaypointList[0]].x);
                sY = short(waypointList[pWaypointList[0]].y);
            }
            break;
    }

    if (!bGetEmptyPosition(sX, sY, newnpc)/* ||
                                            (bHideGenMode && getPlayerNum(mapIndex, sX, sY, 7) > 0)*/)
    {
        return nullptr;
    }

    if ((poX != nullptr) && (poY != nullptr)) {
        *poX = sX;
        *poY = sY;
    }

    newnpc->x = sX;
    newnpc->y = sY;

    newnpc->m_vX = sX;
    newnpc->m_vY = sY;

    for (t = 0; t < 10; t++)
        newnpc->m_iWayPointIndex[t] = pWaypointList[t];

    newnpc->m_cTotalWaypoint = 0;
    for (t = 0; t < 10; t++)
        if (newnpc->m_iWayPointIndex[t] != -1) newnpc->m_cTotalWaypoint++;

    if (pArea != nullptr) {
        // RANDOMAREA Copy
        newnpc->roamArea = *pArea;
        //SetRect(&newnpc->m_rcRandomArea, pArea->left, pArea->top, pArea->right, pArea->bottom);
    }

    switch (cMoveType)
    {
        case MOVETYPE_GUARD:
            newnpc->m_dX = newnpc->x;
            newnpc->m_dY = newnpc->y;
            break;

        case MOVETYPE_SEQWAYPOINT:
            newnpc->m_cCurWaypoint = 1;

            newnpc->m_dX = short(waypointList[newnpc->m_iWayPointIndex[newnpc->m_cCurWaypoint]].x);
            newnpc->m_dY = short(waypointList[newnpc->m_iWayPointIndex[newnpc->m_cCurWaypoint]].y);
            break;

        case MOVETYPE_RANDOMWAYPOINT:
            newnpc->m_cCurWaypoint = (rand() % (newnpc->m_cTotalWaypoint - 1)) + 1;

            newnpc->m_dX = short(waypointList[newnpc->m_iWayPointIndex[newnpc->m_cCurWaypoint]].x);
            newnpc->m_dY = short(waypointList[newnpc->m_iWayPointIndex[newnpc->m_cCurWaypoint]].y);
            break;

        case MOVETYPE_RANDOMAREA:
            newnpc->m_cCurWaypoint = 0;

            sRange = short(newnpc->roamArea.right - newnpc->roamArea.left);
            newnpc->m_dX = short((rand() % sRange) + newnpc->roamArea.left);
            sRange = short(newnpc->roamArea.bottom - newnpc->roamArea.top);
            newnpc->m_dY = short((rand() % sRange) + newnpc->roamArea.top);
            break;

        case MOVETYPE_RANDOM://summon
            newnpc->m_dX = short((rand() % (sizeX - 50)) + 15);
            newnpc->m_dY = short((rand() % (sizeY - 50)) + 15);
            break;
    }

    newnpc->m_tmp_iError = 0;
    newnpc->m_cMoveType = cMoveType;

    switch (newnpc->m_cActionLimit)
    {
        case 2:
        case 3:
        case 5:
            newnpc->m_cBehavior = BEHAVIOR_STOP;

            switch (newnpc->get_type())
            {
                case 15:
                case 19:
                case 20:
                case 24:
                case 25:
                case 26:
                    newnpc->direction = 4 + utility::gen_rand(1, 3) - 1;
                    break;

                default:
                    newnpc->direction = utility::gen_rand(1, 8);
                    break;
            }
            break;
        case 8: // Heldenian gate
            newnpc->direction = 3;
            newnpc->m_cBehavior = BEHAVIOR_STOP;
            if (newnpc->m_cSize > 0)
            {
                for (short sx1 = (sX - 1); sx1 <= sX + 1; sx1++)
                    for (short sy1 = (sY - 1); sy1 <= sY + 1; sy1++)
                    {
                        SetTempMoveAllowedFlag(sx1, sy1, false);

                    }
            }
            break;

        default:
            newnpc->m_cBehavior = BEHAVIOR_MOVE;
            newnpc->direction = 5;
            break;
    }

    newnpc->follow = nullptr;
    newnpc->target = nullptr;
    newnpc->turn = (rand() % 2);

    switch (newnpc->get_type())
    {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            newnpc->appr2 = short(0xF000);
            newnpc->appr2 = newnpc->appr2 | (uint64_t(rand() % 13) << 4);
            newnpc->appr2 = newnpc->appr2 | (rand() % 9);
            break;

        case 36:
        case 37:
        case 38:
        case 39:
            newnpc->appr2 = 3;
            break;
        case 64:
            newnpc->appr2 = 1;
            break;

        default:
            newnpc->appr2 = 0;
            break;
    }

    newnpc->timeLastAction = utility::current_time() + (rand() % 10000);
    newnpc->timeMana = utility::current_time();
    newnpc->timeHealth = newnpc->timeMana;
    newnpc->m_sBehaviorTurnCount = 0;
    newnpc->summoned = bIsSummoned;
    newnpc->isLeader = bIsMaster;
    if (bIsSummoned == true)
        newnpc->timeSummoned = utility::current_time();

    if (bFirmBerserk == true) {
        newnpc->add_magic_effect(MAGICTYPE_BERSERK, 0);
        newnpc->experience *= 2;
        newnpc->m_iNoDieRemainExp *= 2;
    }

    // !!!
    if (changeSide != -1)
    {
        newnpc->side = changeSide;
        newnpc->set_side_flag(changeSide);
    }

    newnpc->m_cBravery = (rand() % 3) + newnpc->m_iMinBravery;
    newnpc->m_iSpotMobIndex = iSpotMobIndex;
    //    newnpc->guildGUID = iGuildGUID;


        //testcode
        //     if (iGuildGUID != 0) {
        //         wsprintf(g_cTxt, "Summon War Unit(%d) GUID(%d)", npc->m_sType, iGuildGUID);
        //         PutLogList(g_cTxt);
        //     }

    SetOwner(newnpc, sX, sY);
    totalActiveObject++;
    totalAliveObject++;

    npclist.push_back(newnpc);

    switch (newnpc->get_type()) {
        case 36:
        case 37:
        case 38:
        case 39:
        case 42:
            bAddCrusadeStructureInfo(newnpc->get_type(), sX, sY, newnpc->side);
            break;
        case 64:
            bAddCropsTotalSum();
            return newnpc;
            break;
    }

    gs->send_event_near_type_a(newnpc, MSGID_MOTION_EVENT_CONFIRM, 0, 0, 0);
    return newnpc;
}

std::shared_ptr<npc> map::GetNpc(uint64_t ObjectID)
{
    for (std::shared_ptr<npc> npc : npclist)
    {
        if (npc->handle == ObjectID)
            return npc;
    }
    return nullptr;
}

void map::RemoveFromDelayEventList(unit * unit, int32_t iEffectType)
{
    delayMutex.lock();
    std::list<std::shared_ptr<delay_event>> removedelaylist;

    for (const std::shared_ptr<delay_event> & delayevent : DelayEventList)
    {
        if (iEffectType != 0)
        {
            if ((delayevent->target->handle == unit->handle) && (delayevent->target->get_type() == unit->get_type()) && (delayevent->m_iEffectType == iEffectType))
            {
                removedelaylist.push_back(delayevent);
            }
        }
        else
        {
            if ((delayevent->target->handle == unit->handle) && (delayevent->target->get_type() == unit->get_type()))
            {
                removedelaylist.push_back(delayevent);
            }
        }
    }

    for (std::shared_ptr<delay_event> & delayevent : removedelaylist)
    {
        DelayEventList.remove(delayevent);
    }
    delayMutex.unlock();
}

bool map::RegisterDelayEvent(int iDelayType, int iEffectType, uint64_t dwLastTime, std::shared_ptr<unit> unit, int dX, int dY, int iV1, int iV2, int iV3)
{
    std::shared_ptr<delay_event> delayevent = std::make_shared<delay_event>();


    delayevent->m_iDelayType = iDelayType;
    delayevent->m_iEffectType = iEffectType;

    delayevent->m_dX = dX;
    delayevent->m_dY = dY;

    delayevent->target = unit;
    delayevent->m_iV1 = iV1;
    delayevent->m_iV2 = iV2;
    delayevent->m_iV3 = iV3;

    delayevent->m_dwTriggerTime = dwLastTime;

    delayMutex.lock();
    DelayEventList.push_back(delayevent);
    delayMutex.unlock();

    return true;
}

void map::RemoveFromTarget(std::shared_ptr<unit> target, int iCode)
{
    auto dwTime = utility::current_time();

    for (const std::shared_ptr<npc> & npc : npclist)
    {
        //         if ((npc->guildGUID != 0) && (target->IsPlayer()) && (target->guildGUID == npc->guildGUID))
        //         {
        //             if (npc->m_cActionLimit == 0)
        //             {
        //                 npc->summoned = true;
        //                 npc->timeSummoned = dwTime;
        //             }
        //         }

        if (npc->target == target)
        {
            switch (iCode)
            {
                case MAGICTYPE_INVISIBILITY:
                    if (npc->specialAbility == 1)
                    {
                    }
                    else
                    {
                        npc->m_cBehavior = BEHAVIOR_MOVE;
                        npc->target = nullptr;
                    }
                    break;
                default:
                    npc->m_cBehavior = BEHAVIOR_MOVE;
                    npc->target = nullptr;
                    break;
            }
        }
    }
}

void map::NpcDeadItemGenerator(std::shared_ptr<unit> attacker, std::shared_ptr<npc> npc_) const
{

}
/*
void Map::NpcDeadItemGenerator(shared_ptr<Unit> attacker, shared_ptr<Npc> npc) const
{
    Item * pItem = nullptr;
    char  cTemp[20];
    string itemName;
    bool  bIsGold;
    int   iGenLevel = 0, iResult, iItemID = 0;
    double dTmp1, dTmp2, dTmp3;
    uint32_t dwValue;
    SYSTEMTIME SysTime;
    uint64_t time = unixtime();

    if (!npc || !attacker) return;

    if (attacker->IsNPC() || npc->summoned) return;

    Client * player = attacker->GetPlayer();

    bIsGold = false;

    switch (npc->Type()) {
    case NPC_GUARD:
    case NPC_DUMMY:
    case NPC_AGT:
    case NPC_CGT:
    case NPC_DT:
        return;
    }

//     if (NpcDeadItemGeneratorWithItemEvent(iNpcH, sAttackerH, cAttackerType) == true)
//         return;

    int iItemprobability = 8000;

    if (player->GetPartyStatus() == PS_INPARTY)
    {
        iItemprobability += 650;
    }

    if (player->HasPartyHuntBonus())
    {
        iItemprobability += 650;
    }

    if (player->IsHeldWinner())
    {
        iItemprobability += 300;
    }

//     if (m_eventWinner[ET_CAPTURE] == player->side)
//     {
//         iItemprobability += 300;
//     }

    if (npc->IsHighlyTrained())
        iItemprobability += 1500;

//     Client * killer = npc->GetKiller();
//     if (killer)
//         iItemprobability += 1000 * (1 - killer->GetDropFactor());

    if (iItemprobability > dice(1, 10000))
    {

        if (dice(1, 10000) <= 200)// Gold Drop Rate
            //- ((killer && killer->HasPartyHuntBonus()) ? 300 : 0)
            //- (npc->IsHighlyTrained() ? 700 : 0)
            //- ( (killer != NULL) ? 300*2 * ( 1 - killer->GetDropFactor() ) : 0) )
        {
            //iItemID = 90; // Gold
            itemName = "Gold";

            pItem = new Item(itemName, gserver->m_pItemConfigList);
            if (pItem->initialized == false)
            {
                delete pItem;
                return;
            }
            // Gold ratio
            if (npc->goldDropValue == 0)
            {
                delete pItem;
                return;
            }
            else if (npc->goldDropValue <= 4) pItem->count = dice(1, npc->goldDropValue);
            else
                switch (dice(1, 2))
            {
                case 1:
                    pItem->count = uint32_t(npc->goldDropValue + dice(1, npc->goldDropValue / 5));
                    break;

                case 2:
                    pItem->count = uint32_t(npc->goldDropValue - dice(1, npc->goldDropValue / 5));
                    break;
            }

            //if npc had an attribute, double gold
            if (npc->specialAbility != 0) pItem->count *= 2;

            // check for items that give +gold%
            if (player->m_iAddGold != 0) {
                pItem->count += int((player->m_iAddGold / 100.0f) * pItem->count);
            }
        }
        else
        {
//             if ((npc->Type() == NPC_HELLCLAW || npc->Type() == NPC_TIGERWORM) && drops.HasPrimaryDrop(npc))
//             {
//                 iItemID = drops.Roll(npc);
//
//                 if (iItemID == ITEM_NONE) return;
//
//                 pItem = new Item(iItemID, gserver->m_pItemConfigList);
//                 if (!pItem->initialized)
//                 {
//                     delete pItem;
//                     return;
//                 }
//             }
            if (dice(1, 10000) <= 500) // Primary Drop Rate
                //- ((killer && killer->HasPartyHuntBonus()) ? 200 : 0)
                //- (npc->IsHighlyTrained() ? 900 : 0)
                //- ((killer != NULL) ? 200*2 * ( 1 - killer->GetDropFactor() ) : 0))
            {
                // pots/zem/stones/etc...
                //int reduction = npc->IsHighlyTrained() ? 5000 : 0;
                //reduction += (killer != NULL) ? 1750*2 * ( 1 - killer->GetDropFactor() ) : 0;
                //reduction += (m_pClientList[sAttackerH]->HasPartyHuntBonus()) ? 1500 : 0;

                iResult = dice(1, 22500 / *- reduction* /)/ * + reduction* /;
                if ((iResult >= 1) && (iResult <= 5500))          dwValue = 1;
                else if ((iResult >= 5501) && (iResult <= 11000))  dwValue = 2;
                else if ((iResult >= 11001) && (iResult <= 17200))  dwValue = 3;
                else if ((iResult >= 17201) && (iResult <= 18650))  dwValue = 4;
                else if ((iResult >= 18651) && (iResult <= 19950))  dwValue = 5;
                else if ((iResult >= 19951) && (iResult <= 19996))  dwValue = 6;
                else if ((iResult >= 19997) && (iResult <= 19998))  dwValue = 7;
                else if ((iResult >= 19999) && (iResult <= 20000))  dwValue = 8;
                else if ((iResult >= 20001) && (iResult <= 22500))    dwValue = 9;
                else dwValue = 0;

                switch (dwValue) {
                case 1: iItemID = 780; break; // Candy Health Potion
                case 2: iItemID = 781; break; // Candy Mana Potion
                case 3: iItemID = 782; break; // Candy Sp Potion
                case 4: iItemID = 1110; break; // Critical Potion
                case 5: iItemID = 1412; break; // 250 Gold Pocket
                case 6: iItemID = 650; break; // Zem Stone
                case 7: iItemID = 656; break; // Xelima Stone
                case 8: iItemID = 657; break; // Merien Stone
                case 9: iItemID = 1592; break; // Lucky Potion
                }

                pItem = new Item(iItemID, gserver->m_pItemConfigList);
                if (!pItem->initialized)
                {
                    delete pItem;
                    return;
                }
            }
            else{
                iGenLevel = npc->GetGenLevel();

                if (iGenLevel == 0) return;

/ *
                if (drops.HasPrimaryDrop(npc))
                {
                    iItemID = drops.Roll(npc);

                    //if(iItemID == ITEM_NONE) return;
                }
                else* / if (dice(1, 10000) <= 2600) { // Weapon Drops Rate 3400
                    // weapons
                    //if (dice(1,10000) <= 5000) { // Wand? Drop Rate? 3000
                    // regular
                    switch (iGenLevel) {
                    case 1:
                        switch (dice(1, 2)) {
                        case 1: iItemID = 1300; break;   // Beginner Sword
                        case 2: iItemID = 1303; break;   // Beginner Axe
                        }
                        break;

                    case 2:
                        switch (dice(1, 2)) {
                        case 1: iItemID = 1301; break;  // Beginner Long Sword
                        case 2: iItemID = 1302; break;  // Beginner Raiper
                            //case 3: iItemID = 1305; break;  // Beginner ShortBow
                        }
                        break;

                    case 3:
                        switch (dice(1, 2)) {
                        case 1: iItemID = 1306; break;  // Great Sword
                        case 2: iItemID = 1307; break;  // Esterk
                            //case 3: iItemID = 1311; break;  // Short Bow
                        }
                        break;

                    case 4:
                        switch (dice(1, 2)) {
                        case 1: iItemID = 1308; break;   // War Axe
                        case 2: iItemID = 1307; break;   // Hammer
                            //case 3: iItemID = 1312; break;   // Long Bow
                        }
                        break;

                    case 5:
                        switch (dice(1, 5)) { // Master
                        case 1: iItemID = 1313; break;   // Flamberge
                        case 2: iItemID = 1314; break;   // Esterk
                        case 3: iItemID = 1315; break;   // WarAxe
                        case 4: iItemID = 1316; break;   // Rapier
                        case 5: iItemID = 1317; break;   // Battle Hammer
                            //case 6: iItemID = 1319; break;   // Short Bow
                            //case 7: iItemID = 1320; break;   // Long Bow
                            //case 8: iItemID = 1321; break;   // Composite Bow
                        }
                        break;

                    case 6:
                        switch (dice(1, 5)) { // Master
                        case 1: iItemID = 1313; break;   // Flamberge
                        case 2: iItemID = 1314; break;   // Esterk
                        case 3: iItemID = 1315; break;   // WarAxe
                        case 4: iItemID = 1316; break;   // Rapier
                        case 5: iItemID = 1317; break;   // Battle Hammer
                            //case 6: iItemID = 1319; break;   // Short Bow
                            //case 7: iItemID = 1320; break;   // Long Bow
                            //case 8: iItemID = 1321; break;   // Composite Bow
                        }
                        break;

                    case 7:
                        switch (dice(1, 5)) { // Master
                        case 1: iItemID = 1313; break;   // Flamberge
                        case 2: iItemID = 1314; break;   // Esterk
                        case 3: iItemID = 1315; break;   // WarAxe
                        case 4: iItemID = 1316; break;   // Rapier
                        case 5: iItemID = 1317; break;   // Battle Hammer
                            //case 6: iItemID = 1319; break;   // Short Bow
                            //case 7: iItemID = 1320; break;   // Long Bow
                            //case 8: iItemID = 1321; break;   // Composite Bow
                        }
                        break;

                    case 8:
                        switch (dice(1, 5)) { // Master
                        case 1: iItemID = 1313; break;   // Flamberge
                        case 2: iItemID = 1314; break;   // Esterk
                        case 3: iItemID = 1315; break;   // WarAxe
                        case 4: iItemID = 1316; break;   // Rapier
                        case 5: iItemID = 1317; break;   // Battle Hammer
                            //case 6: iItemID = 1319; break;   // Short Bow
                            //case 7: iItemID = 1320; break;   // Long Bow
                            //case 8: iItemID = 1321; break;   // Composite Bow
                        }
                        break;

                    case 9:
                        switch (dice(1, 5)) { // Master
                        case 1: iItemID = 1313; break;   // Flamberge
                        case 2: iItemID = 1314; break;   // Esterk
                        case 3: iItemID = 1315; break;   // WarAxe
                        case 4: iItemID = 1316; break;   // Rapier
                        case 5: iItemID = 1317; break;   // Battle Hammer
                            //case 6: iItemID = 1319; break;   // Short Bow
                            //case 7: iItemID = 1320; break;   // Long Bow
                            //case 8: iItemID = 1321; break;   // Composite Bow
                            //default: break;
                        }
                        break;

                    case 10: // Not in use
                        break;

                    }
                }
                else if (dice(1, 10000) <= 600) { // Wand? Drop Rate? 3000{
                    //if (dice(1,10000) <= 5000) { // Wand? Drop Rate? 3000
                    switch (iGenLevel) {
                    case 1: iItemID = 1304;    break; // Beginner Wand (MS0)
                    case 2: iItemID = 1304;    break; // Beginner Wand (MS0)
                    case 3:    iItemID = 1310; break;  // (MS10)
                    case 4: iItemID = 1310; break;  // (MS10)
                    case 5: iItemID = 1310; break;  // (MS10)
                    case 6: iItemID = 1310; break;  // (MS10)
                    case 7: iItemID = 1318; break;  // (MS20)
                    case 8: iItemID = 1318; break;  // (MS20)
                    case 9: iItemID = 1318; break;  // (MS20)
                    }
                }
                //}
                else {
                    switch (iGenLevel) { // Remove dice(1,) xRisenx
                    case 1:
                        switch (dice(1, 11)) {
                        case 1: iItemID = 1150; break; // Beginner Hauberk M
                        case 2: iItemID = 1151; break; // Beginner Hauberk W
                        case 3: iItemID = 1152; break; // Beginner Chain Hose M
                        case 4: iItemID = 1153; break; // Beginner Chain Hose W
                        case 5: iItemID = 1158; break; // Beginner Helm M
                        case 6: iItemID = 1159; break; // Beginner Helm W
                        case 7: iItemID = 1172; break; // Beginner Chain Mail M
                        case 8: iItemID = 1173; break; // Beginner Chain Mail W
                        case 9: iItemID = 1160; break; // Beginner Full Helm M
                        case 10: iItemID = 1161; break; // Beginner Full Helm W
                        case 11: iItemID = 79; break; // Wood Shield
                        }
                        break;
                    case 2:
                        switch (dice(1, 15)) {
                        case 1: iItemID = 1150; break; // Beginner Hauberk M
                        case 2: iItemID = 1151; break; // Beginner Hauberk W
                        case 3: iItemID = 1154; break; // Beginner Plate Leggings M
                        case 4: iItemID = 1155; break; // Beginner Plate Leggings W
                        case 5: iItemID = 1156; break; // Beginner Plate Mail M
                        case 6: iItemID = 1157; break; // Beginner Plate Mail W
                        case 7: iItemID = 1162; break; // Beginner Winged Helm M
                        case 8: iItemID = 1163; break; // Beginner Winged Helm W
                        case 9: iItemID = 1164; break; // Beginner Horned Helm M
                        case 10: iItemID = 1165; break; // Beginner Horned Helm W
                            //case 11: iItemID = 1166; break; // Beginner Wizard Hauberk M
                            //case 12: iItemID = 1167; break; // Beginner Wizard Hauberk W
                        case 11: iItemID = 1168; break; // Beginner Robe M
                        case 12: iItemID = 1169; break; // Beginner Robe W
                        case 13: iItemID = 1170; break; // Beginner Hat M
                        case 14: iItemID = 1171; break; // Beginner Hat W
                        case 15: iItemID = 81; break; // Targe Shield
                        }
                        break;

                    case 3:
                        switch (dice(1, 13)) {
                        case 1: iItemID = 1174; break; // Knight Hauberk
                        case 2: iItemID = 1175; break; // Dame Hauberk
                        case 3: iItemID = 1176; break; // Knight Chain Hose
                        case 4: iItemID = 1177; break; // Dame Chain Hose
                        case 5: iItemID = 1182; break; // Knight Helm
                        case 6: iItemID = 1183; break; // Dame Helm
                        case 7: iItemID = 1184; break; // Knight Full Helm
                        case 8: iItemID = 1185; break; // Dame Full Helm
                            //case 9: iItemID = 1190; break; // Knight Wizard Hauberk
                            //case 10: iItemID = 1191; break; // Dame Wizard Hauberk
                        case 9: iItemID = 1192; break; // Knight Robe
                        case 10: iItemID = 1193; break; // Dame Robe
                        case 11: iItemID = 1196; break; // Knight Chain Mail
                        case 12: iItemID = 1197; break; // Dame Chain Mail
                        case 13: iItemID = 82; break; // Scooterm Shield
                        }
                        break;

                    case 4:
                        switch (dice(1, 15)) {
                        case 1: iItemID = 1174; break; // Knight Hauberk
                        case 2: iItemID = 1175; break; // Dame Hauberk
                        case 3: iItemID = 1176; break; // Knight Plate Leggings
                        case 4: iItemID = 1177; break; // Dame Plate Leggings
                        case 5: iItemID = 1178; break; // Knight Plate Mail
                        case 6: iItemID = 1179; break; // Dame Plate Mail
                        case 7: iItemID = 1186; break; // Knight Winged Helm
                        case 8: iItemID = 1187; break; // Dame Winged Helm
                        case 9: iItemID = 1188; break; // Knight Horned Helm
                        case 10: iItemID = 1189; break; // Dame Horned Helm
                        case 11: iItemID = 1192; break; // Knight Robe
                        case 12: iItemID = 1193; break; // Dame Robe
                        case 13: iItemID = 1194; break; // Knight Hat
                        case 14: iItemID = 1195; break; // Dame Hat
                        case 15: iItemID = 83; break; // Blonde Shield
                        }
                        break;

                    case 5: // Master
                        switch (dice(1, 23)) {
                        case 1: iItemID = 1198; break; // Hauberk M
                        case 2: iItemID = 1199; break; // Hauberk W
                        case 3: iItemID = 1200; break; // Chain Hose M
                        case 4: iItemID = 1201; break; // Chain Hose W
                        case 5: iItemID = 1202; break; // Plate Leggings M
                        case 6: iItemID = 1203; break; // Plate Leggings W
                        case 7: iItemID = 1204; break; // Plate Mail M
                        case 8: iItemID = 1205; break; // Plate Mail W
                        case 9: iItemID = 1206; break; // Penis Helm M
                        case 10: iItemID = 1207; break; // Penis Helm W
                        case 11: iItemID = 1208; break; // Full Helm M
                        case 12: iItemID = 1209; break; // Full Helm W
                        case 13: iItemID = 1210; break; // Winged Helm M
                        case 14: iItemID = 1211; break; // Winged Helm W
                        case 15: iItemID = 1212; break; // Horned Helm M
                        case 16: iItemID = 1213; break; // Horned Helm W
                            //case 17: iItemID = 1214; break; // Wizard Hauberk M
                            //case 18: iItemID = 1215; break; // Wizard Hauberk W
                        case 17: iItemID = 1216; break; // Robe M
                        case 18: iItemID = 1217; break; // Robe W
                        case 19: iItemID = 1218; break; // Hat M
                        case 20: iItemID = 1219; break; // Hat W
                        case 21: iItemID = 1220; break; // Chain Mail M
                        case 22: iItemID = 1221; break; // Chain Mail W
                        case 23: iItemID = 84; break; // Iron Shield
                        }
                        break;

                    case 6: // Master
                        switch (dice(1, 23)) {
                        case 1: iItemID = 1198; break; // Hauberk M
                        case 2: iItemID = 1199; break; // Hauberk W
                        case 3: iItemID = 1200; break; // Chain Hose M
                        case 4: iItemID = 1201; break; // Chain Hose W
                        case 5: iItemID = 1202; break; // Plate Leggings M
                        case 6: iItemID = 1203; break; // Plate Leggings W
                        case 7: iItemID = 1204; break; // Plate Mail M
                        case 8: iItemID = 1205; break; // Plate Mail W
                        case 9: iItemID = 1206; break; // Penis Helm M
                        case 10: iItemID = 1207; break; // Penis Helm W
                        case 11: iItemID = 1208; break; // Full Helm M
                        case 12: iItemID = 1209; break; // Full Helm W
                        case 13: iItemID = 1210; break; // Winged Helm M
                        case 14: iItemID = 1211; break; // Winged Helm W
                        case 15: iItemID = 1212; break; // Horned Helm M
                        case 16: iItemID = 1213; break; // Horned Helm W
                            //case 17: iItemID = 1214; break; // Wizard Hauberk M
                            //case 18: iItemID = 1215; break; // Wizard Hauberk W
                        case 17: iItemID = 1216; break; // Robe M
                        case 18: iItemID = 1217; break; // Robe W
                        case 19: iItemID = 1218; break; // Hat M
                        case 20: iItemID = 1219; break; // Hat W
                        case 21: iItemID = 1220; break; // Chain Mail M
                        case 22: iItemID = 1221; break; // Chain Mail W
                        case 23: iItemID = 85; break; // Lagi Shield
                        }
                        break;

                    case 7: // Emperor/Empress
                        switch (dice(1, 8)) {
                        case 1: iItemID = 1222; break; // Hauberk M
                        case 2: iItemID = 1223; break; // Hauberk W
                        case 3: iItemID = 1224; break; // Chain Hose M
                        case 4: iItemID = 1225; break; // Chain Hose W
                        case 5: iItemID = 1230; break; // Penis Helm M
                        case 6: iItemID = 1231; break; // Penis Helm W
                            //case 7: iItemID = 1238; break; // Wizard Hauberk M
                            //case 8: iItemID = 1239; break; // Wizard Hauberk W
                        case 7: iItemID = 86; break; // Knight Shield
                        case 8: iItemID = 450; break; // Shoes
                        }
                        break;

                    case 8: // Emperor/Empress
                        switch (dice(1, 14)) {
                        case 1: iItemID = 1222; break; // Hauberk M
                        case 2: iItemID = 1223; break; // Hauberk W
                        case 3: iItemID = 1224; break; // Chain Hose M
                        case 4: iItemID = 1225; break; // Chain Hose W
                        case 5: iItemID = 1230; break; // Penis Helm M
                        case 6: iItemID = 1231; break; // Penis Helm W
                            //case 7: iItemID = 1238; break; // Wizard Hauberk M
                            //case 8: iItemID = 1239; break; // Wizard Hauberk W
                        case 7: iItemID = 1232; break; // Full Helm M
                        case 8: iItemID = 1233; break; // Full Helm W
                        case 9: iItemID = 1244; break; // Chain Mail M
                        case 10: iItemID = 1245; break; // Chain Mail W
                        case 11: iItemID = 1240; break; // Robe M
                        case 12: iItemID = 1241; break; // Robe W
                        case 13: iItemID = 87; break; // Tower Shield
                        case 14: iItemID = 451; break; // Long Boots
                        }
                        break;

                    case 9: // Emperor/Empress
                        switch (dice(1, 20)) {
                        case 1: iItemID = 1222; break; // Hauberk M
                        case 2: iItemID = 1223; break; // Hauberk W
                        case 3: iItemID = 1224; break; // Chain Hose M
                        case 4: iItemID = 1225; break; // Chain Hose W
                        case 5: iItemID = 1226; break; // Plate Leggings M
                        case 6: iItemID = 1227; break; // Plate Leggings W
                        case 7: iItemID = 1228; break; // Plate Mail M
                        case 8: iItemID = 1229; break; // Plate Mail W
                        case 9: iItemID = 1234; break; // Winged Helm M
                        case 10: iItemID = 1235; break; // Winged Helm W
                        case 11: iItemID = 1236; break; // Horned Helm M
                        case 12: iItemID = 1237; break; // Horned Helm W
                            //case 13: iItemID = 1238; break; // Wizard Hauberk M
                            //case 14: iItemID = 1239; break; // Wizard Hauberk W
                        case 13: iItemID = 1240; break; // Robe M
                        case 14: iItemID = 1241; break; // Robe W
                        case 15: iItemID = 1242; break; // Hat M
                        case 16: iItemID = 1243; break; // Hat W
                        case 17: iItemID = 1244; break; // Chain Mail M
                        case 18: iItemID = 1245; break; // Chain Mail W
                        case 19: iItemID = 87; break; // Tower Shield
                        case 20: iItemID = 402; break; // Cape
                        }
                        break;
                    }
                }

                if (!iItemID)
                    return;

                pItem = new Item(iItemID, gserver->m_pItemConfigList);
                if (!pItem->initialized)
                {
                    delete pItem;
                    return;
                }

//                 if (bCheckInItemEventList(iItemID, iNpcH) == true) {
//                     delete pItem;
//                     return;
//                 }
            }
        }

        pItem->InitStats(npc->GetGenLevel());


        pItem->effectType = ITET_ID;
        pItem->effectV1 = dice(1, 100000);
        pItem->effectV2 = dice(1, 100000);
#ifdef LOGTIME
        pItem->m_sTouchEffectValue3 = unixtime();
#else
        GetLocalTime(&SysTime);
        ZeroMemory(cTemp, sizeof(cTemp));
        //wsprintf(cTemp, "%d%02d%02d",  (short)SysTime.wMonth, (short)SysTime.wDay,(short) SysTime.wHour);
        wsprintf(cTemp, "%d%02d%", short(SysTime.wMonth), short(SysTime.wDay));

        pItem->effectV3 = atoi(cTemp);
#endif

        if (npc->map->iCheckItem(
            npc->x, npc->y) == ITEMTYPE_RELIC)
        {
            delete pItem;
        }
        else
        {
            npc->map->bSetItem(npc->x,
                npc->y,
                pItem);


            gserver->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, COMMONTYPE_ITEMDROP, npc->map,
                npc->x, npc->y,
                pItem->spriteID, pItem->spriteFrame, pItem->color);

            //_bItemLog(ITEMLOG_NEWGENDROP, 0, npc->m_cNpcName, pItem);

            gserver->AddGroundItem(pItem, npc->x, npc->y, npc->map, TILECLEANTIME);
        }
    }
}
*/

/*
void Map::NpcDeadItemGenerator(shared_ptr<Unit> attacker, shared_ptr<Npc> npc)
{
    if (!npc || !attacker) return;
    if (!gserver->m_npcDropData[npc->Type()]) {
        gserver->logger->information(Poco::format("NPC has no drop data: %d", npc->Type()));
    }
    uint32_t dwTime = unixtime();
    srand(time(0));
    // Select a random item
    int baseChance = rand() % 9999 + 1;
    int currentChance = baseChance;
    int tierMultiplier = 10000;
    int rareChance = rand() % 10 + 1;
    int epicChance = rand() % 100 + 1;
    int godChance = rand() % 1000 + 1;
    if (rareChance == 10) {
        currentChance += 10000;
    }
    if (epicChance == 100) {
        currentChance += 20000;
    }
    if (godChance == 1000) {
        currentChance += 30000;
    }

    int npcItems[MAXITEMTYPES];
    for (int ni = 0; ni < MAXITEMTYPES; ni++) {
        npcItems[ni] = 0;
    }
    int npcItemCount = 0;
    for (int nif = 0; nif < MAXITEMTYPES; nif++) {
        if (gserver->m_npcDropData[npc->Type()][nif] != 0) {
            npcItems[npcItemCount] = nif;
            npcItemCount++;
        }
    }
    gserver->logger->information(Poco::format("Item drop rolling. Chance: %d, Items: %d", currentChance, npcItemCount));
    for (int ii = 0; ii <= npcItemCount; ii++) {
        if (gserver->m_pItemConfigList[npcItems[ii]] != 0) {
            int aProb = gserver->m_npcDropData[npc->Type()][npcItems[ii]];
            if (aProb < currentChance) {
                int itemID = npcItems[ii];
                // Winner, winner, chicken dinner
                if (gserver->m_pItemConfigList[itemID] != 0 && gserver->m_pItemConfigList[itemID]->name != "") {
                    Item * newItem = new Item(itemID, gserver->m_pItemConfigList);
                    bSetItem(npc->x, npc->y, newItem);
                    gserver->logger->information(Poco::format("Dropping %s for from npc #%?d",
                        gserver->m_pItemConfigList[npcItems[ii]]->name, attacker->m_uid));
                    gserver->SendEventToNearClient_TypeB(
                        MSGID_EVENT_COMMON, COMMONTYPE_ITEMDROP, attacker->map,
                        npc->x, npc->y, newItem->spriteID, newItem->spriteFrame, newItem->color);

//                     gserver->SendNotifyMsg(nullptr, (Client*)attacker.get(),
//                         NOTIFY_DROPITEMFIN_COUNTCHANGED, itemID, 1, 0);
                    return;
                }
            }
        }
    }
}
*/

void map::NpcKilledHandler(std::shared_ptr<unit> attacker, std::shared_ptr<npc> npc, int16_t damage) const
{
    if (npc->dead_) return;

    npc->behavior_death(attacker, damage);

    if (attacker && attacker->is_player())
    {
        //         if (m_pClientList[sAttackerH] != NULL)
        //         {
        //             if (m_bNpcHunt)
        //             {
        //                 NpcHuntPointsAdder(sAttackerH);
        //             }
        //         }
    }
    // Monster kill event xRisenx

//     if (attacker->m_ownerType == OWNERTYPE_PLAYER)
//     {
//         shared_ptr<Client> player = static_cast<Client*>(attacker.get())->self.lock();
//         switch (npc->m_sType) {
//         case NPC_ENRAGED_HELLCLAW: // Hellclaw 
//             if (player->_str > 400 && player->_dex > 400) // Warrior
//             {
//                 player->m_iMonsterCount += 1;
//                 if (player->m_iMonsterCount > 600)
//                     player->m_iMonsterCount = 600;
//             }
//             else if (player->_int > 400 && player->_mag > 400) // Mage
//             {
//                 player->m_iMonsterCount += 2;
//                 if (player->m_iMonsterCount > 600)
//                     player->m_iMonsterCount = 600;
//             }
//             break;
// 
// 
//         case NPC_ENRAGED_TIGERWORM: // Tigerworm
//             if (player->_str > 400 && player->_dex > 400) // Warrior
//             {
//                 player->m_iMonsterCount += 1;
//                 if (player->m_iMonsterCount > 600)
//                     player->m_iMonsterCount = 600;
//             }
//             else if (player->_int > 400 && player->_mag > 400) // Mage
//             { // Mage 
//                 player->m_iMonsterCount += 2;
//                 if (player->m_iMonsterCount > 600)
//                     player->m_iMonsterCount = 600;
//             }
//             break;
// 
//         default:
//             break;
//     }
}

void map::NpcBehavior_Dead(std::shared_ptr<npc> npc)
{
    uint32_t dwTime;

    if (npc == nullptr) return;

    dwTime = utility::current_time();
    npc->m_sBehaviorTurnCount++;
    if (npc->m_sBehaviorTurnCount > 5) {
        npc->m_sBehaviorTurnCount = 0;
    }

    if ((dwTime - npc->timeDead) > npc->timeRegen)
        DeleteNpc(npc);
}

void map::NpcBehavior_Flee(std::shared_ptr<npc> npc_) const
{
    char cDir;
    short sX, sY, dX, dY;
    std::shared_ptr<unit> sTarget;

    if (npc_ == nullptr) return;
    if (npc_->dead_ == true) return;

    npc_->m_sBehaviorTurnCount++;


    switch (npc_->m_iAttackStrategy) {
        case ATTACKAI_EXCHANGEATTACK:
        case ATTACKAI_TWOBYONEATTACK:
            if (npc_->m_sBehaviorTurnCount >= 2) {

                npc_->m_cBehavior = BEHAVIOR_ATTACK;
                npc_->m_sBehaviorTurnCount = 0;
                return;
            }
            break;

        default:
            //if (dice(1, 2) == 1) NpcRequestAssistance(iNpcH);
            break;
    }

    if (npc_->m_sBehaviorTurnCount > 10) {

        npc_->m_sBehaviorTurnCount = 0;
        npc_->m_cBehavior = BEHAVIOR_MOVE;
        npc_->m_tmp_iError = 0;
        if (npc_->health <= 3) {
            npc_->health += utility::gen_rand(1, npc_->m_iHitDice);
            if (npc_->health <= 0) npc_->health = 1;
        }
        return;
    }

    sTarget = npc_->TargetSearch();
    if (sTarget != nullptr) {
        npc_->target = sTarget;
        npc_->targetType = sTarget->get_owner_type();
    }

    sX = npc_->x;
    sY = npc_->y;

    dX = npc_->target->x;
    dY = npc_->target->y;

    dX = sX - (dX - sX);
    dY = sY - (dY - sY);

    cDir = cGetNextMoveDir(sX, sY, dX, dY, npc_->map_, npc_->turn, &npc_->m_tmp_iError);
    if (cDir == 0) {
    }
    else {
        dX = npc_->x + _tmp_cTmpDirX[cDir];
        dY = npc_->y + _tmp_cTmpDirY[cDir];
        npc_->map_->ClearOwner(npc_->x, npc_->y);

        npc_->map_->SetOwner(npc_, dX, dY);
        npc_->x = dX;
        npc_->y = dY;
        npc_->direction = cDir;
        gs->send_event_near_type_a(npc_, MSGID_MOTION_MOVE, 0, 0, 0);
    }
}

char map::cGetNextMoveDir(short sX, short sY, short dstX, short dstY, map * map_, char cTurn, int * pError) const
{
    char  cDir, cTmpDir;
    int   aX, aY, dX, dY;
    int   i, iResX, iResY;

    if ((sX == dstX) && (sY == dstY)) return 0;

    dX = sX;
    dY = sY;

    if ((abs(dX - dstX) <= 1) && (abs(dY - dstY) <= 1)) {
        iResX = dstX;
        iResY = dstY;
    }
    else utility::GetPoint(dX, dY, dstX, dstY, &iResX, &iResY, pError);

    cDir = utility::cGetNextMoveDir(dX, dY, iResX, iResY);

    if (cTurn == 0)
        for (i = cDir; i <= cDir + 7; i++) {
            cTmpDir = i;
            if (cTmpDir > 8) cTmpDir -= 8;
            aX = _tmp_cTmpDirX[cTmpDir];
            aY = _tmp_cTmpDirY[cTmpDir];
            if (map_->bGetMoveable(dX + aX, dY + aY) == true) return cTmpDir;
        }

    if (cTurn == 1)
        for (i = cDir; i >= cDir - 7; i--) {
            cTmpDir = i;
            if (cTmpDir < 1) cTmpDir += 8;
            aX = _tmp_cTmpDirX[cTmpDir];
            aY = _tmp_cTmpDirY[cTmpDir];
            if (map_->bGetMoveable(dX + aX, dY + aY) == true) return cTmpDir;
        }

    return 0;
}

char map::cGetNextMoveDir(short sX, short sY, short dstX, short dstY, map * map_, char cTurn, int * pError, short * DOType) const
{
    char  cDir, cTmpDir;
    int   aX, aY, dX, dY;
    int   i, iResX, iResY;

    if ((sX == dstX) && (sY == dstY)) return 0;

    dX = sX;
    dY = sY;

    if ((abs(dX - dstX) <= 1) && (abs(dY - dstY) <= 1)) {
        iResX = dstX;
        iResY = dstY;
    }
    else utility::GetPoint(dX, dY, dstX, dstY, &iResX, &iResY, pError);

    cDir = utility::cGetNextMoveDir(dX, dY, iResX, iResY);

    if (cTurn == 0)
        for (i = cDir; i <= cDir + 7; i++) {
            cTmpDir = i;
            if (cTmpDir > 8) cTmpDir -= 8;
            aX = _tmp_cTmpDirX[cTmpDir];
            aY = _tmp_cTmpDirY[cTmpDir];
            if (map_->bGetMoveable(dX + aX, dY + aY, DOType) == true) return cTmpDir;
        }

    if (cTurn == 1)
        for (i = cDir; i >= cDir - 7; i--) {
            cTmpDir = i;
            if (cTmpDir < 1) cTmpDir += 8;
            aX = _tmp_cTmpDirX[cTmpDir];
            aY = _tmp_cTmpDirY[cTmpDir];
            if (map_->bGetMoveable(dX + aX, dY + aY, DOType) == true) return cTmpDir;
        }

    return 0;
}

bool map::bGetEmptyPosition(int16_t & pX, int16_t & pY, std::shared_ptr<unit> client_)
{
    int i;
    int16_t sX, sY;

    for (i = 0; i < 25; i++)
        if ((bGetMoveable(pX + _tmp_cEmptyPosX[i], pY + _tmp_cEmptyPosY[i]) == true) &&
            (bGetIsTeleport(pX + _tmp_cEmptyPosX[i], pY + _tmp_cEmptyPosY[i]) == false))
        {
            pX += _tmp_cEmptyPosX[i];
            pY += _tmp_cEmptyPosY[i];
            return true;
        }

    if (client_->is_player())
        GetInitialPoint(sX, sY, std::static_pointer_cast<client>(client_)->nation);
    else
    {
        std::string npt("NONE");
        GetInitialPoint(sX, sY, npt);
    }
    return false;
}

void map::DelayEventProcessor()
{
    auto _time = utility::current_time();
    delayMutex.lock();
    std::list<std::shared_ptr<delay_event>> tempDelayEventList = DelayEventList;
    for (const auto & delayevent : tempDelayEventList)
    {
        if (delayevent->m_dwTriggerTime < _time)
        {
            switch (delayevent->m_iDelayType)
            {
                case DELAYEVENTTYPE_CALCMETEORSTRIKEEFFECT:
                    //CalcMeteorStrikeEffectHandler(delayevent->map);//good ol' HB handlers for handling handlers
                    break;

                case DELAYEVENTTYPE_DOMETEORSTRIKEDAMAGE:
                    //DoMeteorStrikeDamageHandler(delayevent->map);
                    break;

                case DELAYEVENTTYPE_METEORSTRIKE:
                    //MeteorStrikeHandler(delayevent->map);
                    break;

                case DELAYEVENTTYPE_USEITEM_SKILL:
                    if (delayevent->target != nullptr && delayevent->target->is_player())
                    {
                        int skillnum = delayevent->m_iEffectType;

                        std::shared_ptr<client> player = std::static_pointer_cast<client>(delayevent->target);

                        if (player->skillInUse[skillnum] == 0 || player->skillInUse[skillnum] != delayevent->m_iV2) break;

                        player->skillInUse[skillnum] = false;
                        player->skillInUseTime[skillnum] = 0;

                        int32_t result = gs->CalculateUseSkillItemEffect(player, delayevent->m_iV1, skillnum, player->map_, delayevent->m_dX, delayevent->m_dY);

                        gs->SendNotifyMsg(nullptr, player, NOTIFY_SKILLUSINGEND, result);
                    }
                    break;

                case DELAYEVENTTYPE_DAMAGEOBJECT:
                    break;
                case DELAYEVENTTYPE_MAGICRELEASE:
                    break;
            }

            DelayEventList.remove(delayevent);
        }
    }
    delayMutex.unlock();
}

}
