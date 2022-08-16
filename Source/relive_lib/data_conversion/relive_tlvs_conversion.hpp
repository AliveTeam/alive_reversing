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

// Convert an AO or AE TLV to a relive TLV

namespace relive {
    inline void BaseConvert(relive::Path_TLV& r, const AO::Path_TLV& base)
    {
        // TODO: Actually convert
        r.mWidth = base.mTopLeft.x;
    }

    inline void BaseConvert(relive::Path_TLV& r, const ::Path_TLV& base)
    {
        // TODO: Actually convert
        r.mWidth = base.mTopLeft.x;
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

class Path_ShadowZone_Converter final
{
public:
    static Path_ShadowZone From(const AO::Path_ShadowZone& tlv)
    {
        Path_ShadowZone r;
        BaseConvert(r, tlv);
        r.mRGB.SetRGB(tlv.field_1C_r, tlv.field_1E_g, tlv.field_20_b);
        r.mScale = From(tlv.field_24_scale);
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
        r.field_18_zone_number = tlv.field_18_zone_number;
        r.field_1A_clear_from_id = tlv.field_1A_clear_from_id;
        r.field_1C_clear_to_id = tlv.field_1C_clear_to_id;
        r.field_1E_elum_restarts = relive::From(tlv.field_1E_elum_restarts);
        r.field_20_abe_direction = From(tlv.field_20_abe_direction);
        return r;
    }

    static Path_ContinuePoint From(const ::Path_ContinuePoint& tlv)
    {
        Path_ContinuePoint r;
        BaseConvert(r, tlv);
        r.field_10_scale = From(tlv.field_10_scale);
        r.field_12_save_file_id = tlv.field_12_save_file_id;
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
        r.field_10_lift_point_id = tlv.field_18_lift_point_id;
        r.field_12_bStart_point = relive::From(tlv.field_1A_bstart_point);
        r.field_16_lift_point_stop_type = From(tlv.field_1E_lift_point_stop_type);
        r.field_18_scale = relive::From(tlv.field_20_scale);
        r.field_1A_bIgnore_lift_mover = relive::From(tlv.field_22_bIgnore_lift_mover);
        return r;
    }

    static Path_LiftPoint From(const ::Path_LiftPoint& tlv)
    {
        Path_LiftPoint r;
        BaseConvert(r, tlv);
        r.field_10_lift_point_id = tlv.field_10_lift_point_id;
        r.field_12_bStart_point = relive::From(tlv.field_12_bStart_point);
        r.field_16_lift_point_stop_type = From(tlv.field_16_lift_point_stop_type);
        r.field_18_scale = relive::From(tlv.field_18_scale);
        r.field_1A_bIgnore_lift_mover = relive::From(tlv.field_1A_bIgnore_lift_mover);
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
        r.field_10_fall_direction = relive::From(tlv.field_18_fall_direction);
        r.field_12_x_vel = tlv.field_1A_x_vel;
        r.field_14_y_vel = tlv.field_1C_y_vel;
        r.field_16_scale = relive::From(tlv.field_1E_scale);
        r.field_18_rock_amount = tlv.field_20_rock_amount;
        return r;
    }

    static Path_RockSack From(const ::Path_RockSack& tlv)
    {
        Path_RockSack r;
        BaseConvert(r, tlv);
        r.field_10_fall_direction = relive::From(tlv.field_10_fall_direction);
        r.field_12_x_vel = tlv.field_12_x_vel;
        r.field_14_y_vel = tlv.field_14_y_vel;
        r.field_16_scale = relive::From(tlv.field_16_scale);
        r.field_18_rock_amount = tlv.field_18_rock_amount;
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
        r.mSwitchId = tlv.field_18_switch_id;
        r.mScale = relive::From(tlv.field_1A_scale);
        r.mFallInterval = tlv.field_1C_fall_interval;
        r.mMaxFallingItems = tlv.field_1E_max_falling_items;
        r.mResetSwitchIdAfterUse = relive::From(tlv.field_20_reset_switch_id_after_use);
        return r;
    }

    static Path_FallingItem From(const ::Path_FallingItem& tlv)
    {
        Path_FallingItem r;
        BaseConvert(r, tlv);
        r.mSwitchId = tlv.field_10_switch_id;
        r.mScale = relive::From(tlv.field_12_scale);
        r.mFallInterval = tlv.field_14_fall_interval;
        r.mMaxFallingItems = tlv.field_16_max_falling_items;
        r.mResetSwitchIdAfterUse = relive::From(tlv.field_18_reset_switch_id_after_use);
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
        r.mSwitchId = tlv.field_18_switch_id;
        r.mAction = relive::From(tlv.field_1A_action);
        r.mRopeLength = tlv.field_1C_rope_length;
        r.mScale = relive::From(tlv.field_1E_scale);
        r.mOnSound = From(tlv.field_20_on_sound);
        r.mOffSound = From(tlv.field_22_off_sound);
        r.mSoundDirection = From(tlv.field_24_sound_direction);
        return r;
    }

    static Path_PullRingRope From(const ::Path_PullRingRope& tlv)
    {
        Path_PullRingRope r;
        BaseConvert(r, tlv);
        r.mSwitchId = tlv.field_10_switch_id;
        r.mAction = relive::From(tlv.field_12_action);
        r.mRopeLength = tlv.field_14_rope_length;
        r.mScale = relive::From(tlv.field_16_scale);
        r.mOnSound = From(tlv.field_18_on_sound);
        r.mOffSound = From(tlv.field_1A_off_sound);
        r.mSoundDirection = From(tlv.field_1C_sound_direction);
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
        r.mSwitchId = tlv.field_18_switch_id;
        r.mState = tlv.field_1A_state;
        r.mScale = relive::From(tlv.field_1C_scale);
        r.mTicksUntilExplosion = tlv.mTicksUntilExplosion;
        r.mDisabledResources = tlv.mDisabledResources;
        return r;
    }

    static Path_TimedMine From(const ::Path_TimedMine& tlv)
    {
        Path_TimedMine r;
        BaseConvert(r, tlv);
        r.mSwitchId = tlv.field_10_switch_id;
        r.mState = tlv.field_12_state;
        r.mScale = relive::From(tlv.field_14_scale);
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
        r.mHoistType = From(tlv.field_18_hoist_type);
        r.mGrabDirection= From(tlv.field_1A_grab_direction);
        return r;
    }

    static Path_Hoist From(const ::Path_Hoist& tlv)
    {
        Path_Hoist r;
        BaseConvert(r, tlv);
        r.mHoistType = From(tlv.field_10_type);
        r.mGrabDirection = From(tlv.field_12_grab_direction);
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
        r.mSelfClosing = relive::From(tlv.field_1C_self_closing);
        r.mScale = relive::From(tlv.field_1E_scale);
        r.mDirection = relive::From(tlv.field_22_direction);
        r.field_1C_xOff = tlv.field_24_xOff;
        return r;
    }

    static Path_TrapDoor From(const ::Path_TrapDoor& tlv)
    {
        Path_TrapDoor r;
        BaseConvert(r, tlv);
        r.mSwitchId = tlv.mSwitchId;
        r.mStartState = From(tlv.mStartState);
        r.mSelfClosing = relive::From(tlv.mSelfClosing);
        r.mScale = relive::From(tlv.field_16_scale);
        r.mDirection = relive::From(tlv.mDirection);
        r.field_1C_xOff = tlv.field_1C_xOff;
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
        r.field_10_message_1_id = tlv.field_18_message_1_id;
        r.field_12_message_rand_min_id = tlv.field_1A_message_rand_min;
        r.field_14_message_rand_max_id = tlv.field_1C_message_rand_max;
        return r;
    }

    static Path_LCDScreen From(const ::Path_LCDScreen& tlv)
    {
        Path_LCDScreen r;
        BaseConvert(r, tlv);
        r.field_10_message_1_id = tlv.field_10_message_1_id;
        r.field_12_message_rand_min_id = tlv.field_12_message_rand_min_id;
        r.field_14_message_rand_max_id = tlv.field_14_message_rand_max_id;
        r.field_16_message_2_id = tlv.field_16_message_2_id;
        r.field_18_toggle_message_switch_id = tlv.field_18_toggle_message_switch_id;
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
        r.field_10_num_patterns = tlv.field_18_num_patterns;
        r.field_12_pattern= tlv.field_1A_pattern;
        r.field_14_scale = relive::From(tlv.field_1C_scale);
        r.field_16_disabled_resources = tlv.field_1E_disabled_resources;
        r.field_18_persist_offscreen = relive::From(tlv.field_20_persists_offscreen);
        return r;
    }

    static Path_Mine From(const ::Path_Mine& tlv)
    {
        Path_Mine r;
        BaseConvert(r, tlv);
        r.field_10_num_patterns = tlv.field_10_num_patterns;
        r.field_12_pattern = tlv.field_12_pattern;
        r.field_14_scale = relive::From(tlv.field_14_scale);
        r.field_16_disabled_resources = tlv.field_16_disabled_resources;
        r.field_18_persist_offscreen = relive::From(tlv.field_18_persist_offscreen);
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
        r.field_10_switch_id = tlv.field_18_switch_id;
        r.field_12_action = relive::From(tlv.field_1A_action);
        r.field_14_delay = tlv.field_1C_delay;
        r.field_16_set_off_alarm = relive::From(tlv.field_1E_set_off_alarm);
        r.field_18_scale = From(tlv.field_20_scale);
        return r;
    }

    static Path_InvisibleSwitch From(const ::Path_InvisibleSwitch& tlv)
    {
        Path_InvisibleSwitch r;
        BaseConvert(r, tlv);
        r.field_10_switch_id = tlv.field_10_switch_id;
        r.field_12_action = relive::From(tlv.field_12_action);
        r.field_14_delay = tlv.field_14_delay;
        r.field_16_set_off_alarm = relive::From(tlv.field_16_set_off_alarm);
        r.field_18_scale = From(tlv.field_18_scale);
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
        r.field_10_scale = relive::From(tlv.field_18_scale);
        r.field_12_switch_id = tlv.field_1A_switch_id;
        r.field_14_start_state = From(tlv.field_1C_start_state);
        return r;
    }

    static Path_ElectricWall From(const ::Path_ElectricWall& tlv)
    {
        Path_ElectricWall r;
        BaseConvert(r, tlv);
        r.field_10_scale = relive::From(tlv.field_10_scale);
        r.field_12_switch_id = tlv.field_12_switch_id;
        r.field_14_start_state = From(tlv.field_14_start_state);
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
        r.field_10_scale = relive::From(tlv.field_18_scale);
        r.field_12_nozzle_side = From(tlv.field_1A_nozzle_side);
        r.field_14_disabled_resources = tlv.field_1C_disabled_resources;
        r.field_16_number_of_grenades = tlv.field_1E_number_of_grenades;
        return r;
    }

    static Path_BoomMachine From(const ::Path_BoomMachine& tlv)
    {
        Path_BoomMachine r;
        BaseConvert(r, tlv);
        r.field_10_scale = relive::From(tlv.field_10_scale);
        r.field_12_nozzle_side = From(tlv.field_12_nozzle_side);
        r.field_14_disabled_resources = tlv.field_14_disabled_resources;
        r.field_16_number_of_grenades = tlv.field_16_number_of_grenades;
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
        r.mPatternLength = tlv.field_18_pattern_length;
        r.mPattern = tlv.field_1A_pattern;
        r.mScale = relive::From(tlv.field_1C_scale);
        r.mStartState = From(tlv.field_1E_state);
        r.mDisabledResources = tlv.field_20_disabled_resources;
        return r;
    }

    static Path_UXB From(const ::Path_UXB& tlv)
    {
        Path_UXB r;
        BaseConvert(r, tlv);
        r.mPatternLength = tlv.field_10_pattern_length;
        r.mPattern = tlv.field_12_pattern;
        r.mScale = relive::From(tlv.field_14_scale);
        r.mStartState = From(tlv.field_16_start_state);
        r.mDisabledResources = tlv.field_18_disabled_resources;
        return r;
    }

private:
    static relive::Path_UXB::StartState From(AO::UXBStartState startState)
    {
        switch (startState)
        {
        case AO::UXBStartState::eOn_0:
            return relive::Path_UXB::StartState::eOn;
        case AO::UXBStartState::eOff_1:
            return relive::Path_UXB::StartState::eOff;
        }
        ALIVE_FATAL("Bad uxb start state");
    }

    static relive::Path_UXB::StartState From(::Path_UXB::StartState startState)
    {
        switch (startState)
        {
            case ::Path_UXB::StartState::eOn_0:
                return relive::Path_UXB::StartState::eOn;
            case ::Path_UXB::StartState::eOff_1:
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
        r.field_18_scale = relive::From(tlv.field_18_scale);
        r.field_1A_switch_min_time_off = tlv.field_1A_switch_min_time_off;
        r.field_1C_switch_max_time_off = tlv.field_1C_switch_max_time_off;
        r.field_1E_max_rise_time = tlv.field_1E_max_rise_time;
        r.field_20_switch_id = tlv.field_20_switch_id;
        r.field_22_type = From(tlv.field_22_type);
        r.field_24_speed = tlv.field_24_speed;
        r.field_26_start_state = From(tlv.field_26_start_state);
        r.field_28_off_speed = tlv.field_28_off_speed;
        r.field_2A_automatic_min_time_off = tlv.field_2A_automatic_min_time_off;
        r.field_2C_automatic_max_time_off = tlv.field_2C_automatic_max_time_off;
        r.field_2E_inital_position = tlv.field_2E_inital_position;
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
        r.field_10_action = relive::From(tlv.field_1A_action);
        r.field_12_scale = relive::From(tlv.field_1C_scale);
        r.field_14_on_sound = From(tlv.field_1E_on_sound);
        r.field_16_off_sound = From(tlv.field_20_off_sound);
        r.field_18_sound_direction = From(tlv.field_22_sound_direction);
        r.field_1A_switch_id = tlv.field_18_switch_id;
        return r;
    }

    static Path_Lever From(const ::Path_Lever& tlv)
    {
        Path_Lever r;
        BaseConvert(r, tlv);
        r.field_10_action = relive::From(tlv.field_10_action);
        r.field_12_scale = relive::From(tlv.field_12_scale);
        r.field_14_on_sound = From(tlv.field_14_on_sound);
        r.field_16_off_sound = From(tlv.field_16_off_sound);
        r.field_18_sound_direction = From(tlv.field_18_sound_direction);
        r.field_1A_switch_id = tlv.field_1A_switch_id;
        r.field_1C_persist_offscreen = relive::From(tlv.field_1C_persist_offscreen);
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
        r.mGrabDirection = From(tlv.field_18_grab_direction);
        r.mCanGrab = relive::From(tlv.field_1A_can_grab);
        return r;
    }

    static Path_Edge From(const ::Path_Edge& tlv)
    {
        Path_Edge r;
        BaseConvert(r, tlv);
        r.mGrabDirection = From(tlv.field_10_grab_direction);
        r.mCanGrab = relive::From(tlv.field_12_bCan_grab);
        r.mScale = relive::From(tlv.field_14_scale);
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
        r.field_18_type = From(tlv.field_18_type);
        r.field_1A_size = tlv.field_1A_size;
        r.field_1C_switch_id = tlv.field_1C_switch_id;
        r.field_1E_direction = relive::From(tlv.field_1E_direction);
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
        r.field_10_music_type = From(tlv.field_18_music_type);
        r.field_12_triggered_by = From(tlv.field_1A_triggered_by);
        r.mSwitchId = tlv.field_1C_switch_id;
        r.field_14_music_delay = tlv.field_1E_music_delay;
        return r;
    }

    static Path_MusicTrigger From(const ::Path_MusicTrigger& tlv)
    {
        Path_MusicTrigger r;
        BaseConvert(r, tlv);
        r.field_10_music_type = From(tlv.field_10_music_type);
        r.field_12_triggered_by = From(tlv.field_12_triggered_by);
        r.field_14_music_delay = tlv.field_14_music_delay;
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
        r.mSwitchId = tlv.field_10_switch_id;
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
        r.mLiftMoverSwitchId = tlv.field_18_lift_mover_switch_id;
        r.mTargetLiftPointId = tlv.field_1A_target_lift_point_id;
        r.mMoveDirection = From(tlv.field_1C_direction);
        return r;
    }

    static Path_LiftMover From(const ::Path_LiftMover& tlv)
    {
        Path_LiftMover r;
        BaseConvert(r, tlv);
        r.mLiftMoverSwitchId = tlv.field_10_lift_mover_switch_id;
        r.mTargetLiftPointId = tlv.field_12_target_lift_point_id;
        r.mMoveDirection = From(tlv.field_14_move_direction);
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
        r.mMessageId = tlv.field_18_message_id;
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
        r.field_10_input_switch_id = tlv.field_18_input_switch_id;
        r.field_12_trigger_interval = tlv.field_1A_trigger_interval;
        r.field_14_output_switch_id1 = tlv.field_1C_output_switch_id1;
        r.field_16_output_switch_id2 = tlv.field_1C_output_switch_id2;
        r.field_18_output_switch_id3 = tlv.field_1C_output_switch_id3;
        r.field_1A_output_switch_id4 = tlv.field_1C_output_switch_id4;
        return r;
    }

    static Path_TimerTrigger From(const ::Path_TimerTrigger& tlv)
    {
        Path_TimerTrigger r;
        BaseConvert(r, tlv);
        r.field_10_input_switch_id = tlv.field_10_input_switch_id;
        r.field_12_trigger_interval = tlv.field_12_trigger_interval;
        r.field_14_output_switch_id1 = tlv.field_14_output_switch_id1;
        r.field_16_output_switch_id2 = tlv.field_16_output_switch_id2;
        r.field_18_output_switch_id3 = tlv.field_18_output_switch_id3;
        r.field_1A_output_switch_id4 = tlv.field_1A_output_switch_id4;
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
        r.mScale = relive::From(tlv.field_18_scale);
        r.mSwitchId = tlv.field_1A_switch_id;
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
        r.mChaseTicks = tlv.field_18_chase_ticks;
        r.mScale = relive::From(tlv.field_1A_scale);
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
        r.mTicksBeforeMoving = tlv.mTicksBeforeMoving;
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
        r.field_18_stopper_switch_id = tlv.field_18_stopper_switch_id;
        r.field_1A_scale = relive::From(tlv.field_1A_scale);
        r.field_1C_ball_switch_id = tlv.field_1C_ball_switch_id;
        r.field_1E_direction = relive::From(tlv.field_1E_direction);
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
        r.field_18_scale = relive::From(tlv.field_18_scale);
        r.field_1A_roll_direction = relive::From(tlv.field_1A_roll_direction);
        r.field_1C_release_switch_id = tlv.field_1C_release_switch_id;
        r.field_1E_speed = tlv.field_1E_speed;
        r.field_20_acceleration = tlv.field_20_acceleration;
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
        r.mScale = relive::From(tlv.field_18_scale);
        r.mDeviceX = tlv.field_1A_device_x;
        r.mDeviceY = tlv.field_1C_device_y;
        r.mSpeedx256 = tlv.field_1E_speed_x256;
        r.mInitialMoveDirection = From(tlv.field_20_initial_move_direction);
        r.mDrawFlare = relive::From(tlv.field_22_draw_flare);
        r.mDisableSwitchId = tlv.field_24_disable_switch_id;
        r.mAlarmSwitchId = tlv.field_26_alarm_switch_id;
        r.mAlarmDuration = tlv.field_28_alarm_duration;
        return r;
    }

    static Path_MotionDetector From(const ::Path_MotionDetector& tlv)
    {
        Path_MotionDetector r;
        BaseConvert(r, tlv);
        r.mScale = relive::From(tlv.field_10_scale);
        r.mDeviceX = tlv.field_12_device_x;
        r.mDeviceY = tlv.field_14_device_y;
        r.mSpeedx256 = tlv.field_16_speed_x256;
        r.mInitialMoveDirection = From(tlv.field_18_initial_move_direction);
        r.mDrawFlare = relive::From(tlv.field_1A_draw_flare);
        r.mDisableSwitchId = tlv.field_1C_disable_switch_id;
        r.mAlarmSwitchId = tlv.field_1E_alarm_switch_id;
        r.mAlarmDuration = tlv.field_20_alarm_duration;
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



// TODO: Need to be able to actually get to Path_MenuController and AO::Path_MenuController
// and then call From in the TLV conversion switch case
/*
class Path_MenuController_Converter final
{
public:
    static Path_MenuController From(const AO::Path_MenuController& tlv)
    {
        Path_BirdPortal r;
        BaseConvert(r, tlv); // TODO: actually need to call these in every From :annoyingSlug:
        return r;
    }

    static Path_MenuController From(const ::Path_MenuController& tlv)
    {
        Path_BirdPortal r;
        BaseConvert(r, tlv);
        return r;
    }
*/

} // namespace relive
