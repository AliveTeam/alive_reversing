#include "stdafx.h"
#include "Flash.hpp"
#include "../AliveLibAE/Game.hpp"

Flash::Flash(Layer layer, s32 r, s32 g, s32 b, relive::TBlendModes abr, s32 time)
    : EffectBase(layer, abr)
{
    SetType(ReliveTypes::eFlash);
    mEffectBaseRed = static_cast<s16>(r);
    mEffectBaseGreen = static_cast<s16>(g);
    mEffectBaseBlue = static_cast<s16>(b);
    mFlashTime = time + sGnFrame;
}

void Flash::VUpdate()
{
    if (static_cast<s32>(sGnFrame) >= mFlashTime)
    {
        SetDead(true);
    }
}
