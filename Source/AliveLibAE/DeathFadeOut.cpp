#include "stdafx.h"
#include "DeathFadeOut.hpp"
#include "Function.hpp"
#include "stdlib.hpp"

s32 sIsFadingOut_5BC204 = 0;


DeathFadeOut::DeathFadeOut(Layer layer, s32 direction, bool destroyOnDone, s32 speed, TPageAbr abr)
    : EffectBase(layer, abr)
{
    SetType(ReliveTypes::eMainMenuTransistion);

    if (direction)
    {
        field_78_current_fade_rgb = 0;
    }
    else
    {
        field_78_current_fade_rgb = 255;
    }

    Init(layer, direction, destroyOnDone, speed);

    mEffectBaseBlue = field_78_current_fade_rgb;
    mEffectBaseGreen = field_78_current_fade_rgb;
    mEffectBaseRed = field_78_current_fade_rgb;
}

DeathFadeOut::~DeathFadeOut()
{

}

void DeathFadeOut::VScreenChanged()
{
    // Empty
}

void DeathFadeOut::Init(Layer layer, s32 direction, bool destroyOnDone, s32 speed)
{
    mEffectBaseLayer = layer;
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

    sIsFadingOut_5BC204 = true;
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
    mEffectBaseBlue = field_78_current_fade_rgb;
    mEffectBaseGreen = field_78_current_fade_rgb;
    mEffectBaseRed = field_78_current_fade_rgb;

    EffectBase::VRender(ppOt);

    if ((field_78_current_fade_rgb == 255 && field_7C_direction) || (field_78_current_fade_rgb == 0 && !field_7C_direction))
    {
        field_7E_bDone = 1;
        if (!sIsFadingOut_5BC204)
        {
            if (field_80_destroy_on_done)
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
        }
        else
        {
            sIsFadingOut_5BC204 = false;
        }
    }
}
