// Item.cpp: implementation of the CItem class.
//
//////////////////////////////////////////////////////////////////////

#include "Item.h"

//extern class Item ** g_itemConfigList;
//extern HashMap<uint64_t, uint8_t> g_socketMap;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Item::Item(uint32_t count) : m_dwCount(count)
{
	Init();
}

Item::Item(uint64_t itemID, Item ** itemconfig, uint32_t count) : m_dwCount(count)
{
	Init();
	InitItemAttr(itemID, itemconfig);
}

Item::Item(char * itemName, Item ** itemconfig, uint32_t count) : m_dwCount(count)
{
	Init();
	InitItemAttr(itemName, itemconfig);
}

void Item::Init()
{
	m_slot = 0;

	equipped = false;
	x = y = 0;

	m_sSprite = 0;
	m_sSpriteFrame = 0;

	m_sItemEffectValue1 = 0;
	m_sItemEffectValue2 = 0;
	m_sItemEffectValue3 = 0; 

	m_sItemEffectValue4 = 0;
	m_sItemEffectValue5 = 0;
	m_sItemEffectValue6 = 0; 

	m_sTouchEffectType   = 0;
	m_sTouchEffectValue1 = 0;
	m_sTouchEffectValue2 = 0;
	m_sTouchEffectValue3 = 0;

	m_ItemColor = 0;
	m_sItemSpecEffectValue1 = 0;
	m_sItemSpecEffectValue2 = 0;
	m_sItemSpecEffectValue3 = 0;

	m_sSpecialEffectValue1 = 0;
	m_sSpecialEffectValue2 = 0;

	m_wCurLifeSpan = 0;
	m_dwAttribute   = 0;

	for(int i = 0; i < MAXITEMSOCKETS; i++)
		m_sockets[i] = ITEM_NONE;

	m_cCategory = 0;
	m_sIDnum    = ITEM_INVALID;

	m_bIsForSale = false;
	ItemUniqueID = 0;

	m_disabled = false;
}

Item::~Item()
{

}

uint32_t Item::GetWeight(int count) const
{
	if(count < 0) count = 1;

	uint32_t weight = m_wWeight * count;

	if(m_sIDnum == ITEM_GOLD)
		weight /= 20;

	return (weight <= 0) ? 1 : weight;
}

bool Item::IsLogged() const
{
	if(m_sIDnum == ITEM_GOLD && m_isLogged)
		return (m_dwCount > 10000) ? true : false;

	return m_isLogged;
}

bool Item::InitItemAttr(uint64_t itemID, Item ** itemconfig)
{
	if(itemID == ITEM_NONE || itemID == ITEM_INVALID)
		return false;

	for(int i = 0; i < MAXITEMTYPES; i++) 
	{
		if (itemconfig[i] != 0) {
			if (itemID ==  itemconfig[i]->m_sIDnum) {

				m_cName = itemconfig[i]->m_cName;
				m_cItemType         = itemconfig[i]->m_cItemType;
				m_cEquipPos         = itemconfig[i]->m_cEquipPos;
				m_sItemEffectType   = itemconfig[i]->m_sItemEffectType;
				m_sItemEffectValue1 = itemconfig[i]->m_sItemEffectValue1;
				m_sItemEffectValue2 = itemconfig[i]->m_sItemEffectValue2;
				m_sItemEffectValue3 = itemconfig[i]->m_sItemEffectValue3;
				m_sItemEffectValue4 = itemconfig[i]->m_sItemEffectValue4;
				m_sItemEffectValue5 = itemconfig[i]->m_sItemEffectValue5;
				m_sItemEffectValue6 = itemconfig[i]->m_sItemEffectValue6;
				m_wMaxLifeSpan      = itemconfig[i]->m_wMaxLifeSpan;
				m_wCurLifeSpan	   = m_wMaxLifeSpan;
				m_sSpecialEffect    = itemconfig[i]->m_sSpecialEffect;

				m_sSprite           = itemconfig[i]->m_sSprite;
				m_sSpriteFrame      = itemconfig[i]->m_sSpriteFrame;
				m_wPrice            = itemconfig[i]->m_wPrice;
				m_wWeight           = itemconfig[i]->m_wWeight;
				m_cApprValue        = itemconfig[i]->m_cApprValue;
				m_cSpeed            = itemconfig[i]->m_cSpeed;
				m_sLevelLimit       = itemconfig[i]->m_sLevelLimit;
				m_cGenderLimit      = itemconfig[i]->m_cGenderLimit;

				m_sSpecialEffectValue1 = itemconfig[i]->m_sSpecialEffectValue1;
				m_sSpecialEffectValue2 = itemconfig[i]->m_sSpecialEffectValue2;

				m_sRelatedSkill     = itemconfig[i]->m_sRelatedSkill;
				m_cCategory         = itemconfig[i]->m_cCategory;
				m_sIDnum			   = itemconfig[i]->m_sIDnum;

				m_bIsForSale	       = itemconfig[i]->m_bIsForSale;
				m_ItemColor        = itemconfig[i]->m_ItemColor;
				m_isLogged			= itemconfig[i]->m_isLogged;
				return true;
			}
		}
	}

	return false;
}

bool Item::InitItemAttr(const string pItemName, Item ** itemconfig)
{
	string cTmpName = pItemName;

	for(int i = 0; i < MAXITEMTYPES; i++) 
	{
		if (itemconfig[i] != 0) {
			if (cTmpName == itemconfig[i]->m_cName) {

				m_cName = itemconfig[i]->m_cName;
				m_cItemType         = itemconfig[i]->m_cItemType;
				m_cEquipPos         = itemconfig[i]->m_cEquipPos;
				m_sItemEffectType   = itemconfig[i]->m_sItemEffectType;
				m_sItemEffectValue1 = itemconfig[i]->m_sItemEffectValue1;
				m_sItemEffectValue2 = itemconfig[i]->m_sItemEffectValue2;
				m_sItemEffectValue3 = itemconfig[i]->m_sItemEffectValue3;
				m_sItemEffectValue4 = itemconfig[i]->m_sItemEffectValue4;
				m_sItemEffectValue5 = itemconfig[i]->m_sItemEffectValue5;
				m_sItemEffectValue6 = itemconfig[i]->m_sItemEffectValue6;
				m_wMaxLifeSpan      = itemconfig[i]->m_wMaxLifeSpan;
				m_wCurLifeSpan	   = m_wMaxLifeSpan;
				m_sSpecialEffect    = itemconfig[i]->m_sSpecialEffect;

				m_sSprite           = itemconfig[i]->m_sSprite;
				m_sSpriteFrame      = itemconfig[i]->m_sSpriteFrame;
				m_wPrice            = itemconfig[i]->m_wPrice;
				m_wWeight           = itemconfig[i]->m_wWeight;
				m_cApprValue        = itemconfig[i]->m_cApprValue;
				m_cSpeed            = itemconfig[i]->m_cSpeed;
				m_sLevelLimit       = itemconfig[i]->m_sLevelLimit;
				m_cGenderLimit      = itemconfig[i]->m_cGenderLimit;

				m_sSpecialEffectValue1 = itemconfig[i]->m_sSpecialEffectValue1;
				m_sSpecialEffectValue2 = itemconfig[i]->m_sSpecialEffectValue2;

				m_sRelatedSkill     = itemconfig[i]->m_sRelatedSkill;
				m_cCategory         = itemconfig[i]->m_cCategory;
				m_sIDnum			   = itemconfig[i]->m_sIDnum;

				m_bIsForSale	       = itemconfig[i]->m_bIsForSale;
				m_ItemColor        = itemconfig[i]->m_ItemColor;
				m_isLogged			= itemconfig[i]->m_isLogged;
				return true;
			}
		}
	}

	return false;
}
// Whole function is re-done to better suit a high-rate server
// By xRisenx
//TODO: redo this entire function properly
void Item::InitStats(int iGenLevel)
{
// 	uint32_t statType, statValue, stat2Type, stat2Value;
// 	int iResult;
// 
// 	switch(m_sItemEffectType)
// 	{
// 	default:
// 		return;
// 	case ITEMEFFECTTYPE_ATTACK_MAGICITEM: // Battle Mages xRisenx
// 	case ITEMEFFECTTYPE_ATTACK_MANASAVE:
// 	case ITEMEFFECTTYPE_ATTACK:
// 		if(m_sItemEffectType == ITEMEFFECTTYPE_ATTACK_MANASAVE)
// 		{
// 			statType = ITEMSTAT_CASTPROB;
// 			m_cItemColor = 5;
// 		}else
// 		{
// 		   //iResult = dice(1,10000);
// 		   if ((iResult >= 1) && (iResult <= 3049)) { // 30.50%
// 			statType = ITEMSTAT_LIGHT;
// 			m_cItemColor = 1; 
// 		   }
// 		   else if ((iResult >= 3050) && (iResult <= 6122)) { // 30.70%
// 			statType = ITEMSTAT_STRONG;
// 			m_cItemColor = 3;
// 		   }
// 		   else if ((iResult >= 6123) && (iResult <= 6153)) { // 0.30%
// 			statType = ITEMSTAT_RIGHTEOUS;
// 			m_cItemColor = 7;
// 		   }
// 		   else if ((iResult >= 6154) && (iResult <= 7654)) { // 15.00%
// 			statType = ITEMSTAT_AGILE;
// 			m_cItemColor = 1;
// 		   }
// 		   else if ((iResult >= 7655) && (iResult <= 8655)) { // 10.00%
// 			statType = ITEMSTAT_CRITICAL;
// 			m_cItemColor = 5;
// 		   }
// 		   else if ((iResult >= 8656) && (iResult <= 9656)) { // 10.00%
// 			statType = ITEMSTAT_POISONING;
// 			m_cItemColor = 4;
// 		   }
// 		   else if ((iResult >= 9657) && (iResult <= 9857)) { // 2%
// 			statType = ITEMSTAT_SHARP;
// 			m_cItemColor = 6;
// 		   }
// 		   else if ((iResult >= 9858) && (iResult <= 9958)) { // 1%
// 			statType = ITEMSTAT_ANCIENT;
// 			m_cItemColor = 8;
// 		   }
// 		   else if ((iResult >= 9959) && (iResult <= 9979)) { // 0.50%
// 			statType = ITEMSTAT_FLAWLESSSHARP;
// 			m_cItemColor = 2;
// 		   }
// 		   else if ((iResult >= 9980) && (iResult <= 10000)) { // 0.20%
// 			statType = ITEMSTAT_FLAWLESSANCIENT;
// 			m_cItemColor = 13;
// 		}
// 	}
// 
// 		//iResult = dice(1, 10000);
// 		if ((iResult >= 1) && (iResult < 3435)) statValue = 1; // 34.35%
// 		else if ((iResult >= 3435) && (iResult < 6436)) statValue = 2; // 30.00%
// 		else if ((iResult >= 6436) && (iResult < 8437)) statValue = 3; // 20.00%
// 		else if ((iResult >= 8437) && (iResult < 9438)) statValue = 4; // 10.00%
// 		else if ((iResult >= 9438) && (iResult < 9759)) statValue = 5; // 3.20%
// 		else if ((iResult >= 9759) && (iResult < 9840)) statValue = 6; // 1.60%
// 		else if ((iResult >= 9840) && (iResult < 9921)) statValue = 7; // 0.80%
// 		else if ((iResult >= 9921) && (iResult < 9962)) statValue = 8; // 0.40%
// 		else if ((iResult >= 9962) && (iResult < 9983)) statValue = 9; // 0.20%
// 		else if ((iResult >= 9983) && (iResult < 9992)) statValue = 10; // 0.08%
// 		else if ((iResult >= 9992) && (iResult < 9997)) statValue = 11; // 0.04%
// 		else if ((iResult >= 9997) && (iResult < 9999)) statValue = 12; // 0.02%
// 		else if ((iResult >= 9999) && (iResult <= 10000)) statValue = 13; // 0.01% 
// 		else statValue = 1;
// 
// 		switch (statType)
// 		{
// 		/*case ITEMSTAT_CRITICAL:
// 			if (statValue <= 5) statValue = 5;
// 			break;*/
// 		case ITEMSTAT_POISONING:
// 			if (statValue <= 5) statValue = 5;
// 			break;
// 		/*case ITEMSTAT_LIGHT:
// 			if (statValue <= 4) statValue = 4;
// 			break;
// 		case ITEMSTAT_STRONG:
// 			if (statValue <= 2) statValue = 2;
// 			break;*/
// 		}
// 
// 		//if ((iGenLevel <= 2) && (statValue > 7)) statValue = 7;
// 
// 		//SetNibble(m_dwAttribute, 5, statType);
// 		//SetNibble(m_dwAttribute, 4, statValue);
// 
// 
// 		if (/*dice(1,10000) >=*/ 8000) { // Higher the less chance of a 2ndary stat on a weapon.
// 			iResult = dice(1,10000);
// 			if ((iResult >= 1) && (iResult <= 3999))          stat2Type = ITEMSTAT2_HITPROB; // 40.00%
// 			else if ((iResult >= 4000) && (iResult <= 7999))  stat2Type = ITEMSTAT2_CAD; // 40.00%
// 			else if ((iResult >= 8000) && (iResult <= 8999))  stat2Type = ITEMSTAT2_GOLD; // 10.00%
// 			else if ((iResult >= 9000) && (iResult <= 10000)) stat2Type = ITEMSTAT2_EXP; // 10.00%
// 
// 
// 			iResult = dice(1, 10000);
// 			if ((iResult >= 1) && (iResult < 3435)) stat2Value = 1; // 34.35%
// 			else if ((iResult >= 3435) && (iResult < 6436)) stat2Value = 2; // 30.00%
// 			else if ((iResult >= 6436) && (iResult < 8437)) stat2Value = 3; // 20.00%
// 			else if ((iResult >= 8437) && (iResult < 9438)) stat2Value = 4; // 10.00%
// 			else if ((iResult >= 9438) && (iResult < 9759)) stat2Value = 5; // 3.20%
// 			else if ((iResult >= 9759) && (iResult < 9840)) stat2Value = 6; // 1.60%
// 			else if ((iResult >= 9840) && (iResult < 9921)) stat2Value = 7; // 0.80%
// 			else if ((iResult >= 9921) && (iResult < 9962)) stat2Value = 8; // 0.40%
// 			else if ((iResult >= 9962) && (iResult < 9983)) stat2Value = 9; // 0.20%
// 			else if ((iResult >= 9983) && (iResult < 9992)) stat2Value = 10; // 0.08%
// 			else if ((iResult >= 9992) && (iResult < 9997)) stat2Value = 11; // 0.04%
// 			else if ((iResult >= 9997) && (iResult < 9999)) stat2Value = 12; // 0.02%
// 			else if ((iResult >= 9999) && (iResult <= 10000)) stat2Value = 13; // 0.01% 
// 			else stat2Value = 1; 
// 
// 			switch (stat2Type) 
// 			{
// 			//case 12: // Gold
// 			//case 11: // Experience
// 			//case 10: // Rep Damage
// 			//case 2:  // Hitting Prob
// 				//if (stat2Value <= 9) stat2Value = 9;
// 				//break; 
// 			//case 10: 							
// 				//if (stat2Value > 7) stat2Value = 7; 
// 				//break;
// 			/*case 11: 
// 				stat2Value = 2;
// 				break; 
// 			case 12: 
// 				stat2Value = 5;
// 				break; */
// 			}
// 
// 			//if ((iGenLevel <= 2) && (stat2Value > 7)) stat2Value = 7;
// 
// 			//SetNibble(m_dwAttribute, 3, stat2Type);
// 			//SetNibble(m_dwAttribute, 2, stat2Value);
// 		}
// 		break;
// 
// 	case ITEMEFFECTTYPE_DEFENSE:
// 		//iResult = dice(1,10000);
// 		if ((iResult >= 1) && (iResult <= 4999))          statType = ITEMSTAT_STRONG;
// 		else if ((iResult >= 5000) && (iResult <= 10000))  statType = ITEMSTAT_LIGHT;
// 		//else if ((iResult >= 9000) && (iResult <= 9554))  statType = ITEMSTAT_MANACONV;
// 		//else if ((iResult >= 9555) && (iResult <= 10000)) statType = ITEMSTAT_CRITICAL2;
// 
// 		//iResult = dice(1, 10000);
// 			if ((iResult >= 1) && (iResult < 3435)) statValue = 1; // 34.35%
// 			else if ((iResult >= 3435) && (iResult < 6436)) statValue = 2; // 30.00%
// 			else if ((iResult >= 6436) && (iResult < 8437)) statValue = 3; // 20.00%
// 			else if ((iResult >= 8437) && (iResult < 9438)) statValue = 4; // 10.00%
// 			else if ((iResult >= 9438) && (iResult < 9759)) statValue = 5; // 3.20%
// 			else if ((iResult >= 9759) && (iResult < 9840)) statValue = 6; // 1.60%
// 			else if ((iResult >= 9840) && (iResult < 9921)) statValue = 7; // 0.80%
// 			else if ((iResult >= 9921) && (iResult < 9962)) statValue = 8; // 0.40%
// 			else if ((iResult >= 9962) && (iResult < 9983)) statValue = 9; // 0.20%
// 			else if ((iResult >= 9983) && (iResult < 9992)) statValue = 10; // 0.08%
// 			else if ((iResult >= 9992) && (iResult < 9997)) statValue = 11; // 0.04%
// 			else if ((iResult >= 9997) && (iResult < 9999)) statValue = 12; // 0.02%
// 			else if ((iResult >= 9999) && (iResult <= 10000)) statValue = 13; // 0.01% 
// 			else statValue = 1; 
// 
// 		switch (statType) 
// 		{
// 		/*case ITEMSTAT_LIGHT: 
// 			if (statValue <= 5) statValue = 5;
// 			break; 
// 		case ITEMSTAT_STRONG: 						
// 			if (statValue <= 5) statValue = 5;
// 			break; */
// 		//case ITEMSTAT_MANACONV:
// 		//case ITEMSTAT_CRITICAL2:
// 		//	statValue = (statValue+1) / 2;
// 		//	if (statValue < 1) statValue = 1;
// 		//	//if ((iGenLevel <= 3) && (statValue > 2)) statValue = 2;
// 		//	break;
// 		}
// 
// 		//if ((iGenLevel <= 2) && (statValue > 7)) statValue = 7;
// 
// 		//SetNibble(m_dwAttribute, 5, statType);
// 		//SetNibble(m_dwAttribute, 4, statValue);
// 
// 		if (/*dice(1,10000)*/ <= 8000) {
// 			iResult = dice(1,100);
// 			   if (iResult <= 15)        stat2Type = ITEMSTAT2_PSNRES; // 15
// 			   else if (iResult <= 30)  stat2Type = ITEMSTAT2_SPREC;  // 15
// 			   else if (iResult <= 40)  stat2Type = ITEMSTAT2_HPREC;  // 10
// 			   else if (iResult <= 50)  stat2Type = ITEMSTAT2_MPREC;  // 10
// 			   else if (iResult <= 60)  stat2Type = ITEMSTAT2_DEF;  // 10
// 			   else if (iResult <= 70)  stat2Type = ITEMSTAT2_MR; // 10
// 			   else if (iResult <= 80)  stat2Type = ITEMSTAT2_EXP;   // 10
// 			   else if (iResult <= 90)  stat2Type = ITEMSTAT2_GOLD;  // 10 
// 			   else if (iResult <= 95)  stat2Type = ITEMSTAT2_MA; // 5
// 			   else if (iResult <= 100) stat2Type = ITEMSTAT2_PA; // 5
// 
// 			iResult = dice(1, 10000);
// 			if ((iResult >= 1) && (iResult < 3435)) stat2Value = 1; // 34.35%
// 			else if ((iResult >= 3435) && (iResult < 6436)) stat2Value = 2; // 30.00%
// 			else if ((iResult >= 6436) && (iResult < 8437)) stat2Value = 3; // 20.00%
// 			else if ((iResult >= 8437) && (iResult < 9438)) stat2Value = 4; // 10.00%
// 			else if ((iResult >= 9438) && (iResult < 9759)) stat2Value = 5; // 3.20%
// 			else if ((iResult >= 9759) && (iResult < 9840)) stat2Value = 6; // 1.60%
// 			else if ((iResult >= 9840) && (iResult < 9921)) stat2Value = 7; // 0.80%
// 			else if ((iResult >= 9921) && (iResult < 9962)) stat2Value = 8; // 0.40%
// 			else if ((iResult >= 9962) && (iResult < 9983)) stat2Value = 9; // 0.20%
// 			else if ((iResult >= 9983) && (iResult < 9992)) stat2Value = 10; // 0.08%
// 			else if ((iResult >= 9992) && (iResult < 9997)) stat2Value = 11; // 0.04%
// 			else if ((iResult >= 9997) && (iResult < 9999)) stat2Value = 12; // 0.02%
// 			else if ((iResult >= 9999) && (iResult <= 10000)) stat2Value = 13; // 0.01% 
// 			else stat2Value = 1; 
// 
// 			switch (stat2Type) 
// 			{
// 			/*case ITEMSTAT2_SPREC:
// 			case ITEMSTAT2_HPREC:
// 			case ITEMSTAT2_MPREC:
// 			case ITEMSTAT2_PSNRES:
// 			case ITEMSTAT2_DEF:
// 			case ITEMSTAT2_MR:
// 			case ITEMSTAT2_GOLD:
// 			case ITEMSTAT2_EXP:
// 				if (stat2Value <= 9) stat2Value = 9;
// 				break;
// 
// 			case ITEMSTAT2_PA:
// 				if (stat2Value <= 6) stat2Value = 6;
// 				break;
// 
// 			case ITEMSTAT2_MA:
// 				if (stat2Value < 5) stat2Value = 5;
// 				if (stat2Value >= 9) stat2Value = 9;
// 				break;*/
// 			}
// 
// 			//if ((iGenLevel <= 2) && (stat2Value > 7)) stat2Value = 7;
// 
// 			//SetNibble(m_dwAttribute, 3, stat2Type);
// 			//SetNibble(m_dwAttribute, 2, stat2Value);
// #ifdef LOG_ARMORSTATS
// 			wsprintf(g_cTxt, "[I] Armor Drop: %s %s+%u (%u)", 
// 				m_cName, itemStats2[stat2Type].desc, stat2Value * itemStats2[stat2Type].mult, stat2Value);
// 			PutLogList(g_cTxt);
// #endif
// 		}
// 		break;
// 
// 	//case ITEMEFFECTTYPE_JEWELRY:
// 	//	iResult = dice(1,10000);
// 	//	if ((iResult >= 1) && (iResult <= 4999))  statType = ITEMSTAT_LIGHT;
// 	//	else if ((iResult >= 5000) && (iResult <= 7999))  statType = ITEMSTAT_MANACONV;
// 	//	else if ((iResult >= 8000) && (iResult <= 10000)) statType = ITEMSTAT_CRITICAL2;
// 
// 
// 	//	uint32_t bonus = m_sItemEffectValue1;
// 
// 	//	iResult = dice(1, 30000-bonus) + bonus;
// 	//	if ((iResult >= 1) && (iResult < 10000))           statValue = 1;
// 	//	else if ((iResult >= 10000) && (iResult < 17400))  statValue = 2; 
// 	//	else if ((iResult >= 17400) && (iResult < 22400))  statValue = 3;
// 	//	else if ((iResult >= 22400) && (iResult < 25400))  statValue = 4;
// 	//	else if ((iResult >= 25400) && (iResult < 27400))  statValue = 5;
// 	//	else if ((iResult >= 27400) && (iResult < 28400))  statValue = 6;
// 	//	else if ((iResult >= 28400) && (iResult < 28900))  statValue = 7;
// 	//	else if ((iResult >= 28900) && (iResult < 29300))  statValue = 8;
// 	//	else if ((iResult >= 29300) && (iResult < 29600))  statValue = 9;
// 	//	else if ((iResult >= 29600) && (iResult < 29800))  statValue = 10;
// 	//	else if ((iResult >= 29800) && (iResult < 29900))  statValue = 11;
// 	//	else if ((iResult >= 29900) && (iResult < 29970))  statValue = 12;
// 	//	else if ((iResult >= 29970) && (iResult <= 30000))  statValue = 13;
// 	//	else statValue = 1; 
// 
// 	//	switch (statType) 
// 	//	{
// 	//	case ITEMSTAT_LIGHT: 
// 	//		if (statValue <= 4) statValue = 4;
// 	//		break; 
// 	//	case ITEMSTAT_STRONG: 						
// 	//		if (statValue <= 2) statValue = 2;
// 	//		break; 
// 	//	case ITEMSTAT_MANACONV:
// 	//	case ITEMSTAT_CRITICAL2:
// 	//		statValue = (statValue+1) / 2;
// 	//		if (statValue < 1) statValue = 1;
// 	//		//if ((iGenLevel <= 3) && (statValue > 2)) statValue = 2;
// 	//		break;
// 	//	}
// 
// 	//	//if ((iGenLevel <= 2) && (statValue > 7)) statValue = 7;
// 
// 	//	SetNibble(m_dwAttribute, 5, statType);
// 	//	SetNibble(m_dwAttribute, 4, statValue);
// 
// 	//	if (dice(1,10000) <= 8000) {
// 	//		iResult = dice(1,13000);
// 	//		if (iResult <= 1000)       stat2Type = ITEMSTAT2_PSNRES;	// 10
// 	//		else if (iResult <= 2000)  stat2Type = ITEMSTAT2_DEF;		// 10
// 	//		else if (iResult <= 3000)  stat2Type = ITEMSTAT2_SPREC;	// 16
// 	//		else if (iResult <= 5400)  stat2Type = ITEMSTAT2_HPREC;	// 24
// 	//		else if (iResult <= 7800)  stat2Type = ITEMSTAT2_MPREC;	// 24
// 	//		else if (iResult <= 9000)  stat2Type = ITEMSTAT2_MR;		// 12
// 	//		else if (iResult <= 11000)  stat2Type = ITEMSTAT2_EXP;
// 	//		else if (iResult <= 13000)  stat2Type = ITEMSTAT2_GOLD;
// 
// 	//		iResult = dice(1, 29980 - bonus) + bonus;
// 	//		if ((iResult >= 1) && (iResult < 15000))           stat2Value = 1; 
// 	//		else if ((iResult >= 15000) && (iResult < 22400))  stat2Value = 2; 
// 	//		else if ((iResult >= 23000) && (iResult < 26100))  stat2Value = 3; 
// 	//		else if ((iResult >= 26100) && (iResult < 27700))  stat2Value = 4;  
// 	//		else if ((iResult >= 27700) && (iResult < 28700))  stat2Value = 5; 
// 	//		else if ((iResult >= 28700) && (iResult < 29200))  stat2Value = 6;  
// 	//		else if ((iResult >= 29200) && (iResult < 29450))  stat2Value = 7;  
// 	//		else if ((iResult >= 29450) && (iResult < 29649))  stat2Value = 8; 
// 	//		else if ((iResult >= 29649) && (iResult < 29793))  stat2Value = 9; 
// 	//		else if ((iResult >= 29793) && (iResult < 29888))  stat2Value = 10;
// 	//		else if ((iResult >= 29888) && (iResult < 29935))  stat2Value = 11; 
// 	//		else if ((iResult >= 29935) && (iResult < 29967))  stat2Value = 12;
// 	//		else if ((iResult >= 29967) && (iResult <= 29980))  stat2Value = 13;
// 	//		else stat2Value = 1;
// 
// 	//		switch (stat2Type) 
// 	//		{
// 	//		case ITEMSTAT2_PSNRES:
// 	//		case ITEMSTAT2_DEF:
// 	//		case ITEMSTAT2_MR:
// 	//			if (stat2Value <= 3) stat2Value = 3;
// 	//			break; 
// 	//		}
// 
// 	//		//if (iGenLevel <= 2 && stat2Value > 7) stat2Value = 7;
// 
// 	//		SetNibble(m_dwAttribute, 3, stat2Type);
// 	//		SetNibble(m_dwAttribute, 2, stat2Value);
// 	//	}
// 	//	break;
// 	}
// 
// 	AdjustByStat();
}

void Item::AdjustByStat()
{
	return;
	uint32_t dwSWEType, dwSWEValue;
	double dV1, dV2, dV3;

	//dwSWEType = GetNibble(m_dwAttribute, 5);
	//dwSWEValue = GetNibble(m_dwAttribute, 4);

	switch (dwSWEType)
	{
	case ITEMSTAT_AGILE: 			
		m_cSpeed--;
		if (m_cSpeed < 0) m_cSpeed = 0;
		break;

	case ITEMSTAT_LIGHT: 
		dV2 = (double)m_wWeight;
		dV3 = (double)(dwSWEValue*5); // Changed Light % from *4 to *5 of a total 65%
		dV1 = (dV3/100.0f)*dV2;
		m_wWeight -= (int)dV1;

		if (m_wWeight < 1) m_wWeight = 1;
		break;

	case ITEMSTAT_STRONG: // Changed STRONG stat to be more usefull
		dV2 = (double)m_wMaxLifeSpan;
		dV3 = (double)(dwSWEValue*8); // changed from 91% to *8 = 104%
		dV1 = (dV3/100.0f)*dV2;
		m_wMaxLifeSpan += (int)dV1;
		break;

	case ITEMSTAT_SHARP:
		dV2 = (double)m_wMaxLifeSpan;
		dV3 = (double)(dwSWEValue*2);
		dV1 = (dV3/100.0f)*dV2;
		m_wMaxLifeSpan += (int)dV1;
		break;

	case ITEMSTAT_ANCIENT:
		dV2 = (double)m_wMaxLifeSpan;
		dV3 = (double)(dwSWEValue*4);
		dV1 = (dV3/100.0f)*dV2;
		m_wMaxLifeSpan += (int)dV1;
		break;

	case ITEMSTAT_FLAWLESSSHARP:
		dV2 = (double)m_wMaxLifeSpan;
		dV3 = (double)(dwSWEValue*8);
		dV1 = (dV3/100.0f)*dV2;
		m_wMaxLifeSpan += (int)dV1;
		break;

	case ITEMSTAT_FLAWLESSANCIENT:
		dV2 = (double)m_wMaxLifeSpan;
		dV3 = (double)(dwSWEValue*10);
		dV1 = (dV3/100.0f)*dV2;
		m_wMaxLifeSpan += (int)dV1;
		break;
	}
}

uint8_t Item::GetMaxSockets() const
{
	//TODO: redesign socket system and how you obtain them
	if(IsVortexed())
	{
		return 2;
	}
	else if(!IsManued())
	{
		return 0;
	}

	switch(m_cEquipPos)
	{
	case EQUIPPOS_BODY:
		if(GetManuCompletion() < 50)
			return 1;
		else if(GetManuCompletion() < 100)
			return 2;
		else if(GetManuCompletion() >= 100)
			return 3;
		break;
		
	case EQUIPPOS_ARMS:
	case EQUIPPOS_PANTS:
		if(GetManuCompletion() >= 50 && GetManuCompletion() < 100)
			return 1;
		else if(GetManuCompletion() >= 100)
			return 2;
		break;

	case EQUIPPOS_HEAD:
		if(GetManuCompletion() >= 50)
			return 1;
		break;
	}

	return 0;
}

bool Item::AddSocket(Item * gem)
{
	uint8_t sockets = GetMaxSockets();

	if(gem->m_sIDnum == ITEM_VORTEXGEM)
	{
		if(sockets == 0 && 
			(m_cEquipPos == EQUIPPOS_BODY || m_cEquipPos == EQUIPPOS_ARMS || 
			m_cEquipPos == EQUIPPOS_PANTS || m_cEquipPos == EQUIPPOS_HEAD)/* &&
			GetNibble(m_dwAttribute, 5)*/ )
		{
				m_sockets[0] = SG_VORTEXGEM;
				m_ItemColor = ARGB(255,0,0,255); // blue? old: 10
				return true;
		} else {
			return false;
		}
	}

	int socket = -1;
	for(uint8_t i = 0; i < sockets; i++)
	{
		if(m_sockets[i] == SG_NONE)
		{
			socket = i;
			break;
		}
	}

	if(socket == -1)
	{
		// no free sockets
		return false;
	}

	//m_sockets[socket] = g_socketMap[ gem->m_sIDnum ];

	return true;
}

bool Item::IsManued() const
{
	 return (m_dwAttribute & 1) && m_sItemEffectType != ITEMEFFECTTYPE_VARIABLEADD;
}

Item * Item::CreateItem()
{
	return new Item;
}
