#include "stdafx_ao.h"
#include "Function.hpp"
#include "ThrowableTotalIndicator.hpp"
#include "stdlib.hpp"
#include "Game.hpp"

START_NS_AO

ALIVE_VAR(1, 0x504C70, WORD, bThrowableIndicatorExists_504C70, 0);

void ThrowableTotalIndicator::VScreenChanged()
{
    VScreenChanged_41BB70();
}

BaseGameObject* ThrowableTotalIndicator::Vdtor_41BB80(signed int flags)
{
    dtor_41B610();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* ThrowableTotalIndicator::VDestructor(signed int flags)
{
    return Vdtor_41BB80(flags);
}

BaseGameObject* ThrowableTotalIndicator::dtor_41B610()
{
    SetVTable(this, 0x4BAED8);

    if (field_6_flags.Get(BaseGameObject::eDrawable_Bit4))
    {
        gObjList_drawables_504618->Remove_Item(this);
    }

    if (field_19E_bFade)
    {
        bThrowableIndicatorExists_504C70--;
    }

    return dtor_487DF0();
}

void ThrowableTotalIndicator::VScreenChanged_41BB70()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void ThrowableTotalIndicator::VUpdate()
{
    VUpdate_41B690();
}

void ThrowableTotalIndicator::VUpdate_41B690()
{
    NOT_IMPLEMENTED();
}

void ThrowableTotalIndicator::VRender(int** ppOt)
{
    VRender_41B810(ppOt);
}

void ThrowableTotalIndicator::VRender_41B810(int** /*ppOt*/)
{
    NOT_IMPLEMENTED();
}

ThrowableTotalIndicator* ThrowableTotalIndicator::ctor_41B520(FP xpos, FP ypos, __int16 layer, FP /*scale*/, __int16 count, __int16 bFade)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BAED8);

    field_6_flags.Set(Options::eDrawable_Bit4);
    field_4_typeId = Types::eThrowableTotalIndicator_35;

    gObjList_drawables_504618->Push_Back(this);

    field_10_xpos = xpos;
    field_14_ypos = ypos;

    field_18_cur_xpos = xpos;
    field_1C_cur_ypos = ypos;

    field_20_xspeed = FP_FromInteger(0);

    field_19E_bFade = bFade;

    if (bFade)
    {
        field_24_yspeed = FP_FromDouble(-0.7);
    }
    else
    {
        field_24_yspeed = FP_FromInteger(0);
    }

    field_28_scale = FP_FromInteger(1);  // OG bug - should be using scale ??
    field_2C_scale_speed = FP_FromInteger(0);
    field_30_layer = layer;

    field_32_r = 0;
    field_34_g = 0;
    field_36_b = 0;

    if (bFade)
    {
        field_19C_state = 1;
    }
    else
    {
        field_19C_state = 0;
    }

    if (count == -1)
    {
        field_38_num_to_show = 10;
    }
    else
    {
        field_38_num_to_show = count;
    }

    if (bFade)
    {
        bThrowableIndicatorExists_504C70++;
    }

    return this;
}

END_NS_AO

