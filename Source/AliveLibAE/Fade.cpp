#include "stdafx.h"
#include "Fade.hpp"
#include "stdlib.hpp"

static bool sIsFadingOut = false;


Fade::Fade(Layer layer, FadeOptions fade, bool destroyOnDone, s32 speed, relive::TBlendModes abr)
    : EffectBase(layer, abr)
{
    SetType(ReliveTypes::eMainMenuTransistion);

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

Fade::~Fade()
{

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

    sIsFadingOut = true;
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

void Fade::VRender(BasePrimitive** ppOt)
{
    mEffectBaseBlue = mFadeColour;
    mEffectBaseGreen = mFadeColour;
    mEffectBaseRed = mFadeColour;

    EffectBase::VRender(ppOt);

    if ((mFadeColour == 255 && mFadeOption == FadeOptions::eFadeIn) ||
        (mFadeColour == 0 && mFadeOption == FadeOptions::eFadeOut))
    {
        mDone = true;
        if (!sIsFadingOut)
        {
            if (mDestroyOnDone)
            {
                SetDead(true);
            }
        }
        else
        {
            sIsFadingOut = false;
        }
    }
}
