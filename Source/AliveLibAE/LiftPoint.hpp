#pragma once

#include "PlatformBase.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

struct LiftPointSaveState final
{
    ReliveTypes mType;
    FP mXPos;
    FP mYPos;
    Guid mPlatformId;
    Guid mTlvId;
    FP mFloorLevelY;
    relive::Path_LiftPoint::LiftPointStopType mLiftPointStopType;
    bool mMoving;
    bool mTopFloor;
    bool mMiddleFloor;
    bool mBottomFloor;
    bool mMoveToFloorLevel;
    bool mKeepOnMiddleFloor;
};

class LiftPoint final : public PlatformBase
{
public:
    LiftPoint(relive::Path_LiftPoint* pTlv, const Guid& tlvId);
    ~LiftPoint();
    
    void LoadAnimations();

    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    static s32 CreateFromSaveState(const u8* pData);

    void vKeepOnMiddleFloor();
    bool vOnTopFloor();
    bool vOnMiddleFloor();
    bool vOnBottomFloor();
    bool vOnAnyFloor();
    bool vOnAFloorLiftMoverCanUse();
    bool vMovingToFloorLevel();

    void Move(FP xSpeed, FP ySpeed, s32 not_used);

private:
    void MoveObjectsOnLift(FP xVelocity);
    static void ClearTlvFlags(relive::Path_TLV* pTlv);
    void vStayOnFloor(bool floor, relive::Path_LiftPoint* pTlv);

    void CreatePulleyIfExists();

public:
    u8 mLiftPointId = 0;
    bool mMoving = false;

private:
    relive::Path_LiftPoint::LiftPointStopType mLiftPointStopType = relive::Path_LiftPoint::LiftPointStopType::eTopFloor;
    Guid mRopeId2;
    Guid mRopeId1;
    Animation mLiftWheelAnim = {};
    Animation mPulleyAnim = {};
    s16 mPulleyXPos = 0;
    s16 mPulleyYPos = 0;
    FP mFloorLevelY = {};
    Guid mTlvId;
    bool mTopFloor = false;
    bool mMiddleFloor = false;
    bool mBottomFloor = false;
    bool mHasPulley = false;
    bool mMoveToFloorLevel = false;
    bool mKeepOnMiddleFloor = false;
    bool mIgnoreLiftMover = false;
};
