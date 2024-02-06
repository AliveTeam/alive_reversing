#pragma once
#include "../MapWrapper.hpp"
#include "../ObjectTypes.hpp"

// serialization support for each relive tlv type and any supporting nested types
inline void to_json(nlohmann::json& j, const Guid& p)
{
    j = nlohmann::json{
        {"guid", p.ToString()},
    };
}
inline void from_json(const nlohmann::json& j, Guid& p)
{
    // Pull the json value into the string
    std::string tmp;
    j.at("guid").get_to(tmp);
    // Make the guid from the string
    p = Guid::FromString(tmp);
}

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

inline void to_json(nlohmann::json& j, const PSX_Point& p)
{
    j = nlohmann::json{
        {"x", p.x},
        {"y", p.y},
    };
}

inline void from_json(const nlohmann::json& j, PSX_Point& p)
{
    j.at("x").get_to(p.x);
    j.at("y").get_to(p.y);
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

NLOHMANN_JSON_SERIALIZE_ENUM(eLineTypes, {
    {eLineTypes::eNone_m1, "none"},
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
    {eLineTypes::eFlyingObjectWall_17, "flying_object_wall"},
    {eLineTypes::eBackgroundFlyingObjectWall_18, "background_flying_object_wall"},
    {eLineTypes::eDynamicCollision_32, "dynamic_collision"},
    {eLineTypes::eBackgroundDynamicCollision_36, "background_dynamic_collision"},
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
    {ReliveTypes::eAbe, "abe"},
    {ReliveTypes::MainMenuFade, "main_menu_fade"},
    {ReliveTypes::eHoneySack, "honey_sack"},
    {ReliveTypes::eHoney, "honey"},
    {ReliveTypes::eClawOrBirdPortalTerminator, "claw_or_bird_portal_terminator"},
    {ReliveTypes::eMudokon, "mudokon"},
    {ReliveTypes::eLiftPoint, "lift_point"},
    {ReliveTypes::eMeat, "meat"},
    {ReliveTypes::eMeatSack, "meat_sack"},
    {ReliveTypes::eMeatSaw, "meat_saw"},
    {ReliveTypes::eMine, "mine"},
    {ReliveTypes::eRollingBallStopperShaker, "rolling_ball_stopper_shaker"},
    {ReliveTypes::eMotionDetector, "motion_detector"},
    {ReliveTypes::eRollingBallStopper, "rolling_ball_stopper"},
    {ReliveTypes::ePauseMenu, "pause_menu"},
    {ReliveTypes::eParamite, "paramite"},
    {ReliveTypes::eDemoPlayback, "demo_playback"},
    {ReliveTypes::eBirdPortal, "bird_portal"},
    {ReliveTypes::eBirdPortalTerminator, "bird_portal_terminator"},
    {ReliveTypes::eFG1, "fg1"},
    {ReliveTypes::eAbilityRing, "ability_ring"},
    {ReliveTypes::eRock, "rock"},
    {ReliveTypes::eRockSack, "rock_sack"},
    {ReliveTypes::eRollingBall, "rolling_ball"},
    {ReliveTypes::eRope, "rope"},
    {ReliveTypes::eAirExplosion, "air_explosion"},
    {ReliveTypes::eRedLaser, "red_laser"},
    {ReliveTypes::eScrab, "scrab"},
    {ReliveTypes::eScreenClipper, "screen_clipper"},
    {ReliveTypes::eEffectBase, "effect_base"},
    {ReliveTypes::eFade, "fade"},
    {ReliveTypes::eFlash, "flash"},
    {ReliveTypes::eScreenWave, "screen_wave"},
    {ReliveTypes::eUnknown, "unknown"},
    {ReliveTypes::eShrykull, "shrykull"},
    {ReliveTypes::eSlig, "slig"},
    {ReliveTypes::eSlog, "slog"},
    {ReliveTypes::SlingMud, "sling_mud"},
    {ReliveTypes::eSligSpawner, "slig_spawner"},
    {ReliveTypes::eZBall, "z_ball"},
    {ReliveTypes::eParticle, "particle"},
    {ReliveTypes::eZapLine, "zap_line"},
    {ReliveTypes::eBeeSwarm, "bee_swarm"},
    {ReliveTypes::eBeeNest, "bee_nest"},
    {ReliveTypes::eLever, "lever"},
    {ReliveTypes::eTrapDoor, "trap_door"},
    {ReliveTypes::eUXB, "uxb"},
    {ReliveTypes::eMovie, "movie"},
    {ReliveTypes::eCameraSwapper, "camera_swapper"},
    {ReliveTypes::eElectrocute, "electrocute"},
    {ReliveTypes::eTimedMine_or_MovingBomb, "timed_mine_or_moving_bomb"},
    {ReliveTypes::eBone, "bone"},
    {ReliveTypes::eBoneBag, "bone_bag"},
    {ReliveTypes::eBrewMachine, "brew_machine"},
    {ReliveTypes::eSligButton, "slig_button"},
    {ReliveTypes::eExplosionSet, "explosion_set"},
    {ReliveTypes::eZapSpark, "zap_spark"},
    {ReliveTypes::eMetal, "metal"},
    {ReliveTypes::eMinesAlarm, "mines_alarm"},
    {ReliveTypes::eCrawlingSlig, "crawling_slig"},
    {ReliveTypes::eDrill, "drill"},
    {ReliveTypes::eLaughingGas, "laughing_gas"},
    {ReliveTypes::eDoorLock, "door_lock"},
    {ReliveTypes::eDove, "dove"},
    {ReliveTypes::eEvilFart, "evil_fart"},
    {ReliveTypes::eFleech, "fleech"},
    {ReliveTypes::ePossessionFlicker, "possession_flicker"},
    {ReliveTypes::eFlyingSlig, "flying_slig"},
    {ReliveTypes::eFlyingSligSpawner, "flying_slig_spawner"},
    {ReliveTypes::eColourfulMeter, "colourful_meter"},
    {ReliveTypes::eSlapLock_OrbWhirlWind, "slap_lock_orb_whirlwind"},
    {ReliveTypes::eGreeter, "greeter"},
    {ReliveTypes::eGlukkon, "glukkon"},
    {ReliveTypes::eHelpPhone, "help_phone"},
    {ReliveTypes::eEyeOrbPart, "eye_orb_part"},
    {ReliveTypes::eInvisibleEffect, "invisible_effect"},
    {ReliveTypes::ePulley, "pulley"},
    {ReliveTypes::eResourceManager, "resource_manager"},
    {ReliveTypes::eSligGetPantsOrWings, "slig_get_pants_or_wings"},
    {ReliveTypes::eRingOrLiftMud, "ring_or_lift_mud"},
    {ReliveTypes::eSecurityOrb, "security_orb"},
    {ReliveTypes::eText, "text"},
    {ReliveTypes::eMineCar, "mine_car"},
    {ReliveTypes::eGreeterBody, "greeter_body"},
    {ReliveTypes::eMusicController, "music_controller"},
    {ReliveTypes::eMusicTrigger, "music_trigger"},
    {ReliveTypes::ePullRingRope, "pull_ring_rope"},
    {ReliveTypes::eScrabSpawner, "scrab_spawner"},
    {ReliveTypes::eMainMenuTransistion, "main_menu_transition"},
    {ReliveTypes::eScreenShake, "screen_shake"},
    {ReliveTypes::eSlamDoor, "slam_door"},
    {ReliveTypes::eSnoozeParticle, "snooze_particle"},
    {ReliveTypes::eSlurgSpawner, "slurg_spawner"},
    {ReliveTypes::eSlurg, "slurg"},
    {ReliveTypes::eTimerTrigger, "timer_trigger"},
    {ReliveTypes::eTorturedMud, "tortured_mud"},
    {ReliveTypes::eWebLine, "web_line"},
    {ReliveTypes::eWell, "well"},
    {ReliveTypes::eThrowableArray, "throwable_array"},
})