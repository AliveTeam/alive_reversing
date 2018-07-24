#include "stdafx.h"
#include "PsxDisplay.hpp"
#include "Function.hpp"
#include "ScreenManager.hpp"
#include "VGA.hpp"
#include "Error.hpp"
#include "Midi.hpp"
#include "stdlib.hpp"
#include <type_traits>
#include <gmock/gmock.h>

ALIVE_VAR(1, 0x5C1130, PsxDisplay, gPsxDisplay_5C1130, {});


EXPORT void CC sub_4945D0()
{
    NOT_IMPLEMENTED();
}

// TODO: Probably not font related at all?
EXPORT int __cdecl Fnt_4955F0(__int16 a1, __int16 a2, __int16 a3, __int16 a4)
{
    NOT_IMPLEMENTED();
    return 0;
}

/*
// No visible effect but just resets data
void __cdecl sub_495660()
{

}

// LED font/menu glyphs related
signed int __cdecl sub_4958F0(signed __int16 *a1, char a2)
{
    return 1;
}

// LED font rendering related
void __cdecl sub_495A60(int a1, int a2)
{

}
*/


signed __int16 CC Vram_alloc_4956C0(unsigned __int16 width, __int16 height, unsigned __int16 colourDepth, PSX_RECT *pRect)
{
    NOT_IMPLEMENTED();
}

void CC Vram_free_495A60(int xy, int wh)
{
    NOT_IMPLEMENTED();
}

struct TextRecords
{
    char field_0_src_txt[1024];
    char field_400_dst_txt[1027];
};
ALIVE_ASSERT_SIZEOF(TextRecords, 0x803);

struct DebugTexts
{
    BYTE field_0_xMargin;
    BYTE field_1_yMargin;
    BYTE field_2_displayWidth;
    BYTE field_3_displayHeight;
    DWORD field_4_max_len;
    BYTE field_8_bgColour;
    TextRecords field_9_text;
};
ALIVE_ASSERT_SIZEOF(DebugTexts, 0x80C);

ALIVE_VAR(1, 0xBD0F28, int, sFntCount_BD0F28, 0);
ALIVE_ARY(1, 0xC27640, DebugTexts, 4, sTexts_C27640, {});

EXPORT void CC DebugFont_Reset_4F8B40()
{
    memset(sTexts_C27640, 0, sizeof(DebugTexts)*4); // 8240u
    sFntCount_BD0F28 = 0;
}

EXPORT void CC DebugFont_Update_Text_4F8BE0(signed int idx)
{
    if (idx >= 0 && idx <= 3)
    {
        strcpy(sTexts_C27640[idx].field_9_text.field_400_dst_txt, sTexts_C27640[idx].field_9_text.field_0_src_txt);
        sTexts_C27640[idx].field_9_text.field_0_src_txt[0] = 0;
    }
}

ALIVE_ARY(1, 0xBB47CC, char, 600, sDebugFontTmpBuffer_BB47CC, {});
ALIVE_VAR(1, 0xBB4A24, short, sbDebugFontLoaded_BB4A24, 0);
ALIVE_VAR(1, 0xBB47C8, int, sDebugTextIdx_BB47C8, 0);


EXPORT int CC DebugFont_Open_4F8AB0(BYTE xMargin, BYTE yMargin, BYTE displayWidth, BYTE displayHeight, BYTE bgColour, unsigned int maxLenChars)
{
    const int idx = sFntCount_BD0F28;
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

    int limitedMaxLen = maxLenChars;
    if (maxLenChars > 1023)
    {
        limitedMaxLen = 1023;
    }
    sTexts_C27640[idx].field_4_max_len = limitedMaxLen;
    return idx;
}

EXPORT int CC DebugFont_Init_4DCF40() // Font
{
    if (!sbDebugFontLoaded_BB4A24)
    {
        Fnt_4955F0(960, 256, 991, 287);
        Fnt_4955F0(960, 384, 975, 385);
        sbDebugFontLoaded_BB4A24 = 1;
    }
    DebugFont_Reset_4F8B40();
    sDebugTextIdx_BB47C8 = DebugFont_Open_4F8AB0(8, 16, static_cast<BYTE>(gPsxDisplay_5C1130.field_0_width), 200, 0, 600u);
    //nullsub_7(sTextIdx_BB47C8);
    sDebugFontTmpBuffer_BB47CC[0] = 0;
    return 0;
}


EXPORT int CC DebugFont_Printf_4F8B60(int idx, const char* formatStr, ...)
{
    va_list va;
    va_start(va, formatStr);
    if (idx < 0 || idx > 3)
    {
        return -1;
    }

    char buffer[1024] = {};
    vsprintf(buffer, formatStr, va);
    strncat(sTexts_C27640[idx].field_9_text.field_0_src_txt, buffer, sTexts_C27640[idx].field_4_max_len);
    return strlen(sTexts_C27640[idx].field_9_text.field_0_src_txt);
}

EXPORT void CC DebugFont_Flush_4DD050()
{
    DebugFont_Printf_4F8B60(sDebugTextIdx_BB47C8, sDebugFontTmpBuffer_BB47CC);
    DebugFont_Update_Text_4F8BE0(sDebugTextIdx_BB47C8);
    sDebugFontTmpBuffer_BB47CC[0] = 0;
}

namespace Test
{
    static void Test_sub_4F8AB0()
    {
        DebugFont_Reset_4F8B40();

        DebugFont_Open_4F8AB0(8, 16, 77, 200, 33, 600);
        ASSERT_EQ(16, sTexts_C27640[0].field_1_yMargin);
        ASSERT_EQ(0, sTexts_C27640[0].field_9_text.field_0_src_txt[0]);
        ASSERT_EQ(0, sTexts_C27640[0].field_9_text.field_400_dst_txt[0]);

        DebugFont_Open_4F8AB0(8, 22, 77, 200, 33, 600);
        ASSERT_EQ(22, sTexts_C27640[1].field_1_yMargin);

        DebugFont_Open_4F8AB0(8, 33, 77, 200, 33, 600);
        ASSERT_EQ(33, sTexts_C27640[2].field_1_yMargin);

        DebugFont_Open_4F8AB0(8, 55, 77, 200, 33, 600);
        ASSERT_EQ(55, sTexts_C27640[3].field_1_yMargin);

        ASSERT_EQ(-1, DebugFont_Open_4F8AB0(8, 33, 77, 200, 33, 600));

        DebugFont_Reset_4F8B40();
    }

    void PsxDisplayTests()
    {
        Test_sub_4F8AB0();
    }
}

void PSX_DrawDebugTextBuffers(Bitmap* pBmp, const RECT& rect)
{
    if (sFntCount_BD0F28 <= 0)
    {
        return;
    }

    const LONG fontHeight = BMP_Get_Font_Height_4F21F0(pBmp);
    for (int i = 0; i < sFntCount_BD0F28; i++)
    {
        DebugTexts* pRecord = &sTexts_C27640[i];
        const int xpos = rect.left + pRecord->field_0_xMargin;
        int ypos = rect.top + pRecord->field_1_yMargin;
        const int bgColour = pRecord->field_8_bgColour;
        for (char* j = strtok(pRecord->field_9_text.field_400_dst_txt, "\n\r"); j; j = strtok(0, "\n\r"))
        {
            int fontColour = Bmp_Convert_Colour_4F17D0(&sPsxVram_C1D160, 255, 255, 191);
            BMP_Draw_String_4F2230(pBmp, xpos, ypos, fontColour, bgColour, j);
            ypos += fontHeight;
        }
    }
}

EXPORT void __cdecl sub_495660()
{
    NOT_IMPLEMENTED();
}

EXPORT int __cdecl sub_483080(__int16 , __int16 , unsigned __int16 , unsigned __int16 )
{
    NOT_IMPLEMENTED();
}

void PsxDisplay::ctor_41DC30()
{
    PSX_VSync_4F6170(0);
    PSX_SetDispMask_4F89F0(0);
    PSX_SetVideoMode_4FA8F0();
    field_0_width = 640;
    field_2_height = 240;
    field_4 = 0;
    field_6_bpp = 16;
    field_8_max_buffers = 1;
    field_A_buffer_size = 43;
    field_C_buffer_index = 0;
    PSX_ResetGraph_4F8800(0);
    PSX_SetGraphDebug_4F8A10(0);
    sub_495660();
    Fnt_4955F0(0, 0, 639, 271);
    sub_483080(0, 240, 640, 32);
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
ALIVE_VAR(1, 0x55EF8C, int, sbDisplayRenderFrame_55EF8C, 1);

void PsxDisplay::PSX_Display_Render_OT_41DDF0()
{
    if (field_8_max_buffers <= 1)
    {
        // Single buffered rendering
        PSX_PutDrawEnv_4F5980(&field_10_drawEnv[0].field_0_draw_env);
        sub_4945D0();
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
                pScreenManager_5BB5F4->sub_40EE10();
                byte_BD0F20 = 1;
            }
            PSX_VSync_4F6170(2);
        }
        PSX_PutDispEnv_4F58E0(&field_10_drawEnv[0].field_5C_disp_env);
        PSX_ClearOTag_4F6290(field_10_drawEnv[0].field_70_ot_buffer, field_A_buffer_size);
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
        PSX_ClearOTag_4F6290(field_10_drawEnv[field_C_buffer_index].field_70_ot_buffer,  field_A_buffer_size);
        PSX_PutDispEnv_4F58E0(&field_10_drawEnv[field_C_buffer_index].field_5C_disp_env);
        PSX_PutDrawEnv_4F5980(&field_10_drawEnv[field_C_buffer_index].field_0_draw_env);

        // Display current
        PSX_DrawOTag_4F6540(field_10_drawEnv[prevBufferIdx].field_70_ot_buffer);
    }
}
