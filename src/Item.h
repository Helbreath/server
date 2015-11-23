// Item.h: interface for the CItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEM_H__211A1360_91B9_11D2_B143_00001C7030A6__INCLUDED_)
#define AFX_ITEM_H__211A1360_91B9_11D2_B143_00001C7030A6__INCLUDED_

#include "common.h"
#include "Items.h"
#include <list>

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define ARGB(a,r,g,b)          ((uint32_t)(((uint8_t)(r)|((uint16_t)((uint8_t)(g))<<8))|(((uint32_t)(uint8_t)(b))<<16)|(((uint32_t)(uint8_t)(a))<<24)))

enum ItemStat{
	ITEMSTAT_NONE,
	ITEMSTAT_CRITICAL,
	ITEMSTAT_POISONING,
	ITEMSTAT_RIGHTEOUS,
	ITEMSTAT_UNUSED,		// UNUSED
	ITEMSTAT_AGILE,
	ITEMSTAT_LIGHT,
	ITEMSTAT_SHARP,
	ITEMSTAT_STRONG,
	ITEMSTAT_ANCIENT,
	ITEMSTAT_CASTPROB,
	ITEMSTAT_MANACONV,
	ITEMSTAT_CRITICAL2,
	ITEMSTAT_FLAWLESSSHARP, // Flawless Sharp
	ITEMSTAT_FLAWLESSANCIENT // Flawless Ancient
};

enum ItemStat2{
	ITEMSTAT2_NONE,
	ITEMSTAT2_PSNRES,
	ITEMSTAT2_HITPROB,
	ITEMSTAT2_DEF,
	ITEMSTAT2_HPREC,
	ITEMSTAT2_SPREC,
	ITEMSTAT2_MPREC,
	ITEMSTAT2_MR,
	ITEMSTAT2_PA,
	ITEMSTAT2_MA,
	ITEMSTAT2_CAD,
	ITEMSTAT2_EXP,
	ITEMSTAT2_GOLD
};

const struct ItemStats2{
	string desc;
	uint32_t mult;
} itemStats2[] = {
	{"",0},
	{"PSN", 10},
	{"HIT", 10},
	{"DF", 10},
	{"HP", 10},
	{"SP", 10},
	{"MP", 10},
	{"MR", 10},
	{"PA", 3},
	{"MA", 2},
	{"CAD", 1},
	{"XP", 10},
	{"GOLD", 10},
	{"",0}
};

#define MAXITEMEQUIPPOS		15
#define EQUIPPOS_NONE		0
#define EQUIPPOS_HEAD		1
#define EQUIPPOS_BODY		2
#define EQUIPPOS_ARMS		3
#define EQUIPPOS_PANTS		4
#define EQUIPPOS_BOOTS		5
#define EQUIPPOS_NECK		6
#define EQUIPPOS_LHAND		7
#define EQUIPPOS_RHAND		8
#define EQUIPPOS_TWOHAND	9
#define EQUIPPOS_RFINGER	10
#define EQUIPPOS_LFINGER	11
#define EQUIPPOS_BACK		12
#define EQUIPPOS_FULLBODY	13     

enum ItemTypes
{
	ITEMTYPE_NOTUSED = -1,
	ITEMTYPE_NONE,				// 0
	ITEMTYPE_EQUIP,			// 1
	ITEMTYPE_APPLY,			// 2
	ITEMTYPE_USE_DEPLETE,	// 3
	ITEMTYPE_INSTALL,			// 4
	ITEMTYPE_CONSUME,			// 5
	ITEMTYPE_ARROW,			// 6
	ITEMTYPE_EAT,				// 7
	ITEMTYPE_USE_SKILL,		// 8
	ITEMTYPE_USE_PERM,		// 9
	ITEMTYPE_USE_SKILL_ENABLEDIALOGBOX,	// 10
	ITEMTYPE_USE_DEPLETE_DEST,				// 11
	ITEMTYPE_MATERIAL,		// 12
	ITEMTYPE_RELIC				// 13
};


#define ITEMEFFECTTYPE_NONE				0
#define ITEMEFFECTTYPE_ATTACK			1
#define ITEMEFFECTTYPE_DEFENSE			2
#define ITEMEFFECTTYPE_ATTACK_ARROW		3
#define ITEMEFFECTTYPE_HP   		    4
#define ITEMEFFECTTYPE_MP   		    5
#define ITEMEFFECTTYPE_SP   		    6
#define ITEMEFFECTTYPE_HPSTOCK 		    7
#define ITEMEFFECTTYPE_GET			    8
#define ITEMEFFECTTYPE_STUDYSKILL		9
#define ITEMEFFECTTYPE_SHOWLOCATION		10
#define ITEMEFFECTTYPE_MAGIC			11
#define ITEMEFFECTTYPE_CHANGEATTR		12
#define ITEMEFFECTTYPE_ATTACK_MANASAVE	13
#define ITEMEFFECTTYPE_ADDEFFECT	    14
#define ITEMEFFECTTYPE_MAGICDAMAGESAVE	15
#define ITEMEFFECTTYPE_OCCUPYFLAG		16
#define ITEMEFFECTTYPE_DYE				17
#define ITEMEFFECTTYPE_STUDYMAGIC		18
#define ITEMEFFECTTYPE_ATTACK_MAXHPDOWN	19
#define ITEMEFFECTTYPE_ATTACK_DEFENSE	20
#define ITEMEFFECTTYPE_MATERIAL_ATTR	21
#define ITEMEFFECTTYPE_FIRMSTAMINAR		22
#define ITEMEFFECTTYPE_LOTTERY			23

#define ITEMEFFECTTYPE_ATTACK_SPECABLTY		24
#define ITEMEFFECTTYPE_DEFENSE_SPECABLTY	25
#define ITEMEFFECTTYPE_ALTERITEMDROP		26

#define ITEMEFFECTTYPE_CONSTRUCTIONKIT		27
#define ITEMEFFECTTYPE_WARM					28
#define ITEMEFFECTTYPE_DEFENSE_ANTIMINE		29
#define ITEMEFFECTTYPE_ITEMFARM				30  
#define ITEMEFFECTTYPE_SLATES			31
#define ITEMEFFECTTYPE_JEWELRY			32
#define ITEMEFFECTTYPE_SOCKET			33
#define ITEMEFFECTTYPE_VARIABLEADD			34
#define ITEMEFFECTTYPE_ATTACK_MAGICITEM		50 // Battle Mages xRisenx
#define ITEMEFFECTTYPE_CRIT				51  // Critical Potion xRisenx
#define ITEMEFFECTTYPE_ARMORDYE			52	// Armor Dye xRisenx
#define ITEMEFFECTTYPE_WEAPONDYE		53	// Weapon Dye xRisenx
#define ITEMEFFECTTYPE_ADDSKILLS		54  // New Skill % To Items xRisenx
#define ITEMEFFECTTYPE_ADDLUCK			55  // Drop System Luck xRisenx
#define ITEMEFFECTTYPE_ADDREP			56  // Rep Pots xRisenx
//#define ITEMEFFECTTYPE_ADDBALLPOINTS	57	// Ball Points xRisenx

enum WandEffectTypes{
	WET_NONE,
	WET_ADDMAGICDMGPCT
};

enum SocketGems : int8_t{
	SG_NONE = (int8_t)ITEM_NONE,
	SG_REJUGEM7 = 1,
	SG_REJUGEM14,
	SG_REJUGEM21,
	SG_BLOODGEM7,
	SG_BLOODGEM14,
	SG_BLOODGEM21,
	SG_MINDGEM7,
	SG_MINDGEM14,
	SG_MINDGEM21,
	SG_ARMORGEM7,
	SG_ARMORGEM14,
	SG_ARMORGEM21,
	SG_ENCHANTEDGEM2,
	SG_ENCHANTEDGEM4,
	SG_ENCHANTEDGEM6,
	SG_TACTICALGEM3,
	SG_TACTICALGEM5,
	SG_TACTICALGEM7,
	SG_VORTEXGEM,
	SG_MAX
};

const int64_t SocketGems[SG_MAX] = {
	ITEM_NONE,
	ITEM_REJUGEM7,
	ITEM_REJUGEM14,
	ITEM_REJUGEM21,
	ITEM_BLOODGEM7,
	ITEM_BLOODGEM14,
	ITEM_BLOODGEM21,
	ITEM_MINDGEM7,
	ITEM_MINDGEM14,
	ITEM_MINDGEM21,
	ITEM_ARMORGEM7,
	ITEM_ARMORGEM14,
	ITEM_ARMORGEM21,
	ITEM_ENCHANTEDGEM2,
	ITEM_ENCHANTEDGEM4,
	ITEM_ENCHANTEDGEM6,
	ITEM_TACTICALGEM3,
	ITEM_TACTICALGEM5,
	ITEM_TACTICALGEM7,
	ITEM_VORTEXGEM
};

#define ITET_UNIQUE_OWNER			1
#define ITET_ID						2
#define ITET_DATE						3
#define ITET_GM_MADE					4

class Item  
{
public:

	Item(uint32_t count = 1);
	Item(uint64_t itemID, Item ** itemconfig, uint32_t count = 1);
	Item(char * itemName, Item ** itemconfig, uint32_t count = 1);
	virtual ~Item();

	void Init();

	static Item * CreateItem();

	uint32_t GetWeight(int count) const;
	uint32_t GetWeight()			const { return GetWeight( count ); }
	uint32_t GetBaseWeight()		const { return GetWeight(1); }
	uint32_t GetManuCompletion()	const { return itemSpecialEffectV2 + 100; }
	bool IsValid()		const { return (m_sIDnum != ITEM_INVALID); }
	bool IsLogged() 	const;
	bool IsManued()	const;
	bool IsVortexed()	const { return m_sockets[0] == SG_VORTEXGEM; }
	uint8_t GetMaxSockets() const;
	bool AddSocket(Item * gem);

	bool InitItemAttr(const string pItemName, Item ** itemconfig);
	bool InitItemAttr(uint64_t iItemID, Item ** itemconfig);
	void InitStats(int iGenLevel);
	void AdjustByStat();
	string Dump() const;

	uint16_t m_slot;

	string name;

	uint64_t m_sIDnum;
	char  m_cItemType;
	char  m_cEquipPos;
	short itemEffectType;
	short itemEffectV1, itemEffectV2, itemEffectV3;
	short itemEffectV4, itemEffectV5, itemEffectV6;
	uint64_t uid;
	uint16_t  durability;
	int16_t specialEffect;
	int16_t specialEffectV1, specialEffectV2;

	int16_t spriteID;
	int16_t spriteFrame;

	char  appearanceValue;
	char  swingSpeed;

	//uint32_t m_wPrice;
	int32_t price;
	uint16_t  weight;
	short levelLimit;
	int8_t genderLimit;

	int16_t relatedSkill;

	char category;
	bool m_bIsForSale;
	bool logAction;

	uint32_t count;
	int16_t effectType;
	int16_t effectV1, effectV2, effectV3;
	uint32_t color; 
	int16_t itemSpecialEffectV1, itemSpecialEffectV2, itemSpecialEffectV3;

	int32_t  m_wCurLifeSpan;
	uint32_t m_dwAttribute;
	uint8_t m_sockets[MAXITEMSOCKETS];

	bool m_disabled;

	bool equipped;
	uint16_t x;
	uint16_t y;
};

typedef std::list<Item*> ItemList;
typedef std::list<Item*>::iterator ItemListIter;
typedef std::list<Item*>::const_iterator ItemListCIter;

class ItemWrap
{
public:
	ItemWrap() { _item = 0; }
	~ItemWrap() { if (_item) delete _item; _item = 0; }
	Item * _item;
	void SetItem(Item * item) { _item = item; }
	void ClearItem() { _item = 0; }
	void DeleteItem() { if (_item) delete _item; _item = 0; }
};

#endif // !defined(AFX_ITEM_H__211A1360_91B9_11D2_B143_00001C7030A6__INCLUDED_)
