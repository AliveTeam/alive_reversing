#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_Bat final : public Path_TLV
{
    s16 field_18_ticks_before_moving;
    s16 field_1A_speed;
    Scale_short field_1C_scale;
    s16 field_1E_attack_duration;
};
ALIVE_ASSERT_SIZEOF(Path_Bat, 0x20);

class PathLine;

class Bat final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Bat(Path_Bat* pTlv, s32 tlvInfo);
    ~Bat();
    
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    void FlyTo(FP xpos, FP ypos, FP* xSpeed, FP* ySpeed);

    PathLine* field_E4_pLine = nullptr;
    FP field_E8_speed = FP_FromInteger(0);
    s32 field_EC_ticks_before_moving = 0;
    s32 field_F0_tlvInfo = 0;
    enum class BatStates : s16
    {
        eSetTimer_0 = 0,
        eInit_1 = 1,
        eStartMoving_2 = 2,
        eFlying_3 = 3,
        eAttackTarget_4 = 4,
        eFlyAwayAndDie_5 = 5,
    };
    BatStates field_F4_state = BatStates::eSetTimer_0;
    s16 field_F6_attack_duration = 0;
    s32 field_F8_timer = 0;
    s32 field_FC_attack_duration_timer = 0;
    FP field_100_velx = FP_FromInteger(0);
    FP field_104_target_xpos = FP_FromInteger(0);
    FP field_108_target_ypos = FP_FromInteger(0);
    BaseAliveGameObject* field_10C = nullptr;
};
ALIVE_ASSERT_SIZEOF(Bat, 0x110);

} // namespace AO
