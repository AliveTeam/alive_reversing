#pragma once

#include "../Types.hpp"
#include "../../AliveLibAE/Path.hpp"
#include "../Psx.hpp"

// TODO: Don't use this directly in here, use AE type and convert to relive type
enum class SwitchOp : s16;

struct Path_TLV
{
    BitField8<TlvFlags> mTlvFlags;
    u8 mTlvSpecificMeaning;
    s16 mLength;
    TlvTypes32 mTlvType32;
    PSX_Point mTopLeft;
    PSX_Point mBottomRight;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TLV, 0x10);

struct Path_Fleech final : public Path_TLV
{
    Scale_short mScale;
    XDirection_short mFacing;
    Choice_short mAsleep;
    s16 padding1;
    s16 padding2;
    s16 mAttackAngerIncreaser;
    s16 padding3;
    s16 mWakeUpSwitchId;
    Choice_short mHanging;
    s16 mLostTargetTimeout;
    Choice_short mGoesToSleep;
    s16 mPatrolRangeInGrids;
    s16 mWakeUpSwitchAngerValue;
    s16 mCanWakeUpSwitchId;
    Choice_short mPersistant;
    s16 padding4;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Fleech, 0x30);

struct Path_UXB final : public Path_TLV
{
    enum class StartState : s16
    {
        eOn = 0,
        eOff = 1,
    };
    s16 mPatternLength;
    s16 mPattern;
    Scale_short mScale;
    StartState mStartState;
    s32 mDisabledResources;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_UXB, 0x1c);

struct Path_WorkWheel final : public Path_TLV
{
    Scale_short mScale;
    s16 mSwitchId;
    s16 mActivationTime;
    s16 mTurnOffTime;
    Choice_short mTurnOffWhenStopped;
    s16 padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_WorkWheel, 0x1C);

struct Path_SlamDoor final : public Path_TLV
{
    Choice_short mStartClosed;
    Scale_short mScale;
    s16 mSwitchId;
    Choice_short mFlipY;
    Choice_short mDelete;
    s16 padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SlamDoor, 0x1C);

struct Path_LaughingGas final : public Path_TLV
{
    Choice_short mLaughingGas;
    s16 mLaughingGasSwitchId;
    s16 mRedPercent;
    s16 mGreenPercent;
    s16 mBluePercent;
    s16 padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_LaughingGas, 0x1C);

struct Path_MusicTrigger final : public Path_TLV
{
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

    enum class TriggeredBy : s16
    {
        eTimer_0 = 0,
        eTouching_1 = 1
    };

    MusicTriggerMusicType mMusicType;
    TriggeredBy mTriggeredBy;
    s16 mMusicDelay;
    s16 field_16_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MusicTrigger, 0x18);

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
    s16 mActivationDelay;
    Choice_short mSetOffAlarm;
    InvisibleSwitchScale mScale;
    s16 field_1A_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_InvisibleSwitch, 0x1C);

struct Path_Water_Data final
{
    s16 mMaxDrops;
    s16 mSwitchId;
    s16 mSplashTime;
    s16 mSplashVelX;
    s16 mSplashVelY; // unused
    s16 mWaterDuration;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Water_Data, 0xC);

struct Path_Water final : public Path_TLV
{
    Path_Water_Data mWaterData;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Water, 0x1C);

struct Path_GasEmitter final : public Path_TLV
{
    enum class GasColour : s16
    {
        Yellow_0,
        Red_1,
        Green_2,
        Blue_3,
        White_4
    };
    s16 mSwitchId;
    GasColour mColour;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_GasEmitter, 0x14);

struct Path_StatusLight final : public Path_TLV
{
    s16 mInputSwitchId;
    Scale_short mScale;
    s16 mLinkedStatusLightSwitchId1;
    s16 mLinkedStatusLightSwitchId2;
    s16 mLinkedStatusLightSwitchId3;
    s16 mLinkedStatusLightSwitchId4;
    s16 mLinkedStatusLightSwitchId5;
    Choice_short mIgnoreGridSnapping;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_StatusLight, 0x20);

struct Path_PullRingRope final : public Path_TLV
{
    enum class PullRingSwitchSound : s16
    {
        eNone_0 = 0,
        eWellExit_1 = 1,
        eRingBellHammer_2 = 2,
        eDoorEffect_3 = 3,
    };

    enum class PullRingSoundDirection : s16
    {
        eLeftAndRight_0 = 0,
        eLeft_1 = 1,
        eRight_2 = 2,
    };

    s16 mSwitchId;
    SwitchOp mAction;
    s16 mRopeLength;
    Scale_short mScale;
    PullRingSwitchSound mOnSound;
    PullRingSwitchSound mOffSound;
    PullRingSoundDirection mSoundDirection;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_PullRingRope, 0x20);

struct Path_SecurityClaw final : public Path_TLV
{
    Scale_int mScale;
    s16 mDisabledResources;
    s16 field_14_unknown;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SecurityClaw, 0x18);

struct Path_SecurityOrb final : public Path_TLV
{
    Scale_short mScale;
    s16 mDisabledResources;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SecurityOrb, 0x14);

struct Path_WheelSyncer final : public Path_TLV
{
    enum class WheelSyncerOutputRequirement : s16
    {
        eAllOn_0 = 0,
        e1OnAnd2Off_1 = 1,
        e1Or2On_2 = 2,
        e1OnOr2Off_3 = 3
    };
    s16 mInputSwitchId1;
    s16 mInputSwitchId2;
    s16 mOutputSwitchId;
    WheelSyncerOutputRequirement mOutputRequirement;
    s16 mInputSwitchId3;
    s16 mInputSwitchId4;
    s16 mInputSwitchId5;
    s16 mInputSwitchId6;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_WheelSyncer, 0x20);

struct Path_Drill_Data final
{
    enum class DrillDirection : s16
    {
        eDown_0 = 0,
        eRight_1 = 1,
        eLeft_2 = 2,
    };

    enum class DrillBehavior : s16
    {
        eNotInteractable_0 = 0,
        eToggle_1 = 1,
        eUse_2 = 2
    };
    Scale_short mScale;
    s16 mOnMinPauseTime;
    s16 mOnMaxPauseTime;
    s16 mSwitchId;
    DrillBehavior mDrillBehavior;
    s16 mOnSpeed;
    Choice_short mStartStateOn;
    s16 mOffSpeed;
    s16 mOffMinPauseTime;
    s16 mOffMaxPauseTime;
    Choice_short mStartPositionBottom;
    DrillDirection mDirection;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Drill_Data, 0x18);

struct Path_Drill final : public Path_TLV
{
    Path_Drill_Data mDrillData;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Drill, 0x28);

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
        // eMudTeleport_3 = 3, // TODO: not used ??
    };
    PortalSide mEnterSide;
    LevelIds mExitLevel;
    s16 mExitPath;
    s16 mExitCamera;
    Scale_short mScale;
    s16 mMovieId;
    PortalType mPortalType;
    s16 mMudCountForShrykull;
    s16 mCreatePortalSwitchId;
    s16 mDeletePortalSwitchId;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BirdPortal, 0x24);

struct Path_BirdPortalExit final : public Path_TLV
{
    PortalSide mExitSide;
    Scale_short mScale;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BirdPortal, 0x24);

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
    s16 field_16_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ElectricWall, 0x18);

struct Path_Lever final : public Path_TLV
{
    enum class LeverSoundType : s16
    {
        eNone = 0,
        eWell_1 = 1,
        eSwitchBellHammer_2 = 2,
        eDoor_3 = 3,
        eElectricWall_4 = 4,
        eSecurityOrb_5 = 5,
        eLift_6 = 6
    };

    enum class LeverSoundDirection : s16
    {
        eLeftAndRight_0 = 0,
        eLeft_1 = 1,
        eRight_2 = 2,
    };
    SwitchOp mAction;
    Scale_short mScale;
    LeverSoundType mOnSound;
    LeverSoundType mOffSound;
    LeverSoundDirection mSoundDirection;
    s16 mSwitchId;
    Choice_short mPersistOffscreen;
    s16 field_1E_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Lever, 0x20);

struct Path_LCDStatusBoard final : public Path_TLV
{
    s16 mNumberOfMuds;
    s16 mZulagNumber;
    Choice_int mHideBoard;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_LCDStatusBoard, 0x18);

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
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BoomMachine, 0x18);

struct Path_LCDScreen final : public Path_TLV
{
    s16 mMessageId1;
    s16 mMessageRandMinId;
    s16 mMessageRandMaxId;
    s16 mMessageId2;
    s32 mToggleMessageSwitchId;
};
// TODO: size

struct Path_BackgroundAnimation final : public Path_TLV
{
    u16 mAnimId;
    Choice_short mIsSemiTrans;
    u8 mSemiTransMode; // Must match size on disk, TODO: need OG data enum
    // pad
    s16 field_16_sound_effect; // unused
    s16 field_18_id; // probably also unused
    Layer mLayer;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BackgroundAnimation, 0x1C);

struct Path_LevelLoader final : public Path_TLV
{
    s16 mSwitchId;
    LevelIds mDestLevel;
    s16 mDestPath;
    s16 mDestCamera;
    s16 mMovieId;
    s16 padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_LevelLoader, 0x1C);

struct Path_BrewMachine final : public Path_TLV
{
    s16 mBrewCount;
    s16 field_12_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BrewMachine, 0x14);

struct Path_Mine final : public Path_TLV
{
    s16 field_10_num_patterns; // unused
    s16 field_12_pattern; // unused
    Scale_short mScale;
    s16 mDisabledResources;
    Choice_short mPersistOffscreen;
    s16 pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Mine, 0x1C);

struct Path_Dove final : public Path_TLV
{
    s16 mDoveCount;
    Choice_short mPixelPerfect;
    Scale_short mScale;
    s16 field_16_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Dove, 0x18);

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
    s16 field_18_dest_level;
    XDirection_short mDirection;
    s16 mXOff;
    s16 mStayOpenTime;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TrapDoor, 0x20);

struct Path_RockSack final : public Path_TLV
{
    XDirection_short mRockFallDirection;
    u16 mVelX;
    u16 mVelY;
    Scale_short mScale;
    s16 mRockAmount;
    s16 field_1A_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_RockSack, 0x1C);

struct Path_TimerTrigger final : public Path_TLV
{
    s16 mInputSwitchId;
    u16 mActivationDelay;
    s16 mOutputSwitchId1;
    s16 mOutputSwitchId2;
    s16 mOutputSwitchId3;
    s16 mOutputSwitchId4;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TimerTrigger, 0x1C);

struct Path_TimedMine final : public Path_TLV
{
    s16 mSwitchId;
    s16 mState;
    Scale_short mScale;
    s16 mTicksUntilExplosion;
    s32 mDisabledResources;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TimedMine, 0x1C);

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
    s16 field_22_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MotionDetector, 0x24);

struct Path_MineCar final : public Path_TLV
{
    Scale_short mScale;
    s16 mMaxDamage; // unused
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MineCar, 0x14);

struct Path_ExplosionSet final : public Path_TLV
{
    Choice_short mStartEnabled;
    s16 mSwitchId;
    Choice_short mSpawnAssets;
    s16 mStartDelay;
    XDirection_short mStartDirection;
    s16 mAssetInterval;
    s16 mGridSpacing;
    s16 mIncreasingGridSpacing;
    Scale_short mScale;
    s16 padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ExplosionSet, 0x24);

struct Path_ColourfulMeter final : public Path_TLV
{
    s16 mSwitchId;
    s16 mNumberOfMeterBars;
    s16 mMinesAlarmCountdown;
    Choice_short mStartFilled;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ColourfulMeter, 0x18);

struct Path_SlapLock final : public Path_TLV
{
    Scale_short mScale;
    s16 mTargetTombSwitchId1;
    s16 mTargetTombSwitchId2;
    Choice_short mPersistant; // unused
    Choice_short mHasGhost;
    Choice_short mGiveInvisibilityPowerup;
    s16 mInvisibilityDuration;
    s16 mSlapOutputSwitchId;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SlapLock, 0x20);

struct Slurg_Path_Data final
{
    s16 mMovingTimer;
    XDirection_short mFacing;
    Scale_short mScale;
    s16 mSwitchId;
};

struct Path_Slurg : public Path_TLV
{
    Slurg_Path_Data mSlurgData;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Slurg, 0x18);

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
    s16 mBoneEatingTime;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Slog, 0x24);

struct Path_LiftMover final : public Path_TLV
{
    s16 mLiftMoverSwitchId;
    s16 mTargetLiftPointId;
    enum class YDirection : s16
    {
        eDown_0 = 0,
        eUp_1 = 1,
    };
    YDirection mMoveDirection;
    s16 field_16_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_LiftMover, 0x18);

struct Path_DoorBlocker final : public Path_TLV
{
    Scale_short mScale;
    s16 mSwitchId;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_DoorBlocker, 0x14);

struct Path_DoorFlame final : public Path_TLV
{
    s16 mSwitchId;
    Scale_short mScale;
    s16 field_14_colour;
    s16 field_16_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_DoorFlame, 0x18);

struct Path_Greeter final : public Path_TLV
{
    Scale_short mScale;
    s16 mMotionDetectorSpeed;
    XDirection_short mFacing;
    s16 field_16_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Greeter, 0x18);

struct Path_MovingBomb final : public Path_TLV
{
    u16 mSpeed;
    s16 mStartMovingSwitchId;
    Choice_short mTriggeredByAlarm;
    Scale_short mScale;
    s16 field_18_max_rise; // seems unused
    s16 mDisabledResources;
    u16 mStartSpeed;
    Choice_short mPersistOffscreen;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MovingBomb, 0x20);

struct Path_SecurityDoor final : public Path_TLV
{
    Scale_short mScale;
    s16 mSwitchId;
    s16 mCode1;
    s16 mCode2;
    s16 mXPos;
    s16 mYPos;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SecurityDoor, 0x1C);

struct Path_MultiSwitchController final : public Path_TLV
{
    s16 mOutputSwitchId;
    SwitchOp mAction;
    u16 mOnOffDelay;
    u16 mInputSwitchId1;
    u16 mInputSwitchId2;
    u16 mInputSwitchId3;
    u16 mInputSwitchId4;
    u16 mInputSwitchId5;
    u16 mInputSwitchId6;
    s16 padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MultiSwitchController, 0x24);

struct Path_Pulley final : public Path_TLV
{
    s16 field_10_unused1;
    s16 field_12_unused2;
};
// TODO: size

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
    s16 field_14_lift_type; // TODO: Not actually used
    LiftPointStopType mLiftPointStopType;
    Scale_short mScale;
    Choice_short mIgnoreLiftMover;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_LiftPoint, 0x1C);

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
    Choice_short mListenToSligs;
    s16 mChaseDelay;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SlogSpawner, 0x20);

struct Path_CreditsController final : public Path_TLV
{
    // Empty
};

struct Path_MainMenuController final : public Path_TLV
{
    // Empty
};

struct Path_DemoSpawnPoint final : public Path_TLV
{
    // Empty
};

struct Path_TorturedMudokon final : public Path_TLV
{
    s16 mKillSwitchId;
    s16 mReleaseSwitchId;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TorturedMudokon, 0x14);

struct Path_ParamiteWebLine final : public Path_TLV
{
    Scale_short mScale;
    s16 field_12_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ParamiteWebLine, 0x14);

struct Path_Paramite final : public Path_TLV
{
    Scale_short mScale;
    enum class EntranceType : s16
    {
        ePatrol_0 = 0,
        eSurpriseWeb_1 = 1,
        eSlightlyHigherSpawnSurpriseWeb_2 = 2,
        eUnused_ScaleToLeftGridSize_3 = 3,
        eUnused_ScaleToRightGridSize_4 = 4,
    };
    EntranceType mEntranceType;
    s16 mAloneChaseDelay;
    s16 mSurpriseWebDelayTimer;
    s16 mMeatEatingTime;
    u16 mGroupChaseDelay;
    s16 field_1C_disabled_resources; // // Unused
    s16 mSurpriseWebSwitchId;
    Choice_short mHissBeforeAttack;
    Choice_short mDeleteWhenOutOfSight;
    Choice_short mAttackFleeches;
    s16 field_26_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Paramite, 0x28);

struct Path_MeatSack final : public Path_TLV
{
    XDirection_short mMeatFallDirection;
    u16 mVelX;
    u16 mVelY;
    Scale_short mScale;
    s16 mMeatAmount;
    s16 field_1A_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MeatSack, 0x1C);

struct Path_BoneBag final : public Path_TLV
{
    XDirection_short mBoneFallDirection;
    u16 mVelX;
    u16 mVelY;
    Scale_short mScale;
    s16 mBoneAmount;
    s16 padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BoneBag, 0x1C);

struct Path_ScrabBoundLeft final : public Path_TLV
{
    // Empty
};

struct Path_ScrabBoundRight final : public Path_TLV
{
    // Empty
};

struct Path_Scrab : public Path_TLV
{
    Scale_short mScale;
    s16 mAttackDelay;
    s16 mPatrolTypeRunOrWalkChance;
    s16 mPauseLeftMin;
    s16 mPauseLeftMax;
    s16 mPauseRightMin;
    s16 mPauseRightMax;
    u16 mPauseAfterChaseTime;
    s16 mDisabledResources;
    Choice_short mRoarRandomly;
    Choice_short mPersistant;
    s16 mPossessedMaxWhirlAttackDuration;
    s16 field_28_unused;
    Choice_short mKillEnemy;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Scrab, 0x2C);

struct Path_ScrabSpawner final : public Path_Scrab
{
    enum class ScrabSpawnDirection : s16
    {
        eNone_0 = 0,
        eLeft_1 = 1,
        eRight_2 = 2,
    };
    s16 mSpawnerSwitchId;
    ScrabSpawnDirection mFacing;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ScrabSpawner, 0x30);

struct Slurg_Spawner_Path_Data final
{
    s16 mSpawnInterval;
    s16 mMaxSlurgs;
    s16 mSwitchId;
    s16 padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Slurg_Spawner_Path_Data, 0x8);

struct Path_SlurgSpawner final : public Path_Slurg
{
    Slurg_Spawner_Path_Data mSpawnerData;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SlurgSpawner, 0x20);

struct Path_ZzzSpawner final : public Path_TLV
{
    Scale_short mScale;
    s16 mSwitchId;
    s16 mZzzInterval;
    s16 field_16_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ZzzSpawner, 0x18);

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
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_FootSwitch, 0x18);

struct Path_FallingItem final : public Path_TLV
{
    s16 mSwitchId;
    Scale_short mScale;
    s16 mFallInterval;
    s16 mMaxFallingItems;
    Choice_short mResetSwitchIdAfterUse;
    s16 field_20_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_FallingItem, 0x1C);

struct Path_GasCountDown final : public Path_TLV
{
    s16 mStartTimerSwitchId;
    u16 mGasCountdownTimer;
    s16 mStopTimerSwitchId;
    s16 field_16_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_GasCountDown, 0x18);

struct Path_GlukkonSwitch final : public Path_TLV
{
    enum class Scale : s16
    {
        eHalf_0 = 0,
        eFull_1 = 1,
    };
    Scale mScale;
    s16 mOkSwitchId;
    s16 mFailSwitchId;
    u16 mXPos;
    u16 mYPos;
    s16 field_1A_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_GlukkonSwitch, 0x1C);

struct Path_Glukkon final : public Path_TLV
{
    enum class GlukkonTypes : s16
    {
        eNormal_0 = 0,
        eStoryAslik_1 = 1,
        eStoryDripik_2 = 2,
        eStoryPhleg_3 = 3,
        eNormal_4 = 4,
        eNormal_5 = 5,
    };
    Scale_short mScale;
    enum class Facing : s16
    {
        eRight_0 = 0,
        eLeft_1 = 1,
    };
    Facing mFacing;
    enum class Behavior : s16
    {
        eIgnoreWalls_0 = 0,
        eCheckForWalls_1 = 1
    };
    Behavior mBehavior;
    s16 mScreamHelpDelay;
    s16 mHelpSwitchId;
    s16 mToCalmDelay;
    s16 mSpawnSwitchId;
    enum class SpawnType : s16
    {
        eRegularSpawn_0 = 0,
        eFacingLeft_1 = 1,
        eFacingRight_2 = 2,
        eFullSpawnEffects_3 = 3
    };
    SpawnType mSpawnType;
    s16 mSpawnDelay;
    GlukkonTypes mGlukkonType;
    s16 mDeathSwitchId;
    s16 mPlayMovieSwitchId;
    s16 mMovieId;
    s16 padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Glukkon, 0x2C);

struct Path_TrainDoor final : public Path_TLV
{
    XDirection_int mDirection;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TrainDoor, 0x14);

struct Path_Door final : public Path_TLV
{
    enum DoorStates : s16
    {
        eOpen_0 = 0,
        eClosed_1 = 1,
        eOpening_2 = 2,
        eClosing_3 = 3,
    };

    enum DoorTypes : s16
    {
        eBasicDoor_0 = 0,
        eDoorPadding_1 = 1,
        eTasksDoorWithSecretMusic_2 = 2,
        eTasksDoor_3 = 3,
    };
    LevelIds mNextLevel;
    s16 mNextPath;
    s16 mNextCamera;
    Scale_short mScale;
    s16 mDoorId;
    s16 mSwitchId;
    s16 mTargetDoorNumber;
    DoorTypes mDoorType;
    DoorStates mStartState;
    s16 mHub1;
    s16 mHub2;
    s16 mHub3;
    s16 mHub4;
    s16 mHub5;
    s16 mHub6;
    s16 mHub7;
    s16 mHub8;
    ScreenChangeEffects mWipeEffect;
    s16 mMovieId;
    s16 mDoorOffsetX;
    s16 mDoorOffsetY;
    s16 field_3A_wipe_x_org;
    s16 field_3C_wipe_y_org;
    XDirection_short mExitDirection;
    Choice_short mCloseOnExit;
    Choice_short mClearThrowables;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Door, 0x44);

struct Path_MovieHandstone final : public Path_TLV
{
    s16 padding;
    Scale_short scale;
    s16 movie_number;
    s16 trigger_switch_id;
};
// TODO: size

struct Path_DeathDrop final : public Path_TLV
{
    s16 field_10_animation;
    s16 field_12_sound;
    s16 field_14_id;
    s16 field_16_action;
    s16 field_18_unknown1;
    s16 field_1A_unknown2;
};
// TODO: size

struct Path_InvisibleZone final : public Path_TLV
{
    // Empty
};

struct Path_ContinuePoint final : public Path_TLV
{
    enum class Scale : s16
    {
        eNone_0 = 0,
        eHalf_1 = 1,
        eFull_2 = 2,
    };
    Scale mScale;
    s16 mSaveFileId;
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
    Choice_short mEnabled;
    s16 padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ResetPath, 0x20);

struct Path_CrawlingSlig final : public Path_TLV
{
    Scale_short mScale;
    XDirection_short mFacing; // unused

    enum class StartState : s16
    {
        eSleeping_0 = 0,
        eSleeping_1 = 1,
        eAwake_2 = 2,
    };
    StartState mStartState;

    enum class CrawlDirection : s16
    {
        eLeft_0 = 0,
        eRight_1 = 1,
        eRandom_2 = 2,
    };
    CrawlDirection mCrawlDirection;
    s16 mPanicSwitchId;
    Choice_short mRespawnOnDeath;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_CrawlingSlig, 0x1C);

struct Path_CrawlingSligButton final : public Path_TLV
{
    enum class CrawlingSligButtonSounds : s16
    {
        None_0 = 0,
        SackHit_1 = 1,
        FallingItemPresence2_2 = 2,
        SecurityOrb_3 = 3,
        SackHit_4 = 4,
        Bullet1_5 = 5,
        AbeGenericMovement_6 = 6,
    };
    Scale_short mScale;
    s16 mSwitchId;
    SwitchOp mAction;
    CrawlingSligButtonSounds mOnSound;
    CrawlingSligButtonSounds mOffSound;
    s16 mSoundDirection;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_CrawlingSligButton, 0x1C);

struct Path_FlyingSlig_Data final
{
    Scale_short mScale;
    enum class SpawnDelayStates : s16
    {
        eMoveImmediately_0 = 0,
        eUseCustomSpawnMoveDelay_1 = 1,
    };
    SpawnDelayStates mSpawnDelayState;
    s16 mSpawnMoveDelay;
    s16 mPatrolPauseMin;
    s16 mPatrolPauseMax;
    XDirection_short mFacing;
    s16 mPanicDelay;
    s16 mGiveUpChaseDelay;
    s16 mPrechaseDelay;
    s16 mSligBoundId;
    s16 mAlertedListenTime;
    s16 mSpawnerSwitchId;
    s16 mGrenadeDelay;
    s16 mMaxVelocity;
    s16 mLaunchGrenadeSwitchId;
    Choice_short mPersistant;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_FlyingSlig_Data, 0x20);

struct Path_FlyingSlig final : public Path_TLV
{
    Path_FlyingSlig_Data mFlyingSligData;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_FlyingSlig, 0x30);

struct Path_FlyingSligSpawner final : public Path_TLV
{
    Path_FlyingSlig_Data mFlyingSligSpawnerData;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_FlyingSligSpawner, 0x30);

struct Path_Mudokon final : public Path_TLV
{
    enum class MudJobs : s16
    {
        eChisle_0 = 0,
        eSitScrub_1 = 1,
        eAngryWorker_2 = 2,
        eDamageRingGiver_3 = 3,
        eHealthRingGiver_4 = 4,
    };
    enum class Mud_TLV_Emotion : s16
    {
        eNormal_0 = 0,
        eAngry_1 = 1,
        eSad_2 = 2,
        eWired_3 = 3,
        eSick_4 = 4
    };
    Scale_short mScale;
    MudJobs mJob;
    XDirection_short mFacing;
    s16 mVoicePitch;
    s16 mRescueSwitchId;
    Choice_short mDeaf;
    s16 mDisabledResources;
    Choice_short mPersistAndResetOffscreen;
    Mud_TLV_Emotion mEmotion;
    Choice_short mBlind;
    s16 mAngrySwitchId;
    Choice_short mWorkAfterTurningWheel;
    Choice_short mGetsDepressed;
    s16 mRingPulseInterval;
    Choice_short mGiveRingWithoutPassword;
    s16 field_2E_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Mudokon, 0x30);

struct Path_ZSligCover final : public Path_TLV
{
    // Empty
};

struct Path_SligGetWings final : public Path_TLV
{
    Scale_short mScale;
    Path_FlyingSlig_Data::SpawnDelayStates mSpawnDelayState;
    s16 mSpawnMoveDelay;
    s16 mPatrolPauseMin;
    s16 mPatrolPauseMax;
    XDirection_short mFacing;
    s16 mPanicDelay;
    s16 mGiveUpChaseDelay;
    s16 mPrechaseDelay;
    s16 mSligBoundId;
    s16 mAlertedListenTime;
    s16 mSpawnerSwitchId;
    s16 mGrenadeDelay;
    s16 mMaxVelocity;
    s16 mLaunchGrenadeSwitchId;
    Choice_short mPersistant;
};
// TODO: size

struct Path_Slig final : public Path_TLV
{
    Scale_short mScale;

    // TODO: Check these - GameEnder is probably only AO for instance ?
    enum class StartState : s16
    {
        Listening_0 = 0,
        Patrol_1 = 1,
        Sleeping_2 = 2,
        Chase_3 = 3,
        ChaseAndDisappear_4 = 4,
        Unused_5 = 5,
        ListeningToGlukkon_6 = 6,
    };
    StartState mStartState;

    s16 mPauseTime;
    s16 mPauseLeftMin;
    s16 mPauseLeftMax;
    s16 mPauseRightMin;
    s16 mPauseRightMax;
    enum class ShootPossessedSligs : s16
    {
        eYes_0 = 0,
        eYes_1 = 1,
        eNo_3 = 3, // used in an OG level, breaks lvl exporting if removed
    };
    ShootPossessedSligs mShootPossessedSligs;
    s16 mShootOnSightDelay;
    s16 mNumTimesToShoot;
    s16 field_24_unused; // unused
    s16 mCode1;
    s16 mCode2;
    Choice_short mChaseAbeWhenSpotted;
    XDirection_short mFacing;
    s16 mPanicTimeout;
    s16 field_30_num_panic_sounds; // unused
    s16 field_32_panic_sound_timeout; // unused
    s16 mStopChaseDelay;
    s16 mTimeToWaitBeforeChase;
    s16 mSligBoundId;
    s16 mAlertedListenTime;
    s16 mPercentSayWhat;
    s16 mPercentBeatMud;
    s16 field_40_talk_to_abe; // unused
    s16 field_42_dont_shoot; // unused
    s16 mZShootDelay;
    Choice_short mStayAwake;
    s16 mDisabledResources;
    s16 mNoiseWakeUpDistance;
    s16 mSligSpawnerSwitchId;
    Choice_short mUnlimitedSpawns;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Slig, 0x50);

struct Path_SligGetPants final : public Path_TLV
{
    Scale_short Scale;
    Path_Slig::StartState start_state;
    s16 pause_time;
    s16 pause_left_min;
    s16 pause_left_max;
    s16 pause_right_min;
    s16 pause_right_max;
    Path_Slig::ShootPossessedSligs shoot_possessed_sligs;
    s16 shoot_on_sight_delay;
    s16 num_times_to_shoot;
    s16 padding;
    s16 code1;
    s16 code2;
    Choice_short chase_abe_when_spotted;
    XDirection_short start_direction;
    s16 panic_timeout;
    s16 amount_of_panic_sounds;
    s16 panic_sounds_timeout;
    s16 stop_chase_delay;
    s16 time_to_wait_before_chase;
    s16 slig_bound_id;
    s16 alerted_listen_time;
    s16 percent_say_what;
    s16 percent_beat_mud;
    s16 talk_to_abe;
    s16 dont_shoot;
    s16 z_shoot_delay;
    Choice_short stay_awake;
    s16 disable_resources;
    s16 noise_wake_up_distance;
    s16 mSligSpawnerSwitchId;
    Choice_short unlimited_spawns;
};
// TODO: size

// This is a left bound, right bound and a persist.
struct Path_SligBound final : public Path_TLV
{
    s16 mSligBoundId;
    s16 mDisabledResources;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SligBound, 0x14);

using Path_SligBoundLeft = Path_SligBound;
using Path_SligBoundRight = Path_SligBound;
using Path_SligPersist = Path_SligBound;

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
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_EnemyStopper, 0x14);

struct Path_PathTransition final : public Path_TLV
{
    LevelIds mNextLevel;
    s16 mNextPath;
    s16 mNextCamera;
    s16 mMovieId;
    s16 mWipeEffect;
    Scale_short mNextPathScale;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_PathTransition, 0x1C);

struct Path_ShadowZone final : public Path_TLV
{
    enum class ShadowZoneScale : s16
    {
        eBoth_0 = 0,
        eHalf_1 = 1,
        eFull_2 = 2,
    };
    s16 field_10_center_w;
    s16 field_12_center_h;
    u16 mRed;
    u16 mGreen;
    u16 mBlue;
    s16 field_1A_switch_id;
    ShadowZoneScale mScale;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ShadowZone, 0x20);

struct Path_MovingBombStopper final : public Path_TLV
{
    s16 mMinStopTime;
    s16 mMaxStopTime;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MovingBombStopper, 0x14);

struct Path_Teleporter_Data
{
    s16 mTeleporterId;
    s16 mOtherTeleporterId;
    s16 mDestCamera;
    s16 mDestPath;
    LevelIds mDestLevel;
    s16 mSwitchId;
    Scale_short mScale;
    ScreenChangeEffects mWipeEffect;
    s16 mMovieId;
    s16 mElectricX;
    s16 mElectricY;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Teleporter_Data, 0x16);

struct Path_Teleporter final : public Path_TLV
{
    Path_Teleporter_Data mData;
    s16 field_26_padding; // Actually padding here as the game won't copy these 2 bytes, but its included in the TLV length
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Teleporter, 0x28); // 0x10 for base

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
    u8 field_14_switch_id; // never used
    // pad

    Scale_short field_16_scale;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Hoist, 0x18);

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

    Scale_int mScale;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Edge, 0x18);

struct Path_SoftLanding final : public Path_TLV
{
    s32 mSwitchId;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SoftLanding, 0x14);

struct Path_WellBase : public Path_TLV
{
    Scale_short mScale;
    s16 mSwitchId;
    s16 mOtherWellId;
    s16 mAnimId;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_WellBase, 0x18);

struct Path_WellLocal final : public Path_WellBase
{
    s16 field_18_off_dx;
    s16 field_1A_off_dy;
    s16 mOnDestX;
    s16 mOnDestY;
    Choice_short mEmitLeaves;
    s16 mLeafX;
    s16 mLeafY;
    s16 field_26_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_WellLocal, 0x28);

struct Path_WellExpress final : public Path_WellBase
{
    s16 field_18_exit_x;
    s16 field_1A_exit_y;
    LevelIds field_1C_disabled_well_level;
    s16 field_1E_disabled_well_path;
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
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_WellExpress, 0x34);

struct Path_Alarm final : public Path_TLV
{
    s16 mSwitchId;
    u16 mAlarmDuration;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Alarm, 0x14);

struct Path_MovieStone final : public Path_TLV
{
    s16 mMovieId;
    Scale_short mScale;
    s32 mTriggerSwitchId;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MovieStone, 0x18);

struct Path_HandStone final : public Path_TLV
{
    Scale_short mScale;
    s16 mCameraId1;
    s16 mCameraId2;
    s16 mCameraId3;
    s32 mTriggerSwitchId;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_HandStone, 0x1C);

struct Path_AbeStart final : public Path_TLV
{
    s16 field_10_xpos;
    s16 field_12_ypos;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_AbeStart, 0x14);

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
    s16 padding; // unused
    s16 mCode1;
    s16 mCode2;
    Choice_short mChaseAbeWhenSpotted;
    XDirection_short mFacing;
    s16 mPanicTimeout;
    s16 num_panic_sounds; // unused
    s16 panic_sound_timeout; // unused
    s16 mStopChaseDelay;
    s16 mTimeToWaitBeforeChase;
    s16 mSligBoundId;
    s16 mAlertedListenTime;
    s16 mPercentSayWhat;
    s16 mPercentBeatMud;
    s16 talk_to_abe; // unused
    s16 dont_shoot; // unused
    s16 mZShootDelay;
    Choice_short mStayAwake;
    s16 mDisabledResources;
    s16 mNoiseWakeUpDistance;
    s16 mSligSpawnerSwitchId;
    Choice_short mUnlimitedSpawns;
};
