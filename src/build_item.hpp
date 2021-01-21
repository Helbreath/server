
#pragma once

#include <cstdint>
#include <string>

namespace hbx
{

class build_item
{
public:
    build_item()
    {
        m_sItemID = -1;

        m_iSkillLimit = 0;

        for (int i = 0; i < 6; i++) {
            m_iMaterialItemID[i] = 0;
            m_iMaterialItemCount[i] = 0;
            m_iMaterialItemValue[i] = 0;
            m_iIndex[i] = -1;
        }

        m_iMaxValue = 0;
        m_iAverageValue = 0;
        m_iMaxSkill = 0;
        m_wAttribute = 0;
    }
    ~build_item() = default;

    std::string name;
    int16_t m_sItemID;

    int32_t  m_iSkillLimit;

    int32_t  m_iMaterialItemID[6];
    int32_t  m_iMaterialItemCount[6];
    int32_t  m_iMaterialItemValue[6];
    int32_t  m_iIndex[6];

    int32_t	 m_iMaxValue;
    int32_t  m_iAverageValue;
    int32_t   m_iMaxSkill;
    uint16_t  m_wAttribute;
};

}
