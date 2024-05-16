#pragma once

#include "AESaveConverter.hpp"
#include "relive_tlvs_serialization.hpp" // needed for some common enum and struct serialization (should probably move those to a different file)
#include "CommonTypesSerialization.hpp"

// clang-format off

NLOHMANN_JSON_SERIALIZE_ENUM(LevelIds, {
    {LevelIds::eNone, "none"},
    {LevelIds::eMenu_0, "menu"},
    {LevelIds::eMines_1, "mines"},
    {LevelIds::eNecrum_2, "necrum"},
    {LevelIds::eMudomoVault_3, "mudomo_vault"},
    {LevelIds::eMudancheeVault_4, "mudanchee_vault"},
    {LevelIds::eFeeCoDepot_5, "feeco_depot"},
    {LevelIds::eBarracks_6, "barracks"},
    {LevelIds::eMudancheeVault_Ender_7, "mudanchee_vault_ender"},
    {LevelIds::eBonewerkz_8, "bonewerkz"},
    {LevelIds::eBrewery_9, "brewery"},
    {LevelIds::eBrewery_Ender_10, "brewery_ender"},
    {LevelIds::eMudomoVault_Ender_11, "mudomo_vault_ender"},
    {LevelIds::eFeeCoDepot_Ender_12, "feeco_depot_ender"},
    {LevelIds::eBarracks_Ender_13, "barracks_ender"},
    {LevelIds::eBonewerkz_Ender_14, "bonewerkz_ender"},
    {LevelIds::eTestLevel_15, "test_level"},
    {LevelIds::eCredits_16, "credits"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(Scale, {
    {Scale::Bg, "bg"},
    {Scale::Fg, "fg"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(SligSpeak, {
    {SligSpeak::eNone, "none"},
    {SligSpeak::eHi_0, "hi"},
    {SligSpeak::eHereBoy_1, "here_boy"},
    {SligSpeak::eGetHim_2, "get_him"},
    {SligSpeak::eLaugh_3, "laugh"},
    {SligSpeak::eStay_4, "stay"},
    {SligSpeak::eBullshit_5, "bullshit"},
    {SligSpeak::eLookOut_6, "look_out"},
    {SligSpeak::eBullshit2_7, "bullshit2"},
    {SligSpeak::eFreeze_8, "freeze"},
    {SligSpeak::eWhat_9, "what"},
    {SligSpeak::eHelp_10, "help"},
    {SligSpeak::eGotYa_12, "got_ya"},
    {SligSpeak::eBlurgh_11, "blurgh"},
    {SligSpeak::eOuch1_13, "ouch_1"},
    {SligSpeak::eOuch2_14, "ouch_2"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(SpawnerStates, {
    {SpawnerStates::eInactive_0, "inactive"},
    {SpawnerStates::eSligSpawned_1, "slig_spawned"},
})

inline void to_json(nlohmann::json& j, const SligSpawnerSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"tlv_id", p.mTlvId},
        {"state", p.mState},
        {"spawned_slig_id", p.mSpawnedSligId},
    };
}

inline void from_json(const nlohmann::json& j, SligSpawnerSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("tlv_id").get_to(p.mTlvId);
    j.at("state").get_to(p.mState);
    j.at("spawned_slig_id").get_to(p.mSpawnedSligId);
}

NLOHMANN_JSON_SERIALIZE_ENUM(LiftMoverStates, {
    {LiftMoverStates::eInactive_0, "inactive"},
    {LiftMoverStates::eStartMovingDown_1, "start_moving_down"},
    {LiftMoverStates::eMovingDown_2, "moving_down"},
    {LiftMoverStates::eStartMovingUp_3, "start_moving_up"},
    {LiftMoverStates::eMovingUp_4, "moving_up"},
    {LiftMoverStates::eMovingDone_5, "moving_done"},
})

inline void to_json(nlohmann::json& j, const LiftMoverSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"tlv_id", p.mTlvId},
        {"state", p.mState},
    };
}

inline void from_json(const nlohmann::json& j, LiftMoverSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("tlv_id").get_to(p.mTlvId);
    j.at("state").get_to(p.mState);
}

NLOHMANN_JSON_SERIALIZE_ENUM(BoneStates, {
    {BoneStates::eSpawned_0, "spawned"},
    {BoneStates::eAirborne_1, "airborne"},
    {BoneStates::eCollided_2, "collided"},
    {BoneStates::eOnGround_3, "on_ground"},
    {BoneStates::eEdible_4, "edible"},
    {BoneStates::eFalling_5, "falling"},
})

inline void to_json(nlohmann::json& j, const BoneSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"base_tlv_id", p.mBaseTlvId},
        {"xpos", p.mXPos},
        {"ypos", p.mYPos},
        {"velx", p.mVelX},
        {"vely", p.mVelY},
        {"sprite_scale", p.mSpriteScale},
        {"current_path", p.mCurrentPath},
        {"current_level", p.mCurrentLevel},
        {"render", p.mRender},
        {"drawable", p.mDrawable},
        {"loop", p.mLoop},
        {"interactive", p.mInteractive},
        {"hit_object", p.mHitObject},
        {"platform_id", p.mPlatformId},
        {"collision_line_type", p.mCollisionLineType},
        {"base_throwable_count", p.mThrowableCount},
        {"state", p.mState},
        {"volume_modifier", p.mBounceCount},
        {"initial_xpos", p.mPreviousXPos},
        {"initial_ypos", p.mPreviousYPos},
        {"time_to_live_timer", p.mTimeToLiveTimer},
    };
}

inline void from_json(const nlohmann::json& j, BoneSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("base_tlv_id").get_to(p.mBaseTlvId);
    j.at("xpos").get_to(p.mXPos);
    j.at("ypos").get_to(p.mYPos);
    j.at("velx").get_to(p.mVelX);
    j.at("vely").get_to(p.mVelY);
    j.at("sprite_scale").get_to(p.mSpriteScale);
    j.at("current_path").get_to(p.mCurrentPath);
    j.at("current_level").get_to(p.mCurrentLevel);
    j.at("render").get_to(p.mRender);
    j.at("drawable").get_to(p.mDrawable);
    j.at("loop").get_to(p.mLoop);
    j.at("interactive").get_to(p.mInteractive);
    j.at("hit_object").get_to(p.mHitObject);
    j.at("platform_id").get_to(p.mPlatformId);
    j.at("collision_line_type").get_to(p.mCollisionLineType);
    j.at("base_throwable_count").get_to(p.mThrowableCount);
    j.at("state").get_to(p.mState);
    j.at("volume_modifier").get_to(p.mBounceCount);
    j.at("initial_xpos").get_to(p.mPreviousXPos);
    j.at("initial_ypos").get_to(p.mPreviousYPos);
    j.at("time_to_live_timer").get_to(p.mTimeToLiveTimer);
}

inline void to_json(nlohmann::json& j, const MinesAlarmSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"explosion_timer", p.mExplosionTimer},
    };
}

inline void from_json(const nlohmann::json& j, MinesAlarmSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("explosion_timer").get_to(p.mExplosionTimer);
}

NLOHMANN_JSON_SERIALIZE_ENUM(CrawlingSligMotion, {
    {CrawlingSligMotion::None, "none"},
    {CrawlingSligMotion::Motion_0_Idle, "idle"},
    {CrawlingSligMotion::Motion_1_UsingButton, "using_button"},
    {CrawlingSligMotion::Motion_2_WakingUp, "waking_up"},
    {CrawlingSligMotion::Motion_3_Crawling, "crawling"},
    {CrawlingSligMotion::Motion_4_StartFalling, "start_falling"},
    {CrawlingSligMotion::Motion_5_Falling, "falling"},
    {CrawlingSligMotion::Motion_6_Landing, "landing"},
    {CrawlingSligMotion::Motion_7_ToShakingToIdle, "to_shaking_to_idle"},
    {CrawlingSligMotion::Motion_8_Speaking, "speaking"},
    {CrawlingSligMotion::Motion_9_Snoozing, "snoozing"},
    {CrawlingSligMotion::Motion_10_PushingWall, "pushing_wall"},
    {CrawlingSligMotion::Motion_11_TurnAround, "turn_around"},
    {CrawlingSligMotion::Motion_12_Shaking, "shaking"},
    {CrawlingSligMotion::Motion_13_Empty, "motion_13_empty"},
    {CrawlingSligMotion::Motion_14_ShakingToIdle, "shaking_to_idle"},
    {CrawlingSligMotion::Motion_15_EndCrawling, "end_crawling"},
    {CrawlingSligMotion::Motion_16_IdleToPushingWall, "idle_to_pushing_wall"},
    {CrawlingSligMotion::Motion_17_EndPushingWall, "end_pushing_wall"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(ICrawlingSligBrain::EBrainTypes, {
    {ICrawlingSligBrain::EBrainTypes::Sleeping, "sleeping"},
    {ICrawlingSligBrain::EBrainTypes::Idle, "idle"},
    {ICrawlingSligBrain::EBrainTypes::PanicGetALocker, "panic_get_a_locker"},
    {ICrawlingSligBrain::EBrainTypes::Possessed, "possessed"},
    {ICrawlingSligBrain::EBrainTypes::GetKilled, "get_killed"},
    {ICrawlingSligBrain::EBrainTypes::Transformed, "transformed"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(SleepingBrain::EState, {
    {SleepingBrain::EState::eSleeping, "sleeping"},
    {SleepingBrain::EState::eWakingUp, "waking_up"},
    {SleepingBrain::EState::eIsAwake, "is_awake"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(PanicGetALockerBrain::EState, {
    {PanicGetALockerBrain::EState::eDetermineCrawlDirection, "determine_crawl_direction"},
    {PanicGetALockerBrain::EState::eFalling, "falling"},
    {PanicGetALockerBrain::EState::eSearchLocker, "search_locker"},
    {PanicGetALockerBrain::EState::eTurnAroundForLocker, "turn_around_for_locker"},
    {PanicGetALockerBrain::EState::eSearchLockerOrTurnAround, "search_locker_or_turn_around"},
    {PanicGetALockerBrain::EState::eGetPantsOrWings, "get_pants_or_wings"},
    {PanicGetALockerBrain::EState::eUsingButton, "using_button"},
    {PanicGetALockerBrain::EState::eTurnAround, "turn_around"},
    {PanicGetALockerBrain::EState::eCrawling, "crawling"},
    {PanicGetALockerBrain::EState::eCheckIfWallHit, "check_if_wall_hit"},
    {PanicGetALockerBrain::EState::eBeatBySlig, "beat_by_slig"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(PossessedBrain::EState, {
    {PossessedBrain::EState::eStartPossession, "start_possession"},
    {PossessedBrain::EState::ePossessed, "possessed"},
    {PossessedBrain::EState::eUnpossessing, "unpossessing"},
    {PossessedBrain::EState::eBeatBySlig, "beat_by_slig"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(GetKilledBrain::EState, {
    {GetKilledBrain::EState::eUnknown, "unknown"},
    {GetKilledBrain::EState::eVaporize, "vaporize"},
    {GetKilledBrain::EState::eGibsDeath, "gibs_death"},
    {GetKilledBrain::EState::eSetDead, "set_dead"},
    {GetKilledBrain::EState::eDeathBySlog, "death_by_slog"},
    {GetKilledBrain::EState::eDeathDrop, "death_drop"},
})

inline void to_json(nlohmann::json& j, const CrawlingSligSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"base_tlv_id", p.mBaseTlvId},
        {"xpos", p.mXPos},
        {"ypos", p.mYPos},
        {"velx", p.mVelX},
        {"vely", p.mVelY},
        {"current_path", p.mCurrentPath},
        {"current_level", p.mCurrentLevel},
        {"sprite_scale", p.mSpriteScale},
        {"r", p.mR},
        {"g", p.mG},
        {"b", p.mB},
        {"flip_x", p.mFlipX},
        {"current_motion", p.mCurrentMotion},
        {"current_frame", p.mCurrentFrame},
        {"frame_change_counter", p.mFrameChangeCounter},
        {"render", p.mRender},
        {"drawable", p.mDrawable},
        {"health", p.mHealth},
        {"current_motion2", p.mCurrentMotion2},
        {"next_motion", p.mNextMotion},
        {"last_line_ypos", p.mLastLineYPos},
        {"collision_line_type", p.mCollisionLineType},
        {"controlled", p.mControlled},
        {"crawling_slig_tlv_id", p.mCrawlingSligTlvId},
        {"brain_state", p.mBrainType},
        {"sleeping_brain_state", p.mSleepingBrainState},
        {"panic_get_a_locker_brain_state", p.mPanicGetALockerBrainState},
        {"possessed_brain_state", p.mPossessedBrainState},
        {"get_killed_brain_state", p.mGetKilledBrainState},
        {"multi_use_timer", p.mMultiUseTimer},
        {"velx_scale_factor", p.mVelxScaleFactor},
        {"chanting", p.mChanting},
        {"abe_level", p.mAbeLevel},
        {"abe_path", p.mAbePath},
        {"abe_camera", p.mAbeCamera},
        {"slig_button_tlv_id", p.mSligButtonTlvId},
        {"70_obj_id", p.field_70_obj_id},
        {"transformed_slig_id", p.mTransformedSligId},
        {"speak", p.mSpeak},
        {"say_help_timer", p.mSayHelpTimer},
    };
}

inline void from_json(const nlohmann::json& j, CrawlingSligSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("base_tlv_id").get_to(p.mBaseTlvId);
    j.at("xpos").get_to(p.mXPos);
    j.at("ypos").get_to(p.mYPos);
    j.at("velx").get_to(p.mVelX);
    j.at("vely").get_to(p.mVelY);
    j.at("current_path").get_to(p.mCurrentPath);
    j.at("current_level").get_to(p.mCurrentLevel);
    j.at("sprite_scale").get_to(p.mSpriteScale);
    j.at("r").get_to(p.mR);
    j.at("g").get_to(p.mG);
    j.at("b").get_to(p.mB);
    j.at("flip_x").get_to(p.mFlipX);
    j.at("current_motion").get_to(p.mCurrentMotion);
    j.at("current_frame").get_to(p.mCurrentFrame);
    j.at("frame_change_counter").get_to(p.mFrameChangeCounter);
    j.at("render").get_to(p.mRender);
    j.at("drawable").get_to(p.mDrawable);
    j.at("health").get_to(p.mHealth);
    j.at("current_motion2").get_to(p.mCurrentMotion2);
    j.at("next_motion").get_to(p.mNextMotion);
    j.at("last_line_ypos").get_to(p.mLastLineYPos);
    j.at("collision_line_type").get_to(p.mCollisionLineType);
    j.at("controlled").get_to(p.mControlled);
    j.at("crawling_slig_tlv_id").get_to(p.mCrawlingSligTlvId);
    j.at("brain_state").get_to(p.mBrainType);
    j.at("sleeping_brain_state").get_to(p.mSleepingBrainState);
    j.at("panic_get_a_locker_brain_state").get_to(p.mPanicGetALockerBrainState);
    j.at("possessed_brain_state").get_to(p.mPossessedBrainState);
    j.at("get_killed_brain_state").get_to(p.mGetKilledBrainState);
    j.at("multi_use_timer").get_to(p.mMultiUseTimer);
    j.at("velx_scale_factor").get_to(p.mVelxScaleFactor);
    j.at("chanting").get_to(p.mChanting);
    j.at("abe_level").get_to(p.mAbeLevel);
    j.at("abe_path").get_to(p.mAbePath);
    j.at("abe_camera").get_to(p.mAbeCamera);
    j.at("slig_button_tlv_id").get_to(p.mSligButtonTlvId);
    j.at("70_obj_id").get_to(p.field_70_obj_id);
    j.at("transformed_slig_id").get_to(p.mTransformedSligId);
    j.at("speak").get_to(p.mSpeak);
    j.at("say_help_timer").get_to(p.mSayHelpTimer);
}

NLOHMANN_JSON_SERIALIZE_ENUM(DrillStates, {
    {DrillStates::eRestartCycle_0, "restart_cycle"},
    {DrillStates::eGoingDown_1, "going_down"},
    {DrillStates::eGoingUp_2, "going_up"},
})

inline void to_json(nlohmann::json & j, const DrillSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"drill_tlv_id", p.mDrillTlvId},
        {"off_timer", p.mOffTimer},
        {"state", p.mState},
        {"xyoff", p.mXYOff},
    };
}

inline void from_json(const nlohmann::json& j, DrillSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("drill_tlv_id").get_to(p.mDrillTlvId);
    j.at("off_timer").get_to(p.mOffTimer);
    j.at("state").get_to(p.mState);
    j.at("xyoff").get_to(p.mXYOff);
}

NLOHMANN_JSON_SERIALIZE_ENUM(FartStates, {
    {FartStates::eIdle_0, "idle"},
    {FartStates::eFlying_1, "flying"},
    {FartStates::eDechanting_2, "dechanting"},
})

inline void to_json(nlohmann::json& j, const EvilFartSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"red", p.mRed},
        {"green", p.mGreen},
        {"blue", p.mBlue},
        {"current_path", p.mCurrentPath},
        {"current_level", p.mCurrentLevel},
        {"xpos", p.mXPos},
        {"ypos", p.mYPos},
        {"velx", p.mVelX},
        {"vely", p.mVelY},
        {"sprite_scale", p.mSpriteScale},
        {"current_frame", p.mCurrentFrame},
        {"frame_change_counter", p.mFrameChangeCounter},
        {"anim_render", p.mAnimRender},
        {"drawable", p.mDrawable},
        {"abe_level", p.mAbeLevel},
        {"abe_path", p.mAbePath},
        {"abe_camera", p.mAbeCamera},
        {"controlled", p.mControlled},
        {"fart_exploded", p.mFartExploded},
        {"possessed_alive_timer", p.mPossessedAliveTimer},
        {"state", p.mState},
        {"unpossession_timer", p.mUnpossessionTimer},
        {"back_to_abe_timer", p.mBackToAbeTimer},
    };
}

inline void from_json(const nlohmann::json& j, EvilFartSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("red").get_to(p.mRed);
    j.at("green").get_to(p.mGreen);
    j.at("blue").get_to(p.mBlue);
    j.at("current_path").get_to(p.mCurrentPath);
    j.at("current_level").get_to(p.mCurrentLevel);
    j.at("xpos").get_to(p.mXPos);
    j.at("ypos").get_to(p.mYPos);
    j.at("velx").get_to(p.mVelX);
    j.at("vely").get_to(p.mVelY);
    j.at("sprite_scale").get_to(p.mSpriteScale);
    j.at("current_frame").get_to(p.mCurrentFrame);
    j.at("frame_change_counter").get_to(p.mFrameChangeCounter);
    j.at("anim_render").get_to(p.mAnimRender);
    j.at("drawable").get_to(p.mDrawable);
    j.at("abe_level").get_to(p.mAbeLevel);
    j.at("abe_path").get_to(p.mAbePath);
    j.at("abe_camera").get_to(p.mAbeCamera);
    j.at("controlled").get_to(p.mControlled);
    j.at("fart_exploded").get_to(p.mFartExploded);
    j.at("possessed_alive_timer").get_to(p.mPossessedAliveTimer);
    j.at("state").get_to(p.mState);
    j.at("unpossession_timer").get_to(p.mUnpossessionTimer);
    j.at("back_to_abe_timer").get_to(p.mBackToAbeTimer);
}

NLOHMANN_JSON_SERIALIZE_ENUM(eFleechMotions, {
    {eFleechMotions::eNone_m1, "none"},
    {eFleechMotions::Motion_0_Sleeping, "motion_sleeping"},
    {eFleechMotions::Motion_1_WakingUp, "motion_waking_up"},
    {eFleechMotions::Motion_2_Unknown, "motion_unknown"},
    {eFleechMotions::Motion_3_Idle, "motion_idle"},
    {eFleechMotions::Motion_4_Crawl, "motion_crawl"},
    {eFleechMotions::Motion_5_PatrolCry, "motion_patrol_cry"},
    {eFleechMotions::Motion_6_Knockback, "motion_knockback"},
    {eFleechMotions::Motion_7_StopCrawling, "motion_stop_crawling"},
    {eFleechMotions::Motion_8_StopMidCrawlCycle, "motion_stop_mid_crawl_cycle"},
    {eFleechMotions::Motion_9_Fall, "motion_fall"},
    {eFleechMotions::Motion_10_Land, "motion_land"},
    {eFleechMotions::Motion_11_RaiseHead, "motion_raise_head"},
    {eFleechMotions::Motion_12_Climb, "motion_climb"},
    {eFleechMotions::Motion_13_SettleOnGround, "motion_settle_on_ground"},
    {eFleechMotions::Motion_14_ExtendTongueFromEnemy, "motion_extend_tongue_from_enemy"},
    {eFleechMotions::Motion_15_RetractTongueFromEnemey, "motion_retract_tongue_from_enemy"},
    {eFleechMotions::Motion_16_DeathByFalling, "motion_death_by_falling"},
    {eFleechMotions::Motion_17_SleepingWithTongue, "motion_sleeping_with_tongue"},
    {eFleechMotions::Motion_18_Consume, "motion_consume"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(IFleechBrain::EBrainTypes, {
    {IFleechBrain::EBrainTypes::Patrol, "patrol"},
    {IFleechBrain::EBrainTypes::ChasingAbe, "chasing_abe"},
    {IFleechBrain::EBrainTypes::Scared, "scared"},
    {IFleechBrain::EBrainTypes::Death, "death"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(PatrolBrain::EState, {
    {PatrolBrain::EState::eInit, "state_0_init"},
    {PatrolBrain::EState::eSleeping, "sleeping_1"},
    {PatrolBrain::EState::eWakingUp, "state_2"},
    {PatrolBrain::EState::eGoingBackToSleep, "going_back_to_sleep"},
    {PatrolBrain::EState::eAlerted, "alerted_4"},
    {PatrolBrain::EState::eHearingScrabOrParamite, "hearing_scrab_or_paramite_5"},
    {PatrolBrain::EState::eDangerNearby, "state_6"},
    {PatrolBrain::EState::eBackToAlerted, "state_7"},
    {PatrolBrain::EState::eAlertedByAbe, "alerted_by_abe_8"},
    {PatrolBrain::EState::eNearHoist, "state_9"},
    {PatrolBrain::EState::eClimbHoist, "state_10"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(ChasingAbeBrain::EState, {
    {ChasingAbeBrain::EState::eInit, "init_0"},
    {ChasingAbeBrain::EState::eChasingAbe_1, "chasing_abe_1"},
    {ChasingAbeBrain::EState::eUnknown_2, "unknown_2"},
    {ChasingAbeBrain::EState::eContinueChaseAfterFall_3, "continue_chase_after_fall_3"},
    {ChasingAbeBrain::EState::eBlockedByWall_4, "blocked_by_wall_4"},
    {ChasingAbeBrain::EState::eUnknown_5, "unknown_5"},
    {ChasingAbeBrain::EState::eScrabOrParamiteNearby_6, "scrab_or_paramite_nearby_6"},
    {ChasingAbeBrain::EState::eUnknown_7, "unknown_7"},
    {ChasingAbeBrain::EState::eFleechUnknown_8, "fleech_unknown_8"},
    {ChasingAbeBrain::EState::eUnknown_9, "unknown_9"},
    {ChasingAbeBrain::EState::eAbeIsInTongueRange_10, "abe_is_in_tongue_range_10"},
    {ChasingAbeBrain::EState::eIsAbeDead_11, "is_abe_dead_11"},
    {ChasingAbeBrain::EState::eUnknown_12, "unknown_12"},
    {ChasingAbeBrain::EState::eBackToPatrol_13, "back_to_patrol_13"},
    {ChasingAbeBrain::EState::ePrepareToHoist_14, "prepare_to_hoist_14"},
    {ChasingAbeBrain::EState::eHoistDone_15, "hoist_done_15"},
    {ChasingAbeBrain::EState::eGoBackToChasingAbe_16, "go_back_to_chasing_abe_16"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(ScaredBrain::EState, {
    {ScaredBrain::EState::eScared_0, "scared_0"},
    {ScaredBrain::EState::eReactToDanger_1, "react_to_danger_1"},
    {ScaredBrain::EState::eCrawl_2, "crawl_2"},
    {ScaredBrain::EState::eLookForHoist_3, "look_for_hoist_3"},
    {ScaredBrain::EState::eCornered_4, "cornered_4"},
    {ScaredBrain::EState::eCorneredPrepareAttack_5, "cornered_prepare_attack_5"},
    {ScaredBrain::EState::eCorneredAttack_6, "cornered_attack_6"},
    {ScaredBrain::EState::eCheckIfEnemyDead_7, "check_if_enemy_dead_7"},
    {ScaredBrain::EState::eEnemyStillAlive_8, "enemy_still_alive_8"},
    {ScaredBrain::EState::ePatrolArea_9, "patrol_area_9"},
    {ScaredBrain::EState::ePrepareToHoist_10, "prepare_to_hoist_10"},
    {ScaredBrain::EState::eHoisting_11, "hoisting_11"},
})

inline void to_json(nlohmann::json& j, const FleechSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"field_4_obj_id", p.field_4_obj_id},
        {"xpos", p.mXPos},
        {"ypos", p.mYPos},
        {"velx", p.mVelX},
        {"vely", p.mVelY},
        {"path_number", p.mCurrentPath},
        {"lvl_number", p.mCurrentLevel},
        {"sprite_scale", p.mSpriteScale},
        {"red", p.mRed},
        {"green", p.mGreen},
        {"blue", p.mBlue},
        {"flip_x", p.mFlipX},
        {"current_motion", p.field_28_current_motion},
        {"anim_current_frame", p.field_2A_anim_current_frame},
        {"frame_change_counter", p.field_2C_frame_change_counter},
        {"render", p.mRender},
        {"drawable", p.mDrawable},
        {"health", p.mHealth},
        {"current_motion", p.mCurrentMotion},
        {"next_motion", p.mNextMotion},
        {"last_line_y_pos", p.mLastLineYPos},
        {"collision_line_type", p.mCollisionLineType},
        {"platform_id", p.mPlatformId},
        {"tlv_info", p.mTlvInfo},
        {"field_44_obj_id", p.mFoodObjId},
        {"tongue_state", p.mTongueState},
        {"tongue_sub_state", p.mTongueSubState},
        {"enemy_x_pos", p.mEnemyXPos},
        {"enemy_y_pos", p.mEnemyYPos},
        {"tongue_origin_x", p.mTongueOriginX},
        {"tongue_origin_y", p.mTongueOriginY},
        {"tongue_destination_x", p.mTongueDestinationX},
        {"tongue_destination_y", p.mTongueDestinationY},
        {"field_5a", p.field_5A},
        {"tongue_active_flag", p.mTongueActive},
        {"render_flag", p.mRenderTongue},
        {"brain_type", p.mBrainType},
        {"patrol_brain_state", p.mPatrolBrainState},
        {"chasing_abe_brain_state", p.mChasingAbeBrainState},
        {"scared_brain_state", p.mScaredBrainState},
        {"field_62", p.mReturnToPreviousMotion},
        {"shrivel_timer", p.field_64_shrivel_timer},
        {"fleech_random_idx", p.field_68_fleech_random_idx},
        {"field_69", p.field_69},
        {"did_map_follow_me", p.field_6A_bDidMapFollowMe},
        {"vel_x_factor", p.field_70_velx_factor},
        {"current_anger", p.field_76_current_anger},
        {"max_anger", p.mMaxAnger},
        {"attack_anger", p.mAttackAngerIncreaser},
        {"wakeup_id", p.mWakeUpSwitchId},
        {"wake_up_switch_anger_value", p.mWakeUpSwitchAngerValue},
        {"wake_up_switch_value", p.mWakeUpSwitchValue},
        {"can_wake_up_id", p.mCanWakeUpSwitchId},
        {"event_x_pos", p.field_84_EventXPos},
        {"scrab_paramite_event_x_pos", p.field_86_ScrabParamiteEventXPos},
        {"patrol_range", p.mPatrolRange},
        {"old_x_pos", p.field_8A_old_xpos},
        {"field_8c", p.field_8C},
        {"rnd_crawl", p.field_8E_rnd_crawl},
        {"chase_delay", p.field_90_chase_delay},
        {"chase_timer", p.field_92_chase_timer},
        {"lost_target_timeout", p.mLostTargetTimeout},
        {"lost_target_timer", p.field_96_lost_target_timer},
        {"hoist_x", p.mHoistX},
        {"hoist_y", p.mHoistY},
        {"angle", p.field_9E_angle},
        {"field_9f", p.field_9F},
        {"hoist_y_distance", p.mHoistYDistance},
        {"hoist_x_distance", p.mHoistXDistance},
        {"field_a8", p.mScrabOrParamite},
        {"field_ac_obj_id", p.field_AC_obj_id},
        {"hoist_done", p.mHoistDone},
        {"chasing_or_scared_crawling_left", p.mChasingOrScaredCrawlingLeft},
        {"shrivel_death", p.mShrivelDeath},
        {"scared_sound", p.mScaredSound},
        {"asleep", p.mAsleep},
        {"goes_to_sleep", p.mGoesToSleep},
        {"persistant", p.mPersistant},
        {"field_b2", p.field_B2},
    };
}

inline void from_json(const nlohmann::json& j, FleechSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("field_4_obj_id").get_to(p.field_4_obj_id);
    j.at("xpos").get_to(p.mXPos);
    j.at("ypos").get_to(p.mYPos);
    j.at("velx").get_to(p.mVelX);
    j.at("vely").get_to(p.mVelY);
    j.at("path_number").get_to(p.mCurrentPath);
    j.at("lvl_number").get_to(p.mCurrentLevel);
    j.at("sprite_scale").get_to(p.mSpriteScale);
    j.at("red").get_to(p.mRed);
    j.at("green").get_to(p.mGreen);
    j.at("blue").get_to(p.mBlue);
    j.at("flip_x").get_to(p.mFlipX);
    j.at("current_motion").get_to(p.field_28_current_motion);
    j.at("anim_current_frame").get_to(p.field_2A_anim_current_frame);
    j.at("frame_change_counter").get_to(p.field_2C_frame_change_counter);
    j.at("render").get_to(p.mRender);
    j.at("drawable").get_to(p.mDrawable);
    j.at("health").get_to(p.mHealth);
    j.at("current_motion").get_to(p.mCurrentMotion);
    j.at("next_motion").get_to(p.mNextMotion);
    j.at("last_line_y_pos").get_to(p.mLastLineYPos);
    j.at("collision_line_type").get_to(p.mCollisionLineType);
    j.at("platform_id").get_to(p.mPlatformId);
    j.at("tlv_info").get_to(p.mTlvInfo);
    j.at("field_44_obj_id").get_to(p.mFoodObjId);
    j.at("tongue_state").get_to(p.mTongueState);
    j.at("tongue_sub_state").get_to(p.mTongueSubState);
    j.at("enemy_x_pos").get_to(p.mEnemyXPos);
    j.at("enemy_y_pos").get_to(p.mEnemyYPos);
    j.at("tongue_origin_x").get_to(p.mTongueOriginX);
    j.at("tongue_origin_y").get_to(p.mTongueOriginY);
    j.at("tongue_destination_x").get_to(p.mTongueDestinationX);
    j.at("tongue_destination_y").get_to(p.mTongueDestinationY);
    j.at("field_5a").get_to(p.field_5A);
    j.at("tongue_active_flag").get_to(p.mTongueActive);
    j.at("render_flag").get_to(p.mRenderTongue);
    j.at("brain_type").get_to(p.mBrainType);
    j.at("patrol_brain_state").get_to(p.mPatrolBrainState);
    j.at("chasing_abe_brain_state").get_to(p.mChasingAbeBrainState);
    j.at("scared_brain_state").get_to(p.mScaredBrainState);
    j.at("field_62").get_to(p.mReturnToPreviousMotion);
    j.at("shrivel_timer").get_to(p.field_64_shrivel_timer);
    j.at("fleech_random_idx").get_to(p.field_68_fleech_random_idx);
    j.at("field_69").get_to(p.field_69);
    j.at("did_map_follow_me").get_to(p.field_6A_bDidMapFollowMe);
    j.at("vel_x_factor").get_to(p.field_70_velx_factor);
    j.at("current_anger").get_to(p.field_76_current_anger);
    j.at("max_anger").get_to(p.mMaxAnger);
    j.at("attack_anger").get_to(p.mAttackAngerIncreaser);
    j.at("wakeup_id").get_to(p.mWakeUpSwitchId);
    j.at("wake_up_switch_anger_value").get_to(p.mWakeUpSwitchAngerValue);
    j.at("wake_up_switch_value").get_to(p.mWakeUpSwitchValue);
    j.at("can_wake_up_id").get_to(p.mCanWakeUpSwitchId);
    j.at("event_x_pos").get_to(p.field_84_EventXPos);
    j.at("scrab_paramite_event_x_pos").get_to(p.field_86_ScrabParamiteEventXPos);
    j.at("patrol_range").get_to(p.mPatrolRange);
    j.at("old_x_pos").get_to(p.field_8A_old_xpos);
    j.at("field_8c").get_to(p.field_8C);
    j.at("rnd_crawl").get_to(p.field_8E_rnd_crawl);
    j.at("chase_delay").get_to(p.field_90_chase_delay);
    j.at("chase_timer").get_to(p.field_92_chase_timer);
    j.at("lost_target_timeout").get_to(p.mLostTargetTimeout);
    j.at("lost_target_timer").get_to(p.field_96_lost_target_timer);
    j.at("hoist_x").get_to(p.mHoistX);
    j.at("hoist_y").get_to(p.mHoistY);
    j.at("angle").get_to(p.field_9E_angle);
    j.at("field_9f").get_to(p.field_9F);
    j.at("hoist_y_distance").get_to(p.mHoistYDistance);
    j.at("hoist_x_distance").get_to(p.mHoistXDistance);
    j.at("field_a8").get_to(p.mScrabOrParamite);
    j.at("field_ac_obj_id").get_to(p.field_AC_obj_id);
    j.at("hoist_done").get_to(p.mHoistDone);
    j.at("chasing_or_scared_crawling_left").get_to(p.mChasingOrScaredCrawlingLeft);
    j.at("shrivel_death").get_to(p.mShrivelDeath);
    j.at("scared_sound").get_to(p.mScaredSound);
    j.at("asleep").get_to(p.mAsleep);
    j.at("goes_to_sleep").get_to(p.mGoesToSleep);
    j.at("persistant").get_to(p.mPersistant);
    j.at("field_b2").get_to(p.field_B2);
}

NLOHMANN_JSON_SERIALIZE_ENUM(eFlyingSligMotions, {
    {eFlyingSligMotions::None_m1, "none"},
    {eFlyingSligMotions::Motion_0_Idle, "idle"},
    {eFlyingSligMotions::Motion_1_HorizontalMovement, "horizontal_movement"},
    {eFlyingSligMotions::Motion_2_IdleToTurn, "idle_to_turn"},
    {eFlyingSligMotions::Motion_3_DownMovement, "down_movement"},
    {eFlyingSligMotions::Motion_4_DownMovementToTurn, "down_movement_to_turn"},
    {eFlyingSligMotions::Motion_5_UpMovement, "up_movement"},
    {eFlyingSligMotions::Motion_6_UpMovementToTurn, "up_movement_to_turn"},
    {eFlyingSligMotions::Motion_7_LeverPull, "lever_pull"},
    {eFlyingSligMotions::Motion_8_GameSpeak, "game_speak"},
    {eFlyingSligMotions::Motion_9_Possession, "possession"},
    {eFlyingSligMotions::Motion_10_EndHorizontalMovement, "end_horizontal_movement"},
    {eFlyingSligMotions::Motion_11_BeginUpMovement, "begin_up_movement"},
    {eFlyingSligMotions::Motion_12_HorizontalToDownMovement, "horizontal_to_down_movement"},
    {eFlyingSligMotions::Motion_13_UpToHorizontalMovement, "up_to_horizontal_movement"},
    {eFlyingSligMotions::Motion_14_DownToHorizontalMovement, "down_to_horizontal_movement"},
    {eFlyingSligMotions::Motion_15_QuickTurn, "quick_turn"},
    {eFlyingSligMotions::Motion_16_IdleToHorizontalMovement, "idle_to_horizontal_movement"},
    {eFlyingSligMotions::Motion_17_BeginDownMovement, "begin_down_movement"},
    {eFlyingSligMotions::Motion_18_EndDownMovement, "end_down_movement"},
    {eFlyingSligMotions::Motion_19_DownKnockback, "down_knockback"},
    {eFlyingSligMotions::Motion_20_UpKnockback, "up_knockback"},
    {eFlyingSligMotions::Motion_21_EndUpMovement, "end_up_movement"},
    {eFlyingSligMotions::Motion_22_InstantUpXTurn, "instant_up_x_turn"},
    {eFlyingSligMotions::Motion_23_InstantDownXTurn, "instant_down_x_turn"},
    {eFlyingSligMotions::Motion_24_HorizontalToUpMovement, "horizontal_to_up_movement"},
    {eFlyingSligMotions::Motion_25_TurnToHorizontalMovement, "turn_to_horizontal_movement"},
    })

inline void to_json(nlohmann::json& j, const FlyingSligSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"xpos", p.field_4_xpos},
        {"ypos", p.field_8_ypos},
        {"velx", p.field_C_velx},
        {"vely", p.field_10_vely},
        {"path_number", p.field_14_path_number},
        {"lvl_number", p.field_16_lvl_number},
        {"sprite_scale", p.field_18_sprite_scale},
        {"old_r", p.field_1C_oldr},
        {"old_g", p.field_1E_oldg},
        {"old_b", p.field_20_oldb},
        {"anim_flip_x", p.field_22_bAnimFlipX},
        {"current_state", p.field_24_current_state},
        {"current_frame", p.field_26_current_frame},
        {"frame_change_counter", p.field_28_frame_change_counter},
        {"anim_render", p.field_2A_bAnimRender},
        {"drawable", p.field_2B_bDrawable},
        {"current_health", p.field_2C_current_health},
        {"current_state", p.field_30_current_state},
        {"last_line_y_pos", p.field_34_lastLineYPos},
        {"line_idx", p.field_36_line_idx},
        {"launch_switch_id", p.field_38_launch_switch_id},
        {"possessed", p.mPossessed},
        {"throw_grenade", p.mThrowGrenade},
        {"alerted_and_not_facing_abe", p.mAlertedAndNotFacingAbe},
        {"do_action", p.mDoAction},
        {"chanting", p.mChanting},
        {"speaking2", p.mSpeaking2},
        {"speaking1", p.mSpeaking1},
        {"last_line", p.mLastLine},
        {"unknown1", p.mUnknown1},
        {"unknown2", p.mUnknown2},
        {"tlv_info", p.field_3C_tlvInfo},
        {"timer", p.field_40_timer},
        {"grenade_delay", p.field_44_grenade_delay},
        {"collision_reaction_timer", p.field_48_collision_reaction_timer},
        {"x_speed", p.field_4C_xSpeed},
        {"y_speed", p.field_50_ySpeed},
        {"next_speak", p.field_54_next_speak},
        {"voice_pitch_min", p.field_56_voice_pitch_min},
        {"obj_id", p.field_58_obj_id},
        {"field_5c", p.field_5C},
        {"field_60", p.field_60},
        {"field_64", p.field_64},
        {"line_length", p.field_68_line_length},
        {"field_6c", p.field_6C},
        {"lever_pull_range_x_pos", p.field_70_lever_pull_range_xpos},
        {"lever_pull_range_y_pos", p.field_74_lever_pull_range_ypos},
        {"next_x_pos", p.field_88_nextXPos},
        {"next_y_pos", p.field_8C_nextYPos},
        {"fns1_idx", p.field_90_fns1_idx},
        {"abe_level", p.field_9A_abe_level},
        {"abe_path", p.field_9C_abe_path},
        {"abe_camera", p.field_9E_abe_camera},
        {"bobbing_values_table_index", p.field_A0_bobbing_values_table_index},
        {"bobbing_values_index", p.field_A4_bobbing_values_index},
        {"bobbing_value", p.field_A8_bobbing_value},
    };
}

inline void from_json(const nlohmann::json& j, FlyingSligSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("xpos").get_to(p.field_4_xpos);
    j.at("ypos").get_to(p.field_8_ypos);
    j.at("velx").get_to(p.field_C_velx);
    j.at("vely").get_to(p.field_10_vely);
    j.at("path_number").get_to(p.field_14_path_number);
    j.at("lvl_number").get_to(p.field_16_lvl_number);
    j.at("sprite_scale").get_to(p.field_18_sprite_scale);
    j.at("old_r").get_to(p.field_1C_oldr);
    j.at("old_g").get_to(p.field_1E_oldg);
    j.at("old_b").get_to(p.field_20_oldb);
    j.at("anim_flip_x").get_to(p.field_22_bAnimFlipX);
    j.at("current_state").get_to(p.field_24_current_state);
    j.at("current_frame").get_to(p.field_26_current_frame);
    j.at("frame_change_counter").get_to(p.field_28_frame_change_counter);
    j.at("anim_render").get_to(p.field_2A_bAnimRender);
    j.at("drawable").get_to(p.field_2B_bDrawable);
    j.at("current_health").get_to(p.field_2C_current_health);
    j.at("current_state").get_to(p.field_30_current_state);
    j.at("last_line_y_pos").get_to(p.field_34_lastLineYPos);
    j.at("line_idx").get_to(p.field_36_line_idx);
    j.at("launch_switch_id").get_to(p.field_38_launch_switch_id);
    j.at("possessed").get_to(p.mPossessed);
    j.at("throw_grenade").get_to(p.mThrowGrenade);
    j.at("alerted_and_not_facing_abe").get_to(p.mAlertedAndNotFacingAbe);
    j.at("do_action").get_to(p.mDoAction);
    j.at("chanting").get_to(p.mChanting);
    j.at("speaking2").get_to(p.mSpeaking2);
    j.at("speaking1").get_to(p.mSpeaking1);
    j.at("last_line").get_to(p.mLastLine);
    j.at("unknown1").get_to(p.mUnknown1);
    j.at("unknown2").get_to(p.mUnknown2);
    j.at("tlv_info").get_to(p.field_3C_tlvInfo);
    j.at("timer").get_to(p.field_40_timer);
    j.at("grenade_delay").get_to(p.field_44_grenade_delay);
    j.at("collision_reaction_timer").get_to(p.field_48_collision_reaction_timer);
    j.at("x_speed").get_to(p.field_4C_xSpeed);
    j.at("y_speed").get_to(p.field_50_ySpeed);
    j.at("next_speak").get_to(p.field_54_next_speak);
    j.at("voice_pitch_min").get_to(p.field_56_voice_pitch_min);
    j.at("obj_id").get_to(p.field_58_obj_id);
    j.at("field_5c").get_to(p.field_5C);
    j.at("field_60").get_to(p.field_60);
    j.at("field_64").get_to(p.field_64);
    j.at("line_length").get_to(p.field_68_line_length);
    j.at("field_6c").get_to(p.field_6C);
    j.at("lever_pull_range_x_pos").get_to(p.field_70_lever_pull_range_xpos);
    j.at("lever_pull_range_y_pos").get_to(p.field_74_lever_pull_range_ypos);
    j.at("next_x_pos").get_to(p.field_88_nextXPos);
    j.at("next_y_pos").get_to(p.field_8C_nextYPos);
    j.at("fns1_idx").get_to(p.field_90_fns1_idx);
    j.at("abe_level").get_to(p.field_9A_abe_level);
    j.at("abe_path").get_to(p.field_9C_abe_path);
    j.at("abe_camera").get_to(p.field_9E_abe_camera);
    j.at("bobbing_values_table_index").get_to(p.field_A0_bobbing_values_table_index);
    j.at("bobbing_values_index").get_to(p.field_A4_bobbing_values_index);
    j.at("bobbing_value").get_to(p.field_A8_bobbing_value);
}

inline void to_json(nlohmann::json& j, const FlyingSligSpawnerSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"tlv_info", p.field_4_tlvInfo},
        {"spawned", p.field_8_bSpawned},
        {"spawned_slig_obj_id", p.field_C_spawned_slig_obj_id},
    };
}

inline void from_json(const nlohmann::json& j, FlyingSligSpawnerSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("tlv_info").get_to(p.field_4_tlvInfo);
    j.at("spawned").get_to(p.field_8_bSpawned);
    j.at("spawned_slig_obj_id").get_to(p.field_C_spawned_slig_obj_id);
}

NLOHMANN_JSON_SERIALIZE_ENUM(GameEnderControllerStates, {
    {GameEnderControllerStates::eInit_0, "init"},
    {GameEnderControllerStates::eDetermineEnding_1, "determine_ending"},
    {GameEnderControllerStates::eFinish_2, "finish"},
    {GameEnderControllerStates::eBadEnding_3, "bad_ending"},
    {GameEnderControllerStates::eGoodEnding_4, "good_ending"},
    {GameEnderControllerStates::eAngelicEnding_5, "angelic_ending"},
    {GameEnderControllerStates::eAngelicEndingCredits_6, "angelic_ending_credits"},
    })

inline void to_json(nlohmann::json& j, const GameEnderControllerSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"obj_id", p.mObjId},
        {"timer", p.mTimer},
        {"state", p.mState},
    };
}

inline void from_json(const nlohmann::json& j, GameEnderControllerSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("obj_id").get_to(p.mObjId);
    j.at("timer").get_to(p.mTimer);
    j.at("state").get_to(p.mState);
}
inline void to_json(nlohmann::json& j, const SlapLockWhirlWindSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"switch_id", p.mSwitchId},
    };
}

inline void from_json(const nlohmann::json& j, SlapLockWhirlWindSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("switch_id").get_to(p.mSwitchId);
}

NLOHMANN_JSON_SERIALIZE_ENUM(SlapLockStates, {
    {SlapLockStates::eShaking_0, "shaking"},
    {SlapLockStates::eIdle_1, "idle"},
    {SlapLockStates::eSlapped_2, "slapped"},
    {SlapLockStates::eBroken_3, "broken"},
    {SlapLockStates::eEmitInvisibilityPowerupRing_4, "emit_invisibility_powerup_ring"},
    {SlapLockStates::eFlickerHero_5, "flicker_hero"},
    {SlapLockStates::eGiveInvisibilityFromFlicker_6, "give_invisibility_from_flicker"},
    {SlapLockStates::eGiveInvisibility_7, "give_invisibility"},
})

inline void to_json(nlohmann::json& j, const SlapLockSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"anim_render", p.mAnimRender},
        {"tlv_info", p.mTlvInfo},
        {"tlv_state", p.mTlvState},
        {"state", p.mState},
        {"timer", p.mTimer1},
        {"ability_ring_id", p.mAbilityRingId},
        {"shiny_particle_timer", p.mShinyParticleTimer},
    };
}

inline void from_json(const nlohmann::json& j, SlapLockSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("anim_render").get_to(p.mAnimRender);
    j.at("tlv_info").get_to(p.mTlvInfo);
    j.at("tlv_state").get_to(p.mTlvState);
    j.at("state").get_to(p.mState);
    j.at("timer").get_to(p.mTimer1);
    j.at("ability_ring_id").get_to(p.mAbilityRingId);
    j.at("shiny_particle_timer").get_to(p.mShinyParticleTimer);
}

NLOHMANN_JSON_SERIALIZE_ENUM(GreeterBrainStates, {
    {GreeterBrainStates::eBrain_0_Patrol, "patrol"},
    {GreeterBrainStates::eBrain_1_PatrolTurn, "patrol_turn"},
    {GreeterBrainStates::eBrain_2_Speak, "speak"},
    {GreeterBrainStates::eBrain_3_ChaseSpeak, "chase_speak"},
    {GreeterBrainStates::eBrain_4_Chase, "chase"},
    {GreeterBrainStates::eBrain_5_Knockback, "knockback"},
    {GreeterBrainStates::eBrain_6_ToChase, "to_chase"},
    {GreeterBrainStates::eBrain_7_Fall, "fall"},
})

inline void to_json(nlohmann::json& j, const GreeterSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"r", p.field_2_r},
        {"g", p.field_4_g},
        {"b", p.field_6_b},
        {"path_number", p.field_8_path_number},
        {"lvl_number", p.field_A_lvl_number},
        {"xpos", p.field_C_xpos},
        {"ypos", p.field_10_ypos},
        {"velx", p.field_14_velx},
        {"vely", p.field_18_vely},
        {"sprite_scale", p.field_1C_sprite_scale},
        {"current_frame", p.mCurrentFrame},
        {"frame_change_counter", p.mFrameChangeCounter},
        {"anim_render", p.mAnimRender},
        {"drawable", p.mDrawable},
        {"tlv_info", p.mTlvId},
        {"last_turn_time", p.field_30_last_turn_time},
        {"timer", p.field_34_timer},
        {"times_shot", p.mTimesShot},
        {"dont_set_destroyed", p.field_3A_bDontSetDestroyed},
        {"chasing", p.mChasing},
        {"speed", p.field_40_speed},
        {"brain_state", p.mBrainState},
        {"target_on_left", p.field_46_targetOnLeft},
        {"target_on_right", p.field_48_targetOnRight},
        {"motion_laser_xpos", p.mMotionLaserXPos},
    };
}

inline void from_json(const nlohmann::json& j, GreeterSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("r").get_to(p.field_2_r);
    j.at("g").get_to(p.field_4_g);
    j.at("b").get_to(p.field_6_b);
    j.at("path_number").get_to(p.field_8_path_number);
    j.at("lvl_number").get_to(p.field_A_lvl_number);
    j.at("xpos").get_to(p.field_C_xpos);
    j.at("ypos").get_to(p.field_10_ypos);
    j.at("velx").get_to(p.field_14_velx);
    j.at("vely").get_to(p.field_18_vely);
    j.at("sprite_scale").get_to(p.field_1C_sprite_scale);
    j.at("current_frame").get_to(p.mCurrentFrame);
    j.at("frame_change_counter").get_to(p.mFrameChangeCounter);
    j.at("anim_render").get_to(p.mAnimRender);
    j.at("drawable").get_to(p.mDrawable);
    j.at("tlv_info").get_to(p.mTlvId);
    j.at("last_turn_time").get_to(p.field_30_last_turn_time);
    j.at("timer").get_to(p.field_34_timer);
    j.at("times_shot").get_to(p.mTimesShot);
    j.at("dont_set_destroyed").get_to(p.field_3A_bDontSetDestroyed);
    j.at("chasing").get_to(p.mChasing);
    j.at("speed").get_to(p.field_40_speed);
    j.at("brain_state").get_to(p.mBrainState);
    j.at("target_on_left").get_to(p.field_46_targetOnLeft);
    j.at("target_on_right").get_to(p.field_48_targetOnRight);
    j.at("motion_laser_xpos").get_to(p.mMotionLaserXPos);
}

NLOHMANN_JSON_SERIALIZE_ENUM(GrenadeStates, {
    {GrenadeStates::eFallingToBeCollected_0, "falling_to_be_collected"},
    {GrenadeStates::eWaitToBeCollected_1, "wait_to_be_collected"},
    {GrenadeStates::eDoesNothing_2, "does_nothing_2"},
    {GrenadeStates::eCountingDown_3, "counting_down"},
    {GrenadeStates::eFalling_4, "falling"},
    {GrenadeStates::eHitGround_5, "hit_ground"},
    {GrenadeStates::eWaitForExplodeEnd_6, "wait_for_explode_end"},
    {GrenadeStates::eExploded_7, "exploded"},
    {GrenadeStates::eDoesNothing_8, "does_nothing_8"},
    {GrenadeStates::eFallingBlowUpOnGround_9, "falling_blow_up_on_ground"},
})

inline void to_json(nlohmann::json& j, const GrenadeSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"tlv_info", p.mTlvInfo},
        {"xpos", p.mXPos},
        {"ypos", p.mYPos},
        {"velx", p.mVelX},
        {"vely", p.mVelY},
        {"sprite_scale", p.mSpriteScale},
        {"current_path", p.mCurrentPath},
        {"current_level", p.mCurrentLevel},
        {"render", p.mRender},
        {"drawable", p.mDrawable},
        {"loop", p.mLoop},
        {"interactive", p.mInteractive},
        {"explode_now", p.mExplodeNow},
        {"blow_up_on_collision", p.mBlowUpOnCollision},
        {"platform_id", p.mPlatformId},
        {"collision_line_type", p.mCollisionLineType},
        {"throwable_count", p.mThrowableCount},
        {"state", p.mState},
        {"bounce_count", p.mBounceCount},
        {"explode_countdown", p.mExplodeCountdown},
        {"previous_xpos", p.mPreviousXPos},
        {"previous_ypos", p.mPreviousYPos},
    };
}

inline void from_json(const nlohmann::json& j, GrenadeSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("tlv_info").get_to(p.mTlvInfo);
    j.at("xpos").get_to(p.mXPos);
    j.at("ypos").get_to(p.mYPos);
    j.at("velx").get_to(p.mVelX);
    j.at("vely").get_to(p.mVelY);
    j.at("sprite_scale").get_to(p.mSpriteScale);
    j.at("current_path").get_to(p.mCurrentPath);
    j.at("current_level").get_to(p.mCurrentLevel);
    j.at("render").get_to(p.mRender);
    j.at("drawable").get_to(p.mDrawable);
    j.at("loop").get_to(p.mLoop);
    j.at("interactive").get_to(p.mInteractive);
    j.at("explode_now").get_to(p.mExplodeNow);
    j.at("blow_up_on_collision").get_to(p.mBlowUpOnCollision);
    j.at("platform_id").get_to(p.mPlatformId);
    j.at("collision_line_type").get_to(p.mCollisionLineType);
    j.at("throwable_count").get_to(p.mThrowableCount);
    j.at("state").get_to(p.mState);
    j.at("bounce_count").get_to(p.mBounceCount);
    j.at("explode_countdown").get_to(p.mExplodeCountdown);
    j.at("previous_xpos").get_to(p.mPreviousXPos);
    j.at("previous_ypos").get_to(p.mPreviousYPos);
}

NLOHMANN_JSON_SERIALIZE_ENUM(GlukkonSpeak, {
    {GlukkonSpeak::None, "none"},
    {GlukkonSpeak::Hey_0, "hey"},
    {GlukkonSpeak::DoIt_1, "do_it"},
    {GlukkonSpeak::StayHere_2, "stay_here"},
    {GlukkonSpeak::Commere_3, "commere"},
    {GlukkonSpeak::AllOYa_4, "alloya"},
    {GlukkonSpeak::Heh_5, "heh"},
    {GlukkonSpeak::Help_6, "help"},
    {GlukkonSpeak::Laugh_7, "laugh"},
    {GlukkonSpeak::KillEm_8, "kill_em"},
    {GlukkonSpeak::Unused_9, "unused_9"},
    {GlukkonSpeak::What_11, "what"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(eGlukkonMotions, {
    {eGlukkonMotions::eNone_m1, "none"},
    {eGlukkonMotions::Motion_0_Idle, "idle"},
    {eGlukkonMotions::Motion_1_Walk, "walk"},
    {eGlukkonMotions::Motion_2_Turn, "turn"},
    {eGlukkonMotions::Motion_3_KnockBack, "knockback"},
    {eGlukkonMotions::Motion_4_Jump, "jump"},
    {eGlukkonMotions::Motion_5_JumpToFall, "jump_to_fall"},
    {eGlukkonMotions::Motion_6_WalkToFall, "walk_to_fall"},
    {eGlukkonMotions::Motion_7_Fall, "fall"},
    {eGlukkonMotions::Motion_8_DeathFall, "death_fall"},
    {eGlukkonMotions::Motion_9_Land, "land"},
    {eGlukkonMotions::Motion_10_ChantShake, "chant_shake"},
    {eGlukkonMotions::Motion_11_Speak1, "speak1"},
    {eGlukkonMotions::Motion_12_Speak2, "speak2"},
    {eGlukkonMotions::Motion_13_LongLaugh, "long_laugh"},
    {eGlukkonMotions::Motion_14_BeginWalk, "begin_walk"},
    {eGlukkonMotions::Motion_15_EndWalk, "end_walk"},
    {eGlukkonMotions::Motion_16_StandToJump, "stand_to_jump"},
    {eGlukkonMotions::Motion_17_JumpToStand, "jump_to_stand"},
    {eGlukkonMotions::Motion_18_WalkToJump, "walk_to_jump"},
    {eGlukkonMotions::Motion_19_JumpToWalk, "jump_to_walk"},
    {eGlukkonMotions::Motion_20_KnockBackStandBegin, "knockback_stand_begin"},
    {eGlukkonMotions::Motion_21_GetShot, "get_shot"},
    {eGlukkonMotions::Motion_22_KnockBackStandEnd, "knockback_stand_end"},
    {eGlukkonMotions::Motion_23_Speak3, "speak3"},
    {eGlukkonMotions::Motion_24_EndSingleStep, "end_single_step"},
})

inline void to_json(nlohmann::json& j, const GlukkonSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"field_4_object_id", p.field_4_object_id},
        {"xpos", p.mXPos},
        {"ypos", p.mYPos},
        {"velx", p.mVelX},
        {"vely", p.mVelY},
        {"field_18_path", p.mCurrentPath},
        {"field_1a_level", p.mCurrentLevel},
        {"sprite_scale", p.mSpriteScale},
        {"ring_red", p.mRed},
        {"ring_green", p.mGreen},
        {"ring_blue", p.mBlue},
        {"flip_x", p.mFlipX},
        {"field_28_current_motion", p.mCurrentMotion},
        {"current_frame", p.mCurrentFrame},
        {"frame_change_counter", p.mFrameChangeCounter},
        {"render", p.mRender},
        {"drawable", p.mDrawable},
        {"health", p.mHealth},
        {"field_34_current_motion", p.mCurrentMotion2},
        {"next_motion", p.mNextMotion},
        {"last_line_ypos", p.field_38_last_line_ypos},
        {"line_type", p.mLineType},
        {"is_active_char", p.mIsActiveChar},
        {"tlv_info", p.mTlvId},
        {"brain_state_idx", p.mBrainStateIdx},
        {"brain_sub_state", p.mBrainSubState},
        {"timer", p.field_54_timer},
        {"vel_x_scale_factor", p.mFallingVelXScaleFactor},
        {"prevent_depossession", p.mPreventDepossession},
        {"field_60_level", p.mAbeLevel},
        {"field_62_path", p.mAbePath},
        {"camera", p.mAbeCamera},
        {"speak", p.mSpeak},
        {"gamespeak_pitch", p.mGamespeakPitch},
        {"previous_ypos", p.mPreviousYPos},
        {"randomish_speak_timer", p.mRandomishSpeakTimer},
        {"turn_or_help_timer", p.mTurnOrHelpTimer},
        {"panic_timer", p.mPanicTimer},
        {"field_7c", p.field_7C},
        {"knockback_delay_after_getting_shot_timer", p.mKnockbackDelayAfterGettingShotTimer},
        {"getting_shot_timer", p.mGettingShotTimer},
        {"field_88_obj_id", p.mFadeId},
        {"can_be_possessed", p.mCanBePossessed},
        {"current_type", p.mCurrentType},
    };
}

inline void from_json(const nlohmann::json& j, GlukkonSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("field_4_object_id").get_to(p.field_4_object_id);
    j.at("xpos").get_to(p.mXPos);
    j.at("ypos").get_to(p.mYPos);
    j.at("velx").get_to(p.mVelX);
    j.at("vely").get_to(p.mVelY);
    j.at("field_18_path").get_to(p.mCurrentPath);
    j.at("field_1a_level").get_to(p.mCurrentLevel);
    j.at("sprite_scale").get_to(p.mSpriteScale);
    j.at("ring_red").get_to(p.mRed);
    j.at("ring_green").get_to(p.mGreen);
    j.at("ring_blue").get_to(p.mBlue);
    j.at("flip_x").get_to(p.mFlipX);
    j.at("field_28_current_motion").get_to(p.mCurrentMotion);
    j.at("current_frame").get_to(p.mCurrentFrame);
    j.at("frame_change_counter").get_to(p.mFrameChangeCounter);
    j.at("render").get_to(p.mRender);
    j.at("drawable").get_to(p.mDrawable);
    j.at("health").get_to(p.mHealth);
    j.at("field_34_current_motion").get_to(p.mCurrentMotion2);
    j.at("next_motion").get_to(p.mNextMotion);
    j.at("last_line_ypos").get_to(p.field_38_last_line_ypos);
    j.at("line_type").get_to(p.mLineType);
    j.at("is_active_char").get_to(p.mIsActiveChar);
    j.at("tlv_info").get_to(p.mTlvId);
    j.at("brain_state_idx").get_to(p.mBrainStateIdx);
    j.at("brain_sub_state").get_to(p.mBrainSubState);
    j.at("timer").get_to(p.field_54_timer);
    j.at("vel_x_scale_factor").get_to(p.mFallingVelXScaleFactor);
    j.at("prevent_depossession").get_to(p.mPreventDepossession);
    j.at("field_60_level").get_to(p.mAbeLevel);
    j.at("field_62_path").get_to(p.mAbePath);
    j.at("camera").get_to(p.mAbeCamera);
    j.at("speak").get_to(p.mSpeak);
    j.at("gamespeak_pitch").get_to(p.mGamespeakPitch);
    j.at("previous_ypos").get_to(p.mPreviousYPos);
    j.at("randomish_speak_timer").get_to(p.mRandomishSpeakTimer);
    j.at("turn_or_help_timer").get_to(p.mTurnOrHelpTimer);
    j.at("panic_timer").get_to(p.mPanicTimer);
    j.at("field_7c").get_to(p.field_7C);
    j.at("knockback_delay_after_getting_shot_timer").get_to(p.mKnockbackDelayAfterGettingShotTimer);
    j.at("getting_shot_timer").get_to(p.mGettingShotTimer);
    j.at("field_88_obj_id").get_to(p.mFadeId);
    j.at("can_be_possessed").get_to(p.mCanBePossessed);
    j.at("current_type").get_to(p.mCurrentType);
}

NLOHMANN_JSON_SERIALIZE_ENUM(Mud_Emotion, {
    {Mud_Emotion::eNormal_0, "normal"},
    {Mud_Emotion::eAngry_1, "angry"},
    {Mud_Emotion::eAggressive_2, "aggressive"},
    {Mud_Emotion::eSad_3, "sad"},
    {Mud_Emotion::eSuicidal_4, "suicidal"},
    {Mud_Emotion::eHappy_5, "happy"},
    {Mud_Emotion::eWired_6, "wired"},
    {Mud_Emotion::eSick_7, "sick"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(MudSounds, {
    {MudSounds::eNone, "none"},
    {MudSounds::eEmpty_0, "empty_0"},
    {MudSounds::eEmpty_1, "empty_1"},
    {MudSounds::eEmpty_2, "empty_2"},
    {MudSounds::eHelloNeutral_3, "hello_neutral"},
    {MudSounds::eFollowMe_4, "follow_me"},
    {MudSounds::eAnger_5, "anger"},
    {MudSounds::eWait_6, "wait"},
    {MudSounds::eFart_7, "fart"},
    {MudSounds::eGiggle_8, "giggle"},
    {MudSounds::eHurt2_9, "hurt2"},
    {MudSounds::eLaugh_10, "laugh"},
    {MudSounds::eGoodbye_11, "goodbye"},
    {MudSounds::eOkay_12, "okay"},
    {MudSounds::eNuhUh_13, "nuh_uh"},
    {MudSounds::eOops_14, "oops"},
    {MudSounds::eDeathDropScream_15, "death_drop_scream"},
    {MudSounds::eHurt1_16, "hurt1"},
    {MudSounds::eAllOYa_17, "alloya"},
    {MudSounds::eHiAngry_18, "hi_angry"},
    {MudSounds::eHiHappy_19, "hi_happy"},
    {MudSounds::eHiSad_20, "hi_sad"},
    {MudSounds::eNoAngry_21, "no_angry"},
    {MudSounds::eNoSad_22, "no_sad"},
    {MudSounds::eFartPuh_23, "fart_puh"},
    {MudSounds::eSick_24, "sick"},
    {MudSounds::eWork_25, "work"},
    {MudSounds::eStopIt_26, "stop_it"},
    {MudSounds::eSorry_27, "sorry"},
    {MudSounds::eSadUgh_28, "sad_ugh"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(eAbeMotions, {
    { eAbeMotions::None_m1, "none"},
    { eAbeMotions::Motion_0_Idle_44EEB0, "idle"},
    { eAbeMotions::Motion_1_WalkLoop_44FBA0, "walk_loop"},
    { eAbeMotions::Motion_2_StandingTurn_451830, "standing_turn" },
    { eAbeMotions::Motion_3_Fall_459B60, "fall" },
    { eAbeMotions::Motion_4_WalkToIdle_44FFC0, "walk_to_idle" },
    { eAbeMotions::Motion_5_MidWalkToIdle_450080, "mid_walk_to_idle" },
    { eAbeMotions::Motion_6_WalkBegin_44FEE0, "walk_begin" },
    { eAbeMotions::Motion_7_Speak_45B140, "motion_7_speak" },
    { eAbeMotions::Motion_8_Speak_45B160, "motion_8_speak" },
    { eAbeMotions::Motion_9_Speak_45B180, "motion_9_speak" },
    { eAbeMotions::Motion_10_Fart_45B1A0, "fart" },
    { eAbeMotions::Motion_11_Speak_45B0A0, "motion_11_speak" },
    { eAbeMotions::Motion_12_Null_4569C0, "motion_12_null" },
    { eAbeMotions::Motion_13_HoistBegin_452B20, "hoist_begin" },
    { eAbeMotions::Motion_14_HoistIdle_452440, "hoist_idle" },
    { eAbeMotions::Motion_15_HoistLand_452BA0, "hoist_land" },
    { eAbeMotions::Motion_16_LandSoft_45A360, "land_soft" },
    { eAbeMotions::Motion_17_CrouchIdle_456BC0, "crouch_idle" },
    { eAbeMotions::Motion_18_CrouchToStand_454600, "crouch_to_stand" },
    { eAbeMotions::Motion_19_StandToCrouch_453DC0, "stand_to_crouch" },
    { eAbeMotions::Motion_20_CrouchSpeak_454550, "crouch_speak" },
    { eAbeMotions::jMotion_21_ToCrouchSpeak_4545E0, "to_crouch_speak" },
    { eAbeMotions::Motion_22_RollBegin_4539A0, "roll_begin" },
    { eAbeMotions::Motion_23_RollLoop_453A90, "roll_loop" },
    { eAbeMotions::Motion_24_453D00, "motion_24" },
    { eAbeMotions::Motion_25_RunSlideStop_451330, "run_slide_stop" },
    { eAbeMotions::Motion_26_RunTurn_451500, "run_turn" },
    { eAbeMotions::Motion_27_HopBegin_4521C0, "hop_begin" },
    { eAbeMotions::Motion_28_HopMid_451C50, "hop_mid" },
    { eAbeMotions::Motion_29_HopLand_4523D0, "hop_land" },
    { eAbeMotions::Motion_30_RunJumpBegin_4532E0, "run_jump_begin" },
    { eAbeMotions::Motion_31_RunJumpMid_452C10, "run_jump_mid" },
    { eAbeMotions::Motion_32_RunJumpLand_453460, "run_jump_land" },
    { eAbeMotions::Motion_33_RunLoop_4508E0, "run_loop" },
    { eAbeMotions::Motion_34_DunnoBegin_44ECF0, "dunno_begin" },
    { eAbeMotions::Motion_35_DunnoEnd_44ED10, "dunno_end" },
    { eAbeMotions::Motion_36_Null_45BC50, "motion_36_null" },
    { eAbeMotions::Motion_37_CrouchTurn_454390, "crouch_turn" },
    { eAbeMotions::jMotion_38_RunToRoll_453A70, "run_to_roll" },
    { eAbeMotions::Motion_39_StandingToRun_450D40, "standing_to_run" },
    { eAbeMotions::Motion_40_SneakLoop_450550, "sneak_loop" },
    { eAbeMotions::Motion_41_WalkToSneak_450250, "walk_to_sneak" },
    { eAbeMotions::Motion_42_SneakToWalk_4503D0, "sneak_to_walk" },
    { eAbeMotions::Motion_43_MidWalkToSneak_450380, "mid_walk_to_sneak" },
    { eAbeMotions::Motion_44_MidSneakToWalk_450500, "mid_sneak_to_walk" },
    { eAbeMotions::Motion_45_SneakBegin_4507A0, "sneak_begin" },
    { eAbeMotions::Motion_46_SneakToIdle_450870, "sneak_to_idle" },
    { eAbeMotions::jMotion_47_MidSneakToIdle_4508C0, "mid_sneak_to_idle" },
    { eAbeMotions::Motion_48_WalkToRun_4500A0, "walk_to_run" },
    { eAbeMotions::Motion_49_MidWalkToRun_450200, "mid_walk_to_run" },
    { eAbeMotions::Motion_50_RunToWalk_450E20, "run_to_walk" },
    { eAbeMotions::Motion_51_MidRunToWalk_450F50, "mid_run_to_walk" },
    { eAbeMotions::Motion_52_RunTurnToRun_451710, "run_turn_to_run" },
    { eAbeMotions::Motion_53_RunTurnToWalk_451800, "run_turn_to_walk" },
    { eAbeMotions::Motion_54_RunJumpLandRun_4538F0, "run_jump_land_run" },
    { eAbeMotions::Motion_55_RunJumpLandWalk_453970, "run_jump_land_walk" },
    { eAbeMotions::Motion_56_DeathDropFall_4591F0, "death_drop_fall" },
    { eAbeMotions::Motion_57_Dead_4589A0, "dead" },
    { eAbeMotions::Motion_58_DeadPre_4593E0, "dead_pre" },
    { eAbeMotions::Motion_59_Null_459450, "motion_59_null" },
    { eAbeMotions::Motion_60_Unused_4A3200, "motion_60_unused" },
    { eAbeMotions::Motion_61_TurnToRun_456530, "turn_to_run" },
    { eAbeMotions::Motion_62_Punch_454750, "punch" },
    { eAbeMotions::Motion_63_Sorry_454670, "sorry" },
    { eAbeMotions::Motion_64_AfterSorry_454730, "after_sorry" },
    { eAbeMotions::Motion_65_LedgeAscend_4548E0, "ledge_ascend" },
    { eAbeMotions::Motion_66_LedgeDescend_454970, "ledge_descend" },
    { eAbeMotions::Motion_67_LedgeHang_454E20, "ledge_hang" },
    { eAbeMotions::Motion_68_ToOffScreenHoist_454B80, "to_off_screen_hoist" },
    { eAbeMotions::Motion_69_LedgeHangWobble_454EF0, "ledge_hang_wobble" },
    { eAbeMotions::Motion_70_RingRopePullHang_455AF0, "ring_rope_pull_hang" },
    { eAbeMotions::Motion_71_Knockback_455090, "knockback" },
    { eAbeMotions::Motion_72_KnockbackGetUp_455340, "knockback_get_up" },
    { eAbeMotions::Motion_73_PushWall_4553A0, "push_wall" },
    { eAbeMotions::Motion_74_RollingKnockback_455290, "rolling_knockback" },
    { eAbeMotions::Motion_75_JumpIntoWell_45C7B0, "jump_into_well" },
    { eAbeMotions::Motion_76_ToInsideOfAWellLocal_45CA40, "to_inside_of_a_well_local" },
    { eAbeMotions::Motion_77_ToWellShotOut_45D130, "to_well_shot_out" },
    { eAbeMotions::Motion_78_WellBegin_45C810, "motion_78_well_begin" },
    { eAbeMotions::Motion_79_InsideWellLocal_45CA60, "inside_well_local" },
    { eAbeMotions::Motion_80_WellShotOut_45D150, "well_shot_out" },
    { eAbeMotions::jMotion_81_WellBegin_45C7F0, "jmotion_81_well_begin" },
    { eAbeMotions::Motion_82_InsideWellExpress_45CC80, "inside_well_express" },
    { eAbeMotions::Motion_83_WellExpressShotOut_45CF70, "well_express_shot_out" },
    { eAbeMotions::Motion_84_FallLandDie_45A420, "fall_land_die" },
    { eAbeMotions::jMotion_85_Fall_455070, "fall" },
    { eAbeMotions::Motion_86_HandstoneBegin, "handstone_begin" },
    { eAbeMotions::Motion_87_HandstoneEnd, "handstone_end" },
    { eAbeMotions::Motion_88_GrenadeMachineUse, "grenade_machine_use" },
    { eAbeMotions::Motion_89_BrewMachineBegin, "brew_machine_begin" },
    { eAbeMotions::Motion_90_BrewMachineEnd, "brew_machine_end" },
    { eAbeMotions::Motion_91_FallingFromGrab, "falling_from_grab" },
    { eAbeMotions::Motion_92_ForceDownFromHoist, "force_down_from_hoist" },
    { eAbeMotions::Motion_93_WalkOffEdge, "walk_off_edge" },
    { eAbeMotions::Motion_94_RunOffEdge, "run_off_edge" },
    { eAbeMotions::Motion_95_SneakOffEdge, "sneak_off_edge" },
    { eAbeMotions::Motion_96_HopToFall, "hop_to_fall" },
    { eAbeMotions::Motion_97_RunJumpToFall, "run_jump_to_fall" },
    { eAbeMotions::Motion_98_RollOffEdge, "roll_off_edge" },
    { eAbeMotions::Motion_99_LeverUse, "lever_use" },
    { eAbeMotions::Motion_100_SlapBomb, "slap_bomb" },
    { eAbeMotions::Motion_101_KnockForward, "knock_forward" },
    { eAbeMotions::Motion_102_RollingKnockForward, "rolling_knock_forward" },
    { eAbeMotions::jMotion_103_KnockForwardGetUp, "knock_forward_get_up" },
    { eAbeMotions::Motion_104_RockThrowStandingHold, "rock_throw_standing_hold" },
    { eAbeMotions::Motion_105_RockThrowStandingThrow, "rock_throw_standing_throw" },
    { eAbeMotions::Motion_106_RockThrowStandingEnd, "rock_throw_standing_end" },
    { eAbeMotions::Motion_107_RockThrowCrouchingHold, "rock_throw_crouching_hold" },
    { eAbeMotions::Motion_108_RockThrowCrouchingThrow, "rock_throw_crouching_throw" },
    { eAbeMotions::Motion_109_ZShotRolling, "z_shot_rolling" },
    { eAbeMotions::Motion_110_ZShot, "z_shot" },
    { eAbeMotions::Motion_111_PickupItem, "pickup_item" },
    { eAbeMotions::Motion_112_Chant, "chant" },
    { eAbeMotions::Motion_113_ChantEnd, "chant_end" },
    { eAbeMotions::Motion_114_DoorEnter, "door_enter" },
    { eAbeMotions::Motion_115_DoorExit, "door_exit" },
    { eAbeMotions::Motion_116_MineCarEnter, "mine_car_enter" },
    { eAbeMotions::Motion_117_InMineCar, "in_mine_car" },
    { eAbeMotions::Motion_118_MineCarExit, "mine_car_exit" },
    { eAbeMotions::Motion_119_ToShrykull, "to_shrykull" },
    { eAbeMotions::Motion_120_EndShrykull, "end_shrykull" },
    { eAbeMotions::Motion_121_LiftGrabBegin, "lift_grab_begin" },
    { eAbeMotions::Motion_122_LiftGrabEnd, "lift_grab_end" },
    { eAbeMotions::Motion_123_LiftGrabIdle, "lift_grab_idle" },
    { eAbeMotions::Motion_124_LiftUseUp, "lift_use_up" },
    { eAbeMotions::Motion_125_LiftUseDown, "lift_use_down" },
    { eAbeMotions::Motion_126_TurnWheelBegin, "turn_wheel_begin" },
    { eAbeMotions::Motion_127_TurnWheelLoop, "turn_wheel_loop" },
    { eAbeMotions::Motion_128_TurnWheelEnd, "turn_wheel_end" },
    { eAbeMotions::Motion_129_PoisonGasDeath, "poison_gas_death" }
    })

inline void to_json(nlohmann::json& j, const AbeSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"xpos", p.mXPos},
        {"ypos", p.mYPos},
        {"velx", p.mVelX},
        {"vely", p.mVelY},
        {"current_path", p.mCurrentPath},
        {"current_level", p.mCurrentLevel},
        {"sprite_scale", p.mSpriteScale},
        {"scale", p.mScale},
        {"red", p.mRed},
        {"green", p.mGreen},
        {"blue", p.mBlue},
        {"anim_flip_x", p.bAnimFlipX},
        {"current_motion", p.mCurrentMotion},
        {"current_frame", p.mCurrentFrame},
        {"frame_change_counter", p.mFrameChangeCounter},
        {"render_layer", p.mRenderLayer},
        {"anim_render", p.mAnimRender},
        {"is_drawable", p.mIsDrawable},
        {"health", p.mHealth},
        {"current_motion_2", p.mCurrentMotion2},
        {"next_motion", p.mNextMotion},
        {"last_line_y_pos", p.mLastLineYPos},
        {"collision_line_type", p.mCollisionLineType},
        {"platform_id", p.mPlatformId},
        {"is_electrocuted", p.mIsElectrocuted},
        {"is_invisible", p.mIsInvisible},
        {"is_abe_controlled", p.mIsAbeControlled},
        {"x_vel_slow_by", p.field_48_x_vel_slow_by},
        {"state", p.field_50_state},
        {"timer", p.field_54_timer},
        {"abe_timer", p.field_58_abe_timer},
        {"regen_health_timer", p.mRegenHealthTimer},
        {"mood", p.mMood},
        {"say", p.mSay},
        {"auto_say_timer", p.mAutoSayTimer},
        {"ring_pulse_timer", p.mRingPulseTimer},
        {"base_throwable_count", p.mBaseThrowableCount},
        {"shrivel", p.mShrivel},
        {"have_shrykull", p.mHaveShrykull},
        {"have_invisibility", p.bHaveInvisiblity},
        {"prev_input", p.mPrevInput},
        {"released_buttons", p.mReleasedButtons},
        {"knockdown_motion", p.mKnockdownMotion},
        {"rolling_motion_timer", p.mRollingMotionTimer},
        {"death_fade_out_id", p.mDeathFadeOutId},
        {"circular_fade_id", p.mCircularFadeId},
        {"orb_whirlwind_id", p.mOrbWhirlWindId},
        {"possessed_object_id", p.mPossessedObjectId},
        {"throwable_id", p.mThrowableId},
        {"invisibility_timer", p.mInvisibilityTimer},
        {"invisibility_duration", p.mInvisibilityDuration},
        {"handstone_cam_idx", p.mHandStoneCamIdx},
        {"handstone_type", p.mHandStoneType},
        {"fmv_id", p.mFmvId},
        {"handstone_cam_1", p.mHandStoneCam1},
        {"handstone_cam_2", p.mHandStoneCam2},
        {"handstone_cam_3", p.mHandStoneCam3},
        {"has_evil_fart", p.mHasEvilFart},
        {"dest_well_level", p.mDstWellLevel},
        {"dest_well_path", p.mDstWellPath},
        {"dest_well_camera", p.mDstWellCamera},
        {"door_id", p.door_id},
        {"throw_direction", p.mThrowDirection},
        {"bird_portal_sub_state", p.mBirdPortalSubState},
        {"bird_portal_id", p.mBirdPortalId},
        {"return_to_previous_motion", p.mReturnToPreviousMotion},
        {"prevent_chanting", p.mPreventChanting},
        {"land_softly", p.mLandSoftly},
        {"laugh_at_chant_end", p.mLaughAtChantEnd},
        {"play_ledge_grab_sounds", p.mPlayLedgeGrabSounds},
        {"have_healing", p.mHaveHealing},
        {"teleporting", p.mTeleporting},
        {"mudanchee_done", p.mMudancheeDone},
        {"mudomo_done", p.mMudomoDone},
        {"shadow_enabled", p.mShadowEnabled},
        {"shadow_at_bottom", p.mShadowAtBottom},
    };
}

inline void from_json(const nlohmann::json& j, AbeSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("xpos").get_to(p.mXPos);
    j.at("ypos").get_to(p.mYPos);
    j.at("velx").get_to(p.mVelX);
    j.at("vely").get_to(p.mVelY);
    j.at("current_path").get_to(p.mCurrentPath);
    j.at("current_level").get_to(p.mCurrentLevel);
    j.at("sprite_scale").get_to(p.mSpriteScale);
    j.at("scale").get_to(p.mScale);
    j.at("red").get_to(p.mRed);
    j.at("green").get_to(p.mGreen);
    j.at("blue").get_to(p.mBlue);
    j.at("anim_flip_x").get_to(p.bAnimFlipX);
    j.at("current_motion").get_to(p.mCurrentMotion);
    j.at("current_frame").get_to(p.mCurrentFrame);
    j.at("frame_change_counter").get_to(p.mFrameChangeCounter);
    j.at("render_layer").get_to(p.mRenderLayer);
    j.at("anim_render").get_to(p.mAnimRender);
    j.at("is_drawable").get_to(p.mIsDrawable);
    j.at("health").get_to(p.mHealth);
    j.at("current_motion_2").get_to(p.mCurrentMotion2);
    j.at("next_motion").get_to(p.mNextMotion);
    j.at("last_line_y_pos").get_to(p.mLastLineYPos);
    j.at("collision_line_type").get_to(p.mCollisionLineType);
    j.at("platform_id").get_to(p.mPlatformId);
    j.at("is_electrocuted").get_to(p.mIsElectrocuted);
    j.at("is_invisible").get_to(p.mIsInvisible);
    j.at("is_abe_controlled").get_to(p.mIsAbeControlled);
    j.at("x_vel_slow_by").get_to(p.field_48_x_vel_slow_by);
    j.at("state").get_to(p.field_50_state);
    j.at("timer").get_to(p.field_54_timer);
    j.at("abe_timer").get_to(p.field_58_abe_timer);
    j.at("regen_health_timer").get_to(p.mRegenHealthTimer);
    j.at("mood").get_to(p.mMood);
    j.at("say").get_to(p.mSay);
    j.at("auto_say_timer").get_to(p.mAutoSayTimer);
    j.at("ring_pulse_timer").get_to(p.mRingPulseTimer);
    j.at("base_throwable_count").get_to(p.mBaseThrowableCount);
    j.at("shrivel").get_to(p.mShrivel);
    j.at("have_shrykull").get_to(p.mHaveShrykull);
    j.at("have_invisibility").get_to(p.bHaveInvisiblity);
    j.at("prev_input").get_to(p.mPrevInput);
    j.at("released_buttons").get_to(p.mReleasedButtons);
    j.at("knockdown_motion").get_to(p.mKnockdownMotion);
    j.at("rolling_motion_timer").get_to(p.mRollingMotionTimer);
    j.at("death_fade_out_id").get_to(p.mDeathFadeOutId);
    j.at("circular_fade_id").get_to(p.mCircularFadeId);
    j.at("orb_whirlwind_id").get_to(p.mOrbWhirlWindId);
    j.at("possessed_object_id").get_to(p.mPossessedObjectId);
    j.at("throwable_id").get_to(p.mThrowableId);
    j.at("invisibility_timer").get_to(p.mInvisibilityTimer);
    j.at("invisibility_duration").get_to(p.mInvisibilityDuration);
    j.at("handstone_cam_idx").get_to(p.mHandStoneCamIdx);
    j.at("handstone_type").get_to(p.mHandStoneType);
    j.at("fmv_id").get_to(p.mFmvId);
    j.at("handstone_cam_1").get_to(p.mHandStoneCam1);
    j.at("handstone_cam_2").get_to(p.mHandStoneCam2);
    j.at("handstone_cam_3").get_to(p.mHandStoneCam3);
    j.at("has_evil_fart").get_to(p.mHasEvilFart);
    j.at("dest_well_level").get_to(p.mDstWellLevel);
    j.at("dest_well_path").get_to(p.mDstWellPath);
    j.at("dest_well_camera").get_to(p.mDstWellCamera);
    j.at("door_id").get_to(p.door_id);
    j.at("throw_direction").get_to(p.mThrowDirection);
    j.at("bird_portal_sub_state").get_to(p.mBirdPortalSubState);
    j.at("bird_portal_id").get_to(p.mBirdPortalId);
    j.at("return_to_previous_motion").get_to(p.mReturnToPreviousMotion);
    j.at("prevent_chanting").get_to(p.mPreventChanting);
    j.at("land_softly").get_to(p.mLandSoftly);
    j.at("laugh_at_chant_end").get_to(p.mLaughAtChantEnd);
    j.at("play_ledge_grab_sounds").get_to(p.mPlayLedgeGrabSounds);
    j.at("have_healing").get_to(p.mHaveHealing);
    j.at("teleporting").get_to(p.mTeleporting);
    j.at("mudanchee_done").get_to(p.mMudancheeDone);
    j.at("mudomo_done").get_to(p.mMudomoDone);
    j.at("shadow_enabled").get_to(p.mShadowEnabled);
    j.at("shadow_at_bottom").get_to(p.mShadowAtBottom);
}
inline void to_json(nlohmann::json& j, const LiftPointSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"xpos", p.mXPos},
        {"ypos", p.mYPos},
        {"platform_id", p.mPlatformId},
        {"tlv_id", p.mTlvId},
        {"floor_level_y", p.mFloorLevelY},
        {"lift_point_stop_type", p.mLiftPointStopType},
        {"moving", p.mMoving},
        {"top_floor", p.mTopFloor},
        {"middle_floor", p.mMiddleFloor},
        {"bottom_floor", p.mBottomFloor},
        {"move_to_floor_level", p.mMoveToFloorLevel},
        {"keep_on_middle_floor", p.mKeepOnMiddleFloor},
    };
}

inline void from_json(const nlohmann::json& j, LiftPointSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("xpos").get_to(p.mXPos);
    j.at("ypos").get_to(p.mYPos);
    j.at("platform_id").get_to(p.mPlatformId);
    j.at("tlv_id").get_to(p.mTlvId);
    j.at("floor_level_y").get_to(p.mFloorLevelY);
    j.at("lift_point_stop_type").get_to(p.mLiftPointStopType);
    j.at("moving").get_to(p.mMoving);
    j.at("top_floor").get_to(p.mTopFloor);
    j.at("middle_floor").get_to(p.mMiddleFloor);
    j.at("bottom_floor").get_to(p.mBottomFloor);
    j.at("move_to_floor_level").get_to(p.mMoveToFloorLevel);
    j.at("keep_on_middle_floor").get_to(p.mKeepOnMiddleFloor);
}

NLOHMANN_JSON_SERIALIZE_ENUM(MudAction, {
    {MudAction::eHelloOrAllYa_0, "hello_or_allya"},
    {MudAction::eFollowMe_1, "follow_me"},
    {MudAction::eWait_2, "wait"},
    {MudAction::eUnused_3, "unused_3"},
    {MudAction::eStopIt_4, "stop_it"},
    {MudAction::eUnused_5, "unused_5"},
    {MudAction::eFart_6, "fart"},
    {MudAction::eSlapOrWater_7, "slap_or_water"},
    {MudAction::eSorry_8, "sorry"},
    {MudAction::eMudAbuse_9, "mud_abuse"},
    {MudAction::eComfort_10, "comfort"},
    {MudAction::eSmackIntoWall_11, "smack_into_wall"},
    {MudAction::eLaugh_12, "laugh"},
    {MudAction::eDuck_13, "duck"},
    {MudAction::eMudDied_14, "mud_died"},
    {MudAction::eUnknown_15, "unknown_15"},
    {MudAction::eUnknown_16, "unknown_16"},
    {MudAction::eNone_17, "none"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(Mud_Brain_State, {
    {Mud_Brain_State::Brain_0_GiveRings, "give_rings"},
    {Mud_Brain_State::Brain_1_Chisel, "chisel"},
    {Mud_Brain_State::Brain_2_CrouchScrub, "crouch_scrub"},
    {Mud_Brain_State::Brain_3_TurnWheel, "turn_wheel"},
    {Mud_Brain_State::Brain_4_ListeningToAbe, "listening_to_abe"},
    {Mud_Brain_State::Brain_5_ShrivelDeath, "shrivel_death"},
    {Mud_Brain_State::Brain_6_Escape, "escape"},
    {Mud_Brain_State::Brain_7_FallAndSmackDeath, "fall_and_smack_death"},
    {Mud_Brain_State::Brain_8_AngryWorker, "angry_worker"},
    {Mud_Brain_State::Brain_9_Sick, "sick"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(eMudMotions, {
    {eMudMotions::None_m1, "none"},
    {eMudMotions::Motion_0_Idle, "idle"},
    {eMudMotions::Motion_1_WalkLoop, "walk_loop"},
    {eMudMotions::Motion_2_StandingTurn, "standing_turn"},
    {eMudMotions::M_Speak_3_472FA0, "speak_3"},
    {eMudMotions::M_Speak_4_472FA0, "speak_4"},
    {eMudMotions::M_Speak_5_472FA0, "speak_5"},
    {eMudMotions::M_Speak_6_472FA0, "speak_6"},
    {eMudMotions::Motion_7_WalkBegin, "walk_begin"},
    {eMudMotions::Motion_8_WalkToIdle, "walk_to_idle"},
    {eMudMotions::Motion_9_MidWalkToIdle, "mid_walk_to_idle"},
    {eMudMotions::Motion_10_LeverUse, "lever_use"},
    {eMudMotions::Motion_11_Chisel, "chisel"},
    {eMudMotions::Motion_12_StartChisel, "start_chisel"},
    {eMudMotions::Motion_13_StopChisel, "stop_chisel"},
    {eMudMotions::Motion_14_CrouchScrub, "crouch_scrub"},
    {eMudMotions::Motion_15_CrouchIdle, "crouch_idle"},
    {eMudMotions::Motion_16_CrouchTurn, "crouch_turn"},
    {eMudMotions::Motion_17_StandToCrouch, "stand_to_crouch"},
    {eMudMotions::Motion_18_CrouchToStand, "crouch_to_stand"},
    {eMudMotions::Motion_19_WalkToRun, "walk_to_run"},
    {eMudMotions::Motion_20_MidWalkToRun, "mid_walk_to_run"},
    {eMudMotions::Motion_21_RunLoop, "run_loop"},
    {eMudMotions::Motion_22_RunToWalk, "run_to_walk"},
    {eMudMotions::Motion_23_MidRunToWalk, "mid_run_to_walk"},
    {eMudMotions::Motion_24_RunSlideStop, "run_slide_stop"},
    {eMudMotions::Motion_25_RunSlideTurn, "run_slide_turn"},
    {eMudMotions::Motion_26_RunTurnToRun, "run_turn_to_run"},
    {eMudMotions::Motion_27_SneakLoop, "sneak_loop"},
    {eMudMotions::Motion_28_MidWalkToSneak, "mid_walk_to_sneak"},
    {eMudMotions::Motion_29_SneakToWalk, "sneak_to_walk"},
    {eMudMotions::Motion_30_WalkToSneak, "walk_to_sneak"},
    {eMudMotions::Motion_31_MidSneakToWalk, "mid_sneak_to_walk"},
    {eMudMotions::Motion_32_SneakBegin, "sneak_begin"},
    {eMudMotions::Motion_33_SneakToIdle, "sneak_to_idle"},
    {eMudMotions::Motion_34_MidSneakToIdle, "mid_sneak_to_idle"},
    {eMudMotions::Motion_35_RunJumpBegin, "run_jump_begin"},
    {eMudMotions::Motion_36_RunJumpMid, "run_jump_mid"},
    {eMudMotions::Motion_37_StandToRun, "stand_to_run"},
    {eMudMotions::Motion_38_Punch, "punch"},
    {eMudMotions::Motion_39_HoistBegin, "hoist_begin"},
    {eMudMotions::Motion_40_HoistLand, "hoist_land"},
    {eMudMotions::Motion_41_LandSoft1, "land_soft1"},
    {eMudMotions::Motion_42_LandSoft2, "land_soft2"},
    {eMudMotions::Motion_43_DunnoBegin, "dunno_begin"},
    {eMudMotions::Motion_44_DunnoEnd, "dunno_end"},
    {eMudMotions::Motion_45_KnockForward, "knock_forward"},
    {eMudMotions::Motion_46_Knockback, "knockback"},
    {eMudMotions::Motion_47_KnockbackGetUp, "knockback_get_up"},
    {eMudMotions::Motion_48_WalkOffEdge, "walk_off_edge"},
    {eMudMotions::Motion_49_Fall, "fall"},
    {eMudMotions::Motion_50_Chant, "chant"},
    {eMudMotions::Motion_51_ChantEnd, "chant_end"},
    {eMudMotions::Motion_52_ToDuck, "to_duck"},
    {eMudMotions::Motion_53_Duck, "duck"},
    {eMudMotions::Motion_54_DuckToCrouch, "duck_to_crouch"},
    {eMudMotions::Motion_55_DuckKnockback, "duck_knockback"},
    {eMudMotions::Motion_56_SlapOwnHead, "slap_own_head"},
    {eMudMotions::Motion_57_TurnWheelBegin, "turn_wheel_begin"},
    {eMudMotions::Motion_58_TurnWheelLoop, "turn_wheel_loop"},
    {eMudMotions::Motion_59_TurnWheelEnd, "turn_wheel_end"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(GameSpeakEvents, {
    {GameSpeakEvents::eSameAsLast, "same_as_last"},
    {GameSpeakEvents::eNone, "none"},
    {GameSpeakEvents::eUnknown_0, "unknown_0"},
    {GameSpeakEvents::eUnknown_1, "unknown_1"},
    {GameSpeakEvents::eAbe_Fart, "fart"},
    {GameSpeakEvents::eSlig_BS, "slig_bs"},
    {GameSpeakEvents::eSlig_LookOut, "slig_look_out"},
    {GameSpeakEvents::eSlig_BS2, "slig_bs2"},
    {GameSpeakEvents::eSlig_Laugh, "slig_laugh"},
    {GameSpeakEvents::eAbe_Hello, "hello"},
    {GameSpeakEvents::eAbe_FollowMe, "follow_me"},
    {GameSpeakEvents::eAbe_Anger, "anger"},
    {GameSpeakEvents::eAbe_Wait, "wait"},
    {GameSpeakEvents::eAbe_Work, "work"},
    {GameSpeakEvents::eAbe_StopIt, "stop_it"},
    {GameSpeakEvents::eAbe_AllYa, "allya"},
    {GameSpeakEvents::eAbe_Sorry, "sorry"},
    {GameSpeakEvents::eSlig_Hi, "slig_hi"},
    {GameSpeakEvents::eSlig_HereBoy, "slig_here_boy"},
    {GameSpeakEvents::eSlig_GetEm, "slig_get_em"},
    {GameSpeakEvents::eSlig_Freeze, "slig_freeze"},
    {GameSpeakEvents::eGlukkon_None, "unknown_35"},
    {GameSpeakEvents::eGlukkon_Hey, "glukkon_hey"},
    {GameSpeakEvents::eGlukkon_DoIt, "glukkon_do_it"},
    {GameSpeakEvents::eGlukkon_StayHere, "glukkon_stay_here"},
    {GameSpeakEvents::eGlukkon_Commere, "glukkon_commere"},
    {GameSpeakEvents::eGlukkon_AllOYa, "glukkon_alloya"},
    {GameSpeakEvents::eGlukkon_Heh, "glukkon_heh"},
    {GameSpeakEvents::eGlukkon_Help, "glukkon_help"},
    {GameSpeakEvents::eGlukkon_Laugh, "glukkon_laugh"},
    {GameSpeakEvents::eGlukkon_KillEm, "glukkon_kill_em"},
    {GameSpeakEvents::eGlukkon_Unknown, "glukkon_unknown_45"},
    {GameSpeakEvents::eGlukkon_What, "glukkon_what"},
    {GameSpeakEvents::eParamite_Howdy, "paramite_howdy"},
    {GameSpeakEvents::eParamite_Stay, "paramite_stay"},
    {GameSpeakEvents::eParamite_CMon_or_Attack, "paramite_cmon_or_attack"},
    {GameSpeakEvents::eParamite_DoIt, "paramite_do_it"},
    {GameSpeakEvents::eParamite_AllYa, "paramite_allya"},
    {GameSpeakEvents::eScrab_Howl, "scrab_howl"},
    {GameSpeakEvents::eScrab_Shriek, "scrab_shriek"},
})

inline void to_json(nlohmann::json& j, const MudokonSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"xpos", p.field_4_xpos},
        {"ypos", p.field_8_ypos},
        {"velx", p.field_C_velx},
        {"vely", p.field_10_vely},
        {"path_number", p.field_14_path_number},
        {"lvl_number", p.field_16_lvl_number},
        {"sprite_scale", p.field_18_sprite_scale},
        {"r", p.field_1C_r},
        {"g", p.field_1E_g},
        {"b", p.field_20_b},
        {"flip_x", p.field_22_bFlipX},
        {"field_24_current_motion", p.field_24_current_motion},
        {"anim_current_frame", p.field_26_anim_current_frame},
        {"anim_frame_change_counter", p.field_28_anim_frame_change_counter},
        {"anim_render", p.field_2A_bAnimRender},
        {"drawable", p.field_2B_bDrawable},
        {"health", p.field_2C_health},
        {"field_30_current_motion", p.field_30_current_motion},
        {"next_motion", p.field_32_next_motion},
        {"last_line_ypos", p.field_34_lastLineYPos},
        {"line_type", p.field_36_line_type},
        {"can_be_possessed", p.field_3C_can_be_possessed},
        {"is_player", p.field_3D_bIsPlayer},
        {"tlv_info", p.field_40_tlvInfo},
        {"velx_slow_by", p.field_44_velx_slow_by},
        {"portal_id", p.field_4C_portal_id},
        {"angry_trigger", p.field_50_angry_trigger},
        {"laugh_and_crouch_timer", p.field_54_laugh_and_crouch_timer},
        {"angry_timer", p.field_58_angry_timer},
        {"voice_pitch", p.field_5E_voice_pitch},
        {"wheel_id", p.field_60_wheel_id},
        {"field_68", p.field_68},
        {"max_x_offset", p.field_6A_maxXOffset},
        {"not_rescued", p.mNotRescued},
        {"persist_and_reset_offscreen", p.mPersistAndResetOffscreen},
        {"alerted", p.mAlerted},
        {"blind", p.mBlind},
        {"following_abe", p.mFollowingAbe},
        {"standing_for_sad_or_angry", p.mStandingForSadOrAngry},
        {"stopped_at_wheel", p.mStoppedAtWheel},
        {"do_angry", p.mDoAngry},
        {"seen_while_sick", p.mSeenWhileSick},
        {"work_after_turning_wheel", p.mWorkAfterTurningWheel},
        {"return_to_previous_motion", p.mReturnToPreviousMotion},
        {"get_depressed", p.mGetDepressed},
        {"alert_enemies", p.mAlertEnemies},
        {"noise_unknown", p.mNoiseUnknown},
        {"make_sad_noise", p.mMakeSadNoise},
        {"ring_and_angry_mud_timeout", p.mRingAndAngryMudTimeout},
        {"abe_has_ring", p.mAbeHasRing},
        {"is_mud_standing_up2", p.mIsMudStandingUp2},
        {"brain_sub_state2", p.field_70_brain_sub_state2},
        {"stand_idle_timer", p.field_72_stand_idle_timer},
        {"delayed_speak", p.field_74_delayed_speak},
        {"emo_tlb", p.field_76_emo_tlb},
        {"speak_event", p.field_78_speak_event},
        {"motion", p.field_7A_motion},
        {"brain_state", p.field_7C_brain_state},
        {"brain_sub_state", p.field_7E_brain_sub_state},
        {"timer", p.field_80_timer},
        {"response_entry_idx", p.field_84_response_entry_idx},
    };
}

inline void from_json(const nlohmann::json& j, MudokonSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("xpos").get_to(p.field_4_xpos);
    j.at("ypos").get_to(p.field_8_ypos);
    j.at("velx").get_to(p.field_C_velx);
    j.at("vely").get_to(p.field_10_vely);
    j.at("path_number").get_to(p.field_14_path_number);
    j.at("lvl_number").get_to(p.field_16_lvl_number);
    j.at("sprite_scale").get_to(p.field_18_sprite_scale);
    j.at("r").get_to(p.field_1C_r);
    j.at("g").get_to(p.field_1E_g);
    j.at("b").get_to(p.field_20_b);
    j.at("flip_x").get_to(p.field_22_bFlipX);
    j.at("field_24_current_motion").get_to(p.field_24_current_motion);
    j.at("anim_current_frame").get_to(p.field_26_anim_current_frame);
    j.at("anim_frame_change_counter").get_to(p.field_28_anim_frame_change_counter);
    j.at("anim_render").get_to(p.field_2A_bAnimRender);
    j.at("drawable").get_to(p.field_2B_bDrawable);
    j.at("health").get_to(p.field_2C_health);
    j.at("field_30_current_motion").get_to(p.field_30_current_motion);
    j.at("next_motion").get_to(p.field_32_next_motion);
    j.at("last_line_ypos").get_to(p.field_34_lastLineYPos);
    j.at("line_type").get_to(p.field_36_line_type);
    j.at("can_be_possessed").get_to(p.field_3C_can_be_possessed);
    j.at("is_player").get_to(p.field_3D_bIsPlayer);
    j.at("tlv_info").get_to(p.field_40_tlvInfo);
    j.at("velx_slow_by").get_to(p.field_44_velx_slow_by);
    j.at("portal_id").get_to(p.field_4C_portal_id);
    j.at("angry_trigger").get_to(p.field_50_angry_trigger);
    j.at("laugh_and_crouch_timer").get_to(p.field_54_laugh_and_crouch_timer);
    j.at("angry_timer").get_to(p.field_58_angry_timer);
    j.at("voice_pitch").get_to(p.field_5E_voice_pitch);
    j.at("wheel_id").get_to(p.field_60_wheel_id);
    j.at("field_68").get_to(p.field_68);
    j.at("max_x_offset").get_to(p.field_6A_maxXOffset);
    j.at("not_rescued").get_to(p.mNotRescued);
    j.at("persist_and_reset_offscreen").get_to(p.mPersistAndResetOffscreen);
    j.at("alerted").get_to(p.mAlerted);
    j.at("blind").get_to(p.mBlind);
    j.at("following_abe").get_to(p.mFollowingAbe);
    j.at("standing_for_sad_or_angry").get_to(p.mStandingForSadOrAngry);
    j.at("stopped_at_wheel").get_to(p.mStoppedAtWheel);
    j.at("do_angry").get_to(p.mDoAngry);
    j.at("seen_while_sick").get_to(p.mSeenWhileSick);
    j.at("work_after_turning_wheel").get_to(p.mWorkAfterTurningWheel);
    j.at("return_to_previous_motion").get_to(p.mReturnToPreviousMotion);
    j.at("get_depressed").get_to(p.mGetDepressed);
    j.at("alert_enemies").get_to(p.mAlertEnemies);
    j.at("noise_unknown").get_to(p.mNoiseUnknown);
    j.at("make_sad_noise").get_to(p.mMakeSadNoise);
    j.at("ring_and_angry_mud_timeout").get_to(p.mRingAndAngryMudTimeout);
    j.at("abe_has_ring").get_to(p.mAbeHasRing);
    j.at("is_mud_standing_up2").get_to(p.mIsMudStandingUp2);
    j.at("brain_sub_state2").get_to(p.field_70_brain_sub_state2);
    j.at("stand_idle_timer").get_to(p.field_72_stand_idle_timer);
    j.at("delayed_speak").get_to(p.field_74_delayed_speak);
    j.at("emo_tlb").get_to(p.field_76_emo_tlb);
    j.at("speak_event").get_to(p.field_78_speak_event);
    j.at("motion").get_to(p.field_7A_motion);
    j.at("brain_state").get_to(p.field_7C_brain_state);
    j.at("brain_sub_state").get_to(p.field_7E_brain_sub_state);
    j.at("timer").get_to(p.field_80_timer);
    j.at("response_entry_idx").get_to(p.field_84_response_entry_idx);
}

NLOHMANN_JSON_SERIALIZE_ENUM(MeatStates, {
    {MeatStates::eCreated_0, "created"},
    {MeatStates::eIdle_1, "idle"},
    {MeatStates::eBeingThrown_2, "being_thrown"},
    {MeatStates::eBecomeAPickUp_3, "become_a_pick_up"},
    {MeatStates::eWaitForPickUp_4, "wait_for_pick_up"},
    {MeatStates::eFall_5, "fall"},
})

inline void to_json(nlohmann::json& j, const MeatSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"tlv_id", p.mTlvId},
        {"xpos", p.mXPos},
        {"ypos", p.mYPos},
        {"velx", p.mVelX},
        {"vely", p.mVelY},
        {"sprite_scale", p.mSpriteScale},
        {"current_path", p.mCurrentPath},
        {"current_level", p.mCurrentLevel},
        {"render", p.mRender},
        {"drawable", p.mDrawable},
        {"loop", p.mLoop},
        {"interactive", p.mInteractive},
        {"platform_id", p.mPlatformId},
        {"line_type", p.mLineType},
        {"throwable_count", p.mThrowableCount},
        {"state", p.mState},
        {"previous_xpos", p.mPreviousXPos},
        {"previous_ypos", p.mPreviousYPos},
        {"dead_timer", p.mDeadTimer},
    };
}

inline void from_json(const nlohmann::json& j, MeatSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("tlv_id").get_to(p.mTlvId);
    j.at("xpos").get_to(p.mXPos);
    j.at("ypos").get_to(p.mYPos);
    j.at("velx").get_to(p.mVelX);
    j.at("vely").get_to(p.mVelY);
    j.at("sprite_scale").get_to(p.mSpriteScale);
    j.at("current_path").get_to(p.mCurrentPath);
    j.at("current_level").get_to(p.mCurrentLevel);
    j.at("render").get_to(p.mRender);
    j.at("drawable").get_to(p.mDrawable);
    j.at("loop").get_to(p.mLoop);
    j.at("interactive").get_to(p.mInteractive);
    j.at("platform_id").get_to(p.mPlatformId);
    j.at("line_type").get_to(p.mLineType);
    j.at("throwable_count").get_to(p.mThrowableCount);
    j.at("state").get_to(p.mState);
    j.at("previous_xpos").get_to(p.mPreviousXPos);
    j.at("previous_ypos").get_to(p.mPreviousYPos);
    j.at("dead_timer").get_to(p.mDeadTimer);
}

NLOHMANN_JSON_SERIALIZE_ENUM(MineCarStates, {
    {MineCarStates::eParkedWithoutAbe_0, "parked_without_abe"},
    {MineCarStates::eParkedWithAbe_1, "parked_with_abe"},
    {MineCarStates::eMoving_2, "moving"},
    {MineCarStates::eFalling_3, "falling"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(MineCarDirs, {
    {MineCarDirs::eDown_0, "down"},
    {MineCarDirs::eRight_1, "right"},
    {MineCarDirs::eLeft_2, "left"},
    {MineCarDirs::eUp_3, "up"},
})

inline void to_json(nlohmann::json& j, const MineCarSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"xpos", p.field_4_xpos},
        {"ypos", p.field_8_ypos},
        {"velx", p.field_C_velx},
        {"vely", p.field_10_vely},
        {"sprite_scale", p.field_14_sprite_scale},
        {"path_number", p.field_18_path_number},
        {"lvl_number", p.field_1A_lvl_number},
        {"r", p.field_1C_r},
        {"g", p.field_1E_g},
        {"b", p.field_20_b},
        {"x_flip", p.field_22_xFlip},
        {"frame_table", p.field_24_frame_table},
        {"current_anim_frame", p.field_2A_current_anim_frame},
        {"frame_change_counter", p.field_2C_frame_change_counter},
        {"render", p.field_2E_render},
        {"drawable", p.field_2F_drawable},
        {"frame_table_offset2", p.field_38_frame_table_offset2},
        {"health", p.field_3C_health},
        {"last_line_ypos", p.field_44_last_line_ypos},
        {"collision_line_type", p.field_46_collision_line_type},
        {"tlv_info", p.field_4C_tlvInfo},
        {"state", p.field_50_state},
        {"turn_direction", p.field_52_turn_direction},
        {"falling_counter", p.field_58_falling_counter},
        {"abe_in_car", p.field_5A_bAbeInCar},
        {"frame_mod_16", p.field_5C_frame_mod_16},
        {"spawned_path", p.field_60_spawned_path},
        {"spawned_camera", p.field_62_spawned_camera},
        {"throw_item_key1", p.field_64_throw_item_key1},
        {"continue_move_input", p.field_66_continue_move_input},
    };
}

inline void from_json(const nlohmann::json& j, MineCarSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("xpos").get_to(p.field_4_xpos);
    j.at("ypos").get_to(p.field_8_ypos);
    j.at("velx").get_to(p.field_C_velx);
    j.at("vely").get_to(p.field_10_vely);
    j.at("sprite_scale").get_to(p.field_14_sprite_scale);
    j.at("path_number").get_to(p.field_18_path_number);
    j.at("lvl_number").get_to(p.field_1A_lvl_number);
    j.at("r").get_to(p.field_1C_r);
    j.at("g").get_to(p.field_1E_g);
    j.at("b").get_to(p.field_20_b);
    j.at("x_flip").get_to(p.field_22_xFlip);
    j.at("frame_table").get_to(p.field_24_frame_table);
    j.at("current_anim_frame").get_to(p.field_2A_current_anim_frame);
    j.at("frame_change_counter").get_to(p.field_2C_frame_change_counter);
    j.at("render").get_to(p.field_2E_render);
    j.at("drawable").get_to(p.field_2F_drawable);
    j.at("frame_table_offset2").get_to(p.field_38_frame_table_offset2);
    j.at("health").get_to(p.field_3C_health);
    j.at("last_line_ypos").get_to(p.field_44_last_line_ypos);
    j.at("collision_line_type").get_to(p.field_46_collision_line_type);
    j.at("tlv_info").get_to(p.field_4C_tlvInfo);
    j.at("state").get_to(p.field_50_state);
    j.at("turn_direction").get_to(p.field_52_turn_direction);
    j.at("falling_counter").get_to(p.field_58_falling_counter);
    j.at("abe_in_car").get_to(p.field_5A_bAbeInCar);
    j.at("frame_mod_16").get_to(p.field_5C_frame_mod_16);
    j.at("spawned_path").get_to(p.field_60_spawned_path);
    j.at("spawned_camera").get_to(p.field_62_spawned_camera);
    j.at("throw_item_key1").get_to(p.field_64_throw_item_key1);
    j.at("continue_move_input").get_to(p.field_66_continue_move_input);
}

NLOHMANN_JSON_SERIALIZE_ENUM(eParamiteMotions, {
    {eParamiteMotions::Motion_0_Idle, "idle"},
    {eParamiteMotions::Motion_1_WalkBegin, "walk_begin"},
    {eParamiteMotions::Motion_2_Walking, "walking"},
    {eParamiteMotions::Motion_3_Running, "running"},
    {eParamiteMotions::Motion_4_Turn, "turn"},
    {eParamiteMotions::Motion_5_Hop, "hop"},
    {eParamiteMotions::Motion_6_Unused, "motion_6_unused"},
    {eParamiteMotions::Motion_7_WalkRunTransition, "walk_run_transition"},
    {eParamiteMotions::Motion_8_WalkEnd, "walk_end"},
    {eParamiteMotions::Motion_9_RunBegin, "run_begin"},
    {eParamiteMotions::Motion_10_RunEnd, "run_end"},
    {eParamiteMotions::Motion_11_Falling, "falling"},
    {eParamiteMotions::Motion_12_JumpUpBegin, "jump_up_begin"},
    {eParamiteMotions::Motion_13_JumpUpMidair, "jump_up_midair"},
    {eParamiteMotions::Motion_14_JumpUpLand, "jump_up_land"},
    {eParamiteMotions::Motion_15_RopePull, "rope_pull"},
    {eParamiteMotions::Motion_16_CloseAttack, "close_attack"},
    {eParamiteMotions::Motion_17_Landing, "landing"},
    {eParamiteMotions::Motion_18_Unused, "motion_18_unused"},
    {eParamiteMotions::Motion_19_Knockback, "knockback"},
    {eParamiteMotions::Motion_20_GameSpeakBegin, "gamespeak_begin"},
    {eParamiteMotions::Motion_21_PreHiss, "pre_hiss"},
    {eParamiteMotions::Motion_22_Hiss1, "hiss1"},
    {eParamiteMotions::Motion_23_Hiss2, "hiss2"},
    {eParamiteMotions::Motion_24_Empty, "motion_24_empty"},
    {eParamiteMotions::Motion_25_AllOYaGameSpeakBegin, "all_o_ya_gamespeak_begin"},
    {eParamiteMotions::Motion_26_Hiss3, "hiss3"},
    {eParamiteMotions::Motion_27_PostHiss, "post_hiss"},
    {eParamiteMotions::Motion_28_GameSpeakEnd, "gamespeak_end"},
    {eParamiteMotions::Motion_29_GetDepossessedBegin, "get_depossessed_begin"},
    {eParamiteMotions::Motion_30_GetDepossessedEnd, "get_depossessed_end"},
    {eParamiteMotions::Motion_31_RunningAttack, "running_attack"},
    {eParamiteMotions::Motion_32_Empty, "motion_32_empty"},
    {eParamiteMotions::Motion_33_SurpriseWeb, "surprise_web"},
    {eParamiteMotions::Motion_34_WebLeaveDown, "web_leave_down"},
    {eParamiteMotions::Motion_35_WebIdle, "web_idle"},
    {eParamiteMotions::Motion_36_WebGoingUp, "web_going_up"},
    {eParamiteMotions::Motion_37_WebGoingDown, "web_going_down"},
    {eParamiteMotions::Motion_38_WebGrab, "web_grab"},
    {eParamiteMotions::Motion_39_WebLeaveUp, "web_leave_up"},
    {eParamiteMotions::Motion_40_Eating, "eating"},
    {eParamiteMotions::Motion_41_Death, "death"},
    {eParamiteMotions::Motion_42_Squawk, "squawk"},
    {eParamiteMotions::Motion_43_Attack, "attack"},
})

inline void to_json(nlohmann::json& j, const ParamiteSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"xpos", p.mXPos},
        {"ypos", p.mYPos},
        {"velx", p.mVelX},
        {"vely", p.mVelY},
        {"path_number", p.mCurrentPath},
        {"lvl_number", p.mCurrentLevel},
        {"sprite_scale", p.mSpriteScale},
        {"r", p.mR},
        {"g", p.mG},
        {"b", p.mB},
        {"x_flip", p.mFlipX},
        {"field_24_current_motion", p.field_24_current_motion},
        {"anim_current_frame", p.mAnimCurrentFrame},
        {"frame_change_counter", p.mFrameChangeCounter},
        {"render", p.mRender},
        {"drawable", p.mDrawable},
        {"health", p.mHealth},
        {"field_30_current_motion", p.field_30_current_motion},
        {"next_motion", p.field_32_next_motion},
        {"last_line_ypos", p.field_34_last_line_ypos},
        {"line_type", p.field_36_line_type},
        {"tlv_info", p.field_3C_tlvInfo},
        {"meat_id", p.field_40_meat_id},
        {"web_id", p.field_44_web_id},
        {"obj_id", p.field_48_obj_id},
        {"pull_ring_rope_id", p.field_4C_pull_ring_rope_id},
        {"brain_idx", p.mBrainIdx},
        {"brain_ret", p.mBrainSubState},
        {"field_5c_timer", p.field_5C_timer},
        {"depossession_timer", p.field_60_depossession_timer},
        {"velx_offset", p.field_64_velx_offset},
        {"field_68_timer", p.field_68_timer},
        {"return_level", p.mAbeLevel},
        {"return_path", p.mAbePath},
        {"return_camera", p.mAbeCamera},
        {"input", p.field_72_input},
        {"next_brain_ret", p.field_74_next_brain_ret},
        {"controlled", p.mControlled},
        {"running", p.mRunning},
        {"hissed_or_left_screen", p.mHissedOrLeftScreen},
        {"prevent_depossession", p.mPreventDepossession},
        {"spawned", p.mSpawned},
        {"alerted", p.mAlerted},
        {"can_be_possessed", p.mCanBePossessed},
    };
}

inline void from_json(const nlohmann::json& j, ParamiteSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("xpos").get_to(p.mXPos);
    j.at("ypos").get_to(p.mYPos);
    j.at("velx").get_to(p.mVelX);
    j.at("vely").get_to(p.mVelY);
    j.at("path_number").get_to(p.mCurrentPath);
    j.at("lvl_number").get_to(p.mCurrentLevel);
    j.at("sprite_scale").get_to(p.mSpriteScale);
    j.at("r").get_to(p.mR);
    j.at("g").get_to(p.mG);
    j.at("b").get_to(p.mB);
    j.at("x_flip").get_to(p.mFlipX);
    j.at("field_24_current_motion").get_to(p.field_24_current_motion);
    j.at("anim_current_frame").get_to(p.mAnimCurrentFrame);
    j.at("frame_change_counter").get_to(p.mFrameChangeCounter);
    j.at("render").get_to(p.mRender);
    j.at("drawable").get_to(p.mDrawable);
    j.at("health").get_to(p.mHealth);
    j.at("field_30_current_motion").get_to(p.field_30_current_motion);
    j.at("next_motion").get_to(p.field_30_current_motion);
    j.at("last_line_ypos").get_to(p.field_34_last_line_ypos);
    j.at("line_type").get_to(p.field_36_line_type);
    j.at("tlv_info").get_to(p.field_3C_tlvInfo);
    j.at("meat_id").get_to(p.field_40_meat_id);
    j.at("web_id").get_to(p.field_44_web_id);
    j.at("obj_id").get_to(p.field_48_obj_id);
    j.at("pull_ring_rope_id").get_to(p.field_4C_pull_ring_rope_id);
    j.at("brain_idx").get_to(p.mBrainIdx);
    j.at("brain_ret").get_to(p.mBrainSubState);
    j.at("field_5c_timer").get_to(p.field_5C_timer);
    j.at("depossession_timer").get_to(p.field_60_depossession_timer);
    j.at("velx_offset").get_to(p.field_64_velx_offset);
    j.at("field_68_timer").get_to(p.field_68_timer);
    j.at("return_level").get_to(p.mAbeLevel);
    j.at("return_path").get_to(p.mAbePath);
    j.at("return_camera").get_to(p.mAbeCamera);
    j.at("input").get_to(p.field_72_input);
    j.at("next_brain_ret").get_to(p.field_74_next_brain_ret);
    j.at("controlled").get_to(p.mControlled);
    j.at("running").get_to(p.mRunning);
    j.at("hissed_or_left_screen").get_to(p.mHissedOrLeftScreen);
    j.at("prevent_depossession").get_to(p.mPreventDepossession);
    j.at("spawned").get_to(p.mSpawned);
    j.at("alerted").get_to(p.mAlerted);
    j.at("can_be_possessed").get_to(p.mCanBePossessed);
}

NLOHMANN_JSON_SERIALIZE_ENUM(PortalStates, {
    {PortalStates::CreatePortal_0, "create_portal"},
    {PortalStates::IdlePortal_1, "idle_portal"},
    {PortalStates::JoinDovesInCenter_2, "join_doves_in_center"},
    {PortalStates::KillDoves_3, "kill_doves"},
    {PortalStates::CreateTerminators_4, "create_terminators"},
    {PortalStates::ExpandTerminators_5, "expand_terminators"},
    {PortalStates::ActivePortal_6, "active_portal"},
    {PortalStates::ShrykullGetDoves_7, "shrykull_get_doves"},
    {PortalStates::Unused_8, "unused_8"},
    {PortalStates::GetShrykull_9, "get_shrykull"},
    {PortalStates::CollapseTerminators_10, "collapse_terminators"},
    {PortalStates::StopSound_11, "stop_sound"},
    {PortalStates::CreateFlash1_12, "create_flash1"},
    {PortalStates::CreateFlash2_13, "create_flash2"},
    {PortalStates::CreateFlash3_14, "create_flash3"},
    {PortalStates::KillPortal_15, "kill_portal"},
    {PortalStates::AbeInsidePortal_16, "abe_inside_portal"},
    {PortalStates::PortalExit_SetPosition_17, "portal_exit_set_position"},
    {PortalStates::PortalExit_CreateTerminators_18, "portal_exit_create_terminators"},
    {PortalStates::PortalExit_ExpandTerminators_19, "portal_exit_expand_terminators"},
    {PortalStates::PortalExit_AbeExitting_20, "portal_exit_abe_exitting"},
    {PortalStates::KillPortalClipper_21, "kill_portal_clipper"},
    {PortalStates::FadeoutTerminators_22, "fadeout_terminators"},
})

inline void to_json(nlohmann::json& j, const BirdPortalSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"state", p.mState},
        {"mud_count_for_shrykull", p.mMudCountForShrykull},
        {"tlv_info", p.mTlvInfo},
    };
}

inline void from_json(const nlohmann::json& j, BirdPortalSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("state").get_to(p.mState);
    j.at("mud_count_for_shrykull").get_to(p.mMudCountForShrykull);
    j.at("tlv_info").get_to(p.mTlvInfo);
}

inline void to_json(nlohmann::json& j, const ThrowableArraySaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"count", p.mCount},
    };
}

inline void from_json(const nlohmann::json& j, ThrowableArraySaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("count").get_to(p.mCount);
}

NLOHMANN_JSON_SERIALIZE_ENUM(RingTypes, {
    {RingTypes::eExplosive_Pulse_0, "explosive_pulse"},
    {RingTypes::eExplosive_Emit_1, "explosive_emit"},
    {RingTypes::eExplosive_Emit_Effect_2, "explosive_emit_effect"},
    {RingTypes::eExplosive_Give_3, "explosive_give"},
    {RingTypes::eShrykull_Pulse_Small_4, "shrykull_pulse_small"},
    {RingTypes::eShrykull_Pulse_Large_5, "shrykull_pulse_large"},
    {RingTypes::eShrykull_Pulse_Orange_6, "shrykull_pulse_orange"},
    {RingTypes::eInvisible_Pulse_Small_7, "invisible_pulse_small"},
    {RingTypes::eInvisible_Pulse_Large_8, "invisible_pulse_large"},
    {RingTypes::eInvisible_Pulse_Emit_9, "invisible_pulse_emit"},
    {RingTypes::eInvisible_Pulse_Give_10, "invisible_pulse_give"},
    {RingTypes::eHealing_Emit_Effect_11, "healing_emit_effect"},
    {RingTypes::eHealing_Emit_12, "healing_emit"},
    {RingTypes::eHealing_Give_13, "healing_give"},
    {RingTypes::eHealing_Pulse_14, "healing_pulse"},
})

inline void to_json(nlohmann::json & j, const AbilityRingSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"ring_xpos", p.mRingXPos},
        {"ring_ypos", p.mRingYPos},
        {"ring_type", p.mRingType},
        {"ring_scale", p.mRingScale},
        {"ring_tlv_info", p.mRingTlvInfo},
        {"ring_right", p.mRingRight},
        {"ring_count", p.mRingCount},
        {"ring_red", p.mRingRed},
        {"ring_green", p.mRingGreen},
        {"ring_red", p.mRingBlue},
    };
}

inline void from_json(const nlohmann::json& j, AbilityRingSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("ring_xpos").get_to(p.mRingXPos);
    j.at("ring_ypos").get_to(p.mRingYPos);
    j.at("ring_type").get_to(p.mRingType);
    j.at("ring_scale").get_to(p.mRingScale);
    j.at("ring_tlv_info").get_to(p.mRingTlvInfo);
    j.at("ring_right").get_to(p.mRingRight);
    j.at("ring_count").get_to(p.mRingCount);
    j.at("ring_red").get_to(p.mRingRed);
    j.at("ring_green").get_to(p.mRingGreen);
    j.at("ring_red").get_to(p.mRingBlue);
}

NLOHMANN_JSON_SERIALIZE_ENUM(RockStates, {
    {RockStates::eNone_0, "none"},
    {RockStates::eFallingOutOfRockSack_1, "falling_out_of_rock_sack"},
    {RockStates::eRolling_2, "rolling"},
    {RockStates::eOnGround_3, "on_ground"},
    {RockStates::eBouncing_4, "bouncing"},
    {RockStates::eFallingOutOfWorld_5, "falling_out_of_world"},
})

inline void to_json(nlohmann::json& j, const RockSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"obj_id", p.mTlvId},
        {"field_8_xpos", p.mXPos},
        {"field_c_ypos", p.mYPos},
        {"velx", p.mVelX},
        {"vely", p.mVelY},
        {"sprite_scale", p.mSpriteScale},
        {"path_number", p.mCurrentPath},
        {"lvl_number", p.mCurrentLevel},
        {"render", p.mRender},
        {"drawable", p.mDrawable},
        {"loop", p.mLoop},
        {"interactive", p.mInteractive},
        {"id", p.mPlatformId},
        {"line_type", p.mCollisionLineType},
        {"count", p.mThrowableCount},
        {"state", p.mState},
        {"volume", p.mBounceCount},
        {"field_30_xpos", p.mPreviousXPos},
        {"field_34_ypos", p.mPreviousYPos},
    };
}

inline void from_json(const nlohmann::json& j, RockSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("obj_id").get_to(p.mTlvId);
    j.at("field_8_xpos").get_to(p.mXPos);
    j.at("field_c_ypos").get_to(p.mYPos);
    j.at("velx").get_to(p.mVelX);
    j.at("vely").get_to(p.mVelY);
    j.at("sprite_scale").get_to(p.mSpriteScale);
    j.at("path_number").get_to(p.mCurrentPath);
    j.at("lvl_number").get_to(p.mCurrentLevel);
    j.at("render").get_to(p.mRender);
    j.at("drawable").get_to(p.mDrawable);
    j.at("loop").get_to(p.mLoop);
    j.at("interactive").get_to(p.mInteractive);
    j.at("id").get_to(p.mPlatformId);
    j.at("line_type").get_to(p.mCollisionLineType);
    j.at("count").get_to(p.mThrowableCount);
    j.at("state").get_to(p.mState);
    j.at("volume").get_to(p.mBounceCount);
    j.at("field_30_xpos").get_to(p.mPreviousXPos);
    j.at("field_34_ypos").get_to(p.mPreviousYPos);
}

NLOHMANN_JSON_SERIALIZE_ENUM(eScrabMotions, {
    {eScrabMotions::Motion_0_Stand, "stand"},
    {eScrabMotions::Motion_1_Walk, "walk"},
    {eScrabMotions::Motion_2_Run, "run"},
    {eScrabMotions::Motion_3_Turn, "turn"},
    {eScrabMotions::Motion_4_RunToStand, "run_to_stand"},
    {eScrabMotions::Motion_5_HopBegin, "hop_begin"},
    {eScrabMotions::Motion_6_HopMidair, "hop_midair"},
    {eScrabMotions::Motion_7_HopLand, "hop_land"},
    {eScrabMotions::Motion_8_JumpToFall, "jump_to_fall"},
    {eScrabMotions::Motion_9_StandToWalk, "stand_to_walk"},
    {eScrabMotions::Motion_10_StandToRun, "stand_to_run"},
    {eScrabMotions::Motion_11_WalkToStand, "walk_to_stand"},
    {eScrabMotions::Motion_12_RunJumpBegin, "run_jump_begin"},
    {eScrabMotions::Motion_13_RunJumpEnd, "run_jump_end"},
    {eScrabMotions::Motion_14_WalkToFall, "walk_to_fall"},
    {eScrabMotions::Motion_15_RunToFall, "run_to_fall"},
    {eScrabMotions::Motion_16_WalkToRun , "walk_to_run"},
    {eScrabMotions::Motion_17_RunToWalk, "run_to_walk"},
    {eScrabMotions::Motion_18_Knockback, "knockback"},
    {eScrabMotions::Motion_19_GetEaten, "get_eaten"},
    {eScrabMotions::Motion_20_Fall, "fall"},
    {eScrabMotions::Motion_21_Stamp, "stamp"},
    {eScrabMotions::Motion_22_GetPossessed, "get_possessed"},
    {eScrabMotions::Motion_23_Empty, "motion_23_empty"},
    {eScrabMotions::Motion_24_DeathEnd, "death_end"},
    {eScrabMotions::Motion_25_Empty, "motion_25_empty"},
    {eScrabMotions::Motion_26_HowlBegin, "howl_begin"},
    {eScrabMotions::Motion_27_HowlEnd, "howl_end"},
    {eScrabMotions::Motion_28_GetDepossessedBegin, "get_depossessed_begin"},
    {eScrabMotions::Motion_29_GetDepossessedEnd, "get_depossessed_end"},
    {eScrabMotions::Motion_30_Shriek, "shriek"},
    {eScrabMotions::Motion_31_ScrabBattleAnim, "scrab_battle_anim"},
    {eScrabMotions::Motion_32_AttackSpin, "attack_spin"},
    {eScrabMotions::Motion_33_FeedToGulp, "feed_to_gulp"},
    {eScrabMotions::Motion_34_GulpToStand, "gulp_to_stand"},
    {eScrabMotions::Motion_35_StandToFeed, "stand_to_feed"},
    {eScrabMotions::Motion_36_Feed, "feed"},
    {eScrabMotions::Motion_37_AttackLunge, "attack_lunge"},
    {eScrabMotions::Motion_38_LegKick, "leg_kick"},
    {eScrabMotions::Motion_39_DeathBegin, "death_begin"},
})

inline void to_json(nlohmann::json& j, const ScrabSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"obj_id", p.field_4_obj_id},
        {"xpos", p.mXPos},
        {"ypos", p.mYPos},
        {"velx", p.mVelX},
        {"vely", p.mVelY},
        {"path_number", p.mCurrentPath},
        {"lvl_number", p.mCurrentLevel},
        {"sprite_scale", p.mSpriteScale},
        {"ring_red", p.mRingRed},
        {"ring_green", p.mRingGreen},
        {"ring_blue", p.mRingBlue},
        {"anim_flipX", p.mFlipX},
        {"field_28_current_motion", p.field_28_current_motion},
        {"current_frame", p.mCurrentFrame},
        {"frame_change_counter", p.mFrameChangeCounter},
        {"anim_render", p.mAnimRender},
        {"drawable", p.mDrawable},
        {"health", p.mHealth},
        {"field_34_current_motion", p.field_34_current_motion},
        {"next_motion", p.field_36_next_motion},
        {"last_line_ypos", p.field_38_last_line_ypos},
        {"line_type", p.mLineType},
        {"is_controlled", p.mIsControlled},
        {"tlv_info", p.field_44_tlvInfo},
        {"brain_idx", p.field_48_brain_idx},
        {"sub_state", p.mBrainSubState},
        {"obj_id", p.field_54_obj_id},
        {"target_obj_id", p.field_58_target_obj_id},
        {"timer", p.field_5C_timer},
        {"depossession_timer", p.field_60_depossession_timer},
        {"falling_velx_scale_factor", p.field_64_falling_velx_scale_factor},
        {"motion_resource_block_index", p.field_68_motion_resource_block_index},
        {"spotting_abe_timer", p.field_6C_spotting_abe_timer},
        {"attack_delay_timer", p.field_70_attack_delay_timer},
        {"movement_timer", p.field_74_movement_timer},
        {"sfx_bitmask", p.field_78_sfx_bitmask},
        {"prevent_depossession", p.mPreventDepossession},
        {"return_level", p.mAbeLevel},
        {"return_path", p.mAbePath},
        {"return_camera", p.mAbeCamera},
        {"input", p.field_84_input},
        {"shred_power_active", p.field_8C_shred_power_active},
        {"speak", p.field_8E_speak},
        {"max_xpos", p.field_90_max_xpos},
        {"max_ypos", p.field_94_max_ypos},
        {"speak_counter", p.field_98_speak_counter},
        {"attacking", p.mAttacking},
        {"force_update_animation", p.mForceUpdateAnimation},
        {"roar_randomly", p.mRoarRandomly},
        {"persistant", p.mPersistant},
    };
}

inline void from_json(const nlohmann::json& j, ScrabSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("obj_id").get_to(p.field_4_obj_id);
    j.at("xpos").get_to(p.mXPos);
    j.at("ypos").get_to(p.mYPos);
    j.at("velx").get_to(p.mVelX);
    j.at("vely").get_to(p.mVelY);
    j.at("path_number").get_to(p.mCurrentPath);
    j.at("lvl_number").get_to(p.mCurrentLevel);
    j.at("sprite_scale").get_to(p.mSpriteScale);
    j.at("ring_red").get_to(p.mRingRed);
    j.at("ring_green").get_to(p.mRingGreen);
    j.at("ring_blue").get_to(p.mRingBlue);
    j.at("anim_flipX").get_to(p.mFlipX);
    j.at("field_28_current_motion").get_to(p.field_28_current_motion);
    j.at("current_frame").get_to(p.mCurrentFrame);
    j.at("frame_change_counter").get_to(p.mFrameChangeCounter);
    j.at("anim_render").get_to(p.mAnimRender);
    j.at("drawable").get_to(p.mDrawable);
    j.at("health").get_to(p.mHealth);
    j.at("field_34_current_motion").get_to(p.field_34_current_motion);
    j.at("next_motion").get_to(p.field_36_next_motion);
    j.at("last_line_ypos").get_to(p.field_38_last_line_ypos);
    j.at("line_type").get_to(p.mLineType);
    j.at("is_controlled").get_to(p.mIsControlled);
    j.at("tlv_info").get_to(p.field_44_tlvInfo);
    j.at("brain_idx").get_to(p.field_48_brain_idx);
    j.at("sub_state").get_to(p.mBrainSubState);
    j.at("obj_id").get_to(p.field_54_obj_id);
    j.at("target_obj_id").get_to(p.field_58_target_obj_id);
    j.at("timer").get_to(p.field_5C_timer);
    j.at("depossession_timer").get_to(p.field_60_depossession_timer);
    j.at("falling_velx_scale_factor").get_to(p.field_64_falling_velx_scale_factor);
    j.at("motion_resource_block_index").get_to(p.field_68_motion_resource_block_index);
    j.at("spotting_abe_timer").get_to(p.field_6C_spotting_abe_timer);
    j.at("attack_delay_timer").get_to(p.field_70_attack_delay_timer);
    j.at("movement_timer").get_to(p.field_74_movement_timer);
    j.at("sfx_bitmask").get_to(p.field_78_sfx_bitmask);
    j.at("prevent_depossession").get_to(p.mPreventDepossession);
    j.at("return_level").get_to(p.mAbeLevel);
    j.at("return_path").get_to(p.mAbePath);
    j.at("return_camera").get_to(p.mAbeCamera);
    j.at("input").get_to(p.field_84_input);
    j.at("shred_power_active").get_to(p.field_8C_shred_power_active);
    j.at("speak").get_to(p.field_8E_speak);
    j.at("max_xpos").get_to(p.field_90_max_xpos);
    j.at("max_ypos").get_to(p.field_94_max_ypos);
    j.at("speak_counter").get_to(p.field_98_speak_counter);
    j.at("attacking").get_to(p.mAttacking);
    j.at("force_update_animation").get_to(p.mForceUpdateAnimation);
    j.at("roar_randomly").get_to(p.mRoarRandomly);
    j.at("persistant").get_to(p.mPersistant);
}

NLOHMANN_JSON_SERIALIZE_ENUM(ScrabSpawnerStates, {
    {ScrabSpawnerStates::eInactive_0, "inactive"},
    {ScrabSpawnerStates::eScrabSpawned_1, "scrab_spawned"},
})

inline void to_json(nlohmann::json& j, const ScrabSpawnerSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"tlv_info", p.field_4_tlvInfo},
        {"state", p.field_8_state},
        {"spawned_scrab_id", p.field_C_spawned_scrab_id},
    };
}

inline void from_json(const nlohmann::json& j, ScrabSpawnerSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("tlv_info").get_to(p.field_4_tlvInfo);
    j.at("state").get_to(p.field_8_state);
    j.at("spawned_scrab_id").get_to(p.field_C_spawned_scrab_id);
}

inline void to_json(nlohmann::json& j, const SlamDoorSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"tlv_info", p.mTlvInfo},
    };
}

inline void from_json(const nlohmann::json& j, SlamDoorSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("tlv_info").get_to(p.mTlvInfo);
}

NLOHMANN_JSON_SERIALIZE_ENUM(eSligMotions, {
    {eSligMotions::Motion_0_StandIdle, "stand_idle"},
    {eSligMotions::Motion_1_StandToWalk, "stand_to_walk"},
    {eSligMotions::Motion_2_Walking, "walking"},
    {eSligMotions::Motion_3_StandToRun, "stand_to_run"},
    {eSligMotions::Motion_4_Running, "running"},
    {eSligMotions::Motion_5_TurnAroundStanding, "turn_around_standing"},
    {eSligMotions::Motion_6_Shoot, "shoot"},
    {eSligMotions::Motion_7_Falling, "falling"},
    {eSligMotions::Motion_8_SlidingToStand, "sliding_to_stand"},
    {eSligMotions::Motion_9_SlidingTurn, "sliding_turn"},
    {eSligMotions::Motion_10_SlidingTurnToWalk, "sliding_turn_to_walk"},
    {eSligMotions::Motion_11_SlidingTurnToRun, "sliding_turn_to_run"},
    {eSligMotions::Motion_12_ReloadGun, "reload_gun"},
    {eSligMotions::Motion_13_ShootToStand, "shoot_to_stand"},
    {eSligMotions::Motion_14_SteppingToStand, "stepping_to_stand"},
    {eSligMotions::Motion_15_StandingToStep, "standing_to_step"},
    {eSligMotions::Motion_16_DepossessingAbort, "depossessing_abort"},
    {eSligMotions::Motion_17_GameSpeak, "gamespeak"},
    {eSligMotions::Motion_18_WalkToStand, "walk_to_stand"},
    {eSligMotions::Motion_19_Recoil, "recoil"},
    {eSligMotions::Motion_20_SpeakHereBoy, "speak_here_boy"},
    {eSligMotions::Motion_21_SpeakHi, "speak_hi"},
    {eSligMotions::Motion_22_SpeakFreeze, "speak_freeze"},
    {eSligMotions::Motion_23_SpeakGetHim, "speak_get_him"},
    {eSligMotions::Motion_24_SpeakLaugh, "speak_laugh"},
    {eSligMotions::Motion_25_SpeakBullshit1, "speak_bullshit1"},
    {eSligMotions::Motion_26_SpeakLookOut, "speak_look_out"},
    {eSligMotions::Motion_27_SpeakBullshit2, "speak_bullshit2"},
    {eSligMotions::Motion_28_SpeakPanic, "speak_panic"},
    {eSligMotions::Motion_29_SpeakWhat, "speak_what"},
    {eSligMotions::Motion_30_SpeakAIFreeze, "speak_ai_freeze"},
    {eSligMotions::Motion_31_Blurgh, "blurgh"},
    {eSligMotions::Motion_32_Sleeping, "sleeping"},
    {eSligMotions::Motion_33_SleepingToStand, "sleeping_to_stand"},
    {eSligMotions::Motion_34_Knockback, "knockback"},
    {eSligMotions::Motion_35_KnockbackToStand, "knockback_to_stand"},
    {eSligMotions::Motion_36_Depossessing, ""},
    {eSligMotions::Motion_37_Possess, "possess"},
    {eSligMotions::Motion_38_OutToFall, "out_to_fall"},
    {eSligMotions::Motion_39_FallingInitiate, "falling_initiate"},
    {eSligMotions::Motion_40_LandingSoft, "landing_soft"},
    {eSligMotions::Motion_41_LandingFatal, "landing_fatal"},
    {eSligMotions::Motion_42_ShootZ, "shoot_z"},
    {eSligMotions::Motion_43_ShootZtoStand, "shoot_z_to_stand"},
    {eSligMotions::Motion_44_Smash, "smash"},
    {eSligMotions::Motion_45_PullLever, "pull_lever"},
    {eSligMotions::Motion_46_LiftGrip, "lift_grip"},
    {eSligMotions::Motion_47_LiftUngrip, "lift_ungrip"},
    {eSligMotions::Motion_48_LiftGripping, "lift_gripping"},
    {eSligMotions::Motion_49_LiftUp, "lift_up"},
    {eSligMotions::Motion_50_LiftDown, "lift_down"},
    {eSligMotions::Motion_51_Beat, "beat"},
})

inline void to_json(nlohmann::json& j, const SligSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"xpos", p.field_4_xpos},
        {"ypos", p.field_8_ypos},
        {"velx", p.field_C_velx},
        {"vely", p.field_10_vely},
        {"path_number", p.field_14_path_number},
        {"lvl_number", p.field_16_lvl_number},
        {"sprite_scale", p.field_18_sprite_scale},
        {"scale", p.field_1C_scale},
        {"r", p.field_1E_r},
        {"g", p.field_20_g},
        {"b", p.field_22_b},
        {"flipx", p.field_24_bFlipX},
        {"field_26_current_motion", p.field_26_current_motion},
        {"current_frame ", p.field_28_current_frame },
        {"frame_change_counter", p.field_2A_frame_change_counter},
        {"render", p.field_2C_bRender},
        {"drawable", p.field_2D_bDrawable},
        {"health", p.field_30_health},
        {"field_34_current_motion", p.field_34_current_motion},
        {"next_motion", p.field_36_next_motion},
        {"last_line_ypos", p.field_38_last_line_ypos},
        {"collision_line_type", p.field_3A_collision_line_type},
        {"active_char", p.field_40_bActiveChar},
        {"brain_sub_state", p.field_42_brain_sub_state},
        {"pitch_min", p.field_44_pitch_min},
        {"field_48_timer", p.field_48_timer},
        {"return_to_previous_motion", p.mReturnToPreviousMotion},
        {"checked_if_off_screen", p.mCheckedIfOffScreen},
        {"input", p.field_50_input},
        {"field_54_timer", p.field_54_timer},
        {"falling_velx_scale_factor", p.mFallingVelxScaleFactor},
        {"tlv_info", p.field_5C_tlvInfo},
        {"res_idx", p.field_60_res_idx},
        {"shot_motion", p.field_62_shot_motion},
        {"zone_rect", p.field_64_zone_rect},
        {"return_level", p.field_72_return_level},
        {"return_path", p.field_74_return_path},
        {"return_camera", p.field_76_return_camera},
        {"death_by_being_shot_timer", p.field_78_death_by_being_shot_timer},
        {"explode_timer", p.field_7C_explode_timer},
        {"brain_state_idx", p.field_80_brain_state_idx},
        {"num_times_to_shoot", p.field_8C_num_times_to_shoot},
        {"force_alive_state", p.field_90_force_alive_state},
        {"spotted_possessed_slig", p.field_92_spotted_possessed_slig},
        {"glukkon_id", p.field_94_glukkon_id},
        {"state_after_speak", p.field_98_state_after_speak},
        {"attention_timeout", p.field_9A_attention_timeout},
        {"next_command_arg1", p.field_9E_next_command_arg1},
        {"cmd_idx", p.field_A0_cmd_idx},
        {"follow_glukkon", p.mFollowGlukkon},
        {"stopped_for_lever_or_lift", p.mStoppedForLeverOrLift},
        {"glukkon_called_alloya", p.mGlukkonCalledAllOYa},
        {"heard_glukkon", p.mHeardGlukkon},
    };
}

inline void from_json(const nlohmann::json& j, SligSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("xpos").get_to(p.field_4_xpos);
    j.at("ypos").get_to(p.field_8_ypos);
    j.at("velx").get_to(p.field_C_velx);
    j.at("vely").get_to(p.field_10_vely);
    j.at("path_number").get_to(p.field_14_path_number);
    j.at("lvl_number").get_to(p.field_16_lvl_number);
    j.at("sprite_scale").get_to(p.field_18_sprite_scale);
    j.at("scale").get_to(p.field_1C_scale);
    j.at("r").get_to(p.field_1E_r);
    j.at("g").get_to(p.field_20_g);
    j.at("b").get_to(p.field_22_b);
    j.at("flipx").get_to(p.field_24_bFlipX);
    j.at("field_26_current_motion").get_to(p.field_26_current_motion);
    j.at("current_frame ").get_to(p.field_28_current_frame);
    j.at("frame_change_counter").get_to(p.field_2A_frame_change_counter);
    j.at("render").get_to(p.field_2C_bRender);
    j.at("drawable").get_to(p.field_2D_bDrawable);
    j.at("health").get_to(p.field_30_health);
    j.at("field_34_current_motion").get_to(p.field_34_current_motion);
    j.at("next_motion").get_to(p.field_36_next_motion);
    j.at("last_line_ypos").get_to(p.field_38_last_line_ypos);
    j.at("collision_line_type").get_to(p.field_3A_collision_line_type);
    j.at("active_char").get_to(p.field_40_bActiveChar);
    j.at("brain_sub_state").get_to(p.field_42_brain_sub_state);
    j.at("pitch_min").get_to(p.field_44_pitch_min);
    j.at("field_48_timer").get_to(p.field_48_timer);
    j.at("return_to_previous_motion").get_to(p.mReturnToPreviousMotion);
    j.at("checked_if_off_screen").get_to(p.mCheckedIfOffScreen);
    j.at("input").get_to(p.field_50_input);
    j.at("field_54_timer").get_to(p.field_54_timer);
    j.at("falling_velx_scale_factor").get_to(p.mFallingVelxScaleFactor);
    j.at("tlv_info").get_to(p.field_5C_tlvInfo);
    j.at("res_idx").get_to(p.field_60_res_idx);
    j.at("shot_motion").get_to(p.field_62_shot_motion);
    j.at("zone_rect").get_to(p.field_64_zone_rect);
    j.at("return_level").get_to(p.field_72_return_level);
    j.at("return_path").get_to(p.field_74_return_path);
    j.at("return_camera").get_to(p.field_76_return_camera);
    j.at("death_by_being_shot_timer").get_to(p.field_78_death_by_being_shot_timer);
    j.at("explode_timer").get_to(p.field_7C_explode_timer);
    j.at("brain_state_idx").get_to(p.field_80_brain_state_idx);
    j.at("num_times_to_shoot").get_to(p.field_8C_num_times_to_shoot);
    j.at("force_alive_state").get_to(p.field_90_force_alive_state);
    j.at("spotted_possessed_slig").get_to(p.field_92_spotted_possessed_slig);
    j.at("glukkon_id").get_to(p.field_94_glukkon_id);
    j.at("state_after_speak").get_to(p.field_98_state_after_speak);
    j.at("attention_timeout").get_to(p.field_9A_attention_timeout);
    j.at("next_command_arg1").get_to(p.field_9E_next_command_arg1);
    j.at("cmd_idx").get_to(p.field_A0_cmd_idx);
    j.at("follow_glukkon").get_to(p.mFollowGlukkon);
    j.at("stopped_for_lever_or_lift").get_to(p.mStoppedForLeverOrLift);
    j.at("glukkon_called_alloya").get_to(p.mGlukkonCalledAllOYa);
    j.at("heard_glukkon").get_to(p.mHeardGlukkon);
}

NLOHMANN_JSON_SERIALIZE_ENUM(eSlogMotions, {
    {eSlogMotions::m2, "m2"},
    {eSlogMotions::m1, "m1"},
    {eSlogMotions::Motion_0_Idle, "idle"},
    {eSlogMotions::Motion_1_Walk, "walk"},
    {eSlogMotions::Motion_2_Run, "run"},
    {eSlogMotions::Motion_3_TurnAround, "turn_around"},
    {eSlogMotions::Motion_4_Fall, "fall"},
    {eSlogMotions::Motion_5_MoveHeadUpwards, "move_head_upwards"},
    {eSlogMotions::Motion_6_StopRunning, "stop_running"},
    {eSlogMotions::Motion_7_SlideTurn, "slide_turn"},
    {eSlogMotions::Motion_8_StartWalking, "start_walking"},
    {eSlogMotions::Motion_9_EndWalking, "end_walking"},
    {eSlogMotions::Motion_10_Land, "land"},
    {eSlogMotions::Motion_11_Unused, "motion_11_unused"},
    {eSlogMotions::Motion_12_StartFastBarking, "start_fast_barking"},
    {eSlogMotions::Motion_13_EndFastBarking, "end_fast_barking"},
    {eSlogMotions::Motion_14_AngryBark, "angry_bark"},
    {eSlogMotions::Motion_15_Sleeping, "sleeping"},
    {eSlogMotions::Motion_16_MoveHeadDownwards, "move_head_downwards"},
    {eSlogMotions::Motion_17_WakeUp, "wake_up"},
    {eSlogMotions::Motion_18_JumpForwards, "jump_forwards"},
    {eSlogMotions::Motion_19_JumpUpwards, "jump_upwards"},
    {eSlogMotions::Motion_20_Eating, "eating"},
    {eSlogMotions::Motion_21_Dying, "dying"},
    {eSlogMotions::Motion_22_Scratch, "scratch"},
    {eSlogMotions::Motion_23_Growl, "growl"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(eSlogBrains, {
    {eSlogBrains::Brain_0_ListeningToSlig, "listening_to_slig"},
    {eSlogBrains::Brain_1_Idle, "idle"},
    {eSlogBrains::Brain_2_ChasingAbe, "chasing_abe"},
    {eSlogBrains::Brain_3_Death, "death"},
})

inline void to_json(nlohmann::json& j, const SlogSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"base_tlv_id", p.mBaseTlvId},
        {"xpos", p.mXPos},
        {"ypos", p.mYPos},
        {"velx", p.mVelX},
        {"vely", p.mVelY},
        {"current_path", p.mCurrentPath},
        {"current_level", p.mCurrentLevel},
        {"sprite_scale", p.mSpriteScale},
        {"r", p.mR},
        {"g", p.mG},
        {"b", p.mB},
        {"flipx", p.mFlipX},
        {"current_motion", p.mCurrentMotion},
        {"current_frame", p.mCurrentFrame},
        {"frame_change_counter", p.mFrameChangeCounter},
        {"render", p.mRender},
        {"drawable", p.mDrawable},
        {"health", p.mHealth},
        {"current_motion2", p.mCurrentMotion2},
        {"next_motion", p.mNextMotion},
        {"last_line_ypos", p.mLastLineYPos},
        {"collision_line_type", p.mCollisionLineType},
        {"platform_id", p.mPlatformId},
        {"slog_tlv_id", p.mSlogTlvId},
        {"target_id", p.mTargetId},
        {"brain_state", p.mBrainState},
        {"brain_sub_state", p.mBrainSubState},
        {"multi_use_timer", p.mMultiUseTimer},
        {"falling_velx_scale_factor", p.mFallingVelxScaleFactor},
        {"listen_to_slig_id", p.mListeningToSligId},
        {"has_woofed", p.mHasWoofed},
        {"waiting_counter", p.mWaitingCounter},
        {"response_idx", p.mResponseIdx},
        {"response_part", p.mResponsePart},
        {"anger_level", p.mAngerLevel},
        {"jump_counter", p.mJumpCounter},
        {"scratch_timer", p.mScratchTimer},
        {"growl_timer", p.mGrowlTimer},
        {"bone_id", p.mBoneId},
        {"chase_delay", p.mChaseDelay},
        {"slog_random_idx", p.mSlogRandomIdx},
        {"biting_target", p.mBitingTarget},
        {"eBit3_Asleep", p.eBit3_Asleep},
        {"moved_off_screen", p.mMovedOffScreen},
        {"stop_running", p.mStopRunning},
        {"shot", p.mShot},
        {"hungry", p.mHungry},
        {"commanded_to_attack", p.mCommandedToAttack},
        {"hit_by_ability_ring", p.mHitByAbilityRing},
        {"listen_to_sligs", p.mListenToSligs},
    };
}

inline void from_json(const nlohmann::json& j, SlogSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("base_tlv_id").get_to(p.mBaseTlvId);
    j.at("xpos").get_to(p.mXPos);
    j.at("ypos").get_to(p.mYPos);
    j.at("velx").get_to(p.mVelX);
    j.at("vely").get_to(p.mVelY);
    j.at("current_path").get_to(p.mCurrentPath);
    j.at("current_level").get_to(p.mCurrentLevel);
    j.at("sprite_scale").get_to(p.mSpriteScale);
    j.at("r").get_to(p.mR);
    j.at("g").get_to(p.mG);
    j.at("b").get_to(p.mB);
    j.at("flipx").get_to(p.mFlipX);
    j.at("current_motion").get_to(p.mCurrentMotion);
    j.at("current_frame").get_to(p.mCurrentFrame);
    j.at("frame_change_counter").get_to(p.mFrameChangeCounter);
    j.at("render").get_to(p.mRender);
    j.at("drawable").get_to(p.mDrawable);
    j.at("health").get_to(p.mHealth);
    j.at("current_motion2").get_to(p.mCurrentMotion2);
    j.at("next_motion").get_to(p.mNextMotion);
    j.at("last_line_ypos").get_to(p.mLastLineYPos);
    j.at("collision_line_type").get_to(p.mCollisionLineType);
    j.at("platform_id").get_to(p.mPlatformId);
    j.at("slog_tlv_id").get_to(p.mSlogTlvId);
    j.at("target_id").get_to(p.mTargetId);
    j.at("brain_state").get_to(p.mBrainState);
    j.at("brain_sub_state").get_to(p.mBrainSubState);
    j.at("multi_use_timer").get_to(p.mMultiUseTimer);
    j.at("falling_velx_scale_factor").get_to(p.mFallingVelxScaleFactor);
    j.at("listen_to_slig_id").get_to(p.mListeningToSligId);
    j.at("has_woofed").get_to(p.mHasWoofed);
    j.at("waiting_counter").get_to(p.mWaitingCounter);
    j.at("response_idx").get_to(p.mResponseIdx);
    j.at("response_part").get_to(p.mResponsePart);
    j.at("anger_level").get_to(p.mAngerLevel);
    j.at("jump_counter").get_to(p.mJumpCounter);
    j.at("scratch_timer").get_to(p.mScratchTimer);
    j.at("growl_timer").get_to(p.mGrowlTimer);
    j.at("bone_id").get_to(p.mBoneId);
    j.at("chase_delay").get_to(p.mChaseDelay);
    j.at("slog_random_idx").get_to(p.mSlogRandomIdx);
    j.at("biting_target").get_to(p.mBitingTarget);
    j.at("eBit3_Asleep").get_to(p.eBit3_Asleep);
    j.at("moved_off_screen").get_to(p.mMovedOffScreen);
    j.at("stop_running").get_to(p.mStopRunning);
    j.at("shot").get_to(p.mShot);
    j.at("hungry").get_to(p.mHungry);
    j.at("commanded_to_attack").get_to(p.mCommandedToAttack);
    j.at("hit_by_ability_ring").get_to(p.mHitByAbilityRing);
    j.at("listen_to_sligs").get_to(p.mListenToSligs);
}

NLOHMANN_JSON_SERIALIZE_ENUM(SlurgStates, {
    {SlurgStates::eMoving_0, "moving"},
    {SlurgStates::ePausing_1, "pausing"},
    {SlurgStates::eBurst_2, "burst"},
})

inline void to_json(nlohmann::json& j, const SlurgSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"xpos", p.mXPos},
        {"ypos", p.mYPos},
        {"velx", p.mVelX},
        {"slurg_sprite_scale", p.mSlurgSpriteScale},
        {"flipx", p.mFlipX},
        {"anim_current_frame", p.mAnimCurrentFrame},
        {"frame_change_counter", p.mFrameChangeCounter},
        {"drawable", p.mDrawable},
        {"render", p.mRender},
        {"tlv_id", p.mTlvId},
        {"slurg_state", p.mSlurgState},
        {"going_right", p.mGoingRight},
        {"moving", p.mMoving},
    };
}

inline void from_json(const nlohmann::json& j, SlurgSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("xpos").get_to(p.mXPos);
    j.at("ypos").get_to(p.mYPos);
    j.at("velx").get_to(p.mVelX);
    j.at("slurg_sprite_scale").get_to(p.mSlurgSpriteScale);
    j.at("flipx").get_to(p.mFlipX);
    j.at("anim_current_frame").get_to(p.mAnimCurrentFrame);
    j.at("frame_change_counter").get_to(p.mFrameChangeCounter);
    j.at("drawable").get_to(p.mDrawable);
    j.at("render").get_to(p.mRender);
    j.at("tlv_id").get_to(p.mTlvId);
    j.at("slurg_state").get_to(p.mSlurgState);
    j.at("going_right").get_to(p.mGoingRight);
    j.at("moving").get_to(p.mMoving);
}

NLOHMANN_JSON_SERIALIZE_ENUM(TimerTrigger::TimerTriggerStates, {
    {TimerTrigger::TimerTriggerStates::eWaitForEnabled_0, "wait_for_enabled"},
    {TimerTrigger::TimerTriggerStates::eWaitForFirstTrigger_1, "wait_for_first_trigger"},
    {TimerTrigger::TimerTriggerStates::eCheckForStartAgain_2, "check_for_start_again"},
    {TimerTrigger::TimerTriggerStates::eWaitForSecondTrigger_3, "wait_for_second_trigger"},
})

inline void to_json(nlohmann::json& j, const TimerTriggerSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"tlv_id", p.mTlvId},
        {"activation_delay_timer", p.mActivationDelayTimer},
        {"state", p.mState},
        {"starting_switch_state", p.mStartingSwitchState},
    };
}

inline void from_json(const nlohmann::json& j, TimerTriggerSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("tlv_id").get_to(p.mTlvId);
    j.at("activation_delay_timer").get_to(p.mActivationDelayTimer);
    j.at("state").get_to(p.mState);
    j.at("starting_switch_state").get_to(p.mStartingSwitchState);
}

NLOHMANN_JSON_SERIALIZE_ENUM(TrapDoorState, {
    {TrapDoorState::eClosed_0, "closed"},
    {TrapDoorState::eOpening_1, "opening"},
    {TrapDoorState::eOpen_2, "open"},
    {TrapDoorState::eClosing_3, "closing"},
})

inline void to_json(nlohmann::json& j, const TrapDoorSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"state", p.mState},
        {"open_time", p.mOpenTime},
        {"tlv_id", p.mTlvId},
    };
}

inline void from_json(const nlohmann::json& j, TrapDoorSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("state").get_to(p.mState);
    j.at("open_time").get_to(p.mOpenTime);
    j.at("tlv_id").get_to(p.mTlvId);
}

NLOHMANN_JSON_SERIALIZE_ENUM(UXBState, {
    {UXBState::eDelay, "delay"},
    {UXBState::eActive, "active"},
    {UXBState::eExploding, "exploding"},
    {UXBState::eDeactivated, "deactivated"},
})

inline void to_json(nlohmann::json& j, const UXBSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"tlv_info", p.mTlvInfo},
        {"next_state_timer", p.mNextStateTimer},
        {"current_state", p.mCurrentState},
        {"starting_state", p.mStartingState},
        {"pattern_index", p.mPatternIndex},
        {"red_blink_count", p.mRedBlinkCount},
        {"is_red", p.mIsRed},
    };
}

inline void from_json(const nlohmann::json& j, UXBSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("tlv_info").get_to(p.mTlvInfo);
    j.at("next_state_timer").get_to(p.mNextStateTimer);
    j.at("current_state").get_to(p.mCurrentState);
    j.at("starting_state").get_to(p.mStartingState);
    j.at("pattern_index").get_to(p.mPatternIndex);
    j.at("red_blink_count").get_to(p.mRedBlinkCount);
    j.at("is_red").get_to(p.mIsRed);
}

NLOHMANN_JSON_SERIALIZE_ENUM(WheelStates, {
    {WheelStates::eIdle_0, "idle"},
    {WheelStates::eTurning_1, "turning"},
})

inline void to_json(nlohmann::json& j, const WorkWheelSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"tlv_id", p.mTlvId},
        {"turning_time", p.mTurningTime},
        {"state", p.mState},
    };
}

inline void from_json(const nlohmann::json& j, WorkWheelSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("tlv_id").get_to(p.mTlvId);
    j.at("turning_time").get_to(p.mTurningTime);
    j.at("state").get_to(p.mState);
}

inline void to_json(nlohmann::json& j, const Quicksave_WorldInfo& p)
{
    j = nlohmann::json{
        {"gn_frame", p.mGnFrame},
        {"level", p.mLevel},
        {"path", p.mPath},
        {"cam", p.mCam},
        {"save_file_id", p.mSaveFileId},
        {"controlled_x", p.mControlledCharX},
        {"controlled_y", p.mControlledCharY},
        {"controlled_scale", p.mControlledCharScale},
        {"saved_muds", p.mRescuedMudokons},
        {"killed_muds", p.mKilledMudokons},
        {"muds_in_area", p.field_16_muds_in_area},
        {"last_saved_killed_muds_per_path", p.field_17_last_saved_killed_muds_per_path},
        {"saved_killed_muds_per_zulag", p.field_18_saved_killed_muds_per_zulag}, // array
        {"current_zulag_number", p.field_2C_current_zulag_number},
        {"total_meter_bars", p.mTotalMeterBars},
        {"draw_meter_count_down", p.field_30_bDrawMeterCountDown},
        {"visited_bonewerkz", p.mVisitedBonewerkz},
        {"visited_barracks", p.mVisitedBarracks},
        {"visited_feeco_ender", p.mVisitedFeecoEnder},
        {"gas_timer", p.mGasTimer},
        {"abe_invincible", p.mAbeInvincible},
    };
}

inline void from_json(const nlohmann::json& j, Quicksave_WorldInfo& p)
{
    j.at("gn_frame").get_to(p.mGnFrame);
    j.at("level").get_to(p.mLevel);
    j.at("path").get_to(p.mPath);
    j.at("cam").get_to(p.mCam);
    j.at("save_file_id").get_to(p.mSaveFileId);
    j.at("controlled_x").get_to(p.mControlledCharX);
    j.at("controlled_y").get_to(p.mControlledCharY);
    j.at("controlled_scale").get_to(p.mControlledCharScale);
    j.at("saved_muds").get_to(p.mRescuedMudokons);
    j.at("killed_muds").get_to(p.mKilledMudokons);
    j.at("muds_in_area").get_to(p.field_16_muds_in_area);
    j.at("last_saved_killed_muds_per_path").get_to(p.field_17_last_saved_killed_muds_per_path);
    j.at("saved_killed_muds_per_zulag").get_to(p.field_18_saved_killed_muds_per_zulag); // array
    j.at("current_zulag_number").get_to(p.field_2C_current_zulag_number);
    j.at("total_meter_bars").get_to(p.mTotalMeterBars);
    j.at("draw_meter_count_down").get_to(p.field_30_bDrawMeterCountDown);
    j.at("visited_bonewerkz").get_to(p.mVisitedBonewerkz);
    j.at("visited_barracks").get_to(p.mVisitedBarracks);
    j.at("visited_feeco_ender").get_to(p.mVisitedFeecoEnder);
    j.at("gas_timer").get_to(p.mGasTimer);
    j.at("abe_invincible").get_to(p.mAbeInvincible);
}

inline void to_json(nlohmann::json& j, const SwitchStates& p)
{
    j = nlohmann::json{
        {"data", p.mData},
    };
}

inline void from_json(const nlohmann::json& j, SwitchStates& p)
{
    j.at("data").get_to(p.mData);
}

// TODO: put forward declare in the correct location
void ConvertObjectsStatesToJson(nlohmann::json& j, const SerializedObjectData& pData);

// TODO: Not AE specific move out of here
static inline nlohmann::json WriteObjectStateJson(const SerializedObjectData& object_states)
{
    nlohmann::json j;
    ConvertObjectsStatesToJson(j, object_states);
    return j;
}

// TODO: Not AE specific move out of here
static inline nlohmann::json WriteObjectBlyJson(const Quicksave& q)
{
    if (!q.mObjectBlyData.CanRead())
    {
        return {};
    }

    // TODO: This func is likely very slow
    const u32 flagsCount = q.mObjectBlyData.ReadU32();

    nlohmann::json j = nlohmann::json::array();

    j.push_back(flagsCount);
    // Flags and TLV specific meaning byte
    for (u32 i = 0; i < flagsCount * 2; i++)
    {
        j.push_back(q.mObjectBlyData.ReadU8());
    }

    return j;
}

inline void to_json(nlohmann::json& j, const Quicksave& p)
{
    j = nlohmann::json{
        {"world_info", p.mWorldInfo},
        {"restart_path_world_info", p.mRestartPathWorldInfo},
        {"restart_path_abe_state", p.mRestartPathAbeState},
        {"restart_path_switch_states", p.mRestartPathSwitchStates},
        {"switch_states", p.mSwitchStates}, 
        {"object_states", WriteObjectStateJson(p.mObjectsStateData)}, 
        {"object_bly_data", WriteObjectBlyJson(p)}
    };
}

template<typename T>
static void write_object_state(const nlohmann::json& j, SerializedObjectData& object_states)
{
    T data = j.get<T>();
    object_states.Write(data);
}

static void WriteObjectStateFromJson(const nlohmann::json& j, SerializedObjectData& object_states)
{
    const auto& type = j["type"];
    if (type == "slig_spawner")
    {
        write_object_state<SligSpawnerSaveState>(j, object_states);
    }
    else if (type == "lift_mover")
    {
        write_object_state<LiftMoverSaveState>(j, object_states);
    }
    else if (type == "bone")
    {
        write_object_state<BoneSaveState>(j, object_states);
    }
    else if (type == "mines_alarm")
    {
        write_object_state<MinesAlarmSaveState>(j, object_states);
    }
    else if (type == "crawling_slig")
    {
        write_object_state<CrawlingSligSaveState>(j, object_states);
    }
    else if (type == "drill")
    {
        write_object_state<DrillSaveState>(j, object_states);
    }
    else if (type == "evil_fart")
    {
        write_object_state<EvilFartSaveState>(j, object_states);
    }
    else if (type == "fleech")
    {
        write_object_state<FleechSaveState>(j, object_states);
    }
    else if (type == "flying_slig")
    {
        write_object_state<FlyingSligSaveState>(j, object_states);
    }
    else if (type == "flying_slig_spawner")
    {
        write_object_state<FlyingSligSpawnerSaveState>(j, object_states);
    }
    else if (type == "game_ender_controller")
    {
        write_object_state<GameEnderControllerSaveState>(j, object_states);
    }
    else if (type == "slap_lock_orb_whirlwind")
    {
        write_object_state<SlapLockWhirlWindSaveState>(j, object_states);
    }
    else if (type == "slap_lock")
    {
        write_object_state<SlapLockSaveState>(j, object_states);
    }
    else if (type == "greeter")
    {
        write_object_state<GreeterSaveState>(j, object_states);
    }
    else if (type == "grenade")
    {
        write_object_state<GrenadeSaveState>(j, object_states);
    }
    else if (type == "glukkon")
    {
        write_object_state<GlukkonSaveState>(j, object_states);
    }
    else if (type == "abe")
    {
        write_object_state<AbeSaveState>(j, object_states);
    }
    else if (type == "lift_point")
    {
        write_object_state<LiftPointSaveState>(j, object_states);
    }
    else if (type == "mudokon" || type == "ring_or_lift_mud")
    {
        write_object_state<MudokonSaveState>(j, object_states);
    }
    else if (type == "meat")
    {
        write_object_state<MeatSaveState>(j, object_states);
    }
    else if (type == "mine_car")
    {
        write_object_state<MineCarSaveState>(j, object_states);
    }
    else if (type == "paramite")
    {
        write_object_state<ParamiteSaveState>(j, object_states);
    }
    else if (type == "bird_portal")
    {
        write_object_state<BirdPortalSaveState>(j, object_states);
    }
    else if (type == "throwable_array")
    {
        write_object_state<ThrowableArraySaveState>(j, object_states);
    }
    else if (type == "ability_ring")
    {
        write_object_state<AbilityRingSaveState>(j, object_states);
    }
    else if (type == "rock")
    {
        write_object_state<RockSaveState>(j, object_states);
    }
    else if (type == "scrab")
    {
        write_object_state<ScrabSaveState>(j, object_states);
    }
    else if (type == "scrab_spawner")
    {
        write_object_state<ScrabSpawnerSaveState>(j, object_states);
    }
    else if (type == "slam_door")
    {
        write_object_state<SlamDoorSaveState>(j, object_states);
    }
    else if (type == "slig")
    {
        write_object_state<SligSaveState>(j, object_states);
    }
    else if (type == "slog")
    {
        write_object_state<SlogSaveState>(j, object_states);
    }
    else if (type == "slurg")
    {
        write_object_state<SlurgSaveState>(j, object_states);
    }
    else if (type == "timer_trigger")
    {
        write_object_state<TimerTriggerSaveState>(j, object_states);
    }
    else if (type == "trap_door")
    {
        write_object_state<TrapDoorSaveState>(j, object_states);
    }
    else if (type == "uxb")
    {
        write_object_state<UXBSaveState>(j, object_states);
    }
    else if (type == "work_wheel")
    {
        write_object_state<WorkWheelSaveState>(j, object_states);
    }
    else
    {
        ALIVE_FATAL("over");
    }
}

// TODO: Not AE specific move out of here
static inline void ReadObjectStateJson(const nlohmann::json& j, SerializedObjectData& object_data)
{
    for (const auto& state : j["object_states"]) 
    {
        WriteObjectStateFromJson(state, object_data);
    }
}

// TODO: Not AE specific move out of here
static inline void ReadObjectBlyJson(const nlohmann::json& j, Quicksave& q)
{
    // Remove the first element since it's the flags count and divide by 2 because 1 flag has 2 fields/elements
    const u32 flagsCount = static_cast<u32>((j.at("object_bly_data").size() - 1) / 2);

    q.mObjectBlyData.WriteRewind();
    q.mObjectBlyData.WriteU32(flagsCount);
    for (size_t i = 1; i < j["object_bly_data"].size(); i++)
    {
        const auto& state = j["object_bly_data"].at(i);
        q.mObjectBlyData.WriteU8(state);
    }
}

inline void from_json(const nlohmann::json& j, Quicksave& p)
{
    j.at("world_info").get_to(p.mWorldInfo);
    j.at("restart_path_world_info").get_to(p.mRestartPathWorldInfo);
    j.at("restart_path_abe_state").get_to(p.mRestartPathAbeState);
    j.at("restart_path_switch_states").get_to(p.mRestartPathSwitchStates);
    j.at("switch_states").get_to(p.mSwitchStates);
    ReadObjectStateJson(j, p.mObjectsStateData);
    ReadObjectBlyJson(j, p);
}