#pragma once

#include "BaseAliveGameObject.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_RockSack final : public Path_TLV
{
    XDirection_short field_18_fall_direction;
    s16 field_1A_x_vel;
    s16 field_1C_y_vel;
    Scale_short field_1E_scale;
    s16 field_20_rock_amount;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF(Path_RockSack, 0x24);

class RockSack final : public BaseAliveGameObject
{
public:
    EXPORT RockSack* ctor_4573F0(Path_RockSack* pTlv, s32 tlvInfo);

    EXPORT BaseGameObject* dtor_457580();

    virtual void VUpdate() override;

    EXPORT void VUpdate_4575F0();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT RockSack* Vdtor_4578A0(s32 flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_457890();

    s32 field_10C_tlvInfo;
    s16 field_110_has_been_hit;
    s16 field_112_rock_amount;
    s16 field_114_can_play_wobble_sound;
    s16 field_116_force_wobble_sound;
    FP field_118_x_vel;
    FP field_11C_y_vel;
};
ALIVE_ASSERT_SIZEOF(RockSack, 0x120);
}
