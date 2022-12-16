#pragma once

#include "relive_tlvs_serialization.hpp"
#include "PathTlvsAO.hpp"
#include "PathTlvsAE.hpp"
#include "SwitchStates.hpp"
#include "FatalError.hpp"
#include "../AliveLibAO/Path.hpp"
#include "../AliveLibAE/Path.hpp"

// Convert an AO or AE TLV to a relive TLV

namespace relive {
inline void BaseConvert(relive::Path_TLV& r, const AO::Path_TLV& base, const Guid& tlvId)
    {
        r.mId = tlvId;
        r.mTopLeftX = base.mTopLeft.x;
        r.mTopLeftY = base.mTopLeft.y;
        r.mBottomRightX = base.mBottomRight.x;
        r.mBottomRightY = base.mBottomRight.y;
        r.mTlvSpecificMeaning = base.mTlvSpecificMeaning;
        r.mTlvFlags.Raw().all = base.mTlvFlags.Raw().all;
    }

    inline void BaseConvert(relive::Path_TLV& r, const ::Path_TLV& base, const Guid& tlvId)
    {
        r.mId = tlvId;
        r.mTopLeftX = base.mTopLeft.x;
        r.mTopLeftY = base.mTopLeft.y;
        r.mBottomRightX = base.mBottomRight.x;
        r.mBottomRightY = base.mBottomRight.y;
        r.mTlvSpecificMeaning = base.mTlvSpecificMeaning;
        r.mTlvFlags.Raw().all = base.mTlvFlags.Raw().all;
    }

    // also used for AO
    static reliveScale From(const ::Scale_short scale)
    {
        switch (scale)
        {
            case Scale_short::eFull_0:
                return reliveScale::eFull;
            case Scale_short::eHalf_1:
                return reliveScale::eHalf;
        }
        ALIVE_FATAL("Bad scale");
    }

    static reliveScale From(const ::Scale_int scale)
    {
        switch (scale)
        {
        case Scale_int::eFull_0:
            return reliveScale::eFull;
        case Scale_int::eHalf_1:
            return reliveScale::eHalf;
        }
        ALIVE_FATAL("Bad scale");
    }

    static reliveChoice From(const ::Choice_short choice)
    {
        switch (choice)
        {
        case Choice_short::eNo_0:
            return reliveChoice::eNo;
        case Choice_short::eYes_1:
            return reliveChoice::eYes;
        }
        ALIVE_FATAL("Bad choice");
    }

    static reliveChoice From(const ::Choice_int choice)
    {
        switch (choice)
        {
        case Choice_int::eNo_0:
            return reliveChoice::eNo;
        case Choice_int::eYes_1:
            return reliveChoice::eYes;
        }
        ALIVE_FATAL("Bad choice");
    }

    static reliveXDirection From(const ::XDirection_short xdirection)
    {
        switch (xdirection)
        {
        case XDirection_short::eLeft_0:
            return reliveXDirection::eLeft;
        case XDirection_short::eRight_1:
            return reliveXDirection::eRight;
        }
        ALIVE_FATAL("Bad x direction");
    }

    static reliveXDirection From(const ::XDirection_int xdirection)
    {
        switch (xdirection)
        {
        case XDirection_int::eLeft_0:
            return reliveXDirection::eLeft;
        case XDirection_int::eRight_1:
            return reliveXDirection::eRight;
        }
        ALIVE_FATAL("Bad x direction");
    }

    static reliveSwitchOp From(const SwitchOp op)
    {
        switch (op)
        {
            case SwitchOp::eSetTrue_0:
                return reliveSwitchOp::eSetTrue;
            case SwitchOp::eSetFalse_1:
                return reliveSwitchOp::eSetFalse;
            case SwitchOp::eToggle_2:
                return reliveSwitchOp::eToggle;
            case SwitchOp::eIncrement_3:
                return reliveSwitchOp::eIncrement;
            case SwitchOp::eDecrement_4:
                return reliveSwitchOp::eDecrement;
        }
        ALIVE_FATAL("Bad switch operator");
    }

    static reliveScreenChangeEffects From(::ScreenChangeEffects effect)
    {
        switch (effect)
        {
            case ::ScreenChangeEffects::ePlay1FMV_0:
                return reliveScreenChangeEffects::ePlay1FMV;
            case ::ScreenChangeEffects::eRightToLeft_1:
                return reliveScreenChangeEffects::eRightToLeft;
            case ::ScreenChangeEffects::eLeftToRight_2:
                return reliveScreenChangeEffects::eLeftToRight;
            case ::ScreenChangeEffects::eBottomToTop_3:
                return reliveScreenChangeEffects::eBottomToTop;
            case ::ScreenChangeEffects::eTopToBottom_4:
                return reliveScreenChangeEffects::eTopToBottom;
            case ::ScreenChangeEffects::eBoxOut_5:
                return reliveScreenChangeEffects::eBoxOut;
            case ::ScreenChangeEffects::eVerticalSplit_6:
                return reliveScreenChangeEffects::eVerticalSplit;
            case ::ScreenChangeEffects::eHorizontalSplit_7:
                return reliveScreenChangeEffects::eHorizontalSplit;
            case ::ScreenChangeEffects::eUnknown_8:
                return reliveScreenChangeEffects::eUnknown_8;
            case ::ScreenChangeEffects::eInstantChange_9:
                return reliveScreenChangeEffects::eInstantChange;
        }
        ALIVE_FATAL("Bad screen change effect");
    }

class Path_ShadowZone_Converter final
{
public:
    static Path_ShadowZone From(const AO::Path_ShadowZone& tlv, const Guid& tlvId)
    {
        Path_ShadowZone r;
        BaseConvert(r, tlv, tlvId);
        r.mRGB.SetRGB(tlv.mRed, tlv.mGreen, tlv.mBlue);
        r.mScale = From(tlv.mScale);
        return r;
    }

    static Path_ShadowZone From(const ::Path_ShadowZone& tlv, const Guid& tlvId)
    {
        Path_ShadowZone r;
        BaseConvert(r, tlv, tlvId);
        r.mRGB.SetRGB(tlv.mRed, tlv.mGreen, tlv.mBlue);
        r.mScale = From(tlv.mScale);
        return r;
    }

private:
    static Path_ShadowZone::Scale From(::Path_ShadowZone::ShadowZoneScale aeScale)
    {
        switch (aeScale)
        {
            case ::Path_ShadowZone::ShadowZoneScale::eHalf_1:
                return Path_ShadowZone::Scale::eHalf;
            case ::Path_ShadowZone::ShadowZoneScale::eFull_2:
                return Path_ShadowZone::Scale::eFull;
            case ::Path_ShadowZone::ShadowZoneScale::eBoth_0:
                return Path_ShadowZone::Scale::eBoth;
        }
        ALIVE_FATAL("Bad AE shadow scale");
    }

    static Path_ShadowZone::Scale From(AO::Path_ShadowZone::ShadowZoneScale aoScale)
    {
        switch (aoScale)
        {
            case AO::Path_ShadowZone::ShadowZoneScale::eHalf_1:
                return Path_ShadowZone::Scale::eHalf;
            case AO::Path_ShadowZone::ShadowZoneScale::eFull_2:
                return Path_ShadowZone::Scale::eFull;
            case AO::Path_ShadowZone::ShadowZoneScale::eBoth_0:
                return Path_ShadowZone::Scale::eBoth;
        }
        ALIVE_FATAL("Bad AO shadow scale");
    }
};

class Path_SecurityOrb_Converter final
{
public:
    static Path_SecurityOrb From(const AO::Path_SecurityOrb& tlv, const Guid& tlvId)
    {
        Path_SecurityOrb r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mDisabledResources = tlv.mDisabledResources;
        return r;
    }

    static Path_SecurityOrb From(const ::Path_SecurityOrb& tlv, const Guid& tlvId)
    {
        Path_SecurityOrb r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mDisabledResources = tlv.mDisabledResources;
        return r;
    }
};

class Path_ContinuePoint_Converter final
{
public:
    static Path_ContinuePoint From(const AO::Path_ContinuePoint& tlv, const Guid& tlvId)
    {
        Path_ContinuePoint r;
        BaseConvert(r, tlv, tlvId);
        r.mZoneNumber = tlv.mZoneNumber;
        r.mClearFromId = tlv.mClearFromId;
        r.mClearToId = tlv.mClearToId;
        r.mElumRestarts = relive::From(tlv.mElumRestarts);
        r.mAbeSpawnDir = From(tlv.mAbeSpawnDir);
        return r;
    }

    static Path_ContinuePoint From(const ::Path_ContinuePoint& tlv, const Guid& tlvId)
    {
        Path_ContinuePoint r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = From(tlv.mScale);
        r.mSaveFileId = tlv.mSaveFileId;
        return r;
    }

private:
    static relive::Path_ContinuePoint::spawnDirection From(AO::Path_ContinuePoint::spawnDirection spawnDirection)
    {
        switch (spawnDirection)
        {
            case AO::Path_ContinuePoint::spawnDirection::eRight_0:
                return relive::Path_ContinuePoint::spawnDirection::eRight;
            case AO::Path_ContinuePoint::spawnDirection::eLeft_1:
                return relive::Path_ContinuePoint::spawnDirection::eLeft;
        }
        ALIVE_FATAL("Bad spawn direction");
    }

    static relive::Path_ContinuePoint::Scale From(::Path_ContinuePoint::Scale scale)
    {
        switch (scale)
        {
            case ::Path_ContinuePoint::Scale::eNone_0:
                return relive::Path_ContinuePoint::Scale::eNone;
            case ::Path_ContinuePoint::Scale::eHalf_1:
                return relive::Path_ContinuePoint::Scale::eHalf;
            case ::Path_ContinuePoint::Scale::eFull_2:
                return relive::Path_ContinuePoint::Scale::eFull;
        }
        ALIVE_FATAL("Bad scale");
    }
};

class Path_LiftPoint_Converter final
{
public:
    static Path_LiftPoint From(const AO::Path_LiftPoint& tlv, const Guid& tlvId)
    {
        Path_LiftPoint r;
        BaseConvert(r, tlv, tlvId);
        r.mLiftPointId = tlv.mLiftPointId;
        r.mIsStartPoint = relive::From(tlv.mIsStartPoint);
        r.mLiftPointStopType = From(tlv.mLiftPointStopType);
        r.mScale = relive::From(tlv.mScale);
        r.mIgnoreLiftMover = relive::From(tlv.mIgnoreLiftMover);
        return r;
    }

    static Path_LiftPoint From(const ::Path_LiftPoint& tlv, const Guid& tlvId)
    {
        Path_LiftPoint r;
        BaseConvert(r, tlv, tlvId);
        r.mLiftPointId = tlv.mLiftPointId;
        r.mIsStartPoint = relive::From(tlv.mIsStartPoint);
        r.mLiftPointStopType = From(tlv.mLiftPointStopType);
        r.mScale = relive::From(tlv.mScale);
        r.mIgnoreLiftMover = relive::From(tlv.mIgnoreLiftMover);
        return r;
    }

private:
    static relive::Path_LiftPoint::LiftPointStopType From(AO::Path_LiftPoint::LiftPointStopType stopType)
    {
        switch (stopType)
        {
            case AO::Path_LiftPoint::LiftPointStopType::eTopFloor_0:
                return relive::Path_LiftPoint::LiftPointStopType::eTopFloor;
            case AO::Path_LiftPoint::LiftPointStopType::eBottomFloor_1:
                return relive::Path_LiftPoint::LiftPointStopType::eBottomFloor;
            case AO::Path_LiftPoint::LiftPointStopType::eMiddleFloor_2:
                return relive::Path_LiftPoint::LiftPointStopType::eMiddleFloor;
            case AO::Path_LiftPoint::LiftPointStopType::eMiddleLockFloor_3:
                return relive::Path_LiftPoint::LiftPointStopType::eMiddleLockFloor;
            case AO::Path_LiftPoint::LiftPointStopType::eStartPointOnly_4:
                return relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly;
        }
        ALIVE_FATAL("Bad lift point stop type");
    }

    static relive::Path_LiftPoint::LiftPointStopType From(::Path_LiftPoint::LiftPointStopType stopType)
    {
        switch (stopType)
        {
            case ::Path_LiftPoint::LiftPointStopType::eTopFloor_0:
                return relive::Path_LiftPoint::LiftPointStopType::eTopFloor;
            case ::Path_LiftPoint::LiftPointStopType::eBottomFloor_1:
                return relive::Path_LiftPoint::LiftPointStopType::eBottomFloor;
            case ::Path_LiftPoint::LiftPointStopType::eMiddleFloor_2:
                return relive::Path_LiftPoint::LiftPointStopType::eMiddleFloor;
            case ::Path_LiftPoint::LiftPointStopType::eMiddleLockFloor_3:
                return relive::Path_LiftPoint::LiftPointStopType::eMiddleLockFloor;
            case ::Path_LiftPoint::LiftPointStopType::eStartPointOnly_4:
                return relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly;
        }
        ALIVE_FATAL("Bad lift point stop type");
    }
};

class Path_Dove_Converter final
{
public:
    static Path_Dove From(const AO::Path_Dove& tlv, const Guid& tlvId)
    {
        Path_Dove r;
        BaseConvert(r, tlv, tlvId);
        r.mDoveCount = tlv.mDoveCount;
        r.mPixelPerfect = relive::From(tlv.mPixelPerfect);
        r.mScale = relive::From(tlv.mScale);
        return r;
    }

    static Path_Dove From(const ::Path_Dove& tlv, const Guid& tlvId)
    {
        Path_Dove r;
        BaseConvert(r, tlv, tlvId);
        r.mDoveCount = tlv.mDoveCount;
        r.mPixelPerfect = relive::From(tlv.mPixelPerfect);
        r.mScale = relive::From(tlv.mScale);
        return r;
    }
};

class Path_RockSack_Converter final
{
public:
    static Path_RockSack From(const AO::Path_RockSack& tlv, const Guid& tlvId)
    {
        Path_RockSack r;
        BaseConvert(r, tlv, tlvId);
        r.mRockFallDirection = relive::From(tlv.mRockFallDirection);
        r.mVelX = tlv.mVelX;
        r.mVelY = tlv.mVelY;
        r.mScale = relive::From(tlv.mScale);
        r.mRockAmount = tlv.mRockAmount;
        return r;
    }

    static Path_RockSack From(const ::Path_RockSack& tlv, const Guid& tlvId)
    {
        Path_RockSack r;
        BaseConvert(r, tlv, tlvId);
        r.mRockFallDirection = relive::From(tlv.mRockFallDirection);
        r.mVelX = tlv.mVelX;
        r.mVelY = tlv.mVelY;
        r.mScale = relive::From(tlv.mScale);
        r.mRockAmount = tlv.mRockAmount;
        return r;
    }
};

class Path_ZBall_Converter final
{
public:
    static Path_ZBall From(const AO::Path_ZBall& tlv, const Guid& tlvId)
    {
        Path_ZBall r;
        BaseConvert(r, tlv, tlvId);
        r.mStartPos = From(tlv.mStartPos);
        r.mScale = relive::From(tlv.mScale);
        r.mSpeed = From(tlv.mSpeed);
        return r;
    }

private:
    static relive::Path_ZBall::StartPos From(AO::Path_ZBall::StartPos startPos)
    {
        switch (startPos)
        {
            case AO::Path_ZBall::StartPos::eCenter_0:
                return relive::Path_ZBall::StartPos::eCenter;
            case AO::Path_ZBall::StartPos::eOut_1:
                return relive::Path_ZBall::StartPos::eOut;
            case AO::Path_ZBall::StartPos::eIn_2:
                return relive::Path_ZBall::StartPos::eIn;
        }
        ALIVE_FATAL("Bad start pos");
    }

    static relive::Path_ZBall::Speed From(AO::Path_ZBall::Speed speed)
    {
        switch (speed)
        {
            case AO::Path_ZBall::Speed::eNormal_0:
                return relive::Path_ZBall::Speed::eNormal;
            case AO::Path_ZBall::Speed::eFast_1:
                return relive::Path_ZBall::Speed::eFast;
            case AO::Path_ZBall::Speed::eSlow_2:
                return relive::Path_ZBall::Speed::eSlow;
        }
        ALIVE_FATAL("Bad speed");
    }
};

class Path_FallingItem_Converter final
{
public:
    static Path_FallingItem From(const AO::Path_FallingItem& tlv, const Guid& tlvId)
    {
        Path_FallingItem r;
        BaseConvert(r, tlv, tlvId);
        r.mSwitchId = tlv.mSwitchId;
        r.mScale = relive::From(tlv.mScale);
        r.mFallInterval = tlv.mFallInterval;
        r.mMaxFallingItems = tlv.mMaxFallingItems;
        r.mResetSwitchIdAfterUse = relive::From(tlv.mResetSwitchIdAfterUse);
        return r;
    }

    static Path_FallingItem From(const ::Path_FallingItem& tlv, const Guid& tlvId)
    {
        Path_FallingItem r;
        BaseConvert(r, tlv, tlvId);
        r.mSwitchId = tlv.mSwitchId;
        r.mScale = relive::From(tlv.mScale);
        r.mFallInterval = tlv.mFallInterval;
        r.mMaxFallingItems = tlv.mMaxFallingItems;
        r.mResetSwitchIdAfterUse = relive::From(tlv.mResetSwitchIdAfterUse);
        return r;
    }
};

class Path_PullRingRope_Converter final
{
public:
    static Path_PullRingRope From(const AO::Path_PullRingRope& tlv, const Guid& tlvId)
    {
        Path_PullRingRope r;
        BaseConvert(r, tlv, tlvId);
        r.mSwitchId = tlv.mSwitchId;
        r.mAction = relive::From(tlv.mAction);
        r.mRopeLength = tlv.mRopeLength;
        r.mScale = relive::From(tlv.mScale);
        r.mOnSound = From(tlv.mOnSound);
        r.mOffSound = From(tlv.mOffSound);
        r.mSoundDirection = From(tlv.mSoundDirection);
        return r;
    }

    static Path_PullRingRope From(const ::Path_PullRingRope& tlv, const Guid& tlvId)
    {
        Path_PullRingRope r;
        BaseConvert(r, tlv, tlvId);
        r.mSwitchId = tlv.mSwitchId;
        r.mAction = relive::From(tlv.mAction);
        r.mRopeLength = tlv.mRopeLength;
        r.mScale = relive::From(tlv.mScale);
        r.mOnSound = From(tlv.mOnSound);
        r.mOffSound = From(tlv.mOffSound);
        r.mSoundDirection = From(tlv.mSoundDirection);
        return r;
    }

private:
    static relive::Path_PullRingRope::PullRingSwitchSound From(AO::Path_PullRingRope::PullRingSwitchSound switchSound)
    {
        switch (switchSound)
        {
            case AO::Path_PullRingRope::PullRingSwitchSound::eNone_0:
                return relive::Path_PullRingRope::PullRingSwitchSound::eNone;
            case AO::Path_PullRingRope::PullRingSwitchSound::eWellExit_1:
                return relive::Path_PullRingRope::PullRingSwitchSound::eWellExit;
            case AO::Path_PullRingRope::PullRingSwitchSound::RingBellHammer_2:
                return relive::Path_PullRingRope::PullRingSwitchSound::eRingBellHammer;
            case AO::Path_PullRingRope::PullRingSwitchSound::eDoorEffect_3:
                return relive::Path_PullRingRope::PullRingSwitchSound::eDoorEffect;
        }
        ALIVE_FATAL("Bad pull ring switch sound");
    }

    static relive::Path_PullRingRope::PullRingSoundDirection From(AO::Path_PullRingRope::PullRingSoundDirection soundDir)
    {
        switch (soundDir)
        {
            case AO::Path_PullRingRope::PullRingSoundDirection::eLeftAndRight_0:
                return relive::Path_PullRingRope::PullRingSoundDirection::eLeftAndRight;
            case AO::Path_PullRingRope::PullRingSoundDirection::eLeft_1:
                return relive::Path_PullRingRope::PullRingSoundDirection::eLeft;
            case AO::Path_PullRingRope::PullRingSoundDirection::eRight_2:
                return relive::Path_PullRingRope::PullRingSoundDirection::eRight;
        }
        ALIVE_FATAL("Bad pull ring sound direction");
    }

    static relive::Path_PullRingRope::PullRingSwitchSound From(::Path_PullRingRope::PullRingSwitchSound switchSound)
    {
        switch (switchSound)
        {
        case ::Path_PullRingRope::PullRingSwitchSound::eNone_0:
            return relive::Path_PullRingRope::PullRingSwitchSound::eNone;
        case ::Path_PullRingRope::PullRingSwitchSound::eWellExit_1:
            return relive::Path_PullRingRope::PullRingSwitchSound::eWellExit;
        case ::Path_PullRingRope::PullRingSwitchSound::eRingBellHammer_2:
            return relive::Path_PullRingRope::PullRingSwitchSound::eRingBellHammer;
        case ::Path_PullRingRope::PullRingSwitchSound::eDoorEffect_3:
            return relive::Path_PullRingRope::PullRingSwitchSound::eDoorEffect;
        }
        ALIVE_FATAL("Bad pull ring switch sound");
    }

    static relive::Path_PullRingRope::PullRingSoundDirection From(::Path_PullRingRope::PullRingSoundDirection soundDir)
    {
        switch (soundDir)
        {
        case ::Path_PullRingRope::PullRingSoundDirection::eLeftAndRight_0:
            return relive::Path_PullRingRope::PullRingSoundDirection::eLeftAndRight;
        case ::Path_PullRingRope::PullRingSoundDirection::eLeft_1:
            return relive::Path_PullRingRope::PullRingSoundDirection::eLeft;
        case ::Path_PullRingRope::PullRingSoundDirection::eRight_2:
            return relive::Path_PullRingRope::PullRingSoundDirection::eRight;
        }
        ALIVE_FATAL("Bad pull ring sound direction");
    }
};

class Path_TimedMine_Converter final
{
public:
    static Path_TimedMine From(const AO::Path_TimedMine& tlv, const Guid& tlvId)
    {
        Path_TimedMine r;
        BaseConvert(r, tlv, tlvId);
        r.mSwitchId = tlv.mSwitchId;
        r.mState = tlv.mState;
        r.mScale = relive::From(tlv.mScale);
        r.mTicksUntilExplosion = tlv.mTicksUntilExplosion;
        r.mDisabledResources = tlv.mDisabledResources;
        return r;
    }

    static Path_TimedMine From(const ::Path_TimedMine& tlv, const Guid& tlvId)
    {
        Path_TimedMine r;
        BaseConvert(r, tlv, tlvId);
        r.mSwitchId = tlv.mSwitchId;
        r.mState = tlv.mState;
        r.mScale = relive::From(tlv.mScale);
        r.mTicksUntilExplosion = tlv.mTicksUntilExplosion;
        r.mDisabledResources = tlv.mDisabledResources;
        return r;
    }
};

class Path_Hoist_Converter final
{
public:
    static Path_Hoist From(const AO::Path_Hoist& tlv, const Guid& tlvId)
    {
        Path_Hoist r;
        BaseConvert(r, tlv, tlvId);
        r.mHoistType = From(tlv.mHoistType);
        r.mGrabDirection = From(tlv.mGrabDirection);
        return r;
    }

    static Path_Hoist From(const ::Path_Hoist& tlv, const Guid& tlvId)
    {
        Path_Hoist r;
        BaseConvert(r, tlv, tlvId);
        r.mHoistType = From(tlv.mHoistType);
        r.mGrabDirection = From(tlv.mGrabDirection);
        r.mScale = relive::From(tlv.field_16_scale);
        return r;
    }

private:
    static relive::Path_Hoist::Type From(AO::Path_Hoist::Type type)
    {
        switch (type)
        {
        case AO::Path_Hoist::Type::eNextFloor:
            return relive::Path_Hoist::Type::eNextFloor;
        case AO::Path_Hoist::Type::eNextEdge:
            return relive::Path_Hoist::Type::eNextEdge;
        case AO::Path_Hoist::Type::eOffScreen:
            return relive::Path_Hoist::Type::eOffScreen;
        }
        ALIVE_FATAL("Bad hoist type");
    }

    static relive::Path_Hoist::Type From(::Path_Hoist::Type type)
    {
        switch (type)
        {
        case ::Path_Hoist::Type::eNextFloor:
            return relive::Path_Hoist::Type::eNextFloor;
        case ::Path_Hoist::Type::eNextEdge:
            return relive::Path_Hoist::Type::eNextEdge;
        case ::Path_Hoist::Type::eOffScreen:
            return relive::Path_Hoist::Type::eOffScreen;
        }
        ALIVE_FATAL("Bad hoist type");
    }

    static relive::Path_Hoist::GrabDirection From(AO::Path_Hoist::GrabDirection grabDir)
    {
        switch (grabDir)
        {
        case AO::Path_Hoist::GrabDirection::eFacingLeft:
            return relive::Path_Hoist::GrabDirection::eFacingLeft;
        case AO::Path_Hoist::GrabDirection::eFacingRight:
            return relive::Path_Hoist::GrabDirection::eFacingRight;
        case AO::Path_Hoist::GrabDirection::eFacingAnyDirection:
            return relive::Path_Hoist::GrabDirection::eFacingAnyDirection;
        }
        ALIVE_FATAL("Bad hoist grab direction");
    }

    static relive::Path_Hoist::GrabDirection From(::Path_Hoist::GrabDirection grabDir)
    {
        switch (grabDir)
        {
        case ::Path_Hoist::GrabDirection::eFacingLeft:
            return relive::Path_Hoist::GrabDirection::eFacingLeft;
        case ::Path_Hoist::GrabDirection::eFacingRight:
            return relive::Path_Hoist::GrabDirection::eFacingRight;
        case ::Path_Hoist::GrabDirection::eFacingAnyDirection:
            return relive::Path_Hoist::GrabDirection::eFacingAnyDirection;
        }
        ALIVE_FATAL("Bad hoist grab direction");
    }
};

class Path_TrapDoor_Converter final
{
public:
    static Path_TrapDoor From(const AO::Path_TrapDoor& tlv, const Guid& tlvId)
    {
        Path_TrapDoor r;
        BaseConvert(r, tlv, tlvId);
        r.mSwitchId = tlv.mSwitchId;
        r.mStartState = From(tlv.mStartState);
        r.mSelfClosing = relive::From(tlv.mSelfClosing);
        r.mScale = relive::From(tlv.mScale);
        r.mDirection = relive::From(tlv.mDirection);
        r.mXOff = tlv.mXOff;
        return r;
    }

    static Path_TrapDoor From(const ::Path_TrapDoor& tlv, const Guid& tlvId)
    {
        Path_TrapDoor r;
        BaseConvert(r, tlv, tlvId);
        r.mSwitchId = tlv.mSwitchId;
        r.mStartState = From(tlv.mStartState);
        r.mSelfClosing = relive::From(tlv.mSelfClosing);
        r.mScale = relive::From(tlv.mScale);
        r.mDirection = relive::From(tlv.mDirection);
        r.mXOff = tlv.mXOff;
        r.mStayOpenTime = tlv.mStayOpenTime;
        return r;
    }

private:
    static relive::Path_TrapDoor::StartState From(AO::Path_TrapDoor::StartState startState)
    {
        switch (startState)
        {
            case AO::Path_TrapDoor::StartState::eOpen_0:
                return relive::Path_TrapDoor::StartState::eOpen;
            case AO::Path_TrapDoor::StartState::eClosed_1:
                return relive::Path_TrapDoor::StartState::eClosed;
        }
        ALIVE_FATAL("Bad trap door start state");
    }

    static relive::Path_TrapDoor::StartState From(::Path_TrapDoor::StartState startState)
    {
        switch (startState)
        {
        case ::Path_TrapDoor::StartState::eOpen_0:
            return relive::Path_TrapDoor::StartState::eOpen;
        case ::Path_TrapDoor::StartState::eClosed_1:
            return relive::Path_TrapDoor::StartState::eClosed;
        }
        ALIVE_FATAL("Bad trap door start state");
    }
};

class Path_LCDScreen_Converter final
{
public:
    static Path_LCDScreen From(const AO::Path_LCDScreen& tlv, const Guid& tlvId)
    {
        Path_LCDScreen r;
        BaseConvert(r, tlv, tlvId);
        r.mMessageId1 = tlv.mMessageId1;
        r.mMessageRandMinId = tlv.mMessageRandMinId;
        r.mMessageRandMaxId = tlv.mMessageRandMaxId;
        return r;
    }

    static Path_LCDScreen From(const ::Path_LCDScreen& tlv, const Guid& tlvId)
    {
        Path_LCDScreen r;
        BaseConvert(r, tlv, tlvId);
        r.mMessageId1 = tlv.mMessageId1;
        r.mMessageRandMinId = tlv.mMessageRandMinId;
        r.mMessageRandMaxId = tlv.mMessageRandMaxId;
        r.mMessageId2 = tlv.mMessageId2;
        r.mToggleMessageSwitchId = tlv.mToggleMessageSwitchId;
        return r;
    }
};

class Path_Mine_Converter final
{
public:
    static Path_Mine From(const AO::Path_Mine& tlv, const Guid& tlvId)
    {
        Path_Mine r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mPersistOffscreen = relive::From(tlv.mPersistOffscreen);
        return r;
    }

    static Path_Mine From(const ::Path_Mine& tlv, const Guid& tlvId)
    {
        Path_Mine r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mPersistOffscreen = relive::From(tlv.mPersistOffscreen);
        return r;
    }
};

class Path_InvisibleSwitch_Converter final
{
public:
    static Path_InvisibleSwitch From(const AO::Path_InvisibleSwitch& tlv, const Guid& tlvId)
    {
        Path_InvisibleSwitch r;
        BaseConvert(r, tlv, tlvId);
        r.mSwitchId = tlv.mSwitchId;
        r.mAction = relive::From(tlv.mAction);
        r.mActivationDelay = tlv.mActivationDelay;
        r.mSetOffAlarm = relive::From(tlv.mSetOffAlarm);
        r.mScale = From(tlv.mScale);
        return r;
    }

    static Path_InvisibleSwitch From(const ::Path_InvisibleSwitch& tlv, const Guid& tlvId)
    {
        Path_InvisibleSwitch r;
        BaseConvert(r, tlv, tlvId);
        r.mSwitchId = tlv.mSwitchId;
        r.mAction = relive::From(tlv.mAction);
        r.mActivationDelay = tlv.mActivationDelay;
        r.mSetOffAlarm = relive::From(tlv.mSetOffAlarm);
        r.mScale = From(tlv.mScale);
        return r;
    }

private:
    static relive::Path_InvisibleSwitch::InvisibleSwitchScale From(AO::Path_InvisibleSwitch::InvisibleSwitchScale scale)
    {
        switch (scale)
        {
            case AO::Path_InvisibleSwitch::InvisibleSwitchScale::eHalf_0:
                return relive::Path_InvisibleSwitch::InvisibleSwitchScale::eHalf;
            case AO::Path_InvisibleSwitch::InvisibleSwitchScale::eFull_1:
                return relive::Path_InvisibleSwitch::InvisibleSwitchScale::eFull;
            case AO::Path_InvisibleSwitch::InvisibleSwitchScale::eAny_2:
                return relive::Path_InvisibleSwitch::InvisibleSwitchScale::eAny;
        }
        ALIVE_FATAL("Bad invisible switch scale");
    }

    static relive::Path_InvisibleSwitch::InvisibleSwitchScale From(::Path_InvisibleSwitch::InvisibleSwitchScale scale)
    {
        switch (scale)
        {
            case ::Path_InvisibleSwitch::InvisibleSwitchScale::eHalf_0:
                return relive::Path_InvisibleSwitch::InvisibleSwitchScale::eHalf;
            case ::Path_InvisibleSwitch::InvisibleSwitchScale::eFull_1:
                return relive::Path_InvisibleSwitch::InvisibleSwitchScale::eFull;
            case ::Path_InvisibleSwitch::InvisibleSwitchScale::eAny_2:
                return relive::Path_InvisibleSwitch::InvisibleSwitchScale::eAny;
        }
        ALIVE_FATAL("Bad invisible switch scale");
    }
};

class Path_ElectricWall_Converter final
{
public:
    static Path_ElectricWall From(const AO::Path_ElectricWall& tlv, const Guid& tlvId)
    {
        Path_ElectricWall r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mSwitchId = tlv.mSwitchId;
        r.mStartState = From(tlv.mStartState);
        return r;
    }

    static Path_ElectricWall From(const ::Path_ElectricWall& tlv, const Guid& tlvId)
    {
        Path_ElectricWall r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mSwitchId = tlv.mSwitchId;
        r.mStartState = From(tlv.mStartState);
        return r;
    }

private:
    static relive::Path_ElectricWall::ElectricWallStartState From(AO::Path_ElectricWall::ElectricWallStartState startState)
    {
        switch (startState)
        {
            case AO::Path_ElectricWall::ElectricWallStartState::eOff_0:
                return relive::Path_ElectricWall::ElectricWallStartState::eOff;
            case AO::Path_ElectricWall::ElectricWallStartState::eOn_1:
                return relive::Path_ElectricWall::ElectricWallStartState::eOn;
        }
        ALIVE_FATAL("Bad electric wall start state");
    }

    static relive::Path_ElectricWall::ElectricWallStartState From(::Path_ElectricWall::ElectricWallStartState startState)
    {
        switch (startState)
        {
            case ::Path_ElectricWall::ElectricWallStartState::eOff_0:
                return relive::Path_ElectricWall::ElectricWallStartState::eOff;
            case ::Path_ElectricWall::ElectricWallStartState::eOn_1:
                return relive::Path_ElectricWall::ElectricWallStartState::eOn;
        }
        ALIVE_FATAL("Bad electric wall start state");
    }
};

class Path_BoomMachine_Converter final
{
public:
    static Path_BoomMachine From(const AO::Path_BoomMachine& tlv, const Guid& tlvId)
    {
        Path_BoomMachine r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mNozzleSide = From(tlv.mNozzleSide);
        r.mDisabledResources = tlv.mDisabledResources;
        r.mGrenadeAmount = tlv.mGrenadeAmount;
        return r;
    }

    static Path_BoomMachine From(const ::Path_BoomMachine& tlv, const Guid& tlvId)
    {
        Path_BoomMachine r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mNozzleSide = From(tlv.mNozzleSide);
        r.mDisabledResources = tlv.mDisabledResources;
        r.mGrenadeAmount = tlv.mGrenadeAmount;
        return r;
    }

private:
    static relive::Path_BoomMachine::NozzleSide From(AO::Path_BoomMachine::NozzleSide nozzleSide)
    {
        switch (nozzleSide)
        {
            case AO::Path_BoomMachine::NozzleSide::eRight_0:
                return relive::Path_BoomMachine::NozzleSide::eRight;
            case AO::Path_BoomMachine::NozzleSide::eLeft_1:
                return relive::Path_BoomMachine::NozzleSide::eLeft;
        }
        ALIVE_FATAL("Bad boom machine nozzle side");
    }

    static relive::Path_BoomMachine::NozzleSide From(::Path_BoomMachine::NozzleSide nozzleSide)
    {
        switch (nozzleSide)
        {
            case ::Path_BoomMachine::NozzleSide::eRight_0:
                return relive::Path_BoomMachine::NozzleSide::eRight;
            case ::Path_BoomMachine::NozzleSide::eLeft_1:
                return relive::Path_BoomMachine::NozzleSide::eLeft;
        }
        ALIVE_FATAL("Bad boom machine nozzle side");
    }
};

class Path_UXB_Converter final
{
public:
    static Path_UXB From(const AO::Path_UXB& tlv, const Guid& tlvId)
    {
        Path_UXB r;
        BaseConvert(r, tlv, tlvId);
        r.mPatternLength = tlv.mPatternLength;
        r.mPattern = tlv.mPattern;
        r.mScale = relive::From(tlv.mScale);
        r.mStartState = From(tlv.mStartState);
        // note: removed
        //r.mDisabledResources = tlv.mDisabledResources;
        return r;
    }

    static Path_UXB From(const ::Path_UXB& tlv, const Guid& tlvId)
    {
        Path_UXB r;
        BaseConvert(r, tlv, tlvId);
        r.mPatternLength = tlv.mPatternLength;
        r.mPattern = tlv.mPattern;
        r.mScale = relive::From(tlv.mScale);
        r.mStartState = From(tlv.mStartState);
        // note: removed
        //r.mDisabledResources = tlv.mDisabledResources;
        return r;
    }

private:
    static relive::Path_UXB::StartState From(AO::Path_UXB::UXBStartState startState)
    {
        switch (startState)
        {
        case AO::Path_UXB::UXBStartState::eOn:
            return relive::Path_UXB::StartState::eOn;
        case AO::Path_UXB::UXBStartState::eOff:
            return relive::Path_UXB::StartState::eOff;
        }
        ALIVE_FATAL("Bad uxb start state");
    }

    static relive::Path_UXB::StartState From(::Path_UXB::StartState startState)
    {
        switch (startState)
        {
            case ::Path_UXB::StartState::eOn:
                return relive::Path_UXB::StartState::eOn;
            case ::Path_UXB::StartState::eOff:
                return relive::Path_UXB::StartState::eOff;
        }
        ALIVE_FATAL("Bad uxb start state");
    }
};

class Path_MeatSaw_Converter final
{
public:
    static Path_MeatSaw From(const AO::Path_MeatSaw& tlv, const Guid& tlvId)
    {
        Path_MeatSaw r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mSwitchMinTimeOff = tlv.mSwitchMinTimeOff;
        r.mSwitchMaxTimeOff = tlv.mSwitchMaxTimeOff;
        r.mMaxRiseTime = tlv.mMaxRiseTime;
        r.mSwitchId = tlv.mSwitchId;
        r.mType = From(tlv.mType);
        r.mSpeed = tlv.mSpeed;
        r.mStartState = From(tlv.mStartState);
        r.mOffSpeed = tlv.mOffSpeed;
        r.mAutomaticMinTimeOff = tlv.mAutomaticMinTimeOff;
        r.mAutomaticMaxTimeOff = tlv.mAutomaticMaxTimeOff;
        r.mInitialPosition = tlv.mInitialPosition;
        return r;
    }

private:
    static relive::Path_MeatSaw::Type From(AO::Path_MeatSaw::Type type)
    {
        switch (type)
        {
            case AO::Path_MeatSaw::Type::eAutomaticPersistOffscreen_0:
                return relive::Path_MeatSaw::Type::eAutomaticPersistOffscreen;
            case AO::Path_MeatSaw::Type::eAutomatic_1:
                return relive::Path_MeatSaw::Type::eAutomatic;
            case AO::Path_MeatSaw::Type::eSwitchId_2:
                return relive::Path_MeatSaw::Type::eSwitchId;
        }
        ALIVE_FATAL("Bad meat saw type");
    }

    static relive::Path_MeatSaw::StartState From(AO::Path_MeatSaw::StartState startState)
    {
        switch (startState)
        {
            case AO::Path_MeatSaw::StartState::eOff_0:
                return relive::Path_MeatSaw::StartState::eOff;
            case AO::Path_MeatSaw::StartState::eOn_1:
                return relive::Path_MeatSaw::StartState::eOn;
        }
        ALIVE_FATAL("Bad meat saw start state");
    }
};

class Path_Lever_Converter final
{
public:
    static Path_Lever From(const AO::Path_Lever& tlv, const Guid& tlvId)
    {
        Path_Lever r;
        BaseConvert(r, tlv, tlvId);
        r.mAction = relive::From(tlv.mAction);
        r.mScale = relive::From(tlv.mScale);
        r.mOnSound = From(tlv.mOnSound);
        r.mOffSound = From(tlv.mOffSound);
        r.mSoundDirection = From(tlv.mSoundDirection);
        r.mSwitchId = tlv.mSwitchId;
        return r;
    }

    static Path_Lever From(const ::Path_Lever& tlv, const Guid& tlvId)
    {
        Path_Lever r;
        BaseConvert(r, tlv, tlvId);
        r.mAction = relive::From(tlv.mAction);
        r.mScale = relive::From(tlv.mScale);
        r.mOnSound = From(tlv.mOnSound);
        r.mOffSound = From(tlv.mOffSound);
        r.mSoundDirection = From(tlv.mSoundDirection);
        r.mSwitchId = tlv.mSwitchId;
        r.mPersistOffscreen = relive::From(tlv.mPersistOffscreen);
        return r;
    }

private:
    static relive::Path_Lever::LeverSoundType From(AO::Path_Lever::LeverSoundType soundType)
    {
        switch (soundType)
        {
            case AO::Path_Lever::LeverSoundType::eNone:
                return relive::Path_Lever::LeverSoundType::eNone;
            case AO::Path_Lever::LeverSoundType::eWell_1:
                return relive::Path_Lever::LeverSoundType::eWell;
            case AO::Path_Lever::LeverSoundType::eSwitchBellHammer_2:
                return relive::Path_Lever::LeverSoundType::eSwitchBellHammer;
            case AO::Path_Lever::LeverSoundType::eDoor_3:
                return relive::Path_Lever::LeverSoundType::eDoor;
            case AO::Path_Lever::LeverSoundType::eElectricWall_4:
                return relive::Path_Lever::LeverSoundType::eElectricWall;
            case AO::Path_Lever::LeverSoundType::eSecurityOrb_5:
                return relive::Path_Lever::LeverSoundType::eSecurityOrb;
        }
        ALIVE_FATAL("Bad lever sound type");
    }

    static relive::Path_Lever::LeverSoundDirection From(AO::Path_Lever::LeverSoundDirection soundDir)
    {
        switch (soundDir)
        {
            case AO::Path_Lever::LeverSoundDirection::eLeftAndRight_0:
                return relive::Path_Lever::LeverSoundDirection::eLeftAndRight;
            case AO::Path_Lever::LeverSoundDirection::eLeft_1:
                return relive::Path_Lever::LeverSoundDirection::eLeft;
            case AO::Path_Lever::LeverSoundDirection::eRight_2:
                return relive::Path_Lever::LeverSoundDirection::eRight;
        }
        ALIVE_FATAL("Bad lever sound direction");
    }

    static relive::Path_Lever::LeverSoundType From(::Path_Lever::LeverSoundType soundType)
    {
        switch (soundType)
        {
            case ::Path_Lever::LeverSoundType::eNone:
                return relive::Path_Lever::LeverSoundType::eNone;
            case ::Path_Lever::LeverSoundType::eWell_1:
                return relive::Path_Lever::LeverSoundType::eWell;
            case ::Path_Lever::LeverSoundType::eSwitchBellHammer_2:
                return relive::Path_Lever::LeverSoundType::eSwitchBellHammer;
            case ::Path_Lever::LeverSoundType::eDoor_3:
                return relive::Path_Lever::LeverSoundType::eDoor;
            case ::Path_Lever::LeverSoundType::eElectricWall_4:
                return relive::Path_Lever::LeverSoundType::eElectricWall;
            case ::Path_Lever::LeverSoundType::eSecurityOrb_5:
                return relive::Path_Lever::LeverSoundType::eSecurityOrb;
            case ::Path_Lever::LeverSoundType::eLift_6:
                return relive::Path_Lever::LeverSoundType::eLift;
        }
        ALIVE_FATAL("Bad lever sound type");
    }

    static relive::Path_Lever::LeverSoundDirection From(::Path_Lever::LeverSoundDirection soundDir)
    {
        switch (soundDir)
        {
            case ::Path_Lever::LeverSoundDirection::eLeftAndRight_0:
                return relive::Path_Lever::LeverSoundDirection::eLeftAndRight;
            case ::Path_Lever::LeverSoundDirection::eLeft_1:
                return relive::Path_Lever::LeverSoundDirection::eLeft;
            case ::Path_Lever::LeverSoundDirection::eRight_2:
                return relive::Path_Lever::LeverSoundDirection::eRight;
        }
        ALIVE_FATAL("Bad lever sound direction");
    }
};

class Path_Edge_Converter final
{
public:
    static Path_Edge From(const AO::Path_Edge& tlv, const Guid& tlvId)
    {
        Path_Edge r;
        BaseConvert(r, tlv, tlvId);
        r.mGrabDirection = From(tlv.mGrabDirection);
        r.mCanGrab = relive::From(tlv.mCanGrab);
        return r;
    }

    static Path_Edge From(const ::Path_Edge& tlv, const Guid& tlvId)
    {
        Path_Edge r;
        BaseConvert(r, tlv, tlvId);
        r.mGrabDirection = From(tlv.mGrabDirection);
        r.mCanGrab = relive::From(tlv.mCanGrab);
        r.mScale = relive::From(tlv.mScale);
        return r;
    }

private:
    static relive::Path_Edge::GrabDirection From(AO::Path_Edge::GrabDirection grabDir)
    {
        switch (grabDir)
        {
            case AO::Path_Edge::GrabDirection::eFacingLeft:
                return relive::Path_Edge::GrabDirection::eFacingLeft;
            case AO::Path_Edge::GrabDirection::eFacingRight:
                return relive::Path_Edge::GrabDirection::eFacingRight;
            case AO::Path_Edge::GrabDirection::eFacingAnyDirection:
                return relive::Path_Edge::GrabDirection::eFacingAnyDirection;
        }
        ALIVE_FATAL("Bad edge grab direction");
    }

    static relive::Path_Edge::GrabDirection From(::Path_Edge::GrabDirection grabDir)
    {
        switch (grabDir)
        {
        case ::Path_Edge::GrabDirection::eFacingLeft:
            return relive::Path_Edge::GrabDirection::eFacingLeft;
        case ::Path_Edge::GrabDirection::eFacingRight:
            return relive::Path_Edge::GrabDirection::eFacingRight;
        case ::Path_Edge::GrabDirection::eFacingAnyDirection:
            return relive::Path_Edge::GrabDirection::eFacingAnyDirection;
        }
        ALIVE_FATAL("Bad edge grab direction");
    }
};

class Path_BirdPortal_Converter final
{
public:
    static Path_BirdPortal From(const AO::Path_BirdPortal& tlv, const Guid& tlvId)
    {
        Path_BirdPortal r;
        BaseConvert(r, tlv, tlvId);
        r.mEnterSide = From(tlv.mEnterSide);
        r.mExitLevel = MapWrapper::FromAO(tlv.mExitLevel);
        r.mExitPath = tlv.mExitPath;
        r.mExitCamera = tlv.mExitCamera;
        r.mScale = relive::From(tlv.mScale);
        r.mMovieId = tlv.mMovieId;
        r.mPortalType = From(tlv.mPortalType);
        r.mMudCountForShrykull = tlv.mMudCountForShrykull;
        return r;
    }

    static Path_BirdPortal From(const ::Path_BirdPortal& tlv, const Guid& tlvId)
    {
        Path_BirdPortal r;
        BaseConvert(r, tlv, tlvId);
        r.mEnterSide = From(tlv.mEnterSide);
        r.mExitLevel = MapWrapper::FromAE(tlv.mExitLevel);
        r.mExitPath = tlv.mExitPath;
        r.mExitCamera = tlv.mExitCamera;
        r.mScale = relive::From(tlv.mScale);
        r.mMovieId = tlv.mMovieId;
        r.mPortalType = From(tlv.mPortalType);
        r.mMudCountForShrykull = tlv.mMudCountForShrykull;
        r.mCreatePortalSwitchId = tlv.mCreatePortalSwitchId;
        r.mDeletePortalSwitchId = tlv.mDeletePortalSwitchId;
        return r;
    }

    static relive::Path_BirdPortal::PortalSide From(AO::PortalSide portalSide)
    {
        switch (portalSide)
        {
            case AO::PortalSide::eRight_0:
                return relive::Path_BirdPortal::PortalSide::eRight;
            case AO::PortalSide::eLeft_1:
                return relive::Path_BirdPortal::PortalSide::eLeft;
        }
        ALIVE_FATAL("Bad bird portal enter side");
    }

    static relive::Path_BirdPortal::PortalSide From(::PortalSide portalSide)
    {
        switch (portalSide)
        {
            case ::PortalSide::eRight_0:
                return relive::Path_BirdPortal::PortalSide::eRight;
            case ::PortalSide::eLeft_1:
                return relive::Path_BirdPortal::PortalSide::eLeft;
        }
        ALIVE_FATAL("Bad bird portal enter side");
    }

private:
    static relive::Path_BirdPortal::PortalType From(AO::Path_BirdPortal::PortalType portalType)
    {
        switch (portalType)
        {
            case AO::Path_BirdPortal::PortalType::eAbe_0:
                return relive::Path_BirdPortal::PortalType::eAbe;
            case AO::Path_BirdPortal::PortalType::eWorker_1:
                return relive::Path_BirdPortal::PortalType::eWorker;
            case AO::Path_BirdPortal::PortalType::eShrykull_2:
                return relive::Path_BirdPortal::PortalType::eShrykull;
            case AO::Path_BirdPortal::PortalType::eMudTeleport_3:
                return relive::Path_BirdPortal::PortalType::eMudTeleport;
        }
        ALIVE_FATAL("Bad bird portal type");
    }

    static relive::Path_BirdPortal::PortalType From(::Path_BirdPortal::PortalType portalType)
    {
        switch (portalType)
        {
            case ::Path_BirdPortal::PortalType::eAbe_0:
                return relive::Path_BirdPortal::PortalType::eAbe;
            case ::Path_BirdPortal::PortalType::eWorker_1:
                return relive::Path_BirdPortal::PortalType::eWorker;
            case ::Path_BirdPortal::PortalType::eShrykull_2:
                return relive::Path_BirdPortal::PortalType::eShrykull;
        }
        ALIVE_FATAL("Bad bird portal type");
    }
};

class Path_BirdPortalExit_Converter final
{
public:
    static Path_BirdPortalExit From(const AO::Path_BirdPortalExit& tlv, const Guid& tlvId)
    {
        Path_BirdPortalExit r;
        BaseConvert(r, tlv, tlvId);
        r.mExitSide = Path_BirdPortal_Converter::From(tlv.mExitSide);
        r.mScale = relive::From(tlv.mScale);
        return r;
    }

    static Path_BirdPortalExit From(const ::Path_BirdPortalExit& tlv, const Guid& tlvId)
    {
        Path_BirdPortalExit r;
        BaseConvert(r, tlv, tlvId);
        r.mExitSide = Path_BirdPortal_Converter::From(tlv.mExitSide);
        r.mScale = relive::From(tlv.mScale);
        return r;
    }
};

class Path_LightEffect_Converter final
{
public:
    static Path_LightEffect From(const AO::Path_LightEffect& tlv, const Guid& tlvId)
    {
        Path_LightEffect r;
        BaseConvert(r, tlv, tlvId);
        r.mType = From(tlv.mType);
        r.mSize = tlv.mSize;
        r.mSwitchId = tlv.mSwitchId;
        r.mDirection = relive::From(tlv.mDirection);
        return r;
    }

private:
    static relive::Path_LightEffect::Type From(const AO::Path_LightEffect::Type type)
    {
        switch (type)
        {
            case AO::Path_LightEffect::Type::Star_0:
                return relive::Path_LightEffect::Type::Star;
            case AO::Path_LightEffect::Type::GoldGlow_1:
                return relive::Path_LightEffect::Type::GoldGlow;
            case AO::Path_LightEffect::Type::GreenGlow_2:
                return relive::Path_LightEffect::Type::GreenGlow;
            case AO::Path_LightEffect::Type::FlintGlow_3:
                return relive::Path_LightEffect::Type::FlintGlow;
            case AO::Path_LightEffect::Type::Switchable_RedGreenDoorLights_4:
                return relive::Path_LightEffect::Type::Switchable_RedGreenDoorLights;
            case AO::Path_LightEffect::Type::Switchable_RedGreenHubLight_5:
                return relive::Path_LightEffect::Type::Switchable_RedGreenHubLight;
        }
        ALIVE_FATAL("Bad light effect type");
    }
};

class Path_MusicTrigger_Converter final
{
public:
    static Path_MusicTrigger From(const AO::Path_MusicTrigger& tlv, const Guid& tlvId)
    {
        Path_MusicTrigger r;
        BaseConvert(r, tlv, tlvId);
        r.mMusicType = From(tlv.mMusicType);
        r.mTriggeredBy = From(tlv.mTriggeredBy);
        r.mSwitchId = tlv.mSwitchId;
        r.mMusicDelay = tlv.mMusicDelay;
        return r;
    }

    static Path_MusicTrigger From(const ::Path_MusicTrigger& tlv, const Guid& tlvId)
    {
        Path_MusicTrigger r;
        BaseConvert(r, tlv, tlvId);
        r.mMusicType = From(tlv.mMusicType);
        r.mTriggeredBy = From(tlv.mTriggeredBy);
        r.mMusicDelay = tlv.mMusicDelay;
        return r;
    }

private:
    static relive::Path_MusicTrigger::MusicTriggerMusicType From(const AO::Path_MusicTrigger::MusicTriggerMusicType musicType)
    {
        switch (musicType)
        {
            case AO::Path_MusicTrigger::MusicTriggerMusicType::eDrumAmbience_0:
                return relive::Path_MusicTrigger::MusicTriggerMusicType::eDrumAmbience;
            case AO::Path_MusicTrigger::MusicTriggerMusicType::eDeathDrumShort_1:
                return relive::Path_MusicTrigger::MusicTriggerMusicType::eDeathDrumShort;
            case AO::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaLong_2:
                return relive::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaLong;
            case AO::Path_MusicTrigger::MusicTriggerMusicType::eSoftChase_3:
                return relive::Path_MusicTrigger::MusicTriggerMusicType::eSoftChase;
            case AO::Path_MusicTrigger::MusicTriggerMusicType::eIntenseChase_4:
                return relive::Path_MusicTrigger::MusicTriggerMusicType::eIntenseChase;
            case AO::Path_MusicTrigger::MusicTriggerMusicType::eChime_5:
                return relive::Path_MusicTrigger::MusicTriggerMusicType::eChime;
            case AO::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaShort_6:
                return relive::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaShort;
        }
        ALIVE_FATAL("Bad music trigger music type");
    }

    static relive::Path_MusicTrigger::TriggeredBy From(const AO::Path_MusicTrigger::TriggeredBy triggeredBy)
    {
        switch (triggeredBy)
        {
            case AO::Path_MusicTrigger::TriggeredBy::eTimer_0:
                return relive::Path_MusicTrigger::TriggeredBy::eTimer;
            case AO::Path_MusicTrigger::TriggeredBy::eTouching_1:
                return relive::Path_MusicTrigger::TriggeredBy::eTouching;
            case AO::Path_MusicTrigger::TriggeredBy::eSwitchID_2:
                return relive::Path_MusicTrigger::TriggeredBy::eSwitchId;
            case AO::Path_MusicTrigger::TriggeredBy::eUnknown_3:
                return relive::Path_MusicTrigger::TriggeredBy::eUnknown;
        }
        ALIVE_FATAL("Bad music trigger triggered by value");
    }

    static relive::Path_MusicTrigger::MusicTriggerMusicType From(const ::Path_MusicTrigger::MusicTriggerMusicType musicType)
    {
        switch (musicType)
        {
            case ::Path_MusicTrigger::MusicTriggerMusicType::eDrumAmbience_0:
                return relive::Path_MusicTrigger::MusicTriggerMusicType::eDrumAmbience;
            case ::Path_MusicTrigger::MusicTriggerMusicType::eDeathDrumShort_1:
                return relive::Path_MusicTrigger::MusicTriggerMusicType::eDeathDrumShort;
            case ::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaLong_2:
                return relive::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaLong;
            case ::Path_MusicTrigger::MusicTriggerMusicType::eSoftChase_3:
                return relive::Path_MusicTrigger::MusicTriggerMusicType::eSoftChase;
            case ::Path_MusicTrigger::MusicTriggerMusicType::eIntenseChase_4:
                return relive::Path_MusicTrigger::MusicTriggerMusicType::eIntenseChase;
            case ::Path_MusicTrigger::MusicTriggerMusicType::eChime_5:
                return relive::Path_MusicTrigger::MusicTriggerMusicType::eChime;
            case ::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaShort_6:
                return relive::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaShort;
        }
        ALIVE_FATAL("Bad music trigger music type");
    }

    static relive::Path_MusicTrigger::TriggeredBy From(const ::Path_MusicTrigger::TriggeredBy triggeredBy)
    {
        switch (triggeredBy)
        {
            case ::Path_MusicTrigger::TriggeredBy::eTimer_0:
                return relive::Path_MusicTrigger::TriggeredBy::eTimer;
            case ::Path_MusicTrigger::TriggeredBy::eTouching_1:
                return relive::Path_MusicTrigger::TriggeredBy::eTouching;
        }
        ALIVE_FATAL("Bad music trigger triggered by value");
    }
};

class Path_SoftLanding_Converter final
{
public:
    static Path_SoftLanding From(const AO::Path_SoftLanding& tlv, const Guid& tlvId)
    {
        Path_SoftLanding r;
        BaseConvert(r, tlv, tlvId);
        return r;
    }

    static Path_SoftLanding From(const ::Path_SoftLanding& tlv, const Guid& tlvId)
    {
        Path_SoftLanding r;
        BaseConvert(r, tlv, tlvId);
        r.mSwitchId = tlv.mSwitchId;
        return r;
    }
};

class Path_LiftMover_Converter final
{
public:
    static Path_LiftMover From(const AO::Path_LiftMover& tlv, const Guid& tlvId)
    {
        Path_LiftMover r;
        BaseConvert(r, tlv, tlvId);
        r.mLiftMoverSwitchId = tlv.mLiftMoverSwitchId;
        r.mTargetLiftPointId = tlv.mTargetLiftPointId;
        r.mMoveDirection = From(tlv.mMoveDirection);
        return r;
    }

    static Path_LiftMover From(const ::Path_LiftMover& tlv, const Guid& tlvId)
    {
        Path_LiftMover r;
        BaseConvert(r, tlv, tlvId);
        r.mLiftMoverSwitchId = tlv.mLiftMoverSwitchId;
        r.mTargetLiftPointId = tlv.mTargetLiftPointId;
        r.mMoveDirection = From(tlv.mMoveDirection);
        return r;
    }

private:
    static relive::Path_LiftMover::YDirection From(const AO::Path_LiftMover::YDirection yDir)
    {
        switch (yDir)
        {
            case AO::Path_LiftMover::YDirection::eDown_0:
                return relive::Path_LiftMover::YDirection::eDown;
            case AO::Path_LiftMover::YDirection::eUp_1:
                return relive::Path_LiftMover::YDirection::eUp;
        }
        ALIVE_FATAL("Bad lift mover y direction");
    }

    static relive::Path_LiftMover::YDirection From(const ::Path_LiftMover::YDirection yDir)
    {
        switch (yDir)
        {
            case ::Path_LiftMover::YDirection::eDown_0:
                return relive::Path_LiftMover::YDirection::eDown;
            case ::Path_LiftMover::YDirection::eUp_1:
                return relive::Path_LiftMover::YDirection::eUp;
        }
        ALIVE_FATAL("Bad lift mover y direction");
    }
};

class Path_HintFly_Converter final
{
public:
    static Path_HintFly From(const AO::Path_HintFly& tlv, const Guid& tlvId)
    {
        Path_HintFly r;
        BaseConvert(r, tlv, tlvId);
        r.mMessageId = tlv.mMessageId;
        return r;
    }
};

class Path_TimerTrigger_Converter final
{
public:
    static Path_TimerTrigger From(const AO::Path_TimerTrigger& tlv, const Guid& tlvId)
    {
        Path_TimerTrigger r;
        BaseConvert(r, tlv, tlvId);
        r.mInputSwitchId = tlv.mInputSwitchId;
        r.mActivationDelay = tlv.mActivationDelay;
        r.mOutputSwitchId1 = tlv.mOutputSwitchId1;
        r.mOutputSwitchId2 = tlv.mOutputSwitchId2;
        r.mOutputSwitchId3 = tlv.mOutputSwitchId3;
        r.mOutputSwitchId4 = tlv.mOutputSwitchId4;
        return r;
    }

    static Path_TimerTrigger From(const ::Path_TimerTrigger& tlv, const Guid& tlvId)
    {
        Path_TimerTrigger r;
        BaseConvert(r, tlv, tlvId);
        r.mInputSwitchId = tlv.mInputSwitchId;
        r.mActivationDelay = tlv.mActivationDelay;
        r.mOutputSwitchId1 = tlv.mOutputSwitchId1;
        r.mOutputSwitchId2 = tlv.mOutputSwitchId2;
        r.mOutputSwitchId3 = tlv.mOutputSwitchId3;
        r.mOutputSwitchId4 = tlv.mOutputSwitchId4;
        return r;
    }
};

class Path_FlintLockFire_Converter final
{
public:
    static Path_FlintLockFire From(const AO::Path_FlintLockFire& tlv, const Guid& tlvId)
    {
        Path_FlintLockFire r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mSwitchId = tlv.mSwitchId;
        return r;
    }
};

class Path_HoneySack_Converter final
{
public:
    static Path_HoneySack From(const AO::Path_HoneySack& tlv, const Guid& tlvId)
    {
        Path_HoneySack r;
        BaseConvert(r, tlv, tlvId);
        r.mChaseTime = tlv.mChaseTime;
        r.mScale = relive::From(tlv.mScale);
        return r;
    }
};

class Path_Bat_Converter final
{
public:
    static Path_Bat From(const AO::Path_Bat& tlv, const Guid& tlvId)
    {
        Path_Bat r;
        BaseConvert(r, tlv, tlvId);
        r.mTimeBeforeMoving = tlv.mTimeBeforeMoving;
        r.mSpeed = tlv.mSpeed;
        r.mScale = relive::From(tlv.mScale);
        r.mAttackDuration = tlv.mAttackDuration;
        return r;
    }
};

class Path_RollingBallStopper_Converter final
{
public:
    static Path_RollingBallStopper From(const AO::Path_RollingBallStopper& tlv, const Guid& tlvId)
    {
        Path_RollingBallStopper r;
        BaseConvert(r, tlv, tlvId);
        r.mStopperSwitchId = tlv.mStopperSwitchId;
        r.mScale = relive::From(tlv.mScale);
        r.mBallSwitchId = tlv.mBallSwitchId;
        r.mStopDirection = relive::From(tlv.mStopDirection);
        return r;
    }
};

class Path_RollingBall_Converter final
{
public:
    static Path_RollingBall From(const AO::Path_RollingBall& tlv, const Guid& tlvId)
    {
        Path_RollingBall r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mRollDirection = relive::From(tlv.mRollDirection);
        r.mReleaseSwitchId = tlv.mReleaseSwitchId;
        r.mMaxSpeed = tlv.mMaxSpeed;
        r.mAcceleration = tlv.mAcceleration;
        return r;
    }
};

class Path_MotionDetector_Converter final
{
public:
    static Path_MotionDetector From(const AO::Path_MotionDetector& tlv, const Guid& tlvId)
    {
        Path_MotionDetector r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mDeviceX = tlv.mDeviceX;
        r.mDeviceY = tlv.mDeviceY;
        r.mSpeedx256 = tlv.mSpeedx256;
        r.mInitialMoveDirection = From(tlv.mInitialMoveDirection);
        r.mDrawFlare = relive::From(tlv.mDrawFlare);
        r.mDisableSwitchId = tlv.mDisableSwitchId;
        r.mAlarmSwitchId = tlv.mAlarmSwitchId;
        r.mAlarmDuration = tlv.mAlarmDuration;
        return r;
    }

    static Path_MotionDetector From(const ::Path_MotionDetector& tlv, const Guid& tlvId)
    {
        Path_MotionDetector r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mDeviceX = tlv.mDeviceX;
        r.mDeviceY = tlv.mDeviceY;
        r.mSpeedx256 = tlv.mSpeedx256;
        r.mInitialMoveDirection = From(tlv.mInitialMoveDirection);
        r.mDrawFlare = relive::From(tlv.mDrawFlare);
        r.mDisableSwitchId = tlv.mDisableSwitchId;
        r.mAlarmSwitchId = tlv.mAlarmSwitchId;
        r.mAlarmDuration = tlv.mAlarmDuration;
        return r;
    }

private:
    static relive::Path_MotionDetector::InitialMoveDirection From(AO::Path_MotionDetector::InitialMoveDirection dir)
    {
        switch (dir)
        {
            case AO::Path_MotionDetector::InitialMoveDirection::eRight_0:
                return relive::Path_MotionDetector::InitialMoveDirection::eRight;
            case AO::Path_MotionDetector::InitialMoveDirection::eLeft_1:
                return relive::Path_MotionDetector::InitialMoveDirection::eLeft;
        }
        ALIVE_FATAL("Bad motion detector initial move direction");
    }

    static relive::Path_MotionDetector::InitialMoveDirection From(::Path_MotionDetector::InitialMoveDirection dir)
    {
        switch (dir)
        {
            case ::Path_MotionDetector::InitialMoveDirection::eRight_0:
                return relive::Path_MotionDetector::InitialMoveDirection::eRight;
            case ::Path_MotionDetector::InitialMoveDirection::eLeft_1:
                return relive::Path_MotionDetector::InitialMoveDirection::eLeft;
        }
        ALIVE_FATAL("Bad motion detector initial move direction");
    }
};

class Path_BellHammer_Converter final
{
public:
    static Path_BellHammer From(const AO::Path_BellHammer& tlv, const Guid& tlvId)
    {
        Path_BellHammer r;
        BaseConvert(r, tlv, tlvId);
        r.mSwitchId = tlv.mSwitchId;
        r.mAction = relive::From(tlv.mAction);
        r.mScale = relive::From(tlv.mScale);
        r.mDirection = relive::From(tlv.mDirection);
        return r;
    }
};

class Path_SligBoundLeft_Converter final
{
public:
    static Path_SligBoundLeft From(const AO::Path_SligBoundLeft& tlv, const Guid& tlvId)
    {
        Path_SligBoundLeft r;
        BaseConvert(r, tlv, tlvId);
        r.mSligBoundId = tlv.mSligBoundId;
        r.mDisabledResourcesAO = tlv.mDisabledResources;
        return r;
    }

    static Path_SligBoundLeft From(const ::Path_SligBoundLeft& tlv, const Guid& tlvId)
    {
        Path_SligBoundLeft r;
        BaseConvert(r, tlv, tlvId);
        r.mSligBoundId = tlv.mSligBoundId;
        r.mDisabledResourcesAE = tlv.mDisabledResources;
        return r;
    }
};

class Path_SligBoundRight_Converter final
{
public:
    static Path_SligBoundRight From(const AO::Path_SligBoundRight& tlv, const Guid& tlvId)
    {
        Path_SligBoundRight r;
        BaseConvert(r, tlv, tlvId);
        r.mSligBoundId = tlv.mSligBoundId;
        r.mDisabledResourcesAO = tlv.mDisabledResources;
        return r;
    }

    static Path_SligBoundRight From(const ::Path_SligBoundRight& tlv, const Guid& tlvId)
    {
        Path_SligBoundRight r;
        BaseConvert(r, tlv, tlvId);
        r.mSligBoundId = tlv.mSligBoundId;
        r.mDisabledResourcesAE = tlv.mDisabledResources;
        return r;
    }
};

class Path_SligPersist_Converter final
{
public:
    static Path_SligPersist From(const AO::Path_SligPersist& tlv, const Guid& tlvId)
    {
        Path_SligPersist r;
        BaseConvert(r, tlv, tlvId);
        r.mSligBoundId = tlv.mSligBoundId;
        r.mDisabledResourcesAO = tlv.mDisabledResources;
        return r;
    }

    static Path_SligPersist From(const ::Path_SligPersist& tlv, const Guid& tlvId)
    {
        Path_SligPersist r;
        BaseConvert(r, tlv, tlvId);
        r.mSligBoundId = tlv.mSligBoundId;
        r.mDisabledResourcesAE = tlv.mDisabledResources;
        return r;
    }
};

class Path_BackgroundAnimation_Converter final
{
public:
    static Path_BackgroundAnimation From(const AO::Path_BackgroundAnimation& tlv, const Guid& tlvId)
    {
        Path_BackgroundAnimation r;
        BaseConvert(r, tlv, tlvId);
        r.mAnimId = tlv.mAnimId;
        r.mIsSemiTrans = relive::From(tlv.mIsSemiTrans);
        r.mSemiTransMode = tlv.mSemiTransMode;
        r.mSoundEffect = From(tlv.mSoundEffect);
        return r;
    }

    static Path_BackgroundAnimation From(const ::Path_BackgroundAnimation& tlv, const Guid& tlvId)
    {
        Path_BackgroundAnimation r;
        BaseConvert(r, tlv, tlvId);
        r.mAnimId = tlv.mAnimId;
        r.mIsSemiTrans = relive::From(tlv.mIsSemiTrans);
        r.mSemiTransMode = tlv.mSemiTransMode;
        r.mLayer = From(tlv.mLayer);
        return r;
    }

private:
    static Path_BackgroundAnimation::Layer From(Layer layer)
    {
        switch (layer)
        {
            case Layer::eLayer_0:
                return Path_BackgroundAnimation::Layer::eLayer0;

            case Layer::eLayer_NotUsed_2:
                return Path_BackgroundAnimation::Layer::eLayer2;

        }
        ALIVE_FATAL("Bad BG Anim layer");
    }

    static Path_BackgroundAnimation::BgAnimSounds From(AO::Path_BackgroundAnimation::BgAnimSounds sound)
    {
        switch (sound)
        {
            case AO::Path_BackgroundAnimation::BgAnimSounds::eNone_m1:
                return Path_BackgroundAnimation::BgAnimSounds::eNone;
            case AO::Path_BackgroundAnimation::BgAnimSounds::eNone_0:
                return Path_BackgroundAnimation::BgAnimSounds::eNone;
            case AO::Path_BackgroundAnimation::BgAnimSounds::eFire_1:
                return Path_BackgroundAnimation::BgAnimSounds::eFire;
        }
        ALIVE_FATAL("Bad bg anim sound");
    }
};

class Path_MainMenuController_Converter final
{
public:
    static Path_MainMenuController From(const AO::Path_MainMenuController& tlv, const Guid& tlvId)
    {
        Path_MainMenuController r;
        BaseConvert(r, tlv, tlvId);
        return r;
    }

    static Path_MainMenuController From(const ::Path_MainMenuController& tlv, const Guid& tlvId)
    {
        Path_MainMenuController r;
        BaseConvert(r, tlv, tlvId);
        return r;
    }
};

class Path_ElumWall_Converter final
{
public:
    static Path_ElumWall From(const AO::Path_ElumWall& tlv, const Guid& tlvId)
    {
        Path_ElumWall r;
        BaseConvert(r, tlv, tlvId);
        return r;
    }
};

class Path_ElumStart_Converter final
{
public:
    static Path_ElumStart From(const AO::Path_ElumStart& tlv, const Guid& tlvId)
    {
        Path_ElumStart r;
        BaseConvert(r, tlv, tlvId);
        return r;
    }
};

class Path_KillUnsavedMuds_Converter final
{
public:
    static Path_KillUnsavedMuds From(const AO::Path_KillUnsavedMuds& tlv, const Guid& tlvId)
    {
        Path_KillUnsavedMuds r;
        BaseConvert(r, tlv, tlvId);
        return r;
    }
};

class Path_InvisibleZone_Converter final
{
public:
    static Path_InvisibleZone From(const AO::Path_InvisibleZone& tlv, const Guid& tlvId)
    {
        Path_InvisibleZone r;
        BaseConvert(r, tlv, tlvId);
        return r;
    }

    static Path_InvisibleZone From(const ::Path_InvisibleZone& tlv, const Guid& tlvId)
    {
        Path_InvisibleZone r;
        BaseConvert(r, tlv, tlvId);
        return r;
    }
};

class Path_StartController_Converter final
{
public:
    static Path_StartController From(const AO::Path_StartController& tlv, const Guid& tlvId)
    {
        Path_StartController r;
        BaseConvert(r, tlv, tlvId);
        return r;
    }
};

class Path_ScrabNoFall_Converter final
{
public:
    static Path_ScrabNoFall From(const AO::Path_ScrabNoFall& tlv, const Guid& tlvId)
    {
        Path_ScrabNoFall r;
        BaseConvert(r, tlv, tlvId);
        return r;
    }
};

class Path_ScrabBoundLeft_Converter final
{
public:
    static Path_ScrabBoundLeft From(const AO::Path_ScrabBoundLeft& tlv, const Guid& tlvId)
    {
        Path_ScrabBoundLeft r;
        BaseConvert(r, tlv, tlvId);
        return r;
    }

    static Path_ScrabBoundLeft From(const ::Path_ScrabBoundLeft& tlv, const Guid& tlvId)
    {
        Path_ScrabBoundLeft r;
        BaseConvert(r, tlv, tlvId);
        return r;
    }
};

class Path_ScrabBoundRight_Converter final
{
public:
    static Path_ScrabBoundRight From(const AO::Path_ScrabBoundRight& tlv, const Guid& tlvId)
    {
        Path_ScrabBoundRight r;
        BaseConvert(r, tlv, tlvId);
        return r;
    }

    static Path_ScrabBoundRight From(const ::Path_ScrabBoundRight& tlv, const Guid& tlvId)
    {
        Path_ScrabBoundRight r;
        BaseConvert(r, tlv, tlvId);
        return r;
    }
};

class Path_HoneyDripTarget_Converter final
{
public:
    static Path_HoneyDripTarget From(const AO::Path_HoneyDripTarget& tlv, const Guid& tlvId)
    {
        Path_HoneyDripTarget r;
        BaseConvert(r, tlv, tlvId);
        return r;
    }
};

class Path_ZSligCover_Converter final
{
public:
    static Path_ZSligCover From(const AO::Path_ZSligCover& tlv, const Guid& tlvId)
    {
        Path_ZSligCover r;
        BaseConvert(r, tlv, tlvId);
        return r;
    }

    static Path_ZSligCover From(const ::Path_ZSligCover& tlv, const Guid& tlvId)
    {
        Path_ZSligCover r;
        BaseConvert(r, tlv, tlvId);
        return r;
    }
};

class Path_DeathDrop_Converter final
{
public:
    static Path_DeathDrop From(const AO::Path_DeathDrop& tlv, const Guid& tlvId)
    {
        Path_DeathDrop r;
        BaseConvert(r, tlv, tlvId);
        return r;
    }

    static Path_DeathDrop From(const ::Path_DeathDrop& tlv, const Guid& tlvId)
    {
        Path_DeathDrop r;
        BaseConvert(r, tlv, tlvId);
        return r;
    }
};

class Path_ChimeLock_Converter final
{
public:
    static Path_ChimeLock From(const AO::Path_ChimeLock& tlv, const Guid& tlvId)
    {
        Path_ChimeLock r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mSolveSwitchId = tlv.mSolveSwitchId;
        r.mCode1 = tlv.mCode1;
        r.mCode2 = tlv.mCode2;
        r.mPasswordSwitchId = tlv.mPasswordSwitchId;
        return r;
    }
};

class Path_LCDStatusBoard_Converter final
{
public:
    static Path_LCDStatusBoard From(const AO::Path_LCDStatusBoard& tlv, const Guid& tlvId)
    {
        Path_LCDStatusBoard r;
        BaseConvert(r, tlv, tlvId);
        return r;
    }

    static Path_LCDStatusBoard From(const ::Path_LCDStatusBoard& tlv, const Guid& tlvId)
    {
        Path_LCDStatusBoard r;
        BaseConvert(r, tlv, tlvId);
        r.mNumberOfMuds = tlv.mNumberOfMuds;
        r.mZulagNumber = tlv.mZulagNumber;
        r.mHideBoard = relive::From(tlv.mHideBoard);
        return r;
    }
};

class Path_CreditsController_Converter final
{
public:
    static Path_CreditsController From(const AO::Path_CreditsController& tlv, const Guid& tlvId)
    {
        Path_CreditsController r;
        BaseConvert(r, tlv, tlvId);
        return r;
    }

    static Path_CreditsController From(const ::Path_CreditsController& tlv, const Guid& tlvId)
    {
        Path_CreditsController r;
        BaseConvert(r, tlv, tlvId);
        return r;
    }
};

class Path_ResetPath_Converter final
{
public:
    static Path_ResetPath From(const AO::Path_ResetPath& tlv, const Guid& tlvId)
    {
        Path_ResetPath r;
        BaseConvert(r, tlv, tlvId);
        r.mClearIds = relive::From(tlv.mClearIds);
        r.mFrom = tlv.mFrom;
        r.mTo = tlv.mTo;
        r.mExclude = tlv.mExclude;
        r.mClearObjects = relive::From(tlv.mClearObjects);
        r.mPath = tlv.mPath;
        return r;
    }

    static Path_ResetPath From(const ::Path_ResetPath& tlv, const Guid& tlvId)
    {
        Path_ResetPath r;
        BaseConvert(r, tlv, tlvId);
        r.mClearIds = relive::From(tlv.mClearIds);
        r.mFrom = tlv.mFrom;
        r.mTo = tlv.mTo;
        r.mExclude = tlv.mExclude;
        r.mClearObjects = relive::From(tlv.mClearObjects);
        r.mPath = tlv.mPath;
        r.mEnabled = relive::From(tlv.mEnabled);
        return r;
    }
};

class Path_MeatSack_Converter final
{
public:
    static Path_MeatSack From(const AO::Path_MeatSack& tlv, const Guid& tlvId)
    {
        Path_MeatSack r;
        BaseConvert(r, tlv, tlvId);
        r.mMeatFallDirection = relive::From(tlv.mMeatFallDirection);
        r.mVelX = tlv.mVelX;
        r.mVelY = tlv.mVelY;
        r.mScale = relive::From(tlv.mScale);
        r.mMeatAmount = tlv.mMeatAmount;
        return r;
    }

    static Path_MeatSack From(const ::Path_MeatSack& tlv, const Guid& tlvId)
    {
        Path_MeatSack r;
        BaseConvert(r, tlv, tlvId);
        r.mMeatFallDirection = relive::From(tlv.mMeatFallDirection);
        r.mVelX = tlv.mVelX;
        r.mVelY = tlv.mVelY;
        r.mScale = relive::From(tlv.mScale);
        r.mMeatAmount = tlv.mMeatAmount;
        return r;
    }
};

class Path_BeeNest_Converter final
{
public:
    static Path_BeeNest From(const AO::Path_BeeNest& tlv, const Guid& tlvId)
    {
        Path_BeeNest r;
        BaseConvert(r, tlv, tlvId);
        r.mSwitchId = tlv.mSwitchId;
        r.mSwarmSize = tlv.mSwarmSize;
        r.mChaseTime = tlv.mChaseTime;
        r.mSpeed = tlv.mSpeed;
        r.mDisabledResources = tlv.mDisabledResources;
        r.mBeesAmount = tlv.mBeesAmount;
        return r;
    }
};

class Path_BellsongStone_Converter final
{
public:
    static Path_BellsongStone From(const AO::Path_BellsongStone& tlv, const Guid& tlvId)
    {
        Path_BellsongStone r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mData.mScale);
        r.mType = From(tlv.mData.mType);
        r.mCode1 = tlv.mData.mCode1;
        r.mCode2 = tlv.mData.mCode2;
        r.mSwitchId = tlv.mData.mSwitchId;
        return r;
    }

private:
    static Path_BellsongStone::BellsongTypes From(AO::BellsongTypes type)
    {
        switch (type)
        {
            case AO::BellsongTypes::eWhistle:
                return Path_BellsongStone::BellsongTypes::eWhistle;
            case AO::BellsongTypes::eChimes:
                return Path_BellsongStone::BellsongTypes::eChimes;
        }
        ALIVE_FATAL("Bad bellsong type");
    }
};

class Path_MovieStone_Converter final
{
public:
    static Path_MovieStone From(const AO::Path_MovieStone& tlv, const Guid& tlvId)
    {
        Path_MovieStone r;
        BaseConvert(r, tlv, tlvId);
        r.mMovieId = tlv.mData.mMovieId;
        r.mScale = relive::From(tlv.mData.mScale);
        return r;
    }

    static Path_MovieStone From(const ::Path_MovieStone& tlv, const Guid& tlvId)
    {
        Path_MovieStone r;
        BaseConvert(r, tlv, tlvId);
        r.mMovieId = tlv.mMovieId;
        r.mScale = relive::From(tlv.mScale);
        r.mTriggerSwitchId = tlv.mTriggerSwitchId;
        return r;
    }
};

class Path_HandStone_Converter final
{
public:
    static Path_HandStone From(const AO::Path_HandStone& tlv, const Guid& tlvId)
    {
        Path_HandStone r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mData.scale);
        r.mCameraId1 = tlv.mData.camera1.camera;
        r.mPath1 = tlv.mData.camera1.path;
        r.mLevel1 = MapWrapper::FromAO(tlv.mData.camera1.level);
        r.mCameraId2 = tlv.mData.camera2.camera;
        r.mPath2 = tlv.mData.camera2.path;
        r.mLevel2 = MapWrapper::FromAO(tlv.mData.camera2.level);
        r.mCameraId3 = tlv.mData.camera3.camera;
        r.mPath3 = tlv.mData.camera3.path;
        r.mLevel3 = MapWrapper::FromAO(tlv.mData.camera3.level);
        return r;
    }

    static Path_HandStone From(const ::Path_HandStone& tlv, const Guid& tlvId)
    {
        Path_HandStone r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mCameraId1 = tlv.mCameraId1;
        r.mCameraId2 = tlv.mCameraId2;
        r.mCameraId3 = tlv.mCameraId3;
        r.mTriggerSwitchId = tlv.mTriggerSwitchId;
        return r;
    }

};

class Path_PathTransition_Converter final
{
public:
    static Path_PathTransition From(const AO::Path_PathTransition& tlv, const Guid& tlvId)
    {
        Path_PathTransition r;
        BaseConvert(r, tlv, tlvId);
        r.mNextLevel = MapWrapper::FromAO(tlv.mNextLevel);
        r.mNextPath = tlv.mNextPath;
        r.mNextCamera = tlv.mNextCamera;
        r.mMovieId = tlv.mMovieId;
        r.mWipeEffect = tlv.mWipeEffect;
        r.mNextPathScale = relive::From(tlv.mNextPathScale);
        return r;
    }

    static Path_PathTransition From(const ::Path_PathTransition& tlv, const Guid& tlvId)
    {
        Path_PathTransition r;
        BaseConvert(r, tlv, tlvId);
        r.mNextLevel = MapWrapper::FromAE(tlv.mNextLevel);
        r.mNextPath = tlv.mNextPath;
        r.mNextCamera = tlv.mNextCamera;
        r.mMovieId = tlv.mMovieId;
        r.mWipeEffect = tlv.mWipeEffect;
        r.mNextPathScale = relive::From(tlv.mNextPathScale);
        return r;
    }
};

class Path_Pulley_Converter final
{
public:
    static Path_Pulley From(const AO::Path_Pulley& tlv, const Guid& tlvId)
    {
        Path_Pulley r;
        BaseConvert(r, tlv, tlvId);
        return r;
    }

    static Path_Pulley From(const ::Path_Pulley& tlv, const Guid& tlvId)
    {
        Path_Pulley r;
        BaseConvert(r, tlv, tlvId);
        return r;
    }
};

class Path_Honey_Converter final
{
public:
    static Path_Honey From(const AO::Path_Honey& tlv, const Guid& tlvId)
    {
        Path_Honey r;
        BaseConvert(r, tlv, tlvId);
        return r;
    }
};

class Path_BeeSwarmHole_Converter final
{
public:
    static Path_BeeSwarmHole From(const AO::Path_BeeSwarmHole& tlv, const Guid& tlvId)
    {
        Path_BeeSwarmHole r;
        BaseConvert(r, tlv, tlvId);
        r.mStartInterval = tlv.mStartInterval;
        r.mMovementType = From(tlv.mMovementType);
        r.mBeesAmount = tlv.mBeesAmount;
        r.mChaseTime = tlv.mChaseTime;
        r.mSpeed = tlv.mSpeed;
        return r;
    }
private:
    static Path_BeeSwarmHole::MovementType From(AO::Path_BeeSwarmHole::MovementType type)
    {
        switch (type)
        {
            case AO::Path_BeeSwarmHole::MovementType::eHover_0:
                return Path_BeeSwarmHole::MovementType::eHover;
            case AO::Path_BeeSwarmHole::MovementType::eAttack_1:
                return Path_BeeSwarmHole::MovementType::eAttack;
            case AO::Path_BeeSwarmHole::MovementType::eFollowPath_2:
                return Path_BeeSwarmHole::MovementType::eFollowPath;
        }
        ALIVE_FATAL("Bad bee swarm hole movement type");
    }
};

class Path_Door_Converter final
{
public:
    static Path_Door From(const AO::Path_Door& tlv, const Guid& tlvId)
    {
        Path_Door r;
        BaseConvert(r, tlv, tlvId);
        r.mNextLevel = MapWrapper::FromAO(tlv.mNextLevel);
        r.mNextPath = tlv.mNextPath;
        r.mNextCamera = tlv.mNextCamera;
        r.mScale = relive::From(tlv.mScale);
        r.mDoorId = tlv.mDoorId;
        r.mSwitchId = tlv.mSwitchId;
        r.mTargetDoorId = tlv.mTargetDoorId;
        r.mDoorType = From(tlv.mDoorType);
        r.mDoorClosed = relive::From(tlv.mDoorClosed);
        r.mHub1 = tlv.mHub1;
        r.mHub2 = tlv.mHub2;
        r.mHub3 = tlv.mHub3;
        r.mHub4 = tlv.mHub4;
        r.mHub5 = tlv.mHub5;
        r.mHub6 = tlv.mHub6;
        r.mHub7 = tlv.mHub7;
        r.mHub8 = tlv.mHub8;
        // for the time being until we have an enum for wipe_effect
        r.mWipeEffect = static_cast<reliveScreenChangeEffects>(tlv.mWipeEffect);
        r.mMovieId = tlv.mMovieId;
        r.mDoorOffsetX = tlv.mDoorOffsetX;
        r.mDoorOffsetY = tlv.mDoorOffsetY;
        r.mExitDirection = relive::From(tlv.mExitDirection);
        return r;
    }

    static Path_Door From(const ::Path_Door& tlv, const Guid& tlvId)
    {
        Path_Door r;
        BaseConvert(r, tlv, tlvId);
        r.mNextLevel = MapWrapper::FromAE(tlv.mNextLevel);
        r.mNextPath = tlv.mNextPath;
        r.mNextCamera = tlv.mNextCamera;
        r.mScale = relive::From(tlv.mScale);
        r.mDoorId = tlv.mDoorId;
        r.mSwitchId = tlv.mSwitchId;
        r.mTargetDoorId = tlv.mTargetDoorNumber;
        r.mDoorType = From(tlv.mDoorType);
        r.mStartState = From(tlv.mStartState);
        r.mHub1 = tlv.mHub1;
        r.mHub2 = tlv.mHub2;
        r.mHub3 = tlv.mHub3;
        r.mHub4 = tlv.mHub4;
        r.mHub5 = tlv.mHub5;
        r.mHub6 = tlv.mHub6;
        r.mHub7 = tlv.mHub7;
        r.mHub8 = tlv.mHub8;
        r.mWipeEffect = relive::From(tlv.mWipeEffect);
        r.mMovieId = tlv.mMovieId;
        r.mDoorOffsetX = tlv.mDoorOffsetX;
        r.mDoorOffsetY = tlv.mDoorOffsetY;
        r.mExitDirection = relive::From(tlv.mExitDirection);
        r.mCloseOnExit = relive::From(tlv.mCloseOnExit);
        r.mClearThrowables = relive::From(tlv.mClearThrowables);
        return r;
    }
private:
    static Path_Door::DoorStates From(::Path_Door::DoorStates state)
    {
        switch (state)
        {
            case ::Path_Door::DoorStates::eOpen_0:
                return Path_Door::DoorStates::eOpen;
            case ::Path_Door::DoorStates::eClosed_1:
                return Path_Door::DoorStates::eClosed;
            case ::Path_Door::DoorStates::eOpening_2:
                return Path_Door::DoorStates::eOpening;
            case ::Path_Door::DoorStates::eClosing_3:
                return Path_Door::DoorStates::eClosing;
        }
        ALIVE_FATAL("Bad door state");
    }

    static Path_Door::DoorTypes From(AO::Path_Door::DoorTypes type)
    {
        switch (type)
        {
            case AO::Path_Door::DoorTypes::eBasicDoor_0:
                return Path_Door::DoorTypes::eBasicDoor;
            case AO::Path_Door::DoorTypes::eTrialDoor_1:
                return Path_Door::DoorTypes::eTrialDoor;
            case AO::Path_Door::DoorTypes::eHubDoor_2:
                return Path_Door::DoorTypes::eHubDoor;
        }
        ALIVE_FATAL("Bad door type");
    }

    static Path_Door::DoorTypes From(::Path_Door::DoorTypes type)
    {
        switch (type)
        {
            case ::Path_Door::DoorTypes::eBasicDoor_0:
                return Path_Door::DoorTypes::eBasicDoor;
            case ::Path_Door::DoorTypes::eTasksDoorWithSecretMusic_2:
                return Path_Door::DoorTypes::eTasksDoorWithSecretMusic;
            case ::Path_Door::DoorTypes::eTasksDoor_3:
                return Path_Door::DoorTypes::eTasksDoor;
        }
        ALIVE_FATAL("Bad door type");
    }
};

class Path_AbeStart_Converter final
{
public:
    static Path_AbeStart From(const ::Path_AbeStart& tlv, const Guid& tlvId)
    {
        Path_AbeStart r;
        BaseConvert(r, tlv, tlvId);
        return r;
    }

    static Path_AbeStart From(const AO::Path_AbeStart& tlv, const Guid& tlvId)
    {
        Path_AbeStart r;
        BaseConvert(r, tlv, tlvId);
        return r;
    }
};

class Path_EnemyStopper_Converter final
{
public:
    static Path_EnemyStopper From(const AO::Path_EnemyStopper& tlv, const Guid& tlvId)
    {
        Path_EnemyStopper r;
        BaseConvert(r, tlv, tlvId);
        r.mStopDirection = From(tlv.mStopDirection);
        r.mSwitchId = tlv.mSwitchId;
        return r;
    }

    static Path_EnemyStopper From(const ::Path_EnemyStopper& tlv, const Guid& tlvId)
    {
        Path_EnemyStopper r;
        BaseConvert(r, tlv, tlvId);
        r.mStopDirection = From(tlv.mStopDirection);
        r.mSwitchId = tlv.mSwitchId;
        return r;
    }
private:
    static Path_EnemyStopper::StopDirection From(AO::Path_EnemyStopper::StopDirection stopDir)
    {
        switch (stopDir)
        {
            case AO::Path_EnemyStopper::StopDirection::Left_0:
                return Path_EnemyStopper::StopDirection::Left;
            case AO::Path_EnemyStopper::StopDirection::Right_1:
                return Path_EnemyStopper::StopDirection::Right;
            case AO::Path_EnemyStopper::StopDirection::Both_2:
                return Path_EnemyStopper::StopDirection::Both;
        }
        ALIVE_FATAL("Bad enemy stopper stop direction");
    }

    static Path_EnemyStopper::StopDirection From(::Path_EnemyStopper::StopDirection stopDir)
    {
        switch (stopDir)
        {
            case ::Path_EnemyStopper::StopDirection::Left_0:
                return Path_EnemyStopper::StopDirection::Left;
            case ::Path_EnemyStopper::StopDirection::Right_1:
                return Path_EnemyStopper::StopDirection::Right;
            case ::Path_EnemyStopper::StopDirection::Both_2:
                return Path_EnemyStopper::StopDirection::Both;
        }
        ALIVE_FATAL("Bad enemy stopper stop direction");
    }
};

class Path_MovingBombStopper_Converter final
{
public:
    static Path_MovingBombStopper From(const AO::Path_MovingBombStopper& tlv, const Guid& tlvId)
    {
        Path_MovingBombStopper r;
        BaseConvert(r, tlv, tlvId);
        r.mMinStopTime = tlv.mMinStopTime;
        r.mMaxStopTime = tlv.mMaxStopTime;
        return r;
    }

    static Path_MovingBombStopper From(const ::Path_MovingBombStopper& tlv, const Guid& tlvId)
    {
        Path_MovingBombStopper r;
        BaseConvert(r, tlv, tlvId);
        r.mMinStopTime = tlv.mMinStopTime;
        r.mMaxStopTime = tlv.mMaxStopTime;
        return r;
    }
};

class Path_DoorFlame_Converter final
{
public:
    static Path_DoorFlame From(const AO::Path_DoorFlame& tlv, const Guid& tlvId)
    {
        Path_DoorFlame r;
        BaseConvert(r, tlv, tlvId);
        r.mSwitchId = tlv.mSwitchId;
        r.mScale = From(tlv.mScale);
        r.mColour = From(tlv.mColour);
        return r;
    }

    static Path_DoorFlame From(const ::Path_DoorFlame& tlv, const Guid& tlvId)
    {
        Path_DoorFlame r;
        BaseConvert(r, tlv, tlvId);
        r.mSwitchId = tlv.mSwitchId;
        r.mScale = relive::From(tlv.mScale);
        return r;
    }
private:
    static reliveScale From(AO::Path_DoorFlame::Scale scale)
    {
        switch (scale)
        {
            case AO::Path_DoorFlame::Scale::eFull_0:
                return reliveScale::eFull;
            case AO::Path_DoorFlame::Scale::eHalf_1:
            case AO::Path_DoorFlame::Scale::eHalf_2:
                return reliveScale::eHalf;
        }
        ALIVE_FATAL("Bad door flame scale");
    }

    static Path_DoorFlame::Colour From(AO::Path_DoorFlame::Colour colour)
    {
        switch (colour)
        {
            case AO::Path_DoorFlame::Colour::default_0:
                return Path_DoorFlame::Colour::eDefault;
            case AO::Path_DoorFlame::Colour::red_1:
                return Path_DoorFlame::Colour::eRed;
            case AO::Path_DoorFlame::Colour::green_2:
                return Path_DoorFlame::Colour::eGreen;
            case AO::Path_DoorFlame::Colour::blue_3:
                return Path_DoorFlame::Colour::eBlue;
        }
        ALIVE_FATAL("Bad door flame colour");
    }
};

class Path_Mudokon_Converter final
{
public:
    static Path_Mudokon From(const AO::Path_Mudokon& tlv, const Guid& tlvId)
    {
        Path_Mudokon r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mJob = From(tlv.mJob);
        r.mFacing = relive::From(tlv.mFacing);
        r.mVoicePitch = tlv.mVoicePitch;
        r.mRescueSwitchId = tlv.mRescueSwitchId;
        r.mDeaf = relive::From(tlv.mDeaf);
        r.mDisabledResources = tlv.mDisabledResources;
        r.mPersistAndResetOffscreen = static_cast<reliveChoice>(tlv.mPersistAndResetOffscreen); // TODO: enum
        return r;
    }

    static Path_Mudokon From(const ::Path_Mudokon& tlv, const Guid& tlvId)
    {
        Path_Mudokon r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mJob = From(tlv.mJob);
        r.mFacing = relive::From(tlv.mFacing);
        r.mVoicePitch = tlv.mVoicePitch;
        r.mRescueSwitchId = tlv.mRescueSwitchId;
        r.mDeaf = relive::From(tlv.mDeaf);
        r.mDisabledResources = tlv.mDisabledResources;
        r.mPersistAndResetOffscreen = relive::From(tlv.mPersistAndResetOffscreen);
        r.mEmotion = From(tlv.mEmotion);
        r.mBlind = relive::From(tlv.mBlind);
        r.mAngrySwitchId = tlv.mAngrySwitchId;
        r.mWorkAfterTurningWheel = relive::From(tlv.mWorkAfterTurningWheel);
        r.mGetsDepressed = relive::From(tlv.mGetsDepressed);
        r.mRingPulseInterval = tlv.mRingPulseInterval;
        r.mGiveRingWithoutPassword = relive::From(tlv.mGiveRingWithoutPassword);
        return r;
    }
private:
    static Path_Mudokon::MudJobs From(AO::Path_Mudokon::MudJobs job)
    {
        switch (job)
        {
            case AO::Path_Mudokon::MudJobs::eStandScrub_0:
                return Path_Mudokon::MudJobs::eStandScrub;
            case AO::Path_Mudokon::MudJobs::eSitScrub_1:
                return Path_Mudokon::MudJobs::eSitScrub;
            case AO::Path_Mudokon::MudJobs::eSitChant_2:
                return Path_Mudokon::MudJobs::eSitChant;
        }
        ALIVE_FATAL("Bad mudokon job");
    }

    static Path_Mudokon::MudJobs From(::Path_Mudokon::MudJobs job)
    {
        switch (job)
        {
            case ::Path_Mudokon::MudJobs::eChisle_0:
                return Path_Mudokon::MudJobs::eChisle;
            case ::Path_Mudokon::MudJobs::eSitScrub_1:
                return Path_Mudokon::MudJobs::eSitScrub;
            case ::Path_Mudokon::MudJobs::eAngryWorker_2:
                return Path_Mudokon::MudJobs::eAngryWorker;
            case ::Path_Mudokon::MudJobs::eDamageRingGiver_3:
                return Path_Mudokon::MudJobs::eDamageRingGiver;
            case ::Path_Mudokon::MudJobs::eHealthRingGiver_4:
                return Path_Mudokon::MudJobs::eHealthRingGiver;
        }
        ALIVE_FATAL("Bad mudokon job");
    }
    
    static Path_Mudokon::Mud_TLV_Emotion From(::Path_Mudokon::Mud_TLV_Emotion emotion)
    {
        switch (emotion)
        {
            case ::Path_Mudokon::Mud_TLV_Emotion::eNormal_0:
                return Path_Mudokon::Mud_TLV_Emotion::eNormal;
            case ::Path_Mudokon::Mud_TLV_Emotion::eAngry_1:
                return Path_Mudokon::Mud_TLV_Emotion::eAngry;
            case ::Path_Mudokon::Mud_TLV_Emotion::eSad_2:
                return Path_Mudokon::Mud_TLV_Emotion::eSad;
            case ::Path_Mudokon::Mud_TLV_Emotion::eWired_3:
                return Path_Mudokon::Mud_TLV_Emotion::eWired;
            case ::Path_Mudokon::Mud_TLV_Emotion::eSick_4:
                return Path_Mudokon::Mud_TLV_Emotion::eSick;
        }
        ALIVE_FATAL("Bad mudokon emotion");
    }
};

class Path_MovingBomb_Converter final
{
public:
    static Path_MovingBomb From(const AO::Path_MovingBomb& tlv, const Guid& tlvId)
    {
        Path_MovingBomb r;
        BaseConvert(r, tlv, tlvId);
        r.mSpeed = tlv.mSpeed;
        r.mStartMovingSwitchId = tlv.mStartMovingSwitchId;
        r.mTriggeredByAlarm = relive::From(tlv.mTriggeredByAlarm);
        r.mScale = relive::From(tlv.mScale);
        r.mDisabledResources = tlv.mDisabledResources;
        r.mStartSpeed = tlv.mStartSpeed;
        r.mPersistOffscreen = relive::From(tlv.mPersistOffscreen);
        return r;
    }

    static Path_MovingBomb From(const ::Path_MovingBomb& tlv, const Guid& tlvId)
    {
        Path_MovingBomb r;
        BaseConvert(r, tlv, tlvId);
        r.mSpeed = tlv.mSpeed;
        r.mStartMovingSwitchId = tlv.mStartMovingSwitchId;
        r.mTriggeredByAlarm = relive::From(tlv.mTriggeredByAlarm);
        r.mScale = relive::From(tlv.mScale);
        r.mDisabledResources = tlv.mDisabledResources;
        r.mStartSpeed = tlv.mStartSpeed;
        r.mPersistOffscreen = relive::From(tlv.mPersistOffscreen);
        return r;
    }
};

class Path_ElumPathTrans_Converter final
{
public:
    static Path_ElumPathTrans From(const AO::Path_ElumPathTrans& tlv, const Guid& tlvId)
    {
        Path_ElumPathTrans r;
        BaseConvert(r, tlv, tlvId);
        r.mNextLevel = MapWrapper::FromAO(tlv.mNextLevel);
        r.mNextPath = tlv.mNextPath;
        r.mNextCamera = tlv.mNextCamera;
        return r;
    }
};

class Path_MudokonPathTrans_Converter final
{
public:
    static Path_MudokonPathTrans From(const AO::Path_MudokonPathTrans& tlv, const Guid& tlvId)
    {
        Path_MudokonPathTrans r;
        BaseConvert(r, tlv, tlvId);
        r.mNextLevel = MapWrapper::FromAO(tlv.mNextLevel);
        r.mNextPath = tlv.mNextPath;
        r.mNextCamera = tlv.mNextCamera;
        return r;
    }
};

class Path_SecurityClaw_Converter final
{
public:
    static Path_SecurityClaw From(const AO::Path_SecurityClaw& tlv, const Guid& tlvId)
    {
        Path_SecurityClaw r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mAlarmSwitchId = tlv.mAlarmSwitchId;
        r.mAlarmDuration = tlv.mAlarmDuration;
        r.mDisabledResources = tlv.mDisabledResources;
        return r;
    }

    // This is just a security orb in AE
    static Path_SecurityClaw From(const ::Path_SecurityClaw& tlv, const Guid& tlvId)
    {
        Path_SecurityClaw r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mDisabledResources = tlv.mDisabledResources;
        return r;
    }
};

class Path_SlingMudokon_Converter final
{
public:
    static Path_SlingMudokon From(const AO::Path_SlingMudokon& tlv, const Guid& tlvId)
    {
        Path_SlingMudokon r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mDontWhistlePassword = relive::From(tlv.mDontWhistlePassword);
        r.mCode1 = tlv.mCode1;
        r.mCode2 = tlv.mCode2;
        return r;
    }
};

class Path_FootSwitch_Converter final
{
public:
    static Path_FootSwitch From(const AO::Path_FootSwitch& tlv, const Guid& tlvId)
    {
        Path_FootSwitch r;
        BaseConvert(r, tlv, tlvId);
        r.mSwitchId = tlv.mSwitchId;
        r.mScale = relive::From(tlv.mScale);
        r.mAction = relive::From(tlv.mAction);
        r.mTriggeredBy = From(tlv.mTriggeredBy);
        return r;
    }

    static Path_FootSwitch From(const ::Path_FootSwitch& tlv, const Guid& tlvId)
    {
        Path_FootSwitch r;
        BaseConvert(r, tlv, tlvId);
        r.mSwitchId = tlv.mSwitchId;
        r.mScale = relive::From(tlv.mScale);
        r.mAction = relive::From(tlv.mAction);
        r.mTriggeredBy = From(tlv.mTriggeredBy);
        return r;
    }
private:
    static Path_FootSwitch::FootSwitchTriggerBy From(AO::Path_FootSwitch::FootSwitchTriggerBy triggerdBy)
    {
        switch (triggerdBy)
        {
            case AO::Path_FootSwitch::FootSwitchTriggerBy::eAbe_0:
                return Path_FootSwitch::FootSwitchTriggerBy::eAbe;
            case AO::Path_FootSwitch::FootSwitchTriggerBy::eAnyone_1:
                return Path_FootSwitch::FootSwitchTriggerBy::eAnyone;
        }
        ALIVE_FATAL("Bad foot switch triggered by value");
    }

    static Path_FootSwitch::FootSwitchTriggerBy From(::Path_FootSwitch::FootSwitchTriggerBy triggerdBy)
    {
        switch (triggerdBy)
        {
            case ::Path_FootSwitch::FootSwitchTriggerBy::eAbe_0:
                return Path_FootSwitch::FootSwitchTriggerBy::eAbe;
            case ::Path_FootSwitch::FootSwitchTriggerBy::eAnyone_1:
                return Path_FootSwitch::FootSwitchTriggerBy::eAnyone;
        }
        ALIVE_FATAL("Bad foot switch triggered by value");
    }
};

class Path_Paramite_Converter final
{
public:
    static Path_Paramite From(const AO::Path_Paramite& tlv, const Guid& tlvId)
    {
        Path_Paramite r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mEntranceType = From(tlv.mEnterFromWeb);
        r.mAloneChaseDelay = tlv.mAloneChaseDelay;
        r.mSurpriseWebDelayTimer = tlv.mSurpriseWebDelayTimer;
        r.mMeatEatingTime = tlv.mMeatEatingTime;
        r.mGroupChaseDelay = tlv.mGroupChaseDelay;
        r.mSurpriseWebSwitchId = tlv.mSurpriseWebSwitchId;
        r.mHissBeforeAttack = relive::From(tlv.mHissBeforeAttack);
        r.mDeleteWhenOutOfSight = relive::From(tlv.mDeleteWhenOutOfSight);
        return r;
    }

    static Path_Paramite From(const ::Path_Paramite& tlv, const Guid& tlvId)
    {
        Path_Paramite r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mEntranceType = From(tlv.mEntranceType);
        r.mAloneChaseDelay = tlv.mAloneChaseDelay;
        r.mSurpriseWebDelayTimer = tlv.mSurpriseWebDelayTimer;
        r.mMeatEatingTime = tlv.mMeatEatingTime;
        r.mGroupChaseDelay = tlv.mGroupChaseDelay;
        r.mSurpriseWebSwitchId = tlv.mSurpriseWebSwitchId;
        r.mHissBeforeAttack = relive::From(tlv.mHissBeforeAttack);
        r.mDeleteWhenOutOfSight = relive::From(tlv.mDeleteWhenOutOfSight);
        r.mAttackFleeches = relive::From(tlv.mAttackFleeches);
        return r;
    }
private:
    static Path_Paramite::EntranceType From(Choice_short choice)
    {
        switch (choice)
        {
            case Choice_short::eNo_0:
                return Path_Paramite::EntranceType::ePatrol;
            case Choice_short::eYes_1:
                return Path_Paramite::EntranceType::eSurpriseWeb;
        }
        ALIVE_FATAL("Can't convert choice to entrance type");
    }

    static Path_Paramite::EntranceType From(::Path_Paramite::EntranceType entranceType)
    {
        switch (entranceType)
        {
            case ::Path_Paramite::EntranceType::ePatrol_0:
                return Path_Paramite::EntranceType::ePatrol;
            case ::Path_Paramite::EntranceType::eSurpriseWeb_1:
                return Path_Paramite::EntranceType::eSurpriseWeb;
            case ::Path_Paramite::EntranceType::eSlightlyHigherSpawnSurpriseWeb_2:
                return Path_Paramite::EntranceType::eSlightlyHigherSpawnSurpriseWeb;
        }
        ALIVE_FATAL("Bad paramite entrance type");
    }
};

class Path_ZzzSpawner_Converter final
{
public:
    static Path_ZzzSpawner From(const AO::Path_ZzzSpawner& tlv, const Guid& tlvId)
    {
        Path_ZzzSpawner r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mSwitchId = tlv.mSwitchId;
        r.mZzzInterval = tlv.mZzzInterval;
        return r;
    }

    static Path_ZzzSpawner From(const ::Path_ZzzSpawner& tlv, const Guid& tlvId)
    {
        Path_ZzzSpawner r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mSwitchId = tlv.mSwitchId;
        r.mZzzInterval = tlv.mZzzInterval;
        return r;
    }
};

class Path_BackgroundGlukkon_Converter final
{
public:
    static Path_BackgroundGlukkon From(const AO::Path_BackgroundGlukkon& tlv, const Guid& tlvId)
    {
        Path_BackgroundGlukkon r;
        BaseConvert(r, tlv, tlvId);
        r.mScalePercent = tlv.mScalePercent;
        r.mPalId = tlv.mPalId;
        return r;
    }
};

class Path_GasEmitter_Converter final
{
public:
    static Path_GasEmitter From(const AO::Path_GasEmitter& tlv, const Guid& tlvId)
    {
        Path_GasEmitter r;
        BaseConvert(r, tlv, tlvId);
        return r;
    }

    static Path_GasEmitter From(const ::Path_GasEmitter& tlv, const Guid& tlvId)
    {
        Path_GasEmitter r;
        BaseConvert(r, tlv, tlvId);
        r.mSwitchId = tlv.mSwitchId;
        r.mColour = From(tlv.mColour);
        return r;
    }
private:
    static Path_GasEmitter::GasColour From(::Path_GasEmitter::GasColour colour)
    {
        switch (colour)
        {
            case ::Path_GasEmitter::GasColour::Yellow_0:
                return Path_GasEmitter::GasColour::eYellow;
            case ::Path_GasEmitter::GasColour::Red_1:
                return Path_GasEmitter::GasColour::eRed;
            case ::Path_GasEmitter::GasColour::Green_2:
                return Path_GasEmitter::GasColour::eGreen;
            case ::Path_GasEmitter::GasColour::Blue_3:
                return Path_GasEmitter::GasColour::eBlue;
            case ::Path_GasEmitter::GasColour::White_4:
                return Path_GasEmitter::GasColour::eWhite;
        }
        ALIVE_FATAL("Bad gas emitter gas colour");
    }
};

class Path_GasCountDown_Converter final
{
public:
    static Path_GasCountDown From(const AO::Path_GasCountDown& tlv, const Guid& tlvId)
    {
        Path_GasCountDown r;
        BaseConvert(r, tlv, tlvId);
        r.mStartTimerSwitchId = tlv.mStartTimerSwitchId;
        return r;
    }

    static Path_GasCountDown From(const ::Path_GasCountDown& tlv, const Guid& tlvId)
    {
        Path_GasCountDown r;
        BaseConvert(r, tlv, tlvId);
        r.mStartTimerSwitchId = tlv.mStartTimerSwitchId;
        r.mGasCountdownTimer = tlv.mGasCountdownTimer;
        r.mStopTimerSwitchId = tlv.mStopTimerSwitchId;
        return r;
    }
};

class Path_RingCancel_Converter final
{
public:
    static Path_RingCancel From(const AO::Path_RingCancel& tlv, const Guid& tlvId, AO::LevelIds lvlId, u32 path)
    {
        Path_RingCancel r;
        BaseConvert(r, tlv, tlvId);

        // The field field_18_bShrykull_remove was removed from the Path_RingCancel TLV because it doesnt
        // actually exist in any path data. The actual value for this field was the 2 bytes after the TLV ended
        // which is always 0 apart from in the cases below.
        // However any level saved with the legacy level editor will have this field added which is handled by checking the size.
        switch (lvlId)
        {
            case AO::LevelIds::eDesert_8: // d1.lvl
                if (path == 4)
                {
                    // original TLV data is -1 part of collision line
                    r.mRemovesShrykull = true;
                }
                break;

            case AO::LevelIds::eForestTemple_4: // f2.lvl
                if (path == 6)
                {
                    // original TLV data is 4 part of the flags of the next object
                    r.mRemovesShrykull = true;
                }
                break;
        }

        return r;
    }
};

class Path_SecurityDoor_Converter final
{
public:
    static Path_SecurityDoor From(const AO::Path_SecurityDoor& tlv, const Guid& tlvId)
    {
        Path_SecurityDoor r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mSwitchId = tlv.mSwitchId;
        r.mCode1 = tlv.mCode1;
        r.mCode2 = tlv.mCode2;
        r.mXPos = tlv.mXPos;
        r.mYPos = tlv.mYPos;
        return r;
    }

    static Path_SecurityDoor From(const ::Path_SecurityDoor& tlv, const Guid& tlvId)
    {
        Path_SecurityDoor r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mSwitchId = tlv.mSwitchId;
        r.mCode1 = tlv.mCode1;
        r.mCode2 = tlv.mCode2;
        r.mXPos = tlv.mXPos;
        r.mYPos = tlv.mYPos;
        return r;
    }
};

class Path_LiftMudokon_Converter final
{
public:
    static Path_LiftMudokon From(const AO::Path_LiftMudokon& tlv, const Guid& tlvId)
    {
        Path_LiftMudokon r;
        BaseConvert(r, tlv, tlvId);
        r.mHowFarToWalk = tlv.mHowFarToWalk;
        r.mLiftSwitchId = tlv.mLiftSwitchId;
        r.mFacing = From(tlv.mFacing);
        r.mGivePassword = relive::From(tlv.mGivePassword);
        r.mScale = relive::From(tlv.mScale);
        r.mCode1 = tlv.mCode1;
        r.mCode2 = tlv.mCode2;
        return r;
    }
private:
    static Path_LiftMudokon::Direction From(AO::Path_LiftMudokon::Direction dir)
    {
        switch (dir)
        {
            case AO::Path_LiftMudokon::Direction::eRight_0:
                return Path_LiftMudokon::Direction::eRight;
            case AO::Path_LiftMudokon::Direction::eLeft_1:
                return Path_LiftMudokon::Direction::eLeft;
        }
        ALIVE_FATAL("Bad lift mudokon direction");
    }
};

class Path_RingMudokon_Converter final
{
public:
    static Path_RingMudokon From(const AO::Path_RingMudokon& tlv, const Guid& tlvId)
    {
        Path_RingMudokon r;
        BaseConvert(r, tlv, tlvId);
        r.mFacing = relive::From(tlv.mFacing);
        r.mAbeMustFaceMud = From(tlv.mAbeMustFaceMud);
        r.mScale = relive::From(tlv.mScale);
        r.mGivePassword = relive::From(tlv.mGivePassword);
        r.mCode1 = tlv.mCode1;
        r.mCode2 = tlv.mCode2;
        r.mAction = relive::From(tlv.mAction);
        r.mRingTimeout = tlv.mRingTimeout;
        r.mGiveRingWithoutPassword = relive::From(tlv.mGiveRingWithoutPassword);
        return r;
    }
private:
    static Path_RingMudokon::MustFaceMud From(AO::Path_RingMudokon::MustFaceMud mustFaceMud)
    {
        switch (mustFaceMud)
        {
            case AO::Path_RingMudokon::MustFaceMud::eYes_0:
                return Path_RingMudokon::MustFaceMud::eYes;
            case AO::Path_RingMudokon::MustFaceMud::eNo_1:
                return Path_RingMudokon::MustFaceMud::eNo;
        }
        ALIVE_FATAL("Bad ring mudokon must face mud value");
    }
};

class Path_WellLocal_Converter final
{
public:
    static Path_WellLocal From(const AO::Path_WellLocal& tlv, const Guid& tlvId)
    {
        Path_WellLocal r;
        BaseConvert(r, tlv, tlvId);

        // Well base
        r.mScale = relive::From(tlv.mScale);
        r.mSwitchId = tlv.mSwitchId;
        r.mOtherWellId = tlv.mOtherWellId;
        r.mAnimId = tlv.mAnimId;
        r.mExitX = tlv.mExitX;
        r.mExitY = tlv.mExitY;
        r.mOffDestX = tlv.mOffLevelOrDestX.dx;

        // TODO: union
        r.mOffDestY = tlv.mOffPathOrDestY;

        // Local well
        r.mOnDestX = tlv.mOnDestX;
        r.mOnDestY = tlv.mOnDestY;
        r.mEmitLeaves = relive::From(tlv.mEmitLeaves);
        r.mLeafX = tlv.mLeafX;
        r.mLeafY = tlv.mLeafY;
        return r;
    }

    static Path_WellLocal From(const ::Path_WellLocal& tlv, const Guid& tlvId)
    {
        Path_WellLocal r;
        BaseConvert(r, tlv, tlvId);

        // Well base
        r.mScale = relive::From(tlv.mScale);
        r.mSwitchId = tlv.mSwitchId;
        r.mOtherWellId = tlv.mOtherWellId;
        r.mAnimId = tlv.mAnimId;
        r.mOffDestX = tlv.field_18_off_dx;
        r.mOffDestY = tlv.field_1A_off_dy;

        // Local well
        r.mOnDestX = tlv.mOnDestX;
        r.mOnDestY = tlv.mOnDestY;
        r.mEmitLeaves = relive::From(tlv.mEmitLeaves);
        r.mLeafX = tlv.mLeafX;
        r.mLeafY = tlv.mLeafY;
        return r;
    }
};


class Path_WellExpress_Converter final
{
public:
    static Path_WellExpress From(const AO::Path_WellExpress& tlv, const Guid& tlvId)
    {
        Path_WellExpress r;
        BaseConvert(r, tlv, tlvId);

        // Well base
        r.mScale = relive::From(tlv.mScale);
        r.mSwitchId = tlv.mSwitchId;
        r.mOtherWellId = tlv.mOtherWellId;
        r.mAnimId = tlv.mAnimId;
        r.mExitX = tlv.mExitX;
        r.mExitY = tlv.mExitY;
        r.mOffDestLevel = MapWrapper::FromAO(tlv.mOffLevelOrDestX.level);

        // TODO: union
        r.mOffDestPath = tlv.mOffPathOrDestY;

        // Express well
        r.mOffDestCamera = tlv.mOffDestCamera;
        r.mOffOtherWellId = tlv.mOffOtherWellId;
        r.mOnDestLevel = MapWrapper::FromAO(tlv.mOnDestLevel);
        r.mOnDestPath = tlv.mOnDestPath;
        r.mOnDestCamera = tlv.mOnDestCamera;
        r.mOnOtherWellId = tlv.mOnOtherWellId;
        r.mEmitLeaves = relive::From(tlv.mEmitLeaves);
        r.mLeafX = tlv.mLeafX;
        r.mLeafY = tlv.mLeafY;
        r.mMovieId = tlv.mMovieId;
        return r;
    }

    static Path_WellExpress From(const ::Path_WellExpress& tlv, const Guid& tlvId)
    {
        Path_WellExpress r;
        BaseConvert(r, tlv, tlvId);

        // Well base
        r.mScale = relive::From(tlv.mScale);
        r.mSwitchId = tlv.mSwitchId;
        r.mOtherWellId = tlv.mOtherWellId;
        r.mAnimId = tlv.mAnimId;
        r.mExitX = tlv.field_18_exit_x;
        r.mExitY = tlv.field_1A_exit_y;
        r.mOffDestLevel = MapWrapper::FromAE(tlv.field_1C_disabled_well_level);
        r.mOffDestPath = tlv.field_1E_disabled_well_path;

        // Express well
        r.mOffDestCamera = tlv.mOffDestCamera;
        r.mOffOtherWellId = tlv.mOffOtherWellId;
        r.mOnDestLevel = MapWrapper::FromAE(tlv.mOnDestLevel);
        r.mOnDestPath = tlv.mOnDestPath;
        r.mOnDestCamera = tlv.mOnDestCamera;
        r.mOnOtherWellId = tlv.mOnOtherWellId;
        r.mEmitLeaves = relive::From(tlv.mEmitLeaves);
        r.mLeafX = tlv.mLeafX;
        r.mLeafY = tlv.mLeafY;
        r.mMovieId = tlv.mMovieId;
        return r;
    }
};

class Path_Slog_Converter final
{
public:
    static Path_Slog From(const AO::Path_Slog& tlv, const Guid& tlvId)
    {
        Path_Slog r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mFacing = relive::From(tlv.mFacing);
        r.mAsleep = relive::From(tlv.mAsleep);
        r.mWakeUpAnger = tlv.mWakeUpAnger;
        r.mBarkAnger = tlv.mBarkAnger;
        r.mChaseAnger = tlv.mChaseAnger;
        r.mChaseDelay = tlv.mChaseDelay;
        r.mDisabledResources = tlv.mDisabledResources;
        r.mAngerSwitchId = tlv.mAngerSwitchId;
        return r;
    }

    static Path_Slog From(const ::Path_Slog& tlv, const Guid& tlvId)
    {
        Path_Slog r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mFacing = relive::From(tlv.mFacing);
        r.mAsleep = relive::From(tlv.mAsleep);
        r.mWakeUpAnger = tlv.mWakeUpAnger;
        r.mBarkAnger = tlv.mBarkAnger;
        r.mChaseAnger = tlv.mChaseAnger;
        r.mChaseDelay = tlv.mChaseDelay;
        r.mDisabledResources = tlv.mDisabledResources;
        r.mAngerSwitchId = tlv.mAngerSwitchId;
        r.mBoneEatingTime = tlv.mBoneEatingTime;
        return r;
    }
};

class Path_SlogSpawner_Converter final
{
public:
    static Path_SlogSpawner From(const AO::Path_SlogSpawner& tlv, const Guid& tlvId)
    {
        Path_SlogSpawner r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mMaxSlogs = tlv.mMaxSlogs;
        r.mMaxSlogsAtATime = tlv.mMaxSlogsAtATime;
        r.mStartDirection = From(tlv.mStartDirection);
        r.mSlogSpawnInterval = tlv.mSlogSpawnInterval;
        r.mSpawnerSwitchId = tlv.mSpawnerSwitchId;
        return r;
    }

    static Path_SlogSpawner From(const ::Path_SlogSpawner& tlv, const Guid& tlvId)
    {
        Path_SlogSpawner r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mMaxSlogs = tlv.mMaxSlogs;
        r.mMaxSlogsAtATime = tlv.mMaxSlogsAtATime;
        r.mStartDirection = From(tlv.mStartDirection);
        r.mSlogSpawnInterval = tlv.mSlogSpawnInterval;
        r.mSpawnerSwitchId = tlv.mSpawnerSwitchId;
        r.mListenToSligs = relive::From(tlv.mListenToSligs);
        r.mChaseDelay = tlv.mChaseDelay;
        return r;
    }
private:
    static Path_SlogSpawner::StartDirection From(AO::Path_SlogSpawner::StartDirection startDir)
    {
        switch (startDir)
        {
            case AO::Path_SlogSpawner::StartDirection::eRight_0:
                return Path_SlogSpawner::StartDirection::eRight;
            case AO::Path_SlogSpawner::StartDirection::eLeft_1:
                return Path_SlogSpawner::StartDirection::eLeft;
        }
        ALIVE_FATAL("Bad slog spawner start direction");
    }

    static Path_SlogSpawner::StartDirection From(::Path_SlogSpawner::StartDirection startDir)
    {
        switch (startDir)
        {
            case ::Path_SlogSpawner::StartDirection::eRight_0:
                return Path_SlogSpawner::StartDirection::eRight;
            case ::Path_SlogSpawner::StartDirection::eLeft_1:
                return Path_SlogSpawner::StartDirection::eLeft;
        }
        ALIVE_FATAL("Bad slog spawner start direction");
    }
};

class Path_Scrab_Converter final
{
public:
    static Path_Scrab From(const AO::Path_Scrab& tlv, const Guid& tlvId)
    {
        Path_Scrab r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mAttackDelay = tlv.mAttackDelay;
        r.mPatrolType = From(tlv.mPatrolType);
        r.mPauseLeftMin = tlv.mPauseLeftMin;
        r.mPauseLeftMax = tlv.mPauseLeftMax;
        r.mPauseRightMin = tlv.mPauseRightMin;
        r.mPauseRightMax = tlv.mPauseRightMax;
        r.mSpottingAbeDelay = tlv.mSpottingAbeDelay;
        r.mDisabledResources = tlv.mDisabledResources;
        r.mRoarRandomly = relive::From(tlv.mRoarRandomly);
        return r;
    }

    static Path_Scrab From(const ::Path_Scrab& tlv, const Guid& tlvId)
    {
        Path_Scrab r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mAttackDelay = tlv.mAttackDelay;
        r.mPatrolTypeRunOrWalkChance = tlv.mPatrolTypeRunOrWalkChance;
        r.mPauseLeftMin = tlv.mPauseLeftMin;
        r.mPauseLeftMax = tlv.mPauseLeftMax;
        r.mPauseRightMin = tlv.mPauseRightMin;
        r.mPauseRightMax = tlv.mPauseRightMax;
        r.mPauseAfterChaseTime = tlv.mPauseAfterChaseTime;
        r.mDisabledResources = tlv.mDisabledResources;
        r.mRoarRandomly = relive::From(tlv.mRoarRandomly);
        r.mPersistant = relive::From(tlv.mPersistant);
        r.mPossessedMaxWhirlAttackDuration = tlv.mPossessedMaxWhirlAttackDuration;
        r.mKillEnemy = relive::From(tlv.mKillEnemy);
        return r;
    }
private:
    static Path_Scrab::ScrabPatrolType From(AO::Path_Scrab::ScrabPatrolType type)
    {
        switch (type)
        {
            case AO::Path_Scrab::ScrabPatrolType::eWalk_0:
                return Path_Scrab::ScrabPatrolType::eWalk;
            case AO::Path_Scrab::ScrabPatrolType::eRunOrWalk192_1:
                return Path_Scrab::ScrabPatrolType::eRunOrWalk192;
            case AO::Path_Scrab::ScrabPatrolType::eRunOrWalk128_2:
                return Path_Scrab::ScrabPatrolType::eRunOrWalk128;
            case AO::Path_Scrab::ScrabPatrolType::eRunOrWalk64_3:
                return Path_Scrab::ScrabPatrolType::eRunOrWalk64;
            case AO::Path_Scrab::ScrabPatrolType::eRun_4:
                return Path_Scrab::ScrabPatrolType::eRun;
        }
        ALIVE_FATAL("Bad scrab patrol type");
    }
};

class Path_Slig_Converter final
{
public:
    static Path_Slig From(const AO::Path_Slig& tlv, const Guid& tlvId)
    {
        Path_Slig r;
        BaseConvert(r, tlv, tlvId);
        r.mData.mScale = relive::From(tlv.mScale);
        r.mData.mStartState = From(tlv.mStartState);
        r.mData.mPauseTime = tlv.mPauseTime;
        r.mData.mPauseLeftMin = tlv.mPauseLeftMin;
        r.mData.mPauseLeftMax = tlv.mPauseLeftMax;
        r.mData.mPauseRightMin = tlv.mPauseRightMin;
        r.mData.mPauseRightMax = tlv.mPauseRightMax;
        r.mData.mShootPossessedSligs = From(tlv.mShootPossessedSligs);
        r.mData.mShootOnSightDelay = tlv.mShootOnSightDelay;
        r.mData.mNumTimesToShoot = tlv.mNumTimesToShoot;
        r.mData.mCode1 = tlv.mCode1;
        r.mData.mCode2 = tlv.mCode2;
        r.mData.mChaseAbeWhenSpotted = relive::From(tlv.mChaseAbeWhenSpotted);
        r.mData.mFacing = relive::From(tlv.mFacing);
        r.mData.mPanicTimeout = tlv.mPanicTimeout;
        r.mData.mStopChaseDelay = tlv.mStopChaseDelay;
        r.mData.mTimeToWaitBeforeChase = tlv.mTimeToWaitBeforeChase;
        r.mData.mSligBoundId = tlv.mSligBoundId;
        r.mData.mAlertedListenTime = tlv.mAlertedListenTime;
        r.mData.mPercentSayWhat = tlv.mPercentSayWhat;
        r.mData.mPercentBeatMud = tlv.mPercentBeatMud;
        r.mData.mZShootDelay = tlv.mZShootDelay;
        r.mData.mStayAwake = relive::From(tlv.mStayAwake);
        r.mData.mDisabledResourcesAO = tlv.mDisabledResources;
        r.mData.mNoiseWakeUpDistance = tlv.mNoiseWakeUpDistance;
        r.mData.mSligSpawnerSwitchId = tlv.mSligSpawnerSwitchId;
        return r;
    }

    static Path_Slig From(const ::Path_Slig& tlv, const Guid& tlvId)
    {
        Path_Slig r;
        BaseConvert(r, tlv, tlvId);
        r.mData.mScale = relive::From(tlv.mScale);
        r.mData.mStartState = From(tlv.mStartState);
        r.mData.mPauseTime = tlv.mPauseTime;
        r.mData.mPauseLeftMin = tlv.mPauseLeftMin;
        r.mData.mPauseLeftMax = tlv.mPauseLeftMax;
        r.mData.mPauseRightMin = tlv.mPauseRightMin;
        r.mData.mPauseRightMax = tlv.mPauseRightMax;
        r.mData.mShootPossessedSligs = From(tlv.mShootPossessedSligs);
        r.mData.mShootOnSightDelay = tlv.mShootOnSightDelay;
        r.mData.mNumTimesToShoot = tlv.mNumTimesToShoot;
        r.mData.mCode1 = tlv.mCode1;
        r.mData.mCode2 = tlv.mCode2;
        r.mData.mChaseAbeWhenSpotted = relive::From(tlv.mChaseAbeWhenSpotted);
        r.mData.mFacing = relive::From(tlv.mFacing);
        r.mData.mPanicTimeout = tlv.mPanicTimeout;
        r.mData.mStopChaseDelay = tlv.mStopChaseDelay;
        r.mData.mTimeToWaitBeforeChase = tlv.mTimeToWaitBeforeChase;
        r.mData.mSligBoundId = tlv.mSligBoundId;
        r.mData.mAlertedListenTime = tlv.mAlertedListenTime;
        r.mData.mPercentSayWhat = tlv.mPercentSayWhat;
        r.mData.mPercentBeatMud = tlv.mPercentBeatMud;
        r.mData.mZShootDelay = tlv.mZShootDelay;
        r.mData.mStayAwake = relive::From(tlv.mStayAwake);
        r.mData.mDisabledResourcesAE = tlv.mDisabledResources;
        r.mData.mNoiseWakeUpDistance = tlv.mNoiseWakeUpDistance;
        r.mData.mSligSpawnerSwitchId = tlv.mSligSpawnerSwitchId;
        r.mData.mUnlimitedSpawns = relive::From(tlv.mUnlimitedSpawns);

        // TODO: Add flags/constants for these
        if (tlv.mDisabledResources & 0x80)
        {
            // When dead just shrink in the idle anim - happens in OG when
            // certain resources are turned off. Replicate to avoid de-syncs in playbacks
            r.mData.mDeathMode = Path_Slig_Data::DeathMode::StandIdle;
        }
        else
        {
            r.mData.mDeathMode = Path_Slig_Data::DeathMode::FallForward;
        }

        if (tlv.mDisabledResources & 0x400)
        {
            r.mData.mCanBeat = false;
        }
        else
        {
            r.mData.mCanBeat = true;
        }

        if (tlv.mDisabledResources & 0x4)
        {
            r.mData.mCanZShoot = false;
        }
        else
        {
            r.mData.mCanZShoot = true;
        }

        return r;
    }

    static Path_Slig_Data::StartState From(AO::Path_Slig::StartState startState)
    {
        switch (startState)
        {
            case AO::Path_Slig::StartState::Listening_0:
                return Path_Slig_Data::StartState::Listening;
            case AO::Path_Slig::StartState::Patrol_1:
                return Path_Slig_Data::StartState::Patrol;
            case AO::Path_Slig::StartState::Sleeping_2:
                return Path_Slig_Data::StartState::Sleeping;
            case AO::Path_Slig::StartState::Chase_3:
                return Path_Slig_Data::StartState::Chase;
            case AO::Path_Slig::StartState::ChaseAndDisappear_4:
                return Path_Slig_Data::StartState::ChaseAndDisappear;
            case AO::Path_Slig::StartState::FallingToChase_5:
                return Path_Slig_Data::StartState::eFallingToChase;
        }
        ALIVE_FATAL("Bad slig start state");
    }

    static Path_Slig_Data::StartState From(::Path_Slig::StartState startState)
    {
        switch (startState)
        {
            case ::Path_Slig::StartState::Listening_0:
                return Path_Slig_Data::StartState::Listening;
            case ::Path_Slig::StartState::Patrol_1:
                return Path_Slig_Data::StartState::Patrol;
            case ::Path_Slig::StartState::Sleeping_2:
                return Path_Slig_Data::StartState::Sleeping;
            case ::Path_Slig::StartState::Chase_3:
                return Path_Slig_Data::StartState::Chase;
            case ::Path_Slig::StartState::ChaseAndDisappear_4:
                return Path_Slig_Data::StartState::ChaseAndDisappear;
            case ::Path_Slig::StartState::ListeningToGlukkon_6:
                return Path_Slig_Data::StartState::ListeningToGlukkon;
        }
        ALIVE_FATAL("Bad slig start state");
    }

    static reliveChoice From(AO::Path_Slig::ShootPossessedSligs shootPossessedSligs)
    {
        switch (shootPossessedSligs)
        {
            case AO::Path_Slig::ShootPossessedSligs::eNo_0:
                return reliveChoice::eNo;
            case AO::Path_Slig::ShootPossessedSligs::eYes_1:
            case AO::Path_Slig::ShootPossessedSligs::eYes_2:
                return reliveChoice::eYes;
        }
        ALIVE_FATAL("Bad slig shoot possessed sligs value");
    }

    static reliveChoice From(::Path_Slig::ShootPossessedSligs shootPossessedSligs)
    {
        switch (shootPossessedSligs)
        {
            case ::Path_Slig::ShootPossessedSligs::eYes_0:
            case ::Path_Slig::ShootPossessedSligs::eYes_1:
                return reliveChoice::eYes;
            case ::Path_Slig::ShootPossessedSligs::eNo_3:
                return reliveChoice::eNo;
        }
        ALIVE_FATAL("Bad slig shoot possessed sligs value");
    }
};

class Path_SligSpawner_Converter final
{
public:
    static Path_SligSpawner From(const AO::Path_SligSpawner& tlv, const Guid& tlvId)
    {
        Path_SligSpawner r;
        BaseConvert(r, tlv, tlvId);
        r.mData.mScale = relive::From(tlv.mScale);
        r.mData.mStartState = From(tlv.mStartState);
        r.mData.mPauseTime = tlv.mPauseTime;
        r.mData.mPauseLeftMin = tlv.mPauseLeftMin;
        r.mData.mPauseLeftMax = tlv.mPauseLeftMax;
        r.mData.mPauseRightMin = tlv.mPauseRightMin;
        r.mData.mPauseRightMax = tlv.mPauseRightMax;
        r.mData.mShootPossessedSligs = From(tlv.mShootPossessedSligs);
        r.mData.mShootOnSightDelay = tlv.mShootOnSightDelay;
        r.mData.mNumTimesToShoot = tlv.mNumTimesToShoot;
        r.mData.mCode1 = tlv.mCode1;
        r.mData.mCode2 = tlv.mCode2;
        r.mData.mChaseAbeWhenSpotted = relive::From(tlv.mChaseAbeWhenSpotted);
        r.mData.mFacing = relive::From(tlv.mFacing);
        r.mData.mPanicTimeout = tlv.mPanicTimeout;
        r.mData.mStopChaseDelay = tlv.mStopChaseDelay;
        r.mData.mTimeToWaitBeforeChase = tlv.mTimeToWaitBeforeChase;
        r.mData.mSligBoundId = tlv.mSligBoundId;
        r.mData.mAlertedListenTime = tlv.mAlertedListenTime;
        r.mData.mPercentSayWhat = tlv.mPercentSayWhat;
        r.mData.mPercentBeatMud = tlv.mPercentBeatMud;
        r.mData.mZShootDelay = tlv.mZShootDelay;
        r.mData.mStayAwake = relive::From(tlv.mStayAwake);
        r.mData.mDisabledResourcesAO.Raw().all = tlv.mDisabledResources; // TODO: Fix type
        r.mData.mNoiseWakeUpDistance = tlv.mNoiseWakeUpDistance;
        r.mData.mSligSpawnerSwitchId = static_cast<s16>(tlv.mSligSpawnerSwitchId);
        return r;
    }

    static Path_SligSpawner From(const ::Path_SligSpawner& tlv, const Guid& tlvId)
    {
        Path_SligSpawner r;
        BaseConvert(r, tlv, tlvId);
        r.mData.mScale = relive::From(tlv.mScale);
        r.mData.mStartState = From(tlv.mStartState);
        r.mData.mPauseTime = tlv.mPauseTime;
        r.mData.mPauseLeftMin = tlv.mPauseLeftMin;
        r.mData.mPauseLeftMax = tlv.mPauseLeftMax;
        r.mData.mPauseRightMin = tlv.mPauseRightMin;
        r.mData.mPauseRightMax = tlv.mPauseRightMax;
        r.mData.mShootPossessedSligs = From(tlv.mShootPossessedSligs);
        r.mData.mShootOnSightDelay = tlv.mShootOnSightDelay;
        r.mData.mNumTimesToShoot = tlv.mNumTimesToShoot;
        r.mData.mCode1 = tlv.mCode1;
        r.mData.mCode2 = tlv.mCode2;
        r.mData.mChaseAbeWhenSpotted = relive::From(tlv.mChaseAbeWhenSpotted);
        r.mData.mFacing = relive::From(tlv.mFacing);
        r.mData.mPanicTimeout = tlv.mPanicTimeout;
        r.mData.mStopChaseDelay = tlv.mStopChaseDelay;
        r.mData.mTimeToWaitBeforeChase = tlv.mTimeToWaitBeforeChase;
        r.mData.mSligBoundId = tlv.mSligBoundId;
        r.mData.mAlertedListenTime = tlv.mAlertedListenTime;
        r.mData.mPercentSayWhat = tlv.mPercentSayWhat;
        r.mData.mPercentBeatMud = tlv.mPercentBeatMud;
        r.mData.mZShootDelay = tlv.mZShootDelay;
        r.mData.mStayAwake = relive::From(tlv.mStayAwake);
        r.mData.mDisabledResourcesAE = tlv.mDisabledResources;
        r.mData.mNoiseWakeUpDistance = tlv.mNoiseWakeUpDistance;
        r.mData.mSligSpawnerSwitchId = tlv.mSligSpawnerSwitchId;
        r.mData.mUnlimitedSpawns = relive::From(tlv.mUnlimitedSpawns);

        // TODO: Add flags/constants for these
        if (tlv.mDisabledResources & 0x80)
        {
            // When dead just shrink in the idle anim - happens in OG when
            // certain resources are turned off. Replicate to avoid de-syncs in playbacks
            r.mData.mDeathMode = Path_Slig_Data::DeathMode::StandIdle;
        }
        else
        {
            r.mData.mDeathMode = Path_Slig_Data::DeathMode::FallForward;
        }

        if (tlv.mDisabledResources & 0x400)
        {
            r.mData.mCanBeat = false;
        }
        else
        {
            r.mData.mCanBeat = true;
        }

         if (tlv.mDisabledResources & 0x4)
        {
            r.mData.mCanZShoot = false;
        }
        else
        {
            r.mData.mCanZShoot = true;
        }

        return r;
    }

private:
    static Path_Slig_Data::StartState From(AO::Path_Slig::StartState startState)
    {
        switch (startState)
        {
        case AO::Path_Slig::StartState::Listening_0:
                return Path_Slig_Data::StartState::Listening;
        case AO::Path_Slig::StartState::Patrol_1:
            return Path_Slig_Data::StartState::Patrol;
        case AO::Path_Slig::StartState::Sleeping_2:
            return Path_Slig_Data::StartState::Sleeping;
        case AO::Path_Slig::StartState::Chase_3:
            return Path_Slig_Data::StartState::Chase;
        case AO::Path_Slig::StartState::ChaseAndDisappear_4:
            return Path_Slig_Data::StartState::ChaseAndDisappear;
        case AO::Path_Slig::StartState::FallingToChase_5:
            return Path_Slig_Data::StartState::eFallingToChase;
        }
        ALIVE_FATAL("Bad slig start state");
    }

    static Path_Slig_Data::StartState From(::Path_Slig::StartState startState)
    {
        switch (startState)
        {
        case ::Path_Slig::StartState::Listening_0:
            return Path_Slig_Data::StartState::Listening;
        case ::Path_Slig::StartState::Patrol_1:
            return Path_Slig_Data::StartState::Patrol;
        case ::Path_Slig::StartState::Sleeping_2:
            return Path_Slig_Data::StartState::Sleeping;
        case ::Path_Slig::StartState::Chase_3:
            return Path_Slig_Data::StartState::Chase;
        case ::Path_Slig::StartState::ChaseAndDisappear_4:
            return Path_Slig_Data::StartState::ChaseAndDisappear;
        case ::Path_Slig::StartState::ListeningToGlukkon_6:
            return Path_Slig_Data::StartState::ListeningToGlukkon;
        }
        ALIVE_FATAL("Bad slig start state");
    }

    static reliveChoice From(AO::Path_Slig::ShootPossessedSligs shootPossessedSligs)
    {
        switch (shootPossessedSligs)
        {
        case AO::Path_Slig::ShootPossessedSligs::eNo_0:
            return reliveChoice::eNo;
        case AO::Path_Slig::ShootPossessedSligs::eYes_1:
        case AO::Path_Slig::ShootPossessedSligs::eYes_2:
            return reliveChoice::eYes;
        }
        ALIVE_FATAL("Bad slig shoot possessed sligs value");
    }

    static reliveChoice From(::Path_Slig::ShootPossessedSligs shootPossessedSligs)
    {
        switch (shootPossessedSligs)
        {
        case ::Path_Slig::ShootPossessedSligs::eYes_0:
        case ::Path_Slig::ShootPossessedSligs::eYes_1:
            return reliveChoice::eYes;
        case ::Path_Slig::ShootPossessedSligs::eNo_3:
            return reliveChoice::eNo;
        }
        ALIVE_FATAL("Bad slig shoot possessed sligs value");
    }
};

class Path_TrainDoor_Converter final
{
public:
    static Path_TrainDoor From(const ::Path_TrainDoor& tlv, const Guid& tlvId)
    {
        Path_TrainDoor r;
        BaseConvert(r, tlv, tlvId);
        r.mDirection = relive::From(tlv.mDirection);
        return r;
    }
};

class Path_TorturedMudokon_Converter final
{
public:
    static Path_TorturedMudokon From(const ::Path_TorturedMudokon& tlv, const Guid& tlvId)
    {
        Path_TorturedMudokon r;
        BaseConvert(r, tlv, tlvId);
        r.mKillSwitchId = tlv.mKillSwitchId;
        r.mReleaseSwitchId = tlv.mReleaseSwitchId;
        return r;
    }
};

class Path_DoorBlocker_Converter final
{
public:
    static Path_DoorBlocker From(const ::Path_DoorBlocker& tlv, const Guid& tlvId)
    {
        Path_DoorBlocker r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mSwitchId = tlv.mSwitchId;
        return r;
    }
};

class Path_GlukkonSwitch_Converter final
{
public:
    static Path_GlukkonSwitch From(const ::Path_GlukkonSwitch& tlv, const Guid& tlvId)
    {
        Path_GlukkonSwitch r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = From(tlv.mScale);
        r.mOkSwitchId = tlv.mOkSwitchId;
        r.mFailSwitchId = tlv.mFailSwitchId;
        r.mXPos = tlv.mXPos;
        r.mYPos = tlv.mYPos;
        return r;
    }
private:
    static reliveScale From(::Path_GlukkonSwitch::Scale scale)
    {
        switch (scale)
        {
            case ::Path_GlukkonSwitch::Scale::eHalf_0:
                return reliveScale::eHalf;
            case ::Path_GlukkonSwitch::Scale::eFull_1:
                return reliveScale::eFull;
        }
        ALIVE_FATAL("Bad glukkon switch scale");
    }
};

class Path_Greeter_Converter final
{
public:
    static Path_Greeter From(const ::Path_Greeter& tlv, const Guid& tlvId)
    {
        Path_Greeter r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mMotionDetectorSpeed = tlv.mMotionDetectorSpeed;
        r.mFacing = relive::From(tlv.mFacing);
        return r;
    }
};

class Path_BrewMachine_Converter final
{
public:
    static Path_BrewMachine From(const ::Path_BrewMachine& tlv, const Guid& tlvId)
    {
        Path_BrewMachine r;
        BaseConvert(r, tlv, tlvId);
        r.mBrewCount = tlv.mBrewCount;
        return r;
    }
};

class Path_Alarm_Converter final
{
public:
    static Path_Alarm From(const ::Path_Alarm& tlv, const Guid& tlvId)
    {
        Path_Alarm r;
        BaseConvert(r, tlv, tlvId);
        r.mSwitchId = tlv.mSwitchId;
        r.mAlarmDuration = tlv.mAlarmDuration;
        return r;
    }
};

class Path_ParamiteWebLine_Converter final
{
public:
    static Path_ParamiteWebLine From(const ::Path_ParamiteWebLine& tlv, const Guid& tlvId)
    {
        Path_ParamiteWebLine r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        return r;
    }
};

class Path_SlapLock_Converter final
{
public:
    static Path_SlapLock From(const ::Path_SlapLock& tlv, const Guid& tlvId)
    {
        Path_SlapLock r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mTargetTombSwitchId1 = tlv.mTargetTombSwitchId1;
        r.mTargetTombSwitchId2 = tlv.mTargetTombSwitchId2;
        r.mHasGhost = relive::From(tlv.mHasGhost);
        r.mGiveInvisibilityPowerup = relive::From(tlv.mGiveInvisibilityPowerup);
        r.mInvisibilityDuration = tlv.mInvisibilityDuration;
        r.mSlapOutputSwitchId = tlv.mSlapOutputSwitchId;
        return r;
    }
};

class Path_StatusLight_Converter final
{
public:
    static Path_StatusLight From(const ::Path_StatusLight& tlv, const Guid& tlvId)
    {
        Path_StatusLight r;
        BaseConvert(r, tlv, tlvId);
        r.mInputSwitchId = tlv.mInputSwitchId;
        r.mScale = relive::From(tlv.mScale);
        r.mLinkedStatusLightSwitchId1 = tlv.mLinkedStatusLightSwitchId1;
        r.mLinkedStatusLightSwitchId2 = tlv.mLinkedStatusLightSwitchId2;
        r.mLinkedStatusLightSwitchId3 = tlv.mLinkedStatusLightSwitchId3;
        r.mLinkedStatusLightSwitchId4 = tlv.mLinkedStatusLightSwitchId4;
        r.mLinkedStatusLightSwitchId5 = tlv.mLinkedStatusLightSwitchId5;
        r.mIgnoreGridSnapping = relive::From(tlv.mIgnoreGridSnapping);
        return r;
    }
};

class Path_MultiSwitchController_Converter final
{
public:
    static Path_MultiSwitchController From(const ::Path_MultiSwitchController& tlv, const Guid& tlvId)
    {
        Path_MultiSwitchController r;
        BaseConvert(r, tlv, tlvId);
        r.mOutputSwitchId = tlv.mOutputSwitchId;
        r.mAction = relive::From(tlv.mAction);
        r.mOnOffDelay = tlv.mOnOffDelay;
        r.mInputSwitchId1 = tlv.mInputSwitchId1;
        r.mInputSwitchId2 = tlv.mInputSwitchId2;
        r.mInputSwitchId3 = tlv.mInputSwitchId3;
        r.mInputSwitchId4 = tlv.mInputSwitchId4;
        r.mInputSwitchId5 = tlv.mInputSwitchId5;
        r.mInputSwitchId6 = tlv.mInputSwitchId6;
        return r;
    }
};

class Path_ExplosionSet_Converter final
{
public:
    static Path_ExplosionSet From(const ::Path_ExplosionSet& tlv, const Guid& tlvId)
    {
        Path_ExplosionSet r;
        BaseConvert(r, tlv, tlvId);
        r.mStartEnabled = relive::From(tlv.mStartEnabled);
        r.mSwitchId = tlv.mSwitchId;
        r.mSpawnAssets = relive::From(tlv.mSpawnAssets);
        r.mStartDelay = tlv.mStartDelay;
        r.mStartDirection = relive::From(tlv.mStartDirection);
        r.mAssetInterval = tlv.mAssetInterval;
        r.mGridSpacing = tlv.mGridSpacing;
        r.mIncreasingGridSpacing = tlv.mIncreasingGridSpacing;
        r.mScale = relive::From(tlv.mScale);
        return r;
    }
};

class Path_BoneBag_Converter final
{
public:
    static Path_BoneBag From(const ::Path_BoneBag& tlv, const Guid& tlvId)
    {
        Path_BoneBag r;
        BaseConvert(r, tlv, tlvId);
        r.mBoneFallDirection = relive::From(tlv.mBoneFallDirection);
        r.mVelX = tlv.mVelX;
        r.mVelY = tlv.mVelY;
        r.mScale = relive::From(tlv.mScale);
        r.mBoneAmount = tlv.mBoneAmount;
        return r;
    }
};

class Path_MineCar_Converter final
{
public:
    static Path_MineCar From(const ::Path_MineCar& tlv, const Guid& tlvId)
    {
        Path_MineCar r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        return r;
    }
};

class Path_ColourfulMeter_Converter final
{
public:
    static Path_ColourfulMeter From(const ::Path_ColourfulMeter& tlv, const Guid& tlvId)
    {
        Path_ColourfulMeter r;
        BaseConvert(r, tlv, tlvId);
        r.mSwitchId = tlv.mSwitchId;
        r.mNumberOfMeterBars = tlv.mNumberOfMeterBars;
        r.mMinesAlarmCountdown = tlv.mMinesAlarmCountdown;
        r.mStartFilled = relive::From(tlv.mStartFilled);
        return r;
    }
};

class Path_DemoSpawnPoint_Converter final
{
public:
    static Path_DemoSpawnPoint From(const ::Path_DemoSpawnPoint& tlv, const Guid& tlvId)
    {
        Path_DemoSpawnPoint r;
        BaseConvert(r, tlv, tlvId);
        return r;
    }
};

class Path_LevelLoader_Converter final
{
public:
    static Path_LevelLoader From(const ::Path_LevelLoader& tlv, const Guid& tlvId)
    {
        Path_LevelLoader r;
        BaseConvert(r, tlv, tlvId);
        r.mSwitchId = tlv.mSwitchId;
        r.mDestLevel = MapWrapper::FromAE(tlv.mDestLevel);
        r.mDestPath = tlv.mDestPath;
        r.mDestCamera = tlv.mDestCamera;
        r.mMovieId = tlv.mMovieId;
        return r;
    }
};

class Path_SlamDoor_Converter final
{
public:
    static Path_SlamDoor From(const ::Path_SlamDoor& tlv, const Guid& tlvId)
    {
        Path_SlamDoor r;
        BaseConvert(r, tlv, tlvId);
        r.mStartClosed = relive::From(tlv.mStartClosed);
        r.mScale = relive::From(tlv.mScale);
        r.mSwitchId = tlv.mSwitchId;
        r.mFlipY = relive::From(tlv.mFlipY);
        r.mDelete = relive::From(tlv.mDelete);
        return r;
    }
};

class Path_Slurg_Converter final
{
public:
    static Path_Slurg From(const ::Path_Slurg& tlv, const Guid& tlvId)
    {
        Path_Slurg r;
        BaseConvert(r, tlv, tlvId);
        r.mMovingTimer = tlv.mSlurgData.mMovingTimer;
        r.mFacing = relive::From(tlv.mSlurgData.mFacing);
        r.mScale = relive::From(tlv.mSlurgData.mScale);
        r.mSlurgSwitchId = tlv.mSlurgData.mSwitchId;
        return r;
    }
};

class Path_LaughingGas_Converter final
{
public:
    static Path_LaughingGas From(const ::Path_LaughingGas& tlv, const Guid& tlvId)
    {
        Path_LaughingGas r;
        BaseConvert(r, tlv, tlvId);
        r.mLaughingGas = relive::From(tlv.mLaughingGas);
        r.mLaughingGasSwitchId = tlv.mLaughingGasSwitchId;
        r.mRedPercent = tlv.mRedPercent;
        r.mGreenPercent = tlv.mGreenPercent;
        r.mBluePercent = tlv.mBluePercent;
        return r;
    }
};

class Path_WorkWheel_Converter final
{
public:
    static Path_WorkWheel From(const ::Path_WorkWheel& tlv, const Guid& tlvId)
    {
        Path_WorkWheel r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mSwitchId = tlv.mSwitchId;
        r.mActivationTime = tlv.mActivationTime;
        r.mTurnOffTime = tlv.mTurnOffTime;
        r.mTurnOffWhenStopped = relive::From(tlv.mTurnOffWhenStopped);
        return r;
    }
};

class Path_Water_Converter final
{
public:
    static Path_Water From(const ::Path_Water& tlv, const Guid& tlvId)
    {
        Path_Water r;
        BaseConvert(r, tlv, tlvId);
        r.mMaxDrops = tlv.mWaterData.mMaxDrops;
        r.mSwitchId = tlv.mWaterData.mSwitchId;
        r.mSplashTime = tlv.mWaterData.mSplashTime;
        r.mSplashVelX = tlv.mWaterData.mSplashVelX;
        r.mWaterDuration = tlv.mWaterData.mWaterDuration;
        return r;
    }
};

class Path_WheelSyncer_Converter final
{
public:
    static Path_WheelSyncer From(const ::Path_WheelSyncer& tlv, const Guid& tlvId)
    {
        Path_WheelSyncer r;
        BaseConvert(r, tlv, tlvId);
        r.mInputSwitchId1 = tlv.mInputSwitchId1;
        r.mInputSwitchId2 = tlv.mInputSwitchId2;
        r.mOutputSwitchId = tlv.mOutputSwitchId;
        r.mOutputRequirement = From(tlv.mOutputRequirement);
        r.mInputSwitchId3 = tlv.mInputSwitchId3;
        r.mInputSwitchId4 = tlv.mInputSwitchId4;
        r.mInputSwitchId5 = tlv.mInputSwitchId5;
        r.mInputSwitchId6 = tlv.mInputSwitchId6;
        return r;
    }
private:
    static Path_WheelSyncer::OutputRequirement From(::Path_WheelSyncer::WheelSyncerOutputRequirement requirement)
    {
        switch (requirement)
        {
            case ::Path_WheelSyncer::WheelSyncerOutputRequirement::eAllOn_0:
                return Path_WheelSyncer::OutputRequirement::eAllOn;
            case ::Path_WheelSyncer::WheelSyncerOutputRequirement::e1OnAnd2Off_1:
                return Path_WheelSyncer::OutputRequirement::e1OnAnd2Off;
            case ::Path_WheelSyncer::WheelSyncerOutputRequirement::e1Or2On_2:
                return Path_WheelSyncer::OutputRequirement::e1Or2On;
            case ::Path_WheelSyncer::WheelSyncerOutputRequirement::e1OnOr2Off_3:
                return Path_WheelSyncer::OutputRequirement::e1OnOr2Off;
        }
        ALIVE_FATAL("Bad wheel syncer output requirement");
    }
};

class Path_Fleech_Converter final
{
public:
    static Path_Fleech From(const ::Path_Fleech& tlv, const Guid& tlvId)
    {
        Path_Fleech r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mFacing = relive::From(tlv.mFacing);
        r.mAsleep = relive::From(tlv.mAsleep);
        r.mAttackAngerIncreaser = tlv.mAttackAngerIncreaser;
        r.mWakeUpSwitchId = tlv.mWakeUpSwitchId;
        r.mHanging = relive::From(tlv.mHanging);
        r.mLostTargetTimeout = tlv.mLostTargetTimeout;
        r.mGoesToSleep = relive::From(tlv.mGoesToSleep);
        r.mPatrolRangeInGrids = tlv.mPatrolRangeInGrids;
        r.mWakeUpSwitchAngerValue = tlv.mWakeUpSwitchAngerValue;
        r.mCanWakeUpSwitchId = tlv.mCanWakeUpSwitchId;
        r.mPersistant = relive::From(tlv.mPersistant);
        return r;
    }
};

class Path_SlurgSpawner_Converter final
{
public:
    static Path_SlurgSpawner From(const ::Path_SlurgSpawner& tlv, const Guid& tlvId)
    {
        Path_SlurgSpawner r;
        BaseConvert(r, tlv, tlvId);
        r.mSpawnInterval = tlv.mSpawnerData.mSpawnInterval;
        r.mMaxSlurgs = tlv.mSpawnerData.mMaxSlurgs;
        r.mSpawnerSwitchId = tlv.mSpawnerData.mSwitchId;

        // Path_Slurg
        r.mMovingTimer = tlv.mSlurgData.mMovingTimer;
        r.mFacing = relive::From(tlv.mSlurgData.mFacing);
        r.mScale = relive::From(tlv.mSlurgData.mScale);
        r.mSlurgSwitchId = tlv.mSlurgData.mSwitchId;
        return r;
    }
};

class Path_Drill_Converter final
{
public:
    static Path_Drill From(const ::Path_Drill& tlv, const Guid& tlvId)
    {
        Path_Drill r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mDrillData.mScale);
        r.mOnMinPauseTime = tlv.mDrillData.mOnMinPauseTime;
        r.mOnMaxPauseTime = tlv.mDrillData.mOnMaxPauseTime;
        r.mSwitchId = tlv.mDrillData.mSwitchId;
        r.mDrillBehavior = From(tlv.mDrillData.mDrillBehavior);
        r.mOnSpeed = tlv.mDrillData.mOnSpeed;
        r.mStartStateOn = relive::From(tlv.mDrillData.mStartStateOn);
        r.mOffSpeed = tlv.mDrillData.mOffSpeed;
        r.mOffMinPauseTime = tlv.mDrillData.mOffMinPauseTime;
        r.mOffMaxPauseTime = tlv.mDrillData.mOffMaxPauseTime;
        r.mStartPositionBottom = relive::From(tlv.mDrillData.mStartPositionBottom);
        r.mDrillDirection = From(tlv.mDrillData.mDirection);
        return r;
    }
private:
    static Path_Drill::DrillDirection From(::Path_Drill_Data::DrillDirection direction)
    {
        switch (direction)
        {
            case ::Path_Drill_Data::DrillDirection::eDown_0:
                return Path_Drill::DrillDirection::eDown;
            case ::Path_Drill_Data::DrillDirection::eRight_1:
                return Path_Drill::DrillDirection::eRight;
            case ::Path_Drill_Data::DrillDirection::eLeft_2:
                return Path_Drill::DrillDirection::eLeft;
        }
        ALIVE_FATAL("Bad drill direction");
    }

    static Path_Drill::DrillBehavior From(::Path_Drill_Data::DrillBehavior behavior)
    {
        switch (behavior)
        {
            case ::Path_Drill_Data::DrillBehavior::eNotInteractable_0:
                return Path_Drill::DrillBehavior::eNotInteractable;
            case ::Path_Drill_Data::DrillBehavior::eToggle_1:
                return Path_Drill::DrillBehavior::eToggle;
            case ::Path_Drill_Data::DrillBehavior::eUse_2:
                return Path_Drill::DrillBehavior::eUse;
        }
        ALIVE_FATAL("Bad drill behavior");
    }
};

class Path_Teleporter_Converter final
{
public:
    static Path_Teleporter From(const ::Path_Teleporter& tlv, const Guid& tlvId)
    {
        Path_Teleporter r;
        BaseConvert(r, tlv, tlvId);
        r.mTeleporterId = tlv.mData.mTeleporterId;
        r.mOtherTeleporterId = tlv.mData.mOtherTeleporterId;
        r.mDestCamera = tlv.mData.mDestCamera;
        r.mDestPath = tlv.mData.mDestPath;
        r.mDestLevel = MapWrapper::FromAE(tlv.mData.mDestLevel);
        r.mSwitchId = tlv.mData.mSwitchId;
        r.mScale = relive::From(tlv.mData.mScale);
        r.mWipeEffect = relive::From(tlv.mData.mWipeEffect);
        r.mMovieId = tlv.mData.mMovieId;
        r.mElectricX = tlv.mData.mElectricX;
        r.mElectricY = tlv.mData.mElectricY;
        return r;
    }
};

class Path_Glukkon_Converter final
{
public:
    static Path_Glukkon From(const ::Path_Glukkon& tlv, const Guid& tlvId)
    {
        Path_Glukkon r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mFacing = From(tlv.mFacing);
        r.mBehavior = From(tlv.mBehavior);
        r.mScreamHelpDelay = tlv.mScreamHelpDelay;
        r.mHelpSwitchId = tlv.mHelpSwitchId;
        r.mToCalmDelay = tlv.mToCalmDelay;
        r.mSpawnSwitchId = tlv.mSpawnSwitchId;
        r.mSpawnType = From(tlv.mSpawnType);
        r.mSpawnDelay = tlv.mSpawnDelay;
        r.mGlukkonType = From(tlv.mGlukkonType);
        r.mDeathSwitchId = tlv.mDeathSwitchId;
        r.mPlayMovieSwitchId = tlv.mPlayMovieSwitchId;
        r.mMovieId = tlv.mMovieId;
        return r;
    }
private:
    static Path_Glukkon::Facing From(::Path_Glukkon::Facing facing)
    {
        switch (facing)
        {
            case ::Path_Glukkon::Facing::eRight_0:
                return Path_Glukkon::Facing::eRight;
            case ::Path_Glukkon::Facing::eLeft_1:
                return Path_Glukkon::Facing::eLeft;
        }
        ALIVE_FATAL("Bad glukkon facing value");
    }

    static Path_Glukkon::Behavior From(::Path_Glukkon::Behavior behavior)
    {
        switch (behavior)
        {
            case ::Path_Glukkon::Behavior::eIgnoreWalls_0:
                return Path_Glukkon::Behavior::eIgnoreWalls;
            case ::Path_Glukkon::Behavior::eCheckForWalls_1:
                return Path_Glukkon::Behavior::eCheckForWalls;
        }
        ALIVE_FATAL("Bad glukkon behavior");
    }

    static Path_Glukkon::SpawnType From(::Path_Glukkon::SpawnType spawnType)
    {
        switch (spawnType)
        {
            case ::Path_Glukkon::SpawnType::eRegularSpawn_0:
                return Path_Glukkon::SpawnType::eRegularSpawn;
            case ::Path_Glukkon::SpawnType::eFacingLeft_1:
                return Path_Glukkon::SpawnType::eFacingLeft;
            case ::Path_Glukkon::SpawnType::eFacingRight_2:
                return Path_Glukkon::SpawnType::eFacingRight;
            case ::Path_Glukkon::SpawnType::eFullSpawnEffects_3:
                return Path_Glukkon::SpawnType::eFullSpawnEffects;
        }
        ALIVE_FATAL("Bad glukkon spawn type");
    }

    static Path_Glukkon::GlukkonTypes From(::Path_Glukkon::GlukkonTypes spawnType)
    {
        switch (spawnType)
        {
            case ::Path_Glukkon::GlukkonTypes::eNormal_0:
            case ::Path_Glukkon::GlukkonTypes::eNormal_4:
            case ::Path_Glukkon::GlukkonTypes::eNormal_5:
                return Path_Glukkon::GlukkonTypes::eNormal;
            case ::Path_Glukkon::GlukkonTypes::eStoryAslik_1:
                return Path_Glukkon::GlukkonTypes::eStoryAslik;
            case ::Path_Glukkon::GlukkonTypes::eStoryDripik_2:
                return Path_Glukkon::GlukkonTypes::eStoryDripik;
            case ::Path_Glukkon::GlukkonTypes::eStoryPhleg_3:
                return Path_Glukkon::GlukkonTypes::eStoryPhleg;
        }
        ALIVE_FATAL("Bad glukkon spawn type");
    }
};

class Path_CrawlingSligButton_Converter final
{
public:
    static Path_CrawlingSligButton From(const ::Path_CrawlingSligButton& tlv, const Guid& tlvId)
    {
        Path_CrawlingSligButton r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mSwitchId = tlv.mSwitchId;
        r.mAction = relive::From(tlv.mAction);
        r.mOnSound = From(tlv.mOnSound);
        r.mOffSound = From(tlv.mOffSound);
        r.mSoundDirection = tlv.mSoundDirection;
        return r;
    }
private:
    static Path_CrawlingSligButton::ButtonSounds From(::Path_CrawlingSligButton::CrawlingSligButtonSounds sound)
    {
        switch (sound)
        {
            case ::Path_CrawlingSligButton::CrawlingSligButtonSounds::None_0:
                return Path_CrawlingSligButton::ButtonSounds::None;
            case ::Path_CrawlingSligButton::CrawlingSligButtonSounds::SackHit_1:
                return Path_CrawlingSligButton::ButtonSounds::SackHit1;
            case ::Path_CrawlingSligButton::CrawlingSligButtonSounds::FallingItemPresence2_2:
                return Path_CrawlingSligButton::ButtonSounds::FallingItemPresence2;
            case ::Path_CrawlingSligButton::CrawlingSligButtonSounds::SecurityOrb_3:
                return Path_CrawlingSligButton::ButtonSounds::SecurityOrb;
            case ::Path_CrawlingSligButton::CrawlingSligButtonSounds::SackHit_4:
                return Path_CrawlingSligButton::ButtonSounds::SackHit2;
            case ::Path_CrawlingSligButton::CrawlingSligButtonSounds::Bullet1_5:
                return Path_CrawlingSligButton::ButtonSounds::Bullet1;
            case ::Path_CrawlingSligButton::CrawlingSligButtonSounds::AbeGenericMovement_6:
                return Path_CrawlingSligButton::ButtonSounds::AbeGenericMovement;
        }
        ALIVE_FATAL("Bad crawling slig button sound");
    }
};

class Path_FlyingSlig_Converter final
{
public:
    static Path_FlyingSlig From(const ::Path_FlyingSlig& tlv, const Guid& tlvId)
    {
        Path_FlyingSlig r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mFlyingSligData.mScale);
        r.mSpawnDelayState = From(tlv.mFlyingSligData.mSpawnDelayState);
        r.mSpawnMoveDelay = tlv.mFlyingSligData.mSpawnMoveDelay;
        r.mPatrolPauseMin = tlv.mFlyingSligData.mPatrolPauseMin;
        r.mPatrolPauseMax = tlv.mFlyingSligData.mPatrolPauseMax;
        r.mFacing = relive::From(tlv.mFlyingSligData.mFacing);
        r.mPanicDelay = tlv.mFlyingSligData.mPanicDelay;
        r.mGiveUpChaseDelay = tlv.mFlyingSligData.mGiveUpChaseDelay;
        r.mPrechaseDelay = tlv.mFlyingSligData.mPrechaseDelay;
        r.mSligBoundId = tlv.mFlyingSligData.mSligBoundId;
        r.mAlertedListenTime = tlv.mFlyingSligData.mAlertedListenTime;
        r.mSpawnerSwitchId = tlv.mFlyingSligData.mSpawnerSwitchId;
        r.mGrenadeDelay = tlv.mFlyingSligData.mGrenadeDelay;
        r.mMaxVelocity = tlv.mFlyingSligData.mMaxVelocity;
        r.mLaunchGrenadeSwitchId = tlv.mFlyingSligData.mLaunchGrenadeSwitchId;
        r.mPersistant = relive::From(tlv.mFlyingSligData.mPersistant);
        return r;
    }

    static Path_FlyingSlig::SpawnDelayStates From(::Path_FlyingSlig_Data::SpawnDelayStates state)
    {
        switch (state)
        {
            case ::Path_FlyingSlig_Data::SpawnDelayStates::eMoveImmediately_0:
                return Path_FlyingSlig::SpawnDelayStates::eMoveImmediately;
            case ::Path_FlyingSlig_Data::SpawnDelayStates::eUseCustomSpawnMoveDelay_1:
                return Path_FlyingSlig::SpawnDelayStates::eUseCustomSpawnMoveDelay;
        }
        ALIVE_FATAL("Bad flying slig spawn delay state");
    }
};

class Path_FlyingSligSpawner_Converter final
{
public:
    static Path_FlyingSligSpawner From(const ::Path_FlyingSligSpawner& tlv, const Guid& tlvId)
    {
        Path_FlyingSligSpawner r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mFlyingSligSpawnerData.mScale);
        r.mSpawnDelayState = Path_FlyingSlig_Converter::From(tlv.mFlyingSligSpawnerData.mSpawnDelayState);
        r.mSpawnMoveDelay = tlv.mFlyingSligSpawnerData.mSpawnMoveDelay;
        r.mPatrolPauseMin = tlv.mFlyingSligSpawnerData.mPatrolPauseMin;
        r.mPatrolPauseMax = tlv.mFlyingSligSpawnerData.mPatrolPauseMax;
        r.mFacing = relive::From(tlv.mFlyingSligSpawnerData.mFacing);
        r.mPanicDelay = tlv.mFlyingSligSpawnerData.mPanicDelay;
        r.mGiveUpChaseDelay = tlv.mFlyingSligSpawnerData.mGiveUpChaseDelay;
        r.mPrechaseDelay = tlv.mFlyingSligSpawnerData.mPrechaseDelay;
        r.mSligBoundId = tlv.mFlyingSligSpawnerData.mSligBoundId;
        r.mAlertedListenTime = tlv.mFlyingSligSpawnerData.mAlertedListenTime;
        r.mSpawnerSwitchId = tlv.mFlyingSligSpawnerData.mSpawnerSwitchId;
        r.mGrenadeDelay = tlv.mFlyingSligSpawnerData.mGrenadeDelay;
        r.mMaxVelocity = tlv.mFlyingSligSpawnerData.mMaxVelocity;
        r.mLaunchGrenadeSwitchId = tlv.mFlyingSligSpawnerData.mLaunchGrenadeSwitchId;
        r.mPersistant = relive::From(tlv.mFlyingSligSpawnerData.mPersistant);
        return r;
    }
};

class Path_ScrabSpawner_Converter final
{
public:
    static Path_ScrabSpawner From(const ::Path_ScrabSpawner& tlv, const Guid& tlvId)
    {
        Path_ScrabSpawner r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mAttackDelay = tlv.mAttackDelay;
        r.mPatrolTypeRunOrWalkChance = tlv.mPatrolTypeRunOrWalkChance;
        r.mPauseLeftMin = tlv.mPauseLeftMin;
        r.mPauseLeftMax = tlv.mPauseLeftMax;
        r.mPauseRightMin = tlv.mPauseRightMin;
        r.mPauseRightMax = tlv.mPauseRightMax;
        r.mPauseAfterChaseTime = tlv.mPauseAfterChaseTime;
        r.mDisabledResources = tlv.mDisabledResources;
        r.mRoarRandomly = relive::From(tlv.mRoarRandomly);
        r.mPersistant = relive::From(tlv.mPersistant);
        r.mPossessedMaxWhirlAttackDuration = tlv.mPossessedMaxWhirlAttackDuration;
        r.mKillEnemy = relive::From(tlv.mKillEnemy);

        r.mSpawnerSwitchId = tlv.mSpawnerSwitchId;
        r.mFacing = From(tlv.mFacing);
        return r;
    }
private:
    static Path_ScrabSpawner::SpawnDirection From(::Path_ScrabSpawner::ScrabSpawnDirection spawnDirection)
    {
        switch (spawnDirection)
        {
            case ::Path_ScrabSpawner::ScrabSpawnDirection::eNone_0:
                return Path_ScrabSpawner::SpawnDirection::eNone;
            case ::Path_ScrabSpawner::ScrabSpawnDirection::eLeft_1:
                return Path_ScrabSpawner::SpawnDirection::eLeft;
            case ::Path_ScrabSpawner::ScrabSpawnDirection::eRight_2:
                return Path_ScrabSpawner::SpawnDirection::eRight;
        }
        ALIVE_FATAL("Bad scrab spawner spawn direction");
    }
};

class Path_CrawlingSlig_Converter final
{
public:
    static Path_CrawlingSlig From(const ::Path_CrawlingSlig& tlv, const Guid& tlvId)
    {
        Path_CrawlingSlig r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mStartState = From(tlv.mStartState);
        r.mCrawlDirection = From(tlv.mCrawlDirection);
        r.mPanicSwitchId = tlv.mPanicSwitchId;
        r.mRespawnOnDeath = relive::From(tlv.mRespawnOnDeath);
        return r;
    }
private:
    static Path_CrawlingSlig::StartState From(::Path_CrawlingSlig::StartState startState)
    {
        switch (startState)
        {
            case ::Path_CrawlingSlig::StartState::eSleeping_0:
            case ::Path_CrawlingSlig::StartState::eSleeping_1:
                return Path_CrawlingSlig::StartState::eSleeping;
            case ::Path_CrawlingSlig::StartState::eAwake_2:
                return Path_CrawlingSlig::StartState::eAwake;
        }
        ALIVE_FATAL("Bad crawling slig start state");
    }

    static Path_CrawlingSlig::CrawlDirection From(::Path_CrawlingSlig::CrawlDirection startState)
    {
        switch (startState)
        {
            case ::Path_CrawlingSlig::CrawlDirection::eLeft_0:
                return Path_CrawlingSlig::CrawlDirection::eLeft;
            case ::Path_CrawlingSlig::CrawlDirection::eRight_1:
                return Path_CrawlingSlig::CrawlDirection::eRight;
            case ::Path_CrawlingSlig::CrawlDirection::eRandom_2:
                return Path_CrawlingSlig::CrawlDirection::eRandom;
        }
        ALIVE_FATAL("Bad crawling slig crawl direction");
    }
};

class Path_SligGetWings_Converter final
{
public:
    static Path_SligGetWings From(const ::Path_SligGetWings& tlv, const Guid& tlvId)
    {
        Path_SligGetWings r;
        BaseConvert(r, tlv, tlvId);
        r.mScale = relive::From(tlv.mScale);
        r.mSpawnDelayState = Path_FlyingSlig_Converter::From(tlv.mSpawnDelayState);
        r.mSpawnMoveDelay = tlv.mSpawnMoveDelay;
        r.mPatrolPauseMin = tlv.mPatrolPauseMin;
        r.mPatrolPauseMax = tlv.mPatrolPauseMax;
        r.mFacing = relive::From(tlv.mFacing);
        r.mPanicDelay = tlv.mPanicDelay;
        r.mGiveUpChaseDelay = tlv.mGiveUpChaseDelay;
        r.mPrechaseDelay = tlv.mPrechaseDelay;
        r.mSligBoundId = tlv.mSligBoundId;
        r.mAlertedListenTime = tlv.mAlertedListenTime;
        r.mSpawnerSwitchId = tlv.mSpawnerSwitchId;
        r.mGrenadeDelay = tlv.mGrenadeDelay;
        r.mMaxVelocity = tlv.mMaxVelocity;
        r.mLaunchGrenadeSwitchId = tlv.mLaunchGrenadeSwitchId;
        r.mPersistant = relive::From(tlv.mPersistant);
        return r;
    }
};

class Path_SligGetPants_Converter final
{
public:
    static Path_SligGetPants From(const ::Path_SligGetPants& tlv, const Guid& tlvId)
    {
        Path_SligGetPants r;
        BaseConvert(r, tlv, tlvId);
        r.mData.mScale = relive::From(tlv.Scale);
        r.mData.mStartState = Path_Slig_Converter::From(tlv.start_state);
        r.mData.mPauseTime = tlv.pause_time;
        r.mData.mPauseLeftMin = tlv.pause_left_min;
        r.mData.mPauseLeftMax = tlv.pause_left_max;
        r.mData.mPauseRightMin = tlv.pause_right_min;
        r.mData.mPauseRightMax = tlv.pause_right_max;
        r.mData.mShootPossessedSligs = Path_Slig_Converter::From(tlv.shoot_possessed_sligs);
        r.mData.mShootOnSightDelay = tlv.shoot_on_sight_delay;
        r.mData.mNumTimesToShoot = tlv.num_times_to_shoot;
        r.mData.mCode1 = tlv.code1;
        r.mData.mCode2 = tlv.code2;
        r.mData.mChaseAbeWhenSpotted = relive::From(tlv.chase_abe_when_spotted);
        r.mData.mFacing = relive::From(tlv.start_direction);
        r.mData.mPanicTimeout = tlv.panic_timeout;
        r.mData.mStopChaseDelay = tlv.stop_chase_delay;
        r.mData.mTimeToWaitBeforeChase = tlv.time_to_wait_before_chase;
        r.mData.mSligBoundId = tlv.slig_bound_id;
        r.mData.mAlertedListenTime = tlv.alerted_listen_time;
        r.mData.mPercentSayWhat = tlv.percent_say_what;
        r.mData.mPercentBeatMud = tlv.percent_beat_mud;
        r.mData.mZShootDelay = tlv.z_shoot_delay;
        r.mData.mStayAwake = relive::From(tlv.stay_awake);
        r.mData.mDisabledResourcesAE = tlv.disable_resources;
        r.mData.mNoiseWakeUpDistance = tlv.noise_wake_up_distance;
        r.mData.mSligSpawnerSwitchId = tlv.mSligSpawnerSwitchId;

        // TODO: Add flags/constants for these
        if (tlv.disable_resources & 0x80)
        {
            // When dead just shrink in the idle anim - happens in OG when
            // certain resources are turned off. Replicate to avoid de-syncs in playbacks
            r.mData.mDeathMode = Path_Slig_Data::DeathMode::StandIdle;
        }
        else
        {
            r.mData.mDeathMode = Path_Slig_Data::DeathMode::FallForward;
        }

        if (tlv.disable_resources & 0x400)
        {
            r.mData.mCanBeat = false;
        }
        else
        {
            r.mData.mCanBeat = true;
        }

        if (tlv.disable_resources & 0x4)
        {
            r.mData.mCanZShoot = false;
        }
        else
        {
            r.mData.mCanZShoot = true;
        }

        return r;
    }
};

} // namespace relive
