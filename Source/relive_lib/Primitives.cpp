#include "stdafx.h"
#include "Primitives.hpp"
#include "FatalError.hpp"
#include "Animation.hpp"


void Poly_FT4_Get_Rect(PSX_RECT* pRect, const Poly_FT4* pPoly)
{
    if (pPoly->mType == PrimitivesTypes::ePolyFT4)
    {
        pRect->x = pPoly->X0();
        pRect->y = pPoly->Y0();
        pRect->w = pPoly->X3();
        pRect->h = pPoly->Y3();
    }
    else
    {
        pRect->h = 0;
        pRect->w = 0;
        pRect->y = 0;
        pRect->x = 0;
    }
}
