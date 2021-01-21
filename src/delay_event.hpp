
#pragma once

#include <cstdint>
#include "unit.hpp"
#include "utility.hpp"

namespace hbx
{

class map;
class unit;

class delay_event
{
public:

	delay_event() = default;
	~delay_event() = default;

	int32_t m_iDelayType;
	int32_t m_iEffectType;

	map * m_pMap;
	int32_t m_dX, m_dY;

	std::shared_ptr<unit> target;

	int32_t m_iV1, m_iV2, m_iV3;

	uint64_t m_dwTriggerTime;
};

}
