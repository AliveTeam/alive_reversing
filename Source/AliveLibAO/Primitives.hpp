#pragma once

#include "Primitives_common.hpp"
#include "FunctionFwd.hpp"

namespace AO {

EXPORT void CC Init_SetTPage_495FB0(Prim_SetTPage* pPrim, int /*notUsed1*/, int /*notUsed2*/, int tpage);
EXPORT int CC PSX_getTPage_4965D0(char tp, char abr, int x, __int16 y);

EXPORT void CC Poly_Set_Blending_498A00(PrimHeader* pPrim, int bBlending);

EXPORT int CC PSX_getClut_496840(int x, int y);

EXPORT void CC Poly_Set_SemiTrans_498A40(PrimHeader* pPrim, int bSemiTrans);

void CC Sprt_Init(Prim_Sprt* pPrim);

EXPORT void CC Init_PrimClipper_495FD0(Prim_PrimClipper* pPrim, const PSX_RECT* pClipRect);

EXPORT void CC OrderingTable_Add_498A80(int** ppOt, PrimHeader* pItem);

EXPORT void CC PolyG4_Init_498890(Poly_G4* pPoly);

EXPORT void CC Prim_Init_Poly_G3_498870(Poly_G3* pPrim);

EXPORT void CC InitType_ScreenOffset_496000(Prim_ScreenOffset* pPrim, const PSX_Pos16* pOffset);

}
