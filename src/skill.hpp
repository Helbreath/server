
#pragma once

#include <cstdint>
#include "defines.hpp"

namespace hbx
{

class skill
{
public:
	skill() = default;
	~skill() = default;

	std::string name;

	int16_t m_sType = 0;
	int16_t m_sValue1 = 0;
	int16_t m_sValue2 = 0;
	int16_t m_sValue3 = 0;
	int16_t m_sValue4 = 0;
	int16_t m_sValue5 = 0;
	int16_t m_sValue6 = 0;

};

}
