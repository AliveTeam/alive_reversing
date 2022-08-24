#pragma once

#include "relive_tlvs_serialization.hpp"
#include "../AliveLibAO/SecurityOrb.hpp"
#include "../AliveLibAE/SecurityOrb.hpp"
#include "../AliveLibAE/Path.hpp"
#include "../AliveLibAE/Abe.hpp"
#include "../AliveLibAO/Abe.hpp"
#include "../AliveLibAE/LiftPoint.hpp"
#include "../AliveLibAO/LiftPoint.hpp"
#include "../AliveLibAE/Dove.hpp"
#include "../AliveLibAO/Dove.hpp"
#include "../AliveLibAE/RockSack.hpp"
#include "../AliveLibAO/RockSack.hpp"
#include "../AliveLibAO/ZBall.hpp"
#include "../AliveLibAE/FallingItem.hpp"
#include "../AliveLibAO/FallingItem.hpp"
#include "../AliveLibAO/PullRingRope.hpp"
#include "../AliveLibAE/PullRingRope.hpp"
#include "../AliveLibAO/TimedMine.hpp"
#include "../AliveLibAE/TimedMine.hpp"
#include "../AliveLibAO/HoistRocksEffect.hpp"
#include "../AliveLibAO/TrapDoor.hpp"
#include "../AliveLibAE/TrapDoor.hpp"
#include "../AliveLibAO/LCDScreen.hpp"
#include "../AliveLibAE/LCDScreen.hpp"
#include "../AliveLibAE/Mine.hpp"
#include "../AliveLibAO/Mine.hpp"
#include "../AliveLibAE/InvisibleSwitch.hpp"
#include "../AliveLibAO/InvisibleSwitch.hpp"
#include "../AliveLibAE/ElectricWall.hpp"
#include "../AliveLibAO/ElectricWall.hpp"
#include "../AliveLibAE/BoomMachine.hpp"
#include "../AliveLibAO/BoomMachine.hpp"
#include "../AliveLibAE/UXB.hpp"
#include "../AliveLibAO/UXB.hpp"
#include "../AliveLibAO/MeatSaw.hpp"
#include "../AliveLibAO/Lever.hpp"
#include "../AliveLibAE/Lever.hpp"
#include "../AliveLibAE/BirdPortal.hpp"
#include "../AliveLibAO/BirdPortal.hpp"
#include "../AliveLibAO/DoorLight.hpp"
#include "../AliveLibAO/MusicTrigger.hpp"
#include "../AliveLibAE/MusicTrigger.hpp"
#include "../AliveLibAO/LiftMover.hpp"
#include "../AliveLibAE/LiftMover.hpp"
#include "../AliveLibAO/HintFly.hpp"
#include "../AliveLibAO/TimerTrigger.hpp"
#include "../AliveLibAE/TimerTrigger.hpp"
#include "../AliveLibAO/FlintLockFire.hpp"
#include "../AliveLibAO/HoneySack.hpp"
#include "../AliveLibAO/Bat.hpp"
#include "../AliveLibAO/RollingBallStopper.hpp"
#include "../AliveLibAO/RollingBall.hpp"
#include "../AliveLibAE/MotionDetector.hpp"
#include "../AliveLibAO/MotionDetector.hpp"
#include "../AliveLibAO/BellHammer.hpp"
#include "../AliveLibAO/Slig.hpp"
#include "../AliveLibAE/Slig.hpp"
#include "../AliveLibAO/BackgroundAnimation.hpp"
#include "../AliveLibAE/BackgroundAnimation.hpp"
#include "../AliveLibAE/MainMenu.hpp"
#include "../AliveLibAO/MainMenu.hpp"
#include "../AliveLibAO/Elum.hpp"
#include "../AliveLibAO/Scrab.hpp"
#include "../AliveLibAE/Scrab.hpp"
#include "../AliveLibAO/HoneyDrip.hpp"
#include "../AliveLibAO/ChimeLock.hpp"
#include "../AliveLibAO/LCDStatusBoard.hpp"
#include "../AliveLibAE/LCDStatusBoard.hpp"
#include "../AliveLibAE/CreditsController.hpp"
#include "../AliveLibAO/CreditsController.hpp"
#include "../AliveLibAE/Meat.hpp"
#include "../AliveLibAO/Meat.hpp"
#include "../AliveLibAO/Honey.hpp"
#include "../AliveLibAO/BeeSwarmHole.hpp"
#include "../AliveLibAE/Door.hpp"
#include "../AliveLibAO/Door.hpp"
#include "../AliveLibAO/MovingBomb.hpp"
#include "../AliveLibAE/MovingBomb.hpp"
#include "../AliveLibAO/DoorFlame.hpp"
#include "../AliveLibAE/DoorFlame.hpp"
#include "../AliveLibAE/Mudokon.hpp"
#include "../AliveLibAO/Mudokon.hpp"
#include "../AliveLibAO/SecurityClaw.hpp"
#include "../AliveLibAO/SlingMudokon.hpp"
#include "../AliveLibAE/FootSwitch.hpp"
#include "../AliveLibAO/FootSwitch.hpp"
#include "../AliveLibAO/Paramite.hpp"
#include "../AliveLibAE/Paramite.hpp"
#include "../AliveLibAO/ZzzSpawner.hpp"
#include "../AliveLibAE/ZzzSpawner.hpp"
#include "../AliveLibAO/BackgroundGlukkon.hpp"
#include "../AliveLibAO/GasEmitter.hpp"
#include "../AliveLibAE/GasEmitter.hpp"
#include "../AliveLibAE/GasCountDown.hpp"
#include "../AliveLibAO/GasCountDown.hpp"
#include "../AliveLibAE/SecurityDoor.hpp"
#include "../AliveLibAO/SecurityDoor.hpp"
#include "../AliveLibAO/Well.hpp"
#include "../AliveLibAO/Slog.hpp"
#include "../AliveLibAE/Slog.hpp"
#include "../AliveLibAO/SlogSpawner.hpp"
#include "../AliveLibAE/SlogSpawner.hpp"
#include "../AliveLibAO/SligSpawner.hpp"
#include "../AliveLibAE/SligSpawner.hpp"
#include "../AliveLibAE/TorturedMudokon.hpp"
#include "../AliveLibAE/DoorBlocker.hpp"
#include "../AliveLibAE/GlukkonSwitch.hpp"
#include "../AliveLibAE/Greeter.hpp"
#include "../AliveLibAE/BrewMachine.hpp"
#include "../AliveLibAE/ParamiteWebLine.hpp"
#include "../AliveLibAE/SlapLock.hpp"
#include "../AliveLibAE/StatusLight.hpp"
#include "../AliveLibAE/MultiSwitchController.hpp"
#include "../AliveLibAE/ExplosionSet.hpp"
#include "../AliveLibAE/Bone.hpp"
#include "../AliveLibAE/MineCar.hpp"
#include "../AliveLibAE/ColourfulMeter.hpp"
#include "../AliveLibAE/DemoPlayback.hpp"
#include "../AliveLibAE/LevelLoader.hpp"
#include "../AliveLibAE/SlamDoor.hpp"
#include "../AliveLibAE/Slurg.hpp"
#include "../AliveLibAE/LaughingGas.hpp"
#include "../AliveLibAE/WorkWheel.hpp"
#include "../AliveLibAE/Water.hpp"
#include "../AliveLibAE/WheelSyncer.hpp"
#include "../AliveLibAE/Fleech.hpp"
#include "../AliveLibAE/SlurgSpawner.hpp"
#include "../AliveLibAE/Drill.hpp"
#include "../AliveLibAE/Glukkon.hpp"
#include "../AliveLibAE/CrawlingSligButton.hpp"
#include "../AliveLibAE/ScrabSpawner.hpp"
#include "../AliveLibAE/CrawlingSlig.hpp"

// Convert an AO or AE TLV to a relive TLV

namespace relive {
    inline void BaseConvert(relive::Path_TLV& r, const AO::Path_TLV& base)
    {
        r.mWidth = base.mBottomRight.x - base.mTopLeft.x;
        r.mHeight = base.mBottomRight.y - base.mTopLeft.y;
        r.mTopLeftX = base.mTopLeft.x;
        r.mTopLeftY = base.mTopLeft.y;
        r.mBottomRightX = base.mBottomRight.x;
        r.mBottomRightY = base.mBottomRight.y;
        r.mTlvSpecificMeaning = base.mTlvSpecificMeaning;
        r.mTlvType = static_cast<s32>(base.mTlvType32.mType);
        r.mTlvFlags.Raw().all = base.mTlvFlags.Raw().all;
    }

    inline void BaseConvert(relive::Path_TLV& r, const ::Path_TLV& base)
    {
        r.mWidth = base.mBottomRight.x - base.mTopLeft.x;
        r.mHeight = base.mBottomRight.y - base.mTopLeft.y;
        r.mTopLeftX = base.mTopLeft.x;
        r.mTopLeftY = base.mTopLeft.y;
        r.mBottomRightX = base.mBottomRight.x;
        r.mBottomRightY = base.mBottomRight.y;
        r.mTlvSpecificMeaning = base.mTlvSpecificMeaning;
        r.mTlvType = static_cast<s32>(base.mTlvType32.mType);
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

    static reliveSwitchOp From(const AO::SwitchOp op)
    {
        switch (op)
        {
            case AO::SwitchOp::eSetTrue_0:
                return reliveSwitchOp::eSetTrue;
            case AO::SwitchOp::eSetFalse_1:
                return reliveSwitchOp::eSetFalse;
            case AO::SwitchOp::eToggle_2:
                return reliveSwitchOp::eToggle;
            case AO::SwitchOp::eIncrement_3:
                return reliveSwitchOp::eIncrement;
            case AO::SwitchOp::eDecrement_4:
                return reliveSwitchOp::eDecrement;
        }
        ALIVE_FATAL("Bad switch operator");
    }

    static reliveSwitchOp From(const ::SwitchOp op)
    {
        switch (op)
        {
        case ::SwitchOp::eSetTrue_0:
            return reliveSwitchOp::eSetTrue;
        case ::SwitchOp::eSetFalse_1:
            return reliveSwitchOp::eSetFalse;
        case ::SwitchOp::eToggle_2:
            return reliveSwitchOp::eToggle;
        case ::SwitchOp::eIncrement_3:
            return reliveSwitchOp::eIncrement;
        case ::SwitchOp::eDecrement_4:
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
    static Path_ShadowZone From(const AO::Path_ShadowZone& tlv)
    {
        Path_ShadowZone r;
        BaseConvert(r, tlv);
        r.mRGB.SetRGB(tlv.field_1C_r, tlv.field_1E_g, tlv.field_20_b);
        r.mScale = From(tlv.mScale);
        return r;
    }

    static Path_ShadowZone From(const ::Path_ShadowZone& tlv)
    {
        Path_ShadowZone r;
        BaseConvert(r, tlv);
        r.mRGB.SetRGB(tlv.field_14_r, tlv.field_16_g, tlv.field_18_b);
        r.mScale = From(tlv.field_1C_scale);
        return r;
    }

private:
    static Path_ShadowZone::Scale From(::ShadowZoneScale aeScale)
    {
        switch (aeScale)
        {
            case ::ShadowZoneScale::eHalf_1:
                return Path_ShadowZone::Scale::eHalf;
            case ::ShadowZoneScale::eFull_2:
                return Path_ShadowZone::Scale::eFull;
            case ::ShadowZoneScale::eBoth_0:
                return Path_ShadowZone::Scale::eBoth;
        }
        ALIVE_FATAL("Bad AE shadow scale");
    }

    static Path_ShadowZone::Scale From(AO::ShadowZoneScale aoScale)
    {
        switch (aoScale)
        {
            case AO::ShadowZoneScale::eHalf_1:
                return Path_ShadowZone::Scale::eHalf;
            case AO::ShadowZoneScale::eFull_2:
                return Path_ShadowZone::Scale::eFull;
            case AO::ShadowZoneScale::eBoth_0:
                return Path_ShadowZone::Scale::eBoth;
        }
        ALIVE_FATAL("Bad AO shadow scale");
    }
};

class Path_SecurityOrb_Converter final
{
public:
    static Path_SecurityOrb From(const AO::Path_SecurityOrb& tlv)
    {
        Path_SecurityOrb r;
        BaseConvert(r, tlv);
        r.mScale = relive::From(tlv.mScale);
        r.mDisabledResources = tlv.mDisabledResources;
        return r;
    }

    static Path_SecurityOrb From(const ::Path_SecurityOrb& tlv)
    {
        Path_SecurityOrb r;
        BaseConvert(r, tlv);
        r.mScale = relive::From(tlv.mScale);
        r.mDisabledResources = tlv.mDisabledResources;
        return r;
    }
};

class Path_ContinuePoint_Converter final
{
public:
    static Path_ContinuePoint From(const AO::Path_ContinuePoint& tlv)
    {
        Path_ContinuePoint r;
        BaseConvert(r, tlv);
        r.mZoneNumber = tlv.mZoneNumber;
        r.mClearFromId = tlv.mClearFromId;
        r.mClearToId = tlv.mClearToId;
        r.mElumRestarts = relive::From(tlv.mElumRestarts);
        r.mAbeSpawnDir = From(tlv.mAbeSpawnDir);
        return r;
    }

    static Path_ContinuePoint From(const ::Path_ContinuePoint& tlv)
    {
        Path_ContinuePoint r;
        BaseConvert(r, tlv);
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
    static Path_LiftPoint From(const AO::Path_LiftPoint& tlv)
    {
        Path_LiftPoint r;
        BaseConvert(r, tlv);
        r.mLiftPointId = tlv.mLiftPointId;
        r.mIsStartPoint = relive::From(tlv.mIsStartPoint);
        r.mLiftPointStopType = From(tlv.mLiftPointStopType);
        r.mScale = relive::From(tlv.mScale);
        r.mIgnoreLiftMover = relive::From(tlv.mIgnoreLiftMover);
        return r;
    }

    static Path_LiftPoint From(const ::Path_LiftPoint& tlv)
    {
        Path_LiftPoint r;
        BaseConvert(r, tlv);
        r.mLiftPointId = tlv.mLiftPointId;
        r.mIsStartPoint = relive::From(tlv.mIsStartPoint);
        r.mLiftPointStopType = From(tlv.mLiftPointStopType);
        r.mScale = relive::From(tlv.mScale);
        r.mIgnoreLiftMover = relive::From(tlv.mIgnoreLiftMover);
        return r;
    }

private:
    static relive::Path_LiftPoint::LiftPointStopType From(AO::LiftPointStopType stopType)
    {
        switch (stopType)
        {
            case AO::LiftPointStopType::eTopFloor_0:
                return relive::Path_LiftPoint::LiftPointStopType::eTopFloor;
            case AO::LiftPointStopType::eBottomFloor_1:
                return relive::Path_LiftPoint::LiftPointStopType::eBottomFloor;
            case AO::LiftPointStopType::eMiddleFloor_2:
                return relive::Path_LiftPoint::LiftPointStopType::eMiddleFloor;
            case AO::LiftPointStopType::eMiddleLockFloor_3:
                return relive::Path_LiftPoint::LiftPointStopType::eMiddleLockFloor;
            case AO::LiftPointStopType::eStartPointOnly_4:
                return relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly;
        }
        ALIVE_FATAL("Bad lift point stop type");
    }

    static relive::Path_LiftPoint::LiftPointStopType From(::LiftPointStopType stopType)
    {
        switch (stopType)
        {
            case ::LiftPointStopType::eTopFloor_0:
                return relive::Path_LiftPoint::LiftPointStopType::eTopFloor;
            case ::LiftPointStopType::eBottomFloor_1:
                return relive::Path_LiftPoint::LiftPointStopType::eBottomFloor;
            case ::LiftPointStopType::eMiddleFloor_2:
                return relive::Path_LiftPoint::LiftPointStopType::eMiddleFloor;
            case ::LiftPointStopType::eMiddleLockFloor_3:
                return relive::Path_LiftPoint::LiftPointStopType::eMiddleLockFloor;
            case ::LiftPointStopType::eStartPointOnly_4:
                return relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly;
        }
        ALIVE_FATAL("Bad lift point stop type");
    }
};

class Path_Dove_Converter final
{
public:
    static Path_Dove From(const AO::Path_Dove& tlv)
    {
        Path_Dove r;
        BaseConvert(r, tlv);
        r.mDoveCount = tlv.mDoveCount;
        r.mPixelPerfect = relive::From(tlv.mPixelPerfect);
        r.mScale = relive::From(tlv.mScale);
        return r;
    }

    static Path_Dove From(const ::Path_Dove& tlv)
    {
        Path_Dove r;
        BaseConvert(r, tlv);
        r.mDoveCount = tlv.mDoveCount;
        r.mPixelPerfect = relive::From(tlv.mPixelPerfect);
        r.mScale = relive::From(tlv.mScale);
        return r;
    }
};

class Path_RockSack_Converter final
{
public:
    static Path_RockSack From(const AO::Path_RockSack& tlv)
    {
        Path_RockSack r;
        BaseConvert(r, tlv);
        r.mRockFallDirection = relive::From(tlv.mRockFallDirection);
        r.mVelX = tlv.mVelX;
        r.mVelY = tlv.mVelY;
        r.mScale = relive::From(tlv.mScale);
        r.mRockAmount = tlv.mRockAmount;
        return r;
    }

    static Path_RockSack From(const ::Path_RockSack& tlv)
    {
        Path_RockSack r;
        BaseConvert(r, tlv);
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
    static Path_ZBall From(const AO::Path_ZBall& tlv)
    {
        Path_ZBall r;
        BaseConvert(r, tlv);
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
    static Path_FallingItem From(const AO::Path_FallingItem& tlv)
    {
        Path_FallingItem r;
        BaseConvert(r, tlv);
        r.mSwitchId = tlv.mSwitchId;
        r.mScale = relive::From(tlv.mScale);
        r.mFallInterval = tlv.mFallInterval;
        r.mMaxFallingItems = tlv.mMaxFallingItems;
        r.mResetSwitchIdAfterUse = relive::From(tlv.mResetSwitchIdAfterUse);
        return r;
    }

    static Path_FallingItem From(const ::Path_FallingItem& tlv)
    {
        Path_FallingItem r;
        BaseConvert(r, tlv);
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
    static Path_PullRingRope From(const AO::Path_PullRingRope& tlv)
    {
        Path_PullRingRope r;
        BaseConvert(r, tlv);
        r.mSwitchId = tlv.mSwitchId;
        r.mAction = relive::From(tlv.mAction);
        r.mRopeLength = tlv.mRopeLength;
        r.mScale = relive::From(tlv.mScale);
        r.mOnSound = From(tlv.mOnSound);
        r.mOffSound = From(tlv.mOffSound);
        r.mSoundDirection = From(tlv.mSoundDirection);
        return r;
    }

    static Path_PullRingRope From(const ::Path_PullRingRope& tlv)
    {
        Path_PullRingRope r;
        BaseConvert(r, tlv);
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
    static relive::Path_PullRingRope::PullRingSwitchSound From(AO::PullRingSwitchSound switchSound)
    {
        switch (switchSound)
        {
            case AO::PullRingSwitchSound::eNone_0:
                return relive::Path_PullRingRope::PullRingSwitchSound::eNone;
            case AO::PullRingSwitchSound::eWellExit_1:
                return relive::Path_PullRingRope::PullRingSwitchSound::eWellExit;
            case AO::PullRingSwitchSound::RingBellHammer_2:
                return relive::Path_PullRingRope::PullRingSwitchSound::RingBellHammer;
            case AO::PullRingSwitchSound::eDoorEffect_3:
                return relive::Path_PullRingRope::PullRingSwitchSound::eDoorEffect;
        }
        ALIVE_FATAL("Bad pull ring switch sound");
    }

    static relive::Path_PullRingRope::PullRingSoundDirection From(AO::PullRingSoundDirection soundDir)
    {
        switch (soundDir)
        {
            case AO::PullRingSoundDirection::eLeftAndRight_0:
                return relive::Path_PullRingRope::PullRingSoundDirection::eLeftAndRight;
            case AO::PullRingSoundDirection::eLeft_1:
                return relive::Path_PullRingRope::PullRingSoundDirection::eLeft;
            case AO::PullRingSoundDirection::eRight_2:
                return relive::Path_PullRingRope::PullRingSoundDirection::eRight;
        }
        ALIVE_FATAL("Bad pull ring sound direction");
    }

    static relive::Path_PullRingRope::PullRingSwitchSound From(::PullRingSwitchSound switchSound)
    {
        switch (switchSound)
        {
        case ::PullRingSwitchSound::eNone_0:
            return relive::Path_PullRingRope::PullRingSwitchSound::eNone;
        case ::PullRingSwitchSound::eWellExit_1:
            return relive::Path_PullRingRope::PullRingSwitchSound::eWellExit;
        case ::PullRingSwitchSound::eRingBellHammer_2:
            return relive::Path_PullRingRope::PullRingSwitchSound::RingBellHammer;
        case ::PullRingSwitchSound::eDoorEffect_3:
            return relive::Path_PullRingRope::PullRingSwitchSound::eDoorEffect;
        }
        ALIVE_FATAL("Bad pull ring switch sound");
    }

    static relive::Path_PullRingRope::PullRingSoundDirection From(::PullRingSoundDirection soundDir)
    {
        switch (soundDir)
        {
        case ::PullRingSoundDirection::eLeftAndRight_0:
            return relive::Path_PullRingRope::PullRingSoundDirection::eLeftAndRight;
        case ::PullRingSoundDirection::eLeft_1:
            return relive::Path_PullRingRope::PullRingSoundDirection::eLeft;
        case ::PullRingSoundDirection::eRight_2:
            return relive::Path_PullRingRope::PullRingSoundDirection::eRight;
        }
        ALIVE_FATAL("Bad pull ring sound direction");
    }
};

class Path_TimedMine_Converter final
{
public:
    static Path_TimedMine From(const AO::Path_TimedMine& tlv)
    {
        Path_TimedMine r;
        BaseConvert(r, tlv);
        r.mSwitchId = tlv.mSwitchId;
        r.mState = tlv.mState;
        r.mScale = relive::From(tlv.mScale);
        r.mTicksUntilExplosion = tlv.mTicksUntilExplosion;
        r.mDisabledResources = tlv.mDisabledResources;
        return r;
    }

    static Path_TimedMine From(const ::Path_TimedMine& tlv)
    {
        Path_TimedMine r;
        BaseConvert(r, tlv);
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
    static Path_Hoist From(const AO::Path_Hoist& tlv)
    {
        Path_Hoist r;
        BaseConvert(r, tlv);
        r.mHoistType = From(tlv.mHoistType);
        r.mGrabDirection = From(tlv.mGrabDirection);
        return r;
    }

    static Path_Hoist From(const ::Path_Hoist& tlv)
    {
        Path_Hoist r;
        BaseConvert(r, tlv);
        r.mHoistType = From(tlv.mHoistType);
        r.mGrabDirection = From(tlv.mGrabDirection);
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
    static Path_TrapDoor From(const AO::Path_TrapDoor& tlv)
    {
        Path_TrapDoor r;
        BaseConvert(r, tlv);
        r.mSwitchId = tlv.mSwitchId;
        r.mStartState = From(tlv.mStartState);
        r.mSelfClosing = relive::From(tlv.mSelfClosing);
        r.mScale = relive::From(tlv.mScale);
        r.mDirection = relive::From(tlv.mDirection);
        r.mXOff = tlv.mXOff;
        return r;
    }

    static Path_TrapDoor From(const ::Path_TrapDoor& tlv)
    {
        Path_TrapDoor r;
        BaseConvert(r, tlv);
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
    static Path_LCDScreen From(const AO::Path_LCDScreen& tlv)
    {
        Path_LCDScreen r;
        BaseConvert(r, tlv);
        r.mMessageId1 = tlv.mMessageId1;
        r.mMessageRandMinId = tlv.mMessageRandMinId;
        r.mMessageRandMaxId = tlv.mMessageRandMaxId;
        return r;
    }

    static Path_LCDScreen From(const ::Path_LCDScreen& tlv)
    {
        Path_LCDScreen r;
        BaseConvert(r, tlv);
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
    static Path_Mine From(const AO::Path_Mine& tlv)
    {
        Path_Mine r;
        BaseConvert(r, tlv);
        r.mScale = relive::From(tlv.mScale);
        r.mDisabledResources = tlv.mDisabledResources;
        r.mPersistOffscreen = relive::From(tlv.mPersistOffscreen);
        return r;
    }

    static Path_Mine From(const ::Path_Mine& tlv)
    {
        Path_Mine r;
        BaseConvert(r, tlv);
        r.mScale = relive::From(tlv.mScale);
        r.mDisabledResources = tlv.mDisabledResources;
        r.mPersistOffscreen = relive::From(tlv.mPersistOffscreen);
        return r;
    }
};

class Path_InvisibleSwitch_Converter final
{
public:
    static Path_InvisibleSwitch From(const AO::Path_InvisibleSwitch& tlv)
    {
        Path_InvisibleSwitch r;
        BaseConvert(r, tlv);
        r.mSwitchId = tlv.mSwitchId;
        r.mAction = relive::From(tlv.mAction);
        r.mActivationDelay = tlv.mActivationDelay;
        r.mSetOffAlarm = relive::From(tlv.mSetOffAlarm);
        r.mScale = From(tlv.mScale);
        return r;
    }

    static Path_InvisibleSwitch From(const ::Path_InvisibleSwitch& tlv)
    {
        Path_InvisibleSwitch r;
        BaseConvert(r, tlv);
        r.mSwitchId = tlv.mSwitchId;
        r.mAction = relive::From(tlv.mAction);
        r.mActivationDelay = tlv.mActivationDelay;
        r.mSetOffAlarm = relive::From(tlv.mSetOffAlarm);
        r.mScale = From(tlv.mScale);
        return r;
    }

private:
    static relive::Path_InvisibleSwitch::InvisibleSwitchScale From(AO::InvisibleSwitchScale scale)
    {
        switch (scale)
        {
            case AO::InvisibleSwitchScale::eHalf_0:
                return relive::Path_InvisibleSwitch::InvisibleSwitchScale::eHalf;
            case AO::InvisibleSwitchScale::eFull_1:
                return relive::Path_InvisibleSwitch::InvisibleSwitchScale::eFull;
            case AO::InvisibleSwitchScale::eAny_2:
                return relive::Path_InvisibleSwitch::InvisibleSwitchScale::eAny;
        }
        ALIVE_FATAL("Bad invisible switch scale");
    }

    static relive::Path_InvisibleSwitch::InvisibleSwitchScale From(::InvisibleSwitchScale scale)
    {
        switch (scale)
        {
            case ::InvisibleSwitchScale::eHalf_0:
                return relive::Path_InvisibleSwitch::InvisibleSwitchScale::eHalf;
            case ::InvisibleSwitchScale::eFull_1:
                return relive::Path_InvisibleSwitch::InvisibleSwitchScale::eFull;
            case ::InvisibleSwitchScale::eAny_2:
                return relive::Path_InvisibleSwitch::InvisibleSwitchScale::eAny;
        }
        ALIVE_FATAL("Bad invisible switch scale");
    }
};

class Path_ElectricWall_Converter final
{
public:
    static Path_ElectricWall From(const AO::Path_ElectricWall& tlv)
    {
        Path_ElectricWall r;
        BaseConvert(r, tlv);
        r.mScale = relive::From(tlv.mScale);
        r.mSwitchId = tlv.mSwitchId;
        r.mStartState = From(tlv.mStartState);
        return r;
    }

    static Path_ElectricWall From(const ::Path_ElectricWall& tlv)
    {
        Path_ElectricWall r;
        BaseConvert(r, tlv);
        r.mScale = relive::From(tlv.mScale);
        r.mSwitchId = tlv.mSwitchId;
        r.mStartState = From(tlv.mStartState);
        return r;
    }

private:
    static relive::Path_ElectricWall::ElectricWallStartState From(AO::ElectricWallStartState startState)
    {
        switch (startState)
        {
            case AO::ElectricWallStartState::eOff_0:
                return relive::Path_ElectricWall::ElectricWallStartState::eOff;
            case AO::ElectricWallStartState::eOn_1:
                return relive::Path_ElectricWall::ElectricWallStartState::eOn;
        }
        ALIVE_FATAL("Bad electric wall start state");
    }

    static relive::Path_ElectricWall::ElectricWallStartState From(::ElectricWallStartState startState)
    {
        switch (startState)
        {
            case ::ElectricWallStartState::eOff_0:
                return relive::Path_ElectricWall::ElectricWallStartState::eOff;
            case ::ElectricWallStartState::eOn_1:
                return relive::Path_ElectricWall::ElectricWallStartState::eOn;
        }
        ALIVE_FATAL("Bad electric wall start state");
    }
};

class Path_BoomMachine_Converter final
{
public:
    static Path_BoomMachine From(const AO::Path_BoomMachine& tlv)
    {
        Path_BoomMachine r;
        BaseConvert(r, tlv);
        r.mScale = relive::From(tlv.mScale);
        r.mNozzleSide = From(tlv.mNozzleSide);
        r.mDisabledResources = tlv.mDisabledResources;
        r.mGrenadeAmount = tlv.mGrenadeAmount;
        return r;
    }

    static Path_BoomMachine From(const ::Path_BoomMachine& tlv)
    {
        Path_BoomMachine r;
        BaseConvert(r, tlv);
        r.mScale = relive::From(tlv.field_10_scale);
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
    static Path_UXB From(const AO::Path_UXB& tlv)
    {
        Path_UXB r;
        BaseConvert(r, tlv);
        r.mPatternLength = tlv.mPatternLength;
        r.mPattern = tlv.mPattern;
        r.mScale = relive::From(tlv.mScale);
        r.mStartState = From(tlv.mStartState);
        r.mDisabledResources = tlv.mDisabledResources;
        return r;
    }

    static Path_UXB From(const ::Path_UXB& tlv)
    {
        Path_UXB r;
        BaseConvert(r, tlv);
        r.mPatternLength = tlv.mPatternLength;
        r.mPattern = tlv.mPattern;
        r.mScale = relive::From(tlv.mScale);
        r.mStartState = From(tlv.mStartState);
        r.mDisabledResources = tlv.mDisabledResources;
        return r;
    }

private:
    static relive::Path_UXB::StartState From(AO::UXBStartState startState)
    {
        switch (startState)
        {
        case AO::UXBStartState::eOn:
            return relive::Path_UXB::StartState::eOn;
        case AO::UXBStartState::eOff:
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
    static Path_MeatSaw From(const AO::Path_MeatSaw& tlv)
    {
        Path_MeatSaw r;
        BaseConvert(r, tlv);
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
    static Path_Lever From(const AO::Path_Lever& tlv)
    {
        Path_Lever r;
        BaseConvert(r, tlv);
        r.mAction = relive::From(tlv.mAction);
        r.mScale = relive::From(tlv.mScale);
        r.mOnSound = From(tlv.mOnSound);
        r.mOffSound = From(tlv.mOffSound);
        r.mSoundDirection = From(tlv.mSoundDirection);
        r.mSwitchId = tlv.mSwitchId;
        return r;
    }

    static Path_Lever From(const ::Path_Lever& tlv)
    {
        Path_Lever r;
        BaseConvert(r, tlv);
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
    static relive::Path_Lever::LeverSoundType From(AO::LeverSoundType soundType)
    {
        switch (soundType)
        {
            case AO::LeverSoundType::eNone:
                return relive::Path_Lever::LeverSoundType::eNone;
            case AO::LeverSoundType::eWell_1:
                return relive::Path_Lever::LeverSoundType::eWell;
            case AO::LeverSoundType::eSwitchBellHammer_2:
                return relive::Path_Lever::LeverSoundType::eSwitchBellHammer;
            case AO::LeverSoundType::eDoor_3:
                return relive::Path_Lever::LeverSoundType::eDoor;
            case AO::LeverSoundType::eElectricWall_4:
                return relive::Path_Lever::LeverSoundType::eElectricWall;
            case AO::LeverSoundType::eSecurityOrb_5:
                return relive::Path_Lever::LeverSoundType::eSecurityOrb;
        }
        ALIVE_FATAL("Bad lever sound type");
    }

    static relive::Path_Lever::LeverSoundDirection From(AO::LeverSoundDirection soundDir)
    {
        switch (soundDir)
        {
            case AO::LeverSoundDirection::eLeftAndRight_0:
                return relive::Path_Lever::LeverSoundDirection::eLeftAndRight;
            case AO::LeverSoundDirection::eLeft_1:
                return relive::Path_Lever::LeverSoundDirection::eLeft;
            case AO::LeverSoundDirection::eRight_2:
                return relive::Path_Lever::LeverSoundDirection::eRight;
        }
        ALIVE_FATAL("Bad lever sound direction");
    }

    static relive::Path_Lever::LeverSoundType From(::LeverSoundType soundType)
    {
        switch (soundType)
        {
            case ::LeverSoundType::eNone:
                return relive::Path_Lever::LeverSoundType::eNone;
            case ::LeverSoundType::eWell_1:
                return relive::Path_Lever::LeverSoundType::eWell;
            case ::LeverSoundType::eSwitchBellHammer_2:
                return relive::Path_Lever::LeverSoundType::eSwitchBellHammer;
            case ::LeverSoundType::eDoor_3:
                return relive::Path_Lever::LeverSoundType::eDoor;
            case ::LeverSoundType::eElectricWall_4:
                return relive::Path_Lever::LeverSoundType::eElectricWall;
            case ::LeverSoundType::eSecurityOrb_5:
                return relive::Path_Lever::LeverSoundType::eSecurityOrb;
            case ::LeverSoundType::eLift_6:
                return relive::Path_Lever::LeverSoundType::eLift;
        }
        ALIVE_FATAL("Bad lever sound type");
    }

    static relive::Path_Lever::LeverSoundDirection From(::LeverSoundDirection soundDir)
    {
        switch (soundDir)
        {
            case ::LeverSoundDirection::eLeftAndRight_0:
                return relive::Path_Lever::LeverSoundDirection::eLeftAndRight;
            case ::LeverSoundDirection::eLeft_1:
                return relive::Path_Lever::LeverSoundDirection::eLeft;
            case ::LeverSoundDirection::eRight_2:
                return relive::Path_Lever::LeverSoundDirection::eRight;
        }
        ALIVE_FATAL("Bad lever sound direction");
    }
};

class Path_Edge_Converter final
{
public:
    static Path_Edge From(const AO::Path_Edge& tlv)
    {
        Path_Edge r;
        BaseConvert(r, tlv);
        r.mGrabDirection = From(tlv.mGrabDirection);
        r.mCanGrab = relive::From(tlv.mCanGrab);
        return r;
    }

    static Path_Edge From(const ::Path_Edge& tlv)
    {
        Path_Edge r;
        BaseConvert(r, tlv);
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
    static Path_BirdPortal From(const AO::Path_BirdPortal& tlv)
    {
        Path_BirdPortal r;
        BaseConvert(r, tlv);
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

    static Path_BirdPortal From(const ::Path_BirdPortal& tlv)
    {
        Path_BirdPortal r;
        BaseConvert(r, tlv);
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
    static relive::Path_BirdPortal::PortalType From(AO::PortalType portalType)
    {
        switch (portalType)
        {
            case AO::PortalType::eAbe_0:
                return relive::Path_BirdPortal::PortalType::eAbe;
            case AO::PortalType::eWorker_1:
                return relive::Path_BirdPortal::PortalType::eWorker;
            case AO::PortalType::eShrykull_2:
                return relive::Path_BirdPortal::PortalType::eShrykull;
            case AO::PortalType::eMudTeleport_3:
                return relive::Path_BirdPortal::PortalType::eMudTeleport;
        }
        ALIVE_FATAL("Bad bird portal type");
    }

    static relive::Path_BirdPortal::PortalType From(::PortalType portalType)
    {
        switch (portalType)
        {
            case ::PortalType::eAbe_0:
                return relive::Path_BirdPortal::PortalType::eAbe;
            case ::PortalType::eWorker_1:
                return relive::Path_BirdPortal::PortalType::eWorker;
            case ::PortalType::eShrykull_2:
                return relive::Path_BirdPortal::PortalType::eShrykull;
        }
        ALIVE_FATAL("Bad bird portal type");
    }
};

class Path_BirdPortalExit_Converter final
{
public:
    static Path_BirdPortalExit From(const AO::Path_BirdPortalExit& tlv)
    {
        Path_BirdPortalExit r;
        BaseConvert(r, tlv);
        r.mExitSide = Path_BirdPortal_Converter::From(tlv.mExitSide);
        r.mScale = relive::From(tlv.mScale);
        return r;
    }

    static Path_BirdPortalExit From(const ::Path_BirdPortalExit& tlv)
    {
        Path_BirdPortalExit r;
        BaseConvert(r, tlv);
        r.mExitSide = Path_BirdPortal_Converter::From(tlv.mExitSide);
        r.mScale = relive::From(tlv.mScale);
        return r;
    }
};

class Path_LightEffect_Converter final
{
public:
    static Path_LightEffect From(const AO::Path_LightEffect& tlv)
    {
        Path_LightEffect r;
        BaseConvert(r, tlv);
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
    static Path_MusicTrigger From(const AO::Path_MusicTrigger& tlv)
    {
        Path_MusicTrigger r;
        BaseConvert(r, tlv);
        r.mMusicType = From(tlv.mMusicType);
        r.mTriggeredBy = From(tlv.mTriggeredBy);
        r.mSwitchId = tlv.mSwitchId;
        r.mMusicDelay = tlv.mMusicDelay;
        return r;
    }

    static Path_MusicTrigger From(const ::Path_MusicTrigger& tlv)
    {
        Path_MusicTrigger r;
        BaseConvert(r, tlv);
        r.mMusicType = From(tlv.mMusicType);
        r.mTriggeredBy = From(tlv.mTriggeredBy);
        r.mMusicDelay = tlv.mMusicDelay;
        return r;
    }

private:
    static relive::Path_MusicTrigger::MusicTriggerMusicType From(const AO::MusicTriggerMusicType musicType)
    {
        switch (musicType)
        {
            case AO::MusicTriggerMusicType::eDrumAmbience_0:
                return relive::Path_MusicTrigger::MusicTriggerMusicType::eDrumAmbience;
            case AO::MusicTriggerMusicType::eDeathDrumShort_1:
                return relive::Path_MusicTrigger::MusicTriggerMusicType::eDeathDrumShort;
            case AO::MusicTriggerMusicType::eSecretAreaLong_2:
                return relive::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaLong;
            case AO::MusicTriggerMusicType::eSoftChase_3:
                return relive::Path_MusicTrigger::MusicTriggerMusicType::eSoftChase;
            case AO::MusicTriggerMusicType::eIntenseChase_4:
                return relive::Path_MusicTrigger::MusicTriggerMusicType::eIntenseChase;
            case AO::MusicTriggerMusicType::eChime_5:
                return relive::Path_MusicTrigger::MusicTriggerMusicType::eChime;
            case AO::MusicTriggerMusicType::eSecretAreaShort_6:
                return relive::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaShort;
        }
        ALIVE_FATAL("Bad music trigger music type");
    }

    static relive::Path_MusicTrigger::TriggeredBy From(const AO::TriggeredBy triggeredBy)
    {
        switch (triggeredBy)
        {
            case AO::TriggeredBy::eTimer_0:
                return relive::Path_MusicTrigger::TriggeredBy::eTimer;
            case AO::TriggeredBy::eTouching_1:
                return relive::Path_MusicTrigger::TriggeredBy::eTouching;
            case AO::TriggeredBy::eSwitchID_2:
                return relive::Path_MusicTrigger::TriggeredBy::eSwitchId;
            case AO::TriggeredBy::eUnknown_3:
                return relive::Path_MusicTrigger::TriggeredBy::eUnknown;
        }
        ALIVE_FATAL("Bad music trigger triggered by value");
    }

    static relive::Path_MusicTrigger::MusicTriggerMusicType From(const ::MusicTriggerMusicType musicType)
    {
        switch (musicType)
        {
            case ::MusicTriggerMusicType::eDrumAmbience_0:
                return relive::Path_MusicTrigger::MusicTriggerMusicType::eDrumAmbience;
            case ::MusicTriggerMusicType::eDeathDrumShort_1:
                return relive::Path_MusicTrigger::MusicTriggerMusicType::eDeathDrumShort;
            case ::MusicTriggerMusicType::eSecretAreaLong_2:
                return relive::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaLong;
            case ::MusicTriggerMusicType::eSoftChase_3:
                return relive::Path_MusicTrigger::MusicTriggerMusicType::eSoftChase;
            case ::MusicTriggerMusicType::eIntenseChase_4:
                return relive::Path_MusicTrigger::MusicTriggerMusicType::eIntenseChase;
            case ::MusicTriggerMusicType::eChime_5:
                return relive::Path_MusicTrigger::MusicTriggerMusicType::eChime;
            case ::MusicTriggerMusicType::eSecretAreaShort_6:
                return relive::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaShort;
        }
        ALIVE_FATAL("Bad music trigger music type");
    }

    static relive::Path_MusicTrigger::TriggeredBy From(const ::TriggeredBy triggeredBy)
    {
        switch (triggeredBy)
        {
            case ::TriggeredBy::eTimer_0:
                return relive::Path_MusicTrigger::TriggeredBy::eTimer;
            case ::TriggeredBy::eTouching_1:
                return relive::Path_MusicTrigger::TriggeredBy::eTouching;
        }
        ALIVE_FATAL("Bad music trigger triggered by value");
    }
};

class Path_SoftLanding_Converter final
{
public:
    static Path_SoftLanding From(const AO::Path_SoftLanding& tlv)
    {
        Path_SoftLanding r;
        BaseConvert(r, tlv);
        return r;
    }

    static Path_SoftLanding From(const ::Path_SoftLanding& tlv)
    {
        Path_SoftLanding r;
        BaseConvert(r, tlv);
        r.mSwitchId = tlv.mSwitchId;
        return r;
    }
};

class Path_LiftMover_Converter final
{
public:
    static Path_LiftMover From(const AO::Path_LiftMover& tlv)
    {
        Path_LiftMover r;
        BaseConvert(r, tlv);
        r.mLiftMoverSwitchId = tlv.mLiftMoverSwitchId;
        r.mTargetLiftPointId = tlv.mTargetLiftPointId;
        r.mMoveDirection = From(tlv.mMoveDirection);
        return r;
    }

    static Path_LiftMover From(const ::Path_LiftMover& tlv)
    {
        Path_LiftMover r;
        BaseConvert(r, tlv);
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
    static Path_HintFly From(const AO::Path_HintFly& tlv)
    {
        Path_HintFly r;
        BaseConvert(r, tlv);
        r.mMessageId = tlv.mMessageId;
        return r;
    }
};

class Path_TimerTrigger_Converter final
{
public:
    static Path_TimerTrigger From(const AO::Path_TimerTrigger& tlv)
    {
        Path_TimerTrigger r;
        BaseConvert(r, tlv);
        r.mInputSwitchId = tlv.mInputSwitchId;
        r.mActivationDelay = tlv.mActivationDelay;
        r.mOutputSwitchId1 = tlv.mOutputSwitchId1;
        r.mOutputSwitchId2 = tlv.mOutputSwitchId2;
        r.mOutputSwitchId3 = tlv.mOutputSwitchId3;
        r.mOutputSwitchId4 = tlv.mOutputSwitchId4;
        return r;
    }

    static Path_TimerTrigger From(const ::Path_TimerTrigger& tlv)
    {
        Path_TimerTrigger r;
        BaseConvert(r, tlv);
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
    static Path_FlintLockFire From(const AO::Path_FlintLockFire& tlv)
    {
        Path_FlintLockFire r;
        BaseConvert(r, tlv);
        r.mScale = relive::From(tlv.mScale);
        r.mSwitchId = tlv.mSwitchId;
        return r;
    }
};

class Path_HoneySack_Converter final
{
public:
    static Path_HoneySack From(const AO::Path_HoneySack& tlv)
    {
        Path_HoneySack r;
        BaseConvert(r, tlv);
        r.mChaseTime = tlv.mChaseTime;
        r.mScale = relive::From(tlv.mScale);
        return r;
    }
};

class Path_Bat_Converter final
{
public:
    static Path_Bat From(const AO::Path_Bat& tlv)
    {
        Path_Bat r;
        BaseConvert(r, tlv);
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
    static Path_RollingBallStopper From(const AO::Path_RollingBallStopper& tlv)
    {
        Path_RollingBallStopper r;
        BaseConvert(r, tlv);
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
    static Path_RollingBall From(const AO::Path_RollingBall& tlv)
    {
        Path_RollingBall r;
        BaseConvert(r, tlv);
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
    static Path_MotionDetector From(const AO::Path_MotionDetector& tlv)
    {
        Path_MotionDetector r;
        BaseConvert(r, tlv);
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

    static Path_MotionDetector From(const ::Path_MotionDetector& tlv)
    {
        Path_MotionDetector r;
        BaseConvert(r, tlv);
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
    static Path_BellHammer From(const AO::Path_BellHammer& tlv)
    {
        Path_BellHammer r;
        BaseConvert(r, tlv);
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
    static Path_SligBoundLeft From(const AO::Path_SligBoundLeft& tlv)
    {
        Path_SligBoundLeft r;
        BaseConvert(r, tlv);
        r.mSligBoundId = tlv.mSligBoundId;
        r.mDisabledResources = tlv.mDisabledResources.Raw().all;
        return r;
    }

    static Path_SligBoundLeft From(const ::Path_SligBoundLeft& tlv)
    {
        Path_SligBoundLeft r;
        BaseConvert(r, tlv);
        r.mSligBoundId = tlv.mSligBoundId;
        r.mDisabledResources = tlv.mDisabledResources;
        return r;
    }
};

class Path_SligBoundRight_Converter final
{
public:
    static Path_SligBoundRight From(const AO::Path_SligBoundRight& tlv)
    {
        Path_SligBoundRight r;
        BaseConvert(r, tlv);
        r.mSligBoundId = tlv.mSligBoundId;
        r.mDisabledResources = tlv.mDisabledResources.Raw().all;
        return r;
    }

    static Path_SligBoundRight From(const ::Path_SligBoundRight& tlv)
    {
        Path_SligBoundRight r;
        BaseConvert(r, tlv);
        r.mSligBoundId = tlv.mSligBoundId;
        r.mDisabledResources = tlv.mDisabledResources;
        return r;
    }
};

class Path_SligPersist_Converter final
{
public:
    static Path_SligPersist From(const AO::Path_SligPersist& tlv)
    {
        Path_SligPersist r;
        BaseConvert(r, tlv);
        r.mSligBoundId = tlv.mSligBoundId;
        r.mDisabledResources = tlv.mDisabledResources.Raw().all;
        return r;
    }

    static Path_SligPersist From(const ::Path_SligPersist& tlv)
    {
        Path_SligPersist r;
        BaseConvert(r, tlv);
        r.mSligBoundId = tlv.mSligBoundId;
        r.mDisabledResources = tlv.mDisabledResources;
        return r;
    }
};

class Path_BackgroundAnimation_Converter final
{
public:
    static Path_BackgroundAnimation From(const AO::Path_BackgroundAnimation& tlv)
    {
        Path_BackgroundAnimation r;
        BaseConvert(r, tlv);
        r.mAnimId = tlv.mAnimId;
        r.mIsSemiTrans = relive::From(tlv.mIsSemiTrans);
        r.mSemiTransMode = From(tlv.mSemiTransMode);
        r.mSoundEffect = From(tlv.mSoundEffect);
        return r;
    }

    static Path_BackgroundAnimation From(const ::Path_BackgroundAnimation& tlv)
    {
        Path_BackgroundAnimation r;
        BaseConvert(r, tlv);
        r.mAnimId = tlv.mAnimId;
        r.mIsSemiTrans = relive::From(tlv.mIsSemiTrans);
        r.mSemiTransMode = From(tlv.mSemiTransMode);
        r.mLayer = From(tlv.mLayer);
        return r;
    }

private:
    static Path_BackgroundAnimation::TPageAbr From(TPageAbr mode)
    {
        switch (mode)
        {
            case TPageAbr::eBlend_0:
                return Path_BackgroundAnimation::TPageAbr::eBlend_0;
            case TPageAbr::eBlend_1:
                return Path_BackgroundAnimation::TPageAbr::eBlend_1;
            case TPageAbr::eBlend_2:
                return Path_BackgroundAnimation::TPageAbr::eBlend_2;
            case TPageAbr::eBlend_3:
                return Path_BackgroundAnimation::TPageAbr::eBlend_3;
        }
        ALIVE_FATAL("Bad TPageAbr");
    }

    static Path_BackgroundAnimation::Layer From(Layer layer)
    {
        switch (layer)
        {
            case Layer::eLayer_0:
                return Path_BackgroundAnimation::Layer::eLayer0;
        }
        ALIVE_FATAL("Bad TPageAbr");
    }

    static Path_BackgroundAnimation::BgAnimSounds From(AO::BgAnimSounds sound)
    {
        switch (sound)
        {
            case AO::BgAnimSounds::eNone_m1:
                return Path_BackgroundAnimation::BgAnimSounds::eNone_m1;
            case AO::BgAnimSounds::eNone_0:
                return Path_BackgroundAnimation::BgAnimSounds::eNone_0;
            case AO::BgAnimSounds::eFire_1:
                return Path_BackgroundAnimation::BgAnimSounds::eFire;
            case AO::BgAnimSounds::eFireIdx_40:
                return Path_BackgroundAnimation::BgAnimSounds::eFireIdx;
        }
        ALIVE_FATAL("Bad bg anim sound");
    }
};

class Path_MainMenuController_Converter final
{
public:
    static Path_MainMenuController From(const AO::Path_MainMenuController& tlv)
    {
        Path_MainMenuController r;
        BaseConvert(r, tlv);
        return r;
    }

    static Path_MainMenuController From(const ::Path_MainMenuController& tlv)
    {
        Path_MainMenuController r;
        BaseConvert(r, tlv);
        return r;
    }
};

class Path_ElumWall_Converter final
{
public:
    static Path_ElumWall From(const AO::Path_ElumWall& tlv)
    {
        Path_ElumWall r;
        BaseConvert(r, tlv);
        return r;
    }
};

class Path_ElumStart_Converter final
{
public:
    static Path_ElumStart From(const AO::Path_ElumStart& tlv)
    {
        Path_ElumStart r;
        BaseConvert(r, tlv);
        return r;
    }
};

class Path_KillUnsavedMuds_Converter final
{
public:
    static Path_KillUnsavedMuds From(const AO::Path_KillUnsavedMuds& tlv)
    {
        Path_KillUnsavedMuds r;
        BaseConvert(r, tlv);
        return r;
    }
};

class Path_InvisibleZone_Converter final
{
public:
    static Path_InvisibleZone From(const AO::Path_InvisibleZone& tlv)
    {
        Path_InvisibleZone r;
        BaseConvert(r, tlv);
        return r;
    }

    static Path_InvisibleZone From(const ::Path_InvisibleZone& tlv)
    {
        Path_InvisibleZone r;
        BaseConvert(r, tlv);
        return r;
    }
};

class Path_StartController_Converter final
{
public:
    static Path_StartController From(const AO::Path_StartController& tlv)
    {
        Path_StartController r;
        BaseConvert(r, tlv);
        return r;
    }
};

class Path_ScrabNoFall_Converter final
{
public:
    static Path_ScrabNoFall From(const AO::Path_ScrabNoFall& tlv)
    {
        Path_ScrabNoFall r;
        BaseConvert(r, tlv);
        return r;
    }
};

class Path_ScrabBoundLeft_Converter final
{
public:
    static Path_ScrabBoundLeft From(const AO::Path_ScrabBoundLeft& tlv)
    {
        Path_ScrabBoundLeft r;
        BaseConvert(r, tlv);
        return r;
    }

    static Path_ScrabBoundLeft From(const ::Path_ScrabBoundLeft& tlv)
    {
        Path_ScrabBoundLeft r;
        BaseConvert(r, tlv);
        return r;
    }
};

class Path_ScrabBoundRight_Converter final
{
public:
    static Path_ScrabBoundRight From(const AO::Path_ScrabBoundRight& tlv)
    {
        Path_ScrabBoundRight r;
        BaseConvert(r, tlv);
        return r;
    }

    static Path_ScrabBoundRight From(const ::Path_ScrabBoundRight& tlv)
    {
        Path_ScrabBoundRight r;
        BaseConvert(r, tlv);
        return r;
    }
};

class Path_HoneyDripTarget_Converter final
{
public:
    static Path_HoneyDripTarget From(const AO::Path_HoneyDripTarget& tlv)
    {
        Path_HoneyDripTarget r;
        BaseConvert(r, tlv);
        return r;
    }
};

class Path_ZSligCover_Converter final
{
public:
    static Path_ZSligCover From(const AO::Path_ZSligCover& tlv)
    {
        Path_ZSligCover r;
        BaseConvert(r, tlv);
        return r;
    }

    static Path_ZSligCover From(const ::Path_ZSligCover& tlv)
    {
        Path_ZSligCover r;
        BaseConvert(r, tlv);
        return r;
    }
};

class Path_DeathDrop_Converter final
{
public:
    static Path_DeathDrop From(const AO::Path_DeathDrop& tlv)
    {
        Path_DeathDrop r;
        BaseConvert(r, tlv);
        return r;
    }

    static Path_DeathDrop From(const ::Path_DeathDrop& tlv)
    {
        Path_DeathDrop r;
        BaseConvert(r, tlv);
        return r;
    }
};

class Path_ChimeLock_Converter final
{
public:
    static Path_ChimeLock From(const AO::Path_ChimeLock& tlv)
    {
        Path_ChimeLock r;
        BaseConvert(r, tlv);
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
    static Path_LCDStatusBoard From(const AO::Path_LCDStatusBoard& tlv)
    {
        Path_LCDStatusBoard r;
        BaseConvert(r, tlv);
        return r;
    }

    static Path_LCDStatusBoard From(const ::Path_LCDStatusBoard& tlv)
    {
        Path_LCDStatusBoard r;
        BaseConvert(r, tlv);
        r.mNumberOfMuds = tlv.mNumberOfMuds;
        r.mZulagNumber = tlv.mZulagNumber;
        r.mHideBoard = relive::From(tlv.mHideBoard);
        return r;
    }
};

class Path_CreditsController_Converter final
{
public:
    static Path_CreditsController From(const AO::Path_CreditsController& tlv)
    {
        Path_CreditsController r;
        BaseConvert(r, tlv);
        return r;
    }

    static Path_CreditsController From(const ::Path_CreditsController& tlv)
    {
        Path_CreditsController r;
        BaseConvert(r, tlv);
        return r;
    }
};

class Path_ResetPath_Converter final
{
public:
    static Path_ResetPath From(const AO::Path_ResetPath& tlv)
    {
        Path_ResetPath r;
        BaseConvert(r, tlv);
        r.mClearIds = relive::From(tlv.mClearIds);
        r.mFrom = tlv.mFrom;
        r.mTo = tlv.mTo;
        r.mExclude = tlv.mExclude;
        r.mClearObjects = relive::From(tlv.mClearObjects);
        r.mPath = tlv.mPath;
        return r;
    }

    static Path_ResetPath From(const ::Path_ResetPath& tlv)
    {
        Path_ResetPath r;
        BaseConvert(r, tlv);
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
    static Path_MeatSack From(const AO::Path_MeatSack& tlv)
    {
        Path_MeatSack r;
        BaseConvert(r, tlv);
        r.mMeatFallDirection = relive::From(tlv.mMeatFallDirection);
        r.mVelX = tlv.mVelX;
        r.mVelY = tlv.mVelY;
        r.mScale = relive::From(tlv.mScale);
        r.mMeatAmount = tlv.mMeatAmount;
        return r;
    }

    static Path_MeatSack From(const ::Path_MeatSack& tlv)
    {
        Path_MeatSack r;
        BaseConvert(r, tlv);
        r.mMeatFallDirection = relive::From(tlv.mMeatFallDirection);
        r.mVelX = tlv.mVelX;
        r.mVelY = tlv.mVelY;
        r.mScale = relive::From(tlv.mScale);
        r.mMeatAmount = tlv.mMeatAmount;
        return r;
    }
};

class Path_Bees_Converter final
{
public:
    static Path_Bees From(const AO::Path_Bees& tlv)
    {
        Path_Bees r;
        BaseConvert(r, tlv);
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
    static Path_BellsongStone From(const AO::Path_BellsongStone& tlv)
    {
        Path_BellsongStone r;
        BaseConvert(r, tlv);
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
    static Path_MovieStone From(const AO::Path_MovieStone& tlv)
    {
        Path_MovieStone r;
        BaseConvert(r, tlv);
        r.mMovieId = tlv.mData.mMovieId;
        r.mScale = relive::From(tlv.mData.mScale);
        return r;
    }

    static Path_MovieStone From(const ::Path_MovieStone& tlv)
    {
        Path_MovieStone r;
        BaseConvert(r, tlv);
        r.mMovieId = tlv.mMovieId;
        r.mScale = relive::From(tlv.mScale);
        r.mTriggerSwitchId = tlv.mTriggerSwitchId;
        return r;
    }
};

class Path_HandStone_Converter final
{
public:
    static Path_HandStone From(const AO::Path_HandStone& tlv)
    {
        Path_HandStone r;
        BaseConvert(r, tlv);
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

    static Path_HandStone From(const ::Path_HandStone& tlv)
    {
        Path_HandStone r;
        BaseConvert(r, tlv);
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
    static Path_PathTransition From(const AO::Path_PathTransition& tlv)
    {
        Path_PathTransition r;
        BaseConvert(r, tlv);
        r.mNextLevel = MapWrapper::FromAO(tlv.mNextLevel);
        r.mNextPath = tlv.mNextPath;
        r.mNextCamera = tlv.mNextCamera;
        r.mMovieId = tlv.mMovieId;
        r.mWipeEffect = tlv.mWipeEffect;
        r.mNextPathScale = relive::From(tlv.mNextPathScale);
        return r;
    }

    static Path_PathTransition From(const ::Path_PathTransition& tlv)
    {
        Path_PathTransition r;
        BaseConvert(r, tlv);
        r.mNextLevel = MapWrapper::FromAE(tlv.field_10_level);
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
    static Path_Pulley From(const AO::Path_Pulley& tlv)
    {
        Path_Pulley r;
        BaseConvert(r, tlv);
        return r;
    }

    static Path_Pulley From(const ::Path_Pulley& tlv)
    {
        Path_Pulley r;
        BaseConvert(r, tlv);
        return r;
    }
};

class Path_Honey_Converter final
{
public:
    static Path_Honey From(const AO::Path_Honey& tlv)
    {
        Path_Honey r;
        BaseConvert(r, tlv);
        return r;
    }
};

class Path_BeeSwarmHole_Converter final
{
public:
    static Path_BeeSwarmHole From(const AO::Path_BeeSwarmHole& tlv)
    {
        Path_BeeSwarmHole r;
        BaseConvert(r, tlv);
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
    static Path_Door From(const AO::Path_Door& tlv)
    {
        Path_Door r;
        BaseConvert(r, tlv);
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

    static Path_Door From(const ::Path_Door& tlv)
    {
        Path_Door r;
        BaseConvert(r, tlv);
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
    static Path_Door::DoorStates From(::DoorStates state)
    {
        switch (state)
        {
            case ::DoorStates::eOpen_0:
                return Path_Door::DoorStates::eOpen;
            case ::DoorStates::eClosed_1:
                return Path_Door::DoorStates::eClosed;
            case ::DoorStates::eOpening_2:
                return Path_Door::DoorStates::eOpening;
            case ::DoorStates::eClosing_3:
                return Path_Door::DoorStates::eClosing;
        }
        ALIVE_FATAL("Bad door state");
    }

    static Path_Door::DoorTypes From(AO::DoorTypes type)
    {
        switch (type)
        {
            case AO::DoorTypes::eBasicDoor_0:
                return Path_Door::DoorTypes::eBasicDoor;
            case AO::DoorTypes::eTrialDoor_1:
                return Path_Door::DoorTypes::eTrialDoor;
            case AO::DoorTypes::eHubDoor_2:
                return Path_Door::DoorTypes::eHubDoor;
        }
        ALIVE_FATAL("Bad door type");
    }

    static Path_Door::DoorTypes From(::DoorTypes type)
    {
        switch (type)
        {
            case ::DoorTypes::eBasicDoor_0:
                return Path_Door::DoorTypes::eBasicDoor;
            case ::DoorTypes::eTasksDoorWithSecretMusic_2:
                return Path_Door::DoorTypes::eTasksDoorWithSecretMusic;
            case ::DoorTypes::eTasksDoor_3:
                return Path_Door::DoorTypes::eTasksDoor;
        }
        ALIVE_FATAL("Bad door type");
    }
};

class Path_AbeStart_Converter final
{
public:
    static Path_AbeStart From(const ::Path_AbeStart& tlv)
    {
        Path_AbeStart r;
        BaseConvert(r, tlv);
        return r;
    }
};

class Path_EnemyStopper_Converter final
{
public:
    static Path_EnemyStopper From(const AO::Path_EnemyStopper& tlv)
    {
        Path_EnemyStopper r;
        BaseConvert(r, tlv);
        r.mStopDirection = From(tlv.mStopDirection);
        r.mSwitchId = tlv.mSwitchId;
        return r;
    }

    static Path_EnemyStopper From(const ::Path_EnemyStopper& tlv)
    {
        Path_EnemyStopper r;
        BaseConvert(r, tlv);
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
    static Path_MovingBombStopper From(const AO::Path_MovingBombStopper& tlv)
    {
        Path_MovingBombStopper r;
        BaseConvert(r, tlv);
        r.mMinDelay = tlv.mMinDelay;
        r.mMaxDelay = tlv.mMaxDelay;
        return r;
    }

    static Path_MovingBombStopper From(const ::Path_MovingBombStopper& tlv)
    {
        Path_MovingBombStopper r;
        BaseConvert(r, tlv);
        r.mMinDelay = tlv.mMinDelay;
        r.mMaxDelay = tlv.mMaxDelay;
        return r;
    }
};

class Path_DoorFlame_Converter final
{
public:
    static Path_DoorFlame From(const AO::Path_DoorFlame& tlv)
    {
        Path_DoorFlame r;
        BaseConvert(r, tlv);
        r.mSwitchId = tlv.mSwitchId;
        r.mScale = From(tlv.mScale);
        r.mColour = From(tlv.mColour);
        return r;
    }

    static Path_DoorFlame From(const ::Path_DoorFlame& tlv)
    {
        Path_DoorFlame r;
        BaseConvert(r, tlv);
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
    static Path_Mudokon From(const AO::Path_Mudokon& tlv)
    {
        Path_Mudokon r;
        BaseConvert(r, tlv);
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

    static Path_Mudokon From(const ::Path_Mudokon& tlv)
    {
        Path_Mudokon r;
        BaseConvert(r, tlv);
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

    static Path_Mudokon::MudJobs From(::MudJobs job)
    {
        switch (job)
        {
            case ::MudJobs::eChisle_0:
                return Path_Mudokon::MudJobs::eChisle;
            case ::MudJobs::eSitScrub_1:
                return Path_Mudokon::MudJobs::eSitScrub;
            case ::MudJobs::eAngryWorker_2:
                return Path_Mudokon::MudJobs::eAngryWorker;
            case ::MudJobs::eDamageRingGiver_3:
                return Path_Mudokon::MudJobs::eDamageRingGiver;
            case ::MudJobs::eHealthRingGiver_4:
                return Path_Mudokon::MudJobs::eHealthRingGiver;
        }
    }
    
    static Path_Mudokon::Mud_TLV_Emotion From(::Mud_TLV_Emotion emotion)
    {
        switch (emotion)
        {
            case ::Mud_TLV_Emotion::eNormal_0:
                return Path_Mudokon::Mud_TLV_Emotion::eNormal;
            case ::Mud_TLV_Emotion::eAngry_1:
                return Path_Mudokon::Mud_TLV_Emotion::eAngry;
            case ::Mud_TLV_Emotion::eSad_2:
                return Path_Mudokon::Mud_TLV_Emotion::eSad;
            case ::Mud_TLV_Emotion::eWired_3:
                return Path_Mudokon::Mud_TLV_Emotion::eWired;
            case ::Mud_TLV_Emotion::eSick_4:
                return Path_Mudokon::Mud_TLV_Emotion::eSick;
        }
        ALIVE_FATAL("Bad mudokon emotion");
    }
};

class Path_MovingBomb_Converter final
{
public:
    static Path_MovingBomb From(const AO::Path_MovingBomb& tlv)
    {
        Path_MovingBomb r;
        BaseConvert(r, tlv);
        r.mSpeed = tlv.mSpeed;
        r.mStartMovingSwitchId = tlv.mStartMovingSwitchId;
        r.mTriggeredByAlarm = relive::From(tlv.mTriggeredByAlarm);
        r.mScale = relive::From(tlv.mScale);
        r.mDisabledResources = tlv.mDisabledResources;
        r.mStartSpeed = tlv.mStartSpeed;
        r.mPersistOffscreen = relive::From(tlv.mPersistOffscreen);
        return r;
    }

    static Path_MovingBomb From(const ::Path_MovingBomb& tlv)
    {
        Path_MovingBomb r;
        BaseConvert(r, tlv);
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
    static Path_ElumPathTrans From(const AO::Path_ElumPathTrans& tlv)
    {
        Path_ElumPathTrans r;
        BaseConvert(r, tlv);
        r.mNextLevel = MapWrapper::FromAO(tlv.mNextLevel);
        r.mNextPath = tlv.mNextPath;
        r.mNextCamera = tlv.mNextCamera;
        return r;
    }
};

class Path_MudokonPathTrans_Converter final
{
public:
    static Path_MudokonPathTrans From(const AO::Path_MudokonPathTrans& tlv)
    {
        Path_MudokonPathTrans r;
        BaseConvert(r, tlv);
        r.mNextLevel = MapWrapper::FromAO(tlv.mNextLevel);
        r.mNextPath = tlv.mNextPath;
        r.mNextCamera = tlv.mNextCamera;
        return r;
    }
};

class Path_SecurityClaw_Converter final
{
public:
    static Path_SecurityClaw From(const AO::Path_SecurityClaw& tlv)
    {
        Path_SecurityClaw r;
        BaseConvert(r, tlv);
        r.mScale = relive::From(tlv.mScale);
        r.mAlarmSwitchId = tlv.mAlarmSwitchId;
        r.mAlarmDuration = tlv.mAlarmDuration;
        r.mDisabledResources = tlv.mDisabledResources;
        return r;
    }

    // This is just a security orb in AE
    static Path_SecurityClaw From(const ::Path_SecurityClaw& tlv)
    {
        Path_SecurityClaw r;
        BaseConvert(r, tlv);
        r.mScale = relive::From(tlv.mScale);
        r.mDisabledResources = tlv.mDisabledResources;
        return r;
    }
};

class Path_SlingMudokon_Converter final
{
public:
    static Path_SlingMudokon From(const AO::Path_SlingMudokon& tlv)
    {
        Path_SlingMudokon r;
        BaseConvert(r, tlv);
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
    static Path_FootSwitch From(const AO::Path_FootSwitch& tlv)
    {
        Path_FootSwitch r;
        BaseConvert(r, tlv);
        r.mSwitchId = tlv.mSwitchId;
        r.mScale = relive::From(tlv.mScale);
        r.mAction = relive::From(tlv.mAction);
        r.mTriggeredBy = From(tlv.mTriggeredBy);
        return r;
    }

    static Path_FootSwitch From(const ::Path_FootSwitch& tlv)
    {
        Path_FootSwitch r;
        BaseConvert(r, tlv);
        r.mSwitchId = tlv.mSwitchId;
        r.mScale = relive::From(tlv.mScale);
        r.mAction = relive::From(tlv.mAction);
        r.mTriggeredBy = From(tlv.mTriggeredBy);
        return r;
    }
private:
    static Path_FootSwitch::FootSwitchTriggerBy From(AO::FootSwitchTriggerBy triggerdBy)
    {
        switch (triggerdBy)
        {
            case AO::FootSwitchTriggerBy::eAbe_0:
                return Path_FootSwitch::FootSwitchTriggerBy::eAbe;
            case AO::FootSwitchTriggerBy::eAnyone_1:
                return Path_FootSwitch::FootSwitchTriggerBy::eAnyone;
        }
        ALIVE_FATAL("Bad foot switch triggered by value");
    }

    static Path_FootSwitch::FootSwitchTriggerBy From(::FootSwitchTriggerBy triggerdBy)
    {
        switch (triggerdBy)
        {
            case ::FootSwitchTriggerBy::eAbe_0:
                return Path_FootSwitch::FootSwitchTriggerBy::eAbe;
            case ::FootSwitchTriggerBy::eAnyone_1:
                return Path_FootSwitch::FootSwitchTriggerBy::eAnyone;
        }
        ALIVE_FATAL("Bad foot switch triggered by value");
    }
};

class Path_Paramite_Converter final
{
public:
    static Path_Paramite From(const AO::Path_Paramite& tlv)
    {
        Path_Paramite r;
        BaseConvert(r, tlv);
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

    static Path_Paramite From(const ::Path_Paramite& tlv)
    {
        Path_Paramite r;
        BaseConvert(r, tlv);
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
    static Path_ZzzSpawner From(const AO::Path_ZzzSpawner& tlv)
    {
        Path_ZzzSpawner r;
        BaseConvert(r, tlv);
        r.mScale = relive::From(tlv.mScale);
        r.mSwitchId = tlv.mSwitchId;
        r.mZzzInterval = tlv.mZzzInterval;
        return r;
    }

    static Path_ZzzSpawner From(const ::Path_ZzzSpawner& tlv)
    {
        Path_ZzzSpawner r;
        BaseConvert(r, tlv);
        r.mScale = relive::From(tlv.mScale);
        r.mSwitchId = tlv.mSwitchId;
        r.mZzzInterval = tlv.mZzzInterval;
        return r;
    }
};

class Path_BackgroundGlukkon_Converter final
{
public:
    static Path_BackgroundGlukkon From(const AO::Path_BackgroundGlukkon& tlv)
    {
        Path_BackgroundGlukkon r;
        BaseConvert(r, tlv);
        r.mScalePercent = tlv.mScalePercent;
        r.mPalId = tlv.mPalId;
        return r;
    }
};

class Path_GasEmitter_Converter final
{
public:
    static Path_GasEmitter From(const AO::Path_GasEmitter& tlv)
    {
        Path_GasEmitter r;
        BaseConvert(r, tlv);
        return r;
    }

    static Path_GasEmitter From(const ::Path_GasEmitter& tlv)
    {
        Path_GasEmitter r;
        BaseConvert(r, tlv);
        r.mSwitchId = tlv.mSwitchId;
        r.mColour = From(tlv.mColour);
        return r;
    }
private:
    static Path_GasEmitter::GasColour From(::GasColour colour)
    {
        switch (colour)
        {
            case ::GasColour::Yellow_0:
                return Path_GasEmitter::GasColour::eYellow;
            case ::GasColour::Red_1:
                return Path_GasEmitter::GasColour::eRed;
            case ::GasColour::Green_2:
                return Path_GasEmitter::GasColour::eGreen;
            case ::GasColour::Blue_3:
                return Path_GasEmitter::GasColour::eBlue;
            case ::GasColour::White_4:
                return Path_GasEmitter::GasColour::eWhite;
        }
        ALIVE_FATAL("Bad gas emitter gas colour");
    }
};

class Path_GasCountDown_Converter final
{
public:
    static Path_GasCountDown From(const AO::Path_GasCountDown& tlv)
    {
        Path_GasCountDown r;
        BaseConvert(r, tlv);
        r.mStartTimerSwitchId = tlv.mStartTimerSwitchId;
        return r;
    }

    static Path_GasCountDown From(const ::Path_GasCountDown& tlv)
    {
        Path_GasCountDown r;
        BaseConvert(r, tlv);
        r.mStartTimerSwitchId = tlv.mStartTimerSwitchId;
        r.mGasCountdownTimer = tlv.mGasCountdownTimer;
        r.mStopTimerSwitchId = tlv.mStopTimerSwitchId;
        return r;
    }
};

class Path_RingCancel_Converter final
{
public:
    static Path_RingCancel From(const AO::Path_RingCancel& tlv)
    {
        Path_RingCancel r;
        BaseConvert(r, tlv);
        return r;
    }
};

class Path_SecurityDoor_Converter final
{
public:
    static Path_SecurityDoor From(const AO::Path_SecurityDoor& tlv)
    {
        Path_SecurityDoor r;
        BaseConvert(r, tlv);
        r.mScale = relive::From(tlv.mScale);
        r.mSwitchId = tlv.mSwitchId;
        r.mCode1 = tlv.mCode1;
        r.mCode2 = tlv.mCode2;
        r.mXPos = tlv.mXPos;
        r.mYPos = tlv.mYPos;
        return r;
    }

    static Path_SecurityDoor From(const ::Path_SecurityDoor& tlv)
    {
        Path_SecurityDoor r;
        BaseConvert(r, tlv);
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
    static Path_LiftMudokon From(const AO::Path_LiftMudokon& tlv)
    {
        Path_LiftMudokon r;
        BaseConvert(r, tlv);
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
    static Path_RingMudokon From(const AO::Path_RingMudokon& tlv)
    {
        Path_RingMudokon r;
        BaseConvert(r, tlv);
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
    static Path_WellLocal From(const AO::Path_WellLocal& tlv)
    {
        Path_WellLocal r;
        BaseConvert(r, tlv);

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

    static Path_WellLocal From(const ::Path_WellLocal& tlv)
    {
        Path_WellLocal r;
        BaseConvert(r, tlv);

        // Well base
        r.mScale = relive::From(tlv.mScale);
        r.mSwitchId = tlv.mSwitchId;
        r.mOtherWellId = tlv.mOtherWellId;
        r.mAnimId = tlv.mAnimId;

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
    static Path_WellExpress From(const AO::Path_WellExpress& tlv)
    {
        Path_WellExpress r;
        BaseConvert(r, tlv);

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

    static Path_WellExpress From(const ::Path_WellExpress& tlv)
    {
        Path_WellExpress r;
        BaseConvert(r, tlv);

        // Well base
        r.mScale = relive::From(tlv.mScale);
        r.mSwitchId = tlv.mSwitchId;
        r.mOtherWellId = tlv.mOtherWellId;
        r.mAnimId = tlv.mAnimId;

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
    static Path_Slog From(const AO::Path_Slog& tlv)
    {
        Path_Slog r;
        BaseConvert(r, tlv);
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

    static Path_Slog From(const ::Path_Slog& tlv)
    {
        Path_Slog r;
        BaseConvert(r, tlv);
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
    static Path_SlogSpawner From(const AO::Path_SlogSpawner& tlv)
    {
        Path_SlogSpawner r;
        BaseConvert(r, tlv);
        r.mScale = relive::From(tlv.mScale);
        r.mMaxSlogs = tlv.mMaxSlogs;
        r.mMaxSlogsAtATime = tlv.mMaxSlogsAtATime;
        r.mStartDirection = From(tlv.mStartDirection);
        r.mSlogSpawnInterval = tlv.mSlogSpawnInterval;
        r.mSpawnerSwitchId = tlv.mSpawnerSwitchId;
        return r;
    }

    static Path_SlogSpawner From(const ::Path_SlogSpawner& tlv)
    {
        Path_SlogSpawner r;
        BaseConvert(r, tlv);
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
    static Path_SlogSpawner::StartDirection From(AO::StartDirection startDir)
    {
        switch (startDir)
        {
            case AO::StartDirection::eRight_0:
                return Path_SlogSpawner::StartDirection::eRight;
            case AO::StartDirection::eLeft_1:
                return Path_SlogSpawner::StartDirection::eLeft;
        }
        ALIVE_FATAL("Bad slog spawner start direction");
    }

    static Path_SlogSpawner::StartDirection From(::StartDirection startDir)
    {
        switch (startDir)
        {
            case ::StartDirection::eRight_0:
                return Path_SlogSpawner::StartDirection::eRight;
            case ::StartDirection::eLeft_1:
                return Path_SlogSpawner::StartDirection::eLeft;
        }
        ALIVE_FATAL("Bad slog spawner start direction");
    }
};

class Path_Scrab_Converter final
{
public:
    static Path_Scrab From(const AO::Path_Scrab& tlv)
    {
        Path_Scrab r;
        BaseConvert(r, tlv);
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

    static Path_Scrab From(const ::Path_Scrab& tlv)
    {
        Path_Scrab r;
        BaseConvert(r, tlv);
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
    static Path_Scrab::ScrabPatrolType From(AO::ScrabPatrolType type)
    {
        switch (type)
        {
            case AO::ScrabPatrolType::eWalk_0:
                return Path_Scrab::ScrabPatrolType::eWalk;
            case AO::ScrabPatrolType::eRunOrWalk192_1:
                return Path_Scrab::ScrabPatrolType::eRunOrWalk192;
            case AO::ScrabPatrolType::eRunOrWalk128_2:
                return Path_Scrab::ScrabPatrolType::eRunOrWalk128;
            case AO::ScrabPatrolType::eRunOrWalk64_3:
                return Path_Scrab::ScrabPatrolType::eRunOrWalk64;
            case AO::ScrabPatrolType::eRun_4:
                return Path_Scrab::ScrabPatrolType::eRun;
        }
        ALIVE_FATAL("Bad scrab patrol type");
    }
};

class Path_Slig_Converter final
{
public:
    static Path_Slig From(const AO::Path_Slig& tlv)
    {
        Path_Slig r;
        BaseConvert(r, tlv);
        r.mScale = relive::From(tlv.mScale);
        r.mStartState = From(tlv.mStartState);
        r.mPauseTime = tlv.mPauseTime;
        r.mPauseLeftMin = tlv.mPauseLeftMin;
        r.mPauseLeftMax = tlv.mPauseLeftMax;
        r.mPauseRightMin = tlv.mPauseRightMin;
        r.mPauseRightMax = tlv.mPauseRightMax;
        r.mShootPossessedSligs = From(tlv.mShootPossessedSligs);
        r.mShootOnSightDelay = tlv.mShootOnSightDelay;
        r.mNumTimesToShoot = tlv.mNumTimesToShoot;
        r.mCode1 = tlv.mCode1;
        r.mCode2 = tlv.mCode2;
        r.mChaseAbeWhenSpotted = relive::From(tlv.mChaseAbeWhenSpotted);
        r.mFacing = relive::From(tlv.mFacing);
        r.mPanicTimeout = tlv.mPanicTimeout;
        r.mStopChaseDelay = tlv.mStopChaseDelay;
        r.mTimeToWaitBeforeChase = tlv.mTimeToWaitBeforeChase;
        r.mSligBoundId = tlv.mSligBoundId;
        r.mAlertedListenTime = tlv.mAlertedListenTime;
        r.mPercentSayWhat = tlv.mPercentSayWhat;
        r.mPercentBeatMud = tlv.mPercentBeatMud;
        r.mZShootDelay = tlv.mZShootDelay;
        r.mStayAwake = relive::From(tlv.mStayAwake);
        r.mDisabledResources = tlv.mDisabledResources.Raw().all;
        r.mNoiseWakeUpDistance = tlv.mNoiseWakeUpDistance;
        r.mSligSpawnerSwitchId = tlv.mSligSpawnerSwitchId;
        return r;
    }

    static Path_Slig From(const ::Path_Slig& tlv)
    {
        Path_Slig r;
        BaseConvert(r, tlv);
        r.mScale = relive::From(tlv.mScale);
        r.mStartState = From(tlv.mStartState);
        r.mPauseTime = tlv.mPauseTime;
        r.mPauseLeftMin = tlv.mPauseLeftMin;
        r.mPauseLeftMax = tlv.mPauseLeftMax;
        r.mPauseRightMin = tlv.mPauseRightMin;
        r.mPauseRightMax = tlv.mPauseRightMax;
        r.mShootPossessedSligs = From(tlv.mShootPossessedSligs);
        r.mShootOnSightDelay = tlv.mShootOnSightDelay;
        r.mNumTimesToShoot = tlv.mNumTimesToShoot;
        r.mCode1 = tlv.mCode1;
        r.mCode2 = tlv.mCode2;
        r.mChaseAbeWhenSpotted = relive::From(tlv.mChaseAbeWhenSpotted);
        r.mFacing = relive::From(tlv.mFacing);
        r.mPanicTimeout = tlv.mPanicTimeout;
        r.mStopChaseDelay = tlv.mStopChaseDelay;
        r.mTimeToWaitBeforeChase = tlv.mTimeToWaitBeforeChase;
        r.mSligBoundId = tlv.mSligBoundId;
        r.mAlertedListenTime = tlv.mAlertedListenTime;
        r.mPercentSayWhat = tlv.mPercentSayWhat;
        r.mPercentBeatMud = tlv.mPercentBeatMud;
        r.mZShootDelay = tlv.mZShootDelay;
        r.mStayAwake = relive::From(tlv.mStayAwake);
        r.mDisabledResources = tlv.mDisabledResources;
        r.mNoiseWakeUpDistance = tlv.mNoiseWakeUpDistance;
        r.mSligSpawnerSwitchId = tlv.mSligSpawnerSwitchId;
        r.mUnlimitedSpawns = relive::From(tlv.mUnlimitedSpawns);
        return r;
    }

    static Path_Slig::StartState From(AO::Path_Slig::StartState startState)
    {
        switch (startState)
        {
            case AO::Path_Slig::StartState::Listening_0:
                return Path_Slig::StartState::Listening;
            case AO::Path_Slig::StartState::Patrol_1:
                return Path_Slig::StartState::Patrol;
            case AO::Path_Slig::StartState::Sleeping_2:
                return Path_Slig::StartState::Sleeping;
            case AO::Path_Slig::StartState::Chase_3:
                return Path_Slig::StartState::Chase;
            case AO::Path_Slig::StartState::ChaseAndDisappear_4:
                return Path_Slig::StartState::ChaseAndDisappear;
            case AO::Path_Slig::StartState::FallingToChase_5:
                return Path_Slig::StartState::eFallingToChase;
        }
        ALIVE_FATAL("Bad slig start state");
    }

    static Path_Slig::StartState From(::Path_Slig::StartState startState)
    {
        switch (startState)
        {
            case ::Path_Slig::StartState::Listening_0:
                return Path_Slig::StartState::Listening;
            case ::Path_Slig::StartState::Patrol_1:
                return Path_Slig::StartState::Patrol;
            case ::Path_Slig::StartState::Sleeping_2:
                return Path_Slig::StartState::Sleeping;
            case ::Path_Slig::StartState::Chase_3:
                return Path_Slig::StartState::Chase;
            case ::Path_Slig::StartState::ChaseAndDisappear_4:
                return Path_Slig::StartState::ChaseAndDisappear;
            case ::Path_Slig::StartState::ListeningToGlukkon_6:
                return Path_Slig::StartState::ListeningToGlukkon;
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
    static Path_SligSpawner From(const AO::Path_SligSpawner& tlv)
    {
        Path_SligSpawner r;
        BaseConvert(r, tlv);
        r.mScale = relive::From(tlv.mScale);
        r.mStartState = From(tlv.mStartState);
        r.mPauseTime = tlv.mPauseTime;
        r.mPauseLeftMin = tlv.mPauseLeftMin;
        r.mPauseLeftMax = tlv.mPauseLeftMax;
        r.mPauseRightMin = tlv.mPauseRightMin;
        r.mPauseRightMax = tlv.mPauseRightMax;
        r.mShootPossessedSligs = From(tlv.mShootPossessedSligs);
        r.mShootOnSightDelay = tlv.mShootOnSightDelay;
        r.mNumTimesToShoot = tlv.mNumTimesToShoot;
        r.mCode1 = tlv.mCode1;
        r.mCode2 = tlv.mCode2;
        r.mChaseAbeWhenSpotted = relive::From(tlv.mChaseAbeWhenSpotted);
        r.mFacing = relive::From(tlv.mFacing);
        r.mPanicTimeout = tlv.mPanicTimeout;
        r.mStopChaseDelay = tlv.mStopChaseDelay;
        r.mTimeToWaitBeforeChase = tlv.mTimeToWaitBeforeChase;
        r.mSligBoundId = tlv.mSligBoundId;
        r.mAlertedListenTime = tlv.mAlertedListenTime;
        r.mPercentSayWhat = tlv.mPercentSayWhat;
        r.mPercentBeatMud = tlv.mPercentBeatMud;
        r.mZShootDelay = tlv.mZShootDelay;
        r.mStayAwake = relive::From(tlv.mStayAwake);
        r.mDisabledResources = tlv.mDisabledResources;
        r.mNoiseWakeUpDistance = tlv.mNoiseWakeUpDistance;
        r.mSligSpawnerSwitchId = static_cast<s16>(tlv.mSligSpawnerSwitchId);
        return r;
    }

    static Path_SligSpawner From(const ::Path_SligSpawner& tlv)
    {
        Path_SligSpawner r;
        BaseConvert(r, tlv);
        r.mScale = relive::From(tlv.mScale);
        r.mStartState = From(tlv.mStartState);
        r.mPauseTime = tlv.mPauseTime;
        r.mPauseLeftMin = tlv.mPauseLeftMin;
        r.mPauseLeftMax = tlv.mPauseLeftMax;
        r.mPauseRightMin = tlv.mPauseRightMin;
        r.mPauseRightMax = tlv.mPauseRightMax;
        r.mShootPossessedSligs = From(tlv.mShootPossessedSligs);
        r.mShootOnSightDelay = tlv.mShootOnSightDelay;
        r.mNumTimesToShoot = tlv.mNumTimesToShoot;
        r.mCode1 = tlv.mCode1;
        r.mCode2 = tlv.mCode2;
        r.mChaseAbeWhenSpotted = relive::From(tlv.mChaseAbeWhenSpotted);
        r.mFacing = relive::From(tlv.mFacing);
        r.mPanicTimeout = tlv.mPanicTimeout;
        r.mStopChaseDelay = tlv.mStopChaseDelay;
        r.mTimeToWaitBeforeChase = tlv.mTimeToWaitBeforeChase;
        r.mSligBoundId = tlv.mSligBoundId;
        r.mAlertedListenTime = tlv.mAlertedListenTime;
        r.mPercentSayWhat = tlv.mPercentSayWhat;
        r.mPercentBeatMud = tlv.mPercentBeatMud;
        r.mZShootDelay = tlv.mZShootDelay;
        r.mStayAwake = relive::From(tlv.mStayAwake);
        r.mDisabledResources = tlv.mDisabledResources;
        r.mNoiseWakeUpDistance = tlv.mNoiseWakeUpDistance;
        r.mSligSpawnerSwitchId = tlv.mSligSpawnerSwitchId;
        r.mUnlimitedSpawns = relive::From(tlv.mUnlimitedSpawns);
        return r;
    }

private:
    static Path_Slig::StartState From(AO::Path_Slig::StartState startState)
    {
        switch (startState)
        {
        case AO::Path_Slig::StartState::Listening_0:
            return Path_Slig::StartState::Listening;
        case AO::Path_Slig::StartState::Patrol_1:
            return Path_Slig::StartState::Patrol;
        case AO::Path_Slig::StartState::Sleeping_2:
            return Path_Slig::StartState::Sleeping;
        case AO::Path_Slig::StartState::Chase_3:
            return Path_Slig::StartState::Chase;
        case AO::Path_Slig::StartState::ChaseAndDisappear_4:
            return Path_Slig::StartState::ChaseAndDisappear;
        case AO::Path_Slig::StartState::FallingToChase_5:
            return Path_Slig::StartState::eFallingToChase;
        }
        ALIVE_FATAL("Bad slig start state");
    }

    static Path_Slig::StartState From(::Path_Slig::StartState startState)
    {
        switch (startState)
        {
        case ::Path_Slig::StartState::Listening_0:
            return Path_Slig::StartState::Listening;
        case ::Path_Slig::StartState::Patrol_1:
            return Path_Slig::StartState::Patrol;
        case ::Path_Slig::StartState::Sleeping_2:
            return Path_Slig::StartState::Sleeping;
        case ::Path_Slig::StartState::Chase_3:
            return Path_Slig::StartState::Chase;
        case ::Path_Slig::StartState::ChaseAndDisappear_4:
            return Path_Slig::StartState::ChaseAndDisappear;
        case ::Path_Slig::StartState::ListeningToGlukkon_6:
            return Path_Slig::StartState::ListeningToGlukkon;
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
    static Path_TrainDoor From(const ::Path_TrainDoor& tlv)
    {
        Path_TrainDoor r;
        BaseConvert(r, tlv);
        r.mDirection = relive::From(tlv.field_10_direction);
        return r;
    }
};

class Path_TorturedMudokon_Converter final
{
public:
    static Path_TorturedMudokon From(const ::Path_TorturedMudokon& tlv)
    {
        Path_TorturedMudokon r;
        BaseConvert(r, tlv);
        r.mKillSwitchId = tlv.mKillSwitchId;
        r.mReleaseSwitchId = tlv.mReleaseSwitchId;
        return r;
    }
};

class Path_DoorBlocker_Converter final
{
public:
    static Path_DoorBlocker From(const ::Path_DoorBlocker& tlv)
    {
        Path_DoorBlocker r;
        BaseConvert(r, tlv);
        r.mScale = relive::From(tlv.field_10_scale);
        r.mSwitchId = tlv.field_12_switch_id;
        return r;
    }
};

class Path_GlukkonSwitch_Converter final
{
public:
    static Path_GlukkonSwitch From(const ::Path_GlukkonSwitch& tlv)
    {
        Path_GlukkonSwitch r;
        BaseConvert(r, tlv);
        r.field_10_scale = From(tlv.field_10_scale);
        r.field_12_ok_switch_id = tlv.field_12_ok_switch_id;
        r.field_14_fail_switch_id = tlv.field_14_fail_switch_id;
        r.field_16_xpos = tlv.field_16_xpos;
        r.field_18_ypos = tlv.field_18_ypos;
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
    static Path_Greeter From(const ::Path_Greeter& tlv)
    {
        Path_Greeter r;
        BaseConvert(r, tlv);
        r.mScale = relive::From(tlv.field_10_scale);
        r.mMotionDetectorSpeed = tlv.field_12_motion_detector_speed;
        r.mFacing = relive::From(tlv.field_14_start_direction);
        return r;
    }
};

class Path_BrewMachine_Converter final
{
public:
    static Path_BrewMachine From(const ::Path_BrewMachine& tlv)
    {
        Path_BrewMachine r;
        BaseConvert(r, tlv);
        r.mBrewCount = tlv.mBrewCount;
        return r;
    }
};

class Path_Alarm_Converter final
{
public:
    static Path_Alarm From(const ::Path_Alarm& tlv)
    {
        Path_Alarm r;
        BaseConvert(r, tlv);
        r.mSwitchId = tlv.mSwitchId;
        r.mAlarmDuration = tlv.mAlarmDuration;
        return r;
    }
};

class Path_ParamiteWebLine_Converter final
{
public:
    static Path_ParamiteWebLine From(const ::Path_ParamiteWebLine& tlv)
    {
        Path_ParamiteWebLine r;
        BaseConvert(r, tlv);
        r.mScale = relive::From(tlv.field_10_scale);
        return r;
    }
};

class Path_SlapLock_Converter final
{
public:
    static Path_SlapLock From(const ::Path_SlapLock& tlv)
    {
        Path_SlapLock r;
        BaseConvert(r, tlv);
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
    static Path_StatusLight From(const ::Path_StatusLight& tlv)
    {
        Path_StatusLight r;
        BaseConvert(r, tlv);
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
    static Path_MultiSwitchController From(const ::Path_MultiSwitchController& tlv)
    {
        Path_MultiSwitchController r;
        BaseConvert(r, tlv);
        r.mOutputSwitchId = r.mOutputSwitchId;
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
    static Path_ExplosionSet From(const ::Path_ExplosionSet& tlv)
    {
        Path_ExplosionSet r;
        BaseConvert(r, tlv);
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
    static Path_BoneBag From(const ::Path_BoneBag& tlv)
    {
        Path_BoneBag r;
        BaseConvert(r, tlv);
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
    static Path_MineCar From(const ::Path_MineCar& tlv)
    {
        Path_MineCar r;
        BaseConvert(r, tlv);
        r.mScale = relive::From(tlv.mScale);
        return r;
    }
};

class Path_ColourfulMeter_Converter final
{
public:
    static Path_ColourfulMeter From(const ::Path_ColourfulMeter& tlv)
    {
        Path_ColourfulMeter r;
        BaseConvert(r, tlv);
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
    static Path_DemoSpawnPoint From(const ::Path_DemoSpawnPoint& tlv)
    {
        Path_DemoSpawnPoint r;
        BaseConvert(r, tlv);
        return r;
    }
};

class Path_LevelLoader_Converter final
{
public:
    static Path_LevelLoader From(const ::Path_LevelLoader& tlv)
    {
        Path_LevelLoader r;
        BaseConvert(r, tlv);
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
    static Path_SlamDoor From(const ::Path_SlamDoor& tlv)
    {
        Path_SlamDoor r;
        BaseConvert(r, tlv);
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
    static Path_Slurg From(const ::Path_Slurg& tlv)
    {
        Path_Slurg r;
        BaseConvert(r, tlv);
        r.mMovingTimer = tlv.mSlurgData.mMovingTimer;
        r.mFacing = relive::From(tlv.mSlurgData.mFacing);
        r.mScale = relive::From(tlv.mSlurgData.mScale);
        r.mSwitchId = tlv.mSlurgData.mSwitchId;
        return r;
    }
};

class Path_LaughingGas_Converter final
{
public:
    static Path_LaughingGas From(const ::Path_LaughingGas& tlv)
    {
        Path_LaughingGas r;
        BaseConvert(r, tlv);
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
    static Path_WorkWheel From(const ::Path_WorkWheel& tlv)
    {
        Path_WorkWheel r;
        BaseConvert(r, tlv);
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
    static Path_Water From(const ::Path_Water& tlv)
    {
        Path_Water r;
        BaseConvert(r, tlv);
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
    static Path_WheelSyncer From(const ::Path_WheelSyncer& tlv)
    {
        Path_WheelSyncer r;
        BaseConvert(r, tlv);
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
    static Path_WheelSyncer::OutputRequirement From(::WheelSyncerOutputRequirement requirement)
    {
        switch (requirement)
        {
            case WheelSyncerOutputRequirement::eAllOn_0:
                return Path_WheelSyncer::OutputRequirement::eAllOn;
            case WheelSyncerOutputRequirement::e1OnAnd2Off_1:
                return Path_WheelSyncer::OutputRequirement::e1OnAnd2Off;
            case WheelSyncerOutputRequirement::e1Or2On_2:
                return Path_WheelSyncer::OutputRequirement::e1Or2On;
            case WheelSyncerOutputRequirement::e1OnOr2Off_3:
                return Path_WheelSyncer::OutputRequirement::e1OnOr2Off;
        }
        ALIVE_FATAL("Bad wheel syncer output requirement");
    }
};

class Path_Fleech_Converter final
{
public:
    static Path_Fleech From(const ::Path_Fleech& tlv)
    {
        Path_Fleech r;
        BaseConvert(r, tlv);
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
    static Path_SlurgSpawner From(const ::Path_SlurgSpawner& tlv)
    {
        Path_SlurgSpawner r;
        BaseConvert(r, tlv);
        r.mSpawnInterval = tlv.mSpawnerData.mSpawnInterval;
        r.mMaxSlurgs = tlv.mSpawnerData.mMaxSlurgs;
        r.mSwitchId = tlv.mSpawnerData.mSwitchId;
        return r;
    }
};

class Path_Drill_Converter final
{
public:
    static Path_Drill From(const ::Path_Drill& tlv)
    {
        Path_Drill r;
        BaseConvert(r, tlv);
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
    static Path_Drill::DrillDirection From(::DrillDirection direction)
    {
        switch (direction)
        {
            case ::DrillDirection::eDown_0:
                return Path_Drill::DrillDirection::eDown;
            case ::DrillDirection::eRight_1:
                return Path_Drill::DrillDirection::eRight;
            case ::DrillDirection::eLeft_2:
                return Path_Drill::DrillDirection::eLeft;
        }
        ALIVE_FATAL("Bad drill direction");
    }

    static Path_Drill::DrillBehavior From(::DrillBehavior behavior)
    {
        switch (behavior)
        {
            case ::DrillBehavior::eNotInteractable_0:
                return Path_Drill::DrillBehavior::eNotInteractable;
            case ::DrillBehavior::eToggle_1:
                return Path_Drill::DrillBehavior::eToggle;
            case ::DrillBehavior::eUse_2:
                return Path_Drill::DrillBehavior::eUse;
        }
        ALIVE_FATAL("Bad drill behavior");
    }
};

class Path_Teleporter_Converter final
{
public:
    static Path_Teleporter From(const ::Path_Teleporter& tlv)
    {
        Path_Teleporter r;
        BaseConvert(r, tlv);
        r.mTeleporterId = tlv.field_10_data.mTeleporterId;
        r.mOtherTeleporterId = tlv.field_10_data.mOtherTeleporterId;
        r.mDestCamera = tlv.field_10_data.mDestCamera;
        r.mDestPath = tlv.field_10_data.mDestPath;
        r.mDestLevel = MapWrapper::FromAE(tlv.field_10_data.mDestLevel);
        r.mSwitchId = tlv.field_10_data.mSwitchId;
        r.mScale = relive::From(tlv.field_10_data.mScale);
        r.mWipeEffect = relive::From(tlv.field_10_data.mWipeEffect);
        r.mMovieId = tlv.field_10_data.mMovieId;
        r.mElectricX = tlv.field_10_data.mElectricX;
        r.mElectricY = tlv.field_10_data.mElectricY;
        return r;
    }
};

class Path_Glukkon_Converter final
{
public:
    static Path_Glukkon From(const ::Path_Glukkon& tlv)
    {
        Path_Glukkon r;
        BaseConvert(r, tlv);
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

    static Path_Glukkon::GlukkonTypes From(::GlukkonTypes spawnType)
    {
        switch (spawnType)
        {
            case ::GlukkonTypes::eNormal_0:
            case ::GlukkonTypes::eNormal_4:
            case ::GlukkonTypes::eNormal_5:
                return Path_Glukkon::GlukkonTypes::eNormal;
            case ::GlukkonTypes::eStoryAslik_1:
                return Path_Glukkon::GlukkonTypes::eStoryAslik;
            case ::GlukkonTypes::eStoryDripik_2:
                return Path_Glukkon::GlukkonTypes::eStoryDripik;
            case ::GlukkonTypes::eStoryPhleg_3:
                return Path_Glukkon::GlukkonTypes::eStoryPhleg;
        }
        ALIVE_FATAL("Bad glukkon spawn type");
    }
};

class Path_CrawlingSligButton_Converter final
{
public:
    static Path_CrawlingSligButton From(const ::Path_CrawlingSligButton& tlv)
    {
        Path_CrawlingSligButton r;
        BaseConvert(r, tlv);
        r.mScale = relive::From(tlv.mScale);
        r.mSwitchId = tlv.mSwitchId;
        r.mAction = relive::From(tlv.mAction);
        r.mOnSound = From(tlv.mOnSound);
        r.mOffSound = From(tlv.mOffSound);
        r.mSoundDirection = tlv.mSoundDirection;
        return r;
    }
private:
    static Path_CrawlingSligButton::ButtonSounds From(::CrawlingSligButtonSounds sound)
    {
        switch (sound)
        {
            case ::CrawlingSligButtonSounds::None_0:
                return Path_CrawlingSligButton::ButtonSounds::None;
            case ::CrawlingSligButtonSounds::SackHit_1:
                return Path_CrawlingSligButton::ButtonSounds::SackHit;
            case ::CrawlingSligButtonSounds::FallingItemPresence2_2:
                return Path_CrawlingSligButton::ButtonSounds::FallingItemPresence2;
            case ::CrawlingSligButtonSounds::SecurityOrb_3:
                return Path_CrawlingSligButton::ButtonSounds::SecurityOrb;
            case ::CrawlingSligButtonSounds::SackHit_4:
                return Path_CrawlingSligButton::ButtonSounds::SackHit;
            case ::CrawlingSligButtonSounds::Bullet1_5:
                return Path_CrawlingSligButton::ButtonSounds::Bullet1;
            case ::CrawlingSligButtonSounds::AbeGenericMovement_6:
                return Path_CrawlingSligButton::ButtonSounds::AbeGenericMovement;
        }
        ALIVE_FATAL("Bad crawling slig button sound");
    }
};

class Path_FlyingSlig_Converter final
{
public:
    static Path_FlyingSlig From(const ::Path_FlyingSlig& tlv)
    {
        Path_FlyingSlig r;
        BaseConvert(r, tlv);
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
    static Path_FlyingSligSpawner From(const ::Path_FlyingSligSpawner& tlv)
    {
        Path_FlyingSligSpawner r;
        BaseConvert(r, tlv);
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
    static Path_ScrabSpawner From(const ::Path_ScrabSpawner& tlv)
    {
        Path_ScrabSpawner r;
        BaseConvert(r, tlv);
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
    static Path_ScrabSpawner::SpawnDirection From(::ScrabSpawnDirection spawnDirection)
    {
        switch (spawnDirection)
        {
            case ::ScrabSpawnDirection::eNone_0:
                return Path_ScrabSpawner::SpawnDirection::eNone;
            case ::ScrabSpawnDirection::eLeft_1:
                return Path_ScrabSpawner::SpawnDirection::eLeft;
            case ::ScrabSpawnDirection::eRight_2:
                return Path_ScrabSpawner::SpawnDirection::eRight;
        }
        ALIVE_FATAL("Bad scrab spawner spawn direction");
    }
};

class Path_CrawlingSlig_Converter final
{
public:
    static Path_CrawlingSlig From(const ::Path_CrawlingSlig& tlv)
    {
        Path_CrawlingSlig r;
        BaseConvert(r, tlv);
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
    static Path_SligGetWings From(const ::Path_SligGetWings& tlv)
    {
        Path_SligGetWings r;
        BaseConvert(r, tlv);
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
    static Path_SligGetPants From(const ::Path_SligGetPants& tlv)
    {
        Path_SligGetPants r;
        BaseConvert(r, tlv);
        r.mScale = relive::From(tlv.Scale);
        r.mStartState = Path_Slig_Converter::From(tlv.start_state);
        r.mPauseTime = tlv.pause_time;
        r.mPauseLeftMin = tlv.pause_left_min;
        r.mPauseLeftMax = tlv.pause_left_max;
        r.mPauseRightMin = tlv.pause_right_min;
        r.mPauseRightMax = tlv.pause_right_max;
        r.mShootPossessedSligs = Path_Slig_Converter::From(tlv.shoot_possessed_sligs);
        r.mShootOnSightDelay = tlv.shoot_on_sight_delay;
        r.mNumTimesToShoot = tlv.num_times_to_shoot;
        r.mCode1 = tlv.code1;
        r.mCode2 = tlv.code2;
        r.mChaseAbeWhenSpotted = relive::From(tlv.chase_abe_when_spotted);
        r.mFacing = relive::From(tlv.start_direction);
        r.mPanicTimeout = tlv.panic_timeout;
        r.mStopChaseDelay = tlv.stop_chase_delay;
        r.mTimeToWaitBeforeChase = tlv.time_to_wait_before_chase;
        r.mSligBoundId = tlv.slig_bound_id;
        r.mAlertedListenTime = tlv.alerted_listen_time;
        r.mPercentSayWhat = tlv.percent_say_what;
        r.mPercentBeatMud = tlv.percent_beat_mud;
        r.mZShootDelay = tlv.z_shoot_delay;
        r.mStayAwake = relive::From(tlv.stay_awake);
        r.mDisabledResources = tlv.disable_resources;
        r.mNoiseWakeUpDistance = tlv.noise_wake_up_distance;
        r.mSligSpawnerSwitchId = tlv.slig_spawner_switch_id;
        return r;
    }
};

} // namespace relive
