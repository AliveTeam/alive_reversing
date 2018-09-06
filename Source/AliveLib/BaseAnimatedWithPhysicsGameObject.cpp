#include "stdafx.h"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Game.hpp"

BaseAnimatedWithPhysicsGameObject::BaseAnimatedWithPhysicsGameObject()
{
    LOG_WARNING("Exists only to make things build - func may break behaviour of objects");
}

BaseAnimatedWithPhysicsGameObject * BaseAnimatedWithPhysicsGameObject::BaseAnimatedWithPhysicsGameObject_ctor_424930(signed __int16 resourceArraySize)
{
    BaseGameObject_ctor_4DBFA0(1, resourceArraySize);

    SetVTable(&field_20_animation, 0x544290); // gVtbl_animation_2a_544290
    SetVTable(this, 0x544C9C); // gVtbl_BaseAnimatedWithPhysicsGameObject_544C9C

    field_C4_velx = 0;
    field_C8_vely = 0;

    field_B8_xpos = FP(0);
    field_BC_ypos = FP(0);

    field_D4_b = 127;
    field_D2_g = 127;
    field_D0_r = 127;

    field_6_flags.Clear(BaseGameObject::eInteractive);
    field_6_flags.Clear(BaseGameObject::eCanExplode);

    field_6_flags.Set(BaseGameObject::eDrawable);
    field_6_flags.Set(BaseGameObject::eIsBaseAnimatedWithPhysicsObj);

    field_DC_bApplyShadows &= ~2;
    field_DC_bApplyShadows |= 1;

    field_C0_path_number = gMap_5C3030.sCurrentPathId_5C3032;
    field_C2_lvl_number = gMap_5C3030.sCurrentLevelId_5C3030;

    field_CC_sprite_scale = FP(1);

    field_D6_scale = 1;
    field_D8_yOffset = 0;
    field_DA_xOffset = 0;

    field_E0_176_ptr = nullptr;

    return this;
}

void BaseAnimatedWithPhysicsGameObject::dtor_408210(signed int /*flags*/)
{
    NOT_IMPLEMENTED();
}

void BaseAnimatedWithPhysicsGameObject::VDestructor(signed int flags)
{
    dtor_408210(flags);
}

void BaseAnimatedWithPhysicsGameObject::VUpdate()
{
    Update_424AB0();
}

void BaseAnimatedWithPhysicsGameObject::Update_424AB0()
{
    // Empty
}

void BaseAnimatedWithPhysicsGameObject::Animation_Init_424E10(int frameTableOffset, int maxW, unsigned __int16 maxH, BYTE **ppAnimData, __int16 a6, unsigned __int8 a7)
{
    if (field_20_animation.Init_40A030(
        frameTableOffset,
        gObjList_animations_5C1A24,
        this,
        maxW,
        maxH,
        ppAnimData,
        a7,
        0,
        0))
    {
        if (field_CC_sprite_scale.fpValue == 0x10000)
        {
            field_20_animation.field_C_render_layer = 27;
        }
        else
        {
            field_20_animation.field_C_render_layer = 8;
            field_D6_scale = 0;
        }

        if (!a6 || (gObjList_drawables_5C1124->Push_Back_40CAF0(this)))
        {
            field_20_animation.field_B_render_mode = 0;

            // TODO: Double check this logic
            DWORD animationFlags = field_20_animation.field_4_flags & 0xFFFF;
            animationFlags &= ~0xC000;
            animationFlags |= 0x4000;
            field_20_animation.field_4_flags &= 0xFFFFF0000;
            field_20_animation.field_4_flags |= animationFlags;
        }
        else
        {
            field_6_flags.Set(BaseGameObject::eDead);
            field_6_flags.Set(BaseGameObject::eListAddFailed);
        }
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eDead);
        field_6_flags.Set(BaseGameObject::eListAddFailed);
    }

}

__int16 BaseAnimatedWithPhysicsGameObject::vsub_424EE0(int a2, int a3, int a4, int a5, void* a6)
{
    return sub_424EE0(a2, a3, a4, a5, a6);
}

DWORD* BaseAnimatedWithPhysicsGameObject::vsub_424FD0(__int16 a3, int a4)
{
    return sub_424FD0(a3, a4);
}

__int16 BaseAnimatedWithPhysicsGameObject::vsub_4253B0(int a2, int a3)
{
    return sub_4253B0(a2, a3);
}

__int16 BaseAnimatedWithPhysicsGameObject::vsub_425420(int a2)
{
    return sub_425420(a2);
}

__int16 BaseAnimatedWithPhysicsGameObject::vsub_4254A0(int a2)
{
    return sub_4254A0(a2);
}

__int16 BaseAnimatedWithPhysicsGameObject::vsub_425520(int a2)
{
    return sub_425520(a2);
}

int BaseAnimatedWithPhysicsGameObject::vsub_425840(unsigned __int16 a2)
{
    return sub_425840(a2);
}

void BaseAnimatedWithPhysicsGameObject::vnull_408180()
{
    null_408180();
}

void BaseAnimatedWithPhysicsGameObject::vnull_4081A0()
{
    null_4081A0();
}

__int16 BaseAnimatedWithPhysicsGameObject::sub_424EE0(int /*a2*/, int /*a3*/, int /*a4*/, int /*a5*/, void* /*a6*/)
{
    NOT_IMPLEMENTED();
}

DWORD* BaseAnimatedWithPhysicsGameObject::sub_424FD0(__int16 /*a3*/, int /*a4*/)
{
    NOT_IMPLEMENTED();
}

__int16 BaseAnimatedWithPhysicsGameObject::sub_4253B0(int /*a2*/, int /*a3*/)
{
    NOT_IMPLEMENTED();
}

__int16 BaseAnimatedWithPhysicsGameObject::sub_425420(int /*a2*/)
{
    NOT_IMPLEMENTED();
}

__int16 BaseAnimatedWithPhysicsGameObject::sub_4254A0(int /*a2*/)
{
    NOT_IMPLEMENTED();
}

__int16 BaseAnimatedWithPhysicsGameObject::sub_425520(int /*a2*/)
{
    NOT_IMPLEMENTED();
}

int BaseAnimatedWithPhysicsGameObject::sub_425840(unsigned __int16 /*a2*/)
{
    NOT_IMPLEMENTED();
}

void BaseAnimatedWithPhysicsGameObject::null_408180()
{
    NOT_IMPLEMENTED();
}

void BaseAnimatedWithPhysicsGameObject::null_4081A0()
{
    NOT_IMPLEMENTED();
}
