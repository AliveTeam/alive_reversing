#pragma once

#include "../AliveLibCommon/Primitives_common.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "Layer.hpp"

void Init_SetTPage(Prim_SetTPage* pPrim, s32 /*notUsed1*/, s32 /*notUsed2*/, s32 tpage);

namespace AO {

enum class TPageMode : s8
{
    e4Bit_0 = 0,
    e8Bit_1 = 1,
    e16Bit_2 = 2,
};

enum class TPageAbr : s8
{
    eBlend_0 = 0,
    eBlend_1 = 1,
    eBlend_2 = 2,
    eBlend_3 = 3,
};


void Poly_FT4_Get_Rect(PSX_RECT* pRect, const Poly_FT4* pPoly);

inline s32 PSX_getTPage(TPageMode tp, TPageAbr abr, s32 x, s16 y)
{
    return ((((static_cast<s8>(tp)) & 0x3) << 7) | (((static_cast<s8>(abr)) & 0x3) << 5) | (((y) &0x100) >> 4) | (((x) &0x3ff) >> 6) | (((y) &0x200) << 2));
}

s32 PSX_getClut(s32 x, s32 y);


void Init_PrimClipper_495FD0(Prim_PrimClipper* pPrim, const PSX_RECT* pClipRect);

inline PrimHeader** OtLayer(PrimHeader** ppOt, Layer layer)
{
    return &ppOt[static_cast<u32>(layer)];
}

void PolyG4_Init_498890(Poly_G4* pPoly);

void Prim_Init_Poly_G3_498870(Poly_G3* pPrim);

void InitType_ScreenOffset_496000(Prim_ScreenOffset* pPrim, const PSX_Pos16* pOffset);

} // namespace AO
