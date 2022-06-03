#include "stdafx.h"
#include "DeathFadeOut.hpp"
#include "Function.hpp"
#include "stdlib.hpp"

ALIVE_VAR(1, 0x5bc204, s32, sIsFadingOut_5BC204, 0);

void DeathFadeOut::VScreenChanged()
{
    // Empty
}

DeathFadeOut::DeathFadeOut(Layer layer, s32 direction, bool destroyOnDone, s32 speed, TPageAbr abr)
    : EffectBase(layer, abr)
{
    SetType(AETypes::eMainMenuTransistion_116);

    if (direction)
    {
        field_78_current_fade_rgb = 0;
    }
    else
    {
        field_78_current_fade_rgb = 255;
    }

    Init(layer, direction, destroyOnDone, speed);

    mBlue = field_78_current_fade_rgb;
    mGreen = field_78_current_fade_rgb;
    mRed = field_78_current_fade_rgb;
}

void DeathFadeOut::Init(Layer layer, s32 direction, bool destroyOnDone, s32 speed)
{
    mLayer = layer;
    field_7C_direction = static_cast<s16>(direction);
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
        field_7A_speed = static_cast<s16>(-speed);
    }
    else
    {
        field_7A_speed = static_cast<s16>(speed);
    }

    sIsFadingOut_5BC204 = TRUE;
}

void DeathFadeOut::VUpdate()
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

void DeathFadeOut::VRender(PrimHeader** ppOt)
{
    mBlue = field_78_current_fade_rgb;
    mGreen = field_78_current_fade_rgb;
    mRed = field_78_current_fade_rgb;

    EffectBase::VRender(ppOt);

    if ((field_78_current_fade_rgb == 255 && field_7C_direction) || (field_78_current_fade_rgb == 0 && !field_7C_direction))
    {
        field_7E_bDone = 1;
        if (!sIsFadingOut_5BC204)
        {
            if (field_80_destroy_on_done)
            {
                mGameObjectFlags.Set(BaseGameObject::eDead);
            }
        }
        else
        {
            sIsFadingOut_5BC204 = FALSE;
        }
    }
}
