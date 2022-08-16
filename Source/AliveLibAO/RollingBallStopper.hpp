#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"

class PathLine;

namespace AO {


struct Path_RollingBallStopper final : public Path_TLV
{
    s16 field_18_stopper_switch_id;
    Scale_short field_1A_scale;
    s16 field_1C_ball_switch_id;
    XDirection_short field_1E_direction;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_RollingBallStopper, 0x20);

class RollingBallStopper final : public BaseAliveGameObject
{
public:
    RollingBallStopper(Path_RollingBallStopper* pTlv, s32 tlvInfo);
    ~RollingBallStopper();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    s32 field_10C_tlvInfo;
    s16 field_110_unused;
    enum class States : s16
    {
        eWaitForTrigger_0,
        eMoveStopper_1,
        eMovingDone_2
    };
    States field_112_state = States::eWaitForTrigger_0;
    u16 field_114_release_switch_id = 0;
    u16 field_116_ball_switch_id = 0;
    PathLine* field_118_pLine = nullptr;
};
ALIVE_ASSERT_SIZEOF(RollingBallStopper, 0x11C);

} // namespace AO
