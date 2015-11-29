
#pragma once

#include "common.h"
#include "Server.h"
#include "netmessages.h"

// for Client Data
#define MAXTELEPORTLOC		200
#define MAXWAYPOINTCFG		200
#define MAXMGAR				50
#define MAXNMR				50
#define MAXSPOTMOBGENERATOR	100
#define MAXFISHPOINT		200
#define MAXMINERALPOINT		200
#define MAXOCCUPYFLAG		20001 //3001
#define MAXINITIALPOINT		20

#define MAPTYPE_NORMAL				0
#define MAPTYPE_NOPENALTY_NOREWARD	1
#define MAXENERGYSPHERES			10

#define MAXSTRATEGICPOINTS			200
#define MAXSECTORS					60
#define MAXSTRIKEPOINTS				20

#define MAXITEMEVENTS				50

#define MAXHELDENIANDOOR	10 
#define MAXDYNAMICGATES		10
#define MAXHELDENIANTOWER	200

enum ApocMobGenType{
	AMGT_NONE,
	AMGT_OPENGATE,
	AMGT_SPAWNBOSS
};

class Unit;
class Item;
class Tile;
class Client;
class GServer;
class TeleportLoc;

class Map  
{
public:


	void run();

	thread * actionthread;
	thread * timerthread;
	Server::MsgQueue actionpipe;
	mutex mutaction;
	bool TimerThreadRunning;

	void TimerThread();
	void SocketThread();

	
	
	void NpcProcess();

	void RemoveFromTarget(shared_ptr<Unit> target, int iCode = 0);
	void NpcKilledHandler(shared_ptr<Unit> attacker, shared_ptr<Npc> npc, int16_t damage);
	void NpcBehavior_Flee(shared_ptr<Npc> npc);
	void NpcBehavior_Dead(shared_ptr<Npc> npc);
	void NpcDeadItemGenerator(shared_ptr<Unit> attacker, boost::shared_ptr<Npc> npc);

	shared_ptr<Npc> CreateNpc(string & pNpcName, char cSA, char cMoveType, uint16_t * poX, uint16_t * poY, Side changeSide, char * pWaypointList, rect * pArea, int iSpotMobIndex, bool bHideGenMode = false, bool bIsSummoned = false, bool bFirmBerserk = false, bool bIsMaster = false, int iGuildGUID = 0);
	void DeleteNpc(shared_ptr<Npc> npc);

	char cGetNextMoveDir(short sX, short sY, short dstX, short dstY, Map * map, char cTurn, int * pError);
	char cGetNextMoveDir(short sX, short sY, short dstX, short dstY, Map * map, char cTurn, int * pError, short * DOType);

	void RemoveFromDelayEventList(Unit * unit, int32_t iEffectType);
	bool RegisterDelayEvent(int iDelayType, int iEffectType, uint64_t dwLastTime, Unit * unit, int dX, int dY, int iV1, int iV2, int iV3);
	void DelayEventProcessor();
	bool bGetEmptyPosition(short & pX, short & pY, boost::shared_ptr<Unit> client);

	shared_ptr<Npc> GetNpc(uint64_t ObjectID);

	std::list<shared_ptr<Npc>> npclist;
	uint64_t npchandle;

	struct
	{
		Item * item;
		time_t dropTime;
		uint16_t sx, sy;
		Map * cMapIndex;
		bool bEmpty;
	} m_stGroundNpcItem[MAXGROUNDITEMS];

	std::list<shared_ptr<DelayEvent>> DelayEventList;
	mutex delayMutex;



	void RestoreStrikePoints();
	bool bRemoveCrusadeStructureInfo(short sX, short sY);
	bool bAddCrusadeStructureInfo(char cType, short sX, short sY, char cSide);
	int iGetAttribute(int dX, int dY, int iBitMask);
	void _SetupNoAttackArea();
	void ClearTempSectorInfo();
	void ClearSectorInfo();
	uint64_t iCheckItem(short sX, short sY);
	void SetTempMoveAllowedFlag(int dX, int dY, bool bFlag);
	int iAnalyze(char cType, int *pX, int *pY, int * pV1, int *pV2, int * pV3);
	bool bGetIsWater(short dX, short dY);
	bool bGetIsFarm(short dX, short dY);
	bool IsBuild(short dX, short dY);
	bool bAddCropsTotalSum();
	bool bRemoveCropsTotalSum();
	bool bGetIsMoveAllowedTile(short dX, short dY);
	bool bGetIsMoveAllowedTile(Point p);
	bool bGetDynamicObject(short sX, short sY, short * pType, uint64_t * pRegisterTime, int * pIndex = 0);
	void SetDynamicObject(uint16_t wID, short sType, short sX, short sY, uint64_t dwRegisterTime);
	bool bGetIsTeleport(short dX, short dY);
	bool bSearchTeleportDest(int sX, int sY, string & pMapName, uint16_t * pDx, uint16_t * pDy, uint8_t * pDir);
	bool bInit(string pName);
	bool bIsValidLoc(short sX, short sY);
	Item * pGetItem(short sX, short sY, short * pRemainItemSprite, short * pRemainItemSpriteFrame, uint32_t * pRemainItemColor);
	bool bSetItem(short sX, short sY, Item * pItem);
	void ClearDeadOwner(short sX, short sY);
	void ClearOwner(short sX, short sY);
	bool bGetMoveable(short dX, short dY, short * pDOtype = 0, Item * pTopItem = 0); 
	shared_ptr<Unit> GetOwner(short sX, short sY);
	void SetOwner(shared_ptr<Unit> sOwner, short sX, short sY);
	shared_ptr<Unit> GetDeadOwner(short sX, short sY);
	void SetDeadOwner(shared_ptr<Unit> sOwner, short sX, short sY);
	std::list<shared_ptr<Unit>>GetOwners(short x1, short x2, short y1, short y2);
	void IncPlayerActivity(Client * player);
	bool bDecodeMapConfig();
	bool GetInitialPoint(int16_t *pX, int16_t *pY, string & pPlayerLocation);
	Tile * GetTile(int16_t x, int16_t y);

	Map(GServer * pGame);
	~Map();

	GServer * gserver;
	string name;
	string factionName;
	short sizeX, sizeY, tileDataSize;
	std::vector<TeleportLoc *> teleportLocationList;

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
	string  dynamicGateCoordDestMap;
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

	int m_iMaxNx, m_iMaxNy, m_iMaxAx, m_iMaxAy, m_iMaxEx, m_iMaxEy, m_iMaxMx, m_iMaxMy, m_iMaxPx, m_iMaxPy;//??

	struct strikepoint {
		Map * map;
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
		string cItemName;
		int iAmount;
		int iTotalNum;				
		int iMonth;
		int iDay;
		int iType;
		char *cMob[5];

		int iCurNum;
		int	iNumMob;
	};
	std::vector<itemeventlist> itemEventList;

	short mobEventAmount ;

	int chatZone;//?

	Map::OccupyFlag * iRegisterOccupyFlag(int dX, int dY, int iSide, int iEKNum, int iDOI);

private:
	Tile * _tile;
	bool _bDecodeMapDataFileContents();
};

