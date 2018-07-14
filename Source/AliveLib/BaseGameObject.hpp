#pragma once

#include "FunctionFwd.hpp"
#include "DynamicArray.hpp"

class BaseGameObject
{
public:
    enum Types : __int16
    {
        eNone = 0,
        eBackgroundAnimation = 7,
        eDDCheat = 19,
        eGameSpeak = 38,
        eType_67 = 67,
        eType_69 = 69,
        eResourceManager = 79,
        ePauseMenu = 95,
        eType_112 = 112,
        eType_125 = 125,
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
        eBit04 = 0x010,

        // bit 05 = 0x020 = set by BaseAliveGameObject
        eIsBaseAliveGameObject = 0x020,

        // bit 06 = 0x040 = set by ChantSuppressor::ctor_466350 / MovingBomb::ctor_46FD40 - explodable?
        eCanExplode = 0x040,

        // bit 07 = 0x080 = set by Uxb::ctor_4DE9A0 = pressable ?
        eInteractive = 0x080,

        // bit 08 = 0x100 = ?
        eBit08 = 0x100,

        // bit 09 = 0x200 = still update in some circumstance ? when word_5C1B66 is 0
        eUpdatableExtra = 0x200,

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
    EXPORT virtual void vsub_4DC0A0();
    virtual void vnullsub_4DC0F0();
    virtual int GetSaveState_4DC110(BYTE* pSaveBuffer);


    EXPORT BYTE ** BaseGameObject::Add_Resource_4DC130(int type, int resourceID);
    EXPORT void BaseGameObject_ctor_4DBFA0(__int16 bAddToObjectList, signed __int16 resourceArraySize);
    EXPORT void BaseGameObject_dtor_4DBEC0();

public:
    Types field_4_typeId;
    __int16 field_6_flags;
    int field_8_flags_ex;
    int field_C_objectId;
    DynamicArray field_10_resources_array;
    int field_1C_update_delay;
};
ALIVE_ASSERT_SIZEOF(BaseGameObject, 0x20);

ALIVE_VAR_EXTERN(DynamicArrayT<BaseGameObject>*, gBaseGameObject_list_BB47C4);

//class BaseAnimatedWithPhysicsGameObject : public BaseGameObject
//{
//public:
//    virtual void VDestructor(signed int flags);
//    virtual void VUpdate() override;
//
//    EXPORT void dtor_424A40(char flags);
//    EXPORT void dtor_424AD0();
//    EXPORT void vnull_update_424AB0();
//
//public:
//    AnimationEx field_20_animation;
//    int field_B8_xpos;
//    int field_BC_ypos;
//    __int16 field_C0_path_number;
//    __int16 field_C2_lvl_number;
//    int field_C4_velx;
//    int field_C8_vely;
//    int field_CC_sprite_scale;
//    __int16 field_D0_r;
//    __int16 field_D2_g;
//    __int16 field_D4_b;
//    __int16 field_D6_scale;
//    __int16 field_D8_yOffset;
//    __int16 field_DA_xOffset;
//    __int16 field_DC_bApplyShadows;
//    __int16 field_DE_pad;
//    int field_E0_176_ptr;
//};
//ALIVE_ASSERT_SIZEOF(BaseAnimatedWithPhysicsGameObject, 0xE4);

class Class_5C1B70
{
public:
    EXPORT void sub_449C10(int objCount, void* pGameObj);
};

