// StrategicPoint.h: interface for the CStrategicPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRATEGICPOINT_H__FEF1CCB2_D12D_46AD_8034_4AC488E87D72__INCLUDED_)
#define AFX_STRATEGICPOINT_H__FEF1CCB2_D12D_46AD_8034_4AC488E87D72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>


class CStrategicPoint  
{
public:
	CStrategicPoint();
	virtual ~CStrategicPoint();

	int		m_iSide;			// ������ �������� �Ҽ�: 0�̸� �߸�
	int     m_iValue;			// �߿䵵
	int		m_iX, m_iY;			// ��ġ 
};

#endif // !defined(AFX_STRATEGICPOINT_H__FEF1CCB2_D12D_46AD_8034_4AC488E87D72__INCLUDED_)
