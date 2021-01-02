#include "stdafx_ao.h"
#include "PsxDisplay.hpp"
#include "Function.hpp"
#include "Psx.hpp"
#include "PsxRender.hpp"
#include "VRam.hpp"
#include "Primitives.hpp"
#include "../AliveLibAE/PsxDisplay.hpp"

namespace AO {

const PSX_Display_Params kDisplayParams = { 640, 240, 16, 1, 42, 32, 1, 0 };
ALIVE_VAR(1, 0x4BB830, PSX_Display_Params, gPsxDisplayParams_4BB830, kDisplayParams);
ALIVE_VAR(1, 0x504C78, PsxDisplay, gPsxDisplay_504C78, {});

PsxDisplay* PsxDisplay::ctor_40DAB0(const PSX_Display_Params* pParams)
{
    PSX_SetDispMask_49AE80(0);

    field_A_buffer_index = 0;
    field_4_bpp = pParams->field_4_bpp;

    field_0_width =  pParams->field_0_width;
    field_2_height = pParams->field_2_height;

    field_6_max_buffers = pParams->field_6_max_buffers;
    field_8_buffer_size = pParams->field_8_buffer_size;

    PSX_ResetGraph_4987E0(0);
    PSX_SetGraphDebug_4989F0(0);
    Vram_reset_450840();

    if (pParams->field_4_bpp == 24)
    {
        field_6_max_buffers = 1;
        field_0_width = 3 *  pParams->field_0_width / 2;
    }

    short w = 0;
    short v1 = 0;
    if (pParams->field_2_height == 240)
    {
        w = field_0_width;
        v1 = 240 * field_6_max_buffers;
        Vram_alloc_explicit_4507F0(0, 0, field_0_width, pParams->field_A_k32 + v1);
    }
    else
    {
        w = field_6_max_buffers * pParams->field_2_height;
        v1 = pParams->field_2_height;
        Vram_alloc_explicit_4507F0(0, 0, w, pParams->field_A_k32 + pParams->field_2_height);
    }
    
    Pal_Reset_4476C0(v1, pParams->field_A_k32);

    if (pParams->field_C_k1 == 2)
    {
        Vram_alloc_explicit_4507F0(w, 256, 1024, 512);
    }

    PSX_SetDefDrawEnv_495EF0(&field_C_drawEnv[0].field_0_draw_env, 0, 0, field_0_width, pParams->field_2_height);
    PSX_SetDefDispEnv_4959D0(
        &field_C_drawEnv[0].field_5C_disp_env,
        0,
        0,
        field_0_width,
        field_2_height);
    PSX_ClearOTag_496760(field_C_drawEnv[0].field_70_ot_buffer, field_8_buffer_size);
    PSX_ClearOTag_496760(field_C_drawEnv[1].field_70_ot_buffer, field_8_buffer_size);

    field_C_drawEnv[0].field_0_draw_env.field_17_dfe = 1;

    if (field_4_bpp == 24)
    {
        field_C_drawEnv[0].field_5C_disp_env.isrgb24 += 12;
    }

    if (field_6_max_buffers > 1u)
    {
        PSX_SetDefDrawEnv_495EF0(
            &field_C_drawEnv[0].field_0_draw_env,
            0,
            field_2_height,
            field_0_width,
            field_2_height);

        PSX_SetDefDrawEnv_495EF0(
            &field_C_drawEnv[1].field_0_draw_env,
            0,
            0,
            field_0_width,
            field_2_height);

        PSX_SetDefDispEnv_4959D0(
            &field_C_drawEnv[1].field_5C_disp_env,
            0,
            field_2_height,
            field_0_width,
            field_2_height);

        field_C_drawEnv[0].field_0_draw_env.field_17_dfe = 1;
        field_C_drawEnv[1].field_0_draw_env.field_17_dfe = 1;
    }

    if (field_2_height == 240)
    {
        field_C_drawEnv[0].field_5C_disp_env.isinter = 0;
        field_C_drawEnv[1].field_5C_disp_env.isinter = 0;
    }

    field_C_drawEnv[0].field_5C_disp_env.screen.x = 0;
    field_C_drawEnv[0].field_5C_disp_env.screen.y = 0;
    field_C_drawEnv[1].field_5C_disp_env.screen.x = 0;
    field_C_drawEnv[1].field_5C_disp_env.screen.y = 0;

    PSX_PutDrawEnv_495DD0(&field_C_drawEnv[0].field_0_draw_env);
    PSX_PutDispEnv_495CE0(&field_C_drawEnv[0].field_5C_disp_env);

    field_C_drawEnv[0].field_5C_disp_env.screen.x = 0;
    field_C_drawEnv[0].field_5C_disp_env.screen.y = 0;
    field_C_drawEnv[1].field_5C_disp_env.screen.x = 0;
    field_C_drawEnv[1].field_5C_disp_env.screen.y = 0;

    PSX_RECT rect = {};
    rect.x = 0;
    rect.y = 0;
    rect.w = 1024;
    rect.h = 512;
    PSX_ClearImage_496020(&rect, 0, 0, 0);

    PSX_DrawSync_496750(0);
    PSX_VSync_496620(0);
    PSX_SetDispMask_4989D0(1);

    return this;
}

void PsxDisplay::PSX_Display_Render_OT_40DD20()
{
    if (field_6_max_buffers <= 1u)
    {
        // Single buffer rendering - never used?
        PSX_PutDrawEnv_495DD0(&field_C_drawEnv[0].field_0_draw_env);
        PSX_DrawOTag_4969F0(field_C_drawEnv[0].field_70_ot_buffer);
        PSX_DrawSync_496750(0);
        PSX_VSync_496620(2);
        PSX_PutDispEnv_495D30(&field_C_drawEnv[0].field_5C_disp_env);
        PSX_ClearOTag_496760(field_C_drawEnv[0].field_70_ot_buffer, field_8_buffer_size);
        field_A_buffer_index = 0;
    }
    else
    {
        // Normal double buffer rendering
        PSX_DrawSync_496750(0);
        PSX_VSync_496620(2);

        const int oldBuffer = field_A_buffer_index;
        field_A_buffer_index = field_A_buffer_index + 1;
        if (field_A_buffer_index >= field_6_max_buffers)
        {
            field_A_buffer_index = 0;
        }

        PSX_ClearOTag_496760(
            field_C_drawEnv[field_A_buffer_index].field_70_ot_buffer,
            field_8_buffer_size);
        PSX_PutDispEnv_495D30(&field_C_drawEnv[field_A_buffer_index].field_5C_disp_env);
        PSX_PutDrawEnv_495DD0(&field_C_drawEnv[field_A_buffer_index].field_0_draw_env);
        PSX_DrawOTag_4969F0(field_C_drawEnv[oldBuffer].field_70_ot_buffer);
    }
}

void PsxDisplay::PutCurrentDispEnv_40DE40()
{
    PSX_PutDispEnv_495CE0(&field_C_drawEnv[field_A_buffer_index].field_5C_disp_env);
}

void PsxDisplay::Movie_Render_40DE60(unsigned __int16 bufferIdx)
{
    field_A_buffer_index = bufferIdx;
    PSX_ClearOTag_496760(field_C_drawEnv[0].field_70_ot_buffer, field_8_buffer_size);
    PSX_ClearOTag_496760(field_C_drawEnv[1].field_70_ot_buffer, field_8_buffer_size);

    PSX_RECT rect = {};
    rect.w = field_0_width;
    rect.y = 272;
    rect.x = 0;
    rect.h = field_2_height;

    Prim_MoveImage movePrim = {};
    Prim_Init_MoveImage(&movePrim, &rect, 0, 0);
    OrderingTable_Add_498A80(field_C_drawEnv[0].field_70_ot_buffer, &movePrim.mPrimHeader);
    PSX_DrawOTag_4969F0(field_C_drawEnv[0].field_70_ot_buffer);
    PSX_DrawSync_496750(0);
    PSX_ClearOTag_496760(field_C_drawEnv[0].field_70_ot_buffer, field_8_buffer_size);
}

EXPORT int CC DebugFont_Init_487EC0()
{
    AE_IMPLEMENTED();
    return ::DebugFont_Init_4DCF40();
}

EXPORT void CC DebugFont_Flush_487F50()
{
    AE_IMPLEMENTED();
    ::DebugFont_Flush_4DD050();
}

}
