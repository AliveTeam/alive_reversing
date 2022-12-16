#include "stdafx_ao.h"
#include "Fade.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/FatalError.hpp"

namespace AO {


Fade::Fade(Layer layer, FadeOptions fade, bool destroyOnDone, s32 speed, TPageAbr abr)
    : EffectBase(layer, abr)
{
    SetType(ReliveTypes::eFade);

    if (fade == FadeOptions::eFadeIn)
    {
        mFadeColour = 0;
    }
    else if (fade == FadeOptions::eFadeOut)
    {
        mFadeColour = 255;
    }

    Init(layer, fade, destroyOnDone, speed);

    mEffectBaseBlue = mFadeColour;
    mEffectBaseGreen = mFadeColour;
    mEffectBaseRed = mFadeColour;
}


void Fade::VScreenChanged()
{
    // Empty
}

void Fade::Init(Layer layer, FadeOptions fade, bool destroyOnDone, s32 speed)
{
    mEffectBaseLayer = layer;
    mFadeOption = fade;
    mDone = false;
    mDestroyOnDone = destroyOnDone;

    if (fade == FadeOptions::eFadeOut)
    {
        mSpeed = -speed;
    }
    else if (fade == FadeOptions::eFadeIn)
    {
        mSpeed = speed;
    }
}

void Fade::VUpdate()
{
    if (!mDone)
    {
        mFadeColour += mSpeed;
        if (mFadeOption == FadeOptions::eFadeIn)
        {
            if (mFadeColour > 255)
            {
                mFadeColour = 255;
            }
        }
        else if (mFadeOption == FadeOptions::eFadeOut)
        {
            if (mFadeColour < 0)
            {
                mFadeColour = 0;
            }
        }
    }
}

void Fade::VRender(PrimHeader** ppOt)
{
    mEffectBaseBlue = mFadeColour;
    mEffectBaseGreen = mFadeColour;
    mEffectBaseRed = mFadeColour;

    EffectBase::VRender(ppOt);

    if ((mFadeColour == 255 && mFadeOption == FadeOptions::eFadeIn) ||
        (mFadeColour == 0 && mFadeOption == FadeOptions::eFadeOut))
    {
        mDone = true;
        if (mDestroyOnDone)
        {
            SetDead(true);
        }
    }
}

} // namespace AO
