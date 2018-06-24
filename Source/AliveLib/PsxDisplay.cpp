#include "stdafx.h"
#include "PsxDisplay.hpp"
#include "Function.hpp"
#include "ScreenManager.hpp"

ALIVE_VAR(1, 0x5C1130, PsxDisplay, gPsxDisplay_5C1130, {});

EXPORT void __cdecl PSX_PutDrawEnv_4F5980(const PSX_DRAWENV *pDrawEnv)
{
    NOT_IMPLEMENTED();
}

EXPORT void __cdecl sub_4945D0()
{
    NOT_IMPLEMENTED();
}

EXPORT void __cdecl PSX_Display_OrderingTable_4F6540(int* pOT)
{
    NOT_IMPLEMENTED();
}

EXPORT void __cdecl PSX_OrderingTable_Init_4F6290(int* otBuffer, int otBufferSize)
{
    NOT_IMPLEMENTED();
}

EXPORT void __cdecl PSX_4F58E0(const PSX_DISPENV *pDispEnv)
{
    NOT_IMPLEMENTED();
}

ALIVE_VAR(1, 0x5CA4D1, bool, sCommandLine_NoFrameSkip_5CA4D1, false);
ALIVE_VAR(1, 0x55EF8C, int, sbDisplayRenderFrame_55EF8C, 1);
ALIVE_VAR(1, 0xBD0F20, bool, byte_BD0F20, 0);

void PsxDisplay::PSX_Display_Render_OT_41DDF0()
{
    if (field_8_max_buffers <= 1)
    {
        // Single buffered rendering
        PSX_PutDrawEnv_4F5980(&field_10_drawEnv[0].field_0_draw_env);
        sub_4945D0();
        if (sCommandLine_NoFrameSkip_5CA4D1)
        {
            PSX_Display_OrderingTable_4F6540(field_10_drawEnv[0].field_70_ot_buffer);
            PSX_DrawSync_4F6280(0);
        }
        else
        {
            if (sbDisplayRenderFrame_55EF8C)
            {
                PSX_Display_OrderingTable_4F6540(field_10_drawEnv[0].field_70_ot_buffer);
                PSX_DrawSync_4F6280(0);
            }
            else
            {
                pScreenManager_5BB5F4->sub_40EE10();
                byte_BD0F20 = 1;
            }
            PSX_VSync_4F6170(2);
        }
        PSX_4F58E0(&field_10_drawEnv[0].field_5C_disp_env);
        PSX_OrderingTable_Init_4F6290(field_10_drawEnv[0].field_70_ot_buffer, field_A_buffer_size);
        field_C_buffer_index = 0;
    }
    else
    {
        PSX_DrawSync_4F6280(0);
        PSX_VSync_4F6170(2);

        const auto prevBufferIdx = field_C_buffer_index;
        field_C_buffer_index++;
        if (field_C_buffer_index > field_8_max_buffers)
        {
            field_C_buffer_index = 0;
        }

        // Set up next
        PSX_OrderingTable_Init_4F6290(field_10_drawEnv[field_C_buffer_index].field_70_ot_buffer,  field_A_buffer_size);
        PSX_4F58E0(&field_10_drawEnv[field_C_buffer_index].field_5C_disp_env);
        PSX_PutDrawEnv_4F5980(&field_10_drawEnv[field_C_buffer_index].field_0_draw_env);

        // Display current
        PSX_Display_OrderingTable_4F6540(field_10_drawEnv[prevBufferIdx].field_70_ot_buffer);
    }
}
