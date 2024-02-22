#pragma once
#include "../MapWrapper.hpp"
#include "../ObjectTypes.hpp"
#include "relive_tlvs.hpp"

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
    {ReliveTypes::eCrawlingSligButton, relive::Path_CrawlingSligButton::kClassName},
    {ReliveTypes::eWheelSyncer, relive::Path_WheelSyncer::kClassName},
    {ReliveTypes::eDemoSpawnPoint, relive::Path_DemoSpawnPoint::kClassName},
    {ReliveTypes::eMultiSwitchController, relive::Path_MultiSwitchController::kClassName},
    {ReliveTypes::eStatusLight, relive::Path_StatusLight::kClassName},
    {ReliveTypes::eSlapLock, relive::Path_SlapLock::kClassName},
    {ReliveTypes::eParamiteWebLine, relive::Path_ParamiteWebLine::kClassName},
    {ReliveTypes::eGlukkonSwitch, relive::Path_GlukkonSwitch::kClassName},
    {ReliveTypes::eDoorBlocker, relive::Path_DoorBlocker::kClassName},
    {ReliveTypes::eTrainDoor, relive::Path_TrainDoor::kClassName},
    {ReliveTypes::eLevelLoader, relive::Path_LevelLoader::kClassName},
    {ReliveTypes::eSligGetWings, relive::Path_SligGetWings::kClassName},
    {ReliveTypes::eSligGetPants, relive::Path_SligGetPants::kClassName},
    {ReliveTypes::eTeleporter, relive::Path_Teleporter::kClassName},
    {ReliveTypes::eWater, relive::Path_Water::kClassName},
    {ReliveTypes::eWorkWheel, relive::Path_WorkWheel::kClassName},
    {ReliveTypes::eLCDScreen, relive::Path_LCDScreen::kClassName},
    {ReliveTypes::eInvisibleSwitch, relive::Path_InvisibleSwitch::kClassName},
    {ReliveTypes::eDoorFlame, relive::Path_DoorFlame::kClassName},
    {ReliveTypes::eMovingBomb, relive::Path_MovingBomb::kClassName},
    {ReliveTypes::eMainMenuController, relive::Path_MainMenuController::kClassName},
    {ReliveTypes::eHintFly, relive::Path_HintFly::kClassName},
    {ReliveTypes::eSecurityDoor, relive::Path_SecurityDoor::kClassName},
    {ReliveTypes::eCreditsController, relive::Path_CreditsController::kClassName},
    {ReliveTypes::eLCDStatusBoard, relive::Path_LCDStatusBoard::kClassName},
    {ReliveTypes::eSwitchStateBooleanLogic, "switch_state_boolean_logic"},
    {ReliveTypes::eLightEffect, relive::Path_LightEffect::kClassName},
    {ReliveTypes::eSlogSpawner, relive::Path_SlogSpawner::kClassName},
    {ReliveTypes::eGasEmitter, relive::Path_GasEmitter::kClassName},
    {ReliveTypes::eRingCancel, relive::Path_RingCancel::kClassName},
    {ReliveTypes::eElumWall, relive::Path_ElumWall::kClassName},
    {ReliveTypes::eAbeStart, relive::Path_AbeStart::kClassName},
    {ReliveTypes::eBeeSwarmHole, relive::Path_BeeSwarmHole::kClassName},
    {ReliveTypes::eFallingItem, relive::Path_FallingItem::kClassName},
    {ReliveTypes::eShadowZone, relive::Path_ShadowZone::kClassName},
    {ReliveTypes::eStartController, relive::Path_StartController::kClassName},
    {ReliveTypes::eBirdPortalExit, relive::Path_BirdPortalExit::kClassName},
    {ReliveTypes::eHoneyDripTarget, relive::Path_HoneyDripTarget::kClassName},
    {ReliveTypes::ePathTransition, relive::Path_PathTransition::kClassName},
    {ReliveTypes::eZSligCover, relive::Path_ZSligCover::kClassName},
    {ReliveTypes::eResetPath, relive::Path_ResetPath::kClassName},
    {ReliveTypes::eElumPathTrans, relive::Path_ElumPathTrans::kClassName},
    {ReliveTypes::eScrabBoundLeft, relive::Path_ScrabBoundLeft::kClassName},
    {ReliveTypes::eScrabBoundRight, relive::Path_ScrabBoundRight::kClassName},
    {ReliveTypes::eScrabNoFall, relive::Path_ScrabNoFall::kClassName},
    {ReliveTypes::eMovingBombStopper, relive::Path_MovingBombStopper::kClassName},
    {ReliveTypes::eElumStart, relive::Path_ElumStart::kClassName},
    {ReliveTypes::eEdge, relive::Path_Edge::kClassName},
    {ReliveTypes::eSoftLanding, relive::Path_SoftLanding::kClassName},
    {ReliveTypes::eMovieHandStone, relive::Path_MovieStone::kClassName},
    {ReliveTypes::eBellSongStone, relive::Path_BellsongStone::kClassName},
    {ReliveTypes::eDemoPlaybackStone, "demo_playback_stone"},
    {ReliveTypes::eHandStone, relive::Path_HandStone::kClassName},
    {ReliveTypes::eHoist, relive::Path_Hoist::kClassName},
    {ReliveTypes::eContinuePoint, relive::Path_ContinuePoint::kClassName},
    {ReliveTypes::eWellLocal, relive::Path_WellLocal::kClassName},
    {ReliveTypes::eWellExpress, relive::Path_WellExpress::kClassName},
    {ReliveTypes::eMudokonPathTrans, relive::Path_MudokonPathTrans::kClassName},
    {ReliveTypes::eRingMudokon, relive::Path_RingMudokon::kClassName},
    {ReliveTypes::eLiftMudokon, relive::Path_LiftMudokon::kClassName},
    {ReliveTypes::eInvisibleZone, relive::Path_InvisibleZone::kClassName},
    {ReliveTypes::eEnemyStopper, relive::Path_EnemyStopper::kClassName},
    {ReliveTypes::eSligBoundLeft, relive::Path_SligBoundLeft::kClassName},
    {ReliveTypes::eSligBoundRight, relive::Path_SligBoundRight::kClassName},
    {ReliveTypes::eSligPersist, relive::Path_SligPersist::kClassName},
    {ReliveTypes::eZzzSpawner, relive::Path_ZzzSpawner::kClassName},
    {ReliveTypes::eKillUnsavedMuds, relive::Path_KillUnsavedMuds::kClassName},
    {ReliveTypes::eDeathDrop, relive::Path_DeathDrop::kClassName},
    {ReliveTypes::eAlarm, relive::Path_Alarm::kClassName},
    {ReliveTypes::eScreenManager, "screen_manager"},
    {ReliveTypes::eBackgroundAnimation, relive::Path_BackgroundAnimation::kClassName},
    {ReliveTypes::eBat, relive::Path_Bat::kClassName},
    {ReliveTypes::eLiftMover, relive::Path_LiftMover::kClassName},
    {ReliveTypes::eTimedMine, relive::Path_TimedMine::kClassName},
    {ReliveTypes::eBullet, "bullet"},
    {ReliveTypes::eDDCheat, "ddcheat"},
    {ReliveTypes::eBells, "bells"},
    {ReliveTypes::eChimeLock, relive::Path_ChimeLock::kClassName},
    {ReliveTypes::eGasCountDown, relive::Path_GasCountDown::kClassName},
    {ReliveTypes::eParticleBurst, "particle_burst"},
    {ReliveTypes::eDoor, relive::Path_Door::kClassName},
    {ReliveTypes::eGameSpeak, "gamespeak"},
    {ReliveTypes::eElectricWall, relive::Path_ElectricWall::kClassName},
    {ReliveTypes::eElum, relive::Path_Elum::kClassName},
    {ReliveTypes::eBellHammer, relive::Path_BellHammer::kClassName},
    {ReliveTypes::ePalOverwriter, "pal_overwriter"},
    {ReliveTypes::eGroundExplosion, "ground_explosion"},
    {ReliveTypes::eSecurityClaw, relive::Path_SecurityClaw::kClassName},
    {ReliveTypes::eRockSpawner, "rock_spawner"},
    {ReliveTypes::eFlintLockFire, relive::Path_FlintLockFire::kClassName},
    {ReliveTypes::eThrowableTotalIndicator, "throwable_total_indicator"},
    {ReliveTypes::eFootSwitch, relive::Path_FootSwitch::kClassName},
    {ReliveTypes::eGameEnderController, "game_ender_controller"},
    {ReliveTypes::eDeathBird, "death_bird"},
    {ReliveTypes::eLoadingFile, "loading_file"},
    {ReliveTypes::eGrenade, "grenade"},
    {ReliveTypes::eBoomMachine, relive::Path_BoomMachine::kClassName},
    {ReliveTypes::eBackgroundGlukkon, relive::Path_BackgroundGlukkon::kClassName},
    {ReliveTypes::eAbe, "abe"},
    {ReliveTypes::MainMenuFade, "main_menu_fade"},
    {ReliveTypes::eHoneySack, relive::Path_HoneySack::kClassName},
    {ReliveTypes::eHoney, relive::Path_Honey::kClassName},
    {ReliveTypes::eClawOrBirdPortalTerminator, "claw_or_bird_portal_terminator"},
    {ReliveTypes::eMudokon, relive::Path_Mudokon::kClassName},
    {ReliveTypes::eLiftPoint, relive::Path_LiftPoint::kClassName},
    {ReliveTypes::eMeat, "meat"},
    {ReliveTypes::eMeatSack, relive::Path_MeatSack::kClassName},
    {ReliveTypes::eMeatSaw, relive::Path_MeatSaw::kClassName},
    {ReliveTypes::eMine, relive::Path_Mine::kClassName},
    {ReliveTypes::eRollingBallStopperShaker, "rolling_ball_stopper_shaker"},
    {ReliveTypes::eMotionDetector, relive::Path_MotionDetector::kClassName},
    {ReliveTypes::eRollingBallStopper, relive::Path_RollingBallStopper::kClassName},
    {ReliveTypes::ePauseMenu, "pause_menu"},
    {ReliveTypes::eParamite, relive::Path_Paramite::kClassName},
    {ReliveTypes::eDemoPlayback, "demo_playback"},
    {ReliveTypes::eBirdPortal, relive::Path_BirdPortal::kClassName},
    {ReliveTypes::eBirdPortalTerminator, "bird_portal_terminator"},
    {ReliveTypes::eFG1, "fg1"},
    {ReliveTypes::eAbilityRing, "ability_ring"},
    {ReliveTypes::eRock, "rock"},
    {ReliveTypes::eRockSack, relive::Path_RockSack::kClassName},
    {ReliveTypes::eRollingBall, relive::Path_RollingBall::kClassName},
    {ReliveTypes::eRope, "rope"},
    {ReliveTypes::eAirExplosion, "air_explosion"},
    {ReliveTypes::eRedLaser, "red_laser"},
    {ReliveTypes::eScrab, relive::Path_Scrab::kClassName},
    {ReliveTypes::eScreenClipper, "screen_clipper"},
    {ReliveTypes::eEffectBase, "effect_base"},
    {ReliveTypes::eFade, "fade"},
    {ReliveTypes::eFlash, "flash"},
    {ReliveTypes::eScreenWave, "screen_wave"},
    {ReliveTypes::eUnknown, "unknown"},
    {ReliveTypes::eShrykull, "shrykull"},
    {ReliveTypes::eSlig, relive::Path_Slig::kClassName},
    {ReliveTypes::eSlog, relive::Path_Slog::kClassName},
    {ReliveTypes::SlingMud, relive::Path_SlingMudokon::kClassName},
    {ReliveTypes::eSligSpawner, relive::Path_SligSpawner::kClassName},
    {ReliveTypes::eZBall, relive::Path_ZBall::kClassName},
    {ReliveTypes::eParticle, "particle"},
    {ReliveTypes::eZapLine, "zap_line"},
    {ReliveTypes::eBeeSwarm, "bee_swarm"},
    {ReliveTypes::eBeeNest, relive::Path_BeeNest::kClassName},
    {ReliveTypes::eLever, relive::Path_Lever::kClassName},
    {ReliveTypes::eTrapDoor, relive::Path_TrapDoor::kClassName},
    {ReliveTypes::eUXB, relive::Path_UXB::kClassName},
    {ReliveTypes::eMovie, "movie"},
    {ReliveTypes::eCameraSwapper, "camera_swapper"},
    {ReliveTypes::eElectrocute, "electrocute"},
    {ReliveTypes::eTimedMine_or_MovingBomb, "timed_mine_or_moving_bomb"},
    {ReliveTypes::eBone, "bone"},
    {ReliveTypes::eBoneBag, relive::Path_BoneBag::kClassName},
    {ReliveTypes::eBrewMachine, relive::Path_BrewMachine::kClassName},
    {ReliveTypes::eSligButton, "slig_button"}, // TODO: eCrawlingSligButton?
    {ReliveTypes::eExplosionSet, relive::Path_ExplosionSet::kClassName},
    {ReliveTypes::eZapSpark, "zap_spark"},
    {ReliveTypes::eMetal, "metal"},
    {ReliveTypes::eMinesAlarm, "mines_alarm"},
    {ReliveTypes::eCrawlingSlig, relive::Path_CrawlingSlig::kClassName},
    {ReliveTypes::eDrill, relive::Path_Drill::kClassName},
    {ReliveTypes::eLaughingGas, relive::Path_LaughingGas::kClassName},
    {ReliveTypes::eDoorLock, "door_lock"}, // TODO: eDoorBlocker?
    {ReliveTypes::eDove, relive::Path_Dove::kClassName},
    {ReliveTypes::eEvilFart, "evil_fart"},
    {ReliveTypes::eFleech, relive::Path_Fleech::kClassName},
    {ReliveTypes::ePossessionFlicker, "possession_flicker"},
    {ReliveTypes::eFlyingSlig, relive::Path_FlyingSlig::kClassName},
    {ReliveTypes::eFlyingSligSpawner, relive::Path_FlyingSligSpawner::kClassName},
    {ReliveTypes::eColourfulMeter, relive::Path_ColourfulMeter::kClassName},
    {ReliveTypes::eSlapLock_OrbWhirlWind, "slap_lock_orb_whirlwind"},
    {ReliveTypes::eGreeter, relive::Path_Greeter::kClassName},
    {ReliveTypes::eGlukkon, relive::Path_Glukkon::kClassName},
    {ReliveTypes::eHelpPhone, "help_phone"}, // TODO: probably has a Path_ struct
    {ReliveTypes::eEyeOrbPart, "eye_orb_part"},
    {ReliveTypes::eInvisibleEffect, "invisible_effect"},
    {ReliveTypes::ePulley, relive::Path_Pulley::kClassName},
    {ReliveTypes::eResourceManager, "resource_manager"},
    {ReliveTypes::eSligGetPantsOrWings, "slig_get_pants_or_wings"},
    {ReliveTypes::eRingOrLiftMud, "ring_or_lift_mud"},
    {ReliveTypes::eSecurityOrb, relive::Path_SecurityOrb::kClassName},
    {ReliveTypes::eText, "text"},
    {ReliveTypes::eMineCar, relive::Path_MineCar::kClassName},
    {ReliveTypes::eGreeterBody, "greeter_body"},
    {ReliveTypes::eMusicController, "music_controller"},
    {ReliveTypes::eMusicTrigger, relive::Path_MusicTrigger::kClassName},
    {ReliveTypes::ePullRingRope, relive::Path_PullRingRope::kClassName},
    {ReliveTypes::eScrabSpawner, relive::Path_ScrabSpawner::kClassName},
    {ReliveTypes::eMainMenuTransistion, "main_menu_transition"},
    {ReliveTypes::eScreenShake, "screen_shake"},
    {ReliveTypes::eSlamDoor, relive::Path_SlamDoor::kClassName},
    {ReliveTypes::eSnoozeParticle, "snooze_particle"},
    {ReliveTypes::eSlurgSpawner, relive::Path_SlurgSpawner::kClassName},
    {ReliveTypes::eSlurg, relive::Path_Slurg::kClassName},
    {ReliveTypes::eTimerTrigger, relive::Path_TimerTrigger::kClassName},
    {ReliveTypes::eTorturedMud, relive::Path_TorturedMudokon::kClassName},
    {ReliveTypes::eWebLine, "web_line"},
    {ReliveTypes::eWell, "well"},
    {ReliveTypes::eThrowableArray, "throwable_array"}
})