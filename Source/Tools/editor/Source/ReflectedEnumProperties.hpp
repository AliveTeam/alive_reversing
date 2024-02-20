#pragma once

#include <QString>
#include <array>
#include <tuple>
#include "../../../relive_lib/data_conversion/relive_tlvs.hpp"
#include <QCoreApplication>

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
        static const inline EnumPair mArray[] = __VA_ARGS__; \
        static const inline u32 mArraySize = ALIVE_COUNTOF(mArray);\
    };

REFLECT_ENUM(ReliveTypes, {
    {ReliveTypes::eNone, QCoreApplication::translate("ReflectedEnumValues","None")},
    {ReliveTypes::eCrawlingSligButton, QCoreApplication::translate("ReflectedEnumValues", "Crawling Slig Button")},
    {ReliveTypes::eWheelSyncer, QCoreApplication::translate("ReflectedEnumValues", "Wheel Syncer")},
    {ReliveTypes::eDemoSpawnPoint, QCoreApplication::translate("ReflectedEnumValues", "Demo Spawn Point")},
    {ReliveTypes::eMultiSwitchController, QCoreApplication::translate("ReflectedEnumValues", "Multi Switch Controller")},
    {ReliveTypes::eStatusLight, QCoreApplication::translate("ReflectedEnumValues", "Status Light")},
    {ReliveTypes::eSlapLock, QCoreApplication::translate("ReflectedEnumValues", "Slap Lock")},
    {ReliveTypes::eParamiteWebLine, QCoreApplication::translate("ReflectedEnumValues", "Paramite Web Line")},
    {ReliveTypes::eGlukkonSwitch, QCoreApplication::translate("ReflectedEnumValues", "Glukkon Switch")},
    {ReliveTypes::eDoorBlocker, QCoreApplication::translate("ReflectedEnumValues", "Door Blocker")},
    {ReliveTypes::eTrainDoor, QCoreApplication::translate("ReflectedEnumValues", "Train Door")},
    {ReliveTypes::eLevelLoader, QCoreApplication::translate("ReflectedEnumValues", "Level Loader")},
    {ReliveTypes::eSligGetWings, QCoreApplication::translate("ReflectedEnumValues", "Slig Get Wings")},
    {ReliveTypes::eSligGetPants, QCoreApplication::translate("ReflectedEnumValues", "Slig Get Pants")},
    {ReliveTypes::eTeleporter, QCoreApplication::translate("ReflectedEnumValues", "Teleporter")},
    {ReliveTypes::eWater, QCoreApplication::translate("ReflectedEnumValues", "Water")},
    {ReliveTypes::eWorkWheel, QCoreApplication::translate("ReflectedEnumValues", "Work Wheel")},
    {ReliveTypes::eLCDScreen, QCoreApplication::translate("ReflectedEnumValues", "LCD Screen")},
    {ReliveTypes::eInvisibleSwitch, QCoreApplication::translate("ReflectedEnumValues", "Invisible Switch")},
    {ReliveTypes::eDoorFlame, QCoreApplication::translate("ReflectedEnumValues", "Door Flame")},
    {ReliveTypes::eMovingBomb, QCoreApplication::translate("ReflectedEnumValues", "Moving Bomb")},
    {ReliveTypes::eMainMenuController, QCoreApplication::translate("ReflectedEnumValues", "Main Menu Controller")},
    {ReliveTypes::eHintFly, QCoreApplication::translate("ReflectedEnumValues", "Hint Fly")},
    {ReliveTypes::eSecurityDoor, QCoreApplication::translate("ReflectedEnumValues", "Security Door")},
    {ReliveTypes::eCreditsController, QCoreApplication::translate("ReflectedEnumValues", "Credits Controller")},
    {ReliveTypes::eLCDStatusBoard, QCoreApplication::translate("ReflectedEnumValues", "LCD Status Board")},
    {ReliveTypes::eSwitchStateBooleanLogic, QCoreApplication::translate("ReflectedEnumValues", "Switch State Boolean Logic")},
    {ReliveTypes::eLightEffect, QCoreApplication::translate("ReflectedEnumValues", "Light Effect")},
    {ReliveTypes::eSlogSpawner, QCoreApplication::translate("ReflectedEnumValues", "Slog Spawner")},
    {ReliveTypes::eGasEmitter, QCoreApplication::translate("ReflectedEnumValues", "Gas Emitter")},
    {ReliveTypes::eRingCancel, QCoreApplication::translate("ReflectedEnumValues", "Ring Cancel")},
    {ReliveTypes::eElumWall, QCoreApplication::translate("ReflectedEnumValues", "Elum Wall")},
    {ReliveTypes::eAbeStart, QCoreApplication::translate("ReflectedEnumValues", "Abe Start")},
    {ReliveTypes::eBeeSwarmHole, QCoreApplication::translate("ReflectedEnumValues", "Bee Swarm Hole")},
    {ReliveTypes::eFallingItem, QCoreApplication::translate("ReflectedEnumValues", "Falling Item")},
    {ReliveTypes::eShadowZone, QCoreApplication::translate("ReflectedEnumValues", "Shadow Zone")},
    {ReliveTypes::eStartController, QCoreApplication::translate("ReflectedEnumValues", "Start Controller")},
    {ReliveTypes::eBirdPortalExit, QCoreApplication::translate("ReflectedEnumValues", "Bird Portal Exit")},
    {ReliveTypes::eHoneyDripTarget, QCoreApplication::translate("ReflectedEnumValues", "Honey Drip Target")},
    {ReliveTypes::ePathTransition, QCoreApplication::translate("ReflectedEnumValues", "Path Transition")},
    {ReliveTypes::eZSligCover, QCoreApplication::translate("ReflectedEnumValues", "Z Slig Cover")},
    {ReliveTypes::eResetPath, QCoreApplication::translate("ReflectedEnumValues", "Reset Path")},
    {ReliveTypes::eElumPathTrans, QCoreApplication::translate("ReflectedEnumValues", "Elum Path Trans")},
    {ReliveTypes::eScrabLeftBound, QCoreApplication::translate("ReflectedEnumValues", "Scrab Left Bound")},
    {ReliveTypes::eScrabRightBound, QCoreApplication::translate("ReflectedEnumValues", "Scrab Right Bound")},
    {ReliveTypes::eScrabNoFall, QCoreApplication::translate("ReflectedEnumValues", "Scrab No Fall")},
    {ReliveTypes::eMovingBombStopper, QCoreApplication::translate("ReflectedEnumValues", "Moving Bomb Stopper")},
    {ReliveTypes::eElumStart, QCoreApplication::translate("ReflectedEnumValues", "Elum Start")},
    {ReliveTypes::eEdge, QCoreApplication::translate("ReflectedEnumValues", "Edge")},
    {ReliveTypes::eSoftLanding, QCoreApplication::translate("ReflectedEnumValues", "Soft Landing")},
    {ReliveTypes::eMovieHandStone, QCoreApplication::translate("ReflectedEnumValues", "Movie Hand Stone")},
    {ReliveTypes::eBellSongStone, QCoreApplication::translate("ReflectedEnumValues", "Bell Song Stone")},
    {ReliveTypes::eDemoPlaybackStone, QCoreApplication::translate("ReflectedEnumValues", "Demo Playback Stone")},
    {ReliveTypes::eHandStone, QCoreApplication::translate("ReflectedEnumValues", "Hand Stone")},
    {ReliveTypes::eHoist, QCoreApplication::translate("ReflectedEnumValues", "Hoist")},
    {ReliveTypes::eContinuePoint, QCoreApplication::translate("ReflectedEnumValues", "Continue Point")},
    {ReliveTypes::eWellLocal, QCoreApplication::translate("ReflectedEnumValues", "Well Local")},
    {ReliveTypes::eWellExpress, QCoreApplication::translate("ReflectedEnumValues", "Well Express")},
    {ReliveTypes::eMudokonPathTrans, QCoreApplication::translate("ReflectedEnumValues", "Mudokon Path Trans")},
    {ReliveTypes::eRingMudokon, QCoreApplication::translate("ReflectedEnumValues", "Ring Mudokon")},
    {ReliveTypes::eLiftMudokon, QCoreApplication::translate("ReflectedEnumValues", "Lift Mudokon")},
    {ReliveTypes::eInvisibleZone, QCoreApplication::translate("ReflectedEnumValues", "Invisible Zone")},
    {ReliveTypes::eEnemyStopper, QCoreApplication::translate("ReflectedEnumValues", "Enemy Stopper")},
    {ReliveTypes::eSligBoundLeft, QCoreApplication::translate("ReflectedEnumValues", "Slig Bound Left")},
    {ReliveTypes::eSligBoundRight, QCoreApplication::translate("ReflectedEnumValues", "Slig Boung Right")},
    {ReliveTypes::eSligPersist, QCoreApplication::translate("ReflectedEnumValues", "Slig Persist")},
    {ReliveTypes::eZzzSpawner, QCoreApplication::translate("ReflectedEnumValues", "Zzz Spawner")},
    {ReliveTypes::eKillUnsavedMuds, QCoreApplication::translate("ReflectedEnumValues", "Kill Unsaved Muds")},
    {ReliveTypes::eDeathDrop, QCoreApplication::translate("ReflectedEnumValues", "Death Drop")},
    {ReliveTypes::eAlarm, QCoreApplication::translate("ReflectedEnumValues", "Alarm")},
    {ReliveTypes::eScreenManager, QCoreApplication::translate("ReflectedEnumValues", "Screen Manager")},
    {ReliveTypes::eBackgroundAnimation, QCoreApplication::translate("ReflectedEnumValues", "Background Animation")},
    {ReliveTypes::eBat, QCoreApplication::translate("ReflectedEnumValues", "Bat")},
    {ReliveTypes::eLiftMover, QCoreApplication::translate("ReflectedEnumValues", "Lift Mover")},
    {ReliveTypes::eTimedMine, QCoreApplication::translate("ReflectedEnumValues", "Timed Mine")},
    {ReliveTypes::eBullet, QCoreApplication::translate("ReflectedEnumValues", "Bullet")},
    {ReliveTypes::eDDCheat, QCoreApplication::translate("ReflectedEnumValues", "DDCheat")},
    {ReliveTypes::eBells, QCoreApplication::translate("ReflectedEnumValues", "Bells")},
    {ReliveTypes::eChimeLock, QCoreApplication::translate("ReflectedEnumValues", "Chime Lock")},
    {ReliveTypes::eGasCountDown, QCoreApplication::translate("ReflectedEnumValues", "Gas Count Down")},
    {ReliveTypes::eParticleBurst, QCoreApplication::translate("ReflectedEnumValues", "Particle Burst")},
    {ReliveTypes::eDoor, QCoreApplication::translate("ReflectedEnumValues", "Door")},
    {ReliveTypes::eGameSpeak, QCoreApplication::translate("ReflectedEnumValues", "Game Speak")},
    {ReliveTypes::eElectricWall, QCoreApplication::translate("ReflectedEnumValues", "Electric Wall")},
    {ReliveTypes::eElum, QCoreApplication::translate("ReflectedEnumValues", "Elum")},
    {ReliveTypes::eBellHammer, QCoreApplication::translate("ReflectedEnumValues", "Bell Hammer")},
    {ReliveTypes::ePalOverwriter, QCoreApplication::translate("ReflectedEnumValues", "Pal Overwritter")},
    {ReliveTypes::eGroundExplosion, QCoreApplication::translate("ReflectedEnumValues", "Ground Explosion")},
    {ReliveTypes::eSecurityClaw, QCoreApplication::translate("ReflectedEnumValues", "Security Claw")},
    {ReliveTypes::eRockSpawner, QCoreApplication::translate("ReflectedEnumValues", "Rock Spawner")},
    {ReliveTypes::eFlintLockFire, QCoreApplication::translate("ReflectedEnumValues", "Flint Lock Fire")},
    {ReliveTypes::eThrowableTotalIndicator, QCoreApplication::translate("ReflectedEnumValues", "Throwable Total Indicator")},
    {ReliveTypes::eFootSwitch, QCoreApplication::translate("ReflectedEnumValues", "Foot Switch")},
    {ReliveTypes::eGameEnderController, QCoreApplication::translate("ReflectedEnumValues", "Game Ender Controller")},
    {ReliveTypes::eDeathBird, QCoreApplication::translate("ReflectedEnumValues", "Death Bird")},
    {ReliveTypes::eLoadingFile, QCoreApplication::translate("ReflectedEnumValues", "Loading File")},
    {ReliveTypes::eGrenade, QCoreApplication::translate("ReflectedEnumValues", "Grenade")},
    {ReliveTypes::eBoomMachine, QCoreApplication::translate("ReflectedEnumValues", "Boom Machine")},
    {ReliveTypes::eBackgroundGlukkon, QCoreApplication::translate("ReflectedEnumValues", "Background Glukkon")},
    {ReliveTypes::eAbe, QCoreApplication::translate("ReflectedEnumValues", "Abe")},
    {ReliveTypes::MainMenuFade, QCoreApplication::translate("ReflectedEnumValues", "Main Menu Fade")},
    {ReliveTypes::eHoneySack, QCoreApplication::translate("ReflectedEnumValues", "Honey Sack")},
    {ReliveTypes::eHoney, QCoreApplication::translate("ReflectedEnumValues", "Honey")},
    {ReliveTypes::eClawOrBirdPortalTerminator, QCoreApplication::translate("ReflectedEnumValues", "Claw OR Bird Portal Terminator")},
    {ReliveTypes::eMudokon, QCoreApplication::translate("ReflectedEnumValues", "Mudokon")},
    {ReliveTypes::eLiftPoint, QCoreApplication::translate("ReflectedEnumValues", "Lift Point")},
    {ReliveTypes::eMeat, QCoreApplication::translate("ReflectedEnumValues", "Meat")},
    {ReliveTypes::eMeatSack, QCoreApplication::translate("ReflectedEnumValues", "Meat Sack")},
    {ReliveTypes::eMeatSaw, QCoreApplication::translate("ReflectedEnumValues", "Meat Saw")},
    {ReliveTypes::eMine, QCoreApplication::translate("ReflectedEnumValues", "Mine")},
    {ReliveTypes::eRollingBallStopperShaker, QCoreApplication::translate("ReflectedEnumValues", "Rolling Ball Stopper Shaker")},
    {ReliveTypes::eMotionDetector, QCoreApplication::translate("ReflectedEnumValues", "Motion Detector")},
    {ReliveTypes::eRollingBallStopper, QCoreApplication::translate("ReflectedEnumValues", "Rolling Ball Stopper")},
    {ReliveTypes::ePauseMenu, QCoreApplication::translate("ReflectedEnumValues", "Pause Menu")},
    {ReliveTypes::eParamite, QCoreApplication::translate("ReflectedEnumValues", "Paramite")},
    {ReliveTypes::eDemoPlayback, QCoreApplication::translate("ReflectedEnumValues", "Demo Playback")},
    {ReliveTypes::eBirdPortal, QCoreApplication::translate("ReflectedEnumValues", "Bird Portal")},
    {ReliveTypes::eBirdPortalTerminator, QCoreApplication::translate("ReflectedEnumValues", "Bird Portal Terminator")},
    {ReliveTypes::eFG1, QCoreApplication::translate("ReflectedEnumValues", "FG1")},
    {ReliveTypes::eAbilityRing, QCoreApplication::translate("ReflectedEnumValues", "Ability Ring")},
    {ReliveTypes::eRock, QCoreApplication::translate("ReflectedEnumValues", "Rock")},
    {ReliveTypes::eRockSack, QCoreApplication::translate("ReflectedEnumValues", "Rock Sack")},
    {ReliveTypes::eRollingBall, QCoreApplication::translate("ReflectedEnumValues", "Rolling Ball")},
    {ReliveTypes::eRope, QCoreApplication::translate("ReflectedEnumValues", "Rope")},
    {ReliveTypes::eAirExplosion, QCoreApplication::translate("ReflectedEnumValues", "Air Explosion")},
    {ReliveTypes::eRedLaser, QCoreApplication::translate("ReflectedEnumValues", "Red Laser")},
    {ReliveTypes::eScrab, QCoreApplication::translate("ReflectedEnumValues", "Scrab")},
    {ReliveTypes::eScreenClipper, QCoreApplication::translate("ReflectedEnumValues", "Screen Clipper")},
    {ReliveTypes::eEffectBase, QCoreApplication::translate("ReflectedEnumValues", "Effect Base")},
    {ReliveTypes::eFade, QCoreApplication::translate("ReflectedEnumValues", "Fade")},
    {ReliveTypes::eFlash, QCoreApplication::translate("ReflectedEnumValues", "Flash")},
    {ReliveTypes::eScreenWave, QCoreApplication::translate("ReflectedEnumValues", "Screen Wave")},
    {ReliveTypes::eUnknown, QCoreApplication::translate("ReflectedEnumValues", "Unknown")},
    {ReliveTypes::eShrykull, QCoreApplication::translate("ReflectedEnumValues", "Shrykull")},
    {ReliveTypes::eSlig, QCoreApplication::translate("ReflectedEnumValues", "Slig")},
    {ReliveTypes::eSlog, QCoreApplication::translate("ReflectedEnumValues", "Slog")},
    {ReliveTypes::SlingMud, QCoreApplication::translate("ReflectedEnumValues", "Sling Mudokon")},
    {ReliveTypes::eSligSpawner, QCoreApplication::translate("ReflectedEnumValues", "Slig Spawner")},
    {ReliveTypes::eZBall, QCoreApplication::translate("ReflectedEnumValues", "Z Ball")},
    {ReliveTypes::eParticle, QCoreApplication::translate("ReflectedEnumValues", "Particle")},
    {ReliveTypes::eZapLine, QCoreApplication::translate("ReflectedEnumValues", "Zap Line")},
    {ReliveTypes::eBeeSwarm, QCoreApplication::translate("ReflectedEnumValues", "Bee Swarm")},
    {ReliveTypes::eBeeNest, QCoreApplication::translate("ReflectedEnumValues", "Bee Nest")},
    {ReliveTypes::eLever, QCoreApplication::translate("ReflectedEnumValues", "Lever")},
    {ReliveTypes::eTrapDoor, QCoreApplication::translate("ReflectedEnumValues", "Trap Door")},
    {ReliveTypes::eUXB, QCoreApplication::translate("ReflectedEnumValues", "UXB")},
    {ReliveTypes::eMovie, QCoreApplication::translate("ReflectedEnumValues", "Movie")},
    {ReliveTypes::eCameraSwapper, QCoreApplication::translate("ReflectedEnumValues", "Camera Swapper")},
    {ReliveTypes::eElectrocute, QCoreApplication::translate("ReflectedEnumValues", "Electrocute")},
    {ReliveTypes::eTimedMine_or_MovingBomb, QCoreApplication::translate("ReflectedEnumValues", "Timed Mine Or Moviing Bomb")},
    {ReliveTypes::eBone, QCoreApplication::translate("ReflectedEnumValues", "Bone")},
    {ReliveTypes::eBoneBag, QCoreApplication::translate("ReflectedEnumValues", "Bone Bag")},
    {ReliveTypes::eBrewMachine, QCoreApplication::translate("ReflectedEnumValues", "Brew Machine")},
    {ReliveTypes::eSligButton, QCoreApplication::translate("ReflectedEnumValues", "Slig Button")},
    {ReliveTypes::eExplosionSet, QCoreApplication::translate("ReflectedEnumValues", "Explosion Set")},
    {ReliveTypes::eZapSpark, QCoreApplication::translate("ReflectedEnumValues", "Zap Spark")},
    {ReliveTypes::eMetal, QCoreApplication::translate("ReflectedEnumValues", "Metal")},
    {ReliveTypes::eMinesAlarm, QCoreApplication::translate("ReflectedEnumValues", "Mines Alarm")},
    {ReliveTypes::eCrawlingSlig, QCoreApplication::translate("ReflectedEnumValues", "Crawling Slig")},
    {ReliveTypes::eDrill, QCoreApplication::translate("ReflectedEnumValues", "Drill")},
    {ReliveTypes::eLaughingGas, QCoreApplication::translate("ReflectedEnumValues", "Laughing Gas")},
    {ReliveTypes::eDoorLock, QCoreApplication::translate("ReflectedEnumValues", "Door Lock")},
    {ReliveTypes::eDove, QCoreApplication::translate("ReflectedEnumValues", "Dove")},
    {ReliveTypes::eEvilFart, QCoreApplication::translate("ReflectedEnumValues", "Evil Fart")},
    {ReliveTypes::eFleech, QCoreApplication::translate("ReflectedEnumValues", "Fleech")},
    {ReliveTypes::ePossessionFlicker, QCoreApplication::translate("ReflectedEnumValues", "Possession Flicker")},
    {ReliveTypes::eFlyingSlig, QCoreApplication::translate("ReflectedEnumValues", "Flying Slig")},
    {ReliveTypes::eFlyingSligSpawner, QCoreApplication::translate("ReflectedEnumValues", "Flying Slig Spawner")},
    {ReliveTypes::eColourfulMeter, QCoreApplication::translate("ReflectedEnumValues", "Colourful Meter")},
    {ReliveTypes::eSlapLock_OrbWhirlWind, QCoreApplication::translate("ReflectedEnumValues", "Slap Lock Orb Whirl WInd")},
    {ReliveTypes::eGreeter, QCoreApplication::translate("ReflectedEnumValues", "Greeter")},
    {ReliveTypes::eGlukkon, QCoreApplication::translate("ReflectedEnumValues", "Glukkon")},
    {ReliveTypes::eHelpPhone, QCoreApplication::translate("ReflectedEnumValues", "Help Phone")},
    {ReliveTypes::eEyeOrbPart, QCoreApplication::translate("ReflectedEnumValues", "Eye Orb Part")},
    {ReliveTypes::eInvisibleEffect, QCoreApplication::translate("ReflectedEnumValues", "Invisible Effect")},
    {ReliveTypes::ePulley, QCoreApplication::translate("ReflectedEnumValues", "Pulley")},
    {ReliveTypes::eResourceManager, QCoreApplication::translate("ReflectedEnumValues", "Resource Manager")},
    {ReliveTypes::eSligGetPantsOrWings, QCoreApplication::translate("ReflectedEnumValues", "Slig Get Pants Or Wings")},
    {ReliveTypes::eRingOrLiftMud, QCoreApplication::translate("ReflectedEnumValues", "Ring Or Lift Mud")},
    {ReliveTypes::eSecurityOrb, QCoreApplication::translate("ReflectedEnumValues", "Security Orb")},
    {ReliveTypes::eText, QCoreApplication::translate("ReflectedEnumValues", "Text")},
    {ReliveTypes::eMineCar, QCoreApplication::translate("ReflectedEnumValues", "Mine Car")},
    {ReliveTypes::eGreeterBody, QCoreApplication::translate("ReflectedEnumValues", "Greeter Body")},
    {ReliveTypes::eMusicController, QCoreApplication::translate("ReflectedEnumValues", "Music Controller")},
    {ReliveTypes::eMusicTrigger, QCoreApplication::translate("ReflectedEnumValues", "Music Trigger")},
    {ReliveTypes::ePullRingRope, QCoreApplication::translate("ReflectedEnumValues", "Pull RIng Rope")},
    {ReliveTypes::eScrabSpawner, QCoreApplication::translate("ReflectedEnumValues", "Scrab Spawner")},
    {ReliveTypes::eMainMenuTransistion, QCoreApplication::translate("ReflectedEnumValues", "Main Menu Transition")},
    {ReliveTypes::eScreenShake, QCoreApplication::translate("ReflectedEnumValues", "Screen Shake")},
    {ReliveTypes::eSlamDoor, QCoreApplication::translate("ReflectedEnumValues", "Slam Door")},
    {ReliveTypes::eSnoozeParticle, QCoreApplication::translate("ReflectedEnumValues", "Snooze Particle")},
    {ReliveTypes::eSlurgSpawner, QCoreApplication::translate("ReflectedEnumValues", "Slurg Spawner")},
    {ReliveTypes::eSlurg, QCoreApplication::translate("ReflectedEnumValues", "Slurg")},
    {ReliveTypes::eTimerTrigger, QCoreApplication::translate("ReflectedEnumValues", "Timer Trigger")},
    {ReliveTypes::eTorturedMud, QCoreApplication::translate("ReflectedEnumValues", "Tortured Mud")},
    {ReliveTypes::eWebLine, QCoreApplication::translate("ReflectedEnumValues", "Web Line")},
    {ReliveTypes::eWell, QCoreApplication::translate("ReflectedEnumValues", "Well")},
    {ReliveTypes::eThrowableArray, QCoreApplication::translate("ReflectedEnumValues", "Throwable Array")},
})

REFLECT_ENUM(relive::reliveScale,
{
    {relive::reliveScale::eFull, QCoreApplication::translate("ReflectedEnumValues", "Full")},
    {relive::reliveScale::eHalf, QCoreApplication::translate("ReflectedEnumValues", "Half")}
})

REFLECT_ENUM(relive::Path_Mudokon::Mud_TLV_Emotion,
{
    {relive::Path_Mudokon::Mud_TLV_Emotion::eNormal, QCoreApplication::translate("ReflectedEnumValues", "Normal")},
    {relive::Path_Mudokon::Mud_TLV_Emotion::eAngry, QCoreApplication::translate("ReflectedEnumValues", "Angry")},
    {relive::Path_Mudokon::Mud_TLV_Emotion::eSad, QCoreApplication::translate("ReflectedEnumValues", "Sad")},
    {relive::Path_Mudokon::Mud_TLV_Emotion::eWired, QCoreApplication::translate("ReflectedEnumValues", "Wired")},
    {relive::Path_Mudokon::Mud_TLV_Emotion::eSick, QCoreApplication::translate("ReflectedEnumValues", "Sick")}
})

REFLECT_ENUM(relive::Path_BirdPortal::PortalSide,
{
    {relive::Path_BirdPortal::PortalSide::eRight, QCoreApplication::translate("ReflectedEnumValues", "Right")},
    {relive::Path_BirdPortal::PortalSide::eLeft, QCoreApplication::translate("ReflectedEnumValues", "Left")}
})

REFLECT_ENUM(relive::Path_FlyingSlig::SpawnDelayStates,
{
    {relive::Path_FlyingSlig::SpawnDelayStates::eMoveImmediately, QCoreApplication::translate("ReflectedEnumValues", "Move Immediately")},
    {relive::Path_FlyingSlig::SpawnDelayStates::eUseCustomSpawnMoveDelay, QCoreApplication::translate("ReflectedEnumValues", "Use Custom Spawn Move Delay")}
})

REFLECT_ENUM(relive::Path_Slig_Data::DeathMode,
{
    {relive::Path_Slig_Data::DeathMode::StandIdle, QCoreApplication::translate("ReflectedEnumValues", "Stand Idle")},
    {relive::Path_Slig_Data::DeathMode::FallForward, QCoreApplication::translate("ReflectedEnumValues", "Fall Forward")}
})

REFLECT_ENUM(relive::Path_Slig_Data::StartState,
{
    {relive::Path_Slig_Data::StartState::Listening, QCoreApplication::translate("ReflectedEnumValues", "Listening")},
    {relive::Path_Slig_Data::StartState::Patrol, QCoreApplication::translate("ReflectedEnumValues", "Patrol")},
    {relive::Path_Slig_Data::StartState::Sleeping, QCoreApplication::translate("ReflectedEnumValues", "Sleeping")},
    {relive::Path_Slig_Data::StartState::Chase, QCoreApplication::translate("ReflectedEnumValues", "Chase")},
    {relive::Path_Slig_Data::StartState::ChaseAndDisappear, QCoreApplication::translate("ReflectedEnumValues", "Chase And Disappear")},
    {relive::Path_Slig_Data::StartState::eFallingToChase, QCoreApplication::translate("ReflectedEnumValues", "Falling To Chase")},
    {relive::Path_Slig_Data::StartState::ListeningToGlukkon, QCoreApplication::translate("ReflectedEnumValues", "Listening To Glukkon")},
})

REFLECT_ENUM(relive::reliveXDirection,
{
    {relive::reliveXDirection::eLeft, QCoreApplication::translate("ReflectedEnumValues", "Left")},
    {relive::reliveXDirection::eRight, QCoreApplication::translate("ReflectedEnumValues", "Right")},
})

REFLECT_ENUM(relive::reliveSwitchOp,
{
    {relive::reliveSwitchOp::eSetTrue, QCoreApplication::translate("ReflectedEnumValues", "Set True")},
    {relive::reliveSwitchOp::eSetFalse, QCoreApplication::translate("ReflectedEnumValues", "Set False")},
    {relive::reliveSwitchOp::eToggle, QCoreApplication::translate("ReflectedEnumValues", "Toggle")},
    {relive::reliveSwitchOp::eIncrement, QCoreApplication::translate("ReflectedEnumValues", "Increment")},
    {relive::reliveSwitchOp::eDecrement, QCoreApplication::translate("ReflectedEnumValues", "Decrement")},
})

REFLECT_ENUM(eLineTypes,
{
    {eLineTypes::eFloor_0, QCoreApplication::translate("ReflectedEnumValues", "Floor")},
    {eLineTypes::eWallLeft_1, QCoreApplication::translate("ReflectedEnumValues", "Wall Left")},
    {eLineTypes::eWallRight_2, QCoreApplication::translate("ReflectedEnumValues", "Wall Right")},
    {eLineTypes::eCeiling_3, QCoreApplication::translate("ReflectedEnumValues", "Ceiling")},
    {eLineTypes::eBackgroundFloor_4, QCoreApplication::translate("ReflectedEnumValues", "Background Floor")},
    {eLineTypes::eBackgroundWallLeft_5, QCoreApplication::translate("ReflectedEnumValues", "Background Wall Left")},
    {eLineTypes::eBackgroundWallRight_6, QCoreApplication::translate("ReflectedEnumValues", "Background Wall Right")},
    {eLineTypes::eBackgroundCeiling_7, QCoreApplication::translate("ReflectedEnumValues", "Background Ceiling")},
    {eLineTypes::eTrackLine_8, QCoreApplication::translate("ReflectedEnumValues", "Track Line")},
    {eLineTypes::eArt_9, QCoreApplication::translate("ReflectedEnumValues", "Art")},
    {eLineTypes::eBulletWall_10, QCoreApplication::translate("ReflectedEnumValues", "Bullet Wall")},
    {eLineTypes::eMineCarFloor_11, QCoreApplication::translate("ReflectedEnumValues", "Mine Car Floor")},
    {eLineTypes::eMineCarWall_12, QCoreApplication::translate("ReflectedEnumValues", "Mine Car Wall")},
    {eLineTypes::eMineCarCeiling_13, QCoreApplication::translate("ReflectedEnumValues", "Mine Car Ceiling")},
    {eLineTypes::eBackgroundMineCarFloor_14, QCoreApplication::translate("ReflectedEnumValues", "Background Mine Car Floor")},
    {eLineTypes::eBackgroundMineCarWall_15, QCoreApplication::translate("ReflectedEnumValues", "Background Mine Car Wall")},
    {eLineTypes::eBackgroundMineCarCeiling_16, QCoreApplication::translate("ReflectedEnumValues", "Background Mine Car Ceiling")},
    {eLineTypes::eFlyingObjectWall_17, QCoreApplication::translate("ReflectedEnumValues", "Flying Slig Ceiling")},
    {eLineTypes::eBackgroundFlyingObjectWall_18, QCoreApplication::translate("ReflectedEnumValues", "Background Flying Slig Ceiling")},
    {eLineTypes::eDynamicCollision_32, QCoreApplication::translate("ReflectedEnumValues", "Dynamic Collision")},
    {eLineTypes::eBackgroundDynamicCollision_36, QCoreApplication::translate("ReflectedEnumValues", "Background Dynamic Collision")},
})


REFLECT_ENUM(EReliveLevelIds,
{
    {EReliveLevelIds::eNone, QCoreApplication::translate("ReflectedEnumValues", "None")},
    {EReliveLevelIds::eMenu, QCoreApplication::translate("ReflectedEnumValues", "Menu")},
    {EReliveLevelIds::eMines, QCoreApplication::translate("ReflectedEnumValues", "Mines")},
    {EReliveLevelIds::eNecrum, QCoreApplication::translate("ReflectedEnumValues", "Necrum")},
    {EReliveLevelIds::eMudomoVault, QCoreApplication::translate("ReflectedEnumValues", "Mudomo Vault")},
    {EReliveLevelIds::eMudancheeVault, QCoreApplication::translate("ReflectedEnumValues", "Mudanchee Vault")},
    {EReliveLevelIds::eFeeCoDepot, QCoreApplication::translate("ReflectedEnumValues", "FeeCo Depot")},
    {EReliveLevelIds::eBarracks, QCoreApplication::translate("ReflectedEnumValues", "Barracks")},
    {EReliveLevelIds::eMudancheeVault_Ender, QCoreApplication::translate("ReflectedEnumValues", "Mudanchee Vault Ender")},
    {EReliveLevelIds::eBonewerkz, QCoreApplication::translate("ReflectedEnumValues", "Bonewerkz")},
    {EReliveLevelIds::eBrewery, QCoreApplication::translate("ReflectedEnumValues", "Brewery")},
    {EReliveLevelIds::eBrewery_Ender, QCoreApplication::translate("ReflectedEnumValues", "Brewery Ender")},
    {EReliveLevelIds::eMudomoVault_Ender, QCoreApplication::translate("ReflectedEnumValues", "Mudomo Vault Ender")},
    {EReliveLevelIds::eFeeCoDepot_Ender, QCoreApplication::translate("ReflectedEnumValues", "FeeCo Depot Ender")},
    {EReliveLevelIds::eBarracks_Ender, QCoreApplication::translate("ReflectedEnumValues", "Barracks Ender")},
    {EReliveLevelIds::eBonewerkz_Ender, QCoreApplication::translate("ReflectedEnumValues", "Bonewerkz Ender")},
    {EReliveLevelIds::eCredits, QCoreApplication::translate("ReflectedEnumValues", "Credits")},
    {EReliveLevelIds::eRuptureFarms, QCoreApplication::translate("ReflectedEnumValues", "Rupture Farms")},
    {EReliveLevelIds::eLines, QCoreApplication::translate("ReflectedEnumValues", "Lines")},
    {EReliveLevelIds::eForest, QCoreApplication::translate("ReflectedEnumValues", "Forest")},
    {EReliveLevelIds::eForestTemple, QCoreApplication::translate("ReflectedEnumValues", "Forest Temple")},
    {EReliveLevelIds::eStockYards, QCoreApplication::translate("ReflectedEnumValues", "Stock Yards")},
    {EReliveLevelIds::eStockYardsReturn, QCoreApplication::translate("ReflectedEnumValues", "Stock Yards Return")},
    {EReliveLevelIds::eDesert, QCoreApplication::translate("ReflectedEnumValues", "Desert")},
    {EReliveLevelIds::eDesertTemple, QCoreApplication::translate("ReflectedEnumValues", "Desert Temple")},
    {EReliveLevelIds::eBoardRoom, QCoreApplication::translate("ReflectedEnumValues", "Board Room")},
    {EReliveLevelIds::eRuptureFarmsReturn, QCoreApplication::translate("ReflectedEnumValues", "Rupture Farms Return")},
    {EReliveLevelIds::eForestChase, QCoreApplication::translate("ReflectedEnumValues", "Forest Chase")},
    {EReliveLevelIds::eDesertEscape, QCoreApplication::translate("ReflectedEnumValues", "Desert Escape")},
})

REFLECT_ENUM(relive::reliveScreenChangeEffects,
{
    {relive::reliveScreenChangeEffects::ePlay1FMV, QCoreApplication::translate("ReflectedEnumValues", "Play 1 FMV")},
    {relive::reliveScreenChangeEffects::eRightToLeft, QCoreApplication::translate("ReflectedEnumValues", "Right To Left")},
    {relive::reliveScreenChangeEffects::eLeftToRight, QCoreApplication::translate("ReflectedEnumValues", "Left To Right")},
    {relive::reliveScreenChangeEffects::eBottomToTop, QCoreApplication::translate("ReflectedEnumValues", "Bottom To Top")},
    {relive::reliveScreenChangeEffects::eTopToBottom, QCoreApplication::translate("ReflectedEnumValues", "Top To Bottom")},
    {relive::reliveScreenChangeEffects::eBoxOut, QCoreApplication::translate("ReflectedEnumValues", "Box Out")},
    {relive::reliveScreenChangeEffects::eVerticalSplit, QCoreApplication::translate("ReflectedEnumValues", "Vertical Split")},
    {relive::reliveScreenChangeEffects::eHorizontalSplit, QCoreApplication::translate("ReflectedEnumValues", "Horizontal Split")},
    {relive::reliveScreenChangeEffects::eUnknown_8, QCoreApplication::translate("ReflectedEnumValues", "Unknown8")},
    {relive::reliveScreenChangeEffects::eInstantChange, QCoreApplication::translate("ReflectedEnumValues", "Instant Change")}
})

REFLECT_ENUM(relive::Path_ElectricWall::ElectricWallStartState,
{
    {relive::Path_ElectricWall::ElectricWallStartState::eOff, QCoreApplication::translate("ReflectedEnumValues", "Off")},
    {relive::Path_ElectricWall::ElectricWallStartState::eOn, QCoreApplication::translate("ReflectedEnumValues", "On")},
})

REFLECT_ENUM(relive::Path_Mudokon::MudJobs,
{
    {relive::Path_Mudokon::MudJobs::eChisle, QCoreApplication::translate("ReflectedEnumValues", "Chisle")},
    {relive::Path_Mudokon::MudJobs::eSitScrub, QCoreApplication::translate("ReflectedEnumValues", "Scrub")},
    {relive::Path_Mudokon::MudJobs::eAngryWorker, QCoreApplication::translate("ReflectedEnumValues", "Angry Worker")},
    {relive::Path_Mudokon::MudJobs::eDamageRingGiver, QCoreApplication::translate("ReflectedEnumValues", "Damage Ring Giver")},
    {relive::Path_Mudokon::MudJobs::eHealthRingGiver, QCoreApplication::translate("ReflectedEnumValues", "Health Ring Giver")},
})

REFLECT_ENUM(relive::Path_BirdPortal::PortalType,
{
    {relive::Path_BirdPortal::PortalType::eAbe, QCoreApplication::translate("ReflectedEnumValues", "Abe")},
    {relive::Path_BirdPortal::PortalType::eWorker, QCoreApplication::translate("ReflectedEnumValues", "Worker")},
    {relive::Path_BirdPortal::PortalType::eShrykull, QCoreApplication::translate("ReflectedEnumValues", "Shrykull")},
    {relive::Path_BirdPortal::PortalType::eMudTeleport, QCoreApplication::translate("ReflectedEnumValues", "Mud Teleport")},
})

REFLECT_ENUM(relive::Path_Door::DoorStates,
{
    {relive::Path_Door::DoorStates::eOpen, QCoreApplication::translate("ReflectedEnumValues", "Open")},
    {relive::Path_Door::DoorStates::eClosed, QCoreApplication::translate("ReflectedEnumValues", "Closed")},
    {relive::Path_Door::DoorStates::eOpening, QCoreApplication::translate("ReflectedEnumValues", "Opening")},
    {relive::Path_Door::DoorStates::eClosing, QCoreApplication::translate("ReflectedEnumValues", "Closing")}
})

REFLECT_ENUM(relive::Path_Door::DoorTypes,
{
    {relive::Path_Door::DoorTypes::eBasicDoor, QCoreApplication::translate("ReflectedEnumValues", "Basic Door")},
    {relive::Path_Door::DoorTypes::eTasksDoorWithSecretMusic, QCoreApplication::translate("ReflectedEnumValues", "Tasks Door With Secret Music")},
    {relive::Path_Door::DoorTypes::eTasksDoor, QCoreApplication::translate("ReflectedEnumValues", "Tasks Door")},
    {relive::Path_Door::DoorTypes::eTrialDoor, QCoreApplication::translate("ReflectedEnumValues", "(AO) Trial Door")},
    {relive::Path_Door::DoorTypes::eHubDoor, QCoreApplication::translate("ReflectedEnumValues", "(AO) Hub Door")}
})

REFLECT_ENUM(relive::Path_Lever::LeverSoundType,
{
    {relive::Path_Lever::LeverSoundType::eNone, QCoreApplication::translate("ReflectedEnumValues", "None")},
    {relive::Path_Lever::LeverSoundType::eWell, QCoreApplication::translate("ReflectedEnumValues", "Well")},
    {relive::Path_Lever::LeverSoundType::eSwitchBellHammer, QCoreApplication::translate("ReflectedEnumValues", "Switch Bell Hammer")},
    {relive::Path_Lever::LeverSoundType::eDoor, QCoreApplication::translate("ReflectedEnumValues", "Door")},
    {relive::Path_Lever::LeverSoundType::eElectricWall, QCoreApplication::translate("ReflectedEnumValues", "Electric Wall")},
    {relive::Path_Lever::LeverSoundType::eSecurityOrb, QCoreApplication::translate("ReflectedEnumValues", "Security Orb")},
    {relive::Path_Lever::LeverSoundType::eLift, QCoreApplication::translate("ReflectedEnumValues", "(AE) Lift")}
})

REFLECT_ENUM(relive::Path_Lever::LeverSoundDirection,
{
    {relive::Path_Lever::LeverSoundDirection::eLeftAndRight, QCoreApplication::translate("ReflectedEnumValues", "Left And Right")},
    {relive::Path_Lever::LeverSoundDirection::eLeft, QCoreApplication::translate("ReflectedEnumValues", "Left")},
    {relive::Path_Lever::LeverSoundDirection::eRight, QCoreApplication::translate("ReflectedEnumValues", "Right")},
})

REFLECT_ENUM(relive::Path_Hoist::Type,
{
    {relive::Path_Hoist::Type::eNextFloor, QCoreApplication::translate("ReflectedEnumValues", "Next Floor")},
    {relive::Path_Hoist::Type::eOffScreen, QCoreApplication::translate("ReflectedEnumValues", "Off Screen")},
})

REFLECT_ENUM(relive::Path_Hoist::GrabDirection,
{
    {relive::Path_Hoist::GrabDirection::eFacingLeft, QCoreApplication::translate("ReflectedEnumValues", "Facing Left")},
    {relive::Path_Hoist::GrabDirection::eFacingRight, QCoreApplication::translate("ReflectedEnumValues", "Facing Right")},
    {relive::Path_Hoist::GrabDirection::eFacingAnyDirection, QCoreApplication::translate("ReflectedEnumValues", "Facing Any Direction")},
})

REFLECT_ENUM(relive::Path_BoomMachine::NozzleSide,
{
    {relive::Path_BoomMachine::NozzleSide::eRight, QCoreApplication::translate("ReflectedEnumValues", "Right")},
    {relive::Path_BoomMachine::NozzleSide::eLeft, QCoreApplication::translate("ReflectedEnumValues", "Left")},
})

REFLECT_ENUM(relive::Path_EnemyStopper::StopDirection,
{
    {relive::Path_EnemyStopper::StopDirection::Left, QCoreApplication::translate("ReflectedEnumValues", "Left")},
    {relive::Path_EnemyStopper::StopDirection::Right, QCoreApplication::translate("ReflectedEnumValues", "Right")},
    {relive::Path_EnemyStopper::StopDirection::Both, QCoreApplication::translate("ReflectedEnumValues", "Both")},
})

REFLECT_ENUM(relive::Path_UXB::StartState,
{
    {relive::Path_UXB::StartState::eOn, QCoreApplication::translate("ReflectedEnumValues", "On")},
    {relive::Path_UXB::StartState::eOff, QCoreApplication::translate("ReflectedEnumValues", "Off")},
})

REFLECT_ENUM(relive::Path_Edge::GrabDirection,
{
    {relive::Path_Edge::GrabDirection::eFacingLeft, QCoreApplication::translate("ReflectedEnumValues", "Facing Left")},
    {relive::Path_Edge::GrabDirection::eFacingRight, QCoreApplication::translate("ReflectedEnumValues", "Facing Right")},
    {relive::Path_Edge::GrabDirection::eFacingAnyDirection, QCoreApplication::translate("ReflectedEnumValues", "Facing Any Direction")},
})

REFLECT_ENUM(relive::Path_ShadowZone::Scale,
{
    {relive::Path_ShadowZone::Scale::eBoth, QCoreApplication::translate("ReflectedEnumValues", "Both")},
    {relive::Path_ShadowZone::Scale::eHalf, QCoreApplication::translate("ReflectedEnumValues", "Half")},
    {relive::Path_ShadowZone::Scale::eFull, QCoreApplication::translate("ReflectedEnumValues", "Full")},
})

REFLECT_ENUM(relive::Path_MusicTrigger::MusicTriggerMusicType,
{
    {relive::Path_MusicTrigger::MusicTriggerMusicType::eDrumAmbience, QCoreApplication::translate("ReflectedEnumValues", "Drum Ambience")},
    {relive::Path_MusicTrigger::MusicTriggerMusicType::eDeathDrumShort, QCoreApplication::translate("ReflectedEnumValues", "Death Drum Short")},
    {relive::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaLong, QCoreApplication::translate("ReflectedEnumValues", "Secret Area Long")},
    {relive::Path_MusicTrigger::MusicTriggerMusicType::eSoftChase, QCoreApplication::translate("ReflectedEnumValues", "Soft Chase")},
    {relive::Path_MusicTrigger::MusicTriggerMusicType::eIntenseChase, QCoreApplication::translate("ReflectedEnumValues", "Intense Chase")},
    {relive::Path_MusicTrigger::MusicTriggerMusicType::eChime, QCoreApplication::translate("ReflectedEnumValues", "Chime")},
    {relive::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaShort, QCoreApplication::translate("ReflectedEnumValues", "Secret Area Short")},
})

REFLECT_ENUM(relive::Path_MusicTrigger::TriggeredBy,
{
    {relive::Path_MusicTrigger::TriggeredBy::eTimer, QCoreApplication::translate("ReflectedEnumValues", "Timer")},
    {relive::Path_MusicTrigger::TriggeredBy::eTouching, QCoreApplication::translate("ReflectedEnumValues", "Touching")},
    {relive::Path_MusicTrigger::TriggeredBy::eSwitchId, QCoreApplication::translate("ReflectedEnumValues", "(AO) Switch ID")},
    {relive::Path_MusicTrigger::TriggeredBy::eUnknown, QCoreApplication::translate("ReflectedEnumValues", "(AO) Unknown")},
})

REFLECT_ENUM(relive::Path_InvisibleSwitch::InvisibleSwitchScale,
{
    {relive::Path_InvisibleSwitch::InvisibleSwitchScale::eHalf, QCoreApplication::translate("ReflectedEnumValues", "Half")},
    {relive::Path_InvisibleSwitch::InvisibleSwitchScale::eFull, QCoreApplication::translate("ReflectedEnumValues", "Full")},
    {relive::Path_InvisibleSwitch::InvisibleSwitchScale::eAny, QCoreApplication::translate("ReflectedEnumValues", "Any")},
})

REFLECT_ENUM(relive::Path_GasEmitter::GasColour,
{
    {relive::Path_GasEmitter::GasColour::eYellow, QCoreApplication::translate("ReflectedEnumValues", "Yellow")},
    {relive::Path_GasEmitter::GasColour::eRed, QCoreApplication::translate("ReflectedEnumValues", "Red")},
    {relive::Path_GasEmitter::GasColour::eGreen, QCoreApplication::translate("ReflectedEnumValues", "Green")},
    {relive::Path_GasEmitter::GasColour::eBlue, QCoreApplication::translate("ReflectedEnumValues", "Blue")},
    {relive::Path_GasEmitter::GasColour::eWhite, QCoreApplication::translate("ReflectedEnumValues", "White")},
})

REFLECT_ENUM(::relive::Path_BackgroundAnimation::BgAnimSounds,
{
    {relive::Path_BackgroundAnimation::BgAnimSounds::eNone, QCoreApplication::translate("ReflectedEnumValues", "None")},
    {relive::Path_BackgroundAnimation::BgAnimSounds::eFire, QCoreApplication::translate("ReflectedEnumValues", "Fire")},
})

REFLECT_ENUM(relive::Path_BackgroundAnimation::Layer,
{
    {relive::Path_BackgroundAnimation::Layer::eLayer0, QCoreApplication::translate("ReflectedEnumValues", "Layer 0")},
    {relive::Path_BackgroundAnimation::Layer::eLayer2, QCoreApplication::translate("ReflectedEnumValues", "Layer 2")},
})

REFLECT_ENUM(relive::TBlendModes,
{
    {relive::TBlendModes::eBlend_0, QCoreApplication::translate("ReflectedEnumValues", "Blend 0")},
    {relive::TBlendModes::eBlend_1, QCoreApplication::translate("ReflectedEnumValues", "Blend 1")},
    {relive::TBlendModes::eBlend_2, QCoreApplication::translate("ReflectedEnumValues", "Blend 2")},
    {relive::TBlendModes::eBlend_3, QCoreApplication::translate("ReflectedEnumValues", "Blend 3")},
})

REFLECT_ENUM(relive::Path_LiftPoint::LiftPointStopType,
{
    {relive::Path_LiftPoint::LiftPointStopType::eTopFloor, QCoreApplication::translate("ReflectedEnumValues", "Top Floor")},
    {relive::Path_LiftPoint::LiftPointStopType::eBottomFloor, QCoreApplication::translate("ReflectedEnumValues", "Bottom Floor")},
    {relive::Path_LiftPoint::LiftPointStopType::eMiddleFloor, QCoreApplication::translate("ReflectedEnumValues", "Middle Floor")},
    {relive::Path_LiftPoint::LiftPointStopType::eMiddleLockFloor, QCoreApplication::translate("ReflectedEnumValues", "Middle Lock Floor")},
    {relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly, QCoreApplication::translate("ReflectedEnumValues", "Start Point Only")},
})

REFLECT_ENUM(relive::Path_PullRingRope::PullRingSwitchSound,
{
    {relive::Path_PullRingRope::PullRingSwitchSound::eNone, QCoreApplication::translate("ReflectedEnumValues", "None")},
    {relive::Path_PullRingRope::PullRingSwitchSound::eWellExit, QCoreApplication::translate("ReflectedEnumValues", "Well Exit")},
    {relive::Path_PullRingRope::PullRingSwitchSound::eRingBellHammer, QCoreApplication::translate("ReflectedEnumValues", "Ring Unknown Trigger")},
    {relive::Path_PullRingRope::PullRingSwitchSound::eDoorEffect, QCoreApplication::translate("ReflectedEnumValues", "Door Effect")},
})

REFLECT_ENUM(relive::Path_PullRingRope::PullRingSoundDirection,
{
    {relive::Path_PullRingRope::PullRingSoundDirection::eLeftAndRight, QCoreApplication::translate("ReflectedEnumValues", "Left And Right")},
    {relive::Path_PullRingRope::PullRingSoundDirection::eLeft, QCoreApplication::translate("ReflectedEnumValues", "Left")},
    {relive::Path_PullRingRope::PullRingSoundDirection::eRight, QCoreApplication::translate("ReflectedEnumValues", "Right")},
})

REFLECT_ENUM(relive::Path_ContinuePoint::Scale,
{
    {relive::Path_ContinuePoint::Scale::eNone, QCoreApplication::translate("ReflectedEnumValues", "None")},
    {relive::Path_ContinuePoint::Scale::eHalf, QCoreApplication::translate("ReflectedEnumValues", "Half")},
    {relive::Path_ContinuePoint::Scale::eFull, QCoreApplication::translate("ReflectedEnumValues", "Full")},
})

REFLECT_ENUM(relive::Path_ContinuePoint::spawnDirection,
{
    {relive::Path_ContinuePoint::spawnDirection::eRight, QCoreApplication::translate("ReflectedEnumValues", "Right")},
    {relive::Path_ContinuePoint::spawnDirection::eLeft, QCoreApplication::translate("ReflectedEnumValues", "Left")}
})

REFLECT_ENUM(relive::Path_WheelSyncer::OutputRequirement,
{
    {relive::Path_WheelSyncer::OutputRequirement::eAllOn, QCoreApplication::translate("ReflectedEnumValues", "All On")},
    {relive::Path_WheelSyncer::OutputRequirement::e1OnAnd2Off, QCoreApplication::translate("ReflectedEnumValues", "1 On And 2 Off")},
    {relive::Path_WheelSyncer::OutputRequirement::e1Or2On, QCoreApplication::translate("ReflectedEnumValues", "1 Or 2 On")},
    {relive::Path_WheelSyncer::OutputRequirement::e1OnOr2Off, QCoreApplication::translate("ReflectedEnumValues", "1 On Or 2 Off")},
})

REFLECT_ENUM(relive::Path_Drill::DrillBehavior,
{
    {relive::Path_Drill::DrillBehavior::eNotInteractable, QCoreApplication::translate("ReflectedEnumValues", "Not Interactable")},
    {relive::Path_Drill::DrillBehavior::eToggle, QCoreApplication::translate("ReflectedEnumValues", "Toggle")},
    {relive::Path_Drill::DrillBehavior::eUse, QCoreApplication::translate("ReflectedEnumValues", "Use")},
})

REFLECT_ENUM(relive::Path_Drill::DrillDirection,
{
    {relive::Path_Drill::DrillDirection::eDown, QCoreApplication::translate("ReflectedEnumValues", "Down")},
    {relive::Path_Drill::DrillDirection::eRight, QCoreApplication::translate("ReflectedEnumValues", "Right")},
    {relive::Path_Drill::DrillDirection::eLeft, QCoreApplication::translate("ReflectedEnumValues", "Left")},
})

REFLECT_ENUM(relive::Path_TrapDoor::StartState,
{
    {relive::Path_TrapDoor::StartState::eOpen, QCoreApplication::translate("ReflectedEnumValues", "Open")},
    {relive::Path_TrapDoor::StartState::eClosed, QCoreApplication::translate("ReflectedEnumValues", "Closed")}
})

REFLECT_ENUM(relive::Path_LiftMover::YDirection,
{
    {relive::Path_LiftMover::YDirection::eDown, QCoreApplication::translate("ReflectedEnumValues", "Down")},
    {relive::Path_LiftMover::YDirection::eUp, QCoreApplication::translate("ReflectedEnumValues", "Up")},
})

REFLECT_ENUM(relive::Path_MotionDetector::InitialMoveDirection,
{
    {relive::Path_MotionDetector::InitialMoveDirection::eRight, QCoreApplication::translate("ReflectedEnumValues", "Right")},
    {relive::Path_MotionDetector::InitialMoveDirection::eLeft, QCoreApplication::translate("ReflectedEnumValues", "Left")},
})

REFLECT_ENUM(relive::Path_DoorFlame::Colour,
{
    {relive::Path_DoorFlame::Colour::eDefault, QCoreApplication::translate("ReflectedEnumValues", "Default")},
    {relive::Path_DoorFlame::Colour::eRed, QCoreApplication::translate("ReflectedEnumValues", "Red")},
    {relive::Path_DoorFlame::Colour::eGreen, QCoreApplication::translate("ReflectedEnumValues", "Green")},
    {relive::Path_DoorFlame::Colour::eBlue, QCoreApplication::translate("ReflectedEnumValues", "Blue")},
})

REFLECT_ENUM(relive::Path_CrawlingSlig::StartState,
{
    {relive::Path_CrawlingSlig::StartState::eSleeping, QCoreApplication::translate("ReflectedEnumValues", "Sleeping")},
    {relive::Path_CrawlingSlig::StartState::eAwake, QCoreApplication::translate("ReflectedEnumValues", "Awake")},
})

REFLECT_ENUM(relive::Path_CrawlingSlig::CrawlDirection,
{
    {relive::Path_CrawlingSlig::CrawlDirection::eLeft, QCoreApplication::translate("ReflectedEnumValues", "Left")},
    {relive::Path_CrawlingSlig::CrawlDirection::eRight, QCoreApplication::translate("ReflectedEnumValues", "Right")},
    {relive::Path_CrawlingSlig::CrawlDirection::eRandom, QCoreApplication::translate("ReflectedEnumValues", "Random")},
})

// this probably shouldn't be here since the sound/sound direction implementation is extremely scuffed
// but maybe someone wants to use it anyway
REFLECT_ENUM(relive::Path_CrawlingSligButton::ButtonSounds,
{
    {relive::Path_CrawlingSligButton::ButtonSounds::None, QCoreApplication::translate("ReflectedEnumValues", "None")},
    {relive::Path_CrawlingSligButton::ButtonSounds::SackHit1, QCoreApplication::translate("ReflectedEnumValues", "Sack Hit 1")},
    {relive::Path_CrawlingSligButton::ButtonSounds::SackHit2, QCoreApplication::translate("ReflectedEnumValues", "Sack Hit 2")},
    {relive::Path_CrawlingSligButton::ButtonSounds::FallingItemPresence2, QCoreApplication::translate("ReflectedEnumValues", "Falling Item Presence 2")},
    {relive::Path_CrawlingSligButton::ButtonSounds::SecurityOrb, QCoreApplication::translate("ReflectedEnumValues", "Security Orb")},
    {relive::Path_CrawlingSligButton::ButtonSounds::Bullet1, QCoreApplication::translate("ReflectedEnumValues", "Bullet 1")},
    {relive::Path_CrawlingSligButton::ButtonSounds::AbeGenericMovement, QCoreApplication::translate("ReflectedEnumValues", "Abe Generic Movement")},
})

REFLECT_ENUM(relive::Path_Glukkon::GlukkonTypes,
{
    {relive::Path_Glukkon::GlukkonTypes::eNormal, QCoreApplication::translate("ReflectedEnumValues", "Normal")},
    {relive::Path_Glukkon::GlukkonTypes::eStoryAslik, QCoreApplication::translate("ReflectedEnumValues", "Story Aslik")},
    {relive::Path_Glukkon::GlukkonTypes::eStoryDripik, QCoreApplication::translate("ReflectedEnumValues", "Story Dripik")},
    {relive::Path_Glukkon::GlukkonTypes::eStoryPhleg, QCoreApplication::translate("ReflectedEnumValues", "Story Phleg")},
})

REFLECT_ENUM(relive::Path_Glukkon::Facing,
{
    {relive::Path_Glukkon::Facing::eRight, QCoreApplication::translate("ReflectedEnumValues", "Right")},
    {relive::Path_Glukkon::Facing::eLeft, QCoreApplication::translate("ReflectedEnumValues", "Left")},
})

REFLECT_ENUM(relive::Path_Glukkon::SpawnType,
{
    {relive::Path_Glukkon::SpawnType::eRegularSpawn, QCoreApplication::translate("ReflectedEnumValues", "Regular Spawn")},
    {relive::Path_Glukkon::SpawnType::eFacingLeft, QCoreApplication::translate("ReflectedEnumValues", "Facing Left")},
    {relive::Path_Glukkon::SpawnType::eFacingRight, QCoreApplication::translate("ReflectedEnumValues", "Facing Right")},
    {relive::Path_Glukkon::SpawnType::eFullSpawnEffects, QCoreApplication::translate("ReflectedEnumValues", "Full Spawn Effects")},
})

REFLECT_ENUM(relive::Path_Glukkon::Behavior,
{
    {relive::Path_Glukkon::Behavior::eIgnoreWalls, QCoreApplication::translate("ReflectedEnumValues", "Ignore Walls")},
    {relive::Path_Glukkon::Behavior::eCheckForWalls, QCoreApplication::translate("ReflectedEnumValues", "Check For Walls")},
})

REFLECT_ENUM(relive::Path_FootSwitch::FootSwitchTriggerBy,
{
    {relive::Path_FootSwitch::FootSwitchTriggerBy::eAbe, QCoreApplication::translate("ReflectedEnumValues", "Abe")},
    {relive::Path_FootSwitch::FootSwitchTriggerBy::eAnyone, QCoreApplication::translate("ReflectedEnumValues", "Anyone")},
})

REFLECT_ENUM(relive::Path_SlogSpawner::StartDirection,
{
    {relive::Path_SlogSpawner::StartDirection::eRight, QCoreApplication::translate("ReflectedEnumValues", "Right")},
    {relive::Path_SlogSpawner::StartDirection::eLeft, QCoreApplication::translate("ReflectedEnumValues", "Left")},
})

REFLECT_ENUM(relive::Path_Scrab::ScrabPatrolType,
{
    {relive::Path_Scrab::ScrabPatrolType::eWalk, QCoreApplication::translate("ReflectedEnumValues", "Walk")},
    {relive::Path_Scrab::ScrabPatrolType::eRunOrWalk192, QCoreApplication::translate("ReflectedEnumValues", "Run Or Walk 192")},
    {relive::Path_Scrab::ScrabPatrolType::eRunOrWalk128, QCoreApplication::translate("ReflectedEnumValues", "Run Or Walk 128")},
    {relive::Path_Scrab::ScrabPatrolType::eRunOrWalk64, QCoreApplication::translate("ReflectedEnumValues", "Run Or Walk 64")},
    {relive::Path_Scrab::ScrabPatrolType::eRun, QCoreApplication::translate("ReflectedEnumValues", "Run")}
})

REFLECT_ENUM(relive::Path_ScrabSpawner::SpawnDirection,
{
    {relive::Path_ScrabSpawner::SpawnDirection::eNone, QCoreApplication::translate("ReflectedEnumValues", "None")},
    {relive::Path_ScrabSpawner::SpawnDirection::eLeft, QCoreApplication::translate("ReflectedEnumValues", "Left")},
    {relive::Path_ScrabSpawner::SpawnDirection::eRight, QCoreApplication::translate("ReflectedEnumValues", "Right")},
})

REFLECT_ENUM(relive::Path_Paramite::EntranceType,
{
    {relive::Path_Paramite::EntranceType::ePatrol, QCoreApplication::translate("ReflectedEnumValues", "Patrol")},
    {relive::Path_Paramite::EntranceType::eSurpriseWeb, QCoreApplication::translate("ReflectedEnumValues", "Surprise Web")},
    {relive::Path_Paramite::EntranceType::eSlightlyHigherSpawnSurpriseWeb, QCoreApplication::translate("ReflectedEnumValues", "Slightly Higher Spawn Surprise Web")},
})

REFLECT_ENUM(relive::Path_BellsongStone::BellsongTypes,
{
    {relive::Path_BellsongStone::BellsongTypes::eWhistle, QCoreApplication::translate("ReflectedEnumValues", "Whistle")},
    {relive::Path_BellsongStone::BellsongTypes::eChimes, QCoreApplication::translate("ReflectedEnumValues", "Chimes")},
})

REFLECT_ENUM(relive::Path_LightEffect::Type,
{
    {relive::Path_LightEffect::Type::Star, QCoreApplication::translate("ReflectedEnumValues", "Star")},
    {relive::Path_LightEffect::Type::GoldGlow, QCoreApplication::translate("ReflectedEnumValues", "Gold Glow")},
    {relive::Path_LightEffect::Type::GreenGlow, QCoreApplication::translate("ReflectedEnumValues", "Green Glow")},
    {relive::Path_LightEffect::Type::FlintGlow, QCoreApplication::translate("ReflectedEnumValues", "Flint Glow")},
    {relive::Path_LightEffect::Type::Switchable_RedGreenDoorLights, QCoreApplication::translate("ReflectedEnumValues", "RedGreen Door Light")},
    {relive::Path_LightEffect::Type::Switchable_RedGreenHubLight, QCoreApplication::translate("ReflectedEnumValues", "RedGreen Hub Light")},
})

REFLECT_ENUM(relive::Path_RingMudokon::MustFaceMud,
{
    {relive::Path_RingMudokon::MustFaceMud::eYes, QCoreApplication::translate("ReflectedEnumValues", "Yes")},
    {relive::Path_RingMudokon::MustFaceMud::eNo, QCoreApplication::translate("ReflectedEnumValues", "No")},
})

REFLECT_ENUM(relive::Path_MeatSaw::Type,
{
    {relive::Path_MeatSaw::Type::eAutomaticPersistOffscreen, QCoreApplication::translate("ReflectedEnumValues", "Automatic Persist Offscreen")},
    {relive::Path_MeatSaw::Type::eAutomatic, QCoreApplication::translate("ReflectedEnumValues", "Automatic")},
    {relive::Path_MeatSaw::Type::eSwitchId, QCoreApplication::translate("ReflectedEnumValues", "Switch ID")},
})

REFLECT_ENUM(relive::Path_MeatSaw::StartState,
{
    {relive::Path_MeatSaw::StartState::eOff, QCoreApplication::translate("ReflectedEnumValues", "Off")},
    {relive::Path_MeatSaw::StartState::eOn, QCoreApplication::translate("ReflectedEnumValues", "On")},
})

REFLECT_ENUM(relive::Path_LiftMudokon::Direction,
{
    {relive::Path_LiftMudokon::Direction::eRight, QCoreApplication::translate("ReflectedEnumValues", "Right")},
    {relive::Path_LiftMudokon::Direction::eLeft, QCoreApplication::translate("ReflectedEnumValues", "Left")},
})

REFLECT_ENUM(relive::Path_BeeSwarmHole::MovementType,
{
    {relive::Path_BeeSwarmHole::MovementType::eHover, QCoreApplication::translate("ReflectedEnumValues", "Hover")},
    {relive::Path_BeeSwarmHole::MovementType::eAttack, QCoreApplication::translate("ReflectedEnumValues", "Attack")},
    {relive::Path_BeeSwarmHole::MovementType::eFollowPath, QCoreApplication::translate("ReflectedEnumValues", "Follow Path")},
})

REFLECT_ENUM(relive::Path_ZBall::StartPos,
{
    {relive::Path_ZBall::StartPos::eCenter, QCoreApplication::translate("ReflectedEnumValues", "Center")},
    {relive::Path_ZBall::StartPos::eOut, QCoreApplication::translate("ReflectedEnumValues", "Out")},
    {relive::Path_ZBall::StartPos::eIn, QCoreApplication::translate("ReflectedEnumValues", "In")},
})

REFLECT_ENUM(relive::Path_ZBall::Speed,
{
    {relive::Path_ZBall::Speed::eNormal, QCoreApplication::translate("ReflectedEnumValues", "Normal")},
    {relive::Path_ZBall::Speed::eFast, QCoreApplication::translate("ReflectedEnumValues", "Fast")},
    {relive::Path_ZBall::Speed::eSlow, QCoreApplication::translate("ReflectedEnumValues", "Slow")},
})

