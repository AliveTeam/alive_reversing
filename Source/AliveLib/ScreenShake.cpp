#include "stdafx.h"
#include "ScreenShake.hpp"
#include "Function.hpp"
#include "Events.hpp"
#include "stdlib.hpp"
#include "Game.hpp"

EXPORT ScreenShake* ScreenShake::ctor_4ACF70(__int16 a2, __int16 a3)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    field_6_flags.Set(BaseGameObject::eDrawable);

    SetVTable(this, 0x547070); // vTbl_ScreenShake_547070

    field_4_typeId = Types::eScreenShake_118;
    field_44 = a3;
    field_40 = 16;
    field_42 = a2;

    gObjList_drawables_5C1124->Push_Back(this);

    if (!field_42)
    {
        return this;
    }

    if (!field_44)
    {
        Event_Broadcast_422BC0(kEventScreenShake, this);
    }

    return this;
}

EXPORT void ScreenShake::dtor_4AD060()
{
    SetVTable(this, 0x547070); // vTbl_ScreenShake_547070

    gObjList_drawables_5C1124->Remove_Item(this);
    BaseGameObject_dtor_4DBEC0();
}

EXPORT void ScreenShake::vUpdate_4AD0E0()
{
    if (field_42)
    {
        if (!field_44)
        {
            Event_Broadcast_422BC0(kEventScreenShake, this);
        }
    }

    if (field_40 > 0)
    {
        field_40--;
    }
}

EXPORT BaseGameObject* ScreenShake::vdtor_4AD030(signed int flags)
{
    dtor_4AD060();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

EXPORT void ScreenShake::vRender_4AD120(int** /*pOt*/)
{
    NOT_IMPLEMENTED();
}

BaseGameObject* ScreenShake::VDestructor(signed int flags)
{
    return vdtor_4AD030(flags);
}

void ScreenShake::VUpdate()
{
    vUpdate_4AD0E0();
}

void ScreenShake::VRender(int** pOt)
{
    vRender_4AD120(pOt);
}
