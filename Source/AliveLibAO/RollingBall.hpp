#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"
#include "../AliveLibAE/Path.hpp"

class PathLine;

namespace AO {

class RollingBallShaker;

class RollingBall final : public BaseAliveGameObject
{
public:
    RollingBall(relive::Path_RollingBall* pTlv, const Guid& tlvId);
    ~RollingBall();

    void LoadAnimations();

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
