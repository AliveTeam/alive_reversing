#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "../AliveLibCommon/BitField.hpp"

struct PrimHeader;

class BaseGameObjectCommon
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

//     // Order must match VTable
//     virtual BaseGameObjectCommon* VDestructor(s32) = 0; // Not an actual dtor because the generated compiler code has the param to determine if heap allocated or not
//     virtual void VUpdate();
//     virtual void VRender(PrimHeader** pOrderingTable);
//     virtual void VScreenChanged();
//     virtual void VStopAudio();
//     virtual s32 VGetSaveState(u8* pSaveBuffer);

//     EXPORT void ScreenChanged_4DC0A0();

//     EXPORT u8** Add_Resource_4DC130(u32 type, s32 resourceID);
//     EXPORT void BaseGameObjectCommon_ctor_4DBFA0(s16 bAddToObjectList, s16 resourceArraySize);
//     EXPORT void BaseGameObjectCommon_dtor_4DBEC0();

//     EXPORT static s32 CCSTD Find_Flags_4DC170(s32 objectId);

// protected:
//     // Helper to check if a timer has expired
//     template <class T>
//     static inline bool Expired(const T& value)
//     {
//         return static_cast<s32>(sGnFrame_5C1B84) > value;
//     }

//     template <class T>
//     static s32 MakeTimer(const T value)
//     {
//         return static_cast<s32>(sGnFrame_5C1B84) + value;
//     }

// public:
//     AETypes field_4_typeId;
//     BitField16<Options> field_6_flags;
//     s32 field_8_object_id;
//     s32 field_C_objectId;
//     DynamicArrayT<u8*> field_10_resources_array;
//     s32 field_1C_update_delay;
};
