// Tile.h: interface for the CTile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILE_H__12609161_8060_11D2_A8E6_00001C7030A6__INCLUDED_)
#define AFX_TILE_H__12609161_8060_11D2_A8E6_00001C7030A6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <windows.h>
#include "Item.h"

#define DEF_TILE_PER_ITEMS	20 // 12

class CTile  
{												  
public:
	CTile();
	virtual ~CTile();

	bool m_bCanStayHere;

	char  m_cOwnerClass;		// DEF_OT_PLAYER / DEF_OT_NPC
	short m_sOwner;

	char  m_cDeadOwnerClass;	// DEF_OT_PLAYER / DEF_OT_NPC �׾��ִ� �÷��̾�� �̰��� 
	short m_sDeadOwner;

	class CItem * m_pItem[DEF_TILE_PER_ITEMS];
	char  m_cTotalItem;

	WORD  m_wDynamicObjectID;
	short m_sDynamicObjectType;
	DWORD m_dwDynamicObjectRegisterTime;

	bool  m_bIsMoveAllowed, m_bIsTeleport, m_bIsWater, m_bIsFarm, m_bIsTempMoveAllowed;

	// ������ ǥ�ÿ� 
	int   m_iOccupyStatus;    // Aresden�̸� -������, Elvine�̸� +�� ��������. �� ���� �� ������ ���⼺ 
	int   m_iOccupyFlagIndex; // �� Ÿ�� ���� �� �ִ� ���ɱ���� �ε��� 

	// Crusade
	int	  m_iAttribute;		  // Ÿ���� ���� ��Ʈ: ù��° ��Ʈ(���� ��ȣ) �ι�° ��Ʈ(���� ���� ��ȣ) ����° ��Ʈ(��������)
	bool m_bIsApocalypseGate;
	char m_cApocalypseGateMap;
	bool m_bHeldenianWinner;
	
	
};

#endif // !defined(AFX_TILE_H__12609161_8060_11D2_A8E6_00001C7030A6__INCLUDED_)
