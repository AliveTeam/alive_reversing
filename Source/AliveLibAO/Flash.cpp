#include "stdafx_ao.h"
#include "Flash.hpp"
#include "Game.hpp"
#include "../AliveLibAE/stdlib.hpp"

namespace AO {

Flash::Flash(Layer layer, s32 r, s32 g, s32 b, TPageAbr abr, s32 time)
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
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

} // namespace AO
