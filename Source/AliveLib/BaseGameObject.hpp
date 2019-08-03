#pragma once

#include "FunctionFwd.hpp"
#include "DynamicArray.hpp"
#include "BitField.hpp"

enum class Types : __int16
{
    eNone_0 = 0,
    eAlarm_1 = 1,
    eSligSpawner_2 = 2,
    eType_3 = 3,
    eType_4 = 4,
    eType_5 = 5,
    eScreenManager_6 = 6,
    eBackgroundAnimation_7 = 7,
    eType_8 = 8,
    eLiftMover_9 = 9,
    eTimedMine_or_MovingBomb_10 = 10,
    eBone_11 = 11,
    eBoneBag_12 = 12,
    eBrewMachine_13 = 13,
    eType_14 = 14,
    eBullet_15 = 15,
    eSligButton_16 = 16,
    eType_17 = 17,
    eType_18 = 18,
    eDDCheat_19 = 19,
    eType_20 = 20,
    eType_21 = 21,
    eType_22 = 22,
    eGasClock_23 = 23,
    eType_24 = 24,
    eType_25 = 25,
    eCrawlingSlig_26 = 26,
    eType_27 = 27,
    eType_28 = 28,
    eParticleBurst_29 = 29,
    eGrinder_30 = 30,
    eLaughingGas_31 = 31,
    eType_32 = 32,
    eDoor_33 = 33,
    eDoorLock_34 = 34,
    eBird_35 = 35,
    eType_36 = 36,
    eType_37 = 37,
    eGameSpeak_38 = 38,
    eElectricWall_39 = 39,
    eType_40 = 40,
    eType_41 = 41,
    eType_42 = 42,
    eType_43 = 43,
    ePalOverwriter_44 = 44,
    eType_45_EvilFart = 45,
    eBaseBomb_46 = 46,
    eType_47 = 47,
    eRockSpawner_48 = 48,
    eType_49 = 49,
    eFleech_50 = 50,
    ePossessionFlicker_51 = 51,
    eType_52 = 52,
    eThrowableTotalIndicator_53 = 53,
    eFlyingSlig_54 = 54,
    eFlyingSligSpawner_55 = 55,
    eFootSwitch_56 = 56,
    eType_57 = 57,
    eType_58 = 58,
    eType_59 = 59,
    eSlapLock_OrbWhirlWind_60 = 60,
    eLockedSoul_61 = 61,
    eDeathBird_62 = 62,
    eType_63 = 63,
    eGreeter_64 = 64,
    eGrenade_65 = 65,
    eGrenadeMachine_66 = 66,
    eGlukkon_67 = 67,
    eHelpPhone_68 = 68,
    eType_Abe_69 = 69,
    eType_70 = 70,
    eType_71 = 71,
    eType_72 = 72,
    eType_73 = 73,
    eEyeOrbPart_74 = 74,
    eInvisibleEffect_75 = 75,
    ePulley_76 = 76,
    eType_77 = 77,
    eLiftPoint_78 = 78,
    eResourceManager_70 = 79,
    eSligGetPantsOrWings_80 = 80,
    eMudokon2_81 = 81,
    eType_82 = 82,
    eAntiChant_83 = 83,
    eMeat_84 = 84,
    eMeatSack_85 = 85,
    eType_86 = 86,
    eText_87 = 87,
    eMine_88 = 88,
    eMineCar_89 = 89,
    eType_90 = 90,
    eGreeterBody_91 = 91,
    eType_92 = 92,
    eMusicController_93 = 93,
    eMusicTrigger_94 = 94,
    ePauseMenu_95 = 95,
    eParamite_96 = 96,
    eType_97 = 97,
    eDemoPlayback_98 = 98,
    eBirdPortal_99 = 99,
    eType_100 = 100,
    eFG1_101 = 101,
    eType_102 = 102,
    ePullRope_103 = 103,
    eType_104 = 104,
    eRock_105 = 105,
    eRockSack_106 = 106,
    eType_107 = 107,
    eLiftRope_108 = 108,
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
    eType_119 = 119,
    eType_120 = 120,
    eType_121 = 121,
    eSlamDoor_122 = 122,
    eType_123 = 123,
    eSnoozParticle_124 = 124,
    eSlig_125 = 125,
    eSlog_126 = 126,
    eType_127 = 127,
    eSlurgSpawner_128 = 128,
    eSlurg_129 = 129,
    eType_130 = 130,
    eType_131 = 131,
    eType_132 = 132,
    eType_133 = 133,
    eParticle_134 = 134,
    eZapLine_135 = 135,
    eTimerTrigger_136 = 136,
    eType_137 = 137,
    eType_138 = 138,
    eLever_139 = 139,
    eType_140 = 140,
    eTorturedMud_141 = 141,
    eTrapDoor_142 = 142,
    eUXB_143 = 143,
    eType_144 = 144,
    eMovie_145 = 145,
    eWeb_146 = 146,
    eWell_147 = 147,
    eWheel_148 = 148,
    eCameraSwapper_149 = 149,
    eElectrocute_150 = 150,

    eDebugHelper_1001 = 1001, // NOTE: Not part of the original game - a debug helper only
    eDebugConsole_1002 = 1002,
    eDebugPathRenderer_1003 = 1003,
};

class BaseGameObject
{
public:
    enum Options
    {
        // bit 00 = 0x001 = BaseGameObject couldn't add to list / it unsets inverse of 0xF80A / 0b1111100000001010
        eListAddFailed = 0x001,

        // bit 01 = 0x002 = do update ?
        eUpdatable = 0x002,

        // bit 02 = 0x004 = dead
        eDead = 0x004,

        // bit 03 = 0x008 = render
        eDrawable = 0x008,

        // bit 04 = 0x010 = set by BaseAnimatedWithPhysicsGameObject
        eIsBaseAnimatedWithPhysicsObj = 0x010,

        // bit 05 = 0x020 = set by BaseAliveGameObject
        eIsBaseAliveGameObject = 0x020,

        // bit 06 = 0x040 = set by ChantSuppressor::ctor_466350 / MovingBomb::ctor_46FD40 - explodable?
        eCanExplode = 0x040,

        // bit 07 = 0x080 = set by Uxb::ctor_4DE9A0 = pressable ?
        eInteractive = 0x080,

        // bit 08 = 0x100 = ?
        eSurviveDeathReset = 0x100,

        // bit 09 = 0x200 = still update when the camera is changing
        eUpdateDuringCamSwap = 0x200,

        // bit 10 = 0x400 = can never be removed
        eCantKill = 0x400,

        // bit 11 = 0x800 = ?
        eBit11 = 0x800,

        // bit 12 = 0x1000 = ?
        eBit12 = 0x1000,

    };

    // Order must match VTable
    virtual BaseGameObject* VDestructor(signed int) = 0; // Not an actual dtor because the generated compiler code has the param to determine if heap allocated or not
    virtual void VUpdate();
    virtual void VRender(int** pOrderingTable);
    virtual void VScreenChanged();
    virtual void VStopAudio();
    virtual int VGetSaveState(BYTE* pSaveBuffer);

    EXPORT void ScreenChanged_4DC0A0();

    EXPORT BYTE** Add_Resource_4DC130(DWORD type, int resourceID);
    EXPORT void BaseGameObject_ctor_4DBFA0(__int16 bAddToObjectList, signed __int16 resourceArraySize);
    EXPORT void BaseGameObject_dtor_4DBEC0();

    EXPORT static int CCSTD Find_Flags_4DC170(int objectId);

public:
    Types field_4_typeId;
    BitField16<Options> field_6_flags;
    int field_8_object_id;
    int field_C_objectId;
    DynamicArrayT<BYTE*> field_10_resources_array;
    int field_1C_update_delay;
};
ALIVE_ASSERT_SIZEOF(BaseGameObject, 0x20);


ALIVE_VAR_EXTERN(DynamicArrayT<BaseGameObject>*, gBaseGameObject_list_BB47C4);
