#pragma once

#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

struct Path_RockSack final : public Path_TLV
{
    XDirection_short field_10_fall_direction;
    u16 field_12_x_vel;
    u16 field_14_y_vel;
    Scale_short field_16_scale;
    s16 field_18_rock_amount;
    s16 field_1A_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_RockSack, 0x1C);

class RockSack final : public BaseAliveGameObject
{
public:
    RockSack(Path_RockSack* pTlv, s32 tlvInfo);
    ~RockSack();
    
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

private:
    s32 field_118_tlvInfo = 0;
    s16 field_11C_has_been_hit = 0;
    s16 field_11E_rock_amount = 0;
    s16 field_120_can_play_wobble_sound = 0;
    s16 field_122_force_wobble_sound = 0; // Makes sure RockSacks play the sound when you first encounter them. Unused afterwards. -- Nemin (6/7/2020)
    FP field_124_x_vel = {};
    FP field_128_y_vel = {};
};
ALIVE_ASSERT_SIZEOF(RockSack, 0x12C);
