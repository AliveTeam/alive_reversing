#include "stdafx.h"
#include "Primitives.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Game.hpp"

void Primitives_ForceLink() { }

ALIVE_VAR(1, 0xBD146C, BYTE, byte_BD146C, 0);

static void SetCode(PrimHeader* pPrim, BYTE code)
{
    pPrim->rgb_code.code_or_pad = code;
}
static void SetUnknown(PrimHeader* pPrim)
{
    pPrim->header.mNormal.field_5_unknown = byte_BD146C;
}

static void SetNumLongs(PrimHeader* pPrim, char numLongs)
{
    pPrim->header.mNormal.field_4_num_longs = numLongs;
}

void CC Init_SetTPage_4F5B60(Prim_SetTPage* pPrim, int /*notUsed1*/, int /*notUsed2*/, int tpage)
{
    SetUnknown(&pPrim->mBase);
    SetCode(&pPrim->mBase, PrimTypeCodes::eSetTPage);
    pPrim->field_C_tpage = tpage;
}

void CC Init_PrimClipper_4F5B80(Prim_PrimClipper* pPrim, const PSX_RECT* pClipRect)
{
    // The memory layout of this is crazy..
    SetUnknown(&pPrim->mBase);
    SetCode(&pPrim->mBase, PrimTypeCodes::ePrimClipper);
    pPrim->field_C_x = pClipRect->x;
    pPrim->field_E_y = pClipRect->y;
    pPrim->mBase.header.mRect.w = pClipRect->w;
    pPrim->mBase.header.mRect.h = pClipRect->h;
}

void CC InitType_ScreenOffset_4F5BB0(Prim_ScreenOffset* pPrim, const PSX_Pos16* pOffset)
{
    SetUnknown(&pPrim->mBase);
    SetCode(&pPrim->mBase, PrimTypeCodes::eScreenOffset);
    pPrim->field_C_xoff = pOffset->x;
    pPrim->field_E_yoff = pOffset->y;
}

void Init_Sprt_8(Prim_Sprt_8* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 99);
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::eSprt8);
}

void Init_Sprt_16(Prim_Sprt_16* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 99);
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::eSprt16);
}

void CC Sprt_Init_4F8910(Prim_Sprt* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 4);
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::eSprt);
}

// Note: Inlined everywhere in real game
void PolyF3_Init(Poly_F3* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 4);
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::ePolyF3);
}

void PolyFT3_Init(Poly_FT3* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 99);
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::ePolyFT3);
}

void PolyGT3_Init(Poly_GT3* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 99);
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::ePolyGT3);
}

void Line_F2_Init(Line_F2* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 4);
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::eLineF2);
}

void Line_F3_Init(Line_F3* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 4);
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::eLineF3);
}

void Line_F4_Init(Line_F4* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 4);
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::eLineF4);
}

// Note: Inlined everywhere in real game
void LineG2_Init(Line_G2* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 4);
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::eLineG2);
}

void LineG3_Init(Line_G3* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 4);
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::eLineG3);
}

// Note: Inlined everywhere in real game
void LineG4_Init(Line_G4* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 9);
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::eLineG4);
    pPrim->field_28_pad = 0x55555555;
}

void Init_Tile1(Prim_Tile_1* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 99);
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::eTile1);
}

void Init_Tile8(Prim_Tile_8* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 99);
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::eTile8);
}

void Init_Tile16(Prim_Tile_16* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 99);
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::eTile16);
}

void Init_Tile(Prim_Tile* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 99);
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::eTile);
}

void CC PolyG3_Init_4F8890(Poly_G3* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 6);
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::ePolyG3);
}

void CC PolyG4_Init_4F88B0(Poly_G4* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 8);
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::ePolyG4);
}

void CC PolyF4_Init_4F8830(Poly_F4* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 5);
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::ePolyF4);
}

void CC PolyFT4_Init_4F8870(Poly_FT4* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 9);
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::ePolyFT4);
}

void PolyGT4_Init(Poly_GT4* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 99); // TODO: Num longs never used by anything? 
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::ePolyGT4);
}

int PSX_Prim_Code_Without_Blending_Or_SemiTransparency(int code)
{
    // Last 2 bits are for blending and semi transparency enable
    return code & 0xFC;
}

void CC Poly_FT4_Get_Rect_409DA0(PSX_RECT* pRect, const Poly_FT4* pPoly)
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

void CC Poly_Set_Blending_4F8A20(PrimHeader* pPrim, int bFlag1)
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

void CC Poly_Set_SemiTrans_4F8A60(PrimHeader* pPrim, int bSemiTrans)
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

void CC OrderingTable_Add_4F8AA0(int** ppOt, PrimHeader* pItem)
{
    // Get current OT ptr
    int* pOt = *ppOt;

    // OT points to the new item
    *ppOt = reinterpret_cast<int*>(pItem);

    // Item points back to whatever used to be in the OT, either a pointer to the next OT element
    // or the previously added prim.
    pItem->tag = pOt;
}


int CC PSX_getTPage_4F60E0(char tp, char abr, int x, __int16 y)
{
    return ((((tp) & 0x3) << 7) | (((abr) & 0x3) << 5) | (((y) & 0x100) >> 4) | (((x) & 0x3ff) >> 6) | (((y) & 0x200) << 2));
}

EXPORT int CC PSX_getClut_4F6350(int x, int y)
{
    return (y << 6) | (x >> 4) & 63;
}
