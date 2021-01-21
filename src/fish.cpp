
#include "fish.hpp"
#include "item.hpp"

namespace hbx
{

fish::fish(int cMapIndex, short sX, short sY, short sType, class item * pItem, int iDifficulty)
{
	m_cMapIndex = cMapIndex;
	m_sX = sX;
	m_sY = sY;
	m_sType = sType;
	m_pItem = pItem;

    m_sDynamicObjectHandle = 0;
    m_sEngagingCount = 0;
	m_iDifficulty = iDifficulty;

	if (m_iDifficulty <= 0)
		m_iDifficulty = 1;
}

fish::~fish()
{
	if (m_pItem != nullptr) delete m_pItem;
}

}
