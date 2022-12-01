// Mineral.cpp: implementation of the CMineral class.
//
//////////////////////////////////////////////////////////////////////

#include "Mineral.h"

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

CMineral::CMineral(char cType, char cMapIndex, int sX, int sY, int iRemain)
{
	m_cType = cType;
	m_cMapIndex = cMapIndex;
	m_sX = sX;
	m_sY = sY;;
    m_iRemain = iRemain;
	m_iDifficulty = 0;
}

CMineral::~CMineral()
{

}

bool CGame::_bDecodeBuildItemConfigFileContents(char* pData, DWORD dwMsgSize)
{
	char* pContents, * token, cTxt[120];
	char seps[] = "= \t\n";
	char cReadModeA = 0;
	char cReadModeB = 0;
	int  i, iIndex = 0;
	class CStrTok* pStrTok;
	class CItem* pItem;

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
					// Â¾Ã†Ã€ÃŒÃ…Ã› Ã€ÃŒÂ¸Â§ 
					ZeroMemory(m_pBuildItemList[iIndex]->m_cName, sizeof(m_pBuildItemList[iIndex]->m_cName));
					memcpy(m_pBuildItemList[iIndex]->m_cName, token, strlen(token));

					cReadModeB = 2;
					break;

				case 2:
					// Â½ÂºÃ…Â³ ÃÂ¦Ã‡Ã‘Ã„Â¡ 
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format(1).");
						delete[] pContents;
						delete pStrTok;
						return false;
					}

					m_pBuildItemList[iIndex]->m_iSkillLimit = atoi(token);
					cReadModeB = 3;
					break;

				case 3:
					// 
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemID[0] = atoi(token);
					cReadModeB = 4;
					break;

				case 4:
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemCount[0] = atoi(token);
					cReadModeB = 5;
					break;

				case 5:
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemValue[0] = atoi(token);
					cReadModeB = 6;
					break;

				case 6:
					// 
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemID[1] = atoi(token);
					cReadModeB = 7;
					break;

				case 7:
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemCount[1] = atoi(token);
					cReadModeB = 8;
					break;

				case 8:
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemValue[1] = atoi(token);
					cReadModeB = 9;
					break;

				case 9:
					// 
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemID[2] = atoi(token);
					cReadModeB = 10;
					break;

				case 10:
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemCount[2] = atoi(token);
					cReadModeB = 11;
					break;

				case 11:
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemValue[2] = atoi(token);
					cReadModeB = 12;
					break;


				case 12:
					// 
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemID[3] = atoi(token);
					cReadModeB = 13;
					break;

				case 13:
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemCount[3] = atoi(token);
					cReadModeB = 14;
					break;

				case 14:
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemValue[3] = atoi(token);
					cReadModeB = 15;
					break;

				case 15:
					// 
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemID[4] = atoi(token);
					cReadModeB = 16;
					break;

				case 16:
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pBuildItemList[iIndex]->m_iMaterialItemCount[4] = atoi(token);

					cReadModeB = 17;
					break;

				case 17:
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemValue[4] = atoi(token);

					cReadModeB = 18;
					break;


				case 18:
					// 
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemID[5] = atoi(token);
					cReadModeB = 19;
					break;

				case 19:
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pBuildItemList[iIndex]->m_iMaterialItemCount[5] = atoi(token);

					cReadModeB = 20;
					break;

				case 20:
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemValue[5] = atoi(token);

					cReadModeB = 21;
					break;



				case 21:
					// m_iAverageValue
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}

					m_pBuildItemList[iIndex]->m_iAverageValue = atoi(token);

					cReadModeB = 22;
					break;

				case 22:
					// m_iMaxSkill
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}

					m_pBuildItemList[iIndex]->m_iMaxSkill = atoi(token);

					cReadModeB = 23;
					break;

				case 23:
					// m_wAttribute
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return false;
					}

					m_pBuildItemList[iIndex]->m_wAttribute = atoi(token);

					cReadModeA = 0;
					cReadModeB = 0;

					pItem = new class CItem;
					if (_bInitItemAttr(pItem, m_pBuildItemList[iIndex]->m_cName) == true) {
						// Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ ÃÂ¸Ã€Ã§Â°Â¡ ÃˆÂ®Ã€ÃŽÂµÃ‡Â¾ÃºÂ´Ã™.
						m_pBuildItemList[iIndex]->m_sItemID = pItem->m_sIDnum;

						// ÃƒÃ–Â´Ã« Â°Â¡ÃÃŸÃ„Â¡Â°Âª Â°Ã¨Â»Ãª
						for (i = 0; i < 6; i++)
							m_pBuildItemList[iIndex]->m_iMaxValue += (m_pBuildItemList[iIndex]->m_iMaterialItemValue[i] * 100);

						iIndex++;
					}
					else {
						// Ã€ÃŒÂ·Â± Ã€ÃŒÂ¸Â§Ã€Â» Â°Â¡ÃÃ¸ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ ÃÂ¸Ã€Ã§Ã‡ÃÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™. Â¿Â¡Â·Â¯ 
						wsprintf(G_cTxt, "(!!!) CRITICAL ERROR! BuildItem configuration file error - Not Existing Item(%s)", m_pBuildItemList[iIndex]->m_cName);
						PutLogList(G_cTxt);

						delete m_pBuildItemList[iIndex];
						m_pBuildItemList[iIndex] = 0;

						delete[] pContents;
						delete pStrTok;
						return false;
					}
					delete pItem;
					break;
				}
				break;

			default:
				break;
			}
		}
		else {
			if (memcmp(token, "BuildItem", 9) == 0) {
				cReadModeA = 1;
				cReadModeB = 1;
				m_pBuildItemList[iIndex] = new class CBuildItem;
			}

		}
		token = pStrTok->pGet();
	}

	delete pStrTok;
	delete[] pContents;

	if ((cReadModeA != 0) || (cReadModeB != 0)) {
		PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file contents error!");
		return false;
	}

	wsprintf(cTxt, "(!) BuildItem(Total:%d) configuration - success!", iIndex);
	PutLogList(cTxt);

	return true;
}

// Centu : agregados items con atributos
void CGame::BuildItemHandler(int iClientH, char* pData)
{
	char* cp, cName[21], cElementItemID[6], cColor;
	int    i, x, z, iMatch, iCount, iPlayerSkillLevel, iResult, iTotalValue, iResultValue, iTemp, iItemCount[DEF_MAXITEMS];
	class  CItem* pItem;
	bool   bFlag, bItemFlag[6];
	double dV1, dV2, dV3;
	DWORD  dwTemp, dwTemp2, dwType, dwValue;
	WORD   wTemp;

	if (m_pClientList[iClientH] == 0) return;
	m_pClientList[iClientH]->m_iSkillMsgRecvCount++;

	cp = (char*)(pData + 11);
	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 20);
	cp += 20;

	ZeroMemory(cElementItemID, sizeof(cElementItemID));
	cElementItemID[0] = *cp;
	cp++;
	cElementItemID[1] = *cp;
	cp++;
	cElementItemID[2] = *cp;
	cp++;
	cElementItemID[3] = *cp;
	cp++;
	cElementItemID[4] = *cp;
	cp++;
	cElementItemID[5] = *cp;
	cp++;

	bFlag = true;
	while (bFlag == true) {
		bFlag = false;
		for (i = 0; i <= 4; i++) {
			if ((cElementItemID[i] == -1) && (cElementItemID[i + 1] != -1)) {
				cElementItemID[i] = cElementItemID[i + 1];
				cElementItemID[i + 1] = -1;
				bFlag = true;
			}
		}
	}

	for (i = 0; i < 6; i++) bItemFlag[i] = false;

	iPlayerSkillLevel = m_pClientList[iClientH]->m_cSkillMastery[13];
	iResult = iDice(1, 100);

	if (iResult > iPlayerSkillLevel) {
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_BUILDITEMFAIL, 0, 0, 0, 0);
		return;
	}

	for (i = 0; i < 6; i++) {
		if (cElementItemID[i] != -1) {
			if ((cElementItemID[i] < 0) || (cElementItemID[i] > DEF_MAXITEMS)) return;
			if (m_pClientList[iClientH]->m_pItemList[cElementItemID[i]] == 0) return;
		}
	}
	for (i = 0; i < DEF_MAXBUILDITEMS; i++) {
		if (m_pBuildItemList[i] != 0) {
			if (memcmp(m_pBuildItemList[i]->m_cName, cName, 20) == 0) {
				if (m_pBuildItemList[i]->m_iSkillLimit > m_pClientList[iClientH]->m_cSkillMastery[13]) return;

				for (x = 0; x < DEF_MAXITEMS; x++) {
					if (m_pClientList[iClientH]->m_pItemList[x] != 0)
						iItemCount[x] = m_pClientList[iClientH]->m_pItemList[x]->m_dwCount;
					else iItemCount[x] = 0;
				}

				iMatch = 0;
				iTotalValue = 0;

				for (x = 0; x < 6; x++) {
					if (m_pBuildItemList[i]->m_iMaterialItemCount[x] == 0) {
						iMatch++;
					}
					else {
						for (z = 0; z < 6; z++) {
							if ((cElementItemID[z] != -1) && (bItemFlag[z] == false)) {

								if ((m_pClientList[iClientH]->m_pItemList[cElementItemID[z]]->m_sIDnum == m_pBuildItemList[i]->m_iMaterialItemID[x]) &&
									(m_pClientList[iClientH]->m_pItemList[cElementItemID[z]]->m_dwCount >= m_pBuildItemList[i]->m_iMaterialItemCount[x]) &&
									(iItemCount[cElementItemID[z]] > 0)) {
									iTemp = m_pClientList[iClientH]->m_pItemList[cElementItemID[z]]->m_sItemSpecEffectValue2;
									if (iTemp > m_pClientList[iClientH]->m_cSkillMastery[13]) {
										iTemp = iTemp - (iTemp - m_pClientList[iClientH]->m_cSkillMastery[13]) / 2;
									}

									iTotalValue += (iTemp * m_pBuildItemList[i]->m_iMaterialItemValue[x]);
									iItemCount[cElementItemID[z]] -= m_pBuildItemList[i]->m_iMaterialItemCount[x];
									iMatch++;
									bItemFlag[z] = true;

									break;
								}
							}
						}
					}
				}

				if (iMatch != 6) {
					SendNotifyMsg(0, iClientH, DEF_NOTIFY_BUILDITEMFAIL, 0, 0, 0, 0);
					return;
				}

				dV2 = (double)m_pBuildItemList[i]->m_iMaxValue;
				if (iTotalValue <= 0)
					dV3 = 1.0f;
				else dV3 = (double)iTotalValue;
				dV1 = (double)(dV3 / dV2) * 100.0f;

				iTotalValue = (int)dV1;

				pItem = new class CItem;
				if (_bInitItemAttr(pItem, m_pBuildItemList[i]->m_cName) == false) {
					delete pItem;
					return;
				}

				dwTemp = pItem->m_dwAttribute;
				dwTemp = dwTemp & 0xFFFFFFFE;
				dwTemp = dwTemp | 0x00000001;
				pItem->m_dwAttribute = dwTemp;

				if (pItem->m_cItemType == DEF_ITEMTYPE_MATERIAL) {
					iTemp = iDice(1, (iPlayerSkillLevel / 2) + 1) - 1;
					pItem->m_sItemSpecEffectValue2 = (iPlayerSkillLevel / 2) + iTemp;
					pItem->m_sTouchEffectType = DEF_ITET_ID;
					pItem->m_sTouchEffectValue1 = iDice(1, 100000);
					pItem->m_sTouchEffectValue2 = iDice(1, 100000);
					pItem->m_sTouchEffectValue3 = (short)timeGetTime();

				}
				else {
					dwTemp = pItem->m_dwAttribute;
					dwTemp = dwTemp & 0x0000FFFF;

					dwTemp2 = (WORD)m_pBuildItemList[i]->m_wAttribute;
					dwTemp2 = dwTemp2 << 16;

					dwTemp = dwTemp | dwTemp2;
					pItem->m_dwAttribute = dwTemp;

					iResultValue = (iTotalValue - m_pBuildItemList[i]->m_iAverageValue);
					if (iResultValue > 0) {
						dV2 = (double)iResultValue;
						dV3 = (double)(100.0f - m_pBuildItemList[i]->m_iAverageValue);
						dV1 = (dV2 / dV3) * 100.0f;
						pItem->m_sItemSpecEffectValue2 = (int)dV1;
					}
					else if (iResultValue < 0) {
						dV2 = (double)(iResultValue);
						dV3 = (double)(m_pBuildItemList[i]->m_iAverageValue);
						dV1 = (dV2 / dV3) * 100.0f;
						pItem->m_sItemSpecEffectValue2 = (int)dV1;
					}
					else pItem->m_sItemSpecEffectValue2 = 0;

					dV2 = (double)pItem->m_sItemSpecEffectValue2;
					dV3 = (double)pItem->m_wMaxLifeSpan;
					dV1 = (dV2 / 100.0f) * dV3;

					iTemp = (int)pItem->m_wMaxLifeSpan;
					iTemp += (int)dV1;

					pItem->m_sTouchEffectType = DEF_ITET_ID;
					pItem->m_sTouchEffectValue1 = iDice(1, 100000);
					pItem->m_sTouchEffectValue2 = iDice(1, 100000);
					pItem->m_sTouchEffectValue3 = (short)timeGetTime();

					// Centuu - VER ENDURANCE
					pItem->m_wCurLifeSpan = pItem->m_wMaxLifeSpan;

					if (iTemp <= 0)
						wTemp = 1;
					else wTemp = (WORD)iTemp;

					if (wTemp <= pItem->m_wMaxLifeSpan * 2) {
						//pItem->m_wMaxLifeSpan = wTemp;
						pItem->m_sItemSpecEffectValue1 = (short)wTemp;
						//pItem->m_wCurLifeSpan = pItem->m_wMaxLifeSpan;
					}
					else pItem->m_sItemSpecEffectValue1 = (short)pItem->m_wMaxLifeSpan;
					
					if (pItem->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK || pItem->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK_ARROW) {
						iResult = iDice(1, 1020);
						if ((iResult >= 1) && (iResult <= 29)) {
							dwType = 10;
							cColor = 5;
						}
						else if ((iResult >= 30) && (iResult <= 99)) {
							dwType = 12;
							cColor = 5;
						}
						else if ((iResult >= 100) && (iResult <= 249)) {
							dwType = 7;
							cColor = 6;
						}
						else if ((iResult >= 250) && (iResult <= 449)) {
							dwType = 7;
							cColor = 6;
						}
						else if ((iResult >= 450) && (iResult <= 649)) {
							dwType = 7;
							cColor = 6;
						}
						else if ((iResult >= 650) && (iResult <= 809)) {
							dwType = 7;
							cColor = 6;
						}
						else if ((iResult >= 810) && (iResult <= 969)) {
							dwType = 9;
							cColor = 8;
						}
						else if ((iResult >= 970) && (iResult <= 999)) {
							dwType = 9;
							cColor = 8;
						}
						else if ((iResult >= 1000) && (iResult <= 1020)) {
							dwType = 15; // Magic
							cColor = 10; // Black Color
						}

						pItem->m_cItemColor = cColor;

						if (dwType == 15) {
							iResult = iDice(1, 3060);
							if ((iResult >= 1) && (iResult < 1000))           dwValue = 1;  // Fire-Strike
							else if ((iResult >= 1000) && (iResult < 1740))  dwValue = 2;  // Lightning
							else if ((iResult >= 1740) && (iResult < 2240))  dwValue = 3;  // Chill-Wind
							else if ((iResult >= 2240) && (iResult < 2540))  dwValue = 4;  // Ice-Strike
							else if ((iResult >= 2540) && (iResult < 2740))  dwValue = 5;  // Energy-Strike
							else if ((iResult >= 2740) && (iResult < 2840))  dwValue = 6;  // Mass-Fire-Strike
							else if ((iResult >= 2840) && (iResult < 2890))  dwValue = 7;  // Mass-Chill-Wind
							else if ((iResult >= 2890) && (iResult < 2930))  dwValue = 8;  // Earthworm-Strike
							else if ((iResult >= 2930) && (iResult < 2960))  dwValue = 9;  // Bloody-Shock-Wave
							else if ((iResult >= 2960) && (iResult < 2970))  dwValue = 10; // Mass-Ice-Strike
							else if ((iResult >= 2970) && (iResult < 2980))  dwValue = 11; // Lightning-Strike
							else if ((iResult >= 2980) && (iResult < 2990))  dwValue = 12; // Ice-Whirlwind
							else if ((iResult >= 2990) && (iResult < 3009))  dwValue = 13; // Meteor-Strike
							else if ((iResult >= 3010) && (iResult < 3025))  dwValue = 14; // Mass-Magic-Missile
							else if ((iResult >= 3025) && (iResult < 3045))  dwValue = 15; // Blizzard
							else if ((iResult >= 3045) && (iResult <= 3060))  dwValue = 16; // Earth-Shock-Wave
							
						}
						else {
							iResult = iDice(1, 3000);
							if ((iResult >= 1) && (iResult < 1000))           dwValue = 1;  // 10000/29348 = 34%
							else if ((iResult >= 1000) && (iResult < 1740))  dwValue = 2;  // 6600/29348 = 22.4%
							else if ((iResult >= 1740) && (iResult < 2240))  dwValue = 3;  // 4356/29348 = 14.8%
							else if ((iResult >= 2240) && (iResult < 2540))  dwValue = 4;  // 2874/29348 = 9.7%
							else if ((iResult >= 2540) && (iResult < 2740))  dwValue = 5;  // 1897/29348 = 6.4%
							else if ((iResult >= 2740) && (iResult < 2840))  dwValue = 6;  // 1252/29348 = 4.2%
							else if ((iResult >= 2840) && (iResult < 2890))  dwValue = 7;  // 826/29348 = 2.8%
							else if ((iResult >= 2890) && (iResult < 2930))  dwValue = 8;  // 545/29348 = 1.85%
							else if ((iResult >= 2930) && (iResult < 2950))  dwValue = 9;  // 360/29348 = 1.2%
							else if ((iResult >= 2950) && (iResult < 2960))  dwValue = 10; // 237/29348 = 0.8%
							else if ((iResult >= 2960) && (iResult < 2970))  dwValue = 11; // 156/29348 = 0.5%
							else if ((iResult >= 2970) && (iResult < 2980))  dwValue = 12; // 103/29348 = 0.3%
							else if ((iResult >= 2980) && (iResult < 2990))  dwValue = 13; // 68/29348 = 0.1%
							else if ((iResult >= 2990) && (iResult < 2995))  dwValue = 14; // 68/29348 = 0.1%
							else if ((iResult >= 2995) && (iResult <= 3000))  dwValue = 15; // 68/29348 = 0.1%
							else dwValue = 1; // v2.03 906
							
						}

						switch (dwType) {
						case 1: 
							if (dwValue <= 5) dwValue = 5;
							break;
						case 2: 
						case 6: 
							if (dwValue <= 4) dwValue = 4;
							break;
						case 8:                
							if (dwValue <= 2) dwValue = 2;
							break;

						}
						//if (dwValue > 7) dwValue = 7;

						pItem->m_dwAttribute = 0;
						dwType = dwType << 20;
						dwValue = dwValue << 16;
						pItem->m_dwAttribute = pItem->m_dwAttribute | dwType | dwValue;
					}

					else if (pItem->m_sItemEffectType == DEF_ITEMEFFECTTYPE_DEFENSE) {
						iResult = iDice(1, 1000);
						if ((iResult >= 1) && (iResult <= 599))          dwType = 8;
						else if ((iResult >= 600) && (iResult <= 899))  dwType = 6;
						else if ((iResult >= 900) && (iResult <= 954))  dwType = 11; 
						else if ((iResult >= 955) && (iResult <= 1000)) dwType = 12; 

						iResult = iDice(1, 3000);
						if ((iResult >= 1) && (iResult < 1000))           dwValue = 1;  // 10000/29348 = 34%
						else if ((iResult >= 1000) && (iResult < 1740))  dwValue = 2;  // 6600/29348 = 22.4%
						else if ((iResult >= 1740) && (iResult < 2240))  dwValue = 3;  // 4356/29348 = 14.8%
						else if ((iResult >= 2240) && (iResult < 2540))  dwValue = 4;  // 2874/29348 = 9.7%
						else if ((iResult >= 2540) && (iResult < 2740))  dwValue = 5;  // 1897/29348 = 6.4%
						else if ((iResult >= 2740) && (iResult < 2840))  dwValue = 6;  // 1252/29348 = 4.2%
						else if ((iResult >= 2840) && (iResult < 2890))  dwValue = 7;  // 826/29348 = 2.8%
						else if ((iResult >= 2890) && (iResult < 2930))  dwValue = 8;  // 545/29348 = 1.85%
						else if ((iResult >= 2930) && (iResult < 2950))  dwValue = 9;  // 360/29348 = 1.2%
						else if ((iResult >= 2950) && (iResult < 2960))  dwValue = 10; // 237/29348 = 0.8%
						else if ((iResult >= 2960) && (iResult < 2970))  dwValue = 11; // 156/29348 = 0.5%
						else if ((iResult >= 2970) && (iResult < 2980))  dwValue = 12; // 103/29348 = 0.3%
						else if ((iResult >= 2980) && (iResult < 2990))  dwValue = 13; // 68/29348 = 0.1%
						else if ((iResult >= 2990) && (iResult < 2995))  dwValue = 14; // 68/29348 = 0.1%
						else if ((iResult >= 2995) && (iResult <= 3000))  dwValue = 15; // 68/29348 = 0.1%
						else dwValue = 1; // v2.03 906 

						switch (dwType) {
						case 6: 
							if (dwValue <= 4) dwValue = 4;
							break;
						case 8: 
							if (dwValue <= 2) dwValue = 2;
							break;

						case 11:
						case 12:
							//dwValue = (dwValue + 1) / 2;
							//if (dwValue < 1) dwValue = 1;
							//if (dwValue > 2) dwValue = 2;
							break;
						}
						//if (dwValue > 7) dwValue = 7;

						pItem->m_dwAttribute = 0;
						dwType = dwType << 20;
						dwValue = dwValue << 16;
						pItem->m_dwAttribute = pItem->m_dwAttribute | dwType | dwValue;

						if (iDice(1, 100) >= m_iRareDropRate) {

							iResult = iDice(1, 1000);
							if ((iResult >= 1) && (iResult <= 99)) {
								dwType = 8;
								cColor = 15;
							}
							else if ((iResult >= 100) && (iResult <= 399)) {
								dwType = 8;
								cColor = 15;
							}
							else if ((iResult >= 400) && (iResult <= 549)) {
								dwType = 8;
								cColor = 15;
							}
							else if ((iResult >= 550) && (iResult <= 649)) {
								dwType = 9;
								cColor = 10;
							}
							else if ((iResult >= 650) && (iResult <= 749)) {
								dwType = 9;
								cColor = 10;
							}
							else if ((iResult >= 750) && (iResult <= 939)) {
								dwType = 9;
								cColor = 10;
							}
							else if ((iResult >= 940) && (iResult <= 979)) {
								dwType = 8;
								cColor = 15;
							}
							else if ((iResult >= 980) && (iResult <= 1000)) {
								dwType = 9;
								cColor = 10;
							}

							pItem->m_cItemColor = cColor;

							iResult = iDice(1, 3000);
							if ((iResult >= 1) && (iResult < 1000))           dwValue = 1;  // 10000/29348 = 34%
							else if ((iResult >= 1000) && (iResult < 1740))  dwValue = 2;  // 6600/29348 = 22.4%
							else if ((iResult >= 1740) && (iResult < 2240))  dwValue = 3;  // 4356/29348 = 14.8%
							else if ((iResult >= 2240) && (iResult < 2540))  dwValue = 4;  // 2874/29348 = 9.7%
							else if ((iResult >= 2540) && (iResult < 2740))  dwValue = 5;  // 1897/29348 = 6.4%
							else if ((iResult >= 2740) && (iResult < 2840))  dwValue = 6;  // 1252/29348 = 4.2%
							else if ((iResult >= 2840) && (iResult < 2890))  dwValue = 7;  // 826/29348 = 2.8%
							else if ((iResult >= 2890) && (iResult < 2930))  dwValue = 8;  // 545/29348 = 1.85%
							else if ((iResult >= 2930) && (iResult < 2950))  dwValue = 9;  // 360/29348 = 1.2%
							else if ((iResult >= 2950) && (iResult < 2960))  dwValue = 10; // 237/29348 = 0.8%
							else if ((iResult >= 2960) && (iResult < 2970))  dwValue = 11; // 156/29348 = 0.5%
							else if ((iResult >= 2970) && (iResult < 2980))  dwValue = 12; // 103/29348 = 0.3%
							else if ((iResult >= 2980) && (iResult < 2990))  dwValue = 13; // 68/29348 = 0.1%
							else if ((iResult >= 2990) && (iResult < 2995))  dwValue = 14; // 68/29348 = 0.1%
							else if ((iResult >= 2995) && (iResult <= 3000))  dwValue = 15; // 68/29348 = 0.1%
							else dwValue = 1; // v2.03 906 

							switch (dwType) {
							case 1: 
							case 3: 
							case 7: 
							case 8: 
							case 9: 
								if (dwValue <= 3) dwValue = 3;
								break;
							}
							//if (dwValue > 7) dwValue = 7;

							dwType = dwType << 12;
							dwValue = dwValue << 8;
							pItem->m_dwAttribute = pItem->m_dwAttribute | dwType | dwValue;
						}
					}
				}
				wsprintf(G_cTxt, "Custom-Item(%s) Value(%d) Life(%d/%d)", pItem->m_cName, pItem->m_sItemSpecEffectValue2, pItem->m_wCurLifeSpan, pItem->m_wMaxLifeSpan);
				PutLogList(G_cTxt);

				bAddItem(iClientH, pItem, 0);
				SendNotifyMsg(0, iClientH, DEF_NOTIFY_BUILDITEMSUCCESS, pItem->m_sItemSpecEffectValue2, pItem->m_cItemType, 0, 0); // Integer¸¦ Àü´ÞÇÏ±â À§ÇØ

				for (x = 0; x < 6; x++) {
					if (cElementItemID[x] != -1) {
						if (m_pClientList[iClientH]->m_pItemList[cElementItemID[x]] == 0) {
							wsprintf(G_cTxt, "(?) Char(%s) ElementItemID(%d)", m_pClientList[iClientH]->m_cCharName, cElementItemID[x]);
							PutLogFileList(G_cTxt);
						}
						else {
							iCount = m_pClientList[iClientH]->m_pItemList[cElementItemID[x]]->m_dwCount - m_pBuildItemList[i]->m_iMaterialItemCount[x];
							if (iCount < 0) iCount = 0;
							SetItemCount(iClientH, cElementItemID[x], iCount);
						}
					}
				}
				if (m_pBuildItemList[i]->m_iMaxSkill > m_pClientList[iClientH]->m_cSkillMastery[13]) {
					CalculateSSN_SkillIndex(iClientH, 13, 1);
				}
				GetExp(iClientH, iDice(1, (m_pBuildItemList[i]->m_iSkillLimit / 4)));

				break;
			}
		}
	}
}

void CGame::MineralGenerator()
{
	int i, iP, tX, tY, iRet;

	for (i = 0; i < DEF_MAXMAPS; i++) {
		if ((iDice(1, 4) == 1) && (m_pMapList[i] != 0) &&
			(m_pMapList[i]->m_bMineralGenerator == true) &&
			(m_pMapList[i]->m_iCurMineral < m_pMapList[i]->m_iMaxMineral)) {

			iP = iDice(1, m_pMapList[i]->m_iTotalMineralPoint) - 1;
			if ((m_pMapList[i]->m_MineralPointList[iP].x == -1) || (m_pMapList[i]->m_MineralPointList[iP].y == -1)) break;

			tX = m_pMapList[i]->m_MineralPointList[iP].x;
			tY = m_pMapList[i]->m_MineralPointList[iP].y;

			iRet = iCreateMineral(i, tX, tY, m_pMapList[i]->m_cMineralGeneratorLevel);
		}
	}
}

int CGame::iCreateMineral(char cMapIndex, int tX, int tY, char cLevel)
{
	int i, iDynamicHandle, iMineralType;

	if ((cMapIndex < 0) || (cMapIndex >= DEF_MAXMAPS)) return 0;
	if (m_pMapList[cMapIndex] == 0) return 0;

	for (i = 0; i < DEF_MAXMINERALS; i++)
		if (m_pMineral[i] == 0) {
			// ºó °ø°£¿¡ ±¤¹°µ¢ÀÌ¸¦ ¸¸µç´Ù.
			iMineralType = iDice(1, cLevel);
			m_pMineral[i] = new class CMineral(iMineralType, cMapIndex, tX, tY, 1);
			if (m_pMineral[i] == 0) return 0;

			iDynamicHandle = 0;
			switch (iMineralType) {
			default:
				iDynamicHandle = iAddDynamicObjectList(0, 0, DEF_DYNAMICOBJECT_MINERAL1, cMapIndex, tX, tY, 0, i);
				break;

			case 5: // º¸¼®·ù 
			case 6:
				iDynamicHandle = iAddDynamicObjectList(0, 0, DEF_DYNAMICOBJECT_MINERAL2, cMapIndex, tX, tY, 0, i);
				break;
			}

			if (iDynamicHandle == 0) {
				delete m_pMineral[i];
				m_pMineral[i] = 0;
				return 0;
			}
			m_pMineral[i]->m_sDynamicObjectHandle = iDynamicHandle;
			m_pMineral[i]->m_cMapIndex = cMapIndex;

			// ³­ÀÌµµ¿Í ¼ö·®À» ÀÔ·ÂÇÑ´Ù.
			switch (iMineralType) {
			case 1: m_pMineral[i]->m_iDifficulty = 10; m_pMineral[i]->m_iRemain = 20; break;
			case 2: m_pMineral[i]->m_iDifficulty = 15; m_pMineral[i]->m_iRemain = 15; break;
			case 3: m_pMineral[i]->m_iDifficulty = 20; m_pMineral[i]->m_iRemain = 10; break;
			case 4: m_pMineral[i]->m_iDifficulty = 50; m_pMineral[i]->m_iRemain = 8; break;
			case 5: m_pMineral[i]->m_iDifficulty = 70; m_pMineral[i]->m_iRemain = 6; break;
			case 6: m_pMineral[i]->m_iDifficulty = 90; m_pMineral[i]->m_iRemain = 4; break;
			default: m_pMineral[i]->m_iDifficulty = 10; m_pMineral[i]->m_iRemain = 20; break;
			}

			// ¸ÊÀÇ ¹Ì³×¶ö ¼ö Áõ°¡
			m_pMapList[cMapIndex]->m_iCurMineral++;

			return i;
		}

	return 0;
}

void CGame::_CheckMiningAction(int iClientH, int dX, int dY)
{
	short sType;
	DWORD dwRegisterTime;
	int   iDynamicIndex, iSkillLevel, iResult, iItemID;
	class CItem* pItem;
	WORD  wWeaponType;

	if (m_pClientList[iClientH] == 0)  return;

	m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bGetDynamicObject(dX, dY, &sType, &dwRegisterTime, &iDynamicIndex);

	switch (sType) {
	case DEF_DYNAMICOBJECT_MINERAL1:
	case DEF_DYNAMICOBJECT_MINERAL2:
		// ±¤¹°Ã¤Ãë¸¦ ½ÃµµÇÑ Ä³¸¯ÅÍÀÇ Á¶°ÇÀ» »ìÇÉ´Ù. °î±ªÀÌ¸¦ µé°í ÀÖ´Â°¡? ±¤¹° Ã¤Ãë ½ºÅ³Àº? 
		// °î±ªÀÌ¿¡ ÇØ´çÇÏ´Â ¿ÜÇüÀÌ¸é Åë°ú 
		wWeaponType = ((m_pClientList[iClientH]->m_sAppr2 & 0x0FF0) >> 4);
		if (wWeaponType != 25) {
			// ÇÈ¾×½º¸¦ µé°í ÀÖ´Ù. ±¤¹° Ã¤Ãë °¡´É 
			return;
		}

		// ÀüÅõ ¸ð¼ÇÀÌ ¾Æ´Ï´õ¶óµµ ¸®ÅÏ 
		if ((m_pClientList[iClientH]->m_sAppr2 & 0xF000) == 0) return;

		iSkillLevel = m_pClientList[iClientH]->m_cSkillMastery[0];
		if (iSkillLevel == 0) break;

		if (m_pDynamicObjectList[iDynamicIndex] == 0) break;
		// Ä³´Â ±¤¹°ÀÇ ³­ÀÌµµ¸¸Å­ ½ºÅ³À» ³·Ãá´Ù.
		iSkillLevel -= m_pMineral[m_pDynamicObjectList[iDynamicIndex]->m_iV1]->m_iDifficulty;
		if (iSkillLevel <= 0) iSkillLevel = 1;

		iResult = iDice(1, 100);
		if (iResult <= iSkillLevel) {
			// È®·üÀº ¼º°øÇß´Ù. ±¤¹°Ã¤Ãë ±â¼ú Áõ°¡ 
			CalculateSSN_SkillIndex(iClientH, 0, 1);

			// ÇÃ·¹ÀÌ¾îÀÇ ¹ß ¹Ø¿¡ ±¤¹°À» ¶³¾î¶ß¸°´Ù. 
			switch (m_pMineral[m_pDynamicObjectList[iDynamicIndex]->m_iV1]->m_cType) {
			case 1:
				switch (iDice(1, 5)) {
				case 1:
				case 2:
				case 3:
					iItemID = 355; // Coal
					GetExp(iClientH, iDice(1, iDice(1, 3)));
					break;
				case 4:
					iItemID = 357; // IronOre
					GetExp(iClientH, iDice(1, iDice(1, 3)));
					break;
				case 5:
					iItemID = 507; // BlondeStone
					GetExp(iClientH, iDice(1, iDice(1, 3)));
					break;
				}
				break;

			case 2:
				switch (iDice(1, 5)) {
				case 1:
				case 2:
					iItemID = 355; // Coal
					GetExp(iClientH, iDice(1, iDice(1, 3)));
					break;
				case 3:
				case 4:
					iItemID = 357; // IronOre
					GetExp(iClientH, iDice(1, iDice(1, 3)));
					break;
				case 5:
					if (iDice(1, 3) == 2) {
						iItemID = 356; // SilverNugget
						GetExp(iClientH, iDice(1, iDice(1, 4)));
					}
					else {
						iItemID = 507; // BlondeStone
						GetExp(iClientH, iDice(1, iDice(1, 3)));
					}
					break;
				}
				break;

			case 3:
				switch (iDice(1, 6)) {
				case 1:
					iItemID = 355; // Coal
					GetExp(iClientH, iDice(1, iDice(1, 3)));
					break;
				case 2:
				case 3:
				case 4:
				case 5:
					iItemID = 357; // IronOre
					GetExp(iClientH, iDice(1, iDice(1, 3)));
					break;
				case 6:
					if (iDice(1, 8) == 3) {
						if (iDice(1, 2) == 1) {
							iItemID = 356; // SilverNugget
							GetExp(iClientH, iDice(1, iDice(1, 4)));
						}
						else {
							iItemID = 357; // IronOre
							GetExp(iClientH, iDice(1, iDice(1, 3)));
						}
					}
					else {
						iItemID = 357; // IronOre
						GetExp(iClientH, iDice(1, iDice(1, 3)));
					}
					break;
				}
				break;

			case 4:
				switch (iDice(1, 6)) {
				case 1:
					iItemID = 355; // Coal
					GetExp(iClientH, iDice(1, iDice(1, 3)));
					break;
				case 2:
					if (iDice(1, 3) == 2) {
						iItemID = 356; // SilverNugget
						GetExp(iClientH, iDice(1, iDice(1, 4)));
					}
					break;
				case 3:
				case 4:
				case 5:
					iItemID = 357; // IronOre
					GetExp(iClientH, iDice(1, iDice(1, 3)));
					break;
				case 6:
					if (iDice(1, 8) == 3) {
						if (iDice(1, 4) == 3) {
							if (iDice(1, 4) < 3) {
								iItemID = 508; // Mithral
								GetExp(iClientH, iDice(1, iDice(1, 15)));
							}
							else {
								iItemID = 354; // GoldNugget
								GetExp(iClientH, iDice(1, iDice(1, 5)));
							}
						}
						else {
							iItemID = 356; // SilverNugget
							GetExp(iClientH, iDice(1, iDice(1, 4)));
						}
					}
					else {
						if (iDice(1, 2) == 1) {
							iItemID = 354; // GoldNugget
							GetExp(iClientH, iDice(1, iDice(1, 5)));
						}
						else {
							iItemID = 357;  // IronOre
							GetExp(iClientH, iDice(1, iDice(1, 3)));
						}
					}
					break;
				}
				break;

			case 5:
				switch (iDice(1, 19)) {
				case 3:
					iItemID = 352; // Sapphire
					GetExp(iClientH, iDice(1, iDice(2, 3)));
					break;
				default:
					iItemID = 358; // Crystal
					GetExp(iClientH, iDice(1, iDice(2, 3)));
					break;
				}
				break;

			case 6:
				switch (iDice(1, 5)) {
				case 1:
					if (iDice(1, 6) == 3) {
						iItemID = 353; // Emerald 
						GetExp(iClientH, iDice(1, iDice(2, 4)));
					}
					else {
						iItemID = 358; // Crystal
						GetExp(iClientH, iDice(1, iDice(2, 3)));
					}
					break;
				case 2:
					if (iDice(1, 6) == 3) {
						iItemID = 352; // Saphire
						GetExp(iClientH, iDice(1, iDice(2, 4)));
					}
					else {
						iItemID = 358; // Crystal
						GetExp(iClientH, iDice(1, iDice(2, 3)));
					}
					break;
				case 3:
					if (iDice(1, 6) == 3) {
						iItemID = 351; // Ruby
						GetExp(iClientH, iDice(1, iDice(2, 4)));
					}
					else {
						iItemID = 358; // Crystal 
						GetExp(iClientH, iDice(1, iDice(2, 3)));
					}
					break;
				case 4:
					iItemID = 358; // Crystal 
					GetExp(iClientH, iDice(1, iDice(2, 3)));
					break;
				case 5:
					if (iDice(1, 12) == 3) {
						iItemID = 350; // Diamond
						GetExp(iClientH, iDice(1, iDice(2, 5)));
					}
					else {
						iItemID = 358; // Crystal
						GetExp(iClientH, iDice(1, iDice(2, 3)));
					}
					break;
				}
				break;

			}

			pItem = new class CItem;
			if (_bInitItemAttr(pItem, iItemID) == false) {
				delete pItem;
			}
			else {
				// ¾ÆÀÌÅÛÀ» ¼­ÀÖ´Â À§Ä¡¿¡ ¹ö¸°´Ù. 
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
					m_pClientList[iClientH]->m_sY, pItem);
				// ´Ù¸¥ Å¬¶óÀÌ¾ðÆ®¿¡°Ô ¾ÆÀÌÅÛÀÌ ¶³¾îÁø °ÍÀ» ¾Ë¸°´Ù. 
				/*SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
					m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
					pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor);*/ // v1.4

				SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
					m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
					pItem->m_sIDnum, pItem->m_sSpriteFrame, pItem->m_cItemColor, pItem->m_dwAttribute);
			}

			// ±¤¹°ÀÇ ³²Àº ·®À» ÁÙÀÌ°í 0ÀÎ°æ¿ì »èÁ¦ÇÑ´Ù.
			m_pMineral[m_pDynamicObjectList[iDynamicIndex]->m_iV1]->m_iRemain--;
			if (m_pMineral[m_pDynamicObjectList[iDynamicIndex]->m_iV1]->m_iRemain <= 0) {
				// ±¤¹°ÀÌ ¸ðµÎ ¼Ò¸ðµÇ¾ú´Ù. Delete Mineral 
				bDeleteMineral(m_pDynamicObjectList[iDynamicIndex]->m_iV1);

				// µ¿Àû °´Ã¼ »èÁ¦
				delete m_pDynamicObjectList[iDynamicIndex];
				m_pDynamicObjectList[iDynamicIndex] = 0;
			}
		}
		break;

	default:
		break;
	}
}

void CGame::bDeleteMineral(int iIndex)
{
	int iDynamicIndex;
	DWORD dwTime;

	dwTime = timeGetTime();

	if (m_pMineral[iIndex] == 0) return;
	iDynamicIndex = m_pMineral[iIndex]->m_sDynamicObjectHandle;
	if (m_pDynamicObjectList[iDynamicIndex] == 0) return;

	SendEventToNearClient_TypeB(MSGID_DYNAMICOBJECT, DEF_MSGTYPE_REJECT, m_pDynamicObjectList[iDynamicIndex]->m_cMapIndex,
		m_pDynamicObjectList[iDynamicIndex]->m_sX, m_pDynamicObjectList[iDynamicIndex]->m_sY,
		m_pDynamicObjectList[iDynamicIndex]->m_sType, iDynamicIndex, 0, (short)0);
	// ¸Ê¿¡¼­ ±¤¹° µ¿Àû °´Ã¼¸¦ »èÁ¦ÇÑ´Ù.
	m_pMapList[m_pDynamicObjectList[iDynamicIndex]->m_cMapIndex]->SetDynamicObject(0, 0, m_pDynamicObjectList[iDynamicIndex]->m_sX, m_pDynamicObjectList[iDynamicIndex]->m_sY, dwTime);
	// ±¤¹°ÀÌ »ç¶óÁ³À¸¹Ç·Î ÀÌµ¿ÀÌ °¡´ÉÇÏ°Ô ÇÑ´Ù. 
	m_pMapList[m_pMineral[iIndex]->m_cMapIndex]->SetTempMoveAllowedFlag(m_pDynamicObjectList[iDynamicIndex]->m_sX, m_pDynamicObjectList[iDynamicIndex]->m_sY, true);

	// ±¤¹° °³Ã¼ ¼ö °¨¼Ò 
	m_pMapList[m_pMineral[iIndex]->m_cMapIndex]->m_iCurMineral--;

	// ±¤¹° °´Ã¼ »èÁ¦ 
	delete m_pMineral[iIndex];
	m_pMineral[iIndex] = 0;
}