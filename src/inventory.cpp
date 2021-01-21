
#include "inventory.hpp"
#include "item.hpp"

namespace hbx
{

inventory::inventory()
{
	Init();
	MaxItems = DEFAULTBAGSIZE;
}

inventory::inventory(uint16_t initcount)
{
	Init();
	MaxItems = initcount;
}

void inventory::Init()
{
	CurrentItems = MaxItems = 0;
}

uint16_t inventory::ItemCount()
{
	uint16_t t = 0;
	for (const std::shared_ptr<item> & item : Items)
	{
		if (item != nullptr)
			t++;
	}
	return t;
}

std::shared_ptr<item> inventory::GetItem(uint16_t slot)
{
	if (slot > MaxItems)
		return nullptr;
	if (Items[slot] == nullptr)
		return nullptr;

	return Items[slot];
}

std::shared_ptr<item> inventory::RemoveItem(uint16_t slot)
{
	if ((slot > MaxItems) || (Items[slot] == nullptr))
		return nullptr;

	std::shared_ptr<item> titem = Items[slot];

	Items[slot].reset();

	CurrentItems--;

	return titem;
}

bool inventory::AddItem(std::shared_ptr<item> item_)
{
	uint16_t itemcount = ItemCount();
	if ((itemcount >= MaxItems) || (item_ == nullptr))
		return false;

	for (int16_t i = 0; i < Items.size(); ++i)
	{
		if (Items[i] == nullptr)
		{
			Items[i] = item_;
			CurrentItems = itemcount + 1;
			return true;
		}
	}

	return false;
}

}
