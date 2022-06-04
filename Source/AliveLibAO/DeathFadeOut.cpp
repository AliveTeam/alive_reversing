#include "stdafx_ao.h"
#include "Function.hpp"
#include "DeathFadeOut.hpp"
#include "stdlib.hpp"

namespace AO {

void DeathFadeOut::VRender(PrimHeader** ppOt)
{
    mEffectBaseBlue = field_68_current_fade_rgb;
    mEffectBaseGreen = field_68_current_fade_rgb;
    mEffectBaseRed = field_68_current_fade_rgb;

    EffectBase::VRender(ppOt);

    if ((field_68_current_fade_rgb == 255 && field_6C_direction) || (field_68_current_fade_rgb == 0 && !field_6C_direction))
    {
        field_6E_bDone = 1;
        if (field_70_destroy_on_done)
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }
}

void DeathFadeOut::VUpdate()
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

void DeathFadeOut::VScreenChanged()
{
    // Empty
}

void DeathFadeOut::Init(Layer layer, s16 direction, s16 destroyOnDone, s32 speed)
{
    mEffectBaseLayer = layer;
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
        field_6A_speed = static_cast<s16>(-speed);
    }
    else
    {
        field_6A_speed = static_cast<s16>(speed);
    }
}

DeathFadeOut::DeathFadeOut(Layer layer, s32 direction, bool destroyOnDone, s32 speed, TPageAbr abr)
    : EffectBase(layer, abr)
{
    mBaseGameObjectTypeId = ReliveTypes::eDeathFadeOut;

    if (direction)
    {
        field_68_current_fade_rgb = 0;
    }
    else
    {
        field_68_current_fade_rgb = 255;
    }

    Init(layer, static_cast<s16>(direction), destroyOnDone, speed);

    mEffectBaseBlue = field_68_current_fade_rgb;
    mEffectBaseGreen = field_68_current_fade_rgb;
    mEffectBaseRed = field_68_current_fade_rgb;
}

} // namespace AO
