#include "GServer.h"
#include "Client.h"
#include "Npc.h"
#include "Map.h"
#include "Tile.h"
#include "streams.h"
#include "netmessages.h"
#include "Misc.h"
#include "DelayEvent.h"
#include "Skill.h"
using namespace Poco::JSON;

int AddNpc(lua_State *L);

int AddNpc(lua_State *L)
{
	//lua_register(m_lua,"loadlua",loadlua);
	int n=lua_gettop(L);
	return 0;
}

extern int _tmp_iMoveLocX[9][44];
extern int _tmp_iMoveLocY[9][44];

GServer::GServer(string servername, string config)
	: Server(config)
{

	m_bIsCrusadeMode = false;
	m_cDayOrNight = 0;

	for (int i = 0; i < MAXNPCTYPES; ++i)
		m_npcConfigList[i] = 0;
	for (int i = 0; i < MAXITEMTYPES; ++i)
		m_pItemConfigList[i] = 0;

	for (int i = 0; i < MAXNPCTYPES; ++i)
		for (int j = 0; j < MAXITEMTYPES; ++j)
			m_npcDropData[i][j] = 0;

	for (int i = 0; i <= 100; i++)
		m_iSkillSSNpoint[i] = _iCalcSkillSSNpoint(i);
	for (int i = 0; i < MAXMAGICTYPE; ++i)
		m_pMagicConfigList[i] = 0;

// 	GSID = -1;
// 	m_bIsGameServerRegistered = false;
// 	ReceivedAllConfig		= false;
// 	m_bIsGameStarted = false;
// 	m_bIsItemAvailable      = false;
// 	m_bIsBuildItemAvailable = false;
// 	m_bIsNpcAvailable       = false;
// 	m_bIsMagicAvailable     = false;
// 	m_bIsSkillAvailable     = false;
// 	m_bIsQuestAvailable     = false;
// 	m_bIsPortionAvailable   = false;
// 	m_dropsInitiated			= false;
//
// 	ZeroMemory(m_cServerName, sizeof(m_cServerName));
//
// 	m_sForceRecallTime = 0 ;
//
// 	g_clientList = m_pClientList;
// 	for (i = 0; i < MAXCLIENTS; i++)
// 		m_pClientList[i] = NULL;
//
// 	g_mapList = m_pMapList;
// 	for (i = 0; i < MAXMAPS; i++)
// 		m_pMapList[i] = NULL;
//
// 	g_itemConfigList = m_pItemConfigList;
// 	for (i = 0; i < MAXITEMTYPES; i++)
// 		m_pItemConfigList[i] = NULL;
//
// 	g_npcConfigList = m_npcConfigList;
// 	for (i = 0; i < MAXNPCTYPES; i++)
// 		m_npcConfigList[i] = NULL;
//
// 	g_npcList = m_pNpcList;
// 	for (i = 0; i < MAXNPCS; i++)
// 		m_pNpcList[i] = NULL;
//
// 	for (i = 0; i < MSGQUENESIZE; i++)
// 		m_pMsgQuene[i] = NULL;
//
// 	g_magicConfigList = m_pMagicConfigList;
// 	for (i = 0; i < MAXMAGICTYPE; i++)
// 		m_pMagicConfigList[i] = NULL;
//
// 	for (i = 0; i < MAXSKILLTYPE; i++)
// 		m_pSkillConfigList[i] = NULL;
//
// 	for (i = 0; i < MAXQUESTTYPE; i++)
// 		m_pQuestConfigList[i] = NULL;
//
// 	for (i = 0; i < MAXDYNAMICOBJECTS; i++)
// 		m_pDynamicObjectList[i] = NULL;
//
// 	for (i = 0; i < MAXDELAYEVENTS; i++)
// 		m_pDelayEventList[i] = NULL;
//
// 	for (i = 0; i < MAXNOTIFYMSGS; i++)
// 		m_pNoticeMsgList[i] = NULL;
//
// 	for (i = 0; i < MAXFISHS; i++)
// 		m_pFish[i] = NULL;
//
// 	for (i = 0; i < MAXMINERALS; i++)
// 		m_pMineral[i] = NULL;
//
// 	for (i = 0; i < MAXPORTIONTYPES; i++)
// 		m_pPortionConfigList[i] = NULL;
//
// 	for (i = 0; i < MAXCRAFTING; i++)
// 		m_pCraftingConfigList[i] = NULL;
//
// 	for (i = 0; i < MAXSUBLOGSOCK; i++) {
// 		m_pSubLogSock[i] = NULL;
// 		m_bIsSubLogSockAvailable[i] = false;
// 		m_bIsSocketConnected[i] = false;
// 	}
//
// 	for (i = 0; i < MAXBUILDITEMS; i++)
// 		m_pBuildItemList[i] = NULL;
//
// 	for (i = 0; i < MAXDUPITEMID; i++)
// 		m_pDupItemIDList[i] = NULL;
//
// 	for ( i = 0; i < MAXGROUNDITEMS; i++ )
// 	{
// 		m_stGroundNpcItem[i].bEmpty = true;
// 		m_stGroundNpcItem[i].cMapIndex = 0;
// 		m_stGroundNpcItem[i].dropTime = 0;
// 		m_stGroundNpcItem[i].item = NULL;
// 		m_stGroundNpcItem[i].sx = m_stGroundNpcItem[i].sy = 0;
// 	}
//
// 	m_iQueneHead = 0;
// 	m_iQueneTail = 0;
//
// 	m_iTotalClients = 0;
// 	m_iMaxClients   = 0;
// 	m_iTotalMaps    = 0;
//
// 	for(int i = 0; i < MAXMAPNAMES; i++)
// 	{
// 		m_mapNameList[ GetMapName(i) ] = i;
// 	}
//
// 	for(int i = 0; i < SG_MAX; i++)
// 	{
// 		g_socketMap[ SocketGems[i] ] = i;
// 	}
//
// 	m_iTotalGameServerClients    = 0;
// 	m_iTotalGameServerMaxClients = 0;
//
// 	m_bIsServerShutdowned = false;
// 	m_cShutDownCode = 0;
//
// 	m_iMiddlelandMapIndex = -1;
// 	m_iAresdenOccupyTiles = 0;
// 	m_iElvineOccupyTiles  = 0;
//
// 	m_iCurMsgs = 0;
// 	m_iMaxMsgs = 0;
//
// 	for(int i=0; i < MAXSIDES; i++)
// 	{
// 		m_stCityStatus[i].iCrimes = 0;
// 		m_stCityStatus[i].iFunds  = 0;
// 		m_stCityStatus[i].iWins   = 0;
// 	}
// 	m_sSlateSuccessRate		= 0;
//
// 	m_iAutoRebootingCount = 0;
//
// 	m_pGold = NULL ;
//
// 	m_eventsIni = NULL;
//
// 	// 2002-09-09 #1
// 	m_bReceivedItemList = false;

	chatthread = 0;

	//asio socket thread count
	currentplayersonline = 0;
	currentconnections = 0;

	this->servername = servername;

	sqlpool = 0;

	farmRestartLimit = 20;
	lockedMapTimeDefault = 180;//3 minutes

	//TODO: put name of server in console messages?
	pFCConsole = new FormattingChannel(new PatternFormatter("GServer: %p: %t"));
	pFCConsole->setChannel(new ConsoleChannel);
	pFCConsole->open();

	pFCDefault= new FormattingChannel(new PatternFormatter("GServer: %t"));
	pFCDefault->setChannel(new ConsoleChannel);
	pFCDefault->open();

	pFCFile = new FormattingChannel(new PatternFormatter("%Y-%m-%d %H:%M:%S.%c | %s:%q:%t"));
	pFCFile->setChannel(new FileChannel("gserver.log"));
	pFCFile->setProperty("rotation", "daily");
	pFCFile->setProperty("times", "local");
	pFCFile->open();

	consoleLogger = &Poco::Logger::create(string("ConsoleLogger")+servername, pFCConsole, Message::PRIO_TRACE);
	logger		  = &Poco::Logger::create(string("DefaultLogger")+servername, pFCDefault, Message::PRIO_TRACE);
	fileLogger    = &Poco::Logger::create(string("FileLogger")+servername, pFCFile, Message::PRIO_TRACE);
	poco_information(*logger, "");
	poco_information(*logger, Poco::format("Server %s instantiated", servername));
// 	consoleLogger->error("An error message");
// 	consoleLogger->warning("A warning message");
// 	consoleLogger->information("An information message");
// 	fileLogger->warning("An information message");
// 	if ((consoleLogger)->information())
// 		(consoleLogger)->information("Another informational message", __FILE__, __LINE__);
// 	Poco::Logger::get("ConsoleLogger").error("Another error message");


	MySQL::Connector::registerConnector();
	SQLite::Connector::registerConnector();


	L = luaL_newstate();
	luaL_openlibs(L);
	TimerThreadRunning = false;
}

GServer::~GServer()
{
	for (Map * pmap : maplist)
	{
		delete pmap;
	}
	for (int i = 0; i < MAXNPCTYPES; ++i)
	{
		if (m_npcConfigList[i])
			delete m_npcConfigList[i];
	}
	for (int i = 0; i < MAXITEMTYPES; ++i)
	{
		if (m_pItemConfigList[i])
			delete m_pItemConfigList[i];
	}
	delete sqlpool;
	MySQL::Connector::unregisterConnector();
	SQLite::Connector::unregisterConnector();
	Poco::Logger::destroy(string("ConsoleLogger")+servername);
	Poco::Logger::destroy(string("DefaultLogger")+servername);
	Poco::Logger::destroy(string("FileLogger")+servername);
	pFCConsole->close();
	pFCDefault->close();
	pFCFile->close();
}

//TODO: remove these and finish replacing
void GServer::PutLogFileList(char * str)
{
	poco_information(*fileLogger, str);
}
void GServer::PutLogList(char * str)
{
	poco_information(*logger, str);
}

void GServer::run()
{
	try
	{
		//TODO: Redo entire threading model
		//Map classes themselves hold all the major threads while GServer only handles some basic timers and chat

		serverstatus = SERVERSTATUS_ONLINE;
		logger->information("Creating Server threads.");

		for (auto map : maplist)
		{
			map->run();
		}

		chatthread = new std::thread(std::bind(std::mem_fun(&GServer::ChatThread), this));
		timerthread = new std::thread(std::bind(std::mem_fun(&GServer::TimerThread), this));

		chatthread->join();
		timerthread->join();

		delete chatthread;

		lua_close(L);


	}
	catch (std::exception& e)
	{
		consoleLogger->fatal(Poco::format("Server::run() Exception: %s", e.what() ));
	}
}

void GServer::handle_stop()
{
	logger->information("handle_stop()");
	this->serverstatus = SERVERSTATUS_SHUTDOWN;
}
extern void stack_dump(lua_State *stack);

bool GServer::Init()
{
	try
	{
		consoleLogger->information("Loading Config.");
		if (luaL_dofile(L, configfile.c_str()) != 0)
		{
			poco_fatal(*consoleLogger, Poco::format("%s", (string)lua_tostring(L,-1)));
			return false;
		}
		lua_getglobal(L, "config");

		char * temp;

		//sql host
		{
			lua_getfield(L, -1, "sqlhost");
			temp = (char*)lua_tostring(L, -1);
			if (temp == 0) { consoleLogger->fatal("Invalid sqlhost setting."); return false; }
			sqlhost = temp;
			logger->information(Poco::format("sqlhost: %s", sqlhost));
			lua_pop(L, 1);
		}

		//sql user
		{
			lua_getfield(L, -1, "sqluser");
			temp = (char*)lua_tostring(L, -1);
			if (temp == 0) { consoleLogger->fatal("Invalid sqluser setting."); return false; }
			sqluser = temp;
			logger->information(Poco::format("sqluser: %s", sqluser));
			lua_pop(L, 1);
		}

		//sql pass
		{
			lua_getfield(L, -1, "sqlpass");
			temp = (char*)lua_tostring(L, -1);
			if (temp == 0) { consoleLogger->fatal("Invalid sqlpass setting."); return false; }
			logger->information("sqlpass set");
			sqlpass = temp;
			lua_pop(L, 1);
		}



		//load maps
		{
			lua_getfield(L, -1, "maps");
			if(lua_istable(L,-1))
			{
				uint8_t tableSize = uint8_t(lua_rawlen(L, -1));
				for (uint8_t i = 1; i <= tableSize; ++i)
				{
					lua_pushinteger(L, i);
					lua_gettable(L, -2);

					string mapname = (char*)lua_tostring(L, -1);
					lua_pop(L, 1);

					Map * map = new Map(this);
					try
					{
						if (!map->bInit(mapname))
						{
							//map failed to load
							return false;
						}
						maplist.push_back(map);
					}
					catch (...)
					{
						poco_fatal(*logger, "Failed to load map.");
						return false;
					}

					poco_information(*logger, Poco::format("Loading map: %s", mapname));
				}
				lua_pop(L,1);
			}
		}


		lua_pop(L, 1);

		consoleLogger->information("Loading Npcs.");
		if (luaL_dofile(L, "npc.lua") != 0)
		{
			consoleLogger->fatal(Poco::format("%s", (string)lua_tostring(L,-1)));
			return false;
		}
		lua_getglobal(L, "npcs");

		if(lua_istable(L,-1))
		{
			lua_pushnil(L);
			while (lua_next(L, -2))
			{
				uint8_t tableSize = uint8_t(lua_rawlen(L, -1));
				Npc * npc = new Npc(0, 0);
				lua_pushinteger(L, 1);
				lua_gettable(L, -2);
				npc->name = lua_tostring(L, -1);
				lua_pop(L, 1);
				lua_pushinteger(L, 2);
				lua_gettable(L, -2);
				npc->SetType((uint8_t)lua_tointeger(L, -1));
				npc->SetTypeOriginal(npc->Type());
				lua_pop(L, 1);
				lua_pushinteger(L, 3);
				lua_gettable(L, -2);
				npc->m_iHitDice = (uint32_t)lua_tointeger(L, -1);
				lua_pop(L, 1);
				lua_pushinteger(L, 4);
				lua_gettable(L, -2);
				npc->m_iDefenseRatio = (uint32_t)lua_tointeger(L, -1);
				lua_pop(L, 1);
				lua_pushinteger(L, 5);
				lua_gettable(L, -2);
				npc->m_iHitRatio = (uint32_t)lua_tointeger(L, -1);
				lua_pop(L, 1);
				lua_pushinteger(L, 6);
				lua_gettable(L, -2);
				npc->m_iMinBravery = (uint32_t)lua_tointeger(L, -1);
				lua_pop(L, 1);
				lua_pushinteger(L, 7);
				lua_gettable(L, -2);
				npc->m_iExpDice = (uint32_t)lua_tointeger(L, -1);
				lua_pop(L, 1);
				lua_pushinteger(L, 8);
				lua_gettable(L, -2);
				npc->m_cAttackDiceThrow = (uint8_t)lua_tointeger(L, -1);
				lua_pop(L, 1);
				lua_pushinteger(L, 9);
				lua_gettable(L, -2);
				npc->m_cAttackDiceRange = (uint8_t)lua_tointeger(L, -1);
				lua_pop(L, 1);
				lua_pushinteger(L, 10);
				lua_gettable(L, -2);
				npc->m_cSize = (int8_t)lua_tointeger(L, -1);
				lua_pop(L, 1);
				lua_pushinteger(L, 11);
				lua_gettable(L, -2);
				npc->side = (Side)lua_tointeger(L, -1);
				lua_pop(L, 1);
				lua_pushinteger(L, 12);
				lua_gettable(L, -2);
				npc->m_cActionLimit = (int8_t)lua_tointeger(L, -1);
				lua_pop(L, 1);
				lua_pushinteger(L, 13);
				lua_gettable(L, -2);
				npc->timeActionInterval = (uint64_t)lua_tointeger(L, -1);
				lua_pop(L, 1);
				lua_pushinteger(L, 14);
				lua_gettable(L, -2);
				npc->m_cResistMagic = (int8_t)lua_tointeger(L, -1);
				lua_pop(L, 1);
				lua_pushinteger(L, 15);
				lua_gettable(L, -2);
				npc->m_cMagicLevel = (int8_t)lua_tointeger(L, -1);
				lua_pop(L, 1);
				lua_pushinteger(L, 16);
				lua_gettable(L, -2);
				npc->m_cDayOfWeekLimit = (int8_t)lua_tointeger(L, -1);
				lua_pop(L, 1);
				lua_pushinteger(L, 17);
				lua_gettable(L, -2);
				npc->m_cChatMsgPresence = (int8_t)lua_tointeger(L, -1);
				lua_pop(L, 1);
				lua_pushinteger(L, 18);
				lua_gettable(L, -2);
				npc->m_cTargetSearchRange = (int8_t)lua_tointeger(L, -1);
				lua_pop(L, 1);
				lua_pushinteger(L, 19);
				lua_gettable(L, -2);
				npc->timeRegen = (uint64_t)lua_tointeger(L, -1);
				lua_pop(L, 1);
				lua_pushinteger(L, 20);
				lua_gettable(L, -2);
				npc->element = (Element)lua_tointeger(L, -1);
				lua_pop(L, 1);
				lua_pushinteger(L, 21);
				lua_gettable(L, -2);
				npc->absDamage = (uint32_t)lua_tointeger(L, -1);
				lua_pop(L, 1);
				lua_pushinteger(L, 22);
				lua_gettable(L, -2);
				npc->m_iMaxMana = (uint32_t)lua_tointeger(L, -1);
				lua_pop(L, 1);
				lua_pushinteger(L, 23);
				lua_gettable(L, -2);
				npc->m_iMagicHitRatio = (uint32_t)lua_tointeger(L, -1);
				lua_pop(L, 1);
				lua_pushinteger(L, 24);
				lua_gettable(L, -2);
				npc->attackRange = (uint32_t)lua_tointeger(L, -1);
				lua_pop(L, 1);
				lua_pushinteger(L, 25);
				lua_gettable(L, -2);
				npc->goldDropValue = (uint64_t)lua_tointeger(L, -1) * GOLDDROPMULTIPLIER;
				lua_pop(L, 1);

				if (!m_npcConfigList[npc->Type()])
				{
					m_npcConfigList[npc->Type()] = npc;
				}
				lua_pop(L, 1);
			}
		}
		lua_pop(L, 1);



		consoleLogger->information("Loading Items.");
		if (luaL_dofile(L, "item.lua") != 0)
		{
			consoleLogger->fatal(Poco::format("%s", (string)lua_tostring(L,-1)));
			return false;
		}
		lua_getglobal(L, "items");

		if(lua_istable(L,-1))
		{
			lua_pushnil(L);
			while (lua_next(L, -2))
			{
				uint8_t tableSize = uint8_t(lua_rawlen(L, -1));
				Item * item = new Item();

				lua_pushinteger(L, 1);
				lua_gettable(L, -2);
				item->m_sIDnum = (int64_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 2);
				lua_gettable(L, -2);
				item->m_cName = lua_tostring(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 3);
				lua_gettable(L, -2);
				item->m_cItemType = (int8_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 4);
				lua_gettable(L, -2);
				item->m_cEquipPos = (int8_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 5);
				lua_gettable(L, -2);
				item->m_sItemEffectType = (int16_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 6);
				lua_gettable(L, -2);
				item->m_sItemEffectValue1 = (int16_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 7);
				lua_gettable(L, -2);
				item->m_sItemEffectValue2 = (int16_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 8);
				lua_gettable(L, -2);
				item->m_sItemEffectValue3 = (int16_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 9);
				lua_gettable(L, -2);
				item->m_sItemEffectValue4 = (int16_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 10);
				lua_gettable(L, -2);
				item->m_sItemEffectValue5 = (int16_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 11);
				lua_gettable(L, -2);
				item->m_sItemEffectValue6 = (int16_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 12);
				lua_gettable(L, -2);
				item->m_wMaxLifeSpan = (int16_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 13);
				lua_gettable(L, -2);
				item->m_sSpecialEffect = (int16_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 14);
				lua_gettable(L, -2);
				item->m_sSprite = (int16_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 15);
				lua_gettable(L, -2);
				item->m_sSpriteFrame = (int16_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 16);
				lua_gettable(L, -2);
				item->m_wPrice = (int32_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 17);
				lua_gettable(L, -2);
				item->m_wWeight = (uint8_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 18);
				lua_gettable(L, -2);
				item->m_cApprValue = (int8_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 19);
				lua_gettable(L, -2);
				item->m_cSpeed = (int8_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 20);
				lua_gettable(L, -2);
				item->m_sLevelLimit = (int16_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 21);
				lua_gettable(L, -2);
				item->m_cGenderLimit = (int8_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 22);
				lua_gettable(L, -2);
				item->m_sSpecialEffectValue1 = (int16_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 23);
				lua_gettable(L, -2);
				item->m_sSpecialEffectValue2 = (int16_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 24);
				lua_gettable(L, -2);
				item->m_sRelatedSkill = (int16_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 25);
				lua_gettable(L, -2);
				item->m_cCategory = (int8_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 26);
				lua_gettable(L, -2);
				item->m_ItemColor = (uint32_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				if (!m_pItemConfigList[item->m_sIDnum])
				{
					m_pItemConfigList[item->m_sIDnum] = item;
				}
				lua_pop(L, 1);
			}
		}
		lua_pop(L, 1);


		consoleLogger->information("Loading Magic.");
		if (luaL_dofile(L, "magic.lua") != 0)
		{
			consoleLogger->fatal(Poco::format("%s", (string)lua_tostring(L, -1)));
			return false;
		}
		lua_getglobal(L, "magic");

		if (lua_istable(L, -1))
		{
			lua_pushnil(L);
			while (lua_next(L, -2))
			{
				uint8_t tableSize = uint8_t(lua_rawlen(L, -1));
				Magic * magic = new Magic();

				lua_pushinteger(L, 1);
				lua_gettable(L, -2);
				magic->num = (uint16_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 2);
				lua_gettable(L, -2);
				magic->m_cName = lua_tostring(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 3);
				lua_gettable(L, -2);
				magic->m_sType = (uint16_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 4);
				lua_gettable(L, -2);
				magic->m_dwDelayTime = (uint64_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 5);
				lua_gettable(L, -2);
				magic->m_dwLastTime = (uint64_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 6);
				lua_gettable(L, -2);
				magic->m_manaCost = (uint16_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 7);
				lua_gettable(L, -2);
				magic->m_hRange = (uint8_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 8);
				lua_gettable(L, -2);
				magic->m_vRange = (uint8_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 9);
				lua_gettable(L, -2);
				magic->m_sValue[0] = (uint16_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 10);
				lua_gettable(L, -2);
				magic->m_sValue[1] = (uint16_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 11);
				lua_gettable(L, -2);
				magic->m_sValue[2] = (uint16_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 12);
				lua_gettable(L, -2);
				magic->m_sValue[3] = (uint16_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 13);
				lua_gettable(L, -2);
				magic->m_sValue[4] = (uint16_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 14);
				lua_gettable(L, -2);
				magic->m_sValue[5] = (uint16_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 15);
				lua_gettable(L, -2);
				magic->m_sValue[6] = (uint16_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 16);
				lua_gettable(L, -2);
				magic->m_sValue[7] = (uint16_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 17);
				lua_gettable(L, -2);
				magic->m_sValue[8] = (uint16_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 18);
				lua_gettable(L, -2);
				magic->m_sIntLimit = (uint16_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 19);
				lua_gettable(L, -2);
				magic->m_iGoldCost = (int32_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 20);
				lua_gettable(L, -2);
				magic->m_cCategory = (uint8_t)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 21);
				lua_gettable(L, -2);
				magic->m_element = (Element)lua_tointeger(L, -1);
				lua_pop(L, 1);

				if (!m_pMagicConfigList[magic->num])
				{
					m_pMagicConfigList[magic->num] = magic;
				}
				lua_pop(L, 1);
			}
		}
		lua_pop(L, 1);

		
		consoleLogger->information("Loading Drop Rate JSON.");
		Poco::JSON::Parser * parser = new Poco::JSON::Parser();
		std::ifstream dropJson("droplist.json");
		Poco::Dynamic::Var parsed = parser->parse(dropJson);
		Poco::Dynamic::Var parsedResult = parser->result();

		Array::Ptr npcList = parsedResult.extract<Array::Ptr>();
		int npcCount = npcList->size();
		if (npcList && npcCount) {
			for (int ni = 0; ni < npcCount; ni++) {
				Object::Ptr npcRow = npcList->getObject(ni);
				if (npcRow) {
					Npc * npc = 0;
					string npcName = "";
					int npcID = 0;
					if (npcRow->has("id")) {
						Poco::Dynamic::Var npcIDVar = npcRow->get("id");
						npcIDVar.convert(npcID);
					}
					if (npcRow->has("_name")) {
						Poco::Dynamic::Var npcNameVar = npcRow->get("_name");
						npcName = npcNameVar.convert<std::string>();
					}
					if (npcRow->has("items")) {
						Array::Ptr npcItems = npcRow->getArray("items");
						if (npcID != 0) {
							npc = m_npcConfigList[npcID];
						}
						else if (npcName != "") {
							npc = GetNpcByName(npcName);
						}
						if (npc != 0) {
							logger->information("Processing items for NPC: %?d", npc->Type());
							for (int ii = 0; ii < npcItems->size(); ii++) {
								Object::Ptr itemRow = npcItems->getObject(ii);
								if (itemRow) {
									Item * item = 0;
									int itemProb = 0;
									int itemID = 0;
									string itemName = "";
									if (itemRow->has("id")) {
										Poco::Dynamic::Var itemIDVar = itemRow->get("id");
										itemID = itemIDVar.convert<int>();
									}
									if (itemRow->has("_name")) {
										Poco::Dynamic::Var itemNameVar = itemRow->get("_name");
										itemNameVar.convert<std::string>(itemName);
									}
									if (itemRow->has("prob")) {
										Poco::Dynamic::Var itemProbVar = itemRow->get("prob");
										itemProbVar.convert<int>(itemProb);
									}
									if (itemProb == 0) {
										itemProb = 1;
									}
									if (itemID != 0) {
										item = m_pItemConfigList[itemID];
									}
									else if(itemName != "") {
										item = GetItemByName(itemName);
									}
									if (item != 0) {
										logger->information("Setting drop data; NPC: %?d, Item: %?d, Probability: %?d", npc->Type(), item->m_sIDnum, itemProb);
										m_npcDropData[npc->Type()][item->m_sIDnum] = itemProb;
									}
								}
							}
						}
					}
				}
			}
			logger->information("Done");
		}		
	}
	catch (std::exception& e)
	{
		consoleLogger->fatal(Poco::format("Init() Exception: %s", (string)e.what()));
		system("pause");
		return false;
	}
	catch(...)
	{
		consoleLogger->fatal("Unspecified Init() Exception.");
		system("pause");
		return false;
	}
	ConnectSQL();



	for (int i = 1; i < 300; i++)
		m_iLevelExpTable[i] = iGetLevelExp(i);

	m_iLimitedUserExp = m_iLevelExpTable[LEVELLIMIT+1]; 
	m_iLevelExp51     = m_iLevelExpTable[51]; 


	return true;
}

Npc * GServer::GetNpcByName(string name)
{
	for (int x = 0; x < MAXNPCTYPES; x++)
	{
		if (m_npcConfigList[x]) {
			Npc * npc = m_npcConfigList[x];
			if (npc->name == name) {
				return npc;
			}
		}
	}
	return 0;
}

Item * GServer::GetItemByName(string name)
{
	if (name == "nothing") return 0;
	for (int x = 0; x < MAXITEMTYPES; x++)
	{
		Item * item = m_pItemConfigList[x];
		if (item && item->m_cName == name) {
			return item;
		}
	}
	logger->information("Could not find item %s", name);
	return 0;
}

uint64_t GServer::iGetLevelExp(uint16_t iLevel)
{
	uint64_t iRet;

	if (iLevel == 0) return 0;
	iRet = iGetLevelExp(iLevel - 1) + iLevel * ( 50 + (iLevel * (iLevel / 17) * (iLevel / 17) ) );

	return iRet;
}

void GServer::ChatThread()
{
#ifndef WIN32
	struct timespec req={0};
	req.tv_sec = 0;
	req.tv_nsec = 1000000L;//1ms
#else
	_tzset();
#endif
	while (serverstatus == SERVERSTATUS_ONLINE)
	{
		if (chatpipe.size() > 0)
		{
			//stuff to do!
			mutchat.lock();
			shared_ptr<MsgQueueEntry> msg = chatpipe.front();
			chatpipe.pop_front();
			mutchat.unlock();

			StreamRead sr(msg->data, msg->size);

			uint32_t msgid = sr.ReadInt();

			if (msgid == MSGID_COMMAND_CHECKCONNECTION)
			{//connection timeout check in chat to ease packet load from actionthread
				//TODO: add check for this packet not being received as often as it should. to speed hack freely you could remove this packet from being sent entirely
				//TODO: fix client side times
				//TODO: cheaters shouldn't be "instantly banned to bi", but instead maybe tile locked unable to react for a few seconds
// 				uint64_t timereceived, timenow, timegapclient, timegapserver;
// 				timenow = unixtime();
// 				timereceived = sr.ReadInt();
// 
// 				if (msg->client->m_dwInitCCTimeRcv == NULL) {
// 					msg->client->m_dwInitCCTimeRcv = timereceived;
// 					msg->client->m_dwInitCCTime = timenow;
// 				}
// 				else {
// 					timegapclient = (timereceived - msg->client->m_dwInitCCTimeRcv);
// 					timegapserver = (timenow - msg->client->m_dwInitCCTime);
// 
// 					if (timegapclient < timegapserver) return;
// 
// 					if(msg->client->m_timeHackTurn == 0)
// 					{
// 						msg->client->m_timeHack = timegapclient - timegapserver;
// 						msg->client->m_timeHackDif = 0;
// 					}else{
// 						msg->client->m_timeHackDif += (timegapclient - timegapserver) - msg->client->m_timeHack;
// 					}
// 
// 					if(msg->client->m_timeHackTurn == 3 &&
// 						msg->client->m_timeHackDif > 1400*4)
// 					{
// 						string logmsg = Poco::format("(!) Client(%s) Timeout/Hack(1400+) %?d Time gap %?d", msg->client->m_cCharName, 
// 							msg->client->m_timeHackDif/4,   timegapclient - timegapserver);
// 						poco_error(*logger, logmsg);
// 						poco_error(*fileLogger, logmsg);
// 
// 						if(msg->client->m_timeHackDif > 2500*4)
// 						{
// 							string logmsg = Poco::format("(!) Client(%s) Speed hacker timeout(3000+) %?d Time gap %?d. Banned to BI", msg->client->m_cCharName, 
// 								msg->client->m_timeHackDif/4,   timegapclient - timegapserver);
// 							poco_error(*logger, logmsg);
// 							poco_error(*fileLogger, logmsg);
// 							//DeleteClient(iClientH, true, true, true, true); // "official" code. why delete when you can just teleport them instantly?
// 							//perhaps delete if you want them to suffer the consequences of speed hacking as they stay in game for up to 10 seconds
// 							//on disconnect. that might be the only reason
// 							//TODO: add teleport or delete code later - for now, delete
// 							gate->mutclientlist.lock_shared();
// 							DeleteClient(msg->client, true, false);
// 							gate->mutclientlist.unlock_shared();
// 							return;
// 						}
// 					}
// 
// 					++msg->client->m_timeHackTurn %= 4;
// 				}

			}
			else if (msgid == MSGID_COMMAND_CHATMSG)
			{
				if (msg->size > 83+30)//messages can't be longer. allow longer messages?
					continue;
				if (msg->client->m_iTimeLeft_ShutUp > 0)
					continue;
				if (msg->client->map->flags.chatDisabled && !msg->client->IsGM())
					continue;
				// 			if (m_pMapList[player->m_cMapIndex] != NULL) {
				// 				m_pMapList[player->m_cMapIndex]->IncPlayerActivity(player);
				// 			}
				//activity counts per map (mob spawning and crusade meteor landing i believe?)


				try
				{
					//sr.ReadInt();
					sr.ReadShort();
					sr.ReadShort();//X
					sr.ReadShort();//Y
					string charname = sr.ReadString(10);
					uint8_t iV1 = sr.ReadByte();
					string message = sr.ReadString(msg->size-21);
					int8_t msgtype = 0;

					Client * client = msg->client.get();

					ParseChat(client, message);
				}
				catch (int test)
				{
					//-193 data length error
					consoleLogger->error(Poco::format("Error: %?d", test));
				}
			}
		}
#ifdef WIN32
		Sleep(1);
#else
		nanosleep(&req,0);
#endif
	}
	logger->information("ChatThread() complete.");
#ifndef WIN32
	mysql_thread_end();
#endif
}

void GServer::TimerThread()
{
#ifndef WIN32
	struct timespec req = { 0 };
	req.tv_sec = 0;
	req.tv_nsec = 1000000L;//1ms
#else
	_tzset();
#endif

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

	while (serverstatus == SERVERSTATUS_ONLINE)
	{
		try
		{
			ltime = ltime = unixtime();

			if (t100msectimer < ltime)
			{
				//run delay events
				DelayEventProcessor();
				t100msectimer += 100;
			}
			if (t1sectimer < ltime)
			{
				t1sectimer += 1000;
			}
			if (t5sectimer < ltime)
			{
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
			consoleLogger->critical(Poco::format("TimerThread() SQL Exception: %s", e->displayText()));
		}
		catch (...)
		{
			consoleLogger->fatal("Unspecified TimerThread() Exception.");
			return;
		}
	}
	logger->information("TimerThread() complete.");

#ifndef WIN32
	mysql_thread_end();
#endif
}

void GServer::ParseChat(Client * client, string message)
{
	if (client->m_iAdminUserLevel > 0)
	{
		//TODO: log gm chat
	}

	char sendmode = 0;

	if (message.length() > 0)
	{
		switch (message[0])
		{
			case '$':
				message[0] = ' ';
				sendmode = CHAT_PARTY;
				break;
			case '@':
			case '^':
				message[0] = ' ';
				if (!client->guild)
					break;
				sendmode = CHAT_GUILD;
				break;
			case '!':
				message[0] = ' ';
				if (/*client->m_iLevel <= 10 ||*/ client->IsDead())
					break;
				sendmode = CHAT_SHOUT;
				break;
			case '%':
				message[0] = ' ';
				if (!client->IsGM())
					break;
				sendmode = CHAT_GM;
				break;
			case '~':
				message[0] = ' ';
				if (client->IsDead())
					break;
				sendmode = CHAT_NATIONSHOUT;
				break;
			case '/':
				//TODO: create functions for all the individual commands like classic hb? could even Lua-ize this

				//elegant solution
				std::vector<string> tokens;
				boost::split(tokens, message, boost::is_any_of(" "));


				if (tokens[0] == "/tp")
				{
					if (tokens.size() < 2)
					{
						SendNotifyMsg(0, client, NOTIFY_NOTICEMSG, 0, 0, 0, "Invalid command parameters");
						return;
					}
					string mapname = tokens[1];
					int16_t x = -1, y = -1;

					if (tokens.size() > 3)
					{
						x = atoi(tokens[2].c_str());
						y = atoi(tokens[3].c_str());
					}

					Map * pmap = GetMap(mapname);
					if (!pmap)
					{
						SendNotifyMsg(0, client, NOTIFY_NOTICEMSG, 0, 0, 0, "Map does not exist");
						return;
					}
					RequestTeleportHandler(client, 2, mapname, x, y);
					return;
				}
				else if (tokens[0] == "/createitem")
				{
					if (tokens.size() < 2)
					{
						SendNotifyMsg(0, client, NOTIFY_NOTICEMSG, 0, 0, 0, "Invalid syntax");
						return;
					}
					Item * pItem = new Item;
					if (pItem->InitItemAttr(tokens[1], m_pItemConfigList) == false)
					{
						delete pItem;
						SendNotifyMsg(0, client, NOTIFY_NOTICEMSG, 0, 0, 0, "Invalid item name");
						return;
					}
					if (tokens.size() > 2)
					{
						pItem->m_ItemColor = _atoi64(tokens[2].c_str());
					}

					int iEraseReq;
					if (_bAddClientItemList(client->self.lock(), pItem, &iEraseReq) == true)
					{

						SendItemNotifyMsg(client->self.lock(), NOTIFY_ITEMOBTAINED, pItem, 0);

						if (iEraseReq == 1)
						{
							delete pItem;
							pItem = nullptr;
						}

						logger->information(Poco::format("GM Order(%s): Create ItemName(%s)", client->name, tokens[1]));
						return;
					}
					else
					{
						delete pItem;
						return;
					}
				}
				else if (tokens[0] == "/summon")
				{
					uint16_t pX, pY;
					pX = client->x;
					pY = client->y;
					char cWaypoint;
					if (tokens.size() < 2)
					{
						SendNotifyMsg(0, client, NOTIFY_NOTICEMSG, 0, 0, 0, "Invalid summon");
						return;
					}
					int16_t mobnum = 1;
					if (tokens.size() > 2)
					{
						mobnum = atoi(tokens[2].c_str());
						if (mobnum < 1) mobnum = 1;
						if (mobnum > 50) mobnum = 50;
					}
					shared_ptr<Npc> master = client->map->CreateNpc(tokens[1], /*cSA*/0, MOVETYPE_RANDOM,
											 &pX, &pY, (Side)-1, &cWaypoint, 0, 0, false, /*bSummoned*/false, /*isBerserked*/false, true);
					if (!master)
					{
						return;
					}
					for (int j = 0; j < (mobnum - 1); j++)
					{
						shared_ptr<Npc> slave = client->map->CreateNpc(tokens[1], /*cSA*/0, MOVETYPE_RANDOM,
												&pX, &pY, (Side)-1, &cWaypoint, 0, 0, false, /*bSummoned*/false, /*isBerserked*/false, true);

						if (!slave)
							break;
						
						slave->Follow(master);
					}
					return;
				}

				break;
				if (message == "/to")
				{
					//TODO: tokenize to get client name and such
					client->whisperTarget = this->clientlist.back();
					SendNotifyMsg(0, client, NOTIFY_WHISPERMODEON, 0, 0, 0, client->whisperTarget.lock()->name);
					return;
				}
				break;
		}

		if (sendmode == CHAT_PARTY && !client->GetParty())
		{
			SendNotifyMsg(0, client, NOTIFY_NOTICEMSG, 0, 0, 0, "You are not in a party");
			return;
		}

		if (sendmode == 0 && !client->whisperTarget.expired())
		{
			sendmode = CHAT_WHISPER;

			if (message[0] == '#') sendmode = CHAT_NORMAL;

			if (client->m_iTimeLeft_ShutUp > 0) sendmode = CHAT_NORMAL;
		}

		if ((client->magicEffectStatus[MAGICTYPE_CONFUSE] == 1) && (dice(1, 2) != 2))
		{
			//mess up message
			string temp;
			temp.resize(message.size() + 2);
			transform(message.begin(), message.end(), temp.begin(), [&](char letter)
			{
				if (dice(1, 3) == 2)
					return char(letter + 5);
				return letter;
			});
			message = temp;
		}

		StreamWrite sw;
		sw.WriteInt(MSGID_COMMAND_CHATMSG);
		sw.WriteShort(uint16_t(client->handle));
		sw.WriteShort(client->x);
		sw.WriteShort(client->y);
		sw.WriteString(client->name, 10);
		sw.WriteByte(sendmode);
		sw.WriteString(message, message.length() + 1);

		if (sendmode == CHAT_GUILD)
		{
			if (!client->guild)
				return;
			//client->m_guild->Broadcast();//TODO: guild message
		}
		else if (sendmode != CHAT_WHISPER)
		{
			gate->mutclientlist.lock_shared();
			for (shared_ptr<Client> target : client->gserver->clientlist)
			{
				if (target->socket && (target->disconnecttime == 0))
				{
					if (!target->m_bIsInitComplete)
						continue;

					bool bsend = true;
					if (m_bIsCrusadeMode)
					{
						if ((!client->IsNeutral()) && (!target->IsNeutral()) && (client->side != target->side))
							bsend = false;
					}

					switch (sendmode)
					{
						//TODO: change how chat works? should make a chat mode for:
						// mapwide shout, mapwide town, global town - and make them "channels" that can be
						// /join'd so players can choose to participate in them (or let them be on by default
						// but able to be toggled)
						case CHAT_NORMAL:
							if (bsend && (target->map == client->map)
								&& (target->x > client->x - 13) && (target->x < client->x + 13)
								&& (target->y > client->y - 10) && (target->y < client->y + 10))
							{
								target->mutsocket.lock();
								if (target->socket)
									target->socket->write(sw.data, sw.size);
								target->mutsocket.unlock();
							}
							break;
						case CHAT_SHOUT://both town shout chat - local to map only
							if (bsend && (target->map == client->map))
							{
								target->mutsocket.lock();
								if (target->socket)
									target->socket->write(sw.data, sw.size);
								target->mutsocket.unlock();
							}
							break;
						case CHAT_GM://GM chat, sent global and always overrides
						{
							target->mutsocket.lock();
							if (target->socket)
								target->socket->write(sw.data, sw.size);
							target->mutsocket.unlock();
						}
							break;
						case CHAT_NATIONSHOUT://your town only shout chat - local to map only
							if (bsend && (target->side == client->side) && (target->map == client->map))
							{
								target->mutsocket.lock();
								if (target->socket)
									target->socket->write(sw.data, sw.size);
								target->mutsocket.unlock();
							}
							break;
						case CHAT_PARTY:
							if (target->GetParty() && client->GetParty() == target->GetParty())
								break;
					}
				}
			}
			gate->mutclientlist.unlock_shared();
		}
		else//CHAT_WHISPER
		{
			gate->mutclientlist.lock_shared();//have to make sure a client isn't deleted when dereferencing
			if (client->whisperTarget.expired())
			{
				SendNotifyMsg(0, client, NOTIFY_NOTICEMSG, 0, 0, 0, "Player is not online");
				return;
			}
			shared_ptr<Client> whisperTarget = client->whisperTarget.lock();
			whisperTarget->mutsocket.lock();
			if (whisperTarget->socket)
				whisperTarget->socket->write(sw.data, sw.size);
			whisperTarget->mutsocket.unlock();

			client->mutsocket.lock();
			if (client->socket)
				client->socket->write(sw.data, sw.size);
			client->mutsocket.unlock();

			gate->mutclientlist.unlock_shared();
		}
	}
}

void GServer::RequestTeleportHandler(Client * client, char teleportType, string cMapName, int dX, int dY)
{
	uint16_t sX, sY;
	bool bRet, bIsLockedMapNotify;
	bool setRecallTime = true;
	string DestMapName, TempMapName;
	uint16_t destx, desty;
	uint8_t direction, iMapSide;
	Client * iExH;
	Map * pmap;

	if (!client) return;
	if (!client->m_bIsInitComplete) return;
	if (client->_dead) return;
	if (client->m_bIsOnWaitingProcess) return;

	if ((teleportType == 1 || teleportType == 3) && (client->IsInFoeMap() || (client->map->flags.apocalypseMap && client->map->dynamicGateType != 4)))
	{
		client->Notify(0, NOTIFY_CANNOTRECALL, 1);
		return;
	}

	sX = client->x;
	sY = client->y;

	bRet = client->map->bSearchTeleportDest(sX, sY, DestMapName, &destx, &desty, &direction);

	if (!bRet && teleportType == 4)
	{
		SendObjectMotionRejectMsg(client);
		SendNotifyMsg(0, client, NOTIFY_TELEPORT_REJECTED,sX,sY,0);
		return;
	}

	bIsLockedMapNotify = false;

	if (client->exchangeMode)
	{
		iExH = client->exchangePlayer;
		//_ClearExchangeStatus(iExH)
		//_ClearExchangeStatus(client);
	}
	//Since we moved this to the NPC class... Maybe move it back to gserver :/
	//RemoveFromTarget(client);

	client->map->ClearOwner(sX, sY);

	SendEventToNearClient_TypeA(client, MSGID_MOTION_EVENT_REJECT, 0, 0, 0);

	iMapSide = iGetMapLocationSide(DestMapName);
	if ((client->lockedMapName == "NONE") && (client->lockedMapTime > 0))
	{
		uint32_t tmp_mapSide = iMapSide;
		if (tmp_mapSide >= 11) tmp_mapSide -= 10;

		if (tmp_mapSide == 0 || client->side != tmp_mapSide)
		{
			destx = -1;
			desty = -1;
			bIsLockedMapNotify = true;
			DestMapName = client->lockedMapName;
		}
	}


	if (client->map->flags.permIllusionOn)
	{
		client->RemoveMagicEffect(MAGICTYPE_CONFUSE);
	}

	pmap = GetMap(DestMapName);
	
	if(bRet)
	{
		if(pmap == 0 || (bRet && iMapSide && iMapSide <= 10 && (client->map->factionName == client->faction) != 0 && !client->IsGM() && !client->IsNeutral() ))
		{
			DestMapName = client->mapName;
			destx = client->x;
			desty = client->y;
			setRecallTime = false;
		}
	}

	if (bRet && cMapName.length() == 0)
	{
		for (std::vector<Map*>::iterator iter = maplist.begin(); iter != maplist.end(); ++iter)
		{
			if ((*iter)->name == DestMapName, 10)
			{
				client->x   = destx;
				client->y   = desty;
				client->direction = direction;
				client->map = (*iter);
				client->mapName = (*iter)->name;
				PlayerMapEntry(client->self.lock(), setRecallTime);
				if (bIsLockedMapNotify == true) SendNotifyMsg(0, client, NOTIFY_LOCKEDMAP, client->lockedMapTime, 0, 0, client->lockedMapName);
				return;
			}
		}
		//used to server change here
		return;
	}
	else {
		switch (teleportType)
		{
		case 0: // Forced Recall. 
		case 1: // Normal Recall 
			//if (memcmp(m_pMapList[ player->m_cMapIndex ]->m_cName, "resurr", 6) == 0) return;
			if (client->level > 80)
				TempMapName, sideMap[ client->side ];
			else
				TempMapName = sideMapFarm[ client->side ];

			if ((client->lockedMapName == "NONE") && (client->lockedMapTime > 0))
			{
				bIsLockedMapNotify = true;
				TempMapName = client->lockedMapName;
			}
			
			for (std::vector<Map*>::iterator iter = maplist.begin(); iter != maplist.end(); ++iter)
			{
				if ((*iter)->name == TempMapName, 10)
				{
					(*iter)->GetInitialPoint(&client->x, &client->y, client->faction);
					client->map = (*iter);
					client->mapName = (*iter)->name;
					PlayerMapEntry(client->self.lock(), setRecallTime);
					if (bIsLockedMapNotify == true) SendNotifyMsg(0, client, NOTIFY_LOCKEDMAP, client->lockedMapTime, 0, 0, client->lockedMapName);
					return;
				}
			}
			//used to server change here
			return;

		case 2: // Forced teleport  
		case 3: // Player requested teleport 
			if ((client->lockedMapName == "NONE") && (client->lockedMapTime > 0) && (cMapName == "resurr"))
			{
				dX = -1;
				dY = -1;
				bIsLockedMapNotify = true;
				TempMapName = client->lockedMapName;
			}
			else {
				TempMapName = cMapName;
			}

			pmap = GetMap(TempMapName);
			if (pmap == 0)
			{
				//all else has failed. no map can be found to send the player to - would normally be a server transfer
				//with the login server either giving them the hg with the map, or saying that the game server is not online
				//for this failsafe, just send them to their respective town
				
				//no error checks because this should never ever trigger
				if (client->faction.substr(0, 3) == "are")
				{
					pmap = GetMap("aresden");
				}
				else if (client->faction.substr(0, 3) == "elv")
				{
					pmap = GetMap("elvine");
				}
				pmap->GetInitialPoint(&client->x, &client->y, client->faction);
				client->map = pmap;
				client->mapName = "aresden";

				client->x   = dX; //-1;	  			
				client->y   = dY; //-1;	  

				PlayerMapEntry(client->self.lock(), setRecallTime);
				if (bIsLockedMapNotify == true) SendNotifyMsg(0, client, NOTIFY_LOCKEDMAP, client->lockedMapTime, 0, 0, client->lockedMapName);
				return;
			}

			client->map = pmap;

			if (dX == -1 || dY == -1)
				pmap->GetInitialPoint(&client->x, &client->y, client->faction);
			else 
			{
				client->x   = dX;
				client->y   = dY;
			}

			client->mapName = pmap->name;
			break;
		}
	}
	PlayerMapEntry(client->self.lock(), setRecallTime);
	if (bIsLockedMapNotify == true) SendNotifyMsg(0, client, NOTIFY_LOCKEDMAP, client->lockedMapTime, 0, 0, client->lockedMapName);
}

void GServer::PlayerMapEntry(shared_ptr<Client> client, bool setRecallTime)
{
	if (!client) return;

	iSetSide(client.get());

	StreamWrite sw;

	sw.WriteInt(MSGID_RESPONSE_INITDATA);
	sw.WriteShort(MSGTYPE_CONFIRM);

	if (client->observerMode == false)
		client->map->bGetEmptyPosition(client->x, client->y, client);
	else client->map->GetInitialPoint(&client->x, &client->y, client->faction);

	sw.WriteShort(uint16_t(client->handle));
	sw.WriteShort(client->x - 14 - 5);
	sw.WriteShort(client->y - 12 - 5);

	sw.WriteShort(client->Type());

	sw.WriteShort(client->m_sAppr1);
	sw.WriteShort(client->m_sAppr2);
	sw.WriteShort(client->m_sAppr3);
	sw.WriteShort(client->m_sAppr4);
	sw.WriteInt(client->m_iApprColor);
	sw.WriteShort(client->m_sHeadApprValue);
	sw.WriteShort(client->m_sBodyApprValue);
	sw.WriteShort(client->m_sArmApprValue);
	sw.WriteShort(client->m_sLegApprValue);

	if (client->playerKillCount > 0)
		client->SetStatusFlag(STATUS_PK, true);

	sw.WriteInt(client->status);

	sw.WriteString(client->mapName, 10);
	sw.WriteString(client->map->factionName, 10);

	if (client->map->fixedDay == true) 
		sw.WriteByte(1);
	else
		sw.WriteByte(m_cDayOrNight);

	if (client->map->fixedDay) 
		sw.WriteByte(WEATHER_SUNNY);
	else 
		sw.WriteByte(client->map->weather);

	sw.WriteInt(client->contribution);

	if(!client->observerMode && !client->IsEthereal())
	{
		client->map->SetOwner(client, client->x, client->y);
	}

	sw.WriteByte((client->observerMode)?1:0);
	sw.WriteInt(client->m_reputation);
	sw.WriteInt(uint32_t(client->health));
	sw.WriteInt(client->m_iLucky);
	if (client->m_bIsOnShop && m_iCrusadeWinnerSide == client->side)//crusade shop price change
		sw.WriteByte(-10);
	else
		sw.WriteByte(0);

	uint64_t oldpos = sw.position;
	uint16_t total = 0;

	sw.WriteShort(0);//total placeholder

	Tile * pTileSrc = client->map->GetTile(client->x - 12, client->y - 9);
	//Tile * pTileSrc = (Tile *)(client->pMap->_tile + (client->m_sX - 12) + (client->m_sY - 9)*client->pMap->sizeY);

	for (int y = 0; y < 19; ++y)
	{
		for (int x = 0; x < 25; ++x)
		{
			if (WriteTileData(sw, client.get(), pTileSrc, x, y))
				total++;
		}
	}
	char * temp = sw.data + oldpos;
	*((short *)temp) = total;

	client->mutsocket.lock();//TODO: put (un)locks in socket->write - also put the client->socket sanity check in a client function
	if (client->socket)
		client->socket->write(sw.data, sw.position);
	client->mutsocket.unlock();

//	SendEventToNearClient_TypeA(client, MSGID_MOTION_EVENT_CONFIRM, NULL, NULL, NULL);

// 	if(m_astoria.get() && strcmp(player->m_cMapName, "astoria") == 0)
// 	{
// 		const Casualties * stats = m_astoria->GetStats();
// 		player->Notify(NULL, NOTIFY_CASUALTIES, 
// 			stats[ARESDEN].deaths, stats[ELVINE].deaths,NULL,/*, stats[ISTRIA].deaths,*/ NULL, // Commented out 3rd faction xRisenx
// 			stats[ARESDEN].kills,stats[ELVINE].kills, NULL/*, stats[ISTRIA].kills*/); // Commented out 3rd faction xRisenx
// 
// 		switch(m_astoria->GetEventType())
// 		{
// 		case ET_CAPTURE:
// 			UpdateRelicPos(iClientH);
// 			break;
// 
// 		case ET_DESTROY_SHIELD:
// 			player->Notify(NULL, NOTIFY_SHIELDHP, ARESDEN, m_astoria->GetShieldHP(ARESDEN));
// 			player->Notify(NULL, NOTIFY_SHIELDHP, ELVINE, m_astoria->GetShieldHP(ELVINE));
// 			break;
// 		}
// 	}

// 	if (m_bHeldenianMode)
// 		UpdateHeldenianStatus();

	SendEventToNearClient_TypeA(client.get(), MSGID_MOTION_EVENT_CONFIRM, 0, 0, 0);

/*
	if(setRecallTime)
	{
		player->m_bIsWarLocation = false;
		player->m_iTimeLeft_ForceRecall = 0;

		if(player->IsNeutral())
		{
			if(m_bIsCrusadeMode && !player->IsGM() && 
				((strcmp(m_pMapList[player->m_cMapIndex]->m_cName, sideMap[ARESDEN]) == 0) || 
				 (strcmp(m_pMapList[player->m_cMapIndex]->m_cName, sideMap[ELVINE]) == 0)  / *|| 
				 (strcmp(m_pMapList[player->m_cMapIndex]->m_cName, sideMap[ISTRIA]) == 0)* /))  // Commented out 3rd faction xRisenx
			{
				player->m_dwWarBeginTime = unixtime();
				player->m_bIsWarLocation = true;
				player->m_iTimeLeft_ForceRecall = 1; 
			}
		}
		else if (player->IsInFoeMap() && !player->IsGM()) 
		{
			player->m_dwWarBeginTime = unixtime();
			player->m_bIsWarLocation = true;

			SetForceRecallTime(iClientH) ;
		}

		if (player->m_iTimeLeft_ForceRecall > 0) {
			SendNotifyMsg(NULL, iClientH, NOTIFY_FORCERECALLTIME, player->m_iTimeLeft_ForceRecall, NULL, NULL, NULL);
		}
	}

	iMapSide = iGetMapLocationSide(m_pMapList[player->m_cMapIndex]->m_cName);

	if (iMapSide >= 11) iTmpMapSide = iMapSide - 10 ;
	else iTmpMapSide = iMapSide ;

	player->m_bIsInBuilding = false ;

	if ((player->m_side != iTmpMapSide) && (iMapSide != 0) ) {
		if ( (iMapSide <= 10) && !player->IsGM() && !player->IsNeutral() )
		{
			player->m_dwWarBeginTime = unixtime();
			player->m_bIsWarLocation = true;
			player->m_iTimeLeft_ForceRecall = 1 ; 
			player->m_bIsInBuilding = true ;
		}
	}
	else if (player->IsInJail() && !player->IsGM()) {
		player->m_bIsWarLocation = true;
		player->m_dwWarBeginTime = unixtime();

		if (player->m_iTimeLeft_ForceRecall == 0) {
			player->m_iTimeLeft_ForceRecall = 20*5 ; 
		} else if (player->m_iTimeLeft_ForceRecall > 20*5) {
			player->m_iTimeLeft_ForceRecall = 20*5 ;
		}
	}

	_CheckQuestEnvironment(iClientH);

	if (m_astoria.get() && player->m_cMapIndex == m_iAstoriaMapIndex) {
		UpdateRelicPos(iClientH);
	}

	if (player->m_iSpecialAbilityTime == 0) {
		SendNotifyMsg(NULL, iClientH, NOTIFY_SPECIALABILITYENABLED, NULL, NULL, NULL, NULL);
	}

	if (m_bHeldenianMode)
	{
		SendNotifyMsg(NULL, iClientH, NOTIFY_HELDENIANSTART,  NULL , NULL, NULL, NULL);
	}

	if (m_bIsCrusadeMode == true) {
		if (player->m_dwCrusadeGUID == 0) {
			player->m_iCrusadeDuty = 0;
			player->m_iConstructionPoint = 0;
			player->m_dwCrusadeGUID = m_dwCrusadeGUID;
		}
		else if (player->m_dwCrusadeGUID != m_dwCrusadeGUID) {
			player->m_iCrusadeDuty       = 0;
			player->m_iConstructionPoint = 0;
			player->m_iWarContribution   = 0;
			player->m_dwCrusadeGUID = m_dwCrusadeGUID;
			SendNotifyMsg(NULL, iClientH, NOTIFY_CRUSADE, (uint32_t)m_bIsCrusadeMode, NULL, 0, NULL, -1);
		}
		SendNotifyMsg(NULL, iClientH, NOTIFY_CRUSADE, (uint32_t)m_bIsCrusadeMode, player->m_iCrusadeDuty, NULL, NULL);
	}
	else {
		if (player->m_dwCrusadeGUID == m_dwCrusadeGUID) {
			player->m_iCrusadeDuty = 0;
			player->m_iConstructionPoint = 0;
		}
		else if ((player->m_dwCrusadeGUID != NULL) && (player->m_dwCrusadeGUID != m_dwCrusadeGUID)) {
			SendNotifyMsg(NULL, iClientH, NOTIFY_CRUSADE, (uint32_t)m_bIsCrusadeMode, NULL, 0, NULL, -1);
			player->m_iWarContribution   = 0;
			player->m_dwCrusadeGUID = 0;
		}
	}

	if (m_pMapList[player->m_cMapIndex]->m_bIsFightZone) { //Send all map restrictions
		if (m_pMapList[player->m_cMapIndex]->m_isPartyDisabled && !player->IsGM() && player->GetParty()) 
			partyMgr.RemoveFromParty( player );
		if (m_pMapList[player->m_cMapIndex]->m_isShieldDisabled)
			SendNotifyMsg(NULL, iClientH, NOTIFY_EVENTSHIELD, true, NULL, NULL, NULL, NULL);
		if (m_pMapList[player->m_cMapIndex]->m_isArmorDisabled){
			if (!m_pClientList[iClientH]->IsGM() && !m_pClientList[iClientH]->IsDead()){
				if ( m_pClientList[iClientH]->m_sItemEquipmentStatus[ EQUIPPOS_HEAD ] != -1){
					SendNotifyMsg(NULL, iClientH, NOTIFY_ITEMRELEASED, EQUIPPOS_HEAD, m_pClientList[iClientH]->m_sItemEquipmentStatus[ EQUIPPOS_HEAD ], NULL, NULL);
					ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ EQUIPPOS_HEAD ], false);
				}
				if ( m_pClientList[iClientH]->m_sItemEquipmentStatus[ EQUIPPOS_BODY ] != -1) {
					SendNotifyMsg(NULL, iClientH, NOTIFY_ITEMRELEASED, EQUIPPOS_BODY, m_pClientList[iClientH]->m_sItemEquipmentStatus[ EQUIPPOS_BODY ], NULL, NULL);
					ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ EQUIPPOS_BODY ], false);
				}
				if ( m_pClientList[iClientH]->m_sItemEquipmentStatus[ EQUIPPOS_ARMS ] != -1) {
					SendNotifyMsg(NULL, iClientH, NOTIFY_ITEMRELEASED, EQUIPPOS_ARMS, m_pClientList[iClientH]->m_sItemEquipmentStatus[ EQUIPPOS_ARMS ], NULL, NULL);
					ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ EQUIPPOS_ARMS ], false);
				}
				if ( m_pClientList[iClientH]->m_sItemEquipmentStatus[ EQUIPPOS_BOOTS ] != -1) {
					SendNotifyMsg(NULL, iClientH, NOTIFY_ITEMRELEASED, EQUIPPOS_BOOTS, m_pClientList[iClientH]->m_sItemEquipmentStatus[ EQUIPPOS_BOOTS ], NULL, NULL);
					ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ EQUIPPOS_BOOTS ], false);
				}
				if ( m_pClientList[iClientH]->m_sItemEquipmentStatus[ EQUIPPOS_PANTS ] != -1) {
					SendNotifyMsg(NULL, iClientH, NOTIFY_ITEMRELEASED, EQUIPPOS_PANTS, m_pClientList[iClientH]->m_sItemEquipmentStatus[ EQUIPPOS_PANTS ], NULL, NULL);
					ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ EQUIPPOS_PANTS ], false);
				}
				SendEventToNearClient_TypeA(iClientH, OWNERTYPE_PLAYER, MSGID_MOTION_NULL, NULL, NULL, NULL);
			}

			SendNotifyMsg(NULL, iClientH, NOTIFY_EVENTARMOR, true, NULL, NULL, NULL, NULL);
		}
		if (m_pMapList[player->m_cMapIndex]->m_isPermIllusionOn){
			if (!player->IsGM()){
				for (i = 1; i < MAXCLIENTS; i++)
					if(m_pClientList[i] != NULL && m_pClientList[i]->IsGM() && m_pClientList[i]->m_cMapIndex == player->m_cMapIndex) break;

				if (i != MAXCLIENTS){
					SendNotifyMsg(NULL, iClientH, NOTIFY_MAGICEFFECTON, MAGICTYPE_CONFUSE, 3, i, NULL);
					player->m_cMagicEffectStatus[ MAGICTYPE_CONFUSE ] = 3;
					player->SetStatusFlag(STATUS_ILLUSION, true);
				}
			}
			SendNotifyMsg(NULL, iClientH, NOTIFY_EVENTILLUSION, true, NULL, NULL, NULL, NULL);
		}

		if (m_pMapList[player->m_cMapIndex]->m_isChatDisabled)
			SendNotifyMsg(NULL, iClientH, NOTIFY_EVENTCHAT, m_pMapList[player->m_cMapIndex]->m_isChatDisabled, NULL, NULL, NULL, NULL);

		for (i = 0; i < MAXMAGICTYPE; i++)
			if (m_pMapList[player->m_cMapIndex]->m_magicLimited[i])
				SendNotifyMsg(NULL, iClientH, NOTIFY_EVENTSPELL, true, i, NULL, NULL, NULL);

		poco_warning(*consoleLogger, Poco::format("Char(%s)-Enter(%s) Observer(%?d)", (string)player->m_cCharName, (string)player->m_cMapName, player->m_bIsObserverMode));
	}

	if (player->GetParty())
	{
		player->GetParty()->UpdateMemberMap(player);
	}

	if(player->m_guild)
		player->m_guild->MemberMapChanged(player);

	SendNotifyMsg(NULL, iClientH, NOTIFY_SAFEATTACKMODE, NULL, NULL, NULL, NULL);
	SendNotifyMsg(NULL, iClientH, NOTIFY_DOWNSKILLINDEXSET, player->m_iDownSkillIndex, NULL, NULL, NULL);
	SendNotifyMsg(NULL, iClientH, NOTIFY_ITEMPOSLIST, NULL, NULL, NULL, NULL);

	SendNotifyMsg(NULL, iClientH, NOTIFY_CONSTRUCTIONPOINT, player->m_iConstructionPoint, player->m_iWarContribution, 1, NULL);
	SendNotifyMsg(NULL, iClientH, NOTIFY_GIZONITEMUPGRADELEFT, player->m_iGizonItemUpgradeLeft, NULL, NULL, NULL);
	SendNotifyMsg(NULL, iClientH, NOTIFY_SUPERATTACKLEFT, NULL, NULL, NULL, NULL); // Added Show Criticals Left xRisenx
	Notify_ApocalypseGateState(iClientH);

	player->m_nextRecallPoint = 0;
	if (player->m_iQuest) {
		cQuestRemain = (m_pQuestConfigList[player->m_iQuest]->m_iMaxCount - player->m_iCurQuestCount);
		SendNotifyMsg(NULL, iClientH, NOTIFY_QUESTCOUNTER, cQuestRemain, NULL, NULL, NULL);
		_bCheckIsQuestCompleted(iClientH);
	}*/
}

int GServer::iGetMapLocationSide(string MapName)
{
	if(MapName.length() == 0) return 0;

	if(MapName[ MapName.length() - 2] == '_')
	{
		switch(MapName[ MapName.length() - 1])
		{
			case '1':
				return ARESDEN;
			case '2':
				return ELVINE;
		}
	}

	
	if (MapName == sideMap[ARESDEN]) return ARESDEN + 10 ;
	if (MapName == sideMap[ELVINE])  return ELVINE + 10 ;
	if (MapName == "arebrk11") return ARESDEN + 10 ;
	if (MapName == "elvbrk11") return ELVINE + 10 ;
	if (MapName == "arefarm") return ARESDEN ;
	if (MapName == "elvfarm") return ELVINE ;
	if (MapName == "arewrhus") return ARESDEN ;
	if (MapName == "elvwrhus") return ELVINE ;

	/*  memcmp 를하기 때문에 앞에서 걸린다.
	if (memcmp(pMapName, "gshop_1f") == 0) return 1;
	if (memcmp(pMapName, "bsmith_1f") == 0) return 1;
	if (memcmp(pMapName, "wrhus_1f") == 0) return 1;

	if (memcmp(pMapName, "gshop_2f") == 0) return 2;
	if (memcmp(pMapName, "bsmith_2f") == 0) return 2;
	if (memcmp(pMapName, "wrhus_2f") == 0) return 2; */


	return 0;
}

Map * GServer::GetMap(string name)
{
	for (auto map : maplist)
	{
		if (map->name == name)
			return map;
	}
	return 0;
}

void GServer::RequestHuntmode(Client * client)
{
	if (client == 0) return;

	if (client->playerKillCount > 0 ) return;
	if (client->m_bIsInitComplete == false) return;
	if (client->mapName.substr(0, 8) == "cityhall") return;
	
	if ((client->level > DEF_LIMITHUNTERLEVEL) && (client->m_bIsHunter == false) ) return;


	if (client->faction == "aresden")
	{
		client->faction = "arehunter";
		client->m_bIsHunter = true;
	}
	else if (client->faction == "elvine") 
	{
		client->faction = "elvhunter";
		client->m_bIsHunter = true;
	}
	else if (client->faction == "arehunter")
	{
		client->faction = "aresden";
		client->m_bIsHunter = false;
	}	
	else if (client->faction == "elvhunter")
	{
		client->faction = "elvine";
		client->m_bIsHunter = false;
	}

//	SendNotifyMsg(0, 0, NOTIFY_RESPONSE_HUNTMODE, 0, 0, 0, client->m_cLocation);

	SendEventToNearClient_TypeA(client, MSGID_MOTION_NULL, 0, 0, 0);
}

void GServer::SendNotifyMsg(Client * from, Client * to, uint16_t wMsgType, uint64_t sV1, uint64_t sV2, uint64_t sV3, string pString, 
							uint64_t sV4, uint64_t sV5, uint64_t sV6, uint64_t sV7, uint64_t sV8, uint64_t sV9, string pString2)
{

	StreamWrite sw;

	if (!to) return; // from may not be set.. only like 2 packets in the ENTIRE GAME use it. mostly trade requests. to HAS to be set

	sw.WriteInt(MSGID_NOTIFY);
	sw.WriteShort(wMsgType);


	switch (wMsgType)
	{
	case NOTIFY_PINGMAP:
		sw.WriteByte(uint8_t(sV1));
		sw.WriteShort(uint16_t(sV2));
		sw.WriteShort(uint16_t(sV3));
		to->SWrite(sw);
		//to->socket->write(sw);
		break;
	case NOTIFY_TELEPORT_REJECTED:
		sw.WriteShort(uint16_t(sV1));
		sw.WriteShort(uint16_t(sV2));
		to->SWrite(sw);
		//to->socket->write(sw);
		break;
	case NOTIFY_LOCKEDMAP:
		sw.WriteShort(uint16_t(sV1));
		sw.WriteString(pString, 10);
		to->SWrite(sw);
		//to->socket->write(sw);
		break;
	case NOTIFY_REQDEF:
		sw.WriteInt(uint16_t(sV1));
		to->SWrite(sw);
		//to->socket->write(sw);
		break;
	case NOTIFY_RESPONSE_HUNTMODE:
		sw.WriteString(pString, 10);
		to->SWrite(sw);
		//to->socket->write(sw);
		break;
	case NOTIFY_PARTY:
		switch (sV1) {
		case 4:
		case 6:
			sw.WriteShort(uint16_t(sV1));
			sw.WriteShort(uint16_t(sV2));
			sw.WriteShort(uint16_t(sV3));
			sw.WriteString(pString, 10);
			to->SWrite(sw);
			//to->socket->write(sw);
			break;

		case 5:
			sw.WriteShort(uint16_t(sV1));
			sw.WriteShort(uint16_t(sV2));
			sw.WriteShort(uint16_t(sV3));
			sw.WriteString(pString, sV3*11);
			to->SWrite(sw);
			//to->socket->write(sw);
			break;

		default:
			sw.WriteShort(uint16_t(sV1));
			sw.WriteShort(uint16_t(sV2));
			sw.WriteShort(uint16_t(sV3));
			sw.WriteShort(uint16_t(sV4));
			to->SWrite(sw);
			//to->socket->write(sw);
			break;
		}
		break;

	case NOTIFY_PARTY_COORDS:
		sw.WriteString(pString, 10);
		sw.WriteShort(uint16_t(sV1));
		sw.WriteShort(uint16_t(sV2));
		to->SWrite(sw);
		//to->socket->write(sw);
		break;
	case NOTIFY_CANNOTCONSTRUCT:
	case NOTIFY_METEORSTRIKECOMING:
	case NOTIFY_METEORSTRIKEHIT:
	case NOTIFY_SPECIALABILITYENABLED:
	case NOTIFY_FORCEDISCONN:
	case NOTIFY_OBSERVERMODE:
	case NOTIFY_QUESTCOMPLETED:
	case NOTIFY_QUESTABORTED:
		sw.WriteShort(uint16_t(sV1));
		to->SWrite(sw);
		//to->socket->write(sw);
		break;
	case NOTIFY_ITEMPOSLIST:
 		for (ItemWrap * var : to->itemList)
 		{
			sw.WriteShort(var->_item->x);
			sw.WriteShort(var->_item->y);
 		}
		to->SWrite(sw);
		//to->socket->write(sw);
 		break;
	case NOTIFY_WHISPERMODEON:
	case NOTIFY_WHISPERMODEOFF:
	case NOTIFY_PLAYERNOTONGAME:
		sw.WriteString(pString, 10);
		sw.WriteString("          ", 10);
		to->SWrite(sw);
		//to->socket->write(sw);
		break;

	case NOTIFY_NOTICEMSG:
		sw.WriteString(pString, pString.length());
		sw.WriteByte(0);
		to->SWrite(sw);
		//to->socket->write(sw);
		break;
	case NOTIFY_EXP:
		sw.WriteInt(uint32_t(to->experience));//TODO: make 64bit client side still?
		sw.WriteInt(to->m_reputation);
		to->SWrite(sw);
		//to->socket->write(sw);
		break;

	case NOTIFY_HP:
		sw.WriteInt(uint32_t(to->health));
		sw.WriteInt(uint32_t(to->mana));
		to->SWrite(sw);
		//to->socket->write(sw);
		break;

	case NOTIFY_MP:
		sw.WriteInt(uint32_t(to->mana));
		to->SWrite(sw);
		//to->socket->write(sw);
		break;

	case NOTIFY_SP:
		sw.WriteInt(to->m_iSP);
		to->SWrite(sw);
		//to->socket->write(sw);
		break;

	case NOTIFY_CHARISMA:
		sw.WriteInt(to->m_iCharisma);
		to->SWrite(sw);
		//to->socket->write(sw);
		break;

	case NOTIFY_SETTING_SUCCESS:
	case NOTIFY_LEVELUP:
		sw.WriteInt(to->level);
		sw.WriteInt(to->GetStr());
		sw.WriteInt(to->GetVit());
		sw.WriteInt(to->GetDex());
		sw.WriteInt(to->GetInt());
		sw.WriteInt(to->GetMag());
		sw.WriteInt(to->GetAgi());
		to->SWrite(sw);
		//to->socket->write(sw);
		break;

	case NOTIFY_CHANGETITLE:
		sw.WriteString(pString, 11);
		to->SWrite(sw);
		break;

	case NOTIFY_REQTITLEANSWER:
		sw.WriteInt(sV1);//TitleType
		sw.WriteInt(sV2);//ReturnID
		sw.WriteInt(sV3);//CrusadeJob
		sw.WriteString(pString);//ActiveTitle
		to->SWrite(sw);
		break;

	case NOTIFY_KILLED:

		sw.WriteString(pString, 20);//ActiveTitle

		to->SWrite(sw);
		break;

// 	case NOTIFY_UPDATETITLELIST:
// 
// 		//for ()
// 		int iNumPoints = (uint32_t)sV1;
// 		int iNextRankPoints = (uint32_t)sV2;
// 		int iRank = (uint32_t)sV3;
// 		int iTitleIndex = (uint32_t)sV4;
// 		char cActiveTitle = (char)sV5;
// 
// 		memset(cp, 0, 21);
// 		memcpy(cp, pString, strlen(pString) + 1);
// 		cp += 21;
// 
// 		ip = (int *)cp;
// 		*ip = iNumPoints;
// 		cp += 4;
// 
// 		ip = (int *)cp;
// 		*ip = iNextRankPoints;
// 		cp += 4;
// 
// 		ip = (int *)cp;
// 		*ip = iRank;
// 		cp += 4;
// 
// 		ip = (int *)cp;
// 		*ip = iTitleIndex;
// 		cp += 4;
// 
// 		*cp = cActiveTitle;
// 		cp++;
// 
// 		/ *ip = (int *)cp;
// 		*ip = player->iNumPoints;
// 		cp += 4;
// 
// 		ip = (int *)cp;
// 		*ip = player->iNextRankPoints;
// 		cp += 4;
// 
// 		ip = (int *)cp;
// 		*ip = player->iRank;
// 		cp += 4;
// 
// 		ip = (int *)cp;
// 		*ip = player->iTitleIndex;
// 		cp += 4;
// 
// 		*cp = player->cActiveTitle;
// 		cp++; */
// 
// 			iRet = player->socket->write(cData, 44);
// 		break;
// 
// 	case NOTIFY_TITLELEVELUP:
// 	{
// 		int iNumPoints = (uint32_t)sV1;
// 		int iNextRankPoints = (uint32_t)sV2;
// 		int iRank = (uint32_t)sV3;
// 		int iTitleIndex = (uint32_t)sV4;
// 
// 		memset(cp, 0, 21);
// 		memcpy(cp, pString, strlen(pString) + 1);
// 		cp += 21;
// 
// 		ip = (int *)cp;
// 		*ip = iNumPoints;
// 		cp += 4;
// 
// 		ip = (int *)cp;
// 		*ip = iNextRankPoints;
// 		cp += 4;
// 
// 		ip = (int *)cp;
// 		*ip = iRank;
// 		cp += 4;
// 
// 		ip = (int *)cp;
// 		*ip = iTitleIndex;
// 		cp += 4;
// 
// 		/ *ip = (int *)cp;
// 		*ip = player->iNumPoints;
// 		cp += 4;
// 
// 		ip = (int *)cp;
// 		*ip = player->iNextRankPoints;
// 		cp += 4;
// 
// 		ip = (int *)cp;
// 		*ip = player->iRank;
// 		cp += 4;
// 
// 		ip = (int *)cp;
// 		*ip = player->iTitleIndex;
// 		cp += 4; */
// 
// 			iRet = player->socket->write(cData, 43);
// 		break;
// 	}
// 		SendNotifyMsg(nullptr, client, NOTIFY_REQTITLEANSWER, findclient->TitleType, ReturnID, iCrusadeJob, findclient->ActiveTitle);

	}
/*
	switch (wMsgType)
	{
	case NOTIFY_DONATEEVENTINFO:
		Push(cp, m_donateEventPot);
		
		iRet = player->socket->write(cData, cp - cData);
		break;

	case NOTIFY_ADMININFO:
		Push(cp, (uint32_t)player->m_iAdminUserLevel);
		Push(cp, player->m_GMFlags);
		
		iRet = player->socket->write(cData, cp - cData);
		break;

	case NOTIFY_MAILRECEIVED:
	case NOTIFY_GUILDBOARDPOST:
		Push(cp, pString);

		iRet = player->socket->write(cData, cp - cData);
		break;

	case NOTIFY_MONSTERCOUNT:
		Push(cp, (uint16_t)sV1);
		npc = (CNpc*)sV2;
		if(npc)
		{
			Push(cp, (uint16_t)npc->m_sX);
			Push(cp, (uint16_t)npc->m_sY);
		} else {
			Push(cp, (uint16_t)-1);
			Push(cp, (uint16_t)-1);
		}
		iRet = player->socket->write(cData, cp - cData);
		break;

	case NOTIFY_GUILDCONTRIBUTERSP:
		Push(cp, (uint8_t)sV1);
		Push(cp, sV2);

		iRet = player->socket->write(cData, cp - cData);
		break;

	case NOTIFY_ITEMTOGUILDBANK:
		pItem = (CItem*)sV1;
		if (!pItem) return;

		Push(cp, sV1);
		Push(cp, sV2); // warehouseVer

		WriteItemData(cp, pItem);

		Push(cp, (uint16_t)pItem->m_sItemEffectValue2);
		Push(cp, pItem->m_dwAttribute);
		Push(cp, (uint8_t)pItem->m_sItemSpecEffectValue2);
		
		for(i = 0; i < MAXITEMSOCKETS; i++)
		{
			Push(cp, pItem->m_sockets[i]);
		}
		iRet = player->socket->write(cData, cp - cData);
		break;

	case NOTIFY_SOCCERVICTORY: 
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;
		iRet = player->socket->write(cData, 8);
		break;

	case NOTIFY_SOCCER:
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV2;
		cp += 2;
		iRet = player->socket->write(cData, 10);
		break;

	case NOTIFY_SOCCERBEGIN:					
		iRet = player->socket->write(cData, 6);
		break;

	case NOTIFY_HELDENIANVICTORY: //0x0BEB
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;
		iRet = player->socket->write(cData, 8);
		break;
	
	case NOTIFY_HELDENIANSTART:
	case NOTIFY_HELDENIANEND:					
		iRet = player->socket->write(cData, 6);
		break;
	case NOTIFY_HELDENIANCOUNT:
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV2;
		cp += 2;
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV3;
		cp += 2;
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV4;		
		cp += 2;
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV5;		
		cp += 2;
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV6;
		cp += 2;
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV7;		
		cp += 2;
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV8;		
		cp += 2;
		iRet = player->socket->write(cData, 22);
		break;

	case NOTIFY_APOCGATECLOSE:
	case NOTIFY_APOCGATEOPEN:
		ip  = (int *)cp;
		*ip = sV1;
		cp += 4;
		ip  = (int *)cp;
		*ip = sV2;
		cp += 4;
		memcpy(cp,pString,10);
		cp += 10;
		iRet = player->socket->write(cData, 24);
		break;

	case NOTIFY_ABADDONKILLED:
		memcpy(cp, m_pClientList[iFromH]->m_cCharName, 10);
		cp += 10;
		iRet = player->socket->write(cData, 16);
		break;

	case NOTIFY_APOCGATESTARTMSG:
	case NOTIFY_APOCGATEENDMSG:
		iRet = player->socket->write(cData, 6);
		break;



	case NOTIFY_LGNPTS:
		dwp = (uint32*)cp;
		*dwp = sV1;

		iRet = player->socket->write(cData, 10);
		break;



	case NOTIFY_REQGUILDNAMEANSWER:
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;

		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV2;
		cp += 2;

		memcpy(cp, pString, 20);
		cp += 20;

		iRet = player->socket->write(cData, 32);
		break;

	case NOTIFY_TCLOC:
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;

		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV2;
		cp += 2;

		memcpy(cp, pString, 10);
		cp += 10;

		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV4;
		cp += 2;

		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV5;
		cp += 2;

		memcpy(cp, pString2, 10);
		cp += 10;

		iRet = player->socket->write(cData, 34);
		break;

	case NOTIFY_SLATE_STATUS:
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;

		iRet = player->socket->write(cData, 8);
		break;

	case NOTIFY_QUESTCOUNTER:
		ip  = (int *)cp;
		*ip = sV1;
		cp += 4;

		iRet = player->socket->write(cData, 24);
		break;

	case NOTIFY_SLATE_EXP:
	case NOTIFY_SLATE_MANA:
	case NOTIFY_SLATE_INVINCIBLE:
	case NOTIFY_RESURRECTPLAYER:
		iRet = player->socket->write(cData, 6);
		break;

	case NOTIFY_SLATE_CREATEFAIL:
		iRet = player->socket->write(cData, 6);
		break;

	case NOTIFY_SLATE_CREATESUCCESS:
		dwp  = (uint32_t *)cp;
		*dwp = sV1;
		cp += 4;

		iRet = player->socket->write(cData, 10);
		break;



	case NOTIFY_GRANDMAGICRESULT:
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;

		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV2;
		cp += 2;

		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV3;
		cp += 2;

		memcpy(cp, pString, 10);
		cp += 10;

		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV4;
		cp += 2;

		//		sp = (short *)cp;
		//		*sp = (short)sV9;
		//		cp += 2;

		if (sV9 > 0)  {
			memcpy(cp,pString2, (sV9+1)*2) ;
			cp += (sV9+1)*2;
		}
		else 
		{
			sp = (short *)cp;
			*sp = (short)0;
			cp += 2;
		}

		iRet = player->socket->write(cData, 24 + (sV9+1)*2 );
		break;

	case NOTIFY_MAPSTATUSNEXT:
		memcpy(cp, pString, sV1);
		cp += sV1;
		iRet = player->socket->write(cData, 6+sV1);
		break;

	case NOTIFY_MAPSTATUSLAST:
		memcpy(cp, pString, sV1);
		cp += sV1;
		iRet = player->socket->write(cData, 6+sV1);
		break;



	case NOTIFY_BUILDITEMSUCCESS:
	case NOTIFY_BUILDITEMFAIL:
		if (sV1 >= 0) {
			sp = (short *)cp;
			*sp = (short)sV1;
			cp += 2;
		}
		else {
			sp = (short *)cp;
			*sp = (short)sV1 + 10000;
			cp += 2;
		}

		sp = (short *)cp;
		*sp = (short)sV2;
		cp += 2;

		iRet = player->socket->write(cData, 10);
		break;

	case NOTIFY_QUESTREWARD:
		sp = (short *)cp;
		*sp = (short)sV1;
		cp += 2;

		sp = (short *)cp;
		*sp = (short)sV2;
		cp += 2;

		ip = (int *)cp;
		*ip = (int)sV3;
		cp += 4;

		memcpy(cp, pString, 20);
		cp += 20;

		ip = (int *)cp;
		*ip = (int)sV4;
		cp += 4;

		iRet = player->socket->write(cData, 38);
		break;



	case NOTIFY_QUESTCONTENTS:
		sp = (short *)cp;
		*sp = (short)sV1;
		cp += 2;

		sp = (short *)cp;
		*sp = (short)sV2;
		cp += 2;

		sp = (short *)cp;
		*sp = (short)sV3;
		cp += 2;

		sp = (short *)cp;
		*sp = (short)sV4;
		cp += 2;

		sp = (short *)cp;
		*sp = (short)sV5;
		cp += 2;

		sp = (short *)cp;
		*sp = (short)sV6;
		cp += 2;

		sp = (short *)cp;
		*sp = (short)sV7;
		cp += 2;

		sp = (short *)cp;
		*sp = (short)sV8;
		cp += 2;

		sp = (short *)cp;
		*sp = (short)sV9;
		cp += 2;

		if (pString2 != NULL) memcpy(cp, pString2, 20);
		cp += 20;

		iRet = player->socket->write(cData, 44);
		break;

	case NOTIFY_GIZONITEMUPGRADELEFT:
	case NOTIFY_ITEMATTRIBUTECHANGE:

		sp = (short *)cp;
		*sp = (short)sV1;
		cp += 2;

		dwp = (uint32_t *)cp;
		*dwp = (uint32_t)sV2;
		cp += 4;

		dwp = (uint32_t *)cp;
		*dwp = (uint32_t)sV3;
		cp += 4;

		dwp = (uint32_t *)cp;
		*dwp = (uint32_t)sV4;
		cp += 4;

		iRet = player->socket->write(cData, 20);
		break; 

	case NOTIFY_ITEMUPGRADEFAIL:

		sp = (short *)cp;
		*sp = (short)sV1;
		cp += 2;

		iRet = player->socket->write(cData, 8);

		break ;

	case NOTIFY_GIZONEITEMCHANGE:
		sp = (short *)cp;
		*sp = (short)sV1;
		cp += 2;

		*cp = (char)sV2;
		cp++;

		sp = (short *)cp;
		*sp = (short)sV3;
		cp += 2;

		sp = (short *)cp;
		*sp = (short)sV4;
		cp += 2;

		sp = (short *)cp;
		*sp = (short)sV5;
		cp += 2;

		*cp = (char)sV6;
		cp++;

		*cp = (char)sV7;
		cp++;


		dwp  = (uint32_t *)cp;
		*dwp = (uint32_t)sV8;
		cp += 4;

		memcpy(cp, pString, 20);
		cp += 20;


		iRet = player->socket->write(cData, 41);
		break;

	case NOTIFY_ITEMCOLORCHANGE:
		sp = (short *)cp;
		*sp = (short)sV1;
		cp += 2;

		sp = (short *)cp;
		*sp = (short)sV2;
		cp += 2;

		iRet = player->socket->write(cData, 10);
		break;

	case NOTIFY_ENERGYSPHERECREATED:
		sp = (short *)cp;
		*sp = (short)sV1;
		cp += 2;

		sp = (short *)cp;
		*sp = (short)sV2;
		cp += 2;
		
		memcpy(cp, pString, 20);
		cp += 20;

		iRet = player->socket->write(cData, 30);
		break;

	case NOTIFY_ITEMSOCKETCHANGE:
		Push(cp, (uint16_t)sV1);

		pItem = (CItem*)sV2;
		for(i = 0; i < MAXITEMSOCKETS; i++)
		{
			Push(cp, pItem->m_sockets[i]);
		}

		iRet = player->socket->write(cData, cp - index);
		break;

	case NOTIFY_NOMOREAGRICULTURE:	   
	case NOTIFY_AGRICULTURESKILLLIMIT: 
	case NOTIFY_AGRICULTURENOAREA:     
		iRet = player->socket->write(cData, 6);
		break;

	case NOTIFY_NOMORECRUSADESTRUCTURE:
	case NOTIFY_EXCHANGEITEMCOMPLETE:
	case NOTIFY_CANCELEXCHANGEITEM:
		iRet = player->socket->write(cData, 6);
		break;

	case NOTIFY_SETEXCHANGEITEM:
		sp = (short *)cp;
		*sp = (short)sV1;
		cp += 2;
		
		pItem = (CItem*)sV2;
		
		Push(cp, (uint16_t)pItem->m_sSprite);
		Push(cp, (uint16_t)pItem->m_sSpriteFrame);
		Push(cp, sV3); // amount
		Push(cp, (uint8_t)pItem->m_cItemColor);
		Push(cp, (uint16_t)pItem->m_wCurLifeSpan);
		Push(cp, (uint16_t)pItem->m_wMaxLifeSpan);
		Push(cp, (uint16_t)(pItem->m_sItemSpecEffectValue2 + 100));
		Push(cp, pItem->m_cName, 20);

		Push(cp, m_pClientList[iFromH]->m_cCharName, 10);
		Push(cp, pItem->m_dwAttribute);
		for(i = 0; i < MAXITEMSOCKETS; i++)
		{
			Push(cp, pItem->m_sockets[i]);
		}
		iRet = player->socket->write(cData, 57 + MAXITEMSOCKETS);
		break;

	case NOTIFY_OPENEXCHANGEWINDOW:
		sp = (short *)cp;
		*sp = (short)sV1;
		cp += 2;

		pItem = (CItem*)sV2;
		
		Push(cp, (uint16_t)pItem->m_sSprite);
		Push(cp, (uint16_t)pItem->m_sSpriteFrame);
		Push(cp, sV3); // amount
		Push(cp, (uint8_t)pItem->m_cItemColor);
		Push(cp, (uint16_t)pItem->m_wCurLifeSpan);
		Push(cp, (uint16_t)pItem->m_wMaxLifeSpan);
		Push(cp, (uint16_t)(pItem->m_sItemSpecEffectValue2 + 100));
		Push(cp, pItem->m_cName, 20);

		Push(cp, m_pClientList[iFromH]->m_cCharName, 10);
		Push(cp, pItem->m_dwAttribute);
		for(i = 0; i < MAXITEMSOCKETS; i++)
		{
			Push(cp, pItem->m_sockets[i]);
		}

		iRet = player->socket->write(cData, 57 + MAXITEMSOCKETS);
		break;

	case NOTIFY_NOTFLAGSPOT:
		iRet = player->socket->write(cData, 6);
		break;



	case NOTIFY_ENEMYKILLS:
		ip  = (int *)cp;
		*ip = (int)sV1;
		ip++;
		*ip = (int)sV2;

		iRet = player->socket->write(cData, 14);
		break;

	case NOTIFY_CRUSADE:
		ip = (int *)cp;
		*ip = (int)sV1;
		cp += 4;

		ip = (int *)cp;
		*ip = (int)sV2;
		cp += 4;

		ip = (int *)cp;
		*ip = (int)sV3;
		cp += 4;

		ip = (int *)cp;
		*ip = (int)sV4;
		cp += 4;

		iRet = player->socket->write(cData, 22);
		break;

	case NOTIFY_CONSTRUCTIONPOINT:
	case NOTIFY_SPECIALABILITYSTATUS:
	case NOTIFY_DAMAGEMOVE:
		sp = (short *)cp;
		*sp = (short)sV1;
		cp += 2;

		sp = (short *)cp;
		*sp = (short)sV2;
		cp += 2;

		sp = (short *)cp;
		*sp = (short)sV3;
		cp += 2;

		iRet = player->socket->write(cData, 12);
		break;

	case NOTIFY_DOWNSKILLINDEXSET:
		sp = (short *)cp;
		*sp = (short)sV1;
		cp += 2;

		iRet = player->socket->write(cData, 8);
		break;

	case NOTIFY_ADMINIFO:
		switch (sV1) {
		case 1:

			ip  = (int *)cp;
			*ip	= m_pNpcList[sV2]->m_iHP;
			cp += 4;

			ip  = (int *)cp;
			*ip	= m_pNpcList[sV2]->m_iDefenseRatio;
			cp += 4;

			ip  = (int *)cp;
			*ip	= m_pNpcList[sV2]->m_bIsSummoned;
			cp += 4;

			ip  = (int *)cp;
			*ip	= m_pNpcList[sV2]->m_cActionLimit;
			cp += 4;

			ip  = (int *)cp;
			*ip	= m_pNpcList[sV2]->m_iHitDice;
			cp += 4;

			dwp  = (uint32_t *)cp; 
			*dwp = m_pNpcList[sV2]->m_dwDeadTime;
			cp += 4;

			dwp  = (uint32_t *)cp; 
			*dwp = m_pNpcList[sV2]->m_dwRegenTime;
			cp += 4;

			ip  = (int *)cp; 
			*ip	= (int)m_pNpcList[sV2]->m_bIsKilled;
			cp += 4;

			iRet = player->socket->write(cData, 26 + 12);
			break;
		}
		break;

	case NOTIFY_NPCTALK:
		sp = (short *)cp;
		*sp = (short)sV1;
		cp += 2;

		sp = (short *)cp;
		*sp = (short)sV2;
		cp += 2;

		sp = (short *)cp;
		*sp = (short)sV3;
		cp += 2;

		sp = (short *)cp;
		*sp = (short)sV4;
		cp += 2;

		sp = (short *)cp;
		*sp = (short)sV5;
		cp += 2;

		sp = (short *)cp;
		*sp = (short)sV6;
		cp += 2;

		sp = (short *)cp;
		*sp = (short)sV7;
		cp += 2;

		sp = (short *)cp;
		*sp = (short)sV8;
		cp += 2;

		sp = (short *)cp;
		*sp = (short)sV9;
		cp += 2;

		if (pString != NULL) memcpy(cp, pString, 20);
		cp += 20;

		if (pString2 != NULL) memcpy(cp, pString2, 20);
		cp += 20;

		iRet = player->socket->write(cData, 64);
		break;
	case NOTIFY_EVENTILLUSION:
	case NOTIFY_EVENTTP:
		*cp = (char)sV1;
		cp++;
		*cp = (char)sV2;
		cp++;

		iRet = player->socket->write(cData, 8);
		break;

	case NOTIFY_EVENTSPELL:
		*cp = sV1;
		cp++;
		*cp = sV2;
		cp++;
		*cp = player->IsGM();
		cp++;
		iRet = player->socket->write(cData, 9);
		break;

	case NOTIFY_EVENTARMOR:
	case NOTIFY_EVENTSHIELD:
	case NOTIFY_EVENTCHAT:
	case NOTIFY_EVENTPARTY:
		*cp = sV1;
		cp++;
		*cp = player->IsGM();
		cp++;
		iRet = player->socket->write(cData, 8);
		break;

	case NOTIFY_EVENTRESET:
	case NOTIFY_CRAFTING_SUCCESS:
	case NOTIFY_PORTIONSUCCESS:
	case NOTIFY_LOWPORTIONSKILL:
	case NOTIFY_PORTIONFAIL:
	case NOTIFY_NOMATCHINGPORTION:
	case NOTIFY_CRAFTING_FAIL:
	case NOTIFY_NOMATCHINGCRAFTING:
	case NOTIFY_NO_CRAFT_CONTRIB: 
		iRet = player->socket->write(cData, 6);
		break;

	case NOTIFY_SUPERATTACKLEFT:
		sp = (short *)cp;
		*sp = player->m_iSuperAttackLeft;
		cp += 2;

		iRet = player->socket->write(cData, 8);
		break;

	case NOTIFY_SAFEATTACKMODE:
		*cp = player->m_bIsSafeAttackMode;
		cp++;

		iRet = player->socket->write(cData, 7);
		break;

	case NOTIFY_QUERY_JOINPARTY:
	case NOTIFY_IPACCOUNTINFO:
		strcpy(cp, pString);
		cp += strlen(pString);

		iRet = player->socket->write(cData, 6 + strlen(pString) + 1);
		break;

	case NOTIFY_REWARDGOLD:
		dwp = (uint32_t *)cp;
		*dwp = player->m_iRewardGold;
		cp += 4;

		iRet = player->socket->write(cData, 10);
		break;

	case NOTIFY_SERVERSHUTDOWN:
		*cp = (char)sV1;
		cp++;

		iRet = player->socket->write(cData, 7);
		break;

	case NOTIFY_GLOBALATTACKMODE:
	case NOTIFY_WEATHERCHANGE:
		*cp = (char)sV1;
		cp++;

		iRet = player->socket->write(cData, 7);
		break;

	case NOTIFY_MONSTEREVENT_POSITION:
		*cp = (char)sV3;
		cp++;

		sp = (short*)cp;
		*sp = (short)sV1;
		cp += 2;

		sp = (short*)cp;
		*sp = (short)sV2;
		cp += 2;

		iRet = player->socket->write(cData, 11);
		break;


	case NOTIFY_FISHCANCELED:
	case NOTIFY_FISHSUCCESS:
	case NOTIFY_FISHFAIL:
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;

		iRet = player->socket->write(cData, 8);
		break;

	case NOTIFY_DEBUGMSG:
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;

		iRet = player->socket->write(cData, 8);
		break;

	case NOTIFY_FISHCHANCE:
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;

		iRet = player->socket->write(cData, 8);
		break;

	case NOTIFY_ENERGYSPHEREGOALIN:
	case NOTIFY_EVENTFISHMODE:
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;

		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV2;
		cp += 2;

		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV3;
		cp += 2;

		memcpy(cp, pString, 20);
		cp += 20;

		iRet = player->socket->write(cData, 32);
		break;

	case NOTIFY_NOTICEMSG:
		memcpy(cp, pString, strlen(pString));
		cp += strlen(pString);

		*cp = NULL;
		cp++;

		iRet = player->socket->write(cData, strlen(pString) + 7);
		break;

	case NOTIFY_CANNOTRATING:
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;

		iRet = player->socket->write(cData, 8);
		break;

	case NOTIFY_RATINGPLAYER:
		*cp = (char)sV1;
		cp++;

		memcpy(cp, pString, 10);
		cp += 10;

		ip = (int *)cp;
		*ip = player->m_reputation;
		cp += 4;

		iRet = player->socket->write(cData, 22);
		break;

	case NOTIFY_ADMINUSERLEVELLOW:
		iRet = player->socket->write(cData, 6);
		break;

	case NOTIFY_PLAYERSHUTUP:
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;

		memcpy(cp, pString, 10);
		cp += 10;

		iRet = player->socket->write(cData, 19);
		break;

	case NOTIFY_TIMECHANGE:
		*cp = (char)sV1;
		cp++;

		iRet = player->socket->write(cData, 7);
		break;

	case NOTIFY_TOBERECALLED:
		iRet = player->socket->write(cData, 6);
		break;

	case NOTIFY_HUNGER:
		*cp = (char)sV1;
		cp++;

		iRet = player->socket->write(cData, 7);
		break;
#ifdef MonsterBar
	case NOTIFY_NPCBAR:
		*cp = (char)sV1;
		cp++;

		iRet = m_pClientList[iToH]->socket->write(cData, 7);
		break;

	case NOTIFY_NPCBAR2:
		*cp = (char)sV1;
		cp++;

		iRet = m_pClientList[iToH]->socket->write(cData, 7);
		break;
#endif
	case NOTIFY_PLAYERPROFILE:
		if (strlen(pString) > 100) {
			memcpy(cp, pString, 100);
			cp += 100;
		}
		else {
			memcpy(cp, pString, strlen(pString));
			cp += strlen(pString);
		}
		*cp = NULL;
		cp++;

		iRet = player->socket->write(cData, 7 + strlen(pString));
		break;

	case NOTIFY_PLAYERONGAME:
	case NOTIFY_FRIENDONGAME:
		memcpy(cp, pString, 10);
		cp += 10;
		if (pString != NULL) {
			memcpy(cp, pString2, 14);
			cp += 14;
		}
		iRet = player->socket->write(cData, 31);
		break;



	case NOTIFY_ITEMSOLD:
	case NOTIFY_ITEMREPAIRED:
		dwp = (uint32_t *)cp;
		*dwp = (uint32_t)sV1;
		cp += 4;
		dwp = (uint32_t *)cp;
		*dwp = (uint32_t)sV2;
		cp += 4;

		iRet = player->socket->write(cData, 14);
		break;

	case NOTIFY_REPAIRITEMPRICE:
	case NOTIFY_SELLITEMPRICE:
		dwp = (uint32_t *)cp;
		*dwp = (uint32_t)sV1;
		cp += 4;
		dwp = (uint32_t *)cp;
		*dwp = (uint32_t)sV2;
		cp += 4;
		dwp = (uint32_t *)cp;
		*dwp = (uint32_t)sV3;
		cp += 4;
		dwp = (uint32_t *)cp;
		*dwp = (uint32_t)sV4;
		cp += 4;

		memcpy(cp, pString, 20);
		cp += 20;

		iRet = player->socket->write(cData, 42);
		break;

	case NOTIFY_CANNOTREPAIRITEM:
	case NOTIFY_CANNOTSELLITEM:
		wp = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;

		wp = (uint16_t *)cp;
		*wp = (uint16_t)sV2;
		cp += 2;

		memcpy(cp, pString, 20);
		cp += 20;

		iRet = player->socket->write(cData, 30);

		break;

		//(word)sV1, sV2
	case NOTIFY_SHOWMAP:
		wp  = (uint16_t *)cp;  
		*wp = (uint16_t)sV1;
		cp += 2;

		wp  = (uint16_t *)cp;  
		*wp = (uint16_t)sV2;
		cp += 2;

		iRet = player->socket->write(cData, 10);
		break;

		//(word)sV1
	case NOTIFY_SKILLUSINGEND:
		wp  = (uint16_t *)cp;  
		*wp = (uint16_t)sV1;
		cp += 2;

		iRet = player->socket->write(cData, 8);
		break;

	case NOTIFY_TOTALUSERS:
		wp  = (uint16_t *)cp;    
		*wp = (uint16_t)(m_iTotalGameServerClients/ * + m_onlineCntAdd* /) ; //_iGetTotalClients();
		cp += 2;

		iRet = player->socket->write(cData, 8);
		break;

		// (char)sV1
	case NOTIFY_EVENTSTART:
	case NOTIFY_EVENTSTARTING:
	case NOTIFY_EVENTSTARTING2:
	case NOTIFY_EVENTSTARTING3:
	case NOTIFY_RELICINALTAR:
		*cp = (char)sV1;
		cp++;

		iRet = player->socket->write(cData, 7);
		break;

	case NOTIFY_EVENTWINNER:
		// event
		*cp = (char)sV1;
		cp++;

		// winner
		*cp = (char)sV2;
		cp++;

		iRet = player->socket->write(cData, 8);
		break;

	case NOTIFY_CASUALTIES:
		wp  = (uint16_t *)cp;  
		*wp = (uint16_t)sV1; // Aresden deaths
		cp += 2;

		wp  = (uint16_t *)cp;  
		*wp = (uint16_t)sV2; // Elvine
		cp += 2;

		wp  = (uint16_t *)cp;  
		*wp = (uint16_t)sV4; // Aresden kills
		cp += 2;

		wp  = (uint16_t *)cp;  
		*wp = (uint16_t)sV5; // Elvine
		cp += 2;

		iRet = player->socket->write(cData, 18);
		break;

	case NOTIFY_SHIELDHP: // Shield xRisenx
		Push(cp, (uint8_t)sV1);
		//Push(cp, (uint16_t)sV2);
		Push(cp, (uint32_t)sV2);
		
		iRet = player->socket->write(cData, 11); // 9->11
		break;

	case NOTIFY_RELICPOSITION:
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1; //Relic X
		cp += 2;

		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV2; //Relic Y
		cp += 2;
		iRet = player->socket->write(cData, 10);
		break;

	case NOTIFY_RELICGRABBED:
		memcpy(cp, pString, 10);
		cp += 10;
		iRet = player->socket->write(cData, 17);
		break;

	case NOTIFY_MAGICEFFECTOFF:
	case NOTIFY_MAGICEFFECTON:
		wp  = (uint16_t *)cp;  
		*wp = (uint16_t)sV1;
		cp += 2;

		dwp  = (uint32_t *)cp;  
		*dwp = (uint32_t)sV2;
		cp += 4;

		dwp  = (uint32_t *)cp;  
		*dwp = (uint32_t)sV3;
		cp += 4;

		iRet = player->socket->write(cData, 16);
		break;

	case NOTIFY_CANNOTITEMTOBANK:
	case NOTIFY_SETTING_FAILED:
		iRet = player->socket->write(cData, 6);
		break;

	case NOTIFY_SERVERCHANGE:
		memcpy(cp, player->m_cMapName, 10);
		cp += 10;

		if (m_iGameServerMode == 1) // LAN
			memcpy(cp, m_cGameServerAddrExternal, 15);
		else // INTERNET
			memcpy(cp, m_cLogServerAddr, 15);
		cp += 15;

		ip = (int *)cp;
		*ip = m_iLogServerPort;
		cp += 4;

		iRet = player->socket->write(cData, 16 +19);
		break;

	case NOTIFY_SKILL:
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;

		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV2;
		cp += 2;

		iRet = player->socket->write(cData, 10);
		break;

	case NOTIFY_SETITEMCOUNT:
		wp  = (uint16_t *)cp;  
		*wp = (uint16_t)sV1;
		cp += 2;

		dwp  = (uint32_t *)cp;  
		*dwp = (uint32_t)sV2;
		cp += 4;

		*cp = (char)sV3;
		cp++;

		iRet = player->socket->write(cData, 13);
		break;

	case NOTIFY_ITEMDEPLETED_ERASEITEM:
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;

		wp = (uint16_t *)cp;
		*wp = (uint16_t)sV2;
		cp += 2;

		iRet = player->socket->write(cData, 10);
		break;

	case NOTIFY_DROPITEMFIN_COUNTCHANGED:
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;

		ip  = (int *)cp;
		*ip = (int)sV2;
		cp += 4;

		iRet = player->socket->write(cData, 12);
		break;

	case NOTIFY_DROPITEMFIN_ERASEITEM:
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;

		ip  = (int *)cp;
		*ip = (int)sV2;
		cp += 4;

		iRet = player->socket->write(cData, 12);
		break;

	case NOTIFY_CANNOTGIVEITEM:
	case NOTIFY_GIVEITEMFIN_COUNTCHANGED:
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;

		ip  = (int *)cp;
		*ip = (int)sV2;
		cp += 4;

		memcpy(cp, pString, 20);
		cp += 20;

		iRet = player->socket->write(cData, 32);
		break;

	case NOTIFY_GIVEITEMFIN_ERASEITEM:
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;

		ip  = (int *)cp;
		*ip = (int)sV2;
		cp += 4;

		memcpy(cp, pString, 20);
		cp += 20;

		iRet = player->socket->write(cData, 32);
		break;

	case NOTIFY_ENEMYKILLREWARD:
		dwp  = (uint32_t *)cp;
		*dwp = (uint32_t)player->m_iExp;
		cp += 4;
		dwp  = (uint32_t *)cp;
		*dwp = (uint32_t)player->m_iEnemyKillCount;
		cp += 4;
		memcpy(cp, m_pClientList[sV1]->m_cCharName, 10);
		cp += 10;
		memcpy(cp, m_pClientList[sV1]->m_cGuildName, 20);
		cp += 20;
		sp  = (short *)cp;
		*sp = (short)m_pClientList[sV1]->m_iGuildRank;
		cp += 2;
		sp  = (short *)cp;
		*sp = (short)player->m_iWarContribution;
		cp += 2;

		iRet = player->socket->write(cData, 48);
		break;

	case NOTIFY_PKCAPTURED:

		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV2;
		cp += 2;
		memcpy(cp, pString, 10);
		cp += 10;
		dwp  = (uint32_t *)cp;
		*dwp = (uint32_t)player->m_iRewardGold;
		cp += 4;
		dwp  = (uint32_t *)cp;
		*dwp = player->m_iExp;
		cp += 4;

		iRet = player->socket->write(cData, 28);
		break;

	case NOTIFY_PKPENALTY:

		dwp  = (uint32_t *)cp;
		*dwp = (uint32_t)player->m_iExp;
		cp += 4;
		dwp  = (uint32_t *)cp;
		*dwp = (uint32_t)player->GetStr();
		cp += 4;
		dwp  = (uint32_t *)cp;
		*dwp = (uint32_t)player->m_iVit;
		cp += 4;
		dwp  = (uint32_t *)cp;
		*dwp = (uint32_t)player->GetDex();
		cp += 4;
		dwp  = (uint32_t *)cp;
		*dwp = (uint32_t)player->GetInt();
		cp += 4;
		dwp  = (uint32_t *)cp;
		*dwp = (uint32_t)player->GetMag();
		cp += 4;
		dwp  = (uint32_t *)cp;
		*dwp = (uint32_t)player->m_iCharisma;
		cp += 4;
		dwp  = (uint32_t *)cp;
		*dwp = (uint32_t)player->m_iPKCount;
		cp += 4;

		iRet = player->socket->write(cData, 38);
		break;

	case NOTIFY_TRAVELERLIMITEDLEVEL:
	case NOTIFY_LIMITEDLEVEL:

		dwp  = (uint32_t *)cp;
		*dwp = (uint32_t)player->m_iExp;
		cp += 4;
		iRet = player->socket->write(cData, 10);
		break;

	case NOTIFY_ITEMRELEASED:
	case NOTIFY_ITEMLIFESPANEND:
		sp  = (short *)cp;
		*sp = (short)sV1;
		cp += 2;
		sp = (short *)cp;
		*sp = (short)sV2;
		cp += 2;

		iRet = player->socket->write(cData, 10);
		break;





	case NOTIFY_QUERY_DISMISSGUILDREQPERMISSION:
	case NOTIFY_QUERY_JOINGUILDREQPERMISSION:
	case NOTIFY_CANNOTJOINMOREGUILDSMAN:

		memcpy(cp, m_pClientList[iFromH]->m_cCharName, 10);
		cp += 10;

		iRet = player->socket->write(cData, 16);
		break;

	case COMMONTYPE_JOINGUILDAPPROVE:
		if (m_pClientList[iFromH] != NULL)
			memcpy(cp, m_pClientList[iFromH]->m_cGuildName, 20);
		else memcpy(cp, "?", 1);
		cp += 20;

		sp  = (short *)cp;
		*sp = GUILDSTARTRANK;
		cp += 2;

		iRet = player->socket->write(cData, 28);
		break;

	case COMMONTYPE_JOINGUILDREJECT:
	case COMMONTYPE_DISMISSGUILDAPPROVE:
	case COMMONTYPE_DISMISSGUILDREJECT:
		if (m_pClientList[iFromH] != NULL)
			memcpy(cp, m_pClientList[iFromH]->m_cGuildName, 20);
		else memcpy(cp, "?", 1);
		cp += 20;

		sp  = (short *)cp;
		*sp = GUILDSTARTRANK;
		cp += 2;

		iRet = player->socket->write(cData, 28);
		break;

	case NOTIFY_GUILDDISBANDED:

		memcpy(cp, pString, 20);
		cp += 20;

		memcpy(cp, player->m_cLocation, 10);
		cp += 10;

		iRet = player->socket->write(cData, 36);
		break;

	case NOTIFY_FIGHTZONERESERVE:
		ip = (int *)cp;
		*ip = (int )sV1;
		cp += 4;

		iRet = player->socket->write(cData, 10);
		break; 

	case NOTIFY_NOGUILDMASTERLEVEL:
		iRet = player->socket->write(cData, 6);
		break;

	case NOTIFY_CANNOTBANGUILDMAN:
		iRet = player->socket->write(cData, 6);
		break;

	case NOTIFY_CANNOTRECALL:
	case NOTIFY_CANNOTCRUSADE:
		Push(cp, (uint8_t)sV1);
		iRet = player->socket->write(cData, 7);
		break;

	case NOTIFY_FORCERECALLTIME:
		sp = (short *)cp ;
		*sp = (short )sV1;
		cp += 2;

		iRet = player->socket->write(cData, 8);
		break;

	case NOTIFY_TELEPORT_REJECTED:
		sp  = (short *)cp;
		*sp = (short)sV1;
		cp += 2;
		sp = (short *)cp;
		*sp = (short)sV2;
		cp += 2;
		iRet = player->socket->write(cData, 10);
		break;

	case NOTIFY_SPELL_SKILL:
		for (i = 0; i < MAXMAGICTYPE; i++) {
			*cp = player->m_cMagicMastery[i];
			cp++;
		}

		for (i = 0; i < MAXSKILLTYPE; i++) {
			*cp = player->m_cSkillMastery[i];
			cp++;
		}

		iRet = player->socket->write(cData, 6 + MAXMAGICTYPE + MAXSKILLTYPE);
		break;

	case NOTIFY_REPAIRALLPRICE:
		Push(cp, (uint32_t)sV1);
		iRet = player->socket->write(cData, 10);
		break;

#ifdef TitleHG
	case NOTIFY_CHANGETITLE:
		memcpy(cp, pString, 11);
		cp += 11;
		iRet = player->socket->write(cData, 17);
		break;

	case NOTIFY_REQTITLEANSWER: 
		{
			int iNumPoints = (uint32_t)sV1;
			int iReturnID = (uint32_t)sV2;
			int iCrusadeJob = (uint32_t)sV3;
			int iRank = (uint32_t)sV4;

			ip = (int *)cp;
			*ip = iReturnID;
			cp += 4;

			ip = (int *)cp;
			*ip = iNumPoints;
			cp += 4;

			ip = (int *)cp;
			*ip = iRank;
			cp += 4;

			ip = (int *)cp;
			*ip = iCrusadeJob;
			cp += 4;

			/ *ip = (int *)cp;
			*ip = player->iReturnID;
			cp += 4;

			ip = (int *)cp;
			*ip = player->iNumPoints;
			cp += 4;

			ip = (int *)cp;
			*ip = player->iRank;
			cp += 4;

			ip = (int *)cp;
			*ip = player->iCrusadeJob;
			cp += 4;* /

			memset(cp, 0, 21);
			memcpy(cp, pString, strlen(pString)+1);
			cp += 21;

			iRet = player->socket->write(cData, 43);
			break;
		}

	case NOTIFY_ADDTITLE: 
		{
			int iNumPoints = (uint32_t)sV1;
			int iNextRankPoints = (uint32_t)sV2;
			int iRank = (uint32_t)sV3;
			int iTitleIndex = (uint32_t)sV4;
			char cActiveTitle = (char)sV5;

			memset(cp, 0, 21);
			memcpy(cp, pString, strlen(pString)+1);
			cp += 21;

			ip = (int *)cp;
			*ip = iNumPoints;
			cp += 4;

			ip = (int *)cp;
			*ip = iNextRankPoints;
			cp += 4;

			ip = (int *)cp;
			*ip = iRank;
			cp += 4;

			ip = (int *)cp;
			*ip = iTitleIndex;
			cp += 4;

			*cp = cActiveTitle;
			cp++;

			/ *ip = (int *)cp;
			*ip = player->iNumPoints;
			cp += 4;

			ip = (int *)cp;
			*ip = player->iNextRankPoints;
			cp += 4;

			ip = (int *)cp;
			*ip = player->iRank;
			cp += 4;

			ip = (int *)cp;
			*ip = player->iTitleIndex;
			cp += 4;

			*cp = player->cActiveTitle;
			cp++;* /

			iRet = player->socket->write(cData, 44);
			break;
		}

	case NOTIFY_TITLELEVELUP:
		{
			int iNumPoints = (uint32_t)sV1;
			int iNextRankPoints = (uint32_t)sV2;
			int iRank = (uint32_t)sV3;
			int iTitleIndex = (uint32_t)sV4;
			
			memset(cp, 0, 21);
			memcpy(cp, pString, strlen(pString)+1);
			cp += 21;

			ip = (int *)cp;
			*ip = iNumPoints;
			cp += 4;

			ip = (int *)cp;
			*ip = iNextRankPoints;
			cp += 4;

			ip = (int *)cp;
			*ip = iRank;
			cp += 4;

			ip = (int *)cp;
			*ip = iTitleIndex;
			cp += 4;

			/ *ip = (int *)cp;
			*ip = player->iNumPoints;
			cp += 4;

			ip = (int *)cp;
			*ip = player->iNextRankPoints;
			cp += 4;

			ip = (int *)cp;
			*ip = player->iRank;
			cp += 4;

			ip = (int *)cp;
			*ip = player->iTitleIndex;
			cp += 4;* /

			iRet = player->socket->write(cData, 43);
			break;
		}
#endif
		// Gladiator Arena xRisenx
		/ *case NOTIFY_ARENASTATUS:
			ip  = (int *)cp;
			*ip = sV1;
		    cp += 4;

		    ip  = (int *)cp;
		    *ip = sV2;
		    cp += 4;

		    ip  = (int *)cp;
		    *ip = sV3;
		    cp += 4;

		    memcpy(cp,pString,21);
		    cp += 21;

		    iRet = m_pClientList[iToH]->socket->write(cData,39);
			iRet = player->socket->write(cData,39);
		    break;* /
		// Gladiator Arena xRisenx
		// Monster kill event xRisenx
		case NOTIFY_NPCHUNTON:
		case NOTIFY_NPCHUNTOFF:
		iRet = player->socket->write(cData, 6);
		break;
		case NOTIFY_NPCHUNTWINNER:
		ip = (int *)cp;
		*ip = player->m_iContribution;
		cp += 4;
		ip = (int *)cp;
		*ip = player->m_reputation;
		cp += 4;
		iRet = player->socket->write(cData, 14);
		break;
		// Monster kill event xRisenx

	}

	switch (iRet) {
	case XSOCKEVENT_QUENEFULL:
	case XSOCKEVENT_SOCKETERROR:
	case XSOCKEVENT_CRITICALERROR:
	case XSOCKEVENT_SOCKETCLOSED:

		//DeleteClient(iToH, true, true);
		return;
	}*/
}

uint8_t GServer::iSetSide(Client * client)
{
	if (client == 0) return 0;
	
	client->side = NEUTRAL; 
	client->m_bIsOnTown = false;
	client->m_bIsHunter = false;
	client->neutral = false;

	if (client->faction.length() == 0 || client->faction[0] == '\0')
	{
		//fail safe set to trav?
		client->faction = "NONE";
	}
	if (client->faction[0] == '1')
		client->faction = "aresden";

	if (client->faction.length() <= 3)
	{
		//something borked. check if "are" or "elv" matches
		if (client->faction[0] == 'a')
		{
			client->faction = "aresden";
		}
		else if (client->faction[0] == 'e')
		{
			client->faction = "elvine";
		}
	}

	if (client->faction.substr(3, 6) == "hunter")
	{
		client->m_bIsHunter = true;
	}

	if (client->mapName == "bisle")
	{
		client->m_bIsHunter = false;
	}

	if (client->faction.substr(0, 3) == "are")
	{
		client->side = ARESDEN;
		if (client->mapName == sideMap[ARESDEN])
		{
			client->m_bIsOnTown = true;
		}
	}
	else if (client->faction.substr(0, 3) == "are")
	{
		client->side = ELVINE;
		if (client->mapName == sideMap[ELVINE])
		{
			client->m_bIsOnTown = true;
		}
	}
	else
	{
		client->side = NEUTRAL;
		client->m_bIsOnTown = true;
		if (client->mapName == sideMap[NEUTRAL])
		{
			client->m_bIsOnTown = true;
		}
	}

	client->SetSideFlag((Side) client->side);

	if (client->mapName == sideMap[NEUTRAL])
	{
		client->neutral = true;
	}

	if ((client->mapName.substr(0, 6) == "bsmith") ||
		(client->mapName.substr(0, 7) == "gldhall") ||
		(client->mapName.substr(0, 5) == "gshop") ||
		(client->mapName.substr(0, 5) == "cityhall") )
		client->m_bIsOnShop = true;
	else
		client->m_bIsOnShop = false;


	if ((client->mapName.substr(0, 5) == "wrhus") ||
		(client->mapName.substr(3, 5) == "wrhus") ||
		(client->mapName.substr(0, 5) == "cityhall"))
		client->m_bIsOnWarehouse = true;
	else
		client->m_bIsOnWarehouse = false;


	if((client->mapName.substr(0, 6) == "wzdtwr") ||
		(client->mapName.substr(0, 6) == "cityhall") )
		client->m_bIsOnTower = true;
	else
		client->m_bIsOnTower = false;

	return client->side;
}

Item * GServer::_iGetArrowItemIndex(Client * client)
{
	if (client == 0) return 0;

	for (std::vector<ItemWrap*>::iterator iter = client->itemList.begin(); iter != client->itemList.end(); ++iter)
		if ((*iter)->_item && ((*iter)->_item->m_cItemType == ITEMTYPE_ARROW) && ((*iter)->_item->m_dwCount > 0))
			return (*iter)->_item;

	return 0;
}

void GServer::InitPlayerData(shared_ptr<Client> client)
{
	try
	{
		{
			//initial stuff. no point sending char data if they aren't allowed to login, or the data needs adjusting
			if ((client->x == -1) || (client->y == -1))
			{
				if (!client->map->GetInitialPoint(&client->x, &client->y, client->faction))
				{
					DeleteClient(client, false, true);
					return;
				}
			}

			//if m_bIsInitComplete is already set to true, it means the object is being resumed so use their old spot instead of finding a new one
			//calling SetOwner further below should have no negative effect since it's effectively just replacing the var with the same value
			if (!client->m_bIsInitComplete)
				client->map->bGetEmptyPosition(client->x, client->y, client);

			if(client->playerKillCount > 0)
				client->SetStatusFlag(STATUS_PK, true);

			iSetSide(client.get());

			client->m_iNextLevelExp = m_iLevelExpTable[client->level + 1];

// 			CalcTotalItemEffect(iClientH, -1, true); //false
 			iCalcTotalWeight(client);
// 			_SendQuestContents(iClientH);
// 
// 			player->ValidateSkills(true);
// 			CheckMagicInt(iClientH);
// 			

			//SendNotifyMsg(NULL, iClientH, NOTIFY_REQDEF, m_pClientList[iClientH]->m_iDefenseRatio, NULL, NULL, NULL);
// 			if ((cGuildStatus == 0) && (memcmp(player->m_cGuildName, "NONE", 4) != 0)) {
// 				ZeroMemory(player->m_cGuildName, sizeof(player->m_cGuildName));
// 				strcpy(player->m_cGuildName, "NONE");
// 				player->m_iGuildRank = GUILDRANK_NONE;
// 				player->m_iGuildGUID = -1;
// 				player->m_guild = NULL;
// 
// 				//SendNotifyMsg(NULL, iClientH, NOTIFY_GUILDDISBANDED, NULL, NULL, NULL, player->m_cGuildName);
// 			}

			client->Equipped.Arrow = _iGetArrowItemIndex(client.get());
			//CalcTotalItemEffect(iClientH, -1, false);

		}
		{
			StreamWrite sw;
			sw.WriteInt(MSGID_RESPONSE_INITPLAYER);
			sw.WriteShort(MSGTYPE_CONFIRM);

			client->mutsocket.lock();
			if (client->socket)
				client->socket->write(sw.data, sw.position);
			client->mutsocket.unlock();
		}
		{
			StreamWrite sw;
			sw.WriteInt(MSGID_PLAYERCHARACTERCONTENTS);
			sw.WriteShort(MSGTYPE_CONFIRM);

			sw.WriteInt(uint32_t(client->health));//TODO: figure out. use 64 or 32 bit for values client side?
			sw.WriteInt(uint32_t(client->mana));
			sw.WriteInt(uint32_t(client->m_iSP));

			sw.WriteInt(client->m_iDefenseRatio);
			sw.WriteInt(client->m_iHitRatio);
			sw.WriteInt(client->level);

			sw.WriteInt(client->GetStr());
			sw.WriteInt(client->GetInt());
			sw.WriteInt(client->GetVit());
			sw.WriteInt(client->GetDex());
			sw.WriteInt(client->GetMag());
			sw.WriteInt(client->GetAgi());

			sw.WriteShort(0);//LU_Pool
			sw.WriteInt(uint32_t(client->experience));//TODO: make 64bit in client? shouldn't matter for a 150 server though
			sw.WriteInt(client->m_iEnemyKillCount);
			sw.WriteInt(client->playerKillCount);
			sw.WriteInt(client->m_iRewardGold);

			sw.WriteString(client->faction, 10);//town
			sw.WriteString(string(""), 20);//guild name
			sw.WriteInt(client->guildRank);
			sw.WriteByte(client->superAttack);
			sw.WriteInt(client->arenaNumber);
			sw.WriteInt(client->m_iEnemyKillTotalCount);

			client->mutsocket.lock();
			if (client->socket)
				client->socket->write(sw.data, sw.position);
			client->mutsocket.unlock();
		}
		{
			StreamWrite sw = StreamWrite();

			sw.WriteInt(MSGID_PLAYERITEMLISTCONTENTS);
			sw.WriteShort(0);

			//big for loop for items
			//sw.WriteByte(client->m_pItemList.size());//item count // make itemlist an stl container
			sw.WriteByte(0);//bag items
			sw.WriteByte(0);//bank items

			//magic skill mastery. why it's in items...

			for (uint8_t i = 0; i < MAXMAGICTYPE; ++i)
			{
				sw.WriteByte(client->magicMastery[i]);
			}
			for (uint8_t i = 0; i < MAXSKILLTYPE; ++i)
			{
				sw.WriteByte(client->m_cSkillMastery[i]);
			}

			client->mutsocket.lock();
			if (client->socket)
				client->socket->write(sw.data, sw.position);
			client->mutsocket.unlock();
		}
		{
			StreamWrite sw = StreamWrite();

			sw.WriteInt(MSGID_RESPONSE_INITDATA);
			sw.WriteShort(MSGTYPE_CONFIRM);
			sw.WriteShort(uint16_t(client->charid));//TODO: needs to be at least 32bit in client based on how we're making use of IDs

			//quick explanation. maybe fix to not be so retarded?
			// X - 19 = "base" coord - client takes this and does + 7 to get far left of screen
			// client also takes this and does +19 to get normal coord.
			// (x+7)*32 is the view distance (aka far left) which is -12 from X
			// now why in the fuck does it go -19 and can that be improved upon
			sw.WriteShort(client->x - 14 - 5);
			sw.WriteShort(client->y - 12 - 5);
			sw.WriteShort(client->Type());
			sw.WriteShort(client->m_sAppr1);
			sw.WriteShort(client->m_sAppr2);
			sw.WriteShort(client->m_sAppr3);
			sw.WriteShort(client->m_sAppr4);
			sw.WriteInt(client->m_iApprColor);
			sw.WriteShort(client->m_sHeadApprValue);
			sw.WriteShort(client->m_sBodyApprValue);
			sw.WriteShort(client->m_sArmApprValue);
			sw.WriteShort(client->m_sLegApprValue);

			if (client->playerKillCount > 0)
				client->SetStatusFlag(STATUS_PK, true);

			sw.WriteInt(client->status);
			sw.WriteString(client->map->name, 10);
			sw.WriteString(client->map->factionName, 10);//map location side name


			if (client->map->fixedDay == true) //day(1) or night(0)?
				sw.WriteByte(1);
			else
				sw.WriteByte(m_cDayOrNight);

			if (client->map->fixedDay) //sunny(0) or raining(1-3) or snowing(4-6)?
				sw.WriteByte(WEATHER_SUNNY);
			else 
				sw.WriteByte(client->map->weather);

			sw.WriteInt(client->contribution);

			if(!client->observerMode && !client->IsEthereal())
			{
				client->map->SetOwner(client, client->x, client->y);
			}

			sw.WriteByte((client->observerMode)?1:0);
			sw.WriteInt(client->m_reputation);
			sw.WriteInt(uint32_t(client->health));
			sw.WriteInt(client->m_iLucky);

			if (client->m_bIsOnShop && m_iCrusadeWinnerSide == client->side)//crusade shop price change
				sw.WriteByte(-10);
			else
				sw.WriteByte(0);

//			uint16_t * total = (uint16_t*)(sw.data+sw.position);
			uint64_t oldpos = sw.position;
			uint16_t total = 0;

			sw.WriteShort(0);//total placeholder

			Tile * pTileSrc = client->map->GetTile(client->x - 12, client->y - 9);
			//Tile * pTileSrc = (Tile *)(client->pMap->_tile + (client->m_sX - 12) + (client->m_sY - 9)*client->pMap->sizeY);

			for (int y = 0; y < 19; ++y)
			{
				for (int x = 0; x < 25; ++x)
				{
					if (WriteTileData(sw, client.get(), pTileSrc, x, y))
						total++;
				}
			}
			char * temp = sw.data + oldpos;
			*((short *)temp) = total;
			client->m_bIsInitComplete = true;

			client->mutsocket.lock();
			if (client->socket)
				client->socket->write(sw);
			client->mutsocket.unlock();

			SendEventToNearClient_TypeA(client.get(), MSGID_MOTION_EVENT_CONFIRM, 0, 0, 0);
		}
	}
	catch(int32_t reason)
	{
		if (reason == -192)
		{
			consoleLogger->error("(data == 0)");
		}
		else if (reason == -193)
		{
			consoleLogger->error("(position+a > size)");
		}
	}
	catch (std::exception& e)
	{
		logger->fatal(Poco::format("InitPlayerData() Exception: %s", (string)e.what()));
	}
}

void GServer::ClientMotionHandler(shared_ptr<Client> client, shared_ptr<MsgQueueEntry> msg)
{
	if (!msg->client->m_bIsInitComplete) return;
	if (msg->client->_dead) return;
	if (msg->client->observerMode) return;

	StreamRead sr = StreamRead(msg->data, msg->size);

	uint32_t command = sr.ReadInt();

	uint16_t dX, sX = sr.ReadShort();
	uint16_t dY, sY = sr.ReadShort();

	uint8_t direction = sr.ReadByte();

	int32_t clienttime = sr.ReadInt();//TODO: make 64bit client side

	int16_t magictype;
	int16_t attacktype;
	uint64_t targetid;
	int8_t iRet;
	if (command == MSGID_MOTION_MAGIC)
		magictype = sr.ReadShort();
	else if (command == MSGID_MOTION_ATTACK || command == MSGID_MOTION_ATTACKMOVE)
	{
		dX = sr.ReadShort();
		dY = sr.ReadShort();
		attacktype = sr.ReadShort();
		targetid = sr.ReadShort();//might need to expand to at least 32bit client side
	}

	switch (command)
	{
	case MSGID_MOTION_STOP:
		iRet = iClientMotion_Stop_Handler(msg->client, sX, sY, direction);
		if (iRet == 1) {
			SendEventToNearClient_TypeA(msg->client.get(), command, 0, 0, 0);
		}
		else if (iRet == 2) SendObjectMotionRejectMsg(msg->client.get());
		break;

	case MSGID_MOTION_RUN:
	case MSGID_MOTION_MOVE:
		if (!bCheckClientMoveFrequency(client.get(), (command == MSGID_MOTION_RUN)))
			iRet = 3;
		else
			iRet = iClientMotion_Move_Handler(client, sX, sY, direction, (command == MSGID_MOTION_RUN));

		if (iRet == 1) {
			SendEventToNearClient_TypeA(client.get(), command, 0, 0, 0);
		}		
		else if (iRet == 2 || iRet == 3) {
			SendObjectMotionRejectMsg(client.get());
		}

		if ((client->health <= 0)) client->KilledHandler(nullptr, 1);
		break;

	case MSGID_MOTION_DAMAGEMOVE:
		iRet = iClientMotion_Move_Handler(client, sX, sY, direction, false);
		if (iRet == 1) {
			SendEventToNearClient_TypeA(client.get(), command, client->m_iLastDamage, 0, 0);
		} 
		if ((client != nullptr) && (client->health <= 0)) client->KilledHandler(nullptr, 1);
		break;

	case MSGID_MOTION_ATTACKMOVE:
		iRet = iClientMotion_Move_Handler(client, sX, sY, direction, false);
		if ((iRet == 1) && (client != nullptr)) {
			SendEventToNearClient_TypeA(client.get(), command, 0, 0, 0);

			attacktype = 1;
			iClientMotion_Attack_Handler(client, client->x, client->y, dX, dY, attacktype, direction, targetid, false, true);
		}
		if ((client != nullptr) && (client->health <= 0)) client->KilledHandler(nullptr, 1);

		bCheckClientAttackFrequency(client.get());
		break;

	case MSGID_MOTION_ATTACK:
		_CheckAttackType(client.get(), attacktype);

		if (!bCheckClientAttackFrequency(client.get()))
			iRet = 2;
		else
			iRet = iClientMotion_Attack_Handler(client, sX, sY, dX, dY, attacktype, direction, targetid);
		if (iRet == 1) {
			if (attacktype >= 20) {
				client->superAttack--;

				if (client->superAttack < 0) client->superAttack = 0;
			}

			SendEventToNearClient_TypeA(client.get(), command, dX, dY, attacktype);
		}
		else if (iRet == 2) SendObjectMotionRejectMsg(client.get());

		break;

	case MSGID_MOTION_GETITEM:
		iRet = iClientMotion_GetItem_Handler(client, sX, sY, direction);
		if (iRet == 1) {
			SendEventToNearClient_TypeA(client.get(), command, 0, 0, 0);
		}
		else if (iRet == 2) SendObjectMotionRejectMsg(client.get());
		break;

	case MSGID_MOTION_MAGIC:
		iRet = iClientMotion_Magic_Handler(client, sX, sY, direction);

		if (iRet == 1) {
			SendEventToNearClient_TypeA(client.get(), command, magictype,(short) 10, 0);
			client->m_hasPrecasted = true;
		}
		else if (iRet == 2) SendObjectMotionRejectMsg(client.get());
		break;
	}
}

void GServer::SendObjectMotionRejectMsg(Client * client)
{
	if(!client) return;
	if(client->rejectedMove) return;

	client->rejectedMove = true;
	client->m_resetMoveFreq = true; 

	StreamWrite sw(12);

	sw.WriteInt(MSGID_RESPONSE_MOTION);
	sw.WriteShort(OBJECTMOTION_REJECT);
	sw.WriteShort(client->x);
	sw.WriteShort(client->y);

	client->mutsocket.lock();
	if (client->socket)
		client->socket->write(sw.data, sw.position);
	client->mutsocket.unlock();
}

int GServer::iClientMotion_Stop_Handler(shared_ptr<Client> client, uint16_t sX, uint16_t sY, uint8_t cDir)
{
	shared_ptr<Unit> owner;

	if (!client) return 0;
	if ((cDir <= 0) || (cDir > 8))       return 0;
	if (client->_dead == true) return 0;
	if (client->m_bIsInitComplete == false) return 0;


	if ((sX != client->x) || (sY != client->y)) return 2;


	if (client->skillInUse[19] == true) {
		owner = client->map->GetOwner(sX, sY);
		if (owner != 0) {
			//TODO: pretend corpse and someone is on top of you - make it so you appear on a tile nearby instead
			DeleteClient(client, true, false);
			return 0;
		}
		client->map->ClearOwner(sX, sY);
		client->map->ClearDeadOwner(sX, sY);
		client->map->SetOwner(client, sX, sY);
	}

	ClearSkillUsingStatus(client.get());

	if (client->map != 0) {
		client->map->IncPlayerActivity(client.get());
	}

	client->direction = cDir;

	StreamWrite sw(10);

	sw.WriteInt(MSGID_RESPONSE_MOTION);
	sw.WriteShort(OBJECTMOTION_CONFIRM);

	client->mutsocket.lock();
	if (client->socket)
		client->socket->write(sw.data, sw.position);
	client->mutsocket.unlock();

	return 1;
}

// Calculates the number of players within a given radius.
int GServer::getPlayerNum(Map * pMap, short dX, short dY, char cRadius)
{
	int x, y, ret;
	class Tile * pTile;

	//if ((cMapIndex < 0) || (cMapIndex > MAXMAPS)) return 0;
	//if (maplist[cMapIndex] == NULL) return 0;

	ret = 0;
	for (x = dX - cRadius; x <= dX + cRadius; x++)
		for (y = dY - cRadius; y <= dY + cRadius; y++) {
			if ((x < 0) || (x >= pMap->sizeX) ||
				(y < 0) || (y >= pMap->sizeY)) {
			}
			else {
				pTile = pMap->GetTile(x, y);
				//pTile = (class Tile *)(pMap->_tile + x + y*pMap->sizeY);
				if ((pTile->owner != nullptr) && (pTile->m_cOwnerType == OWNERTYPE_PLAYER))
					ret++;
			}
		}

	return ret;
}

void GServer::SendEventToNearClient_TypeA(Unit * owner, uint32_t msgid, uint32_t sV1, uint32_t sV2, uint32_t sV3)
{
	uint32_t * pstatus, status = 0, statusdummy;
	bool    bOwnerSend;

	pstatus = &statusdummy;

	StreamWrite sw;

	sw.WriteInt(msgid);

	if (owner->IsPlayer())
	{
		Client * player = static_cast<Client*>(owner);

		switch (msgid) {
		case MSGID_MOTION_NULL:
		case MSGID_MOTION_DAMAGE:
		case MSGID_MOTION_DYING:
			bOwnerSend = true;
			break;
		default:
			bOwnerSend = false;
			break;
		}

		switch (msgid) 
		{
		case MSGID_MOTION_MAGIC:
		case MSGID_MOTION_DAMAGE:
		case MSGID_MOTION_DAMAGEMOVE:
		case MSGID_MOTION_DYING:
			sw.WriteShort(uint16_t(owner->handle + 30000));
			sw.WriteByte(owner->direction);
			sw.WriteShort(sV1);
			sw.WriteByte(uint8_t(sV2));

			if(msgid == MSGID_MOTION_DYING)
			{
				sw.WriteShort(player->x);
				sw.WriteShort(player->y);
			}
			break;

		case MSGID_MOTION_ATTACK:
		case MSGID_MOTION_ATTACKMOVE:
			sw.WriteShort(uint16_t(owner->handle + 30000));
			sw.WriteByte(owner->direction);
			sw.WriteShort(player->x);
			sw.WriteShort(player->y);
			sw.WriteByte(uint8_t(sV3));
			break;

		case MSGID_MOTION_EVENT_CONFIRM:
		case MSGID_MOTION_EVENT_REJECT:
		case MSGID_MOTION_NULL:
		default:
			sw.WriteShort(uint16_t(owner->handle));
			sw.WriteShort(player->x);
			sw.WriteShort(player->y);
			sw.WriteShort(player->Type());
			sw.WriteByte(owner->direction);
			sw.WriteString(player->name, 10);

			sw.WriteShort(player->m_sAppr1);
			sw.WriteShort(player->m_sAppr2);
			sw.WriteShort(player->m_sAppr3);
			sw.WriteShort(player->m_sAppr4);
			sw.WriteInt(player->m_iApprColor);

			sw.WriteShort(player->m_sHeadApprValue);
			sw.WriteShort(player->m_sBodyApprValue);
			sw.WriteShort(player->m_sArmApprValue);
			sw.WriteShort(player->m_sLegApprValue);

			pstatus = (uint32_t*)(sw.data+sw.position);

			sw.WriteInt(player->status);

			sw.WriteByte((uint8_t)((msgid != MSGID_MOTION_NULL || !player->_dead) ? 0 : 1));
			break;
		}

		if(bOwnerSend)
		{
			player->mutsocket.lock();
			if (player->socket)
				player->socket->write(sw.data, sw.size);
			player->mutsocket.unlock();
		}

		if(player->IsEthereal())
		{
			return;
		}

		gate->mutclientlist.lock_shared();
		for (shared_ptr<Client> client : clientlist)
		{
			if ((client->handle != player->handle) && (client->m_bIsInitComplete) && (player->map == client->map)
				&& ((client->x > player->x - 15) && (client->x < player->x + 15)//screen res location
				  && (client->y > player->y - 13) && (client->y < player->y + 13)))
			{
				client->mutsocket.lock();
				if(_bGetIsPlayerHostile(player,client.get()) && client->m_iAdminUserLevel == 0)
					*pstatus = player->status & STATUS_ENEMYFLAGS;
// 				else
// 					*pstatus = status;
				if (client->socket)
					client->socket->write(sw.data, sw.size);
				client->mutsocket.unlock();
			}
		}
		gate->mutclientlist.unlock_shared();
	}
	else {
		Npc * npc = static_cast<Npc*>(owner);

		switch (msgid)
        {
        case MSGID_MOTION_MAGIC:
        case MSGID_MOTION_DAMAGE:
        case MSGID_MOTION_DAMAGEMOVE:
        case MSGID_MOTION_DYING:
			sw.WriteShort(uint16_t(npc->handle + 40000));
			sw.WriteByte(uint8_t(npc->direction));
			sw.WriteShort(uint16_t(sV1));
			sw.WriteByte(uint8_t(sV2));

            if(msgid == MSGID_MOTION_DYING)
            {
				sw.WriteShort(uint16_t(npc->x));
				sw.WriteShort(uint16_t(npc->y));
            }
            break;

        case MSGID_MOTION_ATTACK:
        case MSGID_MOTION_ATTACKMOVE:
			sw.WriteShort(uint16_t(npc->handle + 40000));
			sw.WriteByte(uint8_t(npc->direction));
			sw.WriteShort(uint16_t(npc->x));
			sw.WriteShort(uint16_t(npc->y));
			sw.WriteShort(uint16_t(sV3));
            break;

		case MSGTYPE_CONFIRM:
		case MSGTYPE_REJECT:
		case OBJECTNULLACTION:
		default:
			sw.WriteShort(uint16_t(npc->handle + 10000));
			sw.WriteShort(uint16_t(npc->x));
			sw.WriteShort(uint16_t(npc->y));
			sw.WriteShort(uint16_t(npc->Type()));
			sw.WriteByte(uint8_t(npc->direction));
			sw.WriteShort(uint16_t(npc->appr2));

			pstatus = (uint32_t*)(sw.data+sw.position);
			sw.WriteInt(uint32_t(npc->status));

			sw.WriteByte((uint8_t)((msgid != MSGID_MOTION_NULL || !npc->_dead) ? 0 : 1));
			break;
		}


		gate->mutclientlist.lock_shared();
		for (shared_ptr<Client> client : clientlist)
		{
			if (/*(client->m_handle != npc->m_handle) && */(client->m_bIsInitComplete) && (npc->map == client->map)
				&& ((client->x > npc->x - 15) && (client->x < npc->x + 15)//screen res location
				&& (client->y > npc->y - 12) && (client->y < npc->y + 12)))
			{
				client->mutsocket.lock();
// 				if(_bGetIsPlayerHostile(player,client.get()) && client->m_iAdminUserLevel == 0)//can add flag modifiers here
// 					*pstatus = status & STATUS_ENEMYFLAGS;
// 				else
// 					*pstatus = status;
				if (client->socket)
					client->socket->write(sw.data, sw.size);
				client->mutsocket.unlock();
			}
		}
		gate->mutclientlist.unlock_shared();
	}
}

void GServer::SendEventToNearClient_TypeB(uint32_t msgid, uint16_t msgtype, Map * mapIndex, uint16_t sX, uint16_t sY, uint32_t sV1, uint32_t sV2, uint32_t sV3, uint32_t sV4)
{
	int i, iRet, iShortCutIndex;
	char  * cp, cData[100];
	uint32_t * dwp, dwTime;
	uint16_t * wp;
	short * sp;
	bool bFlag;
	char  cKey ;
	StreamWrite sw;

	cKey = (char)(rand() % 255) +1; 

	sw.WriteInt(msgid);
	sw.WriteShort(msgtype);

	sw.WriteShort(sX);
	sw.WriteShort(sY);

	sw.WriteInt(sV1);
	sw.WriteInt(sV2);
	sw.WriteInt(sV3);
	sw.WriteInt(sV4);

	dwTime = unixtime();

	bFlag = true;
	iShortCutIndex = 0;



	gate->mutclientlist.lock_shared();
	for (shared_ptr<Client> client : clientlist)
	{
		if ((client->m_bIsInitComplete) && (client->map == mapIndex)
			&& ((client->x > sX - 15) && (client->x < sX + 15)//screen res location
			&& (client->y > sY - 13) && (client->y < sY + 13)))
		{
			client->mutsocket.lock();
			if (client->socket)
				client->socket->write(sw.data, sw.size);
			client->mutsocket.unlock();
		}
	}
	gate->mutclientlist.unlock_shared();
}

int GServer::iClientMotion_GetItem_Handler(shared_ptr<Client> client, uint16_t sX, uint16_t sY, uint8_t cDir)
{
	uint32_t * dwp;
	uint16_t  * wp;
	short sRemainItemSprite, sRemainItemSpriteFrame;
	char  cData[100];
	uint32_t RemainItemColor;
	int   iRet, iEraseReq;
	Item * pItem;
	StreamWrite sw;

	if ((cDir <= 0) || (cDir > 8))       return 0;
	if (client->_dead == true) return 0;
	if (client->m_bIsInitComplete == false) return 0;


	if ((sX != client->x) || (sY != client->y)) return 2;

	if (client->map != 0) {
		client->map->IncPlayerActivity(client.get());
	}

	if(client->skillInUse[19]) {
		client->map->ClearOwner(sX, sY);
		client->map->SetOwner(client, sX, sY);
	}

	ClearSkillUsingStatus(client.get());

	pItem = client->map->pGetItem(sX, sY, &sRemainItemSprite, &sRemainItemSpriteFrame, &RemainItemColor); 
	if (pItem != 0) {

		if(!pItem->m_disabled && _bAddClientItemList(client, pItem, &iEraseReq)) {

			//_bItemLog(ITEMLOG_GET, iClientH, (int) -1, pItem);

			SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, COMMONTYPE_SETITEM, client->map,
				client->x, client->y,
				sRemainItemSprite, sRemainItemSpriteFrame, RemainItemColor);

			SendItemNotifyMsg(client, NOTIFY_ITEMOBTAINED, pItem, 0);
			if (iEraseReq == 1) delete pItem;
		}
		else 
		{

			client->map->bSetItem(sX, sY, pItem);

			SendItemNotifyMsg(client, NOTIFY_CANNOTCARRYMOREITEM, 0, 0);
		}
	}

	sw.WriteInt(MSGID_RESPONSE_MOTION);
	sw.WriteShort(OBJECTMOTION_CONFIRM);


	//TODO: make all these locks (bool)try_lock_for() to prevent perma locking -just in case?-
	client->mutsocket.lock();
	if (client->socket)
		client->socket->write(sw.data, sw.size);
	client->mutsocket.unlock();

	return 1;
}

int GServer::iClientMotion_Attack_Handler(shared_ptr<Client> client, uint16_t sX, uint16_t sY, uint16_t dX, uint16_t dY, int16_t wType, uint8_t cDir, uint64_t wTargetObjectID, bool bResponse, bool bIsDash)
{
	char cData[100];
	uint64_t dwTime;
	int32_t iRet, iExp, tdX = 0, tdY = 0;
	int16_t sOwner=0, sAbsX, sAbsY =0;
	char    cOwnerType, tgtDist = 1;
	bool    bNearAttack = false;
	int32_t iErr = 0, tX = 0, tY = 0, i = 1;
	int16_t sItemIndex;
	StreamWrite sw;
	
	if (client == nullptr) return 0;
	if ((cDir <= 0) || (cDir > 8))       return 0;
	if (client->m_bIsInitComplete == false) return 0;
	if (client->_dead == true) return 0;

	dwTime = unixtime();
	client->m_dwLastActionTime = dwTime;

	shared_ptr<Client> target;
	shared_ptr<Npc> npc;
	shared_ptr<Unit> maptarget;

	if (wTargetObjectID < MAXCLIENTS)
		target = GetClient(wTargetObjectID);
	else if ((wTargetObjectID > 10000) && (wTargetObjectID < (10000 + MAXNPCS)))
		npc = client->map->GetNpc(wTargetObjectID);
	else
	{
		sw.WriteInt(MSGID_RESPONSE_MOTION);
		sw.WriteShort(OBJECTMOTION_REJECT);
		client->socket->write(sw);
		return 0;//the 0 return is filler to get this to compile
	}

#ifndef NO_MSGSPEEDCHECK
	client->m_iAttackMsgRecvCount++;
	if (client->m_iAttackMsgRecvCount >= 2)
	{
		if (client->m_dwAttackLAT != 0)
		{			//68059813-68020153=39660
			if ((dwTime - client->m_dwAttackLAT) < 12) {
				logger->information(Poco::format("(!) Speed hack suspect(%s) - attack-lat(%?d)", client->name, dwTime - client->m_dwAttackLAT));
				DeleteClient(client->self.lock(), true, true);
				return 0;
			}
		}
		client->m_dwAttackLAT = dwTime;
		client->m_iAttackMsgRecvCount = 0;
	}
#endif

	if ((wTargetObjectID != 0) && (wType != 2)) {
		if (wTargetObjectID < MAXCLIENTS) {
			if (target != nullptr) {
				tdX = target->x;
				tdY = target->y;
				tgtDist = 1;
			}
		}
		else if ((wTargetObjectID > 10000) && (wTargetObjectID < (10000 + MAXNPCS))){
			if (npc != nullptr){
				tdX = npc->x;
				tdY = npc->y;
				tgtDist = (npc->m_cSize < 2) ? 1 : npc->m_cSize;
			}
		}
		maptarget = client->map->GetOwner(dX, dY);
		if (maptarget != nullptr){
			if (maptarget->handle == (wTargetObjectID - 10000)) {
				dX = tdX = maptarget->x;
				dY = tdY = maptarget->y;
				bNearAttack = false;
			}
			else if ((tdX == dX) && (tdY == dY)) {
				bNearAttack = false;
			}
			else if ((abs(tdX - dX) <= 1) && (abs(tdY - dY) <= 1)) {
				dX = tdX;
				dY = tdY;
				bNearAttack = true;
			}
		}
	}
	if ((dX < 0) || (dX >= client->map->sizeX) ||
		(dY < 0) || (dY >= client->map->sizeY)) return 0;


	if ((sX != client->x) || (sY != client->y)) return 2;


	if (client->map != nullptr) {
		client->map->IncPlayerActivity(client.get());
	}

	sAbsX = abs(sX - dX);
	sAbsY = abs(sY - dY);
	if ((wType != 2) && (wType < 20)) {
		if (client->Equipped.RightHand != nullptr) {
			if (client->Equipped.RightHand->m_sIDnum == ITEM_STORMBRINGER) {
				if (sAbsX >(3 + tgtDist) || sAbsY > (3 + tgtDist)) wType = 0;
			}
			else{
				if (sAbsX > tgtDist || sAbsY > tgtDist) wType = 0;
			}
		}
		else{
			if (sAbsX > tgtDist || sAbsY > tgtDist) wType = 0;
		}
	}

	if (client->skillInUse[19]) {
		client->map->ClearOwner(sX, sY);
		client->map->SetOwner(client, sX, sY);
	}

	ClearSkillUsingStatus(client.get());

	client->direction = cDir;

	iExp = 0;

	maptarget = client->map->GetOwner(dX, dY);

	if (maptarget != nullptr) {
		if ((wType != 0) && ((dwTime - client->m_dwRecentAttackTime) > 100)) {
			if (client->m_bIsInBuilding == false) {
				//sItemIndex = m_pClientList[iClientH]->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND]; // Commented because Battle Mages xRisenx
				// Battle Mages xRisenx
				if (client->itemEquipStatus[EQUIPPOS_RHAND] != -1){
					sItemIndex = client->itemEquipStatus[EQUIPPOS_RHAND];
				}
				else if (client->itemEquipStatus[EQUIPPOS_TWOHAND] != -1){
					sItemIndex = client->itemEquipStatus[EQUIPPOS_TWOHAND];
				}
				else sItemIndex = -1;

				if (sItemIndex != -1 && client->itemList[sItemIndex] != nullptr) {

					if (client->Equipped.RightHand->m_sItemEffectType == ITEMEFFECTTYPE_ATTACK_MAGICITEM) {
						short sType;
						if (wType >= 20) {
							sType = client->Equipped.RightHand->m_sItemEffectValue3;
							//PlayerMagicHandler(iClientH, dX, dY, sType, TRUE);
						}
						else {
							sType = client->Equipped.RightHand->m_sItemEffectValue2;
							//PlayerMagicHandler(iClientH, dX, dY, sType, TRUE);
						}
					}
				}
				// Battle Mages xRisenx
				if (client->Equipped.RightHand != nullptr) {
					if (client->Equipped.RightHand->m_sIDnum == ITEM_DIRECTION_BOW){
						sAbsX = abs(sX - dX);
						sAbsY = abs(sY - dY);
						tgtDist = sAbsX > sAbsY ? sAbsX : sAbsY;
						for (i = 1; i < tgtDist; i++){
							if (dice(1, 4) <= 3) {
								iErr = 0;
								CMisc::GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);
								maptarget = client->map->GetOwner(tX, tY);
								if (maptarget->OwnerType() != OWNERTYPE_NONE)
									iExp += CalculateAttackEffect(maptarget.get(), client.get(), tX, tY, wType, bNearAttack, bIsDash);
							}
						}

						maptarget = client->map->GetOwner(dX, dY);
						if (iExp == 0 && maptarget->IsNPC() && dice(1, 2) == 1) {
							iExp += CalculateAttackEffect(maptarget.get(), client.get(), dX, dY, wType, bNearAttack, bIsDash);
						}
						iExp += CalculateAttackEffect(maptarget.get(), client.get(), dX, dY, wType, bNearAttack, bIsDash);
					}
					else if (client->Equipped.TwoHand->m_sIDnum == ITEM_FIRE_BOW){
						if ((client->m_sAppr2 & 0xF000) != 0) {

							if (!client->map->iGetAttribute(sX, sY, 0x00000006) && (abs(dX - sX) > 1 || abs(dY - sY) > 1))
								iAddDynamicObjectList(client.get(), DYNAMICOBJECT_FIRE, client->map, dX, dY, (dice(1, 7) + 3) * 1000, 8);

							iExp += CalculateAttackEffect(maptarget.get(), client.get(), dX, dY, wType, bNearAttack, bIsDash);
						}
					}
					else{
						iExp += CalculateAttackEffect(maptarget.get(), client.get(), dX, dY, wType, bNearAttack, bIsDash);
					}
				}
				else{
					iExp += CalculateAttackEffect(maptarget.get(), client.get(), dX, dY, wType, bNearAttack, bIsDash);
				}
			}
			else{
				iExp += CalculateAttackEffect(maptarget.get(), client.get(), dX, dY, wType, bNearAttack, bIsDash);
			}
			client->m_dwRecentAttackTime = dwTime;
		}
	}
	else _CheckMiningAction(client.get(), dX, dY);


	if (iExp != 0) 	GetExp(client.get(), iExp, true);


	if (bResponse == true)
	{
		sw.WriteInt(MSGID_RESPONSE_MOTION);
		sw.WriteShort(OBJECTMOTION_ATTACK_CONFIRM);

		client->socket->write(sw);
	}

	maptarget = client->map->GetOwner(dX, dY);
	if (maptarget == nullptr){ return 0;  std::cout << "Error Client Motion Handler Error code 1" << endl; }
	switch (maptarget->OwnerType())
	{
		case OWNERTYPE_PLAYER:
			break;

		case OWNERTYPE_NPC:
		{
			/*Npc * npc = static_cast<Npc*>(maptarget);
			Client* client2;
			client2 = GetClient(client->m_handle);
			SendNotifyMsg(NULL, GetClient(client->m_handle), NOTIFY_NPCBAR, (npc->m_iHP * 100) / (npc->m_iHitDice * 6 + npc->m_iHitDice), npc->m_iMP, NULL, NULL);
			if (npc->m_iMP != 0)
				SendNotifyMsg(nullptr, client.get(), NOTIFY_NPCBAR2, (npc->m_iMP * 100) / npc->m_iMaxMana, 0, 0, npc->m_cNpcName);
			else
				SendNotifyMsg(nullptr, client.get(), NOTIFY_NPCBAR2, 0, 0, 0, npc->m_cNpcName);
			break;*/
		}
	}
	return 1;
}

int GServer::iClientMotion_Move_Handler(shared_ptr<Client> client, uint16_t sX, uint16_t sY, uint8_t cDir, bool bIsRun)
{
	uint64_t dwTime;
	short dX, dY, sDOtype;
	int   iDamage;
	bool  bRet;

	if (!client) return 0;
	if ((cDir <= 0) || (cDir > 8))       return 0;
	if (client->_dead == true) return 0;
	if (client->m_bIsInitComplete == false) return 0;


	if ((sX != client->x) || (sY != client->y)) return 2;

	dwTime = unixtime();
	client->m_dwLastActionTime = dwTime;

#ifndef NO_MSGSPEEDCHECK
	if (bIsRun == false) {
		client->m_iMoveMsgRecvCount++;

		if (client->m_iMoveMsgRecvCount >= 7) {
			if (client->m_dwMoveLAT != 0) {
			
				if ((dwTime - client->m_dwMoveLAT) < (72*8*7 -3000)) {
					consoleLogger->information(Poco::format("(!) Speed hack suspect(%s) - move-lat(%?d)",
						(string)client->name, dwTime - client->m_dwMoveLAT)); 
					DeleteClient(client, true, false);
					return 0;
				}
			}
			client->m_dwMoveLAT = dwTime;
			client->m_iMoveMsgRecvCount = 0;
		}
	}
	else {
		client->m_iRunMsgRecvCount++;

		if (client->m_iRunMsgRecvCount >= 7) {
			if (client->m_dwRunLAT != 0) {

				if ((dwTime - client->m_dwRunLAT) < (43*8*7 -1500)) {
					DeleteClient(client, true, false);
					return 0;
				}
			}
			client->m_dwRunLAT	= dwTime;
			client->m_iRunMsgRecvCount = 0;
		}
	}
#endif


	if (client->map != 0) {
		client->map->IncPlayerActivity(client.get());
	}

	//ClearSkillUsingStatus(iClientH);

	dX = client->x;
	dY = client->y;

	switch (cDir) {
	case 1:	dY--; break;
	case 2:	dX++; dY--;	break;
	case 3:	dX++; break;
	case 4:	dX++; dY++;	break;
	case 5: dY++; break;
	case 6:	dX--; dY++;	break;
	case 7:	dX--; break;
	case 8:	dX--; dY--;	break;
	}

	Item * pTopItem = 0; // 2.172

	bRet = client->map->bGetMoveable(dX, dY, &sDOtype, pTopItem);

	if (client->magicEffectStatus[ MAGICTYPE_HOLDOBJECT ] != 0)
		bRet = false;

	if (bRet == true) {

		//if (client->m_iQuest != NULL) _bCheckIsQuestCompleted(iClientH);

		client->map->ClearOwner(client->x, client->y);

		client->x   = dX;
		client->y   = dY;
		client->direction = cDir;

		if(!client->IsEthereal())
		{
			client->map->SetOwner(client, dX, dY);
		}

		if( client->GetParty() )
		{
			client->GetParty()->UpdateMemberCoords( client.get() );
		}

// 		if (m_astoria.get() && m_astoria->GetEventType() == ET_CAPTURE && 
// 			m_astoria->GetRelicHolder() == m_pClientList[iClientH])
// 		{
// 			m_astoria->m_relicHolderSteps++;
// 			if (m_astoria->m_relicHolderSteps > 4){
// 				m_astoria->m_relicHolderSteps = 0;
// 				UpdateRelicPos();
// 			}
// 		}

		if (sDOtype == DYNAMICOBJECT_SPIKE) {
			if ((client->neutral == true) && ((client->m_sAppr2 & 0xF000) == 0)) {

			}
			else {
				//iDamage = dice(2,4);
				iDamage = 175; // Player Damage Spike Field xRisenx

				if (!client->IsInvincible()){
					if (client->health <= iDamage)
						client->health = 0;
					else
						client->health -= iDamage;
					client->m_lastDamageTime = dwTime;
				}
			}
		}

		/*

		short sRemainItemSprite, sRemainItemSpriteFrame;
		char cRemainItemColor;

		switch (pTopItem->m_sIDnum) {
		case 540: 
		if (m_pClientList[iClientH]->m_side == 2) {

		if ((m_pClientList[iClientH]->m_sItemEquipmentStatus[EQUIPPOS_BOOTS] != -1) && 
		(m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_sItemEquipmentStatus[EQUIPPOS_BOOTS]] != NULL) && 
		(m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_sItemEquipmentStatus[EQUIPPOS_BOOTS]]->m_sItemEffectType == ITEMEFFECTTYPE_DEFENSE_ANTIMINE)) {

		}
		else {


		SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, COMMONTYPE_MAGIC, m_pClientList[iClientH]->m_cMapIndex,
		m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, (61+100), m_pClientList[iClientH]->m_sType);
		}


		pTopItem = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->pGetItem(m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, &sRemainItemSprite, &sRemainItemSpriteFrame, &cRemainItemColor);

		SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, COMMONTYPE_SETITEM, m_pClientList[iClientH]->m_cMapIndex,
		m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
		sRemainItemSprite, sRemainItemSpriteFrame, cRemainItemColor);
		delete pTopItem;
		}
		break;
		case 541: 			if (m_pClientList[iClientH]->m_side == 1) {

		if ((m_pClientList[iClientH]->m_sItemEquipmentStatus[EQUIPPOS_BOOTS] != -1) && 
		(m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_sItemEquipmentStatus[EQUIPPOS_BOOTS]] != NULL) && 
		(m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_sItemEquipmentStatus[EQUIPPOS_BOOTS]]->m_sItemEffectType == ITEMEFFECTTYPE_DEFENSE_ANTIMINE)) {

		}
		else {


		SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, COMMONTYPE_MAGIC, m_pClientList[iClientH]->m_cMapIndex,
		m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, (61+100), m_pClientList[iClientH]->m_sType);
		}


		pTopItem = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->pGetItem(m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, &sRemainItemSprite, &sRemainItemSpriteFrame, &cRemainItemColor);

		SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, COMMONTYPE_SETITEM, m_pClientList[iClientH]->m_cMapIndex,
		m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
		sRemainItemSprite, sRemainItemSpriteFrame, cRemainItemColor);
		delete pTopItem;
		}
		break;
		}
		//
		*/

		StreamWrite sw(100);

		sw.WriteInt(MSGID_RESPONSE_MOTION);
		sw.WriteShort(OBJECTMOVE_CONFIRM);
		sw.WriteShort(dX - 12);//800x600
		//sw.WriteShort(dX - 10);
		sw.WriteShort(dY - 9);//800x600
		//sw.WriteShort(dY - 7);
		sw.WriteByte(cDir);

		if (bIsRun == true) {
			if (client->m_iSP > 0) {
				if (client->m_iTimeLeft_FirmStamina == 0)
				{
					client->m_iSP--;
					sw.WriteByte(1);
				}
				else
					sw.WriteByte(0);
			}
			else {
				if (client->m_iTimeLeft_FirmStamina == 0) {
					client->m_iSP--;
					sw.WriteByte(1);
				}
				else
					sw.WriteByte(0);

				if (client->m_iSP < -10) {
					client->m_iSP = 0;
					DeleteClient(client);
					return 0;
				}
			}
		}
		else
			sw.WriteByte(0);

		sw.WriteInt(uint32_t(client->health));


		int ix, iy, iSize = 2, iIndex = 0;
		Tile * pTileSrc;
		short total = 0;
		uint64_t oldpos = sw.position;
		sw.WriteShort(0);//total placeholder

		pTileSrc = client->map->GetTile(dX - 12, dY - 9);
		//pTileSrc = (Tile *)(client->pMap->_tile + (dX - 12) + (dY - 9)*client->pMap->sizeY);

		while (1)
		{
			ix = _tmp_iMoveLocX[cDir][iIndex];
			iy = _tmp_iMoveLocY[cDir][iIndex];
			if ((ix == -1) || (iy == -1)) break;
			iIndex++;

			if (WriteTileData(sw, client.get(), pTileSrc, ix, iy))
				total++;
		}

		
		char * temp = sw.data + oldpos;
		*((short *)temp) = total;

//#ifdef 800x600
		//iSize = iComposeMoveMapData((short)(dX - 12), (short)(dY - 9), iClientH, cDir, cp);
//#else
		//iSize = iComposeMoveMapData((short)(dX - 10), (short)(dY - 7), iClientH, cDir, cp);
//#endif
		client->mutsocket.lock();
		if (client->socket)
			client->socket->write(sw.data, sw.position);
		client->mutsocket.unlock();
	}
	else {
		client->rejectedMove = true; 
		client->m_resetMoveFreq = true; 

		StreamWrite sw(50);

		sw.WriteInt(MSGID_RESPONSE_MOTION);
		sw.WriteShort(OBJECTMOVE_REJECT);
		sw.WriteShort(uint32_t(client->handle));
		sw.WriteShort(client->x);
		sw.WriteShort(client->y);
		sw.WriteShort(client->Type());
		sw.WriteByte(cDir);
		sw.WriteString(client->name, 10);
		sw.WriteShort(client->m_sAppr1);
		sw.WriteShort(client->m_sAppr2);
		sw.WriteShort(client->m_sAppr3);
		sw.WriteShort(client->m_sAppr4);
		sw.WriteInt(client->m_iApprColor);
		sw.WriteShort(client->m_sHeadApprValue);
		sw.WriteShort(client->m_sBodyApprValue);
		sw.WriteShort(client->m_sArmApprValue);
		sw.WriteShort(client->m_sLegApprValue);
		sw.WriteInt(client->status);

		client->mutsocket.lock();
		if (client->socket)
			client->socket->write(sw.data, sw.position);
		client->mutsocket.unlock();

		shared_ptr<Unit> owner = client->map->GetOwner(dX, dY);
		if(owner)
		{
			if(owner->IsPlayer())
				RequestFullObjectData(client, owner.get());
		}

		return 0;
	}

	return 1;
}

int GServer::iClientMotion_Magic_Handler(shared_ptr<Client> client, uint16_t sX, uint16_t sY, uint8_t cDir)
{
	//TODO: like today pls.
	return 1;
}

void GServer::ClientCommonHandler(shared_ptr<Client> client, shared_ptr<MsgQueueEntry> msg)
{
	if (!msg->client->m_bIsInitComplete) return;
	if (msg->client->_dead) return;

	StreamRead sr = StreamRead(msg->data, msg->size);

	uint32_t msgid = sr.ReadInt();
	uint16_t command = sr.ReadShort();

// 	uint16_t sX = sr.ReadShort();
// 	uint16_t sY = sr.ReadShort();
// 
// 	uint8_t direction = sr.ReadByte();
// 
// 	int32_t iV1 = sr.ReadInt();
// 	int32_t iV2 = sr.ReadInt();
// 	int32_t iV3 = sr.ReadInt();
// 
// 	string stringmsg = sr.ReadString(30);
// 
// 	int32_t iV4 = sr.ReadInt();

	switch(command)
	{
// 			case COMMONTYPE_REQ_CREATESLATE:
// 		ReqCreateSlateHandler(iClientH, pData);
// 		break;
// 
// 	case COMMONTYPE_REQGUILDNAME:
// 		RequestGuildNameHandler(iClientH, iV1, iV2);
// 		break;
// 
// 	case COMMONTYPE_UPGRADEITEM:
// 		RequestItemUpgradeHandler(iClientH, iV1);
// 		break;
// 
// 	case COMMONTYPE_REQUEST_ACCEPTJOINPARTY:
// 		RequestAcceptJoinPartyHandler(iClientH, iV1);
// 		break;
// 
// 	case COMMONTYPE_SETGUILDTELEPORTLOC:
// 		RequestSetGuildTeleportLocHandler(iClientH, iV1, iV2, m_pClientList[iClientH]->m_iGuildGUID, pString);
// 		break;
// 
// 	case COMMONTYPE_SETGUILDCONSTRUCTLOC:
// 		RequestSetGuildConstructLocHandler(iClientH, iV1, iV2, m_pClientList[iClientH]->m_iGuildGUID, pString);
// 		break;
// 
// 	case COMMONTYPE_GUILDTELEPORT:
// 		RequestGuildTeleportHandler(iClientH);
// 		break;
// 
// 	case COMMONTYPE_SUMMONWARUNIT:
// 		RequestSummonWarUnitHandler(iClientH, sX, sY, iV1, iV2, iV3);
// 		break;
// 
// 	case COMMONTYPE_REQUEST_MAPSTATUS:
// 		MapStatusHandler(iClientH, iV1, pString);
// 		break;
// 
// 	case COMMONTYPE_REQUEST_SELECTCRUSADEDUTY:
// 		SelectCrusadeDutyHandler(iClientH, iV1);
// 		break;
// 
// 	case COMMONTYPE_REQUEST_CANCELQUEST:
// 		CancelQuestHandler(iClientH);
// 		break;
// 
// 	case COMMONTYPE_REQUEST_ACTIVATESPECABLTY:
// 		ActivateSpecialAbilityHandler(iClientH);
// 		break;
// 
// 	case COMMONTYPE_REQUEST_JOINPARTY:
// 		JoinPartyHandler(iClientH, iV1, pString);
// 		break;
// 
// 	case COMMONTYPE_GETMAGICABILITY:
// 		GetMagicAbilityHandler(iClientH);
// 		break;
// 
// 	case COMMONTYPE_BUILDITEM:
// 		BuildItemHandler(iClientH, pData);
// 		break;
// 
// 	case COMMONTYPE_CANCELEXCHANGEITEM:
// 		CancelExchangeItem(iClientH);
// 		break;
// 
// 	case COMMONTYPE_CONFIRMEXCHANGEITEM:
// 		ConfirmExchangeItem(iClientH);
// 		break;
// 
// 	case COMMONTYPE_SETEXCHANGEITEM:
// 		SetExchangeItem(iClientH, iV1, iV2);
// 		break;
// 
// 	case COMMONTYPE_REQ_GETHEROMANTLE:
// 		GetHeroMantleHandler(iClientH, iV1, pString);
// 		break;
// 		// EK Trade xRisenx
// 	/*case COMMONTYPE_REQ_GETTRADEEKITEM:
//         GetTradeEKItemHandler(iClientH, iV1, pString);  
//         break;*/
// 		// EK Trade xRisenx
// 		// Ball Points xRisenx
// 	/*case COMMONTYPE_REQ_GETTRADEBALLITEM:    
//         GetTradeBALLItemHandler(iClientH, iV1, pString);  
//         break;*/
// 		// Ball Points xRisenx
// 	case COMMONTYPE_REQ_GETOCCUPYFLAG:
// 		GetOccupyFlagHandler(iClientH);
// 		break;
// 
// 	case COMMONTYPE_REQ_SETDOWNSKILLINDEX:
// 		SetDownSkillIndexHandler(iClientH, iV1);
// 		break;
// 
// 	case COMMONTYPE_TALKTONPC:
// 		NpcTalkHandler(iClientH, iV1);
// 		break;
// 
// 	case COMMONTYPE_REQ_CREATEPOTION:
// 		ReqCreatePortionHandler(iClientH, pData);
// 		break;
// 
// 	case COMMONTYPE_REQ_GETFISHTHISTIME:
// 		ReqGetFishThisTimeHandler(iClientH);
// 		break;
// 
// 	case COMMONTYPE_REQ_REPAIRITEMCONFIRM:
// 		ReqRepairItemCofirmHandler(iClientH, iV1, pString);
// 		break;
// 
// 	case COMMONTYPE_REQ_REPAIRITEM:
// 		ReqRepairItemHandler(iClientH, iV1, iV2, pString);
// 		break;
// 
// 	case COMMONTYPE_REQ_SELLITEMCONFIRM:
// 		ReqSellItemConfirmHandler(iClientH, iV1, iV2, pString);
// 		break;
// 
// 	case COMMONTYPE_REQ_SELLITEM:
// 		ReqSellItemHandler(iClientH, iV1, iV2, iV3, pString);
// 		break;
// 
// 	case COMMONTYPE_REQ_USESKILL:
// 		UseSkillHandler(iClientH, iV1, iV2, iV3);
// 		break;
// 
// 	case COMMONTYPE_REQ_USEITEM:
// 		UseItemHandler(iClientH, iV1, iV2, iV3, iV4);
// 		break;
// 
// 	case COMMONTYPE_REQ_GETREWARDMONEY:
// 		GetRewardMoneyHandler(iClientH);
// 		break;
// 
// 	case COMMONTYPE_ITEMDROP:
// 		DropItemHandler(iClientH, iV1, iV2, pString, true);
// 		break;
// 
// 	case COMMONTYPE_EQUIPITEM:
// 		bEquipItemHandler(iClientH, iV1);
// 		break;
// 
// 	case COMMONTYPE_REQ_PURCHASEITEM:
// 		RequestPurchaseItemHandler(iClientH, pString, iV1);
// 		break;
// 
// 	case COMMONTYPE_REQ_STUDYMAGIC:
// 		RequestStudyMagicHandler(iClientH, pString);
// 		break;
// 
// 	case COMMONTYPE_REQ_TRAINSKILL:
// 		//RequestTrainSkillHandler(iClientH, pString);
// 		break;
// 
// 	case COMMONTYPE_GIVEITEMTOCHAR:
// 		GiveItemHandler(iClientH, cDir, iV1, iV2, iV3, iV4, pString);
// 		break;
// 
// 	case COMMONTYPE_EXCHANGEITEMTOCHAR:
// 		ExchangeItemHandler(iClientH, cDir, iV1, iV2, iV3, iV4, pString);
// 		break;
// 
// 	case COMMONTYPE_JOINGUILDAPPROVE:
// 		JoinGuildApproveHandler(iClientH, pString);
// 		break;
// 
// 	case COMMONTYPE_JOINGUILDREJECT:
// 		JoinGuildRejectHandler(iClientH, pString);
// 		break;
// 
// 	case COMMONTYPE_DISMISSGUILDAPPROVE:
// 		DismissGuildApproveHandler(iClientH, pString);
// 		break;
// 
// 	case COMMONTYPE_DISMISSGUILDREJECT:
// 		DismissGuildRejectHandler(iClientH, pString);
// 		break;
// 
// 	case COMMONTYPE_RELEASEITEM:
// 		ReleaseItemHandler(iClientH, iV1, true);
// 		break;
// 
	case COMMONTYPE_TOGGLECOMBATMODE:
		ToggleCombatModeHandler(client);
		break;
// 
// 	case COMMONTYPE_MAGIC:
// 		if(m_pClientList[iClientH]->m_hasPrecasted || m_pClientList[iClientH]->IsGM())
// 		PlayerMagicHandler(iClientH, iV1, iV2, (iV3 - 100));
// 		break;
// 
// 	case COMMONTYPE_TOGGLESAFEATTACKMODE:
// 		ToggleSafeAttackModeHandler(iClientH);
// 		break;
// 
// 	case COMMONTYPE_REQ_GETOCCUPYFIGHTZONETICKET:
// 		GetFightzoneTicketHandler(iClientH);
// 		break;
// 
// 	case COMMONTYPE_CRAFTITEM:
// 		ReqCreateCraftingHandler(iClientH, pData);
// 		break; 
// 
	case COMMONTYPE_REQTITLE:
		RequestTitleHandler(client.get(), sr);
		break;
// 
// 	case COMMONTYPE_REQ_REPAIRALL:
// 		ReqRepairAll(iClientH);
// 		break;
// 
// 	case COMMONTYPE_REQ_REPAIRALLCONFIRM:
// 		ReqRepairAllConfirmHandler(iClientH, iV1);
// 		break;
	default:
		consoleLogger->error(Poco::format("Unknown common packet received from client - 0x%.2X", (uint32_t)command));
		break;
	}
}

shared_ptr<Client> GServer::GetClient(uint64_t ObjectID)
{
	for (shared_ptr<Client> client : clientlist)
	{
		if (client->handle == ObjectID)
			return client;
	}
	return nullptr;
}

void GServer::RequestTitleHandler(Client * client, StreamRead & sr)
{
	int * ip;
	char * cp;
	short * sp;

	if (client == nullptr) return;

	sr.position += 6 + 2 + 2 + 1;

	int32_t ObjectID = sr.ReadInt();
	int32_t ReturnID = sr.ReadInt();

	shared_ptr<Client> findclient = GetClient(ObjectID);

	if (findclient == nullptr) return;

	int iCrusadeJob = 0;
	if (m_bIsCrusadeMode) {
		iCrusadeJob = findclient->crusadeDuty;
	}

	SendNotifyMsg(nullptr, client, NOTIFY_REQTITLEANSWER, findclient->TitleType, ReturnID, iCrusadeJob, findclient->ActiveTitle);
}

void GServer::ToggleCombatModeHandler(shared_ptr<Client> client)
{
	short sAppr2;

	if (client == nullptr) return;
	if (client->m_bIsInitComplete == false) return;
	if (client->_dead == true) return;
	if (client->skillInUse[19] == true) return;

	sAppr2 = (short)((client->m_sAppr2 & 0xF000) >> 12);

	client->m_bIsAttackModeChange = true;


	if (sAppr2 == 0) {
		client->m_sAppr2 = (0xF000 | client->m_sAppr2);
	}
	else {
		client->m_sAppr2 = (0x0FFF & client->m_sAppr2);
	}

	SendEventToNearClient_TypeA(client.get(), MSGID_MOTION_NULL, 0, 0, 0);

}

bool GServer::_bAddClientItemList(shared_ptr<Client> client, Item * pItem, int * pDelReq)
{
	int i;

	if (pItem == 0) return false;


	if ((pItem->m_cItemType == ITEMTYPE_CONSUME) || (pItem->m_cItemType == ITEMTYPE_ARROW)) {
		if ((client->m_iCurWeightLoad + pItem->GetWeight(pItem->m_dwCount)) > _iCalcMaxLoad(client)) 
			return false;
	}
	else {
		if ((client->m_iCurWeightLoad + pItem->GetWeight(1)) > _iCalcMaxLoad(client)) 
			return false;
	}

	if ((pItem->m_cItemType == ITEMTYPE_CONSUME) || (pItem->m_cItemType == ITEMTYPE_ARROW)) {
		for (i = 0; i < client->itemList.size(); i++)
			if ( (client->itemList[i]->_item != nullptr) && 
				(client->itemList[i]->_item->m_cName == pItem->m_cName) ) {

					client->itemList[i]->_item->m_dwCount += pItem->m_dwCount;
					//delete pItem;
					*pDelReq = 1;

					iCalcTotalWeight(client);

					return true;
			}
	}

	for (i = 0; i < client->itemList.size(); i++)
		if (client->itemList[i]->_item == 0) {

			client->itemList[i]->_item = pItem;

			client->itemList[i]->_item->x = 40;
			client->itemList[i]->_item->y = 30;

			*pDelReq = 0;

			if (pItem->m_cItemType == ITEMTYPE_ARROW)
				client->Equipped.Arrow = _iGetArrowItemIndex(client.get());

			iCalcTotalWeight(client);

			return true;
		}

		return false;
}

int GServer::_iCalcMaxLoad(shared_ptr<Client> client)
{
	return (client->GetStr() * 500 + client->level * 2000); // Changed max weight from 5 to 20 per level xRisenx
}

int GServer::iCalcTotalWeight(shared_ptr<Client> client)
{
	client->UpdateWeight();

	return client->GetWeight();
}

void GServer::DropItemHandler(shared_ptr<Client> client, short sItemIndex, int iAmount, string pItemName, bool bByPlayer)
{
	Item * pItem;
	Client * player = client.get();
	if (!player || player->m_bIsOnWaitingProcess || !player->m_bIsInitComplete) return;

	if (sItemIndex < 0 || sItemIndex >= client->itemList.size()) return;

	Item * &itemDrop = player->itemList[sItemIndex]->_item;
	if (!itemDrop || itemDrop->m_disabled) return;
	if (iAmount != -1 && iAmount < 0) return;

	if( (itemDrop->m_cItemType == ITEMTYPE_CONSUME || 
		itemDrop->m_cItemType == ITEMTYPE_ARROW) &&
		iAmount == -1) 
		iAmount = itemDrop->m_dwCount;


	if(itemDrop->m_cName != pItemName) return;
	if(client->map->iCheckItem(player->x, player->y) == ITEM_RELIC)
		return;

	if ( ( (itemDrop->m_cItemType == ITEMTYPE_CONSUME) ||
		(itemDrop->m_cItemType == ITEMTYPE_ARROW) ) &&
		(((int)itemDrop->m_dwCount - iAmount) > 0) ) {
			pItem = new Item;
			if (pItem->InitItemAttr(itemDrop->m_cName, m_pItemConfigList) == false) {

				delete pItem;
				return;
			}
			else {
				if (iAmount <= 0) {

					delete pItem;
					return;
				}
				pItem->m_dwCount = (uint32_t)iAmount;
			}


			if ((uint32_t)iAmount > itemDrop->m_dwCount) {
				delete pItem;
				return;
			}

			itemDrop->m_dwCount -= iAmount;

			SetItemCount(client, sItemIndex, itemDrop->m_dwCount);

			player->map->bSetItem(player->x,	player->y, pItem);

			//_bItemLog(ITEMLOG_DROP, iClientH, (int) -1, pItem, !bByPlayer);

			//AddGroundItem(pItem, player->m_sX, player->m_sY, player->pMap, TILECLEANTIMEPLAYER);

			SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, COMMONTYPE_ITEMDROP, player->map,
				player->x, player->y,  
				pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_ItemColor); 

			SendNotifyMsg(nullptr, client.get(), NOTIFY_DROPITEMFIN_COUNTCHANGED, sItemIndex, iAmount, 0);
	}
	else {

		//ReleaseItemHandler(iClientH, sItemIndex, true);

		if ( player->itemList[sItemIndex]->_item->equipped == true)
			SendNotifyMsg(nullptr, client.get(), NOTIFY_ITEMRELEASED, itemDrop->m_cEquipPos, sItemIndex, 0);

		if ((itemDrop->m_sItemEffectType == ITEMEFFECTTYPE_ALTERITEMDROP) && 
			(itemDrop->m_wCurLifeSpan == 0)) {
				delete itemDrop;
				itemDrop = nullptr;
		}
		else {
			if(itemDrop->m_sIDnum != ITEM_RELIC/* || (m_astoria.get() && m_astoria->IsRelicGenuine(itemDrop))*/)
			{
/*
				if(itemDrop->m_sIDnum == ITEM_RELIC)
				{
					m_astoria->PlayerDropRelic(player);		
					UpdateRelicPos();
				}
				else*/
					AddGroundItem(itemDrop, player->x, player->y, player->map, TILECLEANTIMEPLAYER);

				player->map->bSetItem(player->x, player->y, 
					itemDrop);

				//_bItemLog(ITEMLOG_DROP, iClientH, (int) -1, itemDrop, !bByPlayer);

				SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, COMMONTYPE_ITEMDROP, player->map,
					player->x, player->y,  
					itemDrop->m_sSprite, 
					itemDrop->m_sSpriteFrame, 
					itemDrop->m_ItemColor);
			}/*else{
				// delete the fake relic
				delete itemDrop;
			}*/
		}

		itemDrop = 0;
		player->itemList[sItemIndex]->_item->equipped = false;

		SendNotifyMsg(0, player, NOTIFY_DROPITEMFIN_ERASEITEM, sItemIndex, iAmount, 0);

		player->Equipped.Arrow = _iGetArrowItemIndex(player);
	}

	iCalcTotalWeight(client);
	SendNotifyMsg(0, player, NOTIFY_REQDEF, player->m_iDefenseRatio, 0, 0); // Auto updates defense in game xRisenx
}

void GServer::AddGroundItem(Item * pItem, uint16_t x, uint16_t y, Map * mapIndex, uint64_t dwTime)
{//TODO: move to Map class and revise a bit
// 	for(int i = 0; i < MAXGROUNDITEMS; i++)
// 	{
// 		if(m_stGroundNpcItem[i].bEmpty)
// 		{
// 			m_stGroundNpcItem[i].bEmpty = false;
// 			m_stGroundNpcItem[i].dropTime = unixtime() + dwTime;
// 			m_stGroundNpcItem[i].item = pItem;
// 			m_stGroundNpcItem[i].sx = x;
// 			m_stGroundNpcItem[i].sy = y;
// 			m_stGroundNpcItem[i].cMapIndex = mapIndex;
// 			return;
// 		}
// 	}
// 	PutLogList("WARNING: Ground NPC item list is full, item will not be cleared.");
}

void GServer::SendItemNotifyMsg(shared_ptr<Client> client, uint16_t msgtype, Item *pItem, int iV1)
{
	char  * cp, cData[512];
	uint32_t * dwp;
	uint16_t  * wp;
	int     iRet;
	StreamWrite sw;

	sw.WriteInt(MSGID_NOTIFY);
	sw.WriteShort(msgtype);

	switch (msgtype)
	{
	case NOTIFY_ITEMOBTAINED:
		WriteItemData(sw, pItem);

		sw.WriteByte((char)pItem->m_sItemSpecEffectValue2);
		sw.WriteInt(pItem->m_dwAttribute);

		for(int j = 0; j < MAXITEMSOCKETS; j++)
		{
			sw.WriteByte(pItem->m_sockets[j]);
		}

		client->mutsocket.lock();
		if (client->socket)
			client->socket->write(sw.data, sw.size);
		client->mutsocket.unlock();
		break;

	case NOTIFY_ITEMPURCHASED:
		WriteItemData(sw, pItem);

		sw.WriteShort(iV1); // (iCost - iDiscountCost);

		client->mutsocket.lock();
		if (client->socket)
			client->socket->write(sw.data, sw.size);
		client->mutsocket.unlock();
		break;

	case NOTIFY_CANNOTCARRYMOREITEM:
		client->mutsocket.lock();
		if (client->socket)
			client->socket->write(sw.data, sw.size);
		client->mutsocket.unlock();
		break;
	}
}

void GServer::WriteItemData(StreamWrite & sw, Item * pItem) const
{
	uint32_t * dwp;
	uint16_t * wp;
	short * sp;

	sw.WriteString(pItem->m_cName, 20);
	sw.WriteInt(pItem->m_dwCount);
	sw.WriteByte(pItem->m_cItemType);
	sw.WriteByte(pItem->m_cEquipPos);
	sw.WriteByte(0);
	sw.WriteShort(pItem->m_sLevelLimit);
	sw.WriteByte(pItem->m_cGenderLimit);
	sw.WriteShort(pItem->m_wCurLifeSpan);
	sw.WriteShort(pItem->m_wWeight);
	sw.WriteShort(pItem->m_sSprite);
	sw.WriteShort( pItem->m_sSpriteFrame);
	sw.WriteInt(pItem->m_ItemColor);

	return;
}

int GServer::SetItemCount(shared_ptr<Client> client, int32_t iItemID, uint32_t dwCount)
{
	uint16_t wWeight;


	for (ItemWrap * pitemw : client->itemList)
	{
		if (pitemw->_item->m_sIDnum == iItemID)
		{
			wWeight = pitemw->_item->GetWeight(1);

			if (dwCount == 0) {
				ItemDepleteHandler(client, pitemw->_item, false);
			}
			else
			{
				pitemw->_item->m_dwCount = dwCount;
				SendNotifyMsg(0, client.get(), NOTIFY_SETITEMCOUNT, iItemID, dwCount, (char)true);
			}
			return wWeight;
		}
	}
	return 0;
}

void GServer::ItemDepleteHandler(shared_ptr<Client> client, Item * pItem, bool bIsUseItemResult, bool bIsLog, bool notify)
{
	if (client->m_bIsInitComplete == false) return;

/*
	if ((bIsLog == true) && (client->m_pItemList[sItemIndex]->_item->m_cItemType != ITEMTYPE_CONSUME) 
		&& (client->m_pItemList[sItemIndex]->_item->m_cItemType != ITEMTYPE_EAT) 
		&& (client->m_pItemList[sItemIndex]->_item->m_cItemType != ITEMTYPE_USE_DEPLETE) 
		&& (client->m_pItemList[sItemIndex]->_item->m_cItemType != ITEMTYPE_USE_DEPLETE_DEST)
		&& (client->m_pItemList[sItemIndex]->_item->m_cItemType != ITEMTYPE_MATERIAL)
		&& (client->m_pItemList[sItemIndex]->_item->m_sIDnum != ITEM_ISMANUAL)
		&& (client->m_pItemList[sItemIndex]->_item->m_sIDnum != ITEM_SCANMANUAL)
		&& (client->m_pItemList[sItemIndex]->_item->m_sIDnum != ITEM_MHMANUAL)
		&& (client->m_pItemList[sItemIndex]->_item->m_sIDnum != ITEM_MSMANUAL)
		&& (client->m_pItemList[sItemIndex]->_item->m_sIDnum != ITEM_COTGMANUAL)
		&& (client->m_pItemList[sItemIndex]->_item->m_sIDnum != ITEM_SOTGMANUAL)
		&& (client->m_pItemList[sItemIndex]->_item->m_sIDnum != ITEM_MBMANUAL)
		&& (client->m_pItemList[sItemIndex]->_item->m_sIDnum != ITEM_FSWMANUAL)
		&& (client->m_pItemList[sItemIndex]->_item->m_sIDnum != ITEM_LCMANUAL)
		&& (client->m_pItemList[sItemIndex]->_item->m_sIDnum != ITEM_BSWMANUAL))
	{
	//	_bItemLog(ITEMLOG_DEPLETE, client,(int) -1, client->m_pItemList[sItemIndex]);
	}*/

	//TODO: need to unequip the item if it's equipped
	//ReleaseItemHandler(client, sItemIndex, true);

	if(notify)
		SendNotifyMsg(nullptr, client.get(), NOTIFY_ITEMDEPLETED_ERASEITEM, pItem->m_slot, (int)bIsUseItemResult, 0);

	for (ItemWrap * pitemw : client->itemList)
	{
		if (pitemw->_item == pItem)
		{
			pitemw->DeleteItem();
			break;
		}
	}

	//client->m_pItemList[sItemIndex]->_item->equipped = false;

	// !!! BUG POINT

	client->Equipped.Arrow = _iGetArrowItemIndex(client.get());

	iCalcTotalWeight(client);
}

bool GServer::WriteTileData(StreamWrite & sw, Client * player, Tile * srcTile, uint16_t ix, uint16_t iy)
{
	unsigned char ucHeader;
	bool dataWritten = false;

	Tile * pTile = (Tile *)(srcTile + ix + iy*player->map->sizeY);

	if ( (pTile->owner != 0) || (pTile->deadowner != 0) || (pTile->m_pItem.size() != 0) || (pTile->m_sDynamicObjectType != 0) )
	{
		dataWritten = true;
		sw.WriteShort(ix);
		sw.WriteShort(iy);

		ucHeader = 0;
		if (pTile->owner != nullptr)
		{
			if (pTile->owner->IsPlayer())
				ucHeader = ucHeader | 0x01;
			else/* if (pTile->owner->m_sType == OWNERTYPE_NPC)*/
				ucHeader = ucHeader | 0x01;
		}
		if (pTile->deadowner != nullptr)
		{
			if (pTile->deadowner->IsPlayer())
				ucHeader = ucHeader | 0x02;
			else/* if (pTile->deadowner->m_sType == OWNERTYPE_NPC)*/
				ucHeader = ucHeader | 0x02;
		}
		if (pTile->m_pItem.size() != 0)				ucHeader = ucHeader | 0x04;
		if (pTile->m_sDynamicObjectType != 0)    ucHeader = ucHeader | 0x08;
		//
		sw.WriteByte(ucHeader);

		if ((ucHeader & 0x01) != 0)
		{
			switch (pTile->owner->OwnerType())
			{
			case OWNERTYPE_PLAYER:
				{
					Client * object = static_cast<Client*>(pTile->owner.get());
					// Object ID number(Player) : 1~10000
					sw.WriteShort(uint16_t(object->handle));
					// object type
					sw.WriteShort(object->Type());
					// dir
					sw.WriteByte(object->direction);
					// Appearance1
					sw.WriteShort(object->m_sAppr1);
					// Appearance2
					sw.WriteShort(object->m_sAppr2);
					// Appearance3
					sw.WriteShort(object->m_sAppr3);
					// Appearance4
					sw.WriteShort(object->m_sAppr4);

					sw.WriteInt(object->m_iApprColor);
					sw.WriteShort(object->m_sHeadApprValue);
					sw.WriteShort(object->m_sBodyApprValue);
					sw.WriteShort(object->m_sArmApprValue);
					sw.WriteShort(object->m_sLegApprValue);

					if(_bGetIsPlayerHostile(player, object) && player->m_iAdminUserLevel == 0)
						sw.WriteInt(STATUS_ENEMYFLAGS & object->status);
					else 
						sw.WriteInt(object->status);

					// Name
					sw.WriteString(object->name, 10);
				}
				break;

			case OWNERTYPE_NPC:
				{
					Npc * object = static_cast<Npc*>(pTile->owner.get());
					// Object ID number(NPC) : 10000	~
					sw.WriteShort(uint16_t(object->handle + 10000));
					// object type
					sw.WriteShort(object->Type());
					// dir
					sw.WriteByte(object->direction);
					// Appearance2
					sw.WriteShort(object->appr2);
					// Status
					sw.WriteInt(object->status);
				}
				break;
			}
		}

		if ((ucHeader & 0x02) != 0)
		{
			switch (pTile->deadowner->OwnerType())
			{
			case OWNERTYPE_PLAYER:
				{
					Client * object = static_cast<Client*>(pTile->deadowner.get());
					// Object ID number(Player) : 1~10000
					sw.WriteShort(uint16_t(object->handle));
					// object type
					sw.WriteShort(object->Type());
					// dir
					sw.WriteByte(object->direction);
					// Appearance1
					sw.WriteShort(object->m_sAppr1);
					// Appearance2
					sw.WriteShort(object->m_sAppr2);
					// Appearance3
					sw.WriteShort(object->m_sAppr3);
					// Appearance4
					sw.WriteShort(object->m_sAppr4);

					sw.WriteInt(object->m_iApprColor);
					sw.WriteShort(object->m_sHeadApprValue);
					sw.WriteShort(object->m_sBodyApprValue);
					sw.WriteShort(object->m_sArmApprValue);
					sw.WriteShort(object->m_sLegApprValue);

					if(_bGetIsPlayerHostile(player, object) && player->m_iAdminUserLevel == 0)
						sw.WriteInt(STATUS_ENEMYFLAGS & object->status);
					else 
						sw.WriteInt(object->status);

					// Name
					sw.WriteString(object->name, 10);
				}
				break;

			case OWNERTYPE_NPC:
				{
					Npc * object = static_cast<Npc*>(pTile->deadowner.get());
					// Object ID number(NPC) : 10000	~
					sw.WriteShort(uint16_t(object->handle + 10000));
					// object type
					sw.WriteShort(object->Type());
					// dir
					sw.WriteByte(object->direction);
					// Appearance2
					sw.WriteShort(object->appr2);
					// Status
					sw.WriteInt(object->status);
				}
				break;
			}
		}

		if (pTile->m_pItem.size() > 0)
		{
			sw.WriteShort(pTile->m_pItem[0]->m_sSprite);
			sw.WriteShort(pTile->m_pItem[0]->m_sSpriteFrame);
			sw.WriteInt(pTile->m_pItem[0]->m_ItemColor);
		}

		if (pTile->m_sDynamicObjectType != 0)
		{
			sw.WriteShort(pTile->m_wDynamicObjectID);
			sw.WriteShort(pTile->m_sDynamicObjectType);
		}
	}
	return dataWritten;
}

void GServer::RequestFullObjectData(shared_ptr<Client> client, Unit * target)
{
	uint32_t temp;

	if (client == 0) return;
	if (client->m_bIsInitComplete == false) return;

	if (target != 0)
	{
		if (target->IsPlayer())
		{
			StreamWrite sw(50);

			Client * object = static_cast<Client*>(target);

			sw.WriteInt(MSGID_MOTION_STOP);
			sw.WriteShort(uint16_t(object->handle));
			sw.WriteShort(object->x);
			sw.WriteShort(object->y);
			sw.WriteShort(object->Type());
			sw.WriteByte(object->direction);
			sw.WriteString(object->name, 10);
			sw.WriteShort(object->m_sAppr1);
			sw.WriteShort(object->m_sAppr2);
			sw.WriteShort(object->m_sAppr3);
			sw.WriteShort(object->m_sAppr4);
			sw.WriteInt(object->m_iApprColor);
			sw.WriteShort(object->m_sHeadApprValue);
			sw.WriteShort(object->m_sBodyApprValue);
			sw.WriteShort(object->m_sArmApprValue);
			sw.WriteShort(object->m_sLegApprValue);


			temp = object->status;
			if(_bGetIsPlayerHostile(client.get(), object) && client->m_iAdminUserLevel == 0)
				temp &= STATUS_ENEMYFLAGS;

			sw.WriteInt(temp);
			sw.WriteByte(uint8_t(object->_dead ? true : false));

			client->mutsocket.lock();
			if (client->socket)
				client->socket->write(sw.data, sw.position);
			client->mutsocket.unlock();
		}
		else
		{
			if(target->handle == 0) return;

			Npc * object = static_cast<Npc*>(target);
			if(!object) 
				return;


			StreamWrite sw(50);

			sw.WriteInt(MSGID_MOTION_STOP);
			sw.WriteShort(uint16_t(object->handle + 10000));
			sw.WriteShort(object->x);
			sw.WriteShort(object->y);
			sw.WriteShort(object->Type());
			sw.WriteByte(object->direction);
			sw.WriteShort(object->appr2);
			sw.WriteInt(object->status);
			sw.WriteByte(uint8_t(object->_dead ? true : false));

			client->mutsocket.lock();
			if (client->socket)
				client->socket->write(sw.data, sw.position);
			client->mutsocket.unlock();
		}
	}
}

bool GServer::_bGetIsPlayerHostile(Client * player, Client * target)
{
	if (player) return false;
	if (target) return false;

	if (player == target) return false;

	if (player->IsNeutral()) {
		if (target->playerKillCount != 0) 
			return true;
		else return false;
	}
	else {
		if (player->side != target->side) {
			return true;
		}
		else {
			if (target->playerKillCount != 0) 
				return true;
			else return false;
		}
	}

	return false;
}

bool GServer::LoadCharacterData(shared_ptr<Client> client)
{
	try
	{
		{
			Session ses(sqlpool->get());
			Statement select(ses);
			select << "SELECT * FROM char_database WHERE account_name=? AND char_name=? AND servername=?;", use(client->account), use(client->name), use(servername), now;
			RecordSet rs(select);

			rs.moveFirst();

			client->handle = rs.value("charid").convert<uint64_t>();
			client->mapName = rs.value("maploc").convert<string>();
			client->x = rs.value("locx").convert<int16_t>();
			client->y = rs.value("locy").convert<int16_t>();
			client->gender = rs.value("gender").convert<uint8_t>();
			client->colorSkin = rs.value("skin").convert<uint8_t>();
			client->m_cHairStyle = rs.value("hairstyle").convert<uint8_t>();
			client->colorHair = rs.value("haircolor").convert<uint32_t>();
			client->colorUnderwear = rs.value("underwear").convert<uint32_t>();
			client->guildName = rs.value("guildname").convert<string>();
			string temp = rs.value("magicmastery").convert<string>();
			for (int i = 0; i < temp.length(); ++i)
			{
				client->magicMastery[i] = (temp[i] == '1') ? 1 : 0;
			}
			//memcpy(client->m_cMagicMastery, rs.value("magicmastery").convert<string>().c_str(), rs.value("magicmastery").convert<string>().length());//fix. each should be 0x00, not (char)'0'
			string side = rs.value("nation").convert<string>();
			/*if (side == "NONE")
				side = Side::NEUTRAL;
				else if (side == "Aresden")
				side = Side::ARESDEN;
				else if (side == "Elvine")
				side = Side::ELVINE;*/
			client->faction = side;
			//client->m_cMapName = rs.value("BlockDate").convert<string>();
			client->lockedMapName = rs.value("lockmapname").convert<string>();
			client->lockedMapTime = rs.value("lockmaptime").convert<uint64_t>();
			client->profile = rs.value("profile").convert<string>();
			client->guildRank = rs.value("guildrank").convert<int8_t>();
			client->health = rs.value("hp").convert<uint64_t>();
			client->mana = rs.value("mp").convert<uint64_t>();
			client->m_iSP = rs.value("sp").convert<uint64_t>();
			client->m_iEnemyKillCount = rs.value("ek").convert<int32_t>();
			client->playerKillCount = rs.value("pk").convert<int32_t>();
			client->level = rs.value("level").convert<uint32_t>();
			client->experience = rs.value("exp").convert<uint64_t>();
			client->SetStr(rs.value("strength").convert<int32_t>(), false);
			client->SetVit(rs.value("vitality").convert<int32_t>());
			client->SetDex(rs.value("dexterity").convert<int32_t>());
			client->SetInt(rs.value("intelligence").convert<int32_t>(), false);
			client->SetMag(rs.value("magic").convert<int32_t>());
			client->SetAgi(rs.value("agility").convert<int32_t>());
			client->m_iLuck = rs.value("luck").convert<uint16_t>();
			client->m_iRewardGold = rs.value("rewardgold").convert<int32_t>();
			client->m_iHungerStatus = rs.value("hunger").convert<int32_t>();
			client->m_iAdminUserLevel = rs.value("adminlevel").convert<int32_t>();
			client->m_iTimeLeft_ShutUp = rs.value("leftshutuptime").convert<uint64_t>();
			client->m_iTimeLeft_Rating = rs.value("leftpoptime").convert<uint64_t>();
			client->m_reputation = rs.value("popularity").convert<int32_t>();
			client->guildGUID = rs.value("guildid").convert<uint64_t>();
			//client->m_iDownSkillIndex = rs.value("DownSkillID").convert<int16_t>();//use?
			client->charid = rs.value("charid").convert<uint64_t>();
			client->m_sCharIDnum1 = rs.value("id1").convert<int16_t>();
			client->m_sCharIDnum2 = rs.value("id2").convert<int16_t>();
			client->m_sCharIDnum3 = rs.value("id3").convert<int16_t>();
			client->m_iQuest = rs.value("questnum").convert<int32_t>();
			client->m_iCurQuestCount = rs.value("questcount").convert<int32_t>();
			client->m_iQuestRewardType = rs.value("questrewardtype").convert<int32_t>();
			client->m_iQuestRewardAmount = rs.value("questrewardamount").convert<int32_t>();
			client->contribution = rs.value("contribution").convert<int32_t>();
			client->m_iQuestID = rs.value("questid").convert<int32_t>();
			client->m_bIsQuestCompleted = rs.value("questcompleted").convert<bool>();
			client->m_iTimeLeft_ForceRecall = rs.value("leftforcerecalltime").convert<uint64_t>();
			client->m_iTimeLeft_FirmStamina = rs.value("leftfirmstaminatime").convert<uint64_t>();
			client->m_iSpecialEventID = rs.value("eventid").convert<int32_t>();
			client->superAttack = rs.value("leftsac").convert<int32_t>();
			client->arenaNumber = rs.value("fightnum").convert<int32_t>();
			client->arenaReserveTime = rs.value("fightdate").convert<uint64_t>();
			client->arenaTicketNumber = rs.value("fightticket").convert<int32_t>();
			client->m_iSpecialAbilityTime = rs.value("leftspectime").convert<uint64_t>();
			client->crusadeContribution = rs.value("warcon").convert<int32_t>();
			client->crusadeDuty = rs.value("crujob").convert<int32_t>();
			client->crusadePoint = rs.value("cruconstructpoint").convert<int32_t>();
			client->crusadeGUID = rs.value("cruid").convert<uint64_t>();
			client->deadPenaltyTime = rs.value("leftdeadpenaltytime").convert<uint64_t>();
			uint64_t partyid = rs.value("partyid").convert<uint64_t>();
			// 					if (partyid && partyMgr.PartyExists(partyid))
			// 					{
			// 						client->SetParty(partyMgr.GetParty(partyid));
			// 
			// 					}
			client->m_iGizonItemUpgradeLeft = rs.value("gizonitemupgradeleft").convert<int32_t>();
			client->m_elo = rs.value("elo").convert<int32_t>();
			client->m_iEnemyKillTotalCount = rs.value("totalek").convert<int32_t>();
			//client->m_iLucky = rs.value("Lucky").convert<uint32_t>();//not in DB
			//client->m_iMonsterCount = rs.value("MonsterCount").convert<uint32_t>();//not in DB


			if (client->gender == MALE) client->SetType(1);
			else if (client->gender == FEMALE) client->SetType(4);
			client->SetType(client->Type() + client->colorSkin - 1);
			client->m_sAppr1 = (client->m_cHairStyle << 8) | (client->colorHair << 4) | client->colorUnderwear;
		}
		

		{
			Session ses(sqlpool->get());
			Statement select(ses);
			select << "SELECT currency_id,count FROM char_currency WHERE char_id=?;", use(client->charid), now;
			RecordSet rs(select);

			uint32_t rowcount = rs.rowCount();

			if (rowcount > 0)
			{
				rs.moveFirst();
				Client::stCurrency currency;

				while (rs.moveNext())
				{
					currency._id = rs.value("currency_id").convert<int64_t>();
					currency._count = rs.value("count").convert<int64_t>();
					client->currency.push_back(currency);
				}
			}
		}

		return true;
	}
	SQLCATCH(/*DeleteClient(client, true);*/ return false)
	catch (SessionPoolExhaustedException & e)
	{
		poco_error(*logger, "SessionPoolExhaustedException - CheckLogin()");
	}
	return false;
}
//how to delete client?
//void Server::DeleteClient(Client * client,
//							bool save,//save player data
//							bool notify,//notify surrounding players
//							bool countlogout,//??
//							bool forceclose)//apply bleeding isle "ban" effect
//void Server::DeleteClient(int iClientH, bool bSave, bool bNotify, bool bCountLogout, bool bForceCloseConn)

// DeleteClient only closes the socket with all intents and purposes of leaving
// their character in game unless they are not fully logged in, in which case this
// would kill the client object as well unless param is passed to also delete the object
// a proper logout would get the deleteobj flag passed
void GServer::DeleteClient(shared_ptr<Client> client, bool save, bool deleteobj)
{
	string cTmpMap;
	bool previouslyDCd = false;

	if (!client) return;

	gate->mutclientlist.lock();

	client->socket->stop();

// 	if(client->m_iGuildGUID != -1)
// 	{
// 		if(client->m_guild)
// 			client->m_guild->MemberLogout(player);
// 		else
// 			guildMgr.RemovePendingMember(player);
// 	}

	if (client->m_bIsInitComplete == true)
	{
// 		if (memcmp(player->m_cMapName, "fight", 5) == 0) {
// 			wsprintf(g_cTxt, "Char(%s)-Exit(%s)", player->m_cCharName, player->m_cMapName);
// 			PutLogFileList(g_cTxt,EVENT_LOGFILE);
// 		}

		//in a trade
// 		if (client->m_isExchangeMode == true) {
// 			iExH = client->m_exchangeH;
// 			_ClearExchangeStatus(iExH);
// 			_ClearExchangeStatus(iClientH);
// 		}


		//what is this for?
// 		if ((client->m_iAllocatedFish != NULL) && (m_pFish[player->m_iAllocatedFish] != NULL))
// 			m_pFish[client->m_iAllocatedFish]->m_sEngagingCount--;

		//shouldn't it always notify unless not logged in?
// 		if (bNotify == true)
// 			SendEventToNearClient_TypeA(iClientH, OWNERTYPE_PLAYER, MSGID_MOTION_EVENT_REJECT, NULL, NULL, NULL);

		//make npcs stop chasing what is about to disappear (would only apply to instant logouts)
		//RemoveFromTarget(iClientH, OWNERTYPE_PLAYER);

		//what is this?
		//PlayerConnectionOff(iClientH); // Player Connection xRisenx

		for (shared_ptr<Client> target : clientlist)
		{
			if (target->whisperTarget.lock() == client)
			{
				target->whisperTarget.reset();
				SendNotifyMsg(0, target.get(), NOTIFY_WHISPERMODEOFF, 0, 0, 0, client->name);
				break;
			}
		}

		//removes them from the map - only should apply for instant removals - needs a copy in the delayed removal
// 		m_pMapList[client->m_cMapIndex]->ClearOwner(/*2,*/ iClientH, OWNERTYPE_PLAYER,
// 			client->m_sX,
// 			client->m_sY);


		RemoveFromDelayEventList(client.get(), 0);
	}


	//need to remove items from inventory on logout? Do it here.
	int index = client->HasItem(ITEM_RELIC);
	if(index != ITEM_NONE)
	{
		//DropItemHandler(client->m_handle, index, 1, client->m_pItemList[index]->m_cName, false);
	}

	if(save && !client->m_bIsOnServerChange)
	{
		if(client->_dead)
		{
			client->x = -1;
			client->y = -1;

			cTmpMap = client->mapName;

			client->mapName = "";

			if (client->IsNeutral()) {
				client->mapName = sideMap[NEUTRAL];
			}
			else {
				if (m_bIsCrusadeMode) {
					if (client->deadPenaltyTime > 0) {
						client->lockedMapName = sideMap[ client->side ];
						client->lockedMapTime = 60*5;
						client->deadPenaltyTime = 60*10;
					}
					else
					{
						client->deadPenaltyTime = 60*10;
					}
				}

				switch(client->side)
				{
				case ARESDEN:
					if ((cTmpMap == sideMap[ELVINE]) && !client->IsGM()){
						client->lockedMapName = sideMap[ELVINE];
						client->lockedMapTime = 60*3;
						client->mapName = sideMap[ELVINE];
					}
					else if (client->level > 80)
						client->mapName = sideMap[ARESDEN];
					else
						client->mapName = sideMap[ARESDEN];
					break;
				case ELVINE:
					if ((cTmpMap == sideMap[ARESDEN]) && !client->IsGM()){
						client->lockedMapName = sideMap[ARESDEN];
						client->lockedMapTime = 60*3;
						client->mapName = sideMap[ARESDEN];
					}
					else if (client->level > 80)
						client->mapName = sideMap[ELVINE];
					else
						client->mapName = sideMap[ELVINE];
					break;
				}
			}
		}
// 		else if(bForceCloseConn) {
//
// 			ZeroMemory(client->m_cMapName, sizeof(client->m_cMapName));
// 			memcpy(client->m_cMapName, "bisle", 5);
// 			client->m_sX = -1;
// 			client->m_sY = -1;
//
//
// 			ZeroMemory(client->m_cLockedMapName, sizeof(client->m_cLockedMapName));
// 			strcpy(client->m_cLockedMapName, "bisle");
//
// 			client->m_iLockedMapTime = 10*60;
// 		}


		if(client->observerMode/* || memcmp(client->m_cMapName, "astoria", 7) == 0 */)
		{
			client->mapName = sideMap[ client->side ];
			client->x = -1;
			client->y = -1;
		}

		//this ... is an absolutely retarded way to go about checking if people
		//are d/cing to stay on maps that have forced recall times. how about
		//put a new field in the db and have it be part of the character
		//of how much time they have left until a recall?
		//TODO: Do it right.
// 		if(client->m_cMapIndex >= 0 && !client->IsGM() &&
// 			(m_pMapList[client->m_cMapIndex]->m_bIsApocalypseMap ||
// 			m_pMapList[client->m_cMapIndex]->m_bIsFightZone))
// 		{
// 			for(std::list<RecentDisconnect>::iterator it = m_recentDCs.begin(); it != m_recentDCs.end(); ++it) {
// 				if(strcmp(it->playerName, player->m_cCharName) == 0)
// 				{
// 					if(it->dcCount >= 3){
// 						m_recentDCs.erase(it);
//
// 						ZeroMemory(client->m_cMapName, sizeof(client->m_cMapName));
// 						strcpy(client->m_cMapName, sideMap[ client->m_side ]);
// 						client->m_sX = -1;
// 						client->m_sY = -1;
// 					}
// 					else {
// 						it->dcCount++;
// 						for (i = 0; i < 3; i++)
// 							if (it->disconnectTimes[i] == 0){
// 								it->disconnectTimes[i] = unixtime();
// 								break;
// 							}
// 					}
// 					previouslyDCd = true;
// 					break;
// 				}
// 			}
//
// 			if(!previouslyDCd){
// 				m_recentDCs.push_back(RecentDisconnect(player->m_cCharName, unixtime()));
// 			}
//
// 		}
		// Gladiator Arena xRisenx
		//if(strcmp(m_pClientList[iClientH]->m_cMapName, cArenaMap) == 0)
		/*if(strcmp(player->m_cMapName, cArenaMap) == 0)
		{
		   RequestArenaStatus(iClientH, false);
		}*/
		// Gladiator Arena xRisenx

		//data won't save unless it's an instant logout. should delayed logout run through
		//this function again with flag deleteobj set? everything would be iterated twice
		//but if client was staying in game, it'd need to be checked a second time
		//save is already checked above
		if(deleteobj && client->m_bIsInitComplete)
		{
			//remove all outstanding messages coming in. client is being removed from server
			//regardless of what they are trying to do - socket has been closed so no more coming in
			//TODO: this needs testing. i know you can erase iters mid for loop, but i know there's a trick to it
			mutchat.lock();
			for (std::list<shared_ptr<MsgQueueEntry>>::iterator iter = chatpipe.begin(); iter != chatpipe.end();)
			{
				if (iter->get()->client == client)
				{
					chatpipe.erase(iter);
				}
				++iter;
			}
			mutchat.unlock();

// 			//savedata
// 			if(!bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATALOGOUT, iClientH, bCountLogout) )
// 				LocalSavePlayerData(iClientH);
// //#ifdef TitleHG
// 				LocalSavePlayerTitleData(iClientH);
// //#endif
		} else {
//			bSendMsgToLS(MSGID_REQUEST_NOSAVELOGOUT, iClientH, bCountLogout);
		}
	} else {
		//data not being saved
		//if anything was performed by the client in game, it should be saved unless
		//it was something that does not affect theirs or anyone else's data
		gate->clientlist.remove(client);
		clientlist.remove(client);
		// 		if (!client->m_bIsOnServerChange)
// 		{
// 			bSendMsgToLS(MSGID_REQUEST_NOSAVELOGOUT, iClientH, bCountLogout);
// 		} else {
// 			bSendMsgToLS(MSGID_REQUEST_SETACCOUNTWAITSTATUS, iClientH, false);
// 		}
	}

//	partyMgr.RemoveFromParty( player );

// 	if (client->m_bIsManager)
// 		m_iTotalClients--;

// 	objMgr.Remove( player );
// 	delete player;
// 	m_pClientList[iClientH] = NULL;
//
// 	RemoveClientShortCut(iClientH);
	gate->mutclientlist.unlock();
}

bool GServer::bCheckClientAttackFrequency(Client * client)
{
	uint64_t dwTimeGap;
	uint64_t dwTime = unixtime();

	if (client == nullptr) return false;

	if (client->IsGM())
		return true;

	if (client->m_dwAttackFreqTime == 0)
		client->m_dwAttackFreqTime = dwTime;
	else {
		dwTimeGap = dwTime - client->m_dwAttackFreqTime;
		client->m_dwAttackFreqTime = dwTime;

// #ifndef NO_MSGSPEEDCHECK_ATTACK
// 		if (dwTimeGap < 320) {
// 			logger->information(Poco::format("(!) Speed hack suspect(%s) - attack(%?d)", client->name, dwTimeGap));
// 			return false;
// 		}
// 		else if (dwTimeGap < 240) {
// 			logger->information(Poco::format("(!) Speed hack suspect(%s) - attack(%?d). Disconnected", client->name, dwTimeGap));
// 			DeleteClient(client->self.lock(), true, true);
// 			return false;
// 		}
// #endif
	}

	return true;
}

//TODO: not all magic will have a cast time
bool GServer::bCheckClientMagicFrequency(Client * client)
{
	uint64_t dwTimeGap;
	uint64_t dwTime = unixtime();

	if (client == nullptr) return false;

	if (client->IsGM())
		return true;

	if (client->m_dwMagicFreqTime == 0)
		client->m_dwMagicFreqTime = dwTime;
	else {
		dwTimeGap = dwTime - client->m_dwMagicFreqTime;
		client->m_dwMagicFreqTime = dwTime;

#ifndef NO_MSGSPEEDCHECK_MAGIC
		if (dwTimeGap < 1200) {
			logger->information(Poco::format("(-~-HACKING-~-) Speed hacker detected(%s) - magic(%?d). Disconnected", client->name, dwTimeGap));
			DeleteClient(client->self.lock(), true, true);
			return false;
		}
		else if (dwTimeGap < 1600) {
			logger->information(Poco::format("(-~-HACKING-~-) Speed hack suspect(%s) - magic(%?d)", client->name, dwTimeGap));
			return false;
		}
#endif
	}

	return true;
}

bool GServer::bCheckClientMoveFrequency(Client * client, bool running)
{
	uint64_t dwTimeGap;
	uint64_t dwTime = unixtime();

	if (!client) return false;

	if (client->IsGM())
		return true;

	if (client->m_dwMoveFreqTime == 0)
		client->m_dwMoveFreqTime = dwTime;
	else {
		if (client->m_resetMoveFreq == true) {
			client->m_dwMoveFreqTime = 0;
			client->m_resetMoveFreq = false;
			return true;
		}

		if (client->m_bIsAttackModeChange == true) {
			client->m_dwMoveFreqTime = 0;
			client->m_bIsAttackModeChange = false;
			return true;
		}

		dwTimeGap = dwTime - client->m_dwMoveFreqTime;
		client->m_dwMoveFreqTime = dwTime;

		if (running)
		{
			if (dwTimeGap < 30) dwTimeGap = 300;
			client->m_runTime[client->m_runTurn] = dwTimeGap;

#ifdef DOSPEEDCHECK
			if (client->m_iAdminUserLevel == 0) {
				uint64_t sum = 0;
				for (int i = 0; i < SPEEDCHECKTURNS; i++)
					sum += client->m_runTime[i];

				if (sum < 210 * SPEEDCHECKTURNS)
				{
					logger->information(Poco::format("(-~-HACKING-~-) Speed hacker detected(%s) - run-avg(%?d). BI banned", client->name, sum / SPEEDCHECKTURNS));

					DeleteClient(client->self.lock(), true, true);
					return false;
				}
				else if (sum < 230 * SPEEDCHECKTURNS)
				{
					logger->information(Poco::format("(-~-HACKING-~-) Speed hack suspect(%s) - run-avg(%?d)", client->name, sum / SPEEDCHECKTURNS));

					++client->m_runTurn %= SPEEDCHECKTURNS;
					return false;
				}
				++client->m_runTurn %= SPEEDCHECKTURNS;
			}
#endif
		}
		else{
			if (dwTimeGap < 30) dwTimeGap = 540;
			client->m_moveTime[client->m_moveTurn] = dwTimeGap;
#ifdef DOSPEEDCHECK
			if (client->m_iAdminUserLevel == 0) {
				uint64_t sum = 0;
				for (int i = 0; i < SPEEDCHECKTURNS; i++)
					sum += client->m_moveTime[i];

				/*if(sum < 330*SPEEDCHECKTURNS)
				{
				wsprintf(g_cTxt, "(-~-HACKING-~-) Speed hacker detected(%s) - move-avg(%i). BI banned", player->m_cCharName, sum/SPEEDCHECKTURNS);
				PutLogList(g_cTxt);

				DeleteClient(iClientH, true, true, true, true);
				return false;
				}else */if (sum < 350 * SPEEDCHECKTURNS)
				{
					logger->information(Poco::format("(-~-HACKING-~-) Speed hack suspect(%s) - move-avg(%?d)", client->name, sum / SPEEDCHECKTURNS));

					++client->m_moveTurn %= SPEEDCHECKTURNS;
					return false;
				}
				++client->m_moveTurn %= SPEEDCHECKTURNS;
			}
#endif
		}
	}

	return true;
}

void GServer::_CheckAttackType(Client * client, int16_t & spType)
{
	uint16_t wType;

	if (client == nullptr) return;
	wType = ((client->m_sAppr2 & 0x0FF0) >> 4);

	switch (spType) {
	case 2:
		if (client->Equipped.Arrow == nullptr) spType = 0;
		if (wType < 40) spType = 1;
		break;

	case 20:
		if (client->superAttack <= 0)  spType = 1;
		if (client->m_cSkillMastery[SKILL_HANDATTACK] < 100) spType = 1;
		break;

	case 21:

		if (client->superAttack <= 0)  spType = 1;
		if (client->m_cSkillMastery[SKILL_SHORTSWORD] < 100) spType = 1;
		break;

	case 22:

		if (client->superAttack <= 0)  spType = 1;
		if (client->m_cSkillMastery[SKILL_FENCING] < 100) spType = 1;
		break;

	case 23:
		if (client->superAttack <= 0)   spType = 1;
		if (client->m_cSkillMastery[SKILL_LONGSWORD] < 100) spType = 1;
		break;

	case 24:
		if (client->superAttack <= 0)  spType = 1;
		if (client->m_cSkillMastery[SKILL_AXE] < 100) spType = 1;
		break;

	case 25:
		if (client->superAttack <= 0)  spType = 2;
		if (client->m_cSkillMastery[SKILL_ARCHERY] < 100) spType = 2;
		if (client->Equipped.Arrow == nullptr)      spType = 0;
		if (wType < 40) spType = 1;
		break;
	case 26:

		if (client->superAttack <= 0)  spType = 1;
		if (client->m_cSkillMastery[SKILL_HAMMER] < 100) spType = 1;
		break;

	case 27:

		if (client->superAttack <= 0)  spType = 1;
		if (client->m_cSkillMastery[SKILL_STAFF] < 100) spType = 1;
		break;
	}
}

void GServer::ClearSkillUsingStatus(Client * client)
{
	int i;

	if (client == nullptr) return;

	//m_pClientList[iClientH]->m_hasPrecasted = false;
	for (i = 0; i < MAXSKILLTYPE; i++) {
		client->skillInUse[i] = false;
		client->skillInUseTime[i] = 0;
	}

	//TODO: fishing
// 	if (client->m_iAllocatedFish != 0) {
// 		if (m_pFish[client->m_iAllocatedFish] != 0)
// 			m_pFish[client->m_iAllocatedFish]->m_sEngagingCount--;
// 
// 		client->m_iAllocatedFish = NULL;
// 		SendNotifyMsg(nullptr, client, NOTIFY_FISHCANCELED, 0, 0, 0);
// 	}
}

void GServer::Effect_Damage_Spot(Unit * attacker, Unit * target, short sV1, short sV2, short sV3, bool exp, int32_t attr)
{
	int32_t damage = dice(sV1, sV2) + sV3;
	int64_t dwtime = unixtime();
	if (damage < 0) damage = 0;

	if (attacker->IsPlayer())
	{
		Client * player = (Client*)attacker;
		if (!m_bIsCrusadeMode && player->m_bIsHunter && target->IsPlayer()) return;//no damage to civilians outside of crusade (default)
		//TODO: player damage calculations here
	}
	else
	{
		//TODO: for sake of testing

	}

	if (target->IsPlayer())
	{
		Client * player = (Client*)target;
		if (!player->m_bIsInitComplete) return; //cannot attack players not fully loaded in
		if (player->_dead) return;

		if (!m_bIsCrusadeMode && player->playerKillCount == 0 && player->m_bIsHunter && attacker->IsPlayer()) return;

		ClearSkillUsingStatus(player);

		//TODO: elemental absorption code that was never actually fully implemented
		/*switch (attr)
		{
		case 1:
			if (m_pClientList[sTargetH]->m_iAddAbsEarth != 0) {
				dTmp1 = (double)iDamage;
				dTmp2 = (double)m_pClientList[sTargetH]->m_iAddAbsEarth;
				dTmp3 = (dTmp2 / 100.0f)*dTmp1;
				iDamage = iDamage - (int)(dTmp3);
				if (iDamage < 0) iDamage = 0;
			}
			break;

		case 2:
			if (m_pClientList[sTargetH]->m_iAddAbsAir != 0) {
				dTmp1 = (double)iDamage;
				dTmp2 = (double)m_pClientList[sTargetH]->m_iAddAbsAir;
				dTmp3 = (dTmp2 / 100.0f)*dTmp1;
				iDamage = iDamage - (int)(dTmp3);
				if (iDamage < 0) iDamage = 0;
			}
			break;

		case 3:
			if (m_pClientList[sTargetH]->m_iAddAbsFire != 0) {
				dTmp1 = (double)iDamage;
				dTmp2 = (double)m_pClientList[sTargetH]->m_iAddAbsFire;
				dTmp3 = (dTmp2 / 100.0f)*dTmp1;
				iDamage = iDamage - (int)(dTmp3);
				if (iDamage < 0) iDamage = 0;
			}
			break;

		case 4:
			if (m_pClientList[sTargetH]->m_iAddAbsWater != 0) {
				dTmp1 = (double)iDamage;
				dTmp2 = (double)m_pClientList[sTargetH]->m_iAddAbsWater;
				dTmp3 = (dTmp2 / 100.0f)*dTmp1;
				iDamage = iDamage - (int)(dTmp3);
				if (iDamage < 0) iDamage = 0;
			}
			break;
		}*/

		damage -= dice(1, player->m_iVit / 10) - 1;
		if (damage < 0) damage = 0;

		player->health -= damage;
	}
	else
	{
	}
}

bool GServer::CheckResistingMagicSuccess(char cAttackerDir, Unit * target, int iHitRatio)
{
	double dTmp1, dTmp2, dTmp3;
	int    iTarGetMagicResistRatio, iDestHitRatio, iResult;
	char   cTargetDir, cProtect;

	if (target == nullptr)
		return false;
	if (target->IsPlayer())
	{
		Client * client = static_cast<Client *>(target);
		if (client->IsInvincible()) return true;

		cTargetDir = client->direction;
		iTarGetMagicResistRatio = client->m_cSkillMastery[SKILL_MAGICRES] + client->m_iAddMR;

		if (client->GetMag() > 50)
			iTarGetMagicResistRatio += (client->GetMag() - 50);

		iTarGetMagicResistRatio += client->m_iAddResistMagic;
		cProtect = client->magicEffectStatus[MAGICTYPE_PROTECT];
		if ((client->status & STATUS_REDSLATE) != 0) return true;
	}
	else
	{
		Npc * npc = static_cast<Npc *>(target);
		cTargetDir = npc->direction;
		iTarGetMagicResistRatio = npc->m_cResistMagic;
		cProtect = npc->magicEffectStatus[MAGICTYPE_PROTECT];
	}

	if (cProtect == MAGICPROTECT_AMP) return true;
	// Changed iHitRatio 1000 -> 10000 to fix the hit through pfm xRisenx / Vamp
	if (iHitRatio < 10000 && cProtect == MAGICPROTECT_PFM) return true;
	//if (iHitRatio < 1000 && cProtect == MAGICPROTECT_PFM) return true;
	//if (iHitRatio < 1000 && cProtect == MAGICPROTECT_GPFM) return true;
	if (iHitRatio >= 10000) iHitRatio -= 10000;

	if (iTarGetMagicResistRatio < 1) iTarGetMagicResistRatio = 1;

	dTmp1 = (double)(iHitRatio);
	dTmp2 = (double)(iTarGetMagicResistRatio);

	dTmp3 = (dTmp1 / dTmp2) * MAGICHITRATIO; // Magic Hit Ratio
	iDestHitRatio = (int)(dTmp3);

	if (iDestHitRatio < MINIMUMHITRATIO) iDestHitRatio = MINIMUMHITRATIO;
	if (iDestHitRatio > MAXIMUMHITRATIO) iDestHitRatio = MAXIMUMHITRATIO;

	if (iDestHitRatio >= 100) return false;

	iResult = dice(1, 100);
	if (iResult <= iDestHitRatio) return false;

	// Resisting Magic
	if (target->IsPlayer())
		CalculateSSN_SkillIndex(static_cast<Client *>(target), SKILL_MAGICRES, 1);

	return true;
}

bool GServer::CheckResistingPoisonSuccess(Unit * owner)
{
	int iResist, iResult;

	if (owner == nullptr)
		return false;

	if (owner->IsPlayer())
	{
		Client * client = static_cast<Client *>(owner);
		iResist = client->m_cSkillMastery[SKILL_POISONRES] + client->m_iAddPR;
	}
	else if (owner->IsNPC())
	{
		iResist = 0;
	}

	iResult = dice(1, 100);
	if (iResult >= iResist)
		return false;

	if (owner->IsPlayer())
		CalculateSSN_SkillIndex(static_cast<Client *>(owner), SKILL_POISONRES, 1);

	return true;
}

bool GServer::CheckResistingIceSuccess(char cAttackerDir, Unit * target, int iHitRatio)
{

	int    iTargetIceResistRatio, iResult;

	if (target == nullptr)
		return false;

	if (target->IsPlayer())
	{
		Client * client = static_cast<Client *>(target);

		if (client->IsInvincible()) return true;

		iTargetIceResistRatio = client->IceResist();

		if (client->m_dwWarmEffectTime == 0) {
		}
		else if ((unixtime() - client->m_dwWarmEffectTime) < 1000 * 30) return true;
	}
	else if (target->IsNPC())
	{
		Npc * npc = static_cast<Npc *>(target);
		if (npc->element == ELEMENT_WATER) return true;
		iTargetIceResistRatio = (npc->m_cResistMagic) - (npc->m_cResistMagic / 3);
	}

	if (iTargetIceResistRatio < 1) iTargetIceResistRatio = 1;

	iResult = dice(1, 100);
	if (iResult <= iTargetIceResistRatio) return true;

	return false;
}

void GServer::CalculateSSN_SkillIndex(Client * client, short sSkillIndex, int iValue)
{
	int   iOldSSN, iSSNpoint, iWeaponIndex;

	if (client == nullptr) return;
	if (client->m_bIsInitComplete == false) return;
	if ((sSkillIndex < 0) || (sSkillIndex >= MAXSKILLTYPE)) return;
	if (client->_dead == true) return;

	if (client->m_cSkillMastery[sSkillIndex] == 0) return;

	switch (sSkillIndex) {
	case SKILL_MAGIC:
		iValue *= 13;
		break;
	case SKILL_HANDATTACK:
	case SKILL_ARCHERY:
	case SKILL_SHORTSWORD:
	case SKILL_LONGSWORD:
	case SKILL_FENCING:
	case SKILL_AXE:
	case SKILL_HAMMER:
	case SKILL_STAFF:
	case SKILL_PRETENDCORPSE:
		//case SKILL_MAGICRES:
		//case SKILL_SHIELD:
		//case SKILL_POISONRES:
		iValue *= 15;
		break;

	case SKILL_SHIELD:
	case SKILL_MAGICRES:
	case SKILL_POISONRES:
		iValue *= 3;
		break;
		//case SKILL_FARMING:
		//case SKILL_MANUFACTURING:
		//case SKILL_ALCHEMY:
		//case SKILL_MINING:
		//case SKILL_CRAFTING:
		//case SKILL_FISHING:
		//iValue *= 3;
		//break;
		/*case SKILL_CRAFTING:
		iValue *= 20;
		break;*/
	}

	iOldSSN = client->m_iSkillSSN[sSkillIndex];
	client->m_iSkillSSN[sSkillIndex] += iValue;

	iSSNpoint = m_iSkillSSNpoint[client->m_cSkillMastery[sSkillIndex] + 1];

	if ((client->m_cSkillMastery[sSkillIndex] < 100) &&
		(client->m_iSkillSSN[sSkillIndex] > iSSNpoint)) {

		client->m_cSkillMastery[sSkillIndex]++;
		switch (sSkillIndex)
		{
		case SKILL_MINING:
		case SKILL_HANDATTACK:
		case SKILL_MANUFACTURING:
			if (client->m_cSkillMastery[sSkillIndex] > (client->GetStr() * 2)) {
				client->m_cSkillMastery[sSkillIndex]--;
				client->m_iSkillSSN[sSkillIndex] = iOldSSN;
			}
			else client->m_iSkillSSN[sSkillIndex] = 0;
			break;

		case SKILL_MAGICRES:
			if (client->m_cSkillMastery[sSkillIndex] > (client->level * 2)) {
				client->m_cSkillMastery[sSkillIndex]--;
				client->m_iSkillSSN[sSkillIndex] = iOldSSN;
			}
			else client->m_iSkillSSN[sSkillIndex] = 0;
			break;

		case SKILL_MAGIC:
		case SKILL_STAFF:
		case SKILL_CRAFTING:
			if (client->m_cSkillMastery[sSkillIndex] > (client->GetMag() * 2)) {
				client->m_cSkillMastery[sSkillIndex]--;
				client->m_iSkillSSN[sSkillIndex] = iOldSSN;
			}
			else client->m_iSkillSSN[sSkillIndex] = 0;
			break;

		case SKILL_FISHING:
		case SKILL_ARCHERY:
		case SKILL_SHORTSWORD:
		case SKILL_LONGSWORD:
		case SKILL_FENCING:
		case SKILL_AXE:
		case SKILL_SHIELD:
		case SKILL_HAMMER:
			if (client->m_cSkillMastery[sSkillIndex] > (client->GetDex() * 2)) {
				client->m_cSkillMastery[sSkillIndex]--;
				client->m_iSkillSSN[sSkillIndex] = iOldSSN;
			}
			else client->m_iSkillSSN[sSkillIndex] = 0;
			break;

		case SKILL_FARMING:
		case SKILL_ALCHEMY:
			//case SKILL_15:
		case SKILL_PRETENDCORPSE:
			if (client->m_cSkillMastery[sSkillIndex] > (client->GetInt() * 2)) {
				client->m_cSkillMastery[sSkillIndex]--;
				client->m_iSkillSSN[sSkillIndex] = iOldSSN;
			}
			else client->m_iSkillSSN[sSkillIndex] = 0;
			break;

		case SKILL_POISONRES:
			if (client->m_cSkillMastery[sSkillIndex] > (client->m_iVit * 2)) {
				client->m_cSkillMastery[sSkillIndex]--;
				client->m_iSkillSSN[sSkillIndex] = iOldSSN;
			}
			else client->m_iSkillSSN[sSkillIndex] = 0;
			break;

		default:
			client->m_iSkillSSN[sSkillIndex] = 0;
			break;
		}

		if (client->m_iSkillSSN[sSkillIndex] == 0) {
			if (client->Equipped.TwoHand != nullptr) {
				if (client->Equipped.RightHand->m_sRelatedSkill == sSkillIndex) {
					client->m_iHitRatio++;
				}
			}

			if (client->Equipped.RightHand != nullptr) {
				if (client->Equipped.RightHand->m_sRelatedSkill == sSkillIndex) {
					client->m_iHitRatio++;
				}
			}
		}

		if (client->m_iSkillSSN[sSkillIndex] == 0) {
			client->CheckTotalSkillMasteryPoints(sSkillIndex);

			SendNotifyMsg(nullptr, client, NOTIFY_SKILL, sSkillIndex, client->m_cSkillMastery[sSkillIndex], 0);
		}
	}
}

void GServer::CalculateSSN_ItemIndex(Client * client, Item * Weapon, int iValue)
{
	short sSkillIndex;

	if (client == nullptr) return;
	if (client->m_bIsInitComplete == false) return;
	if (Weapon == nullptr) return;
	if (client->_dead == true) return;

	sSkillIndex = Weapon->m_sRelatedSkill;

	CalculateSSN_SkillIndex(client, sSkillIndex, iValue);

}

int32_t GServer::CalculateAttackEffect(Unit * target, Unit * attacker, int tdX, int tdY, int iAttackMode, bool bNearAttack /*= false*/, bool bIsDash /*= false*/)
{
	int    iAP_SM=0, iAP_L=0, iAttackerHitRatio=0, iTargetDefenseRatio=0, iDestHitRatio=0, iResult=0, iAP_Abs_Armor=0, iAP_Abs_Shield=0;
	char   cAttackerDir=0, cAttackerSide=0, cTargetDir=0, cProtect=0;
	string AttackerName;
	short  sWeaponIndex=0, sAttackerWeapon=0, dX=0, dY=0, sX=0, sY=0, sAtkX=0, sAtkY=0, sTgtX=0, sTgtY=0;
	uint64_t  dwTime = unixtime();
	uint16_t   wWeaponType=0;
	double dTmp1=0, dTmp2=0, dTmp3=0;
	bool   bKilled = false;
	bool   bNormalMissileAttack = false;
	bool   bIsAttackerBerserk=false;
	int32_t iKilledDice, iDamage, iExp, iWepLifeOff, iSideCondition, /*iMaxSuperAttack,*/ iWeaponSkill, iComboBonus, iTemp, iTemp2;
	int32_t iAttackerHP, iMoveDamage, iRepDamage;
	char   cAttackerSA;
	int32_t iAttackerSAvalue, iHitPoint;
	char   cDamageMoveDir;
	int32_t iPartyID, iConstructionPoint, iWarContribution, tX, tY, iDst1, iDst2;
	int32_t	iSkillLV, iCropLimit;
	int16_t sItemIndex;

	Client * cattacker = static_cast<Client*>(attacker);
	Client * ctarget = static_cast<Client*>(target);
	Npc * nattacker = static_cast<Npc*>(attacker);
	Npc * ntarget = static_cast<Npc*>(target);

	iExp = 0;
	iPartyID = 0;

	cAttackerSA = 0;
	iAttackerSAvalue = 0;
	wWeaponType = 0;

	if ((target == nullptr) || (attacker == nullptr))
		return 0;

	if (attacker->map->flags.attackEnabled == false) return 0;

	if (target->_dead == true) return 0;


	if (attacker->IsPlayer())
	{
		if ((cattacker->status & STATUS_INVISIBILITY) != 0) {
			cattacker->SetMagicFlag(MAGICTYPE_INVISIBILITY, false);

			RemoveFromDelayEventList(attacker, MAGICTYPE_INVISIBILITY);
			cattacker->magicEffectStatus[MAGICTYPE_INVISIBILITY] = 0;
		}

		if ((cattacker->m_sAppr2 & 0xF000) == 0) return 0;

		iAP_SM = 0;
		iAP_L = 0;

		wWeaponType = ((cattacker->m_sAppr2 & 0x0FF0) >> 4);
		// Battle Mages xRisenx
		if ((wWeaponType == 35 && cattacker->Equipped.TwoHand != nullptr)
			&& (cattacker->Equipped.TwoHand->m_sItemEffectType == ITEMEFFECTTYPE_ATTACK_MAGICITEM)) {
			return 0;
		}
		// Battle Mages xRisenx
		cAttackerSide = cattacker->side;

		if (wWeaponType == 0) {
			iAP_SM = iAP_L = dice(1, (cattacker->GetStr() / 12));
			if (iAP_SM <= 0) iAP_SM = 1;
			if (iAP_L <= 0) iAP_L = 1;
			iAttackerHitRatio = cattacker->m_iHitRatio + cattacker->m_cSkillMastery[SKILL_HANDATTACK];

			cattacker->m_sUsingWeaponSkill = SKILL_HANDATTACK;

			if (cattacker->HasPartyRaidBonus()) //TODO: party bonus for hand attack?
			{
				iAttackerHitRatio += 40;
				iAP_SM += 10;
				iAP_L += 10;
			}
		}
		else if ((wWeaponType >= 1) && (wWeaponType < 40)) {

			iAP_SM = dice(cattacker->m_cAttackDiceThrow_SM, cattacker->m_cAttackDiceRange_SM);
			iAP_L = dice(cattacker->m_cAttackDiceThrow_L, cattacker->m_cAttackDiceRange_L);


			iAP_SM += cattacker->m_cAttackBonus_SM;
			iAP_L += cattacker->m_cAttackBonus_L;

			if (cattacker->HasPartyRaidBonus()) //TODO: party bonus?
			{
				//iAttackerHitRatio += 100;
				iAP_SM += 50;
				iAP_L += 50;
			}

			iAttackerHitRatio = cattacker->m_iHitRatio;


			dTmp1 = (double)iAP_SM;
			if (cattacker->GetStr() <= 0)
				dTmp2 = 1.0f;
			else dTmp2 = (double)cattacker->GetStr();

			dTmp2 = dTmp2 / 5.0f;
			dTmp3 = dTmp1 + (dTmp1 * (dTmp2 / 100.0f));
			iAP_SM = (int)(dTmp3 + 0.5f);

			dTmp1 = (double)iAP_L;
			if (cattacker->GetStr() <= 0)
				dTmp2 = 1.0f;
			else dTmp2 = (double)cattacker->GetStr();

			dTmp2 = dTmp2 / 5.0f;
			dTmp3 = dTmp1 + (dTmp1 * (dTmp2 / 100.0f));
			iAP_L = (int)(dTmp3 + 0.5f);
			//
		}
		else if (wWeaponType >= 40) {
			iAP_SM = dice(cattacker->m_cAttackDiceThrow_SM, cattacker->m_cAttackDiceRange_SM);
			iAP_L = dice(cattacker->m_cAttackDiceThrow_L, cattacker->m_cAttackDiceRange_L);


			iAP_SM += cattacker->m_cAttackBonus_SM;
			iAP_L += cattacker->m_cAttackBonus_L;

			iAttackerHitRatio = cattacker->m_iHitRatio;
			bNormalMissileAttack = true;


			iAP_SM += dice(1, (cattacker->GetStr() / 20));
			iAP_L += dice(1, (cattacker->GetStr() / 20));
			//
		}

		iAttackerHitRatio += 50;
		if (iAP_SM <= 0) iAP_SM = 1;
		if (iAP_L <= 0) iAP_L = 1;

		if (cattacker->m_iCustomItemValue_Attack != 0) {

			if ((cattacker->m_iMinAP_SM != 0) && (iAP_SM < cattacker->m_iMinAP_SM)) {
				iAP_SM = cattacker->m_iMinAP_SM;
			}
			if ((cattacker->m_iMinAP_L != 0) && (iAP_L < cattacker->m_iMinAP_L)) {
				iAP_L = cattacker->m_iMinAP_L;
			}

			if ((cattacker->m_iMaxAP_SM != 0) && (iAP_SM > cattacker->m_iMaxAP_SM)) {
				iAP_SM = cattacker->m_iMaxAP_SM;
			}
			if ((cattacker->m_iMaxAP_L != 0) && (iAP_L > cattacker->m_iMaxAP_L)) {
				iAP_L = cattacker->m_iMaxAP_L;
			}
		}
		//TODO: fantasy "monster event"
		if (cattacker->m_heroArmourBonus == 1) {
			// 			if (cattacker->m_iMonsterCount >= 10)
			// 			{
			// 				cattacker->m_iMonsterCount / 10;
			// 				iAP_SM += cattacker->m_iMonsterCount / 10;
			// 				iAP_L += cattacker->m_iMonsterCount / 10;
			// 			}
			iAttackerHitRatio += 100;
			iAP_SM += 5;
			iAP_L += 5;
		}

		//if (g_game->m_pClientList[sAttackerH]->m_iMonsterCount > 0) 
		//	{
		//       iAP_SM = g_game->m_pClientList[sAttackerH]->m_iMonsterCount/20; // Change from 10 to 20.
		//	iAP_L = g_game->m_pClientList[sAttackerH]->m_iMonsterCount/20; // Change from 10 to 20.
		//       iAP_SM += 5;  // Warrior
		//       iAP_L += 5;    // Warrior
		//	}

		//TODO: Do something with this?
		// New monster event xRisenx
		if (cattacker->m_heroArmourBonus == 1) { // Warrior Hero Set +50 Damage Per Hit xRisenx
			iAttackerHitRatio += 125;  // Warrior Hero Set +125 Hit Rate Per Hit xRisenx
			iAP_SM += 50;
			iAP_L += 50;
		}

		// New Ancient Warrior Hero Set xRisenx
		if (cattacker->m_AheroArmourBonus == 1) { // Warrior Ancient Hero Set +125 Damage Per Hit xRisenx
			iAttackerHitRatio += 200;  // Warrior Ancient Hero Set +200 Hit Rate Per Hit xRisenx
			iAP_SM += 125;
			iAP_L += 125;
		}
		// New Legendary Warrior Hero Set xRisenx
		if (cattacker->m_LheroArmourBonus == 1) { // Warrior Legendary Hero Set +300 Damage Per Hit xRisenx
			iAttackerHitRatio += 300;  // Warrior Legendary Hero Set +300 Hit Rate Per Hit xRisenx
			iAP_SM += 300;
			iAP_L += 300;
		}

		if (cattacker->Equipped.TwoHand != nullptr)
		{
			iTemp = cattacker->Equipped.TwoHand->m_sIDnum;
			if ((iTemp == ITEM_KLONESSAXE || iTemp == ITEM_KLONESSBLADE) && cattacker->m_reputation > 0)
			{
				iRepDamage = cattacker->m_reputation / 50;
				if (iRepDamage < 5) iRepDamage = 5;
				iAP_SM += iRepDamage;
				iAP_L += iRepDamage;
			}
			else if ((iTemp == ITEM_DKLONESSAXE || iTemp == ITEM_DKLONESSBLADE) && cattacker->m_reputation < 0)
			{
				iRepDamage = abs(cattacker->m_reputation) / 50;
				if (iRepDamage < 5) iRepDamage = 5;
				iAP_SM += iRepDamage;
				iAP_L += iRepDamage;
			}
			else if ((cattacker->Equipped.TwoHand->m_sIDnum == ITEM_DARKEXECUTOR) && (m_cDayOrNight == 2))
			{
				iAP_SM += 80;
				iAP_L += 80;
			}
			else if ((cattacker->Equipped.TwoHand->m_sIDnum == ITEM_LIGHTINGBLADE) && (m_cDayOrNight == 1))
			{
				iAP_SM += 80;
				iAP_L += 80;
			}
		}

		if (cattacker->Equipped.RightHand != nullptr)
		{
			iTemp = cattacker->Equipped.RightHand->m_sIDnum;
			if ((iTemp == ITEM_KLONESSESTERK) && cattacker->m_reputation > 0)
			{
				iRepDamage = cattacker->m_reputation / 50;
				if (iRepDamage < 5) iRepDamage = 5;
				iAP_SM += iRepDamage;
				iAP_L += iRepDamage;
			}
		}

		if (cattacker->Equipped.Necklace != nullptr)
		{
			iTemp = cattacker->Equipped.Necklace->m_sIDnum;
			if ((iTemp == ITEM_NECKLACEOFKLONESS) && cattacker->m_reputation > 0)
			{
				iRepDamage = cattacker->m_reputation / 100;
				if (iRepDamage < 5) iRepDamage = 5;
				iAP_SM += iRepDamage;
				iAP_L += iRepDamage;
			}
			if ((iTemp == ITEM_NECKLACEOFDKLONESS) && cattacker->m_reputation < 0)
			{
				iRepDamage = abs(cattacker->m_reputation) / 100;
				if (iRepDamage < 5) iRepDamage = 5;
				iAP_SM += iRepDamage;
				iAP_L += iRepDamage;
			}
		}

		if (cattacker->Equipped.RightHand != nullptr)
		{
			iTemp = cattacker->Equipped.RightHand->m_sIDnum;
			if ((iTemp == ITEM_DKLONESSESTERK) && cattacker->m_reputation < 0)
			{
				iRepDamage = abs(cattacker->m_reputation) / 50;
				if (iRepDamage < 5) iRepDamage = 5;
				iAP_SM += iRepDamage;
				iAP_L += iRepDamage;
			}
		}

		cAttackerDir = cattacker->direction;
		AttackerName = cattacker->name;

		bIsAttackerBerserk = cattacker->IsBerserked();

		if ((cattacker->superAttack > 0) && (iAttackMode >= 20)) {

			dTmp1 = (double)iAP_SM;
			dTmp2 = (double)cattacker->level;
			dTmp3 = dTmp2 / 100.0f;
			dTmp2 = dTmp1 * dTmp3;
			iTemp = (int)(dTmp2 + 0.5f);
			iAP_SM += iTemp;

			dTmp1 = (double)iAP_L;
			dTmp2 = (double)cattacker->level;
			dTmp3 = dTmp2 / 100.0f;
			dTmp2 = dTmp1 * dTmp3;
			iTemp = (int)(dTmp2 + 0.5f);
			iAP_L += iTemp;

			switch (cattacker->m_sUsingWeaponSkill)
			{
			case SKILL_ARCHERY:
				iAP_SM += (iAP_SM / 10); iAP_L += (iAP_L / 10);
				//iAttackerHitRatio  += 30;
				break;
			case SKILL_SHORTSWORD:
				iAP_SM *= 2; iAP_L *= 2;
				break;
			case SKILL_LONGSWORD:
				iAP_SM += (iAP_SM / 10); iAP_L += (iAP_L / 10);
				iAttackerHitRatio += 45;
				break;
			case SKILL_AXE:
				iAP_SM += (iAP_SM / 3); iAP_L += (iAP_L / 3);
				iAttackerHitRatio += 30;
				break;
			case SKILL_HAMMER:
				iAP_SM += (iAP_SM / 5); iAP_L += (iAP_L / 5);
				iAttackerHitRatio += 15;
				break;
			case SKILL_STAFF:
				iAP_SM += (iAP_SM / 5); iAP_L += (iAP_L / 5);
				iAttackerHitRatio += 50;
				break;
			}


			iAttackerHitRatio += 5; // Hit rate bonus from using Critcals xRisenx
			iAttackerHitRatio += cattacker->m_iCustomItemValue_Attack;
		}


		if (bIsDash == true) {

			iAttackerHitRatio += 20;

			switch (cattacker->m_sUsingWeaponSkill) {
			case SKILL_LONGSWORD:
				iAP_SM += (iAP_SM / 10); iAP_L += (iAP_L / 10);
				iAttackerHitRatio += 45;
				break;
			case SKILL_AXE:
				iAP_SM += (iAP_SM / 3); iAP_L += (iAP_L / 3);
				iAttackerHitRatio += 30;
				break;
			case SKILL_HAMMER:
				iAP_SM += (iAP_SM / 5); iAP_L += (iAP_L / 5);
				iAttackerHitRatio += 15;
				break;
			}
		}

		iAttackerHP = cattacker->health;

		iAttackerHitRatio += cattacker->m_iAddAR;

		sAtkX = cattacker->x;
		sAtkY = cattacker->y;

		if (cattacker->GetParty())
			iPartyID = cattacker->GetParty()->GetID();
	}
	else if (attacker->IsNPC())
	{
		if ((nattacker->status & STATUS_INVISIBILITY) != 0) {
			nattacker->SetMagicFlag(MAGICTYPE_INVISIBILITY, false);

			RemoveFromDelayEventList(attacker, MAGICTYPE_INVISIBILITY);
			nattacker->magicEffectStatus[MAGICTYPE_INVISIBILITY] = 0;
		}


		cAttackerSide = nattacker->side;

		iAP_SM = 0;
		iAP_L = 0;

		// Crusade
		if (nattacker->m_cAttackDiceThrow != 0)
			iAP_L = iAP_SM = dice(nattacker->m_cAttackDiceThrow, nattacker->m_cAttackDiceRange);

		iAttackerHitRatio = nattacker->m_iHitRatio;

		cAttackerDir = nattacker->direction;
		AttackerName = nattacker->name;

		if (nattacker->IsBerserked())
			bIsAttackerBerserk = true;
		else bIsAttackerBerserk = false;


		iAttackerHP = nattacker->health;


		cAttackerSA = nattacker->specialAbility;

		sAtkX = cattacker->x;
		sAtkY = cattacker->y;
	}

	switch (target->OwnerType())
	{
	case OWNERTYPE_PLAYER:
	{
#ifdef SAMESIDETOWNPROTECT

		if ((g_clientList[sTargetH]->m_iPKCount == 0) && (cAttackerType == OWNERTYPE_PLAYER) && (g_clientList[sTargetH]->m_side == cAttackerSide) && (g_clientList[sTargetH]->m_iIsOnTown == NONPK)) return 0;
#endif 
#ifdef BEGINNERTOWNPROTECT
		if ((cAttackerType == OWNERTYPE_PLAYER) && (g_clientList[sTargetH]->m_bIsNeutral == true)
			&& (g_clientList[sTargetH]->m_iPKCount == 0) && (g_clientList[sTargetH]->m_iIsOnTown == NONPK)) return 0;
#endif

		//if ((g_clientList[sTargetH]->m_sX != tdX) || (g_clientList[sTargetH]->m_sY != tdY)) return 0; // Fix for cannot hit moving targets with meele weps xRisenx // Anti Hack vs Criting hack

		if (ctarget->IsInvincible()) return 0;

		if ((attacker->IsPlayer()) && (cattacker->neutral == true) && (ctarget->playerKillCount == 0)) return 0;

		if (ctarget->GetParty() && ctarget->GetParty()->GetID() == iPartyID)
			return 0;

		if ((ctarget->status & STATUS_REDSLATE) != 0) return 0;

		cTargetDir = ctarget->direction;
		iTargetDefenseRatio = ctarget->m_iDefenseRatio;

		if ((attacker->IsPlayer()) && (cattacker->safeAttackMode == true)) {
			iSideCondition = cattacker->GetPlayerRelationship(ctarget);
			if ((iSideCondition == 7) || (iSideCondition == 2) || (iSideCondition == 6)) {

				iAP_SM = iAP_SM / 2;
				iAP_L = iAP_L / 2;
			}
			else {

				if (attacker->map->flags.fightZone == true) {
					if (cattacker->guildGUID == ctarget->guildGUID) return 0;
					else {

						iAP_SM = iAP_SM / 2;
						iAP_L = iAP_L / 2;
					}
				}
				else return 0;
			}
		}

		if (attacker->IsNPC() && ctarget->HasPartyHuntBonus())
		{
			iAP_SM *= 0.85;
			iAP_L *= 0.85;
		}

		iTargetDefenseRatio += ctarget->m_iAddDR;

		sTgtX = ctarget->x;
		sTgtY = ctarget->y;
	}
		break;
	case OWNERTYPE_NPC:
	{
		if (ntarget->health == 0) return 0;

		//if ((g_npcList[sTargetH]->m_sX != tdX) || (g_npcList[sTargetH]->m_sY != tdY)) return 0; // Fix for cannot hit moving targets with meele weps xRisenx // Anti Hack vs Criting hack

		cTargetDir = ntarget->direction;
		iTargetDefenseRatio = ntarget->m_iDefenseRatio;


		if (attacker->IsPlayer()) {
			switch (ntarget->Type())
			{
			case NPC_ESG:
			case NPC_GMG:
			case NPC_AS:
				if ((cattacker->side == NEUTRAL) || (ntarget->side == attacker->side)) return 0;
				break;
			}

			if ((wWeaponType == 25) && (ntarget->m_cActionLimit == 5) && (ntarget->buildCount > 0)) {

				if (cattacker->crusadeDuty != 2 && (cattacker->m_iAdminUserLevel == 0)) break;

				switch (ctarget->Type()) {
				case NPC_AGT:
				case NPC_CGT:
				case NPC_MS:
				case NPC_DT:
					if (cattacker->IsGM()) ntarget->buildCount = 1;
					switch (ntarget->buildCount) {
					case 1:
						ctarget->m_sAppr2 = 0;
						SendEventToNearClient_TypeA(ntarget, MSGID_MOTION_NULL, 0, 0, 0);

						if (m_bIsCrusadeMode)
						{
							switch (ntarget->Type())
							{
							case NPC_AGT: iConstructionPoint = 700; iWarContribution = 700; break;
							case NPC_CGT: iConstructionPoint = 700; iWarContribution = 700; break;
							case NPC_MS: iConstructionPoint = 500; iWarContribution = 500; break;
							case NPC_DT: iConstructionPoint = 500; iWarContribution = 500; break;
							}

							cattacker->crusadeContribution += iWarContribution;
							if (cattacker->crusadeContribution > MAXWARCONTRIBUTION)
								cattacker->crusadeContribution = MAXWARCONTRIBUTION;

							SendNotifyMsg(nullptr,cattacker, NOTIFY_CONSTRUCTIONPOINT, cattacker->crusadePoint, cattacker->crusadeContribution, 0);
						}
						break;
					case 5:
						ntarget->appr2 = 1;
						SendEventToNearClient_TypeA(ntarget, MSGID_MOTION_NULL, 0, 0, 0);
						break;
					case 10:
						ntarget->appr2 = 2;
						SendEventToNearClient_TypeA(ntarget, MSGID_MOTION_NULL, 0, 0, 0);
						break;
					}
					break;
				}

				ntarget->buildCount--;
				if (ntarget->buildCount <= 0) {
					ntarget->buildCount = 0;
				}
				return 0;
			}

			//#ifdef TitleHG
			//			if ((wWeaponType == 27) && (g_npcList[sTargetH]->m_iNpcCrops != 0) && (g_npcList[sTargetH]->m_cActionLimit == 5) && (g_npcList[sTargetH]->m_iBuildCount > 0)) {
			//
			//				//iSkillLV = g_clientList[sAttackerH]->m_cSkillMastery[SKILL_FARMING];
			//				iSkillLV = (g_clientList[sAttackerH]->m_stTitles[TITLE_INDEX_CRAFT_FARMING].m_iRank);
			//				iCropLimit = g_npcList[sTargetH]->m_iCropsSkillLV;
			////#ifdef TitleHG
			//				//cFarmingSkill = (g_game->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_CRAFT_FARMING].m_iRank);
			////#endif
			//
			//				//if(20 > iSkillLV) return 0;  
			//
			//				//if(g_clientList[sAttackerH]->m_iLevel <  20 ) return 0 ;
			//
			//
			//				switch (g_npcList[sTargetH]->m_sType) {
			//				case NPC_CROPS:
			//					switch (g_npcList[sTargetH]->m_iBuildCount) {
			//					case 1: 
			//						g_npcList[sTargetH]->m_sAppr2 = g_npcList[sTargetH]->m_iNpcCrops << 8 | 3;
			//						//						g_npcList[sTargetH]->m_sAppr2 = (char)0;
			//
			//						//if(iSkillLV <= iCropLimit + 10)
			//							//CalculateSSN_SkillIndex(sAttackerH, SKILL_FARMING, 1);
			//						if(iSkillLV <= iCropLimit + 10)
			//							g_game->increaseTitlePoints(sAttackerH, TITLE_INDEX_CRAFT_FARMING, 1);
			//						g_game->SendEventToNearClient_TypeA(sTargetH, OWNERTYPE_NPC, MSGID_MOTION_NULL, NULL, NULL, NULL);
			////#ifdef TitleHG
			//						//g_game->increaseTitlePoints(sAttackerH, TITLE_INDEX_CRAFT_FARMING, iSkillLV <= iCropLimit + 10);
			////#endif
			//						//if(probabilityTable(iSkillLV,iCropLimit,2) > 0)
			//						//{
			//							g_game->bCropsItemDrop(sAttackerH,sTargetH,true);
			//						//}
			//						g_game->DeleteNpc(sTargetH);
			//
			//						break;
			//					case 8: 		
			//						g_npcList[sTargetH]->m_sAppr2 = g_npcList[sTargetH]->m_iNpcCrops << 8 | 3;
			//						//						g_npcList[sTargetH]->m_sAppr2 = (char)3;
			//
			//						//if(iSkillLV <= iCropLimit + 10)
			//							//CalculateSSN_SkillIndex(sAttackerH, SKILL_FARMING, 1);
			//						g_game->SendEventToNearClient_TypeA(sTargetH, OWNERTYPE_NPC, MSGID_MOTION_NULL, NULL, NULL, NULL);
			////#ifdef TitleHG
			//						if(iSkillLV <= iCropLimit + 10)
			//							g_game->increaseTitlePoints(sAttackerH, TITLE_INDEX_CRAFT_FARMING, 1);
			//						//g_game->increaseTitlePoints(sAttackerH, TITLE_INDEX_CRAFT_FARMING, iSkillLV <= iCropLimit + 10);
			////#endif
			//						//if(probabilityTable(iSkillLV,iCropLimit,2) > 0)
			//						//{
			//							//if(iSkillLV <= iCropLimit + 10)
			//								//CalculateSSN_SkillIndex(sAttackerH, SKILL_FARMING, 1);
			////#ifdef TitleHG
			//							//g_game->increaseTitlePoints(sAttackerH, TITLE_INDEX_CRAFT_FARMING, iSkillLV <= iCropLimit + 10);
			//							//g_game->increaseTitlePoints(sAttackerH, TITLE_INDEX_CRAFT_FARMING, cFarmingSkill <= iCropLimit + 10);
			////#endif
			//							g_game->bCropsItemDrop(sAttackerH,sTargetH);
			//						//}
			//						break;
			//
			//					case 18: 
			//						g_npcList[sTargetH]->m_sAppr2 = g_npcList[sTargetH]->m_iNpcCrops << 8 | 2;
			//						//						g_npcList[sTargetH]->m_sAppr2 = (char)2;
			//
			//						//if(iSkillLV <= iCropLimit + 10)
			//							//CalculateSSN_SkillIndex(sAttackerH, SKILL_FARMING, 1);
			//						if(iSkillLV <= iCropLimit + 10)
			//							g_game->increaseTitlePoints(sAttackerH, TITLE_INDEX_CRAFT_FARMING, 1);
			//						g_game->SendEventToNearClient_TypeA(sTargetH, OWNERTYPE_NPC, MSGID_MOTION_NULL, NULL, NULL, NULL);
			////#ifdef TitleHG
			//						//g_game->increaseTitlePoints(sAttackerH, TITLE_INDEX_CRAFT_FARMING, iSkillLV <= iCropLimit + 10);
			////#endif
			//						//if(probabilityTable(iSkillLV,iCropLimit,2) > 0)
			//						//{
			//							//if(iSkillLV <= iCropLimit + 10)
			//								//CalculateSSN_SkillIndex(sAttackerH, SKILL_FARMING, 1);
			////#ifdef TitleHG
			//							//g_game->increaseTitlePoints(sAttackerH, TITLE_INDEX_CRAFT_FARMING, iSkillLV <= iCropLimit + 10);
			//							//g_game->increaseTitlePoints(sAttackerH, TITLE_INDEX_CRAFT_FARMING, cFarmingSkill <= iCropLimit + 10);
			////#endif
			//							g_game->bCropsItemDrop(sAttackerH,sTargetH);
			//						//}
			//						break;
			//					}
			//					break;
			//				}
			//
			//				if(g_npcList[sTargetH] == NULL)
			//				{
			//					return 0;
			//				}
			//
			//				/*if(probabilityTable(iSkillLV,iCropLimit,1) > 0 
			//					|| g_npcList[sTargetH]->m_iBuildCount == 1
			//					|| g_npcList[sTargetH]->m_iBuildCount == 8
			//					|| g_npcList[sTargetH]->m_iBuildCount == 18) {
			//
			//						iTemp = g_clientList[sAttackerH]->m_sItemEquipmentStatus[EQUIPPOS_RHAND];
			//
			//						if ((iTemp != -1) || (g_clientList[sAttackerH]->m_pItemList[iTemp] != NULL)) {
			//
			//							if (g_clientList[sAttackerH]->m_pItemList[iTemp]->m_wCurLifeSpan > 0)
			//								g_clientList[sAttackerH]->m_pItemList[iTemp]->m_wCurLifeSpan--;
			//
			//							if (g_clientList[sAttackerH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 0) {
			//
			//								g_game->SendNotifyMsg(NULL, sAttackerH, NOTIFY_ITEMLIFESPANEND, g_clientList[sAttackerH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
			//
			//								g_game->ReleaseItemHandler(sAttackerH, iTemp, true);  
			//							}
			//						}
			//
			//
			//						g_npcList[sTargetH]->m_iBuildCount--;
			//				}*/
			//
			//				if (g_npcList[sTargetH]->m_iBuildCount <= 0) {
			//					g_npcList[sTargetH]->m_iBuildCount = 0;
			//				}
			//
			//				return 0;
			//			}
			//			if (g_clientList[sAttackerH]->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND] != -1 &&
			//				g_clientList[sAttackerH]->m_pItemList[ g_clientList[sAttackerH]->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND] ] != NULL &&
			//				g_clientList[sAttackerH]->m_pItemList[ g_clientList[sAttackerH]->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND] ]->m_sIDnum == ITEM_DEMONSLAYER &&
			//				g_npcList[sTargetH]->m_sType == NPC_DEMON) {
			//				iAP_L += 10;
			//			}
			//		}
			//
			//		sTgtX = g_npcList[sTargetH]->m_sX;
			//		sTgtY = g_npcList[sTargetH]->m_sY;
			//		break;
			//	}
			//#else

//TODO: Farming?
/*
			if ((wWeaponType == 27) && (ctarget->m_iNpcCrops != 0) && (ctarget->m_cActionLimit == 5) && (ctarget->m_iBuildCount > 0))
			{

				iSkillLV = (static_cast<Client*>(attacker)->m_stTitles[TITLE_INDEX_CRAFT_FARMING].m_iRank);
				//iSkillLV = g_clientList[sAttackerH]->m_cSkillMastery[SKILL_FARMING];
				iCropLimit = ctarget->m_iCropsSkillLV;

				//if(20 > iSkillLV) return 0;  

				//if(g_clientList[sAttackerH]->m_iLevel <  20 ) return 0 ; 


				switch (ctarget->m_sType) {
				case NPC_CROPS:
					switch (ctarget->m_iBuildCount) {
					case 1:
						ctarget->m_sAppr2 = ctarget->m_iNpcCrops << 8 | 3;
						//						g_npcList[sTargetH]->m_sAppr2 = (char)0;

						//if(iSkillLV <= iCropLimit + 10)
						//CalculateSSN_SkillIndex(sAttackerH, SKILL_FARMING, 1);

						increaseTitlePoints(static_cast<Client*>(attacker), TITLE_INDEX_CRAFT_FARMING, iSkillLV <= iCropLimit + 10);

						SendEventToNearClient_TypeA(ctarget, MSGID_MOTION_NULL, 0, 0, 0);


						//if(probabilityTable(iSkillLV,iCropLimit,2) > 0)
						//{
						bCropsItemDrop(static_cast<Client*>(attacker), ctarget, true);
						//}
						DeleteNpc(ctarget);

						break;
					case 8:
						ctarget->m_sAppr2 = ctarget->m_iNpcCrops << 8 | 3;
						//						g_npcList[sTargetH]->m_sAppr2 = (char)3;

						increaseTitlePoints(static_cast<Client*>(attacker), TITLE_INDEX_CRAFT_FARMING, iSkillLV <= iCropLimit + 10);

						//if(iSkillLV <= iCropLimit + 10)
						//CalculateSSN_SkillIndex(sAttackerH, SKILL_FARMING, 1);

						SendEventToNearClient_TypeA(ctarget, MSGID_MOTION_NULL, 0, 0, 0);


						//if(probabilityTable(iSkillLV,iCropLimit,2) > 0)
						//{
						//if(iSkillLV <= iCropLimit + 10)
						//CalculateSSN_SkillIndex(sAttackerH, SKILL_FARMING, 1);
						bCropsItemDrop(static_cast<Client*>(attacker), ctarget);
						//}
						break;

					case 18:
						ctarget->m_sAppr2 = ctarget->m_iNpcCrops << 8 | 2;
						//						g_npcList[sTargetH]->m_sAppr2 = (char)2;

						increaseTitlePoints(static_cast<Client*>(attacker), TITLE_INDEX_CRAFT_FARMING, iSkillLV <= iCropLimit + 10);

						//if(iSkillLV <= iCropLimit + 10)
						//CalculateSSN_SkillIndex(sAttackerH, SKILL_FARMING, 1);

						SendEventToNearClient_TypeA(ctarget, MSGID_MOTION_NULL, 0, 0, 0);


						//if(probabilityTable(iSkillLV,iCropLimit,2) > 0)
						//{
						//if(iSkillLV <= iCropLimit + 10)
						//	CalculateSSN_SkillIndex(sAttackerH, SKILL_FARMING, 1);
						bCropsItemDrop(static_cast<Client*>(attacker), ctarget);
						//}
						break;
					}
					break;
				}

				if (ctarget == nullptr)
				{
					return 0;
				}

				//				if(probabilityTable(iSkillLV,iCropLimit,1) > 0 
				//					|| g_npcList[sTargetH]->m_iBuildCount == 1
				//					|| g_npcList[sTargetH]->m_iBuildCount == 8
				//					|| g_npcList[sTargetH]->m_iBuildCount == 18) {
				//
				//						iTemp = g_clientList[sAttackerH]->m_sItemEquipmentStatus[EQUIPPOS_RHAND];
				//
				//						if ((iTemp != -1) || (g_clientList[sAttackerH]->m_pItemList[iTemp] != NULL)) {
				//
				//							if (g_clientList[sAttackerH]->m_pItemList[iTemp]->m_wCurLifeSpan > 0)
				//								g_clientList[sAttackerH]->m_pItemList[iTemp]->m_wCurLifeSpan--;
				//
				//							if (g_clientList[sAttackerH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 0) {
				//
				//								g_game->SendNotifyMsg(NULL, sAttackerH, NOTIFY_ITEMLIFESPANEND, g_clientList[sAttackerH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				//
				//								g_game->ReleaseItemHandler(sAttackerH, iTemp, true);  
				//							}
				//						}
				////#endif
				//
				//						g_npcList[sTargetH]->m_iBuildCount--;
				//				}

				ctarget->m_iBuildCount--;
				if (ctarget->m_iBuildCount <= 0) {
					ctarget->m_iBuildCount = 0;
				}

				return 0;
			}*/
			if (cattacker->itemEquipStatus[EQUIPPOS_TWOHAND] != -1 &&
				cattacker->Equipped.TwoHand != nullptr &&
				cattacker->Equipped.TwoHand->m_sIDnum == ITEM_DEMONSLAYER &&
				ntarget->Type() == NPC_DEMON) {
				iAP_L += 10;
			}
		}

		sTgtX = ntarget->x;
		sTgtY = ntarget->y;
	}
		break;
	}

	if ((attacker->IsPlayer()) && (target->IsPlayer())) {

		sX = attacker->x;
		sY = attacker->y;

		dX = target->x;
		dY = target->y;

		if (target->map->iGetAttribute(sX, sY, 0x00000006) != 0) return 0;
		if (target->map->iGetAttribute(dX, dY, 0x00000006) != 0) return 0;
	}


	if (attacker->IsPlayer()) {
		if (cattacker->GetDex() > 50) {
			iAttackerHitRatio += (cattacker->GetDex() - 50);
		}
	}

	if (wWeaponType >= 40) {
		switch (attacker->map->weather) {
		case WEATHER_LIGHTRAIN:	iAttackerHitRatio = iAttackerHitRatio - (iAttackerHitRatio / 20); break;
		case WEATHER_MEDIUMRAIN:	iAttackerHitRatio = iAttackerHitRatio - (iAttackerHitRatio / 10); break;
		case WEATHER_HEAVYRAIN:	iAttackerHitRatio = iAttackerHitRatio - (iAttackerHitRatio / 4);  break;
		}
	}
	if (iAttackerHitRatio < 0)   iAttackerHitRatio = 0;

	switch (target->OwnerType())
	{
	case OWNERTYPE_PLAYER:
		cProtect = ctarget->magicEffectStatus[MAGICTYPE_PROTECT];
		break;
	case OWNERTYPE_NPC:
		cProtect = ntarget->magicEffectStatus[MAGICTYPE_PROTECT];
		break;
	}



	if (attacker->IsPlayer())
	{
		if (cattacker->Equipped.TwoHand != nullptr)
		{
			if (cattacker->Equipped.TwoHand->m_sItemEffectType == ITEMEFFECTTYPE_ATTACK_ARROW)
			{
				if (cattacker->Equipped.Arrow == nullptr)
				{
					return 0;
				}
				else
				{
					cattacker->Equipped.Arrow->m_dwCount--;
					if (cattacker->Equipped.Arrow->m_dwCount <= 0) {

						ItemDepleteHandler(cattacker->self.lock(), cattacker->Equipped.Arrow, false);

						cattacker->Equipped.Arrow = _iGetArrowItemIndex(cattacker);
					}
					else {
						SendNotifyMsg(nullptr, cattacker, NOTIFY_SETITEMCOUNT, cattacker->Equipped.Arrow->m_slot, cattacker->Equipped.Arrow->m_dwCount, (char)false);
						iCalcTotalWeight(cattacker->self.lock());
					}
				}
				if (cProtect == MAGICPROTECT_PFA) return 0;
			}
			else {
				switch (cProtect)
				{
				case MAGICPROTECT_DS:
					iTargetDefenseRatio += 40;
					break;
				case MAGICPROTECT_GDS:
					iTargetDefenseRatio += 100;
					break;
				}
				if (iTargetDefenseRatio < 0) iTargetDefenseRatio = 1;
			}
		}
	}
	else {
		switch (cProtect)
		{
		case MAGICPROTECT_PFA:
			switch (nattacker->Type())
			{
			case NPC_DARK_ELF:
			case NPC_AGT:
				if (abs(sTgtX - nattacker->x) >= 1 || abs(sTgtY - nattacker->y) >= 1)
					return 0;
				break;
			}
			break;
		case MAGICPROTECT_DS: iTargetDefenseRatio += 40;  break;
		case MAGICPROTECT_GDS: iTargetDefenseRatio += 100; break;
		}
		if (iTargetDefenseRatio < 0) iTargetDefenseRatio = 1;
	}

	if (cAttackerDir == cTargetDir) iTargetDefenseRatio = iTargetDefenseRatio / 2;

	if (iTargetDefenseRatio < 1)   iTargetDefenseRatio = 1;

	dTmp1 = (double)(iAttackerHitRatio);
	dTmp2 = (double)(iTargetDefenseRatio);

	dTmp3 = (dTmp1 / dTmp2) * HITRATIOFACTOR;
	iDestHitRatio = (int)(dTmp3);

	if (iDestHitRatio < MINIMUMHITRATIO) iDestHitRatio = MINIMUMHITRATIO;

	if (iDestHitRatio > MAXIMUMHITRATIO) iDestHitRatio = MAXIMUMHITRATIO;

	if ((bIsAttackerBerserk == true) && (iAttackMode < 20)) {
		if (attacker->IsPlayer()) {
			iAP_SM = iAP_SM * 4;
			iAP_L = iAP_L * 4;
		}
		else{
			iAP_SM = iAP_SM * 3.6;
			iAP_L = iAP_L  * 3.6;
		}
	}

	if (attacker->IsPlayer()) {
		iAP_SM += cattacker->m_iAddPhysicalDamage;
		iAP_L += cattacker->m_iAddPhysicalDamage;
	}

	if (bNearAttack == true) {
		iAP_SM = iAP_SM / 2;
		iAP_L = iAP_L / 2;
	}

	if (target->IsPlayer()) {
		iAP_SM -= (dice(1, ctarget->m_iVit / 10) - 1);
		iAP_L -= (dice(1, ctarget->m_iVit / 10) - 1);
	}

	/*if (cAttackerType == OWNERTYPE_PLAYER) {
	short weaponStatus = g_clientList[sAttackerH]->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND] != -1 ?
	g_clientList[sAttackerH]->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND] : g_clientList[sAttackerH]->m_sItemEquipmentStatus[EQUIPPOS_RHAND];

	if (weaponStatus != -1 &&	g_clientList[sAttackerH]->m_pItemList[ weaponStatus ] != NULL) {
	ItemID weapon = g_clientList[sAttackerH]->m_pItemList[ weaponStatus ]->m_sIDnum;

	if((g_game->m_cDayOrNight == 2 && weapon == ITEM_DARKEXECUTOR) ||
	(g_game->m_cDayOrNight == 1 && weapon == ITEM_LIGHTINGBLADE)){
	iAP_SM += 8;
	iAP_L  += 8;
	}*/

	//if ((g_clientList[sAttackerH]->m_pItemList[ g_clientList[sAttackerH]->m_sIDnum == 849) || // KlonessBlade 
	//	(g_game->m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 850)) { // KlonessAxe
	//	if (g_game->m_pClientList[sAttackerH]->m_reputation > 0) {
	//		iRepDamage = g_game->m_pClientList[sAttackerH]->m_reputation/100;
	//		if (iRepDamage < 5) iRepDamage = 5;
	//			iAP_SM += iRepDamage;
	//			iAP_L += iRepDamage;
	//	}
	//	if (cTargetType == OWNERTYPE_PLAYER) {
	//		if (g_game->m_pClientList[sTargetH] == NULL) return 0;
	//		if (g_game->m_pClientList[sTargetH]->m_reputation < 0) {
	//			iRepDamage = (abs(g_game->m_pClientList[sTargetH]->m_reputation)/10);
	//			if (iRepDamage > 10) iRepDamage = 10;
	//			iAP_SM += iRepDamage;
	//			iAP_L += iRepDamage;
	//		}
	//	}
	//}
	/*if (cTargetType == OWNERTYPE_PLAYER &&
	(weapon == ITEM_KLONESSAXE ||	weapon == ITEM_KLONESSBLADE || weapon == ITEM_KLONESSESTERK) &&
	g_clientList[sAttackerH]->m_reputation > 0)
	{
	iRepDamage = g_clientList[sAttackerH]->m_reputation/100;
	if (iRepDamage < 5) iRepDamage = 5;
	iDamage += iRepDamage;
	}*/
	//}

	/*if (cTargetType == OWNERTYPE_PLAYER && g_clientList[sAttackerH]->m_sItemEquipmentStatus[EQUIPPOS_NECK] != -1 &&
	g_clientList[sAttackerH]->m_pItemList[ g_clientList[sAttackerH]->m_sItemEquipmentStatus[EQUIPPOS_NECK] ] != NULL &&
	g_clientList[sAttackerH]->m_pItemList[ g_clientList[sAttackerH]->m_sItemEquipmentStatus[EQUIPPOS_NECK] ]->m_sIDnum == ITEM_NECKLACEOFKLONESS &&
	g_clientList[sAttackerH]->m_reputation > g_clientList[sTargetH]->m_reputation)
	{
	iTemp = (g_clientList[sAttackerH]->m_reputation - g_clientList[sTargetH]->m_reputation)/22;
	iAP_SM += iTemp > 15 ? 15 : iTemp;
	}*/
	//}

	if (attacker->IsPlayer()) {
		if (iAP_SM <= 1) iAP_SM = 1;
		if (iAP_L <= 1) iAP_L = 1;
	}
	else {
		if (iAP_SM <= 0) iAP_SM = 0;
		if (iAP_L <= 0) iAP_L = 0;
	}

	iResult = dice(1, 100);

	if (iResult <= iDestHitRatio) {
		if (attacker->IsPlayer())
		{
			if (((cattacker->m_iHungerStatus <= 10) || (cattacker->m_iSP <= 0)) &&
				(dice(1, 10) == 5)) return false;

			cattacker->m_iComboAttackCount++;
			if (cattacker->m_iComboAttackCount < 0) cattacker->m_iComboAttackCount = 0;
			if (cattacker->m_iComboAttackCount > 4) cattacker->m_iComboAttackCount = 1;

			iWeaponSkill = _iGetWeaponSkillType(cattacker);
			iComboBonus = iGetComboAttackBonus(iWeaponSkill, cattacker->m_iComboAttackCount);


			if ((cattacker->m_iComboAttackCount > 1) && (cattacker->m_iAddCD != 0))
				iComboBonus += cattacker->m_iAddCD;

			iAP_SM += iComboBonus;
			iAP_L += iComboBonus;


			switch (cattacker->m_iSpecialWeaponEffectType)
			{
			case ITEMSTAT_CRITICAL:
				if ((cattacker->superAttack > 0) && (iAttackMode >= 20)) {
					iAP_SM += cattacker->m_iSpecialWeaponEffectValue * 10; // *10 xRisenx
					iAP_L += cattacker->m_iSpecialWeaponEffectValue * 10; // *10 xRisenx
				}
				break;

			case ITEMSTAT_POISONING:
				cAttackerSA = 61;
				iAttackerSAvalue = cattacker->m_iSpecialWeaponEffectValue * 50; // *50 xRisenx
				break;

			case ITEMSTAT_RIGHTEOUS:
				cAttackerSA = 62;
				break;
			}

			if (cattacker->map->flags.fightZone == true)
			{
				iAP_SM += iAP_SM / 3;
				iAP_L += iAP_L / 3;
			}

			if (cattacker->map->flags.apocalypseMap)
			{
				iAP_SM *= 1.2;
				iAP_L *= 1.2;
			}

			if (cattacker->CheckNearbyFlags())
			{
				iAP_SM *= 1.3;
				iAP_L *= 1.3;
			}

			if ((target->IsPlayer()) && (m_bIsCrusadeMode == true) && (cattacker->crusadeDuty == 1))
			{

				if (cattacker->level <= 80)
				{
					iAP_SM += iAP_SM;
					iAP_L += iAP_L;
				}

				else if (cattacker->level <= 100)
				{
					iAP_SM += (iAP_SM * 7) / 10;
					iAP_L += (iAP_L * 7) / 10;
				}
				else
				{
					iAP_SM += iAP_SM / 3;;
					iAP_L += iAP_L / 3;
				}
			}
		}

		switch (target->OwnerType()/*target->m_sType*/) {
		case OWNERTYPE_PLAYER:
			ClearSkillUsingStatus(ctarget);

			if ((dwTime -ctarget->m_dwTime) < RAGPROTECTIONTIME) {

				return 0;
			}
			else {

				//if(cAttackerSA == 62 && g_clientList[sTargetH]->m_reputation < g_clientList[sAttackerH]->m_reputation)
				if (cAttackerSA == 62 && cattacker->m_reputation > 0)
				{
					iRepDamage = cattacker->m_reputation / 60;
					if (iRepDamage < 5) iRepDamage = 5;
					iAP_SM += iRepDamage;
					iAP_L += iRepDamage;
					/*iTemp = (g_clientList[sAttackerH]->m_reputation - g_clientList[sTargetH]->m_reputation) / 20;
					if (iTemp > 10) iTemp = 10;
					iAP_SM += iTemp;*/
				}


				iAP_Abs_Armor = 0;
				iAP_Abs_Shield = 0;

				iTemp = dice(1, 10000);
				if ((iTemp >= 1) && (iTemp < 5000))           iHitPoint = 1; // BODY
				else if ((iTemp >= 5000) && (iTemp < 7500))   iHitPoint = 2; // PANTS + LEGGINGS
				else if ((iTemp >= 7500) && (iTemp < 9000))   iHitPoint = 3; // ARMS
				else if ((iTemp >= 9000) && (iTemp <= 10000)) iHitPoint = 4; // HEAD

				//TODO: fix item stats
				switch (iHitPoint) {
				case 1:
					if (ctarget->m_iDamageAbsorption_Armor[EQUIPPOS_BODY] > 0) {

						if (ctarget->m_iDamageAbsorption_Armor[EQUIPPOS_BODY] >= 80)
							dTmp1 = 80.0f;
						else dTmp1 = (double)ctarget->m_iDamageAbsorption_Armor[EQUIPPOS_BODY];
						dTmp2 = (double)iAP_SM;
						dTmp3 = (dTmp1 / 100.0f) * dTmp2;

						iAP_Abs_Armor = (int)dTmp3;
					}
					break;

				case 2:
					if ((ctarget->m_iDamageAbsorption_Armor[EQUIPPOS_PANTS] +
						ctarget->m_iDamageAbsorption_Armor[EQUIPPOS_BOOTS]) > 0) {

						if ((ctarget->m_iDamageAbsorption_Armor[EQUIPPOS_PANTS] +
							ctarget->m_iDamageAbsorption_Armor[EQUIPPOS_BOOTS]) >= 80)
							dTmp1 = 80.0f;
						else dTmp1 = (double)(ctarget->m_iDamageAbsorption_Armor[EQUIPPOS_PANTS] + ctarget->m_iDamageAbsorption_Armor[EQUIPPOS_BOOTS]);
						dTmp2 = (double)iAP_SM;
						dTmp3 = (dTmp1 / 100.0f) * dTmp2;

						iAP_Abs_Armor = (int)dTmp3;
					}
					break;

				case 3:
					if (ctarget->m_iDamageAbsorption_Armor[EQUIPPOS_ARMS] > 0) {

						if (ctarget->m_iDamageAbsorption_Armor[EQUIPPOS_ARMS] >= 80)
							dTmp1 = 80.0f;
						else dTmp1 = (double)ctarget->m_iDamageAbsorption_Armor[EQUIPPOS_ARMS];
						dTmp2 = (double)iAP_SM;
						dTmp3 = (dTmp1 / 100.0f) * dTmp2;

						iAP_Abs_Armor = (int)dTmp3;
					}
					break;

				case 4:
					if (ctarget->m_iDamageAbsorption_Armor[EQUIPPOS_HEAD] > 0) {

						if (ctarget->m_iDamageAbsorption_Armor[EQUIPPOS_HEAD] >= 80)
							dTmp1 = 80.0f;
						else dTmp1 = (double)ctarget->m_iDamageAbsorption_Armor[EQUIPPOS_HEAD];
						dTmp2 = (double)iAP_SM;
						dTmp3 = (dTmp1 / 100.0f) * dTmp2;

						iAP_Abs_Armor = (int)dTmp3;
					}
					break;
				}

				bool shield = false;
				if (cAttackerDir != cTargetDir && ctarget->m_iDamageAbsorption_Shield > 0)
				{
					if (dice(1, 100) <= (ctarget->m_cSkillMastery[SKILL_SHIELD]))
					{
						shield = true;

						CalculateSSN_SkillIndex(ctarget, SKILL_SHIELD, 1);

						if (ctarget->m_iDamageAbsorption_Shield >= 80)
							dTmp1 = 80.0f;
						else dTmp1 = (double)ctarget->m_iDamageAbsorption_Shield;
						dTmp2 = (double)iAP_SM - iAP_Abs_Armor;
						dTmp3 = (dTmp1 / 100.0f) * dTmp2;

						iAP_Abs_Shield = (int)dTmp3;

						iTemp = ctarget->itemEquipStatus[EQUIPPOS_LHAND];
						if (!ctarget->IsNeutral())
						{
							EnduStrippingDamage(target, attacker, iTemp, 2000);
						}
					}
				}

				iAP_SM = iAP_SM - (iAP_Abs_Armor + iAP_Abs_Shield);
				if (iAP_SM <= 0) iAP_SM = 1;


				if ((attacker->IsPlayer()) && (cattacker->m_bIsSpecialAbilityEnabled == true))
				{
					switch (cattacker->m_iSpecialAbilityType) {
					case 0: break;
					case 1:
						iTemp = (ctarget->health / 2);
						if (iTemp > iAP_SM) iAP_SM = iTemp;
						if (iAP_SM <= 0) iAP_SM = 1;
						break;

					case 2:
						if (ctarget->magicEffectStatus[MAGICTYPE_ICE] == 0) {
							ctarget->magicEffectStatus[MAGICTYPE_ICE] = 1;
							ctarget->SetStatusFlag(STATUS_FROZEN, true);
							RegisterDelayEvent(DELAYEVENTTYPE_MAGICRELEASE, MAGICTYPE_ICE, dwTime + (30 * 1000),
								target, nullptr, 0, 0, 1, 0, 0);

							SendNotifyMsg(nullptr, ctarget, NOTIFY_MAGICEFFECTON, MAGICTYPE_ICE, 1, 0, 0);
						}
						break;

					case 3:
						if (ctarget->magicEffectStatus[MAGICTYPE_HOLDOBJECT] == 0) {
							ctarget->magicEffectStatus[MAGICTYPE_HOLDOBJECT] = 2;

							RegisterDelayEvent(DELAYEVENTTYPE_MAGICRELEASE, MAGICTYPE_HOLDOBJECT, dwTime + (10 * 1000),
								ctarget, nullptr, 0, 0, 10, 0, 0);

							SendNotifyMsg(nullptr, ctarget, NOTIFY_MAGICEFFECTON, MAGICTYPE_HOLDOBJECT, 10, 0, 0);
						}
						break;

					case 4:
						iAP_SM = (ctarget->health);
						break;

					case 5:
						ctarget->health += iAP_SM;
						if (ctarget->GetMaxHP() < ctarget->health)
							ctarget->health = ctarget->GetMaxHP();
						SendNotifyMsg(nullptr, ctarget, NOTIFY_HP, 0, 0, 0);
						break;
					}
				}


				if ((attacker->IsPlayer()) && (ctarget->m_bIsSpecialAbilityEnabled == true)) {
					switch (ctarget->m_iSpecialAbilityType) {
					case 50:
						if (cattacker->Equipped.TwoHand != nullptr)
							cattacker->Equipped.TwoHand->m_wCurLifeSpan = 0;
						else if (cattacker->Equipped.RightHand != nullptr)
							cattacker->Equipped.RightHand->m_wCurLifeSpan = 0;
						break;

					case 51:
						if (iHitPoint == ctarget->m_iSpecialAbilityEquipPos)
							iAP_SM = 0;
						break;

					case 52:
						iAP_SM = 0;
						break;
					}
				}

				if ((ctarget->m_bIsLuckyEffect == true) &&
					(dice(1, 10) < 5) && (ctarget->health <= iAP_SM)) {
					iAP_SM = 1;
				}

				switch (iHitPoint) {
				case 1:
					iTemp = ctarget->itemEquipStatus[EQUIPPOS_BODY];
					if (iTemp == -1)
						iTemp = ctarget->itemEquipStatus[EQUIPPOS_FULLBODY];

					EnduStrippingDamage(target, attacker, iTemp, 2000, shield);
					break;

				case 2:
					iTemp = ctarget->itemEquipStatus[EQUIPPOS_PANTS];
					EnduStrippingDamage(target, attacker, iTemp, 2000, shield);

					iTemp = ctarget->itemEquipStatus[EQUIPPOS_BOOTS];
					EnduStrippingDamage(target, attacker, iTemp, 2000, shield);
					break;

				case 3:
					iTemp = ctarget->itemEquipStatus[EQUIPPOS_ARMS];
					EnduStrippingDamage(target, attacker, iTemp, 2000, shield);
					break;

				case 4:
					iTemp = ctarget->itemEquipStatus[EQUIPPOS_HEAD];
					EnduStrippingDamage(target, attacker, iTemp, 3000, shield);
					break;
				}


				if (cAttackerSA == 2 && ctarget->magicEffectStatus[MAGICTYPE_PROTECT]) {
					SendNotifyMsg(nullptr, ctarget, NOTIFY_MAGICEFFECTOFF, MAGICTYPE_PROTECT, ctarget->magicEffectStatus[MAGICTYPE_PROTECT], 0);
					switch (ctarget->magicEffectStatus[MAGICTYPE_PROTECT])
					{
					case MAGICPROTECT_PFA:
						ctarget->SetStatusFlag(STATUS_PFA, false);
						break;
					case MAGICPROTECT_PFM:
						//case MAGICPROTECT_GPFM:
					case MAGICPROTECT_AMP:
						ctarget->SetStatusFlag(STATUS_PFM, false);
						break;
					case MAGICPROTECT_DS:
					case MAGICPROTECT_GDS:
						ctarget->SetStatusFlag(STATUS_DEFENSESHIELD, false);
						break;
					}
					ctarget->magicEffectStatus[MAGICTYPE_PROTECT] = 0;
					RemoveFromDelayEventList(target, MAGICTYPE_PROTECT);
				}


				if ((ctarget->IsPoisoned() == false) &&//TODO: overwrite poisons? At the moment, a stronger poison gets ignored if any is applied prior.
					((cAttackerSA == 5) || (cAttackerSA == 6) || (cAttackerSA == 61))) {
					if (CheckResistingPoisonSuccess(target) == false) {

						if (cAttackerSA == 5)	ctarget->Poison(true, 15, dwTime);
						else if (cAttackerSA == 6)  ctarget->Poison(true, 40, dwTime);
						else if (cAttackerSA == 61) ctarget->Poison(true, iAttackerSAvalue, dwTime);

						ctarget->SetStatusFlag(STATUS_POISON, true);
						SendNotifyMsg(nullptr, ctarget, NOTIFY_MAGICEFFECTON, MAGICTYPE_POISON, ctarget->PoisonLevel(), 0);
					}
				}
#ifdef Showdmg
				gserver->m_pClientList[sTargetH]->m_iHP -= iAP_SM;
				if (gserver->m_pClientList[sAttackerH] != 0){
					if (gserver->m_pClientList[sAttackerH]->iDmgShowon == 1){
						ZeroMemory(cDamageMod, sizeof(cDamageMod));
						//wsprintf(cDamageMod ,"You did [%d] Damage to Character [%s] Remaining Life [%d]", iAP_SM,g_game->m_pClientList[sTargetH]->m_cCharName,g_game->m_pClientList[sTargetH]->m_iHP);
						wsprintf(cDamageMod, "You did [%d] Damage to Character [%s]", iAP_SM, gserver->m_pClientList[sTargetH]->m_cCharName);
						gserver->SendNotifyMsg(0, sAttackerH, NOTIFY_NOTICEMSG, 0, 0, 0, cDamageMod);
					}
				}
#else
				if (ctarget->health <= iAP_SM)
					ctarget->health = 0;
				else
					ctarget->health -= iAP_SM;
#endif
				ctarget->m_lastDamageTime = dwTime;
				if (ctarget->health == 0) {

					if (attacker->IsPlayer())
						bAnalyzeCriminalAction(cattacker, ctarget->x, ctarget->y);

					ctarget->KilledHandler(attacker, iAP_SM);
					bKilled = true;
					iKilledDice = ctarget->level;

				}
				else {
					if (iAP_SM > 0) {

						if (ctarget->m_iAddTransMana > 0) {
							dTmp1 = (double)ctarget->m_iAddTransMana;
							dTmp2 = (double)iAP_SM;
							dTmp3 = (dTmp1 / 100.0f)*dTmp2;


							iTemp = (3 * ctarget->GetMag()) + (2 * ctarget->level) + (2 * ctarget->GetInt());
							ctarget->mana += (int)dTmp3;
							if (ctarget->mana > iTemp) ctarget->mana = iTemp;
						}


						if (ctarget->m_iAddChargeCritical > 0) {
							if (dice(1, 100) < (ctarget->m_iAddChargeCritical)) {
								/*iMaxSuperAttack = (g_clientList[sTargetH]->m_iLevel / 10);
								if (g_clientList[sTargetH]->m_iSuperAttackLeft < iMaxSuperAttack) g_clientList[sTargetH]->m_iSuperAttackLeft++;
								g_game->SendNotifyMsg(0, sTargetH, NOTIFY_SUPERATTACKLEFT, 0, 0, 0, 0);*/
								if (ctarget->superAttack < MAXSUPERATTACK) {
									ctarget->superAttack++;
									SendNotifyMsg(nullptr, ctarget, NOTIFY_SUPERATTACKLEFT, 0, 0, 0);
								}
							}
						}
						SendNotifyMsg(nullptr, ctarget, NOTIFY_HP, 0, 0, 0);

						if (attacker->IsPlayer())
							sAttackerWeapon = ((cattacker->m_sAppr2 & 0x0FF0) >> 4);
						else sAttackerWeapon = 1;


						if ((attacker->IsPlayer()) && (cattacker->map->flags.fightZone == true))
							iMoveDamage = 125; // Damage you take before you start fly when it xRisenx
						else iMoveDamage = 100;

						if (iAP_SM >= iMoveDamage) {

							if (sTgtX == sAtkX) {
								if (sTgtY == sAtkY)     goto CAE_SKIPDAMAGEMOVE;
								else if (sTgtY > sAtkY) cDamageMoveDir = 5;
								else if (sTgtY < sAtkY) cDamageMoveDir = 1;
							}
							else if (sTgtX > sAtkX) {
								if (sTgtY == sAtkY)     cDamageMoveDir = 3;
								else if (sTgtY > sAtkY) cDamageMoveDir = 4;
								else if (sTgtY < sAtkY) cDamageMoveDir = 2;
							}
							else if (sTgtX < sAtkX) {
								if (sTgtY == sAtkY)     cDamageMoveDir = 7;
								else if (sTgtY > sAtkY) cDamageMoveDir = 6;
								else if (sTgtY < sAtkY) cDamageMoveDir = 8;
							}


							ctarget->m_iLastDamage = iAP_SM;

							SendNotifyMsg(nullptr, ctarget, NOTIFY_DAMAGEMOVE, cDamageMoveDir, iAP_SM, sAttackerWeapon);
						}
						else {
						CAE_SKIPDAMAGEMOVE:;
							int iProb;


							if (attacker->IsPlayer()) {
								switch (cattacker->m_sUsingWeaponSkill) {
								case SKILL_ARCHERY: iProb = 3500; break;
								case SKILL_LONGSWORD: iProb = 1000; break;
								case SKILL_FENCING: iProb = 2900; break;
								case SKILL_AXE: iProb = 2500; break;
								case SKILL_HAMMER: iProb = 2000; break;
								case SKILL_STAFF: iProb = 2000; break;
								default: iProb = 1; break;
								}
							}
							else iProb = 1;

							if (dice(1, 10000) >= iProb)
								SendEventToNearClient_TypeA(ctarget, MSGID_MOTION_DAMAGE, iAP_SM, sAttackerWeapon, 0);
						}

						if (ctarget->magicEffectStatus[MAGICTYPE_HOLDOBJECT] == 1) {

							// 1: Hold-Person 
							// 2: Paralyze
							SendNotifyMsg(nullptr, ctarget, NOTIFY_MAGICEFFECTOFF, MAGICTYPE_HOLDOBJECT, ctarget->magicEffectStatus[MAGICTYPE_HOLDOBJECT], 0);

							ctarget->magicEffectStatus[MAGICTYPE_HOLDOBJECT] = 0;
							RemoveFromDelayEventList(target, MAGICTYPE_HOLDOBJECT);
						}

						ctarget->superAttackMax++;
						if (ctarget->superAttackMax > 14) {
							ctarget->superAttackMax = 0;
							/*iMaxSuperAttack = (g_clientList[sTargetH]->m_iLevel / 10);
							if (g_clientList[sTargetH]->m_iSuperAttackLeft < iMaxSuperAttack) g_clientList[sTargetH]->m_iSuperAttackLeft++;
							g_game->SendNotifyMsg(0, sTargetH, NOTIFY_SUPERATTACKLEFT, 0, 0, 0);*/
							if (ctarget->superAttack < MAXSUPERATTACK) {
								ctarget->superAttack++;
								SendNotifyMsg(nullptr, ctarget, NOTIFY_SUPERATTACKLEFT, 0, 0, 0);
							}
						}
					}
				}
			}
			break;

		case OWNERTYPE_NPC:

			if (ntarget->m_cBehavior == BEHAVIOR_DEAD) return 0;
			if (ntarget->_dead == true) return 0;

			if (m_bIsCrusadeMode == true) {
				if (cAttackerSide == ntarget->side) {
					switch (ntarget->Type()) {
					case NPC_ESG:
					case NPC_GMG:
					case NPC_LWB:
					case NPC_GHK:
					case NPC_GHKABS:
					case NPC_TK:
					case NPC_BG:
					case NPC_CP:
						return 0;

					default: break;
					}
				}
				else {
					switch (ntarget->Type()) {
					case NPC_GMG:
						if (cAttackerSide != 0) {
							ntarget->m_iV1 += iAP_L;
							if (ntarget->m_iV1 > 300) {

								ntarget->m_iV1 -= 300;
								m_mana[ntarget->side] -= 5;
								if (m_mana[ntarget->side] <= 0) m_mana[ntarget->side] = 0;
								//testcode
								//wsprintf(g_cTxt, "ManaStock down: %d", m_mana[g_npcList[sTargetH]->m_side]);
								//PutLogList(g_cTxt);
							}
						}
						break;
					}
				}
			}

			switch (ntarget->m_cActionLimit)
			{
			case 1:
			case 2:
			case 6:
				return 0;
			}

			if (ntarget->m_cSize == 0)
				iDamage = iAP_SM;
			else iDamage = iAP_L;

			if (ntarget->absDamage < 0) {
				dTmp1 = (double)iDamage;
				dTmp2 = (double)(abs(ntarget->absDamage)) / 100.0f;
				dTmp3 = dTmp1 * dTmp2;
				dTmp2 = dTmp1 - dTmp3;
				iDamage = (int)dTmp2;
				if (iDamage < 0) iDamage = 1;
			}


			if (cAttackerSA == 2 && ntarget->magicEffectStatus[MAGICTYPE_PROTECT]) {
				switch (ntarget->magicEffectStatus[MAGICTYPE_PROTECT])
				{
				case MAGICPROTECT_PFA:
					ntarget->SetStatusFlag(STATUS_PFA, false);
					break;
				case MAGICPROTECT_PFM:
					//case MAGICPROTECT_GPFM:
				case MAGICPROTECT_AMP:
					ntarget->SetStatusFlag(STATUS_PFM, false);
					break;
				case MAGICPROTECT_DS:
				case MAGICPROTECT_GDS:
					ntarget->SetStatusFlag(STATUS_DEFENSESHIELD, false);
					break;
				}
				ntarget->magicEffectStatus[MAGICTYPE_PROTECT] = 0;
				RemoveFromDelayEventList(target, MAGICTYPE_PROTECT);
			}

			// Hero Damage Bonus 1% Per Rank xRisenx
			/*if (cAttackerType == OWNERTYPE_PLAYER && cTargetType == OWNERTYPE_PLAYER)
			{
			int iTitlesAddedDmg = 0;
			iTitlesAddedDmg = ((g_game->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_EK].m_iRank * iDamage)/100);
			}*/
			//#ifdef TitleHG
			//increase damage according to the player's title rank...
			//	note only physical damage PvE can be altered here. PvP, magical is elsewhere.

/*
			if (attacker->m_sType == OWNERTYPE_PLAYER) // well this still makes a correct check that attacker is not NPC. before we ever need to in the title function.. go pro. and it fixed it lol... well this is the fix we had prepared to put into the title code anyway. since attack checks this for us before any call it makes our fix redundant. useless.
			{
				int iTitlesAddedDmg = 0;
				switch (gserver->m_pNpcList[sTargetH]->m_sType) {
				case 10:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_SLIME].m_iRank * iDamage) / 20); break;
				case 16:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_GIANTANT].m_iRank * iDamage) / 20); break;
				case 22:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_AMPHIS].m_iRank * iDamage) / 20); break;
				case 14:
					if (strcmp(gserver->m_pNpcList[sTargetH]->m_cNpcName, "Orc") == 0) {
						iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_ORC].m_iRank * iDamage) / 20);
					}
					else if (strcmp(gserver->m_pNpcList[sTargetH]->m_cNpcName, "Orc-Mage") == 0) {
						iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_ORCMAGE].m_iRank * iDamage) / 20);
					}
					break;
				case 18:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_ZOMBIE].m_iRank * iDamage) / 20); break;
				case 11:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_SKELETON].m_iRank * iDamage) / 20); break;
				case 17:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_SCORPION].m_iRank * iDamage) / 20); break;
				case 23:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_CLAYGOLEM].m_iRank * iDamage) / 20); break;
				case 12:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_STONEGOLEM].m_iRank * iDamage) / 20); break;
				case 27:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_HELHOUND].m_iRank * iDamage) / 20); break;
				case 57:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_GIANTFROG].m_iRank * iDamage) / 20); break;
				case 61:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_RUDOLF].m_iRank * iDamage) / 20); break;
				case 28:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_TROLL].m_iRank * iDamage) / 20); break;
				case 13:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_CYCLOPS].m_iRank * iDamage) / 20); break;
				case 65:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_ICEGOLEM].m_iRank * iDamage) / 20); break;
				case 53:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_BEHOLDER].m_iRank * iDamage) / 20); break;
				case 60:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_CANNIBALPLANT].m_iRank * iDamage) / 20); break;
				case 29:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_OGRE].m_iRank * iDamage) / 20); break;
				case 62:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_DIREBOAR].m_iRank * iDamage) / 20); break;
				case 80:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_TENTOCLE].m_iRank * iDamage) / 20); break;
				case 74:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_GIANTCRAYFISH].m_iRank * iDamage) / 20); break;
				case 58:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_MOUNTAINGIANT].m_iRank * iDamage) / 20); break;
				case 76:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_GIANTPLANT].m_iRank * iDamage) / 20); break;
				case 30:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_LICHE].m_iRank * iDamage) / 20); break;
				case 48:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_STALKER].m_iRank * iDamage) / 20); break;
				case 33:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_WEREWOLF].m_iRank * iDamage) / 20); break;
				case 54:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_DARKELF].m_iRank * iDamage) / 20); break;
				case 63:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_FROST].m_iRank * iDamage) / 20); break;
				case 72:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_CLAWTURTLE].m_iRank * iDamage) / 20); break;
				case 59:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_ETTIN].m_iRank * iDamage) / 20); break;
				case 31:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_DEMON].m_iRank * iDamage) / 20); break;
				case 32:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_UNICORN].m_iRank * iDamage) / 20); break;
				case 52:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_GARGOYLE].m_iRank * iDamage) / 20); break;
				case 75:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_GIANTLIZARD].m_iRank * iDamage) / 20); break;
				case 79:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_NIZIE].m_iRank * iDamage) / 20); break;
				case 49:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_HELLCLAW].m_iRank * iDamage) / 20); break;
				case 50:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_TIGERWORM].m_iRank * iDamage) / 20); break;
				case 66:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_WYVERN].m_iRank * iDamage) / 20); break;
				case 73:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_FIREWYVERN].m_iRank * iDamage) / 20); break;
				case 81:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_ABBADON].m_iRank * iDamage) / 20); break;
				case 71:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_CENTAURUS].m_iRank * iDamage) / 20); break;
				case 77:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_MASTERMAGEORC].m_iRank * iDamage) / 20); break;
				case 78:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_MINOTAUR].m_iRank * iDamage) / 20); break;
				case 70:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_BARLOG].m_iRank * iDamage) / 20); break;
				case 104:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_DARKUNICORN].m_iRank * iDamage) / 20); break;
				case 105:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_BLACKDEMON].m_iRank * iDamage) / 20); break;
				case 106:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_EARTHWYVERN].m_iRank * iDamage) / 20); break;
				case 107:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_WINDWYVERN].m_iRank * iDamage) / 20); break;
				case 108:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_DARKWYVERN].m_iRank * iDamage) / 20); break;
				case 109:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_GRASSWYVERN].m_iRank * iDamage) / 20); break;
				case 110:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_SKYWYVERN].m_iRank * iDamage) / 20); break;
				case 111:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_SEAWYVERN].m_iRank * iDamage) / 20); break;
				case 112:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_METALWYVERN].m_iRank * iDamage) / 20); break;
				case 113:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_VOIDWYVERN].m_iRank * iDamage) / 20); break;
				case 114:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_ETERNALDRAGON].m_iRank * iDamage) / 20); break;
				case 122:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_ENRAGEDTROLL].m_iRank * iDamage) / 20); break;
				case 123:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_ENRAGEDCYCLOPS].m_iRank * iDamage) / 20); break;
				case 124:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_ENRAGEDSTALKER].m_iRank * iDamage) / 20); break;
				case 125:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_ENRAGEDGARGOYLE].m_iRank * iDamage) / 20); break;
				case 126:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_ENRAGEDHELLCLAW].m_iRank * iDamage) / 20); break;
				case 127:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_ENRAGEDTIGERWORM].m_iRank * iDamage) / 20); break; // if it had any value allocated, the trace would not hide it. i say his design fails to initialize a value to m_iRank in some cases. the variable has been declared but never assigned. might be always worked without any crash or error for over 3 months lol....... DO NOT MAKE ANOTHER EXCUSE FOR THIS NOOBS CODE! I SEE NOTHING BUT BUGS HERE MAN same.. Lets burn ze source :D i'LL BRING THE PETROL yes
					//case xxxxx:  iTitlesAddedDmg = ((g_game->[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_].m_iRank * iDamage)/20); break;
				default: break;
				}
				if (iTitlesAddedDmg > 0)//just in case
					iDamage += iTitlesAddedDmg;

				if (gserver->m_pClientList[sAttackerH] != NULL){
					if (gserver->m_pClientList[sAttackerH]->iDmgShowon == 2){
						char cTitlesMsg[256];
						ZeroMemory(cTitlesMsg, sizeof(cTitlesMsg));
						wsprintf(cTitlesMsg, "StartDmg (%d),  TBonus (%d),  Total (%d),  Remaining Life (%d)", (iDamage - iTitlesAddedDmg), iTitlesAddedDmg, iDamage, gserver->m_pNpcList[sTargetH]->m_iHP);
						gserver->SendNotifyMsg(NULL, sAttackerH, NOTIFY_NOTICEMSG, NULL, NULL, NULL, cTitlesMsg);
					}
				}


				//int iTitlesAddedDmg = g_game->getTitlesMobBonusDamage(sAttackerH, sTargetH, iDamage);
				//iDamage += iTitlesAddedDmg;
			} // Alll that i marked now was commented before .. as i changed it to this but i moved back to this when it crashes as i think this was the error.. and it might have been because i remember i had this bug before and then i added this
*/



			//#endif
#ifdef Showdmg
			//switch (g_game->m_pNpcList[sTargetH]->m_cActionLimit) {
			//case 0:
			//case 3:
			//case 5:
			g_npcList[sTargetH]->ReduceHP(iDamage);
			//{
			//	//g_game->m_pNpcList[sTargetH]->m_iHP -= iDamage;
			//if (g_game->m_pClientList[sAttackerH] != NULL){
			//if (g_game->m_pClientList[sAttackerH]->iDmgShowon == 1){
			//ZeroMemory(cDamageMod, sizeof(cDamageMod));
			//wsprintf(cDamageMod ,"You did [%d] Damage to Npc [%s] Remaining Life [%d]", iDamage,g_game->m_pNpcList[sTargetH]->m_cNpcName,g_game->m_pNpcList[sTargetH]->m_iHP);
			//g_game->SendNotifyMsg(NULL, sAttackerH, NOTIFY_NOTICEMSG, NULL, NULL, NULL, cDamageMod);
			//}
			//}
			//break;
			//}
			//}
#else
			ntarget->ReduceHP(iDamage);
			SendEventToNearClient_TypeA(target, MSGID_MOTION_DAMAGE, iDamage, sAttackerWeapon, 0);
			
#endif
			if (ntarget->health == 0) {
				ntarget->health = 0;
				if (attacker->IsPlayer())
					target->map->NpcKilledHandler(static_cast<Client*>(attacker)->self.lock(), static_cast<Npc*>(target)->self.lock(), iDamage);
				else
					target->map->NpcKilledHandler(static_cast<Npc*>(attacker)->self.lock(), static_cast<Npc*>(target)->self.lock(), iDamage);

				bKilled = true;
				iKilledDice = static_cast<Npc*>(target)->m_iHitDice;
			}
			/*
			else {

				if ((g_npcList[sTargetH]->m_sType != 21) && (g_npcList[sTargetH]->m_sType != 55) && (g_npcList[sTargetH]->m_sType != 56)
					&& (g_npcList[sTargetH]->m_side == cAttackerSide)) goto CAE_SKIPCOUNTERATTACK;


				if (g_npcList[sTargetH]->m_cActionLimit != 0) goto CAE_SKIPCOUNTERATTACK;
				if (g_npcList[sTargetH]->m_bIsPermAttackMode == true) goto CAE_SKIPCOUNTERATTACK;
				if ((g_npcList[sTargetH]->m_bIsSummoned == true) &&
					(g_npcList[sTargetH]->m_iSummonControlMode == 1)) goto CAE_SKIPCOUNTERATTACK;

				if (g_npcList[sTargetH]->m_sType == NPC_CP) goto CAE_SKIPCOUNTERATTACK;

				if (dice(1, 3) == 2) {

					if (g_npcList[sTargetH]->m_cBehavior == BEHAVIOR_ATTACK) {
						tX = tY = 0;
						switch (g_npcList[sTargetH]->m_cTargetType) {
						case OWNERTYPE_PLAYER:
							if (g_clientList[g_npcList[sTargetH]->m_iTargetIndex] != NULL) {
								tX = g_clientList[g_npcList[sTargetH]->m_iTargetIndex]->m_sX;
								tY = g_clientList[g_npcList[sTargetH]->m_iTargetIndex]->m_sY;
							}
							break;

						case OWNERTYPE_NPC:
							if (g_npcList[g_npcList[sTargetH]->m_iTargetIndex] != NULL) {
								tX = g_npcList[g_npcList[sTargetH]->m_iTargetIndex]->m_sX;
								tY = g_npcList[g_npcList[sTargetH]->m_iTargetIndex]->m_sY;
							}
							break;
						}
						iDst1 = (g_npcList[sTargetH]->m_sX - tX)*(g_npcList[sTargetH]->m_sX - tX) + (g_npcList[sTargetH]->m_sY - tY)*(g_npcList[sTargetH]->m_sY - tY);

						tX = tY = 0;
						switch (cAttackerType) {
						case OWNERTYPE_PLAYER:
							if (g_clientList[sAttackerH] != NULL) {
								tX = g_clientList[sAttackerH]->m_sX;
								tY = g_clientList[sAttackerH]->m_sY;
							}
							break;

						case OWNERTYPE_NPC:
							if (g_npcList[sAttackerH] != NULL) {
								tX = g_npcList[sAttackerH]->m_sX;
								tY = g_npcList[sAttackerH]->m_sY;
							}
							break;
						}


						iDst2 = (g_npcList[sTargetH]->m_sX - tX)*(g_npcList[sTargetH]->m_sX - tX) + (g_npcList[sTargetH]->m_sY - tY)*(g_npcList[sTargetH]->m_sY - tY);

						if (iDst2 <= iDst1) {

							g_npcList[sTargetH]->m_cBehavior = BEHAVIOR_ATTACK;
							g_npcList[sTargetH]->m_sBehaviorTurnCount = 0;
							g_npcList[sTargetH]->m_iTargetIndex = sAttackerH;
							g_npcList[sTargetH]->m_cTargetType = cAttackerType;
						}
					}
					else {
						g_npcList[sTargetH]->m_cBehavior = BEHAVIOR_ATTACK;
						g_npcList[sTargetH]->m_sBehaviorTurnCount = 0;
						g_npcList[sTargetH]->m_iTargetIndex = sAttackerH;
						g_npcList[sTargetH]->m_cTargetType = cAttackerType;
					}
				}

			CAE_SKIPCOUNTERATTACK:;

				// Stun
				if ((dice(1, (g_npcList[sTargetH]->dwGoldDropValue / 55) + 3) == 1) &&
					(g_npcList[sTargetH]->m_cActionLimit == 0))
					g_npcList[sTargetH]->m_dwTime = dwTime;


				if (cAttackerType == OWNERTYPE_PLAYER)
					sAttackerWeapon = ((g_clientList[sAttackerH]->m_sAppr2 & 0x0FF0) >> 4);
				else sAttackerWeapon = 1;

				if ((wWeaponType < 40) && (g_npcList[sTargetH]->m_cActionLimit == 4)) {

					if (sTgtX == sAtkX) {
						if (sTgtY == sAtkY)     goto CAE_SKIPDAMAGEMOVE2;
						else if (sTgtY > sAtkY) cDamageMoveDir = 5;
						else if (sTgtY < sAtkY) cDamageMoveDir = 1;
					}
					else if (sTgtX > sAtkX) {
						if (sTgtY == sAtkY)     cDamageMoveDir = 3;
						else if (sTgtY > sAtkY) cDamageMoveDir = 4;
						else if (sTgtY < sAtkY) cDamageMoveDir = 2;
					}
					else if (sTgtX < sAtkX) {
						if (sTgtY == sAtkY)     cDamageMoveDir = 7;
						else if (sTgtY > sAtkY) cDamageMoveDir = 6;
						else if (sTgtY < sAtkY) cDamageMoveDir = 8;
					}

					dX = g_npcList[sTargetH]->m_sX + _tmp_cTmpDirX[cDamageMoveDir];
					dY = g_npcList[sTargetH]->m_sY + _tmp_cTmpDirY[cDamageMoveDir];

					if (g_mapList[g_npcList[sTargetH]->m_cMapIndex]->bGetMoveable(dX, dY, NULL) == false) {

						cDamageMoveDir = dice(1, 8);
						dX = g_npcList[sTargetH]->m_sX + _tmp_cTmpDirX[cDamageMoveDir];
						dY = g_npcList[sTargetH]->m_sY + _tmp_cTmpDirY[cDamageMoveDir];

						if (g_mapList[g_npcList[sTargetH]->m_cMapIndex]->bGetMoveable(dX, dY, NULL) == false) goto CAE_SKIPDAMAGEMOVE2;
					}

					g_mapList[g_npcList[sTargetH]->m_cMapIndex]->ClearOwner(/ *5,* / sTargetH, OWNERTYPE_NPC, g_npcList[sTargetH]->m_sX, g_npcList[sTargetH]->m_sY);

					g_mapList[g_npcList[sTargetH]->m_cMapIndex]->SetOwner(sTargetH, OWNERTYPE_NPC, dX, dY);
					g_npcList[sTargetH]->m_sX = dX;
					g_npcList[sTargetH]->m_sY = dY;
					g_npcList[sTargetH]->m_cDir = cDamageMoveDir;

					gserver->SendEventToNearClient_TypeA(sTargetH, OWNERTYPE_NPC, MSGID_MOTION_MOVE, NULL, NULL, NULL);

					dX = g_npcList[sTargetH]->m_sX + _tmp_cTmpDirX[cDamageMoveDir];
					dY = g_npcList[sTargetH]->m_sY + _tmp_cTmpDirY[cDamageMoveDir];

					if (g_mapList[g_npcList[sTargetH]->m_cMapIndex]->bGetMoveable(dX, dY, NULL) == false) {

						cDamageMoveDir = dice(1, 8);
						dX = g_npcList[sTargetH]->m_sX + _tmp_cTmpDirX[cDamageMoveDir];
						dY = g_npcList[sTargetH]->m_sY + _tmp_cTmpDirY[cDamageMoveDir];

						if (g_mapList[g_npcList[sTargetH]->m_cMapIndex]->bGetMoveable(dX, dY, NULL) == false) goto CAE_SKIPDAMAGEMOVE2;
					}

					g_mapList[g_npcList[sTargetH]->m_cMapIndex]->ClearOwner(/ *5,* / sTargetH, OWNERTYPE_NPC, g_npcList[sTargetH]->m_sX, g_npcList[sTargetH]->m_sY);

					g_mapList[g_npcList[sTargetH]->m_cMapIndex]->SetOwner(sTargetH, OWNERTYPE_NPC, dX, dY);
					g_npcList[sTargetH]->m_sX = dX;
					g_npcList[sTargetH]->m_sY = dY;
					g_npcList[sTargetH]->m_cDir = cDamageMoveDir;

					gserver->SendEventToNearClient_TypeA(sTargetH, OWNERTYPE_NPC, MSGID_MOTION_MOVE, NULL, NULL, NULL);

					if (gserver->bCheckEnergySphereDestination(sTargetH, sAttackerH, cAttackerType) == true) {
						if (cAttackerType == OWNERTYPE_PLAYER) {
							iExp = (g_npcList[sTargetH]->m_iExp / 3);
							if (g_npcList[sTargetH]->m_iNoDieRemainExp > 0)
								iExp += g_npcList[sTargetH]->m_iNoDieRemainExp;


							if (g_clientList[sAttackerH]->m_iAddExp != NULL) {
								dTmp1 = (double)g_clientList[sAttackerH]->m_iAddExp;
								dTmp2 = (double)iExp;
								dTmp3 = (dTmp1 / 100.0f)*dTmp2;
								iExp += (int)dTmp3;
							}

							// Crusade
							if ((gserver->m_bIsCrusadeMode == true) && (iExp > 10)) iExp = 10;

							//g_clientList[sAttackerH]->m_iExpStock += iExp;
							gserver->GetExp(sAttackerH, iExp);


							gserver->DeleteNpc(sTargetH);
							return false;
						}
					}

				CAE_SKIPDAMAGEMOVE2:;
				}
				else {
					gserver->SendEventToNearClient_TypeA(sTargetH, OWNERTYPE_NPC, MSGID_MOTION_DAMAGE, iDamage, sAttackerWeapon, NULL);
				}

				if (g_npcList[sTargetH]->m_cMagicEffectStatus[MAGICTYPE_HOLDOBJECT] == 1) {

					// 1: Hold-Person 
					g_npcList[sTargetH]->m_cMagicEffectStatus[MAGICTYPE_HOLDOBJECT] = NULL;
					gserver->RemoveFromDelayEventList(sTargetH, OWNERTYPE_NPC, MAGICTYPE_HOLDOBJECT);
				}
				else if (g_npcList[sTargetH]->m_cMagicEffectStatus[MAGICTYPE_HOLDOBJECT] == 2) {
					// 2: Paralize
					if ((g_npcList[sTargetH]->m_iHitDice > 50) && (dice(1, 10) == 5)) {
						g_npcList[sTargetH]->m_cMagicEffectStatus[MAGICTYPE_HOLDOBJECT] = NULL;
						gserver->RemoveFromDelayEventList(sTargetH, OWNERTYPE_NPC, MAGICTYPE_HOLDOBJECT);
					}
				}


				if ((g_npcList[sTargetH]->m_iNoDieRemainExp > 0) && (g_npcList[sTargetH]->m_bIsSummoned != true) &&
					(cAttackerType == OWNERTYPE_PLAYER) && (g_clientList[sAttackerH] != NULL)) {


					if (g_npcList[sTargetH]->m_iNoDieRemainExp > iDamage) {
						iExp = iDamage;
						g_npcList[sTargetH]->m_iNoDieRemainExp -= iDamage;
					}
					else {
						iExp = g_npcList[sTargetH]->m_iNoDieRemainExp;
						g_npcList[sTargetH]->m_iNoDieRemainExp = 0;
					}


					if (g_clientList[sAttackerH]->m_iAddExp != NULL) {
						dTmp1 = (double)g_clientList[sAttackerH]->m_iAddExp;
						dTmp2 = (double)iExp;
						dTmp3 = (dTmp1 / 100.0f)*dTmp2;
						iExp += (int)dTmp3;
					}

					if (gserver->m_bIsCrusadeMode == true) iExp = iExp / 3;


					if (g_clientList[sAttackerH]->m_iLevel > 100) {
						switch (g_npcList[sTargetH]->m_sType) {
						case 55:
						case 56:
							iExp = 0;
							break;
						default: break;
						}
					}

				}
			}*/
			break;
		}

		//CAE_SKIPCOUNTERATTACK:;

/*
		if (cAttackerType == OWNERTYPE_PLAYER) {

			if (g_clientList[sAttackerH]->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND] != -1)
				sWeaponIndex = g_clientList[sAttackerH]->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND];
			else sWeaponIndex = g_clientList[sAttackerH]->m_sItemEquipmentStatus[EQUIPPOS_RHAND];

			if (sWeaponIndex != -1) {
				if ((g_clientList[sAttackerH]->m_pItemList[sWeaponIndex] != NULL) &&
					(g_clientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_sIDnum != ITEM_PICKAXE)) {
					if (bKilled == false)
						CalculateSSN_ItemIndex(sAttackerH, sWeaponIndex, 1);
					else {
						if (g_clientList[sAttackerH]->m_iHP <= 3)
							CalculateSSN_ItemIndex(sAttackerH, sWeaponIndex, dice(1, iKilledDice) * 2);
						else CalculateSSN_ItemIndex(sAttackerH, sWeaponIndex, dice(1, iKilledDice));
					}
				}

				if ((g_clientList[sAttackerH]->m_pItemList[sWeaponIndex] != NULL) &&
					(g_clientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_wMaxLifeSpan != 0))
				{
					iWepLifeOff = 1;
					if ((wWeaponType >= 1) && (wWeaponType < 40)) {
						switch (g_mapList[g_clientList[sAttackerH]->m_cMapIndex]->m_weather)
						{
						case WEATHER_LIGHTRAIN:		if (dice(1, 3) == 1) iWepLifeOff++; break;
						case WEATHER_MEDIUMRAIN:	if (dice(1, 2) == 1) iWepLifeOff += dice(1, 2); break;
						case WEATHER_HEAVYRAIN:		if (dice(1, 2) == 1) iWepLifeOff += dice(1, 3); break;
						}
					}

					if (!g_clientList[sAttackerH]->IsNeutral()) {
						if (g_clientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_wCurLifeSpan < iWepLifeOff)
							g_clientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_wCurLifeSpan = 0;
						else g_clientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_wCurLifeSpan -= iWepLifeOff;
					}

					if (g_clientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_wCurLifeSpan == 0) {

						gserver->SendNotifyMsg(NULL, sAttackerH, NOTIFY_ITEMLIFESPANEND, g_clientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_cEquipPos, sWeaponIndex, NULL, NULL);

						gserver->ReleaseItemHandler(sAttackerH, sWeaponIndex, true);
					}
				}
			}
			else {
				if (wWeaponType == 0) {
					CalculateSSN_SkillIndex(sAttackerH, SKILL_HANDATTACK, 1);
				}
			}
		}*/
	}
	else {
/*
		if (cAttackerType == OWNERTYPE_PLAYER) {
			g_clientList[sAttackerH]->m_iComboAttackCount = 0;
		}*/
	}

	return iExp;
}

void GServer::EnduStrippingDamage(Unit * target, Unit * attacker, int item, int higherStripEndu, bool shield)
{
	int equiptItem, enduDamage = 1;
	float stripChance = 0;

/*
	if (g_clientList[sTargetH] == NULL) return;
	if (cAttackerType == OWNERTYPE_PLAYER && g_clientList[sAttackerH] == NULL) return;
	if (item == -1 || g_clientList[sTargetH]->m_pItemList[item] == NULL) return;

	if (g_clientList[sTargetH]->m_bIsSpecialAbilityEnabled == true &&
		g_clientList[sTargetH]->m_iSpecialAbilityType == 52) { //Mer shield active
		return;
	}

	if (cAttackerType == OWNERTYPE_PLAYER &&
		(g_clientList[sTargetH]->m_side != g_clientList[sAttackerH]->m_side || g_mapList[g_clientList[sAttackerH]->m_cMapIndex]->m_bIsFightZone))
	{
		switch (g_clientList[sAttackerH]->m_sUsingWeaponSkill)
		{
		case SKILL_HAMMER:
			stripChance = 0.25;
			enduDamage = 10;
			equiptItem = g_clientList[sAttackerH]->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND];
			if (equiptItem == -1 || g_clientList[sAttackerH]->m_pItemList[equiptItem] == NULL) break;

			switch (g_clientList[sAttackerH]->m_pItemList[equiptItem]->m_sIDnum)
			{ // Reworked Stripping on Hammers
			case ITEM_DAMEHAMMER:
				stripChance = 0.20f;
				enduDamage = 200;
				break;
			case ITEM_MASTERHAMMER:
				stripChance = 0.25f;
				enduDamage = 350;
				break;
			case ITEM_LEGENDHAMMER:
				stripChance = 0.45f;
				enduDamage = 600;
				break;
			case ITEM_GBHAMMER:
				stripChance = 0.55f;
				enduDamage = 950;
				break;
				/ *case ITEM_AGBHAMMER:
				stripChance = 0.60f;
				enduDamage = 1000;
				break;* /
			}
			break;

		case SKILL_AXE:
			enduDamage = 8; // Changed DMG to end on armors from 5 to 8 xRisenx
			break;

		default:
			enduDamage = 1;
			break;
		}
	}


	if (shield)
	{
		enduDamage = ceil(enduDamage * 0.2);
	}
	else if (g_clientList[sTargetH]->m_pItemList[item]->m_cCategory == 5) // shields
	{
		enduDamage = ceil(enduDamage * 0.8);
	}

	if (!g_clientList[sTargetH]->IsNeutral() &&
		g_clientList[sTargetH]->m_pItemList[item]->m_wCurLifeSpan > 0 &&
		memcmp(g_clientList[sTargetH]->m_pItemList[item]->m_cName, "Fantasy", 6) != 0) {
		g_clientList[sTargetH]->m_pItemList[item]->m_wCurLifeSpan -= enduDamage;
	}

	if (g_clientList[sTargetH]->m_pItemList[item]->m_wCurLifeSpan <= 0) {
		g_clientList[sTargetH]->m_pItemList[item]->m_wCurLifeSpan = 0;
		gserver->SendNotifyMsg(NULL, sTargetH, NOTIFY_ITEMLIFESPANEND, g_clientList[sTargetH]->m_pItemList[item]->m_cEquipPos, item, NULL, NULL);
		gserver->ReleaseItemHandler(sTargetH, item, true);
	}
	else if (stripChance > 0)
	{
		switch (g_clientList[sTargetH]->m_pItemList[item]->m_sIDnum)
		{
		case ITEM_MERIENPLATEMAIL_M:
		case ITEM_MERIENPLATEMAIL_W:
			return;
		}

		//if ( g_clientList[sTargetH]->m_pItemList[item]->m_wMaxLifeSpan <  higherStripEndu )
		if (g_clientList[sTargetH]->m_pItemList[item]->m_wMaxLifeSpan <  25000)
			stripChance *= dice(6, (g_clientList[sTargetH]->m_pItemList[item]->m_wMaxLifeSpan - g_clientList[sTargetH]->m_pItemList[item]->m_wCurLifeSpan));
		else
			stripChance *= dice(4, (g_clientList[sTargetH]->m_pItemList[item]->m_wMaxLifeSpan - g_clientList[sTargetH]->m_pItemList[item]->m_wCurLifeSpan));

		if (stripChance >  g_clientList[sTargetH]->m_pItemList[item]->m_wCurLifeSpan) {
			gserver->ReleaseItemHandler(sTargetH, item, true);
			gserver->SendNotifyMsg(NULL, sTargetH, NOTIFY_ITEMRELEASED, g_clientList[sTargetH]->m_pItemList[item]->m_cEquipPos, item, NULL, NULL);
		}
	}*/
}

int GServer::_iCalcSkillSSNpoint(int iLevel)
{
	int iRet = 0;

	if (iLevel < 1) return 1;

	if (iLevel <= 50) // Skill Gain Ratio xRisenx
		iRet = iLevel;
	else if (iLevel > 95)
		iRet = (iLevel * iLevel) / 10;
	else
		iRet = iLevel;

	return iRet;
}

void GServer::_CheckMiningAction(Client * client, int dX, int dY)
{
/*
	int16_t sType;
	uint64_t dwRegisterTime;
	int32_t   iDynamicIndex, iSkillLevel, iResult;
	int32_t   iMineralID = ITEM_COAL;
	Item * pItem;
	uint16_t  wWeaponType;

	if (client == nullptr)  return;


	client->RemoveMagicEffect(MAGICTYPE_INVISIBILITY);


	client->pMap->bGetDynamicObject(dX, dY, &sType, &dwRegisterTime, &iDynamicIndex);

	switch (sType) {
	case DYNAMICOBJECT_MINERAL1:
	case DYNAMICOBJECT_MINERAL2:

		wWeaponType = ((client->m_sAppr2 & 0x0FF0) >> 4);
		if (wWeaponType == 25) {
		}
		else return;

		if ((client->m_sAppr2 & 0xF000) == 0) return;

#ifdef TitleHG
		iSkillLevel = (client->m_stTitles[TITLE_INDEX_CRAFT_MINING].m_iRank * 3) + client->m_iAddMining; // New Skill % To Items xRisenx

#else
		iSkillLevel = client->m_cSkillMastery[SKILL_MINING];
		if (iSkillLevel == 0) break;

		if (m_pDynamicObjectList[iDynamicIndex] == NULL) break;
#endif

		iSkillLevel -= m_pMineral[m_pDynamicObjectList[iDynamicIndex]->m_iV1]->m_iDifficulty;
#ifdef TitleHG
		if (iSkillLevel < 4) iSkillLevel = 4;
#else
		if (iSkillLevel <= 0) iSkillLevel = 1;
#endif
		iResult = dice(1, 100);
		if (iResult <= iSkillLevel) {
#ifdef TitleHG
			increaseTitlePoints(iClientH, TITLE_INDEX_CRAFT_MINING, 1);
#else
			CalculateSSN_SkillIndex(client, SKILL_MINING, 1);
#endif

			switch (m_pMineral[m_pDynamicObjectList[iDynamicIndex]->m_iV1]->m_cType) {
			case 1:
				switch (dice(1, 6)) {
				case 1:
				case 2:
				case 3:
				case 4:
					iMineralID = ITEM_COAL; // Coal
					GetExp(iClientH, dice(1, 3));
					break;
				case 5:
					iMineralID = ITEM_IRONORE; // IronOre
					GetExp(iClientH, dice(1, 3));
					break;
				case 6:
					iMineralID = ITEM_BLONDESTONE; // BlondeStone
					GetExp(iClientH, dice(1, 3));
					break;
				}
				break;

			case 2:
				switch (dice(1, 7)) {
				case 1:
				case 2:
				case 3:
					iMineralID = ITEM_COAL; // Coal
					GetExp(iClientH, dice(1, 3));
					break;
				case 4:
				case 5:
				case 6:
					iMineralID = ITEM_IRONORE; // IronOre
					GetExp(iClientH, dice(1, 3));
					break;
				case 7:
					if (dice(1, 3) == 2) {
						iMineralID = ITEM_SILVERNUGGET; // SilverNugget
						GetExp(iClientH, dice(1, 4));
					}
					else {
						iMineralID = ITEM_BLONDESTONE; // BlondeStone
						GetExp(iClientH, dice(1, 3));
					}
					break;
				}
				break;

			case 3:
				switch (dice(1, 6)) {
				case 1:
					iMineralID = ITEM_COAL; // Coal
					GetExp(iClientH, dice(1, 3));
					break;
				case 2:
				case 3:
				case 4:
				case 5:
					iMineralID = ITEM_IRONORE; // IronOre
					GetExp(iClientH, dice(1, 3));
					break;
				case 6:
					if (dice(1, 8) == 3) {
						if (dice(1, 2) == 1) {
							iMineralID = ITEM_SILVERNUGGET; // SilverNugget
							GetExp(iClientH, dice(1, 4));
						}
						else {
							iMineralID = ITEM_BLONDESTONE; // BlondeStone
							GetExp(iClientH, dice(1, 3));
						}
						break;
					}
					else {
						iMineralID = ITEM_IRONORE; // IronOre
						GetExp(iClientH, dice(1, 3));
					}
					break;
				}
				break;

			case 4:
				switch (dice(1, 6)) {
				case 1:
				case 2:
					iMineralID = ITEM_COAL; // Coal
					GetExp(iClientH, dice(1, 3));
					break;
				case 3:
					if (dice(1, 3) == 2) {
						iMineralID = ITEM_SILVERNUGGET; // SilverNugget
						GetExp(iClientH, dice(1, 4));
					}
					else{
						iMineralID = ITEM_COAL; // Coal
						GetExp(iClientH, dice(1, 3));
					}
					break;
				case 4:
				case 5:
					iMineralID = ITEM_IRONORE; // IronOre
					GetExp(iClientH, dice(1, 3));
					break;
				case 6:
					if (dice(1, 3) == 3)
					{
						// 0.33333
						if (dice(1, 2) == 1)
						{
							// 0.5
							if (dice(1, 2) == 1)
							{
								// 0.5
								iMineralID = ITEM_MITHRAL;
								GetExp(iClientH, dice(1, 30));
							}
							else {
								if (dice(1, 100) <= 75)
								{
									// 0.75
									iMineralID = ITEM_GOLDNUGGET;
									GetExp(iClientH, dice(1, 5));
								}
								else {
									// 0.25
									iResult = dice(1, 10000);
									if (iResult <= 1600)
									{
										iResult = dice(1, 10000);
										if (iResult <= 7000)
											iMineralID = ITEM_ARMORGEM7;
										else if (iResult <= 9980)
											iMineralID = ITEM_ARMORGEM14;
										else if (iResult <= 10000)
											iMineralID = ITEM_ARMORGEM21;
									}
									else if (iResult <= 3200)
									{
										iResult = dice(1, 10000);
										if (iResult <= 7000)
											iMineralID = ITEM_MINDGEM7;
										else if (iResult <= 9980)
											iMineralID = ITEM_MINDGEM14;
										else if (iResult <= 10000)
											iMineralID = ITEM_MINDGEM21;
									}
									else if (iResult <= 6300)
									{
										iResult = dice(1, 10000);
										if (iResult <= 7000)
											iMineralID = ITEM_BLOODGEM7;
										else if (iResult <= 9980)
											iMineralID = ITEM_BLOODGEM14;
										else if (iResult <= 10000)
											iMineralID = ITEM_BLOODGEM21;
									}
									else if (iResult <= 9400)
									{
										iResult = dice(1, 10000);
										if (iResult <= 7000)
											iMineralID = ITEM_REJUGEM7;
										else if (iResult <= 9980)
											iMineralID = ITEM_REJUGEM14;
										else if (iResult <= 10000)
											iMineralID = ITEM_REJUGEM21;
									}
									else if (iResult <= 9700)
									{
										iResult = dice(1, 10000);
										if (iResult <= 7000)
											iMineralID = ITEM_TACTICALGEM3;
										else if (iResult <= 9980)
											iMineralID = ITEM_TACTICALGEM5;
										else if (iResult <= 10000)
											iMineralID = ITEM_TACTICALGEM7;
									}
									else if (iResult <= 9999)
									{
										iResult = dice(1, 10000);
										if (iResult <= 7000)
											iMineralID = ITEM_ENCHANTEDGEM2;
										else if (iResult <= 9980)
											iMineralID = ITEM_ENCHANTEDGEM4;
										else if (iResult <= 10000)
											iMineralID = ITEM_ENCHANTEDGEM6;
									}
									else if (iResult <= 10000)
									{
										iMineralID = ITEM_VORTEXGEM;
									}

									GetExp(iClientH, dice(1, 60));
								}
							}
							break;
						}
						else {
							// 0.5
							iMineralID = ITEM_SILVERNUGGET; // SilverNugget
							GetExp(iClientH, dice(1, 4));
						}
						break;
					}
					else
					{
						// 0.77777
						if (dice(1, 2) == 1)
						{
							iMineralID = ITEM_GOLDNUGGET; // GoldNugget
							GetExp(iClientH, dice(1, 5));
						}
						else {
							iMineralID = ITEM_IRONORE;  // IronOre
							GetExp(iClientH, dice(1, 3));
						}
						break;
					}
					break;
				}
				break;

			case 5:
				switch (dice(1, 19)) {
				case 3:
					iMineralID = ITEM_SAPPHIRE; // Sapphire
					GetExp(iClientH, dice(2, 3));
					break;
				default:
					iMineralID = ITEM_CRYSTAL; // Crystal
					GetExp(iClientH, dice(2, 3));
					break;
				}
				break;

			case 6:
				switch (dice(1, 5)) {
				case 1:
					if (dice(1, 6) == 3) {
						iMineralID = ITEM_EMERALD; // Emerald 
						GetExp(iClientH, dice(2, 4));
					}
					else {
						iMineralID = ITEM_CRYSTAL; // Crystal
						GetExp(iClientH, dice(2, 3));
					}
					break;
				case 2:
					if (dice(1, 6) == 3) {
						iMineralID = ITEM_SAPPHIRE; // Saphire
						GetExp(iClientH, dice(2, 4));
					}
					else {
						iMineralID = ITEM_CRYSTAL; // Crystal
						GetExp(iClientH, dice(2, 3));
					}
					break;
				case 3:
					if (dice(1, 6) == 3) {
						iMineralID = ITEM_RUBY; // Ruby
						GetExp(iClientH, dice(2, 4));
					}
					else {
						iMineralID = ITEM_CRYSTAL; // Crystal 
						GetExp(iClientH, dice(2, 3));
					}
					break;
				case 4:
					iMineralID = ITEM_CRYSTAL; // Crystal 
					GetExp(iClientH, dice(2, 3));
					break;
				case 5:
					if (dice(1, 12) == 3) {
						iMineralID = ITEM_DIAMOND; // Diamond
						GetExp(iClientH, dice(2, 5));
					}
					else {
						iMineralID = ITEM_CRYSTAL; // Crystal
						GetExp(iClientH, dice(2, 3));
					}
					break;
				}
				break;

			}

			pItem = new class CItem;
			if (pItem->InitItemAttr(iMineralID) == false) {
				delete pItem;
			}
			else {

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
					m_pClientList[iClientH]->m_sY, pItem);

				SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
					m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
					pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor);
			}

			m_pMineral[m_pDynamicObjectList[iDynamicIndex]->m_iV1]->m_iRemain--;
			if (m_pMineral[m_pDynamicObjectList[iDynamicIndex]->m_iV1]->m_iRemain <= 0) {
				bDeleteMineral(m_pDynamicObjectList[iDynamicIndex]->m_iV1);
				delete m_pDynamicObjectList[iDynamicIndex];
				m_pDynamicObjectList[iDynamicIndex] = NULL;
			}
		}
		break;

	default:
		break;
	}*/
}

bool GServer::bDeleteMineral(int iIndex)
{
/*
	int iDynamicIndex;
	uint32_t dwTime;

	dwTime = unixtime();

	if (m_pMineral[iIndex] == NULL) return false;
	iDynamicIndex = m_pMineral[iIndex]->m_sDynamicObjectHandle;
	if (m_pDynamicObjectList[iDynamicIndex] == NULL) return false;

	SendEventToNearClient_TypeB(MSGID_DYNAMICOBJECT, MSGTYPE_REJECT, m_pDynamicObjectList[iDynamicIndex]->m_cMapIndex,
		m_pDynamicObjectList[iDynamicIndex]->m_sX, m_pDynamicObjectList[iDynamicIndex]->m_sY,
		m_pDynamicObjectList[iDynamicIndex]->m_sType, iDynamicIndex, NULL);
	m_pMapList[m_pDynamicObjectList[iDynamicIndex]->m_cMapIndex]->SetDynamicObject(NULL, NULL, m_pDynamicObjectList[iDynamicIndex]->m_sX, m_pDynamicObjectList[iDynamicIndex]->m_sY, dwTime);
	m_pMapList[m_pMineral[iIndex]->m_cMapIndex]->SetTempMoveAllowedFlag(m_pDynamicObjectList[iDynamicIndex]->m_sX, m_pDynamicObjectList[iDynamicIndex]->m_sY, true);

	m_pMapList[m_pMineral[iIndex]->m_cMapIndex]->m_iCurMineral--;

	delete m_pMineral[iIndex];
	m_pMineral[iIndex] = NULL;*/

	return true;
}

void GServer::GetExp(Client * client, uint64_t iExp, bool bIsAttackerOwn)
{
	double dV1, dV2, dV3;
	int iUnitValue;
	uint64_t dwTime = unixtime();

	if (client == nullptr) return;

	if (client->level <= 80) {
		dV1 = (double)(80 - client->level);
		dV2 = dV1 * 0.025f;
		dV3 = (double)iExp;
		dV1 = (dV2 + 1.025f)*dV3;
		iExp = (int)dV1;
	}
	else {
		if ((client->level >= 100) && (client->map->name == "arefarm")
			|| (client->map->name == "elvfarm")) {
			iExp = (iExp / 10);
		}
		else if ((client->map->name == "arefarm")
			|| (client->map->name == "elvfarm")) {
			iExp = (iExp * 1 / 4);
		}
	}

	Party * party = client->GetParty();

	if (dwTime - client->m_dwLastActionTime >= 5*60*1000 )
		return;

	if (party)
	{
		if (iExp >= 10 && party->GetPlayerCnt() > 1)
		{
			int iPartyTotalMember = party->GetAlivePlayerCnt();

			dV1 = (double)iExp;

			if (iPartyTotalMember == 1)	dV2 = dV1;
			else dV2 = (dV1 * ((log((float)(iPartyTotalMember + 2) * 9) / 9) + 0.65)) / iPartyTotalMember;

			dV3 = dV2 + 0.5f;
			iUnitValue = (int)dV3;

			//TODO: turn into for ( : )
			while (Client * member = party->GetMembers())
			{
				if (member->IsDead())
					continue;

				uint32_t exp = 0;
				if (member->level == PLAYERMAXLEVEL)
					exp += (iUnitValue / 3);
				else
					exp += iUnitValue;

				if ((member->status & STATUS_GREENSLATE) != 0)
					exp *= 3;

				member->m_iExpStock += exp;
			}

			if ((client->status & STATUS_GREENSLATE) != 0) iUnitValue *= 3;
			if (client->level == PLAYERMAXLEVEL)
				iUnitValue /= 3;
			if ((bIsAttackerOwn == true) && (iPartyTotalMember > 1))
				client->m_iExpStock += (int)(iUnitValue / 10);

		}
		else
		{
			if ((client->status & STATUS_GREENSLATE) != 0) iExp *= 3;
			if (client->level == PLAYERMAXLEVEL)
				iExp /= 3;
			client->m_iExpStock += iExp;
		}
	} // if
	else
	{
		if ((client->status & STATUS_GREENSLATE) != 0) iExp *= 3;
		if (client->level == PLAYERMAXLEVEL)
			iExp /= 3;
		client->m_iExpStock += iExp;
	}

	// Xp System xRisenx
	if (client->level >= 1 && client->level < 205)
		iExp *= 100;
	client->m_iExpStock += iExp;
	//else if (m_pClientList[iClientH]->m_iLevel >= 50 && m_pClientList[iClientH]->m_iLevel <= 99)
	//	iExp *= 3;
	//else if (m_pClientList[iClientH]->m_iLevel >= 100 && m_pClientList[iClientH]->m_iLevel <= 149)
	//	iExp *= 4;
	//else if (m_pClientList[iClientH]->m_iLevel >= 150 && m_pClientList[iClientH]->m_iLevel < 175)
	//	iExp *= 5;
	//if (m_pClientList[iClientH]->m_iLevel >= 1 && m_pClientList[iClientH]->m_iLevel < 200)
	//	iExp *= 17; // Raised a little xRisenx


}

int GServer::iAddDynamicObjectList(Unit * owner, short sType, Map * pmap, short sX, short sY, uint32_t dwLastTime, int iV1)
{
/*
	int i;
	short sPreType;
	uint32_t dwTime, dwRegisterTime;

	m_pMapList[cMapIndex]->bGetDynamicObject(sX, sY, &sPreType, &dwRegisterTime);
	if (sPreType != NULL) return NULL;

	switch (sType) {
		case DYNAMICOBJECT_FIRE:
			if (m_pMapList[cMapIndex]->bGetIsMoveAllowedTile(sX, sY) == false)
				return NULL;

			if (dwLastTime != NULL)  {
				switch (m_pMapList[cMapIndex]->m_weather) {
					case WEATHER_LIGHTRAIN:		dwLastTime = dwLastTime - (dwLastTime / 2);       break;
					case WEATHER_MEDIUMRAIN:	dwLastTime = (dwLastTime / 2) - (dwLastTime / 3); break;
					case WEATHER_HEAVYRAIN:		dwLastTime = (dwLastTime / 3) - (dwLastTime / 4); break;
				}

				if (dwLastTime == NULL) dwLastTime = 1000;
			}
			break;

		case DYNAMICOBJECT_FISHOBJECT:
		case DYNAMICOBJECT_FISH:
			if (m_pMapList[cMapIndex]->bGetIsWater(sX, sY) == false)
				return NULL;
			break;

		case DYNAMICOBJECT_MINERAL1:
		case DYNAMICOBJECT_MINERAL2:
			if (m_pMapList[cMapIndex]->bGetMoveable(sX, sY) == false)
				return NULL;
			m_pMapList[cMapIndex]->SetTempMoveAllowedFlag(sX, sY, false);
			break;
		case DYNAMICOBJECT_ARESDENFLAG:
		case DYNAMICOBJECT_ELVINEFLAG:
			m_pMapList[cMapIndex]->SetTempMoveAllowedFlag(sX, sY, false);
			break;
	}


	for (i = 1; i < MAXDYNAMICOBJECTS; i++)
	{
		if (m_pDynamicObjectList[i] == NULL)
		{
			dwTime = unixtime();

			if (dwLastTime != NULL)
				dwLastTime += (dice(1, 4) * 1000);

			// VAMP - here the dynamic object is given the caster's ownerId and ownerType
			m_pDynamicObjectList[i] = new DynamicObject(sOwner, cOwnerType, sType, cMapIndex, sX, sY, dwTime, dwLastTime, iV1);
			m_pMapList[cMapIndex]->SetDynamicObject(i, sType, sX, sY, dwTime);
			SendEventToNearClient_TypeB(MSGID_DYNAMICOBJECT, MSGTYPE_CONFIRM, cMapIndex, sX, sY, sType, i, NULL);

			return i;
		}
	}
*/

	return 0;
}

void GServer::CheckDynamicObjectList()
{
/*
	int i;
	uint32_t dwTime = unixtime(), dwRegisterTime;
	short sType;

	for (i = 1; i < MAXDYNAMICOBJECTS; i++) {
		if (m_pDynamicObjectList[i] && m_pDynamicObjectList[i]->m_dwLastTime != 0)
		{

			switch (m_pDynamicObjectList[i]->m_sType)
			{
				case DYNAMICOBJECT_FIRE:

					switch (m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->m_weather)
					{
						case WEATHER_LIGHTRAIN:
						case WEATHER_MEDIUMRAIN:
						case WEATHER_HEAVYRAIN:
							m_pDynamicObjectList[i]->m_dwLastTime = m_pDynamicObjectList[i]->m_dwLastTime -
								(m_pDynamicObjectList[i]->m_dwLastTime / 10) * m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->m_weather;
							break;
					}
					break;
			}

			if (dwTime - m_pDynamicObjectList[i]->m_dwRegisterTime >= m_pDynamicObjectList[i]->m_dwLastTime)
			{
				m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->bGetDynamicObject(m_pDynamicObjectList[i]->m_sX, m_pDynamicObjectList[i]->m_sY, &sType, &dwRegisterTime);

				if (dwRegisterTime == m_pDynamicObjectList[i]->m_dwRegisterTime) {
					SendEventToNearClient_TypeB(MSGID_DYNAMICOBJECT, MSGTYPE_REJECT, m_pDynamicObjectList[i]->m_cMapIndex, m_pDynamicObjectList[i]->m_sX, m_pDynamicObjectList[i]->m_sY, m_pDynamicObjectList[i]->m_sType, i, NULL);
					m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->SetDynamicObject(NULL, NULL, m_pDynamicObjectList[i]->m_sX, m_pDynamicObjectList[i]->m_sY, dwTime);
				}

				switch (sType)
				{
					case DYNAMICOBJECT_FISHOBJECT:
					case DYNAMICOBJECT_FISH:
						bDeleteFish(m_pDynamicObjectList[i]->m_sOwner, 2);
						break;
				}

				delete m_pDynamicObjectList[i];
				m_pDynamicObjectList[i] = NULL;
			}
		}
	}*/
}

bool GServer::bAnalyzeCriminalAction(Client * client, short dX, short dY, bool bIsCheck)
{
	uint16_t tX, tY;
	string cNpcName;
	char  cNpcWaypoint[11];

	if (client == nullptr) return false;
	if (client->m_bIsInitComplete == false) return false;
	if (m_bIsCrusadeMode == true) return false;
	if (client->map->flags.fightZone) return false;

	shared_ptr<Unit> target = client->map->GetOwner(dX, dY);

	if ((target->IsPlayer()) && (target != nullptr))
	{
		if (_bGetIsPlayerHostile(client, static_cast<Client*>(target.get())) != true)
		{
			if (bIsCheck == true) return true;

			if (client->map->name == sideMap[ARESDEN])
				cNpcName = "Guard-Aresden";
			else if (client->map->name == sideMap[ELVINE])
				cNpcName = "Guard-Elvine";
			else  if (client->map->name == sideMap[NEUTRAL])
				cNpcName = "Guard-Neutral";
			else
				return false;

			memset(cNpcWaypoint, 0, sizeof(cNpcWaypoint));

			tX = (int)client->x;
			tY = (int)client->y;

			shared_ptr<Npc> guard = client->map->CreateNpc(cNpcName, 0, MOVETYPE_RANDOM,
									 &tX, &tY, (Side)-1, cNpcWaypoint, 0, 0, false, true);

			if (guard)
			{
				guard->SetTarget(client->self.lock());
			}
		}
	}

	return false;
}

int GServer::_iGetWeaponSkillType(Client * client)
{
	uint16_t wWeaponType;

	if (client == nullptr) return 0;


	wWeaponType = ((client->m_sAppr2 & 0x0FF0) >> 4);

	if (wWeaponType == 0) {
		return 5;
	}
	else if ((wWeaponType >= 1) && (wWeaponType <= 2)) {

		return 7;
	}
	else if ((wWeaponType > 2) && (wWeaponType < 20)) {
		if (wWeaponType == 7)
			return 9;
		else return 8;
	}
	else if ((wWeaponType >= 20) && (wWeaponType < 30)) {
		return 10;
	}
	else if ((wWeaponType >= 30) && (wWeaponType < 35)) {

		return 14;
	}
	else if ((wWeaponType >= 35) && (wWeaponType < 40)) {

		return 21;
	}
	else if (wWeaponType >= 40) {
		return 6;
	}

	return 1;
}

//                             1 2 3 
static int ___iCAB5[] = { 0, 0, 0, 1, 2 };
static int ___iCAB6[] = { 0, 0, 0, 0, 0 };
static int ___iCAB7[] = { 0, 0, 1, 2, 3 };
static int ___iCAB8[] = { 0, 0, 1, 3, 5 };
static int ___iCAB9[] = { 0, 0, 2, 4, 8 };
static int ___iCAB10[] = { 0, 0, 1, 2, 3 };

int16_t GServer::iGetComboAttackBonus(int16_t iSkill, int16_t iComboCount)
{
	if (iComboCount <= 1) return 0;
	if (iComboCount > 6) return 0;
	switch (iSkill) {
		case 5: 		return ___iCAB5[iComboCount];
			break;
		case 6: 		return ___iCAB6[iComboCount];
			break;
		case 7:
			return ___iCAB7[iComboCount];
			break;
		case 8: 		return ___iCAB8[iComboCount];
			break;
		case 9:
			return ___iCAB9[iComboCount];
			break;
		case 10: 		return ___iCAB10[iComboCount];
			break;
		case 14:
			return ___iCAB6[iComboCount];
			break;
		case 21:
			return ___iCAB10[iComboCount];
			break;
	}

	return 0;
}

void GServer::RequestRestartHandler(shared_ptr<Client> player)
{
	if (!player->_dead) return;

	//perform buff/debuff removals for non-persistents
	//player->OnDeath(); //or something similar
	if (player->magicEffectStatus[MAGICTYPE_CONFUSE] != 0)
		player->RemoveMagicEffect(MAGICTYPE_CONFUSE);


	if (player->IsGM())
	{
		//GM respawns in place by default
		//do nothing
	}
	else
	{
		//Player is in enemy town and is not an admin, send to jail and lock for 3 minutes
		if (player->IsAres())
		{
			if (player->mapName == sideMap[ELVINE])
			{
				player->lockedMapName = sideMapJail[ELVINE];
				player->lockedMapTime = lockedMapTimeDefault;
				player->mapName = sideMapJail[ELVINE];
			}
			else if (player->level < farmRestartLimit)
			{
				player->mapName = sideMapFarm[ARESDEN];
			}
		}
		else if (player->IsElv())
		{
			if (player->mapName == sideMap[ARESDEN])
			{
				player->lockedMapName = sideMapJail[ARESDEN];
				player->lockedMapTime = lockedMapTimeDefault;
				player->mapName = sideMapJail[ARESDEN];
			}
			else if (player->level < farmRestartLimit)
			{
				player->mapName = sideMapFarm[ELVINE];
			}
		}
		else
		{
			player->mapName = sideMap[NEUTRAL];
		}
	}


	player->_dead = false;
	player->health = player->GetMaxHP();
	player->m_iHungerStatus = 100;

	player->map->ClearDeadOwner(player->x, player->y);

	RequestTeleportHandler(player.get(), 2, player->mapName);
}

void GServer::RemoveFromDelayEventList(Unit * unit, int32_t iEffectType)
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

bool GServer::RegisterDelayEvent(int iDelayType, int iEffectType, uint64_t dwLastTime, Unit * unit, Map * pmap, int dX, int dY, int iV1, int iV2, int iV3)
{
	shared_ptr<DelayEvent> delayevent(new DelayEvent);


	delayevent->m_iDelayType = iDelayType;
	delayevent->m_iEffectType = iEffectType;

	delayevent->m_pMap = pmap;
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

//NOTE: if this becomes a bit bloaty, move Unit timers to their respective classes
//needs live testing to see if this would cause any issues with potentially thousands of delay events
//constantly firing and being checked every ~100ms
void GServer::DelayEventProcessor()
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

					if (!player->skillInUse[skillnum] || player->skillInUse[skillnum] != delayevent->m_iV2) break;

					player->skillInUse[skillnum] = 0;
					player->skillInUseTime[skillnum] = 0;

					int32_t result = CalculateUseSkillItemEffect(player, delayevent->m_iV1, skillnum, player->map, delayevent->m_dX);

					SendNotifyMsg(nullptr, player, NOTIFY_SKILLUSINGEND, result);
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

int32_t GServer::CalculateUseSkillItemEffect(Client * player, int16_t skillvalue, int skillnum, Map * map, int32_t x, int32_t y)
{
	int32_t dX, dY, itemid = 0;
	if (player == nullptr) return 0;
	if (player->map != map) return 0;
	if (skillnum == 0) return 0;
	if (skillvalue == 0) return 0;
	if (m_pSkillConfigList[skillnum] == nullptr) return 0;
	dX = player->x;
	dY = player->y;

	int result = dice(1, 105);
	if (skillvalue <= result) return 0;

	switch (m_pSkillConfigList[skillnum]->m_sType)
	{
	case SKILLEFFECTTYPE_TAMING:
		//_TamingHandler(player, skillnum, map, dX, dY);//??
		break;
	case SKILLEFFECTTYPE_GET:
		switch (m_pSkillConfigList[skillnum]->m_sValue1)
		{
			case 1:
				itemid = 99;
				break;
			case 2:
			{
				if (!map->bGetIsWater(dX, dY)) return 0;
				int fish = 0;// iCheckFish(player, map, dX, dY);
				if (fish == 0) itemid = 100;

				if (itemid != 0)
				{
					if (itemid == 100)
					{
						SendNotifyMsg(nullptr, player, NOTIFY_FISHSUCCESS);
						GetExp(player, dice(2, 5));

						Item * item = new Item;
						if (item->InitItemAttr(itemid, m_pItemConfigList))
						{
							map->bSetItem(dX, dY, item);
							SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, COMMONTYPE_ITEMDROP, map, dX, dY, item->m_sSprite, item->m_sSpriteFrame, item->m_ItemColor);
						}
					}
				}
			}
		}
		break;
	}
	return 1;
}
