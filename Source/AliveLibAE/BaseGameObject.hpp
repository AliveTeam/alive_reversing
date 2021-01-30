#pragma once

#include "FunctionFwd.hpp"
#include "DynamicArray.hpp"
#include "BitField.hpp"

ALIVE_VAR_EXTERN(unsigned int, sGnFrame_5C1B84);

enum class Types : __int16
{
    eNone_0 = 0,
    eAlarm_1 = 1,
    eSligSpawner_2 = 2,
    ePadding_3 = 3,
    ePadding_4 = 4,
    ePadding_5 = 5,
    eScreenManager_6 = 6,
    eBackgroundAnimation_7 = 7,
    eType_8 = 8, // TODO: gets checked but unused??
    eLiftMover_9 = 9,
    eTimedMine_or_MovingBomb_10 = 10,
    eBone_11 = 11,
    eBoneBag_12 = 12,
    eBrewMachine_13 = 13,
    ePadding_14 = 14,
    eBullet_15 = 15,
    eSligButton_16 = 16,
    ePadding_17 = 17,
    eExplosionSet_18 = 18,
    eDDCheat_19 = 19,
    ePadding_20 = 20,
    ePadding_21 = 21,
    eSparks_22 = 22,
    eGasClock_23 = 23,
    eMetal_24 = 24,
    eMinesAlarm_25 = 25,
    eCrawlingSlig_26 = 26,
    ePadding_27 = 27,
    ePadding_28 = 28,
    eParticleBurst_29 = 29,
    eGrinder_30 = 30,
    eLaughingGas_31 = 31,
    ePadding_32 = 32,
    eDoor_33 = 33,
    eDoorLock_34 = 34,
    eBird_35 = 35,
    ePadding_36 = 36,
    ePadding_37 = 37,
    eGameSpeak_38 = 38,
    eElectricWall_39 = 39,
    eType_40 = 40, //TODO: gets checked but unused ??
    ePadding_41 = 41,
    ePadding_42 = 42,
    ePadding_43 = 43,
    ePalOverwriter_44 = 44,
    eEvilFart_45 = 45,
    eBaseBomb_46 = 46,
    eSecurityClaw_47 = 47,
    eRockSpawner_48 = 48,
    ePadding_49 = 49,
    eFleech_50 = 50,
    ePossessionFlicker_51 = 51,
    ePadding_52 = 52,
    eThrowableTotalIndicator_53 = 53,
    eFlyingSlig_54 = 54,
    eFlyingSligSpawner_55 = 55,
    eFootSwitch_56 = 56,
    eGameEnderController_57 = 57,
    ePadding_58 = 58,
    eColourfulMeter_59 = 59,
    eSlapLock_OrbWhirlWind_60 = 60,
    eLockedSoul_61 = 61,
    eDeathBird_62 = 62,
    ePadding_63 = 63,
    eGreeter_64 = 64,
    eGrenade_65 = 65,
    eGrenadeMachine_66 = 66,
    eGlukkon_67 = 67,
    eHelpPhone_68 = 68,
    eAbe_69 = 69,
    ePadding_70 = 70,
    ePadding_71 = 71,
    ePadding_72 = 72,
    ePadding_73 = 73,
    eEyeOrbPart_74 = 74,
    eInvisibleEffect_75 = 75,
    ePulley_76 = 76,
    ePadding_77 = 77,
    eLiftPoint_78 = 78,
    eResourceManager_70 = 79,
    eSligGetPantsOrWings_80 = 80,
    eMudokon2_81 = 81,
    ePadding_82 = 82,
    eSecurityOrb_83 = 83,
    eMeat_84 = 84,
    eMeatSack_85 = 85,
    eMeatSaw_86 = 86, // from AO
    eText_87 = 87,
    eMine_88 = 88,
    eMineCar_89 = 89,
    ePadding_90 = 90,
    eGreeterBody_91 = 91,
    ePadding_92 = 92,
    eMusicController_93 = 93,
    eMusicTrigger_94 = 94,
    ePauseMenu_95 = 95,
    eParamite_96 = 96,
    ePadding_97 = 97,
    eDemoPlayback_98 = 98,
    eBirdPortal_99 = 99,
    eBirdPortalTerminator_100 = 100,
    eFG1_101 = 101,
    ePadding_102 = 102,
    ePullRope_103 = 103,
    eAbilityRing_104 = 104,
    eRock_105 = 105,
    eRockSack_106 = 106,
    eType_107 = 107, // taking damage from something in AO? maybe bees, bats etc?
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
    ePadding_119 = 119,
    ePadding_120 = 120,
    eShrykull_121 = 121,
    eSlamDoor_122 = 122,
    ePadding_123 = 123,
    eSnoozeParticle_124 = 124,
    eSlig_125 = 125,
    eSlog_126 = 126,
    eType_127 = 127, // TODO: gets checked but unused??
    eSlurgSpawner_128 = 128,
    eSlurg_129 = 129,
    ePadding_130 = 130,
    ePadding_131 = 131,
    ePadding_132 = 132,
    ePadding_133 = 133,
    eParticle_134 = 134,
    eZapLine_135 = 135,
    eTimerTrigger_136 = 136,
    ePadding_137 = 137,
    ePadding_138 = 138,
    eLever_139 = 139,
    ePadding_140 = 140,
    eTorturedMud_141 = 141,
    eTrapDoor_142 = 142,
    eUXB_143 = 143,
    eType_144 = 144,
    eMovie_145 = 145,
    eWebLine_146 = 146,
    eWell_147 = 147,
    eWheel_148 = 148,
    eCameraSwapper_149 = 149,
    eElectrocute_150 = 150,

    eDebugHelper_1001 = 1001, // NOTE: Not part of the original game - a debug helper only
    eDebugConsole_1002 = 1002,
    eDebugPathRenderer_1003 = 1003,
};

struct PrimHeader;

class BaseGameObject
{
public:
    enum Options
    {
        // bit 01 = 0x001 = BaseGameObject couldn't add to list / it unsets inverse of 0xF80A / 0b1111100000001010
        eListAddFailed_Bit1 = 0x001,

        // bit 02 = 0x002 = do update?
        eUpdatable_Bit2 = 0x002,

        // bit 03 = 0x004 = dead
        eDead_Bit3 = 0x004,

        // bit 04 = 0x008 = render
        eDrawable_Bit4 = 0x008,

        // bit 05 = 0x010 = set by BaseAnimatedWithPhysicsGameObject
        eIsBaseAnimatedWithPhysicsObj_Bit5 = 0x010,

        // bit 06 = 0x020 = set by BaseAliveGameObject
        eIsBaseAliveGameObject_Bit6 = 0x020,

        // bit 07 = 0x040 = set by ChantSuppressor::ctor_466350 / MovingBomb::ctor_46FD40 - explodable?
        eCanExplode_Bit7 = 0x040,

        // bit 08 = 0x080 = set by Uxb::ctor_4DE9A0 = pressable?
        eInteractive_Bit8 = 0x080,

        // bit 09 = 0x100 = ?
        eSurviveDeathReset_Bit9 = 0x100,

        // bit 10 = 0x200 = still update when the camera is changing
        eUpdateDuringCamSwap_Bit10 = 0x200,

        // bit 11 = 0x400 = can never be removed
        eCantKill_Bit11 = 0x400
    };

    // Order must match VTable
    virtual BaseGameObject* VDestructor(signed int) = 0; // Not an actual dtor because the generated compiler code has the param to determine if heap allocated or not
    virtual void VUpdate();
    virtual void VRender(PrimHeader** pOrderingTable);
    virtual void VScreenChanged();
    virtual void VStopAudio();
    virtual int VGetSaveState(BYTE* pSaveBuffer);

    EXPORT void ScreenChanged_4DC0A0();

    EXPORT BYTE** Add_Resource_4DC130(DWORD type, int resourceID);
    EXPORT void BaseGameObject_ctor_4DBFA0(__int16 bAddToObjectList, signed __int16 resourceArraySize);
    EXPORT void BaseGameObject_dtor_4DBEC0();

    EXPORT static int CCSTD Find_Flags_4DC170(int objectId);

protected:
    // Helper to check if a timer has expired
    template<class T>
    static inline bool Expired(const T& value)
    {
        return static_cast<int>(sGnFrame_5C1B84) > value;
    }

    template<class T>
    static int MakeTimer(const T value)
    {
        return static_cast<int>(sGnFrame_5C1B84) + value;
    }

public:
    Types field_4_typeId;
    BitField16<Options> field_6_flags;
    int field_8_object_id;
    __int32 field_C_objectId;
    DynamicArrayT<BYTE*> field_10_resources_array;
    int field_1C_update_delay;
};
ALIVE_ASSERT_SIZEOF(BaseGameObject, 0x20);


ALIVE_VAR_EXTERN(DynamicArrayT<BaseGameObject>*, gBaseGameObject_list_BB47C4);
