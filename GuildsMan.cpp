// GuildsMan.cpp: implementation of the CGuildsMan class.
//
//////////////////////////////////////////////////////////////////////

#include "GuildsMan.h"

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

#pragma warning (disable : 4996 4018)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuildsMan::CGuildsMan()
{
	ZeroMemory(m_cName, sizeof(m_cName));
}
												 
CGuildsMan::~CGuildsMan()
{

}

// New 07/05/2004
void CGame::RequestGuildNameHandler(int iClientH, int iObjectID, int iIndex)
{
	if (m_pClientList[iClientH] == 0) return;
	if ((iObjectID <= 0) || (iObjectID >= DEF_MAXCLIENTS)) return;

	if (m_pClientList[iObjectID] != 0) {
		// ¿äÃ» ¹ÞÀº Object°¡ ¾ø´Ù.

		SendNotifyMsg(0, iClientH, DEF_NOTIFY_REQGUILDNAMEANSWER, m_pClientList[iObjectID]->m_iGuildRank, iIndex, 0, m_pClientList[iObjectID]->m_cGuildName);

	}
}

int CGame::ObtenerX(char* pGuild)
{
	for (int i = 0; i < DEF_MAXGUILDS; i++)
	{
		if (string(m_stGuildInfo[i].cGuildName) == "NONE") continue;
		if (string(m_stGuildInfo[i].cGuildName) == pGuild)
		{
			return m_stGuildInfo[i].markX;
		}
	}
	return -1;
}
int CGame::ObtenerY(char* pGuild)
{
	for (int i = 0; i < DEF_MAXGUILDS; i++)
	{
		if (string(m_stGuildInfo[i].cGuildName) == "NONE") continue;
		if (string(m_stGuildInfo[i].cGuildName) == pGuild)
		{
			return m_stGuildInfo[i].markY;
		}
	}
	return -1;
}
int CGame::ObtenerID(char* pGuild)
{
	for (int i = 0; i < DEF_MAXGUILDS; i++)
	{
		if (string(m_stGuildInfo[i].cGuildName) == "NONE") continue;
		if (string(m_stGuildInfo[i].cGuildName) == pGuild)
		{
			return i;
		}
	}
	return -1;
}
int CGame::ObtenerNuevoID()
{
	for (int i = 0; i < DEF_MAXGUILDS; i++)
	{
		if (string(m_stGuild[i].cGuildName) == "NONE")
		{
			return i;
		}
	}
	return -1;
}

void CGame::RequestGuildMemberRank(int iClientH, char *pName, int iIndex)
{
	if (m_pClientList[iClientH] == 0) return;
	
	for (int i = 0; i < DEF_MAXCLIENTS; i++) {
		if (m_pClientList[i] != 0 && strcmp(m_pClientList[i]->m_cCharName, pName) == 0) {
			SendNotifyMsg(0, iClientH, DEF_NOTIFY_REQGUILDRANKANSWER, m_pClientList[i]->m_iGuildRank, iIndex, 0, m_pClientList[i]->m_cGuildName);
			break;
		}
	}
}

void CGame::PlayerCommandAddRank(int iClientH, char* pData, DWORD dwMsgSize, int iRank)
{
    char   seps[] = "= \t\n";
    char* token, cName[11], cTargetName[11], cBuff[256], cNpcName[21], cNpcWaypoint[11], cMsg[52];
    class  CStrTok* pStrTok;
    int i, iMajesticCount;

	DWORD dwGoldCount = dwGetItemCount(iClientH, "Gold");

    if (m_pClientList[iClientH] == 0) return;
    if ((dwMsgSize) <= 0) return;

    if (m_pClientList[iClientH]->m_iGuildRank != 0 && m_pClientList[iClientH]->m_iGuildRank != 3) {
        // ???? ??????? ?????* ?? ?????? ??????? ????.
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_NOGUILDMASTERLEVEL, 0, 0, 0, 0);
        return;
    }

	ZeroMemory(cMsg, sizeof(cMsg));
	switch (iRank)
	{
	case 1:
		wsprintf(cMsg, "You are Now a Recluiter");
		iMajesticCount = 60;
		break;
	case 2:
		wsprintf(cMsg, "You are Now a Summoner");
		iMajesticCount = 50;
		break;
	case 3:
		wsprintf(cMsg, "You are Now a Captain");
		iMajesticCount = 80;
		break;
	}

	if (m_pClientList[iClientH]->m_iGizonItemUpgradeLeft < iMajesticCount)
	{
		wsprintf(G_cTxt, "Not enough majestic. Required %d points.", iMajesticCount);
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, G_cTxt);
		return;
	}

    ZeroMemory(cName, sizeof(cName));
    ZeroMemory(cNpcWaypoint, sizeof(cNpcWaypoint));
    ZeroMemory(cTargetName, sizeof(cTargetName));
    ZeroMemory(cNpcName, sizeof(cNpcName));
    ZeroMemory(cBuff, sizeof(cBuff));
    memcpy(cBuff, pData, dwMsgSize);

    pStrTok = new class CStrTok(cBuff, seps);
    token = pStrTok->pGet();
    token = pStrTok->pGet();

    if (token != 0) {
        // token�� �� ä���� �Ұ����ϰ� ���� ����� �̸�
        if (strlen(token) > 10)
            memcpy(cTargetName, token, 10);
        else memcpy(cTargetName, token, strlen(token));

		if (dwGoldCount >= 1000000) {
			for (i = 0; i < DEF_MAXCLIENTS; i++) {
				if ((m_pClientList[i] != 0) && (memcmp(m_pClientList[i]->m_cCharName, cTargetName, 10) == 0)) {
					// ��ǥ ĳ���͸� ã�Ҵ�.     

					if (memcmp(m_pClientList[iClientH]->m_cGuildName, m_pClientList[i]->m_cGuildName, 21) != 0) {
						// ????? ???????? ???? ?????? ????????.

						SendNotifyMsg(0, iClientH, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "Error: Cannot Add Rank...");
						delete pStrTok;
						return;
					}

					if (m_pClientList[i]->m_iGuildRank == 0)
					{
						SendNotifyMsg(0, iClientH, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "Error: Cannot change a Guildmaster.");
						delete pStrTok;
						return;
					}
					SendNotifyMsg(0, i, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, cMsg);
					m_pClientList[i]->m_iGuildRank = iRank;
					SendNotifyMsg(0, i, DEF_UPDATE_GUILDRANK, m_pClientList[i]->m_iGuildRank, 0, 0, 0);
				}
			}
			m_pClientList[iClientH]->m_iGizonItemUpgradeLeft -= iMajesticCount;
			SendNotifyMsg(0, iClientH, DEF_NOTIFY_GIZONITEMUPGRADELEFT, m_pClientList[iClientH]->m_iGizonItemUpgradeLeft, 0, 0, 0);

			SetItemCount(iClientH, "Gold", dwGoldCount - 1000000);
			SendNotifyMsg(0, iClientH, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "New Rank Added");
		}
		else
		{
			SendNotifyMsg(0, iClientH, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "Not enough gold. Required 1kk of Gold.");
		}
    }

    delete pStrTok;
    
}

void CGame::PlayerOrder_DeleteRank(int iClientH, char* pData, DWORD dwMsgSize)
{
    char   seps[] = "= \t\n";
    char* token, cName[11], cTargetName[11], cBuff[256], cNpcName[21], cNpcWaypoint[11];
    class  CStrTok* pStrTok;
    int i;

    if (m_pClientList[iClientH] == 0) return;
    if ((dwMsgSize) <= 0) return;

    if (m_pClientList[iClientH]->m_iGuildRank != 0 && m_pClientList[iClientH]->m_iGuildRank != 3) {
        // ???? ??????? ?????* ?? ?????? ??????? ????.
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_NOGUILDMASTERLEVEL, 0, 0, 0, 0);
        return;
    }

    ZeroMemory(cName, sizeof(cName));
    ZeroMemory(cNpcWaypoint, sizeof(cNpcWaypoint));
    ZeroMemory(cTargetName, sizeof(cTargetName));
    ZeroMemory(cNpcName, sizeof(cNpcName));
    ZeroMemory(cBuff, sizeof(cBuff));
    memcpy(cBuff, pData, dwMsgSize);

    pStrTok = new class CStrTok(cBuff, seps);
    token = pStrTok->pGet();
    token = pStrTok->pGet();

    if (token != 0) {
        // token?�?� �� ?��?�?�?�� �?����?�?�ϰ?� ���� ���?�?� ?�?���
        if (strlen(token) > 10)
            memcpy(cTargetName, token, 10);
        else memcpy(cTargetName, token, strlen(token));

		for (i = 0; i < DEF_MAXCLIENTS; i++) {
			if ((m_pClientList[i] != 0) && (memcmp(m_pClientList[i]->m_cCharName, cTargetName, 10) == 0)) {
				// ��?�� ?����?�͸� ?���?��?�.     

				if (memcmp(m_pClientList[iClientH]->m_cGuildName, m_pClientList[i]->m_cGuildName, 21) != 0) {
					// ????? ???????? ???? ?????? ????????.

					SendNotifyMsg(0, iClientH, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "Error: Cannot Delete rank...");
					delete pStrTok;
					return;
				}
				if (m_pClientList[i]->m_iGuildRank == 0)
				{
					SendNotifyMsg(0, iClientH, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "Error: Cannot delete a Guildmaster.");
					delete pStrTok;
					return;
				}
				m_pClientList[i]->m_iGuildRank = 12;
				SendNotifyMsg(0, i, DEF_UPDATE_GUILDRANK, m_pClientList[i]->m_iGuildRank, 0, 0, 0);
				SendNotifyMsg(0, i, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "You are now a Guildsman");

			}
		}
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "Rank Deleted");
    }

    delete pStrTok;
    
}

void CGame::ResponseCreateNewGuildHandler(char* pData, DWORD dwMsgSize)
{
	int i;
	WORD* wp, wResult;
	DWORD* dwp;
	char* cp, cCharName[11], cData[100], cTxt[120];
	int iRet;

	// ·Î±× ¼­¹ö·ÎºÎÅÍ ±æµå »ý¼º ¿äÃ»¿¡ ´ëÇÑ ÀÀ´äµ¥ÀÌÅÍ°¡ µµÂøÇß´Ù. 
	ZeroMemory(cCharName, sizeof(cCharName));
	cp = (char*)(pData + DEF_INDEX2_MSGTYPE + 2);
	memcpy(cCharName, cp, 10);
	cp += 10;

	// ÀÌ¸§ÀÌ ÀÏÄ¡ÇÏ´Â Å¬¶óÀÌ¾ðÆ®¸¦ Ã£´Â´Ù.
	for (i = 0; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != 0) && (memcmp(m_pClientList[i]->m_cCharName, cCharName, 10) == 0) //&&
			//(m_pClientList[i]->m_iLevel >= 100) && (m_pClientList[i]->m_iCharisma >= 20) &&
			// centu - guild cost gold
			/*(dwGetItemCount(i, "Gold") >= m_iGuildCost)*/) {

			wp = (WORD*)(pData + DEF_INDEX2_MSGTYPE);
			switch (*wp) {
			case DEF_LOGRESMSGTYPE_CONFIRM:
				// Å¬¶óÀÌ¾ðÆ®ÀÇ ±æµå »ý¼º ¿ä±¸°¡ ¼º°øÇÏ¿´´Ù. 
				wResult = DEF_MSGTYPE_CONFIRM;
				// ±æµå ÀÌ¸§Àº ÀÌ¹Ì ÀúÀåµÇ¾î ÀÖÀ¸¹Ç·Î ·©Å©¸¦ 0À¸·Î ¹Ù²ã À¯È¿È­ ÇÑ´Ù.
				m_pClientList[i]->m_iGuildRank = 0;	// ±æµå ·©Å©´Â 0. ±æµå¸¶½ºÅÍÀÓ 
				wsprintf(cTxt, "(!) New guild(%s) creation success! : character(%s)", m_pClientList[i]->m_cGuildName, m_pClientList[i]->m_cCharName);
				PutLogList(cTxt);
				break;

			case DEF_LOGRESMSGTYPE_REJECT:
				// Å¬¶óÀÌ¾ðÆ®ÀÇ ±æµå »ý¼º ¿ä±¸°¡ ½ÇÆÐÇÏ¿´´Ù.
				// ÇØ´ç Å¬¶óÀÌ¾ðÆ®ÀÇ ±æµåÀÌ¸§À» ÃÊ±âÈ­ÇÑ´Ù "NONE".
				wResult = DEF_MSGTYPE_REJECT;
				ZeroMemory(m_pClientList[i]->m_cGuildName, sizeof(m_pClientList[i]->m_cGuildName));
				memcpy(m_pClientList[i]->m_cGuildName, "NONE", 4);
				m_pClientList[i]->m_iGuildRank = -1;
				m_pClientList[i]->m_iGuildGUID = -1;
				wsprintf(cTxt, "(!) New guild(%s) creation Fail! : character(%s)", m_pClientList[i]->m_cGuildName, m_pClientList[i]->m_cCharName);
				PutLogList(cTxt);
				break;
			}

			dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_RESPONSE_CREATENEWGUILD;
			wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
			*wp = wResult;

			// ±æµå »ý¼º ¿ä±¸ ÀÀ´ä ¸Þ½ÃÁö¸¦ Å¬¶óÀÌ¾ðÆ®¿¡°Ô Àü¼Û
			iRet = m_pClientList[i]->m_pXSock->iSendMsg(cData, 6);
			switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				// ¸Þ½ÃÁö¸¦ º¸³¾¶§ ¿¡·¯°¡ ¹ß»ýÇß´Ù¸é Á¦°ÅÇÑ´Ù.
				DeleteClient(i, true, true);
				break;
			}

			return;
		}

	// ÀÌ¸§ÀÌ ÀÏÄ¡ÇÏ´Â Å¬¶óÀÌ¾ðÆ®¸¦ Ã£À» ¼ö ¾ø´Ù.
	wsprintf(cTxt, "(!)Non-existing player data received from Log server(2): CharName(%s)", cCharName);
	PutLogList(cTxt);
}

void CGame::RequestCreateNewGuildHandler(int iClientH, char* pData, DWORD dwMsgSize)
{
	char* cp, cGuildName[21], cTxt[120], cData[100];
	DWORD* dwp;
	WORD* wp;
	int     iRet;
	SYSTEMTIME SysTime;
	
	if (m_pClientList[iClientH] == 0) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if (m_bIsCrusadeMode == true) return;
	if (m_bIsHeldenianMode == true) return;
	if (m_bIsApocalypseMode == true) return;

	if ((m_bAdminSecurity == true) && (m_pClientList[iClientH]->m_iAdminUserLevel > 0 && m_pClientList[iClientH]->m_iAdminUserLevel < 7)) return;

	cp = (char*)(pData + DEF_INDEX2_MSGTYPE + 2);
	cp += 30;

	ZeroMemory(cGuildName, sizeof(cGuildName));
	memcpy(cGuildName, cp, 20);
	cp += 20;

	DWORD dwGoldCount = dwGetItemCount(iClientH, "Gold");

	if (m_pClientList[iClientH]->m_iGuildRank != -1) 
	{
		wsprintf(cTxt, "(!)Cannot create guild! Already guild member.: CharName(%s)", m_pClientList[iClientH]->m_cCharName);
		PutLogList(cTxt);
	}
	else 
	{
		if ((m_pClientList[iClientH]->m_iLevel < 100) || //(m_pClientList[iClientH]->m_iCharisma < 20) ||
			(memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) ||
			(memcmp(m_pClientList[iClientH]->m_cLocation, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, 10) != 0))
		{ // v1.4
		   ZeroMemory(cData, sizeof(cData));

			dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_RESPONSE_CREATENEWGUILD;
			wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_REJECT;

			iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6);
			switch (iRet) 
			{
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				DeleteClient(iClientH, true, true);
				break;
			}
		}
		else 
		{
			if (dwGoldCount >= m_iGuildCost) 
			{
				ZeroMemory(m_pClientList[iClientH]->m_cGuildName, sizeof(m_pClientList[iClientH]->m_cGuildName));
				strcpy(m_pClientList[iClientH]->m_cGuildName, cGuildName);
				ZeroMemory(m_pClientList[iClientH]->m_cLocation, sizeof(m_pClientList[iClientH]->m_cLocation));
				strcpy(m_pClientList[iClientH]->m_cLocation, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName);
				
				SetItemCount(iClientH, "Gold", dwGoldCount - m_iGuildCost); // reduce gold by guildcost

				GetLocalTime(&SysTime);
				m_pClientList[iClientH]->m_iGuildGUID = (int)(SysTime.wYear + SysTime.wMonth + SysTime.wDay + SysTime.wHour + SysTime.wMinute + timeGetTime());

				bSendMsgToLS(MSGID_REQUEST_CREATENEWGUILD, iClientH);

				//_CreateNewGuildFile(cGuildName);
			}
			else 
			{
				wsprintf(cTxt, " Not enough gold. You need %d gold.", m_iGuildCost);
				SendNotifyMsg(iClientH, iClientH, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, cTxt);

				ZeroMemory(cData, sizeof(cData));

				dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
				*dwp = MSGID_RESPONSE_CREATENEWGUILD;
				wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
				*wp = DEF_MSGTYPE_REJECT;

				iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6);
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
	}
}

void CGame::_CreateNewGuildFile(char *cGuildName)
{
	char cTxt[256], cFileName[256], cDir[11], * pData, * cp;
	FILE* pFile;
	int    iSize, id = ObtenerNuevoID();

	m_stGuild[id].iGuildLevel = 0;
	strcpy(m_stGuild[id].cGuildName, cGuildName);

	pData = new char[30000];
	if (pData == 0) return;
	ZeroMemory(pData, 30000);

	cp = (char*)(pData);
	iSize = _iComposeGuildDataFileContents(id, cp);

	// Centuu - guild upgrade
	strcat(cFileName, "Guild");
	strcat(cFileName, "\\");
	strcat(cFileName, "\\");
	wsprintf(cTxt, "AscII%d", (unsigned char)cGuildName[0]);
	strcat(cFileName, cTxt);
	strcpy(cDir, cFileName);
	strcat(cFileName, "\\");
	strcat(cFileName, "\\");
	strcat(cFileName, cGuildName);
	strcat(cFileName, ".txt");

	_mkdir("Guild");
	_mkdir(cDir);

	if (iSize == 0) {
		PutLogList("(!) Guild data body empty: Cannot create & save temporal guild data file.");
		delete[] pData;
		return;
	}

	pFile = fopen(cFileName, "wt");
	if (pFile == 0) {
		wsprintf(cTxt, "(!) Cannot create new Guild file : Name(%s)", cFileName);
		PutLogList(cTxt);
		return;
	}
	
	wsprintf(cTxt, "(!) Guild data file saved : Name(%s)", cFileName);
	PutLogList(cTxt);
	fwrite(cp, iSize, 1, pFile);
	fclose(pFile);
	
}

int CGame::_iComposeGuildDataFileContents(int iGuildH, char* pData)
{
	SYSTEMTIME SysTime;
	char  cTxt[120], cTmp[21];
	int   i;

	GetLocalTime(&SysTime);
	strcat(pData, "[FILE-DATE]\n\n");

	wsprintf(cTxt, "file-saved-date: %d %d %d %d %d\n", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	strcat(pData, cTxt);
	strcat(pData, "\n\n");

	strcat(pData, "[LEVEL]\n\n");
	wsprintf(cTxt, "guild-level       = %d", m_stGuild[iGuildH].iGuildLevel);
	strcat(pData, cTxt);
	strcat(pData, "\n\n");
	
	strcat(pData, "[ITEMLIST]\n\n");

	for (i = 0; i < DEF_MAXBANKITEMS; i++) {
		if (m_stGuild[iGuildH].m_pItemInBankList[i] != 0) {
			strcat(pData, "guild-bank-item = ");
			memset(cTmp, ' ', 21);
			strcpy(cTmp, m_stGuild[iGuildH].m_pItemInBankList[i]->m_cName);
			cTmp[strlen(m_stGuild[iGuildH].m_pItemInBankList[i]->m_cName)] = (char)' ';
			cTmp[20] = 0;
			strcat(pData, cTmp);
			strcat(pData, " ");
			itoa(m_stGuild[iGuildH].m_pItemInBankList[i]->m_dwCount, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(m_stGuild[iGuildH].m_pItemInBankList[i]->m_sTouchEffectType, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(m_stGuild[iGuildH].m_pItemInBankList[i]->m_sTouchEffectValue1, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(m_stGuild[iGuildH].m_pItemInBankList[i]->m_sTouchEffectValue2, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(m_stGuild[iGuildH].m_pItemInBankList[i]->m_sTouchEffectValue3, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(m_stGuild[iGuildH].m_pItemInBankList[i]->m_cItemColor, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(m_stGuild[iGuildH].m_pItemInBankList[i]->m_sItemSpecEffectValue1, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(m_stGuild[iGuildH].m_pItemInBankList[i]->m_sItemSpecEffectValue2, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(m_stGuild[iGuildH].m_pItemInBankList[i]->m_sItemSpecEffectValue3, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(m_stGuild[iGuildH].m_pItemInBankList[i]->m_wCurLifeSpan, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(m_stGuild[iGuildH].m_pItemInBankList[i]->m_dwAttribute, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			//----------------------------------------------------------------------------
			itoa(m_stGuild[iGuildH].m_pItemInBankList[i]->m_sNewEffect1, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			//--------------------
			itoa(m_stGuild[iGuildH].m_pItemInBankList[i]->m_sNewEffect2, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			//--------------------
			itoa(m_stGuild[iGuildH].m_pItemInBankList[i]->m_sNewEffect3, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			//--------------------
			itoa(m_stGuild[iGuildH].m_pItemInBankList[i]->m_sNewEffect4, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, "\n");
		}
	}
	strcat(pData, "\n\n");

	strcat(pData, "[EOF]");
	strcat(pData, "\n\n\n\n");

	return strlen(pData);
}

void CGame::RequestDisbandGuildHandler(int iClientH, char* pData, DWORD dwMsgSize)
{
	char* cp, cGuildName[21], cTxt[120];

	if (m_bIsCrusadeMode == true) return;
	if (m_bIsHeldenianMode == true) return;
	if (m_bIsApocalypseMode == true) return;

	cp = (char*)(pData + DEF_INDEX2_MSGTYPE + 2);
	ZeroMemory(cGuildName, sizeof(cGuildName));

	cp += 30;

	memcpy(cGuildName, cp, 20);
	cp += 20;

	if ((m_pClientList[iClientH]->m_iGuildRank != 0) || (memcmp(m_pClientList[iClientH]->m_cGuildName, cGuildName, 20) != 0)) {
		// ±æµå¸¶½ºÅÍ°¡ ¾Æ´Ï°Å³ª ±æµåÀÇ ÀÌ¸§ÀÌ ´Ù¸£¹Ç·Î ±æµåÇØ»êÀÇ ±ÇÇÑÀÌ ¾ø´Ù.
		wsprintf(cTxt, "(!)Cannot Disband guild! Not guildmaster.: CharName(%s)", m_pClientList[iClientH]->m_cCharName);
		PutLogList(cTxt);
	}
	else {
		// ±æµå ÇØ»ê ¸Þ½ÃÁö¸¦ ·Î±×¼­¹ö·Î Àü¼ÛÇÑ´Ù.
		bSendMsgToLS(MSGID_REQUEST_DISBANDGUILD, iClientH);
	}
}

void CGame::ResponseDisbandGuildHandler(char* pData, DWORD dwMsgSize)
{
	int i;
	WORD* wp, wResult;
	DWORD* dwp;
	char* cp, cCharName[11], cData[100], cTxt[120];
	int iRet;

	// �α� �����κ��� ��� �ػ� ��û�� ���� ���䵥���Ͱ� �����ߴ�. 
	ZeroMemory(cCharName, sizeof(cCharName));
	cp = (char*)(pData + DEF_INDEX2_MSGTYPE + 2);
	memcpy(cCharName, cp, 10);
	cp += 10;

	// �̸��� ��ġ�ϴ� Ŭ���̾�Ʈ�� ã�´�.
	for (i = 0; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != 0) && (memcmp(m_pClientList[i]->m_cCharName, cCharName, 10) == 0)) {

			wp = (WORD*)(pData + DEF_INDEX2_MSGTYPE);
			switch (*wp) {
			case DEF_LOGRESMSGTYPE_CONFIRM:
				// Ŭ���̾�Ʈ�� ��� �ػ� �䱸�� �����Ͽ���. 
				wResult = DEF_MSGTYPE_CONFIRM;
				wsprintf(cTxt, "(!) Disband guild(%s) success! : character(%s)", m_pClientList[i]->m_cGuildName, m_pClientList[i]->m_cCharName);
				PutLogList(cTxt);

				// �������� �����鿡�� ��尡 �ػ�Ǿ����� �˸��� �޽����� �����Ѵ�. 
				SendGuildMsg(i, DEF_NOTIFY_GUILDDISBANDED, 0, 0, 0);

				// ����̸� Ŭ����
				ZeroMemory(m_pClientList[i]->m_cGuildName, sizeof(m_pClientList[i]->m_cGuildName));
				memcpy(m_pClientList[i]->m_cGuildName, "NONE", 4);
				m_pClientList[i]->m_iGuildRank = -1;		// ��� ��ũ�� -1. ������ �ƴϴ�. 
				m_pClientList[i]->m_iGuildGUID = -1;
				break;

			case DEF_LOGRESMSGTYPE_REJECT:
				// Ŭ���̾�Ʈ�� ��� �ػ� �䱸�� �����Ͽ���.
				wResult = DEF_MSGTYPE_REJECT;
				wsprintf(cTxt, "(!) Disband guild(%s) Fail! : character(%s)", m_pClientList[i]->m_cGuildName, m_pClientList[i]->m_cCharName);
				PutLogList(cTxt);
				break;
			}

			dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_RESPONSE_DISBANDGUILD;
			wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
			*wp = wResult;

			// ��� �ػ� �䱸 ���� �޽����� Ŭ���̾�Ʈ���� ����
			iRet = m_pClientList[i]->m_pXSock->iSendMsg(cData, 6);
			switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				// �޽����� ������ ������ �߻��ߴٸ� �����Ѵ�.
				DeleteClient(i, true, true);
				break;
			}
			return;
		}

	// �̸��� ��ġ�ϴ� Ŭ���̾�Ʈ�� ã�� �� ����.
	wsprintf(cTxt, "(!)Non-existing player data received from Log server(2): CharName(%s)", cCharName);
	PutLogList(cTxt);
}

void CGame::JoinGuildApproveHandler(int iClientH, char* pName)
{
	int i;
	bool bIsExist = false;

	if (m_pClientList[iClientH] == 0) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

	// pNameÀ» °®´Â Å¬¶óÀÌ¾ðÆ®ÀÇ iClientH ±æµå¿¡ ´ëÇÑ °¡ÀÔ¿ä±¸°¡ ¼º°øÇÏ¿´´Ù.

	// pNameÀÇ ÀÌ¸§À» °®´Â Å¬¶óÀÌ¾ðÆ® ±¸Á¶Ã¼¸¦ °Ë»öÇÑ´Ù.
	for (i = 0; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != 0) && (memcmp(m_pClientList[i]->m_cCharName, pName, 10) == 0)) {
			// v1.4 ¼Ò¼Ó ¸¶À»ÀÌ ´Þ¶óµµ ¹«½ÃµÈ´Ù.
			if (memcmp(m_pClientList[i]->m_cLocation, m_pClientList[iClientH]->m_cLocation, 10) != 0) return;

			// ±æµåÀÇ ÀÌ¸§À» º¹»çÇÏ°í ¼öÄ¡¸¦ ÃÊ±âÈ­ÇØ ÁØ´Ù.
			ZeroMemory(m_pClientList[i]->m_cGuildName, sizeof(m_pClientList[i]->m_cGuildName));
			strcpy(m_pClientList[i]->m_cGuildName, m_pClientList[iClientH]->m_cGuildName);

			// ±æµå GUIDº¹»çÇÑ´Ù.
			m_pClientList[i]->m_iGuildGUID = m_pClientList[iClientH]->m_iGuildGUID;

			// ±æµåÀÇ »ý¼ºÀ§Ä¡¸¦ ÃÊ±âÈ­ÇÑ´Ù.
			ZeroMemory(m_pClientList[i]->m_cLocation, sizeof(m_pClientList[i]->m_cLocation));
			strcpy(m_pClientList[i]->m_cLocation, m_pClientList[iClientH]->m_cLocation);

			m_pClientList[i]->m_iGuildRank = DEF_GUILDSTARTRANK; //@@@  GuildRankÀÇ ½ÃÀÛÀº DEF_GUILDSTARTRANK

			// °¡ÀÔ ½ÅÃ»ÀÚ¿¡°Ô °¡ÀÔÀÌ ¼º°øÇßÀ½À» ¾Ë¸®´Â ¸Þ½ÃÁö¸¦ º¸³»ÁØ´Ù.
			SendNotifyMsg(0, i, DEF_COMMONTYPE_JOINGUILDAPPROVE, 0, 0, 0, 0);

			// Æ¯¼ºÀÌ ¹Ù²î¹Ç·Î ¿Ü¾çÀ» »õ·Î º¸³½´Ù. 
			SendEventToNearClient_TypeA(i, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0); // Centuu - iClientH -> i

			// ´Ù¸¥ ±æµå¿øµé¿¡°Ô »õ ±æµå¿øÀÌ ÀÖÀ½À» ¾Ë¸°´Ù.
			SendGuildMsg(i, DEF_NOTIFY_NEWGUILDSMAN, 0, 0, 0);

			// ±æµåÁ¤º¸È­ÀÏ¿¡ »õ ±æµå¿øÀÇ ÀÌ¸§À» ±â·ÏÇÑ´Ù.
			bSendMsgToLS(MSGID_REQUEST_UPDATEGUILDINFO_NEWGUILDSMAN, i);
			break;
		}

	// °¡ÀÔÀ» ½ÅÃ»ÇÑ Å¬¶óÀÌ¾ðÆ®¸¦ Ã£À»¼ö ¾ø´Ù.(Á¢¼ÓÀÌ ±×»çÀÌ ²÷°å´Ù´øÁö) ¹«È¿ÀÓ 
}

void CGame::JoinGuildRejectHandler(int iClientH, char* pName)
{
	int i;

	if (m_pClientList[iClientH] == 0) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

	// pNameÀ» °®´Â Å¬¶óÀÌ¾ðÆ®ÀÇ iClientH ±æµå¿¡ ´ëÇÑ °¡ÀÔ ¿ä±¸°¡ ½ÇÆÐ ÇÏ¿´´Ù.

	// pNameÀÇ ÀÌ¸§À» °®´Â Å¬¶óÀÌ¾ðÆ® ±¸Á¶Ã¼¸¦ °Ë»öÇÑ´Ù.
	for (i = 0; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != 0) && (memcmp(m_pClientList[i]->m_cCharName, pName, 10) == 0)) {

			// °¡ÀÔ ½ÅÃ»ÀÚ¿¡°Ô °¡ÀÔÀÌ ½ÇÆÐÇßÀ½À» ¾Ë¸®´Â ¸Þ½ÃÁö¸¦ º¸³»ÁØ´Ù.
			SendNotifyMsg(0, i, DEF_COMMONTYPE_JOINGUILDREJECT, 0, 0, 0, 0);
			break;
		}

	// °¡ÀÔÀ» ½ÅÃ»ÇÑ Å¬¶óÀÌ¾ðÆ®¸¦ Ã£À»¼ö ¾ø´Ù.(Á¢¼ÓÀÌ ±×»çÀÌ ²÷°å´Ù´øÁö) ¹«È¿ÀÓ 
}

void CGame::DismissGuildApproveHandler(int iClientH, char* pName)
{
	int i;


	if (m_pClientList[iClientH] == 0) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	for (i = 0; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != 0) && (memcmp(m_pClientList[i]->m_cCharName, pName, 10) == 0)) {

			bSendMsgToLS(MSGID_REQUEST_UPDATEGUILDINFO_DELGUILDSMAN, i);
			SendGuildMsg(i, DEF_NOTIFY_DISMISSGUILDSMAN, 0, 0, 0);

			ZeroMemory(m_pClientList[i]->m_cGuildName, sizeof(m_pClientList[i]->m_cGuildName));
			strcpy(m_pClientList[i]->m_cGuildName, "NONE");
			m_pClientList[i]->m_iGuildRank = -1;
			m_pClientList[i]->m_iGuildGUID = -1;

			SendNotifyMsg(0, i, DEF_COMMONTYPE_DISMISSGUILDAPPROVE, 0, 0, 0, 0);

			SendEventToNearClient_TypeA(i, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
		}

}

void CGame::DismissGuildRejectHandler(int iClientH, char* pName)
{
	int i;

	if (m_pClientList[iClientH] == 0) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

	// pNameÀ» °®´Â Å¬¶óÀÌ¾ðÆ®ÀÇ iClientH ±æµå¿¡ ´ëÇÑ Å»Åð ¿ä±¸°¡ ½ÇÆÐ ÇÏ¿´´Ù.

	// pNameÀÇ ÀÌ¸§À» °®´Â Å¬¶óÀÌ¾ðÆ® ±¸Á¶Ã¼¸¦ °Ë»öÇÑ´Ù.
	for (i = 0; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != 0) && (memcmp(m_pClientList[i]->m_cCharName, pName, 10) == 0)) {

			// °¡ÀÔ ½ÅÃ»ÀÚ¿¡°Ô Å»Åð°¡ ½ÇÆÐÇßÀ½À» ¾Ë¸®´Â ¸Þ½ÃÁö¸¦ º¸³»ÁØ´Ù.
			SendNotifyMsg(iClientH, i, DEF_COMMONTYPE_DISMISSGUILDREJECT, 0, 0, 0, 0);
			break;
		}

	// Å»Åð¸¦ ½ÅÃ»ÇÑ Å¬¶óÀÌ¾ðÆ®¸¦ Ã£À»¼ö ¾ø´Ù.(Á¢¼ÓÀÌ ±×»çÀÌ ²÷°å´Ù´øÁö) ¹«È¿ÀÓ 
}

void CGame::SendGuildMsg(int iClientH, WORD wNotifyMsgType, short sV1, short sV2, char* pString)
{
	char cData[500];
	DWORD* dwp;
	WORD* wp;
	char* cp;
	int i, iRet;

	// °°Àº ±æµå¿øµé¿¡°Ô¸¸ º¸³»´Â ¸Þ½ÃÁöµé
	if (m_pClientList[iClientH] == 0) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

	for (i = 0; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != 0) &&
			(memcmp(m_pClientList[i]->m_cGuildName, m_pClientList[iClientH]->m_cGuildName, 20) == 0)) {

			// ### BUG POINT À§Ä¡°¡ Àß¸øµÇ¾î Æ÷ÀÎÅÍ ¿¬»êÀÌ Àß¸øµÇ¾ú´Ù. 
			ZeroMemory(cData, sizeof(cData));

			dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
			*wp = wNotifyMsgType;

			cp = (char*)(cData + DEF_INDEX2_MSGTYPE + 2);

			// °°Àº ±æµå ÀÌ¸§À» °®°í ÀÖ´Â Å¬¶óÀÌ¾ðÆ®¸¦ Ã£¾Ò´Ù.
			switch (wNotifyMsgType) {
			case DEF_NOTIFY_GUILDDISBANDED:
				if (i == iClientH) break; // <-- ±æµå ¸¶½ºÅÍ ÀÚ½Å¿¡°Ô´Â ¸Þ½ÃÁö¸¦ º¸³»Áö ¾Ê´Â´Ù.
				// ±æµå ÇØ»ê Åëº¸¿¡ ÇØ´ç Å¬¶óÀÌ¾ðÆ®ÀÇ ±æµå ·©Å©¸¦ Å¬¸®¾îÇÑ´Ù. 
				memcpy(cp, m_pClientList[iClientH]->m_cGuildName, 20);
				cp += 20;

				iRet = m_pClientList[i]->m_pXSock->iSendMsg(cData, 26);
				// ÇØ´ç Å¬¶óÀÌ¾ðÆ®ÀÇ ±æµå³»¿ëÀ» Å¬¸®¾îÇÑ´Ù. @@@@@@@
				ZeroMemory(m_pClientList[i]->m_cGuildName, sizeof(m_pClientList[i]->m_cGuildName));
				strcpy(m_pClientList[i]->m_cGuildName, "NONE");
				m_pClientList[i]->m_iGuildRank = -1;
				m_pClientList[i]->m_iGuildGUID = -1;
				break;

			case DEF_NOTIFY_EVENTMSGSTRING:
				// ±æµå¿øµé¿¡°Ô Àü´ÞµÇ´Â ÀÌº¥Æ® ¸Þ½ÃÁö ½ºÆ®¸µ 
				strcpy(cp, pString);
				cp += strlen(pString);

				iRet = m_pClientList[i]->m_pXSock->iSendMsg(cData, 6 + strlen(pString) + 1);
				break;

			case DEF_NOTIFY_NEWGUILDSMAN:
			case DEF_NOTIFY_DISMISSGUILDSMAN:
				memcpy(cp, m_pClientList[iClientH]->m_cCharName, 10);
				cp += 10;

				iRet = m_pClientList[i]->m_pXSock->iSendMsg(cData, 6 + 10 + 1);
				break;
			}

			switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				// ¸Þ½ÃÁö¸¦ º¸³¾¶§ ¿¡·¯°¡ ¹ß»ýÇß´Ù¸é Á¦°ÅÇÑ´Ù.
				DeleteClient(i, true, true);
				break;
			}
		}

}

void CGame::GuildNotifyHandler(char* pData, DWORD dwMsgSize)
{
	// ´Ù¸¥ °ÔÀÓ¼­¹ö·ÎºÎÅÍ ±æµå ÀÌº¥Æ®°¡ µµÂøÇß´Ù. 
	char* cp, cCharName[11], cGuildName[21];

	ZeroMemory(cCharName, sizeof(cCharName));
	ZeroMemory(cGuildName, sizeof(cGuildName));

	cp = (char*)(pData + DEF_INDEX2_MSGTYPE + 2);

	memcpy(cCharName, cp, 10);
	cp += 10;

	memcpy(cGuildName, cp, 20);
	cp += 20;

	// ¾ÆÁ÷ ±¸ÇöµÇÁö ¾Ê¾Ò´Ù. 
}

// v1.4311-3 Ãß°¡¹× º¯°æ ÇÔ¼ö  ±æµå¿ø °­Åð ¸í·É void CGame::UserCommand_BanGuildsman
void CGame::UserCommand_BanGuildsman(int iClientH, char* pData, DWORD dwMsgSize)
{
	char   seps[] = "= \t\n";
	char* token, cTargetName[11], cBuff[256];
	class  CStrTok* pStrTok;
	int i;

	if (m_pClientList[iClientH] == 0) return;
	if ((dwMsgSize) <= 0) return;

	if (m_pClientList[iClientH]->m_iGuildRank != 0 && m_pClientList[iClientH]->m_iGuildRank != 3) {
		// ±æµå ¸¶½ºÅÍ°¡ ¾Æ´Ï¾î¼­ ÀÌ ±â´ÉÀ» »ç¿ëÇÒ¼ö ¾ø´Ù.
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_NOGUILDMASTERLEVEL, 0, 0, 0, 0);
		return;
	}

	ZeroMemory(cTargetName, sizeof(cTargetName));
	ZeroMemory(cBuff, sizeof(cBuff));
	memcpy(cBuff, pData, dwMsgSize);

	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();

	if (token != 0) {
		// tokenÀÌ ±æµå¸¦ Å»Åð½ÃÅ³ »ç¶÷ 
		if (strlen(token) > 10)
			memcpy(cTargetName, token, 10);
		else memcpy(cTargetName, token, strlen(token));

		for (i = 0; i < DEF_MAXCLIENTS; i++)
			if ((m_pClientList[i] != 0) && (memcmp(m_pClientList[i]->m_cCharName, cTargetName, 10) == 0)) {
				// ¸ñÇ¥ Ä³¸¯ÅÍ¸¦ Ã£¾Ò´Ù. °­Á¦·Î ±æµå¸¦ °­Åð ½ÃÅ²´Ù. 

				if (memcmp(m_pClientList[iClientH]->m_cGuildName, m_pClientList[i]->m_cGuildName, 21) != 0) {
					// ÀÚ½ÅÀÇ ±æµå¿øÀÌ ¾Æ´Ï¶ó Çã¶ôÀÌ ºÒ°¡´ÉÇÏ´Ù.

					SendNotifyMsg(0, iClientH, DEF_NOTIFY_CANNOTBANGUILDMAN, 0, 0, 0, 0);
					delete pStrTok;
					return;
				}
				if (m_pClientList[i]->m_iGuildRank == 0)
				{
					SendNotifyMsg(0, iClientH, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "Error: Cannot ban a Guildmaster.");
					delete pStrTok;
					return;
				}
				// ±æµå Á¤º¸ È­ÀÏ¿¡¼­ Å»ÅðÇÑ ±æµå¿øÀÇ ÀÌ¸§À» »èÁ¦ÇÑ´Ù.
				bSendMsgToLS(MSGID_REQUEST_UPDATEGUILDINFO_DELGUILDSMAN, i);

				// ´Ù¸¥ ±æµå¿øµé¿¡°Ô ±æµå¿øÀÇ Å»Åð¸¦ ¾Ë¸°´Ù.
				SendGuildMsg(i, DEF_NOTIFY_DISMISSGUILDSMAN, 0, 0, 0);

				// ±æµåÀÇ ÀÌ¸§À» ÃÊ±âÈ­ÇØ ÁØ´Ù.
				ZeroMemory(m_pClientList[i]->m_cGuildName, sizeof(m_pClientList[i]->m_cGuildName));
				strcpy(m_pClientList[i]->m_cGuildName, "NONE");
				m_pClientList[i]->m_iGuildRank = -1;
				m_pClientList[i]->m_iGuildGUID = -1;

				// ±æµå ¸¶½ºÅÍ¿¡°Ô °­Á¦ Å»Åð¿¡ ¼º°øÇßÀ½À» ¾Ë¸®´Â ¸Þ½ÃÁö¸¦ º¸³»ÁØ´Ù.
				SendNotifyMsg(0, iClientH, DEF_NOTIFY_SUCCESSBANGUILDMAN, 0, 0, 0, 0);

				// °­Á¦ Å»ÅðµÈ ±æµå¿ø¿¡°Ô °­Á¦ Å»Åð µÇ¾úÀ½À» ¾Ë¸°´Ù.
				SendNotifyMsg(0, i, DEF_COMMONTYPE_BANGUILD, 0, 0, 0, 0);

				// Æ¯¼ºÀÌ ¹Ù²î¹Ç·Î ¿Ü¾çÀ» »õ·Î º¸³½´Ù. 
				SendEventToNearClient_TypeA(i, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);

				delete pStrTok;
				return;
			}
		// ÇöÀç Á¢¼ÓÁßÀÌ ¾Æ´Ï´Ù.
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_PLAYERNOTONGAME, 0, 0, 0, cTargetName);
	}

	delete pStrTok;
}

void CGame::AdminOrder_SummonGuild(int iClientH)
{
	char cTemp[51], cMapName[11], cGuildName[20];
	int    pX, pY, i;
	
	if (m_pClientList[iClientH] == 0) return;
	
	if (m_pClientList[iClientH]->m_iGuildRank != 0 && m_pClientList[iClientH]->m_iGuildRank != 2) return;
	DWORD dwGoldCount = dwGetItemCount(iClientH, "Gold");  // dwGoldCount = player gold
	ZeroMemory(cTemp, sizeof(cTemp));
	if (m_iSummonGuildCost > dwGoldCount)
	{
		wsprintf(cTemp, "Not enough gold. Required %d Gold", m_iSummonGuildCost);
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, cTemp);
		return;
	}
	else // if summonguildcost is less than player gold
	{
		SetItemCount(iClientH, "Gold", dwGoldCount - (DWORD)m_iSummonGuildCost); // reduce gold by summonguildcost   
		wsprintf(cTemp, "You've used %d Gold!", m_iSummonGuildCost);
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, cTemp);
	}
	
	pX = m_pClientList[iClientH]->m_sX;
	pY = m_pClientList[iClientH]->m_sY;
	
	ZeroMemory(cMapName, sizeof(cMapName));
	memcpy(cMapName, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, 11);
	
	ZeroMemory(cGuildName, sizeof(cGuildName));
	memcpy(cGuildName, m_pClientList[iClientH]->m_cGuildName, 21);
	
	bool bExiste = false;
	for (i = 0; i < DEF_MAXGUILDS; i++)
	{
		if (string(m_stSummonGuild[i].cGuildName) == "NONE") continue;
		if (string(m_stSummonGuild[i].cGuildName) == cGuildName)
		{
			m_stSummonGuild[i].sX = pX;
			m_stSummonGuild[i].sY = pY;
			strcpy(m_stSummonGuild[i].cMap, cMapName);
			bExiste = true;
			break;
		}
	}
	if (!bExiste)
	{
		for (i = 0; i < DEF_MAXGUILDS; i++)
		{
			if (string(m_stSummonGuild[i].cGuildName) == "NONE")
			{
				strcpy(m_stSummonGuild[i].cGuildName, cGuildName);
				m_stSummonGuild[i].sX = pX;
				m_stSummonGuild[i].sY = pY;
				strcpy(m_stSummonGuild[i].cMap, cMapName);
				break;
			}
		}
	}

	for (i = 0; i < DEF_MAXCLIENTS; i++) 
	{
		if (i == iClientH) continue;
		if ((m_pClientList[i] != 0) && (strcmp(m_pClientList[i]->m_cGuildName, cGuildName) == 0))
		{
			SendNotifyMsg(0, i, DEF_NOTIFY_SUMMONGUILD, 0, 0, 0, 0);
		}
	}
	wsprintf(G_cTxt, "Guild(%s) summoned by PC(%s) to %s(%d,%d).", cGuildName, m_pClientList[iClientH]->m_cCharName, cMapName, pX, pY);
	PutLogList(G_cTxt);
}