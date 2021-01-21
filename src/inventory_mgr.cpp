
#include "inventory_mgr.hpp"
#include "item.hpp"
#include "inventory.hpp"

namespace hbx
{

inventory_mgr::inventory_mgr()
{
	CurrentItems = MaxItems = CurrentInventory = MaxInventory = 0;
	Init();
}

void inventory_mgr::Init()
{
	bags.push_back(new inventory(DEFAULTBAGSIZE));
}

inventory_mgr::~inventory_mgr()
{

}

//returns a single item matching param
std::shared_ptr<item> inventory_mgr::GetItem(uint32_t itemid)
{
	for (inventory * inv : bags)
	{
		if ((inv != nullptr) && (inv->CurrentItems > 0))
		{
			for (std::shared_ptr<item> Item : inv->Items)
			{
				if (Item->uid == itemid)
					return Item;
			}
		}
	}
	return nullptr;
}

}
