#pragma once

#include "../relive_lib/GameObjects/BaseAliveGameObject.hpp"

class PathLine;

namespace relive
{
    struct Path_RollingBallStopper;
}

namespace AO {

class RollingBallStopper final : public ::BaseAliveGameObject
{
public:
    RollingBallStopper(relive::Path_RollingBallStopper* pTlv, const Guid& tlvId);
    ~RollingBallStopper();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    Guid mTlvInfo;
    enum class States : s16
    {
        eWaitForTrigger,
        eMoveStopper,
        eMovingDone
    };
    States mState = States::eWaitForTrigger;
    u16 mStopperSwitchId = 0;
    u16 mBallSwitchId = 0;
    PathLine* mCollisionLine = nullptr;
};

} // namespace AO
