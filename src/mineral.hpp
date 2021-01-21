
#pragma once

namespace hbx
{

class mineral
{
public:
    mineral(char cType, char cMapIndex, int sX, int sY, int iRemain)
    {
        m_cType = cType;
        m_cMapIndex = cMapIndex;
        m_sX = sX;
        m_sY = sY;;
        m_iRemain = iRemain;
        m_iDifficulty = 0;
    }

	~mineral() = default;

	char  m_cType;

	char  m_cMapIndex;
	int   m_sX, m_sY;
	int   m_iDifficulty;
	short m_sDynamicObjectHandle;

	int   m_iRemain;
};

}
