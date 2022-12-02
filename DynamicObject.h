// DynamicObject.h: interface for the CDynamicObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DYNAMICOBJECT_H__D8E4A581_0C95_11D3_8F1D_00001C7030A6__INCLUDED_)
#define AFX_DYNAMICOBJECT_H__D8E4A581_0C95_11D3_8F1D_00001C7030A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

class CDynamicObject  
{
public:
	CDynamicObject(short sOwner, char cOwnerType, short sType, char cMapIndex, short sX, short sY, DWORD dwRegisterTime, DWORD dwLastTime, int iV1, int iV2, int iV3);
	virtual ~CDynamicObject();

	short m_sOwner;
	char  m_cOwnerType;

	char  cOwnerName[11];

	short m_sType;
	char  m_cMapIndex;
	short m_sX, m_sY; 
	DWORD m_dwRegisterTime;
	DWORD m_dwLastTime;

	int   m_iCount;			// Ư�� ������Ʈ�� ��� ����ϴ� ī���� ���� 
	int   m_iV1, m_iV2, m_iV3;			// �߰� ������ ������ ����Ѵ�.
};

#endif // !defined(AFX_DYNAMICOBJECT_H__D8E4A581_0C95_11D3_8F1D_00001C7030A6__INCLUDED_)
