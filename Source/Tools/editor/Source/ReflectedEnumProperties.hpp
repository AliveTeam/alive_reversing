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
            const char* mName;                                                            \
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
        static const char* to_str(s32 idx)                                                \
        {                                                                                 \
            return mArray[idx].mName;                                                     \
        }                                                                                 \
                                                                                          \
        static ENUM_TYPE to_value(s32 idx)                                                \
        {                                                                                 \
            return mArray[idx].mValue;                                                    \
        }                                                                                 \
        static constexpr EnumPair mArray[] = __VA_ARGS__;                                 \
        static constexpr u32 mArraySize = ALIVE_COUNTOF(mArray);                          \
        static inline const char* mEnumName = #ENUM_TYPE;                                 \
    };

REFLECT_ENUM(ReliveTypes, {
    {ReliveTypes::eNone, QT_TRANSLATE_NOOP("ReflectedEnumValues","None")},
    {ReliveTypes::eCrawlingSligButton, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Crawling Slig Button")},
    {ReliveTypes::eWheelSyncer, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Wheel Syncer")},
    {ReliveTypes::eDemoSpawnPoint, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Demo Spawn Point")},
    {ReliveTypes::eMultiSwitchController, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Multi Switch Controller")},
    {ReliveTypes::eStatusLight, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Status Light")},
    {ReliveTypes::eSlapLock, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Slap Lock")},
    {ReliveTypes::eParamiteWebLine, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Paramite Web Line")},
    {ReliveTypes::eGlukkonSwitch, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Glukkon Switch")},
    {ReliveTypes::eDoorBlocker, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Door Blocker")},
    {ReliveTypes::eTrainDoor, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Train Door")},
    {ReliveTypes::eLevelLoader, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Level Loader")},
    {ReliveTypes::eSligGetWings, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Slig Get Wings")},
    {ReliveTypes::eSligGetPants, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Slig Get Pants")},
    {ReliveTypes::eTeleporter, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Teleporter")},
    {ReliveTypes::eWater, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Water")},
    {ReliveTypes::eWorkWheel, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Work Wheel")},
    {ReliveTypes::eLCDScreen, QT_TRANSLATE_NOOP("ReflectedEnumValues", "LCD Screen")},
    {ReliveTypes::eInvisibleSwitch, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Invisible Switch")},
    {ReliveTypes::eDoorFlame, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Door Flame")},
    {ReliveTypes::eMovingBomb, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Moving Bomb")},
    {ReliveTypes::eMainMenuController, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Main Menu Controller")},
    {ReliveTypes::eHintFly, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Hint Fly")},
    {ReliveTypes::eSecurityDoor, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Security Door")},
    {ReliveTypes::eCreditsController, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Credits Controller")},
    {ReliveTypes::eLCDStatusBoard, QT_TRANSLATE_NOOP("ReflectedEnumValues", "LCD Status Board")},
    {ReliveTypes::eSwitchStateBooleanLogic, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Switch State Boolean Logic")},
    {ReliveTypes::eLightEffect, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Light Effect")},
    {ReliveTypes::eSlogSpawner, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Slog Spawner")},
    {ReliveTypes::eGasEmitter, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Gas Emitter")},
    {ReliveTypes::eRingCancel, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Ring Cancel")},
    {ReliveTypes::eElumWall, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Elum Wall")},
    {ReliveTypes::eAbeStart, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Abe Start")},
    {ReliveTypes::eBeeSwarmHole, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Bee Swarm Hole")},
    {ReliveTypes::eFallingItem, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Falling Item")},
    {ReliveTypes::eShadowZone, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Shadow Zone")},
    {ReliveTypes::eStartController, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Start Controller")},
    {ReliveTypes::eBirdPortalExit, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Bird Portal Exit")},
    {ReliveTypes::eHoneyDripTarget, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Honey Drip Target")},
    {ReliveTypes::ePathTransition, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Path Transition")},
    {ReliveTypes::eZSligCover, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Z Slig Cover")},
    {ReliveTypes::eResetPath, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Reset Path")},
    {ReliveTypes::eElumPathTrans, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Elum Path Trans")},
    {ReliveTypes::eScrabLeftBound, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Scrab Left Bound")},
    {ReliveTypes::eScrabRightBound, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Scrab Right Bound")},
    {ReliveTypes::eScrabNoFall, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Scrab No Fall")},
    {ReliveTypes::eMovingBombStopper, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Moving Bomb Stopper")},
    {ReliveTypes::eElumStart, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Elum Start")},
    {ReliveTypes::eEdge, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Edge")},
    {ReliveTypes::eSoftLanding, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Soft Landing")},
    {ReliveTypes::eMovieHandStone, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Movie Hand Stone")},
    {ReliveTypes::eBellSongStone, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Bell Song Stone")},
    {ReliveTypes::eDemoPlaybackStone, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Demo Playback Stone")},
    {ReliveTypes::eHandStone, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Hand Stone")},
    {ReliveTypes::eHoist, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Hoist")},
    {ReliveTypes::eContinuePoint, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Continue Point")},
    {ReliveTypes::eWellLocal, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Well Local")},
    {ReliveTypes::eWellExpress, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Well Express")},
    {ReliveTypes::eMudokonPathTrans, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Mudokon Path Trans")},
    {ReliveTypes::eRingMudokon, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Ring Mudokon")},
    {ReliveTypes::eLiftMudokon, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Lift Mudokon")},
    {ReliveTypes::eInvisibleZone, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Invisible Zone")},
    {ReliveTypes::eEnemyStopper, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Enemy Stopper")},
    {ReliveTypes::eSligBoundLeft, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Slig Bound Left")},
    {ReliveTypes::eSligBoundRight, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Slig Boung Right")},
    {ReliveTypes::eSligPersist, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Slig Persist")},
    {ReliveTypes::eZzzSpawner, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Zzz Spawner")},
    {ReliveTypes::eKillUnsavedMuds, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Kill Unsaved Muds")},
    {ReliveTypes::eDeathDrop, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Death Drop")},
    {ReliveTypes::eAlarm, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Alarm")},
    {ReliveTypes::eScreenManager, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Screen Manager")},
    {ReliveTypes::eBackgroundAnimation, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Background Animation")},
    {ReliveTypes::eBat, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Bat")},
    {ReliveTypes::eLiftMover, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Lift Mover")},
    {ReliveTypes::eTimedMine, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Timed Mine")},
    {ReliveTypes::eBullet, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Bullet")},
    {ReliveTypes::eDDCheat, QT_TRANSLATE_NOOP("ReflectedEnumValues", "DDCheat")},
    {ReliveTypes::eBells, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Bells")},
    {ReliveTypes::eChimeLock, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Chime Lock")},
    {ReliveTypes::eGasCountDown, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Gas Count Down")},
    {ReliveTypes::eParticleBurst, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Particle Burst")},
    {ReliveTypes::eDoor, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Door")},
    {ReliveTypes::eGameSpeak, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Game Speak")},
    {ReliveTypes::eElectricWall, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Electric Wall")},
    {ReliveTypes::eElum, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Elum")},
    {ReliveTypes::eBellHammer, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Bell Hammer")},
    {ReliveTypes::ePalOverwriter, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Pal Overwritter")},
    {ReliveTypes::eGroundExplosion, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Ground Explosion")},
    {ReliveTypes::eSecurityClaw, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Security Claw")},
    {ReliveTypes::eRockSpawner, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Rock Spawner")},
    {ReliveTypes::eFlintLockFire, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Flint Lock Fire")},
    {ReliveTypes::eThrowableTotalIndicator, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Throwable Total Indicator")},
    {ReliveTypes::eFootSwitch, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Foot Switch")},
    {ReliveTypes::eGameEnderController, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Game Ender Controller")},
    {ReliveTypes::eDeathBird, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Death Bird")},
    {ReliveTypes::eLoadingFile, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Loading File")},
    {ReliveTypes::eGrenade, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Grenade")},
    {ReliveTypes::eBoomMachine, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Boom Machine")},
    {ReliveTypes::eBackgroundGlukkon, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Background Glukkon")},
    {ReliveTypes::eAbe, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Abe")},
    {ReliveTypes::MainMenuFade, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Main Menu Fade")},
    {ReliveTypes::eHoneySack, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Honey Sack")},
    {ReliveTypes::eHoney, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Honey")},
    {ReliveTypes::eClawOrBirdPortalTerminator, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Claw OR Bird Portal Terminator")},
    {ReliveTypes::eMudokon, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Mudokon")},
    {ReliveTypes::eLiftPoint, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Lift Point")},
    {ReliveTypes::eMeat, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Meat")},
    {ReliveTypes::eMeatSack, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Meat Sack")},
    {ReliveTypes::eMeatSaw, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Meat Saw")},
    {ReliveTypes::eMine, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Mine")},
    {ReliveTypes::eRollingBallStopperShaker, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Rolling Ball Stopper Shaker")},
    {ReliveTypes::eMotionDetector, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Motion Detector")},
    {ReliveTypes::eRollingBallStopper, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Rolling Ball Stopper")},
    {ReliveTypes::ePauseMenu, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Pause Menu")},
    {ReliveTypes::eParamite, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Paramite")},
    {ReliveTypes::eDemoPlayback, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Demo Playback")},
    {ReliveTypes::eBirdPortal, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Bird Portal")},
    {ReliveTypes::eBirdPortalTerminator, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Bird Portal Terminator")},
    {ReliveTypes::eFG1, QT_TRANSLATE_NOOP("ReflectedEnumValues", "FG1")},
    {ReliveTypes::eAbilityRing, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Ability Ring")},
    {ReliveTypes::eRock, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Rock")},
    {ReliveTypes::eRockSack, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Rock Sack")},
    {ReliveTypes::eRollingBall, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Rolling Ball")},
    {ReliveTypes::eRope, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Rope")},
    {ReliveTypes::eAirExplosion, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Air Explosion")},
    {ReliveTypes::eRedLaser, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Red Laser")},
    {ReliveTypes::eScrab, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Scrab")},
    {ReliveTypes::eScreenClipper, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Screen Clipper")},
    {ReliveTypes::eEffectBase, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Effect Base")},
    {ReliveTypes::eFade, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Fade")},
    {ReliveTypes::eFlash, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Flash")},
    {ReliveTypes::eScreenWave, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Screen Wave")},
    {ReliveTypes::eUnknown, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Unknown")},
    {ReliveTypes::eShrykull, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Shrykull")},
    {ReliveTypes::eSlig, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Slig")},
    {ReliveTypes::eSlog, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Slog")},
    {ReliveTypes::SlingMud, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Sling Mudokon")},
    {ReliveTypes::eSligSpawner, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Slig Spawner")},
    {ReliveTypes::eZBall, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Z Ball")},
    {ReliveTypes::eParticle, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Particle")},
    {ReliveTypes::eZapLine, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Zap Line")},
    {ReliveTypes::eBeeSwarm, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Bee Swarm")},
    {ReliveTypes::eBeeNest, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Bee Nest")},
    {ReliveTypes::eLever, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Lever")},
    {ReliveTypes::eTrapDoor, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Trap Door")},
    {ReliveTypes::eUXB, QT_TRANSLATE_NOOP("ReflectedEnumValues", "UXB")},
    {ReliveTypes::eMovie, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Movie")},
    {ReliveTypes::eCameraSwapper, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Camera Swapper")},
    {ReliveTypes::eElectrocute, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Electrocute")},
    {ReliveTypes::eTimedMine_or_MovingBomb, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Timed Mine Or Moviing Bomb")},
    {ReliveTypes::eBone, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Bone")},
    {ReliveTypes::eBoneBag, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Bone Bag")},
    {ReliveTypes::eBrewMachine, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Brew Machine")},
    {ReliveTypes::eSligButton, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Slig Button")},
    {ReliveTypes::eExplosionSet, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Explosion Set")},
    {ReliveTypes::eZapSpark, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Zap Spark")},
    {ReliveTypes::eMetal, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Metal")},
    {ReliveTypes::eMinesAlarm, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Mines Alarm")},
    {ReliveTypes::eCrawlingSlig, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Crawling Slig")},
    {ReliveTypes::eDrill, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Drill")},
    {ReliveTypes::eLaughingGas, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Laughing Gas")},
    {ReliveTypes::eDoorLock, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Door Lock")},
    {ReliveTypes::eDove, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Dove")},
    {ReliveTypes::eEvilFart, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Evil Fart")},
    {ReliveTypes::eFleech, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Fleech")},
    {ReliveTypes::ePossessionFlicker, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Possession Flicker")},
    {ReliveTypes::eFlyingSlig, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Flying Slig")},
    {ReliveTypes::eFlyingSligSpawner, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Flying Slig Spawner")},
    {ReliveTypes::eColourfulMeter, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Colourful Meter")},
    {ReliveTypes::eSlapLock_OrbWhirlWind, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Slap Lock Orb Whirl WInd")},
    {ReliveTypes::eGreeter, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Greeter")},
    {ReliveTypes::eGlukkon, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Glukkon")},
    {ReliveTypes::eHelpPhone, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Help Phone")},
    {ReliveTypes::eEyeOrbPart, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Eye Orb Part")},
    {ReliveTypes::eInvisibleEffect, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Invisible Effect")},
    {ReliveTypes::ePulley, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Pulley")},
    {ReliveTypes::eResourceManager, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Resource Manager")},
    {ReliveTypes::eSligGetPantsOrWings, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Slig Get Pants Or Wings")},
    {ReliveTypes::eRingOrLiftMud, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Ring Or Lift Mud")},
    {ReliveTypes::eSecurityOrb, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Security Orb")},
    {ReliveTypes::eText, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Text")},
    {ReliveTypes::eMineCar, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Mine Car")},
    {ReliveTypes::eGreeterBody, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Greeter Body")},
    {ReliveTypes::eMusicController, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Music Controller")},
    {ReliveTypes::eMusicTrigger, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Music Trigger")},
    {ReliveTypes::ePullRingRope, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Pull RIng Rope")},
    {ReliveTypes::eScrabSpawner, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Scrab Spawner")},
    {ReliveTypes::eMainMenuTransistion, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Main Menu Transition")},
    {ReliveTypes::eScreenShake, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Screen Shake")},
    {ReliveTypes::eSlamDoor, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Slam Door")},
    {ReliveTypes::eSnoozeParticle, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Snooze Particle")},
    {ReliveTypes::eSlurgSpawner, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Slurg Spawner")},
    {ReliveTypes::eSlurg, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Slurg")},
    {ReliveTypes::eTimerTrigger, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Timer Trigger")},
    {ReliveTypes::eTorturedMud, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Tortured Mud")},
    {ReliveTypes::eWebLine, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Web Line")},
    {ReliveTypes::eWell, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Well")},
    {ReliveTypes::eThrowableArray, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Throwable Array")},
})

REFLECT_ENUM(relive::reliveScale,
{
    {relive::reliveScale::eFull, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Full")},
    {relive::reliveScale::eHalf, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Half")}
})

REFLECT_ENUM(relive::Path_Mudokon::Mud_TLV_Emotion,
{
    {relive::Path_Mudokon::Mud_TLV_Emotion::eNormal, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Normal")},
    {relive::Path_Mudokon::Mud_TLV_Emotion::eAngry, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Angry")},
    {relive::Path_Mudokon::Mud_TLV_Emotion::eSad, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Sad")},
    {relive::Path_Mudokon::Mud_TLV_Emotion::eWired, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Wired")},
    {relive::Path_Mudokon::Mud_TLV_Emotion::eSick, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Sick")}
})

REFLECT_ENUM(relive::Path_BirdPortal::PortalSide,
{
    {relive::Path_BirdPortal::PortalSide::eRight, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Right")},
    {relive::Path_BirdPortal::PortalSide::eLeft, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Left")}
})

REFLECT_ENUM(relive::Path_FlyingSlig::SpawnDelayStates,
{
    {relive::Path_FlyingSlig::SpawnDelayStates::eMoveImmediately, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Move Immediately")},
    {relive::Path_FlyingSlig::SpawnDelayStates::eUseCustomSpawnMoveDelay, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Use Custom Spawn Move Delay")}
})

REFLECT_ENUM(relive::Path_Slig_Data::DeathMode,
{
    {relive::Path_Slig_Data::DeathMode::StandIdle, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Stand Idle")},
    {relive::Path_Slig_Data::DeathMode::FallForward, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Fall Forward")}
})

REFLECT_ENUM(relive::Path_Slig_Data::StartState,
{
    {relive::Path_Slig_Data::StartState::Listening, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Listening")},
    {relive::Path_Slig_Data::StartState::Patrol, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Patrol")},
    {relive::Path_Slig_Data::StartState::Sleeping, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Sleeping")},
    {relive::Path_Slig_Data::StartState::Chase, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Chase")},
    {relive::Path_Slig_Data::StartState::ChaseAndDisappear, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Chase And Disappear")},
    {relive::Path_Slig_Data::StartState::eFallingToChase, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Falling To Chase")},
    {relive::Path_Slig_Data::StartState::ListeningToGlukkon, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Listening To Glukkon")},
})

REFLECT_ENUM(relive::reliveXDirection,
{
    {relive::reliveXDirection::eLeft, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Left")},
    {relive::reliveXDirection::eRight, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Right")},
})

REFLECT_ENUM(relive::reliveSwitchOp,
{
    {relive::reliveSwitchOp::eSetTrue, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Set True")},
    {relive::reliveSwitchOp::eSetFalse, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Set False")},
    {relive::reliveSwitchOp::eToggle, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Toggle")},
    {relive::reliveSwitchOp::eIncrement, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Increment")},
    {relive::reliveSwitchOp::eDecrement, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Decrement")},
})

REFLECT_ENUM(eLineTypes,
{
    {eLineTypes::eFloor_0, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Floor")},
    {eLineTypes::eWallLeft_1, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Wall Left")},
    {eLineTypes::eWallRight_2, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Wall Right")},
    {eLineTypes::eCeiling_3, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Ceiling")},
    {eLineTypes::eBackgroundFloor_4, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Background Floor")},
    {eLineTypes::eBackgroundWallLeft_5, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Background Wall Left")},
    {eLineTypes::eBackgroundWallRight_6, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Background Wall Right")},
    {eLineTypes::eBackgroundCeiling_7, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Background Ceiling")},
    {eLineTypes::eTrackLine_8, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Track Line")},
    {eLineTypes::eArt_9, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Art")},
    {eLineTypes::eBulletWall_10, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Bullet Wall")},
    {eLineTypes::eMineCarFloor_11, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Mine Car Floor")},
    {eLineTypes::eMineCarWall_12, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Mine Car Wall")},
    {eLineTypes::eMineCarCeiling_13, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Mine Car Ceiling")},
    {eLineTypes::eBackgroundMineCarFloor_14, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Background Mine Car Floor")},
    {eLineTypes::eBackgroundMineCarWall_15, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Background Mine Car Wall")},
    {eLineTypes::eBackgroundMineCarCeiling_16, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Background Mine Car Ceiling")},
    {eLineTypes::eFlyingObjectWall_17, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Flying Slig Ceiling")},
    {eLineTypes::eBackgroundFlyingObjectWall_18, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Background Flying Slig Ceiling")},
    {eLineTypes::eDynamicCollision_32, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Dynamic Collision")},
    {eLineTypes::eBackgroundDynamicCollision_36, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Background Dynamic Collision")},
})


REFLECT_ENUM(EReliveLevelIds,
{
    {EReliveLevelIds::eNone, QT_TRANSLATE_NOOP("ReflectedEnumValues", "None")},
    {EReliveLevelIds::eMenu, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Menu")},
    {EReliveLevelIds::eMines, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Mines")},
    {EReliveLevelIds::eNecrum, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Necrum")},
    {EReliveLevelIds::eMudomoVault, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Mudomo Vault")},
    {EReliveLevelIds::eMudancheeVault, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Mudanchee Vault")},
    {EReliveLevelIds::eFeeCoDepot, QT_TRANSLATE_NOOP("ReflectedEnumValues", "FeeCo Depot")},
    {EReliveLevelIds::eBarracks, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Barracks")},
    {EReliveLevelIds::eMudancheeVault_Ender, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Mudanchee Vault Ender")},
    {EReliveLevelIds::eBonewerkz, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Bonewerkz")},
    {EReliveLevelIds::eBrewery, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Brewery")},
    {EReliveLevelIds::eBrewery_Ender, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Brewery Ender")},
    {EReliveLevelIds::eMudomoVault_Ender, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Mudomo Vault Ender")},
    {EReliveLevelIds::eFeeCoDepot_Ender, QT_TRANSLATE_NOOP("ReflectedEnumValues", "FeeCo Depot Ender")},
    {EReliveLevelIds::eBarracks_Ender, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Barracks Ender")},
    {EReliveLevelIds::eBonewerkz_Ender, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Bonewerkz Ender")},
    {EReliveLevelIds::eCredits, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Credits")},
    {EReliveLevelIds::eRuptureFarms, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Rupture Farms")},
    {EReliveLevelIds::eLines, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Lines")},
    {EReliveLevelIds::eForest, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Forest")},
    {EReliveLevelIds::eForestTemple, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Forest Temple")},
    {EReliveLevelIds::eStockYards, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Stock Yards")},
    {EReliveLevelIds::eStockYardsReturn, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Stock Yards Return")},
    {EReliveLevelIds::eDesert, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Desert")},
    {EReliveLevelIds::eDesertTemple, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Desert Temple")},
    {EReliveLevelIds::eBoardRoom, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Board Room")},
    {EReliveLevelIds::eRuptureFarmsReturn, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Rupture Farms Return")},
    {EReliveLevelIds::eForestChase, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Forest Chase")},
    {EReliveLevelIds::eDesertEscape, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Desert Escape")},
})

REFLECT_ENUM(relive::reliveScreenChangeEffects,
{
    {relive::reliveScreenChangeEffects::ePlay1FMV, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Play 1 FMV")},
    {relive::reliveScreenChangeEffects::eRightToLeft, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Right To Left")},
    {relive::reliveScreenChangeEffects::eLeftToRight, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Left To Right")},
    {relive::reliveScreenChangeEffects::eBottomToTop, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Bottom To Top")},
    {relive::reliveScreenChangeEffects::eTopToBottom, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Top To Bottom")},
    {relive::reliveScreenChangeEffects::eBoxOut, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Box Out")},
    {relive::reliveScreenChangeEffects::eVerticalSplit, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Vertical Split")},
    {relive::reliveScreenChangeEffects::eHorizontalSplit, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Horizontal Split")},
    {relive::reliveScreenChangeEffects::eUnknown_8, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Unknown8")},
    {relive::reliveScreenChangeEffects::eInstantChange, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Instant Change")}
})

REFLECT_ENUM(relive::Path_ElectricWall::ElectricWallStartState,
{
    {relive::Path_ElectricWall::ElectricWallStartState::eOff, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Off")},
    {relive::Path_ElectricWall::ElectricWallStartState::eOn, QT_TRANSLATE_NOOP("ReflectedEnumValues", "On")},
})

REFLECT_ENUM(relive::Path_Mudokon::MudJobs,
{
    {relive::Path_Mudokon::MudJobs::eChisle, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Chisle")},
    {relive::Path_Mudokon::MudJobs::eSitScrub, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Scrub")},
    {relive::Path_Mudokon::MudJobs::eAngryWorker, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Angry Worker")},
    {relive::Path_Mudokon::MudJobs::eDamageRingGiver, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Damage Ring Giver")},
    {relive::Path_Mudokon::MudJobs::eHealthRingGiver, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Health Ring Giver")},
})

REFLECT_ENUM(relive::Path_BirdPortal::PortalType,
{
    {relive::Path_BirdPortal::PortalType::eAbe, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Abe")},
    {relive::Path_BirdPortal::PortalType::eWorker, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Worker")},
    {relive::Path_BirdPortal::PortalType::eShrykull, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Shrykull")},
    {relive::Path_BirdPortal::PortalType::eMudTeleport, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Mud Teleport")},
})

REFLECT_ENUM(relive::Path_Door::DoorStates,
{
    {relive::Path_Door::DoorStates::eOpen, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Open")},
    {relive::Path_Door::DoorStates::eClosed, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Closed")},
    {relive::Path_Door::DoorStates::eOpening, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Opening")},
    {relive::Path_Door::DoorStates::eClosing, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Closing")}
})

REFLECT_ENUM(relive::Path_Door::DoorTypes,
{
    {relive::Path_Door::DoorTypes::eBasicDoor, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Basic Door")},
    {relive::Path_Door::DoorTypes::eTasksDoorWithSecretMusic, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Tasks Door With Secret Music")},
    {relive::Path_Door::DoorTypes::eTasksDoor, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Tasks Door")},
    {relive::Path_Door::DoorTypes::eTrialDoor, QT_TRANSLATE_NOOP("ReflectedEnumValues", "(AO) Trial Door")},
    {relive::Path_Door::DoorTypes::eHubDoor, QT_TRANSLATE_NOOP("ReflectedEnumValues", "(AO) Hub Door")}
})

REFLECT_ENUM(relive::Path_Lever::LeverSoundType,
{
    {relive::Path_Lever::LeverSoundType::eNone, QT_TRANSLATE_NOOP("ReflectedEnumValues", "None")},
    {relive::Path_Lever::LeverSoundType::eWell, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Well")},
    {relive::Path_Lever::LeverSoundType::eSwitchBellHammer, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Switch Bell Hammer")},
    {relive::Path_Lever::LeverSoundType::eDoor, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Door")},
    {relive::Path_Lever::LeverSoundType::eElectricWall, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Electric Wall")},
    {relive::Path_Lever::LeverSoundType::eSecurityOrb, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Security Orb")},
    {relive::Path_Lever::LeverSoundType::eLift, QT_TRANSLATE_NOOP("ReflectedEnumValues", "(AE) Lift")}
})

REFLECT_ENUM(relive::Path_Lever::LeverSoundDirection,
{
    {relive::Path_Lever::LeverSoundDirection::eLeftAndRight, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Left And Right")},
    {relive::Path_Lever::LeverSoundDirection::eLeft, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Left")},
    {relive::Path_Lever::LeverSoundDirection::eRight, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Right")},
})

REFLECT_ENUM(relive::Path_Hoist::Type,
{
    {relive::Path_Hoist::Type::eNextFloor, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Next Floor")},
    {relive::Path_Hoist::Type::eOffScreen, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Off Screen")},
})

REFLECT_ENUM(relive::Path_Hoist::GrabDirection,
{
    {relive::Path_Hoist::GrabDirection::eFacingLeft, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Facing Left")},
    {relive::Path_Hoist::GrabDirection::eFacingRight, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Facing Right")},
    {relive::Path_Hoist::GrabDirection::eFacingAnyDirection, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Facing Any Direction")},
})

REFLECT_ENUM(relive::Path_BoomMachine::NozzleSide,
{
    {relive::Path_BoomMachine::NozzleSide::eRight, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Right")},
    {relive::Path_BoomMachine::NozzleSide::eLeft, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Left")},
})

REFLECT_ENUM(relive::Path_EnemyStopper::StopDirection,
{
    {relive::Path_EnemyStopper::StopDirection::Left, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Left")},
    {relive::Path_EnemyStopper::StopDirection::Right, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Right")},
    {relive::Path_EnemyStopper::StopDirection::Both, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Both")},
})

REFLECT_ENUM(relive::Path_UXB::StartState,
{
    {relive::Path_UXB::StartState::eOn, QT_TRANSLATE_NOOP("ReflectedEnumValues", "On")},
    {relive::Path_UXB::StartState::eOff, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Off")},
})

REFLECT_ENUM(relive::Path_Edge::GrabDirection,
{
    {relive::Path_Edge::GrabDirection::eFacingLeft, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Facing Left")},
    {relive::Path_Edge::GrabDirection::eFacingRight, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Facing Right")},
    {relive::Path_Edge::GrabDirection::eFacingAnyDirection, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Facing Any Direction")},
})

REFLECT_ENUM(relive::Path_ShadowZone::Scale,
{
    {relive::Path_ShadowZone::Scale::eBoth, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Both")},
    {relive::Path_ShadowZone::Scale::eHalf, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Half")},
    {relive::Path_ShadowZone::Scale::eFull, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Full")},
})

REFLECT_ENUM(relive::Path_MusicTrigger::MusicTriggerMusicType,
{
    {relive::Path_MusicTrigger::MusicTriggerMusicType::eDrumAmbience, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Drum Ambience")},
    {relive::Path_MusicTrigger::MusicTriggerMusicType::eDeathDrumShort, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Death Drum Short")},
    {relive::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaLong, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Secret Area Long")},
    {relive::Path_MusicTrigger::MusicTriggerMusicType::eSoftChase, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Soft Chase")},
    {relive::Path_MusicTrigger::MusicTriggerMusicType::eIntenseChase, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Intense Chase")},
    {relive::Path_MusicTrigger::MusicTriggerMusicType::eChime, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Chime")},
    {relive::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaShort, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Secret Area Short")},
})

REFLECT_ENUM(relive::Path_MusicTrigger::TriggeredBy,
{
    {relive::Path_MusicTrigger::TriggeredBy::eTimer, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Timer")},
    {relive::Path_MusicTrigger::TriggeredBy::eTouching, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Touching")},
    {relive::Path_MusicTrigger::TriggeredBy::eSwitchId, QT_TRANSLATE_NOOP("ReflectedEnumValues", "(AO) Switch ID")},
    {relive::Path_MusicTrigger::TriggeredBy::eUnknown, QT_TRANSLATE_NOOP("ReflectedEnumValues", "(AO) Unknown")},
})

REFLECT_ENUM(relive::Path_InvisibleSwitch::InvisibleSwitchScale,
{
    {relive::Path_InvisibleSwitch::InvisibleSwitchScale::eHalf, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Half")},
    {relive::Path_InvisibleSwitch::InvisibleSwitchScale::eFull, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Full")},
    {relive::Path_InvisibleSwitch::InvisibleSwitchScale::eAny, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Any")},
})

REFLECT_ENUM(relive::Path_GasEmitter::GasColour,
{
    {relive::Path_GasEmitter::GasColour::eYellow, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Yellow")},
    {relive::Path_GasEmitter::GasColour::eRed, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Red")},
    {relive::Path_GasEmitter::GasColour::eGreen, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Green")},
    {relive::Path_GasEmitter::GasColour::eBlue, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Blue")},
    {relive::Path_GasEmitter::GasColour::eWhite, QT_TRANSLATE_NOOP("ReflectedEnumValues", "White")},
})

REFLECT_ENUM(::relive::Path_BackgroundAnimation::BgAnimSounds,
{
    {relive::Path_BackgroundAnimation::BgAnimSounds::eNone, QT_TRANSLATE_NOOP("ReflectedEnumValues", "None")},
    {relive::Path_BackgroundAnimation::BgAnimSounds::eFire, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Fire")},
})

REFLECT_ENUM(relive::Path_BackgroundAnimation::Layer,
{
    {relive::Path_BackgroundAnimation::Layer::eLayer0, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Layer 0")},
    {relive::Path_BackgroundAnimation::Layer::eLayer2, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Layer 2")},
})

REFLECT_ENUM(relive::TBlendModes,
{
    {relive::TBlendModes::eBlend_0, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Blend 0")},
    {relive::TBlendModes::eBlend_1, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Blend 1")},
    {relive::TBlendModes::eBlend_2, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Blend 2")},
    {relive::TBlendModes::eBlend_3, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Blend 3")},
})

REFLECT_ENUM(relive::Path_LiftPoint::LiftPointStopType,
{
    {relive::Path_LiftPoint::LiftPointStopType::eTopFloor, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Top Floor")},
    {relive::Path_LiftPoint::LiftPointStopType::eBottomFloor, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Bottom Floor")},
    {relive::Path_LiftPoint::LiftPointStopType::eMiddleFloor, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Middle Floor")},
    {relive::Path_LiftPoint::LiftPointStopType::eMiddleLockFloor, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Middle Lock Floor")},
    {relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Start Point Only")},
})

REFLECT_ENUM(relive::Path_PullRingRope::PullRingSwitchSound,
{
    {relive::Path_PullRingRope::PullRingSwitchSound::eNone, QT_TRANSLATE_NOOP("ReflectedEnumValues", "None")},
    {relive::Path_PullRingRope::PullRingSwitchSound::eWellExit, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Well Exit")},
    {relive::Path_PullRingRope::PullRingSwitchSound::eRingBellHammer, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Ring Unknown Trigger")},
    {relive::Path_PullRingRope::PullRingSwitchSound::eDoorEffect, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Door Effect")},
})

REFLECT_ENUM(relive::Path_PullRingRope::PullRingSoundDirection,
{
    {relive::Path_PullRingRope::PullRingSoundDirection::eLeftAndRight, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Left And Right")},
    {relive::Path_PullRingRope::PullRingSoundDirection::eLeft, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Left")},
    {relive::Path_PullRingRope::PullRingSoundDirection::eRight, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Right")},
})

REFLECT_ENUM(relive::Path_ContinuePoint::Scale,
{
    {relive::Path_ContinuePoint::Scale::eNone, QT_TRANSLATE_NOOP("ReflectedEnumValues", "None")},
    {relive::Path_ContinuePoint::Scale::eHalf, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Half")},
    {relive::Path_ContinuePoint::Scale::eFull, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Full")},
})

REFLECT_ENUM(relive::Path_ContinuePoint::spawnDirection,
{
    {relive::Path_ContinuePoint::spawnDirection::eRight, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Right")},
    {relive::Path_ContinuePoint::spawnDirection::eLeft, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Left")}
})

REFLECT_ENUM(relive::Path_WheelSyncer::OutputRequirement,
{
    {relive::Path_WheelSyncer::OutputRequirement::eAllOn, QT_TRANSLATE_NOOP("ReflectedEnumValues", "All On")},
    {relive::Path_WheelSyncer::OutputRequirement::e1OnAnd2Off, QT_TRANSLATE_NOOP("ReflectedEnumValues", "1 On And 2 Off")},
    {relive::Path_WheelSyncer::OutputRequirement::e1Or2On, QT_TRANSLATE_NOOP("ReflectedEnumValues", "1 Or 2 On")},
    {relive::Path_WheelSyncer::OutputRequirement::e1OnOr2Off, QT_TRANSLATE_NOOP("ReflectedEnumValues", "1 On Or 2 Off")},
})

REFLECT_ENUM(relive::Path_Drill::DrillBehavior,
{
    {relive::Path_Drill::DrillBehavior::eNotInteractable, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Not Interactable")},
    {relive::Path_Drill::DrillBehavior::eToggle, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Toggle")},
    {relive::Path_Drill::DrillBehavior::eUse, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Use")},
})

REFLECT_ENUM(relive::Path_Drill::DrillDirection,
{
    {relive::Path_Drill::DrillDirection::eDown, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Down")},
    {relive::Path_Drill::DrillDirection::eRight, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Right")},
    {relive::Path_Drill::DrillDirection::eLeft, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Left")},
})

REFLECT_ENUM(relive::Path_TrapDoor::StartState,
{
    {relive::Path_TrapDoor::StartState::eOpen, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Open")},
    {relive::Path_TrapDoor::StartState::eClosed, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Closed")}
})

REFLECT_ENUM(relive::Path_LiftMover::YDirection,
{
    {relive::Path_LiftMover::YDirection::eDown, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Down")},
    {relive::Path_LiftMover::YDirection::eUp, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Up")},
})

REFLECT_ENUM(relive::Path_MotionDetector::InitialMoveDirection,
{
    {relive::Path_MotionDetector::InitialMoveDirection::eRight, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Right")},
    {relive::Path_MotionDetector::InitialMoveDirection::eLeft, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Left")},
})

REFLECT_ENUM(relive::Path_DoorFlame::Colour,
{
    {relive::Path_DoorFlame::Colour::eDefault, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Default")},
    {relive::Path_DoorFlame::Colour::eRed, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Red")},
    {relive::Path_DoorFlame::Colour::eGreen, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Green")},
    {relive::Path_DoorFlame::Colour::eBlue, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Blue")},
})

REFLECT_ENUM(relive::Path_CrawlingSlig::StartState,
{
    {relive::Path_CrawlingSlig::StartState::eSleeping, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Sleeping")},
    {relive::Path_CrawlingSlig::StartState::eAwake, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Awake")},
})

REFLECT_ENUM(relive::Path_CrawlingSlig::CrawlDirection,
{
    {relive::Path_CrawlingSlig::CrawlDirection::eLeft, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Left")},
    {relive::Path_CrawlingSlig::CrawlDirection::eRight, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Right")},
    {relive::Path_CrawlingSlig::CrawlDirection::eRandom, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Random")},
})

// this probably shouldn't be here since the sound/sound direction implementation is extremely scuffed
// but maybe someone wants to use it anyway
REFLECT_ENUM(relive::Path_CrawlingSligButton::ButtonSounds,
{
    {relive::Path_CrawlingSligButton::ButtonSounds::None, QT_TRANSLATE_NOOP("ReflectedEnumValues", "None")},
    {relive::Path_CrawlingSligButton::ButtonSounds::SackHit1, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Sack Hit 1")},
    {relive::Path_CrawlingSligButton::ButtonSounds::SackHit2, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Sack Hit 2")},
    {relive::Path_CrawlingSligButton::ButtonSounds::FallingItemPresence2, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Falling Item Presence 2")},
    {relive::Path_CrawlingSligButton::ButtonSounds::SecurityOrb, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Security Orb")},
    {relive::Path_CrawlingSligButton::ButtonSounds::Bullet1, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Bullet 1")},
    {relive::Path_CrawlingSligButton::ButtonSounds::AbeGenericMovement, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Abe Generic Movement")},
})

REFLECT_ENUM(relive::Path_Glukkon::GlukkonTypes,
{
    {relive::Path_Glukkon::GlukkonTypes::eNormal, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Normal")},
    {relive::Path_Glukkon::GlukkonTypes::eStoryAslik, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Story Aslik")},
    {relive::Path_Glukkon::GlukkonTypes::eStoryDripik, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Story Dripik")},
    {relive::Path_Glukkon::GlukkonTypes::eStoryPhleg, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Story Phleg")},
})

REFLECT_ENUM(relive::Path_Glukkon::Facing,
{
    {relive::Path_Glukkon::Facing::eRight, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Right")},
    {relive::Path_Glukkon::Facing::eLeft, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Left")},
})

REFLECT_ENUM(relive::Path_Glukkon::SpawnType,
{
    {relive::Path_Glukkon::SpawnType::eRegularSpawn, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Regular Spawn")},
    {relive::Path_Glukkon::SpawnType::eFacingLeft, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Facing Left")},
    {relive::Path_Glukkon::SpawnType::eFacingRight, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Facing Right")},
    {relive::Path_Glukkon::SpawnType::eFullSpawnEffects, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Full Spawn Effects")},
})

REFLECT_ENUM(relive::Path_Glukkon::Behavior,
{
    {relive::Path_Glukkon::Behavior::eIgnoreWalls, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Ignore Walls")},
    {relive::Path_Glukkon::Behavior::eCheckForWalls, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Check For Walls")},
})

REFLECT_ENUM(relive::Path_FootSwitch::FootSwitchTriggerBy,
{
    {relive::Path_FootSwitch::FootSwitchTriggerBy::eAbe, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Abe")},
    {relive::Path_FootSwitch::FootSwitchTriggerBy::eAnyone, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Anyone")},
})

REFLECT_ENUM(relive::Path_SlogSpawner::StartDirection,
{
    {relive::Path_SlogSpawner::StartDirection::eRight, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Right")},
    {relive::Path_SlogSpawner::StartDirection::eLeft, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Left")},
})

REFLECT_ENUM(relive::Path_Scrab::ScrabPatrolType,
{
    {relive::Path_Scrab::ScrabPatrolType::eWalk, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Walk")},
    {relive::Path_Scrab::ScrabPatrolType::eRunOrWalk192, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Run Or Walk 192")},
    {relive::Path_Scrab::ScrabPatrolType::eRunOrWalk128, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Run Or Walk 128")},
    {relive::Path_Scrab::ScrabPatrolType::eRunOrWalk64, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Run Or Walk 64")},
    {relive::Path_Scrab::ScrabPatrolType::eRun, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Run")}
})

REFLECT_ENUM(relive::Path_ScrabSpawner::SpawnDirection,
{
    {relive::Path_ScrabSpawner::SpawnDirection::eNone, QT_TRANSLATE_NOOP("ReflectedEnumValues", "None")},
    {relive::Path_ScrabSpawner::SpawnDirection::eLeft, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Left")},
    {relive::Path_ScrabSpawner::SpawnDirection::eRight, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Right")},
})

REFLECT_ENUM(relive::Path_Paramite::EntranceType,
{
    {relive::Path_Paramite::EntranceType::ePatrol, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Patrol")},
    {relive::Path_Paramite::EntranceType::eSurpriseWeb, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Surprise Web")},
    {relive::Path_Paramite::EntranceType::eSlightlyHigherSpawnSurpriseWeb, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Slightly Higher Spawn Surprise Web")},
})

REFLECT_ENUM(relive::Path_BellsongStone::BellsongTypes,
{
    {relive::Path_BellsongStone::BellsongTypes::eWhistle, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Whistle")},
    {relive::Path_BellsongStone::BellsongTypes::eChimes, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Chimes")},
})

REFLECT_ENUM(relive::Path_LightEffect::Type,
{
    {relive::Path_LightEffect::Type::Star, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Star")},
    {relive::Path_LightEffect::Type::GoldGlow, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Gold Glow")},
    {relive::Path_LightEffect::Type::GreenGlow, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Green Glow")},
    {relive::Path_LightEffect::Type::FlintGlow, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Flint Glow")},
    {relive::Path_LightEffect::Type::Switchable_RedGreenDoorLights, QT_TRANSLATE_NOOP("ReflectedEnumValues", "RedGreen Door Light")},
    {relive::Path_LightEffect::Type::Switchable_RedGreenHubLight, QT_TRANSLATE_NOOP("ReflectedEnumValues", "RedGreen Hub Light")},
})

REFLECT_ENUM(relive::Path_RingMudokon::MustFaceMud,
{
    {relive::Path_RingMudokon::MustFaceMud::eYes, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Yes")},
    {relive::Path_RingMudokon::MustFaceMud::eNo, QT_TRANSLATE_NOOP("ReflectedEnumValues", "No")},
})

REFLECT_ENUM(relive::Path_MeatSaw::Type,
{
    {relive::Path_MeatSaw::Type::eAutomaticPersistOffscreen, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Automatic Persist Offscreen")},
    {relive::Path_MeatSaw::Type::eAutomatic, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Automatic")},
    {relive::Path_MeatSaw::Type::eSwitchId, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Switch ID")},
})

REFLECT_ENUM(relive::Path_MeatSaw::StartState,
{
    {relive::Path_MeatSaw::StartState::eOff, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Off")},
    {relive::Path_MeatSaw::StartState::eOn, QT_TRANSLATE_NOOP("ReflectedEnumValues", "On")},
})

REFLECT_ENUM(relive::Path_LiftMudokon::Direction,
{
    {relive::Path_LiftMudokon::Direction::eRight, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Right")},
    {relive::Path_LiftMudokon::Direction::eLeft, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Left")},
})

REFLECT_ENUM(relive::Path_BeeSwarmHole::MovementType,
{
    {relive::Path_BeeSwarmHole::MovementType::eHover, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Hover")},
    {relive::Path_BeeSwarmHole::MovementType::eAttack, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Attack")},
    {relive::Path_BeeSwarmHole::MovementType::eFollowPath, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Follow Path")},
})

REFLECT_ENUM(relive::Path_ZBall::StartPos,
{
    {relive::Path_ZBall::StartPos::eCenter, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Center")},
    {relive::Path_ZBall::StartPos::eOut, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Out")},
    {relive::Path_ZBall::StartPos::eIn, QT_TRANSLATE_NOOP("ReflectedEnumValues", "In")},
})

REFLECT_ENUM(relive::Path_ZBall::Speed,
{
    {relive::Path_ZBall::Speed::eNormal, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Normal")},
    {relive::Path_ZBall::Speed::eFast, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Fast")},
    {relive::Path_ZBall::Speed::eSlow, QT_TRANSLATE_NOOP("ReflectedEnumValues", "Slow")},
})

