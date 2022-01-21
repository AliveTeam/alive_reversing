#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/FunctionFwd.hpp"

enum class SwitchOp : s16;

enum class PullRingSwitchSound : s16
{
    eNone_0 = 0,
    eWellExit_1 = 1,
    eRingUnknownTrigger_2 = 2,
    eDoorEffect_3 = 3,
};

enum class PullRingSoundDirection : s16
{
    eLeftAndRight_0 = 0,
    eLeft_1 = 1,
    eRight_2 = 2,
};

struct Path_PullRingRope final : public Path_TLV
{
    s16 field_10_switch_id;
    SwitchOp field_12_action;
    s16 field_14_rope_length;
    Scale_short field_16_scale;
    PullRingSwitchSound field_18_on_sound;
    PullRingSwitchSound field_1A_off_sound;
    PullRingSoundDirection field_1C_sound_direction;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_PullRingRope, 0x20);

class PullRingRope final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT PullRingRope* ctor_49B2D0(Path_PullRingRope* pTlv, s32 tlvInfo);

    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual s16 VPull_49BBD0(BaseGameObject* a2);
    virtual Bool32 VIsNotBeingPulled_49BC90();
    virtual void VMarkAsPulled_49B610();

private:
    EXPORT PullRingRope* vdtor_49B630(s32 flags);
    EXPORT void dtor_49B660();
    EXPORT void vUpdate_49B720();
    EXPORT void vScreenChanged_49BCB0();
    EXPORT s16 vPull_49BBD0(BaseGameObject* a2);
    EXPORT Bool32 vIsNotBeingPulled_49BC90();
    EXPORT void vMarkAsPulled_49B610();

private:
    s32 field_F4_stay_in_state_ticks;
    s32 field_F8_rope_id;
    s32 field_FC_ring_puller_id;
    enum class States : s16
    {
        eIdle_0 = 0,
        eBeingPulled_1 = 1,
        eTriggerEvent_2 = 2,
        eReturnToIdle_3 = 3,
    };
    States field_100_state;
    s16 field_102_switch_id;
    SwitchOp field_104_action;
    PullRingSwitchSound field_106_on_sound;
    PullRingSwitchSound field_108_off_sound;
    PullRingSoundDirection field_10A_sound_direction;
    s32 field_10C_is_pulled;
    s32 field_110_tlvInfo;
};
ALIVE_ASSERT_SIZEOF(PullRingRope, 0x114);
