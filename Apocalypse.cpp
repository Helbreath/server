#include "Apocalypse.h"

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

Apocalypse::Apocalypse()
{}

Apocalypse::~Apocalypse()
{}

void CGame::ApocalypseStarter()
{
	SYSTEMTIME SysTime;
	int i;

	if (m_bIsCrusadeMode == true) return;
	if (m_bIsApocalypseMode == true) return;
	if (m_bIsHeldenianMode) return;
	if (m_bIsApocalypseStarter == false) return;

	char* cp, cData[120];
	DWORD* dwp, dwApocalypseGUID;

	GetLocalTime(&SysTime);
	for (i = 0; i < DEF_MAXSCHEDULE; i++) {
		if ((m_stApocalypseScheduleStart[i].iDay == SysTime.wDayOfWeek) &&
			(m_stApocalypseScheduleStart[i].iHour == SysTime.wHour) &&
			(m_stApocalypseScheduleStart[i].iMinute == SysTime.wMinute)) {
			wsprintf(G_cTxt, "(!) Apocalypse Start : time(%d %d:%d), index(%d) schedule", m_stApocalypseScheduleStart[i].iDay, m_stApocalypseScheduleStart[i].iHour, m_stApocalypseScheduleStart[i].iMinute, i);
			PutLogList(G_cTxt);
			//GlobalStartApocalypseMode(i, 0);

			dwApocalypseGUID = timeGetTime();
			if (dwApocalypseGUID < 10) dwApocalypseGUID += 10;
			ZeroMemory(cData, sizeof(cData));
			cp = (char*)cData;
			*cp = GSM_BEGINAPOCALYPSE;
			cp++;
			dwp = (DWORD*)cp;
			*dwp = dwApocalypseGUID;
			cp += 4;
			bStockMsgToGateServer(cData, 5);
			LocalStartApocalypse(dwApocalypseGUID); // Gate will no return order to this server
			break;
		}
	}
}

//**************************************************************************************
// HBx server introduced only the skeletton off Apocalypse functions, and sometime not so good!
// I rewrote them, maybe not following the "official" pattern but it works!
// I also needed to add many new functions.
//**************************************************************************************
// SNOOPY: Revamped Apocalypse functions:
//         bReadApocalypseGUIDFile(DWORD dwApocalypseGUID) 
//         Reads the GUID file when server starts
// DynamicGateType = 1: Opens Gate when Apoc begins, for 15 min
// DynamicGateType = 2: Open the gate when map is empty
// DynamicGateType = 3: Gate stays closed will change to 4 value to open it, along with a boss spawn.
// DynamicGateType = 4: Gate is openned. (set by server when Abaddon is spawning)
// DynamicGateType = 5: Gate is used by GM command    
// m_iApocalypseMobGenType = 1: Gate will open as soon as the map is empty
// m_iApocalypseMobGenType = 2: A big mob will spawn when map is empty, gate will open depend if Type 2 or 3.
//**************************************************************************************
void CGame::bReadApocalypseGUIDFile(char* cFn)
{
	FILE* pFile;
	HANDLE hFile;
	DWORD  dwFileSize;
	char* cp, * token, cReadMode;
	char seps[] = "= \t\n";
	class CStrTok* pStrTok;
	cReadMode = 0;
	hFile = CreateFile(cFn, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	dwFileSize = GetFileSize(hFile, 0);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
	pFile = fopen(cFn, "rt");
	if (pFile == 0)
	{
		PutLogList("(!) Cannot open ApocalypseGUID file.");
		return;
	}
	else
	{
		PutLogList("(!) Reading ApocalypseGUID file...");
		cp = new char[dwFileSize + 2];
		ZeroMemory(cp, dwFileSize + 2);
		fread(cp, dwFileSize, 1, pFile);
		pStrTok = new class CStrTok(cp, seps);
		token = pStrTok->pGet();
		while (token != 0)
		{
			if (cReadMode != 0)
			{
				switch (cReadMode) {
				case 1:
					m_dwApocalypseGUID = (DWORD)atoi(token);
					cReadMode = 0;
					break;
				}
			}
			else
			{
				if (memcmp(token, "ApocalypseGUID", 14) == 0) cReadMode = 1;
			}
			token = pStrTok->pGet();
		}
		delete pStrTok;
		delete[] cp;
		fclose(pFile);
	}
	wsprintf(G_cTxt, "(!) Read ApocalypseGUID (%d) file.", m_dwApocalypseGUID);
	PutLogList(G_cTxt);
}

//**************************************************************************************
// SNOOPY: Revamped Apocalypse functions:
//         _CreateApocalypseGUID(DWORD dwApocalypseGUID) 
//         Create the GUID file if it doesn't exist at Apoc beginning
//         or at the Apocalypse end.
//**************************************************************************************
void CGame::_CreateApocalypseGUID(DWORD dwApocalypseGUID)
{
	char* cp, cTxt[256], cTemp[1024];
	FILE* pFile;
	
	pFile = fopen("GameConfigs\\ApocalypseGUID.Txt", "wt");
	if (pFile == 0)
	{
		wsprintf(cTxt, "(!) Cannot create Apocalypse GUID (%d) file", dwApocalypseGUID);
		PutLogList(cTxt);
		return;
	}
	else
	{
		ZeroMemory(cTemp, sizeof(cTemp));
		ZeroMemory(cTxt, sizeof(cTxt));
		wsprintf(cTxt, "ApocalypseGUID = %d\n", dwApocalypseGUID);
		strcat(cTemp, cTxt);
		cp = (char*)cTemp;
		fwrite(cp, strlen(cp), 1, pFile);
		wsprintf(cTxt, "(!) Apocalypse GUID (%d) file created", dwApocalypseGUID);
		PutLogList(cTxt);
		fclose(pFile);
	}
}

//**************************************************************************************
// SNOOPY: Revamped Apocalypse functions:
//         LocalStartApocalypse
//		   called by:      started by Gate msg
//         -> Notify players
//         -> Opens the gates
//		   -> Prevent npc generation on "kill all to go out" maps
//		   -> Execute local Openning or closing gates by GM command
//**************************************************************************************
void CGame::LocalStartApocalypse(DWORD dwApocalypseGUID)
{
	/*if (dwApocalypseGUID == 1)// Means want to open Gate
	{
		ForceOpen_ApocalypseGate();
		return;
	}
	else if (dwApocalypseGUID == 2)// Means want to close Gate
	{
		ForceClose_ApocalypseGate();
		return;
	}*/
	int i;
	m_bIsApocalypseMode = true;
	if (dwApocalypseGUID != 0)
	{
		m_dwApocalypseGUID = dwApocalypseGUID;
		_CreateApocalypseGUID(dwApocalypseGUID);
		m_dwApocalypseGateOpenTime = dwApocalypseGUID;
		m_dwApocalypseGateCloseTime = dwApocalypseGUID + m_sApocalypseFinish * 60 * 1000; // will close in 3hs - centu
	}
	for (i = 0; i < DEF_MAXCLIENTS; i++)
	{
		if (m_pClientList[i] != 0)
		{
			SendNotifyMsg(0, i, DEF_NOTIFY_APOCGATESTARTMSG, 0, 0, 0, 0);
		}
	}
	for (i = 0; i < DEF_MAXMAPS; i++)
	{
		if (m_pMapList[i] != 0)
		{
			if (m_pMapList[i]->m_iApocalypseMobGenType != 0)
				//1 for no respawn, 2 for Boss spawn when map empty
			{
				m_pMapList[i]->m_iMaximumObjectDefault = m_pMapList[i]->m_iMaximumObject;
				m_pMapList[i]->m_iMaximumObject = 0;
			}
		}
	}
	OpenCloseApocalypseGate();
	PutLogList("(!)Apocalypse Mode ON.");
	PutLogList(" ");
}

//**************************************************************************************
// SNOOPY: GlobalStartApocalypseMode
//		   Called by: by local command or GM command
//         -> Notify all game servers to start apocalypse
//         -> Used to force Open or Closed the initial Gates
//**************************************************************************************
void CGame::GlobalStartApocalypseMode(int iClientH, int iMode)
{
	char* cp, cData[120];
	DWORD* dwp, dwApocalypseGUID;
	char cString[200];
	ZeroMemory(cString, sizeof(cString));
	if ((iClientH != 0) && (m_pClientList[iClientH]->m_iAdminUserLevel < 3)) {
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, 0);
		return;
	}
	if ((m_bIsApocalypseMode == true) && (iMode == 0)) return;
	if (m_bIsHeldenianMode == true) return;
	if (m_bIsCrusadeMode == true) return;

	switch (iMode) {
	case 1: // Force open Gates
		dwApocalypseGUID = 1;
		if (iClientH != 0)
		{
			wsprintf(G_cTxt, "Admin Order(%s): /openapocalypsegate \tAdminIP(%s)"
				, m_pClientList[iClientH]->m_cCharName
				, m_pClientList[iClientH]->m_cIPaddress);
			wsprintf(cString, "Apocalypse gate opening in progress.");
		}
		else
		{
			wsprintf(G_cTxt, "Local command: openapocalypsegate");
		}
		break;
	case 2: // Force close gates
		dwApocalypseGUID = 2;
		if (iClientH != 0)
		{
			wsprintf(G_cTxt, "Admin Order(%s): /closeapocalypsegate \tAdminIP(%s)"
				, m_pClientList[iClientH]->m_cCharName
				, m_pClientList[iClientH]->m_cIPaddress);
			wsprintf(cString, "Apocalypse gate closing in progress.");
		}
		else
		{
			wsprintf(G_cTxt, "Local command: closeapocalypsegate");
		}
		break;
	case 0: // Start Apocalypse
		dwApocalypseGUID = timeGetTime();
		if (dwApocalypseGUID < 10) dwApocalypseGUID += 10;
		if (iClientH != 0)
		{
			wsprintf(G_cTxt, "Admin Order(%s): /beginapocalypse \tAdminIP(%s)"
				, m_pClientList[iClientH]->m_cCharName
				, m_pClientList[iClientH]->m_cIPaddress);
			wsprintf(cString, "Apocalypse starting in progress.");
		}
		else
		{
			wsprintf(G_cTxt, "Local command: beginapocalypse");
		}
		break;
	}
	PutAdminLogFileList(G_cTxt);
	PutLogEventFileList(G_cTxt);
	PutLogList(G_cTxt);
	if (iClientH != 0)
	{
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_IPACCOUNTINFO, 0, 0, 0, cString);
	}

	ZeroMemory(cData, sizeof(cData));
	cp = (char*)cData;
	*cp = GSM_BEGINAPOCALYPSE;
	cp++;
	dwp = (DWORD*)cp;
	*dwp = dwApocalypseGUID;
	cp += 4;
	bStockMsgToGateServer(cData, 5);
	LocalStartApocalypse(dwApocalypseGUID); // Gate will no return order to this server
}

//**************************************************************************************
// SNOOPY: Revamped Apocalypse functions:
//         GlobalEndApocalypseMode:  
//         -> Notify all game servers
//**************************************************************************************
void CGame::GlobalEndApocalypseMode(int iClientH)
{
	char* cp, cData[120];
	if (m_bIsApocalypseMode == false) return;

	if (iClientH > 0)
	{
		wsprintf(G_cTxt, "Admin Order(%s): /endapocalypse \tAdminIP(%s)"
			, m_pClientList[iClientH]->m_cCharName
			, m_pClientList[iClientH]->m_cIPaddress);
		char cString[200];
		ZeroMemory(cString, sizeof(cString));
		wsprintf(cString, "Apocalypse ending in progress.");
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_IPACCOUNTINFO, 0, 0, 0, cString);
	}
	else
	{
		wsprintf(G_cTxt, "Automated: endapocalypse");
	}
	PutAdminLogFileList(G_cTxt);
	PutLogEventFileList(G_cTxt);
	PutLogList(G_cTxt);

	ZeroMemory(cData, sizeof(cData));
	cp = (char*)cData;
	*cp = GSM_ENDAPOCALYPSE;
	cp++;
	bStockMsgToGateServer(cData, 5);
	LocalEndApocalypse(); // Gate will not return order to this server
}

//**************************************************************************************
// SNOOPY: Revamped Apocalypse functions:
//         LocalEndApocalypse: started by Gate msg
//         -> Notify players
//         -> Close the gates
//		   -> Enable npc generation on all maps
//**************************************************************************************
void CGame::LocalEndApocalypse()
{
	m_bIsApocalypseMode = false;
	m_bIsApocalypseGateOpen = false;
	int i;
	//DWORD  dwTime = timeGetTime();
	m_dwApocalypseGateCloseTime = 0;//dwTime - 1;
	m_dwApocalypseGateOpenTime = 0;//dwTime - 100; // alreaddy closed
	for (i = 0; i < DEF_MAXCLIENTS; i++)
	{
		if (m_pClientList[i] != 0)
		{
			SendNotifyMsg(0, i, DEF_NOTIFY_APOCGATEENDMSG, 0, 0, 0, 0);
			if ((m_pClientList[i]->m_iAdminUserLevel == 0)
				&& (m_pMapList[m_pClientList[i]->m_cMapIndex]->m_bIsApocalypseMap == true))
			{	// forced recall
				SendNotifyMsg(0, i, DEF_NOTIFY_TOBERECALLED, 0, 0, 0, 0);
				RequestTeleportHandler(i, "0   ");
			}
		}
	}
	// restore normal spawn on apoc maps
	for (i = 0; i < DEF_MAXMAPS; i++)
	{
		if (m_pMapList[i] != 0)
		{
			if (m_pMapList[i]->m_iApocalypseMobGenType != 0)
				//1 for not respawn, 2 for Boss spawn.
			{
				m_pMapList[i]->m_iMaximumObject = m_pMapList[i]->m_iMaximumObjectDefault;
			}
			if (m_pMapList[i]->m_cDynamicGateType == 4)
			{
				m_pMapList[i]->m_cDynamicGateType = 3;
			}
			if (m_pMapList[i]->m_cDynamicGateType == 2)
			{
				GenerateSlime(i);	// to force the gate to close		
			}
			Open_EmptyMap_Gate(i);
		}
	}
	PutLogList("(!) Apocalypse Mode OFF.");
	PutLogList(" ");
}

void CGame::SendThunder(int iClient, short sX, short sY, short sV3, short sV4)
{
	char* cp, cData[100];
	DWORD* dwp;
	WORD* wp;
	short* sp;
	int iRet;
	ZeroMemory(cData, sizeof(cData));
	dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
	*dwp = MSGID_EVENT_COMMON;
	wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
	*wp = DEF_COMMONTYPE_MAGIC;
	cp = (char*)(cData + DEF_INDEX2_MSGTYPE + 2);
	sp = (short*)cp;
	*sp = sX;
	cp += 2;
	sp = (short*)cp;
	*sp = sY;
	cp += 2;
	sp = (short*)cp;
	*sp = sX;
	cp += 2;
	sp = (short*)cp;
	*sp = sY;
	cp += 2;
	sp = (short*)cp;
	*sp = sV3;
	cp += 2;
	sp = (short*)cp;
	*sp = sV4;
	cp += 2;
	iRet = m_pClientList[iClient]->m_pXSock->iSendMsg(cData, 18);
}
void CGame::DoAbaddonThunderDamageHandler(char cMapIndex)
{
	int iResult;
	int i;
	if (iDice(1, 15) != 13) return;
	DWORD dwTime = timeGetTime();
	for (i = 0; i < DEF_MAXCLIENTS; i++)
	{
		if (m_pClientList[i] != 0)
		{
			if ((memcmp(m_pClientList[i]->m_cMapName, "abaddon", 7) == 0)
				|| (m_pClientList[i]->m_cMapIndex == cMapIndex))
			{
				switch (iDice(1, 4)) {
				case 1:
					SendThunder(i, m_pClientList[i]->m_sX, m_pClientList[i]->m_sY, 203, m_pClientList[i]->m_sType);
					SendThunder(i, m_pClientList[i]->m_sX - iDice(1, 7), m_pClientList[i]->m_sY + iDice(1, 5), 161, m_pClientList[i]->m_sType);
					break;
				case 2:
					SendThunder(i, m_pClientList[i]->m_sX, m_pClientList[i]->m_sY, 202, m_pClientList[i]->m_sType);
					SendThunder(i, m_pClientList[i]->m_sX + iDice(1, 7), m_pClientList[i]->m_sY - iDice(1, 5), 161, m_pClientList[i]->m_sType);
					break;
				case 3:
					SendThunder(i, m_pClientList[i]->m_sX, m_pClientList[i]->m_sY, 201, m_pClientList[i]->m_sType);
					SendThunder(i, m_pClientList[i]->m_sX + iDice(1, 7), m_pClientList[i]->m_sY + 5 - iDice(1, 9), 161, m_pClientList[i]->m_sType);
					break;
				case 4:
					SendThunder(i, m_pClientList[i]->m_sX, m_pClientList[i]->m_sY, 200, m_pClientList[i]->m_sType);
					SendThunder(i, m_pClientList[i]->m_sX + 5 - iDice(1, 9), m_pClientList[i]->m_sY + 7 - iDice(1, 4), 161, m_pClientList[i]->m_sType);
					break;
				}
				if (m_pClientList[i]->m_iAdminUserLevel > 0) continue;
				if (m_pClientList[i]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 5) continue;
				iResult = iDice(1, 20) + 100;
				if (m_pClientList[i]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 2)
				{
					iResult /= 2;
				}
				// Not for v3.51: DEF_NOTIFY_0BE5
				m_pClientList[i]->m_iHP -= iResult;
				if (m_pClientList[i]->m_iHP <= 0)
				{
					ClientKilledHandler(i, 0, 0, iResult);
				}
				else if (iResult > 0)
				{
					m_pClientList[i]->m_dwRecentAttackTime = dwTime;
					SendNotifyMsg(0, i, DEF_NOTIFY_HP, 0, 0, 0, 0);
					SendEventToNearClient_TypeA(i, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iResult, 0, 0);
					if (m_pClientList[i]->m_bSkillUsingStatus[19] != true)
					{
						m_pMapList[m_pClientList[i]->m_cMapIndex]->ClearOwner(0, i, DEF_OWNERTYPE_PLAYER, m_pClientList[i]->m_sX, m_pClientList[i]->m_sY);
						m_pMapList[m_pClientList[i]->m_cMapIndex]->SetOwner(i, DEF_OWNERTYPE_PLAYER, m_pClientList[i]->m_sX, m_pClientList[i]->m_sY);
					}
					if (m_pClientList[i]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] == 1)	// Hold person
					{
						SendNotifyMsg(0, i, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_HOLDOBJECT, 1, 0, 0);
						bRemoveFromDelayEventList(i, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_HOLDOBJECT);
						m_pClientList[i]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 0;
					}
					else if (m_pClientList[i]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] == 2)	// Para entangle
					{
						SendNotifyMsg(0, i, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_HOLDOBJECT, 2, 0, 0);
						bRemoveFromDelayEventList(i, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_HOLDOBJECT);
						m_pClientList[i]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 0;
					}
				}
			}
		}
	}
}

bool CGame::MobBossGenerator(int iMapIndex)
{
	char cNpcName[21], cName[11], cSA, cNpcWaypointIndex[11];
	int iProbSA, iKindSA, dX, dY, iNamingValue;
	bool bFirmBerserk, bRet;

	if (m_pMapList[iMapIndex]->m_bIsBossInMap == true) return false;
	iNamingValue = m_pMapList[iMapIndex]->iGetEmptyNamingValue();

	if (iNamingValue != -1) {

		ZeroMemory(cName, sizeof(cName));
		switch (m_pMapList[iMapIndex]->m_iApocalypseBossMobNpcID) {
		case 10:  strcpy(cNpcName, "Slime");				iProbSA = 5;  iKindSA = 1;  break;
		case 16:  strcpy(cNpcName, "Giant-Ant");			iProbSA = 10; iKindSA = 2;  break;
		case 14:  strcpy(cNpcName, "Orc");				iProbSA = 15; iKindSA = 1;  break;
		case 18:  strcpy(cNpcName, "Zombie");			iProbSA = 15; iKindSA = 3;  break;
		case 11:  strcpy(cNpcName, "Skeleton");			iProbSA = 35; iKindSA = 8;  break;
		case 6:   strcpy(cNpcName, "Orc-Mage");			iProbSA = 30; iKindSA = 7;  break;
		case 17:  strcpy(cNpcName, "Scorpion");			iProbSA = 15; iKindSA = 3;  break;
		case 12:  strcpy(cNpcName, "Stone-Golem");		iProbSA = 25; iKindSA = 5;  break;
		case 13:  strcpy(cNpcName, "Cyclops");			iProbSA = 35; iKindSA = 8;  break;
		case 22:  strcpy(cNpcName, "Amphis");			iProbSA = 20; iKindSA = 3;  break;
		case 23:  strcpy(cNpcName, "Clay-Golem");		iProbSA = 20; iKindSA = 5;  break;
		case 24:  strcpy(cNpcName, "Guard-Aresden");		iProbSA = 1; iKindSA = 10;  break;
		case 25:  strcpy(cNpcName, "Guard-Elvine");		iProbSA = 1; iKindSA = 10;  break;
		case 26:  strcpy(cNpcName, "Guard-Neutral");		iProbSA = 1; iKindSA = 10;  break;
		case 27:  strcpy(cNpcName, "Hellbound");			iProbSA = 20; iKindSA = 1;  break;
		case 29:  strcpy(cNpcName, "Orge");              iProbSA = 20; iKindSA = 1;  break;
		case 30:  strcpy(cNpcName, "Liche");				iProbSA = 30; iKindSA = 8;  break;
		case 31:  strcpy(cNpcName, "Demon");				iProbSA = 20; iKindSA = 8;  break;
		case 32:  strcpy(cNpcName, "Unicorn");			iProbSA = 35; iKindSA = 7;  break;
		case 33:  strcpy(cNpcName, "WereWolf");			iProbSA = 25; iKindSA = 1;  break;
		case 34:  strcpy(cNpcName, "Dummy");				iProbSA = 5;  iKindSA = 1;  break;
		case 35:  strcpy(cNpcName, "Attack-Dummy");		iProbSA = 5;  iKindSA = 1;  break;
		case 48:  strcpy(cNpcName, "Stalker");			iProbSA = 20; iKindSA = 3;  break;
		case 49:  strcpy(cNpcName, "Hellclaw");			iProbSA = 20; iKindSA = 8;  break;
		case 50:  strcpy(cNpcName, "Tigerworm");			iProbSA = 20; iKindSA = 8;  break;
		case 54:  strcpy(cNpcName, "Dark-Elf");			iProbSA = 20; iKindSA = 8;  break;
		case 53:  strcpy(cNpcName, "Beholder");			iProbSA = 20; iKindSA = 8;  break;
		case 52:  strcpy(cNpcName, "Gagoyle");			iProbSA = 20; iKindSA = 8;  break;
		case 57:  strcpy(cNpcName, "Giant-Frog");		iProbSA = 10; iKindSA = 2;  break;
		case 58:  strcpy(cNpcName, "Mountain-Giant");	iProbSA = 25; iKindSA = 1;  break;
		case 59:  strcpy(cNpcName, "Ettin");				iProbSA = 20; iKindSA = 8;  break;
		case 60:  strcpy(cNpcName, "Cannibal-Plant");	iProbSA = 20; iKindSA = 5;  break;
		case 61:  strcpy(cNpcName, "Rudolph");			iProbSA = 20; iKindSA = 1;  break;
		case 62:  strcpy(cNpcName, "DireBoar");			iProbSA = 20; iKindSA = 1;  break;
		case 63:  strcpy(cNpcName, "Frost");				iProbSA = 20; iKindSA = 8;  break;
		case 65:  strcpy(cNpcName, "Ice-Golem");			iProbSA = 20; iKindSA = 8;  break;
		case 66:  strcpy(cNpcName, "Wyvern");			iProbSA = 20; iKindSA = 9;  break;
		case 55:  strcpy(cNpcName, "Rabbit");			iProbSA = 20; iKindSA = 1;  break;
		case 67:  strcpy(cNpcName, "McGaffin");			iProbSA = 20; iKindSA = 1;  break;
		case 68:  strcpy(cNpcName, "Perry");				iProbSA = 20; iKindSA = 1;  break;
		case 69:  strcpy(cNpcName, "Devlin");			iProbSA = 20; iKindSA = 1;  break;
			// Ice-Wyvern
		case 73:  strcpy(cNpcName, "Fire-Wyvern");		iProbSA = 20; iKindSA = 9;  break;
		case 70:  strcpy(cNpcName, "Barlog");			iProbSA = 20; iKindSA = 1;  break;
		case 80:  strcpy(cNpcName, "Tentocle");			iProbSA = 20; iKindSA = 1;  break;
		case 71:  strcpy(cNpcName, "Centaurus");			iProbSA = 20; iKindSA = 1;  break;
		case 75:  strcpy(cNpcName, "Giant-Lizard");		iProbSA = 20; iKindSA = 1;  break;
		case 78:  strcpy(cNpcName, "Minotaurs");			iProbSA = 20; iKindSA = 1;  break;
		case 81:  strcpy(cNpcName, "Abaddon");			iProbSA = 20; iKindSA = 9;  break;
		case 72:  strcpy(cNpcName, "Claw-Turtle");		iProbSA = 20; iKindSA = 1;  break;
		case 74:  strcpy(cNpcName, "Giant-Crayfish");	iProbSA = 20; iKindSA = 1;  break;
		case 76:  strcpy(cNpcName, "Giant-Plant");		iProbSA = 20; iKindSA = 1;  break;
		case 77:  strcpy(cNpcName, "MasterMage-Orc");	iProbSA = 20; iKindSA = 1;  break;
		case 79:  strcpy(cNpcName, "Nizie");				iProbSA = 20; iKindSA = 1;  break;
		//Magn0S:: New Npc's
		case 92:  strcpy(cNpcName, "Eternal-Dragon");			iProbSA = 20; iKindSA = 9;  break;
		case 93:  strcpy(cNpcName, "Black-Demon");			iProbSA = 20; iKindSA = 9;  break;
		case 94:  strcpy(cNpcName, "Black-Wyvern");			iProbSA = 20; iKindSA = 9;  break;
		case 95:  strcpy(cNpcName, "Light-Wyvern");			iProbSA = 20; iKindSA = 9;  break;
		case 96:  strcpy(cNpcName, "Poison-Wyvern");			iProbSA = 20; iKindSA = 9;  break;
		case 97:  strcpy(cNpcName, "Heaven-Wyvern");			iProbSA = 20; iKindSA = 9;  break;
		case 98:  strcpy(cNpcName, "Illusion-Wyvern");			iProbSA = 20; iKindSA = 9;  break;
		case 99:  strcpy(cNpcName, "Ghost-Abaddon");			iProbSA = 20; iKindSA = 9;  break;
		default:  strcpy(cNpcName, "Orc");				iProbSA = 15; iKindSA = 1; 	break;
		}
		cSA = _cGetSpecialAbility(iKindSA);
		bFirmBerserk = true;
		ZeroMemory(cName, sizeof(cName));
		wsprintf(cName, "XX%d", iNamingValue);
		cName[0] = 95;
		cName[1] = iMapIndex + 65; // centu - replaced 'i' value
		bRet = bCreateNewNpc(cNpcName, cName, m_pMapList[iMapIndex]->m_cName, 53, cSA, DEF_MOVETYPE_RANDOMAREA, &dX, &dY, cNpcWaypointIndex, &m_pMapList[iMapIndex]->m_rcApocalypseBossMobRect, 0, -1, false, false, bFirmBerserk, false, false);
		if (bRet == false) {
			m_pMapList[iMapIndex]->SetNamingValueEmpty(iNamingValue);
			return false;
		}
		else {
			m_pNpcList[bRet]->m_bIsBossMob = true;
			m_pMapList[iMapIndex]->m_bIsBossInMap = true;
			return true;
		}
	}
	return false;
}

//**************************************************************************************
// SNOOPY: Revamped Apocalypse functions:
//         OpenCloseApocalypseGate 
//         Called by ::ForceOpen_ApocalypseGate, ::ForceClose_ApocalypseGate
//         Called by ::LocalStartApocalypse
//         Check if Gates's needs to be openned or closed 
//		   then notify any change to all clients.on current server
//		   NB: Opens/closes only type 1 Gates
//**************************************************************************************
void CGame::OpenCloseApocalypseGate()
{
	DWORD  dwTime = timeGetTime();
	bool bIsOpen = m_bIsApocalypseGateOpen;
	if ((dwTime >= m_dwApocalypseGateOpenTime)
		&& (dwTime < m_dwApocalypseGateCloseTime))
	{
		m_bIsApocalypseGateOpen = true;
	}
	else
	{
		// centu - automated end in 3hs
		m_bIsApocalypseGateOpen = false;
		GlobalEndApocalypseMode(0);
	}

	// If nothing has changed return...
	if (bIsOpen == m_bIsApocalypseGateOpen) return;

	/*if (m_bIsApocalypseGateOpen == true)
	{
		wsprintf(G_cTxt, "(!)Apocalypse Gate opened.");
	}
	else
	{
		wsprintf(G_cTxt, "(!)Apocalypse Gate closed.");
	}
	PutLogList(G_cTxt);
	PutLogEventFileList(G_cTxt);*/
	// Then notify all clients of change,
	int i;
	for (i = 0; i < DEF_MAXCLIENTS; i++)
	{
		if (m_pClientList[i] != 0)
		{
			Notify_ApocalypseGateState(i);
		}
	}
}

//**************************************************************************************
// SNOOPY: Revamped Apocalypse functions: (used also for Admin Gate)
//         Notify_ApocalypseGateState 
//         Called by ::RequestInitDataHandler (when client log in)
//         Called by ::RequestTeleportHandler (when client changes map on same server)
//         Called by ::OpenCloseApocalypseGate (when Gate is openned or closed)
//         Notify the client of Gate's state, needed to show in player's screen
//**************************************************************************************
void CGame::Notify_ApocalypseGateState(int iClientH)
{
	if (iClientH <= 0)	return;
	if (m_pClientList[iClientH] <= 0)	return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	int iMapIndex = m_pClientList[iClientH]->m_cMapIndex;
	int gX, gY;
	switch (m_pMapList[iMapIndex]->m_cDynamicGateType) {
	case 0:	// No gate on current map		
		if (m_bIsApocalypseMode == false) break;
		if (m_bIsApocalypseGateOpen == true)
		{
			SendNotifyMsg(0, iClientH, DEF_NOTIFY_APOCGATEOPEN, 0, 0, 0, m_pClientList[iClientH]->m_cMapName);
			SendNotifyMsg(0, iClientH, DEF_NOTIFY_APOCGATESTARTMSG, 0, 0, 0, 0);
		}
		else
		{
			SendNotifyMsg(0, iClientH, DEF_NOTIFY_APOCGATECLOSE, 0, 0, 0, m_pClientList[iClientH]->m_cMapName);
			SendNotifyMsg(0, iClientH, DEF_NOTIFY_APOCGATEENDMSG, 0, 0, 0, 0);
		}
		break;
	case 1:	// Initial Dynamic gates (toh, IB, D4...)
		if (m_bIsApocalypseMode == false) break;
		gX = (m_pMapList[iMapIndex]->m_sDynamicGateCoordRectX1 + m_pMapList[iMapIndex]->m_sDynamicGateCoordRectX2) / 2;
		gY = (m_pMapList[iMapIndex]->m_sDynamicGateCoordRectY1 + m_pMapList[iMapIndex]->m_sDynamicGateCoordRectY2) / 2;
		if (m_bIsApocalypseGateOpen == true)
		{
			SendNotifyMsg(0, iClientH, DEF_NOTIFY_APOCGATEOPEN, gX, gY, 0, m_pClientList[iClientH]->m_cMapName);
			SendNotifyMsg(0, iClientH, DEF_NOTIFY_APOCGATESTARTMSG, 0, 0, 0, 0);
		}
		else
		{
			SendNotifyMsg(0, iClientH, DEF_NOTIFY_APOCGATECLOSE, gX, gY, 0, m_pClientList[iClientH]->m_cMapName);
			SendNotifyMsg(0, iClientH, DEF_NOTIFY_APOCGATEENDMSG, 0, 0, 0, 0);
		}
		break;
	case 2: // Empty maps Dynamic gates (inferniaA, inferniaB, procella...)
		if (m_bIsApocalypseMode == false) break;
		gX = (m_pMapList[iMapIndex]->m_sDynamicGateCoordRectX1 + m_pMapList[iMapIndex]->m_sDynamicGateCoordRectX2) / 2;
		gY = (m_pMapList[iMapIndex]->m_sDynamicGateCoordRectY1 + m_pMapList[iMapIndex]->m_sDynamicGateCoordRectY2) / 2;
		if (m_pMapList[iMapIndex]->m_iTotalActiveObject == 0)
		{
			SendNotifyMsg(0, iClientH, DEF_NOTIFY_APOCGATEOPEN, gX, gY, 0, m_pClientList[iClientH]->m_cMapName);
			SendNotifyMsg(0, iClientH, DEF_NOTIFY_APOCGATESTARTMSG, 0, 0, 0, 0);
		}
		else
		{
			SendNotifyMsg(0, iClientH, DEF_NOTIFY_APOCGATECLOSE, gX, gY, 0, m_pClientList[iClientH]->m_cMapName);
		}
		break;
	case 3: // Hide the n°3 gates ..
		if (m_bIsApocalypseMode == false) break;
		gX = (m_pMapList[iMapIndex]->m_sDynamicGateCoordRectX1 + m_pMapList[iMapIndex]->m_sDynamicGateCoordRectX2) / 2;
		gY = (m_pMapList[iMapIndex]->m_sDynamicGateCoordRectY1 + m_pMapList[iMapIndex]->m_sDynamicGateCoordRectY2) / 2;
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_APOCGATECLOSE, gX, gY, 0, m_pClientList[iClientH]->m_cMapName);
		break;
	case 4: // Show the gate on abaddon map..when Abaddon spawning.
		if (m_bIsApocalypseMode == false) break;
		gX = (m_pMapList[iMapIndex]->m_sDynamicGateCoordRectX1 + m_pMapList[iMapIndex]->m_sDynamicGateCoordRectX2) / 2;
		gY = (m_pMapList[iMapIndex]->m_sDynamicGateCoordRectY1 + m_pMapList[iMapIndex]->m_sDynamicGateCoordRectY2) / 2;
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_APOCGATEOPEN, gX, gY, 0, m_pClientList[iClientH]->m_cMapName);
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_APOCGATESTARTMSG, 0, 0, 0, 0);
		break;
	case 5:	// GM command Dynamic gates (even not in apocalypse mode)
		gX = m_pMapList[iMapIndex]->m_sDynamicGateCoordRectX1 + 1;
		gY = m_pMapList[iMapIndex]->m_sDynamicGateCoordRectY1 + 1;
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_APOCGATEOPEN, gX, gY, 0, m_pClientList[iClientH]->m_cMapName);
		break;
	}
}

//**************************************************************************************
// SNOOPY: Revamped Apocalypse functions: (used also for Admin Gate)
//         Use_ApocalypseGate 
//         Called by ::CheckClientResponseTime()
//         Test if client uses Dynamic Gate, then TP him. used as well out of apoc mode (gate 5)
//**************************************************************************************
void CGame::Use_ApocalypseGate(int iClientH)
{
	if (iClientH <= 0)	return;
	if (m_pClientList[iClientH] == 0) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	int iMapIndex = m_pClientList[iClientH]->m_cMapIndex;
	switch (m_pMapList[iMapIndex]->m_cDynamicGateType) {
	case 0: // No Dynamic Gate on this map
		break;
	case 1: // Apocalypse normal gate
		if (m_bIsApocalypseMode == false)		return;
		if (m_bIsApocalypseGateOpen == false)	return;
		break;
	case 2: // Apocalypse gate opened when map empty
	case 3: // Apocalypse gate still closed
		if (m_bIsApocalypseMode == false)		return;
		if (m_pMapList[iMapIndex]->m_iTotalActiveObject != 0) return; // centu - tp when procella/abaddon is empty
		break;
	case 4: // Apocalypse gate opened when Abaddon spawned.
	case 5: // Admin created gate
		if (m_bIsApocalypseMode == false)		return;
		break;
	}
	if ((m_pClientList[iClientH]->m_sX >= m_pMapList[iMapIndex]->m_sDynamicGateCoordRectX1)
		&& (m_pClientList[iClientH]->m_sX <= m_pMapList[iMapIndex]->m_sDynamicGateCoordRectX2)
		&& (m_pClientList[iClientH]->m_sY >= m_pMapList[iMapIndex]->m_sDynamicGateCoordRectY1)
		&& (m_pClientList[iClientH]->m_sY <= m_pMapList[iMapIndex]->m_sDynamicGateCoordRectY2))
	{
		RequestTeleportHandler(iClientH, "2   "
			, m_pMapList[iMapIndex]->m_cDynamicGateCoordDestMap
			, m_pMapList[iMapIndex]->m_sDynamicGateCoordTgtX
			, m_pMapList[iMapIndex]->m_sDynamicGateCoordTgtY);
	}
}

//**************************************************************************************
// SNOOPY: Revamped Apocalypse functions:
//         Open_EmptyMap_Gate 
//		   Called by ::DeleteNpc when last npc killed on map disapears
//         Show just-openned gates to the client
//**************************************************************************************
void CGame::Open_EmptyMap_Gate(int MapIndex)
{
	if (m_pMapList[MapIndex]->m_cDynamicGateType < 2)	return;

	int i;
	for (i = 0; i < DEF_MAXCLIENTS; i++)
	{
		if (m_pClientList[i] == 0) continue;
		if (m_pClientList[i]->m_cMapIndex != MapIndex) continue;
		Notify_ApocalypseGateState(i);
	}
}

void CGame::minimap_clear_apoc(int client)
{
	auto p = m_pNpcList[client];
	if (!p) return;

	if (m_bIsApocalypseMode)
	{
		if (string(m_pMapList[p->m_cMapIndex]->m_cName) == "procella" ||
			string(m_pMapList[p->m_cMapIndex]->m_cName) == "abaddon")
		{
			char cData[56];
			DWORD* dwp;
			WORD* wp;
			char* cp;
			int* ip;

			dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);

			*wp = MINIMAPRED_CLEAR;
			
			cp = (char*)(cData + DEF_INDEX2_MSGTYPE + 2);
			ip = (int*)cp;
			*ip = client;
			cp += 4;

			for (int i = 0; i < DEF_MAXCLIENTS; i++)
			{
				auto pi = m_pClientList[i];

				if (!pi) continue;

				if (pi->m_cMapIndex == -1 || pi->m_cMapIndex != p->m_cMapIndex)	continue;

				pi->m_pXSock->iSendMsg(cData, 10);
			}
		}
	}
}

void CGame::minimap_update_apoc(int client)
{
	auto p = m_pNpcList[client];
	if (!p) return;

	if (m_bIsApocalypseMode)
	{
		if (string(m_pMapList[p->m_cMapIndex]->m_cName) == "procella" ||
			string(m_pMapList[p->m_cMapIndex]->m_cName) == "abaddon")
		{
			char cData[56];
			DWORD* dwp;
			WORD* wp;
			char* cp;
			int* ip;
			short* sp;

			dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
			*wp = MINIMAPRED_UPDATE;
			
			cp = (char*)(cData + DEF_INDEX2_MSGTYPE + 2);
			ip = (int*)cp;
			*ip = client;
			cp += 4;

			sp = (short*)cp;
			*sp = p->m_sX;
			cp += 2;
			
			sp = (short*)cp;
			*sp = p->m_sY;
			cp += 2;

			for (int i = 0; i < DEF_MAXCLIENTS; i++)
			{
				auto pi = m_pClientList[i];

				if (!pi) continue;

				if (pi->m_cMapIndex == -1 || pi->m_cMapIndex != p->m_cMapIndex)	continue;

				pi->m_pXSock->iSendMsg(cData, 14);
			}
		}
	}
}

//**************************************************************************************
// SNOOPY: Revamped Apocalypse functions:
//         GenerateApocalypseBoss 
//		   Called by ::NpcKilledHandler
//         Creates the appropriate npc depending on map definition file
//         then change the Dynamic Gate type from 3 to 4 (to open it)
//**************************************************************************************
void CGame::GenerateApocalypseBoss(int MapIndex)
{
	if (m_pMapList[MapIndex]->m_iTotalActiveObject != 0) return;
	char cName[21], cNpcName[21], cNpcWaypoint[11];
	int x;
	int iNamingValue;
	ZeroMemory(cNpcWaypoint, sizeof(cNpcWaypoint));

	iNamingValue = m_pMapList[MapIndex]->iGetEmptyNamingValue();
	if (iNamingValue != -1)
	{
		ZeroMemory(cNpcName, sizeof(cNpcName));
		switch (m_pMapList[MapIndex]->m_iApocalypseBossMobNpcID) {
		case 0: break;
		case 49: strcpy(cNpcName, "Hellclaw");break;
		case 50: strcpy(cNpcName, "Tigerworm");break;
		case 66: strcpy(cNpcName, "Wyvern");break;
		case 73: strcpy(cNpcName, "Fire-Wyvern");break;
		case 81: strcpy(cNpcName, "Abaddon");break;
		case 99: strcpy(cNpcName, "Ghost-Abaddon");break;
		case 93: strcpy(cNpcName, "Black-Demon"); break;
		default: strcpy(cNpcName, "Demon"); break;
		}
		ZeroMemory(cName, sizeof(cName));
		wsprintf(cName, "XX%d", iNamingValue);
		cName[0] = '_';
		cName[1] = MapIndex + 65;
		if (bCreateNewNpc(cNpcName, cName, m_pMapList[MapIndex]->m_cName, 0, 0
			, DEF_MOVETYPE_RANDOMAREA, 0, 0, cNpcWaypoint, &m_pMapList[MapIndex]->m_sApocalypseBossMobRect, 0, -1, false, false, false, true) == false)
		{
			m_pMapList[MapIndex]->SetNamingValueEmpty(iNamingValue);
		}
		else
		{
			wsprintf(G_cTxt, "%s has spawned in %s"
				, cNpcName
				, m_pMapList[MapIndex]->m_cName);
			PutLogList(G_cTxt);
			PutLogFileList(G_cTxt);
			PutLogEventFileList(G_cTxt);
		}
		// Search npc ID
		/*for (i5 = 1; i5 < DEF_MAXNPCS; i5++)
		{
			if ((m_pNpcList[i5] != 0) && (memcmp(m_pNpcList[i5]->m_cName, cName, 5) == 0))
			{
				break;
			}
		}*/
		// Show Spawns on minimap, and tell everybody on Apocalypse server.					
		//DWORD wX = m_pNpcList[i5]->m_sX;
		//DWORD wY = m_pNpcList[i5]->m_sX;
		for (x = 0; x < DEF_MAXCLIENTS; x++)
			if (m_pClientList[x] != 0)
				
			{
				/*if (memcmp(m_pMapList[MapIndex]->m_cName, m_pMapList[m_pClientList[x]->m_cMapIndex]->m_cName, strlen(m_pMapList[MapIndex]->m_cName)) == 0)
				{
					SendNotifyMsg(0, x, DEF_NOTIFY_SPAWNEVENT, wX, wY, m_pMapList[MapIndex]->m_iApocalypseBossMobNpcID, 0, 0, 0);
				}*/
				// Tell everybody on this server if Abaddon has appeared
				if (m_pMapList[MapIndex]->m_iApocalypseBossMobNpcID == 99)
				{
					SendNotifyMsg(0, x, DEF_NOTIFY_IPACCOUNTINFO, 0, 0, 0, "Ghost Abaddon has appeared...");
					SendNotifyMsg(0, x, DEF_NOTIFY_ABBYAPPEAR, 0, 0, 0, 0);
				}
			}

		// Prepare Abaddon's death, and Apocalypse end.
		//if (m_pMapList[MapIndex]->m_iApocalypseBossMobNpcID == 99)
		//{	// Abaddon should die by himself		
		//	DWORD dwTime = timeGetTime();
		//	dwTime += 1000 * 60 * 1; // 1 minute
		//	bRegisterDelayEvent(DEF_DELAYEVENTTYPE_KILL_ABADDON, 0, dwTime, i5
		//		, DEF_OWNERTYPE_NPC, MapIndex, 0, 0, 0, 0, 0);
		//	dwTime = timeGetTime();
		//	dwTime += 1000 * 60 * 5; // 5 minutes
		//	bRegisterDelayEvent(DEF_DELAYEVENTTYPE_END_APOCALYPSE, 0, dwTime, 0
		//		, 0, MapIndex, 0, 0, 0, 0, 0);
		//}

		// Finally open the Exit Gate if type 3 & not 2
		// NB: if m_iApocalypseMobGenType 2 with GateType 2, need to Kill the boss to open the gate.
		if (m_pMapList[MapIndex]->m_cDynamicGateType == 3)
		{
			m_pMapList[MapIndex]->m_cDynamicGateType = 4;
			Open_EmptyMap_Gate(MapIndex);
		}
	}
}
//**************************************************************************************
// SNOOPY: Revamped Apocalypse functions:
//         GenerateSlime 
//         Generate a npc now to close the gate
//**************************************************************************************
void CGame::GenerateSlime(int MapIndex)
{
	if (m_pMapList[MapIndex]->m_iTotalActiveObject != 0) return;
	char cName[21], cNpcName[21], cNpcWaypoint[11];
	int iNamingValue;
	ZeroMemory(cNpcWaypoint, sizeof(cNpcWaypoint));

	iNamingValue = m_pMapList[MapIndex]->iGetEmptyNamingValue();
	if (iNamingValue != -1)

	{
		strcpy(cNpcName, "Slime");
		ZeroMemory(cName, sizeof(cName));
		wsprintf(cName, "XX%d", iNamingValue);
		cName[0] = '_';
		cName[1] = MapIndex + 65;
		if (bCreateNewNpc(cNpcName, cName, m_pMapList[MapIndex]->m_cName, 0, 0
			, DEF_MOVETYPE_RANDOMAREA, 0, 0, cNpcWaypoint, &m_pMapList[MapIndex]->m_sApocalypseBossMobRect, 0, 0, false, false, false, true) == false)
		{
			m_pMapList[MapIndex]->SetNamingValueEmpty(iNamingValue);
		}
	}
}

//**************************************************************************************
// SNOOPY: Revamped Apocalypse functions:
//         ForceOpen_ApocalypseGate 
//         Local execution off a global GM command to force open the gate
//			Beware this will affect only current server
//**************************************************************************************
void CGame::ForceOpen_ApocalypseGate()
{
	if (m_bIsApocalypseMode == false)	return;
	DWORD  dwTime = timeGetTime();
	m_dwApocalypseGateOpenTime = dwTime;
	m_dwApocalypseGateCloseTime = dwTime + 15 * 60 * 1000; // will close in 15 minutes
	OpenCloseApocalypseGate();
}
//**************************************************************************************
// SNOOPY: Revamped Apocalypse functions:
//         ForceClose_ApocalypseGate 
//         GM command to force open the gate
//         Beware this will affect only current server
//**************************************************************************************
void CGame::ForceClose_ApocalypseGate()
{
	if (m_bIsApocalypseMode == false)	return;
	//DWORD  dwTime = timeGetTime();
	m_dwApocalypseGateCloseTime = 0;//dwTime - 1;
	m_dwApocalypseGateOpenTime = 0;//dwTime - 100; // alreaddy closed
	OpenCloseApocalypseGate();
}