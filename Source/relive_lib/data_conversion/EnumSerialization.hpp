#pragma once

#include <nlohmann/json.hpp>
#include "../MapWrapper.hpp"
#include "../ObjectTypes.hpp"
#include "relive_tlvs.hpp"
#include "../Collisions.hpp"

// clang-format off

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


namespace relive {
NLOHMANN_JSON_SERIALIZE_ENUM(TBlendModes, {
    {TBlendModes::None, "none"},
    {TBlendModes::eBlend_0, "blend_0"},
    {TBlendModes::eBlend_1, "blend_1"},
    {TBlendModes::eBlend_2, "blend_2"},
    {TBlendModes::eBlend_3, "blend_3"},
})

// Common TLV enums
NLOHMANN_JSON_SERIALIZE_ENUM(reliveScale, {
                                              {reliveScale::eFull, "full"},
                                              {reliveScale::eHalf, "half"},
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

// Path_LiftPoint
NLOHMANN_JSON_SERIALIZE_ENUM(Path_LiftPoint::LiftPointStopType, {
                                                                    {Path_LiftPoint::LiftPointStopType::eTopFloor, "top_floor"},
                                                                    {Path_LiftPoint::LiftPointStopType::eBottomFloor, "bottom_floor"},
                                                                    {Path_LiftPoint::LiftPointStopType::eMiddleFloor, "middle_floor"},
                                                                    {Path_LiftPoint::LiftPointStopType::eMiddleLockFloor, "middle_lock_floor"},
                                                                    {Path_LiftPoint::LiftPointStopType::eStartPointOnly, "start_point_only"},
                                                                })

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

// Path_Hoist
NLOHMANN_JSON_SERIALIZE_ENUM(Path_Hoist::Type, {
                                                   {Path_Hoist::Type::eNextFloor, "next_floor"},
                                                   {Path_Hoist::Type::eOffScreen, "off_screen"},
                                               })

NLOHMANN_JSON_SERIALIZE_ENUM(Path_Hoist::GrabDirection, {
                                                            {Path_Hoist::GrabDirection::eFacingLeft, "facing_left"},
                                                            {Path_Hoist::GrabDirection::eFacingRight, "facing_right"},
                                                            {Path_Hoist::GrabDirection::eFacingAnyDirection, "facing_any_direction"},
                                                        })

// Path_InvisibleSwitch
NLOHMANN_JSON_SERIALIZE_ENUM(Path_InvisibleSwitch::InvisibleSwitchScale, {
                                                                             {Path_InvisibleSwitch::InvisibleSwitchScale::eHalf, "half"},
                                                                             {Path_InvisibleSwitch::InvisibleSwitchScale::eFull, "full"},
                                                                             {Path_InvisibleSwitch::InvisibleSwitchScale::eAny, "any"},
                        
                                                 })

                                                 // Path_ElectricWall
NLOHMANN_JSON_SERIALIZE_ENUM(Path_ElectricWall::ElectricWallStartState, {
                                                                            {Path_ElectricWall::ElectricWallStartState::eOff, "off"},
                                                                            {Path_ElectricWall::ElectricWallStartState::eOn, "on"},
                                                                        })

// Path_BoomMachine
NLOHMANN_JSON_SERIALIZE_ENUM(Path_BoomMachine::PipeSide, {
                                                             {Path_BoomMachine::PipeSide::eRight, "right"},
                                                             {Path_BoomMachine::PipeSide::eLeft, "left"},
                                                         })

// Path_UXB
NLOHMANN_JSON_SERIALIZE_ENUM(Path_UXB::StartState, {
                                                       {Path_UXB::StartState::eOn, "on"},
                                                       {Path_UXB::StartState::eOff, "off"},
                                                   })


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

// Path_Edge
NLOHMANN_JSON_SERIALIZE_ENUM(Path_Edge::GrabDirection, {
                                                           {Path_Edge::GrabDirection::eFacingLeft, "facing_left"},
                                                           {Path_Edge::GrabDirection::eFacingRight, "facing_right"},
                                                           {Path_Edge::GrabDirection::eFacingAnyDirection, "facing_any_direction"},
                                                       })


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

// Path_LightEffect
NLOHMANN_JSON_SERIALIZE_ENUM(Path_LightEffect::Type, {
    {Path_LightEffect::Type::Star, "star"},
    {Path_LightEffect::Type::GoldGlow, "gold_glow"},
    {Path_LightEffect::Type::GreenGlow, "green_glow"},
    {Path_LightEffect::Type::FlintGlow, "flint_glow"},
    {Path_LightEffect::Type::Switchable_RedGreenDoorLights, "switchable_red_greendoor_lights"},
    {Path_LightEffect::Type::Switchable_RedGreenHubLight, "switchable_red_green_hub_light"},
})

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

// Path_LiftMover
NLOHMANN_JSON_SERIALIZE_ENUM(Path_LiftMover::YDirection, {
    {Path_LiftMover::YDirection::eDown, "down"},
    {Path_LiftMover::YDirection::eUp, "up"},
})

// Path_MotionDetector
NLOHMANN_JSON_SERIALIZE_ENUM(Path_MotionDetector::InitialMoveDirection, {
    {Path_MotionDetector::InitialMoveDirection::eRight, "right"},
    {Path_MotionDetector::InitialMoveDirection::eLeft, "left"},
})

// Path_BackgroundAnimation
NLOHMANN_JSON_SERIALIZE_ENUM(Path_BackgroundAnimation::BgAnimSounds, {
    {Path_BackgroundAnimation::BgAnimSounds::eNone, "none"},
    {Path_BackgroundAnimation::BgAnimSounds::eFire, "fire"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(Path_BackgroundAnimation::Layer, {
    {Path_BackgroundAnimation::Layer::eLayer0, "layer_0"},
    {Path_BackgroundAnimation::Layer::eLayer2, "layer_2"},
})

// Path_BellsongStone
NLOHMANN_JSON_SERIALIZE_ENUM(Path_BellsongStone::BellsongTypes, {
    {Path_BellsongStone::BellsongTypes::eWhistle, "whistle"},
    {Path_BellsongStone::BellsongTypes::eChimes, "chimes"},
})

// Path_BeeSwarmHole
NLOHMANN_JSON_SERIALIZE_ENUM(Path_BeeSwarmHole::MovementType, {
    {Path_BeeSwarmHole::MovementType::eHover, "hover"},
    {Path_BeeSwarmHole::MovementType::eAttack, "attack"},
    {Path_BeeSwarmHole::MovementType::eFollowPath, "follow_path"},
})

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

// Path_EnemyStopper
NLOHMANN_JSON_SERIALIZE_ENUM(Path_EnemyStopper::StopDirection, {
    {Path_EnemyStopper::StopDirection::Left, "left"},
    {Path_EnemyStopper::StopDirection::Right, "right"},
    {Path_EnemyStopper::StopDirection::Both, "both"},
})

// Path_DoorFlame
NLOHMANN_JSON_SERIALIZE_ENUM(Path_DoorFlame::Colour, {
    {Path_DoorFlame::Colour::eDefault, "default"},
    {Path_DoorFlame::Colour::eRed, "red"},
    {Path_DoorFlame::Colour::eGreen, "green"},
    {Path_DoorFlame::Colour::eBlue, "blue"},
})

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

// Path_FootSwitch
NLOHMANN_JSON_SERIALIZE_ENUM(Path_FootSwitch::FootSwitchTriggerBy, {
    {Path_FootSwitch::FootSwitchTriggerBy::eAbe, "abe"},
    {Path_FootSwitch::FootSwitchTriggerBy::eAnyone, "anyone"},
})

// Path_Paramite
NLOHMANN_JSON_SERIALIZE_ENUM(Path_Paramite::EntranceType, {
    {Path_Paramite::EntranceType::ePatrol, "patrol"},
    {Path_Paramite::EntranceType::eSurpriseWeb, "surprise_web"},
    {Path_Paramite::EntranceType::eSlightlyHigherSpawnSurpriseWeb, "slightly_higher_spawn_surprise_web"},
})

// Path_TrapDoor
NLOHMANN_JSON_SERIALIZE_ENUM(Path_TrapDoor::StartState, {
    {Path_TrapDoor::StartState::eOpen, "open"},
    {Path_TrapDoor::StartState::eClosed, "closed"},
})

// Path_GasEmitter
NLOHMANN_JSON_SERIALIZE_ENUM(Path_GasEmitter::GasColour, {
    {Path_GasEmitter::GasColour::eYellow, "yellow"},
    {Path_GasEmitter::GasColour::eRed, "red"},
    {Path_GasEmitter::GasColour::eGreen, "green"},
    {Path_GasEmitter::GasColour::eBlue, "blue"},
    {Path_GasEmitter::GasColour::eWhite, "white"},
})

// Path_LiftMudokon
NLOHMANN_JSON_SERIALIZE_ENUM(Path_LiftMudokon::Direction, {
    {Path_LiftMudokon::Direction::eRight, "right"},
    {Path_LiftMudokon::Direction::eLeft, "left"},
})

// Path_RingMudokon
NLOHMANN_JSON_SERIALIZE_ENUM(Path_RingMudokon::MustFaceMud, {
    {Path_RingMudokon::MustFaceMud::eYes, "yes"},
    {Path_RingMudokon::MustFaceMud::eNo, "no"},
})

// Path_Scrab
NLOHMANN_JSON_SERIALIZE_ENUM(Path_Scrab::ScrabPatrolType, {
    {Path_Scrab::ScrabPatrolType::eWalk, "walk"},
    {Path_Scrab::ScrabPatrolType::eRunOrWalk192, "run_or_walk_192"},
    {Path_Scrab::ScrabPatrolType::eRunOrWalk128, "run_or_walk_128"},
    {Path_Scrab::ScrabPatrolType::eRunOrWalk64, "run_or_walk_64"},
    {Path_Scrab::ScrabPatrolType::eRun, "run"},
})

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

NLOHMANN_JSON_SERIALIZE_ENUM(Path_Slig::DeathMode, {
    {Path_Slig::DeathMode::FallForward, "fall_forward"},
    {Path_Slig::DeathMode::StandIdle, "stand_idle"},
})

// Path_WheelSyncer
NLOHMANN_JSON_SERIALIZE_ENUM(Path_WheelSyncer::OutputRequirement, {
    {Path_WheelSyncer::OutputRequirement::eAllOn, "all_on"},
    {Path_WheelSyncer::OutputRequirement::e1OnAnd2Off, "1_on_and_2_off"},
    {Path_WheelSyncer::OutputRequirement::e1Or2On, "1_or_2_on"},
    {Path_WheelSyncer::OutputRequirement::e1OnOr2Off, "1_on_or_2_off"},
})

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


// Path_FlyingSlig
NLOHMANN_JSON_SERIALIZE_ENUM(Path_FlyingSlig::SpawnDelayStates, {
    {Path_FlyingSlig::SpawnDelayStates::eMoveImmediately, "move_immediately"},
    {Path_FlyingSlig::SpawnDelayStates::eUseCustomSpawnMoveDelay, "use_custom_spawn_move_delay"},
})

// Path_ScrabSpawner
NLOHMANN_JSON_SERIALIZE_ENUM(Path_ScrabSpawner::SpawnDirection, {
    {Path_ScrabSpawner::SpawnDirection::eNone, "none"},
    {Path_ScrabSpawner::SpawnDirection::eLeft, "left"},
    {Path_ScrabSpawner::SpawnDirection::eRight, "right"},
})

// Path_CrawlingSlig
NLOHMANN_JSON_SERIALIZE_ENUM(Path_CrawlingSlig::CrawlDirection, {
    {Path_CrawlingSlig::CrawlDirection::eLeft, "left"},
    {Path_CrawlingSlig::CrawlDirection::eRight, "right"},
    {Path_CrawlingSlig::CrawlDirection::eRandom, "random"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(Path_CrawlingSlig::StartState, {
    {Path_CrawlingSlig::StartState::eSleeping, ""},
})

// Path_SlogSpawner
NLOHMANN_JSON_SERIALIZE_ENUM(Path_SlogSpawner::StartDirection, {
    {Path_SlogSpawner::StartDirection::eRight, "right"},
    {Path_SlogSpawner::StartDirection::eLeft, "left"},
})

} // namespace relive
