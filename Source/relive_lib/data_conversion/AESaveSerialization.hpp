#pragma once

#include "AESaveConverter.hpp"
#include "relive_tlvs_serialization.hpp" // needed for some common enum and struct serialization (should probably move those to a different file)

inline void to_json(nlohmann::json& j, const PSX_RECT& p)
{
    j = nlohmann::json{
        {"x", p.x},
        {"y", p.y},
        {"w", p.w},
        {"h", p.h},
    };
}

inline void from_json(const nlohmann::json& j, PSX_RECT& p)
{
    j.at("x").get_to(p.x);
    j.at("y").get_to(p.y);
    j.at("w").get_to(p.w);
    j.at("h").get_to(p.h);
}

inline void to_json(nlohmann::json& j, const FP& p)
{
    j = nlohmann::json{
        {"fp_value", p.fpValue},
    };
}

inline void from_json(const nlohmann::json& j, FP& p)
{
    j.at("fp_value").get_to(p.fpValue);
}

NLOHMANN_JSON_SERIALIZE_ENUM(Scale, {
    {Scale::Bg, "bg"},
    {Scale::Fg, "fg"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(ReliveTypes, {
    {ReliveTypes::eNone, "none"},
    {ReliveTypes::eCrawlingSligButton, "crawling_slig_button"},
    {ReliveTypes::eWheelSyncer, "wheel_syncer"},
    {ReliveTypes::eDemoSpawnPoint, "demo_spawn_point"},
    {ReliveTypes::eMultiSwitchController, "multi_switch_controller"},
    {ReliveTypes::eStatusLight, "status_light"},
    {ReliveTypes::eSlapLock, "slap_lock"},
    {ReliveTypes::eParamiteWebLine, "paramite_web_line"},
    {ReliveTypes::eGlukkonSwitch, "glukkon_switch"},
    {ReliveTypes::eDoorBlocker, "door_blocker"},
    {ReliveTypes::eTrainDoor, "train_door"},
    {ReliveTypes::eLevelLoader, "level_loader"},
    {ReliveTypes::eSligGetWings, "slig_get_wings"},
    {ReliveTypes::eSligGetPants, "slig_get_pants"},
    {ReliveTypes::eTeleporter, "teleporter"},
    {ReliveTypes::eWater, "water"},
    {ReliveTypes::eWorkWheel, "work_wheel"},
    {ReliveTypes::eLCDScreen, "lcd_screen"},
    {ReliveTypes::eInvisibleSwitch, "invisible_switch"},
    {ReliveTypes::eDoorFlame, "door_flame"},
    {ReliveTypes::eMovingBomb, "moving_bomb"},
    {ReliveTypes::eMainMenuController, "main_menu_controller"},
    {ReliveTypes::eHintFly, "hint_fly"},
    {ReliveTypes::eSecurityDoor, "security_door"},
    {ReliveTypes::eCreditsController, "credits_controller"},
    {ReliveTypes::eLCDStatusBoard, "lcd_status_board"},
    {ReliveTypes::eSwitchStateBooleanLogic, "switch_state_boolean_logic"},
    {ReliveTypes::eLightEffect, "light_effect"},
    {ReliveTypes::eSlogSpawner, "slog_spawner"},
    {ReliveTypes::eGasEmitter, "gas_emitter"},
    {ReliveTypes::eRingCancel, "ring_cancel"},
    {ReliveTypes::eElumWall, "elum_wall"},
    {ReliveTypes::eAbeStart, "abe_start"},
    {ReliveTypes::eBeeSwarmHole, "bee_swarm_hole"},
    {ReliveTypes::eFallingItem, "falling_item"},
    {ReliveTypes::eShadowZone, "shadow_zone"},
    {ReliveTypes::eStartController, "start_controller"},
    {ReliveTypes::eBirdPortalExit, "bird_portal_exit"},
    {ReliveTypes::eHoneyDripTarget, "honey_drip_target"},
    {ReliveTypes::ePathTransition, "path_transition"},
    {ReliveTypes::eZSligCover, "z_slig_cover"},
    {ReliveTypes::eResetPath, "reset_path"},
    {ReliveTypes::eElumPathTrans, "elum_path_trans"},
    {ReliveTypes::eScrabLeftBound, "scrab_left_bound"},
    {ReliveTypes::eScrabRightBound, "scrab_right_bound"},
    {ReliveTypes::eScrabNoFall, "scrab_no_fall"},
    {ReliveTypes::eMovingBombStopper, "moving_bomb_stopper"},
    {ReliveTypes::eElumStart, "elum_start"},
    {ReliveTypes::eEdge, "edge"},
    {ReliveTypes::eSoftLanding, "soft_landing"},
    {ReliveTypes::eMovieHandStone, "movie_hand_stone"},
    {ReliveTypes::eBellSongStone, "bell_song_stone"},
    {ReliveTypes::eDemoPlaybackStone, "demo_playback_stone"},
    {ReliveTypes::eHandStone, "hand_stone"},
    {ReliveTypes::eHoist, "hoist"},
    {ReliveTypes::eContinuePoint, "continue_point"},
    {ReliveTypes::eWellLocal, "well_local"},
    {ReliveTypes::eWellExpress, "well_express"},
    {ReliveTypes::eMudokonPathTrans, "mudokon_path_trans"},
    {ReliveTypes::eRingMudokon, "ring_mudokon"},
    {ReliveTypes::eLiftMudokon, "lift_mudokon"},
    {ReliveTypes::eInvisibleZone, "invisible_zone"},
    {ReliveTypes::eEnemyStopper, "enemy_stopper"},
    {ReliveTypes::eSligBoundLeft, "slig_left_bound"},
    {ReliveTypes::eSligBoundRight, "slig_bound_right"},
    {ReliveTypes::eSligPersist, "slig_persist"},
    {ReliveTypes::eZzzSpawner, "zzz_spawner"},
    {ReliveTypes::eKillUnsavedMuds, "kill_unsaved_muds"},
    {ReliveTypes::eDeathDrop, "death_drop"},
    {ReliveTypes::eAlarm, "alarm"},
    {ReliveTypes::eScreenManager, "screen_manager"},
    {ReliveTypes::eBackgroundAnimation, "background_animation"},
    {ReliveTypes::eBat, "bat"},
    {ReliveTypes::eLiftMover, "lift_mover"},
    {ReliveTypes::eTimedMine, "timed_mine"},
    {ReliveTypes::eBullet, "bullet"},
    {ReliveTypes::eDDCheat, "ddcheat"},
    {ReliveTypes::eBells, "bells"},
    {ReliveTypes::eChimeLock, "chime_lock"},
    {ReliveTypes::eGasCountDown, "gas_countdown"},
    {ReliveTypes::eParticleBurst, "particle_burst"},
    {ReliveTypes::eDoor, "door"},
    {ReliveTypes::eBird, "bird"},
    {ReliveTypes::eGameSpeak, "gamespeak"},
    {ReliveTypes::eElectricWall, "electric_wall"},
    {ReliveTypes::eElum, "elum"},
    {ReliveTypes::eBellHammer, "bell_hammer"},
    {ReliveTypes::ePalOverwriter, "pal_overwriter"},
    {ReliveTypes::eGroundExplosion, "ground_explosion"},
    {ReliveTypes::eSecurityClaw, "security_claw"},
    {ReliveTypes::eRockSpawner, "rock_spawner"},
    {ReliveTypes::eFlintLockFire, "flint_lock_fire"},
    {ReliveTypes::eThrowableTotalIndicator, "throwable_total_indicator"},
    {ReliveTypes::eFootSwitch, "foot_switch"},
    {ReliveTypes::eGameEnderController, "game_ender_controller"},
    {ReliveTypes::eDeathBird, "death_bird"},
    {ReliveTypes::eLoadingFile, "loading_file"},
    {ReliveTypes::eGrenade, "grenade"},
    {ReliveTypes::eBoomMachine, "boom_machine"},
    {ReliveTypes::eBackgroundGlukkon, "background_glukkon"},
    { ReliveTypes::eAbe, "abe" },
    { ReliveTypes::MainMenuFade, "main_menu_fade" },
    { ReliveTypes::eHoneySack, "honey_sack" },
    { ReliveTypes::eHoney, "honey" },
    { ReliveTypes::eClawOrBirdPortalTerminator, "claw_or_bird_portal_terminator" },
    { ReliveTypes::eMudokon, "mudokon" },
    { ReliveTypes::eLiftPoint, "lift_point" },
    { ReliveTypes::eMeat, "meat" },
    { ReliveTypes::eMeatSack, "meat_sack" },
    { ReliveTypes::eMeatSaw, "meat_saw" },
    { ReliveTypes::eMine, "mine" },
    { ReliveTypes::eRollingBallStopperShaker, "rolling_ball_stopper_shaker" },
    { ReliveTypes::eMotionDetector, "motion_detector" },
    { ReliveTypes::eRollingBallStopper, "rolling_ball_stopper" },
    { ReliveTypes::ePauseMenu, "pause_menu" },
    { ReliveTypes::eParamite, "paramite" },
    { ReliveTypes::eDemoPlayback, "demo_playback" },
    { ReliveTypes::eBirdPortal, "bird_portal" },
    { ReliveTypes::eBirdPortalTerminator, "bird_portal_terminator" },
    { ReliveTypes::eFG1, "fg1" },
    { ReliveTypes::eAbilityRing, "ability_ring" },
    { ReliveTypes::eRock, "rock" },
    { ReliveTypes::eRockSack, "rock_sack" },
    { ReliveTypes::eRollingBall, "rolling_ball" },
    { ReliveTypes::eRope, "rope" },
    { ReliveTypes::eAirExplosion, "air_explosion" },
    { ReliveTypes::eRedLaser, "red_laser" },
    { ReliveTypes::eScrab, "scrab" },
    { ReliveTypes::eScreenClipper, "screen_clipper" },
    { ReliveTypes::eEffectBase, "effect_base" },
    { ReliveTypes::eDeathFadeOut, "death_fade_out" },
    { ReliveTypes::eFlash, "flash" },
    { ReliveTypes::ScreenShake, "screen_shake" },
    { ReliveTypes::eScreenWave, "screen_wave" },
    { ReliveTypes::eUnknown, "unknown" },
    { ReliveTypes::eShrykull, "shrykull" },
    { ReliveTypes::eSnoozParticle, "snooz_particle" },
    { ReliveTypes::eSlig, "slig" },
    { ReliveTypes::eSlog, "slog" },
    { ReliveTypes::SlingMud, "sling_mud" },
    { ReliveTypes::eSligSpawner, "slig_spawner" },
    { ReliveTypes::eZBall, "z_ball" },
    { ReliveTypes::eParticle, "particle" },
    { ReliveTypes::eZapLine, "zap_line" },
    { ReliveTypes::eBeeSwarm, "bee_swarm" },
    { ReliveTypes::eBeeNest, "bee_nest" },
    { ReliveTypes::eLever, "lever" },
    { ReliveTypes::eTrapDoor, "trap_door" },
    { ReliveTypes::eUXB, "uxb" },
    { ReliveTypes::eMovie, "movie" },
    { ReliveTypes::eCameraSwapper, "camera_swapper" },
    { ReliveTypes::eElectrocute, "electrocute" },
    { ReliveTypes::eTimedMine_or_MovingBomb, "timed_mine_or_moving_bomb" },
    { ReliveTypes::eBone, "bone" },
    { ReliveTypes::eBoneBag, "bone_bag" },
    { ReliveTypes::eBrewMachine, "brew_machine" },
    { ReliveTypes::eSligButton, "slig_button" },
    { ReliveTypes::eExplosionSet, "explosion_set" },
    { ReliveTypes::eZapSpark, "zap_spark" },
    { ReliveTypes::eMetal, "metal" },
    { ReliveTypes::eMinesAlarm, "mines_alarm" },
    { ReliveTypes::eCrawlingSlig, "crawling_slig" },
    { ReliveTypes::eDrill, "drill" },
    { ReliveTypes::eLaughingGas, "laughing_gas" },
    { ReliveTypes::eDoorLock, "door_lock" },
    { ReliveTypes::eDove, "dove" },
    { ReliveTypes::eEvilFart, "evil_fart" },
    { ReliveTypes::eFleech, "fleech" },
    { ReliveTypes::ePossessionFlicker, "possession_flicker" },
    { ReliveTypes::eFlyingSlig, "flying_slig" },
    { ReliveTypes::eFlyingSligSpawner, "flying_slig_spawner" },
    { ReliveTypes::eColourfulMeter, "colourful_meter" },
    { ReliveTypes::eSlapLock_OrbWhirlWind, "slap_lock_orb_whirlwind" },
    { ReliveTypes::eGreeter, "greeter" },
    { ReliveTypes::eGlukkon, "glukkon" },
    { ReliveTypes::eHelpPhone, "help_phone" },
    { ReliveTypes::eEyeOrbPart, "eye_orb_part" },
    { ReliveTypes::eInvisibleEffect, "invisible_effect" },
    { ReliveTypes::ePulley, "pulley" },
    { ReliveTypes::eResourceManager, "resource_manager" },
    { ReliveTypes::eSligGetPantsOrWings, "slig_get_pants_or_wings" },
    { ReliveTypes::eRingOrLiftMud, "ring_or_lift_mud" },
    { ReliveTypes::eSecurityOrb, "security_orb" },
    { ReliveTypes::eText, "text" },
    { ReliveTypes::eMineCar, "mine_car" },
    { ReliveTypes::eGreeterBody, "greeter_body" },
    { ReliveTypes::eMusicController, "music_controller" },
    { ReliveTypes::eMusicTrigger, "music_trigger" },
    { ReliveTypes::ePullRingRope, "pull_ring_rope" },
    { ReliveTypes::eScrabSpawner, "scrab_spawner" },
    { ReliveTypes::eMainMenuTransistion, "main_menu_transition" },
    { ReliveTypes::eScreenShake, "screen_shake" },
    { ReliveTypes::eSlamDoor, "slam_door" },
    { ReliveTypes::eSnoozeParticle, "snooze_particle" },
    { ReliveTypes::eSlurgSpawner, "slurg_spawner" },
    { ReliveTypes::eSlurg, "slurg" },
    { ReliveTypes::eTimerTrigger, "timer_trigger" },
    { ReliveTypes::eTorturedMud, "tortured_mud" },
    { ReliveTypes::eWebLine, "web_line" },
    { ReliveTypes::eWell, "well" },
    { ReliveTypes::eThrowableArray, "throwable_array" },
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
        {"base_throwable_count", p.mBaseThrowableCount},
        {"state", p.mState},
        {"volume_modifier", p.mVolumeModifier},
        {"initial_xpos", p.mInitialXPos},
        {"initial_ypos", p.mInitialYPos},
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
    j.at("base_throwable_count").get_to(p.mBaseThrowableCount);
    j.at("state").get_to(p.mState);
    j.at("volume_modifier").get_to(p.mVolumeModifier);
    j.at("initial_xpos").get_to(p.mInitialXPos);
    j.at("initial_ypos").get_to(p.mInitialYPos);
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
        {"brain_state", p.mBrainState},
        {"brain_sub_state", p.mBrainSubState},
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
    j.at("brain_state").get_to(p.mBrainState);
    j.at("brain_sub_state").get_to(p.mBrainSubState);
    j.at("multi_use_timer").get_to(p.mMultiUseTimer);
    j.at("velx_scale_factor").get_to(p.mVelxScaleFactor);
    j.at("chanting").get_to(p.mChanting);
    j.at("abe_level").get_to(p.mAbeLevel);
    j.at("abe_path").get_to(p.mAbePath);
    j.at("abe_camera").get_to(p.mAbeCamera);
    j.at("slig_button_tlv_id").get_to(p.mSligButtonTlvId);
    j.at("70_obj_id").get_to(p.field_70_obj_id);
    j.at("transformerd_slig_id").get_to(p.mTransformedSligId);
    j.at("speak").get_to(p.mSpeak);
    j.at("say_help_timer").get_to(p.mSayHelpTimer);
}

NLOHMANN_JSON_SERIALIZE_ENUM(DrillStates, {
    {DrillStates::State_0_Restart_Cycle, "restart_cycle"},
    {DrillStates::State_1_Going_Down, "going_down"},
    {DrillStates::State_2_GoingUp, "going_up"},
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

inline void to_json(nlohmann::json& j, EvilFartSaveState& p)
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

inline void to_json(nlohmann::json& j, const FleechSaveState& p)
{
    j = nlohmann::json{
        {"type", p.field_0_type},
        {"field_2", p.field_2},
        {"field_4_obj_id", p.field_4_obj_id},
        {"xpos", p.mXPos},
        {"ypos", p.mYPos},
        {"velx", p.mVelX},
        {"vely", p.mVelY},
        {"path_number", p.mPathNumber},
        {"lvl_number", p.mLvlNumber},
        {"sprite_scale", p.mSpriteScale},
        {"ring_red", p.mRingRed},
        {"ring_green", p.mRingGreen},
        {"ring_blue", p.mRingBlue},
        {"flip_x", p.field_26_bFlipX},
        {"current_motion", p.field_28_current_motion},
        {"anim_current_frame", p.field_2A_anim_current_frame},
        {"frame_change_counter", p.field_2C_frame_change_counter},
        {"render", p.field_2E_bRender},
        {"drawable", p.field_2F_bDrawable},
        {"health", p.mHealth},
        {"current_motion", p.mCurrentMotion},
        {"next_motion", p.mNextMotion},
        {"last_line_y_pos", p.mLastLineYPos},
        {"collision_line_type", p.mCollisionLineType},
        {"platform_id", p.mPlatformId},
        {"tlv_info", p.mTlvInfo},
        {"field_44_obj_id", p.field_44_obj_id},
        {"tongue_state", p.mTongueState},
        {"tongue_sub_state", p.mTongueSubState},
        {"enemy_x_pos", p.mEnemyXPos},
        {"enemy_y_pos", p.mEnemyYPos},
        {"tongue_origin_x", p.mTongueOriginX},
        {"tongue_origin_y", p.mTongueOriginY},
        {"tongue_destination_x", p.mTongueDestinationX},
        {"tongue_destination_y", p.mTongueDestinationY},
        {"field_5a", p.field_5A},
        {"tongue_active_flag", p.field_5C_tongue_active_flag},
        {"render_flag", p.field_5D_render_flag},
        {"brain_state", p.field_5E_brain_state},
        {"state", p.field_60_state},
        {"field_62", p.field_62},
        {"shrivel_timer", p.field_64_shrivel_timer},
        {"fleech_random_idx", p.field_68_fleech_random_idx},
        {"field_69", p.field_69},
        {"did_map_follow_me", p.field_6A_bDidMapFollowMe},
        {"vel_x_factor", p.field_70_velx_factor},
        {"current_anger", p.field_76_current_anger},
        {"max_anger", p.field_78_max_anger},
        {"attack_anger", p.field_7A_attack_anger},
        {"wakeup_id", p.field_7C_wakeup_id},
        {"wake_up_switch_anger_value", p.field_7E_wake_up_switch_anger_value},
        {"wake_up_switch_value", p.field_80_wake_up_switch_value},
        {"can_wake_up_id", p.field_82_can_wake_up_id},
        {"event_x_pos", p.field_84_EventXPos},
        {"scrab_paramite_event_x_pos", p.field_86_ScrabParamiteEventXPos},
        {"patrol_range", p.field_88_patrol_range},
        {"old_x_pos", p.field_8A_old_xpos},
        {"field_8c", p.field_8C},
        {"rnd_crawl", p.field_8E_rnd_crawl},
        {"chase_delay", p.field_90_chase_delay},
        {"chase_timer", p.field_92_chase_timer},
        {"lost_target_timeout", p.field_94_lost_target_timeout},
        {"lost_target_timer", p.field_96_lost_target_timer},
        {"hoist_x", p.field_98_hoistX},
        {"hoist_y", p.field_9A_hoistY},
        {"always_0", p.field_9C_always_0},
        {"angle", p.field_9E_angle},
        {"field_9f", p.field_9F},
        {"hoist_y_distance", p.field_A0_hoistY_distance},
        {"hoist_x_distance", p.field_A4_hoistX_distance},
        {"field_a8", p.field_A8},
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
    j.at("type").get_to(p.field_0_type);
    j.at("field_2").get_to(p.field_2);
    j.at("field_4_obj_id").get_to(p.field_4_obj_id);
    j.at("xpos").get_to(p.mXPos);
    j.at("ypos").get_to(p.mYPos);
    j.at("velx").get_to(p.mVelX);
    j.at("vely").get_to(p.mVelY);
    j.at("path_number").get_to(p.mPathNumber);
    j.at("lvl_number").get_to(p.mLvlNumber);
    j.at("sprite_scale").get_to(p.mSpriteScale);
    j.at("ring_red").get_to(p.mRingRed);
    j.at("ring_green").get_to(p.mRingGreen);
    j.at("ring_blue").get_to(p.mRingBlue);
    j.at("flip_x").get_to(p.field_26_bFlipX);
    j.at("current_motion").get_to(p.field_28_current_motion);
    j.at("anim_current_frame").get_to(p.field_2A_anim_current_frame);
    j.at("frame_change_counter").get_to(p.field_2C_frame_change_counter);
    j.at("render").get_to(p.field_2E_bRender);
    j.at("drawable").get_to(p.field_2F_bDrawable);
    j.at("health").get_to(p.mHealth);
    j.at("current_motion").get_to(p.mCurrentMotion);
    j.at("next_motion").get_to(p.mNextMotion);
    j.at("last_line_y_pos").get_to(p.mLastLineYPos);
    j.at("collision_line_type").get_to(p.mCollisionLineType);
    j.at("platform_id").get_to(p.mPlatformId);
    j.at("tlv_info").get_to(p.mTlvInfo);
    j.at("field_44_obj_id").get_to(p.field_44_obj_id);
    j.at("tongue_state").get_to(p.mTongueState);
    j.at("tongue_sub_state").get_to(p.mTongueSubState);
    j.at("enemy_x_pos").get_to(p.mEnemyXPos);
    j.at("enemy_y_pos").get_to(p.mEnemyYPos);
    j.at("tongue_origin_x").get_to(p.mTongueOriginX);
    j.at("tongue_origin_y").get_to(p.mTongueOriginY);
    j.at("tongue_destination_x").get_to(p.mTongueDestinationX);
    j.at("tongue_destination_y").get_to(p.mTongueDestinationY);
    j.at("field_5a").get_to(p.field_5A);
    j.at("tongue_active_flag").get_to(p.field_5C_tongue_active_flag);
    j.at("render_flag").get_to(p.field_5D_render_flag);
    j.at("brain_state").get_to(p.field_5E_brain_state);
    j.at("state").get_to(p.field_60_state);
    j.at("field_62").get_to(p.field_62);
    j.at("shrivel_timer").get_to(p.field_64_shrivel_timer);
    j.at("fleech_random_idx").get_to(p.field_68_fleech_random_idx);
    j.at("field_69").get_to(p.field_69);
    j.at("did_map_follow_me").get_to(p.field_6A_bDidMapFollowMe);
    j.at("vel_x_factor").get_to(p.field_70_velx_factor);
    j.at("current_anger").get_to(p.field_76_current_anger);
    j.at("max_anger").get_to(p.field_78_max_anger);
    j.at("attack_anger").get_to(p.field_7A_attack_anger);
    j.at("wakeup_id").get_to(p.field_7C_wakeup_id);
    j.at("wake_up_switch_anger_value").get_to(p.field_7E_wake_up_switch_anger_value);
    j.at("wake_up_switch_value").get_to(p.field_80_wake_up_switch_value);
    j.at("can_wake_up_id").get_to(p.field_82_can_wake_up_id);
    j.at("event_x_pos").get_to(p.field_84_EventXPos);
    j.at("scrab_paramite_event_x_pos").get_to(p.field_86_ScrabParamiteEventXPos);
    j.at("patrol_range").get_to(p.field_88_patrol_range);
    j.at("old_x_pos").get_to(p.field_8A_old_xpos);
    j.at("field_8c").get_to(p.field_8C);
    j.at("rnd_crawl").get_to(p.field_8E_rnd_crawl);
    j.at("chase_delay").get_to(p.field_90_chase_delay);
    j.at("chase_timer").get_to(p.field_92_chase_timer);
    j.at("lost_target_timeout").get_to(p.field_94_lost_target_timeout);
    j.at("lost_target_timer").get_to(p.field_96_lost_target_timer);
    j.at("hoist_x").get_to(p.field_98_hoistX);
    j.at("hoist_y").get_to(p.field_9A_hoistY);
    j.at("always_0").get_to(p.field_9C_always_0);
    j.at("angle").get_to(p.field_9E_angle);
    j.at("field_9f").get_to(p.field_9F);
    j.at("hoist_y_distance").get_to(p.field_A0_hoistY_distance);
    j.at("hoist_x_distance").get_to(p.field_A4_hoistX_distance);
    j.at("field_a8").get_to(p.field_A8);
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

inline void to_json(nlohmann::json& j, const FlyingSligSaveState& p)
{
    j = nlohmann::json{
        {"type", p.field_0_type},
        {"field_2", p.field_2},
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
        {"delayed_state", p.field_32_delayed_state},
        {"last_line_y_pos", p.field_34_lastLineYPos},
        {"line_idx", p.field_36_line_idx},
        {"launch_switch_id", p.field_38_launch_switch_id},
        {"field_3a", p.field_3A.Raw().all},
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
    j.at("type").get_to(p.field_0_type);
    j.at("field_2").get_to(p.field_2);
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
    j.at("frame_change_death_counter").get_to(p.field_28_frame_change_counter);
    j.at("anim_render").get_to(p.field_2A_bAnimRender);
    j.at("drawable").get_to(p.field_2B_bDrawable);
    j.at("current_health").get_to(p.field_2C_current_health);
    j.at("current_state").get_to(p.field_30_current_state);
    j.at("delayed_state").get_to(p.field_32_delayed_state);
    j.at("last_line_y_pos").get_to(p.field_34_lastLineYPos);
    j.at("line_idx").get_to(p.field_36_line_idx);
    j.at("launch_switch_id").get_to(p.field_38_launch_switch_id);
    j.at("field_3a").get_to(p.field_3A.Raw().all);
    j.at("tlv_info").get_to(p.field_3C_tlvInfo);
    j.at("timer").get_to(p.field_40_timer);
    j.at("grenade_delay").get_to(p.field_44_grenade_delay);
    j.at("collision_reaction_timer").get_to(p.field_48_collision_reaction_timer);
    j.at("x_speed").get_to(p.field_4C_xSpeed);
    j.at("y_speed").get_to(p.field_50_ySpeed);
    j.at("next_speak").get_to(p.field_54_next_speak);
    j.at("voice_pitch_min").get_to(p.field_56_voice_pitch_min);
    j.at("obj_id").get_to(p.field_58_obj_id);
    j.at("field_5C").get_to(p.field_5C);
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
        {"type", p.field_0_type},
        {"tlv_info", p.field_4_tlvInfo},
        {"spawned", p.field_8_bSpawned},
        {"spawned_slig_obj_id", p.field_C_spawned_slig_obj_id},
    };
}

inline void from_json(const nlohmann::json& j, FlyingSligSpawnerSaveState& p)
{
    j.at("type").get_to(p.field_0_type);
    j.at("tlv_info").get_to(p.field_4_tlvInfo);
    j.at("spawned").get_to(p.field_8_bSpawned);
    j.at("spawned_slig_obj_id").get_to(p.field_C_spawned_slig_obj_id);
}

inline void to_json(nlohmann::json& j, const GameEnderControllerSaveState& p)
{
    j = nlohmann::json{
        {"type", p.field_0_type},
        {"obj_id", p.field_4_obj_id},
        {"timer", p.field_8_timer},
        {"state", p.field_C_state},
    };
}

NLOHMANN_JSON_SERIALIZE_ENUM(GameEnderControllerStates, {
    {GameEnderControllerStates::eInit_0, "init"},
    {GameEnderControllerStates::eDetermineEnding_1, "determine_ending"},
    {GameEnderControllerStates::eFinish_2, "finish"},
    {GameEnderControllerStates::eBadEnding_3, "bad_ending"},
    {GameEnderControllerStates::eGoodEnding_4, "good_ending"},
    {GameEnderControllerStates::eAngelicEnding_5, "angelic_ending"},
    {GameEnderControllerStates::eAngelicEndingCredits_6, "angelic_ending_credits"},
    {GameEnderControllerStates::ePadding_7, "padding7"},
    {GameEnderControllerStates::ePadding_8, "padding8"},
    {GameEnderControllerStates::ePadding_9, "padding9"},
})

inline void from_json(const nlohmann::json& j, GameEnderControllerSaveState& p)
{
    j.at("type").get_to(p.field_0_type);
    j.at("obj_id").get_to(p.field_4_obj_id);
    j.at("timer").get_to(p.field_8_timer);
    j.at("state").get_to(p.field_C_state);
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
        {"type", p.field_0_type},
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
        {"current_frame", p.field_20_current_frame},
        {"frame_change_counter", p.field_22_frame_change_counter},
        {"anim_render", p.field_24_bAnimRender},
        {"drawable", p.field_25_bDrawable},
        {"tlv_info", p.field_28_tlvInfo},
        {"last_turn_time", p.field_30_last_turn_time},
        {"timer", p.field_34_timer},
        {"times_shot", p.field_38_timesShot},
        {"dont_set_destroyed", p.field_3A_bDontSetDestroyed},
        {"chasing", p.field_3C_bChasing},
        {"speed", p.field_40_speed},
        {"brain_state", p.field_44_brain_state},
        {"target_on_left", p.field_46_targetOnLeft},
        {"target_on_right", p.field_48_targetOnRight},
        {"motion_laser_xpos", p.field_4C_motion_laser_xpos},
    };
}

inline void from_json(const nlohmann::json& j, GreeterSaveState& p)
{
    j.at("type").get_to(p.field_0_type);
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
    j.at("current_frame").get_to(p.field_20_current_frame);
    j.at("frame_change_counter").get_to(p.field_22_frame_change_counter);
    j.at("anim_render").get_to(p.field_24_bAnimRender);
    j.at("drawable").get_to(p.field_25_bDrawable);
    j.at("tlv_info").get_to(p.field_28_tlvInfo);
    j.at("last_turn_time").get_to(p.field_30_last_turn_time);
    j.at("timer").get_to(p.field_34_timer);
    j.at("times_shot").get_to(p.field_38_timesShot);
    j.at("dont_set_destroyed").get_to(p.field_3A_bDontSetDestroyed);
    j.at("chasing").get_to(p.field_3C_bChasing);
    j.at("speed").get_to(p.field_40_speed);
    j.at("brain_state").get_to(p.field_44_brain_state);
    j.at("target_on_left").get_to(p.field_46_targetOnLeft);
    j.at("target_on_right").get_to(p.field_48_targetOnRight);
    j.at("motion_laser_xpos").get_to(p.field_4C_motion_laser_xpos);
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
        {"type", p.field_0_type},
        {"obj_id", p.field_4_obj_id},
        {"field_8_xpos", p.field_8_xpos},
        {"field_C_ypos", p.field_C_ypos},
        {"velx", p.field_10_velx},
        {"vely", p.field_14_vely},
        {"sprite_scale", p.field_18_sprite_scale},
        {"path_number", p.field_1C_path_number},
        {"lvl_number", p.field_1E_lvl_number},
        {"render", p.mRender},
        {"drawable", p.mDrawable},
        {"loop", p.mLoop},
        {"interactive", p.mInteractive},
        {"explode_now", p.mExplodeNow},
        {"blow_up_on_collision", p.mBlowUpOnCollision},
        {"base_id", p.field_24_base_id},
        {"line_type", p.field_28_line_type},
        {"saved_count", p.field_2A_savedcount},
        {"state", p.field_2C_state},
        {"field_2e", p.field_2E},
        {"explode_timer", p.field_30_explode_timer},
        {"field_34_xpos", p.field_34_xpos},
        {"field_38_ypos", p.field_38_ypos},
    };
}

inline void from_json(const nlohmann::json& j, GrenadeSaveState& p)
{
    j.at("type").get_to(p.field_0_type);
    j.at("obj_id").get_to(p.field_4_obj_id);
    j.at("field_8_xpos").get_to(p.field_8_xpos);
    j.at("field_C_ypos").get_to(p.field_C_ypos);
    j.at("velx").get_to(p.field_10_velx);
    j.at("vely").get_to(p.field_14_vely);
    j.at("sprite_scale").get_to(p.field_18_sprite_scale);
    j.at("path_number").get_to(p.field_1C_path_number);
    j.at("lvl_number").get_to(p.field_1E_lvl_number);
    j.at("render").get_to(p.mRender);
    j.at("drawable").get_to(p.mDrawable);
    j.at("loop").get_to(p.mLoop);
    j.at("interactive").get_to(p.mInteractive);
    j.at("explode_now").get_to(p.mExplodeNow);
    j.at("blow_up_on_collision").get_to(p.mBlowUpOnCollision);
    j.at("base_id").get_to(p.field_24_base_id);
    j.at("line_type").get_to(p.field_28_line_type);
    j.at("saved_count").get_to(p.field_2A_savedcount);
    j.at("state").get_to(p.field_2C_state);
    j.at("field_2e").get_to(p.field_2E);
    j.at("explode_timer").get_to(p.field_30_explode_timer);
    j.at("field_34_xpos").get_to(p.field_34_xpos);
    j.at("field_38_ypos").get_to(p.field_38_ypos);
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
    {GlukkonSpeak::Unused_10, "unused_10"},
    {GlukkonSpeak::What_11, "what"},
    {GlukkonSpeak::Unused_12, "unused_12"},
    {GlukkonSpeak::Unused_13, "unused_13"},
    {GlukkonSpeak::Unused_14, "unused_14"},
})

inline void to_json(nlohmann::json& j, const GlukkonSaveState& p)
{
    j = nlohmann::json{
        {"id", p.field_0_id},
        {"field_4_object_id", p.field_4_object_id},
        {"xpos", p.field_8_xpos},
        {"ypos", p.field_C_ypos},
        {"velx", p.field_10_xvel},
        {"vely", p.field_14_yvel},
        {"field_18_path", p.field_18_path},
        {"field_1a_level", p.field_1A_level},
        {"sprite_scale", p.field_1C_sprite_scale},
        {"ring_red", p.mRingRed},
        {"ring_green", p.mRingGreen},
        {"ring_blue", p.mRingBlue},
        {"flip_x", p.field_26_flipX},
        {"field_28_current_motion", p.field_28_current_motion},
        {"current_frame", p.field_2A_current_frame},
        {"frame_change_counter", p.field_2C_frame_change_counter},
        {"render", p.field_2E_render},
        {"drawable", p.field_2F_drawable},
        {"health", p.field_30_health},
        {"field_34_current_motion", p.field_34_current_motion},
        {"next_motion", p.field_36_next_motion},
        {"last_line_ypos", p.field_38_last_line_ypos},
        {"line_type", p.field_3A_line_type},
        {"is_active_char", p.field_40_bIsActiveChar},
        {"tlv_info", p.field_44_tlvInfo},
        {"brain_state_idx", p.field_48_brain_state_idx},
        {"brain_sub_state", p.field_50_brain_sub_state},
        {"timer", p.field_54_timer},
        {"vel_x_scale_factor", p.field_58_falling_velx_scale_factor},
        {"prevent_depossession", p.field_5E_prevent_depossession},
        {"field_60_level", p.field_60_level},
        {"field_62_path", p.field_62_path},
        {"camera", p.field_64_camera},
        {"speak", p.field_66_speak},
        {"gamespeak_pitch", p.field_68_gamespeak_pitch},
        {"previous_ypos", p.field_6C_previous_ypos},
        {"randomish_speak_timer", p.field_70_randomish_speak_timer},
        {"turn_or_help_timer", p.field_74_turn_or_help_timer},
        {"panic_timer", p.field_78_panic_timer},
        {"field_7c", p.field_7C},
        {"knockback_delay_after_getting_shot_timer", p.field_80_knockback_delay_after_getting_shot_timer},
        {"getting_shot_timer", p.field_84_getting_shot_timer},
        {"field_88_obj_id", p.field_88_obj_id},
        {"can_be_possessed", p.field_8C_can_be_possessed},
        {"type_id", p.field_8E_type_id},
    };
}

inline void from_json(const nlohmann::json& j, GlukkonSaveState& p)
{
    j.at("id").get_to(p.field_0_id);
    j.at("field_4_object_id").get_to(p.field_4_object_id);
    j.at("xpos").get_to(p.field_8_xpos);
    j.at("ypos").get_to(p.field_C_ypos);
    j.at("velx").get_to(p.field_10_xvel);
    j.at("vely").get_to(p.field_14_yvel);
    j.at("field_18_path").get_to(p.field_18_path);
    j.at("field_1a_level").get_to(p.field_1A_level);
    j.at("sprite_scale").get_to(p.field_1C_sprite_scale);
    j.at("ring_red").get_to(p.mRingRed);
    j.at("ring_green").get_to(p.mRingGreen);
    j.at("ring_blue").get_to(p.mRingBlue);
    j.at("flip_x").get_to(p.field_26_flipX);
    j.at("field_28_current_motion").get_to(p.field_28_current_motion);
    j.at("current_frame").get_to(p.field_2A_current_frame);
    j.at("frame_change_counter").get_to(p.field_2C_frame_change_counter);
    j.at("render").get_to(p.field_2E_render);
    j.at("drawable").get_to(p.field_2F_drawable);
    j.at("health").get_to(p.field_30_health);
    j.at("field_34_current_motion").get_to(p.field_34_current_motion);
    j.at("next_motion").get_to(p.field_36_next_motion);
    j.at("last_line_ypos").get_to(p.field_38_last_line_ypos);
    j.at("line_type").get_to(p.field_3A_line_type);
    j.at("is_active_char").get_to(p.field_40_bIsActiveChar);
    j.at("tlv_info").get_to(p.field_44_tlvInfo);
    j.at("brain_state_idx").get_to(p.field_48_brain_state_idx);
    j.at("brain_sub_state").get_to(p.field_50_brain_sub_state);
    j.at("timer").get_to(p.field_54_timer);
    j.at("vel_x_scale_factor").get_to(p.field_58_falling_velx_scale_factor);
    j.at("prevent_depossession").get_to(p.field_5E_prevent_depossession);
    j.at("field_60_level").get_to(p.field_60_level);
    j.at("field_62_path").get_to(p.field_62_path);
    j.at("camera").get_to(p.field_64_camera);
    j.at("speak").get_to(p.field_66_speak);
    j.at("gamespeak_pitch").get_to(p.field_68_gamespeak_pitch);
    j.at("previous_ypos").get_to(p.field_6C_previous_ypos);
    j.at("randomish_speak_timer").get_to(p.field_70_randomish_speak_timer);
    j.at("turn_or_help_timer").get_to(p.field_74_turn_or_help_timer);
    j.at("panic_timer").get_to(p.field_78_panic_timer);
    j.at("field_7c").get_to(p.field_7C);
    j.at("knockback_delay_after_getting_shot_timer").get_to(p.field_80_knockback_delay_after_getting_shot_timer);
    j.at("getting_shot_timer").get_to(p.field_84_getting_shot_timer);
    j.at("field_88_obj_id").get_to(p.field_88_obj_id);
    j.at("can_be_possessed").get_to(p.field_8C_can_be_possessed);
    j.at("type_id").get_to(p.field_8E_type_id);
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

inline void to_json(nlohmann::json& j, const AbeSaveState& p)
{
    j = nlohmann::json{
        {"ae_type", p.mAEType},
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
        {"prev_held", p.mPrevHeld},
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
    j.at("ae_type").get_to(p.mAEType);
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
    j.at("prev_held").get_to(p.mPrevHeld);
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
    j.at("mudanchee:done").get_to(p.mMudancheeDone);
    j.at("mudomo_done").get_to(p.mMudomoDone);
    j.at("shadow_enabled").get_to(p.mShadowEnabled);
    j.at("shadow_at_bottom").get_to(p.mShadowAtBottom);
}
inline void to_json(nlohmann::json& j, const LiftPointSaveState& p)
{
    j = nlohmann::json{
        {"type", p.field_0_type},
        {"xpos", p.field_4_xpos},
        {"ypos", p.field_8_ypos},
        {"tlv_info", p.field_C_tlvInfo},
        {"tlv", p.field_10_pTlv},
        {"floor_y_level", p.field_14_floorYLevel},
        {"lift_point_stop_type", p.field_18_lift_point_stop_type},
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
    j.at("type").get_to(p.field_0_type);
    j.at("xpos").get_to(p.field_4_xpos);
    j.at("ypos").get_to(p.field_8_ypos);
    j.at("tlv_info").get_to(p.field_C_tlvInfo);
    j.at("tlv").get_to(p.field_10_pTlv);
    j.at("floor_y_level").get_to(p.field_14_floorYLevel);
    j.at("lift_point_stop_type").get_to(p.field_18_lift_point_stop_type);
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
    {GameSpeakEvents::eSameAsLast_m2, "same_as_last"},
    {GameSpeakEvents::eNone_m1, "none"},
    {GameSpeakEvents::eUnknown_0, "unknown_0"},
    {GameSpeakEvents::eUnknown_1, "unknown_1"},
    {GameSpeakEvents::eUnknown_2, "unknown_2"},
    {GameSpeakEvents::eFart_3, "fart"},
    {GameSpeakEvents::eUnknown_4, "unknown_4"},
    {GameSpeakEvents::Slig_BS_5, "slig_bs"},
    {GameSpeakEvents::Slig_LookOut_6, "slig_look_out"},
    {GameSpeakEvents::Slig_BS2_7, "slig_bs2"},
    {GameSpeakEvents::Slig_Laugh_8, "slig_laugh"},
    {GameSpeakEvents::eHello_9, "hello"},
    {GameSpeakEvents::eFollowMe_10, "follow_me"},
    {GameSpeakEvents::eAnger_11, "anger"},
    {GameSpeakEvents::eWait_12, "wait"},
    {GameSpeakEvents::eUnknown_13, "unknown_13"},
    {GameSpeakEvents::eUnknown_14, "unknown_14"},
    {GameSpeakEvents::eUnknown_15, "unknown_15"},
    {GameSpeakEvents::eUnknown_16, "unknown_16"},
    {GameSpeakEvents::eUnknown_17, "unknown_17"},
    {GameSpeakEvents::eUnknown_18, "unknown_18"},
    {GameSpeakEvents::eUnknown_19, "unknown_19"},
    {GameSpeakEvents::eUnknown_20, "unknown_20"},
    {GameSpeakEvents::eWork_21, "work"},
    {GameSpeakEvents::eStopIt_22, "stop_it"},
    {GameSpeakEvents::eAllYa_23, "allya"},
    {GameSpeakEvents::eSorry_24, "sorry"},
    {GameSpeakEvents::eUnknown_25, "unknown_25"},
    {GameSpeakEvents::eUnknown_26, "unknown_26"},
    {GameSpeakEvents::Slig_Hi_27, "slig_hi"},
    {GameSpeakEvents::Slig_HereBoy_28, "slig_here_boy"},
    {GameSpeakEvents::Slig_GetEm_29, "slig_get_em"},
    {GameSpeakEvents::eUnknown_30, "unknown_30"},
    {GameSpeakEvents::Slig_Freeze_31, "slig_freeze"},
    {GameSpeakEvents::eUnknown_32, "unknown_32"},
    {GameSpeakEvents::eUnknown_33, "unknown_33"},
    {GameSpeakEvents::eUnknown_34, "unknown_34"},
    {GameSpeakEvents::eUnknown_35, "unknown_35"},
    {GameSpeakEvents::Glukkon_Hey_36, "glukkon_hey"},
    {GameSpeakEvents::Glukkon_DoIt_37, "glukkon_do_it"},
    {GameSpeakEvents::Glukkon_StayHere_38, "glukkon_stay_here"},
    {GameSpeakEvents::Glukkon_Commere_39, "glukkon_commere"},
    {GameSpeakEvents::Glukkon_AllOYa_40, "glukkon_alloya"},
    {GameSpeakEvents::Glukkon_Heh_41, "glukkon_heh"},
    {GameSpeakEvents::Glukkon_Help_42, "glukkon_help"},
    {GameSpeakEvents::Glukkon_Laugh_43, "glukkon_laugh"},
    {GameSpeakEvents::Glukkon_KillEm_44, "glukkon_kill_em"},
    {GameSpeakEvents::Glukkon_Unknown_45, "glukkon_unknown_45"},
    {GameSpeakEvents::Glukkon_Unknown_46, "glukkon_unknown_46"},
    {GameSpeakEvents::Glukkon_What_47, "glukkon_what"},
    {GameSpeakEvents::Paramite_Howdy_48, "paramite_howdy"},
    {GameSpeakEvents::Paramite_Stay_49, "paramite_stay"},
    {GameSpeakEvents::Paramite_CMon_or_Attack_50, "paramite_cmon_or_attack"},
    {GameSpeakEvents::Paramite_DoIt_51, "paramite_do_it"},
    {GameSpeakEvents::Paramite_AllYa_52, "paramite_allya"},
    {GameSpeakEvents::Scrab_Howl_53, "scrab_howl"},
    {GameSpeakEvents::Scrab_Shriek_54, "scrab_shriek"},
})

inline void to_json(nlohmann::json& j, const MudokonSaveState& p)
{
    j = nlohmann::json{
        {"type", p.field_0_type},
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
    j.at("type").get_to(p.field_0_type);
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
        {"type", p.field_0_type},
        {"obj_id", p.field_4_obj_id},
        {"field_8_xpos", p.field_8_xpos},
        {"field_C_ypos", p.field_C_ypos},
        {"velx", p.field_10_velx},
        {"vely", p.field_14_vely},
        {"sprite_scale", p.field_18_sprite_scale},
        {"path_number", p.field_1C_path_number},
        {"lvl_number", p.field_1E_lvl_number},
        {"render", p.mRender},
        {"drawable", p.mDrawable},
        {"loop", p.mLoop},
        {"interactive", p.mInteractive},
        {"base_id", p.field_24_base_id},
        {"line_type", p.field_28_line_type},
        {"count", p.field_2A_count},
        {"state", p.field_2C_state},
        {"field_30_xpos", p.field_30_xpos},
        {"field_34_ypos", p.field_34_ypos},
        {"deadtimer", p.field_38_deadtimer},
    };
}

inline void from_json(const nlohmann::json& j, MeatSaveState& p)
{
    j.at("type").get_to(p.field_0_type);
    j.at("obj_id").get_to(p.field_4_obj_id);
    j.at("field_8_xpos").get_to(p.field_8_xpos);
    j.at("field_C_ypos").get_to(p.field_C_ypos);
    j.at("velx").get_to(p.field_10_velx);
    j.at("vely").get_to(p.field_14_vely);
    j.at("sprite_scale").get_to(p.field_18_sprite_scale);
    j.at("path_number").get_to(p.field_1C_path_number);
    j.at("lvl_number").get_to(p.field_1E_lvl_number);
    j.at("render").get_to(p.mRender);
    j.at("drawable").get_to(p.mDrawable);
    j.at("loop").get_to(p.mLoop);
    j.at("interactive").get_to(p.mInteractive);
    j.at("base_id").get_to(p.field_24_base_id);
    j.at("line_type").get_to(p.field_28_line_type);
    j.at("count").get_to(p.field_2A_count);
    j.at("state").get_to(p.field_2C_state);
    j.at("field_30_xpos").get_to(p.field_30_xpos);
    j.at("field_34_ypos").get_to(p.field_34_ypos);
    j.at("deadtimer").get_to(p.field_38_deadtimer);
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
        {"type", p.field_0_type},
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
        {"field_28_current_motion", p.field_28_current_motion},
        {"current_anim_frame", p.field_2A_current_anim_frame},
        {"frame_change_counter", p.field_2C_frame_change_counter},
        {"render", p.field_2E_render},
        {"drawable", p.field_2F_drawable},
        {"frame_table_offset2", p.field_38_frame_table_offset2},
        {"health", p.field_3C_health},
        {"field_40_current_motion", p.field_40_current_motion},
        {"next_motion", p.field_42_next_motion},
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
    j.at("type").get_to(p.field_0_type);
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
    j.at("field_28_current_motion").get_to(p.field_28_current_motion);
    j.at("current_anim_frame").get_to(p.field_2A_current_anim_frame);
    j.at("frame_change_counter").get_to(p.field_2C_frame_change_counter);
    j.at("render").get_to(p.field_2E_render);
    j.at("drawable").get_to(p.field_2F_drawable);
    j.at("frame_table_offset2").get_to(p.field_38_frame_table_offset2);
    j.at("health").get_to(p.field_3C_health);
    j.at("field_40_current_motion").get_to(p.field_40_current_motion);
    j.at("next_motion").get_to(p.field_42_next_motion);
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

inline void to_json(nlohmann::json& j, const ParamiteSaveState& p)
{
    j = nlohmann::json{
        {"type", p.field_0_type},
        {"xpos", p.field_4_xpos},
        {"ypos", p.field_8_ypos},
        {"velx", p.field_C_velx},
        {"vely", p.field_10_vely},
        {"path_number", p.field_14_path_number},
        {"lvl_number", p.field_16_lvl_number},
        {"r", p.field_1C_r},
        {"g", p.field_1E_g},
        {"b", p.field_20_b},
        {"x_flip", p.field_22_flip_x},
        {"field_24_current_motion", p.field_24_current_motion},
        {"anim_current_frame", p.field_26_anim_current_frame},
        {"frame_change_counter", p.field_28_frame_change_counter},
        {"render", p.field_2A_render},
        {"drawable", p.field_2B_drawable},
        {"health", p.field_2C_health},
        {"field_30_current_motion", p.field_30_current_motion},
        {"next_motion", p.field_32_next_motion},
        {"last_line_ypos", p.field_34_last_line_ypos},
        {"line_type", p.field_36_line_type},
        {"tlv_info", p.field_3C_tlvInfo},
        {"meat_id", p.field_40_meat_id},
        {"web_id", p.field_44_web_id},
        {"obj_id", p.field_48_obj_id},
        {"pull_ring_rope_id", p.field_4C_pull_ring_rope_id},
        {"brain_idx", p.field_50_brain_idx},
        {"brain_ret", p.field_58_brain_ret},
        {"field_5c_timer", p.field_5C_timer},
        {"depossession_timer", p.field_60_depossession_timer},
        {"velx_offset", p.field_64_velx_offset},
        {"field_68_timer", p.field_68_timer},
        {"return_level", p.field_6C_return_level},
        {"return_path", p.field_6E_return_path},
        {"return_camera", p.field_70_return_camera},
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
    j.at("type").get_to(p.field_0_type);
    j.at("xpos").get_to(p.field_4_xpos);
    j.at("ypos").get_to(p.field_8_ypos);
    j.at("velx").get_to(p.field_C_velx);
    j.at("vely").get_to(p.field_10_vely);
    j.at("path_number").get_to(p.field_14_path_number);
    j.at("lvl_number").get_to(p.field_16_lvl_number);
    j.at("r").get_to(p.field_1C_r);
    j.at("g").get_to(p.field_1E_g);
    j.at("b").get_to(p.field_20_b);
    j.at("x_flip").get_to(p.field_22_flip_x);
    j.at("field_24_current_motion").get_to(p.field_24_current_motion);
    j.at("anim_current_frame").get_to(p.field_26_anim_current_frame);
    j.at("frame_change_counter").get_to(p.field_28_frame_change_counter);
    j.at("render").get_to(p.field_2A_render);
    j.at("drawable").get_to(p.field_2B_drawable);
    j.at("health").get_to(p.field_2C_health);
    j.at("field_30_current_motion").get_to(p.field_30_current_motion);
    j.at("next_motion").get_to(p.field_30_current_motion);
    j.at("last_line_ypos").get_to(p.field_34_last_line_ypos);
    j.at("line_type").get_to(p.field_36_line_type);
    j.at("tlv_info").get_to(p.field_3C_tlvInfo);
    j.at("meat_id").get_to(p.field_40_meat_id);
    j.at("web_id").get_to(p.field_44_web_id);
    j.at("obj_id").get_to(p.field_48_obj_id);
    j.at("pull_ring_rope_id").get_to(p.field_4C_pull_ring_rope_id);
    j.at("brain_idx").get_to(p.field_50_brain_idx);
    j.at("brain_ret").get_to(p.field_58_brain_ret);
    j.at("field_5c_timer").get_to(p.field_5C_timer);
    j.at("depossession_timer").get_to(p.field_60_depossession_timer);
    j.at("velx_offset").get_to(p.field_64_velx_offset);
    j.at("field_68_timer").get_to(p.field_68_timer);
    j.at("return_level").get_to(p.field_6C_return_level);
    j.at("return_path").get_to(p.field_6E_return_path);
    j.at("return_camera").get_to(p.field_70_return_camera);
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
        {"ring_object_type", p.mRingObjectType},
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
    j.at("ring_object_type").get_to(p.mRingObjectType);
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
        {"type", p.field_0_type},
        {"obj_id", p.field_4_obj_id},
        {"field_8_xpos", p.field_8_xpos},
        {"field_c_ypos", p.field_C_ypos},
        {"velx", p.field_10_velx},
        {"vely", p.field_14_vely},
        {"sprite_scale", p.field_18_sprite_scale},
        {"path_number", p.field_1C_path_number},
        {"lvl_number", p.field_1E_lvl_number},
        {"render", p.mRender},
        {"drawable", p.mDrawable},
        {"loop", p.mLoop},
        {"interactive", p.mInteractive},
        {"id", p.field_24_id},
        {"line_type", p.field_28_line_type},
        {"count", p.field_2A_count},
        {"state", p.field_2C_state},
        {"volume", p.field_2E_volume},
        {"field_30_xpos", p.field_30_xpos},
        {"field_34_ypos", p.field_34_ypos},
    };
}

inline void from_json(const nlohmann::json& j, RockSaveState& p)
{
    j.at("type").get_to(p.field_0_type);
    j.at("obj_id").get_to(p.field_4_obj_id);
    j.at("field_8_xpos").get_to(p.field_8_xpos);
    j.at("field_c_ypos").get_to(p.field_C_ypos);
    j.at("velx").get_to(p.field_10_velx);
    j.at("vely").get_to(p.field_14_vely);
    j.at("sprite_scale").get_to(p.field_18_sprite_scale);
    j.at("path_number").get_to(p.field_1C_path_number);
    j.at("lvl_number").get_to(p.field_1E_lvl_number);
    j.at("render").get_to(p.mRender);
    j.at("drawable").get_to(p.mDrawable);
    j.at("loop").get_to(p.mLoop);
    j.at("interactive").get_to(p.mInteractive);
    j.at("id").get_to(p.field_24_id);
    j.at("line_type").get_to(p.field_28_line_type);
    j.at("count").get_to(p.field_2A_count);
    j.at("state").get_to(p.field_2C_state);
    j.at("volume").get_to(p.field_2E_volume);
    j.at("field_30_xpos").get_to(p.field_30_xpos);
    j.at("field_34_ypos").get_to(p.field_34_ypos);
}

inline void to_json(nlohmann::json& j, const ScrabSaveState& p)
{
    j = nlohmann::json{
        {"type", p.field_0_type},
        {"obj_id", p.field_4_obj_id},
        {"xpos", p.field_8_xpos},
        {"ypos", p.field_C_ypos},
        {"velx", p.field_10_velx},
        {"vely", p.field_14_vely},
        {"path_number", p.field_18_path_number},
        {"lvl_number", p.field_1A_lvl_number},
        {"sprite_scale", p.field_1C_sprite_scale},
        {"ring_red", p.mRingRed},
        {"ring_green", p.mRingGreen},
        {"ring_blue", p.mRingBlue},
        {"anim_flipX", p.field_26_bAnimFlipX},
        {"field_28_current_motion", p.field_28_current_motion},
        {"current_frame", p.field_2A_current_frame},
        {"frame_change_counter", p.field_2C_frame_change_counter},
        {"anim_render", p.field_2E_bAnimRender},
        {"drawable", p.field_2F_bDrawable},
        {"health", p.field_30_health},
        {"field_34_current_motion", p.field_34_current_motion},
        {"next_motion", p.field_36_next_motion},
        {"last_line_ypos", p.field_38_last_line_ypos},
        {"line_type", p.field_3A_line_type},
        {"is_controlled", p.field_40_bIsControlled},
        {"tlv_info", p.field_44_tlvInfo},
        {"brain_idx", p.field_48_brain_idx},
        {"sub_state", p.field_50_sub_state},
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
        {"prevent_depossession", p.field_7C_prevent_depossession},
        {"return_level", p.field_7E_return_level},
        {"return_path", p.field_80_return_path},
        {"return_camera", p.field_82_return_camera},
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
    j.at("type").get_to(p.field_0_type);
    j.at("obj_id").get_to(p.field_4_obj_id);
    j.at("xpos").get_to(p.field_8_xpos);
    j.at("ypos").get_to(p.field_C_ypos);
    j.at("velx").get_to(p.field_10_velx);
    j.at("vely").get_to(p.field_14_vely);
    j.at("path_number").get_to(p.field_18_path_number);
    j.at("lvl_number").get_to(p.field_1A_lvl_number);
    j.at("sprite_scale").get_to(p.field_1C_sprite_scale);
    j.at("ring_red").get_to(p.mRingRed);
    j.at("ring_green").get_to(p.mRingGreen);
    j.at("ring_blue").get_to(p.mRingBlue);
    j.at("anim_flipX").get_to(p.field_26_bAnimFlipX);
    j.at("field_28_current_motion").get_to(p.field_28_current_motion);
    j.at("current_frame").get_to(p.field_2A_current_frame);
    j.at("frame_change_counter").get_to(p.field_2C_frame_change_counter);
    j.at("anim_render").get_to(p.field_2E_bAnimRender);
    j.at("drawable").get_to(p.field_2F_bDrawable);
    j.at("health").get_to(p.field_30_health);
    j.at("field_34_current_motion").get_to(p.field_34_current_motion);
    j.at("next_motion").get_to(p.field_36_next_motion);
    j.at("last_line_ypos").get_to(p.field_38_last_line_ypos);
    j.at("line_type").get_to(p.field_3A_line_type);
    j.at("is_controlled").get_to(p.field_40_bIsControlled);
    j.at("tlv_info").get_to(p.field_44_tlvInfo);
    j.at("brain_idx").get_to(p.field_48_brain_idx);
    j.at("sub_state").get_to(p.field_50_sub_state);
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
    j.at("prevent_depossession").get_to(p.field_7C_prevent_depossession);
    j.at("return_level").get_to(p.field_7E_return_level);
    j.at("return_path").get_to(p.field_80_return_path);
    j.at("return_camera").get_to(p.field_82_return_camera);
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
        {"type", p.field_0_type},
        {"tlv_info", p.field_4_tlvInfo},
        {"state", p.field_8_state},
        {"spawned_scrab_id", p.field_C_spawned_scrab_id},
    };
}

inline void from_json(const nlohmann::json& j, ScrabSpawnerSaveState& p)
{
    j.at("type").get_to(p.field_0_type);
    j.at("tlv_info").get_to(p.field_4_tlvInfo);
    j.at("state").get_to(p.field_8_state);
    j.at("spawned_scrab_id").get_to(p.field_C_spawned_scrab_id);
}

inline void to_json(nlohmann::json& j, const SlamDoorSaveState& p)
{
    j = nlohmann::json{
    {"tlv_info", p.mTlvInfo},
    };
}

inline void from_json(const nlohmann::json& j, SlamDoorSaveState& p)
{
    j.at("tlv_info").get_to(p.mTlvInfo);
}

inline void to_json(nlohmann::json& j, const SligSaveState& p)
{
    j = nlohmann::json{
        {"type", p.field_0_type},
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
        {"return_to_previous_motion", p.field_4C_return_to_previous_motion},
        {"checked_if_off_screen", p.field_4E_checked_if_off_screen},
        {"input", p.field_50_input},
        {"field_54_timer", p.field_54_timer},
        {"falling_velx_scale_factor", p.field_58_falling_velx_scale_factor},
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
    j.at("type").get_to(p.field_0_type);
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
    j.at("return_to_previous_motion").get_to(p.field_4C_return_to_previous_motion);
    j.at("checked_if_off_screen").get_to(p.field_4E_checked_if_off_screen);
    j.at("input").get_to(p.field_50_input);
    j.at("field_54_timer").get_to(p.field_54_timer);
    j.at("falling_velx_scale_factor").get_to(p.field_58_falling_velx_scale_factor);
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
        {"current_motion", p.mCurrentMotion},
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
    j.at("current_motion").get_to(p.mCurrentMotion);
    j.at("anim_current_frame").get_to(p.mAnimCurrentFrame);
    j.at("frame_change_counter").get_to(p.mFrameChangeCounter);
    j.at("drawable").get_to(p.mDrawable);
    j.at("render").get_to(p.mRender);
    j.at("tlv_id").get_to(p.mTlvId);
    j.at("slurg_state").get_to(p.mSlurgState);
    j.at("going_right").get_to(p.mGoingRight);
    j.at("moving").get_to(p.mMoving);
}

NLOHMANN_JSON_SERIALIZE_ENUM(TimerTriggerStates, {
    {TimerTriggerStates::eWaitForEnabled_0, "wait_for_enabled"},
    {TimerTriggerStates::eWaitForFirstTrigger_1, "wait_for_first_trigger"},
    {TimerTriggerStates::eCheckForStartAgain_2, "check_for_start_again"},
    {TimerTriggerStates::eWaitForSecondTrigger_3, "wait_for_second_trigger"},
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