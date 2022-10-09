#pragma once

#include "../relive_lib/Layer.hpp"

struct Prim_Sprt;
struct Poly_G3;
struct Poly_G4;
struct Poly_F4;
struct Poly_GT4;
struct Prim_SetTPage;
struct Prim_PrimClipper;
struct Prim_ScreenOffset;
struct PSX_RECT;
struct PSX_Pos16;
struct PrimHeader;
struct Poly_FT4;

void Sprt_Init(Prim_Sprt* pPrim);

void PolyG3_Init(Poly_G3* pPoly);
void PolyG4_Init(Poly_G4* pPoly);
void PolyF4_Init(Poly_F4* pPoly);

void PolyGT4_Init(Poly_GT4* pPoly);

void Init_SetTPage(Prim_SetTPage* pPrim, s32 /*notUsed1*/, s32 /*notUsed2*/, s32 tpage);
void Init_PrimClipper(Prim_PrimClipper* pPrim, const PSX_RECT* pClipRect);
void InitType_ScreenOffset(Prim_ScreenOffset* pPrim, const PSX_Pos16* pOffset);

void Poly_FT4_Get_Rect(PSX_RECT* pRect, const Poly_FT4* pPoly);
void Poly_Set_Blending(PrimHeader* pPrim, s32 bBlending);
void Poly_Set_SemiTrans(PrimHeader* pPrim, s32 bSemiTrans);
PrimHeader** OtLayer(PrimHeader** ppOt, Layer layer);
void OrderingTable_Add(PrimHeader** ppOt, PrimHeader* pItem);

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

void Init_SetTPage(Prim_SetTPage* pPrim, s32 /*notUsed1*/, s32 /*notUsed2*/, s32 tpage);
s32 PSX_getTPage(TPageMode tp, TPageAbr abr, s32 x, s16 y);
s32 PSX_getClut(s32 x, s32 y);
