#pragma once

#include "Types.hpp"

enum class ReliveTypes : s16
{
    eNone = 7000,
    eCrawlingSligButton,
    eWheelSyncer,
    eDemoSpawnPoint,
    eMultiSwitchController,
    eStatusLight,
    eSlapLock,
    eParamiteWebLine,
    eGlukkonSwitch,
    eDoorBlocker,
    eTrainDoor,
    eLevelLoader,
    eSligGetWings,
    eSligGetPants,
    eTeleporter,
    eWater,
    eWorkWheel,
    eLCDScreen,
    eInvisibleSwitch,
    eDoorFlame,
    eMovingBomb,
    eMainMenuController,
    eHintFly,
    eSecurityDoor,
    eCreditsController,
    eLCDStatusBoard,
    eSwitchStateBooleanLogic,
    eLightEffect,
    eSlogSpawner,
    eGasEmitter,
    eRingCancel,
    eElumWall,
    eAbeStart,
    eBeeSwarmHole,
    eFallingItem,
    eShadowZone,
    eStartController,
    eBirdPortalExit,
    eHoneyDripTarget,
    ePathTransition,
    eZSligCover,
    eResetPath,
    eElumPathTrans,
    eScrabLeftBound,
    eScrabRightBound,
    eScrabNoFall,
    eMovingBombStopper,
    eElumStart,
    eEdge,
    eSoftLanding,
    eMovieHandStone,
    eBellSongStone,
    eDemoPlaybackStone,
    eHandStone,
    eHoist,
    eContinuePoint,
    eWellLocal,
    eWellExpress,
    eMudokonPathTrans,
    eRingMudokon,
    eLiftMudokon,
    eInvisibleZone,
    eEnemyStopper,
    eSligBoundLeft,
    eSligBoundRight,
    eSligPersist,
    eZzzSpawner,
    eKillUnsavedMuds,
    eDeathDrop,
    eAlarm,
    eScreenManager,
    eBackgroundAnimation,
    eBat,
    eLiftMover,
    eTimedMine, // AO only
    eBullet,
    eDDCheat,
    eBells,
    eChimeLock,
    eGasCountDown,
    eParticleBurst,
    eDoor,
    eGameSpeak,
    eElectricWall,
    eElum,
    eBellHammer,
    ePalOverwriter,
    eGroundExplosion,
    eSecurityClaw,
    eRockSpawner,
    eFlintLockFire,
    eThrowableTotalIndicator,
    eFootSwitch,
    eGameEnderController,
    eDeathBird,
    eLoadingFile,
    eGrenade,
    eBoomMachine,
    eBackgroundGlukkon,
    eAbe,
    MainMenuFade, // TODO: rename to eMainMenuFade
    eHoneySack,
    eHoney,
    eClawOrBirdPortalTerminator,
    eMudokon,
    eLiftPoint,
    eMeat,
    eMeatSack,
    eMeatSaw,
    eMine,
    eRollingBallStopperShaker,
    eMotionDetector,
    eRollingBallStopper,
    ePauseMenu,
    eParamite,
    eDemoPlayback,
    eBirdPortal,
    eBirdPortalTerminator,
    eFG1,
    eAbilityRing,
    eRock,
    eRockSack,
    eRollingBall,
    eRope,
    eAirExplosion,
    eRedLaser,
    eScrab,
    eScreenClipper,
    eEffectBase,
    eFade,
    eFlash,
    eScreenWave,
    eUnknown, // todo: remove ?
    eShrykull,
    eSlig,
    eSlog,
    SlingMud, // TODO: rename to eSlingMud
    eSligSpawner,
    eZBall,
    eParticle,
    eZapLine,
    eBeeSwarm, // not the same as beeswarmhole?
    eBeeNest,
    eLever,
    eTrapDoor,
    eUXB,
    eMovie,
    eCameraSwapper,
    eElectrocute,
    eTimedMine_or_MovingBomb, // AE only
    eBone,
    eBoneBag,
    eBrewMachine,
    eSligButton,
    eExplosionSet,
    eZapSpark,
    eMetal,
    eMinesAlarm,
    eCrawlingSlig,
    eDrill,
    eLaughingGas,
    eDoorLock,
    eDove,
    eEvilFart,
    eFleech,
    ePossessionFlicker,
    eFlyingSlig,
    eFlyingSligSpawner,
    eColourfulMeter,
    eSlapLock_OrbWhirlWind,
    eGreeter,
    eGlukkon,
    eHelpPhone,
    eEyeOrbPart,
    eInvisibleEffect,
    ePulley,
    eResourceManager,
    eSligGetPantsOrWings,
    eRingOrLiftMud,
    eSecurityOrb,
    eText,
    eMineCar,
    eGreeterBody,
    eMusicController,
    eMusicTrigger,
    ePullRingRope,
    eScrabSpawner,
    eMainMenuTransistion, // TODO: fix typo
    eScreenShake,
    eSlamDoor,
    eSnoozeParticle,
    eSlurgSpawner,
    eSlurg,
    eTimerTrigger,
    eTorturedMud,
    eWebLine,
    eWell,
    eThrowableArray
};

enum class AETypes : s16
{
    eNone_0 = 0,
    eAlarm_1 = 1,
    eSligSpawner_2 = 2,
    eScreenManager_6 = 6,
    eBackgroundAnimation_7 = 7,
    eLiftMover_9 = 9,
    eTimedMine_or_MovingBomb_10 = 10,
    eBone_11 = 11,
    eBoneBag_12 = 12,
    eBrewMachine_13 = 13,
    eBullet_15 = 15,
    eSligButton_16 = 16,
    eExplosionSet_18 = 18,
    eDDCheat_19 = 19,
    eZapSpark_22 = 22,
    eGasClock_23 = 23,
    eMetal_24 = 24,
    eMinesAlarm_25 = 25,
    eCrawlingSlig_26 = 26,
    eParticleBurst_29 = 29,
    eDrill_30 = 30,
    eLaughingGas_31 = 31,
    eDoor_33 = 33,
    eDoorLock_34 = 34,
    eDove_35 = 35,
    eGameSpeak_38 = 38,
    eElectricWall_39 = 39,
    ePalOverwriter_44 = 44,
    eEvilFart_45 = 45,
    eBaseBomb_46 = 46,
    eSecurityClaw_47 = 47,
    eRockSpawner_48 = 48,
    eFleech_50 = 50,
    ePossessionFlicker_51 = 51,
    eThrowableTotalIndicator_53 = 53,
    eFlyingSlig_54 = 54,
    eFlyingSligSpawner_55 = 55,
    eFootSwitch_56 = 56,
    eGameEnderController_57 = 57,
    eColourfulMeter_59 = 59,
    eSlapLock_OrbWhirlWind_60 = 60,
    eSlapLock_61 = 61,
    eDeathBird_62 = 62,
    eGreeter_64 = 64,
    eGrenade_65 = 65,
    eGrenadeMachine_66 = 66,
    eGlukkon_67 = 67,
    eHelpPhone_68 = 68,
    eAbe_69 = 69,
    eEyeOrbPart_74 = 74,
    eInvisibleEffect_75 = 75,
    ePulley_76 = 76,
    eLiftPoint_78 = 78,
    eResourceManager_70 = 79,
    eSligGetPantsOrWings_80 = 80,
    eRingOrLiftMud_81 = 81,
    eSecurityOrb_83 = 83,
    eMeat_84 = 84,
    eMeatSack_85 = 85,
    eMeatSaw_86 = 86, // from AO
    eText_87 = 87,
    eMine_88 = 88,
    eMineCar_89 = 89,
    eGreeterBody_91 = 91,
    eMusicController_93 = 93,
    eMusicTrigger_94 = 94,
    ePauseMenu_95 = 95,
    eParamite_96 = 96,
    eDemoPlayback_98 = 98,
    eBirdPortal_99 = 99,
    eBirdPortalTerminator_100 = 100,
    eFG1_101 = 101,
    eThrowableArray_102 = 102,
    ePullRope_103 = 103,
    eAbilityRing_104 = 104,
    eRock_105 = 105,
    eRockSack_106 = 106,
    eRope_108 = 108,
    eExplosion_109 = 109,
    eMudokon_110 = 110,
    eRedLaser_111 = 111,
    eScrab_112 = 112,
    eScrabSpawner_113 = 113,
    eScreenClipper_114 = 114,
    eEffectBase_115 = 115,
    eMainMenuTransistion_116 = 116,
    eFlash_117 = 117,
    eScreenShake_118 = 118,
    eShrykull_121 = 121,
    eSlamDoor_122 = 122,
    eSnoozeParticle_124 = 124,
    eSlig_125 = 125,
    eSlog_126 = 126,
    eSlurgSpawner_128 = 128,
    eSlurg_129 = 129,
    eParticle_134 = 134,
    eZapLine_135 = 135,
    eTimerTrigger_136 = 136,
    eLever_139 = 139,
    eTorturedMud_141 = 141,
    eTrapDoor_142 = 142,
    eUXB_143 = 143,
    eMovie_145 = 145,
    eWebLine_146 = 146,
    eWell_147 = 147,
    eWorkWheel_148 = 148,
    eCameraSwapper_149 = 149,
    eElectrocute_150 = 150,
};


namespace AO {

enum class AOTypes : s16
{
    eNone_0 = 0,
    eAlarm_1 = 1,

    eScreenManager_4 = 4,
    eBackgroundAnimation_5 = 5,
    eBat_6 = 6,
    eLiftMover_7 = 7,
    eTimedMine_8 = 8,

    eBullet_10 = 10,
    eDDCheat_12 = 12,
    eBells_13 = 13,
    eChimeLock_14 = 14,
    eZapSpark_15 = 15,
    eGasClock_16 = 16,

    eParticleBurst_19 = 19,

    eDoor_21 = 21,
    eDove_22 = 22,

    eGameSpeak_24 = 24,
    eElectricWall_25 = 25,
    eElum_26 = 26,
    eBellHammer_27 = 27,

    ePalOverwriter_29 = 29,
    eBaseBomb_30 = 30,
    eSecurityClaw_31 = 31,
    eRockSpawner_32 = 32,

    eFlintLockFire_34 = 34,
    eThrowableTotalIndicator_35 = 35,
    eFootSwitch36 = 36,
    eGameEnderController_37 = 37,
    eDeathBird_38 = 38,
    eLoadingFile_39 = 39,
    eGrenade_40 = 40,
    eGrenadeMachine_41 = 41,
    eBackgroundGlukkon_42 = 42,
    eAbe_43 = 43,
    MainMenuFade_44 = 44,
    eHoneySack_45 = 45,

    eHoney_47 = 47,
    eClawOrBirdPortalTerminator_48 = 48,

    eRingOrLiftMud_52 = 52,
    eLiftPoint_51 = 51,
    SecurityOrb_53 = 53,
    eMeat_54 = 54,
    eMeatStack_55 = 55,
    eMeatSaw_56 = 56,
    eMine_57 = 57,
    eRollingBallStopperShaker_58 = 58,
    eMotionDetector_59 = 59,
    eRollingBallStopper_60 = 60,
    ePauseMenu_61 = 61,
    eParamite_62 = 62,
    eDemoPlayback = 64,
    eBirdPortal_65 = 65,
    eBirdPortalTerminator_66 = 66,
    eFG1_67 = 67,
    ePullRingRope_68 = 68,
    eAbilityRing_69 = 69,
    eRock_70 = 70,
    eRockSack_71 = 71,
    eRollingBall_72 = 72,
    eRope_73 = 73,
    eExplosion_74 = 74,
    eMudokon_75 = 75,
    eRedLaser_76 = 76,
    eScrab_77 = 77,
    eScreenClipper_78 = 78,
    eEffectBase_79 = 79,
    eDeathFadeOut_80 = 80, // TODO: Actually eMainMenuTransistion in AE, shared?
    eFlash_81 = 81,
    ScreenShake_82 = 82,
    eScreenWave_83 = 83,
    eUnknown_84 = 84,
    eShrykull_85 = 85,

    eSnoozParticle_87 = 87,
    eSlig_88 = 88,
    eSlog_89 = 89,
    SlingMud_90 = 90,
    eSligSpawner_91 = 91,
    eZBall_92 = 92,
    eParticle_93 = 93,
    eZapLine_94 = 94,
    eBeeSwarm_95 = 95,
    eBeeNest_96 = 96,
    eLever_97 = 97,
    eTrapDoor_98 = 98,
    eUXB_99 = 99,
    eMovie_100 = 100,
    eWell_101 = 101,
    eCameraSwapper_102 = 102,
    eElectrocute_103 = 103,
};
}