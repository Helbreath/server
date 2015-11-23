
#include "Map.h"
#include "Unit.h"
#include "Client.h"
#include "Npc.h"
#include "Tile.h"
#include "GServer.h"
#include "Misc.h"
#include <fstream>
#include "streams.h"
//#include "OccupyFlag.h"
//#include "StrategicPoint.h"
#include "TeleportLoc.h"
#include <lua5.2/lua.hpp>
#include <lua5.2/lauxlib.h>

#pragma region movedirection
//#ifndef 800x600
int _tmp_iMoveLocX[9][44] = {
	// 0
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//done
		// 1 NORTH
		{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
		20, 21, 22, 23, 24, -1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//done
		//2
		{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
		20, 21, 22, 23, 24, 24, 24, 24, 24, 24,
		24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, -1 },//done
		//3 EAST
		{ 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
		24, 24, 24, 24, 24, 24, 24, 24, -1, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//done
		//4
		{ 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
		24, 24, 24, 24, 24, 24, 24, 24, 24, 23, 22, 21,
		20, 19, 18, 17, 16, 15, 14, 13, 12, 11,
		10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, -1 },//done
		//5 SOUTH
		{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
		20, 21, 22, 23, 24, -1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//done
		//6
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2,
		3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, -1 },//done
		//7 WEST
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//done
		//8
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3,
		4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
		14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, -1 }//done
};

int _tmp_iMoveLocY[9][44] = {
	// 0
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//done
		//1 NORTH
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, -1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//done
		//2
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6,
		7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 - 1 },//done
		//3 EAST
		{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		10, 11, 12, 13, 14, 15, 16, 17, 18, -1, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//done
		//4
		{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		10, 11, 12, 13, 14, 15, 16, 17, 18, 18,
		18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
		18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, -1 },//done
		//5 SOUTH
		{ 18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
		18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
		18, 18, 18, 18, 18, -1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//done
		//6
		{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		10, 11, 12, 13, 14, 15, 16, 17, 18, 18,
		18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
		18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, -1 },//done
		//7 WEST
		{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		10, 11, 12, 13, 14, 15, 16, 17, 18, -1, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//done
		// 8
		{ 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8,
		7, 6, 5, 4, 3, 2, 1, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1 }//done
};
//#else
//int _tmp_iMoveLocX[9][37] = {
//	// 0
//	{0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0},
//	// 1
//	{0,1,2,3,4,5,6,7,8,9,
//	10,11,12,13,14,15,16,17,18,19,
//	20,-1,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0},
//	//2
//	{0,1,2,3,4,5,6,7,8,9,
//	10,11,12,13,14,15,16,17,18,19,
//	20,20,20,20,20,20,20,20,20,20,
//	20,20,20,20,20,20,-1},
//	//3
//	{20,20,20,20,20,20,20,20,20,20,
//	20,20,20,20,20,20,-1,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0},
//	//4
//	{20,20,20,20,20,20,20,20,20,20,
//	20,20,20,20,20,20,19,18,17,16,
//	15,14,13,12,11,10,9,8,7,6,
//	5,4,3,2,1,0,-1},
//	//5
//	{0,1,2,3,4,5,6,7,8,9,
//	10,11,12,13,14,15,16,17,18,19,
//	20,-1,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0},
//	//6
//	{0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,1,2,3,4,
//	5,6,7,8,9,10,11,12,13,14,
//	15,16,17,18,19,20,-1},
//	//7
//	{0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,-1,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0},
//	//8
//	{0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,1,2,3,4,
//	5,6,7,8,9,10,11,12,13,14,
//	15,16,17,18,19,20,-1}
//};
//
//int _tmp_iMoveLocY[9][37] = {
//	// 0
//	{0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0},
//	//1
//	{0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,
//	0,-1,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0},
//	//2
//	{0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,
//	0,1,2,3,4,5,6,7,8,9,
//	10,11,12,13,14,15,-1},
//	//3
//	{0,1,2,3,4,5,6,7,8,9,
//	10,11,12,13,14,15,-1,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0},
//	//4
//	{0,1,2,3,4,5,6,7,8,9,
//	10,11,12,13,14,15,15,15,15,15,
//	15,15,15,15,15,15,15,15,15,15,
//	15,15,15,15,15,15,-1},
//	//5
//	{15,15,15,15,15,15,15,15,15,15,
//	15,15,15,15,15,15,15,15,15,15,
//	15,-1,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0},
//	//6
//	{0,1,2,3,4,5,6,7,8,9,
//	10,11,12,13,14,15,15,15,15,15,
//	15,15,15,15,15,15,15,15,15,15,
//	15,15,15,15,15,15,-1},
//	//7
//	{0,1,2,3,4,5,6,7,8,9,
//	10,11,12,13,14,15,-1,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0},
//	// 8
//	{15,14,13,12,11,10,9,8,7,6,
//	5,4,3,2,1,0,0,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,-1}
//};
//#endif

char _tmp_cTmpDirX[9] = { 0, 0, 1, 1, 1, 0, -1, -1, -1 };
char _tmp_cTmpDirY[9] = { 0, -1, -1, 0, 1, 1, 1, 0, -1 };

char _tmp_cEmptyPosX[] = { 0, 1, 1, 0, -1, -1, -1, 0, 1, 2, 2, 2, 2, 1, 0, -1, -2, -2, -2, -2, -2, -1, 0, 1, 2 };
char _tmp_cEmptyPosY[] = { 0, 0, 1, 1, 1, 0, -1, -1, -1, -1, 0, 1, 2, 2, 2, 2, 2, 1, 0, -1, -2, -2, -2, -2, -2 };
#pragma endregion

Map::Map(GServer * pGame)
{
	npchandle = 0;

	TimerThreadRunning = false;
	sizeX = sizeY = 0;

	totalActiveObject = 0;
	totalAliveObject  = 0;

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
	fixedSnow = false ; 

	totalFishPoints = 0;
	fishMax = 0;
	fishCurrent = 0;

	mineralTotalPoint = 0;
	mineralMax = 0;
	mineralCurrent = 0;

	_tile = 0;

	weather = WEATHER_SUNNY;
	type          = MAPTYPE_NORMAL;

	gserver = pGame;

	levelLimitLower = 0;
	levelLimitUpper = 0; 
	flags.mineralGenerator = false;

	occupyFlagTotal = 0;

	flags.attackEnabled = true;
	mobGenLevel = 0;

	for(int i = 0; i < MAXMAGICTYPE; i++)
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

	mobEventAmount = 15 ;   

	}

Map::~Map()
{
	if (_tile != 0) 
		delete []_tile;

	for (TeleportLoc * var : teleportLocationList)
	{
		delete var;
	}
	for (OccupyFlag * var : occupyFlag)
	{
		delete var;
	}
	for (StrategicPoint * var : strategicPointList)
	{
		delete var;
	}
}

void Map::run()
{
	TimerThreadRunning = true;
	actionthread = new std::thread(std::bind(std::mem_fun(&Map::SocketThread), this));
	timerthread = new std::thread(std::bind(std::mem_fun(&Map::TimerThread), this));
}

void Map::SocketThread()
{
#ifndef WIN32
	struct timespec req = { 0 };
	req.tv_sec = 0;
	req.tv_nsec = 1000000L;//1ms
#else
	_tzset();
#endif

	while (gserver->serverstatus == SERVERSTATUS_ONLINE)
	{
		if (actionpipe.size() > 0)
		{
			mutaction.lock();
			shared_ptr<Server::MsgQueueEntry> msg = gserver->GetMsgQueue(actionpipe);
			mutaction.unlock();

			StreamRead sr = StreamRead(msg->data, msg->size);
			StreamWrite sw = StreamWrite();

			shared_ptr<Client> client = msg->client;

			uint32_t msgid = sr.ReadInt();
			uint32_t cmd = sr.ReadShort();

			//logger->error(Poco::format("Packet received from client - 0x%.4X - 0x%.2X", msgid, cmd));

			if (msgid & MSGIDTYPE_MOTION)
			{
				gserver->ClientMotionHandler(msg->client, msg);
			}
			else if (msgid == MSGID_COMMAND_COMMON)
			{
				//this first as it will be the most often sent packet type
				try
				{
					gserver->ClientCommonHandler(msg->client, msg);
				}
				catch (int test)
				{
					//-193 data length error
					gserver->logger->error(Poco::format("Error: %?d", test));
				}
			}
			else switch (msgid)
			{
			case MSGID_REQUEST_SETITEMPOS:
				break;

			case MSGID_REQUEST_TELEPORT:
				gserver->RequestTeleportHandler(client.get(), 4);
				break;

			case MSGID_REQUEST_INITPLAYER:

				if (!client->m_bIsInitComplete && !gserver->LoadCharacterData(client))
				{
					gserver->DeleteClient(msg->client, false, true);
					continue;
				}
				gserver->InitPlayerData(client);
				break;

			case MSGID_REQUEST_RESTART:
				gserver->RequestRestartHandler(client);
				break;

			default:
				gserver->logger->error(Poco::format("Unknown packet received from client - 0x%.4X", msgid));
				//DeleteClient(msg->client, false, true);
				break;
			}
		}

#ifdef WIN32
		Sleep(1);
#else
		nanosleep(&req, 0);
#endif
	}
	gserver->logger->information("ActionThread() complete.");
#ifndef WIN32
	mysql_thread_end();
#endif
}

void Map::TimerThread()
{
#ifndef WIN32
	struct timespec req = { 0 };
	req.tv_sec = 0;
	req.tv_nsec = 1000000L;//1ms
#else
	_tzset();
#endif

	TimerThreadRunning = true;

	uint64_t t1htimer;
	uint64_t t30mintimer;
	uint64_t t6mintimer;
	uint64_t t5mintimer;
	uint64_t t3mintimer;
	uint64_t t1mintimer;
	uint64_t t30sectimer;
	uint64_t t5sectimer;
	uint64_t t1sectimer;
	uint64_t t100msectimer;
	uint64_t ltime;

	t1htimer = t30mintimer = t6mintimer = t5mintimer = t3mintimer = t1mintimer = t30sectimer = t5sectimer = t1sectimer = t100msectimer = unixtime();

	while (gserver->serverstatus == SERVERSTATUS_ONLINE)
	{
		try
		{
			ltime = ltime = unixtime();

			if (t100msectimer < ltime)
			{
				NpcProcess();
				DelayEventProcessor();

				t100msectimer += 100;
			}
			if (t1sectimer < ltime)
			{
				t1sectimer += 1000;
			}
			if (t5sectimer < ltime)
			{
				//test code - keep this until beta?

				//regenerates stamina at a drastically increased rate
				{
					boost::shared_lock_guard<boost::shared_mutex> lock(Gate::GetSingleton()->mutclientlist);
					for (shared_ptr<Client> client : gserver->clientlist)
					{
						client->m_iSP = client->GetStr() + 17;
						gserver->SendNotifyMsg(0, client.get(), NOTIFY_SP);
					}
				}

				//check connections for recent data (ghost sockets)
				// 				for (shared_ptr<Client> client : clientlist)
				// 				{
				// 					if (client->disconnecttime == 0 && client->lastpackettime + 30000 < ltime)
				// 					{
				// 						//socket idle for 30 seconds (should never happen unless disconnected)
				// 						poco_information(*logger, Poco::format("Client Timeout! <%s>", client->socket->address));
				// 						gate->stop(client->socket);
				// 						//when to delete client object...... ?
				// 						//force a ~10 second delay on object deletion to prevent logout hacking/plug pulling/etc? or destroy instantly?
				// 					}
				// 				}
				t5sectimer += 5000;
			}
			if (t30sectimer < ltime)
			{
				t30sectimer += 30000;
			}
			if (t1mintimer < ltime)
			{
				t1mintimer += 60000;
			}
			if (t3mintimer < ltime)
			{
				t3mintimer += 180000;
			}
			if (t6mintimer < ltime)
			{
				t6mintimer += 360000;
			}
			if (t1htimer < ltime)
			{

				t1htimer += 3600000;
			}
#ifdef WIN32
			Sleep(1);
#else
			nanosleep(&req, 0);
#endif
		}
		catch (Poco::Data::MySQL::MySQLException * e)
		{
			gserver->logger->critical(Poco::format("TimerThread() SQL Exception: %s", e->displayText()));
		}
		catch (boost::system::system_error & e)
		{
			gserver->logger->fatal(Poco::format("Map::TimerThread() boost::Exception: %s", e.what()));
		}
		catch (std::exception & e)
		{
			gserver->logger->fatal(Poco::format("Map::TimerThread() std::Exception: %s", e.what()));
		}
		catch (...)
		{
			gserver->logger->fatal("Unspecified Map::TimerThread() Exception.");
		}
	}
	TimerThreadRunning = false;
	gserver->logger->information("TimerThread() complete.");

#ifndef WIN32
	mysql_thread_end();
#endif
}

void Map::SetOwner(shared_ptr<Unit> sOwner, short sX, short sY)
{
	Tile * pTile;

	if ((sX < 0) || (sX >= sizeX) || (sY < 0) || (sY >= sizeY)) return;

	pTile = (Tile *)(_tile + sX + sY*sizeY);
	pTile->owner      = sOwner;
	pTile->m_cOwnerType = sOwner->IsPlayer()?OWNERTYPE_PLAYER:OWNERTYPE_NPC;
}


void Map::SetDeadOwner(shared_ptr<Unit> sOwner, short sX, short sY)
{
	Tile * pTile;

	if ((sX < 0) || (sX >= sizeX) || (sY < 0) || (sY >= sizeY)) return;

	pTile = (Tile *)(_tile + sX + sY*sizeY);
	pTile->deadowner      = sOwner;
	pTile->m_cDeadOwnerType = sOwner->IsPlayer()?OWNERTYPE_PLAYER:OWNERTYPE_NPC;
}

shared_ptr<Unit> Map::GetOwner(short sX, short sY)
{
	Tile * pTile;

	if ((sX < 0) || (sX >= sizeX) || (sY < 0) || (sY >= sizeY)) {
		return 0;
	}

	pTile = (Tile *)(_tile + sX + sY*sizeY);
	return pTile->owner;

}

shared_ptr<Unit> Map::GetDeadOwner(short sX, short sY)
{
	Tile * pTile;

	if ((sX < 0) || (sX >= sizeX) || (sY < 0) || (sY >= sizeY)) {
		return 0;
	}

	pTile = (Tile *)(_tile + sX + sY*sizeY);
	return pTile->deadowner;

}

std::list<shared_ptr<Unit>>Map::GetOwners(short x1, short x2, short y1, short y2)
{
	class Tile * pTile;
	//TODO: this. shoot whoever wrote it and rewrite it before an alpha test
	//return 0;
	std::list<shared_ptr<Unit>> owners;
 
 	if(x1 < 0)
 		x1 = 0;
 
 	if(x2 >= sizeX)
 		x2 = sizeX-1;
 
 	if(y1 < 0)
 		y1 = 0;
 
 	if(y2 >= sizeY)
 		y2 = sizeY-1;
 
 	int n = 0;
 
 	for(int y = y1; y <= y2; y++) 
 	{
 		for (int x = x1; x <= x2; x++)
 		{
 			pTile = (Tile *)(_tile + x + y*sizeY);
 
 			switch(pTile->m_cOwnerType)
 			{
 			case OWNERTYPE_PLAYER:
 				owners.push_back(pTile->owner);
 				n++;
 				break;;
 
 			case OWNERTYPE_NPC:
				owners.push_back(pTile->owner);
 				n++;
 				break;
			case 0:
				owners.push_back(nullptr);
				n++;
				break;
			}
 		}
 	}
 
	owners.push_back(nullptr);
 
 	return owners;
}

bool Map::bGetMoveable(short dX, short dY, short * pDOtype, Item * pTopItem) 
{
	Tile * pTile;

	if ((dX < 20) || (dX >= sizeX - 20) || (dY < 20) || (dY >= sizeY - 20)) return false;

	pTile = (Tile *)(_tile + dX + dY*sizeY);

	if (pDOtype != 0) *pDOtype = pTile->m_sDynamicObjectType; 
	if ((pTopItem != 0) && (pTile->m_pItem.size() > 0)) pTopItem = pTile->m_pItem[0]; 

	if (pTile->owner != 0) return false;
	if (pTile->m_bIsMoveAllowed == false) return false;
	if (pTile->m_bIsTempMoveAllowed == false) return false;

	return true;
}

bool Map::bGetIsMoveAllowedTile(short dX, short dY)
{
	Tile * pTile;

	if ((dX < 20) || (dX >= sizeX - 20) || (dY < 20) || (dY >= sizeY - 20)) return false;

	pTile = (Tile *)(_tile + dX + dY*sizeY);

	if (pTile->m_bIsMoveAllowed == false) return false;
	if (pTile->m_bIsTempMoveAllowed == false) return false;

	return true;
}

bool Map::bGetIsMoveAllowedTile(Point p)
{
	if (p.x < 20 || p.x >= sizeX - 20 
		|| p.y < 20 || p.y >= sizeY - 20
		) return false;

	Tile * pTile = (Tile *)(_tile + p.x + p.y*sizeY);

	if (!pTile->m_bIsMoveAllowed || !pTile->m_bIsTempMoveAllowed) 
		return false;

	return true;
}

bool Map::bGetIsTeleport(short dX, short dY)
{
	Tile * pTile;

	if ((dX < 14) || (dX >= sizeX - 16) || (dY < 12) || (dY >= sizeY - 14)) return false;

	pTile = (Tile *)(_tile + dX + dY*sizeY);

	if (pTile->m_bIsTeleport == false) return false;

	return true;
}

void Map::ClearOwner(short sX, short sY)
{
	Tile * pTile;

	if ((sX < 0) || (sX >= sizeX) || (sY < 0) || (sY >= sizeY)) return;

	pTile = (Tile *)(_tile + sX + sY*sizeY);
	pTile->owner      = 0;
	pTile->m_cOwnerType = 0;
}

void Map::ClearDeadOwner(short sX, short sY)
{
	Tile * pTile;

	if ((sX < 0) || (sX >= sizeX) || (sY < 0) || (sY >= sizeY)) return;

	pTile = (Tile *)(_tile + sX + sY*sizeY);
	pTile->deadowner      = 0;
	pTile->m_cDeadOwnerType = 0;
}

bool Map::bSetItem(short sX, short sY, Item * pItem)
{
	Tile * pTile;

	if ((sX < 0) || (sX >= sizeX) || (sY < 0) || (sY >= sizeY)) return false; 

	pTile = (Tile *)(_tile + sX + sY*sizeY);

	if (pTile->m_pItem.size() > 0)
	{
		if (pTile->m_pItem[0]->m_sIDnum == ITEM_RELIC) return false;
	}
	pTile->m_pItem.push_back(pItem);

//	if(pTile->m_pItem[0] && pTile->m_pItem[0]->m_sIDnum == ITEM_RELIC) return false;

// 	if (pTile->m_pItem[ITEMS_PER_TILE-1] != NULL) 
// 		delete pTile->m_pItem[ITEMS_PER_TILE-1];
// 	else pTile->m_cTotalItem++;

// 	for (i = ITEMS_PER_TILE-2; i >= 0; i--) 
// 		pTile->m_pItem[i+1] = pTile->m_pItem[i];

//	pTile->m_pItem[0] = pItem;
	//pTile->m_cTotalItem++;
	return true;
}


Item * Map::pGetItem(short sX, short sY, short * pRemainItemSprite, short * pRemainItemSpriteFrame, uint32_t * pRemainItemColor)
{
	Tile * pTile;
	Item * pItem;

	if ((sX < 0) || (sX >= sizeX) || (sY < 0) || (sY >= sizeY)) return 0;

	pTile = (Tile *)(_tile + sX + sY*sizeY);
	if (pTile->m_pItem.size() == 0) return 0;
	pItem =  pTile->m_pItem[0];

// 	for (i = 0; i <= ITEMS_PER_TILE-2; i++)
// 		pTile->m_pItem[i] = pTile->m_pItem[i+1];
// 	pTile->m_cTotalItem--;
// 	pTile->m_pItem[pTile->m_cTotalItem] = NULL;

// 	if (pTile->m_pItem[0] == NULL) {
// 		*pRemainItemSprite      = 0;
// 		*pRemainItemSpriteFrame = 0;
// 		*pRemainItemColor       = 0;
// 	}
// 	else
	{
		*pRemainItemSprite      = pItem->spriteID;
		*pRemainItemSpriteFrame = pItem->spriteFrame;
		*pRemainItemColor       = pItem->color;
	}

	return pItem;
}


uint64_t Map::iCheckItem(short sX, short sY)
{
	Tile * pTile;
	Item * pItem;

	if ((sX < 0) || (sX >= sizeX) || (sY < 0) || (sY >= sizeY)) return 0;

	pTile = (Tile *)(_tile + sX + sY*sizeY);
	if (pTile->m_pItem.size() == 0) return 0;
	pItem =  pTile->m_pItem[0];

	return pItem->m_sIDnum;
}


bool Map::bIsValidLoc(short sX, short sY)
{
	if ((sX < 0) || (sX >= sizeX) || (sY < 0) || (sY >= sizeY)) return false;
	return true;
}

bool Map::bInit(string pName)
{
	name = pName;

	if (_bDecodeMapDataFileContents() == false) 
		return false;

	if (bDecodeMapConfig() == false)
		return false;

	return true;
}

extern void stack_dump(lua_State *stack);


bool Map::bDecodeMapConfig()
{
	string mapfile = "mapdata/";
	mapfile += name + ".lua";

	lua_State * L = gserver->L;
	try
	{
		if (luaL_dofile(L, mapfile.c_str()) != 0)
		{
			gserver->logger->fatal(Poco::format("%s", (string)lua_tostring(L,-1)));
			return false;
		}
		lua_getglobal(L, "mapdata");

		if(lua_istable(L,-1))
		{
			lua_pushnil(L);
			while (lua_next(L, -2))
			{
				string option = (char*)lua_tostring(L, -2);
				if (option == "teleportloc")
				{
					if(lua_istable(L,-1))
					{
						uint8_t tableSize = uint8_t(lua_rawlen(L, -1));
						for (uint8_t i = 1; i <= tableSize; ++i)
						{
							lua_pushinteger(L, i);
							lua_gettable(L, -2);

							lua_pushinteger(L, 1);
							lua_gettable(L, -2);
							uint16_t sX = (uint16_t)lua_tointeger(L, -1);
							lua_pop(L, 1);
							lua_pushinteger(L, 2);
							lua_gettable(L, -2);
							uint16_t sY = (uint16_t)lua_tointeger(L, -1);
							lua_pop(L, 1);
							lua_pushinteger(L, 3);
							lua_gettable(L, -2);
							string dest = (char*)lua_tostring(L, -1);
							lua_pop(L, 1);
							lua_pushinteger(L, 4);
							lua_gettable(L, -2);
							uint16_t dX = (uint16_t)lua_tointeger(L, -1);
							lua_pop(L, 1);
							lua_pushinteger(L, 5);
							lua_gettable(L, -2);
							uint16_t dY = (uint16_t)lua_tointeger(L, -1);
							lua_pop(L, 1);
							lua_pushinteger(L, 6);
							lua_gettable(L, -2);
							uint8_t dir = (uint8_t)lua_tointeger(L, -1);
							lua_pop(L, 1);

							TeleportLoc * loc = new TeleportLoc();
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
					if(lua_istable(L,-1))
					{
						uint8_t tableSize = uint8_t(lua_rawlen(L, -1));
						for (uint8_t i = 1; i <= tableSize; ++i)
						{
							lua_pushinteger(L, i);
							lua_gettable(L, -2);

							lua_pushinteger(L, 1);
							lua_gettable(L, -2);
							uint16_t sX = (uint16_t)lua_tointeger(L, -1);
							lua_pop(L, 1);
							lua_pushinteger(L, 2);
							lua_gettable(L, -2);
							uint16_t sY = (uint16_t)lua_tointeger(L, -1);
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
					uint16_t gen = (uint16_t)lua_tointeger(L, -1);
					lua_pop(L, 1);
					lua_pushinteger(L, 2);
					lua_gettable(L, -2);
					uint16_t level = (uint16_t)lua_tointeger(L, -1);
					lua_pop(L, 1);

					flags.randomMobGenerator = (gen>0) ? true : false;
					mobGenLevel = level;
				}
				else if (option == "maximumobject")
				{
					maximumObject = (uint16_t)lua_tointeger(L, -1);
				}
				else if (option == "npcavoidrect")
				{

				}
				else if (option == "spotmobgenerator")
				{

				}
				else if (option == "name")
				{
					this->factionName = (char*)lua_tostring(L, -1);;
				}
				else if (option == "initialpoint")
				{
					if(lua_istable(L,-1))
					{
						uint8_t tableSize = uint8_t(lua_rawlen(L, -1));
						for (uint8_t i = 1; i <= tableSize; ++i)
						{
							lua_pushinteger(L, i);
							lua_gettable(L, -2);

							lua_pushinteger(L, 1);
							lua_gettable(L, -2);
							uint16_t sX = (uint16_t)lua_tointeger(L, -1);
							lua_pop(L, 1);
							lua_pushinteger(L, 2);
							lua_gettable(L, -2);
							uint16_t sY = (uint16_t)lua_tointeger(L, -1);
							lua_pop(L, 1);

							initialPointList.push_back(point(sX, sY));

							lua_pop(L, 1);
						}
					}
				}
				else if (option == "noattackarea")
				{
					if(lua_istable(L,-1))
					{
						uint8_t tableSize = uint8_t(lua_rawlen(L, -1));
						for (uint8_t i = 1; i <= tableSize; ++i)
						{
							lua_pushinteger(L, i);
							lua_gettable(L, -2);

							lua_pushinteger(L, 1);
							lua_gettable(L, -2);
							uint16_t sX = (uint16_t)lua_tointeger(L, -1);
							lua_pop(L, 1);
							lua_pushinteger(L, 2);
							lua_gettable(L, -2);
							uint16_t sY = (uint16_t)lua_tointeger(L, -1);
							lua_pop(L, 1);
							lua_pushinteger(L, 3);
							lua_gettable(L, -2);
							uint16_t dX = (uint16_t)lua_tointeger(L, -1);
							lua_pop(L, 1);
							lua_pushinteger(L, 4);
							lua_gettable(L, -2);
							uint16_t dY = (uint16_t)lua_tointeger(L, -1);
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
	catch (std::exception& e)
	{
		gserver->logger->fatal(Poco::format("bDecodeMapConfig() Exception: %s", (string)e.what()));
		return false;
	}
	catch(...)
	{
		gserver->logger->fatal("Unspecified bDecodeMapConfig() Exception.");
		return false;
	}


	return true;
}

bool Map::_bDecodeMapDataFileContents()
{
	string mapfile = "mapdata/";
	mapfile += name + ".amd";
	FILE * file;
	char * buffer;
	file = fopen(mapfile.c_str(), "r");

	if (!file)
	{
		//not existing
		gserver->logger->fatal(Poco::format("Loaded map does not exist: %s", name));
		return false;
	}

	uint32_t filesize;

	fseek(file, 0, SEEK_END);
	filesize = ftell(file);
	fseek(file, 0, SEEK_SET);

	buffer = new char[filesize+1];

	fread(buffer, 1, filesize, file);

	FileRead fr(buffer, filesize);
	char cHeader[260];

	fr.ReadBytes(cHeader, 256);

	for (int i = 0; i < 256; ++i)
		if (cHeader[i] == 0) cHeader[i] = ' ';

	char * tok;

	//no error checking - assumes valid

	char * ptr;

	tok = strtok_s(cHeader, "\t\n =", &ptr);

	if (memcmp(tok, "MAPSIZEX",8) == 0)
	{
		tok = strtok_s(0, "\t\n =", &ptr);
		sizeX = atoi(tok);
	}
	tok = strtok_s(0, "\t\n =", &ptr);
	if (memcmp(tok, "MAPSIZEY",8) == 0)
	{
		tok = strtok_s(0, "\t\n =", &ptr);
		sizeY = atoi(tok);
	}
	tok = strtok_s(0, "\t\n =", &ptr);
	if (memcmp(tok, "TILESIZE",8) == 0)
	{
		tok = strtok_s(0, "\t\n =", &ptr);
		tileDataSize = atoi(tok);
	}

	_tile = (Tile *)new Tile[sizeX * sizeY];

	char * tileData = new char[tileDataSize];
	Tile * pTile;

	for (int iy = 0; iy < sizeY; iy++)
	{
		for (int ix = 0; ix < sizeX; ix++)
		{
			fr.ReadBytes(tileData, tileDataSize);
			pTile = (Tile *)(_tile + ix + iy*sizeY);
			pTile->m_attribute = tileData[8];
			pTile->m_bIsMoveAllowed = (tileData[8] & 0x80) ? false : true;
			pTile->m_bIsTeleport = (tileData[8] & 0x40) ? true : false;
			pTile->m_bIsFarmingAllowed = (tileData[8] & 0x20) ? true : false;
			pTile->m_bIsWater = (*((short *)&tileData[0])== 19) ? true : false;
		}
	}
	delete[] tileData;

	return true;

// 	HANDLE hFile;
// 	char  cMapFileName[64], cHeader[260];
// 	uint32_t dwFileSize, nRead;
// 	int i, ix, iy;
// 	char * token, cReadMode, *mapData, *tileData;
// 	char seps[] = "= \t\n"; 
// 	class CStrTok * pStrTok = NULL;
// 	class CTile * pTile;
// 
// 	wsprintf(cMapFileName, "mapdata\\%s.amd",m_cName);
// 
// 	hFile = CreateFile(cMapFileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
// 	if (hFile == INVALID_HANDLE_VALUE) return false;
// 	dwFileSize = GetFileSize(hFile, NULL);
// 
// 	ZeroMemory(cHeader, sizeof(cHeader));
// 	ReadFile(hFile, (char *)cHeader, 256, &nRead, NULL);
// 
// 	for (i = 0; i < 256; i++) 
// 		if (cHeader[i] == NULL) cHeader[i] = ' ';
// 
// 	cReadMode = 0;
// 
// 	pStrTok = new StrTok(cHeader, seps);
// 	token = pStrTok->pGet();
// 	//token = strtok( cHeader, seps );   
// 	while( token != NULL )   {
// 
// 		if (cReadMode != 0) {
// 			switch (cReadMode) {
// 			case 1:
// 				m_sSizeX = atoi(token);
// 				cReadMode = 0;
// 				break;
// 			case 2:
// 				m_sSizeY = atoi(token);
// 				cReadMode = 0;
// 				break;
// 			case 3:
// 				m_sTileDataSize = atoi(token);
// 				cReadMode = 0;
// 				break;
// 			}
// 		}
// 		else {
// 			if (memcmp(token, "MAPSIZEX",8) == 0) cReadMode = 1;
// 			if (memcmp(token, "MAPSIZEY",8) == 0) cReadMode = 2;
// 			if (memcmp(token, "TILESIZE",8) == 0) cReadMode = 3; 
// 		}
// 
// 		//token = strtok( NULL, seps );
// 		token = pStrTok->pGet();
// 	}
// 
// 	m_pTile = (Tile *)new Tile[m_sSizeX * m_sSizeY];
// 
// 	mapData = new char[dwFileSize];
// 	tileData = mapData;
// 
// 	ReadFile(hFile, mapData, dwFileSize, &nRead, NULL);
// 	CloseHandle(hFile);
// 
// 	for (iy = 0; iy < m_sSizeY; iy++)
// 		for (ix = 0; ix < m_sSizeX; ix++, tileData += m_sTileDataSize) {
// 			pTile = (Tile *)(m_pTile + ix + iy*m_sSizeY);
// 			pTile->m_attribute = tileData[8];
// 			pTile->m_bIsMoveAllowed = (tileData[8] & 0x80) ? false : true;
// 			pTile->m_bIsTeleport = (tileData[8] & 0x40) ? true : false;
// 			pTile->m_bIsFarmingAllowed = (tileData[8] & 0x20) ? true : false;
// 			pTile->m_bIsWater = (*((short *)&tileData[0])== 19) ? true : false;
// 		}
// 
// 		delete [] mapData;
// 
// 		if (pStrTok != NULL) delete pStrTok;
		return true;
}


bool Map::bSearchTeleportDest(int sX, int sY, string & pMapName, uint16_t * pDx, uint16_t * pDy, uint8_t * pDir)
{
	int i;

	for (i = 0; i < teleportLocationList.size(); i++)
		if ((teleportLocationList[i]->m_sSrcX == sX) && (teleportLocationList[i]->m_sSrcY == sY))
		{
			pMapName = teleportLocationList[i]->m_cDestMapName;
			*pDx  = teleportLocationList[i]->m_sDestX;
			*pDy  = teleportLocationList[i]->m_sDestY;
			*pDir = teleportLocationList[i]->m_cDir;
			return true;
		}

	return false;
}

void Map::SetDynamicObject(uint16_t wID, short sType, short sX, short sY, uint64_t dwRegisterTime)
{
	Tile * pTile;


	if ((sX < 0) || (sX >= sizeX) || (sY < 0) || (sY >= sizeY)) return; 

	pTile = (Tile *)(_tile + sX + sY*sizeY);

	pTile->m_wDynamicObjectID   = wID;
	pTile->m_sDynamicObjectType = sType;
	pTile->m_dwDynamicObjectRegisterTime = dwRegisterTime;
}

bool Map::bGetDynamicObject(short sX, short sY, short *pType, uint64_t *pRegisterTime, int * pIndex)
{
	Tile * pTile;


	if ((sX < 0) || (sX >= sizeX) || (sY < 0) || (sY >= sizeY)) return false; 

	pTile = (Tile *)(_tile + sX + sY*sizeY);

	*pType = pTile->m_sDynamicObjectType;
	*pRegisterTime = pTile->m_dwDynamicObjectRegisterTime;
	if (pIndex != 0) *pIndex = pTile->m_wDynamicObjectID;

	return true;
}

bool Map::bGetIsWater(short dX, short dY)
{
	Tile * pTile;

	if ((dX < 14) || (dX >= sizeX - 16) || (dY < 12) || (dY >= sizeY - 14)) return false;

	pTile = (Tile *)(_tile + dX + dY*sizeY);

	return pTile->m_bIsWater;
}



bool Map::bGetIsFarm(short dX, short dY)
{
	Tile * pTile;

	if ((dX < 14) || (dX >= sizeX - 16) || (dY < 12) || (dY >= sizeY - 14)) return false;

	pTile = (Tile *)(_tile + dX + dY*sizeY);

	return pTile->m_bIsFarmingAllowed;
}

bool Map::IsBuild(short dX, short dY)
{
	Tile * pTile;

	if ((dX < 14) || (dX >= sizeX - 16) || (dY < 12) || (dY >= sizeY - 14)) return false;

	pTile = (Tile *)(_tile + dX + dY*sizeY);

	return pTile->IsBuild();
}


int Map::iAnalyze(char cType, int * pX, int * pY, int * pV1, int * pV2, int * pV3)
{
	switch (cType) {
	case 1:

		break;
	}

	return 0;
}

void Map::SetTempMoveAllowedFlag(int dX, int dY, bool bFlag)
{
	Tile * pTile;

	if ((dX < 20) || (dX >= sizeX - 20) || (dY < 20) || (dY >= sizeY - 20)) return;

	pTile = (Tile *)(_tile + dX + dY*sizeY);
	pTile->m_bIsTempMoveAllowed = bFlag;
}

Map::OccupyFlag * Map::iRegisterOccupyFlag(int dX, int dY, int iSide, int iEKNum, int iDOI)
{
	if ((dX < 20) || (dX >= sizeX - 20) || (dY < 20) || (dY >= sizeY - 20)) return 0;

	OccupyFlag * oflag = new OccupyFlag(dX, dY, iSide, iEKNum, iDOI);

	occupyFlag.push_back(oflag);

	return oflag;
}



void Map::ClearSectorInfo()
{
	for (int i = 0; i < MAXSECTORS*MAXSECTORS; ++i)
	{
		sectorInfo[i].neutralActivity = 0;
		sectorInfo[i].aresdenActivity = 0;
		sectorInfo[i].elvineActivity  = 0;
		sectorInfo[i].mobActivity = 0;
		sectorInfo[i].playerActivity  = 0;
	}
}

void Map::ClearTempSectorInfo()
{
	for (int i = 0; i < MAXSECTORS*MAXSECTORS; ++i)
	{
		sectorInfoTemp[i].neutralActivity = 0;
		sectorInfoTemp[i].aresdenActivity = 0;
		sectorInfoTemp[i].elvineActivity  = 0;
		sectorInfoTemp[i].mobActivity = 0;
		sectorInfoTemp[i].playerActivity  = 0;
	}
}

void Map::_SetupNoAttackArea()
{
	int i, ix, iy;
	Tile * pTile;

	for (i = 0; i < safeZoneList.size(); i++) {
		if (safeZoneList[i].top > 0) {

			for (ix = safeZoneList[i].left; ix <= safeZoneList[i].right; ix++)
				for (iy = safeZoneList[i].top; iy <= safeZoneList[i].bottom; iy++) {
					pTile = (Tile *)(_tile + ix + iy*sizeY);
					pTile->m_attribute |= 0x00000004;
				}
		}

		else if (safeZoneList[i].top == -10) {
			for (ix = 0; ix < sizeX; ix++)
				for (iy = 0; iy < sizeY; iy++) {
					pTile = (Tile *)(_tile + ix + iy*sizeY);
					pTile->m_attribute |= 0x00000004;
				}
		}
	}
}

int Map::iGetAttribute(int dX, int dY, int iBitMask)
{
	Tile * pTile;

	if ((dX < 20) || (dX >= sizeX - 20) || (dY < 20) || (dY >= sizeY - 20)) return -1;

	pTile = (Tile *)(_tile + dX + dY*sizeY);
	return (pTile->m_attribute & iBitMask);
}

bool Map::bAddCrusadeStructureInfo(char cType, short sX, short sY, char cSide)
{
	crusadestructure tempstructure;
	tempstructure.cType = cType;
	tempstructure.cSide = cSide;
	tempstructure.sX = sX;
	tempstructure.sY = sY;
	m_stCrusadeStructureInfo.push_back(tempstructure);
	return true;
	// useful? \/
	//if(cType == NPC_MANASTONE)
	//{
	//	pTile = (class CTile *)(m_pTile + sX + (sY-1)*m_sSizeY);
	//	pTile->m_bIsMoveAllowed = false;
	//}
}

bool Map::bRemoveCrusadeStructureInfo(short sX, short sY)
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



bool Map::bAddCropsTotalSum()
{
// 	if(m_iTotalAgriculture < MAXAGRICULTURE)
// 	{
// 		m_iTotalAgriculture++;
// 		return true;
// 	}
	return false;
}


bool Map::bRemoveCropsTotalSum()
{
// 	if(m_iTotalAgriculture < MAXAGRICULTURE)
// 	{
// 		m_iTotalAgriculture--;
// 		if(m_iTotalAgriculture < 0)
// 		{
// 			m_iTotalAgriculture = 0;
// 		}
// 		return true;
// 	}
	return false;
}

void Map::RestoreStrikePoints()
{
	for (int i = 0; i < strikePointList.size(); i++)
		strikePointList[i].hp = strikePointList[i].hpInit;
}

void Map::IncPlayerActivity(Client * player)
{
	int iStX, iStY;
	return;

	//TODO: vector or 2d array? need to initialize vector

	iStX = player->x / 20;
	iStY = player->y / 20;
	sectorInfoTemp[iStX+iStY*MAXSECTORS].playerActivity++;

	switch (player->side)
	{
		case NEUTRAL: sectorInfoTemp[iStX+iStY*MAXSECTORS].neutralActivity++; break;
		case ARESDEN: sectorInfoTemp[iStX+iStY*MAXSECTORS].aresdenActivity++; break;
		case ELVINE: sectorInfoTemp[iStX+iStY*MAXSECTORS].elvineActivity++;  break;
	}
}

bool Map::GetInitialPoint(int16_t *pX, int16_t *pY, string & pPlayerLocation)//TODO: turn into reference
{
	//every map has to have an initial point as a "fail safe". if none exists by some stupid error, send back to town

	if (initialPointList.size() == 0)
	{
		Map * tempmap = 0;
		if (pPlayerLocation == "NONE")
			tempmap = gserver->GetMap("default");//hardcoded. trololo. -- could use changes if attempting to ever make an additional side... doubtful though
		else if (pPlayerLocation.substr(0, 3) == "are")
			tempmap = gserver->GetMap("aresden");
		else if (pPlayerLocation.substr(0, 3) == "elv")
			tempmap = gserver->GetMap("elvine");

		if (!tempmap)
		{
			//map not found. this is the critical failure point as player cannot load into current map, and their "town" map cannot be found loaded.
			//worst case, pick a random teleport spot. If that fails, return a false to indicate to disconnect the client
			if (teleportLocationList.size() == 0)
			{
				return false;
			}
			else
			{
				//this is incredibly crude and will result in player most likely instant teleporting to where the spot leads to, but is a fail safe to not being able to be
				//placed. this should typically never be trigger, but HAS had a use before but has always resulted in the inability of the player to login, so this should exist
				uint16_t pos = uint16_t(rand()%teleportLocationList.size());
				*pX = teleportLocationList[pos]->m_sSrcX;
				*pY = teleportLocationList[pos]->m_sSrcY;
				return true;
			}
		}
	}

	if (pPlayerLocation == "NONE")
	{
		//apparently travelers always go to the first initial coordinate
		*pX = initialPointList[0].x;
		*pY = initialPointList[0].y;
		return true;
	}

	uint16_t pos = uint16_t(rand()%initialPointList.size());

	*pX = initialPointList[pos].x;
	*pY = initialPointList[pos].y;
	return true;
}

Tile * Map::GetTile(int16_t x, int16_t y)
{
	return (Tile *)(_tile + x + y*sizeY);
}

void Map::NpcProcess()
{
	int iMaxHP;
	uint64_t dwTime, dwActionTime;

	dwTime = unixtime();

	//TODO: either acquire a lock on every npc in list or --> add a mutex for npc deletion
	std::list<shared_ptr<Npc>> tempnpclist = npclist;

	for (shared_ptr<Npc> npc : tempnpclist)
	{
		if (npc == nullptr)
			continue;
		switch (npc->m_cBehavior)
		{
		case BEHAVIOR_ATTACK:
			dwActionTime = npc->timeActionInterval + 50 * dice(1, 7);
			break;
		case BEHAVIOR_MOVE:
		case BEHAVIOR_FLEE:
			dwActionTime = npc->timeActionInterval + 400;
			break;
		default:
			dwActionTime = npc->timeActionInterval;
			break;
		}

		if (npc->magicEffectStatus[MAGICTYPE_ICE] != 0)
			dwActionTime += (dwActionTime / 2);

		if ((dwTime - npc->timeLastAction) > dwActionTime) {
			npc->timeLastAction = dwTime;

			npc->RegenHP();
			npc->RegenMP();
			npc->Behave();

			if ((npc->health != 0) && (npc->summoned == true)) {
				switch (npc->Type()) {
				case 29:
					if ((dwTime - npc->timeSummoned) > 1000 * 90)
						NpcKilledHandler(nullptr, npc, 0);
					break;
				case 64:
					if ((dwTime - npc->timeSummoned) > PLANTTIME)
						DeleteNpc(npc);
					break;
				default:
					if ((dwTime - npc->timeSummoned) > SUMMONTIME)
						NpcKilledHandler(nullptr, npc, 0);
					break;
				}
			}
		}
	}
}

void Map::DeleteNpc(shared_ptr<Npc> npc)
{
	int  i, iNumItem, iItemID, iItemIDs[MAX_NPCITEMDROP], iSlateID;
	char cItemName[21];
	class Item * pItem, *pItem2;
	uint32_t dwCount, dwTime;
	Point ItemPositions[MAX_NPCITEMDROP];
	char cTemp[256];

	dwTime = unixtime();

	//Init number of items to 1 unless its a multidrop;
	iNumItem = 0;
	iItemID = 0; // No current item

	gserver->SendEventToNearClient_TypeA(npc.get(), MSGID_MOTION_EVENT_REJECT, 0, 0, 0);
	ClearDeadOwner(npc->x, npc->y);

	totalActiveObject--;

	if (flags.apocalypseMap)
	{
		if (npc->Type() == apocalypseBossMobNpcID)
		{
			RegisterDelayEvent(DELAYEVENTTYPE_END_APOCALYPSE, 0, unixtime() + 300 * 1000,
				0, 0, 0, 0, 0, 0);
			// open gate back to town
			if (npc->map != nullptr)
			{
				npc->map->dynamicGateType = 4;

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
			else if (npc->map->apocalypseMobGenType == AMGT_SPAWNBOSS)
			{
				//GenerateApocalypseBoss(maplist[iNpcH]->pMap);
			}
		}
	}


	if (npc->m_iSpotMobIndex != 0)
		spotMobGenerator[npc->m_iSpotMobIndex].current--;

	RemoveFromTarget(npc);

	switch (npc->Type())
	{
	case NPC_AGT:
	case NPC_CGT:
	case NPC_MS:
	case NPC_DT:
	case NPC_MANASTONE:
		npc->map->bRemoveCrusadeStructureInfo(npc->x, npc->y);

		for (i = 0; i < MAXGUILDS; i++) {
			if (gserver->m_pGuildTeleportLoc[i].m_iV1 == npc->guildGUID) {
				gserver->m_pGuildTeleportLoc[i].m_dwTime = dwTime;
				gserver->m_pGuildTeleportLoc[i].m_iV2--;
				if (gserver->m_pGuildTeleportLoc[i].m_iV2 < 0) gserver->m_pGuildTeleportLoc[i].m_iV2 = 0;
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
			if (gserver->m_pGuildTeleportLoc[i].m_iV1 == npc->guildGUID) {
				gserver->m_pGuildTeleportLoc[i].m_iNumSummonNpc--;
				if (gserver->m_pGuildTeleportLoc[i].m_iNumSummonNpc < 0) gserver->m_pGuildTeleportLoc[i].m_iNumSummonNpc = 0;
				break;
			}
		}
		break;

	case NPC_CROPS:
		npc->map->bRemoveCropsTotalSum();
		break;
	}


	RemoveFromDelayEventList(npc.get(), 0);

#ifdef NO_SECONDDROP
	return;
#endif

	if (npc->summoned)
		return;

	npclist.remove(npc);
	// 	for (std::list<shared_ptr<Npc>>::const_iterator iter = npclist.cbegin(); iter != npclist.cend(); ++iter)
	// 	{
	// 		if (*iter == npc)
	// 		{
	// 			npclist.remove(iter);
	// 			break;
	// 		}
	// 	}

	// 	pItem = new Item;
	// 	ZeroMemory(cItemName, sizeof(cItemName));
	// 
	// 	if (drops.HasSecondaryDrop(npc.get()))
	// 	{
	// 		if (drops.GetSecDropNum(npc->m_sType) == 1)
	// 			iItemID = drops.Roll(npc.get(), ONNPCDELETE);
	// 		else
	// 			iNumItem = RollMultiple(npc.get(), ITEMSPREAD_FIXED, 4, iItemIDs, ItemPositions);
	// 	}
	// 
	// 	dwCount = 1;
	// 
	// 
	// 	if (iNumItem > 0) {
	// 		GetLocalTime(&SysTime);
	// 		wsprintf(cTemp, "%d%02d%", SysTime.wMonth, SysTime.wDay);
	// 		for (int j = 0; j < iNumItem; j++){
	// 			if (pItem == NULL) {
	// 				pItem = new class CItem;
	// 			}
	// 			if (pItem->InitItemAttr(iItemIDs[j]) == false ||
	// 				maplist[npc->pMap]->bGetIsMoveAllowedTile(ItemPositions[j].x, ItemPositions[j].y) == false) {
	// 				delete pItem;
	// 				pItem = NULL;
	// 			}
	// 			else {
	// 				if (iItemIDs[j] == ITEM_GOLD)
	// 				{
	// 					if (npc->dwGoldDropValue <= 4) pItem->m_dwCount = dice(1, npc->dwGoldDropValue);
	// 					else
	// 						switch (dice(1, 2))
	// 					{
	// 						case 1:
	// 							pItem->m_dwCount = (uint32_t)(npc->dwGoldDropValue + dice(1, npc->dwGoldDropValue / 5));
	// 							break;
	// 
	// 						case 2:
	// 							pItem->m_dwCount = (uint32_t)(npc->dwGoldDropValue - dice(1, npc->dwGoldDropValue / 5));
	// 							break;
	// 					}
	// 				}
	// 				else
	// 					pItem->m_dwCount = dwCount;
	// 
	// 				pItem->m_sTouchEffectType = ITET_ID;
	// 				pItem->m_sTouchEffectValue1 = dice(1, 100000);
	// 				pItem->m_sTouchEffectValue2 = dice(1, 100000);
	// 				pItem->m_sTouchEffectValue3 = (short)dwTime;
	// 				if (!maplist[npc->pMap]->bSetItem(
	// 					ItemPositions[j].x, ItemPositions[j].y, pItem))
	// 				{
	// 					delete pItem;
	// 				}
	// 				else
	// 				{
	// 					SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, COMMONTYPE_ITEMDROP, npc->pMap,
	// 						ItemPositions[j].x, ItemPositions[j].y, pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor);
	// 					_bItemLog(ITEMLOG_NEWGENDROP, NULL, npc->m_cNpcName, pItem);
	// 					AddGroundItem(pItem, ItemPositions[j].x, ItemPositions[j].y, npc->pMap, TILECLEANTIMEPLAYER);
	// 				}
	// 				pItem = NULL;
	// 			}
	// 		}
	// 	}
	// 	else{
	// 		if (iItemID == 0 && npc->dwGoldDropValue > 0 && dice(1, 50) == 13) {
	// 			iItemID = ITEM_GOLD;
	// 			if (npc->dwGoldDropValue <= 4) dwCount = dice(1, npc->dwGoldDropValue);
	// 			else
	// 				switch (dice(1, 2))
	// 			{
	// 				case 1:
	// 					dwCount = (uint32_t)(npc->dwGoldDropValue + dice(1, npc->dwGoldDropValue / 5));
	// 					break;
	// 
	// 				case 2:
	// 					dwCount = (uint32_t)(npc->dwGoldDropValue - dice(1, npc->dwGoldDropValue / 5));
	// 					break;
	// 			}
	// 		}
	// 		if (pItem->InitItemAttr(iItemID) == false) {
	// 			delete pItem;
	// 			pItem = NULL;
	// 		}
	// 		else {
	// 
	// 			pItem->m_dwCount = dwCount;
	// 
	// 			pItem->m_sTouchEffectType = ITET_ID;
	// 			pItem->m_sTouchEffectValue1 = dice(1, 100000);
	// 			pItem->m_sTouchEffectValue2 = dice(1, 100000);
	// #ifdef LOGTIME
	// 			pItem->m_sTouchEffectValue3 = dwTime;
	// #else 
	// 			SYSTEMTIME SysTime;
	// 			char cTemp[256];
	// 			GetLocalTime(&SysTime);
	// 			ZeroMemory(cTemp, sizeof(cTemp));
	// 			//			wsprintf(cTemp, "%d%02d%02d",  (short)SysTime.wMonth, (short)SysTime.wDay,(short) SysTime.wHour);
	// 			wsprintf(cTemp, "%d%02d%", (short)SysTime.wMonth, (short)SysTime.wDay);
	// 
	// 			pItem->m_sTouchEffectValue3 = atoi(cTemp);
	// #endif
	// 			if (!maplist[npc->pMap]->bSetItem(
	// 				npc->m_sX, npc->m_sY, pItem))
	// 			{
	// 				delete pItem;
	// 			}
	// 			else
	// 			{
	// 				SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, COMMONTYPE_ITEMDROP, npc->pMap,
	// 					npc->m_sX, npc->m_sY,
	// 					pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor);
	// 
	// 				_bItemLog(ITEMLOG_NEWGENDROP, 0, npc->m_cNpcName, pItem);
	// 				AddGroundItem(pItem, npc->m_sX, npc->m_sY, npc->pMap, TILECLEANTIMEPLAYER);
	// 			}
	// 			pItem = NULL;
	// 		}
	// 	}
	// 
	// 	if (dice(1, 100000) < 60) {
	// 		pItem2 = new class Item;
	// 		switch (dice(1, 4)){
	// 		case 1:	iSlateID = 868; break;
	// 		case 2: iSlateID = 869; break;
	// 		case 3: iSlateID = 870; break;
	// 		case 4: iSlateID = 871; break;
	// 		}
	// 		if (pItem2->InitItemAttr(iSlateID) == false) {
	// 			delete pItem2;
	// 			pItem2 = NULL;
	// 		}
	// 		else {
	// 			pItem2->m_dwCount = 1;
	// 
	// 			pItem2->m_sTouchEffectType = ITET_ID;
	// 			pItem2->m_sTouchEffectValue1 = dice(1, 100000);
	// 			pItem2->m_sTouchEffectValue2 = dice(1, 100000);
	// 			pItem2->m_sTouchEffectValue3 = (short)dwTime;
	// 
	// 			if (!maplist[npc->pMap]->bSetItem(
	// 				npc->m_sX, npc->m_sY, pItem2))
	// 			{
	// 				delete pItem2;
	// 			}
	// 			else
	// 			{
	// 				SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, COMMONTYPE_ITEMDROP, npc->pMap,
	// 					npc->m_sX, npc->m_sY, pItem2->m_sSprite, pItem2->m_sSpriteFrame, pItem2->m_cItemColor);
	// 				//_bItemLog(ITEMLOG_NEWGENDROP, NULL, npc->m_cNpcName, pItem2);
	// 				AddGroundItem(pItem, npc->m_sX, npc->m_sY, npc->pMap, TILECLEANTIMEPLAYER);
	// 			}
	// 			pItem2 = NULL;
	// 		}
	// 	}
}
shared_ptr<Npc> Map::CreateNpc(string & pNpcName, char cSA, char cMoveType, uint16_t * poX, uint16_t * poY, Side changeSide, char * pWaypointList, rect * pArea, int iSpotMobIndex, bool bHideGenMode, bool bIsSummoned, bool bFirmBerserk, bool bIsMaster, int iGuildGUID)
{
	int i, t, j, k;
	char cTxt[120];
	short sX, sY, sRange;
	bool  bFlag;

	if (pNpcName.length() == 0) return 0;

	shared_ptr<Npc> newnpc(new Npc(++npchandle, this));
	newnpc->self = newnpc;

	switch (cSA)
	{
		// 	case 9:
		// 		newnpc = (Npc*)(new HTNpc(i));
		// 		break;
		// 	case 10:
		// 		newnpc = (Npc*)(new Elite(i));
		// 		break;
		//	default:
		//		newnpc = new Npc(++npchandle, this);
		//		break;
	}

	for (auto npc : gserver->m_npcConfigList)
	{
		if (npc && npc->name == pNpcName)
		{
			if (!newnpc->initNpcAttr(npc, cSA))
			{
				gserver->logger->error(Poco::format("(!) Error in Npc Creation (%s) Ignored.", pNpcName));
				return 0;
			}
			break;
		}
	}
	if (newnpc->name.length() == 0)
	{
		gserver->logger->error(Poco::format("(!) Non-existing NPC creation request! (%s) Ignored.", pNpcName));
		return 0;
	}

	newnpc->map = this;

	// 	if (newnpc->m_cDayOfWeekLimit < 10) 
	// 	{
	// 		if (newnpc->m_cDayOfWeekLimit != SysTime.wDayOfWeek) 
	// 		{
	// 			delete newnpc;
	// 			return false;
	// 		}
	// 	}

	switch (cMoveType)
	{
	case MOVETYPE_GUARD:
	case MOVETYPE_RANDOM:
		if ((poX != 0) && (poY != 0) && (*poX != 0) && (*poY != 0)) {
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
							return 0;
						}
					}
				}
			}
		}
		break;

	case MOVETYPE_RANDOMAREA:
		sRange = (short)(pArea->right - pArea->left);
		sX = (short)((rand() % sRange) + pArea->left);
		sRange = (short)(pArea->bottom - pArea->top);
		sY = (short)((rand() % sRange) + pArea->top);
		break;

	case MOVETYPE_RANDOMWAYPOINT:
		sX = (short)waypointList[pWaypointList[dice(1, 10) - 1]].x;
		sY = (short)waypointList[pWaypointList[dice(1, 10) - 1]].y;
		break;

	default:
		if ((poX != 0) && (poY != 0) && (*poX != 0) && (*poY != 0)) {
			sX = *poX;
			sY = *poY;
		}
		else {
			sX = (short)waypointList[pWaypointList[0]].x;
			sY = (short)waypointList[pWaypointList[0]].y;
		}
		break;
	}

	if (!bGetEmptyPosition(sX, sY, newnpc)/* ||
											(bHideGenMode && getPlayerNum(mapIndex, sX, sY, 7) > 0)*/)
	{
		return 0;
	}

	if ((poX != 0) && (poY != 0)) {
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

	if (pArea != 0) {
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

		newnpc->m_dX = (short)waypointList[newnpc->m_iWayPointIndex[newnpc->m_cCurWaypoint]].x;
		newnpc->m_dY = (short)waypointList[newnpc->m_iWayPointIndex[newnpc->m_cCurWaypoint]].y;
		break;

	case MOVETYPE_RANDOMWAYPOINT:
		newnpc->m_cCurWaypoint = (rand() % (newnpc->m_cTotalWaypoint - 1)) + 1;

		newnpc->m_dX = (short)waypointList[newnpc->m_iWayPointIndex[newnpc->m_cCurWaypoint]].x;
		newnpc->m_dY = (short)waypointList[newnpc->m_iWayPointIndex[newnpc->m_cCurWaypoint]].y;
		break;

	case MOVETYPE_RANDOMAREA:
		newnpc->m_cCurWaypoint = 0;

		sRange = (short)(newnpc->roamArea.right - newnpc->roamArea.left);
		newnpc->m_dX = (short)((rand() % sRange) + newnpc->roamArea.left);
		sRange = (short)(newnpc->roamArea.bottom - newnpc->roamArea.top);
		newnpc->m_dY = (short)((rand() % sRange) + newnpc->roamArea.top);
		break;

	case MOVETYPE_RANDOM://summon
		newnpc->m_dX = (short)((rand() % (sizeX - 50)) + 15);
		newnpc->m_dY = (short)((rand() % (sizeY - 50)) + 15);
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

		switch (newnpc->Type())
		{
		case 15:
		case 19:
		case 20:
		case 24:
		case 25:
		case 26:
			newnpc->direction = 4 + dice(1, 3) - 1;
			break;

		default:
			newnpc->direction = dice(1, 8);
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

	switch (newnpc->Type())
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		newnpc->appr2 = (short)0xF000;
		newnpc->appr2 = newnpc->appr2 | ((rand() % 13) << 4);
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

	newnpc->timeLastAction = unixtime() + (rand() % 10000);
	newnpc->timeMana = unixtime();
	newnpc->timeHealth = newnpc->timeMana;
	newnpc->m_sBehaviorTurnCount = 0;
	newnpc->summoned = bIsSummoned;
	newnpc->isLeader = bIsMaster;
	if (bIsSummoned == true)
		newnpc->timeSummoned = unixtime();

	if (bFirmBerserk == true) {
		newnpc->AddMagicEffect(MAGICTYPE_BERSERK, 0);
		newnpc->experience *= 2;
		newnpc->m_iNoDieRemainExp *= 2;
	}

	// !!!
	if (changeSide != -1)
	{
		newnpc->side = changeSide;
		newnpc->SetSideFlag(changeSide);
	}

	newnpc->m_cBravery = (rand() % 3) + newnpc->m_iMinBravery;
	newnpc->m_iSpotMobIndex = iSpotMobIndex;
	newnpc->guildGUID = iGuildGUID;


	//testcode
	// 	if (iGuildGUID != 0) {
	// 		wsprintf(g_cTxt, "Summon War Unit(%d) GUID(%d)", npc->m_sType, iGuildGUID);
	// 		PutLogList(g_cTxt);
	// 	}

	SetOwner(newnpc, sX, sY);
	totalActiveObject++;
	totalAliveObject++;

	npclist.push_back(newnpc);

	switch (newnpc->Type()) {
	case 36:
	case 37:
	case 38:
	case 39:
	case 42:
		bAddCrusadeStructureInfo(newnpc->Type(), sX, sY, newnpc->side);
		break;
	case 64:
		bAddCropsTotalSum();
		return newnpc;
		break;
	}

	gserver->SendEventToNearClient_TypeA(newnpc.get(), MSGID_MOTION_EVENT_CONFIRM, 0, 0, 0);
	return newnpc;
}

shared_ptr<Npc> Map::GetNpc(uint64_t ObjectID)
{
	for (shared_ptr<Npc> npc : npclist)
	{
		if (npc->handle == ObjectID)
			return npc;
	}
	return nullptr;
}

void Map::RemoveFromDelayEventList(Unit * unit, int32_t iEffectType)
{
	delayMutex.lock();
	std::list<shared_ptr<DelayEvent>> removedelaylist;

	for (shared_ptr<DelayEvent> delayevent : DelayEventList)
	{
		if (iEffectType != 0)
		{
			if ((delayevent->target->handle == unit->handle) && (delayevent->target->Type() == unit->Type()) && (delayevent->m_iEffectType == iEffectType))
			{
				removedelaylist.push_back(delayevent);
			}
		}
		else
		{
			if ((delayevent->target->handle == unit->handle) && (delayevent->target->Type() == unit->Type()))
			{
				removedelaylist.push_back(delayevent);
			}
		}
	}

	for (shared_ptr<DelayEvent> delayevent : removedelaylist)
	{
		DelayEventList.remove(delayevent);
	}
	delayMutex.unlock();
}

bool Map::RegisterDelayEvent(int iDelayType, int iEffectType, uint64_t dwLastTime, Unit * unit, int dX, int dY, int iV1, int iV2, int iV3)
{
	shared_ptr<DelayEvent> delayevent(new DelayEvent);


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

void Map::RemoveFromTarget(shared_ptr<Unit> target, int iCode)
{
	uint64_t dwTime = unixtime();

	for (shared_ptr<Npc> npc : npclist)
	{
		if ((npc->guildGUID != 0) && (target->IsPlayer()) && (target->guildGUID == npc->guildGUID))
		{
			if (npc->m_cActionLimit == 0)
			{
				npc->summoned = true;
				npc->timeSummoned = dwTime;
			}
		}

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
					npc->target = 0;
				}
				break;
			default:
				npc->m_cBehavior = BEHAVIOR_MOVE;
				npc->target = 0;
				break;
			}
		}
	}
}

void Map::NpcDeadItemGenerator(shared_ptr<Unit> attacker, shared_ptr<Npc> npc)
{
	return;
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

// 					gserver->SendNotifyMsg(nullptr, (Client*)attacker.get(),
// 						NOTIFY_DROPITEMFIN_COUNTCHANGED, itemID, 1, 0);
					return;
				}
			}
		}
	}
}

void Map::NpcKilledHandler(shared_ptr<Unit> attacker, shared_ptr<Npc> npc, int16_t damage)
{
	if (npc->_dead) return;

	npc->behavior_death(attacker, damage);

	if (attacker && attacker->IsPlayer())
	{
		// 		if (m_pClientList[sAttackerH] != NULL)
		// 		{
		// 			if (m_bNpcHunt)
		// 			{
		// 				NpcHuntPointsAdder(sAttackerH);
		// 			}
		// 		}
	}
	// Monster kill event xRisenx

	// 	if (attacker->m_ownerType == OWNERTYPE_PLAYER)
	// 	{
	// 		shared_ptr<Client> player = static_cast<Client*>(attacker.get())->self.lock();
	// 		switch (npc->m_sType) {
	// 		case NPC_ENRAGED_HELLCLAW: // Hellclaw 
	// 			if (player->_str > 400 && player->_dex > 400) // Warrior
	// 			{
	// 				player->m_iMonsterCount += 1;
	// 				if (player->m_iMonsterCount > 600)
	// 					player->m_iMonsterCount = 600;
	// 			}
	// 			else if (player->_int > 400 && player->_mag > 400) // Mage
	// 			{
	// 				player->m_iMonsterCount += 2;
	// 				if (player->m_iMonsterCount > 600)
	// 					player->m_iMonsterCount = 600;
	// 			}
	// 			break;
	// 
	// 
	// 		case NPC_ENRAGED_TIGERWORM: // Tigerworm
	// 			if (player->_str > 400 && player->_dex > 400) // Warrior
	// 			{
	// 				player->m_iMonsterCount += 1;
	// 				if (player->m_iMonsterCount > 600)
	// 					player->m_iMonsterCount = 600;
	// 			}
	// 			else if (player->_int > 400 && player->_mag > 400) // Mage
	// 			{ // Mage 
	// 				player->m_iMonsterCount += 2;
	// 				if (player->m_iMonsterCount > 600)
	// 					player->m_iMonsterCount = 600;
	// 			}
	// 			break;
	// 
	// 		default:
	// 			break;
	// 	}
}

void Map::NpcBehavior_Dead(shared_ptr<Npc> npc)
{
	uint32_t dwTime;

	if (npc == nullptr) return;

	dwTime = unixtime();
	npc->m_sBehaviorTurnCount++;
	if (npc->m_sBehaviorTurnCount > 5) {
		npc->m_sBehaviorTurnCount = 0;
	}

	if ((dwTime - npc->timeDead) > npc->timeRegen)
		DeleteNpc(npc);
}

void Map::NpcBehavior_Flee(shared_ptr<Npc> npc)
{
	char cDir;
	short sX, sY, dX, dY;
	shared_ptr<Unit> sTarget;

	if (npc == nullptr) return;
	if (npc->_dead == true) return;

	npc->m_sBehaviorTurnCount++;


	switch (npc->m_iAttackStrategy) {
	case ATTACKAI_EXCHANGEATTACK:
	case ATTACKAI_TWOBYONEATTACK:
		if (npc->m_sBehaviorTurnCount >= 2) {

			npc->m_cBehavior = BEHAVIOR_ATTACK;
			npc->m_sBehaviorTurnCount = 0;
			return;
		}
		break;

	default:
		//if (dice(1, 2) == 1) NpcRequestAssistance(iNpcH);
		break;
	}

	if (npc->m_sBehaviorTurnCount > 10) {

		npc->m_sBehaviorTurnCount = 0;
		npc->m_cBehavior = BEHAVIOR_MOVE;
		npc->m_tmp_iError = 0;
		if (npc->health <= 3) {
			npc->health += dice(1, npc->m_iHitDice);
			if (npc->health <= 0) npc->health = 1;
		}
		return;
	}

	sTarget = npc->TargetSearch();
	if (sTarget != nullptr) {
		npc->target = sTarget;
		npc->targetType = sTarget->OwnerType();
	}

	sX = npc->x;
	sY = npc->y;

	dX = npc->target->x;
	dY = npc->target->y;

	dX = sX - (dX - sX);
	dY = sY - (dY - sY);

	cDir = cGetNextMoveDir(sX, sY, dX, dY, npc->map, npc->turn, &npc->m_tmp_iError);
	if (cDir == 0) {
	}
	else {
		dX = npc->x + _tmp_cTmpDirX[cDir];
		dY = npc->y + _tmp_cTmpDirY[cDir];
		npc->map->ClearOwner(npc->x, npc->y);

		npc->map->SetOwner(npc, dX, dY);
		npc->x = dX;
		npc->y = dY;
		npc->direction = cDir;
		gserver->SendEventToNearClient_TypeA(npc.get(), MSGID_MOTION_MOVE, 0, 0, 0);
	}
}

char Map::cGetNextMoveDir(short sX, short sY, short dstX, short dstY, Map * map, char cTurn, int * pError)
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
	else CMisc::GetPoint(dX, dY, dstX, dstY, &iResX, &iResY, pError);

	cDir = CMisc::cGetNextMoveDir(dX, dY, iResX, iResY);

	if (cTurn == 0)
		for (i = cDir; i <= cDir + 7; i++) {
		cTmpDir = i;
		if (cTmpDir > 8) cTmpDir -= 8;
		aX = _tmp_cTmpDirX[cTmpDir];
		aY = _tmp_cTmpDirY[cTmpDir];
		if (map->bGetMoveable(dX + aX, dY + aY) == true) return cTmpDir;
		}

	if (cTurn == 1)
		for (i = cDir; i >= cDir - 7; i--) {
		cTmpDir = i;
		if (cTmpDir < 1) cTmpDir += 8;
		aX = _tmp_cTmpDirX[cTmpDir];
		aY = _tmp_cTmpDirY[cTmpDir];
		if (map->bGetMoveable(dX + aX, dY + aY) == true) return cTmpDir;
		}

	return 0;
}

char Map::cGetNextMoveDir(short sX, short sY, short dstX, short dstY, Map * map, char cTurn, int * pError, short * DOType)
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
	else CMisc::GetPoint(dX, dY, dstX, dstY, &iResX, &iResY, pError);

	cDir = CMisc::cGetNextMoveDir(dX, dY, iResX, iResY);

	if (cTurn == 0)
		for (i = cDir; i <= cDir + 7; i++) {
		cTmpDir = i;
		if (cTmpDir > 8) cTmpDir -= 8;
		aX = _tmp_cTmpDirX[cTmpDir];
		aY = _tmp_cTmpDirY[cTmpDir];
		if (map->bGetMoveable(dX + aX, dY + aY, DOType) == true) return cTmpDir;
		}

	if (cTurn == 1)
		for (i = cDir; i >= cDir - 7; i--) {
		cTmpDir = i;
		if (cTmpDir < 1) cTmpDir += 8;
		aX = _tmp_cTmpDirX[cTmpDir];
		aY = _tmp_cTmpDirY[cTmpDir];
		if (map->bGetMoveable(dX + aX, dY + aY, DOType) == true) return cTmpDir;
		}

	return 0;
}

bool Map::bGetEmptyPosition(short & x, short & y, shared_ptr<Unit> client)
{
	int i;
	int16_t sX, sY;

	for (i = 0; i < 25; i++)
		if ((bGetMoveable(x + _tmp_cEmptyPosX[i], y + _tmp_cEmptyPosY[i]) == true) &&
			(bGetIsTeleport(x + _tmp_cEmptyPosX[i], y + _tmp_cEmptyPosY[i]) == false))
		{
			x += _tmp_cEmptyPosX[i];
			y += _tmp_cEmptyPosY[i];
			return true;
		}

	if (client->IsPlayer())
		GetInitialPoint(&sX, &sY, static_cast<Client*>(client.get())->faction);
	else
	{
		string npt("NONE");
		GetInitialPoint(&sX, &sY, npt);
	}
	return false;
}

void Map::DelayEventProcessor()
{
	uint64_t _time = unixtime();
	delayMutex.lock();
	std::list<shared_ptr<DelayEvent>> tempDelayEventList = DelayEventList;
	for (auto delayevent : tempDelayEventList)
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
				if (delayevent->target != nullptr && delayevent->target->IsPlayer())
				{
					int skillnum = delayevent->m_iEffectType;

					Client * player = (Client*)delayevent->target;

					if (player->skillInUse[skillnum] == 0 || player->skillInUse[skillnum] != delayevent->m_iV2) break;

					player->skillInUse[skillnum] = 0;
					player->skillInUseTime[skillnum] = 0;

					int32_t result = gserver->CalculateUseSkillItemEffect(player, delayevent->m_iV1, skillnum, player->map, delayevent->m_dX, delayevent->m_dY);

					gserver->SendNotifyMsg(nullptr, player, NOTIFY_SKILLUSINGEND, result);
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
