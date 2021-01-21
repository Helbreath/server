
#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include "defines.hpp"

namespace hbx
{

class item;
class inventory;

//manager for "bags" - this is equiv to the bag bar in game
class inventory_mgr
{
public:

	inventory_mgr();
	virtual ~inventory_mgr();

	void Init();

	std::vector<inventory *> bags;//1 default bag + dynamic optionals? (2 max free?)

	uint8_t MaxInventory;
	uint8_t CurrentInventory;
	uint16_t CurrentItems;
	uint16_t MaxItems;

	std::shared_ptr<item> GetItem(uint32_t itemid);


};

}
