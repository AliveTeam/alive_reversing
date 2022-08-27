#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "SwitchStates.hpp"

enum class PullRingSwitchSound : s16
{
    eNone_0 = 0,
    eWellExit_1 = 1,
    eRingBellHammer_2 = 2,
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
    s16 mSwitchId;
    SwitchOp mAction;
    s16 mRopeLength;
    Scale_short mScale;
    PullRingSwitchSound mOnSound;
    PullRingSwitchSound mOffSound;
    PullRingSoundDirection mSoundDirection;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_PullRingRope, 0x20);

class PullRingRope final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    PullRingRope(relive::Path_PullRingRope* pTlv, s32 tlvInfo);
    ~PullRingRope();
    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual s16 VPull(BaseGameObject* a2);
    virtual bool VIsNotBeingPulled();
    virtual void VMarkAsPulled();

private:
    s32 field_F4_stay_in_state_ticks = 0;
    s32 field_F8_rope_id = 0;
    s32 field_FC_ring_puller_id = 0;
    enum class States : s16
    {
        eIdle_0 = 0,
        eBeingPulled_1 = 1,
        eTriggerEvent_2 = 2,
        eReturnToIdle_3 = 3,
    };
    States field_100_state = States::eIdle_0;
    s16 field_102_switch_id = 0;
    SwitchOp field_104_action = SwitchOp::eSetTrue_0;
    PullRingSwitchSound field_106_on_sound = PullRingSwitchSound::eNone_0;
    PullRingSwitchSound field_108_off_sound = PullRingSwitchSound::eNone_0;
    PullRingSoundDirection field_10A_sound_direction = PullRingSoundDirection::eLeftAndRight_0;
    s32 field_10C_is_pulled = 0;
    s32 field_110_tlvInfo = 0;
};
ALIVE_ASSERT_SIZEOF(PullRingRope, 0x114);
