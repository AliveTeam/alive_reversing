#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

enum class LeverState : s16
{
    eWaiting_0 = 0,
    ePulled_1 = 1,
    eFinished_2 = 2
};

class Lever final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    Lever(relive::Path_Lever* pTlv, const Guid& tlvId);
    ~Lever();

    void LoadAnimations();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual s16 VPull(s16 bLeftDirection);

private:
    s16 mSwitchId = 0;
    LeverState mState = LeverState::eWaiting_0;
    Guid mTlvId;
    bool mPulledFromLeft = false;
    bool mPersistOffscreen = false;
    relive::reliveSwitchOp mAction = relive::reliveSwitchOp::eSetTrue;
    relive::Path_Lever::LeverSoundType mOnSound = relive::Path_Lever::LeverSoundType::eNone;
    relive::Path_Lever::LeverSoundType mOffSound = relive::Path_Lever::LeverSoundType::eNone;
    relive::Path_Lever::LeverSoundDirection mSoundDirection = relive::Path_Lever::LeverSoundDirection::eLeftAndRight;
};
