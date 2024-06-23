#pragma once

#include "../relive_lib/Animation.hpp"
#include "../relive_lib/GameObjects/PlatformBase.hpp"
#include "Map.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

class Rope;

namespace AO {

class LiftPoint final : public ::PlatformBase
{
public:
    LiftPoint(relive::Path_LiftPoint* pTlv, const Guid& tlvId);
    ~LiftPoint();

    void LoadAnimations();
    
    virtual void VRender(OrderingTable& ot) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    bool OnTopFloor() const;
    bool OnBottomFloor() const;
    bool OnMiddleFloor() const;
    bool OnAnyFloor() const;
    bool OnAFloorLiftMoverCanUse() const;

    void Move(FP xSpeed, FP ySpeed);

private:

    void MoveObjectsOnLift(FP xVelocity);

    void ClearTlvFlags(relive::Path_TLV* pTlv);
    void StayOnFloor(s16 floor, relive::Path_LiftPoint* pTlv);

    void CreatePulleyIfExists(s16 camX, s16 camY);
public:
    s16 mLiftPointId = 0;
    bool mMoving = false;
    bool mKeepOnMiddleFloor = false;
    bool mIgnoreLiftMover = false;
private:

    relive::Path_LiftPoint::LiftPointStopType mLiftPointStopType = relive::Path_LiftPoint::LiftPointStopType::eTopFloor;
    Guid mRope2 = {};
    Guid mRope1 = {};
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
