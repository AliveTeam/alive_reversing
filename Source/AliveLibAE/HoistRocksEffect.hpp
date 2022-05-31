#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"
#include "Animation.hpp"

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
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    HoistRocksEffect(Path_Hoist* pTlv, s32 tlvInfo);
    ~HoistRocksEffect();

private:
    void Update_45D460();
    void Render_45D7B0(PrimHeader** ppOt);
    void vScreenChanged_45D790();

private:
    s16 field_20_xpos;
    s16 field_22_ypos;
    s32 field_24_tlvInfo;
    s32 field_28_timer;
    FP field_2C_scale;
    HoistRockParticle field_30_rocks[4];
};
ALIVE_ASSERT_SIZEOF(HoistRocksEffect, 0x2D0);
