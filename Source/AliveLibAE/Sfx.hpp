#pragma once

#include "FunctionFwd.hpp"
#include "FixedPoint.hpp"

enum SoundEffect
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
    SlurgStop_90 = 90,
    Chisel_91 = 91,
    NakedSligTransformEnd_92 = 92,
    NakedSligTransformStart_93 = 93,
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

struct SfxDefinition
{
    s8 field_0_block_idx;
    s8 field_1_program;
    s8 field_2_note;
    s8 field_3_default_volume;
    s16 field_4_pitch_min;
    s16 field_6_pitch_max;
};
ALIVE_ASSERT_SIZEOF(SfxDefinition, 0x8);


enum class CameraPos : s16;
class BaseAnimatedWithPhysicsGameObject;

EXPORT s32 CC SFX_Play_46FB10(u8 sfxId, s32 leftVol, s32 rightVol, FP scale = FP_FromInteger(1));
EXPORT s32 CC SFX_Play_46FBA0(u8 sfxIdx, s16 volume, s32 pitch, FP scale = FP_FromInteger(1));
EXPORT s32 CC SFX_Play_46FA90(u8 sfxIdx, s16 volume, FP scale = FP_FromInteger(1));
EXPORT s32 CC SFX_Play_46FC20(u8 sfxId, s16 volume, CameraPos direction, FP scale = FP_FromInteger(1));
EXPORT s32 CC SND_4CA5D0(s32 a1, s32 a2, s32 a3, s16 vol, s16 min, s16 max);

enum class SligSpeak : s8
{
    eNone        = -1,
    eHi_0        =  0,
    eHereBoy_1   =  1,
    eGetHim_2    =  2,
    eLaugh_3     =  3,
    eStay_4      =  4,
    eBullshit_5  =  5,
    eLookOut_6   =  6,
    eBullshit2_7 =  7,
    eFreeze_8    =  8,
    eWhat_9      =  9,
    eHelp_10     =  10,
    eBlurgh_11    =  11,
    eGotYa_12    =  12,
    eOuch1_13    =  13,
    eOuch2_14    =  14,
};

EXPORT void CC Slig_GameSpeak_SFX_4C04F0(SligSpeak effectId, s16 defaultVol, s16 pitch_min, BaseAnimatedWithPhysicsGameObject* pObj);
EXPORT s16 CC Calc_Slig_Sound_Direction_4C01B0(BaseAnimatedWithPhysicsGameObject* pObj, s16 defaultVol, const SfxDefinition* pSfx, s16* pLeftVol, s16* pRightVol);
