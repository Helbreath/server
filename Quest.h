// Quest.h: interface for the CQuest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUEST_H__3E61C780_FF48_11D3_9DC2_00A0CC5B45EE__INCLUDED_)
#define AFX_QUEST_H__3E61C780_FF48_11D3_9DC2_00A0CC5B45EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <winbase.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <memory.h>
#include <direct.h>
#include <vector>

#include "winmain.h"
#include "StrTok.h"
#include "Xsocket.h"
#include "Client.h"
#include "Npc.h"
#include "Map.h"
#include "ActionID.h"
#include "UserMessages.h"
#include "NetMessages.h"
#include "MessageIndex.h"
#include "Misc.h"
#include "Msg.h"
#include "Magic.h"
#include "Skill.h"
#include "DynamicObject.h"
#include "DelayEvent.h"
#include "Version.h"
#include "Fish.h"
#include "DynamicObject.h"
#include "DynamicObjectID.h"
#include "Potion.h"
#include "Mineral.h"
#include "Quest.h"
#include "BuildItem.h"
#include "Teleport.h"
#include "TeleportLoc.h"
#include "GlobalDef.h"
#include "TempNpcItem.h"
#include "GuildsMan.h"
#include "Party.h"
#include "Heldenian.h"
#include "Apocalypse.h"
#include "Crusade.h"

class CQuest  
{
public:
	
	char m_cSide;				// ��� �� ���ϴ� �ӹ��ΰ�? 
	
	int m_iType;				// Quest ���� 
	int m_iTargetType;			// Quest�� ��ǥ ����. �� Type�� ���� �ٸ� ���� ��������. 
	int m_iMaxCount;			// �ִ� ���� Ƚ�� 

	int m_iFrom;				// Quest�� �����ִ� NPC ���� 
	
	int m_iMinLevel;			// Quest�� �ޱ� ���� ���� ����. 
	int m_iMaxLevel;			// Quest�� �ޱ� ���� �ִ� ���� 

	int m_iRequiredSkillNum;	// ���� �Ǵܿ� �ʿ��� ��ų ��ȣ 
	int m_iRequiredSkillLevel;	// ���� �Ǵܿ� �ʿ��� ��ų ���� 

	int m_iTimeLimit; 			// �ð� ���� ������ 
	int m_iAssignType;			// ����Ʈ �Ҵ� Ÿ��. -1�̸� �ƹ�����. 1�̸� Crusade �������϶���.

								// ��ǰ ���� �� ����. 3�� �� 1���� �����ϰ� ������. 0�� �ε����� ��� ����.
	int m_iRewardType[4]; 
	int m_iRewardAmount[4];

	int m_iContribution;		// ���嵵 
	int m_iContributionLimit;	// �ִ� ���嵵 ���� 

	int m_iResponseMode;		// ���� ���: 0(ok) 1(Accept/Decline) 2(Next)

	char m_cTargetName[21];		// ����Ʈ�� ������ �̸�. �� Ȥ�� ĳ���� �̸�  
	int  m_sX, m_sY, m_iRange;	// ����Ʈ�� ������ �� ��� 

	int  m_iQuestID;			// ����Ʈ ID. ���� ����Ʈ�� ������ ����Ǿ��� ��츦 ����� ����.

	int  m_iReqContribution;	// ����Ʈ�� �ñ� ���� ���� ���嵵 ����. 


	//CQuest();
	//virtual ~CQuest();

};

#endif // !defined(AFX_QUEST_H__3E61C780_FF48_11D3_9DC2_00A0CC5B45EE__INCLUDED_)
