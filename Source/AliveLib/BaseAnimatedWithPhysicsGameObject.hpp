#pragma once

#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"
#include "FunctionFwd.hpp"
#include "Animation.hpp"

class Shadow;

class BaseAnimatedWithPhysicsGameObject : public BaseGameObject
{
public:
    BaseAnimatedWithPhysicsGameObject();
    EXPORT BaseAnimatedWithPhysicsGameObject * BaseAnimatedWithPhysicsGameObject_ctor_424930(signed __int16 resourceArraySize);
    EXPORT void BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
    EXPORT void dtor_408210(signed int flags);
    virtual void VDestructor(signed int flags);
    virtual void VUpdate() override;

    EXPORT void Update_424AB0();

    EXPORT void Animation_Init_424E10(int frameTableOffset, int maxW, unsigned __int16 maxH, BYTE **ppAnimData, __int16 a6, unsigned __int8 a7);

    virtual __int16 vsub_424EE0(int a2, int a3, int a4, int a5, void* a6);
    virtual DWORD* vsub_424FD0(__int16 a3, int a4);
    virtual __int16 vsub_4253B0(int a2, int a3);
    virtual __int16 vsub_425420(int a2);
    virtual __int16 vsub_4254A0(int a2);
    virtual __int16 vsub_425520(int a2);
    virtual int vsub_425840(unsigned __int16 a2);
    virtual void vnull_408180();
    virtual void vnull_4081A0();

    EXPORT __int16 sub_424EE0(int a2, int a3, int a4, int a5, void* a6);
    EXPORT DWORD* sub_424FD0(__int16 a3, int a4);
    EXPORT __int16 sub_4253B0(int a2, int a3);
    EXPORT __int16 sub_425420(int a2);
    EXPORT __int16 sub_4254A0(int a2);
    EXPORT __int16 sub_425520(int a2);
    EXPORT int sub_425840(unsigned __int16 a2);
    EXPORT void null_408180();
    EXPORT void null_4081A0();
public:

    AnimationEx field_20_animation;
    FP field_B8_xpos;
    FP field_BC_ypos;
    __int16 field_C0_path_number;
    __int16 field_C2_lvl_number;
    FP field_C4_velx;
    FP field_C8_vely;
    FP field_CC_sprite_scale;
    __int16 field_D0_r;
    __int16 field_D2_g;
    __int16 field_D4_b;
    __int16 field_D6_scale;
    __int16 field_D8_yOffset;
    __int16 field_DA_xOffset;
    __int16 field_DC_bApplyShadows;
    __int16 field_DE_pad;
    Shadow* field_E0_176_ptr;
};
ALIVE_ASSERT_SIZEOF(BaseAnimatedWithPhysicsGameObject, 0xE4);
