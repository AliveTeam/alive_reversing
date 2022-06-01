#include "stdafx_ao.h"
#include "../AliveLibCommon/Function.hpp"
#include "bmp.hpp"
#include "Function.hpp"

namespace AO {

s32 BMP_ClearRect_48F810(Bitmap* pBmp, RECT* pRect, s32 fillColour)
{
    return BMP_ClearRect_4F1EE0(pBmp, pRect, fillColour);
}

} // namespace AO
