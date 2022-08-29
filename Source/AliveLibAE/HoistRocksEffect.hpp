#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "FixedPoint.hpp"
#include "../relive_lib/Animation.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

struct HoistRockParticle final
{
    s16 field_0_state;
    s16 field_2;
    FP field_4_xpos;
    FP field_8_ypos;
    FP field_C_yVel;
    Animation field_10_mAnim;
};
ALIVE_ASSERT_SIZEOF(HoistRockParticle, 0xA8);

struct Path_Hoist;

class HoistRocksEffect final : public BaseGameObject
{
public:
    HoistRocksEffect(relive::Path_Hoist* pTlv, const Guid& tlvId);
    ~HoistRocksEffect();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

private:
    s16 field_20_xpos = 0;
    s16 field_22_ypos = 0;
    Guid field_24_tlvInfo;
    s32 field_28_timer = 0;
    FP field_2C_scale = {};
    HoistRockParticle field_30_rocks[4] = {};
};
ALIVE_ASSERT_SIZEOF(HoistRocksEffect, 0x2D0);
