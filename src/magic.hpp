
#pragma once

#include <cstdint>
#include <string>
#include "defines.hpp"

namespace hbx
{

class unit;

class magic
{
public:
    magic() = default;
    ~magic() = default;

    //char m_cName[31];
    std::string name;

    int16_t magicType = 0;
    uint32_t delayTime = 0;
    uint32_t lastTime = 0;
    int16_t m_sIntLimit = 0;
    int32_t goldCost = 0;
    uint16_t manaCost = 0;
    uint16_t m_sValue[9];
    uint8_t m_hRange = 0;
    uint8_t m_vRange = 0;

    int8_t category = 0;
    int32_t attribute = 0;
    Element element = ELEMENT_NONE;
    uint16_t num = 0;

    void Effect_RecSP(unit * caster, unit * target, uint32_t dthrow, uint32_t droll, uint32_t dbonus);
};

}
