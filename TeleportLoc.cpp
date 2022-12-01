// TeleportLoc.cpp: implementation of the CTeleportLoc class.
//
//////////////////////////////////////////////////////////////////////

#include "TeleportLoc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTeleportLoc::CTeleportLoc()
{

	ZeroMemory(m_cDestMapName, sizeof(m_cDestMapName));
	m_sSrcX   = -1;
	m_sSrcY	  = -1;
	m_sDestX  = -1;								    
	m_sDestY  = -1;
	m_sDestX2 = -1;
	m_sDestY2 = -1;

	m_iV1     = 0;
	m_iV2     = 0;
	m_dwTime  = 0;
	m_dwTime2 = 0;

}

CTeleportLoc::~CTeleportLoc()
{

}
