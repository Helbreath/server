
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

	if (pTile->m_bIsWater == false) return false;

	return true;
}



bool Map::bGetIsFarm(short dX, short dY)
{
	Tile * pTile;

	if ((dX < 14) || (dX >= sizeX - 16) || (dY < 12) || (dY >= sizeY - 14)) return false;

	pTile = (Tile *)(_tile + dX + dY*sizeY);

	if (pTile->m_bIsFarmingAllowed == false) return false;

	return true;
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

bool Map::GetInitialPoint(int16_t *pX, int16_t *pY, string & pPlayerLocation)
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
