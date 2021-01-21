#include "unit.hpp"
#include "gserver.hpp"
#include "net/netmessages.hpp"
#include "map.hpp"
#include "npc.hpp"
#include "gserver.hpp"
#include "client.hpp"
#include "defines.hpp"

namespace hbx
{

unit::unit(uint64_t handle_)
    : handle(handle_)
{
    m_uid = gen_uid();
    dead_ = false;
    killer = nullptr;
    type_original_ = 0;
    status = 0;
    map_ = nullptr;
}

void unit::set_status_flag(long flag, bool enabled)
{
    if (enabled)
        status |= flag;
    else
        status &= ~flag;

    gs->send_event_near_type_a(shared_from_this(), MSGID_MOTION_NULL, 0, 0, 0);
}

void unit::toggle_status_flag(long flag)
{
    status ^= flag;

    gs->send_event_near_type_a(shared_from_this(), MSGID_MOTION_NULL, 0, 0, 0);
}

bool unit::get_status_flag(long flag) const
{
    return (status & flag) ? true : false;
}

void unit::set_magic_flag(int16_t magic_type_, bool enabled)
{
    long flag = 0;

    switch (magic_type_)
    {
        case MAGICTYPE_INVISIBILITY:
            flag = STATUS_INVISIBILITY;
            break;
        case MAGICTYPE_ICE:
        case MAGICTYPE_ICE_LINEAR:
        case MAGICTYPE_MB_LINEAR:
        case MAGICTYPE_COTG_LINEAR:
            flag = STATUS_FROZEN;
            break;
        case MAGICTYPE_BERSERK:
            flag = STATUS_BERSERK;
            break;
        case MAGICTYPE_PROTECT:
            switch (magic_effect_status[magic_type_])
            {
                case MAGICPROTECT_PFA:
                    flag = STATUS_PFA;
                    break;
                case MAGICPROTECT_PFM:
                    //case MAGICPROTECT_GPFM:
                case MAGICPROTECT_AMP:
                    flag = STATUS_PFM;
                    break;
                case MAGICPROTECT_DS:
                case MAGICPROTECT_GDS:
                    flag = STATUS_DEFENSESHIELD;
                    break;
            }
            break;
        case MAGICTYPE_CONFUSE:
            switch (magic_effect_status[magic_type_])
            {
                case 1:	// Language confuse
                    break;
                case 2:	// Confusion
                    break;
                case 3:
                    flag = STATUS_ILLUSION;
                    break;
                case 4:
                    flag = STATUS_ILLUSIONMOVEMENT;
                    break;
            }
            break;
        default:
            // No flag
            return;
    }

    set_status_flag(flag, enabled);
}

bool unit::add_magic_effect(int16_t magic_type_, uint64_t effectTime, int8_t kind)
{
    //char magicType = spell->m_sType;
    uint64_t  dwTime = utility::current_time();

    if (magic_effect_status[magic_type_])
        return false;

    magic_effect_status[magic_type_] = kind;

    // 	if (effectTime > 0)
    // 		gs->RegisterDelayEvent(DELAYEVENTTYPE_MAGICRELEASE, magicType, dwTime + (effectTime)_s, 
    // 			this, 0, 0, 0, kind, 0, 0);

    if (is_player())
        gs->SendNotifyMsg(nullptr, std::static_pointer_cast<client>(shared_from_this()), NOTIFY_MAGICEFFECTON, magic_type_, kind, 0);

    set_magic_flag(magic_type_, true);
    return true;
}

bool unit::remove_magic_effect(int8_t magic_type_)
{
    if (!magic_effect_status[magic_type_])
        return false;

    if (is_player())
        gs->SendNotifyMsg(nullptr, std::static_pointer_cast<client>(shared_from_this()), NOTIFY_MAGICEFFECTOFF, magic_type_, magic_effect_status[magic_type_], 0);

    set_magic_flag(magic_type_, false);
    magic_effect_status[magic_type_] = 0;
    gs->RemoveFromDelayEventList(shared_from_this(), magic_type_);
    return true;
}

void unit::set_side_flag(faction side)
{
    //	SetNibble(m_iStatus, 7, side);
}

void unit::SetPoison(uint32_t level, uint64_t time)
{
    if (poison_level == 0 || poison_time == 0)
    {
        set_status_flag(STATUS_POISON, false);
        poison_level = 0;
        poison_time = 0;
        return;
    }
    set_status_flag(STATUS_POISON, true);
    poison_level = level;
    poison_time = time;
}

bool unit::CheckResistMagic(char direction, int hitratio)
{
    int targetMagicResistRatio = 0;
    if (is_player())
    {
        std::shared_ptr<client> player = std::static_pointer_cast<client>(shared_from_this());
        if (player->IsInvincible()) return true;

        targetMagicResistRatio = player->skillMastery[SKILL_MAGICRES] + player->m_iAddMR;
        if (player->GetMag() > 50)
            targetMagicResistRatio += (player->GetMag() - 50);

        targetMagicResistRatio += player->m_iAddResistMagic;
        if ((player->status & STATUS_REDSLATE) != 0) return true;
    }
    else
    {
        targetMagicResistRatio = std::static_pointer_cast<npc>(shared_from_this())->m_cResistMagic;
    }

    if (magic_effect_status[MAGICTYPE_PROTECT] == MAGICPROTECT_AMP) return true;

    if (magic_effect_status[MAGICTYPE_PROTECT] == MAGICPROTECT_PFM && hitratio < 1000) return true;

    if (hitratio >= 10000) hitratio -= 10000;

    if (targetMagicResistRatio < 1) targetMagicResistRatio = 1;

    hitratio = int((float(hitratio) / targetMagicResistRatio) / 50.0f);

    if (hitratio < MINIMUMHITRATIO) hitratio = MINIMUMHITRATIO;
    if (hitratio > MAXIMUMHITRATIO) hitratio = MAXIMUMHITRATIO;

    if (utility::gen_rand(1, 100) <= hitratio) return false;

    if (is_player()) std::static_pointer_cast<client>(shared_from_this())->CalculateSSN_SkillIndex(SKILL_MAGICRES, 1);
    return true;
}

bool unit::CheckResistPoison()
{
    int64_t iResist, iResult;

    if (is_player())
    {
        std::shared_ptr<client> client_ = std::static_pointer_cast<client>(shared_from_this());
        iResist = int64_t(client_->skillMastery[SKILL_POISONRES]) + client_->m_iAddPR;
    }
    else if (is_npc())
    {
        iResist = 0;
    }
    else
    {
        return true;
    }

    iResult = utility::gen_rand(1, 100);
    if (iResult >= iResist)
        return false;

    if (is_player())
        std::static_pointer_cast<client>(shared_from_this())->CalculateSSN_SkillIndex(SKILL_POISONRES, 1);

    return true;
}

//TODO: override this as virtual. holy shit what was i thinking.
bool unit::CheckResistIce(char direction, int iHitRatio)
{
    uint64_t iTargetIceResistRatio, iResult;

    if (is_player())
    {
        std::shared_ptr<client> client_ = std::static_pointer_cast<client>(shared_from_this());

        if (client_->IsInvincible()) return true;

        iTargetIceResistRatio = client_->IceResist();

        if (client_->m_dwWarmEffectTime == 0) {
        }
        else if ((utility::current_time() - client_->m_dwWarmEffectTime) < 1000 * 30) return true;
    }
    else if (is_npc())
    {
        std::shared_ptr<npc> npc_ = std::static_pointer_cast<npc>(shared_from_this());
        if (npc_->element == ELEMENT_WATER) return true;
        iTargetIceResistRatio = uint64_t(npc_->m_cResistMagic) - (npc_->m_cResistMagic / 3);
    }
    else
    {
        return true;
    }

    if (iTargetIceResistRatio < 1) iTargetIceResistRatio = 1;

    iResult = utility::gen_rand(1, 100);
    if (iResult <= iTargetIceResistRatio) return true;

    return false;
}

}
