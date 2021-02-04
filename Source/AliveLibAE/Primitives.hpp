#pragma once

#include "FunctionFwd.hpp"
#include "Primitives_common.hpp"
#include "BaseGameObject.hpp"
#include "Psx.hpp"
#include "Layer.hpp"

void Primitives_ForceLink();


EXPORT void CC Sprt_Init_4F8910(Prim_Sprt* pPrim);

EXPORT void CC PolyG3_Init_4F8890(Poly_G3* pPoly);
EXPORT void CC PolyG4_Init_4F88B0(Poly_G4* pPoly);
EXPORT void CC PolyF4_Init_4F8830(Poly_F4* pPoly);

void PolyGT4_Init(Poly_GT4* pPoly);

EXPORT void CC Init_SetTPage_4F5B60(Prim_SetTPage* pPrim, int /*notUsed1*/, int /*notUsed2*/, int tpage);
EXPORT void CC Init_PrimClipper_4F5B80(Prim_PrimClipper* pPrim, const PSX_RECT* pClipRect);
EXPORT void CC InitType_ScreenOffset_4F5BB0(Prim_ScreenOffset* pPrim, const PSX_Pos16* pOffset);

EXPORT void CC Poly_FT4_Get_Rect_409DA0(PSX_RECT* pRect, const Poly_FT4* pPoly);
EXPORT void CC Poly_Set_Blending_4F8A20(PrimHeader* pPrim, int bBlending);
EXPORT void CC Poly_Set_SemiTrans_4F8A60(PrimHeader* pPrim, int bSemiTrans);
PrimHeader** OtLayer(PrimHeader** ppOt, Layer layer);
EXPORT void CC OrderingTable_Add_4F8AA0(PrimHeader** ppOt, PrimHeader* pItem);

EXPORT void CC Init_SetTPage_4F5B60(Prim_SetTPage* pPrim, int /*notUsed1*/, int /*notUsed2*/, int tpage);
EXPORT int CC PSX_getTPage_4F60E0(char tp, char abr, int x, __int16 y);
EXPORT int CC PSX_getClut_4F6350(int x, int y);
