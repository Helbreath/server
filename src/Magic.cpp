// Magic.cpp: implementation of the CMagic class.
//
//////////////////////////////////////////////////////////////////////

#include "Magic.h"
//#include "maths.h"
#include "Unit.h"
#include "Client.h"
using namespace std;
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Magic::Magic()
{
	m_element = ELEMENT_NONE;
	m_dwDelayTime = m_dwLastTime = 0;
	m_vRange = 0;
	m_sType = 0;
	m_manaCost = 0;
	m_iGoldCost = 0;
	m_hRange = 0;
	m_cCategory = 0;
	m_sIntLimit = 0;
}

Magic::~Magic()
{										    

}

void Magic::Effect_RecSP(Unit * caster, Unit * target, uint32_t dthrow, uint32_t droll, uint32_t dbonus)
{
// 	int iSP, iMaxSP;	
// 
// 	if(!target) return;
// 
// 	iSP = dice(m_sValue[MAGICV_THROW], m_sValue[MAGICV_RANGE]) + m_sValue[MAGICV_BONUS];
// 
// 	if(target->IsPlayer() && !target->m_bIsKilled)
// 	{
// 		Client * player = (Client*)target;
// 
// 		iMaxSP = player->GetMaxSP();
// 		if (player->m_iSP < iMaxSP) {
// 			player->m_iSP += iSP;
// 
// 			if (player->m_iSP > iMaxSP) 
// 				player->m_iSP = iMaxSP;
// 
// 			player->Notify(NULL, NOTIFY_SP, NULL, NULL, NULL, NULL);
// 		}
// 	}
}
