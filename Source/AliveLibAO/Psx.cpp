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

s32 PSX_DrawSync_496750(s32 mode)
{
    return PSX_DrawSync_4F6280(mode);
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

s32 PSX_LoadImage_496480(const PSX_RECT* pRect, const u8* pData)
{
    return PSX_LoadImage_4F5FB0(pRect, pData);
}

CdlLOC* PSX_Pos_To_CdLoc_49B340(s32 pos, CdlLOC* pLoc)
{
    return PSX_Pos_To_CdLoc_4FADD0(pos, pLoc);
}

s32 PSX_CD_File_Seek_49B670(s8 mode, CdlLOC* pLoc)
{
    return PSX_CD_File_Seek_4FB1E0(mode, pLoc);
}

s32 PSX_CD_File_Read_49B8B0(s32 numSectors, void* pBuffer)
{
    return PSX_CD_File_Read_4FB210(numSectors, pBuffer);
}

s32 PSX_CD_FileIOWait_49B900(s32 bASync)
{
    return PSX_CD_FileIOWait_4FB260(bASync);
}

s32 PSX_ResetGraph_4987E0(s32 mode)
{
    return PSX_ResetGraph_4F8800(mode);
}

s32 PSX_StopCallBack_49AFC0()
{
    return PSX_StopCallBack_4FAA30();
}

s32 PSX_CdControlB_49BB40(s32 a1, s32 a2, s32 a3)
{
    return PSX_CdControlB_4FB320(a1, a2, a3);
}

void PSX_SetDispMask_4989D0(s32 mode)
{
    PSX_SetDispMask_4F89F0(mode);
}

void PSX_SetGraphDebug_4989F0(s32 mode)
{
    PSX_SetGraphDebug_4F8A10(mode);
}

s32 PSX_SetDispMask_49AE80(s32 mode)
{
    return PSX_SetDispMask_4F89F0(mode);
}

void PSX_Prevent_Rendering_44FFB0()
{
    PSX_Prevent_Rendering_4945B0();
}

s32 PSX_ResetCallBack_49AFB0()
{
    return PSX_ResetCallBack_4FAA20();
}

s32 PSX_MoveImage(const PSX_RECT* pRect, s32 xpos, s32 ypos)
{
    return ::PSX_MoveImage(pRect, xpos, ypos);
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

void PSX_EMU_Set_screen_mode_499910(s8 a1)
{
    PSX_EMU_Set_screen_mode_4F9420(a1);
}

void PSX_DispEnv_Set_48D900(s32 mode)
{
    PSX_DispEnv_Set_4ED960(mode);
}

void PSX_EMU_VideoDeAlloc_49A550()
{
    PSX_EMU_VideoDeAlloc_4FA010();
}

CdlFILE* PSX_CdSearchFile_49B930(CdlFILE*, const char_type*)
{
    // Shouldn't need to be implemented if call sites changed to use file names like AE
    
    return nullptr;
}

s32 PSX_CdLoc_To_Pos_49B3B0(const CdlLOC* pLoc)
{
    return PSX_CdLoc_To_Pos_4FAE40(pLoc);
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
