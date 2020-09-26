#pragma once

#include "FunctionFwd.hpp"

START_NS_AO

enum SoundEffect
{
    Unknown_0 = 0,
    Unknown_1 = 1,
    Unknown_2 = 2,
    GreenTick_3 = 3,
    RedTick_4 = 4,
    ZPop_5 = 5,
    Unknown_6 = 6,
    Unknown_7 = 7,
    Unknown_8 = 8,
    Unknown_9 = 9,
    Unknown_10 = 10,
    Unknown_11 = 11,
    Unknown_12 = 12,
    Unknown_13 = 13,
    Unknown_14 = 14,
    Unknown_15 = 15,
    Unknown_16 = 16,
    Unknown_17 = 17,
    Unknown_18 = 18,
    Unknown_19 = 19,
    Unknown_20 = 20,
    PossessEffect_21 = 21,
    Unknown_22 = 22,
    Unknown_23 = 23,
    Unknown_24 = 24,
    Unknown_25 = 25,
    Unknown_26 = 26,
    Leaf_27 = 27,
    AirStream_28 = 28,
    Unknown_29 = 29,
    Unknown_30 = 30,
    Unknown_31 = 31,
    Unknown_32 = 32,
    PickupItem_33 = 33,
    Unknown_34 = 34,
    Unknown_35 = 35,
    Unknown_36 = 36,
    Unknown_37 = 37,
    Unknown_38 = 38,
    Unknown_39 = 39,
    Unknown_40 = 40,
    Unknown_41 = 41,
    Unknown_42 = 42,
    MeatBounce_43 = 43,
    Unknown_44 = 44,
    Unknown_45 = 45,
    Unknown_46 = 46,
    Unknown_47 = 47,
    BirdPortalSpark_48 = 48,
    Unknown_49 = 49,
    Unknown_50 = 50,
    Unknown_51 = 51,
    Unknown_52 = 52,
    Unknown_53 = 53,
    Unknown_54 = 54,
    Unknown_55 = 55,
    Unknown_56 = 56,
    Unknown_57 = 57,
    Unknown_58 = 58,
    Unknown_59 = 59,
    Unknown_60 = 60,
    MenuNavigation_61 = 61,
    Unknown_62 = 62,
    Unknown_63 = 63,
    Unknown_64 = 64,
    Unknown_65 = 65,
    Unknown_66 = 66,
    Unknown_67 = 67,
    Unknown_68 = 68,
    Unknown_69 = 69,
    Unknown_70 = 70,
    Unknown_71 = 71,
    Unknown_72 = 72,
    Unknown_73 = 73,
    Unknown_74 = 74,
    Unknown_75 = 75,
    Unknown_76 = 76,
    Unknown_77 = 77,
    KillEffect_78 = 78, // was previously KillEffect_64 = 78
    Unknown_79 = 79,
    Unknown_80 = 80,
    Unknown_81 = 81,
    Unknown_82 = 82,
    Unknown_83 = 83,
    Unknown_84 = 84,
    Scrub1_85 = 85,
    Scrub2_86 = 86,
    Unknown_87 = 87,
    Unknown_88 = 88,
    Unknown_89 = 89,
    MeatsawUp_90 = 90,
    MeatsawDown_91 = 91,
    Vaporize_96 = 96,
    Choke_98 = 98,
    Gas1_99 = 99,
    Gas2_100 = 100,
    GlukkonKillHim1_101 = 101,
    GlukkonKillHim2_102 = 102,
    GlukkonLaugh1_103 = 103,
    GlukkonLaugh2_104 = 104,
    Unknown_105 = 105,
    Unknown_106 = 106,
    IngameTransition_107 = 107,
    SligLaugh_110 = 110
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

EXPORT int CC SFX_Play_43AE60(SoundEffect sfxId, int volume, int pitch, BaseAnimatedWithPhysicsGameObject* pObj = nullptr);

EXPORT int CC SFX_Play_43AD70(unsigned __int8 sfxId, int a2, BaseGameObject* a3 = nullptr);

EXPORT int CC SFX_Play_43ADE0(unsigned __int8 sfxId, int leftVol, int rightVol, BaseAliveGameObject* pObj);

EXPORT int CC SFX_Play_43AED0(unsigned __int8 sfxId, signed int volume, CameraPos direction);

END_NS_AO
