#pragma once

#include "FunctionFwd.hpp"
#include "DynamicArray.hpp"
#include "BitField.hpp"

class BaseGameObject
{
public:
    enum Types : __int16
    {
        eNone_0 = 0,
        eScreenManager_6 = 6,
        eBackgroundAnimation_7 = 7,
        eTimedMine_10 = 10,
        eBrewMachine_13 = 13,
        eDDCheat_19 = 19,
        eType_23 = 23,
        eGrinder_30 = 30,
        eLaughingGas_31 = 31,
        eDoor_33 = 33,
        eDoorLock_34 = 34,
        eBird_35 = 35,
        eGameSpeak_38 = 38,
        eElectrocute_39 = 39,
        eType_45 = 45,
        eBaseBomb_46 = 46,
        eType_47 = 47,
        eRockSpawner_48 = 48,
        eFleech_50 = 50,
        eItemCount_53 = 53,
        eFlyingSlig_54 = 54,
        eLockedSoul_61 = 61,
        eGreeter_64 = 64,
        eGlukkon_67 = 67,
        eHelpPhone_68 = 68,
        eGrenadeMachine_66 = 66,
        eType_Abe_69 = 69,
        eInvisibleEffect_75 = 75,
        ePulley_76 = 76,
        eType_78 = 78,
        eResourceManager_70 = 79,
        eAntiChant_83 = 83,
        eMeat_84 = 84,
        eMeatSack_85 = 85,
        eText_87 = 87,
        eMine_88 = 88,
        eType_89 = 89,
        eGreeterBody_91 = 91,
        eMusicController_93 = 93,
        eMusicTrigger_94 = 94,
        ePauseMenu_95 = 95,
        eParamite_96 = 96,
        eFG1_101 = 101, 
        ePullRope_103 = 103,
        eType_104 = 104,
        eRock_105 = 105,
        eRockSack_106 = 106,
        eType_107 = 107,
        eType_109 = 109,
        eMudokon_110 = 110,
        eRedLaser_111 = 111,
        eScrab_112 = 112,
        eScreenClipper_114 = 114,
        eMainMenuTransistion_116 = 116,
        eScreenShake_118 = 118,
        eGate_122 = 122,
        eSnoozParticle_124 = 124,
        eSlig_125 = 125,
        eSlog_126 = 126,
        eSlurg_129 = 129,
        eParticle_134 = 134,
        eLever_139 = 139,
        eTrapDoor_142 = 142,
        eUXB_143 = 143,
        eWeb_146 = 146,
        eType_148 = 148,
        eCameraSwapper_149 = 149,
        eType_150 = 150,

        eDebugHelper_1001 = 1001, // NOTE: Not part of the original game - a debug helper only
        eDebugConsole_1002 = 1002,
        eDebugPathRenderer_1003 = 1003,
    };

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
        eBit08 = 0x100,

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
    virtual void VDestructor(signed int) = 0; // Not an actual dtor because the generated compiler code has the param to determine if heap allocated or not
    virtual void VUpdate();
    virtual void VRender(int** pOrderingTable);
    virtual void VScreenChanged();
    virtual void vnullsub_4DC0F0();
    virtual int GetSaveState_4DC110(BYTE* pSaveBuffer);

    EXPORT void ScreenChanged_4DC0A0();

    EXPORT BYTE** Add_Resource_4DC130(DWORD type, int resourceID);
    EXPORT void BaseGameObject_ctor_4DBFA0(__int16 bAddToObjectList, signed __int16 resourceArraySize);
    EXPORT void BaseGameObject_dtor_4DBEC0();

    EXPORT static int __stdcall Find_Flags_4DC170(int objectId);
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
