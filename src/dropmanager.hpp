// #pragma once
// 
// #include "common.h"
// 
// #define MAXNPCDROPTIERS		3
// #define MAXNPCDROPS			50
// 
// #define ONNPCDEATH			0
// #define ONNPCDELETE			1
// 
// class Npc;
// 
// struct ItemDrop
// {
// 	uint32_t prob;
// 	uint32_t itemID;
// 
// 	void SetDrop(uint32_t itemid, uint32_t probability)
// 	{
// 		itemID = itemid;
// 		prob = probability;
// 	}
// };
// 
// struct NpcDrop
// {
// 	uint32_t dropsNum;
// 	uint32_t dropProbSum;
// 	ItemDrop drops[MAXNPCDROPS];
// 
// 	bool AddDrop(uint32_t itemid, uint32_t probability)
// 	{
// 		if (dropsNum >= MAXNPCDROPS)
// 			return false;
// 		dropProbSum += probability;
// 		drops[dropsNum].SetDrop(itemid, probability);
// 		++dropsNum;
// 		return true;
// 	}
// };
// 
// class DropManager
// {
// public:
// 	DropManager(GServer * server);
// 	~DropManager();
// 	void InitDrops();
// 
// 	uint32_t Roll(const Npc * npc, const uint32_t dropType = ONNPCDEATH) const;
// 	bool HasPrimaryDrop(const Npc * id) const;
// 	bool HasSecondaryDrop(const Npc * id) const;
// 	uint32_t GetSecDropNum(uint16_t npcType) const { return _npcDeleteDrops[npcType].secDropsNum; };
// 
// protected:
// 	void _ParseXML();
// 
// 	GServer * server;
// 
// 	uint32_t _dropCount;
// 	NpcDrop _npcDrops[MAXNPCTYPES];
// 	NpcDrop _npcDeleteDrops[MAXNPCTYPES];
// };
