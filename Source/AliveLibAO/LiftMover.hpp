#pragma once


#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/FixedPoint.hpp"

namespace relive
{
    struct Path_LiftMover;
}

namespace AO {

class LiftPoint;

enum class LiftMoverStates : s16
{
    eInactive_0 = 0,
    eStartMovingDown_1 = 1,
    eMovingDown_2 = 2,
    eStartMovingUp_3 = 3,
    eMovingUp_4 = 4,
    eMovingDone_5 = 5,
};

class LiftMover final : public ::BaseGameObject
{
public:
    LiftMover(relive::Path_LiftMover* pTlv, const Guid& tlvId);
    ~LiftMover();
    
    virtual void VUpdate() override;

    LiftPoint* FindLiftPointWithId(s16 id);

    u16 mLiftMoverSwitchId = 0;
    s16 mTargetLiftPointId = 0;
    Guid mTlvId;
    Guid mTargetLift = {};
    FP mLiftSpeed = {};
    LiftMoverStates mState = LiftMoverStates::eInactive_0;
};

} // namespace AO
