
#include "InventoryMgr.h"

InventoryMgr::InventoryMgr()
{
	CurrentItems = MaxItems = CurrentInventory = MaxInventory = 0;
	Init();
}

void InventoryMgr::Init()
{
	bags.push_back(new Inventory(DEFAULTBAGSIZE));
}

InventoryMgr::~InventoryMgr()
{

}

//returns a single item matching param
std::shared_ptr<Item> InventoryMgr::GetItem(uint32_t itemid)
{
	for (Inventory * inv : bags)
	{
		if ((inv != nullptr) && (inv->CurrentItems > 0))
		{
			for (std::shared_ptr<Item> Item : inv->Items)
			{
				if (Item->uid == itemid)
					return Item;
			}
		}
	}
	return nullptr;
}
