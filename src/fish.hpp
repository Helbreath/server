
#pragma once

namespace hbx
{

class item;

class fish
{
public:

	fish(int cMapIndex, short sX, short sY, short sType, item * pItem, int iDifficulty);
	~fish();

	char  m_cMapIndex;
	short m_sX, m_sY;
	short m_sType;
	item * m_pItem;

	short m_sDynamicObjectHandle;

	short m_sEngagingCount;
	int   m_iDifficulty;
};

}
