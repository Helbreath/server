
#pragma once

#include <memory>
#include "structs.hpp"
#include "unit.hpp"
#include "item.hpp"
#include "defines.hpp"

namespace hbx
{

class server;
class socket;

namespace socket_modes
{
enum class skt_mode {
    loginserver,
    gameserver
};
constexpr auto loginserver = skt_mode::loginserver;
constexpr auto gameserver = skt_mode::gameserver;
}

using namespace socket_modes;

class client : public unit, public std::enable_shared_from_this<client>
{
public:
    client();
    ~client();

    bool bCreateNewParty();

    std::shared_ptr<socket> socket_;

    skt_mode socket_mode() { return socket_mode_; }
    skt_mode socket_mode_;


    std::string account_name;
    std::string account_password;

    uint64_t internal_id;

    uint64_t account_id;

    int32_t screenwidth_v = 0;
    int32_t screenheight_v = 0;

    bool  m_bIsInitComplete;
    bool  m_bIsMsgSendAvailable;
    bool  m_bIsCheckingWhisperPlayer;

    std::string map_name;
    std::string guild_name;
    std::string location;

    int   m_iGuildRank;

    uint32_t m_dwHPTime, m_dwMPTime, m_dwSPTime, m_dwAutoSaveTime, m_dwHungerTime;

    char m_cSex, m_cSkin, m_cHairStyle, m_cHairColor, m_cUnderwear;

    int  m_iHPstock;
    int  m_iHPStatic_stock;
    int  m_iMP;
    int  m_iSP;
    int  m_iNextLevelExp;

    int  m_iDamageAbsorption_Armor[DEF_MAXITEMEQUIPPOS];
    int  m_iDamageAbsorption_Shield;

    int  m_iLevel;
    int  m_iStr, m_iInt, m_iVit, m_iDex, m_iMag, m_iCharisma;
    char m_cLU_Str, m_cLU_Int, m_cLU_Vit, m_cLU_Dex, m_cLU_Mag, m_cLU_Char;
    int  m_iLuck;

    int  m_iEnemyKillCount, m_iPKCount, m_iRewardGold;
    int  m_iCurWeightLoad;

    char m_cAttackDiceThrow_SM;
    char m_cAttackDiceRange_SM;
    char m_cAttackDiceThrow_L;
    char m_cAttackDiceRange_L;
    char m_cAttackBonus_SM;
    char m_cAttackBonus_L;

    std::vector<item *> item_list;
    std::vector<item *> bank_item_list;

    short m_sItemEquipmentStatus[DEF_MAXITEMEQUIPPOS];
    item * arrow;

    char           m_cMagicMastery[DEF_MAXMAGICTYPE];
    unsigned char  m_cSkillMastery[DEF_MAXSKILLTYPE];

    int   m_iSkillSSN[DEF_MAXSKILLTYPE];
    bool  m_bSkillUsingStatus[DEF_MAXSKILLTYPE];
    int   m_iSkillUsingTimeID[DEF_MAXSKILLTYPE];

    char  m_cMagicEffectStatus[DEF_MAXMAGICEFFECTS];

    std::weak_ptr<client> whisper_player;
    std::string profile;

    int   m_iHungerStatus;

    uint32_t m_dwWarBeginTime;
    bool m_bIsWarLocation;

    bool m_bIsPoisoned;
    int m_iPoisonLevel;
    uint32_t m_dwPoisonTime;

    int   m_iPenaltyBlockYear, m_iPenaltyBlockMonth, m_iPenaltyBlockDay;

    int   m_iFightzoneNumber, m_iReserveTime, m_iFightZoneTicketNumber;

    int   m_iAdminUserLevel;
    int   m_iRating;

    int   m_iTimeLeft_ShutUp;
    int   m_iTimeLeft_Rating;
    int   m_iTimeLeft_ForceRecall;
    int   m_iTimeLeft_FirmStaminar;

    bool  m_bIsOnServerChange;

    int   m_iExpStock;
    uint32_t m_dwExpStockTime;

    int   m_iAutoExpAmount;
    uint32_t m_dwAutoExpTime;

    uint32_t m_dwRecentAttackTime;
    uint32_t m_dwLastActionTime;

    int   m_iAllocatedFish;
    int   m_iFishChance;

    bool  m_bIsSafeAttackMode;

    bool  m_bIsOnWaitingProcess;

    int   m_iSuperAttackLeft;
    int   m_iSuperAttackCount;

    short m_sUsingWeaponSkill;

    int   m_iManaSaveRatio;

    bool  m_bIsLuckyEffect;
    int   m_iSideEffect_MaxHPdown;

    int   m_iComboAttackCount;
    int   m_iDownSkillIndex;

    int   m_iMagicDamageSaveItemIndex;

    short m_sCharIDnum1, m_sCharIDnum2, m_sCharIDnum3;

    int   m_iPartyRank;
    int   m_iPartyMemberCount;
    int64_t m_iPartyGUID;
    struct {
        int  iIndex;
        char cName[11];

    } m_stPartyMemberName[DEF_MAXPARTYMEMBERS];

    int   m_iAbuseCount;

    bool  m_bIsBWMonitor;
    bool  m_bIsManager;	


    bool  m_bIsExchangeMode;
    std::weak_ptr<client> exchange_player;

    bool  m_bIsExchangeConfirm;

    int   m_iQuest;
    int   m_iQuestID;
    int   m_iAskedQuest;
    int   m_iCurQuestCount;

    int   m_iQuestRewardType;
    int   m_iQuestRewardAmount;

    int   m_iContribution;

    bool  m_bQuestMatchFlag_Loc;
    bool  m_bIsQuestCompleted;

    int   m_iCustomItemValue_Attack;
    int   m_iCustomItemValue_Defense;

    int   m_iMinAP_SM;
    int   m_iMinAP_L;

    int   m_iMaxAP_SM;
    int   m_iMaxAP_L;

    bool  m_bIsNeutral;
    bool  m_bIsObserverMode;

    int   m_iSpecialEventID;

    int   m_iSpecialWeaponEffectType;
    int   m_iSpecialWeaponEffectValue;

    int   m_iAddHP, m_iAddSP, m_iAddMP;
    int   m_iAddAR, m_iAddPR, m_iAddDR;
    int   m_iAddMR, m_iAddAbsPD, m_iAddAbsMD;
    int   m_iAddCD, m_iAddExp, m_iAddGold;

    int   m_iAddTransMana, m_iAddChargeCritical;

    int   m_iAddResistMagic;
    int   m_iAddPhysicalDamage;
    int   m_iAddMagicalDamage;

    int   m_iAddAbsAir;
    int   m_iAddAbsEarth;
    int   m_iAddAbsFire;
    int   m_iAddAbsWater;

    int   m_iLastDamage;

    int   m_iMoveMsgRecvCount, m_iAttackMsgRecvCount, m_iRunMsgRecvCount, m_iSkillMsgRecvCount;
    uint32_t m_dwMoveLAT, m_dwRunLAT, m_dwAttackLAT;

    int   m_iSpecialAbilityTime;
    bool  m_bIsSpecialAbilityEnabled;
    uint32_t m_dwSpecialAbilityStartTime;
    int   m_iSpecialAbilityLastSec;

    int   m_iSpecialAbilityType;
    int   m_iSpecialAbilityEquipPos;
    bool  m_bIsAdminCreateItemEnabled;
    bool  m_bIsAdminCommandEnabled;
    int   m_iAlterItemDropIndex; 

    int   m_iWarContribution;

    uint32_t m_dwSpeedHackCheckTime;
    int   m_iSpeedHackCheckExp;

    uint32_t m_dwInitCCTimeRcv;
    uint32_t m_dwInitCCTime;

    std::string locked_map_name;
    int   m_iLockedMapTime;
    int   m_iDeadPenaltyTime;

    int   m_iCrusadeDuty;
    uint32_t m_dwCrusadeGUID;

    struct {
        char cType;
        char cSide;
        short sX, sY;
    } m_stCrusadeStructureInfo[DEF_MAXCRUSADESTRUCTURES];
    int m_iCSIsendPoint;

    std::string sending_map_name;
    bool m_bIsSendingMapStatus;

    int  m_iConstructionPoint;

    std::string construct_map_name;
    int  m_iConstructLocX, m_iConstructLocY;

    uint32_t m_dwFightzoneDeadTime;

    bool m_bIsBankModified;

    uint32_t m_dwCharID;

    int m_iPartyID;
    int m_iPartyStatus;
    int m_iReqJoinPartyClientH;
    char m_cReqJoinPartyName[12];

    int m_iGizonItemUpgradeLeft;

    uint32_t m_dwAttackFreqTime, m_dwMagicFreqTime, m_dwMoveFreqTime;
    bool m_bIsMoveBlocked;
    bool m_bIsAttackModeChange;
    int  m_iIsOnTown;
    bool m_bIsOnShop;
    bool m_bIsHunter;
    bool m_bIsOnTower;
    bool m_bIsOnWarehouse;
    bool m_bIsInBuilding;

    uint32_t m_dwWarmEffectTime;

    /// Time client disconnected
    uint64_t disconnect_time;
    /// Last time client sent a message
    uint64_t last_packet_time;
    /// Old client
    uint64_t handle;
    /// Time when logout was initiated from the client
    uint64_t logout_hack_time;

    int8_t server_id;
};

}
