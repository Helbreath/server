
#include "Map.h"
#include "Unit.h"
#include "Client.h"
#include "Npc.h"
#include "Tile.h"
#include "GServer.h"
#include <fstream>
#include "streams.h"
//#include "OccupyFlag.h"
//#include "StrategicPoint.h"
#include "TeleportLoc.h"
#include <lua5.2/lua.hpp>
#include <lua5.2/lauxlib.h>


Map::Map(GServer * pGame)
{
	m_sSizeX = m_sSizeY = 0;

	m_iTotalEnergySphereCreationPoint = 0;
	m_iTotalEnergySphereGoalPoint = 0;

	m_bIsEnergySphereGoalEnabled = false;
	m_iCurEnergySphereGoalPointIndex = -1;

	m_iTotalActiveObject = 0;
	m_iTotalAliveObject  = 0;

	m_bIsApocalypseMap		= false;
	m_iApocalypseMobGenType = AMGT_NONE;
	m_iApocalypseBossMobNpcID = 0;

	m_cDynamicGateType = 0;

	m_sDynamicGateCoordRectX1 = -2;
	m_sDynamicGateCoordRectY1 = -2;
	m_sDynamicGateCoordRectX2 = -2;
	m_sDynamicGateCoordRectY2 = -2;
	m_sDynamicGateCoordTgtX = 0;
	m_sDynamicGateCoordTgtY = 0;

	//m_sInitialPointX = 0;
	//m_sInitialPointY = 0;

	m_bIsFixedDayMode = false;
	m_bIsFixedSnowMode = false ; 

	m_iTotalFishPoint = 0;
	m_iMaxFish = 0;
	m_iCurFish = 0;

	m_iTotalMineralPoint = 0;
	m_iMaxMineral = 0;
	m_iCurMineral = 0;

	m_pTile = 0;

	m_weather = WEATHER_SUNNY;
	m_cType          = MAPTYPE_NORMAL;

	gserver = pGame;

	m_iLevelLimit = 0;
	m_iUpperLevelLimit = 0; 
	m_bMineralGenerator = false;

	m_iTotalOccupyFlags = 0;

	m_bIsAttackEnabled = true;
	m_cRandomMobGeneratorLevel = 0;

	for(int i = 0; i < MAXMAGICTYPE; i++)
		m_magicLimited[i] = false;

	m_isPartyDisabled = false;
	m_isShieldDisabled = false;
	m_isArmorDisabled = false;
	m_isChatDisabled = false;
	m_isPermIllusionOn = false;

	m_bIsFightZone = false;

	m_chatZone = 0;

	m_iMaxNx = m_iMaxNy = m_iMaxAx = m_iMaxAy = m_iMaxEx = m_iMaxEy = m_iMaxMx = m_iMaxMy = m_iMaxPx = m_iMaxPy = 0;

	m_bIsDisabled = false;

	m_bIsHeldenianMap		= false;

	m_bRandomMobGenerator = false;

	sMobEventAmount = 15 ;   
}

Map::~Map()
{
	if (m_pTile != 0) 
		delete []m_pTile;

	for (TeleportLoc * var : m_pTeleportLoc)
	{
		delete var;
	}
	for (OccupyFlag * var : m_pOccupyFlag)
	{
		delete var;
	}
	for (StrategicPoint * var : m_pStrategicPointList)
	{
		delete var;
	}
}

void Map::SetOwner(Unit * sOwner, short sX, short sY)
{
	Tile * pTile;

	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return;

	pTile = (Tile *)(m_pTile + sX + sY*m_sSizeY);
	pTile->owner      = sOwner;
	pTile->m_cOwnerClass = sOwner->IsPlayer()?OWNERTYPE_PLAYER:OWNERTYPE_NPC;
}


void Map::SetDeadOwner(Unit * sOwner, short sX, short sY)
{
	Tile * pTile;

	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return;

	pTile = (Tile *)(m_pTile + sX + sY*m_sSizeY);
	pTile->deadowner      = sOwner;
	pTile->m_cDeadOwnerClass = sOwner->IsPlayer()?OWNERTYPE_PLAYER:OWNERTYPE_NPC;
}

Unit * Map::GetOwner(short sX, short sY)
{
	Tile * pTile;

	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) {
		return 0;
	}

	pTile = (Tile *)(m_pTile + sX + sY*m_sSizeY);
	return pTile->owner;

}

Unit * Map::GetDeadOwner(short sX, short sY)
{
	Tile * pTile;

	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) {
		return 0;
	}

	pTile = (Tile *)(m_pTile + sX + sY*m_sSizeY);
	return pTile->deadowner;

}

Unit ** Map::GetOwners(short x1, short x2, short y1, short y2)
{
	//TODO: this. shoot whoever wrote it and rewrite it before an alpha test
	return 0;
// 	static Unit * owners[400];
// 
// 	if(x1 < 0)
// 		x1 = 0;
// 
// 	if(x2 >= m_sSizeX)
// 		x2 = m_sSizeX-1;
// 
// 	if(y1 < 0)
// 		y1 = 0;
// 
// 	if(y2 >= m_sSizeY)
// 		y2 = m_sSizeY-1;
// 
// 	int n = 0;
// 
// 	for(int y = y1; y <= y2; y++) 
// 	{
// 		for (int x = x1; x <= x2; x++)
// 		{
// 			pTile = (Tile *)(m_pTile + x + y*m_sSizeY);
// 
// 			switch(pTile->m_cOwnerClass)
// 			{
// 			case OWNERTYPE_PLAYER:
// 				owners[n] = g_clientList[pTile->m_sOwner];
// 				n++;
// 				break;;
// 
// 			case OWNERTYPE_NPC:
// 				owners[n] = g_npcList[pTile->m_sOwner];
// 				n++;
// 				break;
// 			}
// 		}
// 	}
// 
// 	owners[n] = NULL;
// 
// 	return owners;
}

bool Map::bGetMoveable(short dX, short dY, short * pDOtype, Item * pTopItem) 
{
	Tile * pTile;

	if ((dX < 20) || (dX >= m_sSizeX - 20) || (dY < 20) || (dY >= m_sSizeY - 20)) return false;

	pTile = (Tile *)(m_pTile + dX + dY*m_sSizeY);

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

	if ((dX < 20) || (dX >= m_sSizeX - 20) || (dY < 20) || (dY >= m_sSizeY - 20)) return false;

	pTile = (Tile *)(m_pTile + dX + dY*m_sSizeY);

	if (pTile->m_bIsMoveAllowed == false) return false;
	if (pTile->m_bIsTempMoveAllowed == false) return false;

	return true;
}

bool Map::bGetIsMoveAllowedTile(Point p)
{
	if (p.x < 20 || p.x >= m_sSizeX - 20 
		|| p.y < 20 || p.y >= m_sSizeY - 20
		) return false;

	Tile * pTile = (Tile *)(m_pTile + p.x + p.y*m_sSizeY);

	if (!pTile->m_bIsMoveAllowed || !pTile->m_bIsTempMoveAllowed) 
		return false;

	return true;
}

bool Map::bGetIsTeleport(short dX, short dY)
{
	Tile * pTile;

	if ((dX < 14) || (dX >= m_sSizeX - 16) || (dY < 12) || (dY >= m_sSizeY - 14)) return false;

	pTile = (Tile *)(m_pTile + dX + dY*m_sSizeY);

	if (pTile->m_bIsTeleport == false) return false;

	return true;
}

void Map::ClearOwner(short sX, short sY)
{
	Tile * pTile;

	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return;

	pTile = (Tile *)(m_pTile + sX + sY*m_sSizeY);
	pTile->owner      = 0;
	pTile->m_cOwnerClass = 0;
}

void Map::ClearDeadOwner(short sX, short sY)
{
	Tile * pTile;

	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return;

	pTile = (Tile *)(m_pTile + sX + sY*m_sSizeY);
	pTile->deadowner      = 0;
	pTile->m_cDeadOwnerClass = 0;
}

bool Map::bSetItem(short sX, short sY, Item * pItem)
{
	Tile * pTile;

	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return false; 

	pTile = (Tile *)(m_pTile + sX + sY*m_sSizeY);

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

	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return 0;

	pTile = (Tile *)(m_pTile + sX + sY*m_sSizeY);
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
		*pRemainItemSprite      = pItem->m_sSprite;
		*pRemainItemSpriteFrame = pItem->m_sSpriteFrame;
		*pRemainItemColor       = pItem->m_ItemColor;
	}

	return pItem;
}


uint64_t Map::iCheckItem(short sX, short sY)
{
	Tile * pTile;
	Item * pItem;

	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return 0;

	pTile = (Tile *)(m_pTile + sX + sY*m_sSizeY);
	if (pTile->m_pItem.size() == 0) return 0;
	pItem =  pTile->m_pItem[0];

	return pItem->m_sIDnum;
}


bool Map::bIsValidLoc(short sX, short sY)
{
	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return false;
	return true;
}

bool Map::bInit(string pName)
{
	m_cName = pName;

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
	mapfile += m_cName + ".lua";

	lua_State * L = gserver->L;
	try
	{
		if (luaL_dofile(L, mapfile.c_str()) != 0)
		{
			gserver->consoleLogger->fatal(Poco::format("%s", (string)lua_tostring(L,-1)));
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

							m_pTeleportLoc.push_back(loc);

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

							m_WaypointList.push_back(point(sX, sY));

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

					m_bRandomMobGenerator = (gen>0)?true:false;
					m_cRandomMobGeneratorLevel = level;
				}
				else if (option == "maximumobject")
				{
					m_iMaximumObject = (uint16_t)lua_tointeger(L, -1);
				}
				else if (option == "npcavoidrect")
				{

				}
				else if (option == "spotmobgenerator")
				{

				}
				else if (option == "name")
				{
					this->m_cLocationName = (char*)lua_tostring(L, -1);;
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

							m_pInitialPoint.push_back(point(sX, sY));

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

							m_rcNoAttackRect.push_back(rect(sX, sY, dX, dY));

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
					m_iMaxFish = (uint32_t)lua_tointeger(L, -1);
				}
				else if (option == "type")
				{

				}
				else if (option == "levellimit")
				{
					m_iLevelLimit = (uint16_t)lua_tointeger(L, -1);
				}
				else if (option == "mineralgenerator")
				{

				}
				else if (option == "mineralpoint")
				{

				}
				else if (option == "maxmineral")
				{
					m_iMaxMineral = (uint16_t)lua_tointeger(L, -1);
				}
				else if (option == "upperlevellimit")
				{
					m_iUpperLevelLimit = (uint16_t)lua_tointeger(L, -1);
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
		gserver->consoleLogger->fatal(Poco::format("bDecodeMapConfig() Exception: %s", (string)e.what()));
		return false;
	}
	catch(...)
	{
		gserver->consoleLogger->fatal("Unspecified bDecodeMapConfig() Exception.");
		return false;
	}


	return true;
}

bool Map::_bDecodeMapDataFileContents()
{
	string mapfile = "mapdata/";
	mapfile += m_cName + ".amd";
	FILE * file;
	char * buffer;
	file = fopen(mapfile.c_str(), "r");

	if (!file)
	{
		//not existing
		gserver->logger->fatal(Poco::format("Loaded map does not exist: %s", m_cName));
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
		m_sSizeX = atoi(tok);
	}
	tok = strtok_s(0, "\t\n =", &ptr);
	if (memcmp(tok, "MAPSIZEY",8) == 0)
	{
		tok = strtok_s(0, "\t\n =", &ptr);
		m_sSizeY = atoi(tok);
	}
	tok = strtok_s(0, "\t\n =", &ptr);
	if (memcmp(tok, "TILESIZE",8) == 0)
	{
		tok = strtok_s(0, "\t\n =", &ptr);
		m_sTileDataSize = atoi(tok);
	}

	m_pTile = (Tile *)new Tile[m_sSizeX * m_sSizeY];

	char * tileData = new char[m_sTileDataSize];
	Tile * pTile;

	for (int iy = 0; iy < m_sSizeY; iy++)
	{
		for (int ix = 0; ix < m_sSizeX; ix++)
		{
			fr.ReadBytes(tileData, m_sTileDataSize);
			pTile = (Tile *)(m_pTile + ix + iy*m_sSizeY);
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

	for (i = 0; i < m_pTeleportLoc.size(); i++)
		if ((m_pTeleportLoc[i]->m_sSrcX == sX) && (m_pTeleportLoc[i]->m_sSrcY == sY))
		{
			pMapName = m_pTeleportLoc[i]->m_cDestMapName;
			*pDx  = m_pTeleportLoc[i]->m_sDestX;
			*pDy  = m_pTeleportLoc[i]->m_sDestY;
			*pDir = m_pTeleportLoc[i]->m_cDir;
			return true;
		}

	return false;
}

void Map::SetDynamicObject(uint16_t wID, short sType, short sX, short sY, uint64_t dwRegisterTime)
{
	Tile * pTile;


	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return; 

	pTile = (Tile *)(m_pTile + sX + sY*m_sSizeY);

	pTile->m_wDynamicObjectID   = wID;
	pTile->m_sDynamicObjectType = sType;
	pTile->m_dwDynamicObjectRegisterTime = dwRegisterTime;
}

bool Map::bGetDynamicObject(short sX, short sY, short *pType, uint64_t *pRegisterTime, int * pIndex)
{
	Tile * pTile;


	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return false; 

	pTile = (Tile *)(m_pTile + sX + sY*m_sSizeY);

	*pType = pTile->m_sDynamicObjectType;
	*pRegisterTime = pTile->m_dwDynamicObjectRegisterTime;
	if (pIndex != 0) *pIndex = pTile->m_wDynamicObjectID;

	return true;
}

bool Map::bGetIsWater(short dX, short dY)
{
	Tile * pTile;

	if ((dX < 14) || (dX >= m_sSizeX - 16) || (dY < 12) || (dY >= m_sSizeY - 14)) return false;

	pTile = (Tile *)(m_pTile + dX + dY*m_sSizeY);

	if (pTile->m_bIsWater == false) return false;

	return true;
}



bool Map::bGetIsFarm(short dX, short dY)
{
	Tile * pTile;

	if ((dX < 14) || (dX >= m_sSizeX - 16) || (dY < 12) || (dY >= m_sSizeY - 14)) return false;

	pTile = (Tile *)(m_pTile + dX + dY*m_sSizeY);

	if (pTile->m_bIsFarmingAllowed == false) return false;

	return true;
}

bool Map::IsBuild(short dX, short dY)
{
	Tile * pTile;

	if ((dX < 14) || (dX >= m_sSizeX - 16) || (dY < 12) || (dY >= m_sSizeY - 14)) return false;

	pTile = (Tile *)(m_pTile + dX + dY*m_sSizeY);

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

	if ((dX < 20) || (dX >= m_sSizeX - 20) || (dY < 20) || (dY >= m_sSizeY - 20)) return;

	pTile = (Tile *)(m_pTile + dX + dY*m_sSizeY);
	pTile->m_bIsTempMoveAllowed = bFlag;
}

Map::OccupyFlag * Map::iRegisterOccupyFlag(int dX, int dY, int iSide, int iEKNum, int iDOI)
{
	if ((dX < 20) || (dX >= m_sSizeX - 20) || (dY < 20) || (dY >= m_sSizeY - 20)) return 0;

	OccupyFlag * oflag = new OccupyFlag(dX, dY, iSide, iEKNum, iDOI);

	m_pOccupyFlag.push_back(oflag);

	return oflag;
}



void Map::ClearSectorInfo()
{
	for (int i = 0; i < MAXSECTORS*MAXSECTORS; ++i)
	{
		m_stSectorInfo[i].iNeutralActivity = 0;
		m_stSectorInfo[i].iAresdenActivity = 0;
		m_stSectorInfo[i].iElvineActivity  = 0;
		m_stSectorInfo[i].iMonsterActivity = 0;
		m_stSectorInfo[i].iPlayerActivity  = 0;
	}
}

void Map::ClearTempSectorInfo()
{
	for (int i = 0; i < MAXSECTORS*MAXSECTORS; ++i)
	{
		m_stTempSectorInfo[i].iNeutralActivity = 0;
		m_stTempSectorInfo[i].iAresdenActivity = 0;
		m_stTempSectorInfo[i].iElvineActivity  = 0;
		m_stTempSectorInfo[i].iMonsterActivity = 0;
		m_stTempSectorInfo[i].iPlayerActivity  = 0;
	}
}

void Map::_SetupNoAttackArea()
{
	int i, ix, iy;
	Tile * pTile;

	for (i = 0; i < m_rcNoAttackRect.size(); i++) {
		if (m_rcNoAttackRect[i].top > 0) {

			for (ix = m_rcNoAttackRect[i].left; ix <= m_rcNoAttackRect[i].right; ix++)
				for (iy = m_rcNoAttackRect[i].top; iy <= m_rcNoAttackRect[i].bottom; iy++) {
					pTile = (Tile *)(m_pTile + ix + iy*m_sSizeY);
					pTile->m_attribute |= 0x00000004;
				}
		}

		else if (m_rcNoAttackRect[i].top == -10) {
			for (ix = 0; ix < m_sSizeX; ix++)
				for (iy = 0; iy < m_sSizeY; iy++) {
					pTile = (Tile *)(m_pTile + ix + iy*m_sSizeY);
					pTile->m_attribute |= 0x00000004;
				}
		}
	}
}

int Map::iGetAttribute(int dX, int dY, int iBitMask)
{
	Tile * pTile;

	if ((dX < 20) || (dX >= m_sSizeX - 20) || (dY < 20) || (dY >= m_sSizeY - 20)) return -1;

	pTile = (Tile *)(m_pTile + dX + dY*m_sSizeY);
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
	for (int i = 0; i < m_stStrikePoint.size(); i++)
		m_stStrikePoint[i].iHP = m_stStrikePoint[i].iInitHP;
}

void Map::IncPlayerActivity(Client * player)
{
	int iStX, iStY;
	return;

	//TODO: vector or 2d array? need to initialize vector

	iStX = player->m_sX / 20;
	iStY = player->m_sY / 20;
	m_stTempSectorInfo[iStX+iStY*MAXSECTORS].iPlayerActivity++;

	switch (player->m_side)
	{
		case NEUTRAL: m_stTempSectorInfo[iStX+iStY*MAXSECTORS].iNeutralActivity++; break;
		case ARESDEN: m_stTempSectorInfo[iStX+iStY*MAXSECTORS].iAresdenActivity++; break;
		case ELVINE: m_stTempSectorInfo[iStX+iStY*MAXSECTORS].iElvineActivity++;  break;
	}
}

bool Map::GetInitialPoint(int16_t *pX, int16_t *pY, string & pPlayerLocation)
{
	//every map has to have an initial point as a "fail safe". if none exists by some stupid error, send back to town

	if (m_pInitialPoint.size() == 0)
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
			if (m_pTeleportLoc.size() == 0)
			{
				return false;
			}
			else
			{
				//this is incredibly crude and will result in player most likely instant teleporting to where the spot leads to, but is a fail safe to not being able to be
				//placed. this should typically never be trigger, but HAS had a use before but has always resulted in the inability of the player to login, so this should exist
				uint16_t pos = uint16_t(rand()%m_pTeleportLoc.size());
				*pX = m_pTeleportLoc[pos]->m_sSrcX;
				*pY = m_pTeleportLoc[pos]->m_sSrcY;
				return true;
			}
		}
	}

	if (pPlayerLocation == "NONE")
	{
		//apparently travelers always go to the first initial coordinate
		*pX = m_pInitialPoint[0].x;
		*pY = m_pInitialPoint[0].y;
		return true;
	}

	uint16_t pos = uint16_t(rand()%m_pInitialPoint.size());

	*pX = m_pInitialPoint[pos].x;
	*pY = m_pInitialPoint[pos].y;
	return true;
}

