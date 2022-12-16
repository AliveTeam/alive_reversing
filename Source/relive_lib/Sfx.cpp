#include "Sfx.hpp"
#include "GameType.hpp"
#include "../AliveLibAE/Math.hpp"
#include "../AliveLibAE/Sound/Midi.hpp"
#include "../relive_lib/FatalError.hpp"

namespace AO {

static const relive::SfxDefinition sSfxEntries[] = {
    {0, 0, 36, 75, 0, 0},           // Unknown_0 = 0,
    {0, 0, 66, 75, 0, 0},           // Bullet1_1 = 1,
    {0, 0, 67, 75, 0, 0},           // Bullet2_2 = 2,
    {0, 0, 72, 65, 0, 0},           // GreenTick_3 = 3,
    {0, 0, 73, 65, 0, 0},           // RedTick_4 = 4,
    {0, 1, 57, 40, 0, 0},           // ZPop_5 = 5,
    {0, 2, 60, 127, -1, 1},         // SligShoot_6 = 6,
    {0, 2, 64, 55, -64, 64},        // BulletShell_7 = 7,
    {0, 4, 36, 127, 0, 0},          // MenuTransition_8 = 8,
    {0, 6, 53, 127, 0, 0},          // RingBellHammer_9 = 9,
    {0, 69, 47, 127, -1, 1},        // UnknownCrystalSFX1_10 = 10,
    {0, 69, 60, 127, -1, 1},        // UnknownCrystalSFX2_11 = 11,
    {0, 6, 60, 127, 0, 0},          // SwitchBellHammer_12 = 12,
    {0, 6, 48, 90, 0, 0},           // HandstoneTransition_13 = 13,
    {0, 7, 59, 70, -127, 127},      // Bees1_14 = 14,
    {0, 7, 60, 70, -127, 127},      // Bees2_15 = 15,
    {0, 7, 64, 70, 0, 0},           // Dove_16 = 16,
    {0, 7, 64, 70, -255, -255},     // RespawnDove_17 = 17,
    {0, 7, 64, 70, -127, -127},     // AbeDove_18 = 18,
    {0, 7, 64, 70, 1, 1},           // FlyingDoves_19 = 19,
    {0, 8, 62, 90, 0, 0},           // Empty_20 = 20,
    {0, 8, 60, 90, 0, 0},           // PossessEffect_21 = 21,
    {0, 8, 60, 70, 1, 1},           // Respawn_22 = 22,
    {0, 8, 60, 90, -2, -2},         // PossessEffect_23 = 23,
    {0, 8, 57, 90, -1, 1},          // WellExit_24 = 24,
    {0, 8, 58, 90, -1, 1},          // WellEnter_25 = 25,
    {0, 8, 61, 60, -1, 1},          // HoneyDrip_26 = 26,
    {0, 9, 36, 50, -2, -2},         // Leaf_27 = 27,
    {0, 9, 37, 50, -2, -2},         // AirStream_28 = 28,
    {0, 9, 38, 60, -1, 1},          // RockBounceOnMine_29 = 29,
    {0, 9, 39, 90, -1, 1},          // SackHit_30 = 30,
    {0, 9, 40, 60, -1, 1},          // RockBounce_31 = 31,
    {0, 9, 41, 60, -1, 1},          // ParticleBurst_32 = 32,
    {0, 9, 60, 115, 0, 0},          // PickupItem_33 = 33,
    {0, 9, 61, 60, 0, 0},           // SackWobble_34 = 34,
    {0, 10, 60, 80, -1, 1},         // LiftStop_35 = 35,
    {0, 10, 61, 55, -1, 1},         // WheelSqueak_36 = 36,
    {0, 11, 60, 60, -1, 1},         // AbeGenericMovement_37 = 37,
    {0, 11, 63, 60, -1, 1},         // MountingElum_38 = 38,
    {0, 12, 44, 90, -2, 2},         // SlogBite_39 = 39,
    {0, 13, 60, 65, -1, 1},         // LoudFire_40 = 40,
    {0, 14, 61, 20, -127, 127},     // Bat1_41 = 41,
    {0, 14, 62, 20, -127, 127},     // Bat2_42 = 42,
    {0, 15, 60, 70, -1, 1},         // MeatBounce_43 = 43,
    {0, 15, 36, 70, -1, 1},         // Empty_44 = 44,
    {0, 16, 48, 70, -900, -900},    // Alarm_45 = 45,
    {0, 17, 36, 127, -1, 1},        // ElectricZap_46 = 46,
    {0, 17, 37, 45, -1, 1},         // ElectricGateLoud_47 = 47,
    {0, 17, 59, 30, -1, 1},         // BirdPortalSpark_48 = 48,
    {0, 19, 48, 70, -1, 1},         // Trapdoor_49 = 49,
    {0, 20, 48, 90, 0, 0},          // BellChime_HighPitch_50 = 50,
    {0, 20, 43, 90, 0, 0},          // BellChime_MediumPitch_51 = 51
    {0, 20, 36, 90, 0, 0},          // BellChime_LowPitch_52 = 52,
    {0, 21, 72, 127, 0, 0},         // FallingItemHit_53 = 53,
    {0, 24, 60, 90, 0, 0},          // Empty_54 = 54,
    {0, 24, 61, 90, 0, 0},          // Empty_55 = 55,
    {0, 24, 62, 55, 0, 0},          // SecurityOrb_56 = 56,
    {0, 25, 36, 127, 0, 0},         // Zap1_57 = 57,
    {0, 25, 37, 70, 0, 0},          // Zap2_58 = 58,
    {0, 26, 40, 46, 0, 0},          // Empty_59 = 59,
    {0, 27, 60, 50, -127, 127},     // HintFly_60 = 60,
    {0, 28, 53, 50, 0, 0},          // MenuNavigation_61 = 61,
    {0, 29, 48, 50, 0, 0},          // ZBall_62 = 62,
    {0, 29, 49, 20, 0, 0},          // Empty_63 = 63,
    {0, 30, 60, 50, 0, 0},          // FootSwitchPress_64 = 64,
    {0, 31, 60, 50, -800, -800},    // RingRopePull_65 = 65,
    {0, 32, 61, 65, -1000, -1000},  // DoorEffect_66 = 66,
    {0, 33, 36, 45, -1860, -1860},  // PortalOpening_67 = 67,
    {0, 34, 60, 80, -1535, -1535},  // FlintLock_68 = 68,
    {0, 34, 61, 60, 0, 0},          // Fire_69 = 69,
    {0, 34, 62, 127, 0, 0},         // PostFlint_70 = 70,
    {0, 35, 36, 90, 0, 0},          // RollingBallNoise1_71 = 71,
    {0, 35, 37, 90, 0, 0},          // RollingBallNoise2_72 = 72,
    {0, 36, 60, 127, 0, 0},         // FallingItemLand_73 = 73,
    {0, 36, 61, 35, -1600, -1600},  // Empty_74 = 74,
    {0, 38, 60, 60, 0, 0},          // LeverPull_75 = 75,
    {0, 39, 36, 90, 0, 0},          // Empty_76 = 76,
    {0, 39, 56, 90, 0, 0},          // Empty_77 = 77,
    {0, 39, 65, 90, 0, 0},          // KillEffect_78 = 78,
    {0, 40, 60, 55, -255, 127},     // Eating1_79 = 79,
    {0, 40, 61, 55, -255, 127},     // Eating2_80 = 80,
    {0, 42, 60, 60, 0, 0},          // Empty_81 = 81,
    {0, 42, 64, 60, 0, 0},          // GrenadeBounce_82 = 82,
    {0, 44, 60, 60, 0, 0},          // SlingshotExtend_83 = 83,
    {0, 44, 61, 60, 0, 0},          // SlingshotShoot_84 = 84,
    {0, 44, 62, 45, -127, 127},     // Clean1_85 = 85,
    {0, 44, 63, 45, -127, 127},     // Clean2_86 = 86,
    {0, 46, 60, 13, 0, 0},          // LCDScreen_87 = 87,
    {0, 48, 36, 40, 0, 0},          // MeatsawOffscreen_88 = 88,
    {0, 48, 42, 40, 0, 0},          // MeatsawIdle_89 = 89,
    {0, 48, 62, 40, 0, 0},          // MeatsawUp_90 = 90,
    {0, 48, 60, 40, 0, 0},          // MeatsawDown_91 = 91,
    {0, 48, 61, 100, 0, 0},         // Empty_92 = 92,
    {0, 49, 60, 90, -800, -800},    // IndustrialNoise1_93 = 93,
    {0, 49, 60, 90, -400, -400},    // IndustrialNoise2_94 = 94,
    {0, 49, 60, 60, 0, 0},          // IndustrialNoise3_95 = 95,
    {0, 49, 61, 45, 0, 0},          // Vaporize_96 = 96,
    {0, 49, 62, 45, 0, 0},          // IndustrialTrigger_97 = 97,
    {0, 50, 60, 60, 0, 0},          // Choke_98 = 98,
    {0, 50, 36, 60, 0, 0},          // Gas1_99 = 99,
    {0, 50, 48, 60, 0, 0},          // Gas2_100 = 100,
    {0, 67, 60, 60, 0, 0},          // GlukkonKillHim1_101 = 101,
    {0, 67, 61, 60, 0, 0},          // GlukkonKillHim2_102 = 102,
    {0, 67, 62, 60, 0, 0},          // GlukkonLaugh1_103 = 103,
    {0, 67, 63, 60, 0, 0},          // GlukkonLaugh2_104 = 104,
    {0, 67, 64, 60, 0, 0},          // Empty_105 = 105,
    {0, 67, 65, 60, 0, 0},          // Empty_106 = 106,
    {0, 80, 36, 90, 0, 0},          // IngameTransition_107 = 107,
    {0, 39, 60, 127, 0, 0},         // Shrykull1_108 = 108,
    {0, 39, 68, 127, 0, 0},         // Shrykull2_109 = 109,
    {0, 63, 38, 90, -1000, -1000},  // SligLaugh_110 = 110,
    {0, 62, 62, 90, -1000, -1000},  // SligHereBoy_111 = 111,
    {0, 64, 39, 90, -1000, -1000}}; // SligBleh_112 = 112
}

static const relive::SfxDefinition sSfxEntries[] = {
    {0u, 0u, 66u, 75u, 0, 0},          // Bullet1_0 = 0,
    {0u, 0u, 67u, 75u, 0, 0},          // Bullet2_1 = 1,
    {0u, 0u, 72u, 55u, 0, 0},          // GreenTick_2 = 2,
    {0u, 0u, 73u, 55u, 0, 0},          // RedTick_3 = 3,
    {0u, 1u, 57u, 40u, 0, 0},          // ZPop_4 = 4,
    {0u, 39u, 60u, 127u, -1, 1},       // SligShoot_5 = 5,
    {0u, 2u, 64u, 55u, -64, 64},       // BulletShell_6 = 6,
    {0u, 7u, 36u, 127u, 0, 0},         // MenuTransition_7 = 7,
    {0u, 6u, 53u, 127u, 0, 0},         // RingUnknownTrigger_8 = 8,
    {0u, 69u, 47u, 127u, -1, 1},       // Empty_9 = 9,
    {0u, 69u, 60u, 127u, -1, 1},       // Empty_10 = 10,
    {0u, 6u, 60u, 127u, 0, 0},         // SwitchUnknownTrigger_11 = 11,
    {0u, 6u, 48u, 90u, 0, 0},          // HandstoneTransition_12 = 12,
    {0u, 7u, 64u, 70u, 0, 0},          // Dove_13 = 13,
    {0u, 7u, 64u, 70u, -255, -255},    // FlyingDove_14 = 14,
    {0u, 7u, 64u, 70u, -127, -127},    // AbeDove_15 = 15,
    {0u, 9u, 62u, 90u, 0, 0},          // PossessEffect_16 = 16,
    {0u, 9u, 62u, 90u, 0, 0},          // PossessEffect_17 = 17,
    {0u, 9u, 62u, 70u, 1, 1},          // ShrykullZap_18 = 18,
    {0u, 9u, 62u, 90u, -2, -2},        // PossessEffect_19 = 19,
    {0u, 9u, 58u, 90u, -1, 1},         // WellExit_20 = 20,
    {0u, 9u, 57u, 90u, -1, 1},         // WellEnter_21 = 21,
    {0u, 9u, 36u, 45u, -2, -2},        // Leaf_22 = 22,
    {0u, 9u, 37u, 50u, -2, -2},        // AirStream_23 = 23,
    {0u, 9u, 38u, 60u, -1, 1},         // RockBounceOnMine_24 = 24,
    {0u, 9u, 39u, 90u, -1, 1},         // SackHit_25 = 25,
    {0u, 9u, 40u, 60u, -1, 1},         // RockBounce_26 = 26,
    {0u, 9u, 41u, 60u, -1, 1},         // ParticleBurst_27 = 27,
    {0u, 9u, 60u, 115u, 0, 0},         // PickupItem_28 = 28,
    {0u, 9u, 61u, 60u, 0, 0},          // SackWobble_29 = 29,
    {0u, 10u, 60u, 80u, -1, 1},        // LiftStop_30 = 30,
    {0u, 10u, 61u, 55u, -1, 1},        // WheelSqueak_31 = 31,
    {0u, 3u, 60u, 60u, -1, 1},         // AbeGenericMovement_32 = 32,
    {0u, 4u, 63u, 60u, -1, 1},         // Empty_33 = 33,
    {0u, 12u, 44u, 90u, -2, 2},        // SlogBite_34 = 34,
    {0u, 13u, 60u, 65u, -1, 1},        // AmbientEffect1_35 = 35,
    {0u, 4u, 58u, 70u, -1, 1},         // MeatBounce_36 = 36,
    {0u, 4u, 59u, 70u, -1, 1},         // Bloop_37 = 37,
    {0u, 0u, 44u, 70u, -900, -900},    // SecurityDoorDeny_38 = 38,
    {0u, 2u, 38u, 127u, -1, 1},        // ElectricZap_39 = 39,
    {0u, 2u, 39u, 45u, -1, 1},         // ElectricGateLoud_40 = 40,
    {0u, 2u, 49u, 30u, -1, 1},         // BirdPortalSpark_41 = 41,
    {0u, 10u, 48u, 70u, -1, 1},        // TrapdoorClose_42 = 42,
    {0u, 10u, 48u, 70u, -127, -125},   // TrapdoorOpen_43 = 43,
    {0u, 20u, 48u, 90u, 0, 0},         // AmbientEffect2_44 = 44,
    {0u, 20u, 43u, 90u, 0, 0},         // AmbientEffect3_45 = 45,
    {0u, 20u, 36u, 90u, 0, 0},         // AmbientEffect4_46 = 46,
    {0u, 0u, 74u, 127u, 0, 0},         // FallingItemHit_47 = 47,
    {0u, 0u, 63u, 55u, 0, 0},          // SecurityOrb_48 = 48,
    {0u, 2u, 36u, 127u, 0, 0},         // Zap1_49 = 49,
    {0u, 2u, 37u, 70u, 0, 0},          // Zap2_50 = 50,
    {0u, 26u, 40u, 46u, 0, 0},         // AmbientEffect5_51 = 51,
    {0u, 1u, 53u, 25u, 0, 0},          // MenuNavigation_52 = 52,
    {0u, 29u, 48u, 50u, 0, 0},         // AmbientEffect6_53 = 53,
    {0u, 29u, 49u, 20u, 0, 0},         // AmbientEffect7_54 = 54,
    {0u, 10u, 59u, 90u, 0, 0},         // FootSwitchPress_55 = 55,
    {0u, 10u, 62u, 50u, -800, -800},   // RingRopePull_56 = 56,
    {0u, 10u, 64u, 65u, -1000, -1000}, // DoorEffect_57 = 57,
    {0u, 10u, 36u, 45u, -1860, -1860}, // PortalOpening_58 = 58,
    {0u, 34u, 61u, 60u, 0, 0},         // Fire_59 = 59,
    {0u, 11u, 36u, 90u, 0, 0},         // AmbientEffect8_60 = 60,
    {0u, 11u, 37u, 90u, 0, 0},         // AmbientEffect9_61 = 61,
    {0u, 11u, 60u, 127u, 0, 0},        // FallingItemLand_62 = 62,
    {0u, 10u, 63u, 60u, 0, 0},         // LeverPull_63 = 63,
    {0u, 4u, 65u, 90u, 0, 0},          // KillEffect_64 = 64,
    {0u, 5u, 58u, 55u, -255, 127},     // Eating1_65 = 65,
    {0u, 5u, 59u, 55u, -255, 127},     // Eating2_66 = 66,
    {0u, 0u, 84u, 127u, 0, 0},         // Empty_67 = 67,
    {0u, 0u, 64u, 60u, 0, 0},          // GrenadeBounce_68 = 68,
    {0u, 44u, 60u, 60u, 0, 0},         // Empty_69 = 69,
    {0u, 44u, 61u, 60u, 0, 0},         // Empty_70 = 70,
    {0u, 3u, 62u, 45u, -127, 127},     // Clean1_71 = 71,
    {0u, 3u, 63u, 45u, -127, 127},     // Clean2_72 = 72,
    {0u, 0u, 70u, 6u, 0, 0},           // LCDScreen_73 = 73,
    {0u, 48u, 36u, 40u, 0, 0},         // FallingItemPresence1_74 = 74,
    {0u, 48u, 42u, 40u, 0, 0},         // FallingItemPresence2_75 = 75,
    {0u, 10u, 65u, 90u, -800, -800},   // IndustrialNoise1_76 = 76,
    {0u, 10u, 65u, 90u, -400, -400},   // IndustrialNoise2_77 = 77,
    {0u, 10u, 65u, 60u, 0, 0},         // IndustrialNoise3_78 = 78,
    {0u, 10u, 66u, 45u, 0, 0},         // Vaporize_79 = 79,
    {0u, 10u, 66u, 45u, 0, 0},         // IndustrialTrigger_80 = 80,
    {0u, 18u, 60u, 60u, 0, 0},         // Choke_81 = 81,
    {0u, 18u, 38u, 60u, 0, 0},         // Gas1_82 = 82,
    {0u, 18u, 48u, 60u, 0, 0},         // Gas2_83 = 83,
    {0u, 10u, 35u, 90u, 0, 0},         // IngameTransition_84 = 84,
    {0u, 4u, 60u, 127u, 0, 0},         // Shrykull1_85 = 85,
    {0u, 4u, 68u, 127u, 0, 0},         // Shrykull2_86 = 86,
    {0u, 25u, 38u, 90u, -1000, -1000}, // SecurityDoorLaugh_87 = 87,
    {0u, 25u, 39u, 90u, -1000, -1000}, // GlukkonSwitchBleh_88 = 88,
    {0u, 6u, 60u, 80u, 600, 700},      // SlurgKill_89 = 89,
    {0u, 6u, 61u, 20u, 2667, 3175},    // SlurgPause_90 = 90,
    {0u, 3u, 48u, 100u, -50, 50},      // Chisel_91 = 91,
    {0u, 10u, 64u, 65u, -1000, -1000}, // NakedSligTransformEnd_92 = 92,
    {0u, 1u, 51u, 127u, 0, 0},         // CrawlingSligTransformStart_93 =
    {0u, 7u, 72u, 40u, 0, 0},          // WaterStart_94 = 94,
    {0u, 7u, 73u, 40u, 0, 0},          // WaterFall_95 = 95,
    {0u, 7u, 74u, 40u, 0, 0},          // WaterEnd_96 = 96,
    {0u, 10u, 40u, 80u, 800, 800},     // DrillMovement_97 = 97,
    {0u, 10u, 41u, 127u, 0, 0},        // Empty_98 = 98,
    {0u, 10u, 42u, 90u, 0, 0},         // DrillCollision_99 = 99,
    {0u, 7u, 60u, 75u, 0, 0},          // MinecarMovement_100 = 100,
    {0u, 7u, 61u, 100u, 0, 0},         // MinecarStop_101 = 101,
    {0u, 1u, 58u, 127u, -2500, -2800}, // MinecarStuck_102 = 102,
    {0u, 5u, 72u, 127u, -500, 500},    // WebDrop1_103 = 103,
    {0u, 5u, 73u, 127u, -500, 500},    // WebDrop2_104 = 104,
    {0u, 6u, 37u, 60u, -200, 200},     // SpiritLockShake_105 = 105,
    {0u, 6u, 36u, 127u, 0, 0},         // SpiritLockBreak_106 = 106,
    {0u, 19u, 36u, 127u, 0, 0},        // FlyingSpirit1_107 = 107,
    {0u, 19u, 60u, 127u, 0, 0},        // FlyingSpirit2_108 = 108,
    {0u, 9u, 62u, 60u, 3048, 3048},    // UnusedSpawn_109 = 109,
    {0u, 9u, 62u, 60u, 3048, 3048},    // ParamiteSpawn_110 = 110,
    {0u, 9u, 62u, 60u, 3048, 3048},    // ScrabSpawn_111 = 111,
    {0u, 9u, 62u, 60u, 3048, 3048},    // GlukkonSpawn_112 = 112,
    {0u, 9u, 62u, 60u, 3175, 3175},    // FlyingSligSpawn_113 = 113,
    {0u, 9u, 62u, 60u, 3302, 3302},    // SligSpawn_114 = 114,
    {0u, 9u, 62u, 60u, 3429, 3429},    // SlogSpawn_115 = 115,
    {0u, 1u, 51u, 100u, 0, 0},         // BrewMachineUseStart_116 = 116,
    {0u, 1u, 52u, 100u, 0, 0},         // BrewMachineUseMid_117 = 117,
    {0u, 1u, 57u, 60u, -1800, -1800},  // BrewMachineUseEmpty_118 = 118,
    {0u, 1u, 86u, 120u, 0, 0},         // BrewMachineUseEnd_119 = 119,
    {0u, 6u, 67u, 127u, 0, 0},         // GreeterLand_120 = 120,
    {0u, 6u, 64u, 127u, 0, 0}};        // GreeterKnockback_121 = 121,


namespace AO {

enum SoundEffect : u8
{
    Unknown_0 = 0,
    Bullet1_1 = 1,
    Bullet2_2 = 2,
    GreenTick_3 = 3,
    RedTick_4 = 4,
    ZPop_5 = 5,
    SligShoot_6 = 6,
    BulletShell_7 = 7,
    MenuTransition_8 = 8,
    RingBellHammer_9 = 9,
    UnknownCrystalSFX1_10 = 10,
    UnknownCrystalSFX2_11 = 11,
    SwitchBellHammer_12 = 12,
    HandstoneTransition_13 = 13,
    Bees1_14 = 14,
    Bees2_15 = 15,
    Dove_16 = 16,
    RespawnDove_17 = 17,
    AbeDove_18 = 18,
    FlyingDoves_19 = 19,
    Empty_20 = 20,
    PossessEffect_21 = 21,
    Respawn_22 = 22,
    PossessEffect_23 = 23,
    WellExit_24 = 24,
    WellEnter_25 = 25,
    HoneyDrip_26 = 26,
    Leaf_27 = 27,
    AirStream_28 = 28,
    RockBounceOnMine_29 = 29,
    SackHit_30 = 30,
    RockBounce_31 = 31,
    ParticleBurst_32 = 32,
    PickupItem_33 = 33,
    SackWobble_34 = 34,
    LiftStop_35 = 35,
    WheelSqueak_36 = 36,
    AbeGenericMovement_37 = 37,
    MountingElum_38 = 38,
    SlogBite_39 = 39,
    LoudFire_40 = 40,
    Bat1_41 = 41,
    Bat2_42 = 42,
    MeatBounce_43 = 43,
    Empty_44 = 44,
    Alarm_45 = 45,
    ElectricZap_46 = 46,
    ElectricGateLoud_47 = 47,
    BirdPortalSpark_48 = 48,
    Trapdoor_49 = 49,
    BellChime_HighPitch_50 = 50,
    BellChime_MediumPitch_51 = 51,
    BellChime_LowPitch_52 = 52,
    FallingItemHit_53 = 53,
    Empty_54 = 54,
    Empty_55 = 55,
    SecurityOrb_56 = 56,
    Zap1_57 = 57,
    Zap2_58 = 58,
    Empty_59 = 59,
    HintFly_60 = 60,
    MenuNavigation_61 = 61,
    ZBall_62 = 62,
    Empty_63 = 63,
    FootSwitchPress_64 = 64,
    RingRopePull_65 = 65,
    DoorEffect_66 = 66,
    PortalOpening_67 = 67,
    FlintLock_68 = 68,
    Fire_69 = 69,
    PostFlint_70 = 70,
    RollingBallNoise1_71 = 71,
    RollingBallNoise2_72 = 72,
    FallingItemLand_73 = 73,
    Empty_74 = 74,
    LeverPull_75 = 75,
    Empty_76 = 76,
    Empty_77 = 77,
    KillEffect_78 = 78,
    Eating1_79 = 79,
    Eating2_80 = 80,
    Empty_81 = 81,
    GrenadeBounce_82 = 82,
    SlingshotExtend_83 = 83,
    SlingshotShoot_84 = 84,
    Clean1_85 = 85,
    Clean2_86 = 86,
    LCDScreen_87 = 87,
    MeatsawOffscreen_88 = 88,
    MeatsawIdle_89 = 89,
    MeatsawUp_90 = 90,
    MeatsawDown_91 = 91,
    Empty_92 = 92,
    IndustrialNoise1_93 = 93,
    IndustrialNoise2_94 = 94,
    IndustrialNoise3_95 = 95,
    Vaporize_96 = 96,
    IndustrialTrigger_97 = 97,
    Choke_98 = 98,
    Gas1_99 = 99,
    Gas2_100 = 100,
    GlukkonKillHim1_101 = 101,
    GlukkonKillHim2_102 = 102,
    GlukkonLaugh1_103 = 103,
    GlukkonLaugh2_104 = 104,
    Empty_105 = 105,
    Empty_106 = 106,
    IngameTransition_107 = 107,
    Shrykull1_108 = 108,
    Shrykull2_109 = 109,
    SligLaugh_110 = 110,
    SligHereBoy_111 = 111,
    SligBleh_112 = 112
};
} // namespace AO

enum SoundEffect : u8
{
    Bullet1_0 = 0,
    Bullet2_1 = 1,
    GreenTick_2 = 2,
    RedTick_3 = 3,
    ZPop_4 = 4,
    SligShoot_5 = 5,
    BulletShell_6 = 6,
    MenuTransition_7 = 7,
    RingUnknownTrigger_8 = 8,
    Empty_9 = 9,
    Empty_10 = 10,
    SwitchUnknownTrigger_11 = 11,
    HandstoneTransition_12 = 12,
    Dove_13 = 13,
    FlyingDove_14 = 14,
    AbeDove_15 = 15,
    PossessEffect_16 = 16,
    PossessEffect_17 = 17,
    ShrykullZap_18 = 18,
    PossessEffect_19 = 19,
    WellExit_20 = 20,
    WellEnter_21 = 21,
    Leaf_22 = 22,
    AirStream_23 = 23,
    RockBounceOnMine_24 = 24,
    SackHit_25 = 25,
    RockBounce_26 = 26,
    ParticleBurst_27 = 27,
    PickupItem_28 = 28,
    SackWobble_29 = 29,
    LiftStop_30 = 30,
    WheelSqueak_31 = 31,
    AbeGenericMovement_32 = 32,
    Empty_33 = 33,
    SlogBite_34 = 34,
    AmbientEffect1_35 = 35,
    MeatBounce_36 = 36,
    Bloop_37 = 37,
    SecurityDoorDeny_38 = 38,
    ElectricZap_39 = 39,
    ElectricGateLoud_40 = 40,
    BirdPortalSpark_41 = 41,
    TrapdoorClose_42 = 42,
    TrapdoorOpen_43 = 43,
    AmbientEffect2_44 = 44,
    AmbientEffect3_45 = 45,
    AmbientEffect4_46 = 46,
    FallingItemHit_47 = 47,
    SecurityOrb_48 = 48,
    Zap1_49 = 49,
    Zap2_50 = 50,
    AmbientEffect5_51 = 51,
    MenuNavigation_52 = 52,
    AmbientEffect6_53 = 53,
    AmbientEffect7_54 = 54,
    FootSwitchPress_55 = 55,
    RingRopePull_56 = 56,
    DoorEffect_57 = 57,
    PortalOpening_58 = 58,
    Fire_59 = 59,
    AmbientEffect8_60 = 60,
    AmbientEffect9_61 = 61,
    FallingItemLand_62 = 62,
    LeverPull_63 = 63,
    KillEffect_64 = 64,
    Eating1_65 = 65,
    Eating2_66 = 66,
    Empty_67 = 67,
    GrenadeBounce_68 = 68,
    Empty_69 = 69,
    Empty_70 = 70,
    Clean1_71 = 71,
    Clean2_72 = 72,
    LCDScreen_73 = 73,
    FallingItemPresence1_74 = 74,
    FallingItemPresence2_75 = 75,
    IndustrialNoise1_76 = 76,
    IndustrialNoise2_77 = 77,
    IndustrialNoise3_78 = 78,
    Vaporize_79 = 79,
    IndustrialTrigger_80 = 80,
    Choke_81 = 81,
    Gas1_82 = 82,
    Gas2_83 = 83,
    IngameTransition_84 = 84,
    Shrykull1_85 = 85,
    Shrykull2_86 = 86,
    SecurityDoorLaugh_87 = 87,
    GlukkonSwitchBleh_88 = 88,
    SlurgKill_89 = 89,
    SlurgPause_90 = 90,
    Chisel_91 = 91,
    NakedSligTransformEnd_92 = 92,
    CrawlingSligTransformStart_93 = 93,
    WaterStart_94 = 94,
    WaterFall_95 = 95,
    WaterEnd_96 = 96,
    DrillMovement_97 = 97,
    Empty_98 = 98,
    DrillCollision_99 = 99,
    MinecarMovement_100 = 100,
    MinecarStop_101 = 101,
    MinecarStuck_102 = 102,
    WebDrop1_103 = 103,
    WebDrop2_104 = 104,
    SpiritLockShake_105 = 105,
    SpiritLockBreak_106 = 106,
    FlyingSpirit1_107 = 107,
    FlyingSpirit2_108 = 108,
    UnusedSpawn_109 = 109,
    ParamiteSpawn_110 = 110,
    ScrabSpawn_111 = 111,
    GlukkonSpawn_112 = 112,
    FlyingSligSpawn_113 = 113,
    SligSpawn_114 = 114,
    SlogSpawn_115 = 115,
    BrewMachineUseStart_116 = 116,
    BrewMachineUseMid_117 = 117,
    BrewMachineUseEmpty_118 = 118,
    BrewMachineUseEnd_119 = 119,
    GreeterLand_120 = 120,
    GreeterKnockback_121 = 121,
};

static AO::SoundEffect ToAo(relive::SoundEffects sfx)
{
    switch (sfx)
    {
        case relive::SoundEffects::Unknown:
            return AO::SoundEffect::Unknown_0;

        case relive::SoundEffects::Bullet1:
            return AO::SoundEffect::Bullet1_1;

        case relive::SoundEffects::Bullet2:
            return AO::SoundEffect::Bullet2_2;

        case relive::SoundEffects::GreenTick:
            return AO::SoundEffect::GreenTick_3;

        case relive::SoundEffects::RedTick:
            return AO::SoundEffect::RedTick_4;

        case relive::SoundEffects::ZPop:
            return AO::SoundEffect::ZPop_5;

        case relive::SoundEffects::SligShoot:
            return AO::SoundEffect::SligShoot_6;

        case relive::SoundEffects::BulletShell:
            return AO::SoundEffect::BulletShell_7;

        case relive::SoundEffects::MenuTransition:
            return AO::SoundEffect::MenuTransition_8;

        case relive::SoundEffects::RingBellHammer:
            return AO::SoundEffect::RingBellHammer_9;

        case relive::SoundEffects::UnknownCrystalSFX1:
            return AO::SoundEffect::UnknownCrystalSFX1_10;

        case relive::SoundEffects::UnknownCrystalSFX2:
            return AO::SoundEffect::UnknownCrystalSFX2_11;

        case relive::SoundEffects::SwitchBellHammer:
            return AO::SoundEffect::SwitchBellHammer_12;

        case relive::SoundEffects::HandstoneTransition:
            return AO::SoundEffect::HandstoneTransition_13;

        case relive::SoundEffects::Bees1:
            return AO::SoundEffect::Bees1_14;

        case relive::SoundEffects::Bees2:
            return AO::SoundEffect::Bees2_15;

        case relive::SoundEffects::Dove:
            return AO::SoundEffect::Dove_16;

        case relive::SoundEffects::RespawnDove:
            return AO::SoundEffect::RespawnDove_17;

        case relive::SoundEffects::AbeDove:
            return AO::SoundEffect::AbeDove_18;

        case relive::SoundEffects::FlyingDoves:
            return AO::SoundEffect::FlyingDoves_19;

        case relive::SoundEffects::PossessEffect:
            return AO::SoundEffect::PossessEffect_21; 

        /*
        case relive::SoundEffects::PossessEffect:
            return AO::SoundEffect::PossessEffect_23; // Seems like OG never used this
        */

        case relive::SoundEffects::Respawn:
            return AO::SoundEffect::Respawn_22;

        case relive::SoundEffects::WellExit:
            return AO::SoundEffect::WellExit_24;

        case relive::SoundEffects::WellEnter:
            return AO::SoundEffect::WellEnter_25;

        case relive::SoundEffects::HoneyDrip:
            return AO::SoundEffect::HoneyDrip_26;

        case relive::SoundEffects::Leaf:
            return AO::SoundEffect::Leaf_27;

        case relive::SoundEffects::AirStream:
            return AO::SoundEffect::AirStream_28;

        case relive::SoundEffects::RockBounceOnMine:
            return AO::SoundEffect::RockBounceOnMine_29;

        case relive::SoundEffects::SackHit:
            return AO::SoundEffect::SackHit_30;

        case relive::SoundEffects::RockBounce:
            return AO::SoundEffect::RockBounce_31;

        case relive::SoundEffects::ParticleBurst:
            return AO::SoundEffect::ParticleBurst_32;

        case relive::SoundEffects::PickupItem:
            return AO::SoundEffect::PickupItem_33;

        case relive::SoundEffects::SackWobble:
            return AO::SoundEffect::SackWobble_34;

        case relive::SoundEffects::LiftStop:
            return AO::SoundEffect::LiftStop_35;

        case relive::SoundEffects::WheelSqueak:
            return AO::SoundEffect::WheelSqueak_36;

        case relive::SoundEffects::AbeGenericMovement:
            return AO::SoundEffect::AbeGenericMovement_37;

        case relive::SoundEffects::MountingElum:
            return AO::SoundEffect::MountingElum_38;

        case relive::SoundEffects::SlogBite:
            return AO::SoundEffect::SlogBite_39;

        case relive::SoundEffects::LoudFire:
            return AO::SoundEffect::LoudFire_40;

        case relive::SoundEffects::Bat1:
            return AO::SoundEffect::Bat1_41;

        case relive::SoundEffects::Bat2:
            return AO::SoundEffect::Bat2_42;

        case relive::SoundEffects::MeatBounce:
            return AO::SoundEffect::MeatBounce_43;

        case relive::SoundEffects::Alarm:
            return AO::SoundEffect::Alarm_45;

        case relive::SoundEffects::ElectricZap:
            return AO::SoundEffect::ElectricZap_46;

        case relive::SoundEffects::ElectricGateLoud:
            return AO::SoundEffect::ElectricGateLoud_47;

        case relive::SoundEffects::BirdPortalSpark:
            return AO::SoundEffect::BirdPortalSpark_48;

        case relive::SoundEffects::Trapdoor:
            return AO::SoundEffect::Trapdoor_49;

        case relive::SoundEffects::BellChime_HighPitch:
            return AO::SoundEffect::BellChime_HighPitch_50;

        case relive::SoundEffects::BellChime_MediumPitch:
            return AO::SoundEffect::BellChime_MediumPitch_51;

        case relive::SoundEffects::BellChime_LowPitch:
            return AO::SoundEffect::BellChime_LowPitch_52;

        case relive::SoundEffects::FallingItemHit:
            return AO::SoundEffect::FallingItemHit_53;

        case relive::SoundEffects::SecurityOrb:
            return AO::SoundEffect::SecurityOrb_56;

        case relive::SoundEffects::Zap1:
            return AO::SoundEffect::Zap1_57;

        case relive::SoundEffects::Zap2:
            return AO::SoundEffect::Zap2_58;

        case relive::SoundEffects::HintFly:
            return AO::SoundEffect::HintFly_60;

        case relive::SoundEffects::MenuNavigation:
            return AO::SoundEffect::MenuNavigation_61;

        case relive::SoundEffects::ZBall:
            return AO::SoundEffect::ZBall_62;

        case relive::SoundEffects::FootSwitchPress:
            return AO::SoundEffect::FootSwitchPress_64;

        case relive::SoundEffects::RingRopePull:
            return AO::SoundEffect::RingRopePull_65;

        case relive::SoundEffects::DoorEffect:
            return AO::SoundEffect::DoorEffect_66;

        case relive::SoundEffects::PortalOpening:
            return AO::SoundEffect::PortalOpening_67;

        case relive::SoundEffects::FlintLock:
            return AO::SoundEffect::FlintLock_68;

        case relive::SoundEffects::Fire:
            return AO::SoundEffect::Fire_69;

        case relive::SoundEffects::PostFlint:
            return AO::SoundEffect::PostFlint_70;

        case relive::SoundEffects::RollingBallNoise1:
            return AO::SoundEffect::RollingBallNoise1_71;

        case relive::SoundEffects::RollingBallNoise2:
            return AO::SoundEffect::RollingBallNoise2_72;

        case relive::SoundEffects::FallingItemLand:
            return AO::SoundEffect::FallingItemLand_73;

        case relive::SoundEffects::LeverPull:
            return AO::SoundEffect::LeverPull_75;

        case relive::SoundEffects::KillEffect:
            return AO::SoundEffect::KillEffect_78;

        case relive::SoundEffects::Eating1:
            return AO::SoundEffect::Eating1_79;

        case relive::SoundEffects::Eating2:
            return AO::SoundEffect::Eating2_80;

        case relive::SoundEffects::GrenadeBounce:
            return AO::SoundEffect::GrenadeBounce_82;

        case relive::SoundEffects::SlingshotExtend:
            return AO::SoundEffect::SlingshotExtend_83;

        case relive::SoundEffects::SlingshotShoot:
            return AO::SoundEffect::SlingshotShoot_84;

        case relive::SoundEffects::Clean1:
            return AO::SoundEffect::Clean1_85;

        case relive::SoundEffects::Clean2:
            return AO::SoundEffect::Clean2_86;

        case relive::SoundEffects::LCDScreen:
            return AO::SoundEffect::LCDScreen_87;

        case relive::SoundEffects::MeatsawOffscreen:
            return AO::SoundEffect::MeatsawOffscreen_88;

        case relive::SoundEffects::MeatsawIdle:
            return AO::SoundEffect::MeatsawIdle_89;

        case relive::SoundEffects::MeatsawUp:
            return AO::SoundEffect::MeatsawUp_90;

        case relive::SoundEffects::MeatsawDown:
            return AO::SoundEffect::MeatsawDown_91;

        case relive::SoundEffects::IndustrialNoise1:
            return AO::SoundEffect::IndustrialNoise1_93;

        case relive::SoundEffects::IndustrialNoise2:
            return AO::SoundEffect::IndustrialNoise2_94;

        case relive::SoundEffects::IndustrialNoise3:
            return AO::SoundEffect::IndustrialNoise3_95;

        case relive::SoundEffects::Vaporize:
            return AO::SoundEffect::Vaporize_96;

        case relive::SoundEffects::IndustrialTrigger:
            return AO::SoundEffect::IndustrialTrigger_97;

        case relive::SoundEffects::Choke:
            return AO::SoundEffect::Choke_98;

        case relive::SoundEffects::Gas1:
            return AO::SoundEffect::Gas1_99;

        case relive::SoundEffects::Gas2:
            return AO::SoundEffect::Gas2_100;

        case relive::SoundEffects::GlukkonKillHim1:
            return AO::SoundEffect::GlukkonKillHim1_101;

        case relive::SoundEffects::GlukkonKillHim2:
            return AO::SoundEffect::GlukkonKillHim2_102;

        case relive::SoundEffects::GlukkonLaugh1:
            return AO::SoundEffect::GlukkonLaugh1_103;

        case relive::SoundEffects::GlukkonLaugh2:
            return AO::SoundEffect::GlukkonLaugh2_104;

        case relive::SoundEffects::Empty:
            return AO::SoundEffect::Empty_105;
        
        /*
         case relive::SoundEffects::Empty:
            return AO::SoundEffect::Empty_20;

        case relive::SoundEffects::Empty:
            return AO::SoundEffect::Empty_74;

        case relive::SoundEffects::Empty:
            return AO::SoundEffect::Empty_44;

        case relive::SoundEffects::Empty:
            return AO::SoundEffect::Empty_54;

        case relive::SoundEffects::Empty:
            return AO::SoundEffect::Empty_55;

        case relive::SoundEffects::Empty:
            return AO::SoundEffect::Empty_59;

        case relive::SoundEffects::Empty:
            return AO::SoundEffect::Empty_63;

        case relive::SoundEffects::Empty:
            return AO::SoundEffect::Empty_76;

        case relive::SoundEffects::Empty:
            return AO::SoundEffect::Empty_77;

        case relive::SoundEffects::Empty:
            return AO::SoundEffect::Empty_81;

        case relive::SoundEffects::Empty:
            return AO::SoundEffect::Empty_92;

        case relive::SoundEffects::Empty:
            return AO::SoundEffect::Empty_106;
        */

        case relive::SoundEffects::IngameTransition:
            return AO::SoundEffect::IngameTransition_107;

        case relive::SoundEffects::Shrykull1:
            return AO::SoundEffect::Shrykull1_108;

        case relive::SoundEffects::Shrykull2:
            return AO::SoundEffect::Shrykull2_109;

        case relive::SoundEffects::SligLaugh:
            return AO::SoundEffect::SligLaugh_110;

        case relive::SoundEffects::SligHereBoy:
            return AO::SoundEffect::SligHereBoy_111;

        case relive::SoundEffects::SligBleh:
            return AO::SoundEffect::SligBleh_112;
    }
    ALIVE_FATAL("Unknown sound");
}

static SoundEffect ToAe(relive::SoundEffects sfx)
{
    switch (sfx)
    {
        case relive::SoundEffects::Bullet1:
            return SoundEffect::Bullet1_0;

        case relive::SoundEffects::Bullet2:
            return SoundEffect::Bullet2_1;

        case relive::SoundEffects::GreenTick:
            return SoundEffect::GreenTick_2;

        case relive::SoundEffects::RedTick:
            return SoundEffect::RedTick_3;

        case relive::SoundEffects::ZPop:
            return SoundEffect::ZPop_4;

        case relive::SoundEffects::SligShoot:
            return SoundEffect::SligShoot_5;

        case relive::SoundEffects::BulletShell:
            return SoundEffect::BulletShell_6;

        case relive::SoundEffects::MenuTransition:
            return SoundEffect::MenuTransition_7;

        case relive::SoundEffects::RingUnknownTrigger:
            return SoundEffect::RingUnknownTrigger_8;

        case relive::SoundEffects::SwitchUnknownTrigger:
            return SoundEffect::SwitchUnknownTrigger_11;

        case relive::SoundEffects::HandstoneTransition:
            return SoundEffect::HandstoneTransition_12;

        case relive::SoundEffects::Dove:
            return SoundEffect::Dove_13;

        case relive::SoundEffects::FlyingDove:
            return SoundEffect::FlyingDove_14;

        case relive::SoundEffects::AbeDove:
            return SoundEffect::AbeDove_15;

        case relive::SoundEffects::PossessEffect:
            return SoundEffect::PossessEffect_17;

        /*
        case relive::SoundEffects::PossessEffect:
            return SoundEffect::PossessEffect_16; // Seems like OG never used this

        case relive::SoundEffects::PossessEffect:
            return SoundEffect::PossessEffect_19; // Seems like OG never used this
        */

        case relive::SoundEffects::ShrykullZap:
            return SoundEffect::ShrykullZap_18;

        case relive::SoundEffects::WellExit:
            return SoundEffect::WellExit_20;

        case relive::SoundEffects::WellEnter:
            return SoundEffect::WellEnter_21;

        case relive::SoundEffects::Leaf:
            return SoundEffect::Leaf_22;

        case relive::SoundEffects::AirStream:
            return SoundEffect::AirStream_23;

        case relive::SoundEffects::RockBounceOnMine:
            return SoundEffect::RockBounceOnMine_24;

        case relive::SoundEffects::SackHit:
            return SoundEffect::SackHit_25;

        case relive::SoundEffects::RockBounce:
            return SoundEffect::RockBounce_26;

        case relive::SoundEffects::ParticleBurst:
            return SoundEffect::ParticleBurst_27;

        case relive::SoundEffects::PickupItem:
            return SoundEffect::PickupItem_28;

        case relive::SoundEffects::SackWobble:
            return SoundEffect::SackWobble_29;

        case relive::SoundEffects::LiftStop:
            return SoundEffect::LiftStop_30;

        case relive::SoundEffects::WheelSqueak:
            return SoundEffect::WheelSqueak_31;

        case relive::SoundEffects::AbeGenericMovement:
            return SoundEffect::AbeGenericMovement_32;

        case relive::SoundEffects::SlogBite:
            return SoundEffect::SlogBite_34;

        case relive::SoundEffects::AmbientEffect1:
            return SoundEffect::AmbientEffect1_35;

        case relive::SoundEffects::MeatBounce:
            return SoundEffect::MeatBounce_36;

        case relive::SoundEffects::Bloop:
            return SoundEffect::Bloop_37;

        case relive::SoundEffects::SecurityDoorDeny:
            return SoundEffect::SecurityDoorDeny_38;

        case relive::SoundEffects::ElectricZap:
            return SoundEffect::ElectricZap_39;

        case relive::SoundEffects::ElectricGateLoud:
            return SoundEffect::ElectricGateLoud_40;

        case relive::SoundEffects::BirdPortalSpark:
            return SoundEffect::BirdPortalSpark_41;

        case relive::SoundEffects::TrapdoorClose:
            return SoundEffect::TrapdoorClose_42;

        case relive::SoundEffects::TrapdoorOpen:
            return SoundEffect::TrapdoorOpen_43;

        case relive::SoundEffects::AmbientEffect2:
            return SoundEffect::AmbientEffect2_44;

        case relive::SoundEffects::AmbientEffect3:
            return SoundEffect::AmbientEffect3_45;

        case relive::SoundEffects::AmbientEffect4:
            return SoundEffect::AmbientEffect4_46;

        case relive::SoundEffects::FallingItemHit:
            return SoundEffect::FallingItemHit_47;

        case relive::SoundEffects::SecurityOrb:
            return SoundEffect::SecurityOrb_48;

        case relive::SoundEffects::Zap1:
            return SoundEffect::Zap1_49;

        case relive::SoundEffects::Zap2:
            return SoundEffect::Zap2_50;

        case relive::SoundEffects::AmbientEffect5:
            return SoundEffect::AmbientEffect5_51;

        case relive::SoundEffects::MenuNavigation:
            return SoundEffect::MenuNavigation_52;

        case relive::SoundEffects::AmbientEffect6:
            return SoundEffect::AmbientEffect6_53;

        case relive::SoundEffects::AmbientEffect7:
            return SoundEffect::AmbientEffect7_54;

        case relive::SoundEffects::FootSwitchPress:
            return SoundEffect::FootSwitchPress_55;

        case relive::SoundEffects::RingRopePull:
            return SoundEffect::RingRopePull_56;

        case relive::SoundEffects::DoorEffect:
            return SoundEffect::DoorEffect_57;

        case relive::SoundEffects::PortalOpening:
            return SoundEffect::PortalOpening_58;

        case relive::SoundEffects::Fire:
            return SoundEffect::Fire_59;

        case relive::SoundEffects::AmbientEffect8:
            return SoundEffect::AmbientEffect8_60;

        case relive::SoundEffects::AmbientEffect9:
            return SoundEffect::AmbientEffect9_61;

        case relive::SoundEffects::FallingItemLand:
            return SoundEffect::FallingItemLand_62;

        case relive::SoundEffects::LeverPull:
            return SoundEffect::LeverPull_63;

        case relive::SoundEffects::KillEffect:
            return SoundEffect::KillEffect_64;

        case relive::SoundEffects::Eating1:
            return SoundEffect::Eating1_65;

        case relive::SoundEffects::Eating2:
            return SoundEffect::Eating2_66;

        case relive::SoundEffects::Empty:
            return SoundEffect::Empty_67;

        /*
        case relive::SoundEffects::Empty:
            return SoundEffect::Empty_9;

        case relive::SoundEffects::Empty:
            return SoundEffect::Empty_10;

        case relive::SoundEffects::Empty:
            return SoundEffect::Empty_33;

        case relive::SoundEffects::Empty:
            return SoundEffect::Empty_69;

        case relive::SoundEffects::Empty:
            return SoundEffect::Empty_70;

         case relive::SoundEffects::Empty:
            return SoundEffect::Empty_98;
        */

        case relive::SoundEffects::GrenadeBounce:
            return SoundEffect::GrenadeBounce_68;

        case relive::SoundEffects::Clean1:
            return SoundEffect::Clean1_71;

        case relive::SoundEffects::Clean2:
            return SoundEffect::Clean2_72;

        case relive::SoundEffects::LCDScreen:
            return SoundEffect::LCDScreen_73;

        case relive::SoundEffects::FallingItemPresence1:
            return SoundEffect::FallingItemPresence1_74;

        case relive::SoundEffects::FallingItemPresence2:
            return SoundEffect::FallingItemPresence2_75;

        case relive::SoundEffects::IndustrialNoise1:
            return SoundEffect::IndustrialNoise1_76;

        case relive::SoundEffects::IndustrialNoise2:
            return SoundEffect::IndustrialNoise2_77;

        case relive::SoundEffects::IndustrialNoise3:
            return SoundEffect::IndustrialNoise3_78;

        case relive::SoundEffects::Vaporize:
            return SoundEffect::Vaporize_79;

        case relive::SoundEffects::IndustrialTrigger:
            return SoundEffect::IndustrialTrigger_80;

        case relive::SoundEffects::Choke:
            return SoundEffect::Choke_81;

        case relive::SoundEffects::Gas1:
            return SoundEffect::Gas1_82;

        case relive::SoundEffects::Gas2:
            return SoundEffect::Gas2_83;

        case relive::SoundEffects::IngameTransition:
            return SoundEffect::IngameTransition_84;

        case relive::SoundEffects::Shrykull1:
            return SoundEffect::Shrykull1_85;

        case relive::SoundEffects::Shrykull2:
            return SoundEffect::Shrykull2_86;

        case relive::SoundEffects::SecurityDoorLaugh:
            return SoundEffect::SecurityDoorLaugh_87;

        case relive::SoundEffects::GlukkonSwitchBleh:
            return SoundEffect::GlukkonSwitchBleh_88;

        case relive::SoundEffects::SlurgKill:
            return SoundEffect::SlurgKill_89;

        case relive::SoundEffects::SlurgPause:
            return SoundEffect::SlurgPause_90;

        case relive::SoundEffects::Chisel:
            return SoundEffect::Chisel_91;

        case relive::SoundEffects::NakedSligTransformEnd:
            return SoundEffect::NakedSligTransformEnd_92;

        case relive::SoundEffects::CrawlingSligTransformStart:
            return SoundEffect::CrawlingSligTransformStart_93;

        case relive::SoundEffects::WaterStart:
            return SoundEffect::WaterStart_94;

        case relive::SoundEffects::WaterFall:
            return SoundEffect::WaterFall_95;

        case relive::SoundEffects::WaterEnd:
            return SoundEffect::WaterEnd_96;

        case relive::SoundEffects::DrillMovement:
            return SoundEffect::DrillMovement_97;

        case relive::SoundEffects::DrillCollision:
            return SoundEffect::DrillCollision_99;

        case relive::SoundEffects::MinecarMovement:
            return SoundEffect::MinecarMovement_100;

        case relive::SoundEffects::MinecarStop:
            return SoundEffect::MinecarStop_101;

        case relive::SoundEffects::MinecarStuck:
            return SoundEffect::MinecarStuck_102;

        case relive::SoundEffects::WebDrop1:
            return SoundEffect::WebDrop1_103;

        case relive::SoundEffects::WebDrop2:
            return SoundEffect::WebDrop2_104;

        case relive::SoundEffects::SpiritLockShake:
            return SoundEffect::SpiritLockShake_105;

        case relive::SoundEffects::SpiritLockBreak:
            return SoundEffect::SpiritLockBreak_106;

        case relive::SoundEffects::FlyingSpirit1:
            return SoundEffect::FlyingSpirit1_107;

        case relive::SoundEffects::FlyingSpirit2:
            return SoundEffect::FlyingSpirit2_108;

        case relive::SoundEffects::UnusedSpawn:
            return SoundEffect::UnusedSpawn_109;

        case relive::SoundEffects::ParamiteSpawn:
            return SoundEffect::ParamiteSpawn_110;

        case relive::SoundEffects::ScrabSpawn:
            return SoundEffect::ScrabSpawn_111;

        case relive::SoundEffects::GlukkonSpawn:
            return SoundEffect::GlukkonSpawn_112;

        case relive::SoundEffects::FlyingSligSpawn:
            return SoundEffect::FlyingSligSpawn_113;

        case relive::SoundEffects::SligSpawn:
            return SoundEffect::SligSpawn_114;

        case relive::SoundEffects::SlogSpawn:
            return SoundEffect::SlogSpawn_115;

        case relive::SoundEffects::BrewMachineUseStart:
            return SoundEffect::BrewMachineUseStart_116;

        case relive::SoundEffects::BrewMachineUseMid:
            return SoundEffect::BrewMachineUseMid_117;

        case relive::SoundEffects::BrewMachineUseEmpty:
            return SoundEffect::BrewMachineUseEmpty_118;

        case relive::SoundEffects::BrewMachineUseEnd:
            return SoundEffect::BrewMachineUseEnd_119;

        case relive::SoundEffects::GreeterLand:
            return SoundEffect::GreeterLand_120;

        case relive::SoundEffects::GreeterKnockback:
            return SoundEffect::GreeterKnockback_121;
    }
    ALIVE_FATAL("Unknown sound");
}

const relive::SfxDefinition& relive::GetSfx(relive::SoundEffects sfx)
{
    if (GetGameType() == GameType::eAo)
    {
        return AO::sSfxEntries[static_cast<u32>(ToAo(sfx))];
    }
    else
    {
        return sSfxEntries[static_cast<u32>(ToAe(sfx))];
    }
}

relive::SoundEffects relive::RandomSfx(relive::SoundEffects sfx1, relive::SoundEffects sfx2)
{
    if (Math_RandomRange(0, 1) == 0)
    {
        return sfx1;
    }
    else
    {
        return sfx2;
    }
}

s32 relive::SFX_Play_Pitch(relive::SoundEffects sfxId, s32 volume, s32 pitch, FP scale)
{
    if (!volume)
    {
        volume = relive::GetSfx(sfxId).mDefaultVolume;
    }
    if (scale == FP_FromDouble(0.5))
    {
        volume = 2 * relive::GetSfx(sfxId).mDefaultVolume / 3;
    }
    return SFX_SfxDefinition_Play_Mono(relive::GetSfx(sfxId), volume, pitch, pitch);
}

s32 relive::SfxPlayMono(relive::SoundEffects sfxId, s32 volume, FP scale)
{
    if (!volume)
    {
        volume = relive::GetSfx(sfxId).mDefaultVolume;
    }
    if (scale == FP_FromDouble(0.5))
    {
        volume /= 3;
    }
    return SFX_SfxDefinition_Play_Mono(relive::GetSfx(sfxId), volume, 0x7FFF, 0x7FFF);
}
