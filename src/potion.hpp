
#pragma once

#include <string>

namespace hbx
{

class potion
{
public:
	potion();
	~potion() = default;

	std::string name;
	short m_sArray[12];

	int   m_iSkillLimit, m_iDifficulty;

};

}
