#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

class PathLine;

struct Path_RollingBallStopper final : public Path_TLV
{
    s16 field_18_stopper_switch_id;
    Scale_short field_1A_scale;
    s16 field_1C_ball_switch_id;
    XDirection_short field_1E_direction;
};
ALIVE_ASSERT_SIZEOF(Path_RollingBallStopper, 0x20);

class RollingBallStopper final : public BaseAliveGameObject
{
public:
    RollingBallStopper(Path_RollingBallStopper* pTlv, s32 tlvInfo);
    ~RollingBallStopper();

    void VScreenChanged_43C130();
    void VUpdate_43BF70();

    
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
    States field_112_state;
    u16 field_114_release_switch_id;
    u16 field_116_ball_switch_id;
    PathLine* field_118_pLine;
};
ALIVE_ASSERT_SIZEOF(RollingBallStopper, 0x11C);

} // namespace AO
