#include "stdafx.h"
#include "FunctionFwd.hpp"
#include "bmp.hpp"
#include "Function.hpp"

namespace AO {

EXPORT s32 CC BMP_ClearRect_48F810(Bitmap* pBmp, RECT* pRect, int fillColour)
{
    return BMP_ClearRect_4F1EE0(pBmp, pRect, fillColour);
}

}
