#pragma once

#include "FunctionFwd.hpp"
#include "DynamicArray.hpp"

class BaseGameObject
{
public:
    // Order must match VTable
    virtual void VDestructor(signed int) = 0; // Not an actual dtor because the generated compiler code has the param to determine if heap allocated or not
    virtual void VUpdate();
    virtual void VRender(int** pOrderingTable);
    EXPORT virtual void vsub_4DC0A0();
    virtual void vnullsub_4DC0F0();
    virtual int GetSaveState_4DC110(BYTE* pSaveBuffer);

    EXPORT void BaseGameObject_ctor_4DBFA0(__int16 bAddToObjectList, signed __int16 resourceArraySize);
    EXPORT void BaseGameObject_dtor_4DBEC0();

public:
    __int16 field_4_typeId;
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

