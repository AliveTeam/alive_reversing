#include "stdafx.h"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Function.hpp"
#include "Map.hpp"

BaseAnimatedWithPhysicsGameObject::BaseAnimatedWithPhysicsGameObject(signed __int16 resourceArraySize)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(resourceArraySize);
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

    field_6_flags &= ~(BaseGameObject::eInteractive | BaseGameObject::eCanExplode);
    field_6_flags |= BaseGameObject::eDrawable | BaseGameObject::eIsBaseAnimatedWithPhysicsObj;

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

void BaseAnimatedWithPhysicsGameObject::dtor_408210(signed int flags)
{
    NOT_IMPLEMENTED();
}

void BaseAnimatedWithPhysicsGameObject::VDestructor(signed int flags)
{
    dtor_408210(flags);
}

signed __int16 BaseAnimatedWithPhysicsGameObject::Animation_Init_424E10(int frameTableOffset, int maxW, unsigned __int16 maxH, AnimHeader * a5, __int16 a6, unsigned __int8 a7)
{
    NOT_IMPLEMENTED();
    return 0;
}
