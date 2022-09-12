#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "SwitchStates.hpp"

class PullRingRope final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    PullRingRope(relive::Path_PullRingRope* pTlv, const Guid& tlvId);
    ~PullRingRope();
    
    void LoadAnimations();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual s16 VPull(BaseGameObject* a2);
    virtual bool VIsNotBeingPulled();
    virtual void VMarkAsPulled();

private:
    s32 field_F4_stay_in_state_ticks = 0;
    Guid field_F8_rope_id;
    Guid field_FC_ring_puller_id;
    enum class States : s16
    {
        eIdle_0 = 0,
        eBeingPulled_1 = 1,
        eTriggerEvent_2 = 2,
        eReturnToIdle_3 = 3,
    };
    States field_100_state = States::eIdle_0;
    s16 field_102_switch_id = 0;
    relive::reliveSwitchOp field_104_action = relive::reliveSwitchOp::eSetTrue;
    relive::Path_PullRingRope::PullRingSwitchSound field_106_on_sound = relive::Path_PullRingRope::PullRingSwitchSound::eNone;
    relive::Path_PullRingRope::PullRingSwitchSound field_108_off_sound = relive::Path_PullRingRope::PullRingSwitchSound::eNone;
    relive::Path_PullRingRope::PullRingSoundDirection field_10A_sound_direction = relive::Path_PullRingRope::PullRingSoundDirection::eLeftAndRight;
    s32 field_10C_is_pulled = 0;
    Guid field_110_tlvInfo;
};
ALIVE_ASSERT_SIZEOF(PullRingRope, 0x114);
