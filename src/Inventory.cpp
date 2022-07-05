
#include "Inventory.h"

Inventory::Inventory()
{
	Init();
	MaxItems = DEFAULTBAGSIZE;
}

Inventory::Inventory(uint16_t initcount)
{
	Init();
	MaxItems = initcount;
}

void Inventory::Init()
{
	CurrentItems = MaxItems = 0;
}
 
Inventory::~Inventory()
{

}

uint16_t Inventory::ItemCount()
{
	uint16_t t = 0;
	for (std::shared_ptr<Item> item : Items)
	{
		if (item != nullptr)
			t++;
	}
	return t;
}

std::shared_ptr<Item> Inventory::GetItem(uint16_t slot)
{
	if (slot > MaxItems)
		return nullptr;
	if (Items[slot] == nullptr)
		return nullptr;

	return Items[slot];
}

std::shared_ptr<Item> Inventory::RemoveItem(uint16_t slot)
{
	if ((slot > MaxItems) || (Items[slot] == nullptr))
		return nullptr;

	std::shared_ptr<Item> titem = Items[slot];

	Items[slot].reset();

	CurrentItems--;

	return titem;
}

bool Inventory::AddItem(std::shared_ptr<Item> item)
{
	uint16_t itemcount = ItemCount();
	if ((itemcount >= MaxItems) || (item == nullptr))
		return false;

	for (int16_t i = 0; i < Items.size(); ++i)
	{
		if (Items[i] == nullptr)
		{
			Items[i] = item;
			CurrentItems = itemcount + 1;
			return true;
		}
	}

	return false;
}
