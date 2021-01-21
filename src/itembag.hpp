

#pragma once

#include <vector>
#include <memory>
#include "item.hpp"

namespace hbx
{

class itembag
{
public:
    itembag() = default;
    ~itembag() = default;

    std::vector<std::shared_ptr<item>> itemList;

    uint16_t maxItems = 12;
    uint16_t slot;
    uint64_t itemId;
    uint64_t unique_id;
};

}
