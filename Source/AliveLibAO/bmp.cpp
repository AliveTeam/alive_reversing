#include "stdafx.h"
#include "../AliveLibCommon/FunctionFwd.hpp"
#include "bmp.hpp"
#include "Function.hpp"

namespace AO {

EXPORT s32 CC BMP_ClearRect_48F810(Bitmap* pBmp, RECT* pRect, s32 fillColour)
{
    return BMP_ClearRect_4F1EE0(pBmp, pRect, fillColour);
}

} // namespace AO
