#pragma once

#include "relive_tlvs.hpp"
#include "nlohmann/json.hpp"

// serialization support for each relive tlv type and any supporting nested types
static void to_json(nlohmann::json& j, const Guid& p)
{
    j = nlohmann::json{
        {"guid", p.ToString()},
    };
}

static void from_json(const nlohmann::json& j, Guid& p)
{
    // Pull the json value into the string
    std::string tmp;
    j.at("guid").get_to(tmp);

    // Make the guid from the string
    p = Guid::FromString(tmp);
}

NLOHMANN_JSON_SERIALIZE_ENUM(eLineTypes, {
    {eLineTypes::eFloor_0, "floor"},
    {eLineTypes::eWallLeft_1, "wall_left"},
    {eLineTypes::eWallRight_2, "wall_right"},
    {eLineTypes::eCeiling_3, "ceiling"},
    {eLineTypes::eBackgroundFloor_4, "background_floor"},
    {eLineTypes::eBackgroundWallLeft_5, "background_wall_left"},
    {eLineTypes::eBackgroundWallRight_6, "background_wall_right"},
    {eLineTypes::eBackgroundCeiling_7, "background_ceiling"},
    {eLineTypes::eTrackLine_8, "trackline"},
    {eLineTypes::eArt_9, "art"},
    {eLineTypes::eBulletWall_10, "bullet_wall"},
    {eLineTypes::eMineCarFloor_11, "minecar_floor"},
    {eLineTypes::eMineCarWall_12, "minecar_wall"},
    {eLineTypes::eMineCarCeiling_13, "minecar_ceiling"},
    {eLineTypes::eBackgroundMineCarFloor_14, "background_minecar_floor"},
    {eLineTypes::eBackgroundMineCarWall_15, "background_minecar_wall"},
    {eLineTypes::eBackgroundMineCarCeiling_16, "background_minecar_ceiling"},
    {eLineTypes::eFlyingObjectWall_17, "flying_slig_ceiling"},
    {eLineTypes::eBackgroundFlyingObjectWall_18, "background_flying_slig_ceiling"},
    {eLineTypes::eDynamicCollision_32, "dynamic_collision"},
    {eLineTypes::eBackgroundDynamicCollision_36, "background_dynamic_collision"},
})

inline void to_json(nlohmann::json& j, const PathLine& p)
{
    j = nlohmann::json{
        {"x", p.mRect.x},
        {"y", p.mRect.y},
        {"w", p.mRect.w},
        {"h", p.mRect.h},
        {"line_type", p.mLineType},
        {"previous", p.field_A_previous},
        {"next", p.field_C_next},
        {"line_length", p.field_12_line_length},
    };
}

inline void from_json(const nlohmann::json& j, PathLine& p)
{
    j.at("x").get_to(p.mRect.x);
    j.at("y").get_to(p.mRect.y);
    j.at("w").get_to(p.mRect.w);
    j.at("h").get_to(p.mRect.h);
    j.at("line_type").get_to(p.mLineType);
    j.at("previous").get_to(p.field_A_previous);
    j.at("next").get_to(p.field_C_next);
    j.at("line_length").get_to(p.field_12_line_length);
}

// RGB16
inline void to_json(nlohmann::json& j, const RGB16& p)
{
    j = nlohmann::json{
        {"r", p.r},
        {"g", p.g},
        {"b", p.b}};
}

inline void from_json(const nlohmann::json& j, RGB16& p)
{
    j.at("r").get_to(p.r);
    j.at("g").get_to(p.g);
    j.at("b").get_to(p.b);
}

NLOHMANN_JSON_SERIALIZE_ENUM(TPageAbr, {
    {TPageAbr::eBlend_0, "blend_0"},
    {TPageAbr::eBlend_1, "blend_1"},
    {TPageAbr::eBlend_2, "blend_2"},
    {TPageAbr::eBlend_3, "blend_3"},
})

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
inline void to_json(nlohmann::json& j, const Path_TLV& p)
{
    j = nlohmann::json{
        {"top_left_x", p.mTopLeftX},
        {"top_left_y", p.mTopLeftY},
        {"bottom_right_x", p.mBottomRightX},
        {"bottom_right_y", p.mBottomRightY},
        {"tlv_specific_meaning", p.mTlvSpecificMeaning},
        {"tlv_flags", p.mTlvFlags.Raw().all},
        {"length", p.mLength},
        {"id", p.mId},
    };
}

inline void from_json(const nlohmann::json& j, Path_TLV& p)
{
    j.at("top_left_x").get_to(p.mTopLeftX);
    j.at("top_left_y").get_to(p.mTopLeftY);
    j.at("bottom_right_x").get_to(p.mBottomRightX);
    j.at("bottom_right_y").get_to(p.mBottomRightY);
    j.at("tlv_specific_meaning").get_to(p.mTlvSpecificMeaning);
    j.at("tlv_flags").get_to(p.mTlvFlags.Raw().all);
    j.at("length").get_to(p.mLength);
    j.at("id").get_to(p.mId);
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

NLOHMANN_JSON_SERIALIZE_ENUM(reliveScreenChangeEffects, {
    {reliveScreenChangeEffects::ePlay1FMV, "play_1_fmv"},
    {reliveScreenChangeEffects::eRightToLeft, "right_to_left"},
    {reliveScreenChangeEffects::eLeftToRight, "left_to_right"},
    {reliveScreenChangeEffects::eBottomToTop, "bottom_to_top"},
    {reliveScreenChangeEffects::eTopToBottom, "top_to_bottom"},
    {reliveScreenChangeEffects::eBoxOut, "box_out"},
    {reliveScreenChangeEffects::eVerticalSplit, "vertical_split"},
    {reliveScreenChangeEffects::eHorizontalSplit, "horizontal_split"},
    {reliveScreenChangeEffects::eUnknown_8, "unknown_8"},
    {reliveScreenChangeEffects::eInstantChange, "instant_change"},
})

// Path_ShadowZone
NLOHMANN_JSON_SERIALIZE_ENUM(Path_ShadowZone::Scale, {
    {Path_ShadowZone::Scale::eBoth, "both"},
    {Path_ShadowZone::Scale::eFull, "full"},
    {Path_ShadowZone::Scale::eHalf, "half"},
})

inline void to_json(nlohmann::json& j, const Path_ShadowZone& p)
{
    j = nlohmann::json{
        {"tlv_type", "shadow_zone"},
        {"base", ToBase(p)},
        {"rgb", p.mRGB},
        {"scale", p.mScale},
    };
}

inline void from_json(const nlohmann::json& j, Path_ShadowZone& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("rgb").get_to(p.mRGB);
    j.at("scale").get_to(p.mScale);
}

// Path_SecurityOrb
inline void to_json(nlohmann::json& j, const Path_SecurityOrb& p)
{
    j = nlohmann::json{
        {"tlv_type", "security_orb"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"disabled_resources", p.mDisabledResources},
    };
}

inline void from_json(const nlohmann::json& j, Path_SecurityOrb& p)
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

inline void to_json(nlohmann::json& j, const Path_ContinuePoint& p)
{
    j = nlohmann::json{
        {"tlv_type", "continue_point"},
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

inline void from_json(const nlohmann::json& j, Path_ContinuePoint& p)
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

inline void to_json(nlohmann::json& j, const Path_LiftPoint& p)
{
    j = nlohmann::json{
        {"tlv_type", "lift_point"},
        {"base", ToBase(p)},
        {"lift_point_id", p.mLiftPointId},
        {"is_start_point", p.mIsStartPoint},
        {"lift_point_stop_type", p.mLiftPointStopType},
        {"scale", p.mScale},
        {"ignore_lift_mover", p.mIgnoreLiftMover},
    };
}

inline void from_json(const nlohmann::json& j, Path_LiftPoint& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("lift_point_id").get_to(p.mLiftPointId);
    j.at("is_start_point").get_to(p.mIsStartPoint);
    j.at("lift_point_stop_type").get_to(p.mLiftPointStopType);
    j.at("scale").get_to(p.mScale);
    j.at("ignore_lift_mover").get_to(p.mIgnoreLiftMover);
}

// Path_Dove
inline void to_json(nlohmann::json& j, const Path_Dove& p)
{
    j = nlohmann::json{
        {"tlv_type", "dove"},
        {"base", ToBase(p)},
        {"dove_count", p.mDoveCount},
        {"pixel_perfect", p.mPixelPerfect},
        {"scale", p.mScale},
    };
}

inline void from_json(const nlohmann::json& j, Path_Dove& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("dove_count").get_to(p.mDoveCount);
    j.at("pixel_perfect").get_to(p.mPixelPerfect);
    j.at("scale").get_to(p.mScale);
}

// Path_RockSack
inline void to_json(nlohmann::json& j, const Path_RockSack& p)
{
    j = nlohmann::json{
        {"tlv_type", "rock_sack"},
        {"base", ToBase(p)},
        {"rock_fall_direction", p.mRockFallDirection},
        {"vel_x", p.mVelX},
        {"vel_y", p.mVelY},
        {"scale", p.mScale},
        {"rock_amount", p.mRockAmount},
    };
}

inline void from_json(const nlohmann::json& j, Path_RockSack& p)
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

inline void to_json(nlohmann::json& j, const Path_ZBall& p)
{
    j = nlohmann::json{
        {"tlv_type", "z_ball"},
        {"base", ToBase(p)},
        {"start_pos", p.mStartPos},
        {"scale", p.mScale},
        {"speed", p.mSpeed},
    };
}

inline void from_json(const nlohmann::json& j, Path_ZBall& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("start_pos").get_to(p.mStartPos);
    j.at("scale").get_to(p.mScale);
    j.at("speed").get_to(p.mSpeed);
}

// Path_FallingItem
inline void to_json(nlohmann::json& j, const Path_FallingItem& p)
{
    j = nlohmann::json{
        {"tlv_type", "falling_item"},
        {"base", ToBase(p)},
        {"switch_id", p.mSwitchId},
        {"scale", p.mScale},
        {"fall_interval", p.mFallInterval},
        {"max_falling_items", p.mMaxFallingItems},
        {"reset_switch_id_after_use", p.mResetSwitchIdAfterUse},
    };
}

inline void from_json(const nlohmann::json& j, Path_FallingItem& p)
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
    {Path_PullRingRope::PullRingSwitchSound::eRingBellHammer, "ring_bell_hammer"},
    {Path_PullRingRope::PullRingSwitchSound::eDoorEffect, "door_effect"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(Path_PullRingRope::PullRingSoundDirection, {
    {Path_PullRingRope::PullRingSoundDirection::eLeftAndRight, "left_and_right"},
    {Path_PullRingRope::PullRingSoundDirection::eLeft, "left"},
    {Path_PullRingRope::PullRingSoundDirection::eRight, "right"},
})

inline void to_json(nlohmann::json& j, const Path_PullRingRope& p)
{
    j = nlohmann::json{
        {"tlv_type", "pull_ring_rope"},
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

inline void from_json(const nlohmann::json& j, Path_PullRingRope& p)
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
inline void to_json(nlohmann::json& j, const Path_TimedMine& p)
{
    j = nlohmann::json{
        {"tlv_type", "timed_mine"},
        {"base", ToBase(p)},
        {"switch_id", p.mSwitchId},
        {"state", p.mState},
        {"scale", p.mScale},
        {"ticks_until_explosion", p.mTicksUntilExplosion},
        {"disabled_resources", p.mDisabledResources},
    };
}

inline void from_json(const nlohmann::json& j, Path_TimedMine& p)
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

inline void to_json(nlohmann::json& j, const Path_Hoist& p)
{
    j = nlohmann::json{
        {"tlv_type", "hoist"},
        {"base", ToBase(p)},
        {"hoist_type", p.mHoistType},
        {"grab_direction", p.mGrabDirection},
        {"scale", p.mScale},
    };
}

inline void from_json(const nlohmann::json& j, Path_Hoist& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("hoist_type").get_to(p.mHoistType);
    j.at("grab_direction").get_to(p.mGrabDirection);
    j.at("scale").get_to(p.mScale);
}

// Path_TrapDoor
NLOHMANN_JSON_SERIALIZE_ENUM(Path_TrapDoor::StartState, {
    {Path_TrapDoor::StartState::eOpen, "open"},
    {Path_TrapDoor::StartState::eClosed, "closed"},
})

inline void to_json(nlohmann::json& j, const Path_TrapDoor& p)
{
    j = nlohmann::json{
        {"tlv_type", "trap_door"},
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

inline void from_json(const nlohmann::json& j, Path_TrapDoor& p)
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
inline void to_json(nlohmann::json& j, const Path_LCDScreen& p)
{
    j = nlohmann::json{
        {"tlv_type", "lcd_screen"},
        {"base", ToBase(p)},
        {"message_id_1", p.mMessageId1},
        {"message_rand_min_id", p.mMessageRandMinId},
        {"message_rand_max_id", p.mMessageRandMaxId},
        {"message_id_2", p.mMessageId2},
        {"toggle_message_switch_id", p.mToggleMessageSwitchId},
    };
}

inline void from_json(const nlohmann::json& j, Path_LCDScreen& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("message_id_1").get_to(p.mMessageId1);
    j.at("message_rand_min_id").get_to(p.mMessageRandMinId);
    j.at("message_rand_max_id").get_to(p.mMessageRandMaxId);
    j.at("message_id_2").get_to(p.mMessageId2);
    j.at("toggle_message_switch_id").get_to(p.mToggleMessageSwitchId);
}

// Path_Mine
inline void to_json(nlohmann::json& j, const Path_Mine& p)
{
    j = nlohmann::json{
        {"tlv_type", "mine"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"persist_offscreen", p.mPersistOffscreen},
    };
}

inline void from_json(const nlohmann::json& j, Path_Mine& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("persist_offscreen").get_to(p.mPersistOffscreen);
}

// Path_InvisibleSwitch
NLOHMANN_JSON_SERIALIZE_ENUM(Path_InvisibleSwitch::InvisibleSwitchScale, {
    {Path_InvisibleSwitch::InvisibleSwitchScale::eHalf, "half"},
    {Path_InvisibleSwitch::InvisibleSwitchScale::eFull, "full"},
    {Path_InvisibleSwitch::InvisibleSwitchScale::eAny, "any"},
})

inline void to_json(nlohmann::json& j, const Path_InvisibleSwitch& p)
{
    j = nlohmann::json{
        {"tlv_type", "invisible_switch"},
        {"base", ToBase(p)},
        {"switch_id", p.mSwitchId},
        {"action", p.mAction},
        {"activation_delay", p.mActivationDelay},
        {"set_off_alarm", p.mSetOffAlarm},
        {"scale", p.mScale},
    };
}

inline void from_json(const nlohmann::json& j, Path_InvisibleSwitch& p)
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

inline void to_json(nlohmann::json& j, const Path_ElectricWall& p)
{
    j = nlohmann::json{
        {"tlv_type", "electric_wall"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"switch_id", p.mSwitchId},
        {"start_state", p.mStartState},
    };
}

inline void from_json(const nlohmann::json& j, Path_ElectricWall& p)
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

inline void to_json(nlohmann::json& j, const Path_BoomMachine& p)
{
    j = nlohmann::json{
        {"tlv_type", "boom_machine"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"nozzle_side", p.mNozzleSide},
        {"disabled_resources", p.mDisabledResources},
        {"grenade_amount", p.mGrenadeAmount},
    };
}

inline void from_json(const nlohmann::json& j, Path_BoomMachine& p)
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

inline void to_json(nlohmann::json& j, const Path_UXB& p)
{
    j = nlohmann::json{
        {"tlv_type", "uxb"},
        {"base", ToBase(p)},
        {"pattern_length", p.mPatternLength},
        {"pattern", p.mPattern},
        {"scale", p.mScale},
        {"start_state", p.mStartState}
    };
}

inline void from_json(const nlohmann::json& j, Path_UXB& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("pattern_length").get_to(p.mPatternLength);
    j.at("pattern").get_to(p.mPattern);
    j.at("scale").get_to(p.mScale);
    j.at("start_state").get_to(p.mStartState);
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

inline void to_json(nlohmann::json& j, const Path_MeatSaw& p)
{
    j = nlohmann::json{
        {"tlv_type", "meat_saw"},
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

inline void from_json(const nlohmann::json& j, Path_MeatSaw& p)
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

inline void to_json(nlohmann::json& j, const Path_Lever& p)
{
    j = nlohmann::json{
        {"tlv_type", "lever"},
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

inline void from_json(const nlohmann::json& j, Path_Lever& p)
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

inline void to_json(nlohmann::json& j, const Path_Edge& p)
{
    j = nlohmann::json{
        {"tlv_type", "edge"},
        {"base", ToBase(p)},
        {"grab_direction", p.mGrabDirection},
        {"can_grab", p.mCanGrab},
        {"scale", p.mScale},
    };
}

inline void from_json(const nlohmann::json& j, Path_Edge& p)
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

inline void to_json(nlohmann::json& j, const Path_BirdPortal& p)
{
    j = nlohmann::json{
        {"tlv_type", "bird_portal"},
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

inline void from_json(const nlohmann::json& j, Path_BirdPortal& p)
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
inline void to_json(nlohmann::json& j, const Path_BirdPortalExit& p)
{
    j = nlohmann::json{
        {"tlv_type", "bird_portal_exit"},
        {"base", ToBase(p)},
        {"exit_side", p.mExitSide},
        {"scale", p.mScale},
    };
}

inline void from_json(const nlohmann::json& j, Path_BirdPortalExit& p)
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

inline void to_json(nlohmann::json& j, const Path_LightEffect& p)
{
    j = nlohmann::json{
        {"tlv_type", "light_effect"},
        {"base", ToBase(p)},
        {"type", p.mType},
        {"size", p.mSize},
        {"switch_id", p.mSwitchId},
        {"direction", p.mDirection},
    };
}

inline void from_json(const nlohmann::json& j, Path_LightEffect& p)
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

inline void to_json(nlohmann::json& j, const Path_MusicTrigger& p)
{
    j = nlohmann::json{
        {"tlv_type", "music_trigger"},
        {"base", ToBase(p)},
        {"music_type", p.mMusicType},
        {"triggered_by", p.mTriggeredBy},
        {"switch_id", p.mSwitchId},
        {"music_delay", p.mMusicDelay},
    };
}

inline void from_json(const nlohmann::json& j, Path_MusicTrigger& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("music_type").get_to(p.mMusicType);
    j.at("triggered_by").get_to(p.mTriggeredBy);
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("music_delay").get_to(p.mMusicDelay);
}

// Path_SoftLanding
inline void to_json(nlohmann::json& j, const Path_SoftLanding& p)
{
    j = nlohmann::json{
        {"tlv_type", "soft_landing"},
        {"base", ToBase(p)},
        {"switch_id", p.mSwitchId},
    };
}

inline void from_json(const nlohmann::json& j, Path_SoftLanding& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("switch_id").get_to(p.mSwitchId);
}

// Path_LiftMover
NLOHMANN_JSON_SERIALIZE_ENUM(Path_LiftMover::YDirection, {
    {Path_LiftMover::YDirection::eDown, "down"},
    {Path_LiftMover::YDirection::eUp, "up"},
})

inline void to_json(nlohmann::json& j, const Path_LiftMover& p)
{
    j = nlohmann::json{
        {"tlv_type", "lift_mover"},
        {"base", ToBase(p)},
        {"lift_mover_switch_id", p.mLiftMoverSwitchId},
        {"target_lift_point_id", p.mTargetLiftPointId},
        {"move_direction", p.mMoveDirection},
    };
}

inline void from_json(const nlohmann::json& j, Path_LiftMover& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("lift_mover_switch_id").get_to(p.mLiftMoverSwitchId);
    j.at("target_lift_point_id").get_to(p.mTargetLiftPointId);
    j.at("move_direction").get_to(p.mMoveDirection);
}

// Path_Hintfly
inline void to_json(nlohmann::json& j, const Path_HintFly& p)
{
    j = nlohmann::json{
        {"tlv_type", "hint_fly"},
        {"base", ToBase(p)},
        {"message_id", p.mMessageId},
    };
}

inline void from_json(const nlohmann::json& j, Path_HintFly& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("message_id").get_to(p.mMessageId);
}

// Path_TimerTrigger
inline void to_json(nlohmann::json& j, const Path_TimerTrigger& p)
{
    j = nlohmann::json{
        {"tlv_type", "timer_trigger"},
        {"base", ToBase(p)},
        {"input_switch_id", p.mInputSwitchId},
        {"activation_delay", p.mActivationDelay},
        {"output_switch_id1", p.mOutputSwitchId1},
        {"output_switch_id2", p.mOutputSwitchId2},
        {"output_switch_id3", p.mOutputSwitchId3},
        {"output_switch_id4", p.mOutputSwitchId4},
    };
}

inline void from_json(const nlohmann::json& j, Path_TimerTrigger& p)
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
inline void to_json(nlohmann::json& j, const Path_FlintLockFire& p)
{
    j = nlohmann::json{
        {"tlv_type", "flint_lock_fire"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"switch_id", p.mSwitchId},
    };
}

inline void from_json(const nlohmann::json& j, Path_FlintLockFire& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("switch_id").get_to(p.mSwitchId);
}

// Path_HoneySack
inline void to_json(nlohmann::json& j, const Path_HoneySack& p)
{
    j = nlohmann::json{
        {"tlv_type", "honey_sack"},
        {"base", ToBase(p)},
        {"chase_time", p.mChaseTime},
        {"scale", p.mScale},
    };
}

inline void from_json(const nlohmann::json& j, Path_HoneySack& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("chase_time").get_to(p.mChaseTime);
    j.at("scale").get_to(p.mScale);
}

// Path_Bat
inline void to_json(nlohmann::json& j, const Path_Bat& p)
{
    j = nlohmann::json{
        {"tlv_type", "bat"},
        {"base", ToBase(p)},
        {"time_before_moving", p.mTimeBeforeMoving},
        {"speed", p.mSpeed},
        {"scale", p.mScale},
        {"attack_duration", p.mAttackDuration},
    };
}

inline void from_json(const nlohmann::json& j, Path_Bat& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("time_before_moving").get_to(p.mTimeBeforeMoving);
    j.at("speed").get_to(p.mSpeed);
    j.at("scale").get_to(p.mScale);
    j.at("attack_duration").get_to(p.mAttackDuration);
}

// Path_RollingBallStopper
inline void to_json(nlohmann::json& j, const Path_RollingBallStopper& p)
{
    j = nlohmann::json{
        {"tlv_type", "rolling_ball_stopper"},
        {"base", ToBase(p)},
        {"stopper_switch_id", p.mStopperSwitchId},
        {"scale", p.mScale},
        {"ball_switch_id", p.mBallSwitchId},
        {"stop_direction", p.mStopDirection},
    };
}

inline void from_json(const nlohmann::json& j, Path_RollingBallStopper& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("stopper_switch_id").get_to(p.mStopperSwitchId);
    j.at("scale").get_to(p.mScale);
    j.at("ball_switch_id").get_to(p.mBallSwitchId);
    j.at("stop_direction").get_to(p.mStopDirection);
}

// Path_RollingBall
inline void to_json(nlohmann::json& j, const Path_RollingBall& p)
{
    j = nlohmann::json{
        {"tlv_type", "rolling_ball"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"roll_direction", p.mRollDirection},
        {"release_switch_id", p.mReleaseSwitchId},
        {"max_speed", p.mMaxSpeed},
        {"acceleration", p.mAcceleration},
    };
}

inline void from_json(const nlohmann::json& j, Path_RollingBall& p)
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

inline void to_json(nlohmann::json& j, const Path_MotionDetector& p)
{
    j = nlohmann::json{
        {"tlv_type", "motion_detector"},
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

inline void from_json(const nlohmann::json& j, Path_MotionDetector& p)
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
inline void to_json(nlohmann::json& j, const Path_BellHammer& p)
{
    j = nlohmann::json{
        {"tlv_type", "bell_hammer"},
        {"base", ToBase(p)},
        {"switch_id", p.mSwitchId},
        {"action", p.mAction},
        {"scale", p.mScale},
        {"direction", p.mDirection},
    };
}

inline void from_json(const nlohmann::json& j, Path_BellHammer& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("action").get_to(p.mAction);
    j.at("scale").get_to(p.mScale);
    j.at("direction").get_to(p.mDirection);
}

// Path_SligBoundLeft
inline void to_json(nlohmann::json& j, const Path_SligBoundLeft& p)
{
    j = nlohmann::json{
        {"tlv_type", "slig_bound_left"},
        {"base", ToBase(p)},
        {"slig_bound_id", p.mSligBoundId},
        {"disabled_resources_ao", p.mDisabledResourcesAO.Raw().all},
        {"disabled_resources_ae", p.mDisabledResourcesAE},
    };
}

inline void from_json(const nlohmann::json& j, Path_SligBoundLeft& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("slig_bound_id").get_to(p.mSligBoundId);
    j.at("disabled_resources_ao").get_to(p.mDisabledResourcesAO.Raw().all); // TODO: prob wrong
    j.at("disabled_resources_ae").get_to(p.mDisabledResourcesAE);
}

// Path_SligBoundRight
inline void to_json(nlohmann::json& j, const Path_SligBoundRight& p)
{
    j = nlohmann::json{
        {"tlv_type", "slig_bound_right"},
        {"base", ToBase(p)},
        {"slig_bound_id", p.mSligBoundId},
        {"disabled_resources_ao", p.mDisabledResourcesAO.Raw().all},
        {"disabled_resources_ae", p.mDisabledResourcesAE},
    };
}

inline void from_json(const nlohmann::json& j, Path_SligBoundRight& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("slig_bound_id").get_to(p.mSligBoundId);
    j.at("disabled_resources_ao").get_to(p.mDisabledResourcesAO.Raw().all); // TODO: prob wrong
    j.at("disabled_resources_ae").get_to(p.mDisabledResourcesAE);
}

// Path_SligPersist
inline void to_json(nlohmann::json& j, const Path_SligPersist& p)
{
    j = nlohmann::json{
        {"tlv_type", "slig_persist"},
        {"base", ToBase(p)},
        {"slig_bound_id", p.mSligBoundId},
        {"disabled_resources_ao", p.mDisabledResourcesAO.Raw().all},
        {"disabled_resources_ae", p.mDisabledResourcesAE},
    };
}

inline void from_json(const nlohmann::json& j, Path_SligPersist& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("slig_bound_id").get_to(p.mSligBoundId);
    j.at("disabled_resources_ao").get_to(p.mDisabledResourcesAO.Raw().all); // TODO: prob wrong
    j.at("disabled_resources_ae").get_to(p.mDisabledResourcesAE);
}

// Path_BackgroundAnimation
NLOHMANN_JSON_SERIALIZE_ENUM(Path_BackgroundAnimation::BgAnimSounds, {
    {Path_BackgroundAnimation::BgAnimSounds::eNone, "none"},
    {Path_BackgroundAnimation::BgAnimSounds::eFire, "fire"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(Path_BackgroundAnimation::Layer, {
    {Path_BackgroundAnimation::Layer::eLayer0, "layer_0"},
    {Path_BackgroundAnimation::Layer::eLayer2, "layer_2"},
})

inline void to_json(nlohmann::json& j, const Path_BackgroundAnimation& p)
{
    j = nlohmann::json{
        {"tlv_type", "background_animation"},
        {"base", ToBase(p)},
        {"anim_id", p.mAnimId},
        {"is_semi_trans", p.mIsSemiTrans},
        {"semi_trans_mode", p.mSemiTransMode},
        {"sound_effect", p.mSoundEffect},
        {"layer", p.mLayer},
    };
}

inline void from_json(const nlohmann::json& j, Path_BackgroundAnimation& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("anim_id").get_to(p.mAnimId);
    j.at("is_semi_trans").get_to(p.mIsSemiTrans);
    j.at("semi_trans_mode").get_to(p.mSemiTransMode);
    j.at("sound_effect").get_to(p.mSoundEffect);
    j.at("layer").get_to(p.mLayer);
}

// Path_MainMenuController
inline void to_json(nlohmann::json& j, const Path_MainMenuController& p)
{
    j = nlohmann::json{
        {"tlv_type", "main_menu_controller"},
        {"base", ToBase(p)}
    };
}

inline void from_json(const nlohmann::json& j, Path_MainMenuController& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_ElumWall
inline void to_json(nlohmann::json& j, const Path_ElumWall& p)
{
    j = nlohmann::json{
        {"tlv_type", "elum_wall"},
        {"base", ToBase(p)}
    };
}

inline void from_json(const nlohmann::json& j, Path_ElumWall& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_ElumStart
inline void to_json(nlohmann::json& j, const Path_ElumStart& p)
{
    j = nlohmann::json{
        {"tlv_type", "elum_start"},
        {"base", ToBase(p)}
    };
}

inline void from_json(const nlohmann::json& j, Path_ElumStart& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_KillUnsavedMuds
inline void to_json(nlohmann::json& j, const Path_KillUnsavedMuds& p)
{
    j = nlohmann::json{
        {"tlv_type", "kill_unsaved_muds"},
        {"base", ToBase(p)}
    };
}

inline void from_json(const nlohmann::json& j, Path_KillUnsavedMuds& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_InvisibleZone
inline void to_json(nlohmann::json& j, const Path_InvisibleZone& p)
{
    j = nlohmann::json{
        {"tlv_type", "invisible_zone"},
        {"base", ToBase(p)}
    };
}

inline void from_json(const nlohmann::json& j, Path_InvisibleZone& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_StartController
inline void to_json(nlohmann::json& j, const Path_StartController& p)
{
    j = nlohmann::json{
        {"tlv_type", "start_controller"},
        {"base", ToBase(p)}
    };
}

inline void from_json(const nlohmann::json& j, Path_StartController& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_ScrabNoFall
inline void to_json(nlohmann::json& j, const Path_ScrabNoFall& p)
{
    j = nlohmann::json{
        {"tlv_type", "scrab_no_fall"},
        {"base", ToBase(p)}
    };
}

inline void from_json(const nlohmann::json& j, Path_ScrabNoFall& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_ScrabBoundLeft
inline void to_json(nlohmann::json& j, const Path_ScrabBoundLeft& p)
{
    j = nlohmann::json{
        {"tlv_type", "scrab_bound_left"},
        {"base", ToBase(p)}
    };
}

inline void from_json(const nlohmann::json& j, Path_ScrabBoundLeft& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_ScrabBoundRight
inline void to_json(nlohmann::json& j, const Path_ScrabBoundRight& p)
{
    j = nlohmann::json{
        {"tlv_type", "scrab_bound_right"},
        {"base", ToBase(p)}
    };
}

inline void from_json(const nlohmann::json& j, Path_ScrabBoundRight& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_HoneyDripTarget
inline void to_json(nlohmann::json& j, const Path_HoneyDripTarget& p)
{
    j = nlohmann::json{
        {"tlv_type", "honey_drip_target"},
        {"base", ToBase(p)}
    };
}

inline void from_json(const nlohmann::json& j, Path_HoneyDripTarget& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_ZSligCover
inline void to_json(nlohmann::json& j, const Path_ZSligCover& p)
{
    j = nlohmann::json{
        {"tlv_type", "z_slig_cover"},
        {"base", ToBase(p)}
    };
}

inline void from_json(const nlohmann::json& j, Path_ZSligCover& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_DeathDrop
inline void to_json(nlohmann::json& j, const Path_DeathDrop& p)
{
    j = nlohmann::json{
        {"tlv_type", "death_drop"},
        {"base", ToBase(p)}
    };
}

inline void from_json(const nlohmann::json& j, Path_DeathDrop& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_ChimeLock
inline void to_json(nlohmann::json& j, const Path_ChimeLock& p)
{
    j = nlohmann::json{
        {"tlv_type", "chime_lock"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"solve_switch_id", p.mSolveSwitchId},
        {"code1", p.mCode1},
        {"code2", p.mCode2},
        {"password_switch_id", p.mPasswordSwitchId},
    };
}

inline void from_json(const nlohmann::json& j, Path_ChimeLock& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("solve_switch_id").get_to(p.mSolveSwitchId);
    j.at("code1").get_to(p.mCode1);
    j.at("code2").get_to(p.mCode2);
    j.at("password_switch_id").get_to(p.mPasswordSwitchId);
}

// Path_LCDStatusBoard
inline void to_json(nlohmann::json& j, const Path_LCDStatusBoard& p)
{
    j = nlohmann::json{
        {"tlv_type", "lcd_status_board"},
        {"base", ToBase(p)},
        {"number_of_muds", p.mNumberOfMuds},
        {"zulag_number", p.mZulagNumber},
        {"hide_board", p.mHideBoard},
    };
}

inline void from_json(const nlohmann::json& j, Path_LCDStatusBoard& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("number_of_muds").get_to(p.mNumberOfMuds);
    j.at("zulag_number").get_to(p.mZulagNumber);
    j.at("hide_board").get_to(p.mHideBoard);
}

// Path_CreditsController
inline void to_json(nlohmann::json& j, const Path_CreditsController& p)
{
    j = nlohmann::json{
        {"tlv_type", "credits_controller"},
        {"base", ToBase(p)}
    };
}

inline void from_json(const nlohmann::json& j, Path_CreditsController& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_ResetPath
inline void to_json(nlohmann::json& j, const Path_ResetPath& p)
{
    j = nlohmann::json{
        {"tlv_type", "reset_path"},
        {"base", ToBase(p)},
        {"clear_ids", p.mClearIds},
        {"from", p.mFrom},
        {"to", p.mTo},
        {"exclude", p.mExclude},
        {"clear_objects", p.mClearObjects},
        {"path", p.mPath},
        {"enabled", p.mEnabled},
    };
}

inline void from_json(const nlohmann::json& j, Path_ResetPath& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("clear_ids").get_to(p.mClearIds);
    j.at("from").get_to(p.mFrom);
    j.at("to").get_to(p.mTo);
    j.at("exclude").get_to(p.mExclude);
    j.at("clear_objects").get_to(p.mClearObjects);
    j.at("path").get_to(p.mPath);
    j.at("enabled").get_to(p.mEnabled);
}

// Path_MeatSack
inline void to_json(nlohmann::json& j, const Path_MeatSack& p)
{
    j = nlohmann::json{
        {"tlv_type", "meat_sack"},
        {"base", ToBase(p)},
        {"meat_fall_direction", p.mMeatFallDirection},
        {"vel_x", p.mVelX},
        {"vel_y", p.mVelY},
        {"scale", p.mScale},
        {"meat_amount", p.mMeatAmount},
    };
}

inline void from_json(const nlohmann::json& j, Path_MeatSack& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("meat_fall_direction").get_to(p.mMeatFallDirection);
    j.at("vel_x").get_to(p.mVelX);
    j.at("vel_y").get_to(p.mVelY);
    j.at("scale").get_to(p.mScale);
    j.at("meat_amount").get_to(p.mMeatAmount);
}

// Path_BeeNest
inline void to_json(nlohmann::json& j, const Path_BeeNest& p)
{
    j = nlohmann::json{
        {"tlv_type", "bee_nest"},
        {"base", ToBase(p)},
        {"switch_id", p.mSwitchId},
        {"swarm_size", p.mSwarmSize},
        {"chase_time", p.mChaseTime},
        {"speed", p.mSpeed},
        {"disabled_resources", p.mDisabledResources},
        {"bees_amount", p.mBeesAmount},
    };
}

inline void from_json(const nlohmann::json& j, Path_BeeNest& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("swarm_size").get_to(p.mSwarmSize);
    j.at("chase_time").get_to(p.mChaseTime);
    j.at("speed").get_to(p.mSpeed);
    j.at("disabled_resources").get_to(p.mDisabledResources);
    j.at("bees_amount").get_to(p.mBeesAmount);
}

// Path_BellsongStone
NLOHMANN_JSON_SERIALIZE_ENUM(Path_BellsongStone::BellsongTypes, {
    {Path_BellsongStone::BellsongTypes::eWhistle, "whistle"},
    {Path_BellsongStone::BellsongTypes::eChimes, "chimes"},
})

inline void to_json(nlohmann::json& j, const Path_BellsongStone& p)
{
    j = nlohmann::json{
        {"tlv_type", "bellsong_stone"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"type", p.mType},
        {"code1", p.mCode1},
        {"code2", p.mCode2},
        {"switch_id", p.mSwitchId},
    };
}

inline void from_json(const nlohmann::json& j, Path_BellsongStone& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("type").get_to(p.mType);
    j.at("code1").get_to(p.mCode1);
    j.at("code2").get_to(p.mCode2);
    j.at("switch_id").get_to(p.mSwitchId);
}

// Path_MovieStone
inline void to_json(nlohmann::json& j, const Path_MovieStone& p)
{
    j = nlohmann::json{
        {"tlv_type", "movie_stone"},
        {"base", ToBase(p)},
        {"movie_id", p.mMovieId},
        {"scale", p.mScale},
        {"trigger_switch_id", p.mTriggerSwitchId},
    };
}

inline void from_json(const nlohmann::json& j, Path_MovieStone& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("movie_id").get_to(p.mMovieId);
    j.at("scale").get_to(p.mScale);
    j.at("trigger_switch_id").get_to(p.mTriggerSwitchId);
}

// Path_HandStone
inline void to_json(nlohmann::json& j, const Path_HandStone& p)
{
    j = nlohmann::json{
        {"tlv_type", "hand_stone"},
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

inline void from_json(const nlohmann::json& j, Path_HandStone& p)
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
inline void to_json(nlohmann::json& j, const Path_PathTransition& p)
{
    j = nlohmann::json{
        {"tlv_type", "path_transition"},
        {"base", ToBase(p)},
        {"next_level", p.mNextLevel},
        {"next_path", p.mNextPath},
        {"next_camera", p.mNextCamera},
        {"movie_id", p.mMovieId},
        {"wipe_effect", p.mWipeEffect},
        {"next_path_scale", p.mNextPathScale},
    };
}

inline void from_json(const nlohmann::json& j, Path_PathTransition& p)
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
inline void to_json(nlohmann::json& j, const Path_Pulley& p)
{
    j = nlohmann::json{
        {"tlv_type", "pulley"},
        {"base", ToBase(p)},
    };
}

inline void from_json(const nlohmann::json& j, Path_Pulley& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_Honey
inline void to_json(nlohmann::json& j, const Path_Honey& p)
{
    j = nlohmann::json{
        {"tlv_type", "honey"},
        {"base", ToBase(p)},
    };
}

inline void from_json(const nlohmann::json& j, Path_Honey& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_BeeSwarmHole
NLOHMANN_JSON_SERIALIZE_ENUM(Path_BeeSwarmHole::MovementType, {
    {Path_BeeSwarmHole::MovementType::eHover, "hover"},
    {Path_BeeSwarmHole::MovementType::eAttack, "attack"},
    {Path_BeeSwarmHole::MovementType::eFollowPath, "follow_path"},
})

inline void to_json(nlohmann::json& j, const Path_BeeSwarmHole& p)
{
    j = nlohmann::json{
        {"tlv_type", "bee_swarm_hole"},
        {"base", ToBase(p)},
        {"start_interval", p.mStartInterval},
        {"movement_type", p.mMovementType},
        {"bees_amount", p.mBeesAmount},
        {"chase_time", p.mChaseTime},
        {"speed", p.mSpeed},
    };
}

inline void from_json(const nlohmann::json& j, Path_BeeSwarmHole& p)
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

inline void to_json(nlohmann::json& j, const Path_Door& p)
{
    j = nlohmann::json{
        {"tlv_type", "door"},
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
        {"door_closed", p.mDoorClosed},
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

inline void from_json(const nlohmann::json& j, Path_Door& p)
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
    j.at("door_closed").get_to(p.mDoorClosed);
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
inline void to_json(nlohmann::json& j, const Path_AbeStart& p)
{
    j = nlohmann::json{
        {"tlv_type", "abe_start"},
        {"base", ToBase(p)},
    };
}

inline void from_json(const nlohmann::json& j, Path_AbeStart& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_EnemyStopper
NLOHMANN_JSON_SERIALIZE_ENUM(Path_EnemyStopper::StopDirection, {
    {Path_EnemyStopper::StopDirection::Left, "left"},
    {Path_EnemyStopper::StopDirection::Right, "right"},
    {Path_EnemyStopper::StopDirection::Both, "both"},
})

inline void to_json(nlohmann::json& j, const Path_EnemyStopper& p)
{
    j = nlohmann::json{
        {"tlv_type", "enemy_stopper"},
        {"base", ToBase(p)},
        {"stop_direction", p.mStopDirection},
        {"switch_id", p.mSwitchId},
    };
}

inline void from_json(const nlohmann::json& j, Path_EnemyStopper& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("stop_direction").get_to(p.mStopDirection);
    j.at("switch_id").get_to(p.mSwitchId);
}

// Path_MovingBombStopper
inline void to_json(nlohmann::json& j, const Path_MovingBombStopper& p)
{
    j = nlohmann::json{
        {"tlv_type", "moving_bomb_stopper"},
        {"base", ToBase(p)},
        {"min", p.mMinDelay},
        {"max", p.mMaxDelay},
    };
}

inline void from_json(const nlohmann::json& j, Path_MovingBombStopper& p)
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

inline void to_json(nlohmann::json& j, const Path_DoorFlame& p)
{
    j = nlohmann::json{
        {"tlv_type", "door_flame"},
        {"base", ToBase(p)},
        {"switch_id", p.mSwitchId},
        {"scale", p.mScale},
        {"colour", p.mColour},
    };
}

inline void from_json(const nlohmann::json& j, Path_DoorFlame& p)
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

inline void to_json(nlohmann::json& j, const Path_Mudokon& p)
{
    j = nlohmann::json{
        {"tlv_type", "mudokon"},
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

inline void from_json(const nlohmann::json& j, Path_Mudokon& p)
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
inline void to_json(nlohmann::json& j, const Path_MovingBomb& p)
{
    j = nlohmann::json{
        {"tlv_type", "moving_bomb"},
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

inline void from_json(const nlohmann::json& j, Path_MovingBomb& p)
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
inline void to_json(nlohmann::json& j, const Path_ElumPathTrans& p)
{
    j = nlohmann::json{
        {"tlv_type", "elum_path_trans"},
        {"base", ToBase(p)},
        {"next_level", p.mNextLevel},
        {"next_path", p.mNextPath},
        {"next_camera", p.mNextCamera},
    };
}

inline void from_json(const nlohmann::json& j, Path_ElumPathTrans& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("next_level").get_to(p.mNextLevel);
    j.at("next_path").get_to(p.mNextPath);
    j.at("next_camera").get_to(p.mNextCamera);
}

// Path_MudokonPathTrans
inline void to_json(nlohmann::json& j, const Path_MudokonPathTrans& p)
{
    j = nlohmann::json{
        {"tlv_type", "mudokon_path_trans"},
        {"base", ToBase(p)},
        {"next_level", p.mNextLevel},
        {"next_path", p.mNextPath},
        {"next_camera", p.mNextCamera},
    };
}

inline void from_json(const nlohmann::json& j, Path_MudokonPathTrans& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("next_level").get_to(p.mNextLevel);
    j.at("next_path").get_to(p.mNextPath);
    j.at("next_camera").get_to(p.mNextCamera);
}

// Path_SecurityClaw
inline void to_json(nlohmann::json& j, const Path_SecurityClaw& p)
{
    j = nlohmann::json{
        {"tlv_type", "security_claw"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"alarm_switch_id", p.mAlarmSwitchId},
        {"alarm_duration", p.mAlarmDuration},
        {"disabled_resources", p.mDisabledResources},
    };
}

inline void from_json(const nlohmann::json& j, Path_SecurityClaw& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("alarm_switch_id").get_to(p.mAlarmSwitchId);
    j.at("alarm_duration").get_to(p.mAlarmDuration);
    j.at("disabled_resources").get_to(p.mDisabledResources);
}

// Path_SlingMudokon
inline void to_json(nlohmann::json& j, const Path_SlingMudokon& p)
{
    j = nlohmann::json{
        {"tlv_type", "sling_mudokon"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"dont_whistle_password", p.mDontWhistlePassword},
        {"code_1", p.mCode1},
        {"code_2", p.mCode2},
    };
}

inline void from_json(const nlohmann::json& j, Path_SlingMudokon& p)
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

inline void to_json(nlohmann::json& j, const Path_FootSwitch& p)
{
    j = nlohmann::json{
        {"tlv_type", "foot_switch"},
        {"base", ToBase(p)},
        {"switch_id", p.mSwitchId},
        {"scale", p.mScale},
        {"action", p.mAction},
        {"triggered_by", p.mTriggeredBy},
    };
}

inline void from_json(const nlohmann::json& j, Path_FootSwitch& p)
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

inline void to_json(nlohmann::json& j, const Path_Paramite& p)
{
    j = nlohmann::json{
        {"tlv_type", "paramite"},
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

inline void from_json(const nlohmann::json& j, Path_Paramite& p)
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
inline void to_json(nlohmann::json& j, const Path_ZzzSpawner& p)
{
    j = nlohmann::json{
        {"tlv_type", "zzz_spawner"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"switch_id", p.mSwitchId},
        {"zzz_interval", p.mZzzInterval},
    };
}

inline void from_json(const nlohmann::json& j, Path_ZzzSpawner& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("zzz_interval").get_to(p.mZzzInterval);
}

// Path_BackgroundGlukkon
inline void to_json(nlohmann::json& j, const Path_BackgroundGlukkon& p)
{
    j = nlohmann::json{
        {"tlv_type", "background_glukkon"},
        {"base", ToBase(p)},
        {"scale_percent", p.mScalePercent},
        {"pal_id", p.mPalId},
    };
}

inline void from_json(const nlohmann::json& j, Path_BackgroundGlukkon& p)
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

inline void to_json(nlohmann::json& j, const Path_GasEmitter& p)
{
    j = nlohmann::json{
        {"tlv_type", "gas_emitter"},
        {"base", ToBase(p)},
        {"switch_id", p.mSwitchId},
        {"colour", p.mColour},
    };
}

inline void from_json(const nlohmann::json& j, Path_GasEmitter& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("colour").get_to(p.mColour);
}

// Path_GasCountDown
inline void to_json(nlohmann::json& j, const Path_GasCountDown& p)
{
    j = nlohmann::json{
        {"tlv_type", "gas_countdown"},
        {"base", ToBase(p)},
        {"start_timer_switch_id", p.mStartTimerSwitchId},
        {"gas_countdown_timer", p.mGasCountdownTimer},
        {"stop_timer_switch_id", p.mStopTimerSwitchId},
    };
}

inline void from_json(const nlohmann::json& j, Path_GasCountDown& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("start_timer_switch_id").get_to(p.mStartTimerSwitchId);
    j.at("gas_countdown_timer").get_to(p.mGasCountdownTimer);
    j.at("stop_timer_switch_id").get_to(p.mStopTimerSwitchId);
}

// Path_RingCancel
inline void to_json(nlohmann::json& j, const Path_RingCancel& p)
{
    j = nlohmann::json{
        {"tlv_type", "ring_cancel"},
        {"base", ToBase(p)},
    };
}

inline void from_json(const nlohmann::json& j, Path_RingCancel& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_SecurityDoor
inline void to_json(nlohmann::json& j, const Path_SecurityDoor& p)
{
    j = nlohmann::json{
        {"tlv_type", "security_door"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"switch_id", p.mSwitchId},
        {"code_1", p.mCode1},
        {"code_2", p.mCode2},
        {"xpos", p.mXPos},
        {"ypos", p.mYPos},
    };
}

inline void from_json(const nlohmann::json& j, Path_SecurityDoor& p)
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

inline void to_json(nlohmann::json& j, const Path_LiftMudokon& p)
{
    j = nlohmann::json{
        {"tlv_type", "lift_mudokon"},
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

inline void from_json(const nlohmann::json& j, Path_LiftMudokon& p)
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

inline void to_json(nlohmann::json& j, const Path_RingMudokon& p)
{
    j = nlohmann::json{
        {"tlv_type", "ring_mudokon"},
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

inline void from_json(const nlohmann::json& j, Path_RingMudokon& p)
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
inline void to_json(nlohmann::json& j, const Path_WellLocal& p)
{
    j = nlohmann::json{
        {"tlv_type", "well_local"},
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

inline void from_json(const nlohmann::json& j, Path_WellLocal& p)
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
inline void to_json(nlohmann::json& j, const Path_WellExpress& p)
{
    j = nlohmann::json{
        {"tlv_type", "well_express"},
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

inline void from_json(const nlohmann::json& j, Path_WellExpress& p)
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
inline void to_json(nlohmann::json& j, const Path_Slog& p)
{
    j = nlohmann::json{
        {"tlv_type", "slog"},
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

inline void from_json(const nlohmann::json& j, Path_Slog& p)
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

inline void to_json(nlohmann::json& j, const Path_SlogSpawner& p)
{
    j = nlohmann::json{
        {"tlv_type", "slog_spawner"},
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

inline void from_json(const nlohmann::json& j, Path_SlogSpawner& p)
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

inline void to_json(nlohmann::json& j, const Path_Scrab& p)
{
    j = nlohmann::json{
        {"tlv_type", "scrab"},
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

inline void from_json(const nlohmann::json& j, Path_Scrab& p)
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
    j.at("kill_enemy").get_to(p.mKillEnemy);
}

// Path_Slig
NLOHMANN_JSON_SERIALIZE_ENUM(Path_Slig_Data::StartState, {
    {Path_Slig_Data::StartState::Listening, "listening"},
    {Path_Slig_Data::StartState::Patrol, "patrol"},
    {Path_Slig_Data::StartState::Sleeping, "sleeping"},
    {Path_Slig_Data::StartState::Chase, "chase"},
    {Path_Slig_Data::StartState::ChaseAndDisappear, "chase_and_disappear"},
    {Path_Slig_Data::StartState::eFallingToChase, "falling_to_chase"},
    {Path_Slig_Data::StartState::ListeningToGlukkon, "listening_to_glukkon"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(Path_Slig_Data::DeathMode, {
    {Path_Slig_Data::DeathMode::FallForward, "fall_forward"},
    {Path_Slig_Data::DeathMode::StandIdle, "stand_idle"},
})

inline void to_json(nlohmann::json& j, const Path_Slig_Data& p)
{
    j = nlohmann::json{
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
        {"disabled_resources_ao", p.mDisabledResourcesAO.Raw().all},
        {"disabled_resources_ae", p.mDisabledResourcesAE},
        {"noise_wake_up_distance", p.mNoiseWakeUpDistance},
        {"slig_spawner_switch_id", p.mSligSpawnerSwitchId},
        {"unlimited_spawns", p.mUnlimitedSpawns},
        {"death_mode", p.mDeathMode},
        {"can_beat", p.mCanBeat},
        {"can_zshoot", p.mCanZShoot},
    };
}

inline void from_json(const nlohmann::json& j, Path_Slig_Data& p)
{
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
    j.at("disabled_resources_ao").get_to(p.mDisabledResourcesAO.Raw().all); // TODO: prob wrong
    j.at("disabled_resources_ae").get_to(p.mDisabledResourcesAE);
    j.at("noise_wake_up_distance").get_to(p.mNoiseWakeUpDistance);
    j.at("slig_spawner_switch_id").get_to(p.mSligSpawnerSwitchId);
    j.at("unlimited_spawns").get_to(p.mUnlimitedSpawns);
    j.at("death_mode").get_to(p.mDeathMode);
    j.at("can_beat").get_to(p.mCanBeat);
    j.at("can_zshoot").get_to(p.mCanZShoot);
}

inline void to_json(nlohmann::json& j, const Path_Slig& p)
{
    j = nlohmann::json{
        {"tlv_type", "slig"},
        {"base", ToBase(p)},
        {"slig_data", p.mData}
    };
}

inline void from_json(const nlohmann::json& j, Path_Slig& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("slig_data").get_to(p.mData);
}

// Path_SligSpawner
inline void to_json(nlohmann::json& j, const Path_SligSpawner& p)
{
    j = nlohmann::json{
        {"tlv_type", "slig_spawner"},
        {"base", ToBase(p)},
        {"slig_data", p.mData},
    };
}

inline void from_json(const nlohmann::json& j, Path_SligSpawner& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("slig_data").get_to(p.mData);
}

// Path_TrainDoor
inline void to_json(nlohmann::json& j, const Path_TrainDoor& p)
{
    j = nlohmann::json{
        {"tlv_type", "train_door"},
        {"base", ToBase(p)},
        {"direction", p.mDirection},
    };
}

inline void from_json(const nlohmann::json& j, Path_TrainDoor& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("direction").get_to(p.mDirection);
}

// Path_TorturedMudokon
inline void to_json(nlohmann::json& j, const Path_TorturedMudokon& p)
{
    j = nlohmann::json{
        {"tlv_type", "tortured_mudokon"},
        {"base", ToBase(p)},
        {"kill_switch_id", p.mKillSwitchId},
        {"release_switch_id", p.mReleaseSwitchId},
    };
}

inline void from_json(const nlohmann::json& j, Path_TorturedMudokon& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("kill_switch_id").get_to(p.mKillSwitchId);
    j.at("release_switch_id").get_to(p.mReleaseSwitchId);
}

// Path_DoorBlocker
inline void to_json(nlohmann::json& j, const Path_DoorBlocker& p)
{
    j = nlohmann::json{
        {"tlv_type", "door_blocker"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"switch_id", p.mSwitchId},
    };
}

inline void from_json(const nlohmann::json& j, Path_DoorBlocker& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("switch_id").get_to(p.mSwitchId);
}

// Path_GlukkonSwitch
inline void to_json(nlohmann::json& j, const Path_GlukkonSwitch& p)
{
    j = nlohmann::json{
        {"tlv_type", "glukkon_switch"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"ok_switch_id", p.mOkSwitchId},
        {"fail_switch_id", p.mFailSwitchId},
        {"xpos", p.mXPos},
        {"ypos", p.mYPos},
    };
}

inline void from_json(const nlohmann::json& j, Path_GlukkonSwitch& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("ok_switch_id").get_to(p.mOkSwitchId);
    j.at("fail_switch_id").get_to(p.mFailSwitchId);
    j.at("xpos").get_to(p.mXPos);
    j.at("ypos").get_to(p.mYPos);
}

// Path_Greeter
inline void to_json(nlohmann::json& j, const Path_Greeter& p)
{
    j = nlohmann::json{
        {"tlv_type", "greeter"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"motion_detector_speed", p.mMotionDetectorSpeed},
        {"facing", p.mFacing},
    };
}

inline void from_json(const nlohmann::json& j, Path_Greeter& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("motion_detector_speed").get_to(p.mMotionDetectorSpeed);
    j.at("facing").get_to(p.mFacing);
}

// Path_BrewMachine
inline void to_json(nlohmann::json& j, const Path_BrewMachine& p)
{
    j = nlohmann::json{
        {"tlv_type", "brew_machine"},
        {"base", ToBase(p)},
        {"brew_count", p.mBrewCount},
    };
}

inline void from_json(const nlohmann::json& j, Path_BrewMachine& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("brew_count").get_to(p.mBrewCount);
}

// Path_Alarm
inline void to_json(nlohmann::json& j, const Path_Alarm& p)
{
    j = nlohmann::json{
        {"tlv_type", "alarm"},
        {"base", ToBase(p)},
        {"switch_id", p.mSwitchId},
        {"alarm_duration", p.mAlarmDuration},
    };
}

inline void from_json(const nlohmann::json& j, Path_Alarm& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("alarm_duration").get_to(p.mAlarmDuration);
}

// Path_ParamiteWebLine
inline void to_json(nlohmann::json& j, const Path_ParamiteWebLine& p)
{
    j = nlohmann::json{
        {"tlv_type", "paramite_web_line"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
    };
}

inline void from_json(const nlohmann::json& j, Path_ParamiteWebLine& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
}

// Path_SlapLock
inline void to_json(nlohmann::json& j, const Path_SlapLock& p)
{
    j = nlohmann::json{
        {"tlv_type", "slap_lock"},
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

inline void from_json(const nlohmann::json& j, Path_SlapLock& p)
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
inline void to_json(nlohmann::json& j, const Path_StatusLight& p)
{
    j = nlohmann::json{
        {"tlv_type", "status_light"},
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

inline void from_json(const nlohmann::json& j, Path_StatusLight& p)
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

// Path_MultiSwitchController
inline void to_json(nlohmann::json& j, const Path_MultiSwitchController& p)
{
    j = nlohmann::json{
        {"tlv_type", "multi_switch_controller"},
        {"base", ToBase(p)},
        {"output_switch_id", p.mOutputSwitchId},
        {"action", p.mAction},
        {"on_off_delay", p.mOnOffDelay},
        {"input_switch_id_1", p.mInputSwitchId1},
        {"input_switch_id_2", p.mInputSwitchId2},
        {"input_switch_id_3", p.mInputSwitchId3},
        {"input_switch_id_4", p.mInputSwitchId4},
        {"input_switch_id_5", p.mInputSwitchId5},
        {"input_switch_id_6", p.mInputSwitchId6},
    };
}

inline void from_json(const nlohmann::json& j, Path_MultiSwitchController& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("output_switch_id").get_to(p.mOutputSwitchId);
    j.at("action").get_to(p.mAction);
    j.at("on_off_delay").get_to(p.mOnOffDelay);
    j.at("input_switch_id_1").get_to(p.mInputSwitchId1);
    j.at("input_switch_id_2").get_to(p.mInputSwitchId2);
    j.at("input_switch_id_3").get_to(p.mInputSwitchId3);
    j.at("input_switch_id_4").get_to(p.mInputSwitchId4);
    j.at("input_switch_id_5").get_to(p.mInputSwitchId5);
    j.at("input_switch_id_6").get_to(p.mInputSwitchId6);
}

// Path_ExplosionSet
inline void to_json(nlohmann::json& j, const Path_ExplosionSet& p)
{
    j = nlohmann::json{
        {"tlv_type", "explosion_set"},
        {"base", ToBase(p)},
        {"start_enabled", p.mStartEnabled},
        {"switch_id", p.mSwitchId},
        {"spawn_assets", p.mSpawnAssets},
        {"start_delay", p.mStartDelay},
        {"start_direction", p.mStartDirection},
        {"asset_interval", p.mAssetInterval},
        {"grid_spacing", p.mGridSpacing},
        {"increasing_grid_spacing", p.mIncreasingGridSpacing},
        {"scale", p.mScale},
    };
}

inline void from_json(const nlohmann::json& j, Path_ExplosionSet& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("start_enabled").get_to(p.mStartEnabled);
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("spawn_assets").get_to(p.mSpawnAssets);
    j.at("start_delay").get_to(p.mStartDelay);
    j.at("start_direction").get_to(p.mStartDirection);
    j.at("asset_interval").get_to(p.mAssetInterval);
    j.at("grid_spacing").get_to(p.mGridSpacing);
    j.at("increasing_grid_spacing").get_to(p.mIncreasingGridSpacing);
    j.at("scale").get_to(p.mScale);
}

// Path_BoneBag
inline void to_json(nlohmann::json& j, const Path_BoneBag& p)
{
    j = nlohmann::json{
        {"tlv_type", "bone_bag"},
        {"base", ToBase(p)},
        {"bone_fall_direction", p.mBoneFallDirection},
        {"vel_x", p.mVelX},
        {"vel_y", p.mVelY},
        {"scale", p.mScale},
        {"bone_amount", p.mBoneAmount},
    };
}

inline void from_json(const nlohmann::json& j, Path_BoneBag& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("bone_fall_direction").get_to(p.mBoneFallDirection);
    j.at("vel_x").get_to(p.mVelX);
    j.at("vel_y").get_to(p.mVelY);
    j.at("scale").get_to(p.mScale);
    j.at("bone_amount").get_to(p.mBoneAmount);
}

// Path_MineCar
inline void to_json(nlohmann::json& j, const Path_MineCar& p)
{
    j = nlohmann::json{
        {"tlv_type", "mine_car"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
    };
}

inline void from_json(const nlohmann::json& j, Path_MineCar& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
}

// Path_ColourfulMeter
inline void to_json(nlohmann::json& j, const Path_ColourfulMeter& p)
{
    j = nlohmann::json{
        {"tlv_type", "colourful_meter"},
        {"base", ToBase(p)},
        {"switch_id", p.mSwitchId},
        {"number_of_meter_bars", p.mNumberOfMeterBars},
        {"mines_alarm_countdown", p.mMinesAlarmCountdown},
        {"start_filled", p.mStartFilled},
    };
}

inline void from_json(const nlohmann::json& j, Path_ColourfulMeter& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("number_of_meter_bars").get_to(p.mNumberOfMeterBars);
    j.at("mines_alarm_countdown").get_to(p.mMinesAlarmCountdown);
    j.at("start_filled").get_to(p.mStartFilled);
}

// Path_DemoSpawnPoint
inline void to_json(nlohmann::json& j, const Path_DemoSpawnPoint& p)
{
    j = nlohmann::json{
        {"tlv_type", "demo_spawn_point"},
        {"base", ToBase(p)},
    };
}

inline void from_json(const nlohmann::json& j, Path_DemoSpawnPoint& p)
{
    j.at("base").get_to(ToBase(p));
}

// Path_LevelLoader
inline void to_json(nlohmann::json& j, const Path_LevelLoader& p)
{
    j = nlohmann::json{
        {"tlv_type", "level_loader"},
        {"base", ToBase(p)},
        {"switch_id", p.mSwitchId},
        {"dest_level", p.mDestLevel},
        {"dest_path", p.mDestPath},
        {"dest_camera", p.mDestCamera},
        {"movie_id", p.mMovieId},
    };
}

inline void from_json(const nlohmann::json& j, Path_LevelLoader& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("dest_level").get_to(p.mDestLevel);
    j.at("dest_path").get_to(p.mDestPath);
    j.at("dest_camera").get_to(p.mDestCamera);
    j.at("movie_id").get_to(p.mMovieId);
}

// Path_SlamDoor
inline void to_json(nlohmann::json& j, const Path_SlamDoor& p)
{
    j = nlohmann::json{
        {"tlv_type", "slam_door"},
        {"base", ToBase(p)},
        {"start_closed", p.mStartClosed},
        {"scale", p.mScale},
        {"switch_id", p.mSwitchId},
        {"flip_y", p.mFlipY},
        {"delete", p.mDelete},
    };
}

inline void from_json(const nlohmann::json& j, Path_SlamDoor& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("start_closed").get_to(p.mStartClosed);
    j.at("scale").get_to(p.mScale);
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("flip_y").get_to(p.mFlipY);
    j.at("delete").get_to(p.mDelete);
}

// Path_Slurg
inline void to_json(nlohmann::json& j, const Path_Slurg& p)
{
    j = nlohmann::json{
        {"tlv_type", "annoying_slurg"},
        {"base", ToBase(p)},
        {"moving_timer", p.mMovingTimer},
        {"facing", p.mFacing},
        {"scale", p.mScale},
        {"switch_id", p.mSlurgSwitchId},
    };
}

inline void from_json(const nlohmann::json& j, Path_Slurg& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("moving_timer").get_to(p.mMovingTimer);
    j.at("facing").get_to(p.mFacing);
    j.at("scale").get_to(p.mScale);
    j.at("switch_id").get_to(p.mSlurgSwitchId);
}

// Path_LaughingGas
inline void to_json(nlohmann::json& j, const Path_LaughingGas& p)
{
    j = nlohmann::json{
        {"tlv_type", "laughing_gas"},
        {"base", ToBase(p)},
        {"laughing_gas", p.mLaughingGas},
        {"laughing_gas_switch_id", p.mLaughingGasSwitchId},
        {"red_percent", p.mRedPercent},
        {"green_percent", p.mGreenPercent},
        {"blue_percent", p.mBluePercent},
    };
}

inline void from_json(const nlohmann::json& j, Path_LaughingGas& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("laughing_gas").get_to(p.mLaughingGas);
    j.at("laughing_gas_switch_id").get_to(p.mLaughingGasSwitchId);
    j.at("red_percent").get_to(p.mRedPercent);
    j.at("green_percent").get_to(p.mGreenPercent);
    j.at("blue_percent").get_to(p.mBluePercent);
}

// Path_WorkWheel
inline void to_json(nlohmann::json& j, const Path_WorkWheel& p)
{
    j = nlohmann::json{
        {"tlv_type", "work_wheel"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"switch_id", p.mSwitchId},
        {"activation_time", p.mActivationTime},
        {"turn_off_time", p.mTurnOffTime},
        {"turn_off_when_stopped", p.mTurnOffWhenStopped},
    };
}

inline void from_json(const nlohmann::json& j, Path_WorkWheel& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("activation_time").get_to(p.mActivationTime);
    j.at("turn_off_time").get_to(p.mTurnOffTime);
    j.at("turn_off_when_stopped").get_to(p.mTurnOffWhenStopped);
}

// Path_Water
inline void to_json(nlohmann::json& j, const Path_Water& p)
{
    j = nlohmann::json{
        {"tlv_type", "water"},
        {"base", ToBase(p)},
        {"max_drops", p.mMaxDrops},
        {"switch_id", p.mSwitchId},
        {"splash_time", p.mSplashTime},
        {"splash_vel_x", p.mSplashVelX},
        {"water_duration", p.mWaterDuration},
    };
}

inline void from_json(const nlohmann::json& j, Path_Water& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("max_drops").get_to(p.mMaxDrops);
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("splash_time").get_to(p.mSplashTime);
    j.at("splash_vel_x").get_to(p.mSplashVelX);
    j.at("water_duration").get_to(p.mWaterDuration);
}
// Path_WheelSyncer
NLOHMANN_JSON_SERIALIZE_ENUM(Path_WheelSyncer::OutputRequirement, {
    {Path_WheelSyncer::OutputRequirement::eAllOn, "all_on"},
    {Path_WheelSyncer::OutputRequirement::e1OnAnd2Off, "1_on_and_2_off"},
    {Path_WheelSyncer::OutputRequirement::e1Or2On, "1_or_2_on"},
    {Path_WheelSyncer::OutputRequirement::e1OnOr2Off, "1_on_or_2_off"},
})

inline void to_json(nlohmann::json& j, const Path_WheelSyncer& p)
{
    j = nlohmann::json{
        {"tlv_type", "wheel_syncer"},
        {"base", ToBase(p)},
        {"input_switch_id_1", p.mInputSwitchId1},
        {"input_switch_id_2", p.mInputSwitchId2},
        {"output_switch_id", p.mOutputSwitchId},
        {"output_requirement", p.mOutputRequirement},
        {"input_switch_id_3", p.mInputSwitchId3},
        {"input_switch_id_4", p.mInputSwitchId4},
        {"input_switch_id_5", p.mInputSwitchId5},
        {"input_switch_id_6", p.mInputSwitchId6},
    };
}

inline void from_json(const nlohmann::json& j, Path_WheelSyncer& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("input_switch_id_1").get_to(p.mInputSwitchId1);
    j.at("input_switch_id_2").get_to(p.mInputSwitchId2);
    j.at("output_switch_id").get_to(p.mOutputSwitchId);
    j.at("output_requirement").get_to(p.mOutputRequirement);
    j.at("input_switch_id_3").get_to(p.mInputSwitchId3);
    j.at("input_switch_id_4").get_to(p.mInputSwitchId4);
    j.at("input_switch_id_5").get_to(p.mInputSwitchId5);
    j.at("input_switch_id_6").get_to(p.mInputSwitchId6);
}

// Path_Fleech
inline void to_json(nlohmann::json& j, const Path_Fleech& p)
{
    j = nlohmann::json{
        {"tlv_type", "fleech"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"facing", p.mFacing},
        {"asleep", p.mAsleep},
        {"attack_anger_increaser", p.mAttackAngerIncreaser},
        {"wake_up_switch_id", p.mWakeUpSwitchId},
        {"hanging", p.mHanging},
        {"lost_target_timeout", p.mLostTargetTimeout},
        {"goes_to_sleep", p.mGoesToSleep},
        {"patrol_range_in_grids", p.mPatrolRangeInGrids},
        {"wake_up_switch_anger_value", p.mWakeUpSwitchAngerValue},
        {"can_wake_up_switch_id", p.mCanWakeUpSwitchId},
        {"persistant", p.mPersistant},
    };
}

inline void from_json(const nlohmann::json& j, Path_Fleech& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("facing").get_to(p.mFacing);
    j.at("asleep").get_to(p.mAsleep);
    j.at("attack_anger_increaser").get_to(p.mAttackAngerIncreaser);
    j.at("wake_up_switch_id").get_to(p.mWakeUpSwitchId);
    j.at("hanging").get_to(p.mHanging);
    j.at("lost_target_timeout").get_to(p.mLostTargetTimeout);
    j.at("goes_to_sleep").get_to(p.mGoesToSleep);
    j.at("patrol_range_in_grids").get_to(p.mPatrolRangeInGrids);
    j.at("wake_up_switch_anger_value").get_to(p.mWakeUpSwitchAngerValue);
    j.at("can_wake_up_switch_id").get_to(p.mCanWakeUpSwitchId);
    j.at("persistant").get_to(p.mPersistant);
}

// Path_SlurgSpawner
inline void to_json(nlohmann::json& j, const Path_SlurgSpawner& p)
{
    j = nlohmann::json{
        {"tlv_type", "slurg_spawner"},
        {"base", ToBase(p)},
        {"spawn_interval", p.mSpawnInterval},
        {"max_slurgs", p.mMaxSlurgs},
        {"spawner_switch_id", p.mSpawnerSwitchId},
        {"moving_timer", p.mMovingTimer},
        {"facing", p.mFacing},
        {"scale", p.mScale},
        {"slurg_switch_id", p.mSlurgSwitchId},
    };
}

inline void from_json(const nlohmann::json& j, Path_SlurgSpawner& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("spawn_interval").get_to(p.mSpawnInterval);
    j.at("max_slurgs").get_to(p.mMaxSlurgs);
    j.at("spawner_switch_id").get_to(p.mSpawnerSwitchId);
    j.at("moving_timer").get_to(p.mMovingTimer);
    j.at("facing").get_to(p.mFacing);
    j.at("scale").get_to(p.mScale);
    j.at("slurg_switch_id").get_to(p.mSlurgSwitchId);
}

// Path_Drill
NLOHMANN_JSON_SERIALIZE_ENUM(Path_Drill::DrillDirection, {
    {Path_Drill::DrillDirection::eDown, "down"},
    {Path_Drill::DrillDirection::eRight, "right"},
    {Path_Drill::DrillDirection::eLeft, "left"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(Path_Drill::DrillBehavior, {
    {Path_Drill::DrillBehavior::eNotInteractable, "not_interactable"},
    {Path_Drill::DrillBehavior::eToggle, "toggle"},
    {Path_Drill::DrillBehavior::eUse, "use"},
})

inline void to_json(nlohmann::json& j, const Path_Drill& p)
{
    j = nlohmann::json{
        {"tlv_type", "drill"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"on_min_pause_time", p.mOnMinPauseTime},
        {"on_max_pause_time", p.mOnMaxPauseTime},
        {"switch_id", p.mSwitchId},
        {"drill_behavior", p.mDrillBehavior},
        {"on_speed", p.mOnSpeed},
        {"start_state_on", p.mStartStateOn},
        {"off_speed", p.mOffSpeed},
        {"off_min_pause_time", p.mOffMinPauseTime},
        {"off_max_pause_time", p.mOffMaxPauseTime},
        {"start_position_bottom", p.mStartPositionBottom},
        {"drill_direction", p.mDrillDirection},
    };
}

inline void from_json(const nlohmann::json& j, Path_Drill& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("on_min_pause_time").get_to(p.mOnMinPauseTime);
    j.at("on_max_pause_time").get_to(p.mOnMaxPauseTime);
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("drill_behavior").get_to(p.mDrillBehavior);
    j.at("on_speed").get_to(p.mOnSpeed);
    j.at("start_state_on").get_to(p.mStartStateOn);
    j.at("off_speed").get_to(p.mOffSpeed);
    j.at("off_min_pause_time").get_to(p.mOffMinPauseTime);
    j.at("off_max_pause_time").get_to(p.mOffMaxPauseTime);
    j.at("start_position_bottom").get_to(p.mStartPositionBottom);
    j.at("drill_direction").get_to(p.mDrillDirection);
}

// Path_Teleporter
inline void to_json(nlohmann::json& j, const Path_Teleporter& p)
{
    j = nlohmann::json{
        {"tlv_type", "teleporter"},
        {"base", ToBase(p)},
        {"teleporter_id", p.mTeleporterId},
        {"other_teleporter_id", p.mOtherTeleporterId},
        {"dest_camera", p.mDestCamera},
        {"dest_path", p.mDestPath},
        {"dest_level", p.mDestLevel},
        {"switch_id", p.mSwitchId},
        {"scale", p.mScale},
        {"wipe_effect", p.mWipeEffect},
        {"movie_id", p.mMovieId},
        {"electric_x", p.mElectricX},
        {"electric_y", p.mElectricY},
    };
}

inline void from_json(const nlohmann::json& j, Path_Teleporter& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("teleporter_id").get_to(p.mTeleporterId);
    j.at("other_teleporter_id").get_to(p.mOtherTeleporterId);
    j.at("dest_camera").get_to(p.mDestCamera);
    j.at("dest_path").get_to(p.mDestPath);
    j.at("dest_level").get_to(p.mDestLevel);
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("scale").get_to(p.mScale);
    j.at("wipe_effect").get_to(p.mWipeEffect);
    j.at("movie_id").get_to(p.mMovieId);
    j.at("electric_x").get_to(p.mElectricX);
    j.at("electric_y").get_to(p.mElectricY);
}

// Path_Glukkon
NLOHMANN_JSON_SERIALIZE_ENUM(Path_Glukkon::Facing, {
    {Path_Glukkon::Facing::eRight, "right"},
    {Path_Glukkon::Facing::eLeft, "left"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(Path_Glukkon::Behavior, {
    {Path_Glukkon::Behavior::eIgnoreWalls, "ignore_walls"},
    {Path_Glukkon::Behavior::eCheckForWalls, "check_for_walls"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(Path_Glukkon::SpawnType, {
    {Path_Glukkon::SpawnType::eRegularSpawn, "regular_spawn"},
    {Path_Glukkon::SpawnType::eFacingLeft, "facing_left"},
    {Path_Glukkon::SpawnType::eFacingRight, "facing_right"},
    {Path_Glukkon::SpawnType::eFullSpawnEffects, "full_spawn_effects"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(Path_Glukkon::GlukkonTypes, {
    {Path_Glukkon::GlukkonTypes::eNormal, "normal"},
    {Path_Glukkon::GlukkonTypes::eStoryAslik, "story_aslik"},
    {Path_Glukkon::GlukkonTypes::eStoryDripik, "story_dripik"},
    {Path_Glukkon::GlukkonTypes::eStoryPhleg, "story_phleg"},
})

inline void to_json(nlohmann::json& j, const Path_Glukkon& p)
{
    j = nlohmann::json{
        {"tlv_type", "glukkon"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"facing", p.mFacing},
        {"behavior", p.mBehavior},
        {"scream_help_delay", p.mScreamHelpDelay},
        {"help_switch_id", p.mHelpSwitchId},
        {"to_calm_delay", p.mToCalmDelay},
        {"spawn_switch_id", p.mSpawnSwitchId},
        {"spawn_type", p.mSpawnType},
        {"spawn_delay", p.mSpawnDelay},
        {"glukkon_type", p.mGlukkonType},
        {"death_switch_id", p.mDeathSwitchId},
        {"play_movie_switch_id", p.mPlayMovieSwitchId},
        {"movie_id", p.mMovieId},
    };
}

inline void from_json(const nlohmann::json& j, Path_Glukkon& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("facing").get_to(p.mFacing);
    j.at("behavior").get_to(p.mBehavior);
    j.at("scream_help_delay").get_to(p.mScreamHelpDelay);
    j.at("help_switch_id").get_to(p.mHelpSwitchId);
    j.at("to_calm_delay").get_to(p.mToCalmDelay);
    j.at("spawn_switch_id").get_to(p.mSpawnSwitchId);
    j.at("spawn_type").get_to(p.mSpawnType);
    j.at("spawn_delay").get_to(p.mSpawnDelay);
    j.at("glukkon_type").get_to(p.mGlukkonType);
    j.at("death_switch_id").get_to(p.mDeathSwitchId);
    j.at("play_movie_switch_id").get_to(p.mPlayMovieSwitchId);
    j.at("movie_id").get_to(p.mMovieId);
}


// Path_CrawlingSligButton
NLOHMANN_JSON_SERIALIZE_ENUM(Path_CrawlingSligButton::ButtonSounds, {
    {Path_CrawlingSligButton::ButtonSounds::None, "none"},
    {Path_CrawlingSligButton::ButtonSounds::SackHit1, "sack_hit1"},
    {Path_CrawlingSligButton::ButtonSounds::SackHit2, "sack_hit2"},
    {Path_CrawlingSligButton::ButtonSounds::FallingItemPresence2, "falling_item_presence_2"},
    {Path_CrawlingSligButton::ButtonSounds::SecurityOrb, "security_orb"},
    {Path_CrawlingSligButton::ButtonSounds::Bullet1, "bullet_1"},
    {Path_CrawlingSligButton::ButtonSounds::AbeGenericMovement, "abe_generic_movement"},
})

inline void to_json(nlohmann::json& j, const Path_CrawlingSligButton& p)
{
    j = nlohmann::json{
        {"tlv_type", "crawling_slig_button"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"switch_id", p.mSwitchId},
        {"action", p.mAction},
        {"on_sound", p.mOnSound},
        {"off_sound", p.mOffSound},
        {"sound_direction", p.mSoundDirection},
    };
}

inline void from_json(const nlohmann::json& j, Path_CrawlingSligButton& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("switch_id").get_to(p.mSwitchId);
    j.at("action").get_to(p.mAction);
    j.at("on_sound").get_to(p.mOnSound);
    j.at("off_sound").get_to(p.mOffSound);
    j.at("sound_direction").get_to(p.mSoundDirection);
}

// Path_FlyingSlig
NLOHMANN_JSON_SERIALIZE_ENUM(Path_FlyingSlig::SpawnDelayStates, {
    {Path_FlyingSlig::SpawnDelayStates::eMoveImmediately, "move_immediately"},
    {Path_FlyingSlig::SpawnDelayStates::eUseCustomSpawnMoveDelay, "use_custom_spawn_move_delay"},
})

inline void to_json(nlohmann::json& j, const Path_FlyingSlig& p)
{
    j = nlohmann::json{
        {"tlv_type", "flying_slig"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"spawn_delay_state", p.mSpawnDelayState},
        {"spawn_move_delay", p.mSpawnMoveDelay},
        {"patrol_pause_min", p.mPatrolPauseMin},
        {"patrol_pause_max", p.mPatrolPauseMax},
        {"facing", p.mFacing},
        {"panic_delay", p.mPanicDelay},
        {"give_up_chase_delay", p.mGiveUpChaseDelay},
        {"prechase_delay", p.mPrechaseDelay},
        {"slig_bound_id", p.mSligBoundId},
        {"alerted_listen_time", p.mAlertedListenTime},
        {"spawner_switch_id", p.mSpawnerSwitchId},
        {"grenade_delay", p.mGrenadeDelay},
        {"max_velocity", p.mMaxVelocity},
        {"launch_grenade_switch_id", p.mLaunchGrenadeSwitchId},
        {"persistant", p.mPersistant},
    };
}

inline void from_json(const nlohmann::json& j, Path_FlyingSlig& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("spawn_delay_state").get_to(p.mSpawnDelayState);
    j.at("spawn_move_delay").get_to(p.mSpawnMoveDelay);
    j.at("patrol_pause_min").get_to(p.mPatrolPauseMin);
    j.at("patrol_pause_max").get_to(p.mPatrolPauseMax);
    j.at("facing").get_to(p.mFacing);
    j.at("panic_delay").get_to(p.mPanicDelay);
    j.at("give_up_chase_delay").get_to(p.mGiveUpChaseDelay);
    j.at("prechase_delay").get_to(p.mPrechaseDelay);
    j.at("slig_bound_id").get_to(p.mSligBoundId);
    j.at("alerted_listen_time").get_to(p.mAlertedListenTime);
    j.at("spawner_switch_id").get_to(p.mSpawnerSwitchId);
    j.at("grenade_delay").get_to(p.mGrenadeDelay);
    j.at("max_velocity").get_to(p.mMaxVelocity);
    j.at("launch_grenade_switch_id").get_to(p.mLaunchGrenadeSwitchId);
    j.at("persistant").get_to(p.mPersistant);
}

// Path_FlyingSligSpawner
inline void to_json(nlohmann::json& j, const Path_FlyingSligSpawner& p)
{
    j = nlohmann::json{
        {"tlv_type", "flying_slig_spawner"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"spawn_delay_state", p.mSpawnDelayState},
        {"spawn_move_delay", p.mSpawnMoveDelay},
        {"patrol_pause_min", p.mPatrolPauseMin},
        {"patrol_pause_max", p.mPatrolPauseMax},
        {"facing", p.mFacing},
        {"panic_delay", p.mPanicDelay},
        {"give_up_chase_delay", p.mGiveUpChaseDelay},
        {"prechase_delay", p.mPrechaseDelay},
        {"slig_bound_id", p.mSligBoundId},
        {"alerted_listen_time", p.mAlertedListenTime},
        {"spawner_switch_id", p.mSpawnerSwitchId},
        {"grenade_delay", p.mGrenadeDelay},
        {"max_velocity", p.mMaxVelocity},
        {"launch_grenade_switch_id", p.mLaunchGrenadeSwitchId},
        {"persistant", p.mPersistant},
    };
}

inline void from_json(const nlohmann::json& j, Path_FlyingSligSpawner& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("spawn_delay_state").get_to(p.mSpawnDelayState);
    j.at("spawn_move_delay").get_to(p.mSpawnMoveDelay);
    j.at("patrol_pause_min").get_to(p.mPatrolPauseMin);
    j.at("patrol_pause_max").get_to(p.mPatrolPauseMax);
    j.at("facing").get_to(p.mFacing);
    j.at("panic_delay").get_to(p.mPanicDelay);
    j.at("give_up_chase_delay").get_to(p.mGiveUpChaseDelay);
    j.at("prechase_delay").get_to(p.mPrechaseDelay);
    j.at("slig_bound_id").get_to(p.mSligBoundId);
    j.at("alerted_listen_time").get_to(p.mAlertedListenTime);
    j.at("spawner_switch_id").get_to(p.mSpawnerSwitchId);
    j.at("grenade_delay").get_to(p.mGrenadeDelay);
    j.at("max_velocity").get_to(p.mMaxVelocity);
    j.at("launch_grenade_switch_id").get_to(p.mLaunchGrenadeSwitchId);
    j.at("persistant").get_to(p.mPersistant);
}

// Path_ScrabSpawner
NLOHMANN_JSON_SERIALIZE_ENUM(Path_ScrabSpawner::SpawnDirection, {
    {Path_ScrabSpawner::SpawnDirection::eNone, "none"},
    {Path_ScrabSpawner::SpawnDirection::eLeft, "left"},
    {Path_ScrabSpawner::SpawnDirection::eRight, "right"},
})

inline void to_json(nlohmann::json& j, const Path_ScrabSpawner& p)
{
    j = nlohmann::json{
        {"tlv_type", "scrab_spawner"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"attack_delay", p.mAttackDelay},
        {"patrol_type_run_or_walk_chance", p.mPatrolTypeRunOrWalkChance},
        {"left_min_delay", p.mPauseLeftMin},
        {"left_max_delay", p.mPauseLeftMax},
        {"right_min_delay", p.mPauseRightMin},
        {"right_max_delay", p.mPauseRightMax},
        {"pause_after_chase_delay", p.mPauseAfterChaseTime},
        {"disabled_resources", p.mDisabledResources},
        {"roar_randomly", p.mRoarRandomly},
        {"persistant", p.mPersistant},
        {"possessed_max_whirl_attack_duration", p.mPossessedMaxWhirlAttackDuration},
        {"kill_enemy", p.mKillEnemy},
        {"spawner_switch_id", p.mSpawnerSwitchId},
        {"facing", p.mFacing},
    };
}

inline void from_json(const nlohmann::json& j, Path_ScrabSpawner& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("attack_delay").get_to(p.mAttackDelay);
    j.at("patrol_type_run_or_walk_chance").get_to(p.mPatrolTypeRunOrWalkChance);
    j.at("left_min_delay").get_to(p.mPauseLeftMin);
    j.at("left_max_delay").get_to(p.mPauseLeftMax);
    j.at("right_min_delay").get_to(p.mPauseRightMin);
    j.at("right_max_delay").get_to(p.mPauseRightMax);
    j.at("pause_after_chase_delay").get_to(p.mPauseAfterChaseTime);
    j.at("disabled_resources").get_to(p.mDisabledResources);
    j.at("roar_randomly").get_to(p.mRoarRandomly);
    j.at("persistant").get_to(p.mPersistant);
    j.at("possessed_max_whirl_attack_duration").get_to(p.mPossessedMaxWhirlAttackDuration);
    j.at("kill_enemy").get_to(p.mKillEnemy);
    j.at("spawner_switch_id").get_to(p.mSpawnerSwitchId);
    j.at("facing").get_to(p.mFacing);
}

// Path_CrawlingSlig
NLOHMANN_JSON_SERIALIZE_ENUM(Path_CrawlingSlig::CrawlDirection, {
    {Path_CrawlingSlig::CrawlDirection::eLeft, "left"},
    {Path_CrawlingSlig::CrawlDirection::eRight, "right"},
    {Path_CrawlingSlig::CrawlDirection::eRandom, "random"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(Path_CrawlingSlig::StartState, {
    {Path_CrawlingSlig::StartState::eSleeping, ""},
})

inline void to_json(nlohmann::json& j, const Path_CrawlingSlig& p)
{
    j = nlohmann::json{
        {"tlv_type", "crawling_slig"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"start_state", p.mStartState},
        {"crawl_direction", p.mCrawlDirection},
        {"panic_switch_id", p.mPanicSwitchId},
        {"respawn_on_death", p.mRespawnOnDeath},
    };
}

inline void from_json(const nlohmann::json& j, Path_CrawlingSlig& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("start_state").get_to(p.mStartState);
    j.at("crawl_direction").get_to(p.mCrawlDirection);
    j.at("panic_switch_id").get_to(p.mPanicSwitchId);
    j.at("respawn_on_death").get_to(p.mRespawnOnDeath);
}

// Path_SligGetWings
inline void to_json(nlohmann::json& j, const Path_SligGetWings& p)
{
    j = nlohmann::json{
        {"tlv_type", "slig_get_wings"},
        {"base", ToBase(p)},
        {"scale", p.mScale},
        {"spawn_delay_state", p.mSpawnDelayState},
        {"spawn_move_delay", p.mSpawnMoveDelay},
        {"patrol_pause_min", p.mPatrolPauseMin},
        {"patrol_pause_max", p.mPatrolPauseMax},
        {"facing", p.mFacing},
        {"panic_delay", p.mPanicDelay},
        {"give_up_chase_delay", p.mGiveUpChaseDelay},
        {"prechase_delay", p.mPrechaseDelay},
        {"slig_bound_id", p.mSligBoundId},
        {"alerted_listen_time", p.mAlertedListenTime},
        {"spawner_switch_id", p.mSpawnerSwitchId},
        {"grenade_delay", p.mGrenadeDelay},
        {"max_velocity", p.mMaxVelocity},
        {"launch_grenade_switch_id", p.mLaunchGrenadeSwitchId},
        {"persistant", p.mPersistant},
    };
}

inline void from_json(const nlohmann::json& j, Path_SligGetWings& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("scale").get_to(p.mScale);
    j.at("spawn_delay_state").get_to(p.mSpawnDelayState);
    j.at("spawn_move_delay").get_to(p.mSpawnMoveDelay);
    j.at("patrol_pause_min").get_to(p.mPatrolPauseMin);
    j.at("patrol_pause_max").get_to(p.mPatrolPauseMax);
    j.at("facing").get_to(p.mFacing);
    j.at("panic_delay").get_to(p.mPanicDelay);
    j.at("give_up_chase_delay").get_to(p.mGiveUpChaseDelay);
    j.at("prechase_delay").get_to(p.mPrechaseDelay);
    j.at("slig_bound_id").get_to(p.mSligBoundId);
    j.at("alerted_listen_time").get_to(p.mAlertedListenTime);
    j.at("spawner_switch_id").get_to(p.mSpawnerSwitchId);
    j.at("grenade_delay").get_to(p.mGrenadeDelay);
    j.at("max_velocity").get_to(p.mMaxVelocity);
    j.at("launch_grenade_switch_id").get_to(p.mLaunchGrenadeSwitchId);
    j.at("persistant").get_to(p.mPersistant);
}

// Path_SligGetPants
inline void to_json(nlohmann::json& j, const Path_SligGetPants& p)
{
    j = nlohmann::json{
        {"tlv_type", "slig_get_pants"},
        {"base", ToBase(p)},
        {"slig_data", p.mData}
    };
}

inline void from_json(const nlohmann::json& j, Path_SligGetPants& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("slig_data").get_to(p.mData);
}

} // namespace relive

