#include "stdafx_ao.h"
#include "Psx.hpp"
#include "Function.hpp"
#include "bmp.hpp"
#include "VGA.hpp"
#include "Sys.hpp"
#include "PsxRender.hpp"
#include "Error.hpp"
#include "../AliveLibAE/Psx.hpp"

namespace AO {

EXPORT void CC PSX_PutDispEnv_495D30(PSX_DISPENV* pDispEnv)
{
    AE_IMPLEMENTED();
    PSX_PutDispEnv_4F58E0(pDispEnv);
}

EXPORT int CC PSX_DrawSync_496750(int mode)
{
    AE_IMPLEMENTED();
    return PSX_DrawSync_4F6280(mode);
}

EXPORT int CC PSX_VSync_496620(int mode)
{
    AE_IMPLEMENTED();
    return PSX_VSync_4F6170(mode);
}

EXPORT void CC PSX_SetDefDrawEnv_495EF0(PSX_DRAWENV* pDrawEnv, __int16 x, __int16 y, __int16 w, __int16 h)
{
    AE_IMPLEMENTED();
    PSX_SetDefDrawEnv_4F5AA0(pDrawEnv, x, y, w, h);
}

EXPORT void CC PSX_PutDrawEnv_495DD0(const PSX_DRAWENV* pDrawEnv)
{
    AE_IMPLEMENTED();
    PSX_PutDrawEnv_4F5980(pDrawEnv);
}

EXPORT void CC PSX_SetDefDispEnv_4959D0(PSX_DISPENV* pOutEnv, __int16 x, __int16 y, __int16 w, __int16 h)
{
    AE_IMPLEMENTED();
    PSX_SetDefDispEnv_4F55A0(pOutEnv, x, y, w, h);
}

EXPORT void CC PSX_PutDispEnv_495CE0(PSX_DISPENV* pDispEnv)
{
    AE_IMPLEMENTED();
    PSX_PutDispEnv_4F5890(pDispEnv);
}

EXPORT int CC PSX_LoadImage16_4962A0(const PSX_RECT* pRect, const BYTE* pData)
{
    AE_IMPLEMENTED();
    return PSX_LoadImage16_4F5E20(pRect, pData);
}

EXPORT int PSX_LoadImage_496480(const PSX_RECT* pRect, const BYTE* pData)
{
    AE_IMPLEMENTED();
    return PSX_LoadImage_4F5FB0(pRect, pData);
}

EXPORT signed int CC PSX_StoreImage_496320(const PSX_RECT* rect, WORD* pData)
{
    AE_IMPLEMENTED();
    return PSX_StoreImage_4F5E90(rect, pData);
}

EXPORT CdlLOC* CC PSX_Pos_To_CdLoc_49B340(int pos, CdlLOC* pLoc)
{
    AE_IMPLEMENTED();
    return PSX_Pos_To_CdLoc_4FADD0(pos, pLoc);
}

EXPORT int CC PSX_CD_File_Seek_49B670(char mode, CdlLOC* pLoc)
{
    AE_IMPLEMENTED();
    return PSX_CD_File_Seek_4FB1E0(mode, pLoc);
}

EXPORT int CC PSX_CD_File_Read_49B8B0(int numSectors, void* pBuffer)
{
    AE_IMPLEMENTED();
    return PSX_CD_File_Read_4FB210(numSectors, pBuffer);
}

EXPORT int CC PSX_CD_FileIOWait_49B900(int bASync)
{
    AE_IMPLEMENTED();
    return PSX_CD_FileIOWait_4FB260(bASync);
}

EXPORT int CC PSX_ResetGraph_4987E0(int mode)
{
    AE_IMPLEMENTED();
    return PSX_ResetGraph_4F8800(mode);
}

EXPORT int CC PSX_StopCallBack_49AFC0()
{
    AE_IMPLEMENTED();
    return PSX_StopCallBack_4FAA30();
}

EXPORT int CC PSX_CdControlB_49BB40(int a1, int a2, int a3)
{
    AE_IMPLEMENTED();
    return PSX_CdControlB_4FB320(a1, a2, a3);
}

EXPORT void CC PSX_SetDispMask_4989D0(int mode)
{
    AE_IMPLEMENTED();
    PSX_SetDispMask_4F89F0(mode);
}

EXPORT void CC PSX_SetGraphDebug_4989F0(int mode)
{
    AE_IMPLEMENTED();
    PSX_SetGraphDebug_4F8A10(mode);
}

EXPORT int CC PSX_SetDispMask_49AE80(int mode)
{
    AE_IMPLEMENTED();
    return PSX_SetDispMask_4F89F0(mode);
}

EXPORT void CC PSX_Prevent_Rendering_44FFB0()
{
    AE_IMPLEMENTED();
    PSX_Prevent_Rendering_4945B0();
}

EXPORT int PSX_ResetCallBack_49AFB0()
{
    AE_IMPLEMENTED();
    return PSX_ResetCallBack_4FAA20();
}

EXPORT int CC PSX_MoveImage_4961A0(const PSX_RECT* pRect, int xpos, int ypos)
{
    AE_IMPLEMENTED();
    return PSX_MoveImage_4F5D50(pRect, xpos, ypos);
}

EXPORT signed int CC PSX_EMU_Set_Cd_Emulation_Paths_49B000(const char* pPath1, const char* pPath2, const char* pPath3)
{
    AE_IMPLEMENTED();
    return PSX_EMU_Set_Cd_Emulation_Paths_4FAA70(pPath1, pPath2, pPath3);
}

EXPORT void CC PSX_EMU_Init_49A1D0(bool bShowVRam)
{
    AE_IMPLEMENTED();
    PSX_EMU_Init_4F9CD0(bShowVRam);
}

EXPORT signed int PSX_EMU_VideoAlloc_49A2B0()
{
    AE_IMPLEMENTED();
    return PSX_EMU_VideoAlloc_4F9D70();
}

EXPORT void CC PSX_EMU_SetCallBack_499920(int callBackType, TPsxEmuCallBack fnPtr)
{
    AE_IMPLEMENTED();
    PSX_EMU_SetCallBack_4F9430(callBackType, fnPtr);
}

EXPORT void CC PSX_EMU_Set_screen_mode_499910(char a1)
{
    AE_IMPLEMENTED();
    PSX_EMU_Set_screen_mode_4F9420(a1);
}

EXPORT void CC PSX_DispEnv_Set_48D900(int mode)
{
    AE_IMPLEMENTED();
    PSX_DispEnv_Set_4ED960(mode);
}

EXPORT void PSX_EMU_VideoDeAlloc_49A550()
{
    AE_IMPLEMENTED();
    PSX_EMU_VideoDeAlloc_4FA010();
}

EXPORT CdlFILE* CC PSX_CdSearchFile_49B930(CdlFILE*, const char*)
{
    // Shouldn't need to be implemented if call sites changed to use file names like AE
    NOT_IMPLEMENTED();
    return nullptr;
}

EXPORT int CC PSX_CdLoc_To_Pos_49B3B0(const CdlLOC* pLoc)
{
    AE_IMPLEMENTED();
    return PSX_CdLoc_To_Pos_4FAE40(pLoc);
}


void Init_VGA_AndPsxVram()
{
    bool bFullScreen = true;
#ifdef BEHAVIOUR_CHANGE_FORCE_WINDOW_MODE
    LOG_INFO("Force window mode hack");
    bFullScreen = false;
#endif
    VGA_FullScreenSet_490160(bFullScreen);

#ifdef _WIN32
#ifdef BEHAVIOUR_CHANGE_FORCE_WINDOW_MODE
    const LONG oldWinStyle = GetWindowLongA((HWND)Sys_GetWindowHandle_48E930(), GWL_STYLE) | WS_OVERLAPPEDWINDOW;
#endif
#endif

    // VGA_DisplaySet_490230 resets the window style - puts it back to something sane.
    VGA_DisplaySet_490230(640u, 480u, 16, 1, 0);

#ifdef _WIN32
#ifdef BEHAVIOUR_CHANGE_FORCE_WINDOW_MODE
    SetWindowLongA((HWND)Sys_GetWindowHandle_48E930(), GWL_STYLE, oldWinStyle);
#endif
#endif

    RECT rect = {};
    rect.left = 0;
    rect.top = 0;
    rect.right = 640;
    rect.bottom = 480;
    BMP_ClearRect_48F810(&sVGA_bmp_primary_A8A4C0, &rect, 0);
    switch (VGA_GetPixelFormat_490E60())
    {
    case 8:
        PSX_EMU_SetDispType_499E60(1);
        break;
    case 15:
        PSX_EMU_SetDispType_499E60(4);
        break;
    case 16:
        PSX_EMU_SetDispType_499E60(2);
        break;
    case 115:
        PSX_EMU_SetDispType_499E60(5);
        break;
    case 116:
        PSX_EMU_SetDispType_499E60(3);
        break;
    default:
        Error_WarningMessageBox_48E470("This program requires a high-color display mode of 32768 or 65536 colors at 640x480 resolution.");
        Error_ShowErrorStackToUser_48DF10(0);
        exit(0);
        return;
    }
}

int CC PSX_CD_OpenFile(const char* pFileName, int bTryAllPaths)
{
    // TODO: Need to return 0 if not using AE funcs to avoid any side effects
    return PSX_CD_OpenFile_4FAE80(pFileName, bTryAllPaths);
}

}
