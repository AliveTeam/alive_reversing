#include "stdafx.h"
#include "DeathFadeOut.hpp"
#include "Function.hpp"
#include "stdlib.hpp"

ALIVE_VAR(1, 0x5bc204, s32, sIsFadingOut_5BC204, 0);

BaseGameObject* DeathFadeOut::VDestructor(s32 flags)
{
    return vdtor_4270F0(flags);
}

void DeathFadeOut::VUpdate()
{
    Update_4271C0();
}

void DeathFadeOut::VScreenChanged()
{
    // Empty
}

void DeathFadeOut::VRender(PrimHeader** ppOt)
{
    vRender_427210(ppOt);
}

DeathFadeOut* DeathFadeOut::ctor_427030(Layer layer, s16 direction, s16 destroyOnDone, s16 speed, TPageAbr abr)
{
    ctor_4AB7A0(layer, abr);
    SetVTable(this, 0x544DA0); // vTbl_DeathFadeOut_00544DA0
    SetType(AETypes::eMainMenuTransistion_116);

    if (direction)
    {
        field_78_current_fade_rgb = 0;
    }
    else
    {
        field_78_current_fade_rgb = 255;
    }

    Init_427140(layer, direction, destroyOnDone, speed);

    field_72_b = field_78_current_fade_rgb;
    field_70_g = field_78_current_fade_rgb;
    field_6E_r = field_78_current_fade_rgb;

    return this;
}

void DeathFadeOut::Init_427140(Layer layer, s16 direction, s16 destroyOnDone, s16 speed)
{
    field_6C_layer = layer;
    field_7C_direction = direction;
    field_7E_bDone = 0;

    if (speed > 0)
    {
        field_82 = 0;
    }
    else
    {
        field_82 = 1;
    }
    field_80_destroy_on_done = destroyOnDone;

    if (direction == 0)
    {
        field_7A_speed = -speed;
    }
    else
    {
        field_7A_speed = speed;
    }

    sIsFadingOut_5BC204 = TRUE;
}

void DeathFadeOut::dtor_427120()
{
    dtor_4AB8F0();
}

DeathFadeOut* DeathFadeOut::vdtor_4270F0(s32 flags)
{
    dtor_427120();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void DeathFadeOut::Update_4271C0()
{
    if (!field_7E_bDone && !field_82)
    {
        field_78_current_fade_rgb += field_7A_speed;
        if (field_7C_direction)
        {
            if (field_78_current_fade_rgb > 255)
            {
                field_78_current_fade_rgb = 255;
            }
        }
        else if (field_78_current_fade_rgb < 0)
        {
            field_78_current_fade_rgb = 0;
        }
    }
}

void DeathFadeOut::vRender_427210(PrimHeader** ppOt)
{
    field_72_b = field_78_current_fade_rgb;
    field_70_g = field_78_current_fade_rgb;
    field_6E_r = field_78_current_fade_rgb;

    EffectBase::VRender(ppOt);

    if ((field_78_current_fade_rgb == 255 && field_7C_direction) || (field_78_current_fade_rgb == 0 && !field_7C_direction))
    {
        field_7E_bDone = 1;
        if (!sIsFadingOut_5BC204)
        {
            if (field_80_destroy_on_done)
            {
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }
        }
        else
        {
            sIsFadingOut_5BC204 = FALSE;
        }
    }
}
