#pragma once

#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

struct Path_RockSack : public Path_TLV
{
    XDirection_short field_10_fall_direction;
    u16 field_12_x_vel;
    u16 field_14_y_vel;
    Scale_short field_16_scale;
    s16 field_18_rock_amount;
    s16 field_1A_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_RockSack, 0x1C);

class RockSack : public BaseAliveGameObject
{
public:
    EXPORT RockSack* ctor_49F100(Path_RockSack* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

private:
    EXPORT RockSack* vdtor_49F2E0(signed int flags);
    EXPORT void dtor_49F310();
    EXPORT void vScreenChanged_49F700();
    EXPORT void vUpdate_49F3A0();

private:
    int field_118_tlvInfo;
    s16 field_11C_has_been_hit;
    s16 field_11E_rock_amount;
    s16 field_120_can_play_wobble_sound;
    s16 field_122_force_wobble_sound; // Makes sure RockSacks play the sound when you first encounter them. Unused afterwards. -- Nemin (6/7/2020)
    FP field_124_x_vel;
    FP field_128_y_vel;
};
ALIVE_ASSERT_SIZEOF(RockSack, 0x12C);
