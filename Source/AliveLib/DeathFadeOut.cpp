#include "stdafx.h"
#include "DeathFadeOut.hpp"
#include "Function.hpp"
#include "stdlib.hpp"

ALIVE_VAR(1, 0x5bc204, int, dword_5BC204, 0);

BaseGameObject* DeathFadeOut::VDestructor(signed int flags)
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

void DeathFadeOut::VRender(int** pOrderingTable)
{
    vRender_427210(pOrderingTable);
}

DeathFadeOut* DeathFadeOut::ctor_427030(__int16 layer, __int16 direction, __int16 destroyOnDone, short speed, char abr)
{
    ctor_4AB7A0(layer, abr);
    SetVTable(this, 0x544DA0); // vTbl_DeathFadeOut_00544DA0
    field_4_typeId = Types::eMainMenuTransistion_116;

    if (direction)
    {
        field_78_current_fade_rgb = 0;
    }
    else
    {
        field_78_current_fade_rgb = 255;
    }

    Init_427140(layer, direction, destroyOnDone, speed);

    field_72_r = field_78_current_fade_rgb;
    field_70_g = field_78_current_fade_rgb;
    field_6E_b = field_78_current_fade_rgb;

    return this;
}

void DeathFadeOut::Init_427140(__int16 layer, __int16 direction, __int16 destroyOnDone, short speed)
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

    if (!direction)
    {
        field_7A_speed = -speed;
    }
    else
    {
        field_7A_speed = speed;
    }

    dword_5BC204 = 1;
}

void DeathFadeOut::dtor_427120()
{
    dtor_4AB8F0();
}

DeathFadeOut* DeathFadeOut::vdtor_4270F0(signed int flags)
{
    dtor_427120();
    if (flags & 1)
    {
        Mem_Free_495540(this);
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

void DeathFadeOut::vRender_427210(int** pOt)
{
    field_72_r = field_78_current_fade_rgb;
    field_70_g = field_78_current_fade_rgb;
    field_6E_b = field_78_current_fade_rgb;

    EffectBase::VRender(pOt);

    if (field_78_current_fade_rgb == 255 && field_7C_direction ||
        field_78_current_fade_rgb == 0 && !field_7C_direction)
    {
        field_7E_bDone = 1;
        if (dword_5BC204 <= 0)
        {
            if (dword_5BC204 == 0)
            {
                if (field_80_destroy_on_done)
                {
                    field_6_flags.Set(BaseGameObject::eDead);
                }
            }
        }
        else
        {
            --dword_5BC204;
        }
    }
}
