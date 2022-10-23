#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../AliveLibCommon/pch_shared.h"
#include "../AliveLibAE/Psx.hpp"

// 368*40/23 =640
// This seems to convert from PSX coordinate space to PC coordinate space
// anywhere you see this calc replace it with this function
template <typename T>
inline T PsxToPCX(T x, s32 addToX = 0)
{
    return static_cast<T>(((40 * x) + static_cast<T>(addToX)) / 23);
}

inline FP PsxToPCX(FP x, FP addToX = FP_FromInteger(0))
{
    return ((FP_FromInteger(40) * x) + addToX) / FP_FromInteger(23);
}

// 640 * 23 / 40 =  368
template <typename T>
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

class PsxDisplay final
{
public:
    void Init();
    void PutCurrentDispEnv();
    void RenderOrderingTable();

	s16 mWidth = 0;
	s16 mHeight = 0;
    s16 mBitsPerPixel = 0;
    s16 mMaxBuffers = 0;
    u16 mBufferSize = 0;
    u16 mBufferIndex = 0;
    PSX_Display_Buffer mDrawEnvs[2] = {};
};

extern PsxDisplay gPsxDisplay;
extern bool sCommandLine_NoFrameSkip;
extern s16 sbDebugFontLoaded;
extern s32 sbDisplayRenderFrame;

void DebugFont_Flush();
s32 DebugFont_Printf(s32 idx, const char_type* formatStr, ...);
s32 DebugFont_Init();

struct Bitmap;
void PSX_DrawDebugTextBuffers(Bitmap* pBmp, const RECT& rect);
