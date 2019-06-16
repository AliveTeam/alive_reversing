#include "stdafx.h"
#include "Bullet.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "stdlib.hpp"

Bullet* Bullet::ctor_414540(BaseAliveGameObject* pParent, __int16 type, FP xpos, FP ypos, FP xDist, int a7, FP scale, __int16 a9)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x54446C);
    field_4_typeId = Types::eBullet_15;
    field_20_type = type;
    field_28_xpos = xpos;
    field_2C_ypos = ypos;
    field_3A_path = gMap_5C3030.sCurrentPathId_5C3032;
    field_40_pParent = pParent;
    field_38_level = gMap_5C3030.sCurrentLevelId_5C3030;
    field_3C_scale = scale;
    field_34 = a7;
    field_44 = a9;
    field_30 = xDist;
    field_22 = 0;
    return this;
}

BaseGameObject* Bullet::VDestructor(signed int flags)
{
    return vdtor_4145E0(flags);
}

Bullet* Bullet::vdtor_4145E0(signed int flags)
{
    BaseGameObject_dtor_4DBEC0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}
