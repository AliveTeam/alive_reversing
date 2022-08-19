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

    s32 field_10C_tlvInfo = 0;
    enum class States : s16
    {
        eWaitForTrigger_0,
        eMoveStopper_1,
        eMovingDone_2
    };
    States field_112_state = States::eWaitForTrigger_0;
    u16 mStopperSwitchId = 0;
    u16 mBallSwitchId = 0;
    PathLine* field_118_pLine = nullptr;
};
ALIVE_ASSERT_SIZEOF(RollingBallStopper, 0x11C);

} // namespace AO
