#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "SwitchStates.hpp"
#include "Path.hpp"

namespace AO {

class Rope;

class PullRingRope final : public BaseAnimatedWithPhysicsGameObject
{
public:
    PullRingRope(relive::Path_PullRingRope* pTlv, const Guid& tlvId);
    ~PullRingRope();

    void LoadAnimations();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    bool vIsNotBeingPulled();
    s16 Pull(BaseAliveGameObject* pFrom);

    s32 field_E4_stay_in_state_ticks = 0;
    Guid mTlvInfo;
    enum class States : s16
    {
        eIdle_0 = 0,
        eBeingPulled_1 = 1,
        eTriggerEvent_2 = 2,
        eReturnToIdle_3 = 3
    };
    States mState = States::eIdle_0;
    s16 mSwitchId = 0;
    relive::reliveSwitchOp mAction = relive::reliveSwitchOp::eSetTrue;
    BaseAliveGameObject* field_F4_pPuller = nullptr;
    Rope* field_F8_pRope = nullptr;
    relive::Path_PullRingRope::PullRingSwitchSound field_FC_on_sound = relive::Path_PullRingRope::PullRingSwitchSound::eNone;
    relive::Path_PullRingRope::PullRingSwitchSound field_FE_off_sound = relive::Path_PullRingRope::PullRingSwitchSound::eNone;
    relive::Path_PullRingRope::PullRingSoundDirection field_100_sound_direction = relive::Path_PullRingRope::PullRingSoundDirection::eLeftAndRight;
};
ALIVE_ASSERT_SIZEOF(PullRingRope, 0x104);


} // namespace AO
