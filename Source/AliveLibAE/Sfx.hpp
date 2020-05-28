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
    SligSpawn_114 = 144,
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
    char field_0_block_idx;
    char field_1_program;
    char field_2_note;
    char field_3_default_volume;
    __int16 field_4_pitch_min;
    __int16 field_6_pitch_max;
};
ALIVE_ASSERT_SIZEOF(SfxDefinition, 0x8);


enum class CameraPos : __int16;
class BaseAnimatedWithPhysicsGameObject;

EXPORT int CC SFX_Play_46FB10(unsigned __int8 sfxId, int leftVol, int rightVol, FP scale = FP_FromInteger(1));
EXPORT int CC SFX_Play_46FBA0(unsigned __int8 sfxIdx, __int16 volume, int pitch, FP scale = FP_FromInteger(1));
EXPORT int CC SFX_Play_46FA90(unsigned __int8 sfxIdx, __int16 volume, FP scale = FP_FromInteger(1));
EXPORT int CC SFX_Play_46FC20(unsigned __int8 sfxId, __int16 volume, CameraPos direction, FP scale = FP_FromInteger(1));
EXPORT int CC SND_4CA5D0(int a1, int a2, int a3, __int16 vol, __int16 min, __int16 max);

enum class SligSpeak : signed char
{
    None        = -1,
    Hi_0        =  0,
    HereBoy_1   =  1,
    GetEm_2    =  2,
    Laugh_3     =  3,
    Stay_4      =  4,
    Bullshit_5  =  5,
    LookOut_6   =  6,
    Bullshit2_7 =  7,
    Freeze_8    =  8,
    What_9      =  9,
    Help_10     =  10,
    Blurg_11    =  11,
    GotYa_12    =  12,
    Ouch1_13    =  13,
    Ouch2_14    =  14,
};

EXPORT void CC Sfx_Slig_GameSpeak_4C04F0(SligSpeak effectId, __int16 defaultVol, __int16 pitch_min, BaseAnimatedWithPhysicsGameObject* pObj);
EXPORT signed __int16 CC Calc_Slig_Sound_Direction_4C01B0(BaseAnimatedWithPhysicsGameObject* pObj, __int16 defaultVol, const SfxDefinition* pSfx, __int16* pLeftVol, __int16* pRightVol);
