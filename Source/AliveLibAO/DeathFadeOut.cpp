#include "stdafx_ao.h"
#include "DeathFadeOut.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../AliveLibCommon/FatalError.hpp"

namespace AO {


DeathFadeOut::DeathFadeOut(Layer layer, FadeOptions fade, bool destroyOnDone, s32 speed, TPageAbr abr)
    : EffectBase(layer, abr)
{
    SetType(ReliveTypes::eDeathFadeOut);

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


void DeathFadeOut::VScreenChanged()
{
    // Empty
}

void DeathFadeOut::Init(Layer layer, FadeOptions fade, bool destroyOnDone, s32 speed)
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

void DeathFadeOut::VUpdate()
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

void DeathFadeOut::VRender(PrimHeader** ppOt)
{
    mEffectBaseBlue = mCurrentFadeRGB;
    mEffectBaseGreen = mCurrentFadeRGB;
    mEffectBaseRed = mCurrentFadeRGB;

    EffectBase::VRender(ppOt);

    if ((mCurrentFadeRGB == 255 && mFadeOption == FadeOptions::eFadeIn) ||
        (mCurrentFadeRGB == 0 && mFadeOption == FadeOptions::eFadeOut))
    {
        mDone = true;
        if (mDestroyOnDone)
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }
}

} // namespace AO
