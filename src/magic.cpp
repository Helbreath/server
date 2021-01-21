
#include "magic.hpp"
#include "unit.hpp"
#include "client.hpp"
#include <cmath>

namespace hbx
{

void magic::Effect_RecSP(unit * caster, unit * target, uint32_t dthrow, uint32_t droll, uint32_t dbonus)
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

}
