#pragma once

#include "FunctionFwd.hpp"
#include "bmp.hpp"

void Psx_ForceLink();

struct PSX_RECT
{
    short x, y, w, h;
};
ALIVE_ASSERT_SIZEOF(PSX_RECT, 8);

struct PSX_DISPENV
{
    PSX_RECT disp;
    PSX_RECT screen;
    char isinter;
    char isrgb24;
    char pad0;
    char pad1;
};
// TODO: Size

struct PSX_DR_ENV
{
    int field_0_tag;
    int field_4_code[15];
};
// TODO: Size

struct PSX_DRAWENV
{
    PSX_RECT field_0_clip;
    __int16 field_8_ofs[2];
    PSX_RECT field_C_tw;
    unsigned __int16 field_14_tpage;
    char field_16_dtd;
    char field_17_dfe;
    char field_18_isbg;
    char field_19_r0;
    char field_1A_g0;
    char field_1B_b0;
    PSX_DR_ENV field_1C_dr_env;
};
// TODO: Size

EXPORT int CC PSX_VSync_4F6170(int mode);
EXPORT signed int CC PSX_ClearImage_4F5BD0(PSX_RECT* pRect, unsigned __int8 r, unsigned __int8 g, __int16 b);
EXPORT int CC PSX_DrawSync_4F6280(int mode);
EXPORT bool CC PSX_Rect_IsInFrameBuffer_4FA050(const PSX_RECT* pRect);
EXPORT void CC PSX_ClearOTag_4F6290(int** otBuffer, int otBufferSize);
EXPORT int CC PSX_SetDispMask_4F89F0(int mode);
EXPORT int CC PSX_SetVideoMode_4FA8F0();
EXPORT int CC PSX_ResetGraph_4F8800(int);
EXPORT int CC PSX_SetGraphDebug_4F8A10(int);
EXPORT void CC PSX_SetDefDrawEnv_4F5AA0(PSX_DRAWENV* pDrawEnv, __int16 x, __int16 y, __int16 w, __int16 h);
EXPORT void CC PSX_SetDefDispEnv_4F55A0(PSX_DISPENV* pOutEnv, __int16 x, __int16 y, __int16 w, __int16 h);
EXPORT void CC PSX_PutDrawEnv_4F5980(const PSX_DRAWENV* pDrawEnv);
EXPORT void CC PSX_PutDispEnv_4F5890(PSX_DISPENV* pDispEnv);
EXPORT void CC PSX_PutDispEnv_4F58E0(const PSX_DISPENV* pDispEnv);
EXPORT void CC PSX_DrawOTag_4F6540(int** pOT);
EXPORT void CC PSX_EMU_VideoDeAlloc_4FA010();
EXPORT int CC PSX_ResetCallBack_4FAA20();
EXPORT int CC PSX_CdInit_4FB2C0();
EXPORT int CC PSX_CdSetDebug_4FB330(int mode);
EXPORT int CC PSX_CdControlB_4FB320(int, int, int);
EXPORT int CC PSX_StopCallBack_4FAA30();
EXPORT int CC PSX_EMU_VideoAlloc_4F9D70();
EXPORT void CC Init_VGA_AndPsxVram_494690();
EXPORT void CC PSX_EMU_Init_4F9CD0(bool bShowVRam);
EXPORT void CC PSX_EMU_SetCallBack_4F9430(int callBackType, void* fnPtr);
EXPORT signed int CC PSX_EMU_Set_Cd_Emulation_Paths_4FAA70(const char* pPath1, const char* pPath2, const char* pPath3);
EXPORT void CC PSX_EMU_Set_screen_mode_4F9420(char mode);
EXPORT signed int CC PSX_MoveImage_4F5D50(const PSX_RECT* pRect, int xpos, int ypos);
EXPORT int CC PSX_LoadImage16_4F5E20(const PSX_RECT* pRect, BYTE* pData);

EXPORT void CC PSX_CD_Normalize_FileName_4FAD90(char* pNormalized, const char* pFileName);
EXPORT int CC PSX_CD_OpenFile_4FAE80(const char* pFileName, int bTryAllPaths);

struct CdlLOC
{
    unsigned __int8 field_0_minute;
    unsigned __int8 field_1_second;
    unsigned __int8 field_2_sector;
    char field_3_track;
};
ALIVE_ASSERT_SIZEOF(CdlLOC, 0x4);

EXPORT CdlLOC* CC PSX_Pos_To_CdLoc_4FADD0(int pos, CdlLOC* pLoc);
EXPORT int CC PSX_CdLoc_To_Pos_4FAE40(const CdlLOC* pLoc);
EXPORT int CC PSX_CD_File_Seek_4FB1E0(char mode, const CdlLOC* pLoc);
EXPORT int CC PSX_CD_File_Read_4FB210(int numSectors, void* pBuffer);
EXPORT int CC PSX_CD_FileIOWait_4FB260(int bASync);

ALIVE_VAR_EXTERN(Bitmap, sPsxVram_C1D160);
ALIVE_VAR_EXTERN(BYTE, byte_BD0F20);

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
    int field_0_tag;
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
    BYTE field_10_u0;
    BYTE field_11_v0;
    __int16 field_12_clut;

    __int16 field_14_x1;
    __int16 field_16_y1;
    BYTE field_18_u1;
    BYTE field_19_v0;
    __int16 field_1A_tpage;

    __int16 field_1C_x2;
    __int16 field_1E_y2;
    BYTE field_20_u2;
    BYTE field_21_v2;
    __int16 field_22_pad1;

    __int16 field_24_x3;
    __int16 field_26_y3;
    BYTE field_28_u3;
    BYTE field_29_v3;
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


namespace Test
{
    void PsxTests();
}
