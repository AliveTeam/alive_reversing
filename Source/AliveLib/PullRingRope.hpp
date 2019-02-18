#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"

enum SwitchOp : __int16;

struct Path_PullRingRope : public Path_TLV
{
    __int16 field_10_id;
    SwitchOp field_12_target_action;
    __int16 field_14_length_of_rope;
    __int16 field_16_scale;
    __int16 field_18_on_sound;
    __int16 field_1A_off_sound;
    __int16 field_1C_sound_direction;
    // pad
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_PullRingRope, 0x20);

class PullRingRope : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT PullRingRope* ctor_49B2D0(Path_PullRingRope* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual __int16 VPull_49BBD0(BaseGameObject* a2);
    virtual BOOL Vsub_49BC90();
    virtual void Vsub_49B610();
private:
    EXPORT PullRingRope* vdtor_49B630(signed int flags);
    EXPORT void dtor_49B660();
    EXPORT void vUpdate_49B720();
    EXPORT void vScreenChanged_49BCB0();
    EXPORT __int16 vPull_49BBD0(BaseGameObject* a2);
    EXPORT BOOL vsub_49BC90();
    EXPORT void vsub_49B610();
private:
    int field_E4_not_used[4];
    int field_F4_stay_in_state_ticks;
    int field_F8_rope_id;
    int field_FC_ring_puller_id;
    enum class State : __int16
    {
        eState_0 = 0,
        eState_1 = 1,
        eState_2 = 2,
        eState_3 = 3,
    };
    State field_100_state;
    __int16 field_102_id;
    SwitchOp field_104_target_action;
    __int16 field_106_on_sound;
    __int16 field_108_off_sound;
    __int16 field_10A_sound_direction;
    int field_10C;
    int field_110_tlvInfo;
};
ALIVE_ASSERT_SIZEOF(PullRingRope, 0x114);
