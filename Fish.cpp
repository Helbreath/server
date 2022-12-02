// Fish.cpp: implementation of the CFish class.
//
//////////////////////////////////////////////////////////////////////

#include "Fish.h"

#pragma warning (disable : 4996)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFish::CFish(char cMapIndex, short sX, short sY, short sType, class CItem * pItem, int iDifficulty)
{
	m_cMapIndex		= cMapIndex;
	m_sX			= sX;
	m_sY			= sY;
	m_sType			= sType;
	m_pItem			= pItem;

	m_sEngagingCount = 0;
	m_iDifficulty    = iDifficulty;
	
	if (m_iDifficulty <= 0) 
		m_iDifficulty = 1;
}

CFish::~CFish()
{
	if (m_pItem != 0) delete m_pItem;
}

int CGame::iCreateFish(char cMapIndex, short sX, short sY, short sType, class CItem* pItem, int iDifficulty, DWORD dwLastTime)
{
	int i, iDynamicHandle;

	// ¸ÊÀÇ À§Ä¡°¡ ¹°ÀÎÁö È®ÀÎÇÑ´Ù. 
	if ((cMapIndex < 0) || (cMapIndex >= DEF_MAXMAPS)) return 0;
	if (m_pMapList[cMapIndex] == 0) return 0;
	if (m_pMapList[cMapIndex]->bGetIsWater(sX, sY) == false) return 0;

	for (i = 0; i < DEF_MAXFISHS; i++)
		if (m_pFish[i] == 0) {
			// ºó °ø°£¿¡ ¹°°í±â¸¦ ¸¸µç´Ù.
			m_pFish[i] = new class CFish(cMapIndex, sX, sY, sType, pItem, iDifficulty);
			if (m_pFish[i] == 0) return 0;

			// Dynamic Object¸¦ ¹ß»ý½ÃÅ²´Ù. Owner¿¡ Fish ÀÎµ¦½º¸¦ ³Ö´Â´Ù.
			switch (pItem->m_sIDnum) {
			case 101: // »¡°­¹°°í±â
			case 102: // ÃÊ·Ï¹°°í±â
			case 103: // ³ë¶û¹°°í±â
			case 570:
			case 571:
			case 572:
			case 573:
			case 574:
			case 575:
			case 576:
			case 577:
				iDynamicHandle = iAddDynamicObjectList(i, 0, DEF_DYNAMICOBJECT_FISH, cMapIndex, sX, sY, dwLastTime);
				break;
			default:
				// ¹°°í±â°¡ ¾Æ´Ñ ´Ù¸¥ ¾ÆÀÌÅÛ 
				iDynamicHandle = iAddDynamicObjectList(i, 0, DEF_DYNAMICOBJECT_FISHOBJECT, cMapIndex, sX, sY, dwLastTime);
				break;
			}

			if (iDynamicHandle == 0) {
				delete m_pFish[i];
				m_pFish[i] = 0;
				return 0;
			}
			m_pFish[i]->m_sDynamicObjectHandle = iDynamicHandle;
			m_pMapList[cMapIndex]->m_iCurFish++;

			return i;
		}

	return 0;
}


bool CGame::bDeleteFish(int iHandle, int iDelMode)
{
	int i, iH;
	DWORD dwTime;

	if (m_pFish[iHandle] == 0) return false;

	dwTime = timeGetTime();

	// ¿¬°üµÇ¾î ÀÖ´Â DynamicObject¸¦ »èÁ¦ÇÑ´Ù.
	iH = m_pFish[iHandle]->m_sDynamicObjectHandle;

	if (m_pDynamicObjectList[iH] != 0) {
		SendEventToNearClient_TypeB(MSGID_DYNAMICOBJECT, DEF_MSGTYPE_REJECT, m_pDynamicObjectList[iH]->m_cMapIndex, m_pDynamicObjectList[iH]->m_sX, m_pDynamicObjectList[iH]->m_sY, m_pDynamicObjectList[iH]->m_sType, iH, 0, (short)0);
		// ¸Ê¿¡¼­ »èÁ¦ÇÑ´Ù.
		m_pMapList[m_pDynamicObjectList[iH]->m_cMapIndex]->SetDynamicObject(0, 0, m_pDynamicObjectList[iH]->m_sX, m_pDynamicObjectList[iH]->m_sY, dwTime);
		m_pMapList[m_pDynamicObjectList[iH]->m_cMapIndex]->m_iCurFish--;

		delete m_pDynamicObjectList[iH];
		m_pDynamicObjectList[iH] = 0;
	}

	// ÀÌ ¹°°í±â¿Í ¿¬°áµÇ¾î ÀÖ´Â ÇÃ·¹ÀÌ¾îµé¿¡°Ô ¹°°í±â°¡ »ç¶óÁ® ³¬½Ã°¡ Ãë¼ÒµÇ¾úÀ½À» ¾Ë·ÁÁØ´Ù. 
	for (i = 0; i < DEF_MAXCLIENTS; i++) {
		if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_bIsInitComplete == true) &&
			(m_pClientList[i]->m_iAllocatedFish == iHandle)) {
			// ¸Þ½ÃÁö Àü¼Û 
			SendNotifyMsg(0, i, DEF_NOTIFY_FISHCANCELED, iDelMode, 0, 0, 0);
			ClearSkillUsingStatus(i); // v1.4 ³¬½Ã ½ºÅ³À» Å¬¸®¾îÇÑ´Ù.
		}
	}

	// »èÁ¦ÇÑ´Ù.
	delete m_pFish[iHandle];
	m_pFish[iHandle] = 0;

	return true;
}


int CGame::iCheckFish(int iClientH, char cMapIndex, short dX, short dY)
{
	int i;
	short sDistX, sDistY;

	if (m_pClientList[iClientH] == 0) return 0;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return 0;

	if ((cMapIndex < 0) || (cMapIndex >= DEF_MAXMAPS)) return 0;

	// ¸ÊÀÇ Æ¯Á¤ À§Ä¡ ³»¿¡ ¹°°í±â ´ÙÀÌ³ª¹Í ¿ÀºêÁ§Æ®°¡ ÀÖ´ÂÁö ÆÇ´ÜÇÑ´Ù. 
	for (i = 0; i < DEF_MAXDYNAMICOBJECTS; i++)
		if (m_pDynamicObjectList[i] != 0) {
			sDistX = abs(m_pDynamicObjectList[i]->m_sX - dX);
			sDistY = abs(m_pDynamicObjectList[i]->m_sY - dY);

			if ((m_pDynamicObjectList[i]->m_cMapIndex == cMapIndex) &&
				((m_pDynamicObjectList[i]->m_sType == DEF_DYNAMICOBJECT_FISH) || (m_pDynamicObjectList[i]->m_sType == DEF_DYNAMICOBJECT_FISHOBJECT)) &&
				(sDistX <= 2) && (sDistY <= 2)) {
				// ¹°°í±â ´ÙÀÌ³ª¹Í ¿ÀºêÁ§Æ®¸¦ Ã£¾Ò´Ù. ÀÌÁ¦ ÀÌ ´ÙÀÌ³ª¹Í ¿ÀºêÁ§Æ® ÀÎµ¦½º¸¦ ¼ÒÀ¯ÇÑ FishÀÇ ÀÎµ¦½º¸¦ ¹ÝÈ¯ÇÑ´Ù. 

				if (m_pFish[m_pDynamicObjectList[i]->m_sOwner] == 0) return 0;
				if (m_pFish[m_pDynamicObjectList[i]->m_sOwner]->m_sEngagingCount >= DEF_MAXENGAGINGFISH) return 0;

				// ÀÌ¹Ì ³¬½Ã¸ðµå¿¡ µé¾î°¡ ÀÖ´Â Ä³¸¯ÅÍ´Â Áßº¹ µ¿ÀÛÀÌ ºÒ°¡´É.
				if (m_pClientList[iClientH]->m_iAllocatedFish != 0) return 0;
				if (m_pClientList[iClientH]->m_cMapIndex != cMapIndex) return 0;
				// ÀÌÁ¦ Ä³¸¯ÅÍ¿¡°Ô ³¬½Ã ¸ðµå¸¦ ½ÃÀÛÇÒ °ÍÀ» ÇÒ´çÇÑ´Ù.
				m_pClientList[iClientH]->m_iAllocatedFish = m_pDynamicObjectList[i]->m_sOwner;
				m_pClientList[iClientH]->m_iFishChance = 1;
				// ÀÌ Ä³¸¯ÅÍ´Â ³¬½Ã ½ºÅ³À» »ç¿ëÁßÀÓÀ» ¼³Á¤.
				m_pClientList[iClientH]->m_bSkillUsingStatus[1] = true;

				SendNotifyMsg(0, iClientH, DEF_NOTIFY_EVENTFISHMODE, (m_pFish[m_pDynamicObjectList[i]->m_sOwner]->m_pItem->m_wPrice / 2), m_pFish[m_pDynamicObjectList[i]->m_sOwner]->m_pItem->m_sSprite,
					m_pFish[m_pDynamicObjectList[i]->m_sOwner]->m_pItem->m_sSpriteFrame, m_pFish[m_pDynamicObjectList[i]->m_sOwner]->m_pItem->m_cName);

				// Ä«¿îÆ® Áõ°¡ 
				m_pFish[m_pDynamicObjectList[i]->m_sOwner]->m_sEngagingCount++;

				return i;
			}
		}

	return 0;
}

void CGame::FishProcessor()
{
	int i, iSkillLevel, iResult, iChangeValue;

	// �̺�Ʈ ���� ��尡 �Ҵ�� �÷��̾���� ó���Ѵ�.
	for (i = 0; i < DEF_MAXCLIENTS; i++) {
		if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_bIsInitComplete == true) &&
			(m_pClientList[i]->m_iAllocatedFish != 0)) {

			if (m_pFish[m_pClientList[i]->m_iAllocatedFish] == 0) continue;

			// �� �÷��̾�� �Ҵ�� ����Ⱑ �ִ�. ���� Ȯ���� �����Ͽ� �뺸�� �ش�. 
			// ���� ����� ��ų 
			iSkillLevel = m_pClientList[i]->m_cSkillMastery[1];
			// ������� ���̵��� ���� ��ų ������ ��������. 
			iSkillLevel -= m_pFish[m_pClientList[i]->m_iAllocatedFish]->m_iDifficulty;
			if (iSkillLevel <= 0) iSkillLevel = 1;

			iChangeValue = iSkillLevel / 10;
			if (iChangeValue <= 0) iChangeValue = 1;
			iChangeValue = iDice(1, iChangeValue);

			iResult = iDice(1, 100);
			if (iSkillLevel > iResult) {
				// ��ų �ֻ��� ���� ����. ���� Ȯ�� ����.
				m_pClientList[i]->m_iFishChance += iChangeValue;
				if (m_pClientList[i]->m_iFishChance > 99) m_pClientList[i]->m_iFishChance = 99;

				SendNotifyMsg(0, i, DEF_NOTIFY_FISHCHANCE, m_pClientList[i]->m_iFishChance, 0, 0, 0);
			}
			else {
				// ��ų �ֻ��� ���� ����. ���� Ȯ�� ����.	
				m_pClientList[i]->m_iFishChance -= iChangeValue;
				if (m_pClientList[i]->m_iFishChance < 1) m_pClientList[i]->m_iFishChance = 1;

				SendNotifyMsg(0, i, DEF_NOTIFY_FISHCHANCE, m_pClientList[i]->m_iFishChance, 0, 0, 0);
			}
		}
	}
}

void CGame::ReqGetFishThisTimeHandler(int iClientH)
{
	int iResult, iFishH;
	class CItem* pItem;

	if (m_pClientList[iClientH] == 0) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if (m_pClientList[iClientH]->m_iAllocatedFish == 0) return;
	if (m_pFish[m_pClientList[iClientH]->m_iAllocatedFish] == 0) return;

	// ���� ��ų ��������� ������Ų��.
	m_pClientList[iClientH]->m_bSkillUsingStatus[1] = false;

	iResult = iDice(1, 100);
	if (m_pClientList[iClientH]->m_iFishChance >= iResult) {
		// �� ����⸦ ���µ� �����Ͽ���!

		// ����ġ ���� 
		GetExp(iClientH, iDice(m_pFish[m_pClientList[iClientH]->m_iAllocatedFish]->m_iDifficulty, 5));
		// ��ų ���� 
		CalculateSSN_SkillIndex(iClientH, 1, m_pFish[m_pClientList[iClientH]->m_iAllocatedFish]->m_iDifficulty);

		// ������ �����͸� ���´�.
		pItem = m_pFish[m_pClientList[iClientH]->m_iAllocatedFish]->m_pItem;
		m_pFish[m_pClientList[iClientH]->m_iAllocatedFish]->m_pItem = 0;

		// ���� �������� �ٴڿ� ����߸���.
		m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
			m_pClientList[iClientH]->m_sY,
			pItem);

		// �ٸ� Ŭ���̾�Ʈ���� �������� ������ ���� �˸���. 
		/*SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
			m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
			pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor);*/ // v1.4 color

		SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
			m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
			pItem->m_sIDnum, pItem->m_sSpriteFrame, pItem->m_cItemColor, pItem->m_dwAttribute);

// ���� ���� �޽��� ���� 
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_FISHSUCCESS, 0, 0, 0, 0);
		iFishH = m_pClientList[iClientH]->m_iAllocatedFish;
		m_pClientList[iClientH]->m_iAllocatedFish = 0;

		// ����⸦ ����� ���� 
		bDeleteFish(iFishH, 1); // <- ���⼭ �ٸ� ���ò۵鿡�� �޽����� ���۵� ���̴�.
		return;
	}

	// ���µ� ����! 
	m_pFish[m_pClientList[iClientH]->m_iAllocatedFish]->m_sEngagingCount--;
	SendNotifyMsg(0, iClientH, DEF_NOTIFY_FISHFAIL, 0, 0, 0, 0);

	m_pClientList[iClientH]->m_iAllocatedFish = 0;
}


void CGame::FishGenerator()
{
	int i, iP, tX, tY, iRet;
	char  cItemName[21];
	short sDifficulty;
	DWORD dwLastTime;
	class CItem* pItem;

	for (i = 0; i < DEF_MAXMAPS; i++) {
		if ((iDice(1, 10) == 5) && (m_pMapList[i] != 0) &&
			(m_pMapList[i]->m_iCurFish < m_pMapList[i]->m_iMaxFish)) {

			iP = iDice(1, m_pMapList[i]->m_iTotalFishPoint) - 1;
			if ((m_pMapList[i]->m_FishPointList[iP].x == -1) || (m_pMapList[i]->m_FishPointList[iP].y == -1)) break;

			tX = m_pMapList[i]->m_FishPointList[iP].x + (iDice(1, 3) - 2);
			tY = m_pMapList[i]->m_FishPointList[iP].y + (iDice(1, 3) - 2);

			pItem = new class CItem;
			if (pItem == 0) break;

			// ¸¸µé ¹°°í±â Á¾·ù¿Í ³­ÀÌµµ, Áö¼Ó ½Ã°£À» °áÁ¤ÇÑ´Ù. 
			ZeroMemory(cItemName, sizeof(cItemName));
			switch (iDice(1, 9)) {
			case 1:   strcpy(cItemName, "RedCarp"); sDifficulty = iDice(1, 10) + 20; break;
			case 2:   strcpy(cItemName, "GreenCarp"); sDifficulty = iDice(1, 5) + 10; break;
			case 3:   strcpy(cItemName, "GoldCarp"); sDifficulty = iDice(1, 10) + 1;  break;
			case 4:   strcpy(cItemName, "CrucianCarp"); sDifficulty = 1;  break;
			case 5:   strcpy(cItemName, "BlueSeaBream"); sDifficulty = iDice(1, 15) + 1;  break;
			case 6:   strcpy(cItemName, "RedSeaBream"); sDifficulty = iDice(1, 18) + 1;  break;
			case 7:   strcpy(cItemName, "Salmon"); sDifficulty = iDice(1, 12) + 1;  break;
			case 8:   strcpy(cItemName, "GrayMullet"); sDifficulty = iDice(1, 10) + 1;  break;
			case 9:
				// °¡²û °¡´Ù ³¬À» ¼ö ÀÖ´Â Æ¯¼ö ¾ÆÀÌÅÛ 
				switch (iDice(1, 150)) {
				case 1:
				case 2:
				case 3:
					strcpy(cItemName, "PowerGreenPotion");
					sDifficulty = iDice(5, 4) + 30;
					break;

				case 10:
				case 11:
					strcpy(cItemName, "SuperGreenPotion");
					sDifficulty = iDice(5, 4) + 50;
					break;

				case 20:
					strcpy(cItemName, "Dagger+2");
					sDifficulty = iDice(5, 4) + 30;
					break;

				case 30:
					strcpy(cItemName, "LongSword+2");
					sDifficulty = iDice(5, 4) + 40;
					break;

				case 40:
					strcpy(cItemName, "Scimitar+2");
					sDifficulty = iDice(5, 4) + 50;
					break;

				case 50:
					strcpy(cItemName, "Rapier+2");
					sDifficulty = iDice(5, 4) + 60;
					break;

				case 60:
					strcpy(cItemName, "Flameberge+2");
					sDifficulty = iDice(5, 4) + 60;
					break;

				case 70:
					strcpy(cItemName, "WarAxe+2");
					sDifficulty = iDice(5, 4) + 50;
					break;

				case 90:
					strcpy(cItemName, "Ruby");
					sDifficulty = iDice(5, 4) + 40;
					break;

				case 95:
					strcpy(cItemName, "Diamond");
					sDifficulty = iDice(5, 4) + 40;
					break;
				}
				break;
			}
			dwLastTime = (60000 * 10) + (iDice(1, 3) - 1) * (60000 * 10);

			if (_bInitItemAttr(pItem, cItemName) == true) {
				iRet = iCreateFish(i, tX, tY, 1, pItem, sDifficulty, dwLastTime);
			}
			else {
				delete pItem;
				pItem = 0;
			}
		}
	}
}