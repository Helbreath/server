
#pragma once

#include <cstdint>
#include <string>
#include "defines.hpp"

namespace hbx
{

class item
{
public:
    item(uint32_t count = 1);
    item(uint64_t itemID, item ** itemconfig, uint32_t count = 1);
    item(std::string itemName, item ** itemconfig, uint32_t count = 1);
    ~item() = default;

    void Init();

    static item * CreateItem();

    uint32_t GetWeight(int count) const;
    uint32_t GetWeight()			const { return GetWeight(m_dwCount); }
    uint32_t GetBaseWeight()		const { return GetWeight(1); }
    uint32_t GetManuCompletion()	const { return m_sSpecialEffectValue2 + 100; }
    bool IsValid()		const { return (m_sIDnum != ITEM_INVALID); }
    bool IsLogged() 	const;
    bool IsManued()	const;
    bool IsVortexed()	const { return m_sockets[0] == ITEM_VORTEXGEM; }
    uint32_t GetMaxSockets() const;
    bool AddSocket(item * gem);

    bool InitItemAttr(const std::string pItemName, item ** itemconfig);
    bool InitItemAttr(uint64_t iItemID, item ** itemconfig);
    void InitStats(int iGenLevel);
    void AdjustByStat();
    std::string Dump() const;

    uint16_t m_slot;
    bool initialized;

    std::string name;

    uint64_t m_sIDnum;
    int8_t m_cItemType;
    int8_t m_cEquipPos;
    int16_t m_sItemEffectType;
    int16_t m_sItemEffectValue1, m_sItemEffectValue2, m_sItemEffectValue3;
    int16_t m_sItemEffectValue4, m_sItemEffectValue5, m_sItemEffectValue6;
    uint64_t uid;
    int32_t m_wMaxLifeSpan;
    int32_t m_wCurLifeSpan;
    int16_t m_sSpecialEffect;
    int16_t m_sSpecialEffectValue1;
    int16_t m_sSpecialEffectValue2;

    int16_t m_sSprite;
    int16_t m_sSpriteFrame;

    int8_t m_cApprValue;
    int8_t m_cSpeed;

    int32_t m_wPrice;
    uint16_t m_wWeight;
    int16_t m_sLevelLimit;
    int8_t m_cGenderLimit;

    int16_t m_sRelatedSkill;

    int8_t m_cCategory;
    bool m_bIsForSale;
    bool m_isLogged;

    uint32_t m_dwCount;
    int16_t m_sTouchEffectType;
    int16_t m_sTouchEffectValue1, m_sTouchEffectValue2, m_sTouchEffectValue3;
    uint32_t m_cItemColor;
    int16_t m_sItemSpecEffectValue1, m_sItemSpecEffectValue2, m_sItemSpecEffectValue3;

    uint32_t m_dwAttribute;
    uint16_t m_sockets[MAXITEMSOCKETS];

    bool m_disabled;

    int8_t equipped;
    uint16_t x;
    uint16_t y;

    //Zero stat control on items
    void ControlStats(uint32_t & statType, uint32_t & statValue);
    void ControlStats2(uint32_t & statType, uint32_t & statValue);
};

}
