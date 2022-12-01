#include "Quest.h"

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

bool CGame::_bDecodeQuestConfigFileContents(char* pData, DWORD dwMsgSize)
{
	char* pContents, * token, cTxt[120];
	char seps[] = "= \t\n";
	char cReadModeA = 0;
	char cReadModeB = 0;
	int  iQuestConfigListIndex = 0;
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
					// Äù½ºÆ® ¹øÈ£ 
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}

					if (m_pQuestConfigList[atoi(token)] != 0) {
						// ÀÌ¹Ì ÇÒ´çµÈ ¹øÈ£°¡ ÀÖ´Ù. ¿¡·¯ÀÌ´Ù.
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Duplicate quest number.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pQuestConfigList[atoi(token)] = new class CQuest;
					iQuestConfigListIndex = atoi(token);

					cReadModeB = 2;
					break;

				case 2:
					// Äù½ºÆ® »çÀÌµå  
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_cSide = atoi(token);
					cReadModeB = 3;
					break;

				case 3:
					// Äù½ºÆ® Á¾·ù  
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iType = atoi(token);
					cReadModeB = 4;
					break;

				case 4:
					// TargetType
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iTargetType = atoi(token);
					cReadModeB = 5;
					break;

				case 5:
					// MaxCount
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iMaxCount = atoi(token);
					cReadModeB = 6;
					break;

				case 6:
					// 
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iFrom = atoi(token);
					cReadModeB = 7;
					break;

				case 7:
					// 
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iMinLevel = atoi(token);
					cReadModeB = 8;
					break;

				case 8:
					// 
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iMaxLevel = atoi(token);
					cReadModeB = 9;
					break;

				case 9:
					// 
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iRequiredSkillNum = atoi(token);
					cReadModeB = 10;
					break;

				case 10:
					// 
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iRequiredSkillLevel = atoi(token);
					cReadModeB = 11;
					break;

				case 11:
					// 
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iTimeLimit = atoi(token);
					cReadModeB = 12;
					break;

				case 12:
					// 
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iAssignType = atoi(token);
					cReadModeB = 13;
					break;

				case 13:
					// 
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iRewardType[1] = atoi(token);
					cReadModeB = 14;
					break;

				case 14:
					// 
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iRewardAmount[1] = atoi(token);
					cReadModeB = 15;
					break;

				case 15:
					// 
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iRewardType[2] = atoi(token);
					cReadModeB = 16;
					break;

				case 16:
					// 
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iRewardAmount[2] = atoi(token);
					cReadModeB = 17;
					break;

				case 17:
					// 
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iRewardType[3] = atoi(token);
					cReadModeB = 18;
					break;

				case 18:
					// 
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iRewardAmount[3] = atoi(token);
					cReadModeB = 19;
					break;

				case 19:
					// 
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iContribution = atoi(token);
					cReadModeB = 20;
					break;

				case 20:
					// 
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iContributionLimit = atoi(token);
					cReadModeB = 21;
					break;

				case 21:
					// 
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iResponseMode = atoi(token);
					cReadModeB = 22;
					break;

				case 22:
					ZeroMemory(m_pQuestConfigList[iQuestConfigListIndex]->m_cTargetName, sizeof(m_pQuestConfigList[iQuestConfigListIndex]->m_cTargetName));
					strcpy(m_pQuestConfigList[iQuestConfigListIndex]->m_cTargetName, token);
					cReadModeB = 23;
					break;

				case 23:
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_sX = atoi(token);
					cReadModeB = 24;
					break;

				case 24:
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_sY = atoi(token);
					cReadModeB = 25;
					break;

				case 25:
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iRange = atoi(token);
					cReadModeB = 26;
					break;

				case 26:
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iQuestID = atoi(token);
					cReadModeB = 27;
					break;

				case 27:
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iReqContribution = atoi(token);
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
			if (memcmp(token, "quest", 5) == 0) {
				cReadModeA = 1;
				cReadModeB = 1;
			}
		}
		token = pStrTok->pGet();
	}

	delete pStrTok;
	delete[] pContents;

	if ((cReadModeA != 0) || (cReadModeB != 0)) {
		PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file contents error!");
		return false;
	}

	wsprintf(cTxt, "(!) QUEST(Total:%d) configuration - success!", iQuestConfigListIndex);
	PutLogList(cTxt);

	return true;
}


int CGame::__iSearchForQuest(int iClientH, int iWho, int* pQuestType, int* pMode, int* pRewardType, int* pRewardAmount, int* pContribution, char* pTargetName, int* pTargetType, int* pTargetCount, int* pX, int* pY, int* pRange)
{
	int i, iQuestList[DEF_MAXQUESTTYPE], iIndex, iQuest, iReward, iQuestIndex;

	if (m_pClientList[iClientH] == 0) return -1;

	// ÀûÇÕÇÑ ¹Ì¼Ç ¹è¿­ ÃÊ±âÈ­.
	iIndex = 0;
	for (i = 0; i < DEF_MAXQUESTTYPE; i++)
		iQuestList[i] = -1;

	for (i = 1; i < DEF_MAXQUESTTYPE; i++)
		if (m_pQuestConfigList[i] != 0) {

			if (m_pQuestConfigList[i]->m_iFrom != iWho) goto SFQ_SKIP;
			if (m_pQuestConfigList[i]->m_cSide != m_pClientList[iClientH]->m_cSide) goto SFQ_SKIP;
			if (m_pQuestConfigList[i]->m_iMinLevel > m_pClientList[iClientH]->m_iLevel) goto SFQ_SKIP;
			if (m_pQuestConfigList[i]->m_iMaxLevel < m_pClientList[iClientH]->m_iLevel) goto SFQ_SKIP;
			if (m_pQuestConfigList[i]->m_iReqContribution > m_pClientList[iClientH]->m_iContribution) goto SFQ_SKIP;

			if (m_pQuestConfigList[i]->m_iRequiredSkillNum != -1) {
				if (m_pClientList[iClientH]->m_cSkillMastery[m_pQuestConfigList[i]->m_iRequiredSkillNum] <
					m_pQuestConfigList[i]->m_iRequiredSkillLevel) goto SFQ_SKIP;
			}

			if ((m_bIsCrusadeMode == true) && (m_pQuestConfigList[i]->m_iAssignType != 1)) goto SFQ_SKIP;
			if ((m_bIsCrusadeMode == false) && (m_pQuestConfigList[i]->m_iAssignType == 1)) goto SFQ_SKIP;

			if (m_pQuestConfigList[i]->m_iContributionLimit < m_pClientList[iClientH]->m_iContribution) goto SFQ_SKIP;

			iQuestList[iIndex] = i;
			iIndex++;

		SFQ_SKIP:;
		}

	if (iIndex == 0) return -1;
	iQuest = (iDice(1, iIndex)) - 1;
	iQuestIndex = iQuestList[iQuest];
	iReward = iDice(1, 3);
	*pMode = m_pQuestConfigList[iQuestIndex]->m_iResponseMode;
	*pRewardType = m_pQuestConfigList[iQuestIndex]->m_iRewardType[iReward];
	// SNOOPY: Added a trap for item non existing on the server...
	if (m_pItemConfigList[m_pQuestConfigList[iQuestIndex]->m_iRewardType[iReward]] == 0)
	{
		*pRewardAmount = -1; // Replaces by Experience				
	}
	else *pRewardAmount = m_pQuestConfigList[iQuestIndex]->m_iRewardAmount[iReward];	// Äù½ºÆ® »óÇ° °¹¼ö 
	*pContribution = m_pQuestConfigList[iQuestIndex]->m_iContribution;			// Äù½ºÆ® ÇØ°á½Ã ¿Ã¶ó°¡´Â °øÇåµµ 

	strcpy(pTargetName, m_pQuestConfigList[iQuestIndex]->m_cTargetName);			// Äù½ºÆ® ¸Ê ÀÌ¸§ ¼ö·Ï 
	*pX = m_pQuestConfigList[iQuestIndex]->m_sX;
	*pY = m_pQuestConfigList[iQuestIndex]->m_sY;
	*pRange = m_pQuestConfigList[iQuestIndex]->m_iRange;

	*pTargetType = m_pQuestConfigList[iQuestIndex]->m_iTargetType;
	*pTargetCount = m_pQuestConfigList[iQuestIndex]->m_iMaxCount;
	*pQuestType = m_pQuestConfigList[iQuestIndex]->m_iType;

	return iQuestIndex;
}

// New 14/05/2004
void CGame::QuestAcceptedHandler(int iClientH)
{
	int iIndex, i;

	if (m_pClientList[iClientH] == 0) return;

	// Does the quest exist ??
	if (m_pQuestConfigList[m_pClientList[iClientH]->m_iAskedQuest] == 0) return;

	for (i = 0; i < DEF_MAXQUEST; i++) { //Magn0S:: Multi Quest
		if ((m_pClientList[iClientH]->m_iQuest[i] != 0) && (m_pClientList[iClientH]->m_iQuest[i] == m_pClientList[iClientH]->m_iAskedQuest)) {
			//Magn0S:: Mult quest can't accept same question
			if (m_pClientList[iClientH]->m_iQuest[i] == m_pClientList[iClientH]->m_iAskedQuest) {
				SendNotifyMsg(0, iClientH, DEF_NOTIFY_IPACCOUNTINFO, 0, 0, 0, "You can't repeat Quest. Please finish your quest to get the same Quest later.");
				return;
			}
		}
		else {
			if (m_pClientList[iClientH]->m_iQuest[i] == 0) {
				if (m_pQuestConfigList[m_pClientList[iClientH]->m_iAskedQuest]->m_iAssignType == 1) {
					switch (m_pQuestConfigList[m_pClientList[iClientH]->m_iAskedQuest]->m_iType) {
					case 10:
						_ClearQuestStatus(iClientH, i);
						RequestTeleportHandler(iClientH, "2   ", m_pQuestConfigList[m_pClientList[iClientH]->m_iAskedQuest]->m_cTargetName,
							m_pQuestConfigList[m_pClientList[iClientH]->m_iAskedQuest]->m_sX, m_pQuestConfigList[m_pClientList[iClientH]->m_iAskedQuest]->m_sY);
						return;
					}
				}

				m_pClientList[iClientH]->m_iQuest[i] = m_pClientList[iClientH]->m_iAskedQuest;
				iIndex = m_pClientList[iClientH]->m_iQuest[i];
				m_pClientList[iClientH]->m_iQuestID[i] = m_pQuestConfigList[iIndex]->m_iQuestID;
				m_pClientList[iClientH]->m_iCurQuestCount[i] = 0;
				m_pClientList[iClientH]->m_bIsQuestCompleted[i] = false;

				//Magn0S:: Fix
				m_pClientList[iClientH]->m_iQuestRewardType[i] = m_pQuestConfigList[iIndex]->m_iRewardType[1]; // Sempre a 1º
				m_pClientList[iClientH]->m_iQuestRewardAmount[i] = m_pQuestConfigList[iIndex]->m_iRewardAmount[1];

				SendNotifyMsg(0, iClientH, DEF_NOTIFY_IPACCOUNTINFO, 0, 0, 0, "Congratulations, you have selected a new Quest!");

				_CheckQuestEnvironment(iClientH);
				_SendQuestContents(iClientH);

				if (m_pClientList[iClientH]->m_iQuest[i] != 0)
				{
					//cQuestRemain = (m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest[i]]->m_iMaxCount - m_pClientList[iClientH]->m_iCurQuestCount[i]);
					//SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTCOUNTER, i, cQuestRemain, 0, 0);
					SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTCOUNTER, i, m_pClientList[iClientH]->m_iCurQuestCount[i], 0, 0);
					_bCheckIsQuestCompleted(iClientH, i);
				}
				return;
			}
		}
	}
}


void CGame::_SendQuestContents(int iClientH)
{
	int iWho, iIndex, iQuestType, iContribution, iTargetType, iTargetCount, iX, iY, iRange, iQuestCompleted, i, iAmount;
	char cTargetName[21];

	if (m_pClientList[iClientH] == 0) return;

	for (i = 0; i < DEF_MAXQUEST; i++) { //Magn0S:: Multi Quest
		if (m_pClientList[iClientH]->m_iQuest[i] != 0) {
			iIndex = m_pClientList[iClientH]->m_iQuest[i];
			if (iIndex == 0) {
				SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTCONTENTS, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0);
			}
			else {
				iWho = m_pQuestConfigList[iIndex]->m_iFrom;
				iQuestType = m_pQuestConfigList[iIndex]->m_iType;
				iContribution = m_pQuestConfigList[iIndex]->m_iContribution;
				iTargetType = m_pQuestConfigList[iIndex]->m_iTargetType;
				iTargetCount = m_pQuestConfigList[iIndex]->m_iMaxCount;
				iX = m_pQuestConfigList[iIndex]->m_sX;
				iY = m_pQuestConfigList[iIndex]->m_sY;
				iRange = m_pQuestConfigList[iIndex]->m_iRange;
				ZeroMemory(cTargetName, sizeof(cTargetName));
				memcpy(cTargetName, m_pQuestConfigList[iIndex]->m_cTargetName, 20);
				iQuestCompleted = (int)m_pClientList[iClientH]->m_bIsQuestCompleted[i];

				SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTCONTENTS, iWho, iQuestType, iContribution, 0,
					iTargetType, iTargetCount, iX, iY, iRange, iQuestCompleted, cTargetName, i);// Centu - crash point

				//------------------------------------------------------------------------------------------------------
				if (m_pClientList[iClientH]->m_iQuestRewardType[i] > 0) {
					iAmount = m_pClientList[iClientH]->m_iQuestRewardAmount[i];
					//wsprintf(G_cTxt, "Amount: %d, Name: %s", iAmount, m_pItemConfigList[m_pClientList[iClientH]->m_iQuestRewardType[i]]->m_cName);
					//PutLogList(G_cTxt);
					SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTPRIZE, i, m_pClientList[iClientH]->m_iQuestRewardType[i], iAmount, m_pItemConfigList[m_pClientList[iClientH]->m_iQuestRewardType[i]]->m_cName);
				}
				//------------------------------------------------------------------------------------------------------

				//Magn0S:: Add to fix.
				//cQuestRemain = (m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest[i]]->m_iMaxCount - m_pClientList[iClientH]->m_iCurQuestCount[i]);
				//SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTCOUNTER, i, cQuestRemain, 0, 0);
				SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTCOUNTER, i, m_pClientList[iClientH]->m_iCurQuestCount[i], 0, 0);
			}
		}
	}
}

void CGame::_CheckQuestEnvironment(int iClientH)
{
	int iIndex, i;
	char cTargetName[21];

	if (m_pClientList[iClientH] == 0) return;

	for (i = 0; i < DEF_MAXQUEST; i++) { //Magn0S:: Multi Quest
	//	if (m_pClientList[iClientH]->m_iQuest[i] != 0) {

			iIndex = m_pClientList[iClientH]->m_iQuest[i];
			if (iIndex == 0) return;
			if (m_pQuestConfigList[iIndex] == 0) return;

			if (iIndex >= 35 && iIndex <= 40) {
				m_pClientList[iClientH]->m_iQuest[i] = 0;
				m_pClientList[iClientH]->m_iQuestID[i] = 0;
				m_pClientList[iClientH]->m_iQuestRewardAmount[i] = 0;
				m_pClientList[iClientH]->m_iQuestRewardType[i] = 0;
				SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTABORTED, i, 0, 0, 0);
				return;
			}

			if (m_pQuestConfigList[iIndex]->m_iQuestID != m_pClientList[iClientH]->m_iQuestID[i]) {
				m_pClientList[iClientH]->m_iQuest[i] = 0;
				m_pClientList[iClientH]->m_iQuestID[i] = 0;
				m_pClientList[iClientH]->m_iQuestRewardAmount[i] = 0;
				m_pClientList[iClientH]->m_iQuestRewardType[i] = 0;

				SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTABORTED, i, 0, 0, 0);
				return;
			}

			switch (m_pQuestConfigList[iIndex]->m_iType) {
			case DEF_QUESTTYPE_MONSTERHUNT:
			case DEF_QUESTTYPE_GOPLACE:
				ZeroMemory(cTargetName, sizeof(cTargetName));
				memcpy(cTargetName, m_pQuestConfigList[iIndex]->m_cTargetName, 20);
				if (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, cTargetName, 10) == 0)
					m_pClientList[iClientH]->m_bQuestMatchFlag_Loc = true;
				else m_pClientList[iClientH]->m_bQuestMatchFlag_Loc = false;
				break;
			}
	//	}
	}
}

bool CGame::_bCheckIsQuestCompleted(int iClientH, int iQuest)
{
	int iQuestIndex;
	char cTargetName[21];

	if (m_pClientList[iClientH] == 0) return false;

	//Magn0S:: Multi Quest
	//for (i = 0; i < DEF_MAXQUEST; i++) {
		//if (m_pClientList[iClientH]->m_iQuest[i] != 0) {
			if (m_pClientList[iClientH]->m_bIsQuestCompleted[iQuest] == true) return false;

			iQuestIndex = m_pClientList[iClientH]->m_iQuest[iQuest];
			if (iQuestIndex == 0) return false;

			if (m_pQuestConfigList[iQuestIndex] != 0) {
				switch (m_pQuestConfigList[iQuestIndex]->m_iType) {
				case DEF_QUESTTYPE_MONSTERHUNT:
				//	if ((m_pClientList[iClientH]->m_bQuestMatchFlag_Loc == true) &&
						ZeroMemory(cTargetName, sizeof(cTargetName));
						memcpy(cTargetName, m_pQuestConfigList[iQuestIndex]->m_cTargetName, 20);
						if ((memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, cTargetName, 10) == 0) &&
						(m_pClientList[iClientH]->m_iCurQuestCount[iQuest] >= m_pQuestConfigList[iQuestIndex]->m_iMaxCount)) {
						m_pClientList[iClientH]->m_bIsQuestCompleted[iQuest] = true;
						m_pClientList[iClientH]->m_iCurQuestCount[iQuest] = m_pQuestConfigList[iQuestIndex]->m_iMaxCount;
						SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTCOMPLETED, iQuest, 0, 0, 0);
						return true;
					}
					break;

				case DEF_QUESTTYPE_GOPLACE:
					//if ((m_pClientList[iClientH]->m_bQuestMatchFlag_Loc == true) &&
					ZeroMemory(cTargetName, sizeof(cTargetName));
					memcpy(cTargetName, m_pQuestConfigList[iQuestIndex]->m_cTargetName, 20);
					if ((memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, cTargetName, 10) == 0) &&
						(m_pClientList[iClientH]->m_sX >= m_pQuestConfigList[iQuestIndex]->m_sX - m_pQuestConfigList[iQuestIndex]->m_iRange) && // ÁÂÇ¥µµ ÀÌ³»ÀÌ¸é 
						(m_pClientList[iClientH]->m_sX <= m_pQuestConfigList[iQuestIndex]->m_sX + m_pQuestConfigList[iQuestIndex]->m_iRange) &&
						(m_pClientList[iClientH]->m_sY >= m_pQuestConfigList[iQuestIndex]->m_sY - m_pQuestConfigList[iQuestIndex]->m_iRange) &&
						(m_pClientList[iClientH]->m_sY <= m_pQuestConfigList[iQuestIndex]->m_sY + m_pQuestConfigList[iQuestIndex]->m_iRange)) {
						m_pClientList[iClientH]->m_bIsQuestCompleted[iQuest] = true;
						SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTCOMPLETED, iQuest, 0, 0, 0);
						return true;
					}
					break;
				}
			}
		//}
	//}

	return false;
}

int CGame::_iTalkToNpcResult_Cityhall(int iClientH, int* pQuestType, int* pMode, int* pRewardType, int* pRewardAmount, int* pContribution, char* pTargetName, int* pTargetType, int* pTargetCount, int* pX, int* pY, int* pRange)
{
	int iQuest;
	

	// Return Code
	// -1  : ÇöÀç ÀÓ¹« ¼öÇàÁß
	// -2  : ¸¶À»ÀÌ ´Ù¸£°Å³ª Áß¸³Àº ÀÓ¹«¸¦ ¾òÀ» ¼ö ¾ø´Ù.    
	// -3  : ¹üÁËÀÚ´Â ÀÓ¹«¸¦ ¾òÀ» ¼ö ¾ø´Ù.
	// -4  : ÇöÀç Á¶°Ç¿¡ ¸Â´Â ÀÓ¹«°¡ ¾ø´Ù. ÀÏ»óÀûÀÎ ´ëÈ­³ª Á¤º¸ Ãâ·Â 

	if (m_pClientList[iClientH] == 0) return 0;

	//Magn0S:: Canceled this part... Now to receive the quest prize, added a new and unique function for that.
	/*for (i = 0; i < DEF_MAXQUEST; i++) { // Magn0S:: Multi Quest
		if (m_pClientList[iClientH]->m_iQuest[i] != 0) {
			if (m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest[i]] == 0) return -4;
			else if (m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest[i]]->m_iFrom == 4) {
				if (m_pClientList[iClientH]->m_bIsQuestCompleted[i] == true) {
					if ((m_pClientList[iClientH]->m_iQuestRewardType > 0) &&
						(m_pItemConfigList[m_pClientList[iClientH]->m_iQuestRewardType[i]] != 0)) {
						pItem = new class CItem;
						_bInitItemAttr(pItem, m_pItemConfigList[m_pClientList[iClientH]->m_iQuestRewardType[i]]->m_cName);
						pItem->m_dwCount = m_pClientList[iClientH]->m_iQuestRewardAmount[i];
						if (_bCheckItemReceiveCondition(iClientH, pItem) == true) {
							_bAddClientItemList(iClientH, pItem, &iEraseReq);
							SendItemNotifyMsg(iClientH, DEF_NOTIFY_ITEMOBTAINED, pItem, 0);
							if (iEraseReq == 1) delete pItem;

							m_pClientList[iClientH]->m_iContribution += m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest[i]]->m_iContribution;
							SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTREWARD, 4, 1, m_pClientList[iClientH]->m_iQuestRewardAmount[i],
								m_pItemConfigList[m_pClientList[iClientH]->m_iQuestRewardType[i]]->m_cName, m_pClientList[iClientH]->m_iContribution, i);

							_ClearQuestStatus(iClientH, i);
							return -5;
						}
						else {
							delete pItem;
							SendItemNotifyMsg(iClientH, DEF_NOTIFY_CANNOTCARRYMOREITEM, 0, 0);

							SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTREWARD, 4, 0, m_pClientList[iClientH]->m_iQuestRewardAmount[i],
								m_pItemConfigList[m_pClientList[iClientH]->m_iQuestRewardType[i]]->m_cName, m_pClientList[iClientH]->m_iContribution, i);

							return -5;
						}
					}
					else if (m_pClientList[iClientH]->m_iQuestRewardType[i] == -1) {
						GetExp(iClientH, iDice(1, m_pClientList[iClientH]->m_iQuestRewardAmount[i]));
						m_pClientList[iClientH]->m_iContribution += m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest[i]]->m_iContribution;

						SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTREWARD, 4, 1, m_pClientList[iClientH]->m_iQuestRewardAmount[i],
							"Exp", m_pClientList[iClientH]->m_iContribution, i);

						_ClearQuestStatus(iClientH, i);
						return -5;
					}
					else if (m_pClientList[iClientH]->m_iQuestRewardType[i] == -2) {
						// * m_pClientList[iClientH]->m_iQuestRewardAmount
						//***
						iExp = (iDice(1, (10 * (m_pClientList[iClientH]->m_iLevel))) * m_pClientList[iClientH]->m_iQuestRewardAmount[i]);

						GetExp(iClientH, iDice(1, iExp));
						m_pClientList[iClientH]->m_iContribution += m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest[i]]->m_iContribution;

						SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTREWARD, 4, 1, iExp,
							"Exp", m_pClientList[iClientH]->m_iContribution, i);

						_ClearQuestStatus(iClientH, i);
						return -5;
					}
					else {
						m_pClientList[iClientH]->m_iContribution += m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest[i]]->m_iContribution;

						SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTREWARD, 4, 1, 0,
							"                     ", m_pClientList[iClientH]->m_iContribution, i);

						_ClearQuestStatus(iClientH, i);
						return -5;
					}
				}
				else return -1;
			}
			return -4;
		}
	}*/


	if (memcmp(m_pClientList[iClientH]->m_cLocation, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, 10) == 0) {
		if (m_pClientList[iClientH]->m_iPKCount > 0) return -3;

		iQuest = __iSearchForQuest(iClientH, 4, pQuestType, pMode, pRewardType, pRewardAmount, pContribution, pTargetName, pTargetType, pTargetCount, pX, pY, pRange);
		if (iQuest <= 0) return -4;
		return iQuest;
	}
	else return -2;

	return -4;
}

void CGame::_ClearQuestStatus(int iClientH, int iQuest)
{
	//int i;

	if (m_pClientList[iClientH] == 0) return;
	if (iQuest < 0 && iQuest > 4) return;

	//for (i = 0; i < DEF_MAXQUEST; i++) {
		m_pClientList[iClientH]->m_iQuest[iQuest] = 0;
		m_pClientList[iClientH]->m_iQuestID[iQuest] = 0;
		m_pClientList[iClientH]->m_iQuestRewardType[iQuest] = 0;
		m_pClientList[iClientH]->m_iQuestRewardAmount[iQuest] = 0;
		m_pClientList[iClientH]->m_bIsQuestCompleted[iQuest] = false;

		//Magn0S:: Update quest list after finising
		_SendQuestContents(iClientH);
	//}
}

void CGame::CancelQuestHandler(int iClientH, int iQuest)
{
	if (m_pClientList[iClientH] == 0) return;
	_ClearQuestStatus(iClientH, iQuest);
	SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTABORTED, iQuest, 0, 0, 0);
}

//Magn0S:: Quest List
void CGame::RequestQuestList(int iClientH, char* pData, DWORD dwMsgSize)
{
	if (m_pClientList[iClientH] == 0) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if (m_pClientList[iClientH]->m_bIsKilled == true) return;
	if (m_pClientList[iClientH]->m_bIsOnWaitingProcess == true) return;

	char* cp, cData[5000];
	int  iRet;
	DWORD* dwp;
	WORD* wp;
	int* listCount;
	int* ip;


	cp = (char*)(pData + DEF_INDEX2_MSGTYPE + 2);

	ZeroMemory(cData, sizeof(cData));
	dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
	*dwp = DEF_MSGID_RESPONSE_QUEST_LIST;
	wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
	*wp = DEF_MSGTYPE_CONFIRM;

	cp = cData + 6;

	listCount = (int*)cp;
	*listCount = 0;
	cp += 4; // sizeof(int)

	for (int index = 0; index < DEF_MAXQUESTTYPE; index++)
	{
		if (m_pQuestConfigList[index] != 0)
		{
			if ((m_pClientList[iClientH]->m_iLevel >= m_pQuestConfigList[index]->m_iMinLevel) &&
				(m_pClientList[iClientH]->m_iLevel <= m_pQuestConfigList[index]->m_iMaxLevel) &&
				(m_pQuestConfigList[index]->m_cSide == m_pClientList[iClientH]->m_cSide) &&
				//for (i = 0; i < DEF_MAXQUEST; i++) {
				(m_pQuestConfigList[index]->m_iQuestID != m_pClientList[iClientH]->m_iQuest[index]) &&
				(m_pQuestConfigList[index]->m_iReqContribution <= m_pClientList[iClientH]->m_iContribution) &&
				(m_pQuestConfigList[index]->m_iContributionLimit >= m_pClientList[iClientH]->m_iContribution) &&
				(m_bIsCrusadeMode == false))
			{
				ip = (int*)cp;
				*ip = index;
				cp += 4;

				ip = (int*)cp;
				*ip = m_pQuestConfigList[index]->m_iContribution;
				cp += 4;

				ip = (int*)cp;
				*ip = m_pQuestConfigList[index]->m_iMaxCount;
				cp += 4;

				ip = (int*)cp;
				*ip = m_pQuestConfigList[index]->m_iTargetType;
				cp += 4;

				//Magn0S:: Canceled to improve the code (now send NPC Type and Reward item name)
				/*for (int o = 0; o < DEF_MAXNPCS; o++)
				{
					if ((m_pNpcConfigList[o] != 0) && (m_pNpcConfigList[o]->m_sType == m_pQuestConfigList[index]->m_iTargetType))
					{
						memcpy(cp, m_pNpcConfigList[o]->m_cNpcName, 20);
						cp += 20;
						break;
					}
				}*/

				if (m_pQuestConfigList[index]->m_iRewardType[1] > 0) {
					memcpy(cp, m_pItemConfigList[m_pQuestConfigList[index]->m_iRewardType[1]]->m_cName, 20);
					
				}
				else {
					memcpy(cp, "Exp", 20);
					
				}
				cp += 20;

				memcpy(cp, m_pQuestConfigList[index]->m_cTargetName, 20);
				cp += 20;

				(*listCount)++;
			}
		}
	}
	if ((*listCount) == 0)*wp = DEF_MSGTYPE_REJECT;
	// Quest List
	iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 10 + (*listCount) * 56);
	switch (iRet)
	{
	case DEF_XSOCKEVENT_QUENEFULL:
	case DEF_XSOCKEVENT_SOCKETERROR:
	case DEF_XSOCKEVENT_CRITICALERROR:
	case DEF_XSOCKEVENT_SOCKETCLOSED:
		DeleteClient(iClientH, true, true);
		return;
	}
}

//Mang0S:: Function to Claim the Prize for Multi Quest
void CGame::RequestQuestPrize(int iClientH, int iQuest)
{
int iEraseReq, iExp;
class CItem* pItem;

	if (m_pClientList[iClientH] == 0) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if (m_pClientList[iClientH]->m_bIsKilled == true) return;
	if (m_pClientList[iClientH]->m_bIsOnWaitingProcess == true) return;

	if (m_pClientList[iClientH]->m_iQuest[iQuest] != 0) {
		if (m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest[iQuest]] == 0)
		{
			SendNotifyMsg(0, iClientH, DEF_NOTIFY_NPCTALK, 104, 0, 0, 0, 0);
		}
		else if (m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest[iQuest]]->m_iFrom == 4)
		{
			if (m_pClientList[iClientH]->m_bIsQuestCompleted[iQuest] == true)
			{
				if ((m_pItemConfigList[m_pClientList[iClientH]->m_iQuestRewardType[iQuest]] != 0) && // Quest Reward = ITEM
					(m_pClientList[iClientH]->m_iQuestRewardType[iQuest] > 0))
				{
					pItem = new class CItem;
					_bInitItemAttr(pItem, m_pItemConfigList[m_pClientList[iClientH]->m_iQuestRewardType[iQuest]]->m_cName);
					pItem->m_dwCount = m_pClientList[iClientH]->m_iQuestRewardAmount[iQuest];

					if (_bCheckItemReceiveCondition(iClientH, pItem) == true)
					{
						_bAddClientItemList(iClientH, pItem, &iEraseReq);
						SendItemNotifyMsg(iClientH, DEF_NOTIFY_ITEMOBTAINED, pItem, 0);

						//Magn0S:: Quest itens = UNIQUE
						pItem->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
						pItem->m_sTouchEffectValue1 = m_pClientList[iClientH]->m_sCharIDnum1;
						pItem->m_sTouchEffectValue2 = m_pClientList[iClientH]->m_sCharIDnum2;
						pItem->m_sTouchEffectValue3 = m_pClientList[iClientH]->m_sCharIDnum3;

						if (iEraseReq == 1)
						{
							delete pItem;
							pItem = 0;
						}
						m_pClientList[iClientH]->m_iContribution += m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest[iQuest]]->m_iContribution;
						SendNotifyMsg(0, iClientH, DEF_NOTIFY_CONTRIBPOINTS, m_pClientList[iClientH]->m_iContribution, 0, 0, 0);
						SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTREWARD, 4, 1, m_pClientList[iClientH]->m_iQuestRewardAmount[iQuest],
							m_pItemConfigList[m_pClientList[iClientH]->m_iQuestRewardType[iQuest]]->m_cName, m_pClientList[iClientH]->m_iContribution);

						_ClearQuestStatus(iClientH, iQuest);
						//Magn0S:: Update quest list after finising
						_SendQuestContents(iClientH);
						return;
					}
					else
					{
						delete pItem;
						pItem = 0;

						SendItemNotifyMsg(iClientH, DEF_NOTIFY_CANNOTCARRYMOREITEM, 0, 0);

						SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTREWARD, 4, 0, m_pClientList[iClientH]->m_iQuestRewardAmount[iQuest],
							m_pItemConfigList[m_pClientList[iClientH]->m_iQuestRewardType[iQuest]]->m_cName, m_pClientList[iClientH]->m_iContribution);

						return;
					}
				}
				else if (m_pClientList[iClientH]->m_iQuestRewardType[iQuest] == -1) // Quest Rewards = Exp / Gold
				{
					GetExp(iClientH, m_pClientList[iClientH]->m_iQuestRewardAmount[iQuest]);

					m_pClientList[iClientH]->m_iContribution += m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest[iQuest]]->m_iContribution;
					SendNotifyMsg(0, iClientH, DEF_NOTIFY_CONTRIBPOINTS, m_pClientList[iClientH]->m_iContribution, 0, 0, 0);
					SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTREWARD, 4, 1, m_pClientList[iClientH]->m_iQuestRewardAmount[iQuest],
						"Exp", m_pClientList[iClientH]->m_iContribution);

					_ClearQuestStatus(iClientH, iQuest);
					//Magn0S:: Update quest list after finising
					_SendQuestContents(iClientH);
					return;
				}
				else if (m_pClientList[iClientH]->m_iQuestRewardType[iQuest] == -2)
				{
					iExp = iDice(1, (10 * m_pClientList[iClientH]->m_iLevel));

					iExp = iExp * m_pClientList[iClientH]->m_iQuestRewardAmount[iQuest];

					GetExp(iClientH, iExp);

					m_pClientList[iClientH]->m_iContribution += m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest[iQuest]]->m_iContribution;
					SendNotifyMsg(0, iClientH, DEF_NOTIFY_CONTRIBPOINTS, m_pClientList[iClientH]->m_iContribution, 0, 0, 0);

					SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTREWARD, 4, 1, iExp,
						"Exp", m_pClientList[iClientH]->m_iContribution);

					_ClearQuestStatus(iClientH, iQuest);
					//Magn0S:: Update quest list after finising
					_SendQuestContents(iClientH);
					return;
				}
				else
				{
					m_pClientList[iClientH]->m_iContribution += m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest[iQuest]]->m_iContribution;
					SendNotifyMsg(0, iClientH, DEF_NOTIFY_CONTRIBPOINTS, m_pClientList[iClientH]->m_iContribution, 0, 0, 0);

					SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTREWARD, 4, 1, 0,
						"                     ", m_pClientList[iClientH]->m_iContribution);

					_ClearQuestStatus(iClientH, iQuest);
					//Magn0S:: Update quest list after finising
					_SendQuestContents(iClientH);
					return;
				}
			}
			else
			{
				SendNotifyMsg(0, iClientH, DEF_NOTIFY_NPCTALK, 101, 0, 0, 0, 0);
			}
		}
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_NPCTALK, 104, 0, 0, 0, 0);
	}
}