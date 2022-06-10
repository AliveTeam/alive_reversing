#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_RollingBall final : public Path_TLV
{
    Scale_short field_18_scale;
    XDirection_short field_1A_roll_direction;
    s16 field_1C_release_switch_id;
    u16 field_1E_speed;
    u16 field_20_acceleration;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_RollingBall, 0x24);

class RollingBallShaker;
class PathLine;

class RollingBall final : public BaseAliveGameObject
{
public:
    RollingBall(Path_RollingBall* pTlv, s32 tlvInfo);
    ~RollingBall();

    virtual void VUpdate() override;

    void Accelerate();
    void CrushThingsInTheWay();

    s32 field_10C_tlvInfo;
    u16 field_110_release_switch_id;
    enum class States : s16
    {
        eInactive_0,
        eStartRolling_1,
        eRolling_2,
        eFallingAndHittingWall_3,
        eCrushedBees_4
    };
    States field_112_state;
    RollingBallShaker* field_114_pRollingBallShaker;
    FP field_118_speed;
    FP field_11C_acceleration;
    PathLine* field_120_pCollisionLine;
};
ALIVE_ASSERT_SIZEOF(RollingBall, 0x12C);


} // namespace AO
