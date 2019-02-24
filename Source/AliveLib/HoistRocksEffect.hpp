#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"
#include "Animation.hpp"

struct HoistRockParticle
{
    __int16 field_0_state;
    __int16 field_2;
    FP field_4_xpos;
    FP field_8_ypos;
    FP field_C_yVel;
    Animation field_10_mAnim;
};
ALIVE_ASSERT_SIZEOF(HoistRockParticle, 0xA8);

struct Path_Hoist;

class HoistRocksEffect : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VScreenChanged() override;
    EXPORT HoistRocksEffect* ctor_45D270(Path_Hoist* pTlv, int tlvInfo);
private:
    EXPORT HoistRocksEffect* vdtor_45D430(signed int flags);
    EXPORT void dtor_45D6D0();
    EXPORT void Update_45D460();
    EXPORT void Render_45D7B0(int** pOrderingTable);
    EXPORT void vScreenChanged_45D790();
private:
    __int16 field_20_xpos;
    __int16 field_22_ypos;
    int field_24_tlvInfo;
    int field_28_timer;
    FP field_2C_scale;
    HoistRockParticle field_30_rocks[4];
};
ALIVE_ASSERT_SIZEOF(HoistRocksEffect, 0x2D0);
