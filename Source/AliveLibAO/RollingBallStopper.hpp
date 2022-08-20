#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"

class PathLine;

namespace AO {


struct Path_RollingBallStopper final : public Path_TLV
{
    s16 mStopperSwitchId;
    Scale_short mScale;
    s16 mBallSwitchId;
    XDirection_short mStopDirection;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_RollingBallStopper, 0x20);

class RollingBallStopper final : public BaseAliveGameObject
{
public:
    RollingBallStopper(Path_RollingBallStopper* pTlv, s32 tlvInfo);
    ~RollingBallStopper();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    s32 mTlvInfo = 0;
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
ALIVE_ASSERT_SIZEOF(RollingBallStopper, 0x11C);

} // namespace AO
