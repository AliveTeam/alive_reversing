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

ALIVE_VAR_EXTERN(Bitmap, sPsxVram_C1D160);
ALIVE_VAR_EXTERN(BYTE, byte_BD0F20);

namespace Test
{
    void PsxTests();
}
