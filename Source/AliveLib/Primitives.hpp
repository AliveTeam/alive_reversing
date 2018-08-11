#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Psx.hpp"

void Primitives_ForceLink();

struct PSX_Pos16
{
    short x, y;
};

struct PrimHeaderPart_Normal
{
    char field_4_num_longs;
    char field_5_unknown;
    __int16 field_6_pad0;
};

struct PrimHeaderPart_PsxRect
{
    short w;
    short h;
};

union PrimHeaderPart
{
    PrimHeaderPart_Normal mNormal;
    PrimHeaderPart_PsxRect mRect;
};
ALIVE_ASSERT_SIZEOF(PrimHeaderPart, 4);

struct PrimHeader
{
    int* field_0_tag;
    PrimHeaderPart field_4;
    BYTE field_8_r0;
    BYTE field_9_g0;
    BYTE field_A_b0;
    BYTE field_B_code;
};
ALIVE_ASSERT_SIZEOF(PrimHeader, 0xC);

struct Poly_G3
{
    PrimHeader field_0_header;

    __int16 field_C_x0;
    __int16 field_E_y0;

    BYTE field_10_r1;
    BYTE field_11_g1;
    BYTE field_12_b1;
    char field_13_pad2;
    __int16 field_14_x1;
    __int16 field_16_y1;

    BYTE field_18_r2;
    BYTE field_19_g2;
    BYTE field_1A_b2;
    char field_1B_pad3;
    __int16 field_1C_x2;
    __int16 field_1E_y2;
};
ALIVE_ASSERT_SIZEOF(Poly_G3, 0x20);

struct Poly_F3
{
    PrimHeader field_0_header;
    __int16 field_C_x0;
    __int16 field_E_y0;
    __int16 field_10_x1;
    __int16 field_12_y1;
    __int16 field_14_x2;
    __int16 field_16_y2;
};
ALIVE_ASSERT_SIZEOF(Poly_F3, 0x18);

struct Poly_F4
{
    PrimHeader field_0_header;
    __int16 field_C_x0;
    __int16 field_E_y0;
    __int16 field_10_x1;
    __int16 field_12_y1;
    __int16 field_14_x2;
    __int16 field_16_y2;
    __int16 field_18_x3;
    __int16 field_1A_y3;
};
ALIVE_ASSERT_SIZEOF(Poly_F4, 0x1C);

struct Poly_FT4
{
    PrimHeader field_0_header;
    __int16 field_C_x0;
    __int16 field_E_y0;
    char field_10_u0;
    char field_11_v0;
    __int16 field_12_clut;
    __int16 field_14_x1;
    __int16 field_16_y1;
    char field_18_u1;
    char field_19_v1;
    __int16 field_1A_tpage;
    __int16 field_1C_x2;
    __int16 field_1E_y2;
    char field_20_u2;
    char field_21_v2;
    __int16 field_22_pad1;
    __int16 field_24_x3;
    __int16 field_26_y3;
    char field_28_u3;
    char field_29_v3;
    __int16 field_2A_pad2;
};
ALIVE_ASSERT_SIZEOF(Poly_FT4, 0x2C);

struct Poly_G4
{
    PrimHeader field_0_header;

    __int16 field_C_x0;
    __int16 field_E_y0;

    BYTE field_10_r1;
    BYTE field_11_g1;
    BYTE field_12_b1;
    char field_13_pad2;
    __int16 field_14_x1;
    __int16 field_16_y1;

    BYTE field_18_r2;
    BYTE field_19_g2;
    BYTE field_1A_b2;
    char field_1B_pad3;
    __int16 field_1C_x2;
    __int16 field_1E_y2;

    BYTE field_20_r3;
    BYTE field_21_g3;
    BYTE field_22_b3;
    char field_23_pad4;
    __int16 field_24_x3;
    __int16 field_26_y3;
};
ALIVE_ASSERT_SIZEOF(Poly_G4, 0x28);


// TODO: FIX ME - in hacked window mode screen offset doesn't actually work. Notice how explosion/screen shakes do nothing.
struct Prim_ScreenOffset
{
    PrimHeader field_0_header;
    short field_C_xoff;
    short field_E_yoff;
};
ALIVE_ASSERT_SIZEOF(Prim_ScreenOffset, 0x10);

struct Prim_PrimClipper
{
    PrimHeader field_0_header;
    short field_C_x;
    short field_E_y;
};
ALIVE_ASSERT_SIZEOF(Prim_PrimClipper, 0x10);

struct Prim_SetTPage
{
    PrimHeader field_0_header;
    int field_C_tpage;
};
ALIVE_ASSERT_SIZEOF(Prim_SetTPage, 0x10);


struct Line_G2
{
    PrimHeader field_0_header;

    __int16 field_C_x0;
    __int16 field_E_y0;

    BYTE field_10_r1;
    BYTE field_11_g1;
    BYTE field_12_b1;
    char field_13_pad2;
    __int16 field_14_x1;
    __int16 field_16_y1;
};
// TODO: Assert size

struct Line_G4
{
    PrimHeader field_0_header;

    __int16 field_C_x0;
    __int16 field_E_y0;

    BYTE field_10_r1;
    BYTE field_11_g1;
    BYTE field_12_b1;
    char field_13_pad2;
    __int16 field_14_x1;
    __int16 field_16_y1;

    BYTE field_18_r2;
    BYTE field_19_g2;
    BYTE field_1A_b2;
    char field_1B_pad3;
    __int16 field_1C_x2;
    __int16 field_1E_y2;

    BYTE field_20_r3;
    BYTE field_21_g3;
    BYTE field_22_b3;
    char field_23_pad4;
    __int16 field_24_x3;
    __int16 field_26_y3;
    DWORD field_28_pad;
};
ALIVE_ASSERT_SIZEOF(Line_G4, 0x2C);

struct Prim_Sprt
{
    PrimHeader field_0_header;
    short field_C_x0;
    short field_E_y0;
    BYTE field_10_u0;
    BYTE field_11_v0;
    WORD field_12_clut;
    __int16 field_14_w;
    __int16 field_16_h;
};
ALIVE_ASSERT_SIZEOF(Prim_Sprt, 0x18);

EXPORT void PolyF3_Init(Poly_F3* pPoly);
EXPORT void LineG2_Init(Line_G2* pLine);
EXPORT void LineG4_Init(Line_G4* pLine);

EXPORT void CC Sprt_Init_4F8910(Prim_Sprt* pPrim);
EXPORT void CC PolyG3_Init_4F8890(Poly_G3* pPoly);
EXPORT void CC PolyG4_Init_4F88B0(Poly_G4* pPoly);
EXPORT void CC PolyF4_Init_4F8830(Poly_F4* pPoly);
EXPORT void CC PolyFT4_Init_4F8870(Poly_FT4 *pPoly);

EXPORT void CC Init_SetTPage_4F5B60(Prim_SetTPage* pPrim, int /*notUsed1*/, int /*notUsed2*/, int tpage);
EXPORT void CC Init_PrimClipper_4F5B80(Prim_PrimClipper* pPrim, const PSX_RECT* pClipRect);
EXPORT void CC InitType_ScreenOffset_4F5BB0(Prim_ScreenOffset* pPrim, const PSX_Pos16* pOffset);
EXPORT void CC Poly_FT4_Get_Rect_409DA0(PSX_RECT* pRect, const Poly_FT4* pPoly);
EXPORT void CC Poly_Set_unknown_4F8A20(PrimHeader* pPrim, int bFlag1);
EXPORT void CC Poly_Set_SemiTrans_4F8A60(PrimHeader* pPrim, int bSemiTrans);
EXPORT void CC OrderingTable_Add_4F8AA0(int** pOt, PrimHeader* pItem);

EXPORT void CC Init_SetTPage_4F5B60(Prim_SetTPage* pPrim, int /*notUsed1*/, int /*notUsed2*/, int tpage);
EXPORT int CC PSX_getTPage_4F60E0(char tp, char abr, int x, __int16 y);
EXPORT int CC PSX_getClut_4F6350(int x, int y);

ALIVE_VAR_EXTERN(BYTE, byte_BD146C);