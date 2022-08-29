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
    RollingBall(relive::Path_RollingBall* pTlv, const Guid& tlvId);
    ~RollingBall();

    virtual void VUpdate() override;

    void Accelerate();
    void CrushThingsInTheWay();

    Guid field_10C_tlvInfo;
    u16 mReleaseSwitchId = 0;
    enum class States : s16
    {
        eInactive,
        eStartRolling,
        eRolling,
        eFallingAndHittingWall,
        eCrushedBees
    };
    States mState = States::eInactive;
    RollingBallShaker* mRollingBallShaker = nullptr;
    FP mMaxSpeed = {};
    FP mAcceleration = {};
};
ALIVE_ASSERT_SIZEOF(RollingBall, 0x12C);


} // namespace AO
