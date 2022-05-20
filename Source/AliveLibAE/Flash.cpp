#include "stdafx.h"
#include "Flash.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "stdlib.hpp"

Flash::Flash(Layer layer, u8 r, u8 g, u8 b, s32 /*not_used*/, TPageAbr abr, s32 time)
    : EffectBase(layer, abr)
{
    SetType(AETypes::eFlash_117);
    field_6E_r = r;
    field_70_g = g;
    field_72_b = b;
    field_78_flash_time = time + sGnFrame_5C1B84;
}

void Flash::VUpdate()
{
    if (static_cast<s32>(sGnFrame_5C1B84) >= field_78_flash_time)
    {
        mFlags.Set(BaseGameObject::eDead);
    }
}
