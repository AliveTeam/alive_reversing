#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"

namespace AO {

struct Path_Bat : public Path_TLV
{
    s16 field_18_ticks_before_moving;
    s16 field_1A_speed;
    u16 field_1C_scale;
    s16 field_1E_attack_duration;
};
ALIVE_ASSERT_SIZEOF(Path_Bat, 0x20);

class PathLine;

class Bat : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Bat* ctor_4046E0(Path_Bat* pTlv, s32 tlvInfo);
    EXPORT BaseGameObject* dtor_404870();

    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_404FE0();
    EXPORT Bat* Vdtor_404FF0(s32 flags);

    EXPORT void FlyTo_404E50(FP xpos, FP ypos, FP* xSpeed, FP* ySpeed);

    virtual void VUpdate() override;

    EXPORT void VUpdate_404950();

    s32 field_D4[4];
    PathLine* field_E4_pLine;
    FP field_E8_speed;
    s32 field_EC_ticks_before_moving;
    s32 field_F0_tlvInfo;
    enum class BatStates : s16
    {
        eSetTimer_0 = 0,
        eInit_1 = 1,
        eStartMoving_2 = 2,
        eFlying_3 = 3,
        eAttackTarget_4 = 4,
        eFlyAwayAndDie_5 = 5,
    };
    BatStates field_F4_state;
    s16 field_F6_attack_duration;
    s32 field_F8_timer;
    s32 field_FC_attack_duration_timer;
    FP field_100_velx;
    FP field_104_target_xpos;
    FP field_108_target_ypos;
    BaseAliveGameObject* field_10C;
};
ALIVE_ASSERT_SIZEOF(Bat, 0x110);

}

