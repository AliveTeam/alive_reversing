#include "stdafx.h"
#include "OrbWhirlWind.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "stdlib.hpp"

BaseGameObject* OrbWhirlWind::VDestructor(signed int flags)
{
    return vdtor_4E3D50(flags);
}

OrbWhirlWind* OrbWhirlWind::ctor_4E3C90(FP xpos, FP ypos, FP scale, __int16 bUnknown)
{
    field_4_typeId = Types::eNone_0;

    BaseGameObject_ctor_4DBFA0(TRUE, 0);

    SetVTable(this, 0x5480B8); // vTbl_OrbWhirlWind_5480B8


    field_6C_xpos = xpos;
    field_70_ypos = ypos;
    field_74_scale = scale;

    field_24 = 0;
    field_20 = 0;

    field_26_bUnknown = bUnknown;
    gObjList_drawables_5C1124->Push_Back(this);
    field_6_flags.Set(BaseGameObject::eDrawable);

    field_28_obj_array_idx = 0;
    memset(field_2C_objArray, 0, sizeof(field_2C_objArray));

    return this;
}

void OrbWhirlWind::sub_4E3FD0(FP /*xpos*/, FP /*ypos*/, FP /*scale*/, BaseGameObject* /*pObj*/)
{
    NOT_IMPLEMENTED();
}

void OrbWhirlWind::sub_4E4050()
{
    NOT_IMPLEMENTED();
}

void OrbWhirlWind::dtor_4E3D80()
{
    NOT_IMPLEMENTED();
}

OrbWhirlWind* OrbWhirlWind::vdtor_4E3D50(signed int flags)
{
    dtor_4E3D80();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}
