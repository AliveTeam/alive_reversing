#pragma once

#include "../../AliveLibAO/Slig_DisabledResources.hpp"
#include "../MapWrapper.hpp"
#include "../RGB16.hpp"
#include "../BitField.hpp"
#include "../Primitives.hpp"
#include "guid.hpp"
#include "../ObjectTypes.hpp"


namespace relive {


enum class reliveScale : s16
{
    eFull,
    eHalf,
};

enum class reliveChoice : s16
{
    eNo,
    eYes,
};

enum class reliveXDirection : s16
{
    eLeft,
    eRight,
};

enum class reliveSwitchOp : s16
{
    eSetTrue,
    eSetFalse,
    eToggle,
    eIncrement,
    eDecrement,
};

enum reliveScreenChangeEffects : s16
{
    ePlay1FMV,
    eRightToLeft,
    eLeftToRight,
    eBottomToTop,
    eTopToBottom,
    eBoxOut,
    eVerticalSplit,
    eHorizontalSplit,
    eUnknown_8,
    eInstantChange,
};


// TODO: A copy for now, should prob be bools or removed if possible
enum TlvFlags
{
    eBit1_Created = 0x1,
    eBit2_Destroyed = 0x2,
    eBit3_End_TLV_List = 0x4,
};

enum class QuiksaveAttribute
{
    eDoNothing_0 = 0,
    eClearTlvFlags_1 = 1,
    eKeepTlvFlags_2 = 2,
};

// TODO: Base type
class Path_TLV
{
public:
    // TODO: not sure if we need to store the type or length
    // if we are using json as the file format
    s16 mTopLeftX = 0;
    s16 mTopLeftY = 0;
    s16 mBottomRightX = 0;
    s16 mBottomRightY = 0;
    s8 mTlvSpecificMeaning = 0; // TODO: remove me and add to the actual tlv's instead
    ReliveTypes mTlvType = {};
    BitField8<TlvFlags> mTlvFlags = {};
    s32 mLength = 0;
    Guid mId;
    QuiksaveAttribute mAttribute = QuiksaveAttribute::eDoNothing_0;

    s16 Width() const
    {
        return mBottomRightX - mTopLeftX;
    }

    s16 Height() const
    {
        return mBottomRightY - mTopLeftY;
    }

    s16 MidPointX() const
    {
        return (mTopLeftX + mBottomRightX) / 2;
    }

    s16 MidPointY() const
    {
        return (mTopLeftY + mBottomRightY) / 2;
    }
};

class Path_ShadowZone final : public Path_TLV
{
public:
    Path_ShadowZone()
    {
        mTlvType = ReliveTypes::eShadowZone;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    enum class Scale : s16
    {
        eBoth,
        eHalf,
        eFull,
    };

    RGB16 mRGB;
    Scale mScale = Scale::eFull;
    static constexpr char kClassName[] = "shadow_zone";
};

class Path_SecurityOrb final : public Path_TLV
{
public:
    Path_SecurityOrb()
    {
        mTlvType = ReliveTypes::eSecurityOrb;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    reliveScale mScale = reliveScale::eFull;
    static constexpr char kClassName[] = "security_orb";
};

class Path_ContinuePoint final : public Path_TLV
{
public:
    Path_ContinuePoint()
    {
        mTlvType = ReliveTypes::eContinuePoint;
        mAttribute = QuiksaveAttribute::eKeepTlvFlags_2;
    }
    // AO fields
    s16 mZoneNumber = 0;
    s16 mClearFromId = 0;
    s16 mClearToId = 0;
    reliveChoice mElumRestarts = reliveChoice::eYes;
    enum class spawnDirection : s16
    {
        eRight,
        eLeft,
    };
    spawnDirection mAbeSpawnDir = spawnDirection::eRight;

    // AE fields
    enum class Scale : s16
    {
        eNone,
        eHalf,
        eFull,
    };
    Scale mScale = Scale::eFull;
    s16 mSaveFileId = 0;
    static constexpr char kClassName[] = "continue_point";
};

struct Path_LiftPoint final : public Path_TLV
{
    Path_LiftPoint()
    {
        mTlvType = ReliveTypes::eLiftPoint;
        mAttribute = QuiksaveAttribute::eKeepTlvFlags_2;
    }
    s16 mLiftPointId = 0;
    reliveChoice mIsStartPoint = reliveChoice::eNo;
    enum class LiftPointStopType : s16
    {
        eTopFloor,
        eBottomFloor,
        eMiddleFloor,
        eMiddleLockFloor,
        eStartPointOnly,
    };
    LiftPointStopType mLiftPointStopType = LiftPointStopType::eTopFloor;
    reliveScale mScale = reliveScale::eFull;
    reliveChoice mIgnoreLiftMover = reliveChoice::eNo;
    static constexpr char kClassName[] = "lift_point";
};

struct Path_Dove final : public Path_TLV
{
    Path_Dove()
    {
        mTlvType = ReliveTypes::eDove;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    s16 mDoveCount = 0;
    reliveChoice mPixelPerfect = reliveChoice::eNo;
    reliveScale mScale = reliveScale::eFull;
    static constexpr char kClassName[] = "dove";
};

struct Path_RockSack final : public Path_TLV
{
    Path_RockSack()
    {
        mTlvType = ReliveTypes::eRockSack;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    reliveXDirection mRockFallDirection = reliveXDirection::eLeft;
    u16 mVelX = 0;
    u16 mVelY = 0;
    reliveScale mScale = reliveScale::eFull;
    s16 mRockAmount = 0;
    static constexpr char kClassName[] = "rock_sack";
};

struct Path_ZBall final : public Path_TLV
{
    Path_ZBall()
    {
        mTlvType = ReliveTypes::eZBall;
    }
    enum class StartPos : s16
    {
        eCenter,
        eOut,
        eIn,
    };

    enum class Speed : s16
    {
        eNormal,
        eFast,
        eSlow,
    };

    StartPos mStartPos = StartPos::eCenter;
    reliveScale mScale = reliveScale::eFull;
    Speed mSpeed = Speed::eNormal;
    static constexpr char kClassName[] = "z_ball";
};

struct Path_FallingItem final : public Path_TLV
{
    Path_FallingItem()
    {
        mTlvType = ReliveTypes::eFallingItem;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    s16 mSwitchId = 0;
    reliveScale mScale = reliveScale::eFull;
    s16 mFallInterval = 0;
    s16 mMaxFallingItems = 0;
    reliveChoice mResetSwitchIdAfterUse = reliveChoice::eYes;
    static constexpr char kClassName[] = "falling_item";
};

struct Path_PullRingRope final : public Path_TLV
{
    Path_PullRingRope()
    {
        mTlvType = ReliveTypes::ePullRingRope;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    enum class PullRingSwitchSound : s16
    {
        eNone,
        eWellExit,
        eRingBellHammer,
        eDoorEffect,
    };

    enum class PullRingSoundDirection : s16
    {
        eLeftAndRight,
        eLeft,
        eRight,
    };

    s16 mSwitchId = 0;
    reliveSwitchOp mAction = reliveSwitchOp::eSetTrue;
    u16 mRopeLength = 0;
    reliveScale mScale = reliveScale::eFull;
    PullRingSwitchSound mOnSound = PullRingSwitchSound::eNone;
    PullRingSwitchSound mOffSound = PullRingSwitchSound::eNone;
    PullRingSoundDirection mSoundDirection = PullRingSoundDirection::eLeftAndRight;
    static constexpr char kClassName[] = "pull_ring_rope";
};

struct Path_TimedMine final : public Path_TLV
{
    Path_TimedMine()
    {
        mTlvType = ReliveTypes::eTimedMine;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    reliveScale mScale = reliveScale::eFull;
    s16 mTicksUntilExplosion = 0;
    static constexpr char kClassName[] = "timed_mine";
};

struct Path_Hoist final : public Path_TLV
{
    Path_Hoist()
    {
        mTlvType = ReliveTypes::eHoist;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    enum class Type : s16
    {
        eNextFloor,
        eOffScreen,
    };
    Type mHoistType = Type::eNextFloor;

    enum class GrabDirection : s16
    {
        eFacingLeft,
        eFacingRight,
        eFacingAnyDirection,
    };
    GrabDirection mGrabDirection = GrabDirection::eFacingLeft;

    // AE only
    reliveScale mScale = reliveScale::eFull;
    static constexpr char kClassName[] = "hoist";
};

struct Path_TrapDoor final : public Path_TLV
{
    Path_TrapDoor()
    {
        mTlvType = ReliveTypes::eTrapDoor;
        mAttribute = QuiksaveAttribute::eKeepTlvFlags_2;
    }
    s16 mSwitchId = 0;
    enum StartState : s16
    {
        eOpen,
        eClosed,
    };
    StartState mStartState = StartState::eClosed;
    reliveChoice mSelfClosing = reliveChoice::eNo;
    reliveScale mScale = reliveScale::eFull;
    reliveXDirection mDirection = reliveXDirection::eLeft;
    s16 mXOff = 0;

    // AE only
    s16 mStayOpenTime = 0;
    static constexpr char kClassName[] = "trap_door";
};

struct Path_LCDScreen final : public Path_TLV
{
    Path_LCDScreen()
    {
        mTlvType = ReliveTypes::eLCDScreen;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    s16 mMessageId1 = 0;
    s16 mMessageRandMinId = 0;
    s16 mMessageRandMaxId = 0;

    // AE only
    s16 mMessageId2 = 0;
    s32 mToggleMessageSwitchId = 0;
    static constexpr char kClassName[] = "lcd_screen";
};

struct Path_Mine final : public Path_TLV
{
    Path_Mine()
    {
        mTlvType = ReliveTypes::eMine;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    reliveScale mScale = reliveScale::eFull;
    reliveChoice mPersistOffscreen = reliveChoice::eYes;
    static constexpr char kClassName[] = "mine";
};

struct Path_InvisibleSwitch final : public Path_TLV
{
    Path_InvisibleSwitch()
    {
        mTlvType = ReliveTypes::eInvisibleSwitch;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    s16 mSwitchId = 0;
    reliveSwitchOp mAction = reliveSwitchOp::eSetTrue;
    s16 mActivationDelay = 0;
    reliveChoice mSetOffAlarm = reliveChoice::eNo;
    enum class InvisibleSwitchScale : s16
    {
        eHalf,
        eFull,
        eAny,
    };
    InvisibleSwitchScale mScale = InvisibleSwitchScale::eFull;
    static constexpr char kClassName[] = "invisible_switch";
};

struct Path_ElectricWall final : public Path_TLV
{
    Path_ElectricWall()
    {
        mTlvType = ReliveTypes::eElectricWall;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    reliveScale mScale = reliveScale::eFull;
    s16 mSwitchId = 0;
    enum ElectricWallStartState : s16
    {
        eOff,
        eOn,
    };
    ElectricWallStartState mStartState = ElectricWallStartState::eOn;
    static constexpr char kClassName[] = "electric_wall";
};

struct Path_BoomMachine final : public Path_TLV
{
    Path_BoomMachine()
    {
        mTlvType = ReliveTypes::eBoomMachine;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    reliveScale mScale = reliveScale::eFull;
    enum class NozzleSide : s16
    {
        eRight,
        eLeft,
    };
    NozzleSide mNozzleSide = NozzleSide::eRight;
    s16 mGrenadeAmount = 0;
    static constexpr char kClassName[] = "boom_machine";
};

struct Path_UXB final : public Path_TLV
{
    Path_UXB()
    {
        mTlvType = ReliveTypes::eUXB;
        mAttribute = QuiksaveAttribute::eKeepTlvFlags_2;
    }
    enum class StartState : s16
    {
        eOn,
        eOff,
    };
    s16 mPatternLength = 0;
    s16 mPattern = 0;
    reliveScale mScale = reliveScale::eFull;
    StartState mStartState = StartState::eOn;
    static constexpr char kClassName[] = "uxb";
};

struct Path_MeatSaw final : public Path_TLV
{
    Path_MeatSaw()
    {
        mTlvType = ReliveTypes::eMeatSaw;
    }
    reliveScale mScale = reliveScale::eFull;
    s16 mSwitchMinTimeOff = 0;
    s16 mSwitchMaxTimeOff = 0;
    s16 mYTravelDistance = 0;
    s16 mSwitchId = 0;
    enum class Type : s16
    {
        eAutomaticPersistOffscreen,
        eAutomatic,
        eSwitchId,
    };
    Type mType = Type::eAutomatic;
    s16 mSwitchSpeed = 0;
    enum class StartState : s16
    {
        eOff,
        eOn,
    };
    StartState mStartState = StartState::eOn;
    s16 mAutomaticSpeed = 0;
    s16 mAutomaticMinTimeOff = 0;
    s16 mAutomaticMaxTimeOff = 0;
    s16 mStartAtBottom = 0;
    static constexpr char kClassName[] = "meat_saw";
};

struct Path_Lever final : public Path_TLV
{
    Path_Lever()
    {
        mTlvType = ReliveTypes::eLever;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    enum class LeverSoundType : s16
    {
        eNone,
        eWell,
        eSwitchBellHammer,
        eDoor,
        eElectricWall,
        eSecurityOrb,
        eLift, // AE only
    };

    enum class LeverSoundDirection : s16
    {
        eLeftAndRight,
        eLeft,
        eRight,
    };

    reliveSwitchOp mAction = reliveSwitchOp::eSetTrue;
    reliveScale mScale = reliveScale::eFull;
    LeverSoundType mOnSound = LeverSoundType::eNone;
    LeverSoundType mOffSound = LeverSoundType::eNone;
    LeverSoundDirection mSoundDirection = LeverSoundDirection::eLeftAndRight;
    s16 mSwitchId = 0;

    // AE only
    reliveChoice mPersistOffscreen = reliveChoice::eYes;
    static constexpr char kClassName[] = "lever";
};

struct Path_Edge final : public Path_TLV
{
    Path_Edge()
    {
        mTlvType = ReliveTypes::eEdge;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    enum class GrabDirection : s16
    {
        eFacingLeft,
        eFacingRight,
        eFacingAnyDirection,
    };
    GrabDirection mGrabDirection = GrabDirection::eFacingRight;
    reliveChoice mCanGrab = reliveChoice::eYes;

    // AE only
    reliveScale mScale = reliveScale::eFull;
    static constexpr char kClassName[] = "edge";
};

struct Path_BirdPortal final : public Path_TLV
{
    Path_BirdPortal()
    {
        mTlvType = ReliveTypes::eBirdPortal;
        mAttribute = QuiksaveAttribute::eKeepTlvFlags_2;
    }
    enum class PortalType : s16
    {
        eAbe,
        eWorker,
        eShrykull,
        eMudTeleport,
    };

    // used for Path_BirdPortal and Path_BirdPortalExit
    enum class PortalSide : s16
    {
        eRight,
        eLeft,
    };

    PortalSide mEnterSide = PortalSide::eRight;
    EReliveLevelIds mExitLevel = EReliveLevelIds::eNone;
    s16 mExitPath = 0;
    s16 mExitCamera = 0;
    reliveScale mScale = reliveScale::eFull;
    s16 mMovieId = 0;
    PortalType mPortalType = PortalType::eWorker;
    s16 mMudCountForShrykull = 0;

    // AE only
    s16 mCreatePortalSwitchId = 0;
    s16 mDeletePortalSwitchId = 0;
    static constexpr char kClassName[] = "bird_portal";
};

struct Path_BirdPortalExit final : public Path_TLV
{
    Path_BirdPortalExit()
    {
        mTlvType = ReliveTypes::eBirdPortalExit;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    Path_BirdPortal::PortalSide mExitSide = Path_BirdPortal::PortalSide::eRight;
    reliveScale mScale = reliveScale::eFull;
    static constexpr char kClassName[] = "bird_portal_exit";
};

struct Path_LightEffect final : public Path_TLV
{
    Path_LightEffect()
    {
        mTlvType = ReliveTypes::eLightEffect;
        mAttribute = QuiksaveAttribute::eDoNothing_0; // assuming this is TlvType light  
    }
    enum class Type : s16
    {
        Star,
        GoldGlow,
        GreenGlow,
        FlintGlow,
        Switchable_RedGreenDoorLights,
        Switchable_RedGreenHubLight,
    };
    Type mType = Type::GreenGlow;
    s16 mSize = 0;
    s16 mSwitchId = 0;
    reliveXDirection mDirection = reliveXDirection::eRight;
    static constexpr char kClassName[] = "light_effect";
};

struct Path_MusicTrigger final : public Path_TLV
{
    Path_MusicTrigger()
    {
        mTlvType = ReliveTypes::eMusicTrigger;
        mAttribute = QuiksaveAttribute::eKeepTlvFlags_2;
    }
    enum class MusicTriggerMusicType : s16
    {
        eDrumAmbience,
        eDeathDrumShort,
        eSecretAreaLong,
        eSoftChase,
        eIntenseChase,
        eChime,
        eSecretAreaShort,
    };

    enum class TriggeredBy : s16
    {
        eTimer,
        eTouching,

        // AO only
        eSwitchId,
        eUnknown,
    };
    MusicTriggerMusicType mMusicType = MusicTriggerMusicType::eSecretAreaLong;
    TriggeredBy mTriggeredBy = TriggeredBy::eTouching;
    s16 mSwitchId = 0; // AO only
    s16 mMusicDelay = 0;
    static constexpr char kClassName[] = "music_trigger";
};

struct Path_SoftLanding final : public Path_TLV
{
    Path_SoftLanding()
    {
        mTlvType = ReliveTypes::eSoftLanding;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    // AE only
    s32 mSwitchId = 0;
    static constexpr char kClassName[] = "soft_landing";
};

struct Path_LiftMover final : public Path_TLV
{
    Path_LiftMover()
    {
        mTlvType = ReliveTypes::eLiftMover;
        mAttribute = QuiksaveAttribute::eKeepTlvFlags_2;
    }
    s16 mLiftMoverSwitchId = 0;
    s16 mTargetLiftPointId = 0;
    enum class YDirection : s16
    {
        eDown,
        eUp,
    };
    YDirection mMoveDirection = YDirection::eDown;
    static constexpr char kClassName[] = "lift_mover";
};

struct Path_HintFly final : public Path_TLV
{
    Path_HintFly()
    {
        mTlvType = ReliveTypes::eHintFly;
    }
    s16 mMessageId = 0;
    static constexpr char kClassName[] = "hint_fly";
};

struct Path_TimerTrigger final : public Path_TLV
{
    Path_TimerTrigger()
    {
        mTlvType = ReliveTypes::eTimerTrigger;
        mAttribute = QuiksaveAttribute::eKeepTlvFlags_2;
    }
    s16 mInputSwitchId = 0;
    u16 mActivationDelay = 0;
    s16 mOutputSwitchId1 = 0;
    s16 mOutputSwitchId2 = 0;
    s16 mOutputSwitchId3 = 0;
    s16 mOutputSwitchId4 = 0;
    static constexpr char kClassName[] = "timer_trigger";
};

struct Path_FlintLockFire final : public Path_TLV
{
    Path_FlintLockFire()
    {
        mTlvType = ReliveTypes::eFlintLockFire;
    }
    reliveScale mScale = reliveScale::eFull;
    s16 mSwitchId = 0;
    static constexpr char kClassName[] = "flint_lock_fire";
};

struct Path_HoneySack final : public Path_TLV
{
    Path_HoneySack()
    {
        mTlvType = ReliveTypes::eHoneySack;
    }
    s16 mChaseTime = 0;
    reliveScale mScale = reliveScale::eFull;
    static constexpr char kClassName[] = "honey_sack";
};

struct Path_Bat final : public Path_TLV
{
    Path_Bat()
    {
        mTlvType = ReliveTypes::eBat;
    }
    s16 mTimeBeforeMoving = 0;
    s16 mSpeed = 0;
    reliveScale mScale = reliveScale::eFull;
    s16 mAttackDuration = 0;
    static constexpr char kClassName[] = "bat";
};

struct Path_RollingBallStopper final : public Path_TLV
{
    Path_RollingBallStopper()
    {
        mTlvType = ReliveTypes::eRollingBallStopper;
    }
    s16 mStopperSwitchId = 0;
    reliveScale mScale = reliveScale::eFull;
    s16 mBallSwitchId = 0;
    reliveXDirection mStopDirection = reliveXDirection::eRight;
    static constexpr char kClassName[] = "rolling_ball_stopper";
};

struct Path_RollingBall final : public Path_TLV
{
    Path_RollingBall()
    {
        mTlvType = ReliveTypes::eRollingBall;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    reliveScale mScale = reliveScale::eFull;
    reliveXDirection mRollDirection = reliveXDirection::eRight;
    s16 mReleaseSwitchId = 0;
    u16 mMaxSpeed = 0;
    u16 mAcceleration = 0;
    static constexpr char kClassName[] = "rolling_ball";
};

struct Path_MotionDetector final : public Path_TLV
{
    Path_MotionDetector()
    {
        mTlvType = ReliveTypes::eMotionDetector;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    reliveScale mScale = reliveScale::eFull;
    s16 mDeviceX = 0;
    s16 mDeviceY = 0;
    s16 mSpeedx256 = 0;
    enum class InitialMoveDirection : s16
    {
        eRight,
        eLeft,
    };
    InitialMoveDirection mInitialMoveDirection = InitialMoveDirection::eRight;
    reliveChoice mDrawFlare = reliveChoice::eNo;
    s16 mDisableSwitchId = 0;
    s16 mAlarmSwitchId = 0;
    s16 mAlarmDuration = 0;
    static constexpr char kClassName[] = "motion_detector";
};

struct Path_BellHammer final : public Path_TLV
{
    Path_BellHammer()
    {
        mTlvType = ReliveTypes::eBellHammer;
    }
    s16 mSwitchId = 0;
    reliveSwitchOp mAction = reliveSwitchOp::eSetTrue;
    reliveScale mScale = reliveScale::eFull;
    reliveXDirection mDirection = reliveXDirection::eRight;
    static constexpr char kClassName[] = "bell_hammer";
};

struct Path_SligBound : public Path_TLV
{
    s16 mSligBoundId = 0;
    
    // TODO: Remove me after disabled resources removed
    BitField16<AO::SligFlags_DisabledRes> mDisabledResourcesAO;
    s16 mDisabledResourcesAE = 0;
};

struct Path_SligBoundLeft final : public Path_SligBound
{
    Path_SligBoundLeft()
    {
        mTlvType = ReliveTypes::eSligBoundLeft;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    // Empty
    static constexpr char kClassName[] = "slig_bound_left";
};

struct Path_SligBoundRight final : public Path_SligBound
{
    Path_SligBoundRight()
    {
        mTlvType = ReliveTypes::eSligBoundRight;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    // Empty
    static constexpr char kClassName[] = "slig_bound_right";
};

struct Path_SligPersist final : public Path_SligBound
{
    Path_SligPersist()
    {
        mTlvType = ReliveTypes::eSligPersist;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    // Empty
    static constexpr char kClassName[] = "slig_persist";
};

struct Path_BackgroundAnimation final : public Path_TLV
{
    Path_BackgroundAnimation()
    {
        mTlvType = ReliveTypes::eBackgroundAnimation;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    u16 mAnimId = 0;
    reliveChoice mIsSemiTrans = reliveChoice::eNo;
    enum class BgAnimSounds : s16
    {
        eNone,
        eFire,
    };

    relive::TBlendModes mSemiTransMode = relive::TBlendModes::eBlend_0;

    // AO only
    BgAnimSounds mSoundEffect = BgAnimSounds::eNone;

    // AE only
    enum class Layer : s16
    {
        eLayer0 = 0,
        eLayer2 = 2,
    };
    Layer mLayer = Layer::eLayer0;
    static constexpr char kClassName[] = "background_animation";
};

struct Path_MainMenuController final : public Path_TLV
{
    // No fields
    Path_MainMenuController()
    {
        mTlvType = ReliveTypes::eMainMenuController;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    static constexpr char kClassName[] = "main_menu_controller";
};

struct Path_ElumWall final : public Path_TLV
{
    // No fields
    Path_ElumWall()
    {
        mTlvType = ReliveTypes::eElumWall;
    }
    static constexpr char kClassName[] = "elum_wall";
};

struct Path_ElumStart final : public Path_TLV
{
    // No fields
    Path_ElumStart()
    {
        mTlvType = ReliveTypes::eElumStart;
    }
    static constexpr char kClassName[] = "elum_start";
};

struct Path_Elum final : public Path_TLV
{
    // No fields
    Path_Elum()
    {
        mTlvType = ReliveTypes::eElum;
    }
    static constexpr char kClassName[] = "elum";
};

struct Path_KillUnsavedMuds final : public Path_TLV
{
    // No fields
    Path_KillUnsavedMuds()
    {
        mTlvType = ReliveTypes::eKillUnsavedMuds;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    static constexpr char kClassName[] = "kill_unsaved_muds";
};

struct Path_InvisibleZone final : public Path_TLV
{
    // No fields
    Path_InvisibleZone()
    {
        mTlvType = ReliveTypes::eInvisibleZone;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    static constexpr char kClassName[] = "invisible_zone";
};

struct Path_StartController final : public Path_TLV
{
    // No fields
    Path_StartController()
    {
        mTlvType = ReliveTypes::eStartController;
    }
    static constexpr char kClassName[] = "start_controller";
};

struct Path_ScrabNoFall final : public Path_TLV
{
    // No fields
    Path_ScrabNoFall()
    {
        mTlvType = ReliveTypes::eScrabNoFall;
    }
    static constexpr char kClassName[] = "scrab_no_fall";
};

struct Path_ScrabBoundLeft final : public Path_TLV
{
    // No fields
    Path_ScrabBoundLeft()
    {
        mTlvType = ReliveTypes::eScrabLeftBound;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    static constexpr char kClassName[] = "scrab_bound_left";
};

struct Path_ScrabBoundRight final : public Path_TLV
{
    // No fields
    Path_ScrabBoundRight()
    {
        mTlvType = ReliveTypes::eScrabRightBound;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    static constexpr char kClassName[] = "scrab_bound_right";
};

struct Path_HoneyDripTarget final : public Path_TLV
{
    // No fields
    Path_HoneyDripTarget()
    {
        mTlvType = ReliveTypes::eHoneyDripTarget;
    }
    static constexpr char kClassName[] = "honey_drip_target";
};

struct Path_ZSligCover final : public Path_TLV
{
    // No fields
    Path_ZSligCover()
    {
        mTlvType = ReliveTypes::eZSligCover;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    static constexpr char kClassName[] = "z_slig_cover";
};

struct Path_DeathDrop final : public Path_TLV
{
    // No fields
    Path_DeathDrop()
    {
        mTlvType = ReliveTypes::eDeathDrop;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    static constexpr char kClassName[] = "death_drop";
};

struct Path_ChimeLock final : public Path_TLV
{
    Path_ChimeLock()
    {
        mTlvType = ReliveTypes::eChimeLock;
    }
    reliveScale mScale = reliveScale::eFull;
    s16 mSolveSwitchId = 0;
    s16 mCode1 = 0;
    s16 mCode2 = 0;
    s16 mPasswordSwitchId = 0;
    static constexpr char kClassName[] = "chime_lock";
};

struct Path_LCDStatusBoard final : public Path_TLV
{
    Path_LCDStatusBoard()
    {
        mTlvType = ReliveTypes::eLCDStatusBoard;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    // AO has no fields
    s16 mNumberOfMuds = 0;
    s16 mZulagNumber = 0;
    reliveChoice mHideBoard = reliveChoice::eNo;
    static constexpr char kClassName[] = "lcd_status_board";
};

struct Path_CreditsController final : public Path_TLV
{
    // Empty
    Path_CreditsController()
    {
        mTlvType = ReliveTypes::eCreditsController;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    static constexpr char kClassName[] = "credits_controller";
};

struct Path_ResetPath final : public Path_TLV
{
    Path_ResetPath()
    {
        mTlvType = ReliveTypes::eResetPath;
        mAttribute = QuiksaveAttribute::eKeepTlvFlags_2;
    }
    reliveChoice mClearIds = reliveChoice::eYes;
    s16 mFrom = 0;
    s16 mTo = 0;
    s16 mExclude = 0;
    reliveChoice mClearObjects = reliveChoice::eYes;
    s16 mPath = 0;

    // AE only
    reliveChoice mEnabled = reliveChoice::eYes;
    static constexpr char kClassName[] = "reset_path";
};

struct Path_MeatSack final : public Path_TLV
{
    Path_MeatSack()
    {
        mTlvType = ReliveTypes::eMeatSack;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    reliveXDirection mMeatFallDirection = reliveXDirection::eRight;
    u16 mVelX = 0;
    u16 mVelY = 0;
    reliveScale mScale = reliveScale::eFull;
    s16 mMeatAmount = 0;
    static constexpr char kClassName[] = "meat_sack";
};

struct Path_BeeNest final : public Path_TLV
{
    Path_BeeNest()
    {
        mTlvType = ReliveTypes::eBeeNest;
    }
    s16 mSwitchId = 0;
    s16 mSwarmSize = 0;
    s16 mChaseTime = 0;
    s16 mSpeed = 0;
    s16 mBeesAmount = 0;
    static constexpr char kClassName[] = "bee_nest";
};

struct Path_BellsongStone final : public Path_TLV
{
    Path_BellsongStone()
    {
        mTlvType = ReliveTypes::eBellSongStone;
    }
    enum class BellsongTypes : s16
    {
        eWhistle,
        eChimes
    };
    reliveScale mScale = reliveScale::eFull;
    BellsongTypes mType = BellsongTypes::eWhistle;
    s16 mCode1 = 0;
    s16 mCode2 = 0;
    s16 mSwitchId = 0;
    static constexpr char kClassName[] = "bellsong_stone";
};

struct Path_MovieStone final : public Path_TLV
{
    Path_MovieStone()
    {
        mTlvType = ReliveTypes::eMovieHandStone;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    s16 mMovieId = 0;
    reliveScale mScale = reliveScale::eFull;

    // AE only
    s32 mTriggerSwitchId = 0;
    static constexpr char kClassName[] = "movie_stone";
};

struct Path_HandStone final : public Path_TLV
{
    Path_HandStone()
    {
        mTlvType = ReliveTypes::eHandStone;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    reliveScale mScale = reliveScale::eFull;
    s16 mCameraId1 = 0;
    s16 mCameraId2 = 0;
    s16 mCameraId3 = 0;

    // AE only
    s32 mTriggerSwitchId = 0;

    // Additional path and level data is only required for AO
    EReliveLevelIds mLevel1 = EReliveLevelIds::eNone;
    s16 mPath1 = 0;
    EReliveLevelIds mLevel2 = EReliveLevelIds::eNone;
    s16 mPath2 = 0;
    EReliveLevelIds mLevel3 = EReliveLevelIds::eNone;
    s16 mPath3 = 0;
    static constexpr char kClassName[] = "hand_stone";
};

struct Path_PathTransition final : public Path_TLV
{
    Path_PathTransition()
    {
        mTlvType = ReliveTypes::ePathTransition;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    EReliveLevelIds mNextLevel = EReliveLevelIds::eNone;
    s16 mNextPath = 0;
    s16 mNextCamera = 0;
    s16 mMovieId = 0;
    s16 mWipeEffect = 0; // TODO: enum
    reliveScale mNextPathScale = reliveScale::eFull;
    static constexpr char kClassName[] = "path_transition";
};

struct Path_Pulley final : public Path_TLV
{
    Path_Pulley()
    {
        mTlvType = ReliveTypes::ePulley;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    // No fields
    static constexpr char kClassName[] = "pulley";
};

struct Path_Honey final : public Path_TLV
{
    Path_Honey()
    {
        mTlvType = ReliveTypes::eHoney;
    }
    // No fields
    static constexpr char kClassName[] = "honey";
};

struct Path_BeeSwarmHole final : public Path_TLV
{
    Path_BeeSwarmHole()
    {
        mTlvType = ReliveTypes::eBeeSwarmHole;
    }
    enum class MovementType : s16
    {
        eHover,
        eAttack,
        eFollowPath,
    };
    s16 mStartInterval = 0;
    MovementType mMovementType = MovementType::eFollowPath;
    s16 mBeesAmount = 0;
    s16 mChaseTime = 0;
    s16 mSpeed = 0;
    static constexpr char kClassName[] = "bee_swarm_hole";
};

struct Path_Door final : public Path_TLV
{
    Path_Door()
    {
        mTlvType = ReliveTypes::eDoor;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    enum DoorStates : s16
    {
        eOpen,
        eClosed,
        eOpening,
        eClosing,
    };
    enum DoorTypes : s16
    {
        eBasicDoor,
        eTasksDoorWithSecretMusic,
        eTasksDoor,

        // AO only
        eTrialDoor,
        eHubDoor,
    };
    EReliveLevelIds mNextLevel = EReliveLevelIds::eNone;
    s16 mNextPath = 0;
    s16 mNextCamera = 0;
    reliveScale mScale = reliveScale::eFull;
    s16 mDoorId = 0;
    s16 mSwitchId = 0;
    s16 mTargetDoorId = 0;
    DoorTypes mDoorType = DoorTypes::eBasicDoor;

    // AO only
    reliveChoice mDoorClosed = reliveChoice::eNo;

    // AE only
    DoorStates mStartState = DoorStates::eOpen;

    s16 mHub1 = 0;
    s16 mHub2 = 0;
    s16 mHub3 = 0;
    s16 mHub4 = 0;
    s16 mHub5 = 0;
    s16 mHub6 = 0;
    s16 mHub7 = 0;
    s16 mHub8 = 0;
    reliveScreenChangeEffects mWipeEffect = reliveScreenChangeEffects::eBoxOut;
    s16 mMovieId = 0;
    s16 mDoorOffsetX = 0;
    s16 mDoorOffsetY = 0;
    reliveXDirection mExitDirection = reliveXDirection::eRight;

    // AE only
    reliveChoice mCloseOnExit = reliveChoice::eNo;
    reliveChoice mClearThrowables = reliveChoice::eNo;
    static constexpr char kClassName[] = "door";
};

struct Path_AbeStart final : public Path_TLV
{
    Path_AbeStart()
    {
        mTlvType = ReliveTypes::eAbeStart;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }

    // No fields
    static constexpr char kClassName[] = "abe_start";
};

struct Path_EnemyStopper final : public Path_TLV
{
    Path_EnemyStopper()
    {
        mTlvType = ReliveTypes::eEnemyStopper;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    enum class StopDirection : s16
    {
        Left,
        Right,
        Both,
    };
    StopDirection mStopDirection = StopDirection::Both;
    s16 mSwitchId = 0;
    static constexpr char kClassName[] = "enemy_stopper";
};

struct Path_MovingBombStopper final : public Path_TLV
{
    Path_MovingBombStopper()
    {
        mTlvType = ReliveTypes::eMovingBombStopper;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    s16 mMinStopTime = 0;
    s16 mMaxStopTime = 0;
    static constexpr char kClassName[] = "moving_bomb_stopper";
};

struct Path_DoorFlame final : public Path_TLV
{
    Path_DoorFlame()
    {
        mTlvType = ReliveTypes::eDoorFlame;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    enum class Colour : s16
    {
        eDefault,
        eRed,
        eGreen,
        eBlue,
    };
    s16 mSwitchId = 0;
    reliveScale mScale = reliveScale::eFull;
    Colour mColour = Colour::eDefault;
    static constexpr char kClassName[] = "door_flame";
};

struct Path_Mudokon final : public Path_TLV
{
    Path_Mudokon()
    {
        mTlvType = ReliveTypes::eMudokon;
        mAttribute = QuiksaveAttribute::eKeepTlvFlags_2;
    }
    enum class MudJobs : s16
    {
        eChisle,
        eSitScrub,
        eAngryWorker,
        eDamageRingGiver,
        eHealthRingGiver,
        eStandScrub,
        eSitChant,
    };
    enum class Mud_TLV_Emotion : s16
    {
        eNormal,
        eAngry,
        eSad,
        eWired,
        eSick
    };
    reliveScale mScale = reliveScale::eFull;
    MudJobs mJob = MudJobs::eChisle;
    reliveXDirection mFacing = reliveXDirection::eRight;
    s16 mVoicePitch = 0;
    s16 mRescueSwitchId = 0;
    reliveChoice mDeaf = reliveChoice::eNo;
    s16 mDisabledResources = 0;
    reliveChoice mPersistAndResetOffscreen = reliveChoice::eNo; // check if this is the same in AO and AE
    Mud_TLV_Emotion mEmotion = Mud_TLV_Emotion::eNormal;
    reliveChoice mBlind = reliveChoice::eNo;
    s16 mAngrySwitchId = 0;
    reliveChoice mWorkAfterTurningWheel = reliveChoice::eYes;
    reliveChoice mGetsDepressed = reliveChoice::eNo;
    s16 mRingPulseInterval = 0;
    reliveChoice mGiveRingWithoutPassword = reliveChoice::eNo;
    static constexpr char kClassName[] = "mudokon";
};

struct Path_MovingBomb final : public Path_TLV
{
    Path_MovingBomb()
    {
        mTlvType = ReliveTypes::eMovingBomb;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    u16 mSpeed = 0;
    s16 mStartMovingSwitchId = 0;
    reliveChoice mTriggeredByAlarm = reliveChoice::eNo;
    reliveScale mScale = reliveScale::eFull;
    u16 mStartSpeed = 0;
    reliveChoice mPersistOffscreen = reliveChoice::eYes;
    static constexpr char kClassName[] = "moving_bomb";
};

struct Path_ElumPathTrans final : public Path_TLV
{
    Path_ElumPathTrans()
    {
        mTlvType = ReliveTypes::eElumPathTrans;
    }
    EReliveLevelIds mNextLevel = EReliveLevelIds::eNone;
    s16 mNextPath = 0;
    s16 mNextCamera = 0;
    static constexpr char kClassName[] = "elum_path_trans";
};

struct Path_MudokonPathTrans final : public Path_TLV
{
    Path_MudokonPathTrans()
    {
        mTlvType = ReliveTypes::eMudokonPathTrans;
    }
    EReliveLevelIds mNextLevel = EReliveLevelIds::eNone;
    s16 mNextPath = 0;
    s32 mNextCamera = 0;
    static constexpr char kClassName[] = "mudokon_path_trans";
};

struct Path_SecurityClaw final : public Path_TLV
{
    Path_SecurityClaw()
    {
        mTlvType = ReliveTypes::eSecurityClaw;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    reliveScale mScale = reliveScale::eFull;
    s16 mAlarmSwitchId = 0;
    s16 mAlarmDuration = 0;
    static constexpr char kClassName[] = "security_claw";
};

struct Path_SlingMudokon final : public Path_TLV
{
    Path_SlingMudokon()
    {
        mTlvType = ReliveTypes::SlingMud;
    }
    reliveScale mScale =  reliveScale::eFull;
    reliveChoice mDontWhistlePassword = reliveChoice::eYes;
    s16 mCode1 = 0;
    s16 mCode2 = 0;
    static constexpr char kClassName[] = "sling_mudokon";
};

struct Path_FootSwitch final : public Path_TLV
{
    Path_FootSwitch()
    {
        mTlvType = ReliveTypes::eFootSwitch;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    enum class FootSwitchTriggerBy : s16
    {
        eAbe,
        eAnyone,
    };
    s16 mSwitchId = 0;
    reliveScale mScale = reliveScale::eFull;
    reliveSwitchOp mAction = reliveSwitchOp::eSetTrue;
    FootSwitchTriggerBy mTriggeredBy = FootSwitchTriggerBy::eAbe;
    static constexpr char kClassName[] = "foot_switch";
};

struct Path_Paramite final : public Path_TLV
{
    Path_Paramite()
    {
        mTlvType = ReliveTypes::eParamite;
        mAttribute = QuiksaveAttribute::eKeepTlvFlags_2;
    }
    reliveScale mScale = reliveScale::eFull;
    enum class EntranceType : s16
    {
        ePatrol,
        eSurpriseWeb,
        eSlightlyHigherSpawnSurpriseWeb,
    };
    EntranceType mEntranceType = EntranceType::ePatrol;
    s16 mAloneChaseDelay = 0;
    s16 mSurpriseWebDelayTimer = 0;
    s16 mMeatEatingTime = 0;
    u16 mGroupChaseDelay = 0;
    s16 mSurpriseWebSwitchId = 0;
    reliveChoice mHissBeforeAttack = reliveChoice::eYes;
    reliveChoice mDeleteWhenOutOfSight = reliveChoice::eNo;

    // AE only
    reliveChoice mAttackFleeches = reliveChoice::eYes;
    static constexpr char kClassName[] = "paramite";
};

struct Path_ZzzSpawner final : public Path_TLV
{
    Path_ZzzSpawner()
    {
        mTlvType = ReliveTypes::eZzzSpawner;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    reliveScale mScale = reliveScale::eFull;
    s16 mSwitchId = 0;
    s16 mZzzInterval = 0;
    static constexpr char kClassName[] = "zzz_spawner";
};

struct Path_BackgroundGlukkon final : public Path_TLV
{
    Path_BackgroundGlukkon()
    {
        mTlvType = ReliveTypes::eBackgroundGlukkon;
    }
    s16 mScalePercent = 0;
    u16 mPalId = 0;
    static constexpr char kClassName[] = "background_glukkon";
};

struct Path_GasEmitter final : public Path_TLV
{
    Path_GasEmitter()
    {
        mTlvType = ReliveTypes::eGasEmitter;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    // NOTE: AO has no fields
    enum class GasColour : s16
    {
        eYellow,
        eRed,
        eGreen,
        eBlue,
        eWhite,
    };
    s16 mSwitchId = 0;
    GasColour mColour = GasColour::eYellow;
    static constexpr char kClassName[] = "gas_emitter";
};

struct Path_GasCountDown final : public Path_TLV
{
    Path_GasCountDown()
    {
        mTlvType = ReliveTypes::eGasCountDown;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    s16 mStartTimerSwitchId = 0;

    // AE only
    u16 mGasCountdownTimer = 0;
    s16 mStopTimerSwitchId = 0;
    static constexpr char kClassName[] = "gas_countdown";
};

struct Path_RingCancel : public Path_TLV
{
    Path_RingCancel()
    {
        mTlvType = ReliveTypes::eRingCancel;
    }
    bool mRemovesShrykull = false;
    static constexpr char kClassName[] = "ring_cancel";
};

struct Path_SecurityDoor final : public Path_TLV
{
    Path_SecurityDoor()
    {
        mTlvType = ReliveTypes::eSecurityDoor;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    reliveScale mScale = reliveScale::eFull;
    s16 mSwitchId = 0;
    s16 mCode1 = 0;
    s16 mCode2 = 0;
    s16 mXPos = 0;
    s16 mYPos = 0;
    static constexpr char kClassName[] = "security_door";
};

struct Path_LiftMudokon final : public Path_TLV
{
    Path_LiftMudokon()
    {
        mTlvType = ReliveTypes::eLiftMudokon;
    }
    s16 mHowFarToWalk = 0;
    s16 mLiftSwitchId = 0;
    enum class Direction : s16
    {
        eRight,
        eLeft
    };
    Direction mFacing = Direction::eRight;
    reliveChoice mGivePassword = reliveChoice::eNo;
    reliveScale mScale = reliveScale::eFull;
    s16 mCode1 = 0;
    s16 mCode2 = 0;
    static constexpr char kClassName[] = "lift_mudokon";
};

struct Path_RingMudokon final : public Path_TLV
{
    Path_RingMudokon()
    {
        mTlvType = ReliveTypes::eRingMudokon;
    }
    reliveXDirection mFacing = reliveXDirection::eRight;
    enum class MustFaceMud : s16
    {
        eYes,
        eNo
    };
    MustFaceMud mAbeMustFaceMud = MustFaceMud::eYes;
    reliveScale mScale = reliveScale::eFull;
    reliveChoice mGivePassword = reliveChoice::eYes;
    s16 mCode1 = 0;
    s16 mCode2 = 0;
    reliveSwitchOp mAction = reliveSwitchOp::eSetTrue;
    s16 mRingTimeout = 0;
    reliveChoice mGiveRingWithoutPassword = reliveChoice::eYes;
    static constexpr char kClassName[] = "ring_mudokon";
};

struct Path_WellBase : public Path_TLV
{
    reliveScale mScale = reliveScale::eFull;
    s16 mSwitchId = 0;

    // Only used in AO
    s16 mOtherWellId = 0; 
    s16 mAnimId = 0;

    // AO only, part of WellExpress in AE
    s16 mExitX = 0;
    s16 mExitY = 0;
    EReliveLevelIds mOffDestLevel = EReliveLevelIds::eNone;
    s16 mOffDestPath = 0;

    // AO only, part of WellLocal in AE
    s16 mOffDestX = 0;
    s16 mOffDestY = 0;
};

struct Path_WellLocal final : public relive::Path_WellBase
{
    Path_WellLocal()
    {
        mTlvType = ReliveTypes::eWellLocal;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    s16 mOnDestX = 0;
    s16 mOnDestY = 0;
    reliveChoice mEmitLeaves = reliveChoice::eNo;
    s16 mLeafX = 0;
    s16 mLeafY = 0;
    static constexpr char kClassName[] = "well_local";
};

struct Path_WellExpress final : public relive::Path_WellBase
{
    Path_WellExpress()
    {
        mTlvType = ReliveTypes::eWellExpress;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    s16 mOffDestCamera = 0;
    s16 mOffOtherWellId = 0;
    EReliveLevelIds mOnDestLevel = EReliveLevelIds::eNone;
    s16 mOnDestPath = 0;
    s16 mOnDestCamera = 0;
    s16 mOnOtherWellId = 0;
    reliveChoice mEmitLeaves = reliveChoice::eNo;
    s16 mLeafX = 0;
    s16 mLeafY = 0;
    s16 mMovieId = 0;
    static constexpr char kClassName[] = "well_express";
};

struct Path_Slog final : public Path_TLV
{
    Path_Slog()
    {
        mTlvType = ReliveTypes::eSlog;
        mAttribute = QuiksaveAttribute::eKeepTlvFlags_2;
    }
    reliveScale mScale = reliveScale::eFull;
    reliveXDirection mFacing = reliveXDirection::eRight;
    reliveChoice mAsleep = reliveChoice::eNo;
    s16 mWakeUpAnger = 0;
    s16 mBarkAnger = 0;
    s16 mChaseAnger = 0;
    s16 mChaseDelay = 0;
    s16 mAngerSwitchId = 0;

    // AE only
    s16 mBoneEatingTime = 0;
    static constexpr char kClassName[] = "slog";
};

struct Path_SlogSpawner final : public Path_TLV
{
    Path_SlogSpawner()
    {
        mTlvType = ReliveTypes::eSlogSpawner;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    enum class StartDirection : s16
    {
        eRight,
        eLeft
    };
    reliveScale mScale = reliveScale::eFull;
    s16 mMaxSlogs = 0;
    s16 mMaxSlogsAtATime = 0;
    StartDirection mStartDirection = StartDirection::eRight;
    s16 mSlogSpawnInterval = 0;
    s16 mSpawnerSwitchId = 0;

    // AE only
    reliveChoice mListenToSligs = reliveChoice::eNo;
    s16 mChaseDelay = 0;
    static constexpr char kClassName[] = "slog_spawner";
};


struct Path_Scrab : public Path_TLV
{
    Path_Scrab()
    {
        mTlvType = ReliveTypes::eScrab;
        mAttribute = QuiksaveAttribute::eKeepTlvFlags_2;
    }

    enum class ScrabPatrolType : s16
    {
        eWalk,
        eRunOrWalk192,
        eRunOrWalk128,
        eRunOrWalk64,
        eRun
    };
    reliveScale mScale = reliveScale::eFull;
    s16 mAttackDelay = 0;
    s16 mPatrolTypeRunOrWalkChance = 0;                   // AE only
    ScrabPatrolType mPatrolType = ScrabPatrolType::eWalk; // AO only
    s16 mPauseLeftMin = 0;
    s16 mPauseLeftMax = 0;
    s16 mPauseRightMin = 0;
    s16 mPauseRightMax = 0;
    u16 mPauseAfterChaseTime = 0;
    s16 mSpottingAbeDelay = 0; // TODO: check if the same field as below?
    reliveChoice mRoarRandomly = reliveChoice::eNo;

    // AE only
    reliveChoice mPersistant = reliveChoice::eYes;
    s16 mPossessedMaxWhirlAttackDuration = 0;
    reliveChoice mKillEnemy = reliveChoice::eYes;
    static constexpr char kClassName[] = "scrab";
};


struct Path_ScrabSpawner : public Path_Scrab
{
    Path_ScrabSpawner()
    {
        mTlvType = ReliveTypes::eScrabSpawner;
        mAttribute = QuiksaveAttribute::eKeepTlvFlags_2;
    }

    enum class SpawnDirection : s16
    {
        eNone,
        eLeft,
        eRight,
    };
    s16 mSpawnerSwitchId = 0;
    SpawnDirection mFacing = SpawnDirection::eNone;
    static constexpr char kClassName[] = "scrab_spawner";
};

struct Path_Slig_Data final
{
    enum class DeathMode
    {
        StandIdle,
        FallForward,
    };

    reliveScale mScale = reliveScale::eFull;

    enum class StartState : s16
    {
        Listening,
        Patrol,
        Sleeping,
        Chase,
        ChaseAndDisappear,
        eFallingToChase,
        ListeningToGlukkon,
    };
    StartState mStartState = StartState::Patrol;

    DeathMode mDeathMode = DeathMode::FallForward;
    bool mCanBeat = true;
    bool mCanZShoot = true;

    s16 mPauseTime = 0;
    s16 mPauseLeftMin = 0;
    s16 mPauseLeftMax = 0;
    s16 mPauseRightMin = 0;
    s16 mPauseRightMax = 0;
    reliveChoice mShootPossessedSligs = reliveChoice::eYes;
    s16 mShootOnSightDelay = 0;
    s16 mNumTimesToShoot = 0;
    s16 mCode1 = 0;
    s16 mCode2 = 0;
    reliveChoice mChaseAbeWhenSpotted = reliveChoice::eNo;
    reliveXDirection mFacing = reliveXDirection::eRight;
    s16 mPanicTimeout = 0;
    s16 mStopChaseDelay = 0;
    s16 mTimeToWaitBeforeChase = 0;
    s16 mSligBoundId = 0;
    s16 mAlertedListenTime = 0;
    s16 mPercentSayWhat = 0;
    s16 mPercentBeatMud = 0;
    s16 mZShootDelay = 0;
    reliveChoice mStayAwake = reliveChoice::eYes;
    s16 mNoiseWakeUpDistance = 0;
    s16 mSligSpawnerSwitchId = 0;

    // TODO: Remove disabled resources after anim loading fixed
    BitField16<AO::SligFlags_DisabledRes> mDisabledResourcesAO;
    s16 mDisabledResourcesAE = 0;

    // AE only
    reliveChoice mUnlimitedSpawns = reliveChoice::eNo;
};

struct Path_Slig final : public Path_TLV
{
    Path_Slig()
    {
        mTlvType = ReliveTypes::eSlig;
        mAttribute = QuiksaveAttribute::eKeepTlvFlags_2;
    }
    Path_Slig_Data mData;
    static constexpr char kClassName[] = "slig";
};

struct Path_SligSpawner final : public Path_TLV
{
    Path_SligSpawner()
    {
        mTlvType = ReliveTypes::eSligSpawner;
        mAttribute = QuiksaveAttribute::eKeepTlvFlags_2;
    }

    Path_Slig_Data mData;
    static constexpr char kClassName[] = "slig_spawner";
};

struct Path_TrainDoor final : public Path_TLV
{
    Path_TrainDoor()
    {
        mTlvType = ReliveTypes::eTrainDoor;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    reliveXDirection mDirection = reliveXDirection::eRight;
    static constexpr char kClassName[] = "train_door";
};

struct Path_TorturedMudokon final : public Path_TLV
{
    Path_TorturedMudokon()
    {
        mTlvType = ReliveTypes::eTorturedMud;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    s16 mKillSwitchId = 0;
    s16 mReleaseSwitchId = 0;
    static constexpr char kClassName[] = "tortured_mudokon";
};

struct Path_DoorBlocker final : public Path_TLV
{
    Path_DoorBlocker()
    {
        mTlvType = ReliveTypes::eDoorBlocker;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    reliveScale mScale = reliveScale::eFull;
    s16 mSwitchId = 0;
    static constexpr char kClassName[] = "door_blocker";
};

struct Path_GlukkonSwitch final : public Path_TLV
{
    Path_GlukkonSwitch()
    {
        mTlvType = ReliveTypes::eGlukkonSwitch;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    reliveScale mScale = reliveScale::eFull;
    s16 mOkSwitchId = 0;
    s16 mFailSwitchId = 0;
    u16 mXPos = 0;
    u16 mYPos = 0;
    static constexpr char kClassName[] = "glukkon_switch";
};

struct Path_Greeter final : public Path_TLV
{
    Path_Greeter()
    {
        mTlvType = ReliveTypes::eGreeter;
        mAttribute = QuiksaveAttribute::eKeepTlvFlags_2;
    }
    reliveScale mScale = reliveScale::eFull;
    s16 mMotionDetectorSpeed = 0;
    reliveXDirection mFacing = reliveXDirection::eRight;
    static constexpr char kClassName[] = "greeter";
};

struct Path_BrewMachine final : public Path_TLV
{
    Path_BrewMachine()
    {
        mTlvType = ReliveTypes::eBrewMachine;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    s16 mBrewCount = 0;
    static constexpr char kClassName[] = "brew_machine";
};

struct Path_Alarm final : public Path_TLV
{
    Path_Alarm()
    {
        mTlvType = ReliveTypes::eAlarm;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    s16 mSwitchId = 0;
    u16 mAlarmDuration = 0;
    static constexpr char kClassName[] = "alarm";
};

struct Path_ParamiteWebLine final : public Path_TLV
{
    Path_ParamiteWebLine()
    {
        mTlvType = ReliveTypes::eParamiteWebLine;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    reliveScale mScale = reliveScale::eFull;
    static constexpr char kClassName[] = "paramite_web_line";
};

struct Path_SlapLock final : public Path_TLV
{
    Path_SlapLock()
    {
        mTlvType = ReliveTypes::eSlapLock;
        mAttribute = QuiksaveAttribute::eKeepTlvFlags_2;
    }
    reliveScale mScale = reliveScale::eFull;
    s16 mTargetTombSwitchId1 = 0;
    s16 mTargetTombSwitchId2 = 0;
    reliveChoice mHasGhost = reliveChoice::eNo;
    reliveChoice mGiveInvisibilityPowerup = reliveChoice::eNo;
    s16 mInvisibilityDuration = 0;
    s16 mSlapOutputSwitchId = 0;
    static constexpr char kClassName[] = "slap_lock";
};

struct Path_StatusLight final : public Path_TLV
{
    Path_StatusLight()
    {
        mTlvType = ReliveTypes::eStatusLight;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    s16 mInputSwitchId = 0;
    reliveScale mScale = reliveScale::eFull;
    s16 mLinkedStatusLightSwitchId1 = 0;
    s16 mLinkedStatusLightSwitchId2 = 0;
    s16 mLinkedStatusLightSwitchId3 = 0;
    s16 mLinkedStatusLightSwitchId4 = 0;
    s16 mLinkedStatusLightSwitchId5 = 0;
    reliveChoice mIgnoreGridSnapping = reliveChoice::eNo;
    static constexpr char kClassName[] = "status_light";
};

struct Path_MultiSwitchController final : public Path_TLV
{
    Path_MultiSwitchController()
    {
        mTlvType = ReliveTypes::eMultiSwitchController;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    s16 mOutputSwitchId = 0;
    reliveSwitchOp mAction = reliveSwitchOp::eSetTrue;
    u16 mOnOffDelay = 0;
    u16 mInputSwitchId1 = 0;
    u16 mInputSwitchId2 = 0;
    u16 mInputSwitchId3 = 0;
    u16 mInputSwitchId4 = 0;
    u16 mInputSwitchId5 = 0;
    u16 mInputSwitchId6 = 0;
    static constexpr char kClassName[] = "multi_switch_controller";
};

struct Path_ExplosionSet final : public Path_TLV
{
    Path_ExplosionSet()
    {
        mTlvType = ReliveTypes::eExplosionSet;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    reliveChoice mStartEnabled = reliveChoice::eNo;
    s16 mSwitchId = 0;
    reliveChoice mSpawnAssets = reliveChoice::eNo;
    s16 mStartDelay = 0;
    reliveXDirection mStartDirection = reliveXDirection::eRight;
    s16 mAssetInterval = 0;
    s16 mGridSpacing = 0;
    s16 mIncreasingGridSpacing = 0;
    reliveScale mScale = reliveScale::eFull;
    static constexpr char kClassName[] = "explosion_set";
};

struct Path_BoneBag final : public Path_TLV
{
    Path_BoneBag()
    {
        mTlvType = ReliveTypes::eBoneBag;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    reliveXDirection mBoneFallDirection = reliveXDirection::eRight;
    u16 mVelX = 0;
    u16 mVelY = 0;
    reliveScale mScale = reliveScale::eFull;
    s16 mBoneAmount = 0;
    static constexpr char kClassName[] = "bone_bag";
};

struct Path_MineCar final : public Path_TLV
{
    Path_MineCar()
    {
        mTlvType = ReliveTypes::eMineCar;
        mAttribute = QuiksaveAttribute::eKeepTlvFlags_2;
    }
    reliveScale mScale = reliveScale::eFull;
    static constexpr char kClassName[] = "mine_car";
};

struct Path_ColourfulMeter final : public Path_TLV
{
    Path_ColourfulMeter()
    {
        mTlvType = ReliveTypes::eColourfulMeter;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    s16 mSwitchId = 0;
    s16 mNumberOfMeterBars = 0;
    s16 mMinesAlarmCountdown = 0;
    reliveChoice mStartFilled = reliveChoice::eNo;
    static constexpr char kClassName[] = "colourful_meter";
};

struct Path_DemoSpawnPoint final : public Path_TLV
{
    Path_DemoSpawnPoint()
    {
        mTlvType = ReliveTypes::eDemoSpawnPoint;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    // Empty
    static constexpr char kClassName[] = "demo_spawn_point";
};

struct Path_LevelLoader final : public Path_TLV
{
    Path_LevelLoader()
    {
        mTlvType = ReliveTypes::eLevelLoader;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    s16 mSwitchId = 0;
    EReliveLevelIds mDestLevel = EReliveLevelIds::eNone;
    s16 mDestPath = 0;
    s16 mDestCamera = 0;
    s16 mMovieId = 0;
    static constexpr char kClassName[] = "level_loader";
};

struct Path_SlamDoor final : public Path_TLV
{
    Path_SlamDoor()
    {
        mTlvType = ReliveTypes::eSlamDoor;
        mAttribute = QuiksaveAttribute::eKeepTlvFlags_2;
    }
    reliveChoice mStartClosed = reliveChoice::eNo;
    reliveScale mScale = reliveScale::eFull;
    s16 mSwitchId = 0;
    reliveChoice mFlipY = reliveChoice::eNo;
    reliveChoice mDelete = reliveChoice::eNo;
    static constexpr char kClassName[] = "slam_door";
};

struct Path_Slurg : public Path_TLV
{
    Path_Slurg()
    {
        mTlvType = ReliveTypes::eSlurg;
        mAttribute = QuiksaveAttribute::eKeepTlvFlags_2;
    }
    s16 mMovingTimer = 0;
    reliveXDirection mFacing = reliveXDirection::eRight;
    reliveScale mScale = reliveScale::eFull;
    s16 mSlurgSwitchId = 0;
    static constexpr char kClassName[] = "annoying_slurg";
};

struct Path_LaughingGas final : public Path_TLV
{
    Path_LaughingGas()
    {
        mTlvType = ReliveTypes::eLaughingGas;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    reliveChoice mLaughingGas = reliveChoice::eYes;
    s16 mLaughingGasSwitchId = 0;
    s16 mRedPercent = 0;
    s16 mGreenPercent = 0;
    s16 mBluePercent = 0;
    static constexpr char kClassName[] = "laughing_gas";
};

struct Path_WorkWheel final : public Path_TLV
{
    Path_WorkWheel()
    {
        mTlvType = ReliveTypes::eWorkWheel;
        mAttribute = QuiksaveAttribute::eKeepTlvFlags_2;
    }
    reliveScale mScale = reliveScale::eFull;
    s16 mSwitchId = 0;
    s16 mActivationTime = 0;
    s16 mTurnOffTime = 0;
    reliveChoice mTurnOffWhenStopped = reliveChoice::eNo;
    static constexpr char kClassName[] = "work_wheel";
};

struct Path_Water final : public Path_TLV
{
    Path_Water()
    {
        mTlvType = ReliveTypes::eWater;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    s16 mMaxDrops = 0;
    s16 mSwitchId = 0;
    s16 mSplashTime = 0;
    s16 mSplashVelX = 0;
    s16 mWaterDuration = 0;
    static constexpr char kClassName[] = "water";
};

struct Path_WheelSyncer final : public Path_TLV
{
    Path_WheelSyncer()
    {
        mTlvType = ReliveTypes::eWheelSyncer;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    enum class OutputRequirement : s16
    {
        eAllOn,
        e1OnAnd2Off,
        e1Or2On,
        e1OnOr2Off
    };
    s16 mInputSwitchId1 = 0;
    s16 mInputSwitchId2 = 0;
    s16 mOutputSwitchId = 0;
    OutputRequirement mOutputRequirement = OutputRequirement::eAllOn;
    s16 mInputSwitchId3 = 0;
    s16 mInputSwitchId4 = 0;
    s16 mInputSwitchId5 = 0;
    s16 mInputSwitchId6 = 0;
    static constexpr char kClassName[] = "wheel_syncer";
};

struct Path_Fleech final : public Path_TLV
{
    Path_Fleech()
    {
        mTlvType = ReliveTypes::eFleech;
        mAttribute = QuiksaveAttribute::eKeepTlvFlags_2;
    }
    reliveScale mScale = reliveScale::eFull;
    reliveXDirection mFacing = reliveXDirection::eRight;
    reliveChoice mAsleep = reliveChoice::eNo;
    s16 mAttackAngerIncreaser = 0;
    s16 mWakeUpSwitchId = 0;
    reliveChoice mHanging = reliveChoice::eNo;
    s16 mLostTargetTimeout = 0;
    reliveChoice mGoesToSleep = reliveChoice::eNo;
    s16 mPatrolRangeInGrids = 0;
    s16 mWakeUpSwitchAngerValue = 0;
    s16 mCanWakeUpSwitchId = 0;
    reliveChoice mPersistant = reliveChoice::eNo;
    static constexpr char kClassName[] = "fleech";
};

struct Path_SlurgSpawner final : public Path_Slurg
{
    Path_SlurgSpawner()
    {
        mTlvType = ReliveTypes::eSlurgSpawner;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    s16 mSpawnInterval = 0;
    s16 mMaxSlurgs = 0;
    s16 mSpawnerSwitchId = 0;
    static constexpr char kClassName[] = "slurg_spawner";
};

struct Path_Drill final : public Path_TLV
{
    Path_Drill()
    {
        mTlvType = ReliveTypes::eDrill;
        mAttribute = QuiksaveAttribute::eKeepTlvFlags_2;
    }
    enum class DrillDirection : s16
    {
        eDown,
        eRight,
        eLeft,
    };

    enum class DrillBehavior : s16
    {
        eNotInteractable,
        eToggle,
        eUse
    };
    reliveScale mScale = reliveScale::eFull;
    s16 mOnMinPauseTime = 0;
    s16 mOnMaxPauseTime = 0;
    s16 mSwitchId = 0;
    DrillBehavior mDrillBehavior = DrillBehavior::eNotInteractable;
    s16 mOnSpeed = 0;
    reliveChoice mStartStateOn = reliveChoice::eNo;
    s16 mOffSpeed = 0;
    s16 mOffMinPauseTime = 0;
    s16 mOffMaxPauseTime = 0;
    reliveChoice mStartPositionBottom = reliveChoice::eNo;
    DrillDirection mDrillDirection = DrillDirection::eRight;
    static constexpr char kClassName[] = "drill";
};

struct Path_Teleporter final : public Path_TLV
{
    Path_Teleporter()
    {
        mTlvType = ReliveTypes::eTeleporter;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    s16 mTeleporterId = 0;
    s16 mOtherTeleporterId = 0;
    s16 mDestCamera = 0;
    s16 mDestPath = 0;
    EReliveLevelIds mDestLevel = EReliveLevelIds::eNone;
    s16 mSwitchId = 0;
    reliveScale mScale = reliveScale::eFull;
    reliveScreenChangeEffects mWipeEffect = reliveScreenChangeEffects::eBoxOut;
    s16 mMovieId = 0;
    s16 mElectricX = 0;
    s16 mElectricY = 0;
    static constexpr char kClassName[] = "teleporter";
};

struct Path_Glukkon final : public Path_TLV
{
    Path_Glukkon()
    {
        mTlvType = ReliveTypes::eGlukkon;
        mAttribute = QuiksaveAttribute::eKeepTlvFlags_2;
    }
    reliveScale mScale = reliveScale::eFull;
    enum class Facing : s16
    {
        eRight,
        eLeft,
    };
    Facing mFacing = Facing::eRight;
    enum class Behavior : s16
    {
        eIgnoreWalls,
        eCheckForWalls
    };
    Behavior mBehavior = Behavior::eCheckForWalls;
    s16 mScreamHelpDelay = 0;
    s16 mHelpSwitchId = 0;
    s16 mToCalmDelay = 0;
    s16 mSpawnSwitchId = 0;
    enum class SpawnType : s16
    {
        eRegularSpawn,
        eFacingLeft,
        eFacingRight,
        eFullSpawnEffects
    };
    SpawnType mSpawnType = SpawnType::eRegularSpawn;
    s16 mSpawnDelay = 0;
    enum class GlukkonTypes : s16
    {
        eNormal,
        eStoryAslik,
        eStoryDripik,
        eStoryPhleg,
    };
    GlukkonTypes mGlukkonType = GlukkonTypes::eNormal;
    s16 mDeathSwitchId = 0;
    s16 mPlayMovieSwitchId = 0;
    s16 mMovieId = 0;
    static constexpr char kClassName[] = "glukkon";
};

struct Path_CrawlingSligButton final : public Path_TLV
{
    Path_CrawlingSligButton()
    {
        mTlvType = ReliveTypes::eCrawlingSligButton;
        mAttribute = QuiksaveAttribute::eDoNothing_0;
    }
    enum class ButtonSounds : s16
    {
        None,
        SackHit1,
        SackHit2,
        FallingItemPresence2,
        SecurityOrb,
        Bullet1,
        AbeGenericMovement,
    };
    reliveScale mScale = reliveScale::eFull;
    s16 mSwitchId = 0;
    reliveSwitchOp mAction = reliveSwitchOp::eSetTrue;
    ButtonSounds mOnSound = ButtonSounds::None;
    ButtonSounds mOffSound = ButtonSounds::None;
    s16 mSoundDirection = 0;
    static constexpr char kClassName[] = "crawling_slig_button";
};

struct Path_FlyingSlig final : public Path_TLV
{
    Path_FlyingSlig()
    {
        mTlvType = ReliveTypes::eFlyingSlig;
        mAttribute = QuiksaveAttribute::eKeepTlvFlags_2;
    }
    reliveScale mScale = reliveScale::eFull;
    enum class SpawnDelayStates : s16
    {
        eMoveImmediately,
        eUseCustomSpawnMoveDelay,
    };
    SpawnDelayStates mSpawnDelayState = SpawnDelayStates::eMoveImmediately;
    s16 mSpawnMoveDelay = 0;
    s16 mPatrolPauseMin = 0;
    s16 mPatrolPauseMax = 0;
    reliveXDirection mFacing = reliveXDirection::eRight;
    s16 mPanicDelay = 0;
    s16 mGiveUpChaseDelay = 0;
    s16 mPrechaseDelay = 0;
    s16 mSligBoundId = 0;
    s16 mAlertedListenTime = 0;
    s16 mSpawnerSwitchId = 0;
    s16 mGrenadeDelay = 0;
    s16 mMaxVelocity = 0;
    s16 mLaunchGrenadeSwitchId = 0;
    reliveChoice mPersistant = reliveChoice::eNo;
    static constexpr char kClassName[] = "flying_slig";
};

struct Path_FlyingSligSpawner final : public Path_TLV
{
    Path_FlyingSligSpawner()
    {
        mTlvType = ReliveTypes::eFlyingSligSpawner;
        mAttribute = QuiksaveAttribute::eKeepTlvFlags_2;
    }
    reliveScale mScale = reliveScale::eFull;
    Path_FlyingSlig::SpawnDelayStates mSpawnDelayState = Path_FlyingSlig::SpawnDelayStates::eMoveImmediately;
    s16 mSpawnMoveDelay = 0;
    s16 mPatrolPauseMin = 0;
    s16 mPatrolPauseMax = 0;
    reliveXDirection mFacing = reliveXDirection::eRight;
    s16 mPanicDelay = 0;
    s16 mGiveUpChaseDelay = 0;
    s16 mPrechaseDelay = 0;
    s16 mSligBoundId = 0;
    s16 mAlertedListenTime = 0;
    s16 mSpawnerSwitchId = 0;
    s16 mGrenadeDelay = 0;
    s16 mMaxVelocity = 0;
    s16 mLaunchGrenadeSwitchId = 0;
    reliveChoice mPersistant = reliveChoice::eNo;
    static constexpr char kClassName[] = "flying_slig_spawner";
};

struct Path_CrawlingSlig final : public Path_TLV
{
    Path_CrawlingSlig()
    {
        mTlvType = ReliveTypes::eCrawlingSlig;
        mAttribute = QuiksaveAttribute::eKeepTlvFlags_2;
    }
    reliveScale mScale = reliveScale::eFull;
    enum class StartState : s16
    {
        eSleeping,
        eAwake,
    };
    StartState mStartState = StartState::eSleeping;

    enum class CrawlDirection : s16
    {
        eLeft,
        eRight,
        eRandom,
    };
    CrawlDirection mCrawlDirection = CrawlDirection::eRight;
    s16 mPanicSwitchId = 0;
    reliveChoice mRespawnOnDeath = reliveChoice::eNo;
    static constexpr char kClassName[] = "crawling_slig";
};

struct Path_SligGetWings final : public Path_TLV
{
    Path_SligGetWings()
    {
        mTlvType = ReliveTypes::eSligGetWings;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    reliveScale mScale = reliveScale::eFull;
    Path_FlyingSlig::SpawnDelayStates mSpawnDelayState = Path_FlyingSlig::SpawnDelayStates::eMoveImmediately;
    s16 mSpawnMoveDelay = 0;
    s16 mPatrolPauseMin = 0;
    s16 mPatrolPauseMax = 0;
    reliveXDirection mFacing = reliveXDirection::eRight;
    s16 mPanicDelay = 0;
    s16 mGiveUpChaseDelay = 0;
    s16 mPrechaseDelay = 0;
    s16 mSligBoundId = 0;
    s16 mAlertedListenTime = 0;
    s16 mSpawnerSwitchId = 0;
    s16 mGrenadeDelay = 0;
    s16 mMaxVelocity = 0;
    s16 mLaunchGrenadeSwitchId = 0;
    reliveChoice mPersistant = reliveChoice::eNo;
    static constexpr char kClassName[] = "slig_get_wings";
};

struct Path_SligGetPants final : public Path_TLV
{
    Path_SligGetPants()
    {
        mTlvType = ReliveTypes::eSligGetPants;
        mAttribute = QuiksaveAttribute::eClearTlvFlags_1;
    }
    
    Path_Slig_Data mData;
    static constexpr char kClassName[] = "slig_get_pants";
};

} // namespace relive
