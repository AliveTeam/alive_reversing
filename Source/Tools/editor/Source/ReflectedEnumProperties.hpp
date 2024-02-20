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
    {ReliveTypes::eNone, QT_TRANSLATE_NOOP("ReliveTypesEnum","None")},
    {ReliveTypes::eCrawlingSligButton, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Crawling Slig Button")},
    {ReliveTypes::eWheelSyncer, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Wheel Syncer")},
    {ReliveTypes::eDemoSpawnPoint, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Demo Spawn Point")},
    {ReliveTypes::eMultiSwitchController, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Multi Switch Controller")},
    {ReliveTypes::eStatusLight, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Status Light")},
    {ReliveTypes::eSlapLock, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Slap Lock")},
    {ReliveTypes::eParamiteWebLine, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Paramite Web Line")},
    {ReliveTypes::eGlukkonSwitch, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Glukkon Switch")},
    {ReliveTypes::eDoorBlocker, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Door Blocker")},
    {ReliveTypes::eTrainDoor, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Train Door")},
    {ReliveTypes::eLevelLoader, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Level Loader")},
    {ReliveTypes::eSligGetWings, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Slig Get Wings")},
    {ReliveTypes::eSligGetPants, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Slig Get Pants")},
    {ReliveTypes::eTeleporter, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Teleporter")},
    {ReliveTypes::eWater, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Water")},
    {ReliveTypes::eWorkWheel, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Work Wheel")},
    {ReliveTypes::eLCDScreen, QT_TRANSLATE_NOOP("ReliveTypesEnum", "LCD Screen")},
    {ReliveTypes::eInvisibleSwitch, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Invisible Switch")},
    {ReliveTypes::eDoorFlame, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Door Flame")},
    {ReliveTypes::eMovingBomb, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Moving Bomb")},
    {ReliveTypes::eMainMenuController, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Main Menu Controller")},
    {ReliveTypes::eHintFly, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Hint Fly")},
    {ReliveTypes::eSecurityDoor, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Security Door")},
    {ReliveTypes::eCreditsController, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Credits Controller")},
    {ReliveTypes::eLCDStatusBoard, QT_TRANSLATE_NOOP("ReliveTypesEnum", "LCD Status Board")},
    {ReliveTypes::eSwitchStateBooleanLogic, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Switch State Boolean Logic")},
    {ReliveTypes::eLightEffect, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Light Effect")},
    {ReliveTypes::eSlogSpawner, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Slog Spawner")},
    {ReliveTypes::eGasEmitter, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Gas Emitter")},
    {ReliveTypes::eRingCancel, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Ring Cancel")},
    {ReliveTypes::eElumWall, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Elum Wall")},
    {ReliveTypes::eAbeStart, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Abe Start")},
    {ReliveTypes::eBeeSwarmHole, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Bee Swarm Hole")},
    {ReliveTypes::eFallingItem, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Falling Item")},
    {ReliveTypes::eShadowZone, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Shadow Zone")},
    {ReliveTypes::eStartController, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Start Controller")},
    {ReliveTypes::eBirdPortalExit, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Bird Portal Exit")},
    {ReliveTypes::eHoneyDripTarget, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Honey Drip Target")},
    {ReliveTypes::ePathTransition, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Path Transition")},
    {ReliveTypes::eZSligCover, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Z Slig Cover")},
    {ReliveTypes::eResetPath, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Reset Path")},
    {ReliveTypes::eElumPathTrans, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Elum Path Trans")},
    {ReliveTypes::eScrabLeftBound, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Scrab Left Bound")},
    {ReliveTypes::eScrabRightBound, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Scrab Right Bound")},
    {ReliveTypes::eScrabNoFall, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Scrab No Fall")},
    {ReliveTypes::eMovingBombStopper, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Moving Bomb Stopper")},
    {ReliveTypes::eElumStart, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Elum Start")},
    {ReliveTypes::eEdge, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Edge")},
    {ReliveTypes::eSoftLanding, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Soft Landing")},
    {ReliveTypes::eMovieHandStone, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Movie Hand Stone")},
    {ReliveTypes::eBellSongStone, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Bell Song Stone")},
    {ReliveTypes::eDemoPlaybackStone, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Demo Playback Stone")},
    {ReliveTypes::eHandStone, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Hand Stone")},
    {ReliveTypes::eHoist, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Hoist")},
    {ReliveTypes::eContinuePoint, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Continue Point")},
    {ReliveTypes::eWellLocal, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Well Local")},
    {ReliveTypes::eWellExpress, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Well Express")},
    {ReliveTypes::eMudokonPathTrans, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Mudokon Path Trans")},
    {ReliveTypes::eRingMudokon, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Ring Mudokon")},
    {ReliveTypes::eLiftMudokon, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Lift Mudokon")},
    {ReliveTypes::eInvisibleZone, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Invisible Zone")},
    {ReliveTypes::eEnemyStopper, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Enemy Stopper")},
    {ReliveTypes::eSligBoundLeft, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Slig Bound Left")},
    {ReliveTypes::eSligBoundRight, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Slig Boung Right")},
    {ReliveTypes::eSligPersist, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Slig Persist")},
    {ReliveTypes::eZzzSpawner, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Zzz Spawner")},
    {ReliveTypes::eKillUnsavedMuds, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Kill Unsaved Muds")},
    {ReliveTypes::eDeathDrop, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Death Drop")},
    {ReliveTypes::eAlarm, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Alarm")},
    {ReliveTypes::eScreenManager, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Screen Manager")},
    {ReliveTypes::eBackgroundAnimation, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Background Animation")},
    {ReliveTypes::eBat, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Bat")},
    {ReliveTypes::eLiftMover, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Lift Mover")},
    {ReliveTypes::eTimedMine, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Timed Mine")},
    {ReliveTypes::eBullet, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Bullet")},
    {ReliveTypes::eDDCheat, QT_TRANSLATE_NOOP("ReliveTypesEnum", "DDCheat")},
    {ReliveTypes::eBells, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Bells")},
    {ReliveTypes::eChimeLock, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Chime Lock")},
    {ReliveTypes::eGasCountDown, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Gas Count Down")},
    {ReliveTypes::eParticleBurst, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Particle Burst")},
    {ReliveTypes::eDoor, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Door")},
    {ReliveTypes::eGameSpeak, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Game Speak")},
    {ReliveTypes::eElectricWall, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Electric Wall")},
    {ReliveTypes::eElum, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Elum")},
    {ReliveTypes::eBellHammer, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Bell Hammer")},
    {ReliveTypes::ePalOverwriter, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Pal Overwritter")},
    {ReliveTypes::eGroundExplosion, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Ground Explosion")},
    {ReliveTypes::eSecurityClaw, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Security Claw")},
    {ReliveTypes::eRockSpawner, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Rock Spawner")},
    {ReliveTypes::eFlintLockFire, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Flint Lock Fire")},
    {ReliveTypes::eThrowableTotalIndicator, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Throwable Total Indicator")},
    {ReliveTypes::eFootSwitch, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Foot Switch")},
    {ReliveTypes::eGameEnderController, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Game Ender Controller")},
    {ReliveTypes::eDeathBird, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Death Bird")},
    {ReliveTypes::eLoadingFile, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Loading File")},
    {ReliveTypes::eGrenade, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Grenade")},
    {ReliveTypes::eBoomMachine, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Boom Machine")},
    {ReliveTypes::eBackgroundGlukkon, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Background Glukkon")},
    {ReliveTypes::eAbe, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Abe")},
    {ReliveTypes::MainMenuFade, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Main Menu Fade")},
    {ReliveTypes::eHoneySack, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Honey Sack")},
    {ReliveTypes::eHoney, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Honey")},
    {ReliveTypes::eClawOrBirdPortalTerminator, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Claw OR Bird Portal Terminator")},
    {ReliveTypes::eMudokon, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Mudokon")},
    {ReliveTypes::eLiftPoint, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Lift Point")},
    {ReliveTypes::eMeat, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Meat")},
    {ReliveTypes::eMeatSack, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Meat Sack")},
    {ReliveTypes::eMeatSaw, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Meat Saw")},
    {ReliveTypes::eMine, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Mine")},
    {ReliveTypes::eRollingBallStopperShaker, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Rolling Ball Stopper Shaker")},
    {ReliveTypes::eMotionDetector, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Motion Detector")},
    {ReliveTypes::eRollingBallStopper, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Rolling Ball Stopper")},
    {ReliveTypes::ePauseMenu, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Pause Menu")},
    {ReliveTypes::eParamite, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Paramite")},
    {ReliveTypes::eDemoPlayback, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Demo Playback")},
    {ReliveTypes::eBirdPortal, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Bird Portal")},
    {ReliveTypes::eBirdPortalTerminator, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Bird Portal Terminator")},
    {ReliveTypes::eFG1, QT_TRANSLATE_NOOP("ReliveTypesEnum", "FG1")},
    {ReliveTypes::eAbilityRing, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Ability Ring")},
    {ReliveTypes::eRock, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Rock")},
    {ReliveTypes::eRockSack, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Rock Sack")},
    {ReliveTypes::eRollingBall, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Rolling Ball")},
    {ReliveTypes::eRope, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Rope")},
    {ReliveTypes::eAirExplosion, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Air Explosion")},
    {ReliveTypes::eRedLaser, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Red Laser")},
    {ReliveTypes::eScrab, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Scrab")},
    {ReliveTypes::eScreenClipper, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Screen Clipper")},
    {ReliveTypes::eEffectBase, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Effect Base")},
    {ReliveTypes::eFade, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Fade")},
    {ReliveTypes::eFlash, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Flash")},
    {ReliveTypes::eScreenWave, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Screen Wave")},
    {ReliveTypes::eUnknown, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Unknown")},
    {ReliveTypes::eShrykull, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Shrykull")},
    {ReliveTypes::eSlig, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Slig")},
    {ReliveTypes::eSlog, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Slog")},
    {ReliveTypes::SlingMud, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Sling Mudokon")},
    {ReliveTypes::eSligSpawner, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Slig Spawner")},
    {ReliveTypes::eZBall, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Z Ball")},
    {ReliveTypes::eParticle, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Particle")},
    {ReliveTypes::eZapLine, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Zap Line")},
    {ReliveTypes::eBeeSwarm, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Bee Swarm")},
    {ReliveTypes::eBeeNest, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Bee Nest")},
    {ReliveTypes::eLever, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Lever")},
    {ReliveTypes::eTrapDoor, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Trap Door")},
    {ReliveTypes::eUXB, QT_TRANSLATE_NOOP("ReliveTypesEnum", "UXB")},
    {ReliveTypes::eMovie, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Movie")},
    {ReliveTypes::eCameraSwapper, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Camera Swapper")},
    {ReliveTypes::eElectrocute, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Electrocute")},
    {ReliveTypes::eTimedMine_or_MovingBomb, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Timed Mine Or Moviing Bomb")},
    {ReliveTypes::eBone, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Bone")},
    {ReliveTypes::eBoneBag, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Bone Bag")},
    {ReliveTypes::eBrewMachine, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Brew Machine")},
    {ReliveTypes::eSligButton, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Slig Button")},
    {ReliveTypes::eExplosionSet, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Explosion Set")},
    {ReliveTypes::eZapSpark, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Zap Spark")},
    {ReliveTypes::eMetal, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Metal")},
    {ReliveTypes::eMinesAlarm, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Mines Alarm")},
    {ReliveTypes::eCrawlingSlig, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Crawling Slig")},
    {ReliveTypes::eDrill, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Drill")},
    {ReliveTypes::eLaughingGas, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Laughing Gas")},
    {ReliveTypes::eDoorLock, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Door Lock")},
    {ReliveTypes::eDove, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Dove")},
    {ReliveTypes::eEvilFart, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Evil Fart")},
    {ReliveTypes::eFleech, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Fleech")},
    {ReliveTypes::ePossessionFlicker, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Possession Flicker")},
    {ReliveTypes::eFlyingSlig, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Flying Slig")},
    {ReliveTypes::eFlyingSligSpawner, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Flying Slig Spawner")},
    {ReliveTypes::eColourfulMeter, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Colourful Meter")},
    {ReliveTypes::eSlapLock_OrbWhirlWind, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Slap Lock Orb Whirl WInd")},
    {ReliveTypes::eGreeter, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Greeter")},
    {ReliveTypes::eGlukkon, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Glukkon")},
    {ReliveTypes::eHelpPhone, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Help Phone")},
    {ReliveTypes::eEyeOrbPart, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Eye Orb Part")},
    {ReliveTypes::eInvisibleEffect, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Invisible Effect")},
    {ReliveTypes::ePulley, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Pulley")},
    {ReliveTypes::eResourceManager, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Resource Manager")},
    {ReliveTypes::eSligGetPantsOrWings, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Slig Get Pants Or Wings")},
    {ReliveTypes::eRingOrLiftMud, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Ring Or Lift Mud")},
    {ReliveTypes::eSecurityOrb, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Security Orb")},
    {ReliveTypes::eText, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Text")},
    {ReliveTypes::eMineCar, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Mine Car")},
    {ReliveTypes::eGreeterBody, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Greeter Body")},
    {ReliveTypes::eMusicController, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Music Controller")},
    {ReliveTypes::eMusicTrigger, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Music Trigger")},
    {ReliveTypes::ePullRingRope, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Pull RIng Rope")},
    {ReliveTypes::eScrabSpawner, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Scrab Spawner")},
    {ReliveTypes::eMainMenuTransistion, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Main Menu Transition")},
    {ReliveTypes::eScreenShake, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Screen Shake")},
    {ReliveTypes::eSlamDoor, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Slam Door")},
    {ReliveTypes::eSnoozeParticle, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Snooze Particle")},
    {ReliveTypes::eSlurgSpawner, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Slurg Spawner")},
    {ReliveTypes::eSlurg, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Slurg")},
    {ReliveTypes::eTimerTrigger, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Timer Trigger")},
    {ReliveTypes::eTorturedMud, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Tortured Mud")},
    {ReliveTypes::eWebLine, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Web Line")},
    {ReliveTypes::eWell, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Well")},
    {ReliveTypes::eThrowableArray, QT_TRANSLATE_NOOP("ReliveTypesEnum", "Throwable Array")},
})

REFLECT_ENUM(relive::reliveScale,
{
    {relive::reliveScale::eFull, QT_TRANSLATE_NOOP("reliveScaleEnum", "Full")},
    {relive::reliveScale::eHalf, QT_TRANSLATE_NOOP("reliveScaleEnum", "Half")}
})

REFLECT_ENUM(relive::Path_Mudokon::Mud_TLV_Emotion,
{
    {relive::Path_Mudokon::Mud_TLV_Emotion::eNormal, QT_TRANSLATE_NOOP("Path_Mudokon_Mud_TLV_EmotionEnum", "Normal")},
    {relive::Path_Mudokon::Mud_TLV_Emotion::eAngry, QT_TRANSLATE_NOOP("Path_Mudokon_Mud_TLV_EmotionEnum", "Angry")},
    {relive::Path_Mudokon::Mud_TLV_Emotion::eSad, QT_TRANSLATE_NOOP("Path_Mudokon_Mud_TLV_EmotionEnum", "Sad")},
    {relive::Path_Mudokon::Mud_TLV_Emotion::eWired, QT_TRANSLATE_NOOP("Path_Mudokon_Mud_TLV_EmotionEnum", "Wired")},
    {relive::Path_Mudokon::Mud_TLV_Emotion::eSick, QT_TRANSLATE_NOOP("Path_Mudokon_Mud_TLV_EmotionEnum", "Sick")}
})

REFLECT_ENUM(relive::Path_BirdPortal::PortalSide,
{
    {relive::Path_BirdPortal::PortalSide::eRight, QT_TRANSLATE_NOOP("Path_BirdPortal_PortalSideEnum", "Right")},
    {relive::Path_BirdPortal::PortalSide::eLeft, QT_TRANSLATE_NOOP("Path_BirdPortal_PortalSideEnum", "Left")}
})

REFLECT_ENUM(relive::Path_FlyingSlig::SpawnDelayStates,
{
    {relive::Path_FlyingSlig::SpawnDelayStates::eMoveImmediately, QT_TRANSLATE_NOOP("Path_FlyingSlig_SpawnDelayStatesEnum", "Move Immediately")},
    {relive::Path_FlyingSlig::SpawnDelayStates::eUseCustomSpawnMoveDelay, QT_TRANSLATE_NOOP("Path_FlyingSlig_SpawnDelayStatesEnum", "Use Custom Spawn Move Delay")}
})

REFLECT_ENUM(relive::Path_Slig_Data::DeathMode,
{
    {relive::Path_Slig_Data::DeathMode::StandIdle, QT_TRANSLATE_NOOP("Path_Slig_Data_DeathModeEnum", "Stand Idle")},
    {relive::Path_Slig_Data::DeathMode::FallForward, QT_TRANSLATE_NOOP("Path_Slig_Data_DeathModeEnum", "Fall Forward")}
})

REFLECT_ENUM(relive::Path_Slig_Data::StartState,
{
    {relive::Path_Slig_Data::StartState::Listening, QT_TRANSLATE_NOOP("Path_Slig_Data_StartStateEnum", "Listening")},
    {relive::Path_Slig_Data::StartState::Patrol, QT_TRANSLATE_NOOP("Path_Slig_Data_StartStateEnum", "Patrol")},
    {relive::Path_Slig_Data::StartState::Sleeping, QT_TRANSLATE_NOOP("Path_Slig_Data_StartStateEnum", "Sleeping")},
    {relive::Path_Slig_Data::StartState::Chase, QT_TRANSLATE_NOOP("Path_Slig_Data_StartStateEnum", "Chase")},
    {relive::Path_Slig_Data::StartState::ChaseAndDisappear, QT_TRANSLATE_NOOP("Path_Slig_Data_StartStateEnum", "Chase And Disappear")},
    {relive::Path_Slig_Data::StartState::eFallingToChase, QT_TRANSLATE_NOOP("Path_Slig_Data_StartStateEnum", "Falling To Chase")},
    {relive::Path_Slig_Data::StartState::ListeningToGlukkon, QT_TRANSLATE_NOOP("Path_Slig_Data_StartStateEnum", "Listening To Glukkon")},
})

REFLECT_ENUM(relive::reliveXDirection,
{
    {relive::reliveXDirection::eLeft, QT_TRANSLATE_NOOP("reliveXDirectionEnum", "Left")},
    {relive::reliveXDirection::eRight, QT_TRANSLATE_NOOP("reliveXDirectionEnum", "Right")},
})

REFLECT_ENUM(relive::reliveSwitchOp,
{
    {relive::reliveSwitchOp::eSetTrue, QT_TRANSLATE_NOOP("reliveSwitchOpEnum", "Set True")},
    {relive::reliveSwitchOp::eSetFalse, QT_TRANSLATE_NOOP("reliveSwitchOpEnum", "Set False")},
    {relive::reliveSwitchOp::eToggle, QT_TRANSLATE_NOOP("reliveSwitchOpEnum", "Toggle")},
    {relive::reliveSwitchOp::eIncrement, QT_TRANSLATE_NOOP("reliveSwitchOpEnum", "Increment")},
    {relive::reliveSwitchOp::eDecrement, QT_TRANSLATE_NOOP("reliveSwitchOpEnum", "Decrement")},
})

REFLECT_ENUM(eLineTypes,
{
    {eLineTypes::eFloor_0, QT_TRANSLATE_NOOP("eLineTypesEnum", "Floor")},
    {eLineTypes::eWallLeft_1, QT_TRANSLATE_NOOP("eLineTypesEnum", "Wall Left")},
    {eLineTypes::eWallRight_2, QT_TRANSLATE_NOOP("eLineTypesEnum", "Wall Right")},
    {eLineTypes::eCeiling_3, QT_TRANSLATE_NOOP("eLineTypesEnum", "Ceiling")},
    {eLineTypes::eBackgroundFloor_4, QT_TRANSLATE_NOOP("eLineTypesEnum", "Background Floor")},
    {eLineTypes::eBackgroundWallLeft_5, QT_TRANSLATE_NOOP("eLineTypesEnum", "Background Wall Left")},
    {eLineTypes::eBackgroundWallRight_6, QT_TRANSLATE_NOOP("eLineTypesEnum", "Background Wall Right")},
    {eLineTypes::eBackgroundCeiling_7, QT_TRANSLATE_NOOP("eLineTypesEnum", "Background Ceiling")},
    {eLineTypes::eTrackLine_8, QT_TRANSLATE_NOOP("eLineTypesEnum", "Track Line")},
    {eLineTypes::eArt_9, QT_TRANSLATE_NOOP("eLineTypesEnum", "Art")},
    {eLineTypes::eBulletWall_10, QT_TRANSLATE_NOOP("eLineTypesEnum", "Bullet Wall")},
    {eLineTypes::eMineCarFloor_11, QT_TRANSLATE_NOOP("eLineTypesEnum", "Mine Car Floor")},
    {eLineTypes::eMineCarWall_12, QT_TRANSLATE_NOOP("eLineTypesEnum", "Mine Car Wall")},
    {eLineTypes::eMineCarCeiling_13, QT_TRANSLATE_NOOP("eLineTypesEnum", "Mine Car Ceiling")},
    {eLineTypes::eBackgroundMineCarFloor_14, QT_TRANSLATE_NOOP("eLineTypesEnum", "Background Mine Car Floor")},
    {eLineTypes::eBackgroundMineCarWall_15, QT_TRANSLATE_NOOP("eLineTypesEnum", "Background Mine Car Wall")},
    {eLineTypes::eBackgroundMineCarCeiling_16, QT_TRANSLATE_NOOP("eLineTypesEnum", "Background Mine Car Ceiling")},
    {eLineTypes::eFlyingObjectWall_17, QT_TRANSLATE_NOOP("eLineTypesEnum", "Flying Slig Ceiling")},
    {eLineTypes::eBackgroundFlyingObjectWall_18, QT_TRANSLATE_NOOP("eLineTypesEnum", "Background Flying Slig Ceiling")},
    {eLineTypes::eDynamicCollision_32, QT_TRANSLATE_NOOP("eLineTypesEnum", "Dynamic Collision")},
    {eLineTypes::eBackgroundDynamicCollision_36, QT_TRANSLATE_NOOP("eLineTypesEnum", "Background Dynamic Collision")},
})


REFLECT_ENUM(EReliveLevelIds,
{
    {EReliveLevelIds::eNone, QT_TRANSLATE_NOOP("EReliveLevelIdsEnum", "None")},
    {EReliveLevelIds::eMenu, QT_TRANSLATE_NOOP("EReliveLevelIdsEnum", "Menu")},
    {EReliveLevelIds::eMines, QT_TRANSLATE_NOOP("EReliveLevelIdsEnum", "Mines")},
    {EReliveLevelIds::eNecrum, QT_TRANSLATE_NOOP("EReliveLevelIdsEnum", "Necrum")},
    {EReliveLevelIds::eMudomoVault, QT_TRANSLATE_NOOP("EReliveLevelIdsEnum", "Mudomo Vault")},
    {EReliveLevelIds::eMudancheeVault, QT_TRANSLATE_NOOP("EReliveLevelIdsEnum", "Mudanchee Vault")},
    {EReliveLevelIds::eFeeCoDepot, QT_TRANSLATE_NOOP("EReliveLevelIdsEnum", "FeeCo Depot")},
    {EReliveLevelIds::eBarracks, QT_TRANSLATE_NOOP("EReliveLevelIdsEnum", "Barracks")},
    {EReliveLevelIds::eMudancheeVault_Ender, QT_TRANSLATE_NOOP("EReliveLevelIdsEnum", "Mudanchee Vault Ender")},
    {EReliveLevelIds::eBonewerkz, QT_TRANSLATE_NOOP("EReliveLevelIdsEnum", "Bonewerkz")},
    {EReliveLevelIds::eBrewery, QT_TRANSLATE_NOOP("EReliveLevelIdsEnum", "Brewery")},
    {EReliveLevelIds::eBrewery_Ender, QT_TRANSLATE_NOOP("EReliveLevelIdsEnum", "Brewery Ender")},
    {EReliveLevelIds::eMudomoVault_Ender, QT_TRANSLATE_NOOP("EReliveLevelIdsEnum", "Mudomo Vault Ender")},
    {EReliveLevelIds::eFeeCoDepot_Ender, QT_TRANSLATE_NOOP("EReliveLevelIdsEnum", "FeeCo Depot Ender")},
    {EReliveLevelIds::eBarracks_Ender, QT_TRANSLATE_NOOP("EReliveLevelIdsEnum", "Barracks Ender")},
    {EReliveLevelIds::eBonewerkz_Ender, QT_TRANSLATE_NOOP("EReliveLevelIdsEnum", "Bonewerkz Ender")},
    {EReliveLevelIds::eCredits, QT_TRANSLATE_NOOP("EReliveLevelIdsEnum", "Credits")},
    {EReliveLevelIds::eRuptureFarms, QT_TRANSLATE_NOOP("EReliveLevelIdsEnum", "Rupture Farms")},
    {EReliveLevelIds::eLines, QT_TRANSLATE_NOOP("EReliveLevelIdsEnum", "Lines")},
    {EReliveLevelIds::eForest, QT_TRANSLATE_NOOP("EReliveLevelIdsEnum", "Forest")},
    {EReliveLevelIds::eForestTemple, QT_TRANSLATE_NOOP("EReliveLevelIdsEnum", "Forest Temple")},
    {EReliveLevelIds::eStockYards, QT_TRANSLATE_NOOP("EReliveLevelIdsEnum", "Stock Yards")},
    {EReliveLevelIds::eStockYardsReturn, QT_TRANSLATE_NOOP("EReliveLevelIdsEnum", "Stock Yards Return")},
    {EReliveLevelIds::eDesert, QT_TRANSLATE_NOOP("EReliveLevelIdsEnum", "Desert")},
    {EReliveLevelIds::eDesertTemple, QT_TRANSLATE_NOOP("EReliveLevelIdsEnum", "Desert Temple")},
    {EReliveLevelIds::eBoardRoom, QT_TRANSLATE_NOOP("EReliveLevelIdsEnum", "Board Room")},
    {EReliveLevelIds::eRuptureFarmsReturn, QT_TRANSLATE_NOOP("EReliveLevelIdsEnum", "Rupture Farms Return")},
    {EReliveLevelIds::eForestChase, QT_TRANSLATE_NOOP("EReliveLevelIdsEnum", "Forest Chase")},
    {EReliveLevelIds::eDesertEscape, QT_TRANSLATE_NOOP("EReliveLevelIdsEnum", "Desert Escape")},
})

REFLECT_ENUM(relive::reliveScreenChangeEffects,
{
    {relive::reliveScreenChangeEffects::ePlay1FMV, QT_TRANSLATE_NOOP("reliveScreenChangeEffectsEnum", "Play 1 FMV")},
    {relive::reliveScreenChangeEffects::eRightToLeft, QT_TRANSLATE_NOOP("reliveScreenChangeEffectsEnum", "Right To Left")},
    {relive::reliveScreenChangeEffects::eLeftToRight, QT_TRANSLATE_NOOP("reliveScreenChangeEffectsEnum", "Left To Right")},
    {relive::reliveScreenChangeEffects::eBottomToTop, QT_TRANSLATE_NOOP("reliveScreenChangeEffectsEnum", "Bottom To Top")},
    {relive::reliveScreenChangeEffects::eTopToBottom, QT_TRANSLATE_NOOP("reliveScreenChangeEffectsEnum", "Top To Bottom")},
    {relive::reliveScreenChangeEffects::eBoxOut, QT_TRANSLATE_NOOP("reliveScreenChangeEffectsEnum", "Box Out")},
    {relive::reliveScreenChangeEffects::eVerticalSplit, QT_TRANSLATE_NOOP("reliveScreenChangeEffectsEnum", "Vertical Split")},
    {relive::reliveScreenChangeEffects::eHorizontalSplit, QT_TRANSLATE_NOOP("reliveScreenChangeEffectsEnum", "Horizontal Split")},
    {relive::reliveScreenChangeEffects::eUnknown_8, QT_TRANSLATE_NOOP("reliveScreenChangeEffectsEnum", "Unknown8")},
    {relive::reliveScreenChangeEffects::eInstantChange, QT_TRANSLATE_NOOP("reliveScreenChangeEffectsEnum", "Instant Change")}
})

REFLECT_ENUM(relive::Path_ElectricWall::ElectricWallStartState,
{
    {relive::Path_ElectricWall::ElectricWallStartState::eOff, QT_TRANSLATE_NOOP("Path_ElectricWall_ElectricWallStartStateEnum", "Off")},
    {relive::Path_ElectricWall::ElectricWallStartState::eOn, QT_TRANSLATE_NOOP("Path_ElectricWall_ElectricWallStartStateEnum", "On")},
})

REFLECT_ENUM(relive::Path_Mudokon::MudJobs,
{
    {relive::Path_Mudokon::MudJobs::eChisle, QT_TRANSLATE_NOOP("Path_Mudokon_MudJobsEnum", "Chisle")},
    {relive::Path_Mudokon::MudJobs::eSitScrub, QT_TRANSLATE_NOOP("Path_Mudokon_MudJobsEnum", "Scrub")},
    {relive::Path_Mudokon::MudJobs::eAngryWorker, QT_TRANSLATE_NOOP("Path_Mudokon_MudJobsEnum", "Angry Worker")},
    {relive::Path_Mudokon::MudJobs::eDamageRingGiver, QT_TRANSLATE_NOOP("Path_Mudokon_MudJobsEnum", "Damage Ring Giver")},
    {relive::Path_Mudokon::MudJobs::eHealthRingGiver, QT_TRANSLATE_NOOP("Path_Mudokon_MudJobsEnum", "Health Ring Giver")},
})

REFLECT_ENUM(relive::Path_BirdPortal::PortalType,
{
    {relive::Path_BirdPortal::PortalType::eAbe, QT_TRANSLATE_NOOP("Path_BirdPortal_PortalTypeEnum", "Abe")},
    {relive::Path_BirdPortal::PortalType::eWorker, QT_TRANSLATE_NOOP("Path_BirdPortal_PortalTypeEnum", "Worker")},
    {relive::Path_BirdPortal::PortalType::eShrykull, QT_TRANSLATE_NOOP("Path_BirdPortal_PortalTypeEnum", "Shrykull")},
    {relive::Path_BirdPortal::PortalType::eMudTeleport, QT_TRANSLATE_NOOP("Path_BirdPortal_PortalTypeEnum", "Mud Teleport")},
})

REFLECT_ENUM(relive::Path_Door::DoorStates,
{
    {relive::Path_Door::DoorStates::eOpen, QT_TRANSLATE_NOOP("Path_Door_DoorStatesEnum", "Open")},
    {relive::Path_Door::DoorStates::eClosed, QT_TRANSLATE_NOOP("Path_Door_DoorStatesEnum", "Closed")},
    {relive::Path_Door::DoorStates::eOpening, QT_TRANSLATE_NOOP("Path_Door_DoorStatesEnum", "Opening")},
    {relive::Path_Door::DoorStates::eClosing, QT_TRANSLATE_NOOP("Path_Door_DoorStatesEnum", "Closing")}
})

REFLECT_ENUM(relive::Path_Door::DoorTypes,
{
    {relive::Path_Door::DoorTypes::eBasicDoor, QT_TRANSLATE_NOOP("Path_Door_DoorTypesEnum", "Basic Door")},
    {relive::Path_Door::DoorTypes::eTasksDoorWithSecretMusic, QT_TRANSLATE_NOOP("Path_Door_DoorTypesEnum", "Tasks Door With Secret Music")},
    {relive::Path_Door::DoorTypes::eTasksDoor, QT_TRANSLATE_NOOP("Path_Door_DoorTypesEnum", "Tasks Door")},
    {relive::Path_Door::DoorTypes::eTrialDoor, QT_TRANSLATE_NOOP("Path_Door_DoorTypesEnum", "(AO) Trial Door")},
    {relive::Path_Door::DoorTypes::eHubDoor, QT_TRANSLATE_NOOP("Path_Door_DoorTypesEnum", "(AO) Hub Door")}
})

REFLECT_ENUM(relive::Path_Lever::LeverSoundType,
{
    {relive::Path_Lever::LeverSoundType::eNone, QT_TRANSLATE_NOOP("Path_Lever_LeverSoundTypeEnum", "None")},
    {relive::Path_Lever::LeverSoundType::eWell, QT_TRANSLATE_NOOP("Path_Lever_LeverSoundTypeEnum", "Well")},
    {relive::Path_Lever::LeverSoundType::eSwitchBellHammer, QT_TRANSLATE_NOOP("Path_Lever_LeverSoundTypeEnum", "Switch Bell Hammer")},
    {relive::Path_Lever::LeverSoundType::eDoor, QT_TRANSLATE_NOOP("Path_Lever_LeverSoundTypeEnum", "Door")},
    {relive::Path_Lever::LeverSoundType::eElectricWall, QT_TRANSLATE_NOOP("Path_Lever_LeverSoundTypeEnum", "Electric Wall")},
    {relive::Path_Lever::LeverSoundType::eSecurityOrb, QT_TRANSLATE_NOOP("Path_Lever_LeverSoundTypeEnum", "Security Orb")},
    {relive::Path_Lever::LeverSoundType::eLift, QT_TRANSLATE_NOOP("Path_Lever_LeverSoundTypeEnum", "(AE) Lift")}
})

REFLECT_ENUM(relive::Path_Lever::LeverSoundDirection,
{
    {relive::Path_Lever::LeverSoundDirection::eLeftAndRight, QT_TRANSLATE_NOOP("Path_Lever_LeverSoundDirectionEnum", "Left And Right")},
    {relive::Path_Lever::LeverSoundDirection::eLeft, QT_TRANSLATE_NOOP("Path_Lever_LeverSoundDirectionEnum", "Left")},
    {relive::Path_Lever::LeverSoundDirection::eRight, QT_TRANSLATE_NOOP("Path_Lever_LeverSoundDirectionEnum", "Right")},
})

REFLECT_ENUM(relive::Path_Hoist::Type,
{
    {relive::Path_Hoist::Type::eNextFloor, QT_TRANSLATE_NOOP("Path_Hoist_TypeEnum", "Next Floor")},
    {relive::Path_Hoist::Type::eOffScreen, QT_TRANSLATE_NOOP("Path_Hoist_TypeEnum", "Off Screen")},
})

REFLECT_ENUM(relive::Path_Hoist::GrabDirection,
{
    {relive::Path_Hoist::GrabDirection::eFacingLeft, QT_TRANSLATE_NOOP("Path_Hoist_GrabDirectionEnum", "Facing Left")},
    {relive::Path_Hoist::GrabDirection::eFacingRight, QT_TRANSLATE_NOOP("Path_Hoist_GrabDirectionEnum", "Facing Right")},
    {relive::Path_Hoist::GrabDirection::eFacingAnyDirection, QT_TRANSLATE_NOOP("Path_Hoist_GrabDirectionEnum", "Facing Any Direction")},
})

REFLECT_ENUM(relive::Path_BoomMachine::NozzleSide,
{
    {relive::Path_BoomMachine::NozzleSide::eRight, QT_TRANSLATE_NOOP("Path_BoomMachine_NozzleSideEnum", "Right")},
    {relive::Path_BoomMachine::NozzleSide::eLeft, QT_TRANSLATE_NOOP("Path_BoomMachine_NozzleSideEnum", "Left")},
})

REFLECT_ENUM(relive::Path_EnemyStopper::StopDirection,
{
    {relive::Path_EnemyStopper::StopDirection::Left, QT_TRANSLATE_NOOP("Path_EnemyStopper_StopDirectionEnum", "Left")},
    {relive::Path_EnemyStopper::StopDirection::Right, QT_TRANSLATE_NOOP("Path_EnemyStopper_StopDirectionEnum", "Right")},
    {relive::Path_EnemyStopper::StopDirection::Both, QT_TRANSLATE_NOOP("Path_EnemyStopper_StopDirectionEnum", "Both")},
})

REFLECT_ENUM(relive::Path_UXB::StartState,
{
    {relive::Path_UXB::StartState::eOn, QT_TRANSLATE_NOOP("Path_UXB_StartStateEnum", "On")},
    {relive::Path_UXB::StartState::eOff, QT_TRANSLATE_NOOP("Path_UXB_StartStateEnum", "Off")},
})

REFLECT_ENUM(relive::Path_Edge::GrabDirection,
{
    {relive::Path_Edge::GrabDirection::eFacingLeft, QT_TRANSLATE_NOOP("Path_Edge_GrabDirectionEnum", "Facing Left")},
    {relive::Path_Edge::GrabDirection::eFacingRight, QT_TRANSLATE_NOOP("Path_Edge_GrabDirectionEnum", "Facing Right")},
    {relive::Path_Edge::GrabDirection::eFacingAnyDirection, QT_TRANSLATE_NOOP("Path_Edge_GrabDirectionEnum", "Facing Any Direction")},
})

REFLECT_ENUM(relive::Path_ShadowZone::Scale,
{
    {relive::Path_ShadowZone::Scale::eBoth, QT_TRANSLATE_NOOP("Path_ShadowZone_ScaleEnum", "Both")},
    {relive::Path_ShadowZone::Scale::eHalf, QT_TRANSLATE_NOOP("Path_ShadowZone_ScaleEnum", "Half")},
    {relive::Path_ShadowZone::Scale::eFull, QT_TRANSLATE_NOOP("Path_ShadowZone_ScaleEnum", "Full")},
})

REFLECT_ENUM(relive::Path_MusicTrigger::MusicTriggerMusicType,
{
    {relive::Path_MusicTrigger::MusicTriggerMusicType::eDrumAmbience, QT_TRANSLATE_NOOP("Path_MusicTrigger_MusicTriggerMusicTypeEnum", "Drum Ambience")},
    {relive::Path_MusicTrigger::MusicTriggerMusicType::eDeathDrumShort, QT_TRANSLATE_NOOP("Path_MusicTrigger_MusicTriggerMusicTypeEnum", "Death Drum Short")},
    {relive::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaLong, QT_TRANSLATE_NOOP("Path_MusicTrigger_MusicTriggerMusicTypeEnum", "Secret Area Long")},
    {relive::Path_MusicTrigger::MusicTriggerMusicType::eSoftChase, QT_TRANSLATE_NOOP("Path_MusicTrigger_MusicTriggerMusicTypeEnum", "Soft Chase")},
    {relive::Path_MusicTrigger::MusicTriggerMusicType::eIntenseChase, QT_TRANSLATE_NOOP("Path_MusicTrigger_MusicTriggerMusicTypeEnum", "Intense Chase")},
    {relive::Path_MusicTrigger::MusicTriggerMusicType::eChime, QT_TRANSLATE_NOOP("Path_MusicTrigger_MusicTriggerMusicTypeEnum", "Chime")},
    {relive::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaShort, QT_TRANSLATE_NOOP("Path_MusicTrigger_MusicTriggerMusicTypeEnum", "Secret Area Short")},
})

REFLECT_ENUM(relive::Path_MusicTrigger::TriggeredBy,
{
    {relive::Path_MusicTrigger::TriggeredBy::eTimer, QT_TRANSLATE_NOOP("Path_MusicTrigger_TriggeredByEnum", "Timer")},
    {relive::Path_MusicTrigger::TriggeredBy::eTouching, QT_TRANSLATE_NOOP("Path_MusicTrigger_TriggeredByEnum", "Touching")},
    {relive::Path_MusicTrigger::TriggeredBy::eSwitchId, QT_TRANSLATE_NOOP("Path_MusicTrigger_TriggeredByEnum", "(AO) Switch ID")},
    {relive::Path_MusicTrigger::TriggeredBy::eUnknown, QT_TRANSLATE_NOOP("Path_MusicTrigger_TriggeredByEnum", "(AO) Unknown")},
})

REFLECT_ENUM(relive::Path_InvisibleSwitch::InvisibleSwitchScale,
{
    {relive::Path_InvisibleSwitch::InvisibleSwitchScale::eHalf, QT_TRANSLATE_NOOP("Path_InvisibleSwitch_InvisibleSwitchScaleEnum", "Half")},
    {relive::Path_InvisibleSwitch::InvisibleSwitchScale::eFull, QT_TRANSLATE_NOOP("Path_InvisibleSwitch_InvisibleSwitchScaleEnum", "Full")},
    {relive::Path_InvisibleSwitch::InvisibleSwitchScale::eAny, QT_TRANSLATE_NOOP("Path_InvisibleSwitch_InvisibleSwitchScaleEnum", "Any")},
})

REFLECT_ENUM(relive::Path_GasEmitter::GasColour,
{
    {relive::Path_GasEmitter::GasColour::eYellow, QT_TRANSLATE_NOOP("Path_GasEmitter_GasColourEnum", "Yellow")},
    {relive::Path_GasEmitter::GasColour::eRed, QT_TRANSLATE_NOOP("Path_GasEmitter_GasColourEnum", "Red")},
    {relive::Path_GasEmitter::GasColour::eGreen, QT_TRANSLATE_NOOP("Path_GasEmitter_GasColourEnum", "Green")},
    {relive::Path_GasEmitter::GasColour::eBlue, QT_TRANSLATE_NOOP("Path_GasEmitter_GasColourEnum", "Blue")},
    {relive::Path_GasEmitter::GasColour::eWhite, QT_TRANSLATE_NOOP("Path_GasEmitter_GasColourEnum", "White")},
})

REFLECT_ENUM(::relive::Path_BackgroundAnimation::BgAnimSounds,
{
    {relive::Path_BackgroundAnimation::BgAnimSounds::eNone, QT_TRANSLATE_NOOP("Path_BackgroundAnimation_BgAnimSoundsEnum", "None")},
    {relive::Path_BackgroundAnimation::BgAnimSounds::eFire, QT_TRANSLATE_NOOP("Path_BackgroundAnimation_BgAnimSoundsEnum", "Fire")},
})

REFLECT_ENUM(relive::Path_BackgroundAnimation::Layer,
{
    {relive::Path_BackgroundAnimation::Layer::eLayer0, QT_TRANSLATE_NOOP("Path_BackgroundAnimation_LayerEnum", "Layer 0")},
    {relive::Path_BackgroundAnimation::Layer::eLayer2, QT_TRANSLATE_NOOP("Path_BackgroundAnimation_LayerEnum", "Layer 2")},
})

REFLECT_ENUM(relive::TBlendModes,
{
    {relive::TBlendModes::eBlend_0, QT_TRANSLATE_NOOP("TBlendModesEnum", "Blend 0")},
    {relive::TBlendModes::eBlend_1, QT_TRANSLATE_NOOP("TBlendModesEnum", "Blend 1")},
    {relive::TBlendModes::eBlend_2, QT_TRANSLATE_NOOP("TBlendModesEnum", "Blend 2")},
    {relive::TBlendModes::eBlend_3, QT_TRANSLATE_NOOP("TBlendModesEnum", "Blend 3")},
})

REFLECT_ENUM(relive::Path_LiftPoint::LiftPointStopType,
{
    {relive::Path_LiftPoint::LiftPointStopType::eTopFloor, QT_TRANSLATE_NOOP("Path_LiftPoint_LiftPointStopTypeEnum", "Top Floor")},
    {relive::Path_LiftPoint::LiftPointStopType::eBottomFloor, QT_TRANSLATE_NOOP("Path_LiftPoint_LiftPointStopTypeEnum", "Bottom Floor")},
    {relive::Path_LiftPoint::LiftPointStopType::eMiddleFloor, QT_TRANSLATE_NOOP("Path_LiftPoint_LiftPointStopTypeEnum", "Middle Floor")},
    {relive::Path_LiftPoint::LiftPointStopType::eMiddleLockFloor, QT_TRANSLATE_NOOP("Path_LiftPoint_LiftPointStopTypeEnum", "Middle Lock Floor")},
    {relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly, QT_TRANSLATE_NOOP("Path_LiftPoint_LiftPointStopTypeEnum", "Start Point Only")},
})

REFLECT_ENUM(relive::Path_PullRingRope::PullRingSwitchSound,
{
    {relive::Path_PullRingRope::PullRingSwitchSound::eNone, QT_TRANSLATE_NOOP("Path_PullRingRope_PullRingSwitchSoundEnum", "None")},
    {relive::Path_PullRingRope::PullRingSwitchSound::eWellExit, QT_TRANSLATE_NOOP("Path_PullRingRope_PullRingSwitchSoundEnum", "Well Exit")},
    {relive::Path_PullRingRope::PullRingSwitchSound::eRingBellHammer, QT_TRANSLATE_NOOP("Path_PullRingRope_PullRingSwitchSoundEnum", "Ring Unknown Trigger")},
    {relive::Path_PullRingRope::PullRingSwitchSound::eDoorEffect, QT_TRANSLATE_NOOP("Path_PullRingRope_PullRingSwitchSoundEnum", "Door Effect")},
})

REFLECT_ENUM(relive::Path_PullRingRope::PullRingSoundDirection,
{
    {relive::Path_PullRingRope::PullRingSoundDirection::eLeftAndRight, QT_TRANSLATE_NOOP("Path_PullRingRope_PullRingSoundDirectionEnum", "Left And Right")},
    {relive::Path_PullRingRope::PullRingSoundDirection::eLeft, QT_TRANSLATE_NOOP("Path_PullRingRope_PullRingSoundDirectionEnum", "Left")},
    {relive::Path_PullRingRope::PullRingSoundDirection::eRight, QT_TRANSLATE_NOOP("Path_PullRingRope_PullRingSoundDirectionEnum", "Right")},
})

REFLECT_ENUM(relive::Path_ContinuePoint::Scale,
{
    {relive::Path_ContinuePoint::Scale::eNone, QT_TRANSLATE_NOOP("Path_ContinuePoint_ScaleEnum", "None")},
    {relive::Path_ContinuePoint::Scale::eHalf, QT_TRANSLATE_NOOP("Path_ContinuePoint_ScaleEnum", "Half")},
    {relive::Path_ContinuePoint::Scale::eFull, QT_TRANSLATE_NOOP("Path_ContinuePoint_ScaleEnum", "Full")},
})

REFLECT_ENUM(relive::Path_ContinuePoint::spawnDirection,
{
    {relive::Path_ContinuePoint::spawnDirection::eRight, QT_TRANSLATE_NOOP("Path_ContinuePoint_spawnDirectionEnum", "Right")},
    {relive::Path_ContinuePoint::spawnDirection::eLeft, QT_TRANSLATE_NOOP("Path_ContinuePoint_spawnDirectionEnum", "Left")}
})

REFLECT_ENUM(relive::Path_WheelSyncer::OutputRequirement,
{
    {relive::Path_WheelSyncer::OutputRequirement::eAllOn, QT_TRANSLATE_NOOP("Path_WheelSyncer_OutputRequirementEnum", "All On")},
    {relive::Path_WheelSyncer::OutputRequirement::e1OnAnd2Off, QT_TRANSLATE_NOOP("Path_WheelSyncer_OutputRequirementEnum", "1 On And 2 Off")},
    {relive::Path_WheelSyncer::OutputRequirement::e1Or2On, QT_TRANSLATE_NOOP("Path_WheelSyncer_OutputRequirementEnum", "1 Or 2 On")},
    {relive::Path_WheelSyncer::OutputRequirement::e1OnOr2Off, QT_TRANSLATE_NOOP("Path_WheelSyncer_OutputRequirementEnum", "1 On Or 2 Off")},
})

REFLECT_ENUM(relive::Path_Drill::DrillBehavior,
{
    {relive::Path_Drill::DrillBehavior::eNotInteractable, QT_TRANSLATE_NOOP("Path_Drill_DrillBehaviorEnum", "Not Interactable")},
    {relive::Path_Drill::DrillBehavior::eToggle, QT_TRANSLATE_NOOP("Path_Drill_DrillBehaviorEnum", "Toggle")},
    {relive::Path_Drill::DrillBehavior::eUse, QT_TRANSLATE_NOOP("Path_Drill_DrillBehaviorEnum", "Use")},
})

REFLECT_ENUM(relive::Path_Drill::DrillDirection,
{
    {relive::Path_Drill::DrillDirection::eDown, QT_TRANSLATE_NOOP("Path_Drill_DrillDirectionEnum", "Down")},
    {relive::Path_Drill::DrillDirection::eRight, QT_TRANSLATE_NOOP("Path_Drill_DrillDirectionEnum", "Right")},
    {relive::Path_Drill::DrillDirection::eLeft, QT_TRANSLATE_NOOP("Path_Drill_DrillDirectionEnum", "Left")},
})

REFLECT_ENUM(relive::Path_TrapDoor::StartState,
{
    {relive::Path_TrapDoor::StartState::eOpen, QT_TRANSLATE_NOOP("Path_TrapDoor_StartStateEnum", "Open")},
    {relive::Path_TrapDoor::StartState::eClosed, QT_TRANSLATE_NOOP("Path_TrapDoor_StartStateEnum", "Closed")}
})

REFLECT_ENUM(relive::Path_LiftMover::YDirection,
{
    {relive::Path_LiftMover::YDirection::eDown, QT_TRANSLATE_NOOP("Path_LiftMover_YDirectionEnum", "Down")},
    {relive::Path_LiftMover::YDirection::eUp, QT_TRANSLATE_NOOP("Path_LiftMover_YDirectionEnum", "Up")},
})

REFLECT_ENUM(relive::Path_MotionDetector::InitialMoveDirection,
{
    {relive::Path_MotionDetector::InitialMoveDirection::eRight, QT_TRANSLATE_NOOP("Path_MotionDetector_InitialMoveDirectionEnum", "Right")},
    {relive::Path_MotionDetector::InitialMoveDirection::eLeft, QT_TRANSLATE_NOOP("Path_MotionDetector_InitialMoveDirectionEnum", "Left")},
})

REFLECT_ENUM(relive::Path_DoorFlame::Colour,
{
    {relive::Path_DoorFlame::Colour::eDefault, QT_TRANSLATE_NOOP("Path_DoorFlame_ColourEnum", "Default")},
    {relive::Path_DoorFlame::Colour::eRed, QT_TRANSLATE_NOOP("Path_DoorFlame_ColourEnum", "Red")},
    {relive::Path_DoorFlame::Colour::eGreen, QT_TRANSLATE_NOOP("Path_DoorFlame_ColourEnum", "Green")},
    {relive::Path_DoorFlame::Colour::eBlue, QT_TRANSLATE_NOOP("Path_DoorFlame_ColourEnum", "Blue")},
})

REFLECT_ENUM(relive::Path_CrawlingSlig::StartState,
{
    {relive::Path_CrawlingSlig::StartState::eSleeping, QT_TRANSLATE_NOOP("Path_CrawlingSlig_StartStateEnum", "Sleeping")},
    {relive::Path_CrawlingSlig::StartState::eAwake, QT_TRANSLATE_NOOP("Path_CrawlingSlig_StartStateEnum", "Awake")},
})

REFLECT_ENUM(relive::Path_CrawlingSlig::CrawlDirection,
{
    {relive::Path_CrawlingSlig::CrawlDirection::eLeft, QT_TRANSLATE_NOOP("Path_CrawlingSlig_CrawlDirectionEnum", "Left")},
    {relive::Path_CrawlingSlig::CrawlDirection::eRight, QT_TRANSLATE_NOOP("Path_CrawlingSlig_CrawlDirectionEnum", "Right")},
    {relive::Path_CrawlingSlig::CrawlDirection::eRandom, QT_TRANSLATE_NOOP("Path_CrawlingSlig_CrawlDirectionEnum", "Random")},
})

// this probably shouldn't be here since the sound/sound direction implementation is extremely scuffed
// but maybe someone wants to use it anyway
REFLECT_ENUM(relive::Path_CrawlingSligButton::ButtonSounds,
{
    {relive::Path_CrawlingSligButton::ButtonSounds::None, QT_TRANSLATE_NOOP("Path_CrawlingSligButton_ButtonSoundsEnum", "None")},
    {relive::Path_CrawlingSligButton::ButtonSounds::SackHit1, QT_TRANSLATE_NOOP("Path_CrawlingSligButton_ButtonSoundsEnum", "Sack Hit 1")},
    {relive::Path_CrawlingSligButton::ButtonSounds::SackHit2, QT_TRANSLATE_NOOP("Path_CrawlingSligButton_ButtonSoundsEnum", "Sack Hit 2")},
    {relive::Path_CrawlingSligButton::ButtonSounds::FallingItemPresence2, QT_TRANSLATE_NOOP("Path_CrawlingSligButton_ButtonSoundsEnum", "Falling Item Presence 2")},
    {relive::Path_CrawlingSligButton::ButtonSounds::SecurityOrb, QT_TRANSLATE_NOOP("Path_CrawlingSligButton_ButtonSoundsEnum", "Security Orb")},
    {relive::Path_CrawlingSligButton::ButtonSounds::Bullet1, QT_TRANSLATE_NOOP("Path_CrawlingSligButton_ButtonSoundsEnum", "Bullet 1")},
    {relive::Path_CrawlingSligButton::ButtonSounds::AbeGenericMovement, QT_TRANSLATE_NOOP("Path_CrawlingSligButton_ButtonSoundsEnum", "Abe Generic Movement")},
})

REFLECT_ENUM(relive::Path_Glukkon::GlukkonTypes,
{
    {relive::Path_Glukkon::GlukkonTypes::eNormal, QT_TRANSLATE_NOOP("Path_Glukkon_GlukkonTypesEnum", "Normal")},
    {relive::Path_Glukkon::GlukkonTypes::eStoryAslik, QT_TRANSLATE_NOOP("Path_Glukkon_GlukkonTypesEnum", "Story Aslik")},
    {relive::Path_Glukkon::GlukkonTypes::eStoryDripik, QT_TRANSLATE_NOOP("Path_Glukkon_GlukkonTypesEnum", "Story Dripik")},
    {relive::Path_Glukkon::GlukkonTypes::eStoryPhleg, QT_TRANSLATE_NOOP("Path_Glukkon_GlukkonTypesEnum", "Story Phleg")},
})

REFLECT_ENUM(relive::Path_Glukkon::Facing,
{
    {relive::Path_Glukkon::Facing::eRight, QT_TRANSLATE_NOOP("Path_Glukkon_FacingEnum", "Right")},
    {relive::Path_Glukkon::Facing::eLeft, QT_TRANSLATE_NOOP("Path_Glukkon_FacingEnum", "Left")},
})

REFLECT_ENUM(relive::Path_Glukkon::SpawnType,
{
    {relive::Path_Glukkon::SpawnType::eRegularSpawn, QT_TRANSLATE_NOOP("Path_Glukkon_SpawnTypeEnum", "Regular Spawn")},
    {relive::Path_Glukkon::SpawnType::eFacingLeft, QT_TRANSLATE_NOOP("Path_Glukkon_SpawnTypeEnum", "Facing Left")},
    {relive::Path_Glukkon::SpawnType::eFacingRight, QT_TRANSLATE_NOOP("Path_Glukkon_SpawnTypeEnum", "Facing Right")},
    {relive::Path_Glukkon::SpawnType::eFullSpawnEffects, QT_TRANSLATE_NOOP("Path_Glukkon_SpawnTypeEnum", "Full Spawn Effects")},
})

REFLECT_ENUM(relive::Path_Glukkon::Behavior,
{
    {relive::Path_Glukkon::Behavior::eIgnoreWalls, QT_TRANSLATE_NOOP("Path_Glukkon_BehaviorEnum", "Ignore Walls")},
    {relive::Path_Glukkon::Behavior::eCheckForWalls, QT_TRANSLATE_NOOP("Path_Glukkon_BehaviorEnum", "Check For Walls")},
})

REFLECT_ENUM(relive::Path_FootSwitch::FootSwitchTriggerBy,
{
    {relive::Path_FootSwitch::FootSwitchTriggerBy::eAbe, QT_TRANSLATE_NOOP("Path_FootSwitch_FootSwitchTriggerByEnum", "Abe")},
    {relive::Path_FootSwitch::FootSwitchTriggerBy::eAnyone, QT_TRANSLATE_NOOP("Path_FootSwitch_FootSwitchTriggerByEnum", "Anyone")},
})

REFLECT_ENUM(relive::Path_SlogSpawner::StartDirection,
{
    {relive::Path_SlogSpawner::StartDirection::eRight, QT_TRANSLATE_NOOP("Path_SlogSpawner_StartDirectionEnum", "Right")},
    {relive::Path_SlogSpawner::StartDirection::eLeft, QT_TRANSLATE_NOOP("Path_SlogSpawner_StartDirectionEnum", "Left")},
})

REFLECT_ENUM(relive::Path_Scrab::ScrabPatrolType,
{
    {relive::Path_Scrab::ScrabPatrolType::eWalk, QT_TRANSLATE_NOOP("Path_Scrab_ScrabPatrolTypeEnum", "Walk")},
    {relive::Path_Scrab::ScrabPatrolType::eRunOrWalk192, QT_TRANSLATE_NOOP("Path_Scrab_ScrabPatrolTypeEnum", "Run Or Walk 192")},
    {relive::Path_Scrab::ScrabPatrolType::eRunOrWalk128, QT_TRANSLATE_NOOP("Path_Scrab_ScrabPatrolTypeEnum", "Run Or Walk 128")},
    {relive::Path_Scrab::ScrabPatrolType::eRunOrWalk64, QT_TRANSLATE_NOOP("Path_Scrab_ScrabPatrolTypeEnum", "Run Or Walk 64")},
    {relive::Path_Scrab::ScrabPatrolType::eRun, QT_TRANSLATE_NOOP("Path_Scrab_ScrabPatrolTypeEnum", "Run")}
})

REFLECT_ENUM(relive::Path_ScrabSpawner::SpawnDirection,
{
    {relive::Path_ScrabSpawner::SpawnDirection::eNone, QT_TRANSLATE_NOOP("Path_ScrabSpawner_SpawnDirectionEnum", "None")},
    {relive::Path_ScrabSpawner::SpawnDirection::eLeft, QT_TRANSLATE_NOOP("Path_ScrabSpawner_SpawnDirectionEnum", "Left")},
    {relive::Path_ScrabSpawner::SpawnDirection::eRight, QT_TRANSLATE_NOOP("Path_ScrabSpawner_SpawnDirectionEnum", "Right")},
})

REFLECT_ENUM(relive::Path_Paramite::EntranceType,
{
    {relive::Path_Paramite::EntranceType::ePatrol, QT_TRANSLATE_NOOP("Path_Paramite_EntranceTypeEnum", "Patrol")},
    {relive::Path_Paramite::EntranceType::eSurpriseWeb, QT_TRANSLATE_NOOP("Path_Paramite_EntranceTypeEnum", "Surprise Web")},
    {relive::Path_Paramite::EntranceType::eSlightlyHigherSpawnSurpriseWeb, QT_TRANSLATE_NOOP("Path_Paramite_EntranceTypeEnum", "Slightly Higher Spawn Surprise Web")},
})

REFLECT_ENUM(relive::Path_BellsongStone::BellsongTypes,
{
    {relive::Path_BellsongStone::BellsongTypes::eWhistle, QT_TRANSLATE_NOOP("Path_BellsongStone_BellsongTypesEnum", "Whistle")},
    {relive::Path_BellsongStone::BellsongTypes::eChimes, QT_TRANSLATE_NOOP("Path_BellsongStone_BellsongTypesEnum", "Chimes")},
})

REFLECT_ENUM(relive::Path_LightEffect::Type,
{
    {relive::Path_LightEffect::Type::Star, QT_TRANSLATE_NOOP("Path_LightEffect_TypeEnum", "Star")},
    {relive::Path_LightEffect::Type::GoldGlow, QT_TRANSLATE_NOOP("Path_LightEffect_TypeEnum", "Gold Glow")},
    {relive::Path_LightEffect::Type::GreenGlow, QT_TRANSLATE_NOOP("Path_LightEffect_TypeEnum", "Green Glow")},
    {relive::Path_LightEffect::Type::FlintGlow, QT_TRANSLATE_NOOP("Path_LightEffect_TypeEnum", "Flint Glow")},
    {relive::Path_LightEffect::Type::Switchable_RedGreenDoorLights, QT_TRANSLATE_NOOP("Path_LightEffect_TypeEnum", "RedGreen Door Light")},
    {relive::Path_LightEffect::Type::Switchable_RedGreenHubLight, QT_TRANSLATE_NOOP("Path_LightEffect_TypeEnum", "RedGreen Hub Light")},
})

REFLECT_ENUM(relive::Path_RingMudokon::MustFaceMud,
{
    {relive::Path_RingMudokon::MustFaceMud::eYes, QT_TRANSLATE_NOOP("Path_RingMudokon_MustFaceMudEnum", "Yes")},
    {relive::Path_RingMudokon::MustFaceMud::eNo, QT_TRANSLATE_NOOP("Path_RingMudokon_MustFaceMudEnum", "No")},
})

REFLECT_ENUM(relive::Path_MeatSaw::Type,
{
    {relive::Path_MeatSaw::Type::eAutomaticPersistOffscreen, QT_TRANSLATE_NOOP("Path_MeatSaw_TypeEnum", "Automatic Persist Offscreen")},
    {relive::Path_MeatSaw::Type::eAutomatic, QT_TRANSLATE_NOOP("Path_MeatSaw_TypeEnum", "Automatic")},
    {relive::Path_MeatSaw::Type::eSwitchId, QT_TRANSLATE_NOOP("Path_MeatSaw_TypeEnum", "Switch ID")},
})

REFLECT_ENUM(relive::Path_MeatSaw::StartState,
{
    {relive::Path_MeatSaw::StartState::eOff, QT_TRANSLATE_NOOP("Path_MeatSaw_StartStateEnum", "Off")},
    {relive::Path_MeatSaw::StartState::eOn, QT_TRANSLATE_NOOP("Path_MeatSaw_StartStateEnum", "On")},
})

REFLECT_ENUM(relive::Path_LiftMudokon::Direction,
{
    {relive::Path_LiftMudokon::Direction::eRight, QT_TRANSLATE_NOOP("Path_LiftMudokon_DirectionEnum", "Right")},
    {relive::Path_LiftMudokon::Direction::eLeft, QT_TRANSLATE_NOOP("Path_LiftMudokon_DirectionEnum", "Left")},
})

REFLECT_ENUM(relive::Path_BeeSwarmHole::MovementType,
{
    {relive::Path_BeeSwarmHole::MovementType::eHover, QT_TRANSLATE_NOOP("Path_BeeSwarmHole_MovementTypeEnum", "Hover")},
    {relive::Path_BeeSwarmHole::MovementType::eAttack, QT_TRANSLATE_NOOP("Path_BeeSwarmHole_MovementTypeEnum", "Attack")},
    {relive::Path_BeeSwarmHole::MovementType::eFollowPath, QT_TRANSLATE_NOOP("Path_BeeSwarmHole_MovementTypeEnum", "Follow Path")},
})

REFLECT_ENUM(relive::Path_ZBall::StartPos,
{
    {relive::Path_ZBall::StartPos::eCenter, QT_TRANSLATE_NOOP("Path_ZBall_StartPosEnum", "Center")},
    {relive::Path_ZBall::StartPos::eOut, QT_TRANSLATE_NOOP("Path_ZBall_StartPosEnum", "Out")},
    {relive::Path_ZBall::StartPos::eIn, QT_TRANSLATE_NOOP("Path_ZBall_StartPosEnum", "In")},
})

REFLECT_ENUM(relive::Path_ZBall::Speed,
{
    {relive::Path_ZBall::Speed::eNormal, QT_TRANSLATE_NOOP("Path_ZBall_SpeedEnum", "Normal")},
    {relive::Path_ZBall::Speed::eFast, QT_TRANSLATE_NOOP("Path_ZBall_SpeedEnum", "Fast")},
    {relive::Path_ZBall::Speed::eSlow, QT_TRANSLATE_NOOP("Path_ZBall_SpeedEnum", "Slow")},
})

