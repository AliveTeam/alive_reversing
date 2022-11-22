#include "stdafx.h"
#include "Fade.hpp"
#include "Function.hpp"
#include "stdlib.hpp"

s32 sIsFadingOut_5BC204 = 0;


Fade::Fade(Layer layer, FadeOptions fade, bool destroyOnDone, s32 speed, TPageAbr abr)
    : EffectBase(layer, abr)
{
    SetType(ReliveTypes::eMainMenuTransistion);

    if (fade == FadeOptions::eFadeIn)
    {
        mCurrentFadeRGB = 0;
    }
    else if (fade == FadeOptions::eFadeOut)
    {
        mCurrentFadeRGB = 255;
    }

    Init(layer, fade, destroyOnDone, speed);

    mEffectBaseBlue = mCurrentFadeRGB;
    mEffectBaseGreen = mCurrentFadeRGB;
    mEffectBaseRed = mCurrentFadeRGB;
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

    sIsFadingOut_5BC204 = true;
}

void Fade::VUpdate()
{
    if (!mDone)
    {
        mCurrentFadeRGB += mSpeed;
        if (mFadeOption == FadeOptions::eFadeIn)
        {
            if (mCurrentFadeRGB > 255)
            {
                mCurrentFadeRGB = 255;
            }
        }
        else if (mFadeOption == FadeOptions::eFadeOut)
        {
            if (mCurrentFadeRGB < 0)
            {
                mCurrentFadeRGB = 0;
            }
        }
    }
}

void Fade::VRender(PrimHeader** ppOt)
{
    mEffectBaseBlue = mCurrentFadeRGB;
    mEffectBaseGreen = mCurrentFadeRGB;
    mEffectBaseRed = mCurrentFadeRGB;

    EffectBase::VRender(ppOt);

    if ((mCurrentFadeRGB == 255 && mFadeOption == FadeOptions::eFadeIn) ||
        (mCurrentFadeRGB == 0 && mFadeOption == FadeOptions::eFadeOut))
    {
        mDone = true;
        if (!sIsFadingOut_5BC204)
        {
            if (mDestroyOnDone)
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