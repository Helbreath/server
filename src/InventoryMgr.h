
#pragma once

#include "common.h"
#include "Item.h"
#include "Items.h"
#include "Inventory.h"
#include <list>

#define DEFAULTINVENTORYSIZE 20
#define DEFAULTBANKSIZE 20

//manager for "bags" - this is equiv to the bag bar in game
class InventoryMgr  
{
public:

	InventoryMgr();
	virtual ~InventoryMgr();

	void Init();

	std::vector<Inventory*> bags;//1 default bag + dynamic optionals? (2 max free?)

	uint8_t MaxInventory;
	uint8_t CurrentInventory;
	uint16_t CurrentItems;
	uint16_t MaxItems;

	std::shared_ptr<Item> GetItem(uint32_t itemid);


};
