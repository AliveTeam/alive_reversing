#pragma once

#include "../relive_lib/Animation.hpp"
#include "PlatformBase.hpp"
#include "Map.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

namespace AO {

class Rope;

class LiftPoint final : public PlatformBase
{
public:
    LiftPoint(relive::Path_LiftPoint* pTlv, const Guid& tlvId);
    ~LiftPoint();

    void LoadAnimations();
    void Move(FP xSpeed, FP ySpeed);
    void StayOnFloor(s16 floor, relive::Path_LiftPoint* pTlv);
    void ClearTlvFlags(relive::Path_TLV* pTlv);
    bool OnTopFloor() const;
    bool OnBottomFloor() const;
    bool OnMiddleFloor() const;
    bool OnAnyFloor() const;
    bool OnAFloorLiftMoverCanUse() const;

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

    void CreatePulleyIfExists(s16 camX, s16 camY);
    void MoveObjectsOnLift(FP xVelocity);

    bool mMoving = false;
    s16 mLiftPointId = 0;
    bool mKeepOnMiddleFloor = false;
    bool mIgnoreLiftMover = false;
private:

    relive::Path_LiftPoint::LiftPointStopType mLiftPointStopType = relive::Path_LiftPoint::LiftPointStopType::eTopFloor;
    Rope* mRope2 = nullptr;
    Rope* mRope1 = nullptr;
    Animation mLiftWheelAnim;
    Animation mPulleyAnim;
    s16 mPulleyXPos = 0;
    s16 mPulleyYPos = 0;
    FP mFloorLevelY = {};
    bool mTopFloor = false;
    bool mMiddleFloor = false;
    bool mBottomFloor = false;
    bool mHasPulley = false;
    bool mMoveToFloorLevel = false;
};


} // namespace AO
