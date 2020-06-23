#include "stdafx_ao.h"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Function.hpp"
#include "Map.hpp"

START_NS_AO

BaseAnimatedWithPhysicsGameObject* BaseAnimatedWithPhysicsGameObject::ctor_417C10()
{
    ctor_487E10(1);

    SetVTable(&field_10_anim, 0x4BA2B8);
    SetVTable(this, 0x4BAA38);

    field_CC |= 1u;
    field_B0_path_number = gMap_507BA8.field_2_current_path;
    field_B2_lvl_number = gMap_507BA8.field_0_current_level;

    field_B4_velx = FP_FromInteger(0);
    field_A8_xpos = FP_FromInteger(0);
    field_B8_vely = FP_FromInteger(0);
    field_AC_ypos = FP_FromInteger(0);
    field_CA = 0;
    field_D0_pShadow = nullptr;
    field_C4_b = 105;
    field_C2_g = 105;
    field_C0_r = 105;
    field_BC_sprite_scale = FP_FromInteger(1);
    field_C6_scale = 1;
    field_C8 = 5;

    field_6_flags.Clear(Options::eCanExplode_Bit7);
    field_6_flags.Clear(Options::eInteractive_Bit8);

    field_6_flags.Set(Options::eIsBaseAnimatedWithPhysicsObj_Bit5);
    field_6_flags.Set(Options::eDrawable_Bit4);

    return this;
}

void BaseAnimatedWithPhysicsGameObject::Animation_Init_417FD0(int /*frameTableOffset*/, int /*maxW*/, int /*maxH*/, BYTE** /*ppAnimData*/, __int16 /*a6*/)
{
    NOT_IMPLEMENTED();
}

void BaseAnimatedWithPhysicsGameObject::VRender_417DA0(int** /*ot*/)
{
    NOT_IMPLEMENTED();
}

void BaseAnimatedWithPhysicsGameObject::SetTint_418750(const TintEntry* /*pTintArray*/, __int16 /*levelId*/)
{
    NOT_IMPLEMENTED();
}

BaseGameObject* BaseAnimatedWithPhysicsGameObject::dtor_417D10()
{
    NOT_IMPLEMENTED();
    return this;
}

__int16 BaseAnimatedWithPhysicsGameObject::SetBaseAnimPaletteTint_4187C0(TintEntry* /*pTintArray*/, LevelIds /*level_id*/, int /*resourceID*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

void BaseAnimatedWithPhysicsGameObject::VStackOnObjectsOfType_418930(unsigned __int16 /*typeToFind*/)
{
    NOT_IMPLEMENTED();
}

END_NS_AO


