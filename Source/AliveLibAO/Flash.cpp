#include "stdafx_ao.h"
#include "Function.hpp"
#include "Flash.hpp"
#include "Game.hpp"
#include "stdlib.hpp"

namespace AO {

Flash::Flash(Layer layer, s32 r, s32 g, s32 b, s32 /*not_used*/, TPageAbr abr, s32 time)
    : EffectBase(layer, abr)
{
    mTypeId = Types::eFlash_81;
    mRed = static_cast<s16>(r);
    mGreen = static_cast<s16>(g);
    mBlue = static_cast<s16>(b);
    field_68_flash_time = time + gnFrameCount_507670;
}

void Flash::VUpdate()
{
    if (static_cast<s32>(gnFrameCount_507670) >= field_68_flash_time)
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

} // namespace AO
