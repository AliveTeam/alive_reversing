#pragma once

#include "FunctionFwd.hpp"
#include "Psx.hpp"

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
    unsigned __int16 field_0_width;
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

EXPORT void CC DebugFont_Flush_4DD050();
EXPORT int CC DebugFont_Printf_4F8B60(int idx, const char* formatStr, ...);
EXPORT int CC DebugFont_Init_4DCF40();

void PSX_DrawDebugTextBuffers(Bitmap* pBmp, const RECT& rect);

namespace Test
{
    void PsxDisplayTests();
}
