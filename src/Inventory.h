
#pragma once

#include "common.h"
#include "Item.h"
#include "Items.h"
#include <vector>

//individual bags on your character. these hold the items themselves
class Inventory  
{
public:

	Inventory();
	Inventory(uint16_t initcount);
	virtual ~Inventory();

	void Init();

	uint16_t ItemCount();
	bool AddItem(shared_ptr<Item> item);
	shared_ptr<Item> GetItem(uint16_t slot);
	shared_ptr<Item> RemoveItem(uint16_t slot);

	uint16_t MaxItems;
	uint16_t CurrentItems;
	std::vector<shared_ptr<Item>> Items;
};
