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
        {"zone_number", p.field_18_zone_number},
        {"clear_from_id", p.field_1A_clear_from_id},
        {"clear_to_id", p.field_1C_clear_to_id},
        {"elum_restarts", p.field_1E_elum_restarts},
        {"abe_direction", p.field_20_abe_direction},

        {"scale", p.field_10_scale},
        {"save_file_id", p.field_12_save_file_id},
    };
}

void from_json(const nlohmann::json& j, Path_ContinuePoint& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("zone_number").get_to(p.field_18_zone_number);
    j.at("clear_from_id").get_to(p.field_1A_clear_from_id);
    j.at("clear_to_id").get_to(p.field_1C_clear_to_id);
    j.at("elum_restarts").get_to(p.field_1E_elum_restarts);
    j.at("abe_direction").get_to(p.field_20_abe_direction);

    j.at("scale").get_to(p.field_10_scale);
    j.at("save_file_id").get_to(p.field_12_save_file_id);
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
        {"base", ToBase(p)},
        {"lift_point_id", p.field_10_lift_point_id},
        {"start_point", p.field_12_bStart_point},
        {"lift_point_stop_type", p.field_16_lift_point_stop_type},
        {"scale", p.field_18_scale},
        {"ignore_lift_mover", p.field_1A_bIgnore_lift_mover},
    };
}

void from_json(const nlohmann::json& j, Path_LiftPoint& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("lift_point_id").get_to(p.field_10_lift_point_id);
    j.at("start_point").get_to(p.field_12_bStart_point);
    j.at("lift_point_stop_type").get_to(p.field_16_lift_point_stop_type);
    j.at("scale").get_to(p.field_18_scale);
    j.at("ignore_lift_mover").get_to(p.field_1A_bIgnore_lift_mover);
}

// Path_Dove
void to_json(nlohmann::json& j, const Path_Dove& p)
{
    j = nlohmann::json{
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
        {"base", ToBase(p)},
        {"fall_direction", p.field_10_fall_direction},
        {"x_vel", p.field_12_x_vel},
        {"y_vel", p.field_14_y_vel},
        {"scale", p.field_16_scale},
        {"rock_amount", p.field_18_rock_amount},
    };
}

void from_json(const nlohmann::json& j, Path_RockSack& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("fall_direction").get_to(p.field_10_fall_direction);
    j.at("x_vel").get_to(p.field_12_x_vel);
    j.at("y_vel").get_to(p.field_14_y_vel);
    j.at("scale").get_to(p.field_16_scale);
    j.at("rock_amount").get_to(p.field_18_rock_amount);
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
        {"base", ToBase(p)},
        {"switch_id", p.mSwitchId},
        {"start_state", p.mStartState},
        {"self_closing", p.mSelfClosing},
        {"scale", p.mScale},
        {"direction", p.mDirection},
        {"x_off", p.field_1C_xOff},
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
    j.at("x_off").get_to(p.field_1C_xOff);
    j.at("stay_open_time").get_to(p.mStayOpenTime);
}

// Path_LCDScreen
void to_json(nlohmann::json& j, const Path_LCDScreen& p)
{
    j = nlohmann::json{
        {"base", ToBase(p)},
        {"message_1_id", p.field_10_message_1_id},
        {"message_rand_min_id", p.field_12_message_rand_min_id},
        {"message_rand_max_id", p.field_14_message_rand_max_id},
        {"message_2_id", p.field_16_message_2_id},
        {"toggle_message_switch_id", p.field_18_toggle_message_switch_id},
    };
}

void from_json(const nlohmann::json& j, Path_LCDScreen& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("message_1_id").get_to(p.field_10_message_1_id);
    j.at("message_rand_min_id").get_to(p.field_12_message_rand_min_id);
    j.at("message_rand_max_id").get_to(p.field_14_message_rand_max_id);
    j.at("message_2_id").get_to(p.field_16_message_2_id);
    j.at("toggle_message_switch_id").get_to(p.field_18_toggle_message_switch_id);
}

// Path_Mine
void to_json(nlohmann::json& j, const Path_Mine& p)
{
    j = nlohmann::json{
        {"base", ToBase(p)},
        {"num_patterns", p.field_10_num_patterns},
        {"pattern", p.field_12_pattern},
        {"scale", p.field_14_scale},
        {"disabled_resources", p.field_16_disabled_resources},
        {"persist_offscreen", p.field_18_persist_offscreen},
    };
}

void from_json(const nlohmann::json& j, Path_Mine& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("num_patterns").get_to(p.field_10_num_patterns);
    j.at("pattern").get_to(p.field_12_pattern);
    j.at("scale").get_to(p.field_14_scale);
    j.at("disabled_resources").get_to(p.field_16_disabled_resources);
    j.at("persist_offscreen").get_to(p.field_18_persist_offscreen);
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
        {"base", ToBase(p)},
        {"switch_id", p.field_10_switch_id},
        {"action", p.field_12_action},
        {"delay", p.field_14_delay},
        {"set_off_alarm", p.field_16_set_off_alarm},
        {"scale", p.field_18_scale},
    };
}

void from_json(const nlohmann::json& j, Path_InvisibleSwitch& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("switch_id").get_to(p.field_10_switch_id);
    j.at("action").get_to(p.field_12_action);
    j.at("delay").get_to(p.field_14_delay);
    j.at("set_off_alarm").get_to(p.field_16_set_off_alarm);
    j.at("scale").get_to(p.field_18_scale);
}

// Path_ElectricWall
NLOHMANN_JSON_SERIALIZE_ENUM(Path_ElectricWall::ElectricWallStartState, {
    {Path_ElectricWall::ElectricWallStartState::eOff, "off"},
    {Path_ElectricWall::ElectricWallStartState::eOn, "on"},
})

void to_json(nlohmann::json& j, const Path_ElectricWall& p)
{
    j = nlohmann::json{
        {"base", ToBase(p)},
        {"scale", p.field_10_scale},
        {"switch_id", p.field_12_switch_id},
        {"start_state", p.field_14_start_state},
    };
}

void from_json(const nlohmann::json& j, Path_ElectricWall& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.field_10_scale);
    j.at("switch_id").get_to(p.field_12_switch_id);
    j.at("start_state").get_to(p.field_14_start_state);
}

// Path_BoomMachine
NLOHMANN_JSON_SERIALIZE_ENUM(Path_BoomMachine::NozzleSide, {
    {Path_BoomMachine::NozzleSide::eRight, "right"},
    {Path_BoomMachine::NozzleSide::eLeft, "left"},
})

void to_json(nlohmann::json& j, const Path_BoomMachine& p)
{
    j = nlohmann::json{
        {"base", ToBase(p)},
        {"scale", p.field_10_scale},
        {"nozzle_side", p.field_12_nozzle_side},
        {"disabled_resources", p.field_14_disabled_resources},
        {"number_of_grenades", p.field_16_number_of_grenades},
    };
}

void from_json(const nlohmann::json& j, Path_BoomMachine& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.field_10_scale);
    j.at("nozzle_side").get_to(p.field_12_nozzle_side);
    j.at("disabled_resources").get_to(p.field_14_disabled_resources);
    j.at("number_of_grenades").get_to(p.field_16_number_of_grenades);
}

// Path_UXB
NLOHMANN_JSON_SERIALIZE_ENUM(Path_UXB::StartState, {
    {Path_UXB::StartState::eOn, "on"},
    {Path_UXB::StartState::eOff, "off"},
})

void to_json(nlohmann::json& j, const Path_UXB& p)
{
    j = nlohmann::json{
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
        {"base", ToBase(p)},
        {"scale", p.field_18_scale},
        {"switch_min_time_off", p.field_1A_switch_min_time_off},
        {"switch_max_time_off", p.field_1C_switch_max_time_off},
        {"max_rise_time", p.field_1E_max_rise_time},
        {"switch_id", p.field_20_switch_id},
        {"type", p.field_22_type},
        {"speed", p.field_24_speed},
        {"start_state", p.field_26_start_state},
        {"off_speed", p.field_28_off_speed},
        {"automatic_min_time_off", p.field_2A_automatic_min_time_off},
        {"automatic_max_time_off", p.field_2C_automatic_max_time_off},
        {"initial_position", p.field_2E_inital_position},
    };
}

void from_json(const nlohmann::json& j, Path_MeatSaw& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.field_18_scale);
    j.at("switch_min_time_off").get_to(p.field_1A_switch_min_time_off);
    j.at("switch_max_time_off").get_to(p.field_1C_switch_max_time_off);
    j.at("max_rise_time").get_to(p.field_1E_max_rise_time);
    j.at("switch_id").get_to(p.field_20_switch_id);
    j.at("type").get_to(p.field_22_type);
    j.at("speed").get_to(p.field_24_speed);
    j.at("start_state").get_to(p.field_26_start_state);
    j.at("off_speed").get_to(p.field_28_off_speed);
    j.at("automatic_min_time_off").get_to(p.field_2A_automatic_min_time_off);
    j.at("automatic_max_time_off").get_to(p.field_2C_automatic_max_time_off);
    j.at("initial_position").get_to(p.field_2E_inital_position);
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
        {"base", ToBase(p)},
        {"action", p.field_10_action},
        {"scale", p.field_12_scale},
        {"on_sound", p.field_14_on_sound},
        {"off_sound", p.field_16_off_sound},
        {"sound_direction", p.field_18_sound_direction},
        {"switch_id", p.field_1A_switch_id},
        {"persist_offscreen", p.field_1C_persist_offscreen},
    };
}

void from_json(const nlohmann::json& j, Path_Lever& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("action").get_to(p.field_10_action);
    j.at("scale").get_to(p.field_12_scale);
    j.at("on_sound").get_to(p.field_14_on_sound);
    j.at("off_sound").get_to(p.field_16_off_sound);
    j.at("sound_direction").get_to(p.field_18_sound_direction);
    j.at("switch_id").get_to(p.field_1A_switch_id);
    j.at("persist_offscreen").get_to(p.field_1C_persist_offscreen);
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
        {"base", ToBase(p)},
        {"type", p.field_18_type},
        {"size", p.field_1A_size},
        {"switch_id", p.field_1C_switch_id},
        {"direction", p.field_1E_direction},
    };
}

void from_json(const nlohmann::json& j, Path_LightEffect& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("type").get_to(p.field_18_type);
    j.at("size").get_to(p.field_1A_size);
    j.at("switch_id").get_to(p.field_1C_switch_id);
    j.at("direction").get_to(p.field_1E_direction);
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
        {"base", ToBase(p)},
        {"music_type", p.field_10_music_type},
        {"triggered_by", p.field_12_triggered_by},
        {"switch_id", p.mSwitchId},
        {"music_delay", p.field_14_music_delay},
    };
}

void from_json(const nlohmann::json& j, Path_MusicTrigger& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("music_type").get_to(p.field_10_music_type);
    j.at("triggered_by").get_to(p.field_12_triggered_by);
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("music_delay").get_to(p.field_14_music_delay);
}

// Path_SoftLanding
void to_json(nlohmann::json& j, const Path_SoftLanding& p)
{
    j = nlohmann::json{
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
        {"base", ToBase(p)},
        {"input_switch_id", p.field_10_input_switch_id},
        {"trigger_interval", p.field_12_trigger_interval},
        {"output_switch_id1", p.field_14_output_switch_id1},
        {"output_switch_id2", p.field_16_output_switch_id2},
        {"output_switch_id3", p.field_18_output_switch_id3},
        {"output_switch_id4", p.field_1A_output_switch_id4},
    };
}

void from_json(const nlohmann::json& j, Path_TimerTrigger& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("input_switch_id").get_to(p.field_10_input_switch_id);
    j.at("trigger_interval").get_to(p.field_12_trigger_interval);
    j.at("output_switch_id1").get_to(p.field_14_output_switch_id1);
    j.at("output_switch_id2").get_to(p.field_16_output_switch_id2);
    j.at("output_switch_id3").get_to(p.field_18_output_switch_id3);
    j.at("output_switch_id4").get_to(p.field_1A_output_switch_id4);
}

// Path_FlintLockFire
void to_json(nlohmann::json& j, const Path_FlintLockFire& p)
{
    j = nlohmann::json{
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
        {"base", ToBase(p)},
        {"chase_ticks", p.mChaseTicks},
        {"scale", p.mScale},
    };
}

void from_json(const nlohmann::json& j, Path_HoneySack& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("chase_ticks").get_to(p.mChaseTicks);
    j.at("scale").get_to(p.mScale);
}

// Path_Bat
void to_json(nlohmann::json& j, const Path_Bat& p)
{
    j = nlohmann::json{
        {"base", ToBase(p)},
        {"ticks_before_moving", p.mTicksBeforeMoving},
        {"speed", p.mSpeed},
        {"scale", p.mScale},
        {"attack_duration", p.mAttackDuration},
    };
}

void from_json(const nlohmann::json& j, Path_Bat& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("ticks_before_moving").get_to(p.mTicksBeforeMoving);
    j.at("speed").get_to(p.mSpeed);
    j.at("scale").get_to(p.mScale);
    j.at("attack_duration").get_to(p.mAttackDuration);
}

// Path_RollingBallStopper
void to_json(nlohmann::json& j, const Path_RollingBallStopper& p)
{
    j = nlohmann::json{
        {"base", ToBase(p)},
        {"stopper_switch_id", p.field_18_stopper_switch_id},
        {"scale", p.field_1A_scale},
        {"ball_switch_id", p.field_1C_ball_switch_id},
        {"direction", p.field_1E_direction},
    };
}

void from_json(const nlohmann::json& j, Path_RollingBallStopper& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("stopper_switch_id").get_to(p.field_18_stopper_switch_id);
    j.at("scale").get_to(p.field_1A_scale);
    j.at("ball_switch_id").get_to(p.field_1C_ball_switch_id);
    j.at("direction").get_to(p.field_1E_direction);
}

// Path_RollingBall
void to_json(nlohmann::json& j, const Path_RollingBall& p)
{
    j = nlohmann::json{
        {"base", ToBase(p)},
        {"scale", p.field_18_scale},
        {"roll_direction", p.field_1A_roll_direction},
        {"release_switch_id", p.field_1C_release_switch_id},
        {"speed", p.field_1E_speed},
        {"acceleration", p.field_20_acceleration},
    };
}

void from_json(const nlohmann::json& j, Path_RollingBall& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.field_18_scale);
    j.at("roll_direction").get_to(p.field_1A_roll_direction);
    j.at("release_switch_id").get_to(p.field_1C_release_switch_id);
    j.at("speed").get_to(p.field_1E_speed);
    j.at("acceleration").get_to(p.field_20_acceleration);
}

// Path_MotionDetector
NLOHMANN_JSON_SERIALIZE_ENUM(Path_MotionDetector::InitialMoveDirection, {
    {Path_MotionDetector::InitialMoveDirection::eRight, "right"},
    {Path_MotionDetector::InitialMoveDirection::eLeft, "left"},
})

void to_json(nlohmann::json& j, const Path_MotionDetector& p)
{
    j = nlohmann::json{
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
        {"base", ToBase(p)},
        {"slig_bound_id", p.field_10_slig_bound_id},
        {"disabled_resources", p.field_12_disabled_resources},
    };
}

void from_json(const nlohmann::json& j, Path_SligBound& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("slig_bound_id").get_to(p.field_10_slig_bound_id);
    j.at("disabled_resources").get_to(p.field_12_disabled_resources);
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
        {"base", ToBase(p)},
        {"anim_id", p.field_10_anim_id},
        {"is_semi_trans", p.field_12_is_semi_trans},
        {"semi_trans_mode", p.field_14_semi_trans_mode},
        {"sound_effect", p.field_16_sound_effect},
        {"layer", p.field_1A_layer},
    };
}

void from_json(const nlohmann::json& j, Path_BackgroundAnimation& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("anim_id").get_to(p.field_10_anim_id);
    j.at("is_semi_trans").get_to(p.field_12_is_semi_trans);
    j.at("semi_trans_mode").get_to(p.field_14_semi_trans_mode);
    j.at("sound_effect").get_to(p.field_16_sound_effect);
    j.at("layer").get_to(p.field_1A_layer);
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
        {"base", ToBase(p)},
        {"scale", p.field_18_scale},
        {"solve_switch_id", p.field_1A_solve_switch_id},
        {"code1", p.field_1C_code1},
        {"code2", p.field_1E_code2},
        {"password_switch_id", p.field_20_password_switch_id},
    };
}

void from_json(const nlohmann::json& j, Path_ChimeLock& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.field_18_scale);
    j.at("solve_switch_id").get_to(p.field_1A_solve_switch_id);
    j.at("code1").get_to(p.field_1C_code1);
    j.at("code2").get_to(p.field_1E_code2);
    j.at("password_switch_id").get_to(p.field_20_password_switch_id);
}

// Path_LCDStatusBoard
void to_json(nlohmann::json& j, const Path_LCDStatusBoard& p)
{
    j = nlohmann::json{
        {"base", ToBase(p)},
        {"number_of_muds", p.field_10_number_of_muds},
        {"zulag_number", p.field_12_zulag_number},
        {"hidden", p.field_14_hidden},
    };
}

void from_json(const nlohmann::json& j, Path_LCDStatusBoard& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("number_of_muds").get_to(p.field_10_number_of_muds);
    j.at("zulag_number").get_to(p.field_12_zulag_number);
    j.at("hidden").get_to(p.field_14_hidden);
}

// Path_CreditsController
void to_json(nlohmann::json& j, const Path_CreditsController& p)
{
    j = nlohmann::json{
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
        {"base", ToBase(p)},
        {"clear_ids", p.field_18_clearIds},
        {"from", p.field_1A_from},
        {"to", p.field_1C_to},
        {"exclude", p.field_1E_exclude},
        {"clear_objects", p.field_20_clearObjects},
        {"path", p.field_22_path},
    };
}

void from_json(const nlohmann::json& j, Path_ResetPath& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("clear_ids").get_to(p.field_18_clearIds);
    j.at("from").get_to(p.field_1A_from);
    j.at("to").get_to(p.field_1C_to);
    j.at("exclude").get_to(p.field_1E_exclude);
    j.at("clear_objects").get_to(p.field_20_clearObjects);
    j.at("path").get_to(p.field_22_path);
}

// Path_MeatSack
void to_json(nlohmann::json& j, const Path_MeatSack& p)
{
    j = nlohmann::json{
        {"base", ToBase(p)},
        {"meat_fall_direction", p.field_10_meat_fall_direction},
        {"x_vel", p.field_12_xVel},
        {"y_vel", p.field_14_yVel},
        {"scale", p.field_16_scale},
        {"amount_of_meat", p.field_18_amount_of_meat},
    };
}

void from_json(const nlohmann::json& j, Path_MeatSack& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("meat_fall_direction").get_to(p.field_10_meat_fall_direction);
    j.at("x_vel").get_to(p.field_12_xVel);
    j.at("y_vel").get_to(p.field_14_yVel);
    j.at("scale").get_to(p.field_16_scale);
    j.at("amount_of_meat").get_to(p.field_18_amount_of_meat);
}

// Path_Bees
void to_json(nlohmann::json& j, const Path_Bees& p)
{
    j = nlohmann::json{
        {"base", ToBase(p)},
        {"switch_id", p.switch_id},
        {"swarm_size", p.swarm_size},
        {"chase_time", p.chase_time},
        {"speed", p.speed},
        {"disable_resources", p.disable_resources},
        {"num_bees", p.num_bees},
    };
}

void from_json(const nlohmann::json& j, Path_Bees& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("switch_id").get_to(p.switch_id);
    j.at("swarm_size").get_to(p.swarm_size);
    j.at("chase_time").get_to(p.chase_time);
    j.at("speed").get_to(p.speed);
    j.at("disable_resources").get_to(p.disable_resources);
    j.at("num_bees").get_to(p.num_bees);
}

// Path_BellsongStone
NLOHMANN_JSON_SERIALIZE_ENUM(Path_BellsongStone::BellsongTypes, {
    {Path_BellsongStone::BellsongTypes::eWhistle, "whistle"},
    {Path_BellsongStone::BellsongTypes::eChimes, "chimes"},
})

void to_json(nlohmann::json& j, const Path_BellsongStone& p)
{
    j = nlohmann::json{
        {"base", ToBase(p)},
        {"scale", p.scale},
        {"type", p.type},
        {"code1", p.code1},
        {"code2", p.code2},
        {"switch_id", p.switch_id},
    };
}

void from_json(const nlohmann::json& j, Path_BellsongStone& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.scale);
    j.at("type").get_to(p.type);
    j.at("code1").get_to(p.code1);
    j.at("code2").get_to(p.code2);
    j.at("switch_id").get_to(p.switch_id);
}

// Path_MovieStone
void to_json(nlohmann::json& j, const Path_MovieStone& p)
{
    j = nlohmann::json{
        {"base", ToBase(p)},
        {"movie_number", p.field_10_movie_number},
        {"scale", p.field_12_scale},
        {"trigger_switch_id", p.field_14_trigger_switch_id},
    };
}

void from_json(const nlohmann::json& j, Path_MovieStone& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("movie_number").get_to(p.field_10_movie_number);
    j.at("scale").get_to(p.field_12_scale);
    j.at("trigger_switch_id").get_to(p.field_14_trigger_switch_id);
}

// Path_HandStone
void to_json(nlohmann::json& j, const Path_HandStone& p)
{
    j = nlohmann::json{
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
        {"base", ToBase(p)},
        {"level", p.field_10_level},
        {"path", p.field_12_path},
        {"camera", p.field_14_camera},
        {"movie", p.field_16_movie},
        {"wipe", p.field_18_wipe},
        {"scale", p.field_1A_scale},
    };
}

void from_json(const nlohmann::json& j, Path_PathTransition& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("level").get_to(p.field_10_level);
    j.at("path").get_to(p.field_12_path);
    j.at("camera").get_to(p.field_14_camera);
    j.at("movie").get_to(p.field_16_movie);
    j.at("wipe").get_to(p.field_18_wipe);
    j.at("scale").get_to(p.field_1A_scale);
}

// Path_Pulley
void to_json(nlohmann::json& j, const Path_Pulley& p)
{
    j = nlohmann::json{
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
        {"base", ToBase(p)},
        {"level", p.field_10_level},
        {"path", p.field_12_path},
        {"camera", p.field_14_camera},
        {"scale", p.field_16_scale},
        {"door_number", p.field_18_door_number},
        {"switch_id", p.field_1A_switch_id},
        {"target_door_id", p.field_1C_target_door_id},
        {"type", p.field_1E_type},
        {"start_state", p.field_20_start_state},
        {"hub_1", p.field_22_hub1},
        {"hub_2", p.field_22_hub2},
        {"hub_3", p.field_22_hub3},
        {"hub_4", p.field_22_hub4},
        {"hub_5", p.field_22_hub5},
        {"hub_6", p.field_22_hub6},
        {"hub_7", p.field_22_hub7},
        {"hub_8", p.field_22_hub8},
        {"wipe_effect", p.field_32_wipe_effect},
        {"movie_number", p.field_34_movie_number},
        {"x_offset", p.field_36_x_offset},
        {"y_offset", p.field_38_y_offset},
        {"abe_direction", p.field_3E_abe_direction},
        {"close_on_exit", p.field_40_close_on_exit},
        {"clear_throwables", p.field_42_clear_throwables},
    };
}

void from_json(const nlohmann::json& j, Path_Door& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("level").get_to(p.field_10_level);
    j.at("path").get_to(p.field_12_path);
    j.at("camera").get_to(p.field_14_camera);
    j.at("scale").get_to(p.field_16_scale);
    j.at("door_number").get_to(p.field_18_door_number);
    j.at("switch_id").get_to(p.field_1A_switch_id);
    j.at("target_door_id").get_to(p.field_1C_target_door_id);
    j.at("type").get_to(p.field_1E_type);
    j.at("start_state").get_to(p.field_20_start_state);
    j.at("hub_1").get_to(p.field_22_hub1);
    j.at("hub_2").get_to(p.field_22_hub2);
    j.at("hub_3").get_to(p.field_22_hub3);
    j.at("hub_4").get_to(p.field_22_hub4);
    j.at("hub_5").get_to(p.field_22_hub5);
    j.at("hub_6").get_to(p.field_22_hub6);
    j.at("hub_7").get_to(p.field_22_hub7);
    j.at("hub_8").get_to(p.field_22_hub8);
    j.at("wipe_effect").get_to(p.field_32_wipe_effect);
    j.at("movie_number").get_to(p.field_34_movie_number);
    j.at("x_offset").get_to(p.field_36_x_offset);
    j.at("y_offset").get_to(p.field_38_y_offset);
    j.at("abe_direction").get_to(p.field_3E_abe_direction);
    j.at("close_on_exit").get_to(p.field_40_close_on_exit);
    j.at("clear_throwables").get_to(p.field_42_clear_throwables);
}

// Path_AbeStart
void to_json(nlohmann::json& j, const Path_AbeStart& p)
{
    j = nlohmann::json{
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
        {"base", ToBase(p)},
        {"min", p.field_10_min},
        {"max", p.field_12_max},
    };
}

void from_json(const nlohmann::json& j, Path_MovingBombStopper& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("min").get_to(p.field_10_min);
    j.at("max").get_to(p.field_12_max);
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
        {"base", ToBase(p)},
        {"scale", p.field_10_scale},
        {"job", p.field_12_job},
        {"direction", p.field_14_direction},
        {"voice_pitch", p.field_16_voice_pitch},
        {"rescue_switch_id", p.field_18_rescue_switch_id},
        {"deaf", p.field_1A_bDeaf},
        {"disabled_resources", p.field_1C_disabled_resources},
        {"persist_and_reset_offscreen", p.field_1E_persist_and_reset_offscreen},
        {"emotion", p.field_20_emotion},
        {"blind", p.field_22_bBlind},
        {"angry_switch_id", p.field_24_angry_switch_id},
        {"work_after_turning_wheel", p.field_26_work_after_turning_wheel},
        {"gets_depressed", p.field_28_bGets_depressed},
        {"ring_pulse_interval", p.field_2A_ring_pulse_interval},
        {"give_ring_without_password", p.field_2C_bGive_ring_without_password},
    };
}

void from_json(const nlohmann::json& j, Path_Mudokon& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.field_10_scale);
    j.at("job").get_to(p.field_12_job);
    j.at("direction").get_to(p.field_14_direction);
    j.at("voice_pitch").get_to(p.field_16_voice_pitch);
    j.at("rescue_switch_id").get_to(p.field_18_rescue_switch_id);
    j.at("deaf").get_to(p.field_1A_bDeaf);
    j.at("disabled_resources").get_to(p.field_1C_disabled_resources);
    j.at("persist_and_reset_offscreen").get_to(p.field_1E_persist_and_reset_offscreen);
    j.at("emotion").get_to(p.field_20_emotion);
    j.at("blind").get_to(p.field_22_bBlind);
    j.at("angry_switch_id").get_to(p.field_24_angry_switch_id);
    j.at("work_after_turning_wheel").get_to(p.field_26_work_after_turning_wheel);
    j.at("gets_depressed").get_to(p.field_28_bGets_depressed);
    j.at("ring_pulse_interval").get_to(p.field_2A_ring_pulse_interval);
    j.at("give_ring_without_password").get_to(p.field_2C_bGive_ring_without_password);

}

// Path_MovingBomb
void to_json(nlohmann::json& j, const Path_MovingBomb& p)
{
    j = nlohmann::json{
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
        {"base", ToBase(p)},
        {"level", p.mLevel},
        {"path", p.mPath},
        {"camera", p.mCamera},
    };
}

void from_json(const nlohmann::json& j, Path_ElumPathTrans& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("level").get_to(p.mLevel);
    j.at("path").get_to(p.mPath);
    j.at("camera").get_to(p.mCamera);
}

// Path_MudokonPathTrans
void to_json(nlohmann::json& j, const Path_MudokonPathTrans& p)
{
    j = nlohmann::json{
        {"base", ToBase(p)},
        {"level", p.mLevel},
        {"path", p.mPath},
        {"camera", p.mCamera},
    };
}

void from_json(const nlohmann::json& j, Path_MudokonPathTrans& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("level").get_to(p.mLevel);
    j.at("path").get_to(p.mPath);
    j.at("camera").get_to(p.mCamera);
}

// Path_SecurityClaw
void to_json(nlohmann::json& j, const Path_SecurityClaw& p)
{
    j = nlohmann::json{
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
        {"base", ToBase(p)},
        {"scale_percent", p.field_18_scale_percent},
        {"pal_id", p.field_1A_pal_id},
        {"target_id", p.field_1C_target_id},
        {"voice_adjust", p.field_1E_voice_adjust},
    };
}

void from_json(const nlohmann::json& j, Path_BackgroundGlukkon& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale_percent").get_to(p.field_18_scale_percent);
    j.at("pal_id").get_to(p.field_1A_pal_id);
    j.at("target_id").get_to(p.field_1C_target_id);
    j.at("voice_adjust").get_to(p.field_1E_voice_adjust);
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
        {"base", ToBase(p)},
        {"how_far_to_walk", p.mHowFarToWalk},
        {"lift_switch_id", p.mLiftSwitchId},
        {"direction", p.mDirection},
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
    j.at("direction").get_to(p.mDirection);
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
        {"base", ToBase(p)},
        {"direction", p.mDirection},
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
    j.at("direction").get_to(p.mDirection);
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
        {"base", ToBase(p)},
        {"scale", p.field_10_scale},
        {"direction", p.field_12_direction},
        {"asleep", p.field_14_asleep},
        {"wake_up_anger", p.field_16_wake_up_anger},
        {"bark_anger", p.field_18_bark_anger},
        {"chase_anger", p.field_1A_chase_anger},
        {"chase_delay", p.field_1C_chase_delay},
        {"disabled_resources", p.field_1E_disabled_resources},
        {"anger_switch_id", p.field_20_anger_switch_id},
        {"bone_eating_time", p.field_22_bone_eating_time},
    };
}

void from_json(const nlohmann::json& j, Path_Slog& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.field_10_scale);
    j.at("direction").get_to(p.field_12_direction);
    j.at("asleep").get_to(p.field_14_asleep);
    j.at("wake_up_anger").get_to(p.field_16_wake_up_anger);
    j.at("bark_anger").get_to(p.field_18_bark_anger);
    j.at("chase_anger").get_to(p.field_1A_chase_anger);
    j.at("chase_delay").get_to(p.field_1C_chase_delay);
    j.at("disabled_resources").get_to(p.field_1E_disabled_resources);
    j.at("anger_switch_id").get_to(p.field_20_anger_switch_id);
    j.at("bone_eating_time").get_to(p.field_22_bone_eating_time);
}

// Path_SlogSpawner
NLOHMANN_JSON_SERIALIZE_ENUM(Path_SlogSpawner::StartDirection, {
    {Path_SlogSpawner::StartDirection::eRight, "right"},
    {Path_SlogSpawner::StartDirection::eLeft, "left"},
})

void to_json(nlohmann::json& j, const Path_SlogSpawner& p)
{
    j = nlohmann::json{
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
        {"base", ToBase(p)},
        {"scale", p.field_10_scale},
        {"attack_delay", p.field_12_attack_delay},
        {"patrol_type_run_or_walk_chance", p.field_14_patrol_type_run_or_walk_chance},
        {"patrol_type", p.field_1C_patrol_type},
        {"left_min_delay", p.field_16_left_min_delay},
        {"left_max_delay", p.field_18_left_max_delay},
        {"right_min_delay", p.field_1A_right_min_delay},
        {"right_max_delay", p.field_1C_right_max_delay},
        {"pause_after_chase_delay", p.field_1E_pause_after_chase_delay},
        {"spotting_abe_delay", p.field_26_spotting_abe_delay},
        {"disabled_resources", p.field_20_disabled_resources},
        {"roar_randomly", p.field_22_roar_randomly},
        {"persistant", p.field_24_persistant},
        {"possessed_max_whirl_attack_duration", p.field_26_possessed_max_whirl_attack_duration},
        {"kill_enemy", p.field_2A_bKill_enemy},
    };
}

void from_json(const nlohmann::json& j, Path_Scrab& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.field_10_scale);
    j.at("attack_delay").get_to(p.field_12_attack_delay);
    j.at("patrol_type_run_or_walk_chance").get_to(p.field_14_patrol_type_run_or_walk_chance);
    j.at("patrol_type").get_to(p.field_1C_patrol_type);
    j.at("left_min_delay").get_to(p.field_16_left_min_delay);
    j.at("left_max_delay").get_to(p.field_18_left_max_delay);
    j.at("right_min_delay").get_to(p.field_1A_right_min_delay);
    j.at("right_max_delay").get_to(p.field_1C_right_max_delay);
    j.at("pause_after_chase_delay").get_to(p.field_1E_pause_after_chase_delay);
    j.at("spotting_abe_delay").get_to(p.field_26_spotting_abe_delay);
    j.at("disabled_resources").get_to(p.field_20_disabled_resources);
    j.at("roar_randomly").get_to(p.field_22_roar_randomly);
    j.at("persistant").get_to(p.field_24_persistant);
    j.at("possessed_max_whirl_attack_duration").get_to(p.field_26_possessed_max_whirl_attack_duration);
    j.at("kil_enemy").get_to(p.field_2A_bKill_enemy);
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
        {"base", ToBase(p)},
        {"scale", p.field_10_scale},
        {"start_state", p.field_12_start_state},
        {"pause_time", p.field_14_pause_time},
        {"pause_left_min", p.field_16_pause_left_min},
        {"pause_left_max", p.field_18_pause_left_max},
        {"pause_right_min", p.field_1A_pause_right_min},
        {"pause_right_max", p.field_1C_pause_right_max},
        {"shoot_possessed_sligs", p.field_1E_shoot_possessed_sligs},
        {"shoot_on_sight_delay", p.field_20_shoot_on_sight_delay},
        {"num_times_to_shoot", p.field_22_num_times_to_shoot},
        {"padding", p.field_24_padding},
        {"code_1", p.field_26_code1},
        {"code_2", p.field_28_code2},
        {"chase_abe_when_spotted", p.field_2A_chase_abe_when_spotted},
        {"start_Direction", p.field_2C_start_direction},
        {"panic_timeout", p.field_2E_panic_timeout},
        {"num_panic_sounds", p.field_30_num_panic_sounds},
        {"panic_sound_timeout", p.field_32_panic_sound_timeout},
        {"stop_chase_delay", p.field_34_stop_chase_delay},
        {"time_to_wait_before_chase", p.field_36_time_to_wait_before_chase},
        {"slig_bound_id", p.field_38_slig_bound_id},
        {"alerted_listen_time", p.field_3A_alerted_listen_time},
        {"percent_say_what", p.field_3C_percent_say_what},
        {"percent_beat_mud", p.field_3E_percent_beat_mud},
        {"talk_to_abe", p.field_40_talk_to_abe},
        {"dont_shoot", p.field_42_dont_shoot},
        {"z_shoot_delay", p.field_44_Z_shoot_delay},
        {"stay_awake", p.field_46_stay_awake},
        {"disabled_resources", p.field_48_disabled_resources},
        {"noise_wake_up_distance", p.field_4A_noise_wake_up_distance},
        {"slig_spawner_Switch_id", p.field_4C_slig_spawner_switch_id},
        {"unlimited_spawns", p.field_4E_unlimited_spawns},
    };
}

void from_json(const nlohmann::json& j, Path_Slig& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.field_10_scale);
    j.at("start_state").get_to(p.field_12_start_state);
    j.at("pause_time").get_to(p.field_14_pause_time);
    j.at("pause_left_min").get_to(p.field_16_pause_left_min);
    j.at("pause_left_max").get_to(p.field_18_pause_left_max);
    j.at("pause_right_min").get_to(p.field_1A_pause_right_min);
    j.at("pause_right_max").get_to(p.field_1C_pause_right_max);
    j.at("shoot_possessed_sligs").get_to(p.field_1E_shoot_possessed_sligs);
    j.at("shoot_on_sight_delay").get_to(p.field_20_shoot_on_sight_delay);
    j.at("num_times_to_shoot").get_to(p.field_22_num_times_to_shoot);
    j.at("padding").get_to(p.field_24_padding);
    j.at("code_1").get_to(p.field_26_code1);
    j.at("code_2").get_to(p.field_28_code2);
    j.at("chase_abe_when_spotted").get_to(p.field_2A_chase_abe_when_spotted);
    j.at("start_direction").get_to(p.field_2C_start_direction);
    j.at("panic_timeout").get_to(p.field_2E_panic_timeout);
    j.at("num_panic_sounds").get_to(p.field_30_num_panic_sounds);
    j.at("panic_sound_timeout").get_to(p.field_32_panic_sound_timeout);
    j.at("stop_chase_delay").get_to(p.field_34_stop_chase_delay);
    j.at("time_to_wait_before_chase").get_to(p.field_36_time_to_wait_before_chase);
    j.at("slig_bound_id").get_to(p.field_38_slig_bound_id);
    j.at("alerted_listen_time").get_to(p.field_3A_alerted_listen_time);
    j.at("percent_say_what").get_to(p.field_3C_percent_say_what);
    j.at("percent_beat_mud").get_to(p.field_3E_percent_beat_mud);
    j.at("talk_to_abe").get_to(p.field_40_talk_to_abe);
    j.at("dont_shoot").get_to(p.field_42_dont_shoot);
    j.at("z_shoot_delay").get_to(p.field_44_Z_shoot_delay);
    j.at("stay_awake").get_to(p.field_46_stay_awake);
    j.at("disabled_resources").get_to(p.field_48_disabled_resources);
    j.at("noise_wake_up_distance").get_to(p.field_4A_noise_wake_up_distance);
    j.at("slig_spawner_switch_id").get_to(p.field_4C_slig_spawner_switch_id);
    j.at("unlimited_spawns").get_to(p.field_4E_unlimited_spawns);
}

// Path_SligSpawner
void to_json(nlohmann::json& j, const Path_SligSpawner& p)
{
    j = nlohmann::json{
        {"base", ToBase(p)},
        {"scale", p.field_10_scale},
        {"start_state", p.field_12_start_state},
        {"pause_time", p.field_14_pause_time},
        {"pause_left_min", p.field_16_pause_left_min},
        {"pause_left_max", p.field_18_pause_left_max},
        {"pause_right_min", p.field_1A_pause_right_min},
        {"pause_right_max", p.field_1C_pause_right_max},
        {"shoot_possessed_sligs", p.field_1E_shoot_possessed_sligs},
        {"shoot_on_sight_delay", p.field_20_shoot_on_sight_delay},
        {"num_times_to_shoot", p.field_22_num_times_to_shoot},
        {"padding", p.field_24_padding},
        {"code_1", p.field_26_code1},
        {"code_2", p.field_28_code2},
        {"chase_abe_when_spotted", p.field_2A_chase_abe_when_spotted},
        {"start_Direction", p.field_2C_start_direction},
        {"panic_timeout", p.field_2E_panic_timeout},
        {"num_panic_sounds", p.field_30_num_panic_sounds},
        {"panic_sound_timeout", p.field_32_panic_sound_timeout},
        {"stop_chase_delay", p.field_34_stop_chase_delay},
        {"time_to_wait_before_chase", p.field_36_time_to_wait_before_chase},
        {"slig_bound_id", p.field_38_slig_bound_id},
        {"alerted_listen_time", p.field_3A_alerted_listen_time},
        {"percent_say_what", p.field_3C_percent_say_what},
        {"percent_beat_mud", p.field_3E_percent_beat_mud},
        {"talk_to_abe", p.field_40_talk_to_abe},
        {"dont_shoot", p.field_42_dont_shoot},
        {"z_shoot_delay", p.field_44_Z_shoot_delay},
        {"stay_awake", p.field_46_stay_awake},
        {"disabled_resources", p.field_48_disabled_resources},
        {"noise_wake_up_distance", p.field_4A_noise_wake_up_distance},
        {"slig_spawner_Switch_id", p.field_4C_slig_spawner_switch_id},
        {"unlimited_spawns", p.field_4E_unlimited_spawns},
    };
}

void from_json(const nlohmann::json& j, Path_SligSpawner& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.field_10_scale);
    j.at("start_state").get_to(p.field_12_start_state);
    j.at("pause_time").get_to(p.field_14_pause_time);
    j.at("pause_left_min").get_to(p.field_16_pause_left_min);
    j.at("pause_left_max").get_to(p.field_18_pause_left_max);
    j.at("pause_right_min").get_to(p.field_1A_pause_right_min);
    j.at("pause_right_max").get_to(p.field_1C_pause_right_max);
    j.at("shoot_possessed_sligs").get_to(p.field_1E_shoot_possessed_sligs);
    j.at("shoot_on_sight_delay").get_to(p.field_20_shoot_on_sight_delay);
    j.at("num_times_to_shoot").get_to(p.field_22_num_times_to_shoot);
    j.at("padding").get_to(p.field_24_padding);
    j.at("code_1").get_to(p.field_26_code1);
    j.at("code_2").get_to(p.field_28_code2);
    j.at("chase_abe_when_spotted").get_to(p.field_2A_chase_abe_when_spotted);
    j.at("start_direction").get_to(p.field_2C_start_direction);
    j.at("panic_timeout").get_to(p.field_2E_panic_timeout);
    j.at("num_panic_sounds").get_to(p.field_30_num_panic_sounds);
    j.at("panic_sound_timeout").get_to(p.field_32_panic_sound_timeout);
    j.at("stop_chase_delay").get_to(p.field_34_stop_chase_delay);
    j.at("time_to_wait_before_chase").get_to(p.field_36_time_to_wait_before_chase);
    j.at("slig_bound_id").get_to(p.field_38_slig_bound_id);
    j.at("alerted_listen_time").get_to(p.field_3A_alerted_listen_time);
    j.at("percent_say_what").get_to(p.field_3C_percent_say_what);
    j.at("percent_beat_mud").get_to(p.field_3E_percent_beat_mud);
    j.at("talk_to_abe").get_to(p.field_40_talk_to_abe);
    j.at("dont_shoot").get_to(p.field_42_dont_shoot);
    j.at("z_shoot_delay").get_to(p.field_44_Z_shoot_delay);
    j.at("stay_awake").get_to(p.field_46_stay_awake);
    j.at("disabled_resources").get_to(p.field_48_disabled_resources);
    j.at("noise_wake_up_distance").get_to(p.field_4A_noise_wake_up_distance);
    j.at("slig_spawner_switch_id").get_to(p.field_4C_slig_spawner_switch_id);
    j.at("unlimited_spawns").get_to(p.field_4E_unlimited_spawns);
}

} // namespace relive

