// TeleportLoc.h: interface for the CTeleportLoc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TELEPORTLOC_H__D4288400_F8C0_11D2_9B6C_00001C7030A6__INCLUDED_)
#define AFX_TELEPORTLOC_H__D4288400_F8C0_11D2_9B6C_00001C7030A6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <string>
#include <stdint.h>

class TeleportLoc  
{
public:
	TeleportLoc()
	{
		m_sSrcX   = -1;
		m_sSrcY	  = -1;
		m_sDestX  = -1;								    
		m_sDestY  = -1;
		m_sDestX2 = -1;
		m_sDestY2 = -1;

		m_cDir    = 0;
		m_iV1     = 0;
		m_iV2     = 0;
		m_dwTime  = 0;
		m_dwTime2 = 0;

		m_iNumSummonNpc = 0;
	}

	~TeleportLoc()
	{

	}

	uint16_t m_sSrcX, m_sSrcY;

	std::string  m_cDestMapName;
	std::string  m_cDestMapName2;
	int8_t m_cDir;
	uint16_t m_sDestX,  m_sDestY;
	uint16_t m_sDestX2, m_sDestY2;

	int32_t   m_iV1;			// Guild ID
	int32_t	  m_iV2;

	int32_t   m_iNumSummonNpc;
	uint64_t m_dwTime, m_dwTime2;

};

#endif // !defined(AFX_TELEPORTLOC_H__D4288400_F8C0_11D2_9B6C_00001C7030A6__INCLUDED_)
