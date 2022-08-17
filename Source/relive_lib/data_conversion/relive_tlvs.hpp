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
    s16 field_18_zone_number = 0;
    s16 field_1A_clear_from_id = 0;
    s16 field_1C_clear_to_id = 0;
    reliveChoice field_1E_elum_restarts = reliveChoice::eYes;
    enum class spawnDirection : s16
    {
        eRight,
        eLeft,
    };
    spawnDirection field_20_abe_direction = spawnDirection::eRight;

    // AE fields
    enum class Scale : s16
    {
        eNone,
        eHalf,
        eFull,
    };
    Scale field_10_scale = Scale::eFull;
    s16 field_12_save_file_id = 0;
};

struct Path_LiftPoint final : public Path_TLV
{
    s16 field_10_lift_point_id = 0;
    reliveChoice field_12_bStart_point = reliveChoice::eNo;
    enum class LiftPointStopType : s16
    {
        eTopFloor,
        eBottomFloor,
        eMiddleFloor,
        eMiddleLockFloor,
        eStartPointOnly,
    };
    LiftPointStopType field_16_lift_point_stop_type = LiftPointStopType::eTopFloor;
    reliveScale field_18_scale = reliveScale::eFull;
    reliveChoice field_1A_bIgnore_lift_mover = reliveChoice::eNo;
};

struct Path_Dove final : public Path_TLV
{
    s16 mDoveCount = 0;
    reliveChoice mPixelPerfect = reliveChoice::eNo;
    reliveScale mScale = reliveScale::eFull;
};

struct Path_RockSack final : public Path_TLV
{
    reliveXDirection field_10_fall_direction = reliveXDirection::eLeft;
    u16 field_12_x_vel = 0;
    u16 field_14_y_vel = 0;
    reliveScale field_16_scale = reliveScale::eFull;
    s16 field_18_rock_amount = 0;
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
    s16 field_1C_xOff = 0;

    // AE only
    s16 mStayOpenTime = 0;
};

struct Path_LCDScreen final : public Path_TLV
{
    s16 field_10_message_1_id = 0;
    s16 field_12_message_rand_min_id = 0;
    s16 field_14_message_rand_max_id = 0;

    // AE only
    s16 field_16_message_2_id = 0;
    s32 field_18_toggle_message_switch_id = 0;
};

struct Path_Mine final : public Path_TLV
{
    s16 field_10_num_patterns = 0;
    s16 field_12_pattern = 0;
    reliveScale field_14_scale = reliveScale::eFull;
    s16 field_16_disabled_resources = 0;
    reliveChoice field_18_persist_offscreen = reliveChoice::eYes;
};

struct Path_InvisibleSwitch final : public Path_TLV
{
    s16 field_10_switch_id = 0;
    reliveSwitchOp field_12_action = reliveSwitchOp::eSetTrue;
    s16 field_14_delay = 0;
    reliveChoice field_16_set_off_alarm = reliveChoice::eNo;
    enum class InvisibleSwitchScale : s16
    {
        eHalf,
        eFull,
        eAny,
    };
    InvisibleSwitchScale field_18_scale = InvisibleSwitchScale::eFull;
};

struct Path_ElectricWall final : public Path_TLV
{
    reliveScale field_10_scale = reliveScale::eFull;
    s16 field_12_switch_id = 0;
    enum ElectricWallStartState : s16
    {
        eOff,
        eOn,
    };
    ElectricWallStartState field_14_start_state = ElectricWallStartState::eOn;
};

struct Path_BoomMachine final : public Path_TLV
{
    reliveScale field_10_scale = reliveScale::eFull;
    enum class NozzleSide : s16
    {
        eRight,
        eLeft,
    };
    NozzleSide field_12_nozzle_side = NozzleSide::eRight;
    s16 field_14_disabled_resources = 0;
    s16 field_16_number_of_grenades = 0;
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
    reliveScale field_18_scale = reliveScale::eFull;
    s16 field_1A_switch_min_time_off = 0;
    s16 field_1C_switch_max_time_off = 0;
    s16 field_1E_max_rise_time = 0;
    s16 field_20_switch_id = 0;
    enum class Type : s16
    {
        eAutomaticPersistOffscreen,
        eAutomatic,
        eSwitchId,
    };
    Type field_22_type = Type::eAutomatic;
    s16 field_24_speed = 0;
    enum class StartState : s16
    {
        eOff,
        eOn,
    };
    StartState field_26_start_state = StartState::eOn;
    s16 field_28_off_speed = 0;
    s16 field_2A_automatic_min_time_off = 0;
    s16 field_2C_automatic_max_time_off = 0;
    s16 field_2E_inital_position = 0;
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

    reliveSwitchOp field_10_action = reliveSwitchOp::eSetTrue;
    reliveScale field_12_scale = reliveScale::eFull;
    LeverSoundType field_14_on_sound = LeverSoundType::eNone;
    LeverSoundType field_16_off_sound = LeverSoundType::eNone;
    LeverSoundDirection field_18_sound_direction = LeverSoundDirection::eLeftAndRight;
    s16 field_1A_switch_id = 0;

    // AE only
    reliveChoice field_1C_persist_offscreen = reliveChoice::eYes;
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
    Type field_18_type = Type::GreenGlow;
    s16 field_1A_size = 0;
    s16 field_1C_switch_id = 0;
    reliveXDirection field_1E_direction = reliveXDirection::eRight;
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
    MusicTriggerMusicType field_10_music_type = MusicTriggerMusicType::eSecretAreaLong;
    TriggeredBy field_12_triggered_by = TriggeredBy::eTouching;
    s16 mSwitchId = 0; // AO only
    s16 field_14_music_delay = 0;
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
    s16 field_10_input_switch_id = 0;
    u16 field_12_trigger_interval = 0;
    s16 field_14_output_switch_id1 = 0;
    s16 field_16_output_switch_id2 = 0;
    s16 field_18_output_switch_id3 = 0;
    s16 field_1A_output_switch_id4 = 0;
};

struct Path_FlintLockFire final : public Path_TLV
{
    reliveScale mScale = reliveScale::eFull;
    s16 mSwitchId = 0;
};

struct Path_HoneySack final : public Path_TLV
{
    s16 mChaseTicks = 0;
    reliveScale mScale = reliveScale::eFull;
};

struct Path_Bat final : public Path_TLV
{
    s16 mTicksBeforeMoving = 0;
    s16 mSpeed = 0;
    reliveScale mScale = reliveScale::eFull;
    s16 mAttackDuration = 0;
};

struct Path_RollingBallStopper final : public Path_TLV
{
    s16 field_18_stopper_switch_id = 0;
    reliveScale field_1A_scale = reliveScale::eFull;
    s16 field_1C_ball_switch_id = 0;
    reliveXDirection field_1E_direction = reliveXDirection::eRight;
};

struct Path_RollingBall final : public Path_TLV
{
    reliveScale field_18_scale = reliveScale::eFull;
    reliveXDirection field_1A_roll_direction = reliveXDirection::eRight;
    s16 field_1C_release_switch_id = 0;
    u16 field_1E_speed = 0;
    u16 field_20_acceleration = 0;
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
    s16 field_10_slig_bound_id = 0;
    s16 field_12_disabled_resources = 0;
};

struct Path_BackgroundAnimation final : public Path_TLV
{
    u16 field_10_anim_id = 0;
    reliveChoice field_12_is_semi_trans = reliveChoice::eNo;
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

    TPageAbr field_14_semi_trans_mode = TPageAbr::eBlend_0;

    // AO only
    BgAnimSounds field_16_sound_effect = BgAnimSounds::eNone_0;

    // AE only
    enum class Layer : s16
    {
        eLayer0,
    };
    Layer field_1A_layer = Layer::eLayer0;
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
    reliveScale field_18_scale = reliveScale::eFull;
    s16 field_1A_solve_switch_id = 0;
    s16 field_1C_code1 = 0;
    s16 field_1E_code2 = 0;
    s16 field_20_password_switch_id = 0;
};

struct Path_LCDStatusBoard final : public Path_TLV
{
    // AO has no fields
    s16 field_10_number_of_muds = 0;
    s16 field_12_zulag_number = 0;
    reliveChoice field_14_hidden = reliveChoice::eNo;
};

struct Path_CreditsController final : public Path_TLV
{
    // Empty
};

struct Path_ResetPath final : public Path_TLV
{
    s16 field_18_clearIds = 0;
    s16 field_1A_from = 0;
    s16 field_1C_to = 0;
    s16 field_1E_exclude = 0;
    s16 field_20_clearObjects = 0;
    s16 field_22_path = 0;
};

struct Path_MeatSack final : public Path_TLV
{
    reliveXDirection field_10_meat_fall_direction = reliveXDirection::eRight;
    u16 field_12_xVel = 0;
    u16 field_14_yVel = 0;
    reliveScale field_16_scale = reliveScale::eFull;
    s16 field_18_amount_of_meat = 0;
};

struct Path_Bees final : public Path_TLV
{
    s16 switch_id = 0;
    s16 swarm_size = 0;
    s16 chase_time = 0;
    s16 speed = 0;
    s16 disable_resources = 0;
    s16 num_bees = 0;
};


struct Path_BellsongStone final : public Path_TLV
{
    enum class BellsongTypes : s16
    {
        eWhistle,
        eChimes
    };
    reliveScale scale = reliveScale::eFull;
    BellsongTypes type = BellsongTypes::eWhistle;
    s16 code1 = 0;
    s16 code2 = 0;
    s16 switch_id = 0;
};

struct Path_MovieStone final : public Path_TLV
{
    s16 field_10_movie_number = 0;
    reliveScale field_12_scale = reliveScale::eFull;

    // AE only
    s32 field_14_trigger_switch_id = 0;
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
    EReliveLevelIds field_10_level = EReliveLevelIds::eNone;
    s16 field_12_path = 0;
    s16 field_14_camera = 0;
    s16 field_16_movie = 0;
    s16 field_18_wipe = 0;
    reliveScale field_1A_scale = reliveScale::eFull;
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
    EReliveLevelIds field_10_level = EReliveLevelIds::eNone;
    s16 field_12_path = 0;
    s16 field_14_camera = 0;
    reliveScale field_16_scale = reliveScale::eFull;
    s16 field_18_door_number = 0;
    s16 field_1A_switch_id = 0;
    s16 field_1C_target_door_id = 0;
    DoorTypes field_1E_type = DoorTypes::eBasicDoor;

    // AE only
    DoorStates field_20_start_state = DoorStates::eOpen;

    s16 field_22_hub1 = 0;
    s16 field_22_hub2 = 0;
    s16 field_22_hub3 = 0;
    s16 field_22_hub4 = 0;
    s16 field_22_hub5 = 0;
    s16 field_22_hub6 = 0;
    s16 field_22_hub7 = 0;
    s16 field_22_hub8 = 0;
    ScreenChangeEffects field_32_wipe_effect = ScreenChangeEffects::eBoxOut;
    s16 field_34_movie_number = 0;
    s16 field_36_x_offset = 0;
    s16 field_38_y_offset = 0;
    reliveXDirection field_3E_abe_direction = reliveXDirection::eRight;

    // AE only
    reliveChoice field_40_close_on_exit = reliveChoice::eNo;
    reliveChoice field_42_clear_throwables = reliveChoice::eNo;
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
    s16 field_10_min = 0;
    s16 field_12_max = 0;
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
    reliveScale field_10_scale = reliveScale::eFull;
    MudJobs field_12_job = MudJobs::eChisle;
    reliveXDirection field_14_direction = reliveXDirection::eRight;
    s16 field_16_voice_pitch = 0;
    s16 field_18_rescue_switch_id = 0;
    reliveChoice field_1A_bDeaf = reliveChoice::eNo;
    s16 field_1C_disabled_resources = 0;
    reliveChoice field_1E_persist_and_reset_offscreen = reliveChoice::eNo; // check if this is the same in AO and AE
    Mud_TLV_Emotion field_20_emotion = Mud_TLV_Emotion::eNormal;
    reliveChoice field_22_bBlind = reliveChoice::eNo;
    s16 field_24_angry_switch_id = 0;
    reliveChoice field_26_work_after_turning_wheel = reliveChoice::eYes;
    reliveChoice field_28_bGets_depressed = reliveChoice::eNo;
    s16 field_2A_ring_pulse_interval = 0;
    reliveChoice field_2C_bGive_ring_without_password = reliveChoice::eNo;
};

struct Path_MovingBomb final : public Path_TLV
{
    u16 mSpeed;
    s16 mStartMovingSwitchId;
    reliveChoice mTriggeredByAlarm;
    reliveScale mScale;
    s16 mDisabledResources;
    u16 mStartSpeed;
    reliveChoice mPersistOffscreen;
};

struct Path_ElumPathTrans final : public Path_TLV
{
    EReliveLevelIds mLevel;
    s16 mPath;
    s16 mCamera;
};

struct Path_MudokonPathTrans final : public Path_TLV
{
    EReliveLevelIds mLevel;
    s16 mPath;
    s32 mCamera;
};

struct Path_SecurityClaw final : public Path_TLV
{
    reliveScale mScale;
    s16 mAlarmSwitchId;
    s16 mAlarmDuration;
    s16 mDisabledResources;
};

} // namespace relive
