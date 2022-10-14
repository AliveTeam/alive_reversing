#include "stdafx_ao.h"
#include "Psx.hpp"
#include "Function.hpp"
#include "../relive_lib/bmp.hpp"
#include "VGA.hpp"
#include "PsxRender.hpp"
#include "../relive_lib/Error.hpp"
#include "../AliveLibAE/Psx.hpp"
#include "Sys.hpp"

namespace AO {

void PSX_PutDispEnv_495D30(PSX_DISPENV* pDispEnv)
{
    PSX_PutDispEnv_4F58E0(pDispEnv);
}

void PSX_VSync_496620(s32 mode)
{
    PSX_VSync_4F6170(mode);
}

void PSX_SetDefDrawEnv_495EF0(PSX_DRAWENV* pDrawEnv, s16 x, s16 y, s16 w, s16 h)
{
    PSX_SetDefDrawEnv_4F5AA0(pDrawEnv, x, y, w, h);
}

void PSX_PutDrawEnv_495DD0(const PSX_DRAWENV* pDrawEnv)
{
    PSX_PutDrawEnv_4F5980(pDrawEnv);
}

void PSX_SetDefDispEnv_4959D0(PSX_DISPENV* pOutEnv, s16 x, s16 y, s16 w, s16 h)
{
    PSX_SetDefDispEnv_4F55A0(pOutEnv, x, y, w, h);
}

void PSX_PutDispEnv_495CE0(PSX_DISPENV* pDispEnv)
{
    PSX_PutDispEnv_4F5890(pDispEnv);
}

CdlLOC* PSX_Pos_To_CdLoc_49B340(s32 pos, CdlLOC* pLoc)
{
    return PSX_Pos_To_CdLoc_4FADD0(pos, pLoc);
}

void PSX_Prevent_Rendering_44FFB0()
{
    PSX_Prevent_Rendering_4945B0();
}

void PSX_EMU_Init_49A1D0()
{
    PSX_EMU_Init_4F9CD0();
}

s32 PSX_EMU_VideoAlloc_49A2B0()
{
    return PSX_EMU_VideoAlloc_4F9D70();
}

void PSX_EMU_SetCallBack_499920(s32 callBackType, TPsxEmuCallBack fnPtr)
{
    PSX_EMU_SetCallBack_4F9430(callBackType, fnPtr);
}

void PSX_EMU_VideoDeAlloc_49A550()
{
    PSX_EMU_VideoDeAlloc_4FA010();
}

void Init_VGA_AndPsxVram()
{
    bool bFullScreen = true;
#if BEHAVIOUR_CHANGE_FORCE_WINDOW_MODE
    LOG_INFO("Force window mode hack");
    bFullScreen = false;
#endif
    VGA_FullScreenSet_490160(bFullScreen);

    // VGA_DisplaySet_490230 resets the window style - puts it back to something sane.
    VGA_DisplaySet_490230(640u, 480u, 16, 1, 0);

}

} // namespace AO
