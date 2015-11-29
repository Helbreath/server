
#pragma once

#include "common.h"
#include "funcs.h"
#include <stdio.h>
#include <stdlib.h>
#include "Unit.h"
#include "Item.h"
#include "Magic.h"
#include "connection.h"
#include "InventoryMgr.h"
#include <list>

class Guild;
class Party;

#define CLIENTSOCKETBLOCKLIMIT	15


enum PartyStatus
{
	PS_NOTINPARTY,
	PS_PROCESSING,
	PS_INPARTY
};

enum Sex{
	NONE,
	MALE,
	FEMALE
};

#define FLAGRANGE_X		11
#define FLAGRANGE_Y		9



enum skillIndexes
{
	SKILL_MINING,			// 0
	SKILL_FISHING,			// 1
	SKILL_FARMING,			// 2
	SKILL_MAGICRES,			// 3
	SKILL_MAGIC,			// 4
	SKILL_HANDATTACK,		// 5
	SKILL_ARCHERY,			// 6
	SKILL_SHORTSWORD,		// 7
	SKILL_LONGSWORD,		// 8
	SKILL_FENCING,			// 9
	SKILL_AXE,				// 10
	SKILL_SHIELD,			// 11
	SKILL_ALCHEMY,			// 12
	SKILL_MANUFACTURING,	// 13
	SKILL_HAMMER,			// 14
	SKILL_15,			    // 15	
	SKILL_CRAFTING,			// 16	
	SKILL_17,				
	SKILL_18,
	SKILL_PRETENDCORPSE,	// 19
	SKILL_20,
	SKILL_STAFF,			// 21
	SKILL_22,
	SKILL_POISONRES			// 23
};

class Client : public Unit, public boost::enable_shared_from_this<Client>, public boost::noncopyable
{
public:
	Client();
	~Client();

	// 0 = dead connect, 1 = login screen, 2 = in game
	// can switch between 1 and 2 but can only be 0 when connection closing
	uint8_t currentstatus;

	std::list<StreamWrite> outgoingqueue;

	string address;

	//This WILL cause memory leaks if this is not unlinked on socket close
	connection_ptr socket;//will force the smart_ptr to remain until client object is deleted - clear early? or leave until object destruction?
	//boost::weak_ptr<connection> socket;//keeps the client object from keeping the socket object open, but the socket object can keep the client object
	uint64_t socknum;

	GServer * gserver;

	uint64_t disconnecttime;
	uint64_t lastpackettime;

	bool CheckNearbyFlags();
	bool IsHeldWinner() const;
	bool IsHeldLoser() const;//TODO: why is there a check for loser as well?

	void WithdrawFromGuild();//TODO: move to guild class

	bool CheckTotalSkillMasteryPoints(int iSkill);
	void ValidateSkills(bool logInvalidSkills);
	int GetPlayerRelationship(Client * target) const;
	void KilledHandler(Unit * attacker, int32_t sDamage);
	void ApplyCombatKilledPenalty(char cPenaltyLevel, bool bIsSAattacked = false, bool bItemDrop = false);
	void PenaltyItemDrop(int iTotal, bool bIsSAattacked = false, bool bItemDrop = false);
	void ApplyPKPenalty(Client * player);
	void ApplyElo(Client * foe);

	//Lock player to a specific map for specific seconds
	void LockMap(string mapName, uint32_t time);

#pragma region Stat getters/setters
	int32_t GetMaxHP() const;
	int32_t GetMaxMP() const;
	int32_t GetMaxSP() const;
	void AddHP(int64_t hp);
	int32_t GetStr()		const { return _str + _angelStr; }
	int32_t GetMag()		const { return _mag + _angelMag; }
	int32_t GetInt()		const { return _int + _angelInt; }
	int32_t GetDex()		const { return _dex + _angelDex; }
	int32_t GetAgi()		const { return _agi + _angelAgi; }
	int32_t GetVit()		const { return _vit + _angelVit; }
	int32_t GetBaseStr()	const { return _str; }
	int32_t GetBaseMag()	const { return _mag; }
	int32_t GetBaseInt()	const { return _int; }
	int32_t GetBaseDex()	const { return _dex; }
	int32_t GetBaseAgi()	const { return _agi; }
	int32_t GetBaseVit()	const { return _vit; }
	void SetStr(int str, bool check = true);
	void SetMag(int mag);
	void SetInt(int __int, bool check = true);
	void SetDex(int dex);
	void SetVit(int vit);
	void SetAgi(int agi);//TODO: old Charisma - turn into something else?
	int32_t GetAngelStr()	const { return _angelStr;	}
	int32_t GetAngelMag()	const { return _angelMag;	}
	int32_t GetAngelInt()	const { return _angelInt; }
	int32_t GetAngelDex()	const { return _angelDex;	}
	int32_t GetAngelAgi()	const { return _angelAgi;	}
	int32_t GetAngelVit()	const { return _angelVit;	}
	void SetAngelStr(int var);
	void SetAngelInt(int var);
	void SetAngelDex(int var);
	void SetAngelMag(int var);
	void SetAngelVit(int var);
	void SetAngelAgi(int var);
#pragma endregion

#ifdef Showdmg
	int32_t iDmgShowon;
#endif
	int32_t GetEffectiveMA() const;//Get current total Magic Absorption

	bool IsInFoeMap();//Is in enemy faction map
	bool IsInJail();
	void DecPKCount();
	void IncPKCount();

	float GetDropFactor() const;

	bool IsGM()				const { return (m_iAdminUserLevel == 0)			? false : true; }
	bool IsInvincible()	const { return (m_GMFlags & GMFLAG_INVINCIBLE)	? true : false; }
	bool IsNoAggro()		const { return (m_GMFlags & GMFLAG_NOAGGRO)			? true : false; }
	bool IsEthereal()		const { return (m_GMFlags & GMFLAG_ETHEREAL)>1; }
	bool IsValid()			const { return (handle != -1) ? true : false; }


#pragma region resists
	uint16_t IceResist();
	uint16_t ParalyzeResist();
	uint16_t FireResist();
	uint16_t PoisonResist();
	uint16_t LightningResist();
	uint16_t ArcaneResist();
#pragma endregion

	void SWrite(StreamWrite & sw);

	void Notify(Client * from, uint16_t wMsgType, uint32_t sV1 = 0, uint32_t sV2 = 0, uint32_t sV3 = 0, string pString = "",
		uint32_t sV4 = 0, uint32_t sV5 = 0, uint32_t sV6 = 0, uint32_t sV7 = 0, uint32_t sV8 = 0, uint32_t sV9 = 0,
		string pString2 = "");
	void NotifyGuildInfo(bool memberList = false);
	void NotifyGuildsmanStatus(Client * player, bool online = true);
	void NotifyGuildSummons(Client * player);

	int32_t GetWeight()	const { return m_iCurWeightLoad; }
	void UpdateWeight();
	uint32_t HasItem(string name);
	uint32_t HasItem(uint64_t id);
	uint32_t GetItemCount(uint64_t id);
	void SetItemCount(uint64_t id, uint32_t val, bool notify = true);


#pragma region Status effects
	bool IsPoisoned() { return _poison; }
	void Poison(bool poison, int32_t level, uint64_t time) { _poison = poison; _poisonLevel = level; _poisonTime = time; }
	int32_t PoisonLevel() { return _poisonLevel; }
	bool IsParalyzed() { return false; }//TODO:
	bool IsHeld() { return false; }
	bool IsConfused() { return false; }
	bool IsIllusioned() { return false; }
	bool IsBerserked() { return false; }
#pragma endregion

	bool IsFlooding(uint32_t sensitivity);
	bool IsInCombatMode()	const { return ((m_sAppr2 & 0xF000) >> 12)>1; }

	bool ResizeInventory(uint16_t newsize);

	// Change currency by amount - _count += amount
	// Returns:
	//		0 - nonexistent currency
	//		1 - success
	//		2 - limits were broken and not all of amount was applied
	int8_t ChangeCurrency(int8_t id, int64_t amount);

	// Set currency to amount - _count = amount
	// Returns:
	//		0 - nonexistent currency
	//		1 - success
	//		2 - limits were broken and not all of amount was applied
	int8_t SetCurrency(int8_t id, int64_t amount);

	struct stCurrency
	{
		stCurrency() { _id = 0; _count = _min = _max = 0; }
		stCurrency(const stCurrency&c) { _id = c._id; _count = c._count; _min = c._min; _max = c._max; }
		int8_t  _id;
		int64_t _count;
		int64_t _min;
		int64_t _max;
	};

	InventoryMgr invBank;
	InventoryMgr invSelf;

	bool civilian;//TODO: either remove or redo the whole 17 year old civilian system

	bool m_bActive;//??

	string account;
	string password;

	bool  m_bIsInitComplete;
	bool  m_bIsMsgSendAvailable;//??
	bool  m_bIsCheckingWhisperPlayer;//??

	string  mapName;



	// Guild things
	uint8_t guildRank;//keep this?
	Guild * guild;
	uint64_t guildSummonsTime;
	
	


	string  faction;



#pragma region Appearance Flags
	int16_t m_sAppr1;
	int16_t m_sAppr2;
	int16_t m_sAppr3;
	int16_t m_sAppr4;
	int32_t m_iApprColor;

	int16_t m_sHeadApprValue;
	int16_t m_sBodyApprValue;
	int16_t m_sArmApprValue;
	int16_t m_sLegApprValue;

	uint8_t gender;
	uint8_t m_cHairStyle;
	uint32_t colorSkin, colorHair, colorUnderwear;
#pragma endregion
#pragma region Base Timers
	uint64_t m_dwTime, m_dwHPTime, m_dwMPTime, m_dwSPTime, m_dwAutoSaveTime, m_dwHungerTime;
	
	bool m_hasPrecasted;
	uint64_t m_timeHack;
	int64_t m_timeHackDif;
	uint64_t m_timeHackTurn; 
	uint64_t m_moveTime[SPEEDCHECKTURNS];
	uint64_t m_moveTurn;
	uint64_t m_runTime[SPEEDCHECKTURNS];
	uint64_t m_runTurn;
#pragma endregion
#pragma region Dice values
	int32_t  m_iHPstock;
	int32_t  m_iHPStatic_stock;  
	bool m_bIsBeingResurrected;
	int32_t  stamina;
	uint64_t  m_iNextLevelExp;

	int32_t  m_iDefenseRatio;
	int32_t  m_iHitRatio;


	//int  m_iHitRatio_ItemEffect_SM; 
	//int  m_iHitRatio_ItemEffect_L;

	int32_t  m_iDamageAbsorption_Armor[MAXITEMEQUIPPOS];
	int32_t  m_iDamageAbsorption_Shield;

	int32_t  level;
	int32_t  m_iVit, m_iCharisma;
	int32_t  m_iLuck; 
	int32_t  levelUpPool;

	int32_t m_elo;
	int32_t  enemyKillCount, playerKillCount, m_iRewardGold;
	int32_t m_iEnemyKillTotalCount;
	int32_t  m_iCurWeightLoad;
	uint64_t logoutHackCheck;

	char m_cAttackDiceThrow_SM;
	char m_cAttackDiceRange_SM;
	char m_cAttackDiceThrow_L;
	char m_cAttackDiceRange_L;
	char m_cAttackBonus_SM;
	char m_cAttackBonus_L;
#pragma endregion



	// Items
	std::vector<ItemWrap *> itemList;
	std::vector<ItemWrap *> itemListBank;
	std::vector<ItemWrap *> itemListGuildBank;

	struct stEquips
	{
		Item * Head;
		Item * Chest;
		Item * Hauberk;
		Item * Leggings;
		Item * Boots;
		Item * Cape;
		Item * Angel;
		Item * Ring1;
		Item * Ring2;
		Item * Necklace;
		Item * LeftHand;
		Item * RightHand;
		Item * TwoHand;
		Item * Arrow;
	} Equipped;

	int16_t itemEquipStatus[MAXITEMEQUIPPOS];




	char magicMastery[MAXMAGICTYPE];
	
	
	
	// Skills
	unsigned char  m_cSkillMastery[MAXSKILLTYPE]; //TODO: remove, unnecessary
	int32_t   m_iSkillSSN[MAXSKILLTYPE];//TODO: remove, unnecessary
	bool  skillInUse[MAXSKILLTYPE];
	int32_t   skillInUseTime[MAXSKILLTYPE];




	weak_ptr<Client> whisperTarget;
	string  whisperTargetName;
	string  profile;

	int32_t   m_iHungerStatus;
	bool  _poison;
	int32_t  _poisonLevel;       
	uint64_t _poisonTime;

	int32_t   m_iPenaltyBlockYear, m_iPenaltyBlockMonth, m_iPenaltyBlockDay; 

	int32_t iReturnID, iNumPoints, iRank, iTitleIndex, iNextRankPoints;
	string ActiveTitle;
	int8_t TitleType;

	std::list<int16_t> TitleList;

	int32_t   m_iAdminUserLevel;
	uint8_t m_GMFlags;
	int32_t   m_reputation;
	int32_t	  m_iLucky;

	uint64_t   m_iTimeLeft_ShutUp;	 
	uint64_t   m_iTimeLeft_Rating;	 
	uint64_t   m_iTimeLeft_ForceRecall;  
	uint64_t   m_iTimeLeft_FirmStamina; 
	bool  m_bIsOnServerChange;     
	uint64_t   m_iExpStock;			 
	uint64_t m_dwExpStockTime;	
	uint64_t m_lastDamageTime;

	int32_t   m_iAutoExpAmount;		 
	uint64_t m_dwAutoExpTime;		 

	uint64_t m_dwRecentAttackTime;  
	uint64_t m_dwLastActionTime;	 

	int32_t   m_iAllocatedFish;//somethingsomething no-one-ever-fished-ever
	int32_t   m_iFishChance;//somethingsomething no-one-ever-fished-ever
	bool  safeAttackMode;//Tab for safemode

	bool  m_bIsOnWaitingProcess;//unused? no more HGserver transfers so this is largely obsolete
	int32_t   superAttack;//remaining "critical strikes"
	int32_t   superAttackMax;//max "critical strikes"

	int16_t m_sUsingWeaponSkill;//useless //because checking which item you have equipped is hard to do in real time

	int32_t   m_iMPSaveRatio;//mana conservation stat on wands

	bool  m_bIsLuckyEffect;//unused? if "on", has a chance to reduce random incoming damage to -1. No item ever set this flag before but has always existed.
	int32_t   m_iSideEffect_MaxHPdown;

	int32_t   m_iComboAttackCount;//used to track consecutive hits for increased damage. Leave as-is? Or change system to function differently?
	int32_t   m_iDownSkillIndex;//unused.

	int32_t   m_iMagicDamageSaveItemIndex; 

	int16_t m_sCharIDnum1, m_sCharIDnum2, m_sCharIDnum3;//unique character ID numbers. Was primarily used for locking items and tracking who originally looted items. Obsolete? Make a better system?
	

	// Parties
	//TODO: overhaul this pile of garbage. Old party system was so limiting. Needs a new UI for it as well. Raise 5/8 player cap to something more meaningful like 20
	Party * GetParty()	const { return _party; }
	void SetParty(Party * party)
	{
		_party = party;
		_partyStatus = (_party) ? PS_INPARTY : PS_NOTINPARTY;
	}
	PartyStatus GetPartyStatus()				const { return _partyStatus; }
	void SetPartyStatus(PartyStatus status)		{ _partyStatus = status; }
	bool HasPartyRaidBonus() const;
	bool HasPartyHuntBonus() const;
	int32_t m_iReqJoinPartyClientH;
	string m_cReqJoinPartyName;
	int32_t   m_partyCoordSteps;
	uint32_t m_pinguid;//??

	int32_t   m_iAbuseCount;//??

	bool  exchangeMode;
	bool  exchangeConfirm;
	Client * exchangePlayer;
	int8_t  exchangeCount;

	struct {
		int8_t  itemIndex; 
		string itemName;
		int32_t   itemAmount;		
	} m_exchangeItems[4];
	

	// Quests
	//TODO: Quest system! Make a Quest class that is referenced here? or keep it old-school style and just have a struct of basic information
	int32_t   m_iQuest;				 
	int32_t   m_iQuestID;			 
	int32_t   m_iAskedQuest;		 
	int32_t   m_iCurQuestCount;		 
	int32_t   m_iQuestRewardType;	 
	int32_t   m_iQuestRewardAmount;	 

	int32_t   contribution;	 
	bool  m_bQuestMatchFlag_Loc; 
	bool  m_bIsQuestCompleted;   




	int32_t   m_iCustomItemValue_Attack;
	int32_t   m_iCustomItemValue_Defense;

	int32_t   m_iMinAP_SM;
	int32_t   m_iMinAP_L;

	int32_t   m_iMaxAP_SM;
	int32_t   m_iMaxAP_L;

	bool  neutral;//Traveler (starting player, hasn't picked side yet) -- possibly better to determine from the Side variable - Remove?
	bool  observerMode;//Admin Observer mode

	int32_t   m_iSpecialEventID;

	int32_t   m_iSpecialWeaponEffectType;
	int32_t   m_iSpecialWeaponEffectValue;

	int32_t	m_nextRecallPoint;

	int32_t   m_iAddHP, m_iAddSP, m_iAddMP; 
	int32_t   m_iAddAR, m_iAddPR, m_iAddDR;
	int32_t   m_iAddMR, m_iAddAbsPD, m_iAddAbsMD; 
	int32_t   m_iAddCD, m_iAddExp, m_iAddGold;

	int32_t   m_iAddTransMana, m_iAddChargeCritical;

	int32_t   m_iAddResistMagic;
	int32_t   m_iAddPhysicalDamage;
	int32_t   m_iAddMagicalDamage;
	uint32_t m_addMagicDmgPct;

	int32_t   m_iAddAbsAir;
	int32_t   m_iAddAbsEarth;
	int32_t   m_iAddAbsFire;
	int32_t   m_iAddAbsWater;

	int32_t   m_iLastDamage;

	int32_t   m_iMoveMsgRecvCount, m_iAttackMsgRecvCount, m_iRunMsgRecvCount, m_iSkillMsgRecvCount;
	uint64_t m_dwMoveLAT, m_dwRunLAT, m_dwAttackLAT;

	uint64_t m_iSpecialAbilityTime;
	bool  m_bIsSpecialAbilityEnabled;
	uint64_t m_specialAbilityStartTime;
	uint64_t m_specialAbilityLastSec;

	int32_t   m_iSpecialAbilityType;
	int32_t  m_iAddMining;
	int32_t  m_iAddCrafting;
	int32_t  m_iAddFishing;

	int32_t   m_iSpecialAbilityEquipPos;
	int32_t   m_iAlterItemDropIndex;

	uint64_t m_dwInitCCTimeRcv;
	uint64_t m_dwInitCCTime;

	string  lockedMapName;
	uint64_t lockedMapTime;
	uint64_t deadPenaltyTime;


	int32_t m_iCSIsendPoint;//unused?

	string m_cSendingMapName;//unused?
	bool m_bIsSendingMapStatus;//unused?


	// Crusades
	int32_t crusadeContribution;//unused? if I remember, points earned during crusade from EKs that was turned into contribution? something along those lines (m_iWarContribution)
	int32_t crusadeDuty;
	uint64_t crusadeGUID;
	int32_t crusadePoint;//points for construction
	string crusadeMap;//construction map (should always be middleland, but option to change)
	int32_t crusadeX, crusadeY;//construction locations for crusade
	struct {
		char _type;
		char _side;
		short x, y;
	} crusadeStructureInfo[MAXCRUSADESTRUCTURES];
	uint64_t crusadeBeginTime;
	bool crusadeLocation;


	// Arenas (fightzones)
	int32_t   arenaNumber, arenaTicketNumber;
	uint64_t arenaReserveTime, arenaDeadTime;


	bool bankModified;//unused?

	bool rejectedMove;
	uint64_t charid;
	
	int32_t m_iGizonItemUpgradeLeft;//old majestic points

	uint64_t m_dwAttackFreqTime, m_dwMagicFreqTime, m_dwMoveFreqTime; 
	bool m_resetMoveFreq; 
	bool m_bIsAttackModeChange; 
	//whether player is on their town's map - ... used exclusively for the now unused define SAMESIDETOWNPROTECT 
	//TODO: Turn into GetLocationStatus() type variable where the return is matched against an enum or something LOC::WAREHOUSE etc
	bool m_bIsOnTown; 
	bool m_bIsOnShop; 
	bool m_bIsOnTower; 
	bool m_bIsOnWarehouse;
	bool m_bIsOnGuildHall; 
	bool m_bIsOnCath;
	bool m_bIsInBuilding; 

	uint64_t m_dwWarmEffectTime; 

	char m_heroArmourBonus;
	char m_AheroArmourBonus;
	char m_LheroArmourBonus;

	std::list<stCurrency> currency;

private:


	uint16_t _IceResist;
	uint16_t _ParalyzeResist;
	uint16_t _FireResist;
	uint16_t _PoisonResist;
	uint16_t _LightningResist;
	uint16_t _ArcaneResist;

	int32_t _str, _int, _dex, _mag, _vit, _agi;
	
	int32_t _angelStr, _angelInt, _angelDex, _angelMag, _angelAgi, _angelVit;

	Party * _party;
	PartyStatus _partyStatus;
};
