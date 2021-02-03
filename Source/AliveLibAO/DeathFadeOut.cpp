#include "stdafx_ao.h"
#include "Function.hpp"
#include "DeathFadeOut.hpp"
#include "stdlib.hpp"

namespace AO {

void DeathFadeOut::VRender_419ED0(PrimHeader** ppOt)
{
    field_62_b = field_68_current_fade_rgb;
    field_60_g = field_68_current_fade_rgb;
    field_5E_r = field_68_current_fade_rgb;

    EffectBase::VRender_461690(ppOt);

    if (field_68_current_fade_rgb == 255 && field_6C_direction || field_68_current_fade_rgb == 0 && !field_6C_direction)
    {
        field_6E_bDone = 1;
        if (field_70_destroy_on_done)
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }
}

void DeathFadeOut::VRender(PrimHeader** ppOt)
{
    VRender_419ED0(ppOt);
}

void DeathFadeOut::VUpdate_419E90()
{
    if (!field_6E_bDone && !field_72)
    {
        field_68_current_fade_rgb += field_6A_speed;
        if (field_6C_direction)
        {
            if (field_68_current_fade_rgb > 255)
            {
                field_68_current_fade_rgb = 255;
            }
        }
        else if (field_68_current_fade_rgb < 0)
        {
            field_68_current_fade_rgb = 0;
        }
    }
}

void DeathFadeOut::VUpdate()
{
    VUpdate_419E90();
}

void DeathFadeOut::VScreenChanged()
{
    // Empty
}

void DeathFadeOut::Init_419E40(Layer layer, __int16 direction, __int16 destroyOnDone, int speed)
{
    field_5C_layer = layer;
    field_6C_direction = direction;
    field_6E_bDone = 0;

    if (speed > 0)
    {
        field_72 = 0;
    }
    else
    {
        field_72 = 1;
    }
    field_70_destroy_on_done = destroyOnDone;

    if (direction == 0)
    {
        field_6A_speed = static_cast<short>(-speed);
    }
    else
    {
        field_6A_speed = static_cast<short>(speed);
    }
}

DeathFadeOut* DeathFadeOut::ctor_419DB0(Layer layer, __int16 direction, __int16 destroyOnDone, int speed, __int16 abr)
{
    ctor_461550(layer, static_cast<char>(abr));
    SetVTable(this, 0x4BAB08);
    field_4_typeId = Types::eDeathFadeOut_80;

    if (direction)
    {
        field_68_current_fade_rgb = 0;
    }
    else
    {
        field_68_current_fade_rgb = 255;
    }

    Init_419E40(layer, direction, destroyOnDone, speed);

    field_62_b = field_68_current_fade_rgb;
    field_60_g = field_68_current_fade_rgb;
    field_5E_r = field_68_current_fade_rgb;

    return this;
}

BaseGameObject* DeathFadeOut::VDestructor(signed int flags)
{
    dtor_461630();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

}

