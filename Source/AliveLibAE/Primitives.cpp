#include "stdafx.h"
#include "Primitives.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Game.hpp"

void Init_SetTPage_4F5B60(Prim_SetTPage* pPrim, s32 /*notUsed1*/, s32 /*notUsed2*/, s32 tpage)
{
    SetUnknown(&pPrim->mBase);
    SetCode(&pPrim->mBase, PrimTypeCodes::eSetTPage);
    pPrim->field_C_tpage = tpage;
}

void Init_PrimClipper_4F5B80(Prim_PrimClipper* pPrim, const PSX_RECT* pClipRect)
{
    SetUnknown(&pPrim->mBase);
    SetCode(&pPrim->mBase, PrimTypeCodes::ePrimClipper);
    pPrim->field_C_x = pClipRect->x;
    pPrim->field_E_y = pClipRect->y;
    pPrim->mBase.header.mRect.w = pClipRect->w;
    pPrim->mBase.header.mRect.h = pClipRect->h;
}

void InitType_ScreenOffset_4F5BB0(Prim_ScreenOffset* pPrim, const PSX_Pos16* pOffset)
{
    SetUnknown(&pPrim->mBase);
    SetCode(&pPrim->mBase, PrimTypeCodes::eScreenOffset);
    pPrim->field_C_xoff = pOffset->x;
    pPrim->field_E_yoff = pOffset->y;
}


void Sprt_Init_4F8910(Prim_Sprt* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 4);
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::eSprt);
}



void PolyG3_Init_4F8890(Poly_G3* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 6);
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::ePolyG3);
}

void PolyG4_Init_4F88B0(Poly_G4* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 8);
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::ePolyG4);
}

void PolyF4_Init_4F8830(Poly_F4* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 5);
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::ePolyF4);
}

void PolyGT4_Init(Poly_GT4* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 99); // TODO: Num longs never used by anything?
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::ePolyGT4);
}

void Poly_FT4_Get_Rect_409DA0(PSX_RECT* pRect, const Poly_FT4* pPoly)
{
    if (PSX_Prim_Code_Without_Blending_Or_SemiTransparency(pPoly->mBase.header.rgb_code.code_or_pad) == PrimTypeCodes::ePolyFT4)
    {
        pRect->x = pPoly->mBase.vert.x;
        pRect->y = pPoly->mBase.vert.y;
        pRect->w = pPoly->mVerts[2].mVert.x;
        pRect->h = pPoly->mVerts[2].mVert.y;
    }
    else
    {
        pRect->h = 0;
        pRect->w = 0;
        pRect->y = 0;
        pRect->x = 0;
    }
}

void Poly_Set_Blending_4F8A20(PrimHeader* pPrim, s32 bFlag1)
{
    SetUnknown(pPrim);
    if (bFlag1)
    {
        pPrim->rgb_code.code_or_pad |= 1;
    }
    else
    {
        pPrim->rgb_code.code_or_pad &= ~1;
    }
}

void Poly_Set_SemiTrans_4F8A60(PrimHeader* pPrim, s32 bSemiTrans)
{
    SetUnknown(pPrim);
    if (bSemiTrans)
    {
        pPrim->rgb_code.code_or_pad |= 2;
    }
    else
    {
        pPrim->rgb_code.code_or_pad &= ~2;
    }
}

void OrderingTable_Add_4F8AA0(PrimHeader** ppOt, PrimHeader* pItem)
{
    // Debugging code, rendering type 2 will currently crash.
    // I can't see where in the code type 2 is ever used but somehow it must be
    // given it crashed in standalone rendering type 2. Type may also be 0 with blending enabled.
    if (pItem->rgb_code.code_or_pad == 2 || pItem->rgb_code.code_or_pad == 0)
    {
        abort();
    }

    // Get current OT ptr
    PrimHeader* pOt = *ppOt;

    // OT points to the new item
    *ppOt = pItem;

    // Item points back to whatever used to be in the OT, either a pointer to the next OT element
    // or the previously added prim.
    pItem->tag = pOt;
}

PrimHeader** OtLayer(PrimHeader** ppOt, Layer layer)
{
    return &ppOt[static_cast<u32>(layer)];
}

s32 PSX_getTPage_4F60E0(TPageMode tp, TPageAbr abr, s32 x, s16 y)
{
    return ((((static_cast<s8>(tp)) & 0x3) << 7) | (((static_cast<s8>(abr)) & 0x3) << 5) | (((y) &0x100) >> 4) | (((x) &0x3ff) >> 6) | (((y) &0x200) << 2));
}

s32 PSX_getClut_4F6350(s32 x, s32 y)
{
    return (y << 6) | ((x >> 4) & 63);
}
