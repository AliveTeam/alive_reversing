#pragma once

#include "../../AliveLibAO/Path.hpp"
#include "../../AliveLibAE/Path.hpp"

enum class SwitchOp : s16;

namespace AO {

struct Path_ShadowZone final : public Path_TLV
{
    enum class ShadowZoneScale : s16
    {
        eBoth_0 = 0,
        eHalf_1 = 1,
        eFull_2 = 2,
    };
    s16 field_18_centre_w;
    s16 field_1A_centre_h;
    s16 mRed;
    s16 mGreen;
    s16 mBlue;
    s16 field_22_id;
    ShadowZoneScale mScale;
    s16 field_26_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ShadowZone, 0x28);

struct Path_Hoist final : public Path_TLV
{
    enum class Type : s16
    {
        eNextFloor = 0,
        eNextEdge = 1,
        eOffScreen = 2,
    };
    Type mHoistType;

    enum class GrabDirection : s16
    {
        eFacingLeft = 0,
        eFacingRight = 1,
        eFacingAnyDirection = 2,
    };
    GrabDirection mGrabDirection;
    s16 field_1C_switch_id;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Hoist, 0x20);

struct Path_Lever final : public Path_TLV
{
    enum class LeverSoundType : s16
    {
        eNone = 0,
        eWell_1 = 1,
        eSwitchBellHammer_2 = 2,
        eDoor_3 = 3,
        eElectricWall_4 = 4,
        eSecurityOrb_5 = 5
    };

    enum class LeverSoundDirection : s16
    {
        eLeftAndRight_0 = 0,
        eLeft_1 = 1,
        eRight_2 = 2,
    };
    s16 mSwitchId;
    SwitchOp mAction;
    Scale_short mScale;
    LeverSoundType mOnSound;
    LeverSoundType mOffSound;
    LeverSoundDirection mSoundDirection;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Lever, 0x24);

struct Path_LightEffect final : public Path_TLV
{
    enum class Type : s16
    {
        Star_0 = 0,
        GoldGlow_1 = 1,
        GreenGlow_2 = 2,
        FlintGlow_3 = 3,
        Switchable_RedGreenDoorLights_4 = 4,
        Switchable_RedGreenHubLight_5 = 5,
    };
    Type mType;
    s16 mSize;
    s16 mSwitchId;
    XDirection_short mDirection;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_LightEffect, 0x20);

struct Path_ElectricWall final : public Path_TLV
{
    enum ElectricWallStartState : s16
    {
        eOff_0 = 0,
        eOn_1 = 1,
    };
    Scale_short mScale;
    s16 mSwitchId;
    ElectricWallStartState mStartState;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ElectricWall, 0x20);

struct Path_FallingItem final : public Path_TLV
{
    s16 mSwitchId;
    Scale_short mScale;
    s16 mFallInterval;
    s16 mMaxFallingItems;
    Choice_short mResetSwitchIdAfterUse;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_FallingItem, 0x24);

struct Path_Mine final : public Path_TLV
{
    s16 field_18_num_patterns; // unused
    s16 field_1A_pattern; // unused
    Scale_short mScale;
    s16 mDisabledResources;
    Choice_short mPersistOffscreen;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Mine, 0x24);

struct Path_Dove final : public Path_TLV
{
    s16 mDoveCount;
    Choice_short mPixelPerfect;
    Scale_short mScale;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Dove, 0x20);

struct Path_HintFly final : public Path_TLV
{
    s16 mMessageId;
    s16 field_1A_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_HintFly, 0x1C);

struct Path_Bat final : public Path_TLV
{
    s16 mTimeBeforeMoving;
    s16 mSpeed;
    Scale_short mScale;
    s16 mAttackDuration;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Bat, 0x20);

struct Path_UXB final : public Path_TLV
{
    enum class UXBStartState : s16
    {
        eOn = 0,
        eOff = 1,
    };
    s16 mPatternLength;
    s16 mPattern;
    Scale_short mScale;
    UXBStartState mStartState;
    s32 mDisabledResources;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_UXB, 0x24);

struct Path_SecurityOrb final : public Path_TLV
{
    Scale_short mScale;
    s16 mDisabledResources;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SecurityOrb, 0x1C);

struct Path_BellHammer final : public Path_TLV
{
    s16 mSwitchId;
    SwitchOp mAction;
    Scale_short mScale;
    XDirection_short mDirection;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BellHammer, 0x20);

struct Path_TimerTrigger final : public Path_TLV
{
    s16 mInputSwitchId;
    u16 mActivationDelay;
    s16 mOutputSwitchId1;
    s16 mOutputSwitchId2;
    s16 mOutputSwitchId3;
    s16 mOutputSwitchId4;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TimerTrigger, 0x24);

struct Path_PullRingRope final : public Path_TLV
{
    enum class PullRingSwitchSound : s16
    {
        eNone_0 = 0,
        eWellExit_1 = 1,
        RingBellHammer_2 = 2,
        eDoorEffect_3 = 3,
    };

    enum class PullRingSoundDirection : s16
    {
        eLeftAndRight_0 = 0,
        eLeft_1 = 1,
        eRight_2 = 2
    };
    s16 mSwitchId;
    SwitchOp mAction;
    u16 mRopeLength;
    Scale_short mScale;
    PullRingSwitchSound mOnSound;
    PullRingSwitchSound mOffSound;
    PullRingSoundDirection mSoundDirection;
    s16 field_26_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_PullRingRope, 0x28);

struct Path_MusicTrigger final : public Path_TLV
{
    enum class TriggeredBy : s16
    {
        eTimer_0 = 0,
        eTouching_1 = 1,
        eSwitchID_2 = 2,
        eUnknown_3 = 3,
    };

    enum class MusicTriggerMusicType : s16
    {
        eDrumAmbience_0 = 0,
        eDeathDrumShort_1 = 1,
        eSecretAreaLong_2 = 2,
        eSoftChase_3 = 3,
        eIntenseChase_4 = 4,
        eChime_5 = 5,
        eSecretAreaShort_6 = 6
    };
    MusicTriggerMusicType mMusicType;
    TriggeredBy mTriggeredBy;
    s16 mSwitchId;
    s16 mMusicDelay;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MusicTrigger, 0x20);

struct Path_Pulley final : public Path_TLV
{
    Scale_int scale;
};

struct Path_LiftPoint final : public Path_TLV
{
    enum class LiftPointStopType : s16
    {
        eTopFloor_0 = 0,
        eBottomFloor_1 = 1,
        eMiddleFloor_2 = 2,
        eMiddleLockFloor_3 = 3,
        eStartPointOnly_4 = 4,
    };
    s16 mLiftPointId;
    Choice_short mIsStartPoint;
    s16 field_1C_lift_type;
    LiftPointStopType mLiftPointStopType;
    Scale_short mScale;
    Choice_short mIgnoreLiftMover;
};

class Path_MovingBomb final : public Path_TLV
{
public:
    s16 mSpeed;
    s16 mStartMovingSwitchId;
    Choice_short mTriggeredByAlarm;
    Scale_short mScale;
    s16 field_20_max_rise; // seems unused
    s16 mDisabledResources;
    s16 mStartSpeed;
    Choice_short mPersistOffscreen;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MovingBomb, 0x28);

struct Path_MovingBombStopper final : public Path_TLV
{
    s16 mMinStopTime;
    s16 mMaxStopTime;
};
// TODO: size

struct Path_MeatSaw final : public Path_TLV
{
    Scale_short mScale;
    s16 mSwitchMinTimeOff;
    s16 mSwitchMaxTimeOff;
    s16 mMaxRiseTime;
    s16 mSwitchId;
    enum class Type : s16
    {
        eAutomaticPersistOffscreen_0 = 0,
        eAutomatic_1 = 1,
        eSwitchId_2 = 2
    };
    Type mType;
    s16 mSpeed;
    enum class StartState : s16
    {
        eOff_0 = 0,
        eOn_1 = 1
    };
    StartState mStartState;
    s16 mOffSpeed;
    s16 mAutomaticMinTimeOff;
    s16 mAutomaticMaxTimeOff;
    s16 mInitialPosition;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MeatSaw, 0x30);

struct Path_LCDScreen final : public Path_TLV
{
    s16 mMessageId1;
    s16 mMessageRandMinId;
    s16 mMessageRandMaxId;
    s16 field_1E_message_2_id_not_used;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_LCDScreen, 0x20);

struct Path_InvisibleSwitch final : public Path_TLV
{
    enum class InvisibleSwitchScale : s16
    {
        eHalf_0 = 0,
        eFull_1 = 1,
        eAny_2 = 2,
    };
    s16 mSwitchId;
    SwitchOp mAction;
    u16 mActivationDelay;
    Choice_short mSetOffAlarm;
    InvisibleSwitchScale mScale;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_InvisibleSwitch, 0x24);

struct Path_TrapDoor final : public Path_TLV
{
    s16 mSwitchId;
    enum StartState : s16
    {
        eOpen_0 = 0,
        eClosed_1 = 1,
    };
    StartState mStartState;
    Choice_short mSelfClosing;
    Scale_short mScale;
    s16 field_20_dest_level;
    XDirection_short mDirection;
    s16 mXOff;
    s16 field_26_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TrapDoor, 0x28);

enum class PortalSide : s16
{
    eRight_0 = 0,
    eLeft_1 = 1,
};

struct Path_BirdPortal final : public Path_TLV
{
    enum class PortalType : s16
    {
        eAbe_0 = 0,
        eWorker_1 = 1,
        eShrykull_2 = 2,
        eMudTeleport_3 = 3,
    };
    PortalSide mEnterSide;
    LevelIds mExitLevel;
    s16 mExitPath;
    s16 mExitCamera;
    Scale_short mScale;
    s16 mMovieId;
    PortalType mPortalType;
    s16 mMudCountForShrykull;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BirdPortal, 0x28);

struct Path_BirdPortalExit final : public Path_TLV
{
    PortalSide mExitSide;
    Scale_short mScale;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BirdPortalExit, 0x1C);


struct Path_BoomMachine final : public Path_TLV
{
    Scale_short mScale;
    enum class NozzleSide : s16
    {
        eRight_0 = 0,
        eLeft_1 = 1
    };
    NozzleSide mNozzleSide;
    s16 mDisabledResources;
    s16 mGrenadeAmount;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BoomMachine, 0x20);

struct Path_Slog final : public Path_TLV
{
    Scale_short mScale;
    XDirection_short mFacing;
    Choice_short mAsleep;
    s16 mWakeUpAnger;
    s16 mBarkAnger;
    s16 mChaseAnger;
    s16 mChaseDelay;
    s16 mDisabledResources;
    s16 mAngerSwitchId;
    s16 field_2A_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Slog, 0x2C);

struct Path_ChimeLock final : public Path_TLV
{
    Scale_short mScale;
    s16 mSolveSwitchId;
    s16 mCode1;
    s16 mCode2;
    s16 mPasswordSwitchId;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ChimeLock, 0x24);

struct Path_FlintLockFire final : public Path_TLV
{
    Scale_short mScale;
    s16 mSwitchId;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_FlintLockFire, 0x1C);

struct Path_LiftMover final : public Path_TLV
{
    s16 mLiftMoverSwitchId;
    s16 mTargetLiftPointId;
    enum class YDirection : s16
    {
        eDown_0 = 0,
        eUp_1 = 1
    };
    YDirection mMoveDirection;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_LiftMover, 0x20);

struct Path_SlogSpawner final : public Path_TLV
{
    enum class StartDirection : s16
    {
        eRight_0 = 0,
        eLeft_1 = 1
    };
    Scale_short mScale;
    s16 mMaxSlogs;
    s16 mMaxSlogsAtATime;
    StartDirection mStartDirection;
    s16 mSlogSpawnInterval;
    s16 mSpawnerSwitchId;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SlogSpawner, 0x24);

struct Path_Scrab final : public Path_TLV
{
    enum class ScrabPatrolType : s16
    {
        eWalk_0 = 0,
        eRunOrWalk192_1 = 1,
        eRunOrWalk128_2 = 2,
        eRunOrWalk64_3 = 3,
        eRun_4 = 4
    };
    Scale_short mScale;
    s16 mAttackDelay;
    ScrabPatrolType mPatrolType;
    s16 mPauseLeftMin;
    s16 mPauseLeftMax;
    s16 mPauseRightMin;
    s16 mPauseRightMax;
    s16 mSpottingAbeDelay;
    s16 mDisabledResources;
    Choice_short mRoarRandomly;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Scrab, 0x2C);

struct Path_ScrabNoFall final : public Path_TLV
{
    // No fields
};

struct Path_ScrabBoundLeft final : public Path_TLV
{
    // No fields
};

struct Path_ScrabBoundRight final : public Path_TLV
{
    // No fields
};

struct Path_RockSack final : public Path_TLV
{
    XDirection_short mRockFallDirection;
    s16 mVelX;
    s16 mVelY;
    Scale_short mScale;
    s16 mRockAmount;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_RockSack, 0x24);

struct Path_ZzzSpawner final : public Path_TLV
{
    Scale_short mScale;
    s16 mSwitchId;
    s16 mZzzInterval;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ZzzSpawner, 0x20);

struct Path_SecurityClaw final : public Path_TLV
{
    Scale_short mScale;
    s16 mAlarmSwitchId;
    s16 mAlarmDuration;
    s16 mDisabledResources;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SecurityClaw, 0x20);

struct Path_SecurityDoor final : public Path_TLV
{
    Scale_short mScale;
    s16 mSwitchId;
    s16 mCode1;
    s16 mCode2;
    s16 mXPos;
    s16 mYPos;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SecurityDoor, 0x24);

struct Path_TimedMine final : public Path_TLV
{
    s16 mSwitchId;
    s16 mState;
    Scale_short mScale;
    s16 mTicksUntilExplosion;
    s16 mDisabledResources;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TimedMine, 0x24);

struct Path_MotionDetector final : public Path_TLV
{
    Scale_short mScale;
    s16 mDeviceX;
    s16 mDeviceY;
    s16 mSpeedx256;
    enum class InitialMoveDirection : s16
    {
        eRight_0 = 0,
        eLeft_1 = 1
    };
    InitialMoveDirection mInitialMoveDirection;
    Choice_short mDrawFlare;
    s16 mDisableSwitchId;
    s16 mAlarmSwitchId;
    s16 mAlarmDuration;
    s16 field_2A_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MotionDetector, 0x2C);

struct Path_BackgroundAnimation final : public Path_TLV
{
    enum class BgAnimSounds : s16
    {
        eNone_m1 = -1,
        eNone_0 = 0,
        eFire_1 = 1,
    };
    u16 mAnimId;
    Choice_short mIsSemiTrans;
    u8 mSemiTransMode; // TODO: Use AEType
    // pad
    BgAnimSounds mSoundEffect;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BackgroundAnimation, 0x20);
// TODO: size

struct Path_LCDStatusBoard final : public Path_TLV
{
    // No fields
};

struct Path_HoneySack final : public Path_TLV
{
    s16 mChaseTime;
    Scale_short mScale;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_HoneySack, 0x1C);

struct Path_SlingMudokon final : public Path_TLV
{
    Scale_short mScale;
    Choice_short mDontWhistlePassword;
    s16 mCode1;
    s16 mCode2;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SlingMudokon, 0x20);

struct Path_BeeSwarmHole final : public Path_TLV
{
    enum class MovementType : s16
    {
        eHover_0 = 0,
        eAttack_1 = 1,
        eFollowPath_2 = 2,
    };
    s16 field_18_what_to_spawn;
    s16 mStartInterval;
    s16 field_1C_fall_switch_id;
    MovementType mMovementType;
    s16 mBeesAmount;
    s16 mChaseTime;
    s16 mSpeed;
    Scale_short field_26_scale;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BeeSwarmHole, 0x28);

struct Path_MeatSack final : public Path_TLV
{
    XDirection_short mMeatFallDirection;
    u16 mVelX;
    s16 mVelY;
    Scale_short mScale;
    s16 mMeatAmount;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MeatSack, 0x24);

struct Path_RollingBall final : public Path_TLV
{
    Scale_short mScale;
    XDirection_short mRollDirection;
    s16 mReleaseSwitchId;
    u16 mMaxSpeed;
    u16 mAcceleration;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_RollingBall, 0x24);

struct Path_RollingBallStopper final : public Path_TLV
{
    s16 mStopperSwitchId;
    Scale_short mScale;
    s16 mBallSwitchId;
    XDirection_short mStopDirection;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_RollingBallStopper, 0x20);

struct Path_FootSwitch final : public Path_TLV
{
    enum class FootSwitchTriggerBy : s16
    {
        eAbe_0 = 0,
        eAnyone_1 = 1,
    };
    s16 mSwitchId;
    Scale_short mScale;
    SwitchOp mAction;
    FootSwitchTriggerBy mTriggeredBy;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_FootSwitch, 0x20);

struct Path_ZBall final : public Path_TLV
{
    enum class StartPos : s16
    {
        eCenter_0 = 0,
        eOut_1 = 1,
        eIn_2 = 2
    };

    enum class Speed : s16
    {
        eNormal_0 = 0,
        eFast_1 = 1,
        eSlow_2 = 2
    };

    StartPos mStartPos;
    Scale_short mScale;
    Speed mSpeed;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ZBall, 0x20);

struct Path_Honey final : public Path_TLV
{
    s16 mSwitchId;
    s16 mState;
    Scale_int mScale;
};
// TODO: size

struct Path_HoneyDripTarget final : public Path_TLV
{
    // No fields
};

struct Path_BeeNest final : public Path_TLV
{
    s16 mSwitchId;
    s16 mSwarmSize;
    s16 mChaseTime;
    s16 mSpeed;
    s16 mDisabledResources;
    s16 mBeesAmount;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BeeNest, 0x24);

struct Path_MainMenuController final : public Path_TLV
{
    // No fields
};

struct Path_DoorFlame final : public Path_TLV
{
    s16 mSwitchId;
    enum class Scale : s16
    {
        eFull_0 = 0,
        eHalf_1 = 1,
        eHalf_2 = 2
    };
    Scale mScale;

    enum class Colour : s16
    {
        default_0 = 0,
        red_1 = 1,
        green_2 = 2,
        blue_3 = 3
    };
    Colour mColour;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_DoorFlame, 0x20);

struct Path_CreditsController final : public Path_TLV
{
    // No fields
};

struct Path_BackgroundGlukkon final : public Path_TLV
{
    s16 mScalePercent;
    u16 mPalId;
    s16 mTargetId; // unused
    s16 mVoiceAdjust; // unused
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BackgroundGlukkon, 0x20);

struct Path_GasEmitter final : public Path_TLV
{
    // No fields
};

struct Path_GasCountDown final : public Path_TLV
{
    s16 mStartTimerSwitchId;
    s16 field_1A_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_GasCountDown, 0x1C);

struct Path_Door final : public Path_TLV
{
    enum DoorStates : s16
    {
        eOpen_0 = 0,
        eClosed_1 = 1,
        eOpening_2 = 2,
        eClosing_3 = 3,
    };

    enum class DoorTypes : s16
    {
        eBasicDoor_0 = 0,
        eTrialDoor_1 = 1,
        eHubDoor_2 = 2,
    };
    LevelIds mNextLevel;
    s16 mNextPath;
    s16 mNextCamera;
    Scale_short mScale;
    u16 mDoorId;
    s16 mSwitchId;
    s16 mTargetDoorId;
    DoorTypes mDoorType;
    Choice_short mDoorClosed;
    s16 mHub1;
    s16 mHub2;
    s16 mHub3;
    s16 mHub4;
    s16 mHub5;
    s16 mHub6;
    s16 mHub7;
    s16 mHub8;
    s16 mWipeEffect;
    s16 mMovieId;
    s16 mDoorOffsetX;
    s16 mDoorOffsetY;
    s16 field_42_wipe_x_org;
    s16 field_44_wipe_y_org;
    XDirection_short mExitDirection;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Door, 0x48);

union OffLevelOrDx
{
    LevelIds level;
    s16 dx;
};

struct Path_WellBase : public Path_TLV
{
    Scale_short mScale;
    s16 mSwitchId;
    s16 mOtherWellId;
    s16 mAnimId;
    s16 mExitX;
    s16 mExitY;
    OffLevelOrDx mOffLevelOrDestX;
    s16 mOffPathOrDestY;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_WellBase, 0x28);

struct Path_WellExpress final : public Path_WellBase
{
    s16 mOffDestCamera;
    s16 mOffOtherWellId;
    LevelIds mOnDestLevel;
    s16 mOnDestPath;
    s16 mOnDestCamera;
    s16 mOnOtherWellId;
    Choice_short mEmitLeaves;
    s16 mLeafX;
    s16 mLeafY;
    s16 mMovieId;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_WellExpress, 0x3C);

struct Path_WellLocal final : public Path_WellBase
{
    s16 mOnDestX;
    s16 mOnDestY;
    Choice_short mEmitLeaves;
    s16 mLeafX;
    s16 mLeafY;
    s16 field_32_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_WellLocal, 0x34);

struct Path_ElumPathTrans final : public Path_TLV
{
    LevelIds mNextLevel;
    s16 mNextPath;
    s16 mNextCamera;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ElumPathTrans, 0x20);

struct Path_ElumStart final : public Path_TLV
{
    // No fields
};

struct Path_ElumWall final : public Path_TLV
{
    // No fields
};

struct Path_Mudokon final : public Path_TLV
{
    Scale_short mScale;
    enum class MudJobs : s16
    {
        eStandScrub_0 = 0,
        eSitScrub_1 = 1,
        eSitChant_2 = 2,
    };
    MudJobs mJob;
    XDirection_short mFacing;
    s16 mVoicePitch;
    s16 mRescueSwitchId;
    Choice_short mDeaf;
    s16 mDisabledResources;
    s16 mPersistAndResetOffscreen;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Mudokon, 0x28);

struct Path_MudokonPathTrans final : public Path_TLV
{
    LevelIds mNextLevel;
    s16 mNextPath;
    s32 mNextCamera;
};

struct Path_RingMudokon final : public Path_TLV
{
    XDirection_short mFacing;
    enum class MustFaceMud : s16
    {
        eYes_0 = 0,
        eNo_1 = 1
    };
    MustFaceMud mAbeMustFaceMud;
    Scale_short mScale;
    Choice_short mGivePassword;
    s16 mCode1;
    s16 mCode2;
    SwitchOp mAction;
    s16 mRingTimeout;
    Choice_short mGiveRingWithoutPassword;
    s16 field_2A_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_RingMudokon, 0x2C);

struct Path_LiftMudokon final : public Path_TLV
{
    s16 mHowFarToWalk;
    s16 mLiftSwitchId;
    enum class Direction : s16
    {
        eRight_0 = 0,
        eLeft_1 = 1
    };
    Direction mFacing;
    Choice_short mGivePassword;
    Scale_short mScale;
    s16 mCode1;
    s16 mCode2;
    s16 field_26_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_LiftMudokon, 0x28);

struct Path_Paramite final : public Path_TLV
{
    Scale_short mScale;
    Choice_short mEnterFromWeb;
    s16 mAloneChaseDelay;
    s16 mSurpriseWebDelayTimer;
    s16 mMeatEatingTime;
    s16 mGroupChaseDelay;
    s16 field_24_disabled_resources; // Unused
    s16 mSurpriseWebSwitchId;
    Choice_short mHissBeforeAttack;
    Choice_short mDeleteWhenOutOfSight;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Paramite, 0x2C);

struct Path_EnemyStopper final : public Path_TLV
{
    enum class StopDirection : s16
    {
        Left_0 = 0,
        Right_1 = 1,
        Both_2 = 2,
    };
    StopDirection mStopDirection;
    s16 mSwitchId;
};
// TODO:: size

struct Path_PathTransition final : public Path_TLV
{
    LevelIds mNextLevel;
    s16 mNextPath;
    s16 mNextCamera;
    s16 mMovieId;
    u16 mWipeEffect;
    Scale_short mNextPathScale;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_PathTransition, 0x24);

struct Path_SwitchStateBooleanLogic final : public Path_TLV
{
    enum class BooleanOperatorType : s16
    {
        eAllOn_0 = 0,
        e1OnAnd2Off_1 = 1,
        e1Or2On_2 = 2,
        e1OnOr2Off_3 = 3,
    };
    s16 mInput1;
    s16 mInput2;
    s16 mOutput;
    BooleanOperatorType mOperator;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SwitchStateBooleanLogic, 0x20);

struct Path_Slig final : public Path_TLV
{
    enum class StartState : s16
    {
        Listening_0 = 0,
        Patrol_1 = 1,
        Sleeping_2 = 2,
        Chase_3 = 3,
        ChaseAndDisappear_4 = 4,
        FallingToChase_5 = 5,
    };
    Scale_short mScale;
    StartState mStartState;
    s16 mPauseTime;
    s16 mPauseLeftMin;
    s16 mPauseLeftMax;
    s16 mPauseRightMin;
    s16 mPauseRightMax;
    enum class ShootPossessedSligs : s16
    {
        eNo_0 = 0,
        eYes_1 = 1,
        eYes_2 = 2, // used in an OG level, breaks lvl exporting if removed
    };
    ShootPossessedSligs mShootPossessedSligs;
    s16 mShootOnSightDelay;
    s16 mNumTimesToShoot;
    s16 field_2C_unused; // unused
    s16 mCode1;
    s16 mCode2;
    Choice_short mChaseAbeWhenSpotted;
    XDirection_short mFacing;
    s16 mPanicTimeout;
    s16 mNumPanicSounds; // unused
    s16 mPanicSoundTimeout; // unused
    s16 mStopChaseDelay;
    s16 mTimeToWaitBeforeChase;
    s16 mSligBoundId;
    s16 mAlertedListenTime;
    s16 mPercentSayWhat;
    s16 mPercentBeatMud;
    s16 mTalkToAbe; // unused
    s16 field_4A_dont_shoot; // unused
    s16 mZShootDelay;
    Choice_short mStayAwake;
    BitField16<SligFlags_DisabledRes> mDisabledResources;
    s16 mNoiseWakeUpDistance;
    s16 mSligSpawnerSwitchId;
    s16 field_56_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Slig, 0x58);

struct Path_ZSligCover final : public Path_TLV
{
    // No fields
};

// This is a left bound, right bound and a persist.
struct Path_SligBound final : public Path_TLV
{
    s16 mSligBoundId;
    BitField16<SligFlags_DisabledRes> mDisabledResources;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SligBound, 0x1C);

using Path_SligBoundLeft = Path_SligBound;
using Path_SligBoundRight = Path_SligBound;
using Path_SligPersist = Path_SligBound;

struct Path_SligSpawner final : public Path_TLV
{
    Scale_short mScale;
    Path_Slig::StartState mStartState;
    s16 mPauseTime;
    s16 mPauseLeftMin;
    s16 mPauseLeftMax;
    s16 mPauseRightMin;
    s16 mPauseRightMax;
    Path_Slig::ShootPossessedSligs mShootPossessedSligs;
    s16 mShootOnSightDelay;
    s16 mNumTimesToShoot;
    s16 field_2C_unused; // unused
    s16 mCode1;
    s16 mCode2;
    Choice_short mChaseAbeWhenSpotted;
    XDirection_short mFacing;
    s16 mPanicTimeout;
    s16 field_38_num_panic_sounds; // unused
    s16 field_3A_panic_sound_timeout; // unused
    s16 mStopChaseDelay;
    s16 mTimeToWaitBeforeChase;
    s16 mSligBoundId;
    s16 mAlertedListenTime;
    s16 mPercentSayWhat;
    s16 mPercentBeatMud;
    s16 field_48_talk_to_abe; // unused
    s16 field_4A_dont_shoot; // unused
    s16 mZShootDelay;
    Choice_short mStayAwake;
    s16 mDisabledResources;
    s16 mNoiseWakeUpDistance;
    s32 mSligSpawnerSwitchId;
};
// TODO: size

struct Path_ResetPath final : public Path_TLV
{
    Choice_short mClearIds;
    s16 mFrom;
    s16 mTo;
    s16 mExclude;
    Choice_short mClearObjects;
    s16 mPath;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ResetPath, 0x24);

struct Path_StartController final : public Path_TLV
{
    // No fields
};

struct Path_InvisibleZone final : public Path_TLV
{
    // No fields
};

struct Path_KillUnsavedMuds final : public Path_TLV
{
    // No fields
};

struct Path_RingCancel : public Path_TLV
{
    // No fields
};

struct Path_Edge final : public Path_TLV
{
    enum class GrabDirection : s16
    {
        eFacingLeft = 0,
        eFacingRight = 1,
        eFacingAnyDirection = 2,
    };
    GrabDirection mGrabDirection;
    Choice_short mCanGrab;
};
// TODO: size

struct Path_AbeStart final : public Path_TLV
{
    Scale_int scale;
};

struct Path_ContinueZone final : public Path_TLV
{
    s32 field_10_zone_number;
};

struct Path_DeathDrop final : public Path_TLV
{
    s16 animation;
    s16 sound;
    s16 id;
    s16 action;
    s32 set_value;
};

struct Path_Stone_camera final
{
    LevelIds level;
    s16 path;
    s16 camera;
};

struct Path_Handstone_data final
{
    Scale_short scale;
    Path_Stone_camera camera1;
    Path_Stone_camera camera2;
    Path_Stone_camera camera3;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Handstone_data, 0x14);

struct Path_HandStone final : public Path_TLV
{
    Path_Handstone_data mData;
};

enum class BellsongTypes : s16
{
    eWhistle = 0,
    eChimes = 1
};

struct Path_BellsongStone_data final
{
    Scale_short mScale;
    BellsongTypes mType;
    s16 mCode1;
    s16 mCode2;
    s16 mSwitchId;
    s16 pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BellsongStone_data, 12);

struct Path_BellsongStone final : public Path_TLV
{
    Path_BellsongStone_data mData;
};

struct Path_Moviestone_data final
{
    s16 mMovieId;
    Scale_short mScale;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Moviestone_data, 4);

struct Path_MovieStone final : public Path_TLV
{
    Path_Moviestone_data mData;
};

union AllStone
{
    Path_Handstone_data dataHandstone;
    Path_BellsongStone_data dataBellsong;
    Path_Moviestone_data dataMovie;
    u16 demoId;
};

struct Path_Stone final : public Path_TLV
{
    AllStone field_18_data;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Stone, 0x2C);

struct Path_SoftLanding final : public Path_TLV
{
    // No fields
};

struct Path_ContinuePoint final : public Path_TLV
{
    s16 mZoneNumber;
    s16 mClearFromId;
    s16 mClearToId;
    Choice_short mElumRestarts;
    enum class spawnDirection : s16
    {
        eRight_0 = 0,
        eLeft_1 = 1
    };
    spawnDirection mAbeSpawnDir;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ContinuePoint, 0x24);

struct Path_OneShotSwitchIdSetter final : public Path_TLV
{
    s16 mIdsToClear[8];
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_OneShotSwitchIdSetter, 0x28);

} // namespace AO
