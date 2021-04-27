#include "stdafx_common.h"
#include "Primitives_common.hpp"

void SetCode(PrimHeader* pPrim, u8 code)
{
    pPrim->rgb_code.code_or_pad = code;
#if !_WIN32 || _WIN64
    pPrim->hackPtr = nullptr;
#endif
}

void SetUnknown(PrimHeader* pPrim)
{
    pPrim->header.mNormal.field_5_unknown = 0; // byte_BD146C; // Note not using the AE or AO value but shouldn't matter as its never read
}

void SetNumLongs(PrimHeader* pPrim, s8 numLongs)
{
    pPrim->header.mNormal.field_4_num_longs = numLongs;
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


void PolyFT4_Init(Poly_FT4* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 9);
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::ePolyFT4);
}

void PolyF4_Init(Poly_F4* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 5);
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::ePolyF4);
}

void Prim_Init_MoveImage(Prim_MoveImage* pPrim, PSX_RECT* pRect, s32 xpos, s32 ypos)
{
    SetUnknown(&pPrim->mPrimHeader);
    SetCode(&pPrim->mPrimHeader, PrimTypeCodes::eMoveImage);
    pPrim->yPos = ypos;
    pPrim->xPos = xpos;
    pPrim->rect = *pRect;
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
    SetNumLongs(&pPrim->mBase.header, 3);
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

s32 PSX_Prim_Code_Without_Blending_Or_SemiTransparency(s32 code)
{
    // Last 2 bits are for blending and semi transparency enable
    return code & 0xFC;
}


void SetPrimExtraPointerHack(Poly_FT4* pPoly, const void* ptr)
{
#if _WIN32 && !_WIN64
    // Store the pointer to the bit field data - this gets used by the lowest level software rendering func
    // TODO: OG game hack
    // TODO: 64bit fail
    u32 asPtr = *((u32*) &ptr);

    s32 ptr_first_half = (s32) asPtr >> 16;
    s16 ptr_second_half = (u16)(s32)(asPtr);
    pPoly->mVerts[1].mUv.tpage_clut_pad = ptr_second_half;
    pPoly->mVerts[2].mUv.tpage_clut_pad = static_cast<u16>(ptr_first_half);
#else
    pPoly->mBase.header.hackPtr = ptr;
#endif
}

const void* GetPrimExtraPointerHack(Poly_FT4* pPoly)
{
#if _WIN32 && !_WIN64
    return reinterpret_cast<void*>(pPoly->mVerts[1].mUv.tpage_clut_pad + (pPoly->mVerts[2].mUv.tpage_clut_pad << 16));
#else
    return pPoly->mBase.header.hackPtr;
#endif
}
