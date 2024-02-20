#pragma once

#include <QString>
#include <QObject>
#include <array>
#include "../../../relive_lib/data_conversion/relive_tlvs.hpp"

template<typename EnumType> struct EnumReflector final  { };

#define REFLECT_ENUM(ENUM_TYPE, ...)                                                      \
    template <>                                                                           \
    struct EnumReflector<ENUM_TYPE> final                                                 \
    {                                                                                     \
        static_assert(std::is_enum<ENUM_TYPE>::value, #ENUM_TYPE " must be an enum!");    \
        struct EnumPair final                                                             \
        {                                                                                 \
            ENUM_TYPE mValue;                                                             \
            const QString mName;                                                            \
        };                                                                                \
                                                                                          \
        static s32 to_index(ENUM_TYPE e)                                                  \
        {                                                                                 \
            s32 idx = 0;                                                                  \
            for (auto& v : mArray)                                                        \
            {                                                                             \
                if (v.mValue == e)                                                        \
                {                                                                         \
                    return idx;                                                           \
                }                                                                         \
                idx++;                                                                    \
            }                                                                             \
            return -1;                                                                    \
        }                                                                                 \
                                                                                          \
        static const QString to_str(s32 idx)                                                \
        {                                                                                 \
            return mArray[idx].mName;                                                     \
        }                                                                                 \
                                                                                          \
        static ENUM_TYPE to_value(s32 idx)                                                \
        {                                                                                 \
            return mArray[idx].mValue;                                                    \
        }                                                                                 \
        static const inline EnumPair tmp[] = __VA_ARGS__;                                    \
        static const inline std::array<EnumPair, ALIVE_COUNTOF(tmp)> mArray = {__VA_ARGS__}; \
    };

REFLECT_ENUM(ReliveTypes, {
    {ReliveTypes::eNone, QObject::tr("None")},
    {ReliveTypes::eCrawlingSligButton, "Crawling Slig Button"},
    {ReliveTypes::eWheelSyncer, "Wheel Syncer"},
    {ReliveTypes::eDemoSpawnPoint, "Demo Spawn Point"},
    {ReliveTypes::eMultiSwitchController, "Multi Switch Controller"},
    {ReliveTypes::eStatusLight, "Status Light"},
    {ReliveTypes::eSlapLock, "Slap Lock"},
    {ReliveTypes::eParamiteWebLine, "Paramite Web Line"},
    {ReliveTypes::eGlukkonSwitch, "Glukkon Switch"},
    {ReliveTypes::eDoorBlocker, "Door Blocker"},
    {ReliveTypes::eTrainDoor, "Train Door"},
    {ReliveTypes::eLevelLoader, "Level Loader"},
    {ReliveTypes::eSligGetWings, "Slig Get Wings"},
    {ReliveTypes::eSligGetPants, "Slig Get Pants"},
    {ReliveTypes::eTeleporter, "Teleporter"},
    {ReliveTypes::eWater, "Water"},
    {ReliveTypes::eWorkWheel, "Work Wheel"},
    {ReliveTypes::eLCDScreen, "LCD Screen"},
    {ReliveTypes::eInvisibleSwitch, "Invisible Switch"},
    {ReliveTypes::eDoorFlame, "Door Flame"},
    {ReliveTypes::eMovingBomb, "Moving Bomb"},
    {ReliveTypes::eMainMenuController, "Main Menu Controller"},
    {ReliveTypes::eHintFly, "Hint Fly"},
    {ReliveTypes::eSecurityDoor, "Security Door"},
    {ReliveTypes::eCreditsController, "Credits Controller"},
    {ReliveTypes::eLCDStatusBoard, "LCD Status Board"},
    {ReliveTypes::eSwitchStateBooleanLogic, "Switch State Boolean Logic"},
    {ReliveTypes::eLightEffect, "Light Effect"},
    {ReliveTypes::eSlogSpawner, "Slog Spawner"},
    {ReliveTypes::eGasEmitter, "Gas Emitter"},
    {ReliveTypes::eRingCancel, "Ring Cancel"},
    {ReliveTypes::eElumWall, "Elum Wall"},
    {ReliveTypes::eAbeStart, "Abe Start"},
    {ReliveTypes::eBeeSwarmHole, "Bee Swarm Hole"},
    {ReliveTypes::eFallingItem, "Falling Item"},
    {ReliveTypes::eShadowZone, "Shadow Zone"},
    {ReliveTypes::eStartController, "Start Controller"},
    {ReliveTypes::eBirdPortalExit, "Bird Portal Exit"},
    {ReliveTypes::eHoneyDripTarget, "Honey Drip Target"},
    {ReliveTypes::ePathTransition, "Path Transition"},
    {ReliveTypes::eZSligCover, "Z Slig Cover"},
    {ReliveTypes::eResetPath, "Reset Path"},
    {ReliveTypes::eElumPathTrans, "Elum Path Trans"},
    {ReliveTypes::eScrabLeftBound, "Scrab Left Bound"},
    {ReliveTypes::eScrabRightBound, "Scrab Right Bound"},
    {ReliveTypes::eScrabNoFall, "Scrab No Fall"},
    {ReliveTypes::eMovingBombStopper, "Moving Bomb Stopper"},
    {ReliveTypes::eElumStart, "Elum Start"},
    {ReliveTypes::eEdge, "Edge"},
    {ReliveTypes::eSoftLanding, "Soft Landing"},
    {ReliveTypes::eMovieHandStone, "Movie Hand Stone"},
    {ReliveTypes::eBellSongStone, "Bell Song Stone"},
    {ReliveTypes::eDemoPlaybackStone, "Demo Playback Stone"},
    {ReliveTypes::eHandStone, "Hand Stone"},
    {ReliveTypes::eHoist, "Hoist"},
    {ReliveTypes::eContinuePoint, "Continue Point"},
    {ReliveTypes::eWellLocal, "Well Local"},
    {ReliveTypes::eWellExpress, "Well Express"},
    {ReliveTypes::eMudokonPathTrans, "Mudokon Path Trans"},
    {ReliveTypes::eRingMudokon, "Ring Mudokon"},
    {ReliveTypes::eLiftMudokon, "Lift Mudokon"},
    {ReliveTypes::eInvisibleZone, "Invisible Zone"},
    {ReliveTypes::eEnemyStopper, "Enemy Stopper"},
    {ReliveTypes::eSligBoundLeft, "Slig Bound Left"},
    {ReliveTypes::eSligBoundRight, "Slig Boung Right"},
    {ReliveTypes::eSligPersist, "Slig Persist"},
    {ReliveTypes::eZzzSpawner, "Zzz Spawner"},
    {ReliveTypes::eKillUnsavedMuds, "Kill Unsaved Muds"},
    {ReliveTypes::eDeathDrop, "Death Drop"},
    {ReliveTypes::eAlarm, "Alarm"},
    {ReliveTypes::eScreenManager, "Screen Manager"},
    {ReliveTypes::eBackgroundAnimation, "Background Animation"},
    {ReliveTypes::eBat, "Bat"},
    {ReliveTypes::eLiftMover, "Lift Mover"},
    {ReliveTypes::eTimedMine, "Timed Mine"},
    {ReliveTypes::eBullet, "Bullet"},
    {ReliveTypes::eDDCheat, "DDCheat"},
    {ReliveTypes::eBells, "Bells"},
    {ReliveTypes::eChimeLock, "Chime Lock"},
    {ReliveTypes::eGasCountDown, "Gas Count Down"},
    {ReliveTypes::eParticleBurst, "Particle Burst"},
    {ReliveTypes::eDoor, "Door"},
    {ReliveTypes::eGameSpeak, "Game Speak"},
    {ReliveTypes::eElectricWall, "Electric Wall"},
    {ReliveTypes::eElum, "Elum"},
    {ReliveTypes::eBellHammer, "Bell Hammer"},
    {ReliveTypes::ePalOverwriter, "Pal Overwritter"},
    {ReliveTypes::eGroundExplosion, "Ground Explosion"},
    {ReliveTypes::eSecurityClaw, "Security Claw"},
    {ReliveTypes::eRockSpawner, "Rock Spawner"},
    {ReliveTypes::eFlintLockFire, "Flint Lock Fire"},
    {ReliveTypes::eThrowableTotalIndicator, "Throwable Total Indicator"},
    {ReliveTypes::eFootSwitch, "Foot Switch"},
    {ReliveTypes::eGameEnderController, "Game Ender Controller"},
    {ReliveTypes::eDeathBird, "Death Bird"},
    {ReliveTypes::eLoadingFile, "Loading File"},
    {ReliveTypes::eGrenade, "Grenade"},
    {ReliveTypes::eBoomMachine, "Boom Machine"},
    {ReliveTypes::eBackgroundGlukkon, "Background Glukkon"},
    {ReliveTypes::eAbe, "Abe"},
    {ReliveTypes::MainMenuFade, "Main Menu Fade"},
    {ReliveTypes::eHoneySack, "Honey Sack"},
    {ReliveTypes::eHoney, "Honey"},
    {ReliveTypes::eClawOrBirdPortalTerminator, "Claw OR Bird Portal Terminator"},
    {ReliveTypes::eMudokon, "Mudokon"},
    {ReliveTypes::eLiftPoint, "Lift Point"},
    {ReliveTypes::eMeat, "Meat"},
    {ReliveTypes::eMeatSack, "Meat Sack"},
    {ReliveTypes::eMeatSaw, "Meat Saw"},
    {ReliveTypes::eMine, "Mine"},
    {ReliveTypes::eRollingBallStopperShaker, "Rolling Ball Stopper Shaker"},
    {ReliveTypes::eMotionDetector, "Motion Detector"},
    {ReliveTypes::eRollingBallStopper, "Rolling Ball Stopper"},
    {ReliveTypes::ePauseMenu, "Pause Menu"},
    {ReliveTypes::eParamite, "Paramite"},
    {ReliveTypes::eDemoPlayback, "Demo Playback"},
    {ReliveTypes::eBirdPortal, "Bird Portal"},
    {ReliveTypes::eBirdPortalTerminator, "Bird Portal Terminator"},
    {ReliveTypes::eFG1, "FG1"},
    {ReliveTypes::eAbilityRing, "Ability Ring"},
    {ReliveTypes::eRock, "Rock"},
    {ReliveTypes::eRockSack, "Rock Sack"},
    {ReliveTypes::eRollingBall, "Rolling Ball"},
    {ReliveTypes::eRope, "Rope"},
    {ReliveTypes::eAirExplosion, "Air Explosion"},
    {ReliveTypes::eRedLaser, "Red Laser"},
    {ReliveTypes::eScrab, "Scrab"},
    {ReliveTypes::eScreenClipper, "Screen Clipper"},
    {ReliveTypes::eEffectBase, "Effect Base"},
    {ReliveTypes::eFade, "Fade"},
    {ReliveTypes::eFlash, "Flash"},
    {ReliveTypes::eScreenWave, "Screen Wave"},
    {ReliveTypes::eUnknown, "Unknown"},
    {ReliveTypes::eShrykull, "Shrykull"},
    {ReliveTypes::eSlig, "Slig"},
    {ReliveTypes::eSlog, "Slog"},
    {ReliveTypes::SlingMud, "Sling Mudokon"},
    {ReliveTypes::eSligSpawner, "Slig Spawner"},
    {ReliveTypes::eZBall, "Z Ball"},
    {ReliveTypes::eParticle, "Particle"},
    {ReliveTypes::eZapLine, "Zap Line"},
    {ReliveTypes::eBeeSwarm, "Bee Swarm"},
    {ReliveTypes::eBeeNest, "Bee Nest"},
    {ReliveTypes::eLever, "Lever"},
    {ReliveTypes::eTrapDoor, "Trap Door"},
    {ReliveTypes::eUXB, "UXB"},
    {ReliveTypes::eMovie, "Movie"},
    {ReliveTypes::eCameraSwapper, "Camera Swapper"},
    {ReliveTypes::eElectrocute, "Electrocute"},
    {ReliveTypes::eTimedMine_or_MovingBomb, "Timed Mine Or Moviing Bomb"},
    {ReliveTypes::eBone, "Bone"},
    {ReliveTypes::eBoneBag, "Bone Bag"},
    {ReliveTypes::eBrewMachine, "Brew Machine"},
    {ReliveTypes::eSligButton, "Slig Button"},
    {ReliveTypes::eExplosionSet, "Explosion Set"},
    {ReliveTypes::eZapSpark, "Zap Spark"},
    {ReliveTypes::eMetal, "Metal"},
    {ReliveTypes::eMinesAlarm, "Mines Alarm"},
    {ReliveTypes::eCrawlingSlig, "Crawling Slig"},
    {ReliveTypes::eDrill, "Drill"},
    {ReliveTypes::eLaughingGas, "Laughing Gas"},
    {ReliveTypes::eDoorLock, "Door Lock"},
    {ReliveTypes::eDove, "Dove"},
    {ReliveTypes::eEvilFart, "Evil Fart"},
    {ReliveTypes::eFleech, "Fleech"},
    {ReliveTypes::ePossessionFlicker, "Possession Flicker"},
    {ReliveTypes::eFlyingSlig, "Flying Slig"},
    {ReliveTypes::eFlyingSligSpawner, "Flying Slig Spawner"},
    {ReliveTypes::eColourfulMeter, "Colourful Meter"},
    {ReliveTypes::eSlapLock_OrbWhirlWind, "Slap Lock Orb Whirl WInd"},
    {ReliveTypes::eGreeter, "Greeter"},
    {ReliveTypes::eGlukkon, "Glukkon"},
    {ReliveTypes::eHelpPhone, "Help Phone"},
    {ReliveTypes::eEyeOrbPart, "Eye Orb Part"},
    {ReliveTypes::eInvisibleEffect, "Invisible Effect"},
    {ReliveTypes::ePulley, "Pulley"},
    {ReliveTypes::eResourceManager, "Resource Manager"},
    {ReliveTypes::eSligGetPantsOrWings, "Slig Get Pants Or Wings"},
    {ReliveTypes::eRingOrLiftMud, "Ring Or Lift Mud"},
    {ReliveTypes::eSecurityOrb, "Security Orb"},
    {ReliveTypes::eText, "Text"},
    {ReliveTypes::eMineCar, "Mine Car"},
    {ReliveTypes::eGreeterBody, "Greeter Body"},
    {ReliveTypes::eMusicController, "Music Controller"},
    {ReliveTypes::eMusicTrigger, "Music Trigger"},
    {ReliveTypes::ePullRingRope, "Pull RIng Rope"},
    {ReliveTypes::eScrabSpawner, "Scrab Spawner"},
    {ReliveTypes::eMainMenuTransistion, "Main Menu Transition"},
    {ReliveTypes::eScreenShake, "Screen Shake"},
    {ReliveTypes::eSlamDoor, "Slam Door"},
    {ReliveTypes::eSnoozeParticle, "Snooze Particle"},
    {ReliveTypes::eSlurgSpawner, "Slurg Spawner"},
    {ReliveTypes::eSlurg, "Slurg"},
    {ReliveTypes::eTimerTrigger, "Timer Trigger"},
    {ReliveTypes::eTorturedMud, "Tortured Mud"},
    {ReliveTypes::eWebLine, "Web Line"},
    {ReliveTypes::eWell, "Well"},
    {ReliveTypes::eThrowableArray, "Throwable Array"},
})

REFLECT_ENUM(relive::reliveScale,
{
    {relive::reliveScale::eFull, "Full"},
    {relive::reliveScale::eHalf, "Half"}
})

REFLECT_ENUM(relive::Path_Mudokon::Mud_TLV_Emotion,
{
    {relive::Path_Mudokon::Mud_TLV_Emotion::eNormal, "Normal"},
    {relive::Path_Mudokon::Mud_TLV_Emotion::eAngry, "Angry"},
    {relive::Path_Mudokon::Mud_TLV_Emotion::eSad, "Sad"},
    {relive::Path_Mudokon::Mud_TLV_Emotion::eWired, "Wired"},
    {relive::Path_Mudokon::Mud_TLV_Emotion::eSick, "Sick"}
})

REFLECT_ENUM(relive::Path_BirdPortal::PortalSide,
{
    {relive::Path_BirdPortal::PortalSide::eRight, "Right"},
    {relive::Path_BirdPortal::PortalSide::eLeft, "Left"}
})

REFLECT_ENUM(relive::Path_FlyingSlig::SpawnDelayStates,
{
    {relive::Path_FlyingSlig::SpawnDelayStates::eMoveImmediately, "Move Immediately"},
    {relive::Path_FlyingSlig::SpawnDelayStates::eUseCustomSpawnMoveDelay, "Use Custom Spawn Move Delay"}
})

REFLECT_ENUM(relive::Path_Slig_Data::DeathMode,
{
    {relive::Path_Slig_Data::DeathMode::StandIdle, "Stand Idle"},
    {relive::Path_Slig_Data::DeathMode::FallForward, "Fall Forward"}
})

REFLECT_ENUM(relive::Path_Slig_Data::StartState,
{
    {relive::Path_Slig_Data::StartState::Listening, "Listening"},
    {relive::Path_Slig_Data::StartState::Patrol, "Patrol"},
    {relive::Path_Slig_Data::StartState::Sleeping, "Sleeping"},
    {relive::Path_Slig_Data::StartState::Chase, "Chase"},
    {relive::Path_Slig_Data::StartState::ChaseAndDisappear, "Chase And Disappear"},
    {relive::Path_Slig_Data::StartState::eFallingToChase, "Falling To Chase"},
    {relive::Path_Slig_Data::StartState::ListeningToGlukkon, "Listening To Glukkon"},
});

REFLECT_ENUM(relive::reliveXDirection,
{
    {relive::reliveXDirection::eLeft, "Left"},
    {relive::reliveXDirection::eRight, "Right"},
});

REFLECT_ENUM(relive::reliveSwitchOp,
{
    {relive::reliveSwitchOp::eSetTrue, "Set True"},
    {relive::reliveSwitchOp::eSetFalse, "Set False"},
    {relive::reliveSwitchOp::eToggle, "Toggle"},
    {relive::reliveSwitchOp::eIncrement, "Increment"},
    {relive::reliveSwitchOp::eDecrement, "Decrement"},
});

REFLECT_ENUM(eLineTypes,
{
    {eLineTypes::eFloor_0, "Floor"},
    {eLineTypes::eWallLeft_1, "Wall Left"},
    {eLineTypes::eWallRight_2, "Wall Right"},
    {eLineTypes::eCeiling_3, "Ceiling"},
    {eLineTypes::eBackgroundFloor_4, "Background Floor"},
    {eLineTypes::eBackgroundWallLeft_5, "Background Wall Left"},
    {eLineTypes::eBackgroundWallRight_6, "Background Wall Right"},
    {eLineTypes::eBackgroundCeiling_7, "Background Ceiling"},
    {eLineTypes::eTrackLine_8, "Track Line"},
    {eLineTypes::eArt_9, "Art"},
    {eLineTypes::eBulletWall_10, "Bullet Wall"},
    {eLineTypes::eMineCarFloor_11, "Mine Car Floor"},
    {eLineTypes::eMineCarWall_12, "Mine Car Wall"},
    {eLineTypes::eMineCarCeiling_13, "Mine Car Ceiling"},
    {eLineTypes::eBackgroundMineCarFloor_14, "Background Mine Car Floor"},
    {eLineTypes::eBackgroundMineCarWall_15, "Background Mine Car Wall"},
    {eLineTypes::eBackgroundMineCarCeiling_16, "Background Mine Car Ceiling"},
    {eLineTypes::eFlyingObjectWall_17, "Flying Slig Ceiling"},
    {eLineTypes::eBackgroundFlyingObjectWall_18, "Background Flying Slig Ceiling"},
    {eLineTypes::eDynamicCollision_32, "Dynamic Collision"},
    {eLineTypes::eBackgroundDynamicCollision_36, "Background Dynamic Collision"},
});


REFLECT_ENUM(EReliveLevelIds,
{
    {EReliveLevelIds::eNone, "None"},
    {EReliveLevelIds::eMenu, "Menu"},
    {EReliveLevelIds::eMines, "Mines"},
    {EReliveLevelIds::eNecrum, "Necrum"},
    {EReliveLevelIds::eMudomoVault, "Mudomo Vault"},
    {EReliveLevelIds::eMudancheeVault, "Mudanchee Vault"},
    {EReliveLevelIds::eFeeCoDepot, "FeeCo Depot"},
    {EReliveLevelIds::eBarracks, "Barracks"},
    {EReliveLevelIds::eMudancheeVault_Ender, "Mudanchee Vault Ender"},
    {EReliveLevelIds::eBonewerkz, "Bonewerkz"},
    {EReliveLevelIds::eBrewery, "Brewery"},
    {EReliveLevelIds::eBrewery_Ender, "Brewery Ender"},
    {EReliveLevelIds::eMudomoVault_Ender, "Mudomo Vault Ender"},
    {EReliveLevelIds::eFeeCoDepot_Ender, "FeeCo Depot Ender"},
    {EReliveLevelIds::eBarracks_Ender, "Barracks Ender"},
    {EReliveLevelIds::eBonewerkz_Ender, "Bonewerkz Ender"},
    {EReliveLevelIds::eCredits, "Credits"},
    {EReliveLevelIds::eRuptureFarms, "Rupture Farms"},
    {EReliveLevelIds::eLines, "Lines"},
    {EReliveLevelIds::eForest, "Forest"},
    {EReliveLevelIds::eForestTemple, "Forest Temple"},
    {EReliveLevelIds::eStockYards, "Stock Yards"},
    {EReliveLevelIds::eStockYardsReturn, "Stock Yards Return"},
    {EReliveLevelIds::eDesert, "Desert"},
    {EReliveLevelIds::eDesertTemple, "Desert Temple"},
    {EReliveLevelIds::eBoardRoom, "Board Room"},
    {EReliveLevelIds::eRuptureFarmsReturn, "Rupture Farms Return"},
    {EReliveLevelIds::eForestChase, "Forest Chase"},
    {EReliveLevelIds::eDesertEscape, "Desert Escape"},
});

REFLECT_ENUM(relive::reliveScreenChangeEffects,
{
    {relive::reliveScreenChangeEffects::ePlay1FMV, "Play 1 FMV"},
    {relive::reliveScreenChangeEffects::eRightToLeft, "Right To Left"},
    {relive::reliveScreenChangeEffects::eLeftToRight, "Left To Right"},
    {relive::reliveScreenChangeEffects::eBottomToTop, "Bottom To Top"},
    {relive::reliveScreenChangeEffects::eTopToBottom, "Top To Bottom"},
    {relive::reliveScreenChangeEffects::eBoxOut, "Box Out"},
    {relive::reliveScreenChangeEffects::eVerticalSplit, "Vertical Split"},
    {relive::reliveScreenChangeEffects::eHorizontalSplit, "Horizontal Split"},
    {relive::reliveScreenChangeEffects::eUnknown_8, "Unknown8"},
    {relive::reliveScreenChangeEffects::eInstantChange, "Instant Change"}
});

REFLECT_ENUM(relive::Path_ElectricWall::ElectricWallStartState,
{
    {relive::Path_ElectricWall::ElectricWallStartState::eOff, "Off"},
    {relive::Path_ElectricWall::ElectricWallStartState::eOn, "On"},
});

REFLECT_ENUM(relive::Path_Mudokon::MudJobs,
{
    {relive::Path_Mudokon::MudJobs::eChisle, "Chisle"},
    {relive::Path_Mudokon::MudJobs::eSitScrub, "Scrub"},
    {relive::Path_Mudokon::MudJobs::eAngryWorker, "Angry Worker"},
    {relive::Path_Mudokon::MudJobs::eDamageRingGiver, "Damage Ring Giver"},
    {relive::Path_Mudokon::MudJobs::eHealthRingGiver, "Health Ring Giver"},
});

REFLECT_ENUM(relive::Path_BirdPortal::PortalType,
{
    {relive::Path_BirdPortal::PortalType::eAbe, "Abe"},
    {relive::Path_BirdPortal::PortalType::eWorker, "Worker"},
    {relive::Path_BirdPortal::PortalType::eShrykull, "Shrykull"},
    {relive::Path_BirdPortal::PortalType::eMudTeleport, "Mud Teleport"},
});

REFLECT_ENUM(relive::Path_Door::DoorStates,
{
    {relive::Path_Door::DoorStates::eOpen, "Open"},
    {relive::Path_Door::DoorStates::eClosed, "Closed"},
    {relive::Path_Door::DoorStates::eOpening, "Opening"},
    {relive::Path_Door::DoorStates::eClosing, "Closing"}
});

REFLECT_ENUM(relive::Path_Door::DoorTypes,
{
    {relive::Path_Door::DoorTypes::eBasicDoor, "Basic Door"},
    {relive::Path_Door::DoorTypes::eTasksDoorWithSecretMusic, "Tasks Door With Secret Music"},
    {relive::Path_Door::DoorTypes::eTasksDoor, "Tasks Door"},
    {relive::Path_Door::DoorTypes::eTrialDoor, "(AO) Trial Door"},
    {relive::Path_Door::DoorTypes::eHubDoor, "(AO) Hub Door"}
});

REFLECT_ENUM(relive::Path_Lever::LeverSoundType,
{
    {relive::Path_Lever::LeverSoundType::eNone, "None"},
    {relive::Path_Lever::LeverSoundType::eWell, "Well"},
    {relive::Path_Lever::LeverSoundType::eSwitchBellHammer, "Switch Bell Hammer"},
    {relive::Path_Lever::LeverSoundType::eDoor, "Door"},
    {relive::Path_Lever::LeverSoundType::eElectricWall, "Electric Wall"},
    {relive::Path_Lever::LeverSoundType::eSecurityOrb, "Security Orb"},
    {relive::Path_Lever::LeverSoundType::eLift, "(AE) Lift"}
});

REFLECT_ENUM(relive::Path_Lever::LeverSoundDirection,
{
    {relive::Path_Lever::LeverSoundDirection::eLeftAndRight, "Left And Right"},
    {relive::Path_Lever::LeverSoundDirection::eLeft, "Left"},
    {relive::Path_Lever::LeverSoundDirection::eRight, "Right"},
});

REFLECT_ENUM(relive::Path_Hoist::Type,
{
    {relive::Path_Hoist::Type::eNextFloor, "Next Floor"},
    {relive::Path_Hoist::Type::eOffScreen, "Off Screen"},
});

REFLECT_ENUM(relive::Path_Hoist::GrabDirection,
{
    {relive::Path_Hoist::GrabDirection::eFacingLeft, "Facing Left"},
    {relive::Path_Hoist::GrabDirection::eFacingRight, "Facing Right"},
    {relive::Path_Hoist::GrabDirection::eFacingAnyDirection, "Facing Any Direction"},
});

REFLECT_ENUM(relive::Path_BoomMachine::NozzleSide,
{
    {relive::Path_BoomMachine::NozzleSide::eRight, "Right"},
    {relive::Path_BoomMachine::NozzleSide::eLeft, "Left"},
});

REFLECT_ENUM(relive::Path_EnemyStopper::StopDirection,
{
    {relive::Path_EnemyStopper::StopDirection::Left, "Left"},
    {relive::Path_EnemyStopper::StopDirection::Right, "Right"},
    {relive::Path_EnemyStopper::StopDirection::Both, "Both"},
});

REFLECT_ENUM(relive::Path_UXB::StartState,
{
    {relive::Path_UXB::StartState::eOn, "On"},
    {relive::Path_UXB::StartState::eOff, "Off"},
});

REFLECT_ENUM(relive::Path_Edge::GrabDirection,
{
    {relive::Path_Edge::GrabDirection::eFacingLeft, "Facing Left"},
    {relive::Path_Edge::GrabDirection::eFacingRight, "Facing Right"},
    {relive::Path_Edge::GrabDirection::eFacingAnyDirection, "Facing Any Direction"},
});

REFLECT_ENUM(relive::Path_ShadowZone::Scale,
{
    {relive::Path_ShadowZone::Scale::eBoth, "Both"},
    {relive::Path_ShadowZone::Scale::eHalf, "Half"},
    {relive::Path_ShadowZone::Scale::eFull, "Full"},
});

REFLECT_ENUM(relive::Path_MusicTrigger::MusicTriggerMusicType,
{
    {relive::Path_MusicTrigger::MusicTriggerMusicType::eDrumAmbience, "Drum Ambience"},
    {relive::Path_MusicTrigger::MusicTriggerMusicType::eDeathDrumShort, "Death Drum Short"},
    {relive::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaLong, "Secret Area Long"},
    {relive::Path_MusicTrigger::MusicTriggerMusicType::eSoftChase, "Soft Chase"},
    {relive::Path_MusicTrigger::MusicTriggerMusicType::eIntenseChase, "Intense Chase"},
    {relive::Path_MusicTrigger::MusicTriggerMusicType::eChime, "Chime"},
    {relive::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaShort, "Secret Area Short"},
});

REFLECT_ENUM(relive::Path_MusicTrigger::TriggeredBy,
{
    {relive::Path_MusicTrigger::TriggeredBy::eTimer, "Timer"},
    {relive::Path_MusicTrigger::TriggeredBy::eTouching, "Touching"},
    {relive::Path_MusicTrigger::TriggeredBy::eSwitchId, "(AO) Switch ID"},
    {relive::Path_MusicTrigger::TriggeredBy::eUnknown, "(AO) Unknown"},
});

REFLECT_ENUM(relive::Path_InvisibleSwitch::InvisibleSwitchScale,
{
    {relive::Path_InvisibleSwitch::InvisibleSwitchScale::eHalf, "Half"},
    {relive::Path_InvisibleSwitch::InvisibleSwitchScale::eFull, "Full"},
    {relive::Path_InvisibleSwitch::InvisibleSwitchScale::eAny, "Any"},
});

REFLECT_ENUM(relive::Path_GasEmitter::GasColour,
{
    {relive::Path_GasEmitter::GasColour::eYellow, "Yellow"},
    {relive::Path_GasEmitter::GasColour::eRed, "Red"},
    {relive::Path_GasEmitter::GasColour::eGreen, "Green"},
    {relive::Path_GasEmitter::GasColour::eBlue, "Blue"},
    {relive::Path_GasEmitter::GasColour::eWhite, "White"},
});

REFLECT_ENUM(::relive::Path_BackgroundAnimation::BgAnimSounds,
{
    {relive::Path_BackgroundAnimation::BgAnimSounds::eNone, "None"},
    {relive::Path_BackgroundAnimation::BgAnimSounds::eFire, "Fire"},
});

REFLECT_ENUM(relive::Path_BackgroundAnimation::Layer,
{
    {relive::Path_BackgroundAnimation::Layer::eLayer0, "Layer 0"},
    {relive::Path_BackgroundAnimation::Layer::eLayer2, "Layer 2"},
});

REFLECT_ENUM(relive::TBlendModes,
{
    {relive::TBlendModes::eBlend_0, "Blend 0"},
    {relive::TBlendModes::eBlend_1, "Blend 1"},
    {relive::TBlendModes::eBlend_2, "Blend 2"},
    {relive::TBlendModes::eBlend_3, "Blend 3"},
});

REFLECT_ENUM(relive::Path_LiftPoint::LiftPointStopType,
{
    {relive::Path_LiftPoint::LiftPointStopType::eTopFloor, "Top Floor"},
    {relive::Path_LiftPoint::LiftPointStopType::eBottomFloor, "Bottom Floor"},
    {relive::Path_LiftPoint::LiftPointStopType::eMiddleFloor, "Middle Floor"},
    {relive::Path_LiftPoint::LiftPointStopType::eMiddleLockFloor, "Middle Lock Floor"},
    {relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly, "Start Point Only"},
});

REFLECT_ENUM(relive::Path_PullRingRope::PullRingSwitchSound,
{
    {relive::Path_PullRingRope::PullRingSwitchSound::eNone, "None"},
    {relive::Path_PullRingRope::PullRingSwitchSound::eWellExit, "Well Exit"},
    {relive::Path_PullRingRope::PullRingSwitchSound::eRingBellHammer, "Ring Unknown Trigger"},
    {relive::Path_PullRingRope::PullRingSwitchSound::eDoorEffect, "Door Effect"},
});

REFLECT_ENUM(relive::Path_PullRingRope::PullRingSoundDirection,
{
    {relive::Path_PullRingRope::PullRingSoundDirection::eLeftAndRight, "Left And Right"},
    {relive::Path_PullRingRope::PullRingSoundDirection::eLeft, "Left"},
    {relive::Path_PullRingRope::PullRingSoundDirection::eRight, "Right"},
});

REFLECT_ENUM(relive::Path_ContinuePoint::Scale,
{
    {relive::Path_ContinuePoint::Scale::eNone, "None"},
    {relive::Path_ContinuePoint::Scale::eHalf, "Half"},
    {relive::Path_ContinuePoint::Scale::eFull, "Full"},
});

REFLECT_ENUM(relive::Path_ContinuePoint::spawnDirection,
{
    {relive::Path_ContinuePoint::spawnDirection::eRight, "Right"},
    {relive::Path_ContinuePoint::spawnDirection::eLeft, "Left"}
});

REFLECT_ENUM(relive::Path_WheelSyncer::OutputRequirement,
{
    {relive::Path_WheelSyncer::OutputRequirement::eAllOn, "All On"},
    {relive::Path_WheelSyncer::OutputRequirement::e1OnAnd2Off, "1 On And 2 Off"},
    {relive::Path_WheelSyncer::OutputRequirement::e1Or2On, "1 Or 2 On"},
    {relive::Path_WheelSyncer::OutputRequirement::e1OnOr2Off, "1 On Or 2 Off"},
});

REFLECT_ENUM(relive::Path_Drill::DrillBehavior,
{
    {relive::Path_Drill::DrillBehavior::eNotInteractable, "Not Interactable"},
    {relive::Path_Drill::DrillBehavior::eToggle, "Toggle"},
    {relive::Path_Drill::DrillBehavior::eUse, "Use"},
});

REFLECT_ENUM(relive::Path_Drill::DrillDirection,
{
    {relive::Path_Drill::DrillDirection::eDown, "Down"},
    {relive::Path_Drill::DrillDirection::eRight, "Right"},
    {relive::Path_Drill::DrillDirection::eLeft, "Left"},
});

REFLECT_ENUM(relive::Path_TrapDoor::StartState,
{
    {relive::Path_TrapDoor::StartState::eOpen, "Open"},
    {relive::Path_TrapDoor::StartState::eClosed, "Closed"}
});

REFLECT_ENUM(relive::Path_LiftMover::YDirection,
{
    {relive::Path_LiftMover::YDirection::eDown, "Down"},
    {relive::Path_LiftMover::YDirection::eUp, "Up"},
});

REFLECT_ENUM(relive::Path_MotionDetector::InitialMoveDirection,
{
    {relive::Path_MotionDetector::InitialMoveDirection::eRight, "Right"},
    {relive::Path_MotionDetector::InitialMoveDirection::eLeft, "Left"},
});

REFLECT_ENUM(relive::Path_DoorFlame::Colour,
{
    {relive::Path_DoorFlame::Colour::eDefault, "Default"},
    {relive::Path_DoorFlame::Colour::eRed, "Red"},
    {relive::Path_DoorFlame::Colour::eGreen, "Green"},
    {relive::Path_DoorFlame::Colour::eBlue, "Blue"},
});

REFLECT_ENUM(relive::Path_CrawlingSlig::StartState,
{
    {relive::Path_CrawlingSlig::StartState::eSleeping, "Sleeping"},
    {relive::Path_CrawlingSlig::StartState::eAwake, "Awake"},
});

REFLECT_ENUM(relive::Path_CrawlingSlig::CrawlDirection,
{
    {relive::Path_CrawlingSlig::CrawlDirection::eLeft, "Left"},
    {relive::Path_CrawlingSlig::CrawlDirection::eRight, "Right"},
    {relive::Path_CrawlingSlig::CrawlDirection::eRandom, "Random"},
});

// this probably shouldn't be here since the sound/sound direction implementation is extremely scuffed
// but maybe someone wants to use it anyway
REFLECT_ENUM(relive::Path_CrawlingSligButton::ButtonSounds,
{
    {relive::Path_CrawlingSligButton::ButtonSounds::None, "None"},
    {relive::Path_CrawlingSligButton::ButtonSounds::SackHit1, "Sack Hit 1"},
    {relive::Path_CrawlingSligButton::ButtonSounds::SackHit2, "Sack Hit 2"},
    {relive::Path_CrawlingSligButton::ButtonSounds::FallingItemPresence2, "Falling Item Presence 2"},
    {relive::Path_CrawlingSligButton::ButtonSounds::SecurityOrb, "Security Orb"},
    {relive::Path_CrawlingSligButton::ButtonSounds::Bullet1, "Bullet 1"},
    {relive::Path_CrawlingSligButton::ButtonSounds::AbeGenericMovement, "Abe Generic Movement"},
});

REFLECT_ENUM(relive::Path_Glukkon::GlukkonTypes,
{
    {relive::Path_Glukkon::GlukkonTypes::eNormal, "Normal"},
    {relive::Path_Glukkon::GlukkonTypes::eStoryAslik, "Story Aslik"},
    {relive::Path_Glukkon::GlukkonTypes::eStoryDripik, "Story Dripik"},
    {relive::Path_Glukkon::GlukkonTypes::eStoryPhleg, "Story Phleg"},
});

REFLECT_ENUM(relive::Path_Glukkon::Facing,
{
    {relive::Path_Glukkon::Facing::eRight, "Right"},
    {relive::Path_Glukkon::Facing::eLeft, "Left"},
});

REFLECT_ENUM(relive::Path_Glukkon::SpawnType,
{
    {relive::Path_Glukkon::SpawnType::eRegularSpawn, "Regular Spawn"},
    {relive::Path_Glukkon::SpawnType::eFacingLeft, "Facing Left"},
    {relive::Path_Glukkon::SpawnType::eFacingRight, "Facing Right"},
    {relive::Path_Glukkon::SpawnType::eFullSpawnEffects, "Full Spawn Effects"},
});

REFLECT_ENUM(relive::Path_Glukkon::Behavior,
{
    {relive::Path_Glukkon::Behavior::eIgnoreWalls, "Ignore Walls"},
    {relive::Path_Glukkon::Behavior::eCheckForWalls, "Check For Walls"},
});

REFLECT_ENUM(relive::Path_FootSwitch::FootSwitchTriggerBy,
{
    {relive::Path_FootSwitch::FootSwitchTriggerBy::eAbe, "Abe"},
    {relive::Path_FootSwitch::FootSwitchTriggerBy::eAnyone, "Anyone"},
});

REFLECT_ENUM(relive::Path_SlogSpawner::StartDirection,
{
    {relive::Path_SlogSpawner::StartDirection::eRight, "Right"},
    {relive::Path_SlogSpawner::StartDirection::eLeft, "Left"},
});

REFLECT_ENUM(relive::Path_Scrab::ScrabPatrolType,
{
    {relive::Path_Scrab::ScrabPatrolType::eWalk, "Walk"},
    {relive::Path_Scrab::ScrabPatrolType::eRunOrWalk192, "Run Or Walk 192"},
    {relive::Path_Scrab::ScrabPatrolType::eRunOrWalk128, "Run Or Walk 128"},
    {relive::Path_Scrab::ScrabPatrolType::eRunOrWalk64, "Run Or Walk 64"},
    {relive::Path_Scrab::ScrabPatrolType::eRun, "Run"}
});

REFLECT_ENUM(relive::Path_ScrabSpawner::SpawnDirection,
{
    {relive::Path_ScrabSpawner::SpawnDirection::eNone, "None"},
    {relive::Path_ScrabSpawner::SpawnDirection::eLeft, "Left"},
    {relive::Path_ScrabSpawner::SpawnDirection::eRight, "Right"},
});

REFLECT_ENUM(relive::Path_Paramite::EntranceType,
{
    {relive::Path_Paramite::EntranceType::ePatrol, "Patrol"},
    {relive::Path_Paramite::EntranceType::eSurpriseWeb, "Surprise Web"},
    {relive::Path_Paramite::EntranceType::eSlightlyHigherSpawnSurpriseWeb, "Slightly Higher Spawn Surprise Web"},
});

REFLECT_ENUM(relive::Path_BellsongStone::BellsongTypes,
{
    {relive::Path_BellsongStone::BellsongTypes::eWhistle, "Whistle"},
    {relive::Path_BellsongStone::BellsongTypes::eChimes, "Chimes"},
});

REFLECT_ENUM(relive::Path_LightEffect::Type,
{
    {relive::Path_LightEffect::Type::Star, "Star"},
    {relive::Path_LightEffect::Type::GoldGlow, "Gold Glow"},
    {relive::Path_LightEffect::Type::GreenGlow, "Green Glow"},
    {relive::Path_LightEffect::Type::FlintGlow, "Flint Glow"},
    {relive::Path_LightEffect::Type::Switchable_RedGreenDoorLights, "RedGreen Door Light"},
    {relive::Path_LightEffect::Type::Switchable_RedGreenHubLight, "RedGreen Hub Light"},
});

REFLECT_ENUM(relive::Path_RingMudokon::MustFaceMud,
{
    {relive::Path_RingMudokon::MustFaceMud::eYes, "Yes"},
    {relive::Path_RingMudokon::MustFaceMud::eNo, "No"},
});

REFLECT_ENUM(relive::Path_MeatSaw::Type,
{
    {relive::Path_MeatSaw::Type::eAutomaticPersistOffscreen, "Automatic Persist Offscreen"},
    {relive::Path_MeatSaw::Type::eAutomatic, "Automatic"},
    {relive::Path_MeatSaw::Type::eSwitchId, "Switch ID"},
});

REFLECT_ENUM(relive::Path_MeatSaw::StartState,
{
    {relive::Path_MeatSaw::StartState::eOff, "Off"},
    {relive::Path_MeatSaw::StartState::eOn, "On"},
});

REFLECT_ENUM(relive::Path_LiftMudokon::Direction,
{
    {relive::Path_LiftMudokon::Direction::eRight, "Right"},
    {relive::Path_LiftMudokon::Direction::eLeft, "Left"},
});

REFLECT_ENUM(relive::Path_BeeSwarmHole::MovementType,
{
    {relive::Path_BeeSwarmHole::MovementType::eHover, "Hover"},
    {relive::Path_BeeSwarmHole::MovementType::eAttack, "Attack"},
    {relive::Path_BeeSwarmHole::MovementType::eFollowPath, "Follow Path"},
});

REFLECT_ENUM(relive::Path_ZBall::StartPos,
{
    {relive::Path_ZBall::StartPos::eCenter, "Center"},
    {relive::Path_ZBall::StartPos::eOut, "Out"},
    {relive::Path_ZBall::StartPos::eIn, "In"},
});

REFLECT_ENUM(relive::Path_ZBall::Speed,
{
    {relive::Path_ZBall::Speed::eNormal, "Normal"},
    {relive::Path_ZBall::Speed::eFast, "Fast"},
    {relive::Path_ZBall::Speed::eSlow, "Slow"},
});

