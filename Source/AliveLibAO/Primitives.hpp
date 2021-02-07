#pragma once

#include "Primitives_common.hpp"
#include "FunctionFwd.hpp"
#include "Layer.hpp"

namespace AO {

EXPORT void CC Init_SetTPage_495FB0(Prim_SetTPage* pPrim, int /*notUsed1*/, int /*notUsed2*/, int tpage);


enum class TPageMode : char
{
    e4Bit_0 = 0,
    e8Bit_1 = 1,
    e16Bit_2 = 2,
};

inline int CC PSX_getTPage_4965D0(TPageMode tp, char abr, int x, __int16 y)
{
    return ((((static_cast<char>(tp)) & 0x3) << 7) | (((abr) & 0x3) << 5) | (((y) & 0x100) >> 4) | (((x) & 0x3ff) >> 6) | (((y) & 0x200) << 2));
}

EXPORT void CC Poly_Set_Blending_498A00(PrimHeader* pPrim, int bBlending);

EXPORT int CC PSX_getClut_496840(int x, int y);

EXPORT void CC Poly_Set_SemiTrans_498A40(PrimHeader* pPrim, int bSemiTrans);

void CC Sprt_Init(Prim_Sprt* pPrim);

EXPORT void CC Init_PrimClipper_495FD0(Prim_PrimClipper* pPrim, const PSX_RECT* pClipRect);

inline PrimHeader** OtLayer(PrimHeader** ppOt, Layer layer)
{
    return &ppOt[static_cast<unsigned int>(layer)];
}

EXPORT void CC OrderingTable_Add_498A80(PrimHeader** ppOt, PrimHeader* pItem);

EXPORT void CC PolyG4_Init_498890(Poly_G4* pPoly);

EXPORT void CC Prim_Init_Poly_G3_498870(Poly_G3* pPrim);

EXPORT void CC InitType_ScreenOffset_496000(Prim_ScreenOffset* pPrim, const PSX_Pos16* pOffset);

}
