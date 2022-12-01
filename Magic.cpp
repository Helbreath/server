// Magic.cpp: implementation of the CMagic class.
//
//////////////////////////////////////////////////////////////////////

#include "Magic.h"

extern void PutLogList(char* cMsg);
extern char G_cTxt[512];
extern char	G_cData50000[50000];
extern void PutLogFileList(char* cStr);
extern void PutAdminLogFileList(char* cStr);
extern void PutItemLogFileList(char* cStr);
extern void PutLogEventFileList(char* cStr);
extern void PutHackLogFileList(char* cStr);
extern void PutPvPLogFileList(char* cStr);
extern FILE* pLogFile;
extern HWND	G_hWnd;

#pragma warning (disable : 4996)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMagic::CMagic()
{
	ZeroMemory(m_cName, sizeof(m_cName));
	m_iAttribute = 0;
}

CMagic::~CMagic()
{										    

}

/*********************************************************************************************************************
**  int CGame::iGetWhetherMagicBonusEffect(short sType, char cWheatherStatus)										**
**  description			:: checks for a weather bonus when magic is cast											**
**  last updated		:: November 20, 2004; 10:34 PM; Hypnotoad													**
**	return value		:: int																						**
*********************************************************************************************************************/
int CGame::iGetWhetherMagicBonusEffect(short sType, char cWheatherStatus)
{
	int iWheatherBonus;

	iWheatherBonus = 0;
	switch (cWheatherStatus) {
	case 0: break;
	case 1:
	case 2:
	case 3:
		switch (sType) {
		case 10:
		case 37:
		case 43:
		case 51:
			iWheatherBonus = 1;
			break;

		case 20:
		case 30:
			iWheatherBonus = -1;
			break;
		}
		break;
	}
	return iWheatherBonus;
}

/*********************************************************************************************************************
**  void CGame::SetInvisibilityFlag(short sOwnerH, char cOwnerType, bool bStatus)									**
**  description			:: changes the status of the player to show invisibility aura								**
**  last updated		:: November 20, 2004; 9:30 PM; Hypnotoad													**
**	return value		:: void																						**
*********************************************************************************************************************/
void CGame::SetInvisibilityFlag(short sOwnerH, char cOwnerType, bool bStatus)
{
	switch (cOwnerType) {
	case DEF_OWNERTYPE_PLAYER:
		if (m_pClientList[sOwnerH] == 0) return;
		if (bStatus == true)
			m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00000010;
		else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFFFFFFEF;
		SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
		break;

	case DEF_OWNERTYPE_NPC:
		if (m_pNpcList[sOwnerH] == 0) return;
		if (bStatus == true)
			m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x00000010;
		else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFFFFFFEF;
		SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
		break;
	}
}

/*********************************************************************************************************************
**  void CGame::SetInhibitionCastingFlag(short sOwnerH, char cOwnerType, bool bStatus)								**
**  description			:: changes the status of the player to show inhibit casting aura							**
**  last updated		:: November 20, 2004; 9:33 PM; Hypnotoad													**
**	return value		:: void																						**
*********************************************************************************************************************/
void CGame::SetInhibitionCastingFlag(short sOwnerH, char cOwnerType, bool bStatus)
{
	switch (cOwnerType) {
	case DEF_OWNERTYPE_PLAYER:
		if (m_pClientList[sOwnerH] == 0) return;
		if (bStatus == true)
			m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00100000;
		else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFFEFFFFF;
		SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
		break;

	case DEF_OWNERTYPE_NPC:
		if (m_pNpcList[sOwnerH] == 0) return;
		if (bStatus == true)
			m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x00100000;
		else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFFEFFFFF;
		SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
		break;
	}
}

/*********************************************************************************************************************
**  void void CGame::SetBerserkFlag(short sOwnerH, char cOwnerType, bool bStatus)									**
**  description			:: changes the status of the player to show berserk aura									**
**  last updated		:: November 20, 2004; 9:35 PM; Hypnotoad													**
**	return value		:: void																						**
*********************************************************************************************************************/
void CGame::SetBerserkFlag(short sOwnerH, char cOwnerType, bool bStatus)
{
	switch (cOwnerType) {
	case DEF_OWNERTYPE_PLAYER:
		if (m_pClientList[sOwnerH] == 0) return;
		if (bStatus == true)
			m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00000020;
		else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFFFFFFDF;
		SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
		break;

	case DEF_OWNERTYPE_NPC:
		if (m_pNpcList[sOwnerH] == 0) return;
		if (bStatus == true)
			m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x00000020;
		else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFFFFFFDF;
		SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
		break;
	}
}

/*********************************************************************************************************************
**  void void CGame::SetIceFlag(short sOwnerH, char cOwnerType, bool bStatus)										**
**  description			:: changes the status of the player to show frozen aura										**
**  last updated		:: November 20, 2004; 9:35 PM; Hypnotoad													**
**	return value		:: void																						**
*********************************************************************************************************************/
void CGame::SetIceFlag(short sOwnerH, char cOwnerType, bool bStatus)
{
	switch (cOwnerType) {
	case DEF_OWNERTYPE_PLAYER:
		if (m_pClientList[sOwnerH] == 0) return;
		if (bStatus == true)
			m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00000040;
		else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFFFFFFBF;
		SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
		break;

	case DEF_OWNERTYPE_NPC:
		if (m_pNpcList[sOwnerH] == 0) return;
		if (bStatus == true)
			m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x00000040;
		else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFFFFFFBF;
		SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
		break;
	}
}

/*********************************************************************************************************************
**  void void CGame::SetPoisonFlag(short sOwnerH, char cOwnerType, bool bStatus)									**
**  description			:: changes the status of the player to show poison aura										**
**  last updated		:: November 20, 2004; 9:36 PM; Hypnotoad													**
**	return value		:: void																						**
*********************************************************************************************************************/
void CGame::SetPoisonFlag(short sOwnerH, char cOwnerType, bool bStatus)
{
	switch (cOwnerType) {
	case DEF_OWNERTYPE_PLAYER:
		if (m_pClientList[sOwnerH] == 0) return;
		if (bStatus == true)
			m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00000080;
		else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFFFFFF7F;
		SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
		break;

	case DEF_OWNERTYPE_NPC:
		if (m_pNpcList[sOwnerH] == 0) return;
		if (bStatus == true)
			m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x00000080;
		else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFFFFFF7F;
		SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
		break;
	}
}

/*********************************************************************************************************************
**  void void CGame::SetIllusionFlag(short sOwnerH, char cOwnerType, bool bStatus)									**
**  description			:: changes the status of the player to show illusion aura									**
**  last updated		:: November 20, 2004; 9:36 PM; Hypnotoad													**
**	return value		:: void																						**
*********************************************************************************************************************/
void CGame::SetIllusionFlag(short sOwnerH, char cOwnerType, bool bStatus)
{
	switch (cOwnerType) {
	case DEF_OWNERTYPE_PLAYER:
		if (m_pClientList[sOwnerH] == 0) return;
		if (bStatus == true)
			m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x01000000;
		else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFEFFFFFF;
		SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
		break;

	case DEF_OWNERTYPE_NPC:
		if (m_pNpcList[sOwnerH] == 0) return;
		if (bStatus == true)
			m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x01000000;
		else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFEFFFFFF;
		SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
		break;
	}
}

/*********************************************************************************************************************
**  void void CGame::SetHeroFlag(short sOwnerH, char cOwnerType, bool bStatus)										**
**  description			:: changes the status of the player to show hero item aura									**
**  last updated		:: November 20, 2004; 9:37 PM; Hypnotoad													**
**	return value		:: void																						**
*********************************************************************************************************************/
void CGame::SetHeroFlag(short sOwnerH, char cOwnerType, bool bStatus)
{
	switch (cOwnerType) {
	case DEF_OWNERTYPE_PLAYER:
		if (m_pClientList[sOwnerH] == 0) return;
		if (bStatus == true)
			m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00020000;
		else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFFFDFFFF;
		SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
		break;

	case DEF_OWNERTYPE_NPC:
		if (m_pNpcList[sOwnerH] == 0) return;
		if (bStatus == true)
			m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x00020000;
		else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFFFDFFFF;
		SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
		break;
	}
}

/*********************************************************************************************************************
**  void void CGame::SetDefenseShieldFlag(short sOwnerH, char cOwnerType, bool bStatus)								**
**  description			:: changes the status of the player to show defense aura									**
**  last updated		:: November 20, 2004; 9:37 PM; Hypnotoad													**
**	return value		:: void																						**
*********************************************************************************************************************/
void CGame::SetDefenseShieldFlag(short sOwnerH, char cOwnerType, bool bStatus)
{
	switch (cOwnerType) {
	case DEF_OWNERTYPE_PLAYER:
		if (m_pClientList[sOwnerH] == 0) return;
		if (bStatus == true)
			m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x02000000;
		else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFDFFFFFF;
		SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
		break;

	case DEF_OWNERTYPE_NPC:
		if (m_pNpcList[sOwnerH] == 0) return;
		if (bStatus == true)
			m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x02000000;
		else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFDFFFFFF;
		SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
		break;
	}
}

/*********************************************************************************************************************
**  void void CGame::SetMagicProtectionFlag(short sOwnerH, char cOwnerType, bool bStatus)							**
**  description			:: changes the status of the player to show magic protect aura								**
**  last updated		:: November 20, 2004; 9:38 PM; Hypnotoad													**
**	return value		:: void																						**
*********************************************************************************************************************/
void CGame::SetMagicProtectionFlag(short sOwnerH, char cOwnerType, bool bStatus)
{
	switch (cOwnerType) {
	case DEF_OWNERTYPE_PLAYER:
		if (m_pClientList[sOwnerH] == 0) return;
		if (bStatus == true)
			m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x04000000;
		else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFBFFFFFF;
		SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
		break;

	case DEF_OWNERTYPE_NPC:
		if (m_pNpcList[sOwnerH] == 0) return;
		if (bStatus == true)
			m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x04000000;
		else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFBFFFFFF;
		SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
		break;
	}
}

/*********************************************************************************************************************
**  void void CGame::SetProtectionFromArrowFlag(short sOwnerH, char cOwnerType, bool bStatus)						**
**  description			:: changes the status of the player to show arrow protect aura								**
**  last updated		:: November 20, 2004; 9:39 PM; Hypnotoad													**
**	return value		:: void																						**
*********************************************************************************************************************/
void CGame::SetProtectionFromArrowFlag(short sOwnerH, char cOwnerType, bool bStatus)
{
	switch (cOwnerType) {
	case DEF_OWNERTYPE_PLAYER:
		if (m_pClientList[sOwnerH] == 0) return;
		if (bStatus == true)
			m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x08000000;
		else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xF7FFFFFF;
		SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
		break;

	case DEF_OWNERTYPE_NPC:
		if (m_pNpcList[sOwnerH] == 0) return;
		if (bStatus == true)
			m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x08000000;
		else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xF7FFFFFF;
		SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
		break;
	}
}

/*********************************************************************************************************************
**  void void CGame::SetIllusionMovementFlag(short sOwnerH, char cOwnerType, bool bStatus)							**
**  description			:: changes the status of the player to show illusion movement aura							**
**  last updated		:: November 20, 2004; 9:39 PM; Hypnotoad													**
**	return value		:: void																						**
*********************************************************************************************************************/
void CGame::SetIllusionMovementFlag(short sOwnerH, char cOwnerType, bool bStatus)
{
	switch (cOwnerType) {
	case DEF_OWNERTYPE_PLAYER:
		if (m_pClientList[sOwnerH] == 0) return;
		if (bStatus == true)
			m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00200000;
		else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFFDFFFFF;
		SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
		break;
	}
}

/*********************************************************************************************************************
**  void CGame::PlayerMagicHandler(int iClientH, int dX, int dY, short sType, bool bItemEffect, int iV1)			**
**  description			:: handles all magic related items/spells													**
**  last updated		:: November 22, 2004; 5:45 PM; Hypnotoad													**
**	return value		:: void																						**
**  commentary			::	-	added 3.51 casting detection														**
**							-	updated it so civilians can only cast certain spells on players and vice versa		**
**							-	fixed bug causing spell to be cast when mana is below required amount				**
**********************************************************************************************************************/
int  _tmp_iMCProb[] = { 0, 300, 250, 200, 150, 100, 80, 70, 60, 50, 40 };
int  _tmp_iMLevelPenalty[] = { 0,   5,   5,   8,   8,  10, 14, 28, 32, 36, 40 };
void CGame::PlayerMagicHandler(int iClientH, int dX, int dY, short sType, bool bItemEffect, int iV1, bool bIgnoreOwnerLimits)
{
	short* sp, sX, sY, sOwnerH, sMagicCircle, rx, ry, sLevelMagic, sTemp;
	char* cp, cData[120], cDir, cOwnerType, cName[11], cItemName[21], cNpcWaypoint[11], cName_Master[11], cNpcName[21], cRemainItemColor;
	double dV1, dV2, dV3, dV4;
	int    i, iErr, iRet, ix, iy, iResult, iDiceRes, iNamingValue, iFollowersNum, iEraseReq, iWhetherBonus;
	int    tX, tY, iManaCost, iMagicAttr;
	class  CItem* pItem;
	DWORD* dwp, dwTime, dwRemainItemAttr;
	WORD* wp, wWeaponType;
	short sEqStatus = -1, sRemainItemID;

	dwTime = timeGetTime();

	if (m_pClientList[iClientH] == 0) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

	if ((dX < 0) || (dX >= m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_sSizeX) ||
		(dY < 0) || (dY >= m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_sSizeY)) return;

	if (((dwTime - m_pClientList[iClientH]->m_dwRecentAttackTime) < 1000) && (bItemEffect == 0)) {
		wsprintf(G_cTxt, "(!) 3.51 Detection: (%s) Player: (%s) - Magic casting speed is too fast! Hack?", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
		PutHackLogFileList(G_cTxt);
		//DeleteClient(iClientH, true, true);
		return;
	}
	m_pClientList[iClientH]->m_dwRecentAttackTime = dwTime;
	m_pClientList[iClientH]->m_dwLastActionTime = dwTime;//m_pClientList[iClientH]->m_dwAFKCheckTime = dwTime;
	m_pClientList[iClientH]->m_bMagicConfirm = true; 
	if (m_pClientList[iClientH]->m_cMapIndex < 0) return;
	if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex] == 0) return;

	if ((sType < 0) || (sType >= 100))     return;
	if (m_pMagicConfigList[sType] == 0) return;

	//Magn0S:: Set Map Restrictions
	if ((sType == 32) && (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bMapInvi == false) && (m_pClientList[iClientH]->m_iAdminUserLevel == 0)) {
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_IPACCOUNTINFO, 0, 0, 0, "Invisibility Effect is disabled in this map.");
		return;
	}
	if ((sType == 65) && (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bMapAMP == false) && (m_pClientList[iClientH]->m_iAdminUserLevel == 0)) {
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_IPACCOUNTINFO, 0, 0, 0, "AMP Effect is disabled in this map.");
		return;
	}

	if ((bItemEffect == false) && (m_pClientList[iClientH]->m_cMagicMastery[sType] != 1)) return;

	if ((m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsAttackEnabled == false) && (m_pClientList[iClientH]->m_iAdminUserLevel == 0)) return;

	if (!bIgnoreOwnerLimits) {
		if (((m_pClientList[iClientH]->m_iStatus & 0x100000) != 0) && (bItemEffect != true)) {
			SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, 0, -1, 0);
			return;
		}

		if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND] != -1) {
			wWeaponType = ((m_pClientList[iClientH]->m_sAppr2 & 0x0FF0) >> 4);
			if ((wWeaponType < 34) || (wWeaponType > 39))
				return;
		}

		if ((m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LHAND] != -1) ||
			(m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND] != -1)) return;

		if ((m_pClientList[iClientH]->m_iSpellCount > 1) && (bItemEffect == false)) {
			wsprintf(G_cTxt, "TSearch Spell Hack: (%s) Player: (%s) - casting magic without precasting.", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
			PutHackLogFileList(G_cTxt);
			//DeleteClient(iClientH, true, true);
			return;
		}

		//Magn0S:: Add to decreased the endurace of wands.
		if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND] != -1)
			sEqStatus = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
		else sEqStatus = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];

		if (sEqStatus != -1) {

			if ((m_pClientList[iClientH]->m_pItemList[sEqStatus] != 0) &&
				(m_pClientList[iClientH]->m_pItemList[sEqStatus]->m_wMaxLifeSpan != 0)) {

				if (m_pClientList[iClientH]->m_cSide != 0) {
					if (m_pClientList[iClientH]->m_pItemList[sEqStatus]->m_wCurLifeSpan <= 0)
						m_pClientList[iClientH]->m_pItemList[sEqStatus]->m_wCurLifeSpan = 0;
					else m_pClientList[iClientH]->m_pItemList[sEqStatus]->m_wCurLifeSpan--;
					SendNotifyMsg(0, iClientH, DEF_NOTIFY_CURLIFESPAN, sEqStatus, m_pClientList[iClientH]->m_pItemList[sEqStatus]->m_wCurLifeSpan, 0, 0);
				}

				if (m_pClientList[iClientH]->m_pItemList[sEqStatus]->m_wCurLifeSpan == 0) {
					SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[iClientH]->m_pItemList[sEqStatus]->m_cEquipPos, sEqStatus, 0, 0);
					ReleaseItemHandler(iClientH, sEqStatus, true);
				}
			}
		}

		if (m_pClientList[iClientH]->m_bInhibition == true) {
			SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, 0, -1, 0);
			return;
		}

		if (m_pMagicConfigList[sType]->m_sType == 32) { // Invisiblity
			sEqStatus = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];
			if ((sEqStatus != -1) && (m_pClientList[iClientH]->m_pItemList[sEqStatus] != 0)) {
				if ((m_pClientList[iClientH]->m_pItemList[sEqStatus]->m_sIDnum == 865) || (m_pClientList[iClientH]->m_pItemList[sEqStatus]->m_sIDnum == 866)) {
					bItemEffect = true;
				}
			}
		}
	}

	sX = m_pClientList[iClientH]->m_sX;
	sY = m_pClientList[iClientH]->m_sY;

	sMagicCircle = (sType / 10) + 1;
	if (m_pClientList[iClientH]->m_cSkillMastery[4] == 0)
		dV1 = 1.0f;
	else dV1 = (double)m_pClientList[iClientH]->m_cSkillMastery[4];

	if (bItemEffect == true) dV1 = (double)100.0f;
	dV2 = (double)(dV1 / 100.0f);
	dV3 = (double)_tmp_iMCProb[sMagicCircle];
	dV1 = dV2 * dV3;
	iResult = (int)dV1;

	if ((m_pClientList[iClientH]->m_iInt) > 50)
		iResult += ((m_pClientList[iClientH]->m_iInt) - 50) / 2;

	sLevelMagic = ((m_pClientList[iClientH]->m_iLevel) / 10);
	if (sMagicCircle != sLevelMagic) {
		if (sMagicCircle > sLevelMagic) {
			dV1 = (double)((m_pClientList[iClientH]->m_iLevel) - sLevelMagic * 10);
			dV2 = (double)abs(sMagicCircle - sLevelMagic) * _tmp_iMLevelPenalty[sMagicCircle];
			dV3 = (double)abs(sMagicCircle - sLevelMagic) * 10;
			dV4 = (dV1 / dV3) * dV2;
			iResult -= abs(abs(sMagicCircle - sLevelMagic) * _tmp_iMLevelPenalty[sMagicCircle] - (int)dV4);
		}
		else {
			iResult += 5 * abs(sMagicCircle - sLevelMagic);
		}
	}

	switch (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cWhetherStatus) {
	case 0: break;
	case 1: iResult = iResult - (iResult / 24); break;
	case 2:	iResult = iResult - (iResult / 12); break;
	case 3: iResult = iResult - (iResult / 5);  break;
	}

	if (m_pClientList[iClientH]->m_iSpecialWeaponEffectType == 10) {
		dV1 = (double)iResult;
		dV2 = (double)(m_pClientList[iClientH]->m_iSpecialWeaponEffectValue * 3);
		dV3 = dV1 + dV2;
		iResult = (int)dV3;
	}

	if (iResult <= 0) iResult = 1;

	iWhetherBonus = iGetWhetherMagicBonusEffect(sType, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cWhetherStatus);

	iManaCost = m_pMagicConfigList[sType]->m_sValue1;
	if ((m_pClientList[iClientH]->m_bIsSafeAttackMode == true) &&
		(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsFightZone == false)) {
		//SNOOPY: Spell in safe mode only cost 10% more	
		iManaCost += (iManaCost / 10);
	}

	if (m_pClientList[iClientH]->m_iManaSaveRatio > 0) {
		dV1 = (double)m_pClientList[iClientH]->m_iManaSaveRatio;
		dV2 = (double)(dV1 / 100.0f);
		dV3 = (double)iManaCost;
		dV1 = dV2 * dV3;
		dV2 = dV3 - dV1;
		iManaCost = (int)dV2;

		if (iManaCost <= 0) iManaCost = 1;
	}

	wWeaponType = ((m_pClientList[iClientH]->m_sAppr2 & 0x0FF0) >> 4);
	if (wWeaponType == 34) {
		iManaCost += 20;
	}

	if (iResult < 100) {
		iDiceRes = iDice(1, 100);
		if (iResult < iDiceRes) {
			SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, 0, -1, 0);
			return;
		}
	}

	if (((m_pClientList[iClientH]->m_iHungerStatus <= 10) || (m_pClientList[iClientH]->m_iSP <= 0)) && (iDice(1, 1000) <= 100)) {
		SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, 0, -1, 0);
		return;
	}

	if (!bIgnoreOwnerLimits && m_pClientList[iClientH]->m_iMP < iManaCost) {
		return;
	}

	iResult = m_pClientList[iClientH]->m_cSkillMastery[4];
	if ((m_pClientList[iClientH]->m_iMag) > 50) iResult += ((m_pClientList[iClientH]->m_iMag) - 50);

	sLevelMagic = ((m_pClientList[iClientH]->m_iLevel) / 10);
	if (sMagicCircle != sLevelMagic) {
		if (sMagicCircle > sLevelMagic) {
			dV1 = (double)((m_pClientList[iClientH]->m_iLevel) - sLevelMagic * 10);
			dV2 = (double)abs(sMagicCircle - sLevelMagic) * _tmp_iMLevelPenalty[sMagicCircle];
			dV3 = (double)abs(sMagicCircle - sLevelMagic) * 10;
			dV4 = (dV1 / dV3) * dV2;

			iResult -= abs(abs(sMagicCircle - sLevelMagic) * _tmp_iMLevelPenalty[sMagicCircle] - (int)dV4);
		}
		else {
			iResult += 5 * abs(sMagicCircle - sLevelMagic); 
		}
	}

	iResult += m_pClientList[iClientH]->m_iMagicHitRatio_ItemEffect; // snoopy
	iResult += m_pClientList[iClientH]->m_iAddAR;
	if (iResult <= 0) iResult = 1;

	if (sType >= 80) iResult += 10000;

	if (m_pMagicConfigList[sType]->m_sType == 28) {
		iResult += 10000;
	}

	if (m_pMagicConfigList[sType]->m_cCategory == 1) {
		if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->iGetAttribute(sX, sY, 0x00000005) != 0) return;
	}

	iMagicAttr = m_pMagicConfigList[sType]->m_iAttribute;
	if ((m_pClientList[iClientH]->m_iStatus & 0x10) != 0 && !bIgnoreOwnerLimits) {
		SetInvisibilityFlag(iClientH, DEF_OWNERTYPE_PLAYER, false);
		bRemoveFromDelayEventList(iClientH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INVISIBILITY);
		m_pClientList[iClientH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = 0;
	}

	m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
	if ((m_bIsCrusadeMode == false) && (cOwnerType == DEF_OWNERTYPE_PLAYER)) {
		if ((m_pClientList[iClientH]->m_bIsPlayerCivil != true) && (m_pClientList[sOwnerH]->m_bIsPlayerCivil == true)) {
			if (m_pClientList[iClientH]->m_cSide != m_pClientList[sOwnerH]->m_cSide) return;
		}
		else if ((m_pClientList[iClientH]->m_bIsPlayerCivil == true) && (m_pClientList[sOwnerH]->m_bIsPlayerCivil == false)) {
			switch (m_pMagicConfigList[sType]->m_sType) {
			case 1:  // DEF_MAGICTYPE_DAMAGE_SPOT
			case 4:  // DEF_MAGICTYPE_SPDOWN_SPOT 4
			case 8:  // DEF_MAGICTYPE_TELEPORT 8
			case 10: // DEF_MAGICTYPE_CREATE 10
			case 11: // DEF_MAGICTYPE_PROTECT 11
			case 12: // DEF_MAGICTYPE_HOLDOBJECT 12
			case 16: // DEF_MAGICTYPE_CONFUSE
			case 17: // DEF_MAGICTYPE_POISON
			case 24: // DEF_MAGICTYPE_RESURRECTION
				goto MAGIC_NOEFFECT;

			}
		}
	}

	if (m_pMagicConfigList[sType]->m_dwDelayTime == 0) {
		switch (m_pMagicConfigList[sType]->m_sType) {

		case DEF_MAGICTYPE_DAMAGE_SPOT:
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
				Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);

			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
			if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0)) {
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
					Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
			}
			break;

		case DEF_MAGICTYPE_HPUP_SPOT:
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			Effect_HpUp_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6);
			break;

		case DEF_MAGICTYPE_DAMAGE_AREA:
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
				Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);

			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
			if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0)) {
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
					Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
			}

			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					}
				}
			break;

		case DEF_MAGICTYPE_SPDOWN_SPOT:
			break;

		case DEF_MAGICTYPE_SPDOWN_AREA:
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
				Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6);
			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
				}
			break;

		case DEF_MAGICTYPE_POLYMORPH: // [20] Polymorph
			/*
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			switch (cOwnerType) {
			case DEF_OWNERTYPE_PLAYER:
				if (m_pClientList[iClientH] == 0) goto MAGIC_NOEFFECT;
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[iClientH] != m_pClientList[sOwnerH])) break;
				if (m_pClientList[iClientH]->m_cMagicEffectStatus[DEF_MAGICTYPE_POLYMORPH] != 0) goto MAGIC_NOEFFECT;
				m_pClientList[iClientH]->m_sOriginalType = m_pClientList[sOwnerH]->m_sType;
				switch (iDice(1, 8)) {
				case 1: m_pClientList[iClientH]->m_sType = 29; break;
				case 2: m_pClientList[iClientH]->m_sType = 14; break;
				case 3: m_pClientList[iClientH]->m_sType = 11; break;
				case 4: m_pClientList[iClientH]->m_sType = 18; break;
				case 5: m_pClientList[iClientH]->m_sType = 12; break;
				case 6: m_pClientList[iClientH]->m_sType = 23; break;
				case 7: m_pClientList[iClientH]->m_sType = 28; break;
				case 8: m_pClientList[iClientH]->m_sType = 13; break;
				}
				m_pClientList[sOwnerH]->m_bIsPolymorph = true;
				ToggleCombatModeHandler(iClientH);
				break;
			}
			SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_POLYMORPH, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
				sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
			if (cOwnerType == DEF_OWNERTYPE_PLAYER)	SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_POLYMORPH, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
			*/
			goto MAGIC_NOEFFECT;
			break;

		case DEF_MAGICTYPE_HELLFIRE:
			iMagicAttr = 3;
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			ShowSpellEffect2(sOwnerH, 99, cOwnerType);
			
			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, 23, 24, 0 + iWhetherBonus, false, iMagicAttr);

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, 23, 24, 0 + iWhetherBonus, false, iMagicAttr);
					}
				}
			break; //end hellfire

		case DEF_MAGICTYPE_FURYOFTHOR:
			sX = m_pClientList[iClientH]->m_sX;
			sY = m_pClientList[iClientH]->m_sY;

			for (i = 2; i < 10; i++) {
				iErr = 0;
				m_Misc.GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);

				// tx, ty
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
						}
						break;
					}
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;
						}
						//
					}
				}

				// tx-1, ty
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);

						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {

							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);

						}
						break;
					}
					//
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;
						}
						//
					}
				}

				// tx+1, ty
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
						}
						break;
					}
					//
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;
						}
						//
					}
				}

				// tx, ty-1
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
						}
						break;
					}
					//
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;
						}
						//
					}
				}

				// tx, ty+1
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
						}
						break;
					}
					//
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;
						}
						//
					}
				}

				if ((abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
			}

			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;
						}
						//
					}

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
						(m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
							switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
								if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
								}
								break;

							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
								if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);

								}
								break;
							}
							//
						}
					}
				}

			// dX, dY
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
				Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false
				switch (cOwnerType) {
				case DEF_OWNERTYPE_PLAYER:
					if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
						//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
					}
					break;

				case DEF_OWNERTYPE_NPC:
					if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
						//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
					}
					break;
				}
				//
			}

			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
			if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
				(m_pClientList[sOwnerH]->m_iHP > 0)) {
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);

						}
						break;
					}
					//
				}
			}
			break;

		

			// 05/20/2004 - Hypnotoad - Cancellation
		case DEF_MAGICTYPE_CANCELLATION:
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0) && (m_pClientList[sOwnerH]->m_iAdminUserLevel == 0)) {

				// Removes Invisibility Flag 0x0010
				SetInvisibilityFlag(sOwnerH, cOwnerType, false);

				// Removes Illusion Flag 0x01000000
				SetIllusionFlag(sOwnerH, cOwnerType, false);

				// Removes Defense Shield Flag 0x02000000
				// Removes Great Defense Shield Flag 0x02000000
				SetDefenseShieldFlag(sOwnerH, cOwnerType, false);

				// Removes Absolute Magic Protection Flag 0x04000000	
				// Removes Protection From Magic Flag 0x04000000
				SetMagicProtectionFlag(sOwnerH, cOwnerType, false);

				// Removes Protection From Arrow Flag 0x08000000
				SetProtectionFromArrowFlag(sOwnerH, cOwnerType, false);

				// Removes Illusion Movement Flag 0x00200000
				SetIllusionMovementFlag(sOwnerH, cOwnerType, false);

				// Removes Berserk Flag 0x0020
				SetBerserkFlag(sOwnerH, cOwnerType, false);

				if (m_pClientList[iClientH]->m_bInhibition == true)
					bRemoveFromDelayEventList(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INHIBITION);
				bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_INHIBITION, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime),
					sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

				if (m_pClientList[iClientH]->m_iStatus & 0x0010)
					bRemoveFromDelayEventList(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INVISIBILITY);
				bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_INVISIBILITY, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime),
					sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

				if (m_pClientList[iClientH]->m_iStatus & 0x0020)
					bRemoveFromDelayEventList(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_BERSERK);
				bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_BERSERK, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime),
					sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

				if ((m_pClientList[iClientH]->m_iStatus & 0x08000000) || (m_pClientList[iClientH]->m_iStatus & 0x04000000) || (m_pClientList[iClientH]->m_iStatus & 0x02000000))
					bRemoveFromDelayEventList(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_PROTECT);
				bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_PROTECT, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime),
					sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

				if ((m_pClientList[iClientH]->m_iStatus & 0x01000000) || (m_pClientList[iClientH]->m_iStatus & 0x00200000))
					bRemoveFromDelayEventList(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_CONFUSE);
				bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_CONFUSE, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime),
					sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

				// Update Client
				SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			}
			break;

		case DEF_MAGICTYPE_DAMAGE_AREA_NOSPOT_SPDOWN:
			// ������ ó������ �ʴ´�.
			// �ֺ� ���� ȿ�� 
			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
					// �ڽŵ� ������ �� ������ ����.
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);
						Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
					}

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
						(m_pClientList[sOwnerH]->m_iHP > 0)) {
						// ���� ô�ϰ� �ִ� �÷��̾��.
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);
							Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
						}
					}
				}
			break;

		case DEF_MAGICTYPE_DAMAGE_LINEAR:
			// Ã€ÃÃÃ·Â¼Â± Â»Ã³Â¿Â¡ Ã€Ã–Â´Ã‚ Â¸Ã±Ã‡Â¥Â¸Â¦ Â¸Ã°ÂµÃŽ Â°Ã¸Â°ÃÃ‡Ã‘Â´Ã™.
			sX = m_pClientList[iClientH]->m_sX;
			sY = m_pClientList[iClientH]->m_sY;

			for (i = 2; i < 10; i++) {
				iErr = 0;
				m_Misc.GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);

				// tx, ty
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					// ÃÃ—Ã€Âº ÃƒÂ´Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â´Ã™.
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
				}

				// tx-1, ty
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					// ÃÃ—Ã€Âº ÃƒÂ´Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â´Ã™.
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
				}

				// tx+1, ty
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					// ÃÃ—Ã€Âº ÃƒÂ´Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â´Ã™.
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
				}

				// tx, ty-1
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					// ÃÃ—Ã€Âº ÃƒÂ´Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â´Ã™.
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
				}

				// tx, ty+1
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					// ÃÃ—Ã€Âº ÃƒÂ´Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â´Ã™.
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
				}

				if ((abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
			}

			// ÃÃ–ÂºÂ¯ Â°Ã¸Â°Ã ÃˆÂ¿Â°Ãº 
			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
					// Ã€ÃšÂ½Ã…ÂµÂµ Ã‡Ã‡Ã†Ã¸ÂµÃ‰ Â¼Ã¶ Ã€Ã–Ã€Â¸Â´Ã ÃÃ–Ã€Ã‡.
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
						(m_pClientList[sOwnerH]->m_iHP > 0)) {
						// ÃÃ—Ã€Âº ÃƒÂ´Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â´Ã™.
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					}
				}

			// dX, dY
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
				Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false

			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
			if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
				(m_pClientList[sOwnerH]->m_iHP > 0)) {
				// ÃÃ—Ã€Âº ÃƒÂ´Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â´Ã™.
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
					Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false
			}
			break;

			// v2.16 2002-5-23 Â°Ã­Â±Â¤Ã‡Ã¶ 
		case DEF_MAGICTYPE_ICE_LINEAR:
			// Ã€ÃÃÃ·Â¼Â± Â»Ã³Â¿Â¡ Ã€Ã–Â´Ã‚ Â¸Ã±Ã‡Â¥Â¸Â¦ Â¸Ã°ÂµÃŽ Â¾Ã³Â¸Â®Â¸Ã§ Â°Ã¸Â°ÃÃ‡Ã‘Â´Ã™.
			sX = m_pClientList[iClientH]->m_sX;
			sY = m_pClientList[iClientH]->m_sY;

			for (i = 2; i < 4; i++) { 
				iErr = 0;
				m_Misc.GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);

				// tx, ty
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					// Â¾Ã³Â¾Ã®Â¼Â­ ÂµÂ¿Ã€Ã›Ã€ÃŒ Â´ÃŠÂ¾Ã®ÃÃ¶Â´Ã‚ ÃˆÂ¿Â°Ãº
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (m_pClientList[sOwnerH]->m_iHP < 0) goto MAGIC_NOEFFECT;
						// Ã…Â¸Â°Ã™Ã€ÃŒ Â»Ã¬Â¾Ã†Ã€Ã–Â°Ã­ Â¾Ã³Ã€Â½ Ã€ÃºÃ‡Ã—Â¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÂ´Ã™Â¸Ã© Â¾Ã³Â¾Ã®ÂºÃ™Â´Ã‚Â´Ã™.
						if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
							if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								// Â´Ã«Â»Ã³Ã€ÃŒ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Ã€ÃŽ Â°Ã¦Â¿Ã¬ Â¾Ã‹Â·ÃÃÃ˜Â´Ã™.
								SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
							}
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
							if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
								m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
							}
						}
						break;
					}
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					// ÃÃ—Ã€Âº ÃƒÂ´Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â´Ã™.
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						// Â¾Ã³Â¾Ã®Â¼Â­ ÂµÂ¿Ã€Ã›Ã€ÃŒ Â´ÃŠÂ¾Ã®ÃÃ¶Â´Ã‚ ÃˆÂ¿Â°Ãº
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							// Ã…Â¸Â°Ã™Ã€ÃŒ Â»Ã¬Â¾Ã†Ã€Ã–Â°Ã­ Â¾Ã³Ã€Â½ Ã€ÃºÃ‡Ã—Â¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÂ´Ã™Â¸Ã© Â¾Ã³Â¾Ã®ÂºÃ™Â´Ã‚Â´Ã™.
							if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									// Â´Ã«Â»Ã³Ã€ÃŒ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Ã€ÃŽ Â°Ã¦Â¿Ã¬ Â¾Ã‹Â·ÃÃÃ˜Â´Ã™.
									SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
								}
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								}
							}
							break;
						}
						//
					}
				}

				// tx-1, ty
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					// Â¾Ã³Â¾Ã®Â¼Â­ ÂµÂ¿Ã€Ã›Ã€ÃŒ Â´ÃŠÂ¾Ã®ÃÃ¶Â´Ã‚ ÃˆÂ¿Â°Ãº
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						// Ã…Â¸Â°Ã™Ã€ÃŒ Â»Ã¬Â¾Ã†Ã€Ã–Â°Ã­ Â¾Ã³Ã€Â½ Ã€ÃºÃ‡Ã—Â¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÂ´Ã™Â¸Ã© Â¾Ã³Â¾Ã®ÂºÃ™Â´Ã‚Â´Ã™.
						if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
							if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								// Â´Ã«Â»Ã³Ã€ÃŒ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Ã€ÃŽ Â°Ã¦Â¿Ã¬ Â¾Ã‹Â·ÃÃÃ˜Â´Ã™.
								SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
							}
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
							if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
								m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
							}
						}
						break;
					}
					//
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					// ÃÃ—Ã€Âº ÃƒÂ´Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â´Ã™.
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						// Â¾Ã³Â¾Ã®Â¼Â­ ÂµÂ¿Ã€Ã›Ã€ÃŒ Â´ÃŠÂ¾Ã®ÃÃ¶Â´Ã‚ ÃˆÂ¿Â°Ãº
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							// Ã…Â¸Â°Ã™Ã€ÃŒ Â»Ã¬Â¾Ã†Ã€Ã–Â°Ã­ Â¾Ã³Ã€Â½ Ã€ÃºÃ‡Ã—Â¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÂ´Ã™Â¸Ã© Â¾Ã³Â¾Ã®ÂºÃ™Â´Ã‚Â´Ã™.
							if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									// Â´Ã«Â»Ã³Ã€ÃŒ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Ã€ÃŽ Â°Ã¦Â¿Ã¬ Â¾Ã‹Â·ÃÃÃ˜Â´Ã™.
									SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
								}
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								}
							}
							break;
						}
						//
					}
				}

				// tx+1, ty
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					// Â¾Ã³Â¾Ã®Â¼Â­ ÂµÂ¿Ã€Ã›Ã€ÃŒ Â´ÃŠÂ¾Ã®ÃÃ¶Â´Ã‚ ÃˆÂ¿Â°Ãº
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						// Ã…Â¸Â°Ã™Ã€ÃŒ Â»Ã¬Â¾Ã†Ã€Ã–Â°Ã­ Â¾Ã³Ã€Â½ Ã€ÃºÃ‡Ã—Â¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÂ´Ã™Â¸Ã© Â¾Ã³Â¾Ã®ÂºÃ™Â´Ã‚Â´Ã™.
						if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
							if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								// Â´Ã«Â»Ã³Ã€ÃŒ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Ã€ÃŽ Â°Ã¦Â¿Ã¬ Â¾Ã‹Â·ÃÃÃ˜Â´Ã™.
								SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
							}
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
							if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
								m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
							}
						}
						break;
					}
					//
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					// ÃÃ—Ã€Âº ÃƒÂ´Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â´Ã™.
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						// Â¾Ã³Â¾Ã®Â¼Â­ ÂµÂ¿Ã€Ã›Ã€ÃŒ Â´ÃŠÂ¾Ã®ÃÃ¶Â´Ã‚ ÃˆÂ¿Â°Ãº
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							// Ã…Â¸Â°Ã™Ã€ÃŒ Â»Ã¬Â¾Ã†Ã€Ã–Â°Ã­ Â¾Ã³Ã€Â½ Ã€ÃºÃ‡Ã—Â¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÂ´Ã™Â¸Ã© Â¾Ã³Â¾Ã®ÂºÃ™Â´Ã‚Â´Ã™.
							if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									// Â´Ã«Â»Ã³Ã€ÃŒ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Ã€ÃŽ Â°Ã¦Â¿Ã¬ Â¾Ã‹Â·ÃÃÃ˜Â´Ã™.
									SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
								}
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								}
							}
							break;
						}
						//
					}
				}

				// tx, ty-1
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					// Â¾Ã³Â¾Ã®Â¼Â­ ÂµÂ¿Ã€Ã›Ã€ÃŒ Â´ÃŠÂ¾Ã®ÃÃ¶Â´Ã‚ ÃˆÂ¿Â°Ãº
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						// Ã…Â¸Â°Ã™Ã€ÃŒ Â»Ã¬Â¾Ã†Ã€Ã–Â°Ã­ Â¾Ã³Ã€Â½ Ã€ÃºÃ‡Ã—Â¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÂ´Ã™Â¸Ã© Â¾Ã³Â¾Ã®ÂºÃ™Â´Ã‚Â´Ã™.
						if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
							if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								// Â´Ã«Â»Ã³Ã€ÃŒ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Ã€ÃŽ Â°Ã¦Â¿Ã¬ Â¾Ã‹Â·ÃÃÃ˜Â´Ã™.
								SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
							}
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
							if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
								m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
							}
						}
						break;
					}
					//
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					// ÃÃ—Ã€Âº ÃƒÂ´Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â´Ã™.
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						// Â¾Ã³Â¾Ã®Â¼Â­ ÂµÂ¿Ã€Ã›Ã€ÃŒ Â´ÃŠÂ¾Ã®ÃÃ¶Â´Ã‚ ÃˆÂ¿Â°Ãº
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							// Ã…Â¸Â°Ã™Ã€ÃŒ Â»Ã¬Â¾Ã†Ã€Ã–Â°Ã­ Â¾Ã³Ã€Â½ Ã€ÃºÃ‡Ã—Â¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÂ´Ã™Â¸Ã© Â¾Ã³Â¾Ã®ÂºÃ™Â´Ã‚Â´Ã™.
							if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									// Â´Ã«Â»Ã³Ã€ÃŒ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Ã€ÃŽ Â°Ã¦Â¿Ã¬ Â¾Ã‹Â·ÃÃÃ˜Â´Ã™.
									SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
								}
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								}
							}
							break;
						}
						//
					}
				}

				// tx, ty+1
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					// Â¾Ã³Â¾Ã®Â¼Â­ ÂµÂ¿Ã€Ã›Ã€ÃŒ Â´ÃŠÂ¾Ã®ÃÃ¶Â´Ã‚ ÃˆÂ¿Â°Ãº
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						// Ã…Â¸Â°Ã™Ã€ÃŒ Â»Ã¬Â¾Ã†Ã€Ã–Â°Ã­ Â¾Ã³Ã€Â½ Ã€ÃºÃ‡Ã—Â¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÂ´Ã™Â¸Ã© Â¾Ã³Â¾Ã®ÂºÃ™Â´Ã‚Â´Ã™.
						if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
							if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								// Â´Ã«Â»Ã³Ã€ÃŒ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Ã€ÃŽ Â°Ã¦Â¿Ã¬ Â¾Ã‹Â·ÃÃÃ˜Â´Ã™.
								SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
							}
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
							if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
								m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
							}
						}
						break;
					}
					//
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					// ÃÃ—Ã€Âº ÃƒÂ´Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â´Ã™.
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						// Â¾Ã³Â¾Ã®Â¼Â­ ÂµÂ¿Ã€Ã›Ã€ÃŒ Â´ÃŠÂ¾Ã®ÃÃ¶Â´Ã‚ ÃˆÂ¿Â°Ãº
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							// Ã…Â¸Â°Ã™Ã€ÃŒ Â»Ã¬Â¾Ã†Ã€Ã–Â°Ã­ Â¾Ã³Ã€Â½ Ã€ÃºÃ‡Ã—Â¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÂ´Ã™Â¸Ã© Â¾Ã³Â¾Ã®ÂºÃ™Â´Ã‚Â´Ã™.
							if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									// Â´Ã«Â»Ã³Ã€ÃŒ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Ã€ÃŽ Â°Ã¦Â¿Ã¬ Â¾Ã‹Â·ÃÃÃ˜Â´Ã™.
									SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
								}
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								}
							}
							break;
						}
						//
					}
				}

				if ((abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
			}

			// ÃÃ–ÂºÂ¯ Â°Ã¸Â°Ã ÃˆÂ¿Â°Ãº 
			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
					// Ã€ÃšÂ½Ã…ÂµÂµ Ã‡Ã‡Ã†Ã¸ÂµÃ‰ Â¼Ã¶ Ã€Ã–Ã€Â¸Â´Ã ÃÃ–Ã€Ã‡.
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						// Â¾Ã³Â¾Ã®Â¼Â­ ÂµÂ¿Ã€Ã›Ã€ÃŒ Â´ÃŠÂ¾Ã®ÃÃ¶Â´Ã‚ ÃˆÂ¿Â°Ãº
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							// Ã…Â¸Â°Ã™Ã€ÃŒ Â»Ã¬Â¾Ã†Ã€Ã–Â°Ã­ Â¾Ã³Ã€Â½ Ã€ÃºÃ‡Ã—Â¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÂ´Ã™Â¸Ã© Â¾Ã³Â¾Ã®ÂºÃ™Â´Ã‚Â´Ã™.
							if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									// Â´Ã«Â»Ã³Ã€ÃŒ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Ã€ÃŽ Â°Ã¦Â¿Ã¬ Â¾Ã‹Â·ÃÃÃ˜Â´Ã™.
									SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
								}
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								}
							}
							break;
						}
						//
					}

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
						(m_pClientList[sOwnerH]->m_iHP > 0)) {
						// ÃÃ—Ã€Âº ÃƒÂ´Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â´Ã™.
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
							// Â¾Ã³Â¾Ã®Â¼Â­ ÂµÂ¿Ã€Ã›Ã€ÃŒ Â´ÃŠÂ¾Ã®ÃÃ¶Â´Ã‚ ÃˆÂ¿Â°Ãº
							switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
								// Ã…Â¸Â°Ã™Ã€ÃŒ Â»Ã¬Â¾Ã†Ã€Ã–Â°Ã­ Â¾Ã³Ã€Â½ Ã€ÃºÃ‡Ã—Â¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÂ´Ã™Â¸Ã© Â¾Ã³Â¾Ã®ÂºÃ™Â´Ã‚Â´Ã™.
								if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
									if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
										m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
										SetIceFlag(sOwnerH, cOwnerType, true);
										// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
										bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
											sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
										// Â´Ã«Â»Ã³Ã€ÃŒ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Ã€ÃŽ Â°Ã¦Â¿Ã¬ Â¾Ã‹Â·ÃÃÃ˜Â´Ã™.
										SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
									}
								}
								break;

							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
								if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
									if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
										m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
										SetIceFlag(sOwnerH, cOwnerType, true);
										// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
										bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
											sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									}
								}
								break;
							}
							//
						}
					}
				}

			// dX, dY
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
				Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false
				// Â¾Ã³Â¾Ã®Â¼Â­ ÂµÂ¿Ã€Ã›Ã€ÃŒ Â´ÃŠÂ¾Ã®ÃÃ¶Â´Ã‚ ÃˆÂ¿Â°Ãº
				switch (cOwnerType) {
				case DEF_OWNERTYPE_PLAYER:
					if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
					// Ã…Â¸Â°Ã™Ã€ÃŒ Â»Ã¬Â¾Ã†Ã€Ã–Â°Ã­ Â¾Ã³Ã€Â½ Ã€ÃºÃ‡Ã—Â¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÂ´Ã™Â¸Ã© Â¾Ã³Â¾Ã®ÂºÃ™Â´Ã‚Â´Ã™.
					if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
						if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
							m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
							SetIceFlag(sOwnerH, cOwnerType, true);
							// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
							bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
								sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
							// Â´Ã«Â»Ã³Ã€ÃŒ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Ã€ÃŽ Â°Ã¦Â¿Ã¬ Â¾Ã‹Â·ÃÃÃ˜Â´Ã™.
							SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
						}
					}
					break;

				case DEF_OWNERTYPE_NPC:
					if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
					if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
						if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
							m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
							SetIceFlag(sOwnerH, cOwnerType, true);
							// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
							bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
								sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
						}
					}
					break;
				}
				//
			}

			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
			if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
				(m_pClientList[sOwnerH]->m_iHP > 0)) {
				// ÃÃ—Ã€Âº ÃƒÂ´Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â´Ã™.
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false
					// Â¾Ã³Â¾Ã®Â¼Â­ ÂµÂ¿Ã€Ã›Ã€ÃŒ Â´ÃŠÂ¾Ã®ÃÃ¶Â´Ã‚ ÃˆÂ¿Â°Ãº
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						// Ã…Â¸Â°Ã™Ã€ÃŒ Â»Ã¬Â¾Ã†Ã€Ã–Â°Ã­ Â¾Ã³Ã€Â½ Ã€ÃºÃ‡Ã—Â¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÂ´Ã™Â¸Ã© Â¾Ã³Â¾Ã®ÂºÃ™Â´Ã‚Â´Ã™.
						if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
							if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								// Â´Ã«Â»Ã³Ã€ÃŒ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Ã€ÃŽ Â°Ã¦Â¿Ã¬ Â¾Ã‹Â·ÃÃÃ˜Â´Ã™.
								SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
							}
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
							if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
								m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
							}
						}
						break;
					}
					//
				}
			}
			break;


		case DEF_MAGICTYPE_INHIBITION:
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			switch (cOwnerType) {
			case DEF_OWNERTYPE_PLAYER:
				if ((m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 5) || (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 2)) goto MAGIC_NOEFFECT; // juan - ic fix para que no pegue si tiene AMP o PFM .
				if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
				if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INHIBITION] != 0) goto MAGIC_NOEFFECT;
				if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) goto MAGIC_NOEFFECT;
				m_pClientList[sOwnerH]->m_bInhibition = true;
				bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_INHIBITION, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
					sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
				break;
			}
			break;


		case DEF_MAGICTYPE_TREMOR: // v1.4 ÃÃ–ÂºÂ¯ Â°Ã¸Â°Ã ÃˆÂ¿Â°ÃºÂ¿Â¡ Ã€ÃŒÂ¾Ã® Ã€ÃœÂ·Ã¹ ÃˆÂ¿Â°ÃºÂ°Â¡ Ã€Ã–Â´Ã™.
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
				Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);

			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
			if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
				(m_pClientList[sOwnerH]->m_iHP > 0)) {
				// ÃÃ—Ã€Âº ÃƒÂ´Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â´Ã™.
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
					Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
			}

			// ÃÃ–ÂºÂ¯ Â°Ã¸Â°Ã ÃˆÂ¿Â°Ãº 
			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
					// Ã€ÃšÂ½Ã…ÂµÂµ Ã‡Ã‡Ã†Ã¸ÂµÃ‰ Â¼Ã¶ Ã€Ã–Ã€Â¸Â´Ã ÃÃ–Ã€Ã‡.
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
						(m_pClientList[sOwnerH]->m_iHP > 0)) {
						// ÃÃ—Ã€Âº ÃƒÂ´Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â´Ã™.
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					}
				}
			break;

		case DEF_MAGICTYPE_DAMAGE_AREA_NOSPOT:
			// ÃÃ·Â°ÃÃ€Âº ÃƒÂ³Â¸Â®Ã‡ÃÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™.
			// ÃÃ–ÂºÂ¯ Â°Ã¸Â°Ã ÃˆÂ¿Â°Ãº 
			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
					// Ã€ÃšÂ½Ã…ÂµÂµ Ã‡Ã‡Ã†Ã¸ÂµÃ‰ Â¼Ã¶ Ã€Ã–Ã€Â¸Â´Ã ÃÃ–Ã€Ã‡.
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
						(m_pClientList[sOwnerH]->m_iHP > 0)) {
						// ÃÃ—Ã€Âº ÃƒÂ´Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â´Ã™.
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					}
				}
			break;

		case DEF_MAGICTYPE_SPUP_AREA:
			// SpÂ°Â¡ Â»Ã³Â½Ã‚Ã‡Ã‘Â´Ã™.
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			// Â¸Â¶Â¹Ã½ Ã€ÃºÃ‡Ã—Ã€ÃŒ Ã‡ÃŠÂ¿Ã¤Â¾Ã¸Â´Ã™. 
			Effect_SpUp_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6);
			// ÃÃ–ÂºÂ¯ Â°Ã¸Â°Ã ÃˆÂ¿Â°Ãº 
			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
					// Ã€ÃšÂ½Ã…ÂµÂµ Ã‡Ã‡Ã†Ã¸ÂµÃ‰ Â¼Ã¶ Ã€Ã–Ã€Â¸Â´Ã ÃÃ–Ã€Ã‡.
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					// Â¸Â¶Â¹Ã½Ã€ÃºÃ‡Ã—Ã€ÃŒ Ã‡ÃŠÂ¿Ã¤ Â¾Ã¸Â´Ã™.
					Effect_SpUp_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
				}
			break;

		case DEF_MAGICTYPE_DAMAGE_LINEAR_SPDOWN:
			// ÀÏÁ÷¼± »ó¿¡ ÀÖ´Â ¸ñÇ¥¸¦ ¸ðµÎ ¾ó¸®¸ç °ø°ÝÇÑ´Ù.
			sX = m_pClientList[iClientH]->m_sX;
			sY = m_pClientList[iClientH]->m_sY;

			for (i = 2; i < 4; i++) {
				iErr = 0;
				m_Misc.GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);

				// tx, ty
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					// ¾ó¾î¼­ µ¿ÀÛÀÌ ´Ê¾îÁö´Â È¿°ú
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						// Å¸°ÙÀÌ »ì¾ÆÀÖ°í ¾óÀ½ ÀúÇ×¿¡ ½ÇÆÐÇß´Ù¸é ¾ó¾îºÙ´Â´Ù.
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
						}
						break;
					}
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					// Á×Àº Ã´ÇÏ°í ÀÖ´Â ÇÃ·¹ÀÌ¾î´Ù.
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						// ¾ó¾î¼­ µ¿ÀÛÀÌ ´Ê¾îÁö´Â È¿°ú
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							// Å¸°ÙÀÌ »ì¾ÆÀÖ°í ¾óÀ½ ÀúÇ×¿¡ ½ÇÆÐÇß´Ù¸é ¾ó¾îºÙ´Â´Ù.
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;
						}
						//
					}
				}

				// tx-1, ty
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					// ¾ó¾î¼­ µ¿ÀÛÀÌ ´Ê¾îÁö´Â È¿°ú
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						// Å¸°ÙÀÌ »ì¾ÆÀÖ°í ¾óÀ½ ÀúÇ×¿¡ ½ÇÆÐÇß´Ù¸é ¾ó¾îºÙ´Â´Ù.
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);

						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {

							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);

						}
						break;
					}
					//
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					// Á×Àº Ã´ÇÏ°í ÀÖ´Â ÇÃ·¹ÀÌ¾î´Ù.
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						// ¾ó¾î¼­ µ¿ÀÛÀÌ ´Ê¾îÁö´Â È¿°ú
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							// Å¸°ÙÀÌ »ì¾ÆÀÖ°í ¾óÀ½ ÀúÇ×¿¡ ½ÇÆÐÇß´Ù¸é ¾ó¾îºÙ´Â´Ù.
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;
						}
						//
					}
				}

				// tx+1, ty
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					// ¾ó¾î¼­ µ¿ÀÛÀÌ ´Ê¾îÁö´Â È¿°ú
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						// Å¸°ÙÀÌ »ì¾ÆÀÖ°í ¾óÀ½ ÀúÇ×¿¡ ½ÇÆÐÇß´Ù¸é ¾ó¾îºÙ´Â´Ù.
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
						}
						break;
					}
					//
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					// Á×Àº Ã´ÇÏ°í ÀÖ´Â ÇÃ·¹ÀÌ¾î´Ù.
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						// ¾ó¾î¼­ µ¿ÀÛÀÌ ´Ê¾îÁö´Â È¿°ú
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							// Å¸°ÙÀÌ »ì¾ÆÀÖ°í ¾óÀ½ ÀúÇ×¿¡ ½ÇÆÐÇß´Ù¸é ¾ó¾îºÙ´Â´Ù.
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;
						}
						//
					}
				}

				// tx, ty-1
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					// ¾ó¾î¼­ µ¿ÀÛÀÌ ´Ê¾îÁö´Â È¿°ú
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						// Å¸°ÙÀÌ »ì¾ÆÀÖ°í ¾óÀ½ ÀúÇ×¿¡ ½ÇÆÐÇß´Ù¸é ¾ó¾îºÙ´Â´Ù.
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
						}
						break;
					}
					//
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					// Á×Àº Ã´ÇÏ°í ÀÖ´Â ÇÃ·¹ÀÌ¾î´Ù.
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						// ¾ó¾î¼­ µ¿ÀÛÀÌ ´Ê¾îÁö´Â È¿°ú
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							// Å¸°ÙÀÌ »ì¾ÆÀÖ°í ¾óÀ½ ÀúÇ×¿¡ ½ÇÆÐÇß´Ù¸é ¾ó¾îºÙ´Â´Ù.
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;
						}
						//
					}
				}

				// tx, ty+1
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					// ¾ó¾î¼­ µ¿ÀÛÀÌ ´Ê¾îÁö´Â È¿°ú
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						// Å¸°ÙÀÌ »ì¾ÆÀÖ°í ¾óÀ½ ÀúÇ×¿¡ ½ÇÆÐÇß´Ù¸é ¾ó¾îºÙ´Â´Ù.
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
						}
						break;
					}
					//
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					// Á×Àº Ã´ÇÏ°í ÀÖ´Â ÇÃ·¹ÀÌ¾î´Ù.
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						// ¾ó¾î¼­ µ¿ÀÛÀÌ ´Ê¾îÁö´Â È¿°ú
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							// Å¸°ÙÀÌ »ì¾ÆÀÖ°í ¾óÀ½ ÀúÇ×¿¡ ½ÇÆÐÇß´Ù¸é ¾ó¾îºÙ´Â´Ù.
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;
						}
						//
					}
				}

				if ((abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
			}

			// ÁÖº¯ °ø°Ý È¿°ú 
			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
					// ÀÚ½Åµµ ÇÇÆøµÉ ¼ö ÀÖÀ¸´Ï ÁÖÀÇ.
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						// ¾ó¾î¼­ µ¿ÀÛÀÌ ´Ê¾îÁö´Â È¿°ú
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							// Å¸°ÙÀÌ »ì¾ÆÀÖ°í ¾óÀ½ ÀúÇ×¿¡ ½ÇÆÐÇß´Ù¸é ¾ó¾îºÙ´Â´Ù.
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;
						}
						//
					}

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
						(m_pClientList[sOwnerH]->m_iHP > 0)) {
						// Á×Àº Ã´ÇÏ°í ÀÖ´Â ÇÃ·¹ÀÌ¾î´Ù.
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
							// ¾ó¾î¼­ µ¿ÀÛÀÌ ´Ê¾îÁö´Â È¿°ú
							switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
								// Å¸°ÙÀÌ »ì¾ÆÀÖ°í ¾óÀ½ ÀúÇ×¿¡ ½ÇÆÐÇß´Ù¸é ¾ó¾îºÙ´Â´Ù.
								if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
								}
								break;

							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
								if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);

								}
								break;
							}
							//
						}
					}
				}

			// dX, dY
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
				Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false
				// ¾ó¾î¼­ µ¿ÀÛÀÌ ´Ê¾îÁö´Â È¿°ú
				switch (cOwnerType) {
				case DEF_OWNERTYPE_PLAYER:
					if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
					// Å¸°ÙÀÌ »ì¾ÆÀÖ°í ¾óÀ½ ÀúÇ×¿¡ ½ÇÆÐÇß´Ù¸é ¾ó¾îºÙ´Â´Ù.
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
						Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
					}
					break;

				case DEF_OWNERTYPE_NPC:
					if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
						Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
					}
					break;
				}
				//
			}

			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
			if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
				(m_pClientList[sOwnerH]->m_iHP > 0)) {
				// Á×Àº Ã´ÇÏ°í ÀÖ´Â ÇÃ·¹ÀÌ¾î´Ù.
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false
					// ¾ó¾î¼­ µ¿ÀÛÀÌ ´Ê¾îÁö´Â È¿°ú
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						// Å¸°ÙÀÌ »ì¾ÆÀÖ°í ¾óÀ½ ÀúÇ×¿¡ ½ÇÆÐÇß´Ù¸é ¾ó¾îºÙ´Â´Ù.
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);

						}
						break;
					}
					//
				}
			}
			break;

		case DEF_MAGICTYPE_TELEPORT:
			// Ã…ÃšÂ·Â¹Ã†Ã·Ã†Â® Â¸Â¶Â¹Ã½. sValue 4Â¿Â¡ ÂµÃ»Â¶Ã³Â¼Â­ Ã…ÃšÂ·Â¹Ã†Ã·Ã†Â® Â¸Ã±Ã€Ã»ÃÃ¶Â°Â¡ Â°Ã¡ÃÂ¤ÂµÃˆÂ´Ã™.
			
			//50Cent - Capture The Flag
			if (m_bIsCTFEvent && (m_pClientList[iClientH]->m_iStatus & 0x80000) != 0)
			{
				ShowClientMsg(iClientH, "You can not use that magic being a flag carrier.");
				goto MAGIC_NOEFFECT;
			}

			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);

			switch (m_pMagicConfigList[sType]->m_sValue4) {
			case 1:
				// Centuu : Aresden cant recall in elvine & Elvine cant recall in aresden - Fixed
				if (m_pClientList[iClientH]->m_cSide == 2 && strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "aresden") == 0) goto MAGIC_NOEFFECT;
				if (m_pClientList[iClientH]->m_cSide == 1 && strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "elvine") == 0) goto MAGIC_NOEFFECT;

				// Ã€ÃšÂ½Ã…Ã€ÃŒ Â¼Ã’Â¼Ã“ÂµÃˆ Â¸Â¶Ã€Â»Â·ÃŽ Ã…ÃšÂ·Â¹Ã†Ã·Ã†Â®. RecallÃ€ÃŒÂ´Ã™.
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (sOwnerH == iClientH)) {
					if (m_pClientList[iClientH]->IsInMap("team"))
					{
						if (m_pClientList[iClientH]->IsLocation("elvine"))
							RequestTeleportHandler(iClientH, "2", "elvine", -1, -1, true);
						else
							RequestTeleportHandler(iClientH, "2", "aresden", -1, -1, true);
					}
					else
					{
						RequestTeleportHandler(iClientH, "1   ");
					}
				}
				break;
			}
			break;

		case DEF_MAGICTYPE_SUMMON:
			// Â¼Ã’ÃˆÂ¯Â¸Â¶Â¹Ã½ 

			// Â»Ã§Ã…ÃµÃ€Ã¥ Â³Â»Â¿Â¡Â¼Â­Â´Ã‚ Â¼Ã’ÃˆÂ¯Â¸Â¶Â¹Ã½Ã€ÃŒ ÂºÃ’Â°Â¡Â´Ã‰.
			if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsFightZone == true) return;

			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			// ÃÃ¶ÃÂ¤ÂµÃˆ OwnerÂ°Â¡ MasterÂ°Â¡ ÂµÃˆÂ´Ã™. 
			if ((sOwnerH != 0) && (cOwnerType == DEF_OWNERTYPE_PLAYER)) {
				// MasterÂ·ÃŽ ÃÃ¶ÃÂ¤ÂµÃˆ Â´Ã«Â»Ã³Ã€Â» ÂµÃ»Â¶Ã³Â´Ã™Â´ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Â°Â´ÃƒÂ¼ Â¼Ã¶Â¸Â¦ Â°Ã¨Â»ÃªÃ‡Ã‘Â´Ã™. 
				iFollowersNum = iGetFollowerNumber(sOwnerH, cOwnerType);

				// Â¼Ã’ÃˆÂ¯Â¸Â¶Â¹Ã½Ã€Â» CastingÃ‡Ã‘ Ã€ÃšÃ€Ã‡ Magery/20 Â¸Â¸Ã…Â­Ã€Ã‡ Â¸Ã³Â½ÂºÃ…ÃÂ¸Â¦ Â¼Ã’ÃˆÂ¯Ã‡Ã’ Â¼Ã¶ Ã€Ã–Â´Ã™.
				if (iFollowersNum >= (m_pClientList[iClientH]->m_cSkillMastery[4] / 20)) break;

				iNamingValue = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->iGetEmptyNamingValue();
				if (iNamingValue != -1) {
					// Â´ÃµÃ€ÃŒÂ»Ã³ Ã€ÃŒ Â¸ÃŠÂ¿Â¡ NPCÂ¸Â¦ Â¸Â¸ÂµÃ©Â¼Ã¶ Â¾Ã¸Â´Ã™. Ã€ÃŒÂ¸Â§Ã€Â» Ã‡Ã’Â´Ã§Ã‡Ã’ Â¼Ã¶ Â¾Ã¸Â±Ã¢ Â¶Â§Â¹Â®.

					// NPCÂ¸Â¦ Â»Ã½Â¼ÂºÃ‡Ã‘Â´Ã™.
					ZeroMemory(cName, sizeof(cName));
					wsprintf(cName, "XX%d", iNamingValue);
					cName[0] = '_';
					cName[1] = m_pClientList[iClientH]->m_cMapIndex + 65;

					// MageryÂ¿Â¡ ÂµÃ»Â¶Ã³ Â¼Ã’ÃˆÂ¯ÂµÃ‡Â´Ã‚ Â¸Ã³Â½ÂºÃ…ÃÃ€Ã‡ ÂµÃ®Â±ÃžÃ€ÃŒ Â´ÃžÂ¶Ã³ÃÃ¸Â´Ã™.
					ZeroMemory(cNpcName, sizeof(cNpcName));

					switch (iV1) {
					case 0: // Ã€ÃÂ¹ÃÃ€Ã»Ã€ÃŽ Â°Ã¦Â¿Ã¬ 
						iResult = iDice(1, m_pClientList[iClientH]->m_cSkillMastery[4] / 10);

						// v1.42 ÃƒÃ–Ã€Ãº Â¸Ã· Â·Â¹ÂºÂ§Ã€Â» Ã€Ã”Â·Ã‚ 
						if (iResult < m_pClientList[iClientH]->m_cSkillMastery[4] / 20)
							iResult = m_pClientList[iClientH]->m_cSkillMastery[4] / 20;

						switch (iResult) {
						case 1: strcpy(cNpcName, "Slime"); break;
						case 2: strcpy(cNpcName, "Giant-Ant"); break;
						case 3: strcpy(cNpcName, "Amphis"); break;
						case 4: strcpy(cNpcName, "Orc"); break;
						case 5: strcpy(cNpcName, "Skeleton"); break;
						case 6:	strcpy(cNpcName, "Clay-Golem"); break;
						case 7:	strcpy(cNpcName, "Stone-Golem"); break;
						case 8: strcpy(cNpcName, "Orc-Mage"); break;
						case 9:	strcpy(cNpcName, "Hellbound"); break;
						case 10:strcpy(cNpcName, "Cyclops"); break;
						}
						break;

					case 1:	strcpy(cNpcName, "Orc"); break;
					case 2: strcpy(cNpcName, "Skeleton"); break;
					case 3: strcpy(cNpcName, "Clay-Golem"); break;
					case 4: strcpy(cNpcName, "Stone-Golem"); break;
					case 5: strcpy(cNpcName, "Hellbound"); break;
					case 6: strcpy(cNpcName, "Cyclops"); break;
					case 7: strcpy(cNpcName, "Troll"); break;
					case 8: strcpy(cNpcName, "Orge"); break;
					case 9: // SummonScroll(SOR)
						switch (m_pClientList[iClientH]->m_cSide) {
						case 1:  strcpy(cNpcName, "Sor-Aresden"); break;
						case 2:  strcpy(cNpcName, "Sor-Elvine"); break;
						}
						break;
					case 10: // SummonScroll(ATK)
						switch (m_pClientList[iClientH]->m_cSide) {
						case 1:  strcpy(cNpcName, "ATK-Aresden"); break;
						case 2:  strcpy(cNpcName, "ATK-Elvine"); break;
						}
						break;
					case 11: // SummonScroll(ELF)
						switch (m_pClientList[iClientH]->m_cSide) {
						case 1:  strcpy(cNpcName, "Elf-Aresden"); break;
						case 2:  strcpy(cNpcName, "Elf-Elvine"); break;
						}
						break;
					case 12: // SummonScroll(DSK)	
						switch (m_pClientList[iClientH]->m_cSide) {
						case 1:  strcpy(cNpcName, "DSK-Aresden"); break;
						case 2:  strcpy(cNpcName, "DSK-Elvine"); break;
						}
						break;
					case 13: // SummonScroll(HBT)
						switch (m_pClientList[iClientH]->m_cSide) {
						case 1:  strcpy(cNpcName, "HBT-Aresden"); break;
						case 2:  strcpy(cNpcName, "HBT-Elvine"); break;
						}
						break;
					case 14: // SummonScroll(BAR)
						switch (m_pClientList[iClientH]->m_cSide) {
						case 1:  strcpy(cNpcName, "Bar-Aresden"); break;
						case 2:  strcpy(cNpcName, "Bar-Elvine"); break;
						}
						break;
					}

					if (bCreateNewNpc(cNpcName, cName, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, 0, 0, DEF_MOVETYPE_RANDOM, &dX, &dY, cNpcWaypoint, 0, 0, m_pClientList[iClientH]->m_cSide, false, true) == false) {
						// Â½Ã‡Ã†ÃÃ‡ÃŸÃ€Â¸Â¹Ã‡Â·ÃŽ Â¿Â¹Â¾Ã ÂµÃˆ NameValueÂ¸Â¦ Ã‡Ã˜ÃÂ¦Â½ÃƒÃ…Â²Â´Ã™.
						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetNamingValueEmpty(iNamingValue);
					}
					else {
						ZeroMemory(cName_Master, sizeof(cName_Master));
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							memcpy(cName_Master, m_pClientList[sOwnerH]->m_cCharName, 10);
							break;
						case DEF_OWNERTYPE_NPC:
							memcpy(cName_Master, m_pNpcList[sOwnerH]->m_cName, 5);
							break;
						}
						bSetNpcFollowMode(cName, cName_Master, cOwnerType);
					}
				}
			}
			break;

		case DEF_MAGICTYPE_CREATE:
			// Â¹Â«Â¾Ã°Â°Â¡Â¸Â¦ Â»Ã½Â¼ÂºÂ½ÃƒÃ…Â°Â´Ã‚ Â¸Â¶Â¹Ã½ 

			// Ã€Â§Ã„Â¡Ã‡Ã’ Â¼Ã¶ Â¾Ã¸Â´Ã‚ Â°Ã·Â¿Â¡Â´Ã‚ Â»Ã½Â±Ã¢ÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™. 
			if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bGetIsMoveAllowedTile(dX, dY) == false)
				goto MAGIC_NOEFFECT;

			pItem = new class CItem;

			switch (m_pMagicConfigList[sType]->m_sValue4) {
			case 1:
				// Food
				if (iDice(1, 2) == 1)
					wsprintf(cItemName, "Meat");
				else wsprintf(cItemName, "Baguette");
				break;
			}

			_bInitItemAttr(pItem, cItemName);

			// v2.15 Â¸Â¶Â¹Ã½Ã€Â¸Â·ÃŽ Â»Ã½Â±Ã¤ Â¾Ã†Ã€ÃŒÃ…Ã›Â¿Â¡ Â°Ã­Ã€Â¯Â¹Ã¸ÃˆÂ£ Ã€ÃºÃ€Ã¥ 
			pItem->m_sTouchEffectType = DEF_ITET_ID;
			pItem->m_sTouchEffectValue1 = iDice(1, 100000);
			pItem->m_sTouchEffectValue2 = iDice(1, 100000);
			pItem->m_sTouchEffectValue3 = (short)timeGetTime();

			// Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â¼Â­Ã€Ã–Â´Ã‚ Ã€Â§Ã„Â¡Â¿Â¡ Â¹Ã¶Â¸Â°Â´Ã™. 
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(dX, dY, pItem);

			// v1.41 ÃˆÃ±Â±Ã Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â¶Â³Â¾Ã®Â¶ÃŸÂ¸Â° Â°ÃÃ€ÃŒÂ¶Ã³Â¸Ã© Â·ÃŽÂ±Ã—Â¸Â¦ Â³Â²Â±Ã¤Â´Ã™. 
			_bItemLog(DEF_ITEMLOG_DROP, iClientH, (int)-1, pItem);

			// Â´Ã™Â¸Â¥ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¿Â¡Â°Ã” Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â¶Â³Â¾Ã®ÃÃ¸ Â°ÃÃ€Â» Â¾Ã‹Â¸Â°Â´Ã™. 
			//SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
			//	dX, dY, pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4 color
			SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
				dX, dY, pItem->m_sIDnum, pItem->m_sSpriteFrame, pItem->m_cItemColor, pItem->m_dwAttribute);
			break;

		case DEF_MAGICTYPE_CREATEPOTION:
			// esto define el tipo de magia

			/*
			if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bGetIsMoveAllowedTile(dX, dY) == false)
				goto MAGIC_NOEFFECT;

			pItem = new class CItem;

			switch (m_pMagicConfigList[sType]->m_sValue4) {
			case 1:
				// Potion
				if (iDice(1, 2) == 1)
					wsprintf(cItemName, "RedCandy");
				else wsprintf(cItemName, "BlueCandy");
				break;
			}

			_bInitItemAttr(pItem, cItemName);

			pItem->m_sTouchEffectType = DEF_ITET_ID;
			pItem->m_sTouchEffectValue1 = iDice(1, 100000);
			pItem->m_sTouchEffectValue2 = iDice(1, 100000);
			pItem->m_sTouchEffectValue3 = (short)timeGetTime();

			//indica las cordenadas en donde estas
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(dX, dY, pItem);

			_bItemLog(DEF_ITEMLOG_DROP, iClientH, (int)-1, pItem);
			// Aca dice que tire el item abajo tullo
			SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
				dX, dY, pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4 color
			*/
			goto MAGIC_NOEFFECT;
			break;

		case DEF_MAGICTYPE_PROTECT:
			// ÂºÂ¸ÃˆÂ£ Â¸Â¶Â¹Ã½ 
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);

			// ÂºÂ¸ÃˆÂ£ Â»Ã³Ã…Ã‚Â¶Ã³Â´Ã‚ Â°ÃÃ€Â» Â¼Â³ÃÂ¤Ã‡ÃÂ±Ã¢ Ã€Ã¼Â¿Â¡ Ã€ÃŒÂ¹ÃŒ Ã‡Ã˜Â´Ã§ ÂºÂ¸ÃˆÂ£Â°Â¡ Â°Ã‰Â·ÃÃ€Ã–Â´Ã‚ÃÃ¶ ÃˆÂ®Ã€ÃŽÃ‡ÃÂ°Ã­ Â°Ã‰Â·ÃÃ€Ã–Â´Ã™Â¸Ã© Â¸Â¶Â¹Ã½Ã€Âº Â¹Â«Â½ÃƒÂµÃˆÂ´Ã™. 
			switch (cOwnerType) {
			case DEF_OWNERTYPE_PLAYER:
				if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
				if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] != 0) goto MAGIC_NOEFFECT;
				// v1.4334 ÃÃŸÂ¸Â³Ã€Âº Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â¿Â¡Â°Ã” Â¸Â¶Â¹Ã¦Ã€Â» Â¸Ã¸Ã‡ÃÂ°Ã” Â¼Ã¶ÃÂ¤
				if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) goto MAGIC_NOEFFECT;

				m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] = (char)m_pMagicConfigList[sType]->m_sValue4;
				switch (m_pMagicConfigList[sType]->m_sValue4) {
				case 1:
					SetProtectionFromArrowFlag(sOwnerH, DEF_OWNERTYPE_PLAYER, true);
					break;
				case 2:
				case 5:
					SetMagicProtectionFlag(sOwnerH, DEF_OWNERTYPE_PLAYER, true);
					break;
				case 3:
				case 4:
					SetDefenseShieldFlag(sOwnerH, DEF_OWNERTYPE_PLAYER, true);
					break;
				}
				break;

			case DEF_OWNERTYPE_NPC:
				if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
				if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] != 0) goto MAGIC_NOEFFECT;
				// ÃÂ¤Ã€Ã» NPCÂµÃ©Ã€Âº ÂºÂ¸ÃˆÂ£ Â¸Â¶Â¹Ã½ Ã€Ã»Â¿Ã« Â¾ÃˆÂµÃŠ.				
				if (m_pNpcList[sOwnerH]->m_cActionLimit != 0) goto MAGIC_NOEFFECT;
				m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] = (char)m_pMagicConfigList[sType]->m_sValue4;

				switch (m_pMagicConfigList[sType]->m_sValue4) {
				case 1:
					SetProtectionFromArrowFlag(sOwnerH, DEF_OWNERTYPE_NPC, true);
					break;
				case 2:
				case 5:
					SetMagicProtectionFlag(sOwnerH, DEF_OWNERTYPE_NPC, true);
					break;
				case 3:
				case 4:
					SetDefenseShieldFlag(sOwnerH, DEF_OWNERTYPE_NPC, true);
					break;
				}
				break;
			}

			// ÂºÂ¸ÃˆÂ£ ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
			bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_PROTECT, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
				sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

			// ÃˆÂ¿Â°ÃºÂ°Â¡ Â»Ã½Â°Ã¥Ã€Â½Ã€Â» Â¾Ã‹Â·ÃÃÃ˜Â´Ã™.
			if (cOwnerType == DEF_OWNERTYPE_PLAYER)
				SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_PROTECT, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
			break;


		case DEF_MAGICTYPE_SCAN:
			/*ZeroMemory(cScanMessage, sizeof(cScanMessage));
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
				switch (cOwnerType) {
				case DEF_OWNERTYPE_PLAYER:
					if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
					wsprintf(cScanMessage, "Player: %s HP:%d MP:%d.", m_pClientList[sOwnerH]->m_cCharName, m_pClientList[sOwnerH]->m_iHP, m_pClientList[sOwnerH]->m_iMP);

					SendNotifyMsg(iClientH, iClientH, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, cScanMessage);
					break;

				case DEF_OWNERTYPE_NPC:
					if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
					wsprintf(cScanMessage, "NPC: %s HP:%d MP:%d", m_pNpcList[sOwnerH]->m_cNpcName, m_pNpcList[sOwnerH]->m_iHP, m_pNpcList[sOwnerH]->m_iMana);

					SendNotifyMsg(iClientH, iClientH, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, cScanMessage);
					break;
				}
				SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_MAGIC, m_pClientList[iClientH]->m_cMapIndex,
					m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, dX, dY, 10, 10);
			}*/
			goto MAGIC_NOEFFECT;
			break;

		case DEF_MAGICTYPE_HOLDOBJECT:
			// Â¿Ã€ÂºÃªÃÂ§Ã†Â®Ã€Ã‡ Â¿Ã²ÃÃ·Ã€Ã“Ã€Â» ÂºÃ€Â¼Ã¢Ã‡Ã‘Â´Ã™. 
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {

				switch (cOwnerType) {
				case DEF_OWNERTYPE_PLAYER:
					if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
					if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] != 0) goto MAGIC_NOEFFECT;
					if (m_pClientList[sOwnerH]->m_iAddPR >= 500) goto MAGIC_NOEFFECT;
					// v1.4334 ÃÃŸÂ¸Â³Ã€Âº Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â¿Â¡Â°Ã” Ã†ÃÂ·Â²Ã€Â» Â¸Ã¸Ã‡ÃÂ°Ã” Â¼Ã¶ÃÂ¤
					if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) goto MAGIC_NOEFFECT;
					// 2002-09-10 #2 Â¾ÃˆÃ€Ã¼ÃÃ¶Â´Ã«(No-Attack-Area) Ã†ÃÂ·Â² Â¸Â¶Â¹Ã½ Â¾ÃˆÂµÃ‡Â°Ã” Ã‡Ã”
					if (cOwnerType == DEF_OWNERTYPE_PLAYER) {

						if (m_pMapList[m_pClientList[sOwnerH]->m_cMapIndex]->iGetAttribute(sX, sY, 0x00000006) != 0) goto MAGIC_NOEFFECT;
						if (m_pMapList[m_pClientList[sOwnerH]->m_cMapIndex]->iGetAttribute(dX, dY, 0x00000006) != 0) goto MAGIC_NOEFFECT;
					}

					// 2002-09-10 #3 Ã€Ã¼Â¸Ã©Ã€Ã¼Â½ÃƒÂ¿Ã Â¹ÃŒÂµÃ©Â·Â£ÂµÃ¥Â¿Â¡Â¼Â­Â¸Â¦ ÃÂ¦Â¿ÃœÃ‡ÃÂ°Ã­ Â°Â°Ã€Âº Ã†Ã­Â¿Â¡Â°Ã”Â´Ã‚ Ã†ÃÂ·Â² Â¾ÃˆÂµÃŠ
					if (strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "middleland") != 0 &&
						m_bIsCrusadeMode == false &&
						m_pClientList[iClientH]->m_cSide == m_pClientList[sOwnerH]->m_cSide)
						goto MAGIC_NOEFFECT;

					m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = (char)m_pMagicConfigList[sType]->m_sValue4;
					break;

				case DEF_OWNERTYPE_NPC:
					if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
					if (m_pNpcList[sOwnerH]->m_cMagicLevel >= 6) goto MAGIC_NOEFFECT; // v1.4 Â¸Â¶Â¹Ã½ Â·Â¹ÂºÂ§ 6Ã€ÃŒÂ»Ã³Ã€ÃŽ Â¸Â®Ã„Â¡Â±Ãž Ã€ÃŒÂ»Ã³Ã€Ã‡ Â¸Â¶Â¹Ã½ Â¸Ã³Â½ÂºÃ…ÃÂ¿Â¡Â°Ã”Â´Ã‚ Â¸Â¶ÂºÃ± Â¸Â¶Â¹Ã½ Ã…Ã«Ã‡ÃÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™.
					if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] != 0) goto MAGIC_NOEFFECT;
					m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = (char)m_pMagicConfigList[sType]->m_sValue4;
					break;
				}

				// ÂºÂ¸ÃˆÂ£ ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
				bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_HOLDOBJECT, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
					sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

				// ÃˆÂ¿Â°ÃºÂ°Â¡ Â»Ã½Â°Ã¥Ã€Â½Ã€Â» Â¾Ã‹Â·ÃÃÃ˜Â´Ã™.
				if (cOwnerType == DEF_OWNERTYPE_PLAYER)
					SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_HOLDOBJECT, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
			}
			break;

		case DEF_MAGICTYPE_INVISIBILITY:
			//50Cent - Capture The Flag
			if (m_bIsCTFEvent && (m_pClientList[iClientH]->m_iStatus & 0x80000) != 0)
			{
				ShowClientMsg(iClientH, "You can not use that magic being a flag carrier.");
				goto MAGIC_NOEFFECT;
			}
			switch (m_pMagicConfigList[sType]->m_sValue4) {
			case 1:
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);

				switch (cOwnerType) {
				case DEF_OWNERTYPE_PLAYER:
					if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
					if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] != 0) goto MAGIC_NOEFFECT;
					if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) goto MAGIC_NOEFFECT;

					//50Cent - Capture The Flag
					if (m_bIsCTFEvent && (m_pClientList[sOwnerH]->m_iStatus & 0x80000) != 0)
					{
						ShowClientMsg(iClientH, "You can not use that magic on a flag carrier.");
						goto MAGIC_NOEFFECT;
					}

					m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = (char)m_pMagicConfigList[sType]->m_sValue4;
					SetInvisibilityFlag(sOwnerH, cOwnerType, true);
					RemoveFromTarget(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INVISIBILITY);
					break;

				case DEF_OWNERTYPE_NPC:
					if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
					if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] != 0) goto MAGIC_NOEFFECT;

					if (m_pNpcList[sOwnerH]->m_cActionLimit == 0) {
						// Ã€ÃŒÂµÂ¿Ã‡ÃÃÃ¶ Â¾ÃŠÂ´Ã‚ NPCÂ´Ã‚ Ã…ÃµÂ¸Ã­ Â¸Â¶Â¹Ã½Ã€Â» Â°Ã‰ Â¼Ã¶ Â¾Ã¸Â´Ã™.
						m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = (char)m_pMagicConfigList[sType]->m_sValue4;
						SetInvisibilityFlag(sOwnerH, cOwnerType, true);
						// Ã€ÃŒ NPCÂ¸Â¦ ÃƒÃŸÃ€Ã»Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã¸ Â¸Ã³Â½ÂºÃ…ÃÂ¸Â¦ Ã‡Ã˜ÃÂ¦Â½ÃƒÃ…Â²Â´Ã™.
						RemoveFromTarget(sOwnerH, DEF_OWNERTYPE_NPC, DEF_MAGICTYPE_INVISIBILITY);
					}
					break;
				}

				// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
				bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_INVISIBILITY, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
					sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

				if (cOwnerType == DEF_OWNERTYPE_PLAYER)
					SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_INVISIBILITY, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
				break;

			case 2:
				// v1.4334 ÃÃŸÂ¸Â³Ã€Âº Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â¿Â¡Â°Ã” ÃÃŸÂµÂ¶Ã€Â» Â¸Ã¸Ã‡ÃÂ°Ã” Â¼Ã¶ÃÂ¤
				if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) goto MAGIC_NOEFFECT;

				// dX, dY Â¹ÃÂ°Ã¦ 8 ÃÃ–ÂºÂ¯Ã€Ã‡ Invisibility Â»Ã³Ã…Ã‚Ã€ÃŽ ObjectÂ°Â¡ Ã€Ã–Ã€Â¸Â¸Ã© Ã‡Ã˜ÃÂ¦ Â½ÃƒÃ…Â²Â´Ã™.
				for (ix = dX - 8; ix <= dX + 8; ix++)
					for (iy = dY - 8; iy <= dY + 8; iy++) {
						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (sOwnerH != 0) {
							switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] != 0) {
									m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = 0;
									SetInvisibilityFlag(sOwnerH, cOwnerType, false);
									bRemoveFromDelayEventList(sOwnerH, cOwnerType, DEF_MAGICTYPE_INVISIBILITY);
								}
								break;

							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
								if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] != 0) {
									m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = 0;
									SetInvisibilityFlag(sOwnerH, cOwnerType, false);
									bRemoveFromDelayEventList(sOwnerH, cOwnerType, DEF_MAGICTYPE_INVISIBILITY);
								}
								break;
							}
						}
					}
				break;
			}
			break;

		case DEF_MAGICTYPE_CREATE_DYNAMIC:
			// Dynamic ObjectÂ¸Â¦ Â»Ã½Â¼ÂºÃ‡ÃÂ°Ã­ ÃˆÂ¿Â·Ã‚Ã€ÃŒ ÃÃ¶Â¼Ã“ÂµÃ‡Â´Ã‚ Ã…Â¸Ã€Ã”.

			// v2.1 Â¸Â¶Ã€Â» Â³Â»Â¿Â¡Â¼Â­Â´Ã‚ Ã‡ÃŠÂµÃ¥ Â¸Â¶Â¹Ã½ Â±ÃÃÃ¶(Ã€Ã¼Â¸Ã©Ã€Ã¼Â½Ãƒ ÃÂ¦Â¿Ãœ)
			if (m_bIsCrusadeMode == false) {
				if (strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "aresden") == 0) return;
				if (strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "elvine") == 0) return;
				// v2.14
				if (strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "arefarm") == 0) return;
				if (strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "elvfarm") == 0) return;
			}

			switch (m_pMagicConfigList[sType]->m_sValue10) {
			case DEF_DYNAMICOBJECT_PCLOUD_BEGIN: // ÂµÂ¶Â±Â¸Â¸Â§

			case DEF_DYNAMICOBJECT_FIRE:   // Fire Ã€ÃŒÂ´Ã™.
			case DEF_DYNAMICOBJECT_SPIKE:  // Spike

				switch (m_pMagicConfigList[sType]->m_sValue11) {
				case 1:
					// wall - type
					cDir = m_Misc.cGetNextMoveDir(m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, dX, dY);
					switch (cDir) {
					case 1:	rx = 1; ry = 0;   break;
					case 2: rx = 1; ry = 1;   break;
					case 3: rx = 0; ry = 1;   break;
					case 4: rx = -1; ry = 1;  break;
					case 5: rx = 1; ry = 0;   break;
					case 6: rx = -1; ry = -1; break;
					case 7: rx = 0; ry = -1;  break;
					case 8: rx = 1; ry = -1;  break;
					}

					iAddDynamicObjectList(iClientH, DEF_OWNERTYPE_PLAYER_INDIRECT, m_pMagicConfigList[sType]->m_sValue10, m_pClientList[iClientH]->m_cMapIndex,
						dX, dY, m_pMagicConfigList[sType]->m_dwLastTime * 1000);

					bAnalyzeCriminalAction(iClientH, dX, dY);

					for (i = 1; i <= m_pMagicConfigList[sType]->m_sValue12; i++) {
						iAddDynamicObjectList(iClientH, DEF_OWNERTYPE_PLAYER_INDIRECT, m_pMagicConfigList[sType]->m_sValue10, m_pClientList[iClientH]->m_cMapIndex,
							dX + i * rx, dY + i * ry, m_pMagicConfigList[sType]->m_dwLastTime * 1000);
						bAnalyzeCriminalAction(iClientH, dX + i * rx, dY + i * ry);

						iAddDynamicObjectList(iClientH, DEF_OWNERTYPE_PLAYER_INDIRECT, m_pMagicConfigList[sType]->m_sValue10, m_pClientList[iClientH]->m_cMapIndex,
							dX - i * rx, dY - i * ry, m_pMagicConfigList[sType]->m_dwLastTime * 1000);
						bAnalyzeCriminalAction(iClientH, dX - i * rx, dY - i * ry);
					}
					break;

				case 2:
					// Field - Type
					bool bFlag = false;
					int cx, cy;
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue12; ix <= dX + m_pMagicConfigList[sType]->m_sValue12; ix++)
						for (iy = dY - m_pMagicConfigList[sType]->m_sValue12; iy <= dY + m_pMagicConfigList[sType]->m_sValue12; iy++) {
							iAddDynamicObjectList(iClientH, DEF_OWNERTYPE_PLAYER_INDIRECT, m_pMagicConfigList[sType]->m_sValue10, m_pClientList[iClientH]->m_cMapIndex,
								ix, iy, m_pMagicConfigList[sType]->m_dwLastTime * 1000, m_pMagicConfigList[sType]->m_sValue5);

							// Â¸Â¸Â¾Ã  Â¸Â¶Ã€Â»Â¿Â¡Â¼Â­ Ã‡ÃŠÂµÃ¥Â¸Â¦ Â±Ã± Â°Ã·Â¿Â¡ Â¹Â«Â°Ã­Ã‡Ã‘ Ã€ÃšÂ°Â¡ Ã€Ã–Â¾ÃºÂ´Ã™Â¸Ã© Â°Ã¸Â°ÃÃ€ÃšÂ´Ã‚ Â°Â¡ÂµÃ¥Ã€Ã‡ Â°Ã¸Â°ÃÃ€Â» Â¹ÃžÂ°Ã” ÂµÃˆÂ´Ã™. 
							if (bAnalyzeCriminalAction(iClientH, ix, iy, true) == true) {
								bFlag = true;
								cx = ix;
								cy = iy;
							}
						}
					// Ã‡ÃŠÂµÃ¥Ã€ÃŽ Â°Ã¦Â¿Ã¬ Â°Ã¦ÂºÃ±Â¸Â¦ 1Â¸Ã­Â¸Â¸ Â¼Ã’ÃˆÂ¯Ã‡ÃÂ±Ã¢ Ã€Â§Ã‡Ã”.
					if (bFlag == true) bAnalyzeCriminalAction(iClientH, cx, cy);
					break;
				}
				//
				break;

			case DEF_DYNAMICOBJECT_ICESTORM:
				// Ice-Storm Dynamic Object 
				iAddDynamicObjectList(iClientH, DEF_OWNERTYPE_PLAYER_INDIRECT, m_pMagicConfigList[sType]->m_sValue10, m_pClientList[iClientH]->m_cMapIndex,
					dX, dY, m_pMagicConfigList[sType]->m_dwLastTime * 1000,
					m_pClientList[iClientH]->m_cSkillMastery[4]);
				break;

			default:
				break;
			}
			break;

		case DEF_MAGICTYPE_POSSESSION:
			// Â¿Ã¸Â°Ã…Â¸Â®Â¿Â¡ Â¶Â³Â¾Ã®ÃÂ® Ã€Ã–Â´Ã‚ Â¹Â°Â°Ã‡Ã€Â» ÃÃ½Â¾Ã®Â¿Ã€Â´Ã‚ Â¸Â¶Â¹Ã½Ã€ÃŒÂ´Ã™. 
			// v2.12 ÃÃŸÂ¸Â³Ã€Âº Ã†Ã·ÃÂ¦Â¼Ã‡ Â¸Â¶Â¹Ã½ Â»Ã§Â¿Ã« ÂºÃ’Â°Â¡ 
			if (m_pClientList[iClientH]->m_cSide == 0) goto MAGIC_NOEFFECT;

			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			if (sOwnerH != 0) break; // v1.41 Ã†Ã·ÃÂ¦Â¼Ã‡ Â¸Â¶Â¹Ã½Ã€Âº Â»Ã§Â¶Ã·Ã€ÃŒ Â¼Â­ Ã€Ã–Â´Ã‚ Ã€Â§Â¿Â¡Â´Ã‚ ÃˆÂ¿Â·Ã‚Ã€ÃŒ Â¾Ã¸Â´Ã™. 

			//pItem = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->pGetItem(dX, dY, &sRemainItemSprite, &sRemainItemSpriteFrame, &cRemainItemColor);
			pItem = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->pGetItem(dX, dY, &sRemainItemID/*, &sRemainItemSprite, &sRemainItemSpriteFrame*/, &cRemainItemColor, &dwRemainItemAttr);
			if (pItem != 0) {
				// Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â°Â¡ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» ÃˆÂ¹ÂµÃ¦Ã‡ÃÂ¿Â´Â´Ã™. 
				if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == true) {
					// Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» ÃˆÂ¹ÂµÃ¦Ã‡ÃŸÂ´Ã™.

					// v1.411 Â·ÃŽÂ±Ã— Â³Â²Â±Ã¤Â´Ã™.
					_bItemLog(DEF_ITEMLOG_GET, iClientH, (int)-1, pItem);

					dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
					*dwp = MSGID_NOTIFY;
					wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
					*wp = DEF_NOTIFY_ITEMOBTAINED;

					cp = (char*)(cData + DEF_INDEX2_MSGTYPE + 2);

					// 1Â°Â³ ÃˆÂ¹ÂµÃ¦Ã‡ÃŸÂ´Ã™. <- Â¿Â©Â±Ã¢Â¼Â­ 1Â°Â³Â¶Ãµ Ã„Â«Â¿Ã®Ã†Â®Â¸Â¦ Â¸Â»Ã‡ÃÂ´Ã‚ Â°ÃÃ€ÃŒ Â¾Ã†Â´ÃÂ´Ã™
					*cp = 1;
					cp++;

					memcpy(cp, pItem->m_cName, 20);
					cp += 20;

					dwp = (DWORD*)cp;
					*dwp = pItem->m_dwCount;
					cp += 4;

					*cp = pItem->m_cItemType;
					cp++;

					*cp = pItem->m_cEquipPos;
					cp++;

					*cp = (char)0; // Â¾Ã²Ã€Âº Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒÂ¹Ã‡Â·ÃŽ Ã€Ã¥Ã‚Ã¸ÂµÃ‡ÃÃ¶ Â¾ÃŠÂ¾Ã’Â´Ã™.
					cp++;

					sp = (short*)cp;
					*sp = pItem->m_sLevelLimit;
					cp += 2;

					*cp = pItem->m_cGenderLimit;
					cp++;

					wp = (WORD*)cp;
					*wp = pItem->m_wCurLifeSpan;
					cp += 2;

					wp = (WORD*)cp;
					*wp = pItem->m_wWeight;
					cp += 2;

					sp = (short*)cp;
					*sp = pItem->m_sSprite;
					cp += 2;

					sp = (short*)cp;
					*sp = pItem->m_sSpriteFrame;
					cp += 2;

					*cp = pItem->m_cItemColor;
					cp++;

					*cp = (char)pItem->m_sItemSpecEffectValue2; // v1.41 
					cp++;

					dwp = (DWORD*)cp;
					*dwp = pItem->m_dwAttribute;
					cp += 4;


					if (iEraseReq == 1) delete pItem;

					// Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» ÃÃÂ°Ã­Â³Â­ ÃˆÃ„ Â³Â²Ã€Âº Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â´Ã™Â¸Â¥ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¿Â¡Â°Ã” Â¾Ã‹Â¸Â°Â´Ã™. 
					//SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_SETITEM, m_pClientList[iClientH]->m_cMapIndex,
					//	dX, dY, sRemainItemSprite, sRemainItemSpriteFrame, cRemainItemColor); // v1.4

					SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_SETITEM, m_pClientList[iClientH]->m_cMapIndex,
						dX, dY, sRemainItemID, 0, cRemainItemColor, dwRemainItemAttr);

					// Â¾Ã†Ã€ÃŒÃ…Ã› ÃÂ¤ÂºÂ¸ Ã€Ã¼Â¼Ã› 
					iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 53);

					switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:
						// Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ ÂºÂ¸Â³Â¾Â¶Â§ Â¿Â¡Â·Â¯Â°Â¡ Â¹ÃŸÂ»Ã½Ã‡ÃŸÂ´Ã™Â¸Ã© ÃÂ¦Â°Ã…Ã‡Ã‘Â´Ã™.
						DeleteClient(iClientH, true, true);
						break;
					}
				}
				else
				{
					// Â°Ã¸Â°Â£Ã€ÃŒ ÂºÃŽÃÂ·Ã‡ÃÂ°Ã…Â³Âª Ã‡Ã‘Â°Ã¨ÃÃŸÂ·Â®Ã€Â» ÃƒÃŠÂ°ÃºÃ‡ÃŸÂ´Ã™. Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â¾Ã²Ã€Â» Â¼Ã¶ Â¾Ã¸Â´Ã™.

					// Â°Â¡ÃÂ®Â¿Ã”Â´Ã¸ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â¿Ã¸Â»Ã³ÃˆÂ¸ÂºÂ¹Â½ÃƒÃ…Â²Â´Ã™. 
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(dX, dY, pItem);

					dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
					*dwp = MSGID_NOTIFY;
					wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
					*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;

					iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6);
					switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:
						// Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ ÂºÂ¸Â³Â¾Â¶Â§ Â¿Â¡Â·Â¯Â°Â¡ Â¹ÃŸÂ»Ã½Ã‡ÃŸÂ´Ã™Â¸Ã© ÃÂ¦Â°Ã…Ã‡Ã‘Â´Ã™.
						DeleteClient(iClientH, true, true);
						break;
					}
				}
			}
			//
			break;

		case DEF_MAGICTYPE_CONFUSE:
			// if the caster side is the same as the targets side, no effect occurs
			switch (m_pMagicConfigList[sType]->m_sValue4) {
			case 1: // confuse LanguageÃ¬ÂÂ´Ã«â€¹Â¤. 
			case 2: // Confusion, Mass Confusion 	
				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
						// Ã¬Å¾ÂÃ¬â€¹Â Ã«Ââ€ž Ã­â€Â¼Ã­ÂÂ­Ã«ÂÂ  Ã¬Ë†Ëœ Ã¬Å¾Ë†Ã¬Å“Â¼Ã«â€¹Ë† Ã¬Â£Â¼Ã¬ÂËœ.
						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (cOwnerType == DEF_OWNERTYPE_PLAYER) {
							// Ã­â€¢Â´Ã«â€¹Â¹ Ã¬Å“â€žÃ¬Â¹ËœÃ¬â€”Â Ã¬ÂºÂÃ«Â¦Â­Ã­â€žÂ°ÃªÂ°â‚¬ Ã¬Å¾Ë†Ã«â€¹Â¤.
							// snoopy debugged: must continue and not exit !
							if (m_pClientList[sOwnerH] == 0) continue;
							// donnot confuse self
							if (sOwnerH == iClientH) continue;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								// Ã¬Æ’ÂÃ«Å’â‚¬Ã«Â°Â©Ã¬ÂÂ´ Ã«Â§Ë†Ã«Â²â€¢ Ã¬Â â‚¬Ã­â€¢Â­Ã¬â€”Â Ã¬â€¹Â¤Ã­Å’Â¨Ã­â€“Ë†Ã«â€¹Â¤.
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_CONFUSE] != 0) break; //Ã¬ÂÂ´Ã«Â¯Â¸ Ã«â€¹Â¤Ã«Â¥Â¸ ConfuseÃ­Å¡Â¨ÃªÂ³Â¼ÃªÂ°â‚¬ Ã¬Å¾Ë†Ã«â€¹Â¤Ã«Â©Â´ Ã«Â¬Â´Ã¬â€¹Å“Ã«ÂÅ“Ã«â€¹Â¤.
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_CONFUSE] = (char)m_pMagicConfigList[sType]->m_sValue4;

								// Ã­Å¡Â¨ÃªÂ³Â¼ÃªÂ°â‚¬ Ã­â€¢Â´Ã¬Â Å“Ã«ÂÂ  Ã«â€¢Å’ Ã«Â°Å“Ã¬Æ’ÂÃ­â€¢Â  Ã«â€Å“Ã«Â Ë†Ã¬ÂÂ´ Ã¬ÂÂ´Ã«Â²Â¤Ã­Å Â¸Ã«Â¥Â¼ Ã«â€œÂ±Ã«Â¡ÂÃ­â€¢Å“Ã«â€¹Â¤.
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_CONFUSE, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

								// Ã«Â§Ë†Ã«Â²â€¢Ã¬â€”Â ÃªÂ±Â¸Ã«Â Â¸Ã¬ÂÅ’Ã¬Ââ€ž Ã¬â€¢Å’Ã«Â¦Â°Ã«â€¹Â¤.
								SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_CONFUSE, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
							}
						}
					}
				break;

			case 3: // Ilusion, Mass-Ilusion
				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
						// Ã¬Å¾ÂÃ¬â€¹Â Ã«Ââ€ž Ã­â€Â¼Ã­ÂÂ­Ã«ÂÂ  Ã¬Ë†Ëœ Ã¬Å¾Ë†Ã¬Å“Â¼Ã«â€¹Ë† Ã¬Â£Â¼Ã¬ÂËœ.
						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (cOwnerType == DEF_OWNERTYPE_PLAYER) {
							// Ã­â€¢Â´Ã«â€¹Â¹ Ã¬Å“â€žÃ¬Â¹ËœÃ¬â€”Â Ã¬ÂºÂÃ«Â¦Â­Ã­â€žÂ°ÃªÂ°â‚¬ Ã¬Å¾Ë†Ã«â€¹Â¤.
							// snoopy debugged: must continue and not exit !
							if (m_pClientList[sOwnerH] == 0) continue;
							// donnot confuse self
							if (sOwnerH == iClientH) continue;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								// Ã¬Æ’ÂÃ«Å’â‚¬Ã«Â°Â©Ã¬ÂÂ´ Ã«Â§Ë†Ã«Â²â€¢ Ã¬Â â‚¬Ã­â€¢Â­Ã¬â€”Â Ã¬â€¹Â¤Ã­Å’Â¨Ã­â€“Ë†Ã«â€¹Â¤.
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_CONFUSE] != 0) break; //Ã¬ÂÂ´Ã«Â¯Â¸ Ã«â€¹Â¤Ã«Â¥Â¸ ConfuseÃ­Å¡Â¨ÃªÂ³Â¼ÃªÂ°â‚¬ Ã¬Å¾Ë†Ã«â€¹Â¤Ã«Â©Â´ Ã«Â¬Â´Ã¬â€¹Å“Ã«ÂÅ“Ã«â€¹Â¤.
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_CONFUSE] = (char)m_pMagicConfigList[sType]->m_sValue4;

								switch (m_pMagicConfigList[sType]->m_sValue4) {
								case 3:
									// Centuu : Same side no illusion by KahBur
									//if ((strcmp(m_pClientList[sOwnerH]->m_cLocation, m_pClientList[iClientH]->m_cLocation)) == 0) goto MAGIC_NOEFFECT;
									SetIllusionFlag(sOwnerH, DEF_OWNERTYPE_PLAYER, true);
									break;
								}

								// Ã­Å¡Â¨ÃªÂ³Â¼ÃªÂ°â‚¬ Ã­â€¢Â´Ã¬Â Å“Ã«ÂÂ  Ã«â€¢Å’ Ã«Â°Å“Ã¬Æ’ÂÃ­â€¢Â  Ã«â€Å“Ã«Â Ë†Ã¬ÂÂ´ Ã¬ÂÂ´Ã«Â²Â¤Ã­Å Â¸Ã«Â¥Â¼ Ã«â€œÂ±Ã«Â¡ÂÃ­â€¢Å“Ã«â€¹Â¤.
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_CONFUSE, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

								// Ã«Â§Ë†Ã«Â²â€¢Ã¬â€”Â ÃªÂ±Â¸Ã«Â Â¸Ã¬ÂÅ’Ã¬Ââ€ž Ã¬â€¢Å’Ã«Â¦Â°Ã«â€¹Â¤.
								SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_CONFUSE, m_pMagicConfigList[sType]->m_sValue4, iClientH, 0);
							}
						}
					}
				break;

			case 4: // Ilusion Movement
				if (m_pClientList[iClientH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] != 0) break;
				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (cOwnerType == DEF_OWNERTYPE_PLAYER) {
							// snoopy debugged: must continue and not exit !
							if (m_pClientList[sOwnerH] == 0) continue;
							// donnot confuse self
							if (sOwnerH == iClientH) continue;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_CONFUSE] != 0) break;
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_CONFUSE] = (char)m_pMagicConfigList[sType]->m_sValue4;
								switch (m_pMagicConfigList[sType]->m_sValue4) {
								case 4:
									// Centuu : Same side no illusion movement by KahBur
									//if ((strcmp(m_pClientList[sOwnerH]->m_cLocation, m_pClientList[iClientH]->m_cLocation)) == 0) goto MAGIC_NOEFFECT;
									SetIllusionMovementFlag(sOwnerH, DEF_OWNERTYPE_PLAYER, true);
									break;
								}

								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_CONFUSE, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

								SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_CONFUSE, m_pMagicConfigList[sType]->m_sValue4, iClientH, 0);
							}
						}
					}
			}
			break;


		case DEF_MAGICTYPE_POISON:
			// ÃÃŸÂµÂ¶ Â¸Â¶Â¹Ã½. Â¸Ã•Ã€Ãº Â¸Â¶Â¹Ã½ Ã€ÃºÃ‡Ã—Ã€Â» Â±Â¼Â¸Â®Â°Ã­ Â´Ã™Ã€Â½Ã€Â¸Â·ÃŽ ÂµÂ¶Â¼ÂºÃ€ÃºÃ‡Ã—Ã€Â» Ã‡Ã‘Â¹Ã¸ Â´Ãµ Â±Â¼Â¸Â°Â´Ã™. 
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);

			if (m_pMagicConfigList[sType]->m_sValue4 == 1) {
				// ÃÃŸÂµÂ¶Ã€Â» Â°Ã…Â´Ã‚ Â¸Â¶Â¹Ã½ 
				switch (cOwnerType) {
				case DEF_OWNERTYPE_PLAYER:
					if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
					// v1.4334 ÃÃŸÂ¸Â³Ã€Âº Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â¿Â¡Â°Ã” ÃÃŸÂµÂ¶Ã€Â» Â¸Ã¸Ã‡ÃÂ°Ã” Â¼Ã¶ÃÂ¤
					if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) goto MAGIC_NOEFFECT;

					// Â¹Ã¼ÃÃ‹Ã‡Ã Ã€Â§Â¶Ã³Â¸Ã© 
					bAnalyzeCriminalAction(iClientH, dX, dY);

					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						// Â¸Â¶Â¹Ã½ Ã€ÃºÃ‡Ã—Â¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÂ´Ã™. ÂµÂ¶Â¼ÂºÃ€ÃºÃ‡Ã—Ã€Â» Â°Ã¨Â»ÃªÃ‡Ã‘Â´Ã™.
						if (bCheckResistingPoisonSuccess(sOwnerH, cOwnerType) == false) {
							// ÃÃŸÂµÂ¶ÂµÃ‡Â¾ÃºÂ´Ã™.
							m_pClientList[sOwnerH]->m_bIsPoisoned = true;
							m_pClientList[sOwnerH]->m_iPoisonLevel = m_pMagicConfigList[sType]->m_sValue5;
							m_pClientList[sOwnerH]->m_dwPoisonTime = dwTime;
							// 05/06/2004 - Hypnotoad - poison aura appears when cast Poison
							SetPoisonFlag(sOwnerH, cOwnerType, true);
							// ÃÃŸÂµÂ¶ÂµÃ‡Â¾ÃºÃ€Â½Ã€Â» Â¾Ã‹Â¸Â°Â´Ã™. 
							SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_POISON, m_pMagicConfigList[sType]->m_sValue5, 0, 0);
#ifdef DEF_TAIWANLOG
							_bItemLog(DEF_ITEMLOG_POISONED, sOwnerH, (char*)0, 0);
#endif
						}
					}
					break;

				case DEF_OWNERTYPE_NPC:
					if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
					if (m_pNpcList[sOwnerH]->m_iHP <= 0) goto MAGIC_NOEFFECT;
					if (bCheckResistingPoisonSuccess(sOwnerH, cOwnerType) == false)
					{	// SNOOPY: Add support for ncp been poisonned
						if (m_pNpcList[sOwnerH]->m_iPoisonLevel < 100)
							m_pNpcList[sOwnerH]->m_iPoisonLevel = 100;
						SetPoisonFlag(sOwnerH, DEF_OWNERTYPE_NPC, true);
					}
					break;
				}
			}
			else if (m_pMagicConfigList[sType]->m_sValue4 == 0) {
				// ÃÃŸÂµÂ¶Ã€Â» Ã‡ÂªÂ´Ã‚ Â¸Â¶Â¹Ã½ 
				switch (cOwnerType) {
				case DEF_OWNERTYPE_PLAYER:
					if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;

					if (m_pClientList[sOwnerH]->m_bIsPoisoned == true) {
						// ÃÃŸÂµÂ¶ÂµÃˆ Â»Ã³Ã…Ã‚Â¿Â´Â´Ã™Â¸Ã© ÃÃŸÂµÂ¶Ã€Â» Ã‡Â¬Â´Ã™.
						m_pClientList[sOwnerH]->m_bIsPoisoned = false;
						// 05/06/2004 - Hypnotoad - poison aura removed when cure cast
						SetPoisonFlag(sOwnerH, cOwnerType, false);
						// ÃÃŸÂµÂ¶Ã€ÃŒ Ã‡Â®Â·ÃˆÃ€Â½Ã€Â» Â¾Ã‹Â¸Â°Â´Ã™. 
						SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_POISON, 0, 0, 0);
					}
					break;

				case DEF_OWNERTYPE_NPC:
					if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
					if (m_pNpcList[sOwnerH]->m_iPoisonLevel > 0)
					{
						m_pNpcList[sOwnerH]->m_iPoisonLevel = 0;
						SetPoisonFlag(sOwnerH, DEF_OWNERTYPE_NPC, false);
					}
					break;
				}
			}
			break;

		case DEF_MAGICTYPE_BERSERK:
			switch (m_pMagicConfigList[sType]->m_sValue4) {
			case 1:
				// Â¹Ã¶Â¼Â­Ã„Â¿ Â¸Ã°ÂµÃ¥Â·ÃŽ Ã€Ã¼ÃˆÂ¯ÂµÃˆÂ´Ã™.
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);

				switch (cOwnerType) {
				case DEF_OWNERTYPE_PLAYER:
					if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
					if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_BERSERK] != 0) goto MAGIC_NOEFFECT;
					m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_BERSERK] = (char)m_pMagicConfigList[sType]->m_sValue4;
					SetBerserkFlag(sOwnerH, cOwnerType, true);
					break;

				case DEF_OWNERTYPE_NPC:
					if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
					if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_BERSERK] != 0) goto MAGIC_NOEFFECT;
					// ÃÂ¤Ã€Ã» NPCÂµÃ©Ã€Âº Â±Â¤ÂºÃ Â¾ÃˆÂµÃŠ				
					if (m_pNpcList[sOwnerH]->m_cActionLimit != 0) goto MAGIC_NOEFFECT;
					// 2002-09-11 #3 Ã€Ã» Â¸Ã³Â½ÂºÃ…ÃÂ´Ã‚ Â±Â¤ÂºÃÂµÃ‡ÃÃ¶ Â¾ÃŠÃ€Â½
					if (m_pClientList[iClientH]->m_cSide != m_pNpcList[sOwnerH]->m_cSide) goto MAGIC_NOEFFECT;

					if (strcmp(m_pNpcList[sOwnerH]->m_cNpcName, "Guard-Aresden")) goto MAGIC_NOEFFECT; // centu: no effect on guards
					if (strcmp(m_pNpcList[sOwnerH]->m_cNpcName, "Guard-Elvine")) goto MAGIC_NOEFFECT; // centu: no effect on guards

					m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_BERSERK] = (char)m_pMagicConfigList[sType]->m_sValue4;
					SetBerserkFlag(sOwnerH, cOwnerType, true);
					break;
				}

				// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
				bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_BERSERK, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
					sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

				if (cOwnerType == DEF_OWNERTYPE_PLAYER)
					SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_BERSERK, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
				break;
			}
			break;

		case DEF_MAGICTYPE_DAMAGE_AREA_ARMOR_BREAK: //28 m_sValue10 is used for armor decrement					
			switch (cOwnerType) {
			// For Player. 
			case DEF_OWNERTYPE_PLAYER:
				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
							ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
						}

						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
							(m_pClientList[sOwnerH]->m_iHP > 0)) {
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
								ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
							}
						}
					}
				break;
			case DEF_OWNERTYPE_NPC:
				goto MAGIC_NOEFFECT;
				break;
			}
			break;

			// Resurrection Magic. 
		case DEF_MAGICTYPE_RESURRECTION:
			// 10 Mins once
			if (m_pClientList[iClientH]->m_iSpecialAbilityTime != 0) goto MAGIC_NOEFFECT;
			m_pClientList[iClientH]->m_iSpecialAbilityTime = DEF_SPECABLTYTIMESEC / 2;
			// Get the ID of the dead Player/NPC on coords dX, dY. 
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
			switch (cOwnerType) {
				// For Player. 
			case DEF_OWNERTYPE_PLAYER:
				// Centuu : Can't use with enemys
				if ((strcmp(m_pClientList[sOwnerH]->m_cLocation, m_pClientList[iClientH]->m_cLocation)) != 0) goto MAGIC_NOEFFECT;
				// The Player has to exist. 
				if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
				// Resurrection is not for alive Players. 
				if (m_pClientList[sOwnerH]->m_bIsKilled == false) goto MAGIC_NOEFFECT;
				// Set Deadflag to Alive. 
				m_pClientList[sOwnerH]->m_bIsKilled = false;
				// Player's HP becomes half of the Max HP. 
				m_pClientList[sOwnerH]->m_iHP = iGetMaxHP(sOwnerH, false) / 2;
				// Send new HP to Player. 
				SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_HP, 0, 0, 0, 0);
				// Make Player stand up. (Currently, by a fake damage). 
				m_pMapList[m_pClientList[sOwnerH]->m_cMapIndex]->ClearDeadOwner(dX, dY);
				m_pMapList[m_pClientList[sOwnerH]->m_cMapIndex]->SetOwner(sOwnerH, DEF_OWNERTYPE_PLAYER, dX, dY);
				SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, 0, 0, 0);
				
				break;
				// Resurrection is not for NPC's. 
			case DEF_OWNERTYPE_NPC:
				goto MAGIC_NOEFFECT;
				break;
			}
			break;

		case DEF_MAGICTYPE_ICE:
			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						// Â´Ã«Â¹ÃŒÃÃ¶Â¿Ã Ã‡Ã”Â²Â²
						//Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
						// Â¾Ã³Â¾Ã®Â¼Â­ ÂµÂ¿Ã€Ã›Ã€ÃŒ Â´ÃŠÂ¾Ã®ÃÃ¶Â´Ã‚ ÃˆÂ¿Â°Ãº
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							// Ã…Â¸Â°Ã™Ã€ÃŒ Â»Ã¬Â¾Ã†Ã€Ã–Â°Ã­ Â¾Ã³Ã€Â½ Ã€ÃºÃ‡Ã—Â¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÂ´Ã™Â¸Ã© Â¾Ã³Â¾Ã®ÂºÃ™Â´Ã‚Â´Ã™.
							if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									// Â´Ã«Â»Ã³Ã€ÃŒ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Ã€ÃŽ Â°Ã¦Â¿Ã¬ Â¾Ã‹Â·ÃÃÃ˜Â´Ã™.
									SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
								}
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								}
							}
							break;
						}

					}

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
						(m_pClientList[sOwnerH]->m_iHP > 0)) {
						// ÃÃ—Ã€Âº ÃƒÂ´Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â´Ã™.
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {

							//Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							// Â¾Ã³Â¾Ã®Â¼Â­ ÂµÂ¿Ã€Ã›Ã€ÃŒ Â´ÃŠÂ¾Ã®ÃÃ¶Â´Ã‚ ÃˆÂ¿Â°Ãº. ÃÃ—ÃÃ¶ Â¾ÃŠÂ¾Ã’Ã€Â¸Â¸Ã© Ã€Ã»Â¿Ã« 
							if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									// ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);

									SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
								}
							}
						}
					}
				}
			break;

		default:
			break;
		}
	}
	else {
		// Casting ÃˆÃ„ ÂµÃ´Â·Â¹Ã€ÃŒÂ°Â¡ Â°Ã‰Â¸Â®Â´Ã‚ Â¸Â¶Â¹Ã½
		// Resurrection wand(MS.10) or Resurrection wand(MS.20)

		if (m_pMagicConfigList[sType]->m_sType == DEF_MAGICTYPE_RESURRECTION) {
			//Check if player has resurrection wand
			if (m_pClientList[iClientH] != 0 && m_pClientList[iClientH]->m_iSpecialAbilityTime == 0 &&
				m_pClientList[iClientH]->m_bIsSpecialAbilityEnabled == false) {
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (m_pClientList[sOwnerH] != 0) {

					if ((m_pClientList[iClientH]->m_iAdminUserLevel < 1) &&
						(m_pClientList[sOwnerH]->m_cSide != m_pClientList[iClientH]->m_cSide)) {
						return;
					}
					if (cOwnerType == DEF_OWNERTYPE_PLAYER && m_pClientList[sOwnerH] != 0 &&
						m_pClientList[sOwnerH]->m_iHP <= 0) {
						m_pClientList[sOwnerH]->m_bIsBeingResurrected = true;
						SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_RESURRECTPLAYER, 0, 0, 0, 0);
						if (m_pClientList[iClientH]->m_iAdminUserLevel < 2) {
							m_pClientList[iClientH]->m_bIsSpecialAbilityEnabled = true;
							m_pClientList[iClientH]->m_dwSpecialAbilityStartTime = dwTime;
							m_pClientList[iClientH]->m_iSpecialAbilityLastSec = 0;
							m_pClientList[iClientH]->m_iSpecialAbilityTime = m_pMagicConfigList[sType]->m_dwDelayTime;

							sTemp = m_pClientList[iClientH]->m_sAppr4;
							sTemp = 0xFF0F & sTemp;
							sTemp = sTemp | 0x40;
							m_pClientList[iClientH]->m_sAppr4 = sTemp;
						}
						SendNotifyMsg(0, iClientH, DEF_NOTIFY_SPECIALABILITYSTATUS, 1, m_pClientList[iClientH]->m_iSpecialAbilityType, m_pClientList[iClientH]->m_iSpecialAbilityLastSec, 0);
						SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
					}
				}
			}
		}
	}

MAGIC_NOEFFECT:;

	if (m_pClientList[iClientH] == 0) return;

	//Mana Slate
	if ((m_pClientList[iClientH]->m_iStatus & 0x800000) != 0) {
		iManaCost = 0;
	}

	if (!bIgnoreOwnerLimits) {
		//Magn0S:: Remove mana cost for GM's
		if (m_pClientList[iClientH]->m_iAdminUserLevel > 0)
			iManaCost = 0;
		else iManaCost = iManaCost;

		m_pClientList[iClientH]->m_iMP -= iManaCost; // Mana Cost
		if (m_pClientList[iClientH]->m_iMP < 0)
			m_pClientList[iClientH]->m_iMP = 0;

		CalculateSSN_SkillIndex(iClientH, 4, 1);

		SendNotifyMsg(0, iClientH, DEF_NOTIFY_MP, 0, 0, 0, 0);
	}
	SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_MAGIC, m_pClientList[iClientH]->m_cMapIndex,
		m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, dX, dY, (sType + 100), m_pClientList[iClientH]->m_sType);

}


void CGame::NpcMagicHandler(int iNpcH, short dX, short dY, short sType)
{
	short  sOwnerH;
	char   cOwnerType;
	int i, iErr, ix, iy, sX, sY, tX, tY;
	int iResult, iIceAtkRatio, iWhetherBonus, iMagicAttr;
	DWORD  dwTime = timeGetTime();

	if (m_pNpcList[iNpcH] == 0) return;
	if ((dX < 0) || (dX >= m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_sSizeX) ||
		(dY < 0) || (dY >= m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_sSizeY)) return;

	if ((sType < 0) || (sType >= 100))     return;
	if (m_pMagicConfigList[sType] == 0) return;

	if (m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_bIsAttackEnabled == false) return;

	iResult = m_pNpcList[iNpcH]->m_iMagicHitRatio;
	iIceAtkRatio = iResult / 8;
	iWhetherBonus = iGetWhetherMagicBonusEffect(sType, m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_cWhetherStatus);
	iMagicAttr = m_pMagicConfigList[sType]->m_iAttribute;

	// sX, sY is Caster position
	sX = m_pNpcList[iNpcH]->m_sX;
	sY = m_pNpcList[iNpcH]->m_sY;
	if (m_pMagicConfigList[sType]->m_dwDelayTime == 0)
	{
		switch (m_pMagicConfigList[sType]->m_sType) {
		
		case DEF_MAGICTYPE_HELLFIRE:
			iMagicAttr = 3;
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			ShowSpellEffect2(sOwnerH, 99, cOwnerType);

			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, dX, dY, 23, 24, 0 + iWhetherBonus, false, iMagicAttr);

					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, dX, dY, 23, 24, 0 + iWhetherBonus, false, iMagicAttr);
					}
				}
			break; //end hellfire

		case DEF_MAGICTYPE_FURYOFTHOR:
			sX = m_pNpcList[iNpcH]->m_sX;
			sY = m_pNpcList[iNpcH]->m_sY;

			for (i = 2; i < 10; i++) {
				iErr = 0;
				m_Misc.GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);

				// tx, ty
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY);
				if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) break;//goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
						}
						break;
					}
				}

				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) break;//goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;
						}
						//
					}
				}

				// tx-1, ty
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
				if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) break;//goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);

						}
						break;
					}
					//
				}

				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) break;//goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;
						}
						//
					}
				}

				// tx+1, ty
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
				if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) break;//goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
						}
						break;
					}
					//
				}

				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) break;//goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;
						}
						//
					}
				}

				// tx, ty-1
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
				if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) break;//goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
						}
						break;
					}
					//
				}

				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) break;//goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;
						}
						//
					}
				}

				// tx, ty+1
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
				if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) break;//goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
						}
						break;
					}
					//
				}

				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) break;//goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;
						}
						//
					}
				}

				if ((abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
			}

			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) break;//goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;
						}
						//
					}

					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
						(m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
							switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == 0) break;//goto MAGIC_NOEFFECT;
								if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false) {
									Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
								}
								break;
							}
							//
						}
					}
				}

			// dX, dY
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false) {
				Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false
				switch (cOwnerType) {
				case DEF_OWNERTYPE_PLAYER:
					if (m_pClientList[sOwnerH] == 0) break;//goto MAGIC_NOEFFECT;
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
						//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
					}
					break;
				}
				//
			}

			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
			if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
				(m_pClientList[sOwnerH]->m_iHP > 0)) {
				if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) break;//goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_ManaDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
						}
						break;
					}
					//
				}
			}
			break;

		case DEF_MAGICTYPE_DAMAGE_SPOT: // 1
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult + 50) == false)
				Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
			if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0)
				&& (m_pClientList[sOwnerH]->m_iHP > 0))
			{
				if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
					Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);
			}
			break;

		case DEF_MAGICTYPE_HPUP_SPOT: // 2
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			Effect_HpUp_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6);
			break;

		case DEF_MAGICTYPE_DAMAGE_AREA: // 3 (will fly from spell center point)
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
				Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);

			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
			if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0)
				&& (m_pClientList[sOwnerH]->m_iHP > 0))
			{
				if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
					Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);
			}
			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++)
				{
					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0)
						&& (m_pClientList[sOwnerH]->m_iHP > 0))
					{
						if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					}
				}
			break;

		case DEF_MAGICTYPE_SPDOWN_AREA: // 5
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
				Effect_SpDown_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6);
			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
						Effect_SpDown_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
				}
			break;

		case DEF_MAGICTYPE_SPUP_AREA: // 7
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			Effect_SpUp_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6);
			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					Effect_SpUp_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
				}
			break;

		case DEF_MAGICTYPE_HOLDOBJECT: // 12
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
			{
				switch (cOwnerType) {
				case DEF_OWNERTYPE_PLAYER:
					if (m_pClientList[sOwnerH] == 0) goto NMH_NOEFFECT;
					if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] != 0) goto NMH_NOEFFECT;
					// Give a chance anti-Para if very good PR... 
					if (((m_pClientList[sOwnerH]->m_cSkillMastery[23] + m_pClientList[sOwnerH]->m_iAddPR + iDice(1, 100)) >= 250)
						&& (m_pClientList[sOwnerH]->m_iSpecialAbilityType != 3)) goto NMH_NOEFFECT;
					m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = (char)m_pMagicConfigList[sType]->m_sValue4;
					break;

				case DEF_OWNERTYPE_NPC:
					if (m_pNpcList[sOwnerH] == 0) goto NMH_NOEFFECT;
					if (m_pNpcList[sOwnerH]->m_cMagicLevel >= 6) goto NMH_NOEFFECT;
					if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] != 0) goto NMH_NOEFFECT;
					m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = (char)m_pMagicConfigList[sType]->m_sValue4;
					break;
				}
				bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_HOLDOBJECT, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
					sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
				if (cOwnerType == DEF_OWNERTYPE_PLAYER)
				{
					short cParaLevel = m_pMagicConfigList[sType]->m_sValue4;
					if (cParaLevel > 1) cParaLevel = 2;
					SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_HOLDOBJECT, cParaLevel, 0, 0);
				}
			}
			break;

		case DEF_MAGICTYPE_INVISIBILITY: //13
			switch (m_pMagicConfigList[sType]->m_sValue4) {
			case 1:	// Invisibility
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				switch (cOwnerType) {
				case DEF_OWNERTYPE_PLAYER:
					if (m_pClientList[sOwnerH] == 0) goto NMH_NOEFFECT;
					if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] != 0) goto NMH_NOEFFECT;
					m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = (char)m_pMagicConfigList[sType]->m_sValue4;
					SetInvisibilityFlag(sOwnerH, cOwnerType, true);
					RemoveFromTarget(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INVISIBILITY);
					break;
				case DEF_OWNERTYPE_NPC:
					if (m_pNpcList[sOwnerH] == 0) goto NMH_NOEFFECT;
					if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] != 0) goto NMH_NOEFFECT;
					m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = (char)m_pMagicConfigList[sType]->m_sValue4;
					SetInvisibilityFlag(sOwnerH, cOwnerType, true);
					RemoveFromTarget(sOwnerH, DEF_OWNERTYPE_NPC, DEF_MAGICTYPE_INVISIBILITY);
					break;
				}
				bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_INVISIBILITY, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
					sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
				if (cOwnerType == DEF_OWNERTYPE_PLAYER)
					SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_INVISIBILITY, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
				break;
			case 2:	// Detect Invisibility
				for (ix = dX - 8; ix <= dX + 8; ix++)
					for (iy = dY - 8; iy <= dY + 8; iy++) {
						m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (sOwnerH != 0) {
							switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == 0) continue;
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] != 0)
								{
									if (m_pClientList[sOwnerH]->m_sType != 66)
									{
										m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = 0;
										SetInvisibilityFlag(sOwnerH, cOwnerType, false);
										bRemoveFromDelayEventList(sOwnerH, cOwnerType, DEF_MAGICTYPE_INVISIBILITY);
									}
								}
								break;
							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == 0) continue;
								if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] != 0)
								{
									if (m_pClientList[sOwnerH]->m_sType != 66)
									{
										m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = 0;
										SetInvisibilityFlag(sOwnerH, cOwnerType, false);
										bRemoveFromDelayEventList(sOwnerH, cOwnerType, DEF_MAGICTYPE_INVISIBILITY);
									}
								}
								break;
							}
						}
					}
				break;
			}
			break;

		case DEF_MAGICTYPE_DAMAGE_LINEAR: // 19 BloodyShockWave, LightningBolt
			for (i = 2; i < 10; i++)
			{
				iErr = 0;
				m_Misc.GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);
				// tx, ty
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY);
				if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
					Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0))
				{
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
				}
				// tx-1, ty
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
				if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
					Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0))
				{
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
				}
				// tx+1, ty
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
				if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
					Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0))
				{
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
				}
				// tx, ty-1
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
				if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
					Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0))
				{
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
				}
				// tx, ty+1
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
				if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
					Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0))
				{
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
				}
				if ((abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
			}
			// Damage Area
			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++)
				{
					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0))
					{
						if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					}
				}
			// Target damage dX, dY
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
				Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
			if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0))
			{
				if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
					Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);
			}
			break;

		case DEF_MAGICTYPE_DAMAGE_AREA_NOSPOT:  //21 (fly from caster)
			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0)
						&& (m_pClientList[sOwnerH]->m_iHP > 0))
					{
						if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					}
				}
			break;

		case DEF_MAGICTYPE_ICE:  //23 (don't fly target) Added by Snoopy
			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++)
				{
					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) 
					{
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) continue; // Snoopy: replaced goto MAGIC_NOEFFECT by continue...
						if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(-1, sOwnerH, cOwnerType, iIceAtkRatio) == false))
						{
							if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
							{
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
							}
						}
						break;
					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) continue; // Snoopy: replaced goto MAGIC_NOEFFECT by continue...
						if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(-1, sOwnerH, cOwnerType, iIceAtkRatio) == false))
						{
							if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
							{
								m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
							}
						}
						break;
					}
					// Will not ice is not discovered pretencorpse		
					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0)
						&& (m_pClientList[sOwnerH]->m_iHP > 0))
					{
						if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);
						if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(-1, sOwnerH, cOwnerType, iIceAtkRatio) == false))
						{
							if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
							{
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
							}
						}
					}
				}
			break;

		case DEF_MAGICTYPE_DAMAGE_AREA_NOSPOT_SPDOWN: //25 // EWS (will fly in random position)
			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++)
				{	// SNOOPY: Damage is on 456, Special SPdown on 789
					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
					{
						if ((iy == dY) && (ix == dX))
						{	// Will fly random direction // 10 more damage if targetted.
							Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, ix, iy, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, 10 + m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);
							Effect_SpDown_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, 10 + m_pMagicConfigList[sType]->m_sValue9);
						}
						else
						{	// Will fly random direction
							Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, ix, iy, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);
							Effect_SpDown_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
						}
					}
					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0)
						&& (m_pClientList[sOwnerH]->m_iHP > 0))
					{
						if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
						{
							Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, ix, iy, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, 10 + m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);
							Effect_SpDown_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, 10 + m_pMagicConfigList[sType]->m_sValue9);
						}
					}
				}
			break;

		case DEF_MAGICTYPE_ICE_LINEAR: //26 Added by Snoopy
			// SNOOPY: used iMagicAttr, to send info to damage procedures, not to add bonus for Berzerk Liche Wands
			iMagicAttr += 100;
			for (i = 2; i < 10; i++)
			{
				iErr = 0;
				m_Misc.GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);
				// tx, ty
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY);
				if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
				{
					Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) break;
						if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(-1, sOwnerH, cOwnerType, iIceAtkRatio) == false))
						{
							if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
							{
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
							}
						}
						break;
					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) break;
						if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(-1, sOwnerH, cOwnerType, iIceAtkRatio) == false)) {
							if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
							{
								m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
							}
						}
						break;
					}
				}
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0))
				{
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
					{
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(-1, sOwnerH, cOwnerType, iIceAtkRatio) == false))
						{
							if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
							{
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
							}
						}
					}
				}

				// tx-1, ty
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
				if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
				{
					Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) break;
						if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(-1, sOwnerH, cOwnerType, iIceAtkRatio) == false))
						{
							if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
							{
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
							}
						}
						break;
					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) break;
						if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(-1, sOwnerH, cOwnerType, iIceAtkRatio) == false)) {
							if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
							{
								m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
							}
						}
						break;
					}
				}
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0))
				{
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
					{
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(-1, sOwnerH, cOwnerType, iIceAtkRatio) == false))
						{
							if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
							{
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
							}
						}
					}
				}

				// tx+1, ty
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
				if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
				{
					Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) break;
						if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(-1, sOwnerH, cOwnerType, iIceAtkRatio) == false))
						{
							if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
							{
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
							}
						}
						break;
					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) break;
						if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(-1, sOwnerH, cOwnerType, iIceAtkRatio) == false)) {
							if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
							{
								m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
							}
						}
						break;
					}
				}
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0))
				{
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
					{
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(-1, sOwnerH, cOwnerType, iIceAtkRatio) == false))
						{
							if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
							{
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
							}
						}
					}
				}

				// tx, ty-1
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
				if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
				{
					Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) break;
						if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(-1, sOwnerH, cOwnerType, iIceAtkRatio) == false))
						{
							if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
							{
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
							}
						}
						break;
					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) break;
						if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(-1, sOwnerH, cOwnerType, iIceAtkRatio) == false)) {
							if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
							{
								m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
							}
						}
						break;
					}
				}
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0))
				{
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
					{
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(-1, sOwnerH, cOwnerType, iIceAtkRatio) == false))
						{
							if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
							{
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
							}
						}
					}
				}

				// tx, ty+1
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
				if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
				{
					Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) break;
						if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(-1, sOwnerH, cOwnerType, iIceAtkRatio) == false))
						{
							if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
							{
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
							}
						}
						break;
					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) break;
						if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(-1, sOwnerH, cOwnerType, iIceAtkRatio) == false)) {
							if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
							{
								m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
							}
						}
						break;
					}
				}
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0))
				{
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
					{
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(-1, sOwnerH, cOwnerType, iIceAtkRatio) == false))
						{
							if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
							{
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
							}
						}
					}
				}
				// Exit if reaching the Target
				if ((abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
			}

			// Area damage: depends on X/Y settings in the spell and m_sValue789, same as linear damge
			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++)
				{
					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
					{
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) break;
							if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(-1, sOwnerH, cOwnerType, iIceAtkRatio) == false)) {
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
								{
									m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
								}
							}
							break;
						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) break;
							if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(-1, sOwnerH, cOwnerType, iIceAtkRatio) == false)) {
								if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
								{
									m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								}
							}
							break;
						}
					}
					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0))
					{
						if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
						{
							Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
							if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(-1, sOwnerH, cOwnerType, iIceAtkRatio) == false))
							{
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
								{
									m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
								}
							}
						}
					}
				}

			// Target Damage: depends on m_sValue456
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
			{
				Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false
				switch (cOwnerType) {
				case DEF_OWNERTYPE_PLAYER:
					if (m_pClientList[sOwnerH] == 0) break;
					if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(-1, sOwnerH, cOwnerType, iIceAtkRatio) == false))
					{
						if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
						{
							m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
							SetIceFlag(sOwnerH, cOwnerType, true);
							bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
								sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
							SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
						}
					}
					break;
				case DEF_OWNERTYPE_NPC:
					if (m_pNpcList[sOwnerH] == 0) break;
					if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(-1, sOwnerH, cOwnerType, iIceAtkRatio) == false))
					{
						if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
						{
							m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
							SetIceFlag(sOwnerH, cOwnerType, true);
							bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
								sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
						}
					}
					break;
				}
			}
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
			if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0))
			{
				if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
				{
					Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false
					if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(-1, sOwnerH, cOwnerType, iIceAtkRatio) == false))
					{
						if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
						{
							m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
							SetIceFlag(sOwnerH, cOwnerType, true);
							bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
								sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
							SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
						}
					}
				}
			}
			break;

		case DEF_MAGICTYPE_DAMAGE_LINEAR_SPDOWN: //30 ESW
			//  m_sValue4 5 6 used for damage target
			//  m_sValue7 8 9 used for damage Linear, Area 
			//  m_sValue10 11 12 used for SP down
			// SNOOPY: used iMagicAttr, to send info to damage procedures, not to add bonus for Berzerk Liche Wands
			iMagicAttr += 100;
			for (i = 2; i < 10; i++)
			{
				iErr = 0;
				m_Misc.GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);
				// tx, ty
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY);
				if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
				{
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) continue;
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, true, iMagicAttr);
						Effect_SpDown_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10, m_pMagicConfigList[sType]->m_sValue11, m_pMagicConfigList[sType]->m_sValue12);
						break;
					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) continue;
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, true, iMagicAttr);
						break;
					}
				}
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0))
				{
					if (m_pClientList[sOwnerH] == 0) continue;
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
					{
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, true, iMagicAttr);
						Effect_SpDown_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10, m_pMagicConfigList[sType]->m_sValue11, m_pMagicConfigList[sType]->m_sValue12);
					}
				}

				// tx-1, ty
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
				if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
				{
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) continue;
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, true, iMagicAttr);
						Effect_SpDown_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10, m_pMagicConfigList[sType]->m_sValue11, m_pMagicConfigList[sType]->m_sValue12);
						break;
					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) continue;
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, true, iMagicAttr);
						break;
					}
				}
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0))
				{
					if (m_pClientList[sOwnerH] == 0) continue;
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
					{
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, true, iMagicAttr);
						Effect_SpDown_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10, m_pMagicConfigList[sType]->m_sValue11, m_pMagicConfigList[sType]->m_sValue12);
					}
				}

				// tx+1, ty
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
				if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
				{
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) continue;
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, true, iMagicAttr);
						Effect_SpDown_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10, m_pMagicConfigList[sType]->m_sValue11, m_pMagicConfigList[sType]->m_sValue12);
						break;
					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) continue;
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, true, iMagicAttr);
						break;
					}
				}
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0))
				{
					if (m_pClientList[sOwnerH] == 0) continue;
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
					{
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, true, iMagicAttr);
						Effect_SpDown_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10, m_pMagicConfigList[sType]->m_sValue11, m_pMagicConfigList[sType]->m_sValue12);
					}
				}

				// tx, ty-1
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
				if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
				{
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) continue;
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, true, iMagicAttr);
						Effect_SpDown_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10, m_pMagicConfigList[sType]->m_sValue11, m_pMagicConfigList[sType]->m_sValue12);
						break;
					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) continue;
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, true, iMagicAttr);
						break;
					}
				}
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0))
				{
					if (m_pClientList[sOwnerH] == 0) continue;
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
					{
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, true, iMagicAttr);
						Effect_SpDown_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10, m_pMagicConfigList[sType]->m_sValue11, m_pMagicConfigList[sType]->m_sValue12);
					}
				}

				// tx, ty+1
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
				if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
				{
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) continue;
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, true, iMagicAttr);
						Effect_SpDown_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10, m_pMagicConfigList[sType]->m_sValue11, m_pMagicConfigList[sType]->m_sValue12);
						break;
					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) continue;
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, true, iMagicAttr);
						break;
					}
				}
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0))
				{
					if (m_pClientList[sOwnerH] == 0) continue;
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
					{
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, true, iMagicAttr);
						Effect_SpDown_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10, m_pMagicConfigList[sType]->m_sValue11, m_pMagicConfigList[sType]->m_sValue12);
					}
				}
				// Do not strike again if target is close to caster
				if ((abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
			}
			// Area Damage
			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
			{
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++)
				{
					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
					{
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) continue;
							Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, true, iMagicAttr);
							Effect_SpDown_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10, m_pMagicConfigList[sType]->m_sValue11, m_pMagicConfigList[sType]->m_sValue12);
							break;
						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) continue;
							Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, true, iMagicAttr);
							break;
						}
					}
					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0))
					{
						if (m_pClientList[sOwnerH] == 0) break;
						if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
						{
							Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, true, iMagicAttr);
							Effect_SpDown_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10, m_pMagicConfigList[sType]->m_sValue11, m_pMagicConfigList[sType]->m_sValue12);
						}
					}
				}
			}
			// Damage on Target m_sValue456
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
			{
				switch (cOwnerType) {
				case DEF_OWNERTYPE_PLAYER:
					Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
					Effect_SpDown_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10, m_pMagicConfigList[sType]->m_sValue11, m_pMagicConfigList[sType]->m_sValue12);
					break;
				case DEF_OWNERTYPE_NPC:
					if (m_pNpcList[sOwnerH] == 0) break;
					Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
					break;
				}
			}
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
			if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0))
			{
				if (m_pClientList[sOwnerH] == 0) break;
				if (bCheckResistingMagicSuccess(-1, sOwnerH, cOwnerType, iResult) == false)
				{
					Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
					Effect_SpDown_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10, m_pMagicConfigList[sType]->m_sValue11, m_pMagicConfigList[sType]->m_sValue12);
				}
			}
			break;

		}
	}
NMH_NOEFFECT:;
	// Mana
	m_pNpcList[iNpcH]->m_iMana -= m_pMagicConfigList[sType]->m_sValue1; // sValue1�� Mana Cost
	if (m_pNpcList[iNpcH]->m_iMana < 0)
		m_pNpcList[iNpcH]->m_iMana = 0;
	SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_MAGIC, m_pNpcList[iNpcH]->m_cMapIndex,
		m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, dX, dY, (sType + 100), m_pNpcList[iNpcH]->m_sType);
}

/*********************************************************************************************************************
**  int bool CGame::bCheckResistingMagicSuccess(char cAttackerDir, short sTargetH, char cTargetType, int iHitRatio) **
**  description			:: calculates if a player resists magic														**
**  last updated		:: November 20, 2004; 8:42 PM; Hypnotoad													**
**	return value		:: bool																						**
**  commentary			::	-	hero armor for target mages adds 50 magic resistance								**
**							-	10000 or more it ratio will deduct 10000 hit ratio									**
**							-	invincible tablet is 100% magic resistance											**
**********************************************************************************************************************/
bool CGame::bCheckResistingMagicSuccess(char cAttackerDir, short sTargetH, char cTargetType, int iHitRatio)
{
	double dTmp1, dTmp2, dTmp3;
	int    iTargetMagicResistRatio, iDestHitRatio;
	char   cTargetDir, cProtect;

	switch (cTargetType) {
	case DEF_OWNERTYPE_PLAYER:
		if (m_pClientList[sTargetH] == 0) return false;
		if (m_pMapList[m_pClientList[sTargetH]->m_cMapIndex]->m_bIsAttackEnabled == false) return false;
		if (m_pClientList[sTargetH]->m_iAdminUserLevel > 0) return true;
		if ((m_pClientList[sTargetH]->m_iStatus & 0x400000) != 0) return true;
		cTargetDir = m_pClientList[sTargetH]->m_cDir;
		iTargetMagicResistRatio = m_pClientList[sTargetH]->m_cSkillMastery[3] + m_pClientList[sTargetH]->m_iAddMR;
		if ((m_pClientList[sTargetH]->m_iMag) > 50)
			iTargetMagicResistRatio += ((m_pClientList[sTargetH]->m_iMag) - 50);
		iTargetMagicResistRatio += m_pClientList[sTargetH]->m_iAddResistMagic;
		cProtect = m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT];
		break;

	case DEF_OWNERTYPE_NPC:
		if (m_pNpcList[sTargetH] == 0) return false;
		cTargetDir = m_pNpcList[sTargetH]->m_cDir;
		iTargetMagicResistRatio = m_pNpcList[sTargetH]->m_cResistMagic;
		cProtect = m_pNpcList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT];
		break;
	}

	if (cProtect == 5) return true;
	if ((iHitRatio < 1000) && (cProtect == 2)) return true;
	if (iTargetMagicResistRatio < 1) iTargetMagicResistRatio = 1;
	/*if (m_pClientList[sTargetH]->m_cHeroArmourBonus == 2) { // Centuu - crash point
		iHitRatio += 50;
	}*/

	dTmp1 = (double)(iHitRatio);
	dTmp2 = (double)(iTargetMagicResistRatio);
	dTmp3 = (dTmp1 / dTmp2) * 50.0f;
	iDestHitRatio = (int)(dTmp3);
	if (iDestHitRatio >= 100) return false;

	if (iDice(1, 100) <= iDestHitRatio) return false;

	if (cTargetType == DEF_OWNERTYPE_PLAYER)
		CalculateSSN_SkillIndex(sTargetH, 3, 1);

	return true;
}

bool CGame::bCheckResistingTremorSuccess(short sTargetH, char cTargetType)
{
	int  iStability;
	switch (cTargetType) {
	case DEF_OWNERTYPE_PLAYER:
		if (m_pClientList[sTargetH] == 0) return false;
		if (m_pClientList[sTargetH]->m_iAdminUserLevel > 0) return true;
		// Max load is lvl150*500 + str250*500 = 200 000 (=>2000)
		iStability = 1300 - (m_pClientList[sTargetH]->m_iCurWeightLoad / 100);
		iStability += m_pClientList[sTargetH]->m_iAddAbsEarth;
		if (iDice(1, 100) < iStability) return true;
		else return false;
		break;

	case DEF_OWNERTYPE_NPC:
		return false;
		break;
	}
	return false;
}
bool CGame::bCheckResistingIceSuccess(char cAttackerDir, short sTargetH, char cTargetType, int iHitRatio)
{
	int iTargetIceResistRatio;

	switch (cTargetType) {
	case DEF_OWNERTYPE_PLAYER:
		if (m_pClientList[sTargetH] == 0) return false;
		if (m_pClientList[sTargetH]->m_iAdminUserLevel > 0) return true;
		iTargetIceResistRatio = m_pClientList[sTargetH]->m_iAddAbsWater * 2;
		if (m_pClientList[sTargetH]->m_dwWarmEffectTime != 0) {
			if ((timeGetTime() - m_pClientList[sTargetH]->m_dwWarmEffectTime) < 30000) return true;
		}
		break;

	case DEF_OWNERTYPE_NPC:
		if (m_pNpcList[sTargetH] == 0) return false;
		if (m_pNpcList[sTargetH]->m_sType == 91) return true; // new
		iTargetIceResistRatio = (m_pNpcList[sTargetH]->m_cResistMagic) - (m_pNpcList[sTargetH]->m_cResistMagic / 3); // ���⿡ ���� ��� ��ġ �Է�. NPC�� ��� ���� ������ 70% ���� 
		break;
	}
	if (iDice(1, 100) <= iTargetIceResistRatio) return true;
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////
//  int CGame::iClientMotion_Magic_Handler(int iClientH, short sX, short sY, char cDir)
//  description			:: checks if player is casting magic
//  last updated		:: October 29, 2004; 6:51 PM; Hypnotoad
//	return value		:: int
/////////////////////////////////////////////////////////////////////////////////////
int CGame::iClientMotion_Magic_Handler(int iClientH, short sX, short sY, char cDir)
{
	char  cData[100];
	DWORD* dwp;
	WORD* wp;
	int     iRet;

	if (m_pClientList[iClientH] == 0) return 0;
	if (m_pClientList[iClientH]->m_bIsKilled == true) return 0;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return 0;

	if ((sX != m_pClientList[iClientH]->m_sX) || (sY != m_pClientList[iClientH]->m_sY)) return 2;

	int iStX, iStY;
	if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex] != 0)
	{
		iStX = m_pClientList[iClientH]->m_sX / 20;
		iStY = m_pClientList[iClientH]->m_sY / 20;
		m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iPlayerActivity++;
		switch (m_pClientList[iClientH]->m_cSide) {
		case 1: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iAresdenActivity++; break;
		case 2: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iElvineActivity++;  break;
		case 0:
		default: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iNeutralActivity++; break;
		}
	}

	ClearSkillUsingStatus(iClientH);

	m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->ClearOwner(0, iClientH, DEF_OWNERTYPE_PLAYER, sX, sY);
	m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetOwner(iClientH, DEF_OWNERTYPE_PLAYER, sX, sY);

	if ((m_pClientList[iClientH]->m_iStatus & 0x10) != 0) {
		SetInvisibilityFlag(iClientH, DEF_OWNERTYPE_PLAYER, false);
		bRemoveFromDelayEventList(iClientH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INVISIBILITY);
		m_pClientList[iClientH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = 0;
	}

	m_pClientList[iClientH]->m_cDir = cDir;

	dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
	*dwp = MSGID_RESPONSE_MOTION;
	wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
	*wp = DEF_OBJECTMOTION_CONFIRM;

	iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6);
	switch (iRet) {
	case DEF_XSOCKEVENT_QUENEFULL:
	case DEF_XSOCKEVENT_SOCKETERROR:
	case DEF_XSOCKEVENT_CRITICALERROR:
	case DEF_XSOCKEVENT_SOCKETCLOSED:
		DeleteClient(iClientH, true, true);
		return 0;
	}

	return 1;
}

bool CGame::_bDecodeMagicConfigFileContents(char* pData, DWORD dwMsgSize)
{
	char* pContents, * token, cTxt[120];
	char seps[] = "= \t\n";
	char cReadModeA = 0;
	char cReadModeB = 0;
	int  iMagicConfigListIndex = 0;
	class CStrTok* pStrTok;

	pContents = new char[dwMsgSize + 1];
	ZeroMemory(pContents, dwMsgSize + 1);
	memcpy(pContents, pData, dwMsgSize);

	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();
	while (token != 0) {
		if (cReadModeA != 0) {
			switch (cReadModeA) {
			case 1:
				switch (cReadModeB) {
				case 1:
					// ¸¶¹ý ¹øÈ£ 
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}

					if (m_pMagicConfigList[atoi(token)] != 0) {
						// ÀÌ¹Ì ÇÒ´çµÈ ¹øÈ£°¡ ÀÖ´Ù. ¿¡·¯ÀÌ´Ù.
						PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Duplicate magic number.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pMagicConfigList[atoi(token)] = new class CMagic;
					iMagicConfigListIndex = atoi(token);

					cReadModeB = 2;
					break;

				case 2:
					// ¸¶¹ý ÀÌ¸§ 
					ZeroMemory(m_pMagicConfigList[iMagicConfigListIndex]->m_cName, sizeof(m_pMagicConfigList[iMagicConfigListIndex]->m_cName));
					memcpy(m_pMagicConfigList[iMagicConfigListIndex]->m_cName, token, strlen(token));
					cReadModeB = 3;
					break;

				case 3:
					// ¸¶¹ý Á¾·ù m_sType
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pMagicConfigList[iMagicConfigListIndex]->m_sType = atoi(token);
					cReadModeB = 4;
					break;

				case 4:
					// ¸¶¹ý µô·¹ÀÌ ½Ã°£ m_dwDelayTime
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pMagicConfigList[iMagicConfigListIndex]->m_dwDelayTime = atoi(token);
					cReadModeB = 5;
					break;

				case 5:
					// ¸¶¹ý Áö¼Ó½Ã°£ m_dwLastTime
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pMagicConfigList[iMagicConfigListIndex]->m_dwLastTime = atoi(token);
					cReadModeB = 6;
					break;

				case 6:
					// m_sValue1
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pMagicConfigList[iMagicConfigListIndex]->m_sValue1 = atoi(token);
					cReadModeB = 7;
					break;

				case 7:
					// m_sValue2
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pMagicConfigList[iMagicConfigListIndex]->m_sValue2 = atoi(token);
					cReadModeB = 8;
					break;

				case 8:
					// m_sValue3
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pMagicConfigList[iMagicConfigListIndex]->m_sValue3 = atoi(token);
					cReadModeB = 9;
					break;

				case 9:
					// m_sValue4
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pMagicConfigList[iMagicConfigListIndex]->m_sValue4 = atoi(token);
					cReadModeB = 10;
					break;

				case 10:
					// m_sValue5
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pMagicConfigList[iMagicConfigListIndex]->m_sValue5 = atoi(token);
					cReadModeB = 11;
					break;

				case 11:
					// m_sValue6
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pMagicConfigList[iMagicConfigListIndex]->m_sValue6 = atoi(token);
					cReadModeB = 12;
					break;

				case 12:
					// m_sValue7
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pMagicConfigList[iMagicConfigListIndex]->m_sValue7 = atoi(token);
					cReadModeB = 13;
					break;

				case 13:
					// m_sValue8
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pMagicConfigList[iMagicConfigListIndex]->m_sValue8 = atoi(token);
					cReadModeB = 14;
					break;

				case 14:
					// m_sValue9
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pMagicConfigList[iMagicConfigListIndex]->m_sValue9 = atoi(token);
					cReadModeB = 15;
					break;

				case 15:
					// m_sValue10
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pMagicConfigList[iMagicConfigListIndex]->m_sValue10 = atoi(token);
					cReadModeB = 16;
					break;

				case 16:
					// m_sValue11
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pMagicConfigList[iMagicConfigListIndex]->m_sValue11 = atoi(token);
					cReadModeB = 17;
					break;

				case 17:
					// m_sValue12
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pMagicConfigList[iMagicConfigListIndex]->m_sValue12 = atoi(token);
					cReadModeB = 18;
					break;

				case 18:
					// m_sIntLimit
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pMagicConfigList[iMagicConfigListIndex]->m_sIntLimit = atoi(token);
					cReadModeB = 19;
					break;

				case 19:
					// m_iGoldCost
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pMagicConfigList[iMagicConfigListIndex]->m_iGoldCost = atoi(token);

					cReadModeB = 20;
					break;

				case 20:
					// m_cCategory
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pMagicConfigList[iMagicConfigListIndex]->m_cCategory = atoi(token);
					cReadModeB = 21;
					break;

				case 21:
					// m_iAttribute
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pMagicConfigList[iMagicConfigListIndex]->m_iAttribute = atoi(token);
					cReadModeA = 0;
					cReadModeB = 0;
					break;
				}
				break;

			default:
				break;
			}
		}
		else {
			if (memcmp(token, "magic", 5) == 0) {
				cReadModeA = 1;
				cReadModeB = 1;
			}

		}
		token = pStrTok->pGet();

	}

	delete pStrTok;
	delete[] pContents;
	if ((cReadModeA != 0) || (cReadModeB != 0)) {
		PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file contents error!");
		return false;
	}

	wsprintf(cTxt, "(!) MAGIC(Total:%d) configuration - success!", iMagicConfigListIndex);
	PutLogList(cTxt);

	return true;

}

bool CGame::RequestStudyMagicHandler(int iClientH, char* pName, bool bSucces, bool bIsPurchase)
{
	char* cp, cMagicName[31], cData[100];
	DWORD* dwp, dwGoldCount = 0;
	WORD* wp;
	int* ip, iReqInt, iCost, iRet, iTempInt;
	bool bMagic = true;
	iTempInt = m_pClientList[iClientH]->m_iInt;
	// SNOOPY: Angelic allow to buy Spell but not to read manual.

	if (m_pClientList[iClientH] == 0) return false;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return false;

	ZeroMemory(cData, sizeof(cData));
	ZeroMemory(cMagicName, sizeof(cMagicName));
	memcpy(cMagicName, pName, 30);
	iRet = _iGetMagicNumber(cMagicName, &iReqInt, &iCost);

	if (iRet != -1)
	{
		
		if (bIsPurchase == true)
		{
			if (m_pMagicConfigList[iRet]->m_iGoldCost < 0) bMagic = false;
			dwGoldCount = dwGetItemCount(iClientH, "Gold");
			if ((DWORD)iCost > dwGoldCount) bMagic = false;
			//iTempInt += m_pClientList[iClientH]->m_iAngelicInt;
			// SNOOPY: Was Buggy couldn't leran a Spell Book outside Magic Tower !
			if (m_pClientList[iClientH]->m_bIsInsideWizardTower == false) bMagic = false; //return false;
		}

		if (m_pClientList[iClientH]->m_cMagicMastery[iRet] != 0) bMagic = false; //return false;

		if ((iReqInt <= iTempInt) && (bMagic == true))
		{
			if (bIsPurchase == true) 
				SetItemCount(iClientH, "Gold", (dwGoldCount - iCost));

			iCalcTotalWeight(iClientH);

			m_pClientList[iClientH]->m_cMagicMastery[iRet] = 1;
			// Snoopy: notify sender function off the succes.
			bSucces = true;
			dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
			*wp = DEF_NOTIFY_MAGICSTUDYSUCCESS;
			cp = (char*)(cData + DEF_INDEX2_MSGTYPE + 2);
			*cp = iRet;
			cp++;
			memcpy(cp, cMagicName, 30);
			cp += 30;
			iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 37);
			switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				DeleteClient(iClientH, true, true);
				break;
			}
			return true;
		}
		else
		{
			bSucces = false;
			dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
			*wp = DEF_NOTIFY_MAGICSTUDYFAIL;
			cp = (char*)(cData + DEF_INDEX2_MSGTYPE + 2);
			*cp = 1;
			cp++;
			*cp = iRet;
			cp++;
			memcpy(cp, cMagicName, 30);
			cp += 30;
			ip = (int*)cp;
			*ip = iCost;
			cp += 4;
			ip = (int*)cp;
			*ip = iReqInt;
			cp += 4;
			iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 46);
			switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				DeleteClient(iClientH, true, true);
				break;
			}
		}
	}
	return false;
}


int CGame::_iGetMagicNumber(char* pMagicName, int* pReqInt, int* pCost)
{
	int i;
	char cTmpName[31];

	ZeroMemory(cTmpName, sizeof(cTmpName));
	strcpy(cTmpName, pMagicName);

	for (i = 0; i < DEF_MAXMAGICTYPE; i++)
		if (m_pMagicConfigList[i] != 0) {
			if (memcmp(cTmpName, m_pMagicConfigList[i]->m_cName, 30) == 0) {
				// °°Àº ÀÌ¸§À» °¡Áø ¸¶¹ý ¼³Á¤À» Ã£¾Ò´Ù. ¸¶¹ý ¹øÈ£¸¦ ¹ÝÈ¯ÇÑ´Ù.
				*pReqInt = (int)m_pMagicConfigList[i]->m_sIntLimit;
				*pCost = (int)m_pMagicConfigList[i]->m_iGoldCost;

				return i;
			}
		}

	return -1;
}

// 2003-04-21 ÀÎÆ®¿¡ µû¸¥ ¸¶¹ýÀ» »èÁ¦ ½ÃÄÑÁØ´Ù...
/////////////////////////////////////////////////////////////////////////////////////
//  bool CGame::bCheckMagicInt(int iClientH)  //another retarded korean function
//  desc		 :: ÀÎÆ®°¡ ³»·Á°¬À»¶§ »èÁ¦ÇÒ ¸¶¹ýÀ» Ã³¸®ÇÑ´Ù... Ã¼Å©¸¸...
//	return value :: ¹«Á¶°Ç true   // ....dumbass koreans
//  date		 :: 2003-04-21
/////////////////////////////////////////////////////////////////////////////////////
void CGame::bCheckMagicInt(int iClientH)
{

	for (int i = 0;i < DEF_MAXMAGICTYPE;i++)
	{
		if (m_pMagicConfigList[i] != 0) {
			if (m_pMagicConfigList[i]->m_sIntLimit > (m_pClientList[iClientH]->m_iInt))
			{
				m_pClientList[iClientH]->m_cMagicMastery[i] = 0;
			}
		}
	}
}

//Magn0S:: Battle Staffs coded by Magn0S for Helbreath Friend's 02/2012
void CGame::BattleMageMagicHandler(int iClientH, int dX, int dY, short sType, bool bItemEffect, int iV1)
{
	short  sX, sY, sOwnerH, sMagicCircle, sLevelMagic, sAppr2, sEqStatus;
	char   cOwnerType;
	double dV1, dV2, dV3, dV4;
	int    i, iErr, ix, iy, iResult, iWhetherBonus;
	int    tX, tY, iManaCost, iMagicAttr, iDmgBonus, iBonusDmgTwo;
	DWORD dwTime = timeGetTime();
	WORD  wWeaponType;


	if (m_pClientList[iClientH] == 0) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

	if ((dX < 0) || (dX >= m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_sSizeX) ||
		(dY < 0) || (dY >= m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_sSizeY)) return;


	if (m_pClientList[iClientH]->m_cMapIndex < 0) return;
	if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex] == 0) return;

	if ((sType < 0) || (sType >= 100))     return;
	if (m_pMagicConfigList[sType] == 0) return;
	if ((bItemEffect == false) && (m_pClientList[iClientH]->m_cMagicMastery[sType] != 1)) return;

	if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsAttackEnabled == false) return;

	//Magn0S:: Change to only uses magic if is weapon type 35. (LLF-MS30WANDS...)
	if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND] != -1) {
		wWeaponType = ((m_pClientList[iClientH]->m_sAppr2 & 0x0FF0) >> 4);
		if (wWeaponType != 51) return;
	}

	//Magn0S:: One Handed or Shield equiped dont uses magic item attack!
	if ((m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND] != -1) ||
		(m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LHAND] != -1)) return;

	//Magn0S:: Magic Attack itens cant be used with IMC!
	if (m_pClientList[iClientH]->m_bInhibition == true) {
		SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, 0, -1, 0);
		return;
	}

	//Magn0S:: Update 07/03/2012 - Cant "attack" in peace mode.
	sAppr2 = (short)((m_pClientList[iClientH]->m_sAppr2 & 0xF000) >> 12);
	if (sAppr2 == 0) return;


	//Magn0S:: Add to decreased the endurace of wands.
	if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND] != -1)
		sEqStatus = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
	else sEqStatus = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];

	if (sEqStatus != -1) {

		if ((m_pClientList[iClientH]->m_pItemList[sEqStatus] != 0) &&
			(m_pClientList[iClientH]->m_pItemList[sEqStatus]->m_wMaxLifeSpan != 0)) {

			if (m_pClientList[iClientH]->m_cSide != 0) {
				if (m_pClientList[iClientH]->m_pItemList[sEqStatus]->m_wCurLifeSpan <= 0)
					m_pClientList[iClientH]->m_pItemList[sEqStatus]->m_wCurLifeSpan = 0;
				else m_pClientList[iClientH]->m_pItemList[sEqStatus]->m_wCurLifeSpan --;
				SendNotifyMsg(0, iClientH, DEF_NOTIFY_CURLIFESPAN, sEqStatus, m_pClientList[iClientH]->m_pItemList[sEqStatus]->m_wCurLifeSpan, 0, 0);
			}

			if (m_pClientList[iClientH]->m_pItemList[sEqStatus]->m_wCurLifeSpan == 0) {
				SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[iClientH]->m_pItemList[sEqStatus]->m_cEquipPos, sEqStatus, 0, 0);
				ReleaseItemHandler(iClientH, sEqStatus, true);
			}
		}
	}

	//Magn0S:: Set extra dmg's increaseting the values.
	switch (iV1) {
	case 0: // Normal attack
		iBonusDmgTwo = 15; // 1�
		iDmgBonus = 30; // 2�
		break;
	case 1: // Critical attack
		iBonusDmgTwo = 30; // 1�
		iDmgBonus = 40;
		break;
	default:
		iBonusDmgTwo = 0; // 1�
		iDmgBonus = 0;
		PutLogList("Diffent iV1 received in PlayerMagicItensHander...");
		break;
	}

	/*if (iV1 == 1)
		iDmgBonus = 10;
	else iDmgBonus = 5;*/

	//iBonusDmgTwo = 0; // 1�

	sX = m_pClientList[iClientH]->m_sX;
	sY = m_pClientList[iClientH]->m_sY;


	iWhetherBonus = iGetWhetherMagicBonusEffect(sType, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cWhetherStatus);

	//Battle Staffs cost 50% of normal MP cost
	iManaCost = m_pMagicConfigList[sType]->m_sValue1 / 2;
	//Magn0S:: Cancelado Extra mana cost em safe-mode
	/*if ((m_pClientList[iClientH]->m_bIsSafeAttackMode == true) &&
		(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsFightZone == false)) {
		iManaCost += (iManaCost / 2) - (iManaCost / 10);
	}*/
	if (m_pClientList[iClientH]->m_iManaSaveRatio > 0) {
		dV1 = (double)m_pClientList[iClientH]->m_iManaSaveRatio;
		dV2 = (double)(dV1 / 100.0f);
		dV3 = (double)iManaCost;
		dV1 = dV2 * dV3;
		dV2 = dV3 - dV1;
		iManaCost = (int)dV2;
		if (iManaCost <= 0) iManaCost = 1;
	}

	//Magn0S:: Hungers status chances of failed magic weapons attacks.
	if (((m_pClientList[iClientH]->m_iHungerStatus <= 10) || (m_pClientList[iClientH]->m_iSP <= 0)) && (iDice(1, 1000) <= 100)) {
		SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, 0, -1, 0);
		return;
	}

	// Mana 
	if (m_pClientList[iClientH]->m_iMP < iManaCost) {
		return;
	}

	iResult = m_pClientList[iClientH]->m_cSkillMastery[4];

	if (m_pClientList[iClientH]->m_iMag > 50) iResult += ((m_pClientList[iClientH]->m_iMag) - 50);

	sMagicCircle = (sType / 10) + 1;
	sLevelMagic = (m_pClientList[iClientH]->m_iLevel / 10);
	if (sMagicCircle != sLevelMagic) {
		if (sMagicCircle > sLevelMagic) {
			dV1 = (double)(m_pClientList[iClientH]->m_iLevel - sLevelMagic * 10);
			dV2 = (double)abs(sMagicCircle - sLevelMagic) * _tmp_iMLevelPenalty[sMagicCircle];
			dV3 = (double)abs(sMagicCircle - sLevelMagic) * 10;
			dV4 = (dV1 / dV3) * dV2;

			iResult -= abs(abs(sMagicCircle - sLevelMagic) * _tmp_iMLevelPenalty[sMagicCircle] - (int)dV4);
		}
		else {
			iResult += 5 * abs(sMagicCircle - sLevelMagic);
		}
	}

	// Magn0S::2020 Adicionado iMag/2 como extra hit-ratio
	iResult += m_pClientList[iClientH]->m_iAddAR + ((m_pClientList[iClientH]->m_iMag) / 2);

	if (iResult <= 0) iResult = 1;
	if (sType >= 80) iResult += 10000;


	if (m_pMagicConfigList[sType]->m_cCategory == 1) {

		if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->iGetAttribute(sX, sY, 0x00000005) != 0) return;
	}
	iMagicAttr = m_pMagicConfigList[sType]->m_iAttribute;

	//Magn0S:: If player is INV, turns no inv.
	if ((m_pClientList[iClientH]->m_iStatus & 0x10) != 0) {
		SetInvisibilityFlag(iClientH, DEF_OWNERTYPE_PLAYER, false);
		bRemoveFromDelayEventList(iClientH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INVISIBILITY);
		m_pClientList[iClientH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = 0;
	}

	m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);

	if (m_pMagicConfigList[sType]->m_dwDelayTime == 0) {
		switch (m_pMagicConfigList[sType]->m_sType) {

		case DEF_MAGICTYPE_DAMAGE_AREA_NOSPOT: // Meteor!
			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)

				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);

					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, (m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo), (m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus) + iWhetherBonus, false, iMagicAttr);

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);

					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
						(m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, (m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo), (m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus) + iWhetherBonus, false, iMagicAttr);
					}
				}
			break;

		case DEF_MAGICTYPE_DAMAGE_SPOT:
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
				Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);

			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
			if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
				(m_pClientList[sOwnerH]->m_iHP > 0)) {
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
					Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
			}
			break;

		case DEF_MAGICTYPE_SPDOWN_SPOT: // DEF_MAGICTYPE_DAMAGE_SPOT_SPDOWN
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
				Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
				Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
			}

			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
			if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
				(m_pClientList[sOwnerH]->m_iHP > 0)) {
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
					Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
				}
			}
			break;


			//Magn0S:: Add para ter dmg igual esw mas sem tirar SP
		case DEF_MAGICTYPE_DAMAGE_LINEAR:
			sX = m_pClientList[iClientH]->m_sX;
			sY = m_pClientList[iClientH]->m_sY;

			for (i = 2; i < 10; i++) {
				iErr = 0;
				m_Misc.GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);

				// tx, ty
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
						}
						break;
					}
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							break;
						}
						//
					}
				}

				// tx-1, ty
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);

						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {

							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);

						}
						break;
					}
					//
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							break;
						}
					}
				}

				// tx+1, ty
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
						}
						break;
					}
					//
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							break;
						}
						//
					}
				}

				// tx, ty-1
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
						}
						break;
					}
					//
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							break;
						}
						//
					}
				}

				// tx, ty+1
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
						}
						break;
					}
					//
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							break;
						}
						//
					}
				}

				if ((abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
			}

			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							break;
						}
						//
					}

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
						(m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
							switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
								if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									//Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
								}
								break;

							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
								if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									//Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);

								}
								break;
							}
							//
						}
					}
				}

			// dX, dY
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
				Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false
				switch (cOwnerType) {
				case DEF_OWNERTYPE_PLAYER:
					if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
						//Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
					}
					break;

				case DEF_OWNERTYPE_NPC:
					if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
						//Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
					}
					break;
				}
			}

			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
			if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
				(m_pClientList[sOwnerH]->m_iHP > 0)) {
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);

						}
						break;
					}
				}
			}
			break;

			// v2.16 2002-5-23  
			//Magn0S:: Changed Ice Damge Linear to equal dmg's beetwen blizz/esw
		case DEF_MAGICTYPE_ICE_LINEAR:
			sX = m_pClientList[iClientH]->m_sX;
			sY = m_pClientList[iClientH]->m_sY;

			for (i = 2; i < 10; i++) {
				iErr = 0;
				m_Misc.GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);

				// tx, ty
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
						}
						if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
							if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
							}
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
						}
						if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
							if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
								m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
							}
						}
						break;
					}
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//	Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
								}
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//	Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								}
							}
							break;
						}
					}
				}

				// tx-1, ty
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
						}
						if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
							if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
							}
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//	Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
						}
						if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
							if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
								m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
							}
						}
						break;
					}
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//	Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
								}
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//		Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								}
							}
							break;
						}
					}
				}

				// tx+1, ty
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//	Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
						}
						if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
							if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
							}
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//		Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
						}
						if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
							if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
								m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
							}
						}
						break;
					}
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//			Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
								}
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//		Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								}
							}
							break;
						}
					}
				}

				// tx, ty-1
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//		Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
						}
						if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
							if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
							}
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//			Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
						}
						if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
							if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
								m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
							}
						}
						break;
					}
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//		Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
								}
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//	Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								}
							}
							break;
						}
					}
				}

				// tx, ty+1
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//			Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
						}
						if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
							if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);

								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
							}
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//		Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
						}
						if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
							if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
								m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
							}
						}
						break;
					}
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//		Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
								}
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//	Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								}
							}
							break;
						}
					}
				}

				if ((abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
			}

			//    
			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//		Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
								}
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								//		Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								}
							}
							break;
						}
					}

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
						(m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
							switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
								if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									//	Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
								}
								if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
									if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
										m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
										SetIceFlag(sOwnerH, cOwnerType, true);
										bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
											sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
										SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
									}
								}
								break;

							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
								if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									//		Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
								}
								if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
									if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
										m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
										SetIceFlag(sOwnerH, cOwnerType, true);

										bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
											sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									}
								}
								break;
							}
						}
					}
				}

			// dX, dY
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
				Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false 
				switch (cOwnerType) {
				case DEF_OWNERTYPE_PLAYER:
					if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
						//	Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
					}
					if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
						if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
							m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
							SetIceFlag(sOwnerH, cOwnerType, true);
							bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
								sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
							SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
						}
					}
					break;

				case DEF_OWNERTYPE_NPC:
					if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
						//		Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
					}
					if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
						if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
							m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
							SetIceFlag(sOwnerH, cOwnerType, true);
							bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
								sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
						}
					}
					break;
				}
			}

			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
			if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
				(m_pClientList[sOwnerH]->m_iHP > 0)) {

				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false 
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//		Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
						}
						if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
							if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
							}
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							//		Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
						}
						if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
							if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
								m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
								SetIceFlag(sOwnerH, cOwnerType, true);
								bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
									sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
							}
						}
						break;
					}
				}
			}
			break;

		case DEF_MAGICTYPE_HPUP_SPOT:
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			Effect_HpUp_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6);
			break;

		case DEF_MAGICTYPE_TREMOR: // v1.4    
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
				Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);

			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
			if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
				(m_pClientList[sOwnerH]->m_iHP > 0)) {
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
					Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
			}

			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
						(m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
					}
				}
			break;

		case DEF_MAGICTYPE_DAMAGE_AREA:
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
				Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);

			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
			if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
				(m_pClientList[sOwnerH]->m_iHP > 0)) {
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
					Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
			}

			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
						(m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
					}
				}
			break;

		case DEF_MAGICTYPE_DAMAGE_AREA_NOSPOT_SPDOWN:
			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);
						Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
					}

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
						(m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);
							Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
						}
					}
				}
			break;

			// v2.16 2002-5-23  
		case DEF_MAGICTYPE_DAMAGE_AREA_ARMOR_BREAK:
			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
						ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
					}

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
						(m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
							ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
						}
					}
				}
			break;

		case DEF_MAGICTYPE_ICE:
			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						//Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							// v2.19 2002-11-17     
							if (m_pClientList[sOwnerH]->m_bIsPlayerCivil == true) goto MAGIC_NOEFFECT;
							if (m_pClientList[sOwnerH]->m_cSide == DEF_NETURAL) goto MAGIC_NOEFFECT;
							if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
								}
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									//    
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								}
							}
							break;
						}

					}

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
						(m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {

							//Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
										sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);

									SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
								}
							}
						}
					}
				}
			break;

		case DEF_MAGICTYPE_DAMAGE_LINEAR_SPDOWN:

			sX = m_pClientList[iClientH]->m_sX;
			sY = m_pClientList[iClientH]->m_sY;

			for (i = 2; i < 10; i++) {
				iErr = 0;
				m_Misc.GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);

				// tx, ty
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
						}
						break;
					}
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							break;
						}
						//
					}
				}

				// tx-1, ty
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);

						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {

							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);

						}
						break;
					}
					//
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							break;
						}
					}
				}

				// tx+1, ty
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
						}
						break;
					}
					//
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							break;
						}
						//
					}
				}

				// tx, ty-1
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
						}
						break;
					}
					//
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							break;
						}
						//
					}
				}

				// tx, ty+1
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
						}
						break;
					}
					//
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
					(m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							break;
						}
						//
					}
				}

				if ((abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
			}

			for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
				for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
							}
							break;
						}
						//
					}

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
						(m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus + iWhetherBonus, false, iMagicAttr);
							switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
								if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
								}
								break;

							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
								if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);

								}
								break;
							}
							//
						}
					}
				}

			// dX, dY
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
				Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false
				switch (cOwnerType) {
				case DEF_OWNERTYPE_PLAYER:
					if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
						Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
					}
					break;

				case DEF_OWNERTYPE_NPC:
					if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
						Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
					}
					break;
				}
			}

			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
			if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
				(m_pClientList[sOwnerH]->m_iHP > 0)) {
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false
					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8 + iBonusDmgTwo, m_pMagicConfigList[sType]->m_sValue9 + iDmgBonus);

						}
						break;
					}
				}
			}
			break;
		default:
			break;
		}
	}
	else {

	}

MAGIC_NOEFFECT:;
	if (m_pClientList[iClientH] == 0) return;

	// Magn0S:: Put to dont decreased mp of gm's.
	if (m_pClientList[iClientH]->m_iAdminUserLevel > 0)
		iManaCost = 0;
	else iManaCost = iManaCost;

	//Mana Slate
	if ((m_pClientList[iClientH]->m_iStatus & 0x800000) != 0) {
		iManaCost = 0;
	}

	m_pClientList[iClientH]->m_iMP -= iManaCost;
	if (m_pClientList[iClientH]->m_iMP < 0)
		m_pClientList[iClientH]->m_iMP = 0;

	CalculateSSN_SkillIndex(iClientH, 4, 1);

	SendNotifyMsg(0, iClientH, DEF_NOTIFY_MP, 0, 0, 0, 0);
	SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_MAGIC, m_pClientList[iClientH]->m_cMapIndex,
		m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, dX, dY, (sType + 100), m_pClientList[iClientH]->m_sType);

}

void CGame::ShowSpellEffect2(int iClientH, short sType, int ownertype)
{
	int iX, iY;
	if (sType == 0) return;
	if (sType <= 0)	sType = 1;
	if (sType > 100) sType = 100;

	switch (ownertype) {
	case DEF_OWNERTYPE_PLAYER:
		if (m_pClientList[iClientH] == 0) return;
		iX = m_pClientList[iClientH]->m_sX;
		iY = m_pClientList[iClientH]->m_sY;
		SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_MAGIC, m_pClientList[iClientH]->m_cMapIndex,
			m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, iX, iY, (sType + 100), m_pClientList[iClientH]->m_sType);
		break;

	case DEF_OWNERTYPE_NPC:
		if (m_pNpcList[iClientH] == 0) return;
		iX = m_pNpcList[iClientH]->m_sX;
		iY = m_pNpcList[iClientH]->m_sY;
		SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_MAGIC, m_pNpcList[iClientH]->m_cMapIndex,
			m_pNpcList[iClientH]->m_sX, m_pNpcList[iClientH]->m_sY, iX, iY, (sType + 100), m_pNpcList[iClientH]->m_sType);
		break;

	}
}