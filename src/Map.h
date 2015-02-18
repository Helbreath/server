
#pragma once

#include "common.h"
#include "funcs.h"

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
	Unit * GetOwner(short sX, short sY);
	void SetOwner(Unit * sOwner, short sX, short sY);
	Unit * GetDeadOwner(short sX, short sY);
	void SetDeadOwner(Unit * sOwner, short sX, short sY);
	Unit ** GetOwners(short x1, short x2, short y1, short y2);
	void IncPlayerActivity(Client * player);
	bool bDecodeMapConfig();
	bool GetInitialPoint(int16_t *pX, int16_t *pY, string & pPlayerLocation);

	Map(GServer * pGame);
	~Map();

	Tile * m_pTile;
	GServer * gserver;
	string m_cName;
	string m_cLocationName;
	short m_sSizeX, m_sSizeY, m_sTileDataSize;
	std::vector<TeleportLoc *> m_pTeleportLoc;

	//short m_sInitialPointX, m_sInitialPointY;
	std::vector<point> m_pInitialPoint;

	bool m_bIsApocalypseMap;
	ApocMobGenType m_iApocalypseMobGenType;
	int m_iApocalypseBossMobNpcID;
	rect	m_sApocalypseBossMobRect;

	char  m_cDynamicGateType;
	short m_sDynamicGateCoordRectX1, m_sDynamicGateCoordRectY1, m_sDynamicGateCoordRectX2, m_sDynamicGateCoordRectY2;
	string m_cDynamicGateCoordDestMap;
	short m_sDynamicGateCoordTgtX, m_sDynamicGateCoordTgtY;

	char  m_cHeldenianModeMap;
	bool m_bIsHeldenianMap;
	short m_sHeldenianWinningZoneX;
	short m_sHeldenianWinningZoneY;

	struct heldgatedoor {
		char  cDir;
		short dX;
		short dY;
	};
	std::vector<heldgatedoor> m_stHeldenianGateDoor;

	struct heldtower {
		short sTypeID;
		short dX;
		short dY;
		char  cSide;
	};
	std::vector<heldtower> m_stHeldenianTower;

	bool  m_bRandomMobGenerator;
	uint16_t  m_cRandomMobGeneratorLevel;
	uint16_t   m_iTotalActiveObject;
	uint16_t   m_iTotalAliveObject;
	uint16_t   m_iMaximumObject;

	char  m_cType;
	bool  m_bIsFixedDayMode;		
	bool  m_bIsFixedSnowMode;
	struct spotmobgen {		    
		bool bDefined;
		char cType;				// 1:RANDOMAREA   2:RANDOMWAYPOINT

		char cWaypoint[10];     
		rect rcRect;

		int  iTotalActiveMob;
		int  iMobType;
		int  iMaxMobs;
		int  iCurMobs;

	};
	std::vector<spotmobgen> m_stSpotMobGenerator;

	std::vector<point> m_WaypointList;
	std::vector<rect>  m_rcMobGenAvoidRect;
	std::vector<rect>  m_rcNoAttackRect;

	std::vector<point> m_FishPointList;
	uint16_t   m_iTotalFishPoint, m_iMaxFish, m_iCurFish;

	bool  m_bMineralGenerator;
	char  m_cMineralGeneratorLevel;
	std::vector<point> m_MineralPointList;
	uint16_t   m_iTotalMineralPoint, m_iMaxMineral, m_iCurMineral;

	Weather m_weather;			
	uint32_t m_dwWeatherLastTime, m_dwWeatherStartTime;  

	uint16_t   m_iLevelLimit;
	uint16_t   m_iUpperLevelLimit;


	struct OccupyFlag
	{
		OccupyFlag(int dX, int dY, char cSide, int iEKNum, int iDOI)
		{
			m_sX = dX;
			m_sY = dY;

			m_side = cSide;
			m_iEKCount = iEKNum;

			m_iDynamicObjectIndex = iDOI;
		}

		char m_side;
		int  m_iEKCount;
		int  m_sX, m_sY;

		int  m_iDynamicObjectIndex;
	};

	struct StrategicPoint
	{
		StrategicPoint()
		{
			m_iSide = m_iValue = m_iX = m_iY = 0;
		}
		int		m_iSide;				
		int     m_iValue;
		int		m_iX, m_iY;
	};

	std::vector<OccupyFlag *> m_pOccupyFlag;
	int   m_iTotalOccupyFlags;

	std::vector<StrategicPoint *> m_pStrategicPointList;

	bool	m_bIsAttackEnabled;
	bool	m_isPartyDisabled;
	bool	m_isShieldDisabled;
	bool	m_isArmorDisabled;
	bool	m_isChatDisabled;
	bool	m_magicLimited[MAXMAGICTYPE];
	bool	m_isPermIllusionOn;

	bool  m_bIsFightZone;

	struct energysphere {
		char cType;
		int sX, sY;
	};
	std::vector<energysphere> m_stEnergySphereCreationList;
	int m_iTotalEnergySphereCreationPoint;


	int m_stEnergySphereGoalList_aresdenX, m_stEnergySphereGoalList_aresdenY, m_stEnergySphereGoalList_elvineX, m_stEnergySphereGoalList_elvineY;
	int m_iTotalEnergySphereGoalPoint;

	bool m_bIsEnergySphereGoalEnabled;
	int m_iCurEnergySphereGoalPointIndex; 

	struct sectorinfo {
		int iPlayerActivity;
		int iNeutralActivity;
		int iAresdenActivity;
		int iElvineActivity;
		int iMonsterActivity;

	};
	//use vectors or maps?
	std::vector<sectorinfo> m_stSectorInfo;
	std::vector<sectorinfo> m_stTempSectorInfo;

	int m_iMaxNx, m_iMaxNy, m_iMaxAx, m_iMaxAy, m_iMaxEx, m_iMaxEy, m_iMaxMx, m_iMaxMy, m_iMaxPx, m_iMaxPy;

	struct strikepoint {
		string cRelatedMapName;
		int iMapIndex;
		int dX, dY;
		int iHP, iInitHP;

		int iEffectX[5];
		int iEffectY[5];

	};
	std::vector<strikepoint> m_stStrikePoint;

	bool m_bIsDisabled;

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
	std::vector<itemeventlist> m_stItemEventList;

	short sMobEventAmount ;

	int m_chatZone;

	Map::OccupyFlag * iRegisterOccupyFlag(int dX, int dY, int iSide, int iEKNum, int iDOI);

private:
	bool _bDecodeMapDataFileContents();
};

