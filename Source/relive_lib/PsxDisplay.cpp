#include "stdafx.h"
#include "PsxDisplay.hpp"
#include <type_traits>
#include "../AliveLibAE/PsxRender.hpp"
#include "Sys.hpp"
#include "FatalError.hpp"
#include "Psx.hpp"

PsxDisplay gPsxDisplay = {};


void PSX_Calc_FrameSkip()
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
        gDisplayRenderFrame = true;
    }
    else
    {
        gDisplayRenderFrame = gDisplayRenderFrame == false;
    }

    sLastTicks_5CA4CC = ticks;
    sPreviousTime_5CA4C8 = currentTime;

    // Override if frame skip is off
    if (gCommandLine_NoFrameSkip)
    {
        gDisplayRenderFrame = true;
    }
}

struct TextRecords final
{
    char_type mSrcTxt[1024];
    char_type mDstTxt[1027];
};

struct DebugTexts final
{
    u8 mMarginX;
    u8 mMarginY;
    u8 mDisplayWidth;
    u8 mDisplayHeight;
    u32 mMaxLength;
    TextRecords mText;
};

static s32 sFntCount = 0;
static DebugTexts sDebugTexts[4] = {};

void DebugFont_Reset_4F8B40()
{
    memset(sDebugTexts, 0, sizeof(DebugTexts) * 4); // 8240u
    sFntCount = 0;
}

void DebugFont_Update_Text_4F8BE0(s32 idx)
{
    if (idx >= 0 && idx <= 3)
    {
        strcpy(sDebugTexts[idx].mText.mDstTxt, sDebugTexts[idx].mText.mSrcTxt);
        sDebugTexts[idx].mText.mSrcTxt[0] = 0;
    }
}

static char_type sDebugFontTmpBuffer[600] = {};
bool gDebugFontLoaded = false;
static s32 sDebugTextIdx = 0;


s32 DebugFont_Open_4F8AB0(u8 xMargin, u8 yMargin, u8 displayWidth, u8 displayHeight, u32 maxLenChars)
{
    const s32 idx = sFntCount;
    if (sFntCount == 4)
    {
        return -1;
    }

    ++sFntCount;

    sDebugTexts[idx].mMarginX = xMargin;
    sDebugTexts[idx].mMarginY = yMargin;
    sDebugTexts[idx].mDisplayWidth = displayWidth;
    sDebugTexts[idx].mDisplayHeight = displayHeight;
    sDebugTexts[idx].mText.mSrcTxt[0] = 0;
    sDebugTexts[idx].mText.mDstTxt[0] = 0;

    s32 limitedMaxLen = maxLenChars;
    if (maxLenChars > 1023)
    {
        limitedMaxLen = 1023;
    }
    sDebugTexts[idx].mMaxLength = limitedMaxLen;
    return idx;
}

s32 DebugFont_Init() // Font
{
    if (!gDebugFontLoaded)
    {
        gDebugFontLoaded = true;
    }
    DebugFont_Reset_4F8B40();
    sDebugTextIdx = DebugFont_Open_4F8AB0(8, 16, static_cast<u8>(gPsxDisplay.mWidth), 200, 600u);
    //nullsub_7(sTextIdx_BB47C8);
    sDebugFontTmpBuffer[0] = 0;
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
    strncat(sDebugTexts[idx].mText.mSrcTxt, buffer, sDebugTexts[idx].mMaxLength);
    return static_cast<s32>(strlen(sDebugTexts[idx].mText.mSrcTxt));
}

void DebugFont_Flush()
{
    DebugFont_Printf(sDebugTextIdx, sDebugFontTmpBuffer);
    DebugFont_Update_Text_4F8BE0(sDebugTextIdx);
    sDebugFontTmpBuffer[0] = 0;
}

void PSX_DrawDebugTextBuffers()
{
    if (sFntCount <= 0)
    {
        return;
    }

    //if (pBmp)
    {
        /*
        const LONG fontHeight = BMP_Get_Font_Height_4F21F0(pBmp);
        for (s32 i = 0; i < sFntCount; i++)
        {
            DebugTexts* pRecord = &sDebugTexts[i];
            const s32 xpos = rect.left + pRecord->mMarginX;
            s32 ypos = rect.top + pRecord->mMarginY;
            for (char_type* j = strtok(pRecord->mText.mDstTxt, "\n\r"); j; j = strtok(0, "\n\r"))
            {
                BMP_Draw_String_4F2230(pBmp, xpos, ypos, j);
                ypos += fontHeight;
            }
        }*/
    }
}

void PsxDisplay::Init()
{
    PSX_VSync(0);

    mBufferIndex = 0;
    mBitsPerPixel = 16;

    mWidth = 640;
    mHeight = 240;

    mMaxBuffers = 1;
    mBufferSize = 43;

    mDrawEnvs[0].mOrderingTable.Clear();
    mDrawEnvs[1].mOrderingTable.Clear();

    PSX_SetDefDispEnv_4F55A0(&mDrawEnvs[0].mDisplayEnv);

    mDrawEnvs[1].mDisplayEnv.screen.x = 0;
    mDrawEnvs[0].mDisplayEnv.screen.x = 0;

    mDrawEnvs[1].mDisplayEnv.screen.y = 0;
    mDrawEnvs[0].mDisplayEnv.screen.y = 0;

    mDrawEnvs[1].mDisplayEnv.screen.h = 240;
    mDrawEnvs[0].mDisplayEnv.screen.h = 240;

   // PSX_PutDispEnv_4F5890();

    PSX_VSync(0);
}

void PsxDisplay::PutCurrentDispEnv()
{
    PSX_PutDispEnv_4F5890();
}

bool gCommandLine_NoFrameSkip = false;
bool gDisplayRenderFrame = true;

void PsxDisplay::RenderOrderingTable()
{
    if (mMaxBuffers <= 1)
    {
        // Single buffered rendering
        PSX_Calc_FrameSkip();
        if (gCommandLine_NoFrameSkip)
        {
            PSX_DrawOTag(mDrawEnvs[0].mOrderingTable);
        }
        else
        {
            if (gDisplayRenderFrame)
            {
                PSX_DrawOTag(mDrawEnvs[0].mOrderingTable);
            }
            else
            {
                gTurnOffRendering = true;
            }
            PSX_VSync(2);
        }
        PSX_PutDispEnv_4F58E0();
        mDrawEnvs[0].mOrderingTable.Clear();
        mBufferIndex = 0;
    }
    else
    {
        ALIVE_FATAL("More than 1 render buffer used");
    }
}
