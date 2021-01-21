// 
// #include "GServer.h"
// #include "DropManager.h"
// #include "Npc.h"
// 
// DropManager::DropManager(GServer * server)
// {
// 	this->server = server;
// 	_dropCount = 0;
// 
// 	memset(_npcDrops, 0, sizeof(_npcDrops));
// 	memset(_npcDeleteDrops, 0, sizeof(_npcDrops));
// }
// 
// DropManager::~DropManager(void)
// {
// }
// 
// uint32_t DropManager::Roll(const Npc * npc, uint32_t dropType) const
// {
// 	const NpcDrop * drop;
// 	float factor = 1.0f;
// 
// 	switch (dropType)
// 	{
// 	case ONNPCDEATH:
// 		drop = &_npcDrops[npc->Type()];
// 		break;
// 	case ONNPCDELETE:
// 		drop = &_npcDeleteDrops[npc->Type()];
// 
// // 		if (server->m_iMiddlelandMapIndex != -1 && server->m_iMiddlelandMapIndex == npc->m_cMapIndex)
// // 		{
// // 			factor *= 0.9f;
// // 		}
// 
// 		if (npc->IsHighlyTrained())
// 			factor *= 0.7f;
// 		break;
// 	default:
// 		return ITEM_NONE;
// 	}
// 
// 
// 	long firstDropProb = drop->drops[0].prob * factor;
// 
// 	long dropRoll = dice(1, drop->dropProbSum - (drop->drops[0].prob - firstDropProb));
// 
// 	if (dropRoll <= firstDropProb){
// 		return drop->drops[0].itemID;
// 	}
// 	dropRoll -= firstDropProb;
// 
// 	for (int n = 1; n < drop->dropsNum; n++)
// 	{
// 		if (dropRoll <= drop->drops[n].prob){
// 			return drop->drops[n].itemID;
// 		}
// 		dropRoll -= drop->drops[n].prob;
// 	}
// 
// 	return ITEM_NONE;
// }
// 
// bool DropManager::HasPrimaryDrop(const Npc * npc) const
// {
// 	return (_npcDrops[npc->Type()].dropsNum > 0) ? true : false;
// }
// 
// bool DropManager::HasSecondaryDrop(const Npc * npc) const
// {
// 	return (_npcDeleteDrops[npc->Type()].dropsNum > 0) ? true : false;
// }
