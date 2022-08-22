#pragma once

#include "relive_tlvs.hpp"

// serialization support for each relive tlv type and any supporting nested types

// RGB16
void to_json(nlohmann::json& j, const RGB16& p)
{
    j = nlohmann::json{
        {"r", p.r},
        {"g", p.g},
        {"b", p.b}};
}

void from_json(const nlohmann::json& j, RGB16& p)
{
    j.at("r").get_to(p.r);
    j.at("g").get_to(p.g);
    j.at("b").get_to(p.b);
}

NLOHMANN_JSON_SERIALIZE_ENUM(EReliveLevelIds, {
    {EReliveLevelIds::eNone, "none"},
    {EReliveLevelIds::eMenu, "menu"},
    {EReliveLevelIds::eMines, "mines"},
    {EReliveLevelIds::eNecrum, "necrum"},
    {EReliveLevelIds::eMudomoVault, "mudomo_vault"},
    {EReliveLevelIds::eMudancheeVault, "mudanchee_vault"},
    {EReliveLevelIds::eFeeCoDepot, "feeco_depot"},
    {EReliveLevelIds::eBarracks, "barracks"},
    {EReliveLevelIds::eMudancheeVault_Ender, "mudanchee_vault_ender"},
    {EReliveLevelIds::eBonewerkz, "bonewerkz"},
    {EReliveLevelIds::eBrewery, "brewery"},
    {EReliveLevelIds::eBrewery_Ender, "brewery_ender"},
    {EReliveLevelIds::eMudomoVault_Ender, "mudomo_vault_ender"},
    {EReliveLevelIds::eFeeCoDepot_Ender, "feeco_depot_ender"},
    {EReliveLevelIds::eBarracks_Ender, "barracks_ender"},
    {EReliveLevelIds::eBonewerkz_Ender, "bonewerkz_ender"},
    {EReliveLevelIds::eCredits, "credits"},
    {EReliveLevelIds::eRuptureFarms, "rupture_farms"},
    {EReliveLevelIds::eLines, "lines"},
    {EReliveLevelIds::eForest, "forest"},
    {EReliveLevelIds::eForestTemple, "forest_temple"},
    {EReliveLevelIds::eStockYards, "stock_yards"},
    {EReliveLevelIds::eStockYardsReturn, "stock_yards_return"},
    {EReliveLevelIds::eDesert, "desert"},
    {EReliveLevelIds::eDesertTemple, "desert_temple"},
    {EReliveLevelIds::eBoardRoom, "board_room"},
    {EReliveLevelIds::eRuptureFarmsReturn, "rupture_farms_return"},
    {EReliveLevelIds::eForestChase, "forest_chase"},
    {EReliveLevelIds::eDesertEscape, "desert_escape"},
})

namespace relive {
template<class T>
static const relive::Path_TLV& ToBase(const T& derivedType)
{
    return derivedType;
}

template <class T>
static relive::Path_TLV& ToBase(T& derivedType)
{
    return derivedType;
}

// Path_TLV
void to_json(nlohmann::json& j, const Path_TLV& p)
{
    j = nlohmann::json{
        {"x", p.mX},
        {"y", p.mY},
        {"width", p.mWidth},
        {"height", p.mHeight},
    };
}

void from_json(const nlohmann::json& j, Path_TLV& p)
{
    j.at("x").get_to(p.mX);
    j.at("y").get_to(p.mY);
    j.at("width").get_to(p.mWidth);
    j.at("height").get_to(p.mHeight);
}

// Common TLV enums

NLOHMANN_JSON_SERIALIZE_ENUM(reliveScale, {
    {reliveScale::eFull, "full"},
    {reliveScale::eHalf, "half"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(reliveChoice, {
    {reliveChoice::eNo, "no"},
    {reliveChoice::eYes, "yes"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(reliveXDirection, {
    {reliveXDirection::eLeft, "left"},
    {reliveXDirection::eRight, "right"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(reliveSwitchOp, {
    {reliveSwitchOp::eSetTrue, "set_true"},
    {reliveSwitchOp::eSetFalse, "set_false"},
    {reliveSwitchOp::eToggle, "toggle"},
    {reliveSwitchOp::eIncrement, "increment"},
    {reliveSwitchOp::eDecrement, "decrement"},
})

// Path_ShadowZone
NLOHMANN_JSON_SERIALIZE_ENUM(Path_ShadowZone::Scale, {
    {Path_ShadowZone::Scale::eBoth, "both"},
    {Path_ShadowZone::Scale::eFull, "full"},
    {Path_ShadowZone::Scale::eHalf, "half"},
})

void to_json(nlohmann::json& j, const Path_ShadowZone& p)
{
    j = nlohmann::json{
        {"name", "shadow_zone"},
        {"base", ToBase(p)},
        {"rgb", p.mRGB},
        {"scale", p.mScale},
    };
}

void from_json(const nlohmann::json& j, Path_ShadowZone& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("rgb").get_to(p.mRGB);
    j.at("scale").get_to(p.mScale);
}

// Path_SecurityOrb
void to_json(nlohmann::json& j, const Path_SecurityOrb& p)
{
    j = nlohmann::json{
        {"name", "security_orb"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"disabled_resources", p.mDisabledResources},
    };
}

void from_json(const nlohmann::json& j, Path_SecurityOrb& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("disabled_resources").get_to(p.mDisabledResources);
}

// Path_ContinuePoint
NLOHMANN_JSON_SERIALIZE_ENUM(Path_ContinuePoint::spawnDirection, {
    {Path_ContinuePoint::spawnDirection::eRight, "right"},
    {Path_ContinuePoint::spawnDirection::eLeft, "left"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(Path_ContinuePoint::Scale, {
    {Path_ContinuePoint::Scale::eNone, "none"},
    {Path_ContinuePoint::Scale::eHalf, "half"},
    {Path_ContinuePoint::Scale::eFull, "full"},
})

void to_json(nlohmann::json& j, const Path_ContinuePoint& p)
{
    j = nlohmann::json{
        {"name", "continue_point"},
        {"base", ToBase(p)},
        {"zone_number", p.mZoneNumber},
        {"clear_from_id", p.mClearFromId},
        {"clear_to_id", p.mClearToId},
        {"elum_restarts", p.mElumRestarts},
        {"abe_direction", p.mAbeSpawnDir},

        {"scale", p.mScale},
        {"save_file_id", p.mSaveFileId},
    };
}

void from_json(const nlohmann::json& j, Path_ContinuePoint& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("zone_number").get_to(p.mZoneNumber);
    j.at("clear_from_id").get_to(p.mClearFromId);
    j.at("clear_to_id").get_to(p.mClearToId);
    j.at("elum_restarts").get_to(p.mElumRestarts);
    j.at("abe_direction").get_to(p.mAbeSpawnDir);

    j.at("scale").get_to(p.mScale);
    j.at("save_file_id").get_to(p.mSaveFileId);
}

// Path_LiftPoint
NLOHMANN_JSON_SERIALIZE_ENUM(Path_LiftPoint::LiftPointStopType, {
    {Path_LiftPoint::LiftPointStopType::eTopFloor, "top_floor"},
    {Path_LiftPoint::LiftPointStopType::eBottomFloor, "bottom_floor"},
    {Path_LiftPoint::LiftPointStopType::eMiddleFloor, "middle_floor"},
    {Path_LiftPoint::LiftPointStopType::eMiddleLockFloor, "middle_lock_floor"},
    {Path_LiftPoint::LiftPointStopType::eStartPointOnly, "start_point_only"},
})

void to_json(nlohmann::json& j, const Path_LiftPoint& p)
{
    j = nlohmann::json{
        {"name", "lift_point"},
        {"base", ToBase(p)},
        {"lift_point_id", p.mLiftPointId},
        {"is_start_point", p.mIsStartPoint},
        {"lift_point_stop_type", p.mLiftPointStopType},
        {"scale", p.mScale},
        {"ignore_lift_mover", p.mIgnoreLiftMover},
    };
}

void from_json(const nlohmann::json& j, Path_LiftPoint& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("lift_point_id").get_to(p.mLiftPointId);
    j.at("is_start_point").get_to(p.mIsStartPoint);
    j.at("lift_point_stop_type").get_to(p.mLiftPointStopType);
    j.at("scale").get_to(p.mScale);
    j.at("ignore_lift_mover").get_to(p.mIgnoreLiftMover);
}

// Path_Dove
void to_json(nlohmann::json& j, const Path_Dove& p)
{
    j = nlohmann::json{
        {"name", "dove"},
        {"base", ToBase(p)},
        {"dove_count", p.mDoveCount},
        {"pixe_perfect", p.mPixelPerfect},
        {"scale", p.mScale},
    };
}

void from_json(const nlohmann::json& j, Path_Dove& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("dove_count").get_to(p.mDoveCount);
    j.at("pixel_perfect").get_to(p.mPixelPerfect);
    j.at("scale").get_to(p.mScale);
}

// Path_RockSack
void to_json(nlohmann::json& j, const Path_RockSack& p)
{
    j = nlohmann::json{
        {"name", "rock_sack"},
        {"base", ToBase(p)},
        {"rock_fall_direction", p.mRockFallDirection},
        {"vel_x", p.mVelX},
        {"vel_y", p.mVelY},
        {"scale", p.mScale},
        {"rock_amount", p.mRockAmount},
    };
}

void from_json(const nlohmann::json& j, Path_RockSack& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("rock_fall_direction").get_to(p.mRockFallDirection);
    j.at("vel_x").get_to(p.mVelX);
    j.at("vel_y").get_to(p.mVelY);
    j.at("scale").get_to(p.mScale);
    j.at("rock_amount").get_to(p.mRockAmount);
}

// Path_ZBall
NLOHMANN_JSON_SERIALIZE_ENUM(Path_ZBall::StartPos, {
    {Path_ZBall::StartPos::eCenter, "center"},
    {Path_ZBall::StartPos::eOut, "out"},
    {Path_ZBall::StartPos::eIn, "in"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(Path_ZBall::Speed, {
    {Path_ZBall::Speed::eNormal, "normal"},
    {Path_ZBall::Speed::eFast, "fast"},
    {Path_ZBall::Speed::eSlow, "slow"},
})

void to_json(nlohmann::json& j, const Path_ZBall& p)
{
    j = nlohmann::json{
        {"name", "z_ball"},
        {"base", ToBase(p)},
        {"start_pos", p.mStartPos},
        {"scale", p.mScale},
        {"speed", p.mSpeed},
    };
}

void from_json(const nlohmann::json& j, Path_ZBall& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("start_pos").get_to(p.mStartPos);
    j.at("scale").get_to(p.mScale);
    j.at("speed").get_to(p.mSpeed);
}

// Path_FallingItem
void to_json(nlohmann::json& j, const Path_FallingItem& p)
{
    j = nlohmann::json{
        {"name", "falling_item"},
        {"base", ToBase(p)},
        {"switch_id", p.mSwitchId},
        {"scale", p.mScale},
        {"fall_interval", p.mFallInterval},
        {"max_falling_items", p.mMaxFallingItems},
        {"reset_switch_id_after_use", p.mResetSwitchIdAfterUse},
    };
}

void from_json(const nlohmann::json& j, Path_FallingItem& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("scale").get_to(p.mScale);
    j.at("fall_interval").get_to(p.mFallInterval);
    j.at("max_falling_items").get_to(p.mMaxFallingItems);
    j.at("reset_switch_id_after_use").get_to(p.mResetSwitchIdAfterUse);
}

// Path_PullRingRope
NLOHMANN_JSON_SERIALIZE_ENUM(Path_PullRingRope::PullRingSwitchSound, {
    {Path_PullRingRope::PullRingSwitchSound::eNone, "none"},
    {Path_PullRingRope::PullRingSwitchSound::eWellExit, "well_exit"},
    {Path_PullRingRope::PullRingSwitchSound::RingBellHammer, "ring_bell_hammer"},
    {Path_PullRingRope::PullRingSwitchSound::eDoorEffect, "door_effect"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(Path_PullRingRope::PullRingSoundDirection, {
    {Path_PullRingRope::PullRingSoundDirection::eLeftAndRight, "left_and_right"},
    {Path_PullRingRope::PullRingSoundDirection::eLeft, "left"},
    {Path_PullRingRope::PullRingSoundDirection::eRight, "right"},
})

void to_json(nlohmann::json& j, const Path_PullRingRope& p)
{
    j = nlohmann::json{
        {"name", "pull_ring_rope"},
        {"base", ToBase(p)},
        {"switch_id", p.mSwitchId},
        {"action", p.mAction},
        {"rope_length", p.mRopeLength},
        {"scale", p.mScale},
        {"on_sound", p.mOnSound},
        {"off_sound", p.mOffSound},
        {"sound_direction", p.mSoundDirection},
    };
}

void from_json(const nlohmann::json& j, Path_PullRingRope& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("action").get_to(p.mAction);
    j.at("rope_length").get_to(p.mRopeLength);
    j.at("scale").get_to(p.mScale);
    j.at("on_sound").get_to(p.mOnSound);
    j.at("off_sound").get_to(p.mOffSound);
    j.at("sound_direction").get_to(p.mSoundDirection);
}

// Path_TimedMine
void to_json(nlohmann::json& j, const Path_TimedMine& p)
{
    j = nlohmann::json{
        {"name", "timed_mine"},
        {"base", ToBase(p)},
        {"switch_id", p.mSwitchId},
        {"state", p.mState},
        {"scale", p.mScale},
        {"ticks_until_explosion", p.mTicksUntilExplosion},
        {"disabled_resources", p.mDisabledResources},
    };
}

void from_json(const nlohmann::json& j, Path_TimedMine& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("state").get_to(p.mState);
    j.at("scale").get_to(p.mScale);
    j.at("ticks_until_explosion").get_to(p.mTicksUntilExplosion);
    j.at("disabled_resources").get_to(p.mDisabledResources);
}

// Path_Hoist
NLOHMANN_JSON_SERIALIZE_ENUM(Path_Hoist::Type, {
    {Path_Hoist::Type::eNextFloor, "next_floor"},
    {Path_Hoist::Type::eNextEdge, "next_edge"},
    {Path_Hoist::Type::eOffScreen, "off_screen"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(Path_Hoist::GrabDirection, {
    {Path_Hoist::GrabDirection::eFacingLeft, "facing_left"},
    {Path_Hoist::GrabDirection::eFacingRight, "facing_right"},
    {Path_Hoist::GrabDirection::eFacingAnyDirection, "facing_any_direction"},
})

void to_json(nlohmann::json& j, const Path_Hoist& p)
{
    j = nlohmann::json{
        {"name", "hoist"},
        {"base", ToBase(p)},
        {"hoist_type", p.mHoistType},
        {"grab_direction", p.mGrabDirection},
    };
}

void from_json(const nlohmann::json& j, Path_Hoist& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("hoist_type").get_to(p.mHoistType);
    j.at("grab_direction").get_to(p.mGrabDirection);
}

// Path_TrapDoor
NLOHMANN_JSON_SERIALIZE_ENUM(Path_TrapDoor::StartState, {
    {Path_TrapDoor::StartState::eOpen, "open"},
    {Path_TrapDoor::StartState::eClosed, "closed"},
})

void to_json(nlohmann::json& j, const Path_TrapDoor& p)
{
    j = nlohmann::json{
        {"name", "trap_door"},
        {"base", ToBase(p)},
        {"switch_id", p.mSwitchId},
        {"start_state", p.mStartState},
        {"self_closing", p.mSelfClosing},
        {"scale", p.mScale},
        {"direction", p.mDirection},
        {"x_off", p.mXOff},
        {"stay_open_time", p.mStayOpenTime},
    };
}

void from_json(const nlohmann::json& j, Path_TrapDoor& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("start_state").get_to(p.mStartState);
    j.at("self_closing").get_to(p.mSelfClosing);
    j.at("scale").get_to(p.mScale);
    j.at("direction").get_to(p.mDirection);
    j.at("x_off").get_to(p.mXOff);
    j.at("stay_open_time").get_to(p.mStayOpenTime);
}

// Path_LCDScreen
void to_json(nlohmann::json& j, const Path_LCDScreen& p)
{
    j = nlohmann::json{
        {"name", "lcd_screen"},
        {"base", ToBase(p)},
        {"message_id_1", p.mMessageId1},
        {"message_rand_min_id", p.mMessageRandMinId},
        {"message_rand_max_id", p.mMessageRandMaxId},
        {"message_id_2", p.mMessageId2},
        {"toggle_message_switch_id", p.mToggleMessageSwitchId},
    };
}

void from_json(const nlohmann::json& j, Path_LCDScreen& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("message_id_1").get_to(p.mMessageId1);
    j.at("message_rand_min_id").get_to(p.mMessageRandMinId);
    j.at("message_rand_max_id").get_to(p.mMessageRandMaxId);
    j.at("message_id_2").get_to(p.mMessageId2);
    j.at("toggle_message_switch_id").get_to(p.mToggleMessageSwitchId);
}

// Path_Mine
void to_json(nlohmann::json& j, const Path_Mine& p)
{
    j = nlohmann::json{
        {"name", "mine"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"disabled_resources", p.mDisabledResources},
        {"persist_offscreen", p.mPersistOffscreen},
    };
}

void from_json(const nlohmann::json& j, Path_Mine& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("disabled_resources").get_to(p.mDisabledResources);
    j.at("persist_offscreen").get_to(p.mPersistOffscreen);
}

// Path_InvisibleSwitch
NLOHMANN_JSON_SERIALIZE_ENUM(Path_InvisibleSwitch::InvisibleSwitchScale, {
    {Path_InvisibleSwitch::InvisibleSwitchScale::eHalf, "half"},
    {Path_InvisibleSwitch::InvisibleSwitchScale::eFull, "full"},
    {Path_InvisibleSwitch::InvisibleSwitchScale::eAny, "any"},
})

void to_json(nlohmann::json& j, const Path_InvisibleSwitch& p)
{
    j = nlohmann::json{
        {"name", "invisible_switch"},
        {"base", ToBase(p)},
        {"switch_id", p.mSwitchId},
        {"action", p.mAction},
        {"activation_delay", p.mActivationDelay},
        {"set_off_alarm", p.mSetOffAlarm},
        {"scale", p.mScale},
    };
}

void from_json(const nlohmann::json& j, Path_InvisibleSwitch& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("action").get_to(p.mAction);
    j.at("activation_delay").get_to(p.mActivationDelay);    
    j.at("set_off_alarm").get_to(p.mSetOffAlarm);
    j.at("scale").get_to(p.mScale);
}

// Path_ElectricWall
NLOHMANN_JSON_SERIALIZE_ENUM(Path_ElectricWall::ElectricWallStartState, {
    {Path_ElectricWall::ElectricWallStartState::eOff, "off"},
    {Path_ElectricWall::ElectricWallStartState::eOn, "on"},
})

void to_json(nlohmann::json& j, const Path_ElectricWall& p)
{
    j = nlohmann::json{
        {"name", "electric_wall"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"switch_id", p.mSwitchId},
        {"start_state", p.mStartState},
    };
}

void from_json(const nlohmann::json& j, Path_ElectricWall& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("start_state").get_to(p.mStartState);
}

// Path_BoomMachine
NLOHMANN_JSON_SERIALIZE_ENUM(Path_BoomMachine::NozzleSide, {
    {Path_BoomMachine::NozzleSide::eRight, "right"},
    {Path_BoomMachine::NozzleSide::eLeft, "left"},
})

void to_json(nlohmann::json& j, const Path_BoomMachine& p)
{
    j = nlohmann::json{
        {"name", "boom_machine"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"nozzle_side", p.mNozzleSide},
        {"disabled_resources", p.mDisabledResources},
        {"grenade_amount", p.mGrenadeAmount},
    };
}

void from_json(const nlohmann::json& j, Path_BoomMachine& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("nozzle_side").get_to(p.mNozzleSide);
    j.at("disabled_resources").get_to(p.mDisabledResources);
    j.at("grenade_amount").get_to(p.mGrenadeAmount);
}

// Path_UXB
NLOHMANN_JSON_SERIALIZE_ENUM(Path_UXB::StartState, {
    {Path_UXB::StartState::eOn, "on"},
    {Path_UXB::StartState::eOff, "off"},
})

void to_json(nlohmann::json& j, const Path_UXB& p)
{
    j = nlohmann::json{
        {"name", "uxb"},
        {"base", ToBase(p)},
        {"pattern_length", p.mPatternLength},
        {"pattern", p.mPattern},
        {"scale", p.mScale},
        {"start_state", p.mStartState},
        {"disabled_resources", p.mDisabledResources},
    };
}

void from_json(const nlohmann::json& j, Path_UXB& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("pattern_length").get_to(p.mPatternLength);
    j.at("pattern").get_to(p.mPattern);
    j.at("scale").get_to(p.mScale);
    j.at("start_state").get_to(p.mStartState);
    j.at("disabled_resources").get_to(p.mDisabledResources);
}

// Path_MeatSaw
NLOHMANN_JSON_SERIALIZE_ENUM(Path_MeatSaw::Type, {
    {Path_MeatSaw::Type::eAutomaticPersistOffscreen, "automatic_persist_offscreen"},
    {Path_MeatSaw::Type::eAutomatic, "automatic"},
    {Path_MeatSaw::Type::eSwitchId, "switch_id"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(Path_MeatSaw::StartState, {
    {Path_MeatSaw::StartState::eOff, "off"},
    {Path_MeatSaw::StartState::eOn, "on"},
})

void to_json(nlohmann::json& j, const Path_MeatSaw& p)
{
    j = nlohmann::json{
        {"name", "meat_saw"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"switch_min_time_off", p.mSwitchMinTimeOff},
        {"switch_max_time_off", p.mSwitchMaxTimeOff},
        {"max_rise_time", p.mMaxRiseTime},
        {"switch_id", p.mSwitchId},
        {"type", p.mType},
        {"speed", p.mSpeed},
        {"start_state", p.mStartState},
        {"off_speed", p.mOffSpeed},
        {"automatic_min_time_off", p.mAutomaticMinTimeOff},
        {"automatic_max_time_off", p.mAutomaticMaxTimeOff},
        {"initial_position", p.mInitialPosition},
    };
}

void from_json(const nlohmann::json& j, Path_MeatSaw& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("switch_min_time_off").get_to(p.mSwitchMinTimeOff);
    j.at("switch_max_time_off").get_to(p.mSwitchMaxTimeOff);
    j.at("max_rise_time").get_to(p.mMaxRiseTime);
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("type").get_to(p.mType);
    j.at("speed").get_to(p.mSpeed);
    j.at("start_state").get_to(p.mStartState);
    j.at("off_speed").get_to(p.mOffSpeed);
    j.at("automatic_min_time_off").get_to(p.mAutomaticMinTimeOff);
    j.at("automatic_max_time_off").get_to(p.mAutomaticMaxTimeOff);
    j.at("initial_position").get_to(p.mInitialPosition);
}

// Path_Lever
NLOHMANN_JSON_SERIALIZE_ENUM(Path_Lever::LeverSoundType, {
    {Path_Lever::LeverSoundType::eNone, "none"},
    {Path_Lever::LeverSoundType::eWell, "well"},
    {Path_Lever::LeverSoundType::eSwitchBellHammer, "switch_bell_hammer"},
    {Path_Lever::LeverSoundType::eDoor, "door"},
    {Path_Lever::LeverSoundType::eElectricWall, "electric_wall"},
    {Path_Lever::LeverSoundType::eSecurityOrb, "security_orb"},
    {Path_Lever::LeverSoundType::eLift, "lift"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(Path_Lever::LeverSoundDirection, {
    {Path_Lever::LeverSoundDirection::eLeftAndRight, "left_and_right"},
    {Path_Lever::LeverSoundDirection::eLeft, "left"},
    {Path_Lever::LeverSoundDirection::eRight, "right"},
})

void to_json(nlohmann::json& j, const Path_Lever& p)
{
    j = nlohmann::json{
        {"name", "lever"},
        {"base", ToBase(p)},
        {"action", p.mAction},
        {"scale", p.mScale},
        {"on_sound", p.mOnSound},
        {"off_sound", p.mOffSound},
        {"sound_direction", p.mSoundDirection},
        {"switch_id", p.mSwitchId},
        {"persist_offscreen", p.mPersistOffscreen},
    };
}

void from_json(const nlohmann::json& j, Path_Lever& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("action").get_to(p.mAction);
    j.at("scale").get_to(p.mScale);
    j.at("on_sound").get_to(p.mOnSound);
    j.at("off_sound").get_to(p.mOffSound);
    j.at("sound_direction").get_to(p.mSoundDirection);
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("persist_offscreen").get_to(p.mPersistOffscreen);
}

// Path_Edge
NLOHMANN_JSON_SERIALIZE_ENUM(Path_Edge::GrabDirection, {
    {Path_Edge::GrabDirection::eFacingLeft, "facing_left"},
    {Path_Edge::GrabDirection::eFacingRight, "facing_right"},
    {Path_Edge::GrabDirection::eFacingAnyDirection, "facing_any_direction"},
})

void to_json(nlohmann::json& j, const Path_Edge& p)
{
    j = nlohmann::json{
        {"name", "edge"},
        {"base", ToBase(p)},
        {"grab_direction", p.mGrabDirection},
        {"can_grab", p.mCanGrab},
        {"scale", p.mScale},
    };
}

void from_json(const nlohmann::json& j, Path_Edge& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("grab_direction").get_to(p.mGrabDirection);
    j.at("can_grab").get_to(p.mCanGrab);
    j.at("scale").get_to(p.mScale);
}

// Path_BirdPortal
NLOHMANN_JSON_SERIALIZE_ENUM(Path_BirdPortal::PortalType, {
    {Path_BirdPortal::PortalType::eAbe, "abe"},
    {Path_BirdPortal::PortalType::eWorker, "worker"},
    {Path_BirdPortal::PortalType::eShrykull, "shrykull"},
    {Path_BirdPortal::PortalType::eMudTeleport, "mud_teleport"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(Path_BirdPortal::PortalSide, {
    {Path_BirdPortal::PortalSide::eRight, "right"},
    {Path_BirdPortal::PortalSide::eLeft, "left"},
})

void to_json(nlohmann::json& j, const Path_BirdPortal& p)
{
    j = nlohmann::json{
        {"name", "bird_portal"},
        {"base", ToBase(p)},
        {"enter_side", p.mEnterSide},
        {"exit_level", p.mExitLevel},
        {"exit_path", p.mExitPath},
        {"exit_camera", p.mExitCamera},
        {"scale", p.mScale},
        {"movie_id", p.mMovieId},
        {"portal_type", p.mPortalType},
        {"mud_count_for_shrykull", p.mMudCountForShrykull},
        {"create_portal_switch_id", p.mCreatePortalSwitchId},
        {"delete_portal_switch_id", p.mDeletePortalSwitchId},
    };
}

void from_json(const nlohmann::json& j, Path_BirdPortal& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("enter_side").get_to(p.mEnterSide);
    j.at("exit_level").get_to(p.mExitLevel);
    j.at("exit_path").get_to(p.mExitPath);
    j.at("exit_camera").get_to(p.mExitCamera);
    j.at("scale").get_to(p.mScale);
    j.at("movie_id").get_to(p.mMovieId);
    j.at("portal_type").get_to(p.mPortalType);
    j.at("mud_count_for_shrykull").get_to(p.mMudCountForShrykull);
    j.at("create_portal_switch_id").get_to(p.mCreatePortalSwitchId);
    j.at("delete_portal_switch_id").get_to(p.mDeletePortalSwitchId);
}

 // Path_BirdPortalExit
void to_json(nlohmann::json& j, const Path_BirdPortalExit& p)
{
    j = nlohmann::json{
        {"name", "bird_portal_exit"},
        {"base", ToBase(p)},
        {"exit_side", p.mExitSide},
        {"scale", p.mScale},
    };
}

void from_json(const nlohmann::json& j, Path_BirdPortalExit& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("exit_side").get_to(p.mExitSide);
    j.at("scale").get_to(p.mScale);
}

// Path_LightEffect
NLOHMANN_JSON_SERIALIZE_ENUM(Path_LightEffect::Type, {
    {Path_LightEffect::Type::Star, "star"},
    {Path_LightEffect::Type::GoldGlow, "gold_glow"},
    {Path_LightEffect::Type::GreenGlow, "green_glow"},
    {Path_LightEffect::Type::FlintGlow, "flint_glow"},
    {Path_LightEffect::Type::Switchable_RedGreenDoorLights, "switchable_red_greendoor_lights"},
    {Path_LightEffect::Type::Switchable_RedGreenHubLight, "switchable_red_green_hub_light"},
})

void to_json(nlohmann::json& j, const Path_LightEffect& p)
{
    j = nlohmann::json{
        {"name", "light_effect"},
        {"base", ToBase(p)},
        {"type", p.mType},
        {"size", p.mSize},
        {"switch_id", p.mSwitchId},
        {"direction", p.mDirection},
    };
}

void from_json(const nlohmann::json& j, Path_LightEffect& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("type").get_to(p.mType);
    j.at("size").get_to(p.mSize);
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("direction").get_to(p.mDirection);
}

// Path_MusicTrigger
NLOHMANN_JSON_SERIALIZE_ENUM(Path_MusicTrigger::MusicTriggerMusicType, {
    {Path_MusicTrigger::MusicTriggerMusicType::eDrumAmbience, "drum_ambience"},
    {Path_MusicTrigger::MusicTriggerMusicType::eDeathDrumShort, "death_drum_ambience"},
    {Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaLong, "secret_area_long"},
    {Path_MusicTrigger::MusicTriggerMusicType::eSoftChase, "soft_chase"},
    {Path_MusicTrigger::MusicTriggerMusicType::eIntenseChase, "intense_chase"},
    {Path_MusicTrigger::MusicTriggerMusicType::eChime, "chime"},
    {Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaShort, "secret_area_short"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(Path_MusicTrigger::TriggeredBy, {
    {Path_MusicTrigger::TriggeredBy::eTimer, "timer"},
    {Path_MusicTrigger::TriggeredBy::eTouching, "touching"},
    {Path_MusicTrigger::TriggeredBy::eSwitchId, "switch_id"},
    {Path_MusicTrigger::TriggeredBy::eUnknown, "unknown"},
})

void to_json(nlohmann::json& j, const Path_MusicTrigger& p)
{
    j = nlohmann::json{
        {"name", "music_trigger"},
        {"base", ToBase(p)},
        {"music_type", p.mMusicType},
        {"triggered_by", p.mTriggeredBy},
        {"switch_id", p.mSwitchId},
        {"music_delay", p.mMusicDelay},
    };
}

void from_json(const nlohmann::json& j, Path_MusicTrigger& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("music_type").get_to(p.mMusicType);
    j.at("triggered_by").get_to(p.mTriggeredBy);
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("music_delay").get_to(p.mMusicDelay);
}

// Path_SoftLanding
void to_json(nlohmann::json& j, const Path_SoftLanding& p)
{
    j = nlohmann::json{
        {"name", "soft_landing"},
        {"base", ToBase(p)},
        {"switch_id", p.mSwitchId},
    };
}

void from_json(const nlohmann::json& j, Path_SoftLanding& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("switch_id").get_to(p.mSwitchId);
}

// Path_LiftMover
NLOHMANN_JSON_SERIALIZE_ENUM(Path_LiftMover::YDirection, {
    {Path_LiftMover::YDirection::eDown, "down"},
    {Path_LiftMover::YDirection::eUp, "up"},
})

void to_json(nlohmann::json& j, const Path_LiftMover& p)
{
    j = nlohmann::json{
        {"name", "lift_mover"},
        {"base", ToBase(p)},
        {"lift_mover_switch_id", p.mLiftMoverSwitchId},
        {"target_lift_point_id", p.mTargetLiftPointId},
        {"move_direction", p.mMoveDirection},
    };
}

void from_json(const nlohmann::json& j, Path_LiftMover& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("lift_mover_switch_id").get_to(p.mLiftMoverSwitchId);
    j.at("target_lift_point_id").get_to(p.mTargetLiftPointId);
    j.at("move_direction").get_to(p.mMoveDirection);
}

// Path_Hintfly
void to_json(nlohmann::json& j, const Path_HintFly& p)
{
    j = nlohmann::json{
        {"name", "hint_fly"},
        {"base", ToBase(p)},
        {"message_id", p.mMessageId},
    };
}

void from_json(const nlohmann::json& j, Path_HintFly& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("message_id").get_to(p.mMessageId);
}

// Path_TimerTrigger
void to_json(nlohmann::json& j, const Path_TimerTrigger& p)
{
    j = nlohmann::json{
        {"name", "timer_trigger"},
        {"base", ToBase(p)},
        {"input_switch_id", p.mInputSwitchId},
        {"activation_delay", p.mActivationDelay},
        {"output_switch_id1", p.mOutputSwitchId1},
        {"output_switch_id2", p.mOutputSwitchId2},
        {"output_switch_id3", p.mOutputSwitchId3},
        {"output_switch_id4", p.mOutputSwitchId4},
    };
}

void from_json(const nlohmann::json& j, Path_TimerTrigger& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("input_switch_id").get_to(p.mInputSwitchId);
    j.at("activation_delay").get_to(p.mActivationDelay);
    j.at("output_switch_id1").get_to(p.mOutputSwitchId1);
    j.at("output_switch_id2").get_to(p.mOutputSwitchId2);
    j.at("output_switch_id3").get_to(p.mOutputSwitchId3);
    j.at("output_switch_id4").get_to(p.mOutputSwitchId4);
}

// Path_FlintLockFire
void to_json(nlohmann::json& j, const Path_FlintLockFire& p)
{
    j = nlohmann::json{
        {"name", "flint_lock_fire"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"switch_id", p.mSwitchId},
    };
}

void from_json(const nlohmann::json& j, Path_FlintLockFire& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("switch_id").get_to(p.mSwitchId);
}

// Path_HoneySack
void to_json(nlohmann::json& j, const Path_HoneySack& p)
{
    j = nlohmann::json{
        {"name", "honey_sack"},
        {"base", ToBase(p)},
        {"chase_time", p.mChaseTime},
        {"scale", p.mScale},
    };
}

void from_json(const nlohmann::json& j, Path_HoneySack& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("chase_time").get_to(p.mChaseTime);
    j.at("scale").get_to(p.mScale);
}

// Path_Bat
void to_json(nlohmann::json& j, const Path_Bat& p)
{
    j = nlohmann::json{
        {"name", "bat"},
        {"base", ToBase(p)},
        {"time_before_moving", p.mTimeBeforeMoving},
        {"speed", p.mSpeed},
        {"scale", p.mScale},
        {"attack_duration", p.mAttackDuration},
    };
}

void from_json(const nlohmann::json& j, Path_Bat& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("time_before_moving").get_to(p.mTimeBeforeMoving);
    j.at("speed").get_to(p.mSpeed);
    j.at("scale").get_to(p.mScale);
    j.at("attack_duration").get_to(p.mAttackDuration);
}

// Path_RollingBallStopper
void to_json(nlohmann::json& j, const Path_RollingBallStopper& p)
{
    j = nlohmann::json{
        {"name", "rolling_ball_stopper"},
        {"base", ToBase(p)},
        {"stopper_switch_id", p.mStopperSwitchId},
        {"scale", p.mScale},
        {"ball_switch_id", p.mBallSwitchId},
        {"stop_direction", p.mStopDirection},
    };
}

void from_json(const nlohmann::json& j, Path_RollingBallStopper& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("stopper_switch_id").get_to(p.mStopperSwitchId);
    j.at("scale").get_to(p.mScale);
    j.at("ball_switch_id").get_to(p.mBallSwitchId);
    j.at("stop_direction").get_to(p.mStopDirection);
}

// Path_RollingBall
void to_json(nlohmann::json& j, const Path_RollingBall& p)
{
    j = nlohmann::json{
        {"name", "rolling_ball"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"roll_direction", p.mRollDirection},
        {"release_switch_id", p.mReleaseSwitchId},
        {"max_speed", p.mMaxSpeed},
        {"acceleration", p.mAcceleration},
    };
}

void from_json(const nlohmann::json& j, Path_RollingBall& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("roll_direction").get_to(p.mRollDirection);
    j.at("release_switch_id").get_to(p.mReleaseSwitchId);
    j.at("max_speed").get_to(p.mMaxSpeed);
    j.at("acceleration").get_to(p.mAcceleration);
}

// Path_MotionDetector
NLOHMANN_JSON_SERIALIZE_ENUM(Path_MotionDetector::InitialMoveDirection, {
    {Path_MotionDetector::InitialMoveDirection::eRight, "right"},
    {Path_MotionDetector::InitialMoveDirection::eLeft, "left"},
})

void to_json(nlohmann::json& j, const Path_MotionDetector& p)
{
    j = nlohmann::json{
        {"name", "motion_detector"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"device_x", p.mDeviceX},
        {"device_y", p.mDeviceY},
        {"speedx256", p.mSpeedx256},
        {"initial_move_direction", p.mInitialMoveDirection},
        {"draw_flare", p.mDrawFlare},
        {"disable_switch_id", p.mDisableSwitchId},
        {"alarm_switch_id", p.mAlarmSwitchId},
        {"alarm_duration", p.mAlarmDuration},
    };
}

void from_json(const nlohmann::json& j, Path_MotionDetector& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("device_x").get_to(p.mDeviceX);
    j.at("device_y").get_to(p.mDeviceY);
    j.at("speedx256").get_to(p.mSpeedx256);
    j.at("initial_move_direction").get_to(p.mInitialMoveDirection);
    j.at("draw_flare").get_to(p.mDrawFlare);
    j.at("disable_switch_id").get_to(p.mDisableSwitchId);
    j.at("alarm_switch_id").get_to(p.mAlarmSwitchId);
    j.at("alarm_duration").get_to(p.mAlarmDuration);
}

// Path_BellHammer
void to_json(nlohmann::json& j, const Path_BellHammer& p)
{
    j = nlohmann::json{
        {"name", "bell_hammer"},
        {"base", ToBase(p)},
        {"switch_id", p.mSwitchId},
        {"action", p.mAction},
        {"scale", p.mScale},
        {"direction", p.mDirection},
    };
}

void from_json(const nlohmann::json& j, Path_BellHammer& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("action").get_to(p.mAction);
    j.at("scale").get_to(p.mScale);
    j.at("direction").get_to(p.mDirection);
}

// Path_SligBound
void to_json(nlohmann::json& j, const Path_SligBound& p)
{
    j = nlohmann::json{
        {"name", "slig_bound"},
        {"base", ToBase(p)},
        {"slig_bound_id", p.mSligBoundId},
        {"disabled_resources", p.mDisabledResources},
    };
}

void from_json(const nlohmann::json& j, Path_SligBound& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("slig_bound_id").get_to(p.mSligBoundId);
    j.at("disabled_resources").get_to(p.mDisabledResources);
}

// Path_BackgroundAnimation
NLOHMANN_JSON_SERIALIZE_ENUM(Path_BackgroundAnimation::TPageAbr, {
    {Path_BackgroundAnimation::TPageAbr::eBlend_0, "blend_0"},
    {Path_BackgroundAnimation::TPageAbr::eBlend_1, "blend_1"},
    {Path_BackgroundAnimation::TPageAbr::eBlend_2, "blend_2"},
    {Path_BackgroundAnimation::TPageAbr::eBlend_3, "blend_3"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(Path_BackgroundAnimation::BgAnimSounds, {
    {Path_BackgroundAnimation::BgAnimSounds::eNone_m1, "none_m1"},
    {Path_BackgroundAnimation::BgAnimSounds::eNone_0, "none_0"},
    {Path_BackgroundAnimation::BgAnimSounds::eFire, "fire"},
    {Path_BackgroundAnimation::BgAnimSounds::eFireIdx, "fire_idx"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(Path_BackgroundAnimation::Layer, {
    {Path_BackgroundAnimation::Layer::eLayer0, "layer_0"},
})

void to_json(nlohmann::json& j, const Path_BackgroundAnimation& p)
{
    j = nlohmann::json{
        {"name", "background_animation"},
        {"base", ToBase(p)},
        {"anim_id", p.mAnimId},
        {"is_semi_trans", p.mIsSemiTrans},
        {"semi_trans_mode", p.mSemiTransMode},
        {"sound_effect", p.mSoundEffect},
        {"layer", p.mLayer},
    };
}

void from_json(const nlohmann::json& j, Path_BackgroundAnimation& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("anim_id").get_to(p.mAnimId);
    j.at("is_semi_trans").get_to(p.mIsSemiTrans);
    j.at("semi_trans_mode").get_to(p.mSemiTransMode);
    j.at("sound_effect").get_to(p.mSoundEffect);
    j.at("layer").get_to(p.mLayer);
}

// Path_MainMenuController
void to_json(nlohmann::json& j, const Path_MainMenuController& p)
{
    j = nlohmann::json{
        {"name", "main_menu_controller"},
        {"base", ToBase(p)}
    };
}

void from_json(const nlohmann::json& j, Path_MainMenuController& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_ElumWall
void to_json(nlohmann::json& j, const Path_ElumWall& p)
{
    j = nlohmann::json{
        {"name", "elum_wall"},
        {"base", ToBase(p)}
    };
}

void from_json(const nlohmann::json& j, Path_ElumWall& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_ElumStart
void to_json(nlohmann::json& j, const Path_ElumStart& p)
{
    j = nlohmann::json{
        {"name", "elum_start"},
        {"base", ToBase(p)}
    };
}

void from_json(const nlohmann::json& j, Path_ElumStart& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_KillUnsavedMuds
void to_json(nlohmann::json& j, const Path_KillUnsavedMuds& p)
{
    j = nlohmann::json{
        {"name", "kill_unsaved_muds"},
        {"base", ToBase(p)}
    };
}

void from_json(const nlohmann::json& j, Path_KillUnsavedMuds& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_InvisibleZone
void to_json(nlohmann::json& j, const Path_InvisibleZone& p)
{
    j = nlohmann::json{
        {"name", "invisible_zone"},
        {"base", ToBase(p)}
    };
}

void from_json(const nlohmann::json& j, Path_InvisibleZone& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_StartController
void to_json(nlohmann::json& j, const Path_StartController& p)
{
    j = nlohmann::json{
        {"name", "start_controller"},
        {"base", ToBase(p)}
    };
}

void from_json(const nlohmann::json& j, Path_StartController& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_ScrabNoFall
void to_json(nlohmann::json& j, const Path_ScrabNoFall& p)
{
    j = nlohmann::json{
        {"name", "scrab_no_fall"},
        {"base", ToBase(p)}
    };
}

void from_json(const nlohmann::json& j, Path_ScrabNoFall& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_ScrabLeftBound
void to_json(nlohmann::json& j, const Path_ScrabLeftBound& p)
{
    j = nlohmann::json{
        {"name", "scrab_left_bound"},
        {"base", ToBase(p)}
    };
}

void from_json(const nlohmann::json& j, Path_ScrabLeftBound& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_ScrabRightBound
void to_json(nlohmann::json& j, const Path_ScrabRightBound& p)
{
    j = nlohmann::json{
        {"name", "scrab_right_bound"},
        {"base", ToBase(p)}
    };
}

void from_json(const nlohmann::json& j, Path_ScrabRightBound& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_HoneyDripTarget
void to_json(nlohmann::json& j, const Path_HoneyDripTarget& p)
{
    j = nlohmann::json{
        {"name", "honey_drip_target"},
        {"base", ToBase(p)}
    };
}

void from_json(const nlohmann::json& j, Path_HoneyDripTarget& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_ZSligCover
void to_json(nlohmann::json& j, const Path_ZSligCover& p)
{
    j = nlohmann::json{
        {"name", "z_slig_cover"},
        {"base", ToBase(p)}
    };
}

void from_json(const nlohmann::json& j, Path_ZSligCover& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_DeathDrop
void to_json(nlohmann::json& j, const Path_DeathDrop& p)
{
    j = nlohmann::json{
        {"name", "death_drop"},
        {"base", ToBase(p)}
    };
}

void from_json(const nlohmann::json& j, Path_DeathDrop& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_ChimeLock
void to_json(nlohmann::json& j, const Path_ChimeLock& p)
{
    j = nlohmann::json{
        {"name", "chime_lock"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"solve_switch_id", p.mSolveSwitchId},
        {"code1", p.mCode1},
        {"code2", p.mCode2},
        {"password_switch_id", p.mPasswordSwitchId},
    };
}

void from_json(const nlohmann::json& j, Path_ChimeLock& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("solve_switch_id").get_to(p.mSolveSwitchId);
    j.at("code1").get_to(p.mCode1);
    j.at("code2").get_to(p.mCode2);
    j.at("password_switch_id").get_to(p.mPasswordSwitchId);
}

// Path_LCDStatusBoard
void to_json(nlohmann::json& j, const Path_LCDStatusBoard& p)
{
    j = nlohmann::json{
        {"name", "lcd_status_board"},
        {"base", ToBase(p)},
        {"number_of_muds", p.mNumberOfMuds},
        {"zulag_number", p.mZulagNumber},
        {"hide_board", p.mHideBoard},
    };
}

void from_json(const nlohmann::json& j, Path_LCDStatusBoard& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("number_of_muds").get_to(p.mNumberOfMuds);
    j.at("zulag_number").get_to(p.mZulagNumber);
    j.at("hide_board").get_to(p.mHideBoard);
}

// Path_CreditsController
void to_json(nlohmann::json& j, const Path_CreditsController& p)
{
    j = nlohmann::json{
        {"name", "credits_controller"},
        {"base", ToBase(p)}
    };
}

void from_json(const nlohmann::json& j, Path_CreditsController& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_ResetPath
void to_json(nlohmann::json& j, const Path_ResetPath& p)
{
    j = nlohmann::json{
        {"name", "reset_path"},
        {"base", ToBase(p)},
        {"clear_ids", p.mClearIds},
        {"from", p.mFrom},
        {"to", p.mTo},
        {"exclude", p.mExclude},
        {"clear_objects", p.mClearObjects},
        {"path", p.mPath},
    };
}

void from_json(const nlohmann::json& j, Path_ResetPath& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("clear_ids").get_to(p.mClearIds);
    j.at("from").get_to(p.mFrom);
    j.at("to").get_to(p.mTo);
    j.at("exclude").get_to(p.mExclude);
    j.at("clear_objects").get_to(p.mClearObjects);
    j.at("path").get_to(p.mPath);
}

// Path_MeatSack
void to_json(nlohmann::json& j, const Path_MeatSack& p)
{
    j = nlohmann::json{
        {"name", "meat_sack"},
        {"base", ToBase(p)},
        {"meat_fall_direction", p.mMeatFallDirection},
        {"vel_x", p.mVelX},
        {"vel_y", p.mVelY},
        {"scale", p.mScale},
        {"meat_amount", p.mMeatAmount},
    };
}

void from_json(const nlohmann::json& j, Path_MeatSack& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("meat_fall_direction").get_to(p.mMeatFallDirection);
    j.at("vel_x").get_to(p.mVelX);
    j.at("vel_y").get_to(p.mVelY);
    j.at("scale").get_to(p.mScale);
    j.at("meat_amount").get_to(p.mMeatAmount);
}

// Path_Bees
void to_json(nlohmann::json& j, const Path_Bees& p)
{
    j = nlohmann::json{
        {"name", "bees"},
        {"base", ToBase(p)},
        {"switch_id", p.mSwitchId},
        {"swarm_size", p.mSwarmSize},
        {"chase_time", p.mChaseTime},
        {"speed", p.mSpeed},
        {"disabled_resources", p.mDisabledResources},
        {"bees_amount", p.mBeesAmount},
    };
}

void from_json(const nlohmann::json& j, Path_Bees& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("swarm_size").get_to(p.mSwarmSize);
    j.at("chase_time").get_to(p.mChaseTime);
    j.at("speed").get_to(p.mSpeed);
    j.at("disable_resources").get_to(p.mDisabledResources);
    j.at("bees_amount").get_to(p.mBeesAmount);
}

// Path_BellsongStone
NLOHMANN_JSON_SERIALIZE_ENUM(Path_BellsongStone::BellsongTypes, {
    {Path_BellsongStone::BellsongTypes::eWhistle, "whistle"},
    {Path_BellsongStone::BellsongTypes::eChimes, "chimes"},
})

void to_json(nlohmann::json& j, const Path_BellsongStone& p)
{
    j = nlohmann::json{
        {"name", "bellsong_stone"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"type", p.mType},
        {"code1", p.mCode1},
        {"code2", p.mCode2},
        {"switch_id", p.mSwitchId},
    };
}

void from_json(const nlohmann::json& j, Path_BellsongStone& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("type").get_to(p.mType);
    j.at("code1").get_to(p.mCode1);
    j.at("code2").get_to(p.mCode2);
    j.at("switch_id").get_to(p.mSwitchId);
}

// Path_MovieStone
void to_json(nlohmann::json& j, const Path_MovieStone& p)
{
    j = nlohmann::json{
        {"name", "movie_stone"},
        {"base", ToBase(p)},
        {"movie_id", p.mMovieId},
        {"scale", p.mScale},
        {"trigger_switch_id", p.mTriggerSwitchId},
    };
}

void from_json(const nlohmann::json& j, Path_MovieStone& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("movie_id").get_to(p.mMovieId);
    j.at("scale").get_to(p.mScale);
    j.at("trigger_switch_id").get_to(p.mTriggerSwitchId);
}

// Path_HandStone
void to_json(nlohmann::json& j, const Path_HandStone& p)
{
    j = nlohmann::json{
        {"name", "hand_stone"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"camera_id_1", p.mCameraId1},
        {"camera_id_2", p.mCameraId2},
        {"camera_id_3", p.mCameraId3},
        {"trigger_switch_id", p.mTriggerSwitchId},
        {"level_1", p.mLevel1},
        {"path_1", p.mPath1},
        {"level_2", p.mLevel2},
        {"path_2", p.mPath2},
        {"level_3", p.mLevel3},
        {"path_3", p.mPath3},
    };
}

void from_json(const nlohmann::json& j, Path_HandStone& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("camera_id_1").get_to(p.mCameraId1);
    j.at("camera_id_2").get_to(p.mCameraId2);
    j.at("camera_id_3").get_to(p.mCameraId3);
    j.at("trigger_switch_id").get_to(p.mTriggerSwitchId);
    j.at("level_1").get_to(p.mLevel1);
    j.at("path_1").get_to(p.mPath1);
    j.at("level_2").get_to(p.mLevel2);
    j.at("path_2").get_to(p.mPath2);
    j.at("level_3").get_to(p.mLevel3);
    j.at("path_3").get_to(p.mPath3);
}

// Path_PathTransition
void to_json(nlohmann::json& j, const Path_PathTransition& p)
{
    j = nlohmann::json{
        {"name", "path_transition"},
        {"base", ToBase(p)},
        {"next_level", p.mNextLevel},
        {"next_path", p.mNextPath},
        {"next_camera", p.mNextCamera},
        {"movie_id", p.mMovieId},
        {"wipe_effect", p.mWipeEffect},
        {"next_path_scale", p.mNextPathScale},
    };
}

void from_json(const nlohmann::json& j, Path_PathTransition& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("next_level").get_to(p.mNextLevel);
    j.at("next_path").get_to(p.mNextPath);
    j.at("next_camera").get_to(p.mNextCamera);
    j.at("movie_id").get_to(p.mMovieId);
    j.at("wipe_effect").get_to(p.mWipeEffect);
    j.at("next_path_scale").get_to(p.mNextPathScale);
}

// Path_Pulley
void to_json(nlohmann::json& j, const Path_Pulley& p)
{
    j = nlohmann::json{
        {"name", "pulley"},
        {"base", ToBase(p)},
    };
}

void from_json(const nlohmann::json& j, Path_Pulley& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_Honey
void to_json(nlohmann::json& j, const Path_Honey& p)
{
    j = nlohmann::json{
        {"name", "honey"},
        {"base", ToBase(p)},
    };
}

void from_json(const nlohmann::json& j, Path_Honey& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_BeeSwarmHole
NLOHMANN_JSON_SERIALIZE_ENUM(Path_BeeSwarmHole::MovementType, {
    {Path_BeeSwarmHole::MovementType::eHover, "hover"},
    {Path_BeeSwarmHole::MovementType::eAttack, "attack"},
    {Path_BeeSwarmHole::MovementType::eFollowPath, "follow_path"},
})

void to_json(nlohmann::json& j, const Path_BeeSwarmHole& p)
{
    j = nlohmann::json{
        {"name", "bee_swarm_hole"},
        {"base", ToBase(p)},
        {"start_interval", p.mStartInterval},
        {"movement_type", p.mMovementType},
        {"bees_amount", p.mBeesAmount},
        {"chase_time", p.mChaseTime},
        {"speed", p.mSpeed},
    };
}

void from_json(const nlohmann::json& j, Path_BeeSwarmHole& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("start_interval").get_to(p.mStartInterval);
    j.at("movement_type").get_to(p.mMovementType);
    j.at("bees_amount").get_to(p.mBeesAmount);
    j.at("chase_time").get_to(p.mChaseTime);
    j.at("speed").get_to(p.mSpeed);
}

// Path_Door
NLOHMANN_JSON_SERIALIZE_ENUM(Path_Door::DoorStates, {
    {Path_Door::DoorStates::eOpen, "open"},
    {Path_Door::DoorStates::eClosed, "closed"},
    {Path_Door::DoorStates::eOpening, "opening"},
    {Path_Door::DoorStates::eClosing, "closing"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(Path_Door::DoorTypes, {
    {Path_Door::DoorTypes::eBasicDoor, "basic_door"},
    {Path_Door::DoorTypes::eTasksDoorWithSecretMusic, "tasks_door_with_secret_music"},
    {Path_Door::DoorTypes::eTasksDoor, "tasks_door"},
    {Path_Door::DoorTypes::eTrialDoor, "trial_door"},
    {Path_Door::DoorTypes::eHubDoor, "hub_door"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(Path_Door::ScreenChangeEffects, {
    {Path_Door::ScreenChangeEffects::ePlay1FMV, "play_1_fmv"},
    {Path_Door::ScreenChangeEffects::eRightToLeft, "right_to_left"},
    {Path_Door::ScreenChangeEffects::eLeftToRight, "left_to_right"},
    {Path_Door::ScreenChangeEffects::eBottomToTop, "bottom_to_top"},
    {Path_Door::ScreenChangeEffects::eTopToBottom, "top_to_bottom"},
    {Path_Door::ScreenChangeEffects::eBoxOut, "box_out"},
    {Path_Door::ScreenChangeEffects::eVerticalSplit, "vertical_split"},
    {Path_Door::ScreenChangeEffects::eHorizontalSplit, "horizontal_split"},
    {Path_Door::ScreenChangeEffects::eUnknown, "unknown"},
    {Path_Door::ScreenChangeEffects::eInstantChange, "instant_change"},
})

void to_json(nlohmann::json& j, const Path_Door& p)
{
    j = nlohmann::json{
        {"name", "door"},
        {"base", ToBase(p)},
        {"next_level", p.mNextLevel},
        {"next_path", p.mNextPath},
        {"next_camera", p.mNextCamera},
        {"scale", p.mScale},
        {"door_id", p.mDoorId},
        {"switch_id", p.mSwitchId},
        {"target_door_id", p.mTargetDoorId},
        {"door_type", p.mDoorType},
        {"start_state", p.mStartState},
        {"hub_1", p.mHub1},
        {"hub_2", p.mHub2},
        {"hub_3", p.mHub3},
        {"hub_4", p.mHub4},
        {"hub_5", p.mHub5},
        {"hub_6", p.mHub6},
        {"hub_7", p.mHub7},
        {"hub_8", p.mHub8},
        {"wipe_effect", p.mWipeEffect},
        {"movie_id", p.mMovieId},
        {"door_offset_x", p.mDoorOffsetX},
        {"door_offset_y", p.mDoorOffsetY},
        {"exit_direction", p.mExitDirection},
        {"close_on_exit", p.mCloseOnExit},
        {"clear_throwables", p.mClearThrowables},
    };
}

void from_json(const nlohmann::json& j, Path_Door& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("next_level").get_to(p.mNextLevel);
    j.at("next_path").get_to(p.mNextPath);
    j.at("next_camera").get_to(p.mNextCamera);
    j.at("scale").get_to(p.mScale);
    j.at("door_id").get_to(p.mDoorId);
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("target_door_id").get_to(p.mTargetDoorId);
    j.at("door_type").get_to(p.mDoorType);
    j.at("start_state").get_to(p.mStartState);
    j.at("hub_1").get_to(p.mHub1);
    j.at("hub_2").get_to(p.mHub2);
    j.at("hub_3").get_to(p.mHub3);
    j.at("hub_4").get_to(p.mHub4);
    j.at("hub_5").get_to(p.mHub5);
    j.at("hub_6").get_to(p.mHub6);
    j.at("hub_7").get_to(p.mHub7);
    j.at("hub_8").get_to(p.mHub8);
    j.at("wipe_effect").get_to(p.mWipeEffect);
    j.at("movie_id").get_to(p.mMovieId);
    j.at("door_offset_x").get_to(p.mDoorOffsetX);
    j.at("door_offset_y").get_to(p.mDoorOffsetY);
    j.at("exit_direction").get_to(p.mExitDirection);
    j.at("close_on_exit").get_to(p.mCloseOnExit);
    j.at("clear_throwables").get_to(p.mClearThrowables);
}

// Path_AbeStart
void to_json(nlohmann::json& j, const Path_AbeStart& p)
{
    j = nlohmann::json{
        {"name", "abe_start"},
        {"base", ToBase(p)},
    };
}

void from_json(const nlohmann::json& j, Path_AbeStart& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_EnemyStopper
NLOHMANN_JSON_SERIALIZE_ENUM(Path_EnemyStopper::StopDirection, {
    {Path_EnemyStopper::StopDirection::Left, "left"},
    {Path_EnemyStopper::StopDirection::Right, "right"},
    {Path_EnemyStopper::StopDirection::Both, "both"},
})

void to_json(nlohmann::json& j, const Path_EnemyStopper& p)
{
    j = nlohmann::json{
        {"name", "enemy_stopper"},
        {"base", ToBase(p)},
        {"stop_direction", p.mStopDirection},
        {"switch_id", p.mSwitchId},
    };
}

void from_json(const nlohmann::json& j, Path_EnemyStopper& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("stop_direction").get_to(p.mStopDirection);
    j.at("switch_id").get_to(p.mSwitchId);
}

// Path_MovingBombStopper
void to_json(nlohmann::json& j, const Path_MovingBombStopper& p)
{
    j = nlohmann::json{
        {"name", "moving_bomb_stopper"},
        {"base", ToBase(p)},
        {"min", p.mMinDelay},
        {"max", p.mMaxDelay},
    };
}

void from_json(const nlohmann::json& j, Path_MovingBombStopper& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("min").get_to(p.mMinDelay);
    j.at("max").get_to(p.mMaxDelay);
}

// Path_DoorFlame
NLOHMANN_JSON_SERIALIZE_ENUM(Path_DoorFlame::Colour, {
    {Path_DoorFlame::Colour::eDefault, "default"},
    {Path_DoorFlame::Colour::eRed, "red"},
    {Path_DoorFlame::Colour::eGreen, "green"},
    {Path_DoorFlame::Colour::eBlue, "blue"},
})

void to_json(nlohmann::json& j, const Path_DoorFlame& p)
{
    j = nlohmann::json{
        {"name", "door_flame"},
        {"base", ToBase(p)},
        {"switch_id", p.mSwitchId},
        {"scale", p.mScale},
        {"colour", p.mColour},
    };
}

void from_json(const nlohmann::json& j, Path_DoorFlame& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("scale").get_to(p.mScale);
    j.at("colour").get_to(p.mColour);
}

// Path_Mudokon
NLOHMANN_JSON_SERIALIZE_ENUM(Path_Mudokon::MudJobs, {
    {Path_Mudokon::MudJobs::eChisle, "chisle"},
    {Path_Mudokon::MudJobs::eSitScrub, "sit_scrub"},
    {Path_Mudokon::MudJobs::eAngryWorker, "angry_worker"},
    {Path_Mudokon::MudJobs::eDamageRingGiver, "damage_ring_giver"},
    {Path_Mudokon::MudJobs::eHealthRingGiver, "health_ring_giver"},
    {Path_Mudokon::MudJobs::eStandScrub, "stand_scrub"},
    {Path_Mudokon::MudJobs::eSitChant, "sit_chant"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(Path_Mudokon::Mud_TLV_Emotion, {
    {Path_Mudokon::Mud_TLV_Emotion::eNormal, "normal"},
    {Path_Mudokon::Mud_TLV_Emotion::eAngry, "angry"},
    {Path_Mudokon::Mud_TLV_Emotion::eSad, "sad"},
    {Path_Mudokon::Mud_TLV_Emotion::eWired, "wired"},
    {Path_Mudokon::Mud_TLV_Emotion::eSick, "sick"},
})

void to_json(nlohmann::json& j, const Path_Mudokon& p)
{
    j = nlohmann::json{
        {"name", "mudokon"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"job", p.mJob},
        {"facing", p.mFacing},
        {"voice_pitch", p.mVoicePitch},
        {"rescue_switch_id", p.mRescueSwitchId},
        {"deaf", p.mDeaf},
        {"disabled_resources", p.mDisabledResources},
        {"persist_and_reset_offscreen", p.mPersistAndResetOffscreen},
        {"emotion", p.mEmotion},
        {"blind", p.mBlind},
        {"angry_switch_id", p.mAngrySwitchId},
        {"work_after_turning_wheel", p.mWorkAfterTurningWheel},
        {"gets_depressed", p.mGetsDepressed},
        {"ring_pulse_interval", p.mRingPulseInterval},
        {"give_ring_without_password", p.mGiveRingWithoutPassword},
    };
}

void from_json(const nlohmann::json& j, Path_Mudokon& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("job").get_to(p.mJob);
    j.at("facing").get_to(p.mFacing);
    j.at("voice_pitch").get_to(p.mVoicePitch);
    j.at("rescue_switch_id").get_to(p.mRescueSwitchId);
    j.at("deaf").get_to(p.mDeaf);
    j.at("disabled_resources").get_to(p.mDisabledResources);
    j.at("persist_and_reset_offscreen").get_to(p.mPersistAndResetOffscreen);
    j.at("emotion").get_to(p.mEmotion);
    j.at("blind").get_to(p.mBlind);
    j.at("angry_switch_id").get_to(p.mAngrySwitchId);
    j.at("work_after_turning_wheel").get_to(p.mWorkAfterTurningWheel);
    j.at("gets_depressed").get_to(p.mGetsDepressed);
    j.at("ring_pulse_interval").get_to(p.mRingPulseInterval);
    j.at("give_ring_without_password").get_to(p.mGiveRingWithoutPassword);

}

// Path_MovingBomb
void to_json(nlohmann::json& j, const Path_MovingBomb& p)
{
    j = nlohmann::json{
        {"name", "moving_bomb"},
        {"base", ToBase(p)},
        {"speed", p.mSpeed},
        {"start_moving_switch_id", p.mStartMovingSwitchId},
        {"triggered_by_alarm", p.mTriggeredByAlarm},
        {"scale", p.mScale},
        {"disabled_resources", p.mDisabledResources},
        {"start_speed", p.mStartSpeed},
        {"persist_offscreen", p.mPersistOffscreen},
    };
}

void from_json(const nlohmann::json& j, Path_MovingBomb& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("speed").get_to(p.mSpeed);
    j.at("start_moving_switch_id").get_to(p.mStartMovingSwitchId);
    j.at("triggered_by_alarm").get_to(p.mTriggeredByAlarm);
    j.at("scale").get_to(p.mScale);
    j.at("disabled_resources").get_to(p.mDisabledResources);
    j.at("start_speed").get_to(p.mStartSpeed);
    j.at("persist_offscreen").get_to(p.mPersistOffscreen);
}

// Path_ElumPathTrans
void to_json(nlohmann::json& j, const Path_ElumPathTrans& p)
{
    j = nlohmann::json{
        {"name", "elum_path_trans"},
        {"base", ToBase(p)},
        {"next_level", p.mNextLevel},
        {"next_path", p.mNextPath},
        {"next_camera", p.mNextCamera},
    };
}

void from_json(const nlohmann::json& j, Path_ElumPathTrans& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("next_level").get_to(p.mNextLevel);
    j.at("next_path").get_to(p.mNextPath);
    j.at("next_camera").get_to(p.mNextCamera);
}

// Path_MudokonPathTrans
void to_json(nlohmann::json& j, const Path_MudokonPathTrans& p)
{
    j = nlohmann::json{
        {"name", "mudokon_path_trans"},
        {"base", ToBase(p)},
        {"next_level", p.mNextLevel},
        {"next_path", p.mNextPath},
        {"next_camera", p.mNextCamera},
    };
}

void from_json(const nlohmann::json& j, Path_MudokonPathTrans& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("next_level").get_to(p.mNextLevel);
    j.at("next_path").get_to(p.mNextPath);
    j.at("next_camera").get_to(p.mNextCamera);
}

// Path_SecurityClaw
void to_json(nlohmann::json& j, const Path_SecurityClaw& p)
{
    j = nlohmann::json{
        {"name", "security_claw"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"alarm_switch_id", p.mAlarmSwitchId},
        {"alarm_duration", p.mAlarmDuration},
        {"disabled_resources", p.mDisabledResources},
    };
}

void from_json(const nlohmann::json& j, Path_SecurityClaw& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("alarm_switch_id").get_to(p.mAlarmSwitchId);
    j.at("alarm_duration").get_to(p.mAlarmDuration);
    j.at("disabled_resources").get_to(p.mDisabledResources);
}

// Path_SlingMudokon
void to_json(nlohmann::json& j, const Path_SlingMudokon& p)
{
    j = nlohmann::json{
        {"name", "sling_mudokon"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"dont_whistle_password", p.mDontWhistlePassword},
        {"code_1", p.mCode1},
        {"code_2", p.mCode2},
    };
}

void from_json(const nlohmann::json& j, Path_SlingMudokon& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("dont_whistle_password").get_to(p.mDontWhistlePassword);
    j.at("code_1").get_to(p.mCode1);
    j.at("code_2").get_to(p.mCode2);
}

// Path_FootSwitch
NLOHMANN_JSON_SERIALIZE_ENUM(Path_FootSwitch::FootSwitchTriggerBy, {
    {Path_FootSwitch::FootSwitchTriggerBy::eAbe, "abe"},
    {Path_FootSwitch::FootSwitchTriggerBy::eAnyone, "anyone"},
})

void to_json(nlohmann::json& j, const Path_FootSwitch& p)
{
    j = nlohmann::json{
        {"name", "foot_switch"},
        {"base", ToBase(p)},
        {"switch_id", p.mSwitchId},
        {"scale", p.mScale},
        {"action", p.mAction},
        {"triggered_by", p.mTriggeredBy},
    };
}

void from_json(const nlohmann::json& j, Path_FootSwitch& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("scale").get_to(p.mScale);
    j.at("action").get_to(p.mAction);
    j.at("triggered_by").get_to(p.mTriggeredBy);
}

// Path_Paramite
NLOHMANN_JSON_SERIALIZE_ENUM(Path_Paramite::EntranceType, {
    {Path_Paramite::EntranceType::ePatrol, "patrol"},
    {Path_Paramite::EntranceType::eSurpriseWeb, "surprise_web"},
    {Path_Paramite::EntranceType::eSlightlyHigherSpawnSurpriseWeb, "slightly_higher_spawn_surprise_web"},
})

void to_json(nlohmann::json& j, const Path_Paramite& p)
{
    j = nlohmann::json{
        {"name", "paramite"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"entrance_type", p.mEntranceType},
        {"alone_chase_delay", p.mAloneChaseDelay},
        {"surprise_web_delay_timer", p.mSurpriseWebDelayTimer},
        {"meat_eating_time", p.mMeatEatingTime},
        {"group_chase_delay", p.mGroupChaseDelay},
        {"surprise_web_switch_id", p.mSurpriseWebSwitchId},
        {"hiss_before_attack", p.mHissBeforeAttack},
        {"delete_when_out_of_sight", p.mDeleteWhenOutOfSight},
        {"attack_fleeches", p.mAttackFleeches},
    };
}

void from_json(const nlohmann::json& j, Path_Paramite& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("entrance_type").get_to(p.mEntranceType);
    j.at("alone_chase_delay").get_to(p.mAloneChaseDelay);
    j.at("surprise_web_delay_timer").get_to(p.mSurpriseWebDelayTimer);
    j.at("meat_eating_time").get_to(p.mMeatEatingTime);
    j.at("group_chase_delay").get_to(p.mGroupChaseDelay);
    j.at("surprise_web_switch_id").get_to(p.mSurpriseWebSwitchId);
    j.at("hiss_before_attack").get_to(p.mHissBeforeAttack);
    j.at("delete_when_out_of_sight").get_to(p.mDeleteWhenOutOfSight);
    j.at("attack_fleeches").get_to(p.mAttackFleeches);
}

// Path_ZzzSpawner
void to_json(nlohmann::json& j, const Path_ZzzSpawner& p)
{
    j = nlohmann::json{
        {"name", "zzz_spawner"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"switch_id", p.mSwitchId},
        {"zzz_interval", p.mZzzInterval},
    };
}

void from_json(const nlohmann::json& j, Path_ZzzSpawner& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("zzz_interval").get_to(p.mZzzInterval);
}

// Path_BackgroundGlukkon
void to_json(nlohmann::json& j, const Path_BackgroundGlukkon& p)
{
    j = nlohmann::json{
        {"name", "background_glukkon"},
        {"base", ToBase(p)},
        {"scale_percent", p.mScalePercent},
        {"pal_id", p.mPalId},
    };
}

void from_json(const nlohmann::json& j, Path_BackgroundGlukkon& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale_percent").get_to(p.mScalePercent);
    j.at("pal_id").get_to(p.mPalId);
}

// Path_GasEmitter
NLOHMANN_JSON_SERIALIZE_ENUM(Path_GasEmitter::GasColour, {
    {Path_GasEmitter::GasColour::eYellow, "yellow"},
    {Path_GasEmitter::GasColour::eRed, "red"},
    {Path_GasEmitter::GasColour::eGreen, "green"},
    {Path_GasEmitter::GasColour::eBlue, "blue"},
    {Path_GasEmitter::GasColour::eWhite, "white"},
})

void to_json(nlohmann::json& j, const Path_GasEmitter& p)
{
    j = nlohmann::json{
        {"name", "gas_emitter"},
        {"base", ToBase(p)},
        {"switch_id", p.mSwitchId},
        {"colour", p.mColour},
    };
}

void from_json(const nlohmann::json& j, Path_GasEmitter& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("colour").get_to(p.mColour);
}

// Path_GasCountDown
void to_json(nlohmann::json& j, const Path_GasCountDown& p)
{
    j = nlohmann::json{
        {"name", "gas_countdown"},
        {"base", ToBase(p)},
        {"start_timer_switch_id", p.mStartTimerSwitchId},
        {"gas_countdown_timer", p.mGasCountdownTimer},
        {"stop_timer_switch_id", p.mStopTimerSwitchId},
    };
}

void from_json(const nlohmann::json& j, Path_GasCountDown& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("start_timer_switch_id").get_to(p.mStartTimerSwitchId);
    j.at("gas_countdown_timer").get_to(p.mGasCountdownTimer);
    j.at("stop_timer_switch_id").get_to(p.mStopTimerSwitchId);
}

// Path_RingCancel
void to_json(nlohmann::json& j, const Path_RingCancel& p)
{
    j = nlohmann::json{
        {"name", "ring_cancel"},
        {"base", ToBase(p)},
    };
}

void from_json(const nlohmann::json& j, Path_RingCancel& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_SecurityDoor
void to_json(nlohmann::json& j, const Path_SecurityDoor& p)
{
    j = nlohmann::json{
        {"name", "security_door"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"switch_id", p.mSwitchId},
        {"code_1", p.mCode1},
        {"code_2", p.mCode2},
        {"xpos", p.mXPos},
        {"ypos", p.mYPos},
    };
}

void from_json(const nlohmann::json& j, Path_SecurityDoor& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("code_1").get_to(p.mCode1);
    j.at("code_2").get_to(p.mCode2);
    j.at("xpos").get_to(p.mXPos);
    j.at("ypos").get_to(p.mYPos);
}

// Path_LiftMudokon
NLOHMANN_JSON_SERIALIZE_ENUM(Path_LiftMudokon::Direction, {
    {Path_LiftMudokon::Direction::eRight, "right"},
    {Path_LiftMudokon::Direction::eLeft, "left"},
})

void to_json(nlohmann::json& j, const Path_LiftMudokon& p)
{
    j = nlohmann::json{
        {"name", "lift_mudokon"},
        {"base", ToBase(p)},
        {"how_far_to_walk", p.mHowFarToWalk},
        {"lift_switch_id", p.mLiftSwitchId},
        {"facing", p.mFacing},
        {"give_password", p.mGivePassword},
        {"scale", p.mScale},
        {"code_1", p.mCode1},
        {"code_2", p.mCode2},
    };
}

void from_json(const nlohmann::json& j, Path_LiftMudokon& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("how_far_to_walk").get_to(p.mHowFarToWalk);
    j.at("lift_switch_id").get_to(p.mLiftSwitchId);
    j.at("facing").get_to(p.mFacing);
    j.at("give_password").get_to(p.mGivePassword);
    j.at("scale").get_to(p.mScale);
    j.at("code_1").get_to(p.mCode1);
    j.at("code_2").get_to(p.mCode2);
}

// Path_RingMudokon
NLOHMANN_JSON_SERIALIZE_ENUM(Path_RingMudokon::MustFaceMud, {
    {Path_RingMudokon::MustFaceMud::eYes, "yes"},
    {Path_RingMudokon::MustFaceMud::eNo, "no"},
})

void to_json(nlohmann::json& j, const Path_RingMudokon& p)
{
    j = nlohmann::json{
        {"name", "ring_mudokon"},
        {"base", ToBase(p)},
        {"facing", p.mFacing},
        {"abe_must_face_mud", p.mAbeMustFaceMud},
        {"scale", p.mScale},
        {"give_password", p.mGivePassword},
        {"code_1", p.mCode1},
        {"code_2", p.mCode2},
        {"action", p.mAction},
        {"ring_timeout", p.mRingTimeout},
        {"give_ring_without_password", p.mGiveRingWithoutPassword},
    };
}

void from_json(const nlohmann::json& j, Path_RingMudokon& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("facing").get_to(p.mFacing);
    j.at("abe_must_face_mud").get_to(p.mAbeMustFaceMud);
    j.at("scale").get_to(p.mScale);
    j.at("give_password").get_to(p.mGivePassword);
    j.at("code_1").get_to(p.mCode1);
    j.at("code_2").get_to(p.mCode2);
    j.at("action").get_to(p.mAction);
    j.at("ring_timeout").get_to(p.mRingTimeout);
    j.at("give_ring_without_password").get_to(p.mGiveRingWithoutPassword);
}

// Path_WellLocal
void to_json(nlohmann::json& j, const Path_WellLocal& p)
{
    j = nlohmann::json{
        {"name", "well_local"},
        {"base", ToBase(p)},
        // Well base
        {"scale", p.mScale},
        {"switch_id", p.mSwitchId},
        {"other_well_id", p.mOtherWellId},
        {"anim_id", p.mAnimId},
        {"exit_x", p.mExitX},
        {"exit_y", p.mExitY},
        {"off_dest_level", p.mOffDestLevel},
        {"off_dest_x", p.mOffDestX},
        {"off_dest_path", p.mOffDestPath},
        {"off_dest_y", p.mOffDestY},

        // Well local
        {"on_dest_x", p.mOnDestX},
        {"on_dest_y", p.mOnDestY},
        {"emit_leaves", p.mEmitLeaves},
        {"leaf_x", p.mLeafX},
        {"leaf_y", p.mLeafY},
    };
}

void from_json(const nlohmann::json& j, Path_WellLocal& p)
{
    j.at("base").get_to(ToBase(p));
    // Well base
    j.at("scale").get_to(p.mScale);
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("other_well_id").get_to(p.mOtherWellId);
    j.at("anim_id").get_to(p.mAnimId);
    j.at("exit_x").get_to(p.mExitX);
    j.at("exit_y").get_to(p.mExitY);
    j.at("off_dest_level").get_to(p.mOffDestLevel);
    j.at("off_dest_x").get_to(p.mOffDestX);
    j.at("off_dest_path").get_to(p.mOffDestPath);
    j.at("off_dest_y").get_to(p.mOffDestY);

    // Well local
    j.at("on_dest_x").get_to(p.mOnDestX);
    j.at("on_dest_y").get_to(p.mOnDestY);
    j.at("emit_leaves").get_to(p.mEmitLeaves);
    j.at("leaf_x").get_to(p.mLeafX);
    j.at("leaf_y").get_to(p.mLeafY);
}

// Path_WellExpress
void to_json(nlohmann::json& j, const Path_WellExpress& p)
{
    j = nlohmann::json{
        {"name", "well_express"},
        {"base", ToBase(p)},
        // Well base
        {"scale", p.mScale},
        {"switch_id", p.mSwitchId},
        {"other_well_id", p.mOtherWellId},
        {"anim_id", p.mAnimId},
        {"exit_x", p.mExitX},
        {"exit_y", p.mExitY},
        {"off_dest_level", p.mOffDestLevel},
        {"off_dest_x", p.mOffDestX},
        {"off_dest_path", p.mOffDestPath},
        {"off_dest_y", p.mOffDestY},

        // Well express
        {"off_dest_camera", p.mOffDestCamera},
        {"off_other_well_id", p.mOffOtherWellId},
        {"on_dest_level", p.mOnDestLevel},
        {"on_dest_path", p.mOnDestPath},
        {"on_dest_camera", p.mOnDestCamera},
        {"on_other_well_id", p.mOnOtherWellId},
        {"emit_leaves", p.mEmitLeaves},
        {"leaf_x", p.mLeafX},
        {"leaf_y", p.mLeafY},
        {"movie_id", p.mMovieId},
    };
}

void from_json(const nlohmann::json& j, Path_WellExpress& p)
{
    j.at("base").get_to(ToBase(p));
    // Well base
    j.at("scale").get_to(p.mScale);
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("other_well_id").get_to(p.mOtherWellId);
    j.at("anim_id").get_to(p.mAnimId);
    j.at("exit_x").get_to(p.mExitX);
    j.at("exit_y").get_to(p.mExitY);
    j.at("off_dest_level").get_to(p.mOffDestLevel);
    j.at("off_dest_x").get_to(p.mOffDestX);
    j.at("off_dest_path").get_to(p.mOffDestPath);
    j.at("off_dest_y").get_to(p.mOffDestY);

    // Well express
    j.at("off_dest_camera").get_to(p.mOffDestCamera);
    j.at("off_other_well_id").get_to(p.mOffOtherWellId);
    j.at("on_dest_level").get_to(p.mOnDestLevel);
    j.at("on_dest_path").get_to(p.mOnDestPath);
    j.at("on_dest_camera").get_to(p.mOnDestCamera);
    j.at("on_other_well_id").get_to(p.mOnOtherWellId);
    j.at("emit_leaves").get_to(p.mEmitLeaves);
    j.at("leaf_x").get_to(p.mLeafX);
    j.at("leaf_y").get_to(p.mLeafY);
    j.at("movie_id").get_to(p.mMovieId);
}

// Path_Slog
void to_json(nlohmann::json& j, const Path_Slog& p)
{
    j = nlohmann::json{
        {"name", "slog"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"facing", p.mFacing},
        {"asleep", p.mAsleep},
        {"wake_up_anger", p.mWakeUpAnger},
        {"bark_anger", p.mBarkAnger},
        {"chase_anger", p.mChaseAnger},
        {"chase_delay", p.mChaseDelay},
        {"disabled_resources", p.mDisabledResources},
        {"anger_switch_id", p.mAngerSwitchId},
        {"bone_eating_time", p.mBoneEatingTime},
    };
}

void from_json(const nlohmann::json& j, Path_Slog& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("facing").get_to(p.mFacing);
    j.at("asleep").get_to(p.mAsleep);
    j.at("wake_up_anger").get_to(p.mWakeUpAnger);
    j.at("bark_anger").get_to(p.mBarkAnger);
    j.at("chase_anger").get_to(p.mChaseAnger);
    j.at("chase_delay").get_to(p.mChaseDelay);
    j.at("disabled_resources").get_to(p.mDisabledResources);
    j.at("anger_switch_id").get_to(p.mAngerSwitchId);
    j.at("bone_eating_time").get_to(p.mBoneEatingTime);
}

// Path_SlogSpawner
NLOHMANN_JSON_SERIALIZE_ENUM(Path_SlogSpawner::StartDirection, {
    {Path_SlogSpawner::StartDirection::eRight, "right"},
    {Path_SlogSpawner::StartDirection::eLeft, "left"},
})

void to_json(nlohmann::json& j, const Path_SlogSpawner& p)
{
    j = nlohmann::json{
        {"name", "slog_spawner"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"max_slogs", p.mMaxSlogs},
        {"max_slogs_at_a_time", p.mMaxSlogsAtATime},
        {"start_direction", p.mStartDirection},
        {"slog_spawn_interval", p.mSlogSpawnInterval},
        {"spawner_switch_id", p.mSpawnerSwitchId},
        {"listen_to_sligs", p.mListenToSligs},
        {"chase_delay", p.mChaseDelay},
    };
}

void from_json(const nlohmann::json& j, Path_SlogSpawner& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("max_slogs").get_to(p.mMaxSlogs);
    j.at("max_slogs_at_a_time").get_to(p.mMaxSlogsAtATime);
    j.at("start_direction").get_to(p.mStartDirection);
    j.at("slog_spawn_interval").get_to(p.mSlogSpawnInterval);
    j.at("spawner_switch_id").get_to(p.mSpawnerSwitchId);
    j.at("listen_to_sligs").get_to(p.mListenToSligs);
    j.at("chase_delay").get_to(p.mChaseDelay);
}

// Path_Scrab
NLOHMANN_JSON_SERIALIZE_ENUM(Path_Scrab::ScrabPatrolType, {
    {Path_Scrab::ScrabPatrolType::eWalk, "walk"},
    {Path_Scrab::ScrabPatrolType::eRunOrWalk192, "run_or_walk_192"},
    {Path_Scrab::ScrabPatrolType::eRunOrWalk128, "run_or_walk_128"},
    {Path_Scrab::ScrabPatrolType::eRunOrWalk64, "run_or_walk_64"},
    {Path_Scrab::ScrabPatrolType::eRun, "run"},
})

void to_json(nlohmann::json& j, const Path_Scrab& p)
{
    j = nlohmann::json{
        {"name", "scrab"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"attack_delay", p.mAttackDelay},
        {"patrol_type_run_or_walk_chance", p.mPatrolTypeRunOrWalkChance},
        {"patrol_type", p.mPatrolType},
        {"left_min_delay", p.mPauseLeftMin},
        {"left_max_delay", p.mPauseLeftMax},
        {"right_min_delay", p.mPauseRightMin},
        {"right_max_delay", p.mPauseRightMax},
        {"pause_after_chase_delay", p.mPauseAfterChaseTime},
        {"spotting_abe_delay", p.mSpottingAbeDelay},
        {"disabled_resources", p.mDisabledResources},
        {"roar_randomly", p.mRoarRandomly},
        {"persistant", p.mPersistant},
        {"possessed_max_whirl_attack_duration", p.mPossessedMaxWhirlAttackDuration},
        {"kill_enemy", p.mKillEnemy},
    };
}

void from_json(const nlohmann::json& j, Path_Scrab& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("attack_delay").get_to(p.mAttackDelay);
    j.at("patrol_type_run_or_walk_chance").get_to(p.mPatrolTypeRunOrWalkChance);
    j.at("patrol_type").get_to(p.mPatrolType);
    j.at("left_min_delay").get_to(p.mPauseLeftMin);
    j.at("left_max_delay").get_to(p.mPauseLeftMax);
    j.at("right_min_delay").get_to(p.mPauseRightMin);
    j.at("right_max_delay").get_to(p.mPauseRightMax);
    j.at("pause_after_chase_delay").get_to(p.mPauseAfterChaseTime);
    j.at("spotting_abe_delay").get_to(p.mSpottingAbeDelay);
    j.at("disabled_resources").get_to(p.mDisabledResources);
    j.at("roar_randomly").get_to(p.mRoarRandomly);
    j.at("persistant").get_to(p.mPersistant);
    j.at("possessed_max_whirl_attack_duration").get_to(p.mPossessedMaxWhirlAttackDuration);
    j.at("kil_enemy").get_to(p.mKillEnemy);
}

// Path_Slig
NLOHMANN_JSON_SERIALIZE_ENUM(Path_Slig::StartState, {
    {Path_Slig::StartState::Listening, "listening"},
    {Path_Slig::StartState::Patrol, "patrol"},
    {Path_Slig::StartState::Sleeping, "sleeping"},
    {Path_Slig::StartState::Chase, "chase"},
    {Path_Slig::StartState::ChaseAndDisappear, "chase_and_disappear"},
    {Path_Slig::StartState::eFallingToChase, "falling_to_chase"},
    {Path_Slig::StartState::ListeningToGlukkon, "listening_to_glukkon"},
})

void to_json(nlohmann::json& j, const Path_Slig& p)
{
    j = nlohmann::json{
        {"name", "slig"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"start_state", p.mStartState},
        {"pause_time", p.mPauseTime},
        {"pause_left_min", p.mPauseLeftMin},
        {"pause_left_max", p.mPauseLeftMax},
        {"pause_right_min", p.mPauseRightMin},
        {"pause_right_max", p.mPauseRightMax},
        {"shoot_possessed_sligs", p.mShootPossessedSligs},
        {"shoot_on_sight_delay", p.mShootOnSightDelay},
        {"num_times_to_shoot", p.mNumTimesToShoot},
        {"code_1", p.mCode1},
        {"code_2", p.mCode2},
        {"chase_abe_when_spotted", p.mChaseAbeWhenSpotted},
        {"facing", p.mFacing},
        {"panic_timeout", p.mPanicTimeout},
        {"stop_chase_delay", p.mStopChaseDelay},
        {"time_to_wait_before_chase", p.mTimeToWaitBeforeChase},
        {"slig_bound_id", p.mSligBoundId},
        {"alerted_listen_time", p.mAlertedListenTime},
        {"percent_say_what", p.mPercentSayWhat},
        {"percent_beat_mud", p.mPercentBeatMud},
        {"z_shoot_delay", p.mZShootDelay},
        {"stay_awake", p.mStayAwake},
        {"disabled_resources", p.mDisabledResources},
        {"noise_wake_up_distance", p.mNoiseWakeUpDistance},
        {"slig_spawner_switch_id", p.mSligSpawnerSwitchId},
        {"unlimited_spawns", p.mUnlimitedSpawns},
    };
}

void from_json(const nlohmann::json& j, Path_Slig& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("start_state").get_to(p.mStartState);
    j.at("pause_time").get_to(p.mPauseTime);
    j.at("pause_left_min").get_to(p.mPauseLeftMin);
    j.at("pause_left_max").get_to(p.mPauseLeftMax);
    j.at("pause_right_min").get_to(p.mPauseRightMin);
    j.at("pause_right_max").get_to(p.mPauseRightMax);
    j.at("shoot_possessed_sligs").get_to(p.mShootPossessedSligs);
    j.at("shoot_on_sight_delay").get_to(p.mShootOnSightDelay);
    j.at("num_times_to_shoot").get_to(p.mNumTimesToShoot);
    j.at("code_1").get_to(p.mCode1);
    j.at("code_2").get_to(p.mCode2);
    j.at("chase_abe_when_spotted").get_to(p.mChaseAbeWhenSpotted);
    j.at("facing").get_to(p.mFacing);
    j.at("panic_timeout").get_to(p.mPanicTimeout);
    j.at("stop_chase_delay").get_to(p.mStopChaseDelay);
    j.at("time_to_wait_before_chase").get_to(p.mTimeToWaitBeforeChase);
    j.at("slig_bound_id").get_to(p.mSligBoundId);
    j.at("alerted_listen_time").get_to(p.mAlertedListenTime);
    j.at("percent_say_what").get_to(p.mPercentSayWhat);
    j.at("percent_beat_mud").get_to(p.mPercentBeatMud);
    j.at("z_shoot_delay").get_to(p.mZShootDelay);
    j.at("stay_awake").get_to(p.mStayAwake);
    j.at("disabled_resources").get_to(p.mDisabledResources);
    j.at("noise_wake_up_distance").get_to(p.mNoiseWakeUpDistance);
    j.at("slig_spawner_switch_id").get_to(p.mSligSpawnerSwitchId);
    j.at("unlimited_spawns").get_to(p.mUnlimitedSpawns);
}

// Path_SligSpawner
void to_json(nlohmann::json& j, const Path_SligSpawner& p)
{
    j = nlohmann::json{
        {"name", "slig"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"start_state", p.mStartState},
        {"pause_time", p.mPauseTime},
        {"pause_left_min", p.mPauseLeftMin},
        {"pause_left_max", p.mPauseLeftMax},
        {"pause_right_min", p.mPauseRightMin},
        {"pause_right_max", p.mPauseRightMax},
        {"shoot_possessed_sligs", p.mShootPossessedSligs},
        {"shoot_on_sight_delay", p.mShootOnSightDelay},
        {"num_times_to_shoot", p.mNumTimesToShoot},
        {"code_1", p.mCode1},
        {"code_2", p.mCode2},
        {"chase_abe_when_spotted", p.mChaseAbeWhenSpotted},
        {"facing", p.mFacing},
        {"panic_timeout", p.mPanicTimeout},
        {"stop_chase_delay", p.mStopChaseDelay},
        {"time_to_wait_before_chase", p.mTimeToWaitBeforeChase},
        {"slig_bound_id", p.mSligBoundId},
        {"alerted_listen_time", p.mAlertedListenTime},
        {"percent_say_what", p.mPercentSayWhat},
        {"percent_beat_mud", p.mPercentBeatMud},
        {"z_shoot_delay", p.mZShootDelay},
        {"stay_awake", p.mStayAwake},
        {"disabled_resources", p.mDisabledResources},
        {"noise_wake_up_distance", p.mNoiseWakeUpDistance},
        {"slig_spawner_switch_id", p.mSligSpawnerSwitchId},
        {"unlimited_spawns", p.mUnlimitedSpawns},
    };
}

void from_json(const nlohmann::json& j, Path_SligSpawner& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("start_state").get_to(p.mStartState);
    j.at("pause_time").get_to(p.mPauseTime);
    j.at("pause_left_min").get_to(p.mPauseLeftMin);
    j.at("pause_left_max").get_to(p.mPauseLeftMax);
    j.at("pause_right_min").get_to(p.mPauseRightMin);
    j.at("pause_right_max").get_to(p.mPauseRightMax);
    j.at("shoot_possessed_sligs").get_to(p.mShootPossessedSligs);
    j.at("shoot_on_sight_delay").get_to(p.mShootOnSightDelay);
    j.at("num_times_to_shoot").get_to(p.mNumTimesToShoot);
    j.at("code_1").get_to(p.mCode1);
    j.at("code_2").get_to(p.mCode2);
    j.at("chase_abe_when_spotted").get_to(p.mChaseAbeWhenSpotted);
    j.at("facing").get_to(p.mFacing);
    j.at("panic_timeout").get_to(p.mPanicTimeout);
    j.at("stop_chase_delay").get_to(p.mStopChaseDelay);
    j.at("time_to_wait_before_chase").get_to(p.mTimeToWaitBeforeChase);
    j.at("slig_bound_id").get_to(p.mSligBoundId);
    j.at("alerted_listen_time").get_to(p.mAlertedListenTime);
    j.at("percent_say_what").get_to(p.mPercentSayWhat);
    j.at("percent_beat_mud").get_to(p.mPercentBeatMud);
    j.at("z_shoot_delay").get_to(p.mZShootDelay);
    j.at("stay_awake").get_to(p.mStayAwake);
    j.at("disabled_resources").get_to(p.mDisabledResources);
    j.at("noise_wake_up_distance").get_to(p.mNoiseWakeUpDistance);
    j.at("slig_spawner_switch_id").get_to(p.mSligSpawnerSwitchId);
    j.at("unlimited_spawns").get_to(p.mUnlimitedSpawns);
}

// Path_TrainDoor
void to_json(nlohmann::json& j, const Path_TrainDoor& p)
{
    j = nlohmann::json{
        {"name", "train_door"},
        {"base", ToBase(p)},
        {"direction", p.mDirection},
    };
}

void from_json(const nlohmann::json& j, Path_TrainDoor& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("direction").get_to(p.mDirection);
}

// Path_TorturedMudokon
void to_json(nlohmann::json& j, const Path_TorturedMudokon& p)
{
    j = nlohmann::json{
        {"name", "tortured_mudokon"},
        {"base", ToBase(p)},
        {"kill_switch_id", p.mKillSwitchId},
        {"release_switch_id", p.mReleaseSwitchId},
    };
}

void from_json(const nlohmann::json& j, Path_TorturedMudokon& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("kill_switch_id").get_to(p.mKillSwitchId);
    j.at("release_switch_id").get_to(p.mReleaseSwitchId);
}

// Path_DoorBlocker
void to_json(nlohmann::json& j, const Path_DoorBlocker& p)
{
    j = nlohmann::json{
        {"name", "door_blocker"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"switch_id", p.mSwitchId},
    };
}

void from_json(const nlohmann::json& j, Path_DoorBlocker& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("switch_id").get_to(p.mSwitchId);
}

// Path_GlukkonSwitch
void to_json(nlohmann::json& j, const Path_GlukkonSwitch& p)
{
    j = nlohmann::json{
        {"name", "glukkon_switch"},
        {"base", ToBase(p)},
        {"scale", p.field_10_scale},
        {"ok_switch_id", p.field_12_ok_switch_id},
        {"fail_switch_id", p.field_14_fail_switch_id},
        {"xpos", p.field_16_xpos},
        {"ypos", p.field_18_ypos},
    };
}

void from_json(const nlohmann::json& j, Path_GlukkonSwitch& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.field_10_scale);
    j.at("ok_switch_id").get_to(p.field_12_ok_switch_id);
    j.at("fail_switch_id").get_to(p.field_14_fail_switch_id);
    j.at("xpos").get_to(p.field_16_xpos);
    j.at("ypos").get_to(p.field_18_ypos);
}

// Path_Greeter
void to_json(nlohmann::json& j, const Path_Greeter& p)
{
    j = nlohmann::json{
        {"name", "greeter"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"motion_detector_speed", p.mMotionDetectorSpeed},
        {"facing", p.mFacing},
    };
}

void from_json(const nlohmann::json& j, Path_Greeter& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("motion_detector_speed").get_to(p.mMotionDetectorSpeed);
    j.at("facing").get_to(p.mFacing);
}

// Path_BrewMachine
void to_json(nlohmann::json& j, const Path_BrewMachine& p)
{
    j = nlohmann::json{
        {"name", "brew_machine"},
        {"base", ToBase(p)},
        {"brew_count", p.mBrewCount},
    };
}

void from_json(const nlohmann::json& j, Path_BrewMachine& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("brew_count").get_to(p.mBrewCount);
}

// Path_Alarm
void to_json(nlohmann::json& j, const Path_Alarm& p)
{
    j = nlohmann::json{
        {"name", "alarm"},
        {"base", ToBase(p)},
        {"switch_id", p.mSwitchId},
        {"alarm_duration", p.mAlarmDuration},
    };
}

void from_json(const nlohmann::json& j, Path_Alarm& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("alarm_duration").get_to(p.mAlarmDuration);
}

// Path_ParamiteWebLine
void to_json(nlohmann::json& j, const Path_ParamiteWebLine& p)
{
    j = nlohmann::json{
        {"name", "paramite_web_line"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
    };
}

void from_json(const nlohmann::json& j, Path_ParamiteWebLine& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
}

// Path_SlapLock
void to_json(nlohmann::json& j, const Path_SlapLock& p)
{
    j = nlohmann::json{
        {"name", "slap_lock"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"target_tomb_switch_id_1", p.mTargetTombSwitchId1},
        {"target_tomb_switch_id_2", p.mTargetTombSwitchId2},
        {"has_ghost", p.mHasGhost},
        {"give_invisibility_powerup", p.mGiveInvisibilityPowerup},
        {"invisibility_duration", p.mInvisibilityDuration},
        {"slap_output_switch_id", p.mSlapOutputSwitchId},
    };
}

void from_json(const nlohmann::json& j, Path_SlapLock& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("target_tomb_switch_id_1").get_to(p.mTargetTombSwitchId1);
    j.at("target_tomb_switch_id_2").get_to(p.mTargetTombSwitchId2);
    j.at("has_ghost").get_to(p.mHasGhost);
    j.at("give_invisibility_powerup").get_to(p.mGiveInvisibilityPowerup);
    j.at("invisibility_duration").get_to(p.mInvisibilityDuration);
    j.at("slap_output_switch_id").get_to(p.mSlapOutputSwitchId);
}

// Path_StatusLight
void to_json(nlohmann::json& j, const Path_StatusLight& p)
{
    j = nlohmann::json{
        {"name", "status_light"},
        {"base", ToBase(p)},
        {"input_switch_id", p.mInputSwitchId},
        {"scale", p.mScale},
        {"linked_status_light_switch_id_1", p.mLinkedStatusLightSwitchId1},
        {"linked_status_light_switch_id_2", p.mLinkedStatusLightSwitchId2},
        {"linked_status_light_switch_id_3", p.mLinkedStatusLightSwitchId3},
        {"linked_status_light_switch_id_4", p.mLinkedStatusLightSwitchId4},
        {"linked_status_light_switch_id_5", p.mLinkedStatusLightSwitchId5},
        {"ignore_grid_snapping", p.mIgnoreGridSnapping},
    };
}

void from_json(const nlohmann::json& j, Path_StatusLight& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("input_switch_id").get_to(p.mInputSwitchId);
    j.at("scale").get_to(p.mScale);
    j.at("linked_status_light_switch_id_1").get_to(p.mLinkedStatusLightSwitchId1);
    j.at("linked_status_light_switch_id_2").get_to(p.mLinkedStatusLightSwitchId2);
    j.at("linked_status_light_switch_id_3").get_to(p.mLinkedStatusLightSwitchId3);
    j.at("linked_status_light_switch_id_4").get_to(p.mLinkedStatusLightSwitchId4);
    j.at("linked_status_light_switch_id_5").get_to(p.mLinkedStatusLightSwitchId5);
    j.at("ignore_grid_snapping").get_to(p.mIgnoreGridSnapping);
}

} // namespace relive

