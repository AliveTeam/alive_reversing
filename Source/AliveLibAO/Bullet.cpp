#include "stdafx_ao.h"
#include "Function.hpp"
#include "Bullet.hpp"
#include "Map.hpp"
#include "stdlib.hpp"

START_NS_AO

EXPORT Bullet* Bullet::ctor_409380(BaseAliveGameObject* pParent, __int16 type, FP xpos, FP ypos, FP xDist, int a7, FP scale, __int16 a9)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BA328);
    field_4_typeId = Types::eBullet_10;
    field_10_type = type;
    field_18_xpos = xpos;
    field_1C_ypos = ypos;
    field_2A_path = gMap_507BA8.field_2_current_path;
    field_30_pParent = pParent;
    field_28_level = gMap_507BA8.field_0_current_level;
    field_2C_scale = scale;
    field_24 = a7;
    field_34 = a9;
    field_20 = xDist;
    field_12 = 0;
    return this;
}

BaseGameObject* Bullet::VDestructor(signed int flags)
{
    dtor_487DF0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

END_NS_AO
