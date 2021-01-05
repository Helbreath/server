
#pragma once

#include <cstdint>
#include <string>

#define DEF_MAXITEMEQUIPPOS		15
#define DEF_EQUIPPOS_NONE		0
#define DEF_EQUIPPOS_HEAD		1
#define DEF_EQUIPPOS_BODY		2
#define DEF_EQUIPPOS_ARMS		3
#define DEF_EQUIPPOS_PANTS		4
#define DEF_EQUIPPOS_LEGGINGS	5
#define DEF_EQUIPPOS_NECK		6
#define DEF_EQUIPPOS_LHAND		7
#define DEF_EQUIPPOS_RHAND		8
#define DEF_EQUIPPOS_TWOHAND	9
#define DEF_EQUIPPOS_RFINGER	10
#define DEF_EQUIPPOS_LFINGER	11
#define DEF_EQUIPPOS_BACK		12
#define DEF_EQUIPPOS_FULLBODY	13

#define DEF_ITEMTYPE_NOTUSED	-1
#define DEF_ITEMTYPE_NONE		 0
#define DEF_ITEMTYPE_EQUIP		 1
#define DEF_ITEMTYPE_APPLY		 2
#define DEF_ITEMTYPE_USE_DEPLETE 3       
#define DEF_ITEMTYPE_INSTALL	 4
#define DEF_ITEMTYPE_CONSUME	 5
#define DEF_ITEMTYPE_ARROW		 6
#define DEF_ITEMTYPE_EAT		 7
#define DEF_ITEMTYPE_USE_SKILL   8
#define DEF_ITEMTYPE_USE_PERM    9
#define DEF_ITEMTYPE_USE_SKILL_ENABLEDIALOGBOX	10
#define DEF_ITEMTYPE_USE_DEPLETE_DEST			11
#define DEF_ITEMTYPE_MATERIAL					12


#define DEF_ITEMEFFECTTYPE_NONE				0
#define DEF_ITEMEFFECTTYPE_ATTACK			1
#define DEF_ITEMEFFECTTYPE_DEFENSE			2
#define DEF_ITEMEFFECTTYPE_ATTACK_ARROW		3
#define DEF_ITEMEFFECTTYPE_HP   		    4
#define DEF_ITEMEFFECTTYPE_MP   		    5
#define DEF_ITEMEFFECTTYPE_SP   		    6
#define DEF_ITEMEFFECTTYPE_HPSTOCK 		    7
#define DEF_ITEMEFFECTTYPE_GET			    8
#define DEF_ITEMEFFECTTYPE_STUDYSKILL		9
#define DEF_ITEMEFFECTTYPE_SHOWLOCATION		10
#define DEF_ITEMEFFECTTYPE_MAGIC			11
#define DEF_ITEMEFFECTTYPE_CHANGEATTR		12
#define DEF_ITEMEFFECTTYPE_ATTACK_MANASAVE	13
#define DEF_ITEMEFFECTTYPE_ADDEFFECT	    14
#define DEF_ITEMEFFECTTYPE_MAGICDAMAGESAVE	15 
#define DEF_ITEMEFFECTTYPE_OCCUPYFLAG		16
#define DEF_ITEMEFFECTTYPE_DYE				17
#define DEF_ITEMEFFECTTYPE_STUDYMAGIC		18
#define DEF_ITEMEFFECTTYPE_ATTACK_MAXHPDOWN	19
#define DEF_ITEMEFFECTTYPE_ATTACK_DEFENSE	20
#define DEF_ITEMEFFECTTYPE_MATERIAL_ATTR	21
#define DEF_ITEMEFFECTTYPE_FIRMSTAMINAR		22
#define DEF_ITEMEFFECTTYPE_LOTTERY			23

#define DEF_ITEMEFFECTTYPE_ATTACK_SPECABLTY		24
#define DEF_ITEMEFFECTTYPE_DEFENSE_SPECABLTY	25
#define DEF_ITEMEFFECTTYPE_ALTERITEMDROP		26

#define DEF_ITEMEFFECTTYPE_CONSTRUCTIONKIT		27
#define DEF_ITEMEFFECTTYPE_WARM					28
#define DEF_ITEMEFFECTTYPE_DEFENSE_ANTIMINE		29
#define DEF_ITEMEFFECTTYPE_ITEMFARM				30

#define DEF_ITET_UNIQUE_OWNER				1
#define DEF_ITET_ID							2
#define DEF_ITET_DATE						3


class item
{
public:

	item();
	~item() = default;

	//char  m_cName[21];
	std::string name;

	int16_t id = 0;
	int8_t  type;
	int8_t  equip_pos;
    uint16_t x;
    uint16_t y;
	int16_t item_effect_type;
	int16_t item_effect_value1 = 0;
	int16_t item_effect_value2 = 0;
	int16_t item_effect_value3 = 0;
	int16_t item_effect_value4 = 0;
	int16_t item_effect_value5 = 0;
	int16_t item_effect_value6 = 0;
	int32_t max_life_span = 0;
    int32_t cur_life_span = 0;
    int16_t special_effect = 0;
	int16_t special_effect_value1 = 0;
	int16_t special_effect_value2 = 0;

	int16_t sprite = 0;
	int16_t sprite_frame = 0;

	int8_t  appr_value = 0;
	int8_t  speed = 0;

	uint32_t price= 0;
	uint16_t weight = 0;
	int16_t level_limit = 0;
	int8_t  gender_limit = 0;

	int16_t related_skill = 0;

	int8_t category = 0;
	bool  for_sale = false;

	uint32_t count = 0;
	int16_t touch_effect_type = 0;
	int16_t touch_effect_value1 = 0;
	int16_t touch_effect_value2 = 0;
	int16_t touch_effect_value3 = 0;
	uint32_t color = 0;
	int16_t item_spec_effect_value1 = 0;
	int16_t item_spec_effect_value2 = 0;
	int16_t item_spec_effect_value3 = 0;
	uint32_t attribute = 0;
};
