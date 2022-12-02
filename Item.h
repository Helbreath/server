// Item.h: interface for the CItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEM_H__211A1360_91B9_11D2_B143_00001C7030A6__INCLUDED_)
#define AFX_ITEM_H__211A1360_91B9_11D2_B143_00001C7030A6__INCLUDED_

#include <windows.h>

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Game.h"
#include "StrTok.h"
#include "TeleportLoc.h"
#include "GlobalDef.h"
#include "OccupyFlag.h"
#include "Tile.h"
#include "StrategicPoint.h"

class CItem  
{
public:
	CItem();
	virtual ~CItem();

	char  m_cName[21];
	
	short m_sIDnum;					// �������� ���� ��ȣ
	char  m_cItemType;
	char  m_cEquipPos;
	short m_sItemEffectType;     
	short m_sItemEffectValue1, m_sItemEffectValue2, m_sItemEffectValue3; 
	short m_sItemEffectValue4, m_sItemEffectValue5, m_sItemEffectValue6; 
	WORD  m_wMaxLifeSpan;
	short m_sSpecialEffect;
	
	//v1.432 ���߷� ���� ��� ���Ѵ�. ��� Ư�� �ɷ� ��ġ�� ����.
	short m_sSpecialEffectValue1, m_sSpecialEffectValue2; 

	short m_sSprite;
	short m_sSpriteFrame;

	char  m_cApprValue;
	char  m_cSpeed;

	DWORD m_wPrice; 
	WORD  m_wWeight;
	short m_sLevelLimit;	
	char  m_cGenderLimit;

	short m_sRelatedSkill;
	
	char  m_cCategory;
	bool  m_bIsForSale;

	DWORD m_dwCount;
	short m_sTouchEffectType;
	short m_sTouchEffectValue1, m_sTouchEffectValue2, m_sTouchEffectValue3;
	char  m_cItemColor; // v1.4 ���� ������ ������ ����Ǿ���. 
	short m_sItemSpecEffectValue1, m_sItemSpecEffectValue2, m_sItemSpecEffectValue3;
	short m_wCurLifeSpan;
	DWORD m_dwAttribute;				// aaaa bbbb cccc dddd eeee ffff xxxx xxx1 
										// 1: Custom-Made Item flag 
										// a: Item �Ӽ� ���� 
										// b: Item �Ӽ� ����
										// c: Ư�� ������ ���� Flag 
										// d: Ư�� ������ ���� ���� 
										// e: Ư�� ������ �߰� ���� Flag
										// f: Ư�� ������ �߰� ���� ���� 
										 
	bool teamcape, teamboots;

	//Magn0S:: New Variables
	bool bEkSale, bContrbSale, bCoinSale;
	int m_wContribPrice, m_wEkPrice, m_wCoinPrice;

	short m_sNewEffect1, m_sNewEffect2, m_sNewEffect3, m_sNewEffect4;

	int m_iReqStat, m_iQuantStat;
	int m_iClass;

	bool m_bIsHero;
	int m_iHeroBonus;
};

/*
������ �������Ͽ� �ʿ��� ���� - ���� ������

char cItemType
 - �������� ����. �����Ǵ� ������ �Ҹ�Ǵ� �������� ���� ����

char cEquipPos
 - �����Ǵ� �������̶�� �� ��ġ 

short sItemEffectType     
  - �������� ȿ�� ����.  

short sItemEffectValue1, 2, 3 ... 
 - ������ ȿ���� ��ġ 1, 2, 3...

WORD wMaxLifeSpan
 - �������� �ִ� ���� 

short sMaxFixCount
 - �ִ� ��ĥ�� �ִ� Ƚ��

short sSprite
short sSpriteFrame
 - ������ ��������Ʈ ��ȣ 

WORD  wPrice 
 - ������ ���� 
WORD  wWeight
 - ������ ���� 
short sLevelLimit
 - ������ ���� ���� 
char  cGenderLimit
 - ������ ���� ���� 

short m_SM_HitRatio, m_sL_HitRatio
 - ��ǥ�� ���߷� ����ġ (������ ���)

short sRelatedSkill
 - ����� Skill

�÷��̾� ������ ���Ͽ� ����Ǿ�� �� ���� - �α� ������ 

DWORD dwCount
 - �������� ����. (���� ������ ȭ���̳� ����, Gold��� �ǹ̸� ���´�)

WORD wCurLifeSpan
 - ���� ���� ���� 

short sCurFixCount
 - ������� ������ Ƚ�� 

short sTouchEffectType
 - �������� ó�� �ǵ������ �۵��Ǵ� ȿ�� ���� 

short sTouchEffectValue1, 2, 3...
 - ������ ���� ȿ���� ��ġ 

short sItemSpecEffectType
 - �������� Ư��ȿ�� ���� (ex:������ �ɸ� ������ ���ݷ��̳� ���¿� ������ ��������)

short sItemSpecEffectValue1, 2, 3...
 - ������ Ư��ȿ���� ��ġ 1, 2, 3...


�� �������� ���(Equip Position�� Body, Leggings, Arms�� ���)
sItemEffectValue4: ���� Ư�� Ư��ġ ���� ������ ��Ÿ����. 

  10: Str
  11: Dex
  12: Vit
  13: Int
  14: Mag
  15: Chr

sItemEffectValue5: ���� Ư�� Ư��ġ ���� ����



*/

#endif // !defined(AFX_ITEM_H__211A1360_91B9_11D2_B143_00001C7030A6__INCLUDED_)
