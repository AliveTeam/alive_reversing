#include "stdafx_ao.h"
#include "PsxDisplay.hpp"
#include "Function.hpp"
#include "Psx.hpp"
#include "PsxRender.hpp"
#include "VRam.hpp"
#include "Primitives.hpp"
#include "../AliveLibAE/VRam.hpp"
#include "../AliveLibAE/PsxDisplay.hpp"
#include "../AliveLibAE/Primitives.hpp"
#include "../AliveLibCommon/Sys_common.hpp"

namespace AO {

ALIVE_VAR(1, 0x504C78, PsxDisplay, gPsxDisplay, {});


s32 DebugFont_Init_487EC0()
{
    return ::DebugFont_Init();
}

void DebugFont_Flush_487F50()
{
    ::DebugFont_Flush_4DD050();
}

void PsxDisplay::Init()
{
    PSX_SetDispMask_49AE80(0);
    PSX_VSync_496620(0);
	PSX_SetVideoMode_4FA8F0();

    mBufferIndex = 0;
    mBitsPerPixel = 16;

    mWidth = 640;
    mHeight = 240;

    mMaxBuffers = 1;
    mBufferSize = 43;

    PSX_SetGraphDebug_4989F0(0);
    PSX_ResetGraph_4987E0(0);

    Vram_init_495660();

    Vram_alloc_explicit(0, 0, 640, 32 + 240); // 272
 
    Pal_Area_Init_483080(0, 240, 640, 32);
    PSX_ClearOTag_496760(mDrawEnvs[0].mOrderingTable, mBufferSize);
    PSX_ClearOTag_496760(mDrawEnvs[1].mOrderingTable, mBufferSize);

    PSX_SetDefDrawEnv_495EF0(&mDrawEnvs[0].mDrawEnv, 0, 0, mWidth, mHeight);
    PSX_SetDefDispEnv_4959D0(&mDrawEnvs[0].mDisplayEnv, 0, 0, mWidth, mHeight);

    mDrawEnvs[0].mDrawEnv.field_17_dfe = 1;
    mDrawEnvs[1].mDrawEnv.field_17_dfe = 1;
    

    mDrawEnvs[0].mDisplayEnv.isinter = 0;
    mDrawEnvs[1].mDisplayEnv.isinter = 0;
    

    mDrawEnvs[0].mDisplayEnv.screen.x = 0;
    mDrawEnvs[1].mDisplayEnv.screen.x = 0;

    mDrawEnvs[0].mDisplayEnv.screen.y = 0;
    mDrawEnvs[1].mDisplayEnv.screen.y = 0;

    PSX_PutDrawEnv_495DD0(&mDrawEnvs[0].mDrawEnv);
    PSX_PutDispEnv_495CE0(&mDrawEnvs[0].mDisplayEnv);

    PSX_RECT rect = {};
    rect.x = 0;
    rect.y = 0;
    rect.w = 1024;
    rect.h = 512;
    PSX_ClearImage_496020(&rect, 0, 0, 0);

    PSX_DrawSync_496750(0);
    PSX_VSync_496620(0);
    PSX_SetDispMask_4989D0(1);
}


void PsxDisplay::PutCurrentDispEnv()
{
    PSX_PutDispEnv_495CE0(&mDrawEnvs[mBufferIndex].mDisplayEnv);
}

void PsxDisplay::RenderOrderingTable()
{
    if (mMaxBuffers <= 1u)
    {
        PSX_PutDrawEnv_495DD0(&mDrawEnvs[0].mDrawEnv);
        PSX_DrawOTag_4969F0(mDrawEnvs[0].mOrderingTable);
        PSX_DrawSync_496750(0);
        PSX_VSync_496620(2);
        PSX_PutDispEnv_495D30(&mDrawEnvs[0].mDisplayEnv);
        PSX_ClearOTag_496760(mDrawEnvs[0].mOrderingTable, mBufferSize);
        mBufferIndex = 0;
    }
    else
    {
        ALIVE_FATAL("More than 1 render buffer used");
    }
}


} // namespace AO
