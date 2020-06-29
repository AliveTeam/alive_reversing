#include "stdafx_ao.h"
#include "Function.hpp"
#include "ScreenShake.hpp"
#include "Events.hpp"
#include "Game.hpp"
#include "stdlib.hpp"

START_NS_AO

ScreenShake* ScreenShake::ctor_4624D0(__int16 enableShakeEvent)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BC950);

    field_4_typeId = Types::ScreenShake_82;

    field_6_flags.Set(Options::eDrawable_Bit4);

    field_30_shakeNumber = 16;
    field_32_enableShakeEvent = enableShakeEvent;

    gObjList_drawables_504618->Push_Back(this);

    if (field_32_enableShakeEvent)
    {
        Event_Broadcast_417220(kEventScreenShake_19, this);
    }

    return this;
}

void ScreenShake::VUpdate()
{
    VUpdate_4625B0();
}

void ScreenShake::VUpdate_4625B0()
{
    if (field_32_enableShakeEvent)
    {
        Event_Broadcast_417220(kEventScreenShake_19, this);
    }

    if (field_30_shakeNumber > 0)
    {
        field_30_shakeNumber--;
    }
}

BaseGameObject* ScreenShake::VDestructor(signed int flags)
{
    return Vdtor_462770(flags);
}

ScreenShake* ScreenShake::Vdtor_462770(signed int flags)
{
    dtor_462550();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* ScreenShake::dtor_462550()
{
    SetVTable(this, 0x4BC950);
    gObjList_drawables_504618->Remove_Item(this);
    return dtor_487DF0();
}

END_NS_AO
