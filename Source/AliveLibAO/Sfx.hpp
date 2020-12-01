#pragma once

#include "FunctionFwd.hpp"

namespace AO {

enum SoundEffect
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
    BellChime1_50 = 50,
    BellChime2_51 = 51,
    BellChime3_52 = 52,
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
    RollingBallStart_71 = 71,
    RollingBallLoop_72 = 72,
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

class BaseAnimatedWithPhysicsGameObject;
class BaseAliveGameObject;
class BaseGameObject;

enum class CameraPos : __int16;

struct SfxDefinition
{
    int field_0_block_idx;
    int field_4_program;
    int field_8_note;
    __int16 field_C_default_volume;
    __int16 field_E_pitch_min;
    __int16 field_10_pitch_max;
    __int16 field_12_pad;
};
ALIVE_ASSERT_SIZEOF(SfxDefinition, 0x14);

EXPORT int CC SFX_Play_43AE60(unsigned __int8 sfxId, int volume, int pitch, BaseAnimatedWithPhysicsGameObject* pObj = nullptr);

EXPORT int CC SFX_Play_43AD70(unsigned __int8 sfxId, int a2, BaseAnimatedWithPhysicsGameObject* a3 = nullptr);

EXPORT int CC SFX_Play_43ADE0(unsigned __int8 sfxId, int leftVol, int rightVol, BaseAnimatedWithPhysicsGameObject* pObj);

EXPORT int CC SFX_Play_43AED0(unsigned __int8 sfxId, signed int volume, CameraPos direction);

}
