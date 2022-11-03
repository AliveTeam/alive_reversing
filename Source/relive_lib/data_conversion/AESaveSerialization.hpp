
#include "nlohmann/json.hpp"
#include "AESaveConverter.hpp"
#include "relive_tlvs_serialization.hpp" // needed for some common enum and struct serialization (should probably move those to a different file)

inline void to_json(nlohmann::json& j, const FP& p)
{
    j = nlohmann::json{
        {"fp_value", p.fpValue} };
}

inline void from_json(const nlohmann::json& j, FP& p)
{
    j.at("fp_value").get_to(p.fpValue);
}

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

NLOHMANN_JSON_SERIALIZE_ENUM(SpawnerStates, {
    {SpawnerStates::eInactive_0, "inactive"},
    {SpawnerStates::eSligSpawned_1, "slig_spawned"},
})

inline void to_json(nlohmann::json& j, const SligSpawnerSaveState& p)
{
    j = nlohmann::json{
        {"type", p.mType},
        {"tlv_info", p.mTlvInfo},
        {"state", p.mState},
        {"spawned_slig_id", p.mSpawnedSligId},
    };
}

inline void from_json(const nlohmann::json& j, SligSpawnerSaveState& p)
{
    j.at("type").get_to(p.mType);
    j.at("tlv_info").get_to(p.mTlvInfo);
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
        {"type", p.field_0_type_id},
        {"tlv_info", p.field_4_tlvInfo},
        {"state", p.field_8_state},
    };
}

inline void from_json(const nlohmann::json& j, LiftMoverSaveState& p)
{
    j.at("type").get_to(p.field_0_type_id);
    j.at("tlv_info").get_to(p.field_4_tlvInfo);
    j.at("state").get_to(p.field_8_state);
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
        {"type", p.mAEType},
        {"obj_id", p.field_4_obj_id},
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
        {"base_id", p.field_24_base_id},
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
    j.at("type").get_to(p.mAEType);
    j.at("obj_id").get_to(p.field_4_obj_id);
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
    j.at("base_id").get_to(p.field_24_base_id);
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
        {"type", p.field_0_type},
        {"timer", p.field_4_timer},
    };
}

inline void from_json(const nlohmann::json& j, MinesAlarmSaveState& p)
{
    j.at("type").get_to(p.field_0_type);
    j.at("timer").get_to(p.field_4_timer);
}
