
#pragma once

#include <cstdint>
#include <memory>
#include "utility.hpp"
#include "defines.hpp"

namespace hbx
{

class gserver;
class map;

class unit : public std::enable_shared_from_this<unit>
{
public:

    unit(uint64_t handle_);

    virtual ~unit() = default;


    void set_status_flag(long flag, bool enabled);
    void toggle_status_flag(long flag);
    bool get_status_flag(long flag) const;
    void set_magic_flag(int16_t magic_type_, bool enabled);
    bool add_magic_effect(int16_t magic_type_, uint64_t effectTime, int8_t kind = 1);
    bool remove_magic_effect(int8_t magic_type_);
    void set_side_flag(faction side);

    bool is_dead()      const noexcept { return dead_; }
    bool is_berserked() const noexcept { return !!magic_effect_status[magic_type::mt_berserk]; };
    bool is_invisible() const noexcept { return !!magic_effect_status[magic_type::mt_invisibility]; };
    bool is_player()    const noexcept { return owner_type_ == ot_player; };
    bool is_npc()       const noexcept { return owner_type_ == ot_npc; };
    bool is_neutral()   const noexcept { return side == faction::neutral; }
    bool is_ares()      const noexcept { return side == faction::aresden; }
    bool is_elv()       const noexcept { return side == faction::elvine; }

    void set_owner_type(owner_type type) noexcept { owner_type_ = type; }
    void set_type(int8_t type) noexcept { type_ = type; }
    void set_type_original(int8_t type) noexcept { type_original_ = type; }
    owner_type get_owner_type() const noexcept { return owner_type_; }
    int8_t get_type() const noexcept { return type_; }
    int8_t get_type_original() const noexcept { return type_original_; }

    virtual float GetDropFactor() const noexcept { return 1.0f; }

    bool IsPoisoned() const noexcept { return poison_level > 0; }
    int32_t PoisonLevel() const noexcept { return poison_level; }

    void SetPoison(uint32_t level, uint64_t time);

    bool CheckResistMagic(char direction, int hitratio);
    bool CheckResistPoison();
    bool CheckResistIce(char direction, int iHitRatio);

    //char name[20];
    std::string name;

    map * map_ = nullptr;
    gserver * gs = nullptr;
    uint64_t handle;

    //stick in stats struct or something?
    int16_t x = -1;
    int16_t y = -1;
    int64_t health;
    int64_t mana;
    uint64_t experience;
    faction side = faction::neutral;
    uint32_t status;
    bool dead_;
    std::shared_ptr<unit> killer;
    int8_t direction = 5;



    uint64_t head_appr;
    uint64_t body_appr;
    uint64_t arm_appr;
    uint64_t leg_appr;

    uint64_t appr1;
    uint64_t appr2;
    uint64_t appr3;
    uint64_t appr4;
    uint64_t appr_color;


    int32_t  poison_level = 0;
    uint64_t poison_time = 0;

    int8_t  magic_effect_status[DEF_MAXMAGICEFFECTS];

    uint64_t m_uid;
    static uint64_t gen_uid()
    {
        static uint64_t uids = 0;
        return uids++;
    }

private:
    //confusing Helbreath names amuck!
    int8_t type_ = 0;//model type
    int8_t type_original_;//original model type
    owner_type owner_type_ = ot_none;//whether is player or npc
};

}
