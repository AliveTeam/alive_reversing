#pragma once

#include "Animation.hpp"
#include "Animation.hpp"
#include "FixedPoint.hpp"

class BaseGameObject;

class OrbWhirlWindParticle
{
public:
    EXPORT OrbWhirlWindParticle* ctor_4E40C0(FP xpos, FP ypos, FP scale, s16 bIsMudokonSpirit);
    EXPORT int IsActive_4E4370();
    EXPORT void Spin_4E4A10(FP xpos, FP ypos, FP scale, BaseGameObject* pObj);
    EXPORT void ToStop_4E4AD0();
    
    // WARNING: Not a derived type - keep virtuals in order for BC
    EXPORT virtual OrbWhirlWindParticle* vdtor_4E42D0(char flags);
    EXPORT virtual void VUpdate_4E4510();
    EXPORT virtual void VRender_4E4B10(PrimHeader** ppOt);

private:
    EXPORT void dtor_4E4300();
    EXPORT void CalculateRenderProperties_4E4390(s16 bStarted);
    EXPORT void SetActive_4E4340(u8 active);

private:
    enum Flags_4
    {
        eBit1_is_active = 0x1
    };
    BitField16<Flags_4> field_4_flags;
    s16 field_6_padding;
    Animation field_8_Anim;
    FP field_A0_xpos_render_offset;
    FP field_A4_ypos_render_offset;
    FP field_A8_render_as_scale;
    FP field_AC_radiusX_offset;
    FP field_B0_ypos_increment;

    enum class State : s16
    {
        State_0_Start = 0,
        State_1_Spin = 1,
        State_2_FlyToTarget = 2,
        State_3_SpinAtTarget = 3,
        State_4_Stop = 4,
    };

    State field_B4_state;

    s16 field_B6_unused;
    int field_B8_render_angle;
    s16 field_BC_counter;
    s16 field_BE_max;
    FP field_C0_current_scale;
    FP field_C4_randomized_scale;
    FP field_C8_scale_offset_fly_to_target;
    FP field_CC_xpos_mid;
    FP field_D0_ypos_mid;
    FP field_D4_radiusX;
    FP field_D8_radiusY;
    int field_DC_position_timer;
    FP field_E0_yMove;
    BaseGameObject* field_E4_pObj;
    FP field_E8_xpos;
    FP field_EC_ypos;
    FP field_F0_scale;
    FP field_F4_xpos_offset;
    FP field_F8_ypos_offset;
    FP field_FC_xpos_offset2;
    FP field_100_ypos_offset2;
    FP field_104_scale_offset_spin_at_target;
    BYTE** field_108_res;
};
ALIVE_ASSERT_SIZEOF(OrbWhirlWindParticle, 0x10C);
