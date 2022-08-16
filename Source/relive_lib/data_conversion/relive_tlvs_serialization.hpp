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
        {"base", ToBase(p)}
    };
}

void from_json(const nlohmann::json& j, Path_MainMenuController& p)
{
    j.at("base").get_to(ToBase(p));
}

} // namespace relive

