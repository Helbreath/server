#include "Teleport.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTeleport::CTeleport()
{
	ZeroMemory(m_cNpcname, sizeof(m_cNpcname));
	ZeroMemory(m_cSourceMap, sizeof(m_cSourceMap));
	ZeroMemory(m_cTargetMap, sizeof(m_cTargetMap));

	m_iX = m_iY = -5;
	m_iCost = 0;
	m_iMinLvl = 0;
	m_iMaxLvl = 0;
	m_iSide = 0;	// all
	m_bHunter = true;
	m_bNetural = true;
	m_bCriminal = true;
}

CTeleport::~CTeleport()
{										    

}