#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Animation.hpp"
#include "FixedPoint.hpp"

namespace AO {

class BaseAliveGameObject;

class OrbWhirlWindParticle final
{
public:
    OrbWhirlWindParticle(FP xpos, FP ypos, FP scale);
    ~OrbWhirlWindParticle();

    // WARNING: Not a derived type - keep virtuals in order for BC

    virtual void VUpdate_48BF00();

    void SetActive(u8 active);

    virtual void VRender_48C430(PrimHeader** ppOt);

    // NOTE: inlined
    s32 IsActive();

    // NOTE: inlined
    void ToStop();

    void CalculateRenderProperties_48BDC0(s16 bStarted);

    void Spin(FP xpos, FP ypos, BaseAliveGameObject* pObj);

    s32 field_4_flags;
    Animation field_8_anim;
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
    s32 field_B8_render_angle;
    s16 field_BC_counter;
    s16 field_BE_max;
    FP field_C0_current_scale;
    FP field_C4_randomized_scale;
    FP field_C8_scale_offset_fly_to_target;
    FP field_CC_xpos_mid;
    FP field_D0_ypos_mid;
    FP field_D4_radiusX;
    FP field_D8_radiosY;
    s32 field_DC_position_timer;
    FP field_E0_yMove;
    BaseAliveGameObject* field_E4_pObj;
    FP field_E8_xpos;
    FP field_EC_ypos;
    FP field_F0_scale;
    FP field_F4_xpos_offset;
    FP field_F8_ypos_offset;
    FP field_FC_xpos_offset2;
    FP field_104_scale_offset_spin_at_target;
};
ALIVE_ASSERT_SIZEOF(OrbWhirlWindParticle, 0x104);

} // namespace AO
