#pragma once

#include "../BaseAnimatedWithPhysicsGameObject.hpp"

// all tlv map types

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

// TODO: Base type
class Path_TLV
{
public:
    // TODO: not sure if we need to store the type or length
    // if we are using json as the file format
    s32 mX = 0;
    s32 mY = 0;
    s32 mWidth = 0;
    s32 mHeight = 0;
};

class Path_ShadowZone final : public Path_TLV
{
public:
    enum class Scale : s16
    {
        eBoth,
        eHalf,
        eFull,
    };

    RGB16 mRGB;
    Scale mScale = Scale::eFull;
};

class Path_SecurityOrb final : public Path_TLV
{
public:
    reliveScale mScale = reliveScale::eFull;
    s16 mDisabledResources = 0;
};

class Path_ContinuePoint final : public Path_TLV
{
public:
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
};

struct Path_LiftPoint final : public Path_TLV
{
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
};

struct Path_Dove final : public Path_TLV
{
    s16 mDoveCount = 0;
    reliveChoice mPixelPerfect = reliveChoice::eNo;
    reliveScale mScale = reliveScale::eFull;
};

struct Path_RockSack final : public Path_TLV
{
    reliveXDirection mRockFallDirection = reliveXDirection::eLeft;
    u16 mVelX = 0;
    u16 mVelY = 0;
    reliveScale mScale = reliveScale::eFull;
    s16 mRockAmount = 0;
};

struct Path_ZBall final : public Path_TLV
{
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
};

struct Path_FallingItem final : public Path_TLV
{
    s16 mSwitchId = 0;
    reliveScale mScale = reliveScale::eFull;
    s16 mFallInterval = 0;
    s16 mMaxFallingItems = 0;
    reliveChoice mResetSwitchIdAfterUse = reliveChoice::eYes;
};

struct Path_PullRingRope final : public Path_TLV
{
    enum class PullRingSwitchSound : s16
    {
        eNone,
        eWellExit,
        RingBellHammer,
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
};

struct Path_TimedMine final : public Path_TLV
{
    s16 mSwitchId = 0;
    s16 mState = 0;
    reliveScale mScale = reliveScale::eFull;
    s16 mTicksUntilExplosion = 0;
    s32 mDisabledResources = 0;
};

struct Path_Hoist final : public Path_TLV
{
    enum class Type : s16
    {
        eNextFloor,
        eNextEdge,
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
};

struct Path_TrapDoor final : public Path_TLV
{
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
};

struct Path_LCDScreen final : public Path_TLV
{
    s16 mMessageId1 = 0;
    s16 mMessageRandMinId = 0;
    s16 mMessageRandMaxId = 0;

    // AE only
    s16 mMessageId2 = 0;
    s32 mToggleMessageSwitchId = 0;
};

struct Path_Mine final : public Path_TLV
{
    reliveScale mScale = reliveScale::eFull;
    s16 mDisabledResources = 0;
    reliveChoice mPersistOffscreen = reliveChoice::eYes;
};

struct Path_InvisibleSwitch final : public Path_TLV
{
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
};

struct Path_ElectricWall final : public Path_TLV
{
    reliveScale mScale = reliveScale::eFull;
    s16 mSwitchId = 0;
    enum ElectricWallStartState : s16
    {
        eOff,
        eOn,
    };
    ElectricWallStartState mStartState = ElectricWallStartState::eOn;
};

struct Path_BoomMachine final : public Path_TLV
{
    reliveScale mScale = reliveScale::eFull;
    enum class NozzleSide : s16
    {
        eRight,
        eLeft,
    };
    NozzleSide mNozzleSide = NozzleSide::eRight;
    s16 mDisabledResources = 0;
    s16 mGrenadeAmount = 0;
};

struct Path_UXB final : public Path_TLV
{
    enum class StartState : s16
    {
        eOn,
        eOff,
    };
    s16 mPatternLength = 0;
    s16 mPattern = 0;
    reliveScale mScale = reliveScale::eFull;
    StartState mStartState = StartState::eOn;
    s32 mDisabledResources = 0;
};

struct Path_MeatSaw final : public Path_TLV
{
    reliveScale mScale = reliveScale::eFull;
    s16 mSwitchMinTimeOff = 0;
    s16 mSwitchMaxTimeOff = 0;
    s16 mMaxRiseTime = 0;
    s16 mSwitchId = 0;
    enum class Type : s16
    {
        eAutomaticPersistOffscreen,
        eAutomatic,
        eSwitchId,
    };
    Type mType = Type::eAutomatic;
    s16 mSpeed = 0;
    enum class StartState : s16
    {
        eOff,
        eOn,
    };
    StartState mStartState = StartState::eOn;
    s16 mOffSpeed = 0;
    s16 mAutomaticMinTimeOff = 0;
    s16 mAutomaticMaxTimeOff = 0;
    s16 mInitialPosition = 0;
};

struct Path_Lever final : public Path_TLV
{
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
};

struct Path_Edge final : public Path_TLV
{
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
};

struct Path_BirdPortal final : public Path_TLV
{
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
};

struct Path_BirdPortalExit final : public Path_TLV
{
    Path_BirdPortal::PortalSide mExitSide = Path_BirdPortal::PortalSide::eRight;
    reliveScale mScale = reliveScale::eFull;
};

struct Path_LightEffect final : public Path_TLV
{
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
};

struct Path_MusicTrigger final : public Path_TLV
{
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
};

struct Path_SoftLanding final : public Path_TLV
{
    // AE only
    s32 mSwitchId = 0;
};

struct Path_LiftMover final : public Path_TLV
{
    s16 mLiftMoverSwitchId = 0;
    s16 mTargetLiftPointId = 0;
    enum class YDirection : s16
    {
        eDown,
        eUp,
    };
    YDirection mMoveDirection = YDirection::eDown;
};

struct Path_HintFly final : public Path_TLV
{
    s16 mMessageId = 0;
};

struct Path_TimerTrigger final : public Path_TLV
{
    s16 mInputSwitchId = 0;
    u16 mActivationDelay = 0;
    s16 mOutputSwitchId1 = 0;
    s16 mOutputSwitchId2 = 0;
    s16 mOutputSwitchId3 = 0;
    s16 mOutputSwitchId4 = 0;
};

struct Path_FlintLockFire final : public Path_TLV
{
    reliveScale mScale = reliveScale::eFull;
    s16 mSwitchId = 0;
};

struct Path_HoneySack final : public Path_TLV
{
    s16 mChaseTime = 0;
    reliveScale mScale = reliveScale::eFull;
};

struct Path_Bat final : public Path_TLV
{
    s16 mTimeBeforeMoving = 0;
    s16 mSpeed = 0;
    reliveScale mScale = reliveScale::eFull;
    s16 mAttackDuration = 0;
};

struct Path_RollingBallStopper final : public Path_TLV
{
    s16 mStopperSwitchId = 0;
    reliveScale mScale = reliveScale::eFull;
    s16 mBallSwitchId = 0;
    reliveXDirection mStopDirection = reliveXDirection::eRight;
};

struct Path_RollingBall final : public Path_TLV
{
    reliveScale mScale = reliveScale::eFull;
    reliveXDirection mRollDirection = reliveXDirection::eRight;
    s16 mReleaseSwitchId = 0;
    u16 mMaxSpeed = 0;
    u16 mAcceleration = 0;
};

struct Path_MotionDetector final : public Path_TLV
{
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
};

struct Path_BellHammer final : public Path_TLV
{
    s16 mSwitchId = 0;
    reliveSwitchOp mAction = reliveSwitchOp::eSetTrue;
    reliveScale mScale = reliveScale::eFull;
    reliveXDirection mDirection = reliveXDirection::eRight;
};

struct Path_SligBound final : public Path_TLV
{
    s16 mSligBoundId = 0;
    s16 mDisabledResources = 0;
};

struct Path_BackgroundAnimation final : public Path_TLV
{
    u16 mAnimId = 0;
    reliveChoice mIsSemiTrans = reliveChoice::eNo;
    enum class TPageAbr : s8
    {
        eBlend_0 = 0,
        eBlend_1 = 1,
        eBlend_2 = 2,
        eBlend_3 = 3,
    };

    enum class BgAnimSounds : s16
    {
        eNone_m1 = -1,
        eNone_0 = 0,
        eFire,
        eFireIdx,
    };

    TPageAbr mSemiTransMode = TPageAbr::eBlend_0;

    // AO only
    BgAnimSounds mSoundEffect = BgAnimSounds::eNone_0;

    // AE only
    enum class Layer : s16
    {
        eLayer0,
    };
    Layer mLayer = Layer::eLayer0;
};

struct Path_MainMenuController final : public Path_TLV
{
    // No fields
};

struct Path_ElumWall final : public Path_TLV
{
    // No fields
};

struct Path_ElumStart final : public Path_TLV
{
    // No fields
};

struct Path_KillUnsavedMuds final : public Path_TLV
{
    // No fields
};

struct Path_InvisibleZone final : public Path_TLV
{
    // No fields
};

struct Path_StartController final : public Path_TLV
{
    // No fields
};

struct Path_ScrabNoFall final : public Path_TLV
{
    // No fields
};

struct Path_ScrabLeftBound final : public Path_TLV
{
    // No fields
};

struct Path_ScrabRightBound final : public Path_TLV
{
    // No fields
};

struct Path_HoneyDripTarget final : public Path_TLV
{
    // No fields
};

struct Path_ZSligCover final : public Path_TLV
{
    // No fields
};

struct Path_DeathDrop final : public Path_TLV
{
    // No fields
};

struct Path_ChimeLock final : public Path_TLV
{
    reliveScale mScale = reliveScale::eFull;
    s16 mSolveSwitchId = 0;
    s16 mCode1 = 0;
    s16 mCode2 = 0;
    s16 mPasswordSwitchId = 0;
};

struct Path_LCDStatusBoard final : public Path_TLV
{
    // AO has no fields
    s16 mNumberOfMuds = 0;
    s16 mZulagNumber = 0;
    reliveChoice mHideBoard = reliveChoice::eNo;
};

struct Path_CreditsController final : public Path_TLV
{
    // Empty
};

struct Path_ResetPath final : public Path_TLV
{
    s16 mClearIds = 0;
    s16 mFrom = 0;
    s16 mTo = 0;
    s16 mExclude = 0;
    s16 mClearObjects = 0;
    s16 mPath = 0;
};

struct Path_MeatSack final : public Path_TLV
{
    reliveXDirection mMeatFallDirection = reliveXDirection::eRight;
    u16 mVelX = 0;
    u16 mVelY = 0;
    reliveScale mScale = reliveScale::eFull;
    s16 mMeatAmount = 0;
};

struct Path_Bees final : public Path_TLV
{
    s16 mSwitchId = 0;
    s16 mSwarmSize = 0;
    s16 mChaseTime = 0;
    s16 mSpeed = 0;
    s16 mDisabledResources = 0;
    s16 mBeesAmount = 0;
};


struct Path_BellsongStone final : public Path_TLV
{
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
};

struct Path_MovieStone final : public Path_TLV
{
    s16 mMovieId = 0;
    reliveScale mScale = reliveScale::eFull;

    // AE only
    s32 mTriggerSwitchId = 0;
};

struct Path_HandStone final : public Path_TLV
{
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
};

struct Path_PathTransition final : public Path_TLV
{
    EReliveLevelIds mNextLevel = EReliveLevelIds::eNone;
    s16 mNextPath = 0;
    s16 mNextCamera = 0;
    s16 mMovieId = 0;
    s16 mWipeEffect = 0;
    reliveScale mNextPathScale = reliveScale::eFull;
};

struct Path_Pulley final : public Path_TLV
{
    // No fields
};

struct Path_Honey final : public Path_TLV
{
    // No fields
};

struct Path_BeeSwarmHole final : public Path_TLV
{
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
};

struct Path_Door final : public Path_TLV
{
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
    enum ScreenChangeEffects : s16
    {
        ePlay1FMV,
        eRightToLeft,
        eLeftToRight,
        eBottomToTop,
        eTopToBottom,
        eBoxOut,
        eVerticalSplit,
        eHorizontalSplit,
        eUnknown,
        eInstantChange,
    };
    EReliveLevelIds mNextLevel = EReliveLevelIds::eNone;
    s16 mNextPath = 0;
    s16 mNextCamera = 0;
    reliveScale mScale = reliveScale::eFull;
    s16 mDoorId = 0;
    s16 mSwitchId = 0;
    s16 mTargetDoorId = 0;
    DoorTypes mDoorType = DoorTypes::eBasicDoor;

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
    ScreenChangeEffects mWipeEffect = ScreenChangeEffects::eBoxOut;
    s16 mMovieId = 0;
    s16 mDoorOffsetX = 0;
    s16 mDoorOffsetY = 0;
    reliveXDirection mExitDirection = reliveXDirection::eRight;

    // AE only
    reliveChoice mCloseOnExit = reliveChoice::eNo;
    reliveChoice mClearThrowables = reliveChoice::eNo;
};

struct Path_AbeStart final : public Path_TLV
{
    // No fields
};

struct Path_EnemyStopper final : public Path_TLV
{
    enum class StopDirection : s16
    {
        Left,
        Right,
        Both,
    };
    StopDirection mStopDirection = StopDirection::Both;
    s16 mSwitchId = 0;
};

struct Path_MovingBombStopper final : public Path_TLV
{
    s16 mMinDelay = 0;
    s16 mMaxDelay = 0;
};

struct Path_DoorFlame final : public Path_TLV
{
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
};

struct Path_Mudokon final : public Path_TLV
{
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
};

struct Path_MovingBomb final : public Path_TLV
{
    u16 mSpeed = 0;
    s16 mStartMovingSwitchId = 0;
    reliveChoice mTriggeredByAlarm = reliveChoice::eNo;
    reliveScale mScale = reliveScale::eFull;
    s16 mDisabledResources = 0;
    u16 mStartSpeed = 0;
    reliveChoice mPersistOffscreen = reliveChoice::eYes;
};

struct Path_ElumPathTrans final : public Path_TLV
{
    EReliveLevelIds mNextLevel = EReliveLevelIds::eNone;
    s16 mNextPath = 0;
    s16 mNextCamera = 0;
};

struct Path_MudokonPathTrans final : public Path_TLV
{
    EReliveLevelIds mNextLevel = EReliveLevelIds::eNone;
    s16 mNextPath = 0;
    s32 mNextCamera = 0;
};

struct Path_SecurityClaw final : public Path_TLV
{
    reliveScale mScale = reliveScale::eFull;
    s16 mAlarmSwitchId = 0;
    s16 mAlarmDuration = 0;
    s16 mDisabledResources = 0;
};

struct Path_SlingMudokon final : public Path_TLV
{
    reliveScale mScale =  reliveScale::eFull;
    reliveChoice mDontWhistlePassword = reliveChoice::eYes;
    s16 mCode1 = 0;
    s16 mCode2 = 0;
};

struct Path_FootSwitch final : public Path_TLV
{
    enum class FootSwitchTriggerBy : s16
    {
        eAbe,
        eAnyone,
    };
    s16 mSwitchId = 0;
    reliveScale mScale = reliveScale::eFull;
    reliveSwitchOp mAction = reliveSwitchOp::eSetTrue;
    FootSwitchTriggerBy mTriggeredBy = FootSwitchTriggerBy::eAbe;
};

struct Path_Paramite final : public Path_TLV
{
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
};

struct Path_ZzzSpawner final : public Path_TLV
{
    reliveScale mScale = reliveScale::eFull;
    s16 mSwitchId = 0;
    s16 mZzzInterval = 0;
};

struct Path_BackgroundGlukkon final : public Path_TLV
{
    s16 mScalePercent = 0;
    u16 mPalId = 0;
};

struct Path_GasEmitter final : public Path_TLV
{
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
};

struct Path_GasCountDown final : public Path_TLV
{
    s16 mStartTimerSwitchId = 0;

    // AE only
    u16 mGasCountdownTimer = 0;
    s16 mStopTimerSwitchId = 0;
};

struct Path_RingCancel : public Path_TLV
{
    // No fields
};

struct Path_SecurityDoor final : public Path_TLV
{
    reliveScale mScale = reliveScale::eFull;
    s16 mSwitchId = 0;
    s16 mCode1 = 0;
    s16 mCode2 = 0;
    s16 mXPos = 0;
    s16 mYPos = 0;
};

struct Path_LiftMudokon final : public Path_TLV
{
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
};

struct Path_RingMudokon final : public Path_TLV
{
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
    s16 mOnDestX = 0;
    s16 mOnDestY = 0;
    reliveChoice mEmitLeaves = reliveChoice::eNo;
    s16 mLeafX = 0;
    s16 mLeafY = 0;
};

struct Path_WellExpress final : public relive::Path_WellBase
{
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
};

struct Path_Slog final : public Path_TLV
{
    reliveScale mScale = reliveScale::eFull;
    reliveXDirection mFacing = reliveXDirection::eRight;
    reliveChoice mAsleep = reliveChoice::eNo;
    s16 mWakeUpAnger = 0;
    s16 mBarkAnger = 0;
    s16 mChaseAnger = 0;
    s16 mChaseDelay = 0;
    s16 mDisabledResources = 0;
    s16 mAngerSwitchId = 0;

    // AE only
    s16 mBoneEatingTime = 0;
};

struct Path_SlogSpawner final : public Path_TLV
{
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
};

struct Path_Scrab : public Path_TLV
{
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
    s16 mPatrolTypeRunOrWalkChance = 0; // AE only
    ScrabPatrolType mPatrolType = ScrabPatrolType::eWalk; // AO only
    s16 mPauseLeftMin = 0;
    s16 mPauseLeftMax = 0;
    s16 mPauseRightMin = 0;
    s16 mPauseRightMax = 0;
    u16 mPauseAfterChaseTime = 0;
    s16 mSpottingAbeDelay = 0; // TODO: check if the same field as below?
    s16 mDisabledResources = 0;
    reliveChoice mRoarRandomly = reliveChoice::eNo;

    // AE only
    reliveChoice mPersistant = reliveChoice::eYes;
    s16 mPossessedMaxWhirlAttackDuration = 0;
    reliveChoice mKillEnemy = reliveChoice::eYes;
};

struct Path_Slig final : public Path_TLV
{
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
    s16 mDisabledResources = 0;
    s16 mNoiseWakeUpDistance = 0;
    s16 mSligSpawnerSwitchId = 0;

    // AE only
    reliveChoice mUnlimitedSpawns = reliveChoice::eNo;
};

struct Path_SligSpawner final : public Path_TLV
{
    reliveScale mScale = reliveScale::eFull;
    Path_Slig::StartState mStartState = Path_Slig::StartState::Patrol;
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
    s16 mDisabledResources = 0;
    s16 mNoiseWakeUpDistance = 0;
    s16 mSligSpawnerSwitchId = 0;

    // AE only
    reliveChoice mUnlimitedSpawns = reliveChoice::eNo;
};

struct Path_TrainDoor final : public Path_TLV
{
    reliveXDirection mDirection = reliveXDirection::eRight;
};

struct Path_TorturedMudokon final : public Path_TLV
{
    s16 mKillSwitchId = 0;
    s16 mReleaseSwitchId = 0;
};

struct Path_DoorBlocker final : public Path_TLV
{
    reliveScale mScale = reliveScale::eFull;
    s16 mSwitchId = 0;
};

struct Path_GlukkonSwitch final : public Path_TLV
{
    reliveScale field_10_scale = reliveScale::eFull;
    s16 field_12_ok_switch_id = 0;
    s16 field_14_fail_switch_id = 0;
    u16 field_16_xpos = 0;
    u16 field_18_ypos = 0;
};

struct Path_Greeter final : public Path_TLV
{
    reliveScale mScale = reliveScale::eFull;
    s16 mMotionDetectorSpeed = 0;
    reliveXDirection mFacing = reliveXDirection::eRight;
};

struct Path_BrewMachine final : public Path_TLV
{
    s16 mBrewCount = 0;
};

struct Path_Alarm final : public Path_TLV
{
    s16 mSwitchId = 0;
    u16 mAlarmDuration = 0;
};

struct Path_ParamiteWebLine final : public Path_TLV
{
    reliveScale mScale = reliveScale::eFull;
};

struct Path_SlapLock final : public Path_TLV
{
    reliveScale mScale = reliveScale::eFull;
    s16 mTargetTombSwitchId1 = 0;
    s16 mTargetTombSwitchId2 = 0;
    reliveChoice mHasGhost = reliveChoice::eNo;
    reliveChoice mGiveInvisibilityPowerup = reliveChoice::eNo;
    s16 mInvisibilityDuration = 0;
    s16 mSlapOutputSwitchId = 0;
};

struct Path_StatusLight final : public Path_TLV
{
    s16 mInputSwitchId = 0;
    reliveScale mScale = reliveScale::eFull;
    s16 mLinkedStatusLightSwitchId1 = 0;
    s16 mLinkedStatusLightSwitchId2 = 0;
    s16 mLinkedStatusLightSwitchId3 = 0;
    s16 mLinkedStatusLightSwitchId4 = 0;
    s16 mLinkedStatusLightSwitchId5 = 0;
    reliveChoice mIgnoreGridSnapping = reliveChoice::eNo;
};

} // namespace relive
