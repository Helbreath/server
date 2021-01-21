
#pragma once

#include <vector>
#include <memory>
#include "item.hpp"

namespace hbx
{

class item;

//individual bags on your character. these hold the items themselves
class inventory
{
public:

	inventory();
	inventory(uint16_t initcount);
	~inventory() = default;

	void Init();

	uint16_t ItemCount();
	bool AddItem(std::shared_ptr<item> item_);
	std::shared_ptr<item> GetItem(uint16_t slot);
	std::shared_ptr<item> RemoveItem(uint16_t slot);

	uint16_t MaxItems;
	uint16_t CurrentItems;
	std::vector<std::shared_ptr<item>> Items;
};

}
