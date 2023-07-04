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

void OrderingTable_Add(BasePrimitive** ppOt, BasePrimitive* pItem)
{
    // Get current OT ptr
    BasePrimitive* pOt = *ppOt;

    // OT points to the new item
    *ppOt = pItem;

    // Item points back to whatever used to be in the OT, either a pointer to the next OT element
    // or the previously added prim.
    pItem->mNext = pOt;
}

BasePrimitive** OtLayer(BasePrimitive** ppOt, Layer layer)
{
    return &ppOt[static_cast<u32>(layer)];
}
