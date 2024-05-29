#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

namespace AO {

class Rope;
class ::BaseAliveGameObject;

class PullRingRope final : public BaseAnimatedWithPhysicsGameObject
{
public:
    PullRingRope(relive::Path_PullRingRope* pTlv, const Guid& tlvId);
    ~PullRingRope();

    void LoadAnimations();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    s16 Pull(::BaseAliveGameObject* pFrom);
    bool vIsNotBeingPulled();

private:
    s32 mStayInStateTicks = 0;
    Guid mRopeId;
    Guid mRingPullerId;
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
    relive::Path_PullRingRope::PullRingSwitchSound mOnSound = relive::Path_PullRingRope::PullRingSwitchSound::eNone;
    relive::Path_PullRingRope::PullRingSwitchSound mOffSound = relive::Path_PullRingRope::PullRingSwitchSound::eNone;
    relive::Path_PullRingRope::PullRingSoundDirection mSoundDirection = relive::Path_PullRingRope::PullRingSoundDirection::eLeftAndRight;
    Guid mTlvId;
};


} // namespace AO
