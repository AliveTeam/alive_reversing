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

} // namespace relive