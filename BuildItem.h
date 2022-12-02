// BuildItem.h: interface for the CBuildItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUILDITEM_H__D8858F0E_7737_4E39_9BF9_960C73B0727A__INCLUDED_)
#define AFX_BUILDITEM_H__D8858F0E_7737_4E39_9BF9_960C73B0727A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

class CBuildItem  
{
public:
	CBuildItem();
	virtual ~CBuildItem();

	char  m_cName[21];
	short m_sItemID;

	int  m_iSkillLimit;
	
	int  m_iMaterialItemID[6];
	int  m_iMaterialItemCount[6];
	int  m_iMaterialItemValue[6];		// ��� ������ ����ġ 
	int  m_iIndex[6];

	int	 m_iMaxValue;
	int  m_iAverageValue;	
	int   m_iMaxSkill;					// �� �������� �����ؼ� �ø� �� �ִ� �ִ� ��ų
	WORD  m_wAttribute;					// �������� �Ӽ��� �ο��� �� �ִ�.
};

#endif // !defined(AFX_BUILDITEM_H__D8858F0E_7737_4E39_9BF9_960C73B0727A__INCLUDED_)
