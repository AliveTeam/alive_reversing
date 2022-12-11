#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

class PullRingRope final : public BaseAnimatedWithPhysicsGameObject
{
public:
    PullRingRope(relive::Path_PullRingRope* pTlv, const Guid& tlvId);
    ~PullRingRope();

    void LoadAnimations();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual s16 VPull(BaseGameObject* pFrom);
    virtual bool VIsNotBeingPulled();
    virtual void VMarkAsPulled();

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
    bool mIsPulled = false;
    Guid mTlvId;
};
