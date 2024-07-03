#pragma once

#include "../relive_lib/GameObjects/BaseAliveGameObject.hpp"

class PathLine;

namespace relive {
struct Path_RollingBall;
}

class RollingBallShaker;

class RollingBall final : public ::BaseAliveGameObject
{
public:
    RollingBall(relive::Path_RollingBall* pTlv, const Guid& tlvId);
    ~RollingBall();

    void LoadAnimations();

    virtual void VUpdate() override;

    void Accelerate();
    void CrushThingsInTheWay();
    bool CollideWithWalls();

    Guid mTlvInfo;
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
    Guid mRollingBallShakerId;
    FP mMaxSpeed = {};
    FP mAcceleration = {};

private:
    void KillRollingBallShaker();
};
