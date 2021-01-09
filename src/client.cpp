
#include "utility.hpp"
#include "client.hpp"
#include "net/socket.hpp"

namespace hbx
{

client::client()
{
    profile = "__________";

    location = "NONE";
    m_iGuildRank = -1;
    m_iGuildGUID = -1;

    m_bIsInitComplete = false;

    m_cLU_Str = m_cLU_Int = m_cLU_Vit = m_cLU_Dex = m_cLU_Mag = m_cLU_Char = 0;

    m_iEnemyKillCount = 0;
    m_iPKCount = 0;
    m_iRewardGold = 0;
    m_iCurWeightLoad = 0;

    m_bIsSafeAttackMode = false;

    for (int i = 0; i < DEF_MAXITEMEQUIPPOS; i++)
        m_sItemEquipmentStatus[i] = -1;

    arrow = nullptr;

    for (int i = 0; i < DEF_MAXMAGICTYPE; i++)
        m_cMagicMastery[i] = 0;

    for (int i = 0; i < DEF_MAXSKILLTYPE; i++)
        m_cSkillMastery[i] = 0;

    for (int i = 0; i < DEF_MAXSKILLTYPE; i++) {
        m_bSkillUsingStatus[i] = false;
        m_iSkillUsingTimeID[i] = 0;
    }

    m_cMapIndex = -1;
    m_sX = -1;
    m_sY = -1;
    m_cDir = 5;
    m_sType = 0;
    m_sOriginalType = 0;
    m_sAppr1 = 0;
    m_sAppr2 = 0;
    m_sAppr3 = 0;
    m_sAppr4 = 0;
    m_iApprColor = 0;
    m_sStatus = 0;

    m_cSex = 0;
    m_cSkin = 0;
    m_cHairStyle = 0;
    m_cHairColor = 0;
    m_cUnderwear = 0;

    m_cAttackDiceThrow_SM = 0;
    m_cAttackDiceRange_SM = 0;
    m_cAttackDiceThrow_L = 0;
    m_cAttackDiceRange_L = 0;
    m_cAttackBonus_SM = 0;
    m_cAttackBonus_L = 0;

    m_cSide = 0;

    m_iHitRatio = 0;
    m_iDefenseRatio = 0;

    for (int i = 0; i < DEF_MAXITEMEQUIPPOS; i++) m_iDamageAbsorption_Armor[i] = 0;
    m_iDamageAbsorption_Shield = 0;

    m_iHPstock = 0;
    m_iHPStatic_stock = 0;

    m_bIsKilled = false;

    for (int i = 0; i < DEF_MAXMAGICEFFECTS; i++)
        m_cMagicEffectStatus[i] = 0;

    m_iHungerStatus = 100;

    m_bIsWarLocation = false;

    m_bIsPoisoned = false;
    m_iPoisonLevel = 0;

    admin_level = 0;
    m_iRating = 0;
    m_iTimeLeft_ShutUp = 0;
    m_iTimeLeft_Rating = 0;
    m_iTimeLeft_ForceRecall = 0;
    m_iTimeLeft_FirmStaminar = 0;

    m_bIsOnServerChange = false;

    m_iExpStock = 0;

    m_iAllocatedFish = 0;
    m_iFishChance = 0;

    m_bIsOnWaitingProcess = false;

    m_iSuperAttackLeft = 0;
    m_iSuperAttackCount = 0;

    m_sUsingWeaponSkill = 5;

    m_iManaSaveRatio = 0;
    m_iAddResistMagic = 0;
    m_iAddPhysicalDamage = 0;
    m_iAddMagicalDamage = 0;
    m_bIsLuckyEffect = false;
    m_iSideEffect_MaxHPdown = 0;

    m_iAddAbsAir = 0;
    m_iAddAbsEarth = 0;
    m_iAddAbsFire = 0;
    m_iAddAbsWater = 0;

    m_iComboAttackCount = 0;
    m_iDownSkillIndex = -1;

    m_iMagicDamageSaveItemIndex = -1;

    m_sCharIDnum1 = m_sCharIDnum2 = m_sCharIDnum3 = 0;

    m_iPartyRank = -1;
    m_iPartyMemberCount = 0;
    m_iPartyGUID = 0;

    for (int i = 0; i < DEF_MAXPARTYMEMBERS; i++) {
        m_stPartyMemberName[i].iIndex = 0;
        memset(m_stPartyMemberName[i].cName, 0, sizeof(m_stPartyMemberName[i].cName));
    }

    m_iAbuseCount = 0;
    m_bIsBWMonitor = false;
    m_bIsExchangeMode = false;

    m_iFightZoneTicketNumber = m_iFightzoneNumber = m_iReserveTime = 0;

    m_iPenaltyBlockYear = m_iPenaltyBlockMonth = m_iPenaltyBlockDay = 0;

    m_bIsExchangeConfirm = false;

    m_iQuest = 0;
    m_iQuestID = 0;
    m_iAskedQuest = 0;
    m_iCurQuestCount = 0;

    m_iQuestRewardType = 0;
    m_iQuestRewardAmount = 0; 

    m_iContribution = 0;
    m_bQuestMatchFlag_Loc = false;
    m_bIsQuestCompleted = false;

    m_bIsNeutral = false;
    m_bIsObserverMode = false;

    m_iSpecialEventID = 200081;

    m_iSpecialWeaponEffectType = 0;
    m_iSpecialWeaponEffectValue = 0;

    m_iAddHP = m_iAddSP = m_iAddMP = 0;
    m_iAddAR = m_iAddPR = m_iAddDR = 0;
    m_iAddAbsPD = m_iAddAbsMD = 0;
    m_iAddCD = m_iAddExp = m_iAddGold = 0;

    m_iSpecialAbilityTime = DEF_SPECABLTYTIMESEC;
    m_iSpecialAbilityType = 0;
    m_bIsSpecialAbilityEnabled = false;
    m_iSpecialAbilityLastSec = 0;

    m_iSpecialAbilityEquipPos = 0;

    m_iMoveMsgRecvCount = 0;
    m_iAttackMsgRecvCount = 0;
    m_iRunMsgRecvCount = 0;
    m_iSkillMsgRecvCount = 0;

    m_bIsAdminCreateItemEnabled = false;
    m_bIsAdminCommandEnabled = false;
    m_iAlterItemDropIndex = -1;

    m_iAutoExpAmount = 0;
    m_iWarContribution = 0;

    m_dwMoveLAT = m_dwRunLAT = m_dwAttackLAT = 0;

    m_dwInitCCTimeRcv = 0;
    m_dwInitCCTime = 0;

    m_iLockedMapTime = 0;
    m_iDeadPenaltyTime = 0;

    m_iCrusadeDuty = 0;
    m_dwCrusadeGUID = 0;

    for (int i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++) {
        m_stCrusadeStructureInfo[i].cType = 0;
        m_stCrusadeStructureInfo[i].cSide = 0;
        m_stCrusadeStructureInfo[i].sX = 0;
        m_stCrusadeStructureInfo[i].sY = 0;
    }
    m_iCSIsendPoint = 0;

    m_bIsSendingMapStatus = false;

    m_iConstructionPoint = 0;

    m_iConstructLocX = m_iConstructLocY = -1;

    m_dwFightzoneDeadTime = 0;

    m_iPartyID = 0;
    m_iPartyStatus = DEF_PARTYSTATUS_0;

    m_iReqJoinPartyClientH = 0;
    memset(m_cReqJoinPartyName, 0, sizeof(m_cReqJoinPartyName));

    m_dwLastActionTime = 0;
    m_bIsCheckingWhisperPlayer = false;

    m_bIsBankModified = false;

    m_iGizonItemUpgradeLeft = 0;

    m_dwAttackFreqTime = m_dwMagicFreqTime = m_dwMoveFreqTime = 0;
    m_bIsMoveBlocked = false;
    m_bIsAttackModeChange = false;
    m_iIsOnTown = false;
    m_bIsOnShop = false;
    m_bIsOnTower = false;
    m_bIsOnWarehouse = false;
    m_bIsHunter = false;
    m_dwWarmEffectTime = 0; 
    m_bIsInBuilding = false;
}

client::~client()
{
    if (socket_)
        socket_->stop();
    for (auto it : item_list)
        delete it;
    for (auto it : bank_item_list)
        delete it;
}


bool client::bCreateNewParty()
{
    if (m_iPartyRank != -1) return false;

    m_iPartyRank = 0;
    m_iPartyMemberCount = 0;
    m_iPartyGUID = (rand() % 999999) + utility::current_time();

    for (int i = 0; i < DEF_MAXPARTYMEMBERS; i++) {
        m_stPartyMemberName[i].iIndex = 0;
        memset(m_stPartyMemberName[i].cName, 0, sizeof(m_stPartyMemberName[i].cName));
    }

    return true;
}

}
