
#pragma once

#include <cstdint>

namespace hbx
{

class dynamic_object  
{
public:

	dynamic_object(short sOwner, char cOwnerType, short sType, char cMapIndex, short sX, short sY, uint32_t dwRegisterTime, uint32_t dwLastTime, int iV1)
		: m_sOwner(sOwner)
        , m_cOwnerType(cOwnerType)
		, m_sType(sType)
		, m_cMapIndex(cMapIndex)
		, m_sX(sX)
		, m_sY(sY)
		, m_dwRegisterTime(dwRegisterTime)
		, m_dwLastTime(dwLastTime)
		, m_iCount(0)
		, m_iV1(iV1)
	{
	}

	~dynamic_object() = default;

	int16_t m_sOwner;
	int8_t m_cOwnerType;

	int16_t m_sType;
	int8_t m_cMapIndex;
	int16_t m_sX;
	int16_t m_sY;
	uint32_t m_dwRegisterTime;
	uint32_t m_dwLastTime;

	int32_t m_iCount;
	int32_t m_iV1;
};

}
