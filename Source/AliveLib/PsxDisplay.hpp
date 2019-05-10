#pragma once

#include "FunctionFwd.hpp"
#include "Psx.hpp"

// 368*40/23 =640
// This seems to convert from PSX coordinate space to PC coordinate space
// anywhere you see this calc replace it with this function
template<class T>
inline T PsxToPCX(T x, int addToX = 0)
{
    return static_cast<T>(((40 * x) + static_cast<T>(addToX)) / 23);
}

// 640 * 23 / 40 =  368
template<class T>
inline T PCToPsxX(T x, int addX = 0)
{
    return (((x) * 23 + static_cast<T>(addX)) / 40);
}

class PSX_Display_Buffer
{
public:
    PSX_DRAWENV field_0_draw_env;
    PSX_DISPENV field_5C_disp_env;
    int* field_70_ot_buffer[256];
};
// TODO: Size

class PsxDisplay
{
public:
    __int16 field_0_width;
    __int16 field_2_height;
    __int16 field_4;
    __int16 field_6_bpp;
    __int16 field_8_max_buffers;
    unsigned __int16 field_A_buffer_size;
    unsigned __int16 field_C_buffer_index;
    __int16 field_E;
    PSX_Display_Buffer field_10_drawEnv[2];

    EXPORT void ctor_41DC30();
    EXPORT void PutCurrentDispEnv_41DFA0();
    EXPORT void PSX_Display_Render_OT_41DDF0();
};
// TODO: Size

ALIVE_VAR_EXTERN(PsxDisplay, gPsxDisplay_5C1130);
ALIVE_VAR_EXTERN(bool, sCommandLine_NoFrameSkip_5CA4D1);
ALIVE_VAR_EXTERN(short, sbDebugFontLoaded_BB4A24);
ALIVE_VAR_EXTERN(int, sbDisplayRenderFrame_55EF8C);

EXPORT void CC DebugFont_Flush_4DD050();
EXPORT int CC DebugFont_Printf_4F8B60(int idx, const char* formatStr, ...);
EXPORT int CC DebugFont_Init_4DCF40();

void PSX_DrawDebugTextBuffers(Bitmap* pBmp, const RECT& rect);

namespace Test
{
    void PsxDisplayTests();
}
