// Map.h: interface for the CMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAP_H__12609160_8060_11D2_A8E6_00001C7030A6__INCLUDED_)
#define AFX_MAP_H__12609160_8060_11D2_A8E6_00001C7030A6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <windows.h>

#include "Game.h"
#include "StrTok.h"
#include "TeleportLoc.h"
#include "GlobalDef.h"
#include "OccupyFlag.h"
#include "Tile.h"
#include "StrategicPoint.h"

class CMap  
{
public:

		//Snoopy:
	bool bGetIsStayAllowedTile(short dX, short dY);
	void SetStayAllowedFlag(int dX, int dY, bool bFlag);
	
	bool bApocalypseGate();
	void ClearBigOwner(short sOwnerH, char cOwnerType, short pX, short pY, char cArea);
	bool bGetMoveableArea(short sOwnerH, short dX, short dY);
	bool bApocalypseGateTeleporter(short dX, short dY, char * cMapName, short * tX, short * tY);
	bool bCheckFlySpaceAvailable(short sX, short sY, char cDir, short sOwner);
	bool bGetIsFarm(short tX, short tY);
	void RestoreStrikePoints();
	bool bRemoveCrusadeStructureInfo(short sX, short sY);
	bool bAddCrusadeStructureInfo(char cType, short sX, short sY, char cSide);
	int iGetAttribute(int dX, int dY, int iBitMask);
	void _SetupNoAttackArea();
	
	int iRegisterOccupyFlag(int dX, int dY, int iSide, int iEKNum, int iDOI);
	int  iCheckItem(short sX, short sY);
	void SetTempMoveAllowedFlag(int dX, int dY, bool bFlag);
	int iAnalyze(char cType, int *pX, int *pY, int * pV1, int *pV2, int * pV3);
	bool bGetIsWater(short dX, short dY);
	void GetDeadOwner(short * pOwner, char * pOwnerClass, short sX, short sY);
	bool bGetIsMoveAllowedTile(short dX, short dY);
	void SetNamingValueEmpty(int iValue);
	int iGetEmptyNamingValue();
	bool bGetDynamicObject(short sX, short sY, short * pType, DWORD * pRegisterTime, int * pIndex = 0);
	void SetDynamicObject(WORD wID, short sType, short sX, short sY, DWORD dwRegisterTime);
	bool bGetIsTeleport(short dX, short dY);
	bool bSearchTeleportDest(int sX, int sY, char * pMapName, int * pDx, int * pDy, char * pDir);
	bool bInit(char * pName);
	bool bIsValidLoc(short sX, short sY);
	//class CItem * pGetItem(short sX, short sY, short * pRemainItemSprite, short * pRemainItemSpriteFrame, char * pRemainItemColor);
	class CItem* pGetItem(short sX, short sY, short* pRemainItemID/*, short * pRemainItemSprite, short * pRemainItemSpriteFrame*/, char* pRemainItemColor, DWORD* pRemainItemAttr);
	bool bSetItem(short sX, short sY, class CItem * pItem);
	void ClearDeadOwner(short sX, short sY);
	void ClearOwner(int iDebugCode, short sOwnerH, char cOwnerType, short sX, short sY);
	bool bGetMoveable(short dX, short dY, short * pDOtype = 0/*, short * pTopItem = 0*/);
	void GetOwner(short * pOwner, char * pOwnerClass, short sX, short sY);
	void SetOwner(short sOwner, char cOwnerClass, short sX, short sY);
	void SetDeadOwner(short sOwner, char cOwnerClass, short sX, short sY);
	bool bRemoveCropsTotalSum();
	bool bAddCropsTotalSum();
	void SetBigOwner(short sOwner, char cOwnerClass, short pX, short pY, char cArea);

	void ClearSectorInfo();
	void ClearTempSectorInfo();

	CMap(class CGame * pGame);
	virtual ~CMap();

	class CTile * m_pTile;
	class CGame * m_pGame;
	char  m_cName[11];
	char  m_cLocationName[11];
	short m_sSizeX, m_sSizeY, m_sTileDataSize;
	class CTeleportLoc * m_pTeleportLoc[DEF_MAXTELEPORTLOC];
	
	POINT m_pInitialPoint[DEF_MAXINITIALPOINT];

	bool  m_bNamingValueUsingStatus[DEF_MAXNPCS]; //LifeX Fix Mobs Respawn Limit 01/01
	bool  m_bRandomMobGenerator;
	char  m_cRandomMobGeneratorLevel;
	int   m_iTotalActiveObject;
	int   m_iTotalAliveObject;
	int   m_iMaximumObject;
	bool  m_bIsGateAvailable;
	char  m_cType;				// 맵의 형식. 0이면 보통. 1이면 공격행위가 범죄가 아니다.


	int	  m_iApocalypseMobGenType, m_iApocalypseBossMobNpcID;
	
	
	//Snoopy: tranformed to RECT
	RECT	m_sApocalypseBossMobRect;

	char  m_cDynamicGateType;
	short m_sDynamicGateCoordRectX1, m_sDynamicGateCoordRectY1, m_sDynamicGateCoordRectX2, m_sDynamicGateCoordRectY2;
	char  m_cDynamicGateCoordDestMap[11];
	short m_sDynamicGateCoordTgtX, m_sDynamicGateCoordTgtY;

	bool  m_bIsFixedDayMode;	// 항상 주간모드인지: 건물 내부 등 

	struct {		    
		bool bDefined;
		char cType;				// 1:RANDOMAREA   2:RANDOMWAYPOINT
		
		char cWaypoint[10];     // RANDOMWAYPOINT 등
		RECT rcRect;			// RANDOMAREA발생의 경우 
		
		int  iTotalActiveMob;
		int  iMobType;
		int  iMaxMobs;
		int  iCurMobs;
		
	} m_stSpotMobGenerator[DEF_MAXSPOTMOBGENERATOR];

	int m_iMaximumObjectDefault;
	int		m_iTotalDynamicGate;
	short	m_sDynamicGateTgtX;
	short	m_sDynamicGateTgtY;
	RECT	m_rcDynamicGateRect[DEF_MAXDYNAMICGATE];
	char	m_cDynamicGateDestMap[11];

	bool m_bIsBossInMap;
	bool m_bIsGateMap;
	POINT m_WaypointList[DEF_MAXWAYPOINTCFG];
	RECT  m_rcMobGenAvoidRect[DEF_MAXMGAR];
	RECT  m_rcNoAttackRect[DEF_MAXNMR];
	RECT  m_rcApocalypseBossMobRect;

	POINT m_FishPointList[DEF_MAXFISHPOINT];
	int   m_iTotalFishPoint, m_iMaxFish, m_iCurFish;
	
	
	bool  m_bIsCitizenLimit;
	short m_sHeldenianTowerType;
	char  m_cHeldenianTowerSide;
	char  m_cHeldenianModeMap;

	short m_sHeldenianWinningZoneX, m_sHeldenianWinningZoneY;

	bool  m_bMineralGenerator;
	char  m_cMineralGeneratorLevel;
	POINT m_MineralPointList[DEF_MAXMINERALPOINT];
	int   m_iTotalMineralPoint, m_iMaxMineral, m_iCurMineral;

	char  m_cWhetherStatus;		// 기상 상태. 0이면 없음. 1~3 비 4~6 눈 7~9 폭풍 
	DWORD m_dwWhetherLastTime, m_dwWhetherStartTime;  // 날씨 지속, 시작 시간 

	int   m_iLevelLimit;
	int   m_iUpperLevelLimit;	// 상위 렙 제한 

	class COccupyFlag * m_pOccupyFlag[DEF_MAXOCCUPYFLAG];
	int   m_iTotalOccupyFlags;
	
	class CStrategicPoint * m_pStrategicPointList[DEF_MAXSTRATEGICPOINTS];
	bool  m_bIsAttackEnabled;

	//Magn0S:: Maps Restrictions
	bool bMapTP = true, bMapParty = true, bMapIllusion = false, bMapActivate = true, bMapInvi = true, bMapAMP = true;
	bool bMapRegens = true, bMapHideEnemy = false, bMapBonusDmg = false, bMapEquip = true;

	bool  m_bIsFightZone;

	struct {
		char cType;
		int sX, sY;

	} m_stEnergySphereCreationList[DEF_MAXENERGYSPHERES];

	int m_iTotalEnergySphereCreationPoint;
	
	struct {
		char cResult;
		int aresdenX, aresdenY, elvineX, elvineY;
	} m_stEnergySphereGoalList[DEF_MAXENERGYSPHERES];

	int m_iTotalEnergySphereGoalPoint;

	bool m_bIsEnergySphereGoalEnabled;
	int m_iCurEnergySphereGoalPointIndex; 

	struct {
		int iPlayerActivity;
		int iNeutralActivity;
		int iAresdenActivity;
		int iElvineActivity;
		int iMonsterActivity;

	} m_stSectorInfo[DEF_MAXSECTORS][DEF_MAXSECTORS], m_stTempSectorInfo[DEF_MAXSECTORS][DEF_MAXSECTORS];
	
	short sMobEventAmount;
	int m_iTotalItemEvents;
	struct {
		char cItemName[21];
		int iAmount;
		int iTotal;
		int iMonth;
		int iDay;
		int iTotalNum;
	} m_stItemEventList[DEF_MAXITEMEVENTS];

	struct {
		char  cDir;
		short dX;
		short dY;
	} m_stHeldenianGateDoor[DEF_MAXHELDENIANDOOR];

	struct {
		short sTypeID;
		short dX;
		short dY;
		char  cSide;
	} m_stHeldenianTower[DEF_MAXHELDENIANTOWER];

	int m_iMaxNx, m_iMaxNy, m_iMaxAx, m_iMaxAy, m_iMaxEx, m_iMaxEy, m_iMaxMx, m_iMaxMy, m_iMaxPx, m_iMaxPy;

	struct {
		char cRelatedMapName[11];
		int iMapIndex;
		int dX, dY;
		int iHP, iInitHP;

		int iEffectX[5];
		int iEffectY[5];
	
	} m_stStrikePoint[DEF_MAXSTRIKEPOINTS];
	int m_iTotalStrikePoints;

	bool m_bIsDisabled;		// 폭격으로 기능이 마비된 경우 
	int m_iTotalAgriculture;

	struct {
		char cType;			// 이게 NULL이면 정의되지 않은것을 의미.
		char cSide;			// 사이드
		short sX, sY;		// 설치된 위치 
	} m_stCrusadeStructureInfo[DEF_MAXCRUSADESTRUCTURES];
	int m_iTotalCrusadeStructures;
	bool m_bIsEnergySphereAutoCreation;
	bool m_bIsApocalypseMobSpawn;

private:
	bool _bDecodeMapDataFileContents();
public:
	// Snow BOOLean for certain maps to snow instead of rain
	bool m_bIsSnowEnabled;
	bool m_bIsRecallImpossible;
	bool m_bIsApocalypseMap;
	bool m_bIsHeldenianMap;

	char m_cThunder; //0: normal, 1:Lightning, 2: Apoc FireThunder
};

#endif // !defined(AFX_MAP_H__12609160_8060_11D2_A8E6_00001C7030A6__INCLUDED_)
