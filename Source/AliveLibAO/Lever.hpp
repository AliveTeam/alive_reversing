#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

namespace AO {

enum class LeverState : s16
{
    eWaiting_0 = 0,
    ePulled_1 = 1,
    eFinished_2 = 2
};

class Lever final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Lever(relive::Path_Lever* pTlv, const Guid& tlvId);
    ~Lever();

    void LoadAnimations();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    // New virtual
    virtual s32 VPull(s16 bLeftDirection);

    s16 mSwitchId = 0;
    LeverState mState = LeverState::eWaiting_0;
    Guid mTlvId;
    bool mPulledFromLeft = false;
    relive::reliveSwitchOp mAction = relive::reliveSwitchOp::eSetTrue;
    relive::Path_Lever::LeverSoundType mOnSound = relive::Path_Lever::LeverSoundType::eNone;
    relive::Path_Lever::LeverSoundType mOffSound = relive::Path_Lever::LeverSoundType::eNone;
    relive::Path_Lever::LeverSoundDirection mSoundDirection = relive::Path_Lever::LeverSoundDirection::eLeftAndRight;
};

} // namespace AO
