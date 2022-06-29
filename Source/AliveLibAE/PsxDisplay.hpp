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
    PSX_DRAWENV mDrawEnv;
    PSX_DISPENV mDisplayEnv;
    PrimHeader* mOrderingTable[256];
};
// TODO: Size

class PsxDisplay final
{
public:
    s16 mWidth = 0;
    s16 mHeight = 0;
    s16 mBitsPerPixel = 0;
    s16 mMaxBuffers = 0;
    u16 mBufferSize = 0;
    u16 mBufferIndex = 0;
    PSX_Display_Buffer mDrawEnvs[2] = {};

    void Init();
    void PutCurrentDispEnv();
    void RenderOrderingTable();
};
// TODO: Size

ALIVE_VAR_EXTERN(PsxDisplay, gPsxDisplay);
ALIVE_VAR_EXTERN(bool, sCommandLine_NoFrameSkip);
ALIVE_VAR_EXTERN(s16, sbDebugFontLoaded);
ALIVE_VAR_EXTERN(s32, sbDisplayRenderFrame);

void DebugFont_Flush_4DD050();
s32 DebugFont_Printf(s32 idx, const char_type* formatStr, ...);
s32 DebugFont_Init();

void PSX_DrawDebugTextBuffers(Bitmap* pBmp, const RECT& rect);
