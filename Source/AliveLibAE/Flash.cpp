#include "stdafx.h"
#include "Flash.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "stdlib.hpp"

Flash::Flash(Layer layer, s32 r, s32 g, s32 b, s32 /*not_used*/, TPageAbr abr, s32 time)
    : EffectBase(layer, abr)
{
    SetType(AETypes::eFlash_117);
    mEffectBaseRed = static_cast<s16>(r);
    mEffectBaseGreen = static_cast<s16>(g);
    mEffectBaseBlue = static_cast<s16>(b);
    field_78_flash_time = time + sGnFrame_5C1B84;
}

void Flash::VUpdate()
{
    if (static_cast<s32>(sGnFrame_5C1B84) >= field_78_flash_time)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}
