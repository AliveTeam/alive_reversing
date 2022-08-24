#pragma once

#include "BaseAliveGameObject.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_RockSack final : public Path_TLV
{
    XDirection_short mRockFallDirection;
    s16 mVelX;
    s16 mVelY;
    Scale_short mScale;
    s16 mRockAmount;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_RockSack, 0x24);

class RockSack final : public BaseAliveGameObject
{
public:
    RockSack(relive::Path_RockSack* pTlv, s32 tlvInfo);
    ~RockSack();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    s32 field_10C_tlvInfo = 0;
    s16 field_110_has_been_hit = 0;
    s16 field_112_rock_amount = 0;
    s16 field_114_can_play_wobble_sound = 0;
    s16 field_116_force_wobble_sound = 0;
    FP field_118_x_vel = {};
    FP field_11C_y_vel = {};
};
ALIVE_ASSERT_SIZEOF(RockSack, 0x120);
}
