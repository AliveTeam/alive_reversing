#pragma once

#include "FunctionFwd.hpp"
#include "Psx_common.hpp"
#include "FixedPoint.hpp"

struct PrimHeader;

namespace AO {

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


struct PSX_Display_Params
{
    s16 field_0_width;
    s16 field_2_height;
    s16 field_4_bpp;
    s16 field_6_max_buffers;
    s16 field_8_buffer_size;
    s16 field_A_k32;
    s16 field_C_k1;
    s16 field_E;
};
ALIVE_ASSERT_SIZEOF(PSX_Display_Params, 0x10);

struct PSX_Display_Buffer
{
    PSX_DRAWENV field_0_draw_env;
    PSX_DISPENV field_5C_disp_env;
    PrimHeader* field_70_ot_buffer[256];
};
ALIVE_ASSERT_SIZEOF(PSX_Display_Buffer, 0x470);

class PsxDisplay
{
public:
    EXPORT PsxDisplay* ctor_40DAB0(const PSX_Display_Params* pParams);

    EXPORT void PSX_Display_Render_OT_40DD20();

    EXPORT void PutCurrentDispEnv_40DE40();

    EXPORT void Movie_Render_40DE60(u16 bufferIdx);

    u16 field_0_width;
    u16 field_2_height;
    s16 field_4_bpp;
    s16 field_6_max_buffers;
    u16 field_8_buffer_size;
    u16 field_A_buffer_index;
    PSX_Display_Buffer field_C_drawEnv[2];
};
ALIVE_ASSERT_SIZEOF(PsxDisplay, 0x8EC);

ALIVE_VAR_EXTERN(PSX_Display_Params, gPsxDisplayParams_4BB830);
ALIVE_VAR_EXTERN(PsxDisplay, gPsxDisplay_504C78);

EXPORT s32 CC DebugFont_Init_487EC0();

EXPORT void CC DebugFont_Flush_487F50();


} // namespace AO
