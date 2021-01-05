
#pragma once

#include <cstdint>
#include "magic.hpp"

namespace hbx
{

class unit
{
public:

    unit() = default;
    ~unit() = default;

    //char name[20];
    std::string name;

    int8_t  m_cMapIndex;
    int16_t m_sX;
    int16_t m_sY;

    int8_t  m_cDir;
    int16_t m_sType;
    int16_t m_sOriginalType;
    int16_t m_sAppr1;
    int16_t m_sAppr2;
    int16_t m_sAppr3;
    int16_t m_sAppr4;
    int32_t   m_iApprColor;
    int16_t m_sStatus;

    uint32_t m_dwTime;

    int32_t  m_iHP;
    int32_t  m_iExp;

    int32_t  m_iDefenseRatio;
    int32_t  m_iHitRatio;

    int8_t m_cSide;

    int32_t  m_iMP;
    int32_t  m_iMaxMP;
    int8_t  m_cMagicEffectStatus[DEF_MAXMAGICEFFECTS];

    bool  m_bIsKilled;

    int32_t   m_iGuildGUID;

    int32_t   m_iV1;
};

}
