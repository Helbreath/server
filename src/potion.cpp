
#include "potion.hpp"

namespace hbx
{

potion::potion()
{
	int i;

	m_iSkillLimit = 0;
	m_iDifficulty = 0;

	for (i = 0; i < 12; i++)
		m_sArray[i] = -1;
}

}
