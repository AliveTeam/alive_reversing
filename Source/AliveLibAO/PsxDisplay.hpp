#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Psx_common.hpp"
#include "FixedPoint.hpp"

struct PrimHeader;

namespace AO {

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

struct PSX_Display_Buffer final
{
    PSX_DRAWENV mDrawEnv;
    PSX_DISPENV mDisplayEnv;
    PrimHeader* mOrderingTable[256];
};
ALIVE_ASSERT_SIZEOF(PSX_Display_Buffer, 0x470);

class PsxDisplay final
{
public:
    void Init();

    void RenderOrderingTable();

    void PutCurrentDispEnv();

    u16 mWidth;
    u16 mHeight;
    s16 mBitsPerPixel;
    s16 mMaxBuffers;
    u16 mBufferSize;
    u16 mBufferIndex;
    PSX_Display_Buffer mDrawEnvs[2];
};
ALIVE_ASSERT_SIZEOF(PsxDisplay, 0x8EC);

ALIVE_VAR_EXTERN(PsxDisplay, gPsxDisplay);

s32 DebugFont_Init_487EC0();

void DebugFont_Flush_487F50();


} // namespace AO
