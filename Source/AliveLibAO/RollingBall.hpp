#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"
#include "../AliveLibAE/Path.hpp"

class PathLine;

namespace AO {

struct Path_RollingBall final : public Path_TLV
{
    Scale_short mScale;
    XDirection_short mRollDirection;
    s16 mReleaseSwitchId;
    u16 mMaxSpeed;
    u16 mAcceleration;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_RollingBall, 0x24);

class RollingBallShaker;

class RollingBall final : public BaseAliveGameObject
{
public:
    RollingBall(Path_RollingBall* pTlv, s32 tlvInfo);
    ~RollingBall();

    virtual void VUpdate() override;

    void Accelerate();
    void CrushThingsInTheWay();

    s32 field_10C_tlvInfo = 0;
    u16 field_110_release_switch_id = 0;
    enum class States : s16
    {
        eInactive_0,
        eStartRolling_1,
        eRolling_2,
        eFallingAndHittingWall_3,
        eCrushedBees_4
    };
    States field_112_state = States::eInactive_0;
    RollingBallShaker* field_114_pRollingBallShaker = nullptr;
    FP mMaxSpeed = {};
    FP field_11C_acceleration = {};
    PathLine* field_120_pCollisionLine = nullptr;
};
ALIVE_ASSERT_SIZEOF(RollingBall, 0x12C);


} // namespace AO
