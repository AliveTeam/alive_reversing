#include "stdafx.h"
#include "Primitives.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Game.hpp"

void Primitives_ForceLink() { }

ALIVE_VAR(1, 0xBD146C, BYTE, byte_BD146C, 0);

void CC Init_SetTPage_4F5B60(Prim_SetTPage* pPrim, int /*notUsed1*/, int /*notUsed2*/, int tpage)
{
    pPrim->field_0_header.field_4.mNormal.field_5_unknown = byte_BD146C;
    pPrim->field_0_header.field_B_code = 0x80u;
    pPrim->field_C_tpage = tpage;
}

void CC Init_PrimClipper_4F5B80(Prim_PrimClipper* pPrim, const PSX_RECT* pClipRect)
{
    // The memory layout of this is crazy..
    pPrim->field_0_header.field_4.mNormal.field_5_unknown = byte_BD146C;
    pPrim->field_0_header.field_B_code = 0x81u;
    pPrim->field_C_x = pClipRect->x;
    pPrim->field_E_y = pClipRect->y;
    pPrim->field_0_header.field_4.mRect.w = pClipRect->w;
    pPrim->field_0_header.field_4.mRect.h = pClipRect->h;
}

void CC InitType_ScreenOffset_4F5BB0(Prim_ScreenOffset* pPrim, const PSX_Pos16* pOffset)
{
    pPrim->field_0_header.field_4.mNormal.field_5_unknown = byte_BD146C;
    pPrim->field_0_header.field_B_code = 0x82u;
    pPrim->field_C_xoff = pOffset->x;
    pPrim->field_E_yoff = pOffset->y;
}

void CC Sprt_Init_4F8910(Prim_Sprt* pPrim)
{
    pPrim->field_0_header.field_4.mNormal.field_4_num_longs = 4;
    pPrim->field_0_header.field_4.mNormal.field_5_unknown = byte_BD146C;
    pPrim->field_0_header.field_B_code = 0x64;
}

// Note: Inlined everywhere in real game
void PolyF3_Init(Poly_F3* pPoly)
{
    pPoly->field_0_header.field_4.mNormal.field_4_num_longs = 4;
    pPoly->field_0_header.field_4.mNormal.field_5_unknown = byte_BD146C;
    pPoly->field_0_header.field_B_code = 0x20;
}

// Note: Inlined everywhere in real game
void LineG2_Init(Line_G2* pLine)
{
    pLine->field_0_header.field_4.mNormal.field_4_num_longs = 4;
    pLine->field_0_header.field_4.mNormal.field_5_unknown = byte_BD146C;
    pLine->field_0_header.field_B_code = 0x50;
}

// Note: Inlined everywhere in real game
void LineG4_Init(Line_G4* pLine)
{
    pLine->field_0_header.field_4.mNormal.field_4_num_longs = 9;
    pLine->field_0_header.field_4.mNormal.field_5_unknown = byte_BD146C;
    pLine->field_0_header.field_B_code = 0x5C;
    pLine->field_28_pad = 0x55555555;
}


void CC PolyG3_Init_4F8890(Poly_G3* pPoly)
{
    pPoly->field_0_header.field_4.mNormal.field_4_num_longs = 6;
    pPoly->field_0_header.field_4.mNormal.field_5_unknown = byte_BD146C;
    pPoly->field_0_header.field_B_code = 0x30;
}

void CC PolyG4_Init_4F88B0(Poly_G4* pPoly)
{
    pPoly->field_0_header.field_4.mNormal.field_4_num_longs = 8;
    pPoly->field_0_header.field_4.mNormal.field_5_unknown = byte_BD146C;
    pPoly->field_0_header.field_B_code = 0x38;
}

void CC PolyF4_Init_4F8830(Poly_F4* pPoly)
{
    pPoly->field_0_header.field_4.mNormal.field_4_num_longs = 5;
    pPoly->field_0_header.field_4.mNormal.field_5_unknown = byte_BD146C;
    pPoly->field_0_header.field_B_code = 0x28;
}

void CC PolyFT4_Init_4F8870(Poly_FT4 *pPoly)
{
    pPoly->field_0_header.field_4.mNormal.field_4_num_longs = 9;
    pPoly->field_0_header.field_4.mNormal.field_5_unknown = byte_BD146C;
    pPoly->field_0_header.field_B_code = 44;
}

void CC Poly_FT4_Get_Rect_409DA0(PSX_RECT* pRect, const Poly_FT4* pPoly)
{
    if ((pPoly->field_0_header.field_B_code & 0xFC) == 0x2C) // TODO: Add helper for this check, also used in DrawOTag?
    {
        pRect->x = pPoly->field_C_x0;
        pRect->y = pPoly->field_E_y0;
        pRect->w = pPoly->field_24_x3;
        pRect->h = pPoly->field_26_y3;
    }
    else
    {
        pRect->h = 0;
        pRect->w = 0;
        pRect->y = 0;
        pRect->x = 0;
    }
}

void CC Poly_Set_unknown_4F8A20(PrimHeader* pPrim, int bFlag1)
{
    pPrim->field_4.mNormal.field_5_unknown = byte_BD146C;
    if (bFlag1)
    {
        pPrim->field_B_code = pPrim->field_B_code | 1;
    }
    else
    {
        pPrim->field_B_code = pPrim->field_B_code & ~1;
    }
}

void CC Poly_Set_SemiTrans_4F8A60(PrimHeader* pPrim, int bSemiTrans)
{
    pPrim->field_4.mNormal.field_5_unknown = byte_BD146C;
    if (bSemiTrans)
    {
        pPrim->field_B_code = pPrim->field_B_code | 2;
    }
    else
    {
        pPrim->field_B_code = pPrim->field_B_code & ~2;
    }
}

void CC OrderingTable_Add_4F8AA0(int** ppOt, PrimHeader* pItem)
{
    // Get current OT ptr
    int* pOt = *ppOt;

    // OT points to the new item
    *ppOt = reinterpret_cast<int*>(pItem);

    // Item points back to whatever used to be in the OT, either a pointer to the next OT element
    // or the previously added prim.
    pItem->field_0_tag = pOt;
}


int CC PSX_getTPage_4F60E0(char tp, char abr, int x, __int16 y)
{
    return ((((tp) & 0x3) << 7) | (((abr) & 0x3) << 5) | (((y) & 0x100) >> 4) | (((x) & 0x3ff) >> 6) | (((y) & 0x200) << 2));
}

EXPORT int CC PSX_getClut_4F6350(int x, int y)
{
    return (y << 6) | (x >> 4) & 63;
}
