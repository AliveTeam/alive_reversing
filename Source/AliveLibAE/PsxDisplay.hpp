#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../AliveLibCommon/pch_shared.h"
#include "Psx.hpp"

// 368*40/23 =640
// This seems to convert from PSX coordinate space to PC coordinate space
// anywhere you see this calc replace it with this function
template <class T>
inline T PsxToPCX(T x, s32 addToX = 0)
{
    return static_cast<T>(((40 * x) + static_cast<T>(addToX)) / 23);
}

inline FP PsxToPCX(FP x, FP addToX = FP_FromInteger(0))
{
    return ((FP_FromInteger(40) * x) + addToX) / FP_FromInteger(23);
}

// 640 * 23 / 40 =  368
template <class T>
inline T PCToPsxX(T x, s32 addX = 0)
{
    return (((x) *23 + static_cast<T>(addX)) / 40);
}

struct PrimHeader;

class PSX_Display_Buffer final
{
public:
    PSX_DRAWENV field_0_draw_env;
    PSX_DISPENV field_5C_disp_env;
    PrimHeader* field_70_ot_buffer[256];
};
// TODO: Size

class PsxDisplay final
{
public:
    s16 field_0_width;
    s16 field_2_height;
    s16 field_4_unused;
    s16 field_6_bpp;
    s16 field_8_max_buffers;
    u16 field_A_buffer_size;
    u16 field_C_buffer_index;
    s16 field_E_padding;
    PSX_Display_Buffer field_10_drawEnv[2];

    void ctor_41DC30();
    void PutCurrentDispEnv_41DFA0();
    void PSX_Display_Render_OT_41DDF0();
};
// TODO: Size

ALIVE_VAR_EXTERN(PsxDisplay, gPsxDisplay_5C1130);
ALIVE_VAR_EXTERN(bool, sCommandLine_NoFrameSkip_5CA4D1);
ALIVE_VAR_EXTERN(s16, sbDebugFontLoaded_BB4A24);
ALIVE_VAR_EXTERN(s32, sbDisplayRenderFrame_55EF8C);

void DebugFont_Flush_4DD050();
s32 DebugFont_Printf_4F8B60(s32 idx, const char_type* formatStr, ...);
s32 DebugFont_Init_4DCF40();

void PSX_DrawDebugTextBuffers(Bitmap* pBmp, const RECT& rect);
