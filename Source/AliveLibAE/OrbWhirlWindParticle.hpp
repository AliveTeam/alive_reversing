#pragma once

#include "Animation.hpp"
#include "Animation.hpp"
#include "FixedPoint.hpp"

class BaseGameObject;

class OrbWhirlWindParticle final
{
public:
    OrbWhirlWindParticle(FP xpos, FP ypos, FP scale, s16 bIsMudokonSpirit);
    ~OrbWhirlWindParticle();

    s32 IsActive();
    void Spin(FP xpos, FP ypos, FP scale, BaseGameObject* pObj);
    void ToStop();

    void Update();
    void Render(PrimHeader** ppOt);

private:
    void CalculateRenderProperties(s16 bStarted);
    void SetActive(u8 active);

private:
    enum Flags_4
    {
        eBit1_is_active = 0x1
    };
    BitField16<Flags_4> mAnimFlags;
    s16 field_6_padding;
    Animation field_8_Anim = {};
    FP field_A0_xpos_render_offset = {};
    FP field_A4_ypos_render_offset = {};
    FP field_A8_render_as_scale = {};
    FP field_AC_radiusX_offset = {};
    FP field_B0_ypos_increment = {};

    enum class State : s16
    {
        State_0_Start = 0,
        State_1_Spin = 1,
        State_2_FlyToTarget = 2,
        State_3_SpinAtTarget = 3,
        State_4_Stop = 4,
    };

    State field_B4_state = State::State_0_Start;

    s16 field_B6_unused = 0;
    s32 field_B8_render_angle = 0;
    s16 field_BC_counter = 0;
    s16 field_BE_max = 0;
    FP field_C0_current_scale = {};
    FP field_C4_randomized_scale = {};
    FP field_C8_scale_offset_fly_to_target = {};
    FP field_CC_xpos_mid = {};
    FP field_D0_ypos_mid = {};
    FP field_D4_radiusX = {};
    FP field_D8_radiusY = {};
    s32 field_DC_position_timer = 0;
    FP field_E0_yMove = {};
    BaseGameObject* field_E4_pObj = nullptr;
    FP field_E8_xpos = {};
    FP field_EC_ypos = {};
    FP field_F0_scale = {};
    FP field_F4_xpos_offset = {};
    FP field_F8_ypos_offset = {};
    FP field_FC_xpos_offset2 = {};
    FP field_100_ypos_offset2 = {};
    FP field_104_scale_offset_spin_at_target = {};
    u8** field_108_res = nullptr;
};
