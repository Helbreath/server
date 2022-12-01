// Teleport.h: interface for the CTeleport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TELEPORT_H__A9554BE2_A96B_11D2_B143_00001C7030A6__INCLUDED_)
#define AFX_TELEPORT_H__A9554BE2_A96B_11D2_B143_00001C7030A6__INCLUDED_

#include <windows.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTeleport  
{
public:
	CTeleport(); 
	virtual ~CTeleport();

	char	m_cNpcname[21];
	char	m_cSourceMap[11];
	char	m_cTargetMap[11];
	int		m_iX, m_iY;
	int		m_iCost;
	int		m_iMinLvl;
	int		m_iMaxLvl;
	int		m_iSide;
	bool	m_bHunter;
	bool	m_bNetural;
	bool	m_bCriminal;
};

#endif // AFX_TELEPORT_H__A9554BE2_A96B_11D2_B143_00001C7030A6__INCLUDED_






