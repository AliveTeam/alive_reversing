#include "stdafx.h"
#include "PsxDisplay.hpp"
#include "Function.hpp"
#include "ScreenManager.hpp"
#include "VGA.hpp"
#include "Error.hpp"
#include "Sound/Midi.hpp"
#include "stdlib.hpp"
#include <type_traits>
#include "VRam.hpp"
#include "DebugHelpers.hpp"
#include "PsxRender.hpp"
#include "Sys.hpp"
#include <gmock/gmock.h>

ALIVE_VAR(1, 0x5C1130, PsxDisplay, gPsxDisplay_5C1130, {});


void PSX_Calc_FrameSkip_4945D0()
{
    static u32 delta_time_ring_buffer_5CA310[10] = {};
    static u32 sPreviousTime_5CA4C8 = 0;
    static u32 sLastTicks_5CA4CC = 0;

    const u32 currentTime = SYS_GetTicks();
    const s32 ticks = currentTime - sPreviousTime_5CA4C8 - delta_time_ring_buffer_5CA310[0] + sLastTicks_5CA4CC;

    // Move all elements down one, so the the last value is "empty"
    for (s32 i = 0; i < ALIVE_COUNTOF(delta_time_ring_buffer_5CA310) - 1; i++)
    {
        delta_time_ring_buffer_5CA310[i] = delta_time_ring_buffer_5CA310[i + 1];
    }

    // Store at the last/"empty" value
    delta_time_ring_buffer_5CA310[ALIVE_COUNTOF(delta_time_ring_buffer_5CA310) - 1] = currentTime - sPreviousTime_5CA4C8;

    if (ticks <= 400)
    {
        sbDisplayRenderFrame_55EF8C = 1;
    }
    else
    {
        sbDisplayRenderFrame_55EF8C = sbDisplayRenderFrame_55EF8C == 0;
    }

    sLastTicks_5CA4CC = ticks;
    sPreviousTime_5CA4C8 = currentTime;

    // Override if frame skip is off
    if (sCommandLine_NoFrameSkip_5CA4D1)
    {
        sbDisplayRenderFrame_55EF8C = 1;
    }
}

struct TextRecords final
{
    char_type field_0_src_txt[1024];
    char_type field_400_dst_txt[1027];
};
ALIVE_ASSERT_SIZEOF(TextRecords, 0x803);

struct DebugTexts final
{
    u8 field_0_xMargin;
    u8 field_1_yMargin;
    u8 field_2_displayWidth;
    u8 field_3_displayHeight;
    u32 field_4_max_len;
    u8 field_8_bgColour;
    TextRecords field_9_text;
};
ALIVE_ASSERT_SIZEOF(DebugTexts, 0x80C);

ALIVE_VAR(1, 0xBD0F28, s32, sFntCount_BD0F28, 0);
ALIVE_ARY(1, 0xC27640, DebugTexts, 4, sTexts_C27640, {});

void DebugFont_Reset_4F8B40()
{
    memset(sTexts_C27640, 0, sizeof(DebugTexts) * 4); // 8240u
    sFntCount_BD0F28 = 0;
}

void DebugFont_Update_Text_4F8BE0(s32 idx)
{
    if (idx >= 0 && idx <= 3)
    {
        strcpy(sTexts_C27640[idx].field_9_text.field_400_dst_txt, sTexts_C27640[idx].field_9_text.field_0_src_txt);
        sTexts_C27640[idx].field_9_text.field_0_src_txt[0] = 0;
    }
}

ALIVE_ARY(1, 0xBB47CC, char_type, 600, sDebugFontTmpBuffer_BB47CC, {});
ALIVE_VAR(1, 0xBB4A24, s16, sbDebugFontLoaded_BB4A24, 0);
ALIVE_VAR(1, 0xBB47C8, s32, sDebugTextIdx_BB47C8, 0);


s32 DebugFont_Open_4F8AB0(u8 xMargin, u8 yMargin, u8 displayWidth, u8 displayHeight, u8 bgColour, u32 maxLenChars)
{
    const s32 idx = sFntCount_BD0F28;
    if (sFntCount_BD0F28 == 4)
    {
        return -1;
    }

    ++sFntCount_BD0F28;

    sTexts_C27640[idx].field_0_xMargin = xMargin;
    sTexts_C27640[idx].field_1_yMargin = yMargin;
    sTexts_C27640[idx].field_2_displayWidth = displayWidth;
    sTexts_C27640[idx].field_3_displayHeight = displayHeight;
    sTexts_C27640[idx].field_8_bgColour = bgColour | 1;
    sTexts_C27640[idx].field_9_text.field_0_src_txt[0] = 0;
    sTexts_C27640[idx].field_9_text.field_400_dst_txt[0] = 0;

    s32 limitedMaxLen = maxLenChars;
    if (maxLenChars > 1023)
    {
        limitedMaxLen = 1023;
    }
    sTexts_C27640[idx].field_4_max_len = limitedMaxLen;
    return idx;
}

s32 DebugFont_Init_4DCF40() // Font
{
    if (!sbDebugFontLoaded_BB4A24)
    {
        Vram_alloc_explicit_4955F0(960, 256, 991, 287);
        Vram_alloc_explicit_4955F0(960, 384, 975, 385);
        sbDebugFontLoaded_BB4A24 = 1;
    }
    DebugFont_Reset_4F8B40();
    sDebugTextIdx_BB47C8 = DebugFont_Open_4F8AB0(8, 16, static_cast<u8>(gPsxDisplay_5C1130.field_0_width), 200, 0, 600u);
    //nullsub_7(sTextIdx_BB47C8);
    sDebugFontTmpBuffer_BB47CC[0] = 0;
    return 0;
}


s32 DebugFont_Printf_4F8B60(s32 idx, const char_type* formatStr, ...)
{
    va_list va;
    va_start(va, formatStr);
    if (idx < 0 || idx > 3)
    {
        return -1;
    }

    char_type buffer[1024] = {};
    vsprintf(buffer, formatStr, va);
    strncat(sTexts_C27640[idx].field_9_text.field_0_src_txt, buffer, sTexts_C27640[idx].field_4_max_len);
    return static_cast<s32>(strlen(sTexts_C27640[idx].field_9_text.field_0_src_txt));
}

void DebugFont_Flush_4DD050()
{
    DebugFont_Printf_4F8B60(sDebugTextIdx_BB47C8, sDebugFontTmpBuffer_BB47CC);
    DebugFont_Update_Text_4F8BE0(sDebugTextIdx_BB47C8);
    sDebugFontTmpBuffer_BB47CC[0] = 0;
}

void PSX_DrawDebugTextBuffers(Bitmap* pBmp, const RECT& rect)
{
    if (sFntCount_BD0F28 <= 0)
    {
        return;
    }

    const LONG fontHeight = BMP_Get_Font_Height_4F21F0(pBmp);
    for (s32 i = 0; i < sFntCount_BD0F28; i++)
    {
        DebugTexts* pRecord = &sTexts_C27640[i];
        const s32 xpos = rect.left + pRecord->field_0_xMargin;
        s32 ypos = rect.top + pRecord->field_1_yMargin;
        const s32 bgColour = pRecord->field_8_bgColour;
        for (char_type* j = strtok(pRecord->field_9_text.field_400_dst_txt, "\n\r"); j; j = strtok(0, "\n\r"))
        {
            s32 fontColour = Bmp_Convert_Colour_4F17D0(&sPsxVram_C1D160, 255, 255, 191);
            BMP_Draw_String_4F2230(pBmp, xpos, ypos, fontColour, bgColour, j);
            ypos += fontHeight;
        }
    }
}

void PsxDisplay::ctor_41DC30()
{
    PSX_VSync_4F6170(0);
    PSX_SetDispMask_4F89F0(0);
    PSX_SetVideoMode_4FA8F0();
    field_0_width = 640;
    field_2_height = 240;
    field_4_unused = 0;
    field_6_bpp = 16;
    field_8_max_buffers = 1;
    field_A_buffer_size = 43;
    field_C_buffer_index = 0;
    PSX_ResetGraph_4F8800(0);
    PSX_SetGraphDebug_4F8A10(0);
    Vram_init_495660();
    Vram_alloc_explicit_4955F0(0, 0, 639, 271);
    Pal_Area_Init_483080(0, 240, 640, 32);
    PSX_ClearOTag_4F6290(field_10_drawEnv[0].field_70_ot_buffer, field_A_buffer_size);
    PSX_ClearOTag_4F6290(field_10_drawEnv[1].field_70_ot_buffer, field_A_buffer_size);
    PSX_SetDefDrawEnv_4F5AA0(&field_10_drawEnv[0].field_0_draw_env, 0, 0, field_0_width, field_2_height);
    PSX_SetDefDispEnv_4F55A0(&field_10_drawEnv[0].field_5C_disp_env, 0, 0, field_0_width, field_2_height);

    field_10_drawEnv[0].field_0_draw_env.field_17_dfe = 1;
    field_10_drawEnv[1].field_0_draw_env.field_17_dfe = 1;

    field_10_drawEnv[1].field_5C_disp_env.screen.x = 0;
    field_10_drawEnv[0].field_5C_disp_env.screen.x = 0;

    field_10_drawEnv[1].field_5C_disp_env.screen.y = 0;
    field_10_drawEnv[0].field_5C_disp_env.screen.y = 0;

    field_10_drawEnv[1].field_5C_disp_env.screen.h = 240;
    field_10_drawEnv[0].field_5C_disp_env.screen.h = 240;

    PSX_PutDrawEnv_4F5980(&field_10_drawEnv[0].field_0_draw_env);
    PSX_PutDispEnv_4F5890(&field_10_drawEnv[0].field_5C_disp_env);

    PSX_RECT rect = {};
    rect.x = 0;
    rect.y = 0;
    rect.w = 1024;
    rect.h = 512;
    PSX_ClearImage_4F5BD0(&rect, 0, 0, 0);
    PSX_DrawSync_4F6280(0);
    PSX_VSync_4F6170(0);
    PSX_SetDispMask_4F89F0(1);
}

void PsxDisplay::PutCurrentDispEnv_41DFA0()
{
    PSX_PutDispEnv_4F5890(&field_10_drawEnv[field_C_buffer_index].field_5C_disp_env);
}

ALIVE_VAR(1, 0x5CA4D1, bool, sCommandLine_NoFrameSkip_5CA4D1, false);
ALIVE_VAR(1, 0x55EF8C, s32, sbDisplayRenderFrame_55EF8C, 1);

void PsxDisplay::PSX_Display_Render_OT_41DDF0()
{
#if DEVELOPER_MODE
    DEV::DebugOnFrameDraw(field_10_drawEnv[0].field_70_ot_buffer);
#endif

    if (field_8_max_buffers <= 1)
    {
        // Single buffered rendering
        PSX_PutDrawEnv_4F5980(&field_10_drawEnv[0].field_0_draw_env);
        PSX_Calc_FrameSkip_4945D0();
        if (sCommandLine_NoFrameSkip_5CA4D1)
        {
            PSX_DrawOTag_4F6540(field_10_drawEnv[0].field_70_ot_buffer);
            PSX_DrawSync_4F6280(0);
        }
        else
        {
            if (sbDisplayRenderFrame_55EF8C)
            {
                PSX_DrawOTag_4F6540(field_10_drawEnv[0].field_70_ot_buffer);
                PSX_DrawSync_4F6280(0);
            }
            else
            {
                pScreenManager->sub_40EE10();
                turn_off_rendering_BD0F20 = 1;
            }
            PSX_VSync_4F6170(2);
        }
        PSX_PutDispEnv_4F58E0(&field_10_drawEnv[0].field_5C_disp_env);
        PSX_ClearOTag_4F6290(field_10_drawEnv[0].field_70_ot_buffer, field_A_buffer_size);
        field_C_buffer_index = 0;
    }
    else
    {
        ALIVE_FATAL("More than 1 render buffer used");
    }
}
