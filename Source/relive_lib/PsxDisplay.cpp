#include "stdafx.h"
#include "PsxDisplay.hpp"
#include <type_traits>
#include "../AliveLibAE/PsxRender.hpp"
#include "GameType.hpp"
#include "ScreenManager.hpp"
#include "../AliveLibCommon/Sys_common.hpp"
#include "../AliveLibCommon/FatalError.hpp"

PsxDisplay gPsxDisplay = {};


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
        sbDisplayRenderFrame = 1;
    }
    else
    {
        sbDisplayRenderFrame = sbDisplayRenderFrame == 0;
    }

    sLastTicks_5CA4CC = ticks;
    sPreviousTime_5CA4C8 = currentTime;

    // Override if frame skip is off
    if (sCommandLine_NoFrameSkip)
    {
        sbDisplayRenderFrame = 1;
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
    TextRecords field_9_text;
};
ALIVE_ASSERT_SIZEOF(DebugTexts, 0x80C);

s32 sFntCount_BD0F28 = 0;
DebugTexts sTexts_C27640[4] = {};

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

char_type sDebugFontTmpBuffer_BB47CC[600] = {};
s16 sbDebugFontLoaded = 0;
s32 sDebugTextIdx_BB47C8 = 0;


s32 DebugFont_Open_4F8AB0(u8 xMargin, u8 yMargin, u8 displayWidth, u8 displayHeight, u32 maxLenChars)
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

s32 DebugFont_Init() // Font
{
    if (!sbDebugFontLoaded)
    {
        sbDebugFontLoaded = 1;
    }
    DebugFont_Reset_4F8B40();
    sDebugTextIdx_BB47C8 = DebugFont_Open_4F8AB0(8, 16, static_cast<u8>(gPsxDisplay.mWidth), 200, 600u);
    //nullsub_7(sTextIdx_BB47C8);
    sDebugFontTmpBuffer_BB47CC[0] = 0;
    return 0;
}


s32 DebugFont_Printf(s32 idx, const char_type* formatStr, ...)
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

void DebugFont_Flush()
{
    DebugFont_Printf(sDebugTextIdx_BB47C8, sDebugFontTmpBuffer_BB47CC);
    DebugFont_Update_Text_4F8BE0(sDebugTextIdx_BB47C8);
    sDebugFontTmpBuffer_BB47CC[0] = 0;
}

void PSX_DrawDebugTextBuffers()
{
    if (sFntCount_BD0F28 <= 0)
    {
        return;
    }

    //if (pBmp)
    {
        /*
        const LONG fontHeight = BMP_Get_Font_Height_4F21F0(pBmp);
        for (s32 i = 0; i < sFntCount_BD0F28; i++)
        {
            DebugTexts* pRecord = &sTexts_C27640[i];
            const s32 xpos = rect.left + pRecord->field_0_xMargin;
            s32 ypos = rect.top + pRecord->field_1_yMargin;
            for (char_type* j = strtok(pRecord->field_9_text.field_400_dst_txt, "\n\r"); j; j = strtok(0, "\n\r"))
            {
                BMP_Draw_String_4F2230(pBmp, xpos, ypos, j);
                ypos += fontHeight;
            }
        }*/
    }
}

void PsxDisplay::Init()
{
    PSX_VSync_4F6170(0);

    mBufferIndex = 0;
    mBitsPerPixel = 16;

    mWidth = 640;
    mHeight = 240;

    mMaxBuffers = 1;
    mBufferSize = 43;

    PSX_ClearOTag(mDrawEnvs[0].mOrderingTable, mBufferSize);
    PSX_ClearOTag(mDrawEnvs[1].mOrderingTable, mBufferSize);
    PSX_SetDefDrawEnv_4F5AA0(&mDrawEnvs[0].mDrawEnv, 0, 0, mWidth, mHeight);
    PSX_SetDefDispEnv_4F55A0(&mDrawEnvs[0].mDisplayEnv, 0, 0, mWidth, mHeight);

    mDrawEnvs[0].mDrawEnv.field_17_dfe = 1;
    mDrawEnvs[1].mDrawEnv.field_17_dfe = 1;

    mDrawEnvs[1].mDisplayEnv.screen.x = 0;
    mDrawEnvs[0].mDisplayEnv.screen.x = 0;

    mDrawEnvs[1].mDisplayEnv.screen.y = 0;
    mDrawEnvs[0].mDisplayEnv.screen.y = 0;

    mDrawEnvs[1].mDisplayEnv.screen.h = 240;
    mDrawEnvs[0].mDisplayEnv.screen.h = 240;

    PSX_PutDrawEnv_4F5980(&mDrawEnvs[0].mDrawEnv);
    //PSX_PutDispEnv_4F5890();

    //PSX_VSync_4F6170(0);
}

void PsxDisplay::PutCurrentDispEnv()
{
    PSX_PutDispEnv_4F5890();
}

bool sCommandLine_NoFrameSkip = false;
s32 sbDisplayRenderFrame = 1;

void PsxDisplay::RenderOrderingTable()
{
    if (mMaxBuffers <= 1)
    {
        // Single buffered rendering
        PSX_PutDrawEnv_4F5980(&mDrawEnvs[0].mDrawEnv);
        PSX_Calc_FrameSkip_4945D0();
        if (sCommandLine_NoFrameSkip)
        {
            PSX_DrawOTag(mDrawEnvs[0].mOrderingTable);
        }
        else
        {
            if (sbDisplayRenderFrame)
            {
                PSX_DrawOTag(mDrawEnvs[0].mOrderingTable);
            }
            else
            {
                turn_off_rendering_BD0F20 = 1;
            }
            PSX_VSync_4F6170(2);
        }
        PSX_PutDispEnv_4F58E0();
        PSX_ClearOTag(mDrawEnvs[0].mOrderingTable, mBufferSize);
        mBufferIndex = 0;
    }
    else
    {
        ALIVE_FATAL("More than 1 render buffer used");
    }
}
