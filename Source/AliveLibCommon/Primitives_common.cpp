#include "stdafx_common.h"
#include "Primitives_common.hpp"

void SetCode(PrimHeader* pPrim, BYTE code)
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

void SetNumLongs(PrimHeader* pPrim, char numLongs)
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
