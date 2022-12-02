// Npc.h: interface for the CNpc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPC_H__6F7A5C80_A709_11D2_B143_00001C7030A6__INCLUDED_)
#define AFX_NPC_H__6F7A5C80_A709_11D2_B143_00001C7030A6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <windows.h>
#include <winbase.h>
#include <vector>

#include "Game.h"
#include "Magic.h"
#include "GlobalDef.h"

// 2002-09-06 #1
class CNpcItem
{
 public :
    CNpcItem();

    char  m_cName[21];
    int m_sItemID;    // 2002-09-09 #1 Item ID
    int m_sFirstProbability;
    char  m_cFirstTargetValue;
    int m_sSecondProbability;
    char  m_cSecondTargetValue;
};

class CNpc  
{
public:
	CNpc(char * pName5);
	virtual ~CNpc();

	int m_iSummonSlave;

	int m_iPoisonLevel;
	int m_cPoisonResistance;

	//50Cent - HP Bar
	int m_iMaxHP;
	// Auras
	char m_pMagicConfigList[100];

	char  m_cNpcName[21]; // NPC�� �̸�. ��� �̸����� ��
	char m_cOriginalSide;

	// Taming
	DWORD m_dwTamingTime;
	char  m_cFollowOwnerCharName[11];
	DWORD m_dwSummonWaitTime;

	char m_sAreaSize;
	char  m_cName[6];	// �� �̸��� ���α׷��� ���� �ĺ�����
	char  m_cMapIndex;
	short m_sX, m_sY;
	short m_dX, m_dY;
	short m_vX, m_vY;
	int   m_tmp_iError;
	RECT  m_rcRandomArea;	// MOVETYPE_RANDOMAREA�϶� ������ǥ

	char  m_cDir;
	char  m_cAction;
	char  m_cTurn;

	short m_sType;
	short m_sOriginalType;
	short m_sAppr2;
	int	  m_iStatus;

	DWORD m_dwTime;
	DWORD m_dwActionTime;
	DWORD m_dwHPupTime, m_dwMPupTime;
	DWORD m_dwDeadTime, m_dwRegenTime;

	int  m_iHP;						// Hit Point 
	DWORD  m_iExp;                    // �׿��� ��� ��� ����ġ. ExpDice�� ���� �����ȴ�.

	int  m_iHitDice;				// ������ Hit Dice. �̰Ϳ� ���� HP�� �����ȴ�.
	int  m_iDefenseRatio;			// Defense Ratio
	int  m_iHitRatio;				// HitRatio
	int  m_iMagicHitRatio;			// v1.4 ���� ���� Ratio
	int  m_iMinBravery;			    // �ּ� �밨ġ 
	DWORD m_iExpDiceMin;
	DWORD m_iExpDiceMax;
	int  m_iGoldDiceMin;
	int  m_iGoldDiceMax;

	char m_cSide;					// NPC�� ���� �� 
	char m_cActionLimit;            // �� ���� 1�̸� Move�� �����ϸ� ������ �ʴ´�.  �� ���� 2�� ���������� ������ ������ �ʴ´�. 3�̸� Dummy��. �´� ����, 
	                                // 4�� ������ ���Ǿ�, 5�� ���๰   

	char m_cSize;					// 0: Small-Medium 1: Large
	char m_cAttackDiceThrow;		// ����ġ �ֻ��� ������ ȸ�� 
	char m_cAttackDiceRange;		// ����ġ �ֻ��� ���� 
	char m_cAttackBonus;			// ���� ���ʽ� 
	char m_cBravery;				// NPC�� �밨�� 
	char m_cResistMagic;			// ���� ���� 
	char m_cMagicLevel;				// ���� ���� 
	char m_cDayOfWeekLimit;		    // ������ ��¥ ���� 
	char m_cChatMsgPresence;		// ����� Chat Msg �������� 
	int  m_iMana;                   // �ִ밪�� MagicLevel*30
	int  m_iMaxMana;				// v1.4 ���� �߰��� �͵�.
																    
	char  m_cMoveType;				// NPC�� �̵���� ��� 
	char  m_cBehavior;				// NPC�� �ൿ 
	short m_sBehaviorTurnCount;		// �ൿ ī��Ʈ 
	char  m_cTargetSearchRange;		// NPC���ݸ�ǥ �˻� ���� 

	int   m_iFollowOwnerIndex;		// NPC�� ���󰡰� �ִ� ��ü �ε��� 
	char  m_cFollowOwnerType;		// ���󰡴� ��ü�� ���� (NPC or Player)
	bool  m_bIsSummoned;            // ��ȯ�� NPC��� HP�� ������ �ʴ´�. 
	DWORD m_dwSummonedTime;			// ��ȯ���� �����ð� 

	int   m_iTargetIndex;			// NPC�� ���ݸ�ǥ �ε���
	char  m_cTargetType;			// ���ݸ�ǥ�� ���� (NPC or Player)
	char  m_cCurWaypoint;			// ���� �̵���ġ 
	char  m_cTotalWaypoint;			// �� �̵���ġ ���� 

	int   m_iSpotMobIndex;			// spot-mob-generator���� ��������ٸ� �ε����� ���� 
	int   m_iWayPointIndex[DEF_MAXWAYPOINTS+1];
	char  m_cMagicEffectStatus[DEF_MAXMAGICEFFECTS];

	bool  m_bIsPermAttackMode;		// �� �÷��װ� Ȱ��ȭ �Ǿ� ������ ���ݵ��� ��ǥ�� �ٲ��� �ʴ´�.
	DWORD   m_iNoDieRemainExp;		// ���� �ʰ� ���߽ÿ� ���� �� �ִ� ����ġ 
	int   m_iAttackStrategy; 		// ���� ���� �ΰ����� Ÿ��  
	int   m_iAILevel;				// ���� ���� 

	int   m_iAttackRange;			// ���� ���� 1�̻��̸� ���Ÿ� ������ �����ϴٴ� �� 
	/*
		AI-Level 
			1: ���� �ൿ 
			2: �������� ���� ���� ��ǥ���� ���� 
			3: ���� ��ȣ���� ��ǥ�� ���� ���ݴ�󿡼� ���� 
	*/
	int   m_iAttackCount;			// ���� ���� ���� Ƚ�� ī��Ʈ 
	bool  m_bIsKilled;
	bool  m_bIsUnsummoned;

	int   m_iLastDamage;			// ������ �ױ� ���� ���� �����. ������� �ʹ� ũ�� ���ݼ� ��ᰡ �������� �ʴ´�.
	int   m_iSummonControlMode;		// ��ȯ�� ���� ���: 0�̸� Free, 1 Hold 2 Tgt 
	char  m_cAttribute;				// ���� �Ӽ�:  �� 1 �ϴ� 2 �� 3 �� 4  
	int   m_iAbsDamage;				// ����� ���� ����� ������, ������ ���� ����� ������ 

	int   m_iItemRatio;				// Ư�� ������ Ȯ�� 
	int   m_iAssignedItem;			// �Ҵ�� ������ 

	char  m_cSpecialAbility;		// Ư�� �ɷ� 0: ����  

									/*
case 0: break;
case 1:  "Penetrating Invisibility"
case 2:  "Breaking Magic Protection"
case 3:  "Absorbing Physical Damage"
case 4:  "Absorbing Magical Damage"
case 5:  "Poisonous"
case 6:  "Extremely Poisonous"
case 7:  "Explosive"
case 8:  "Hi-Explosive" 

 ���� �� ���� 60���� ũ�� NPC�� ȿ���ʹ� �����ϹǷ� �����Ѵ�.
									*/

	int	  m_iBuildCount;			// ���� Ÿ���� �������� ��� �ϼ��� ������ ���� ����� Ƚ���� �ǹ�. �ʱⰪ�� m_iMinBravery�̴�.
									// �� ���� 0�̾�߸� ������ �Ѵ�.
	int   m_iManaStock;				// �׷��� ���� ���׷������� ��� ���������� 
	bool  m_bIsMaster;				// ������ �������� ���� 
	int   m_iGuildGUID;				// ����� ����Ʈ�� ��� ��ȯ���� ��� ���̵� �Էµȴ�.
	
	char m_cCropType;
	char m_cCropSkill;

	int   m_iV1;
	char m_cArea;
	
	std::vector<CNpcItem>	m_vNpcItem;

	int m_iNpcItemType;
	int m_iNpcItemMax;
	bool m_bIsBossMob;
};



#endif // !defined(AFX_NPC_H__6F7A5C80_A709_11D2_B143_00001C7030A6__INCLUDED_)

