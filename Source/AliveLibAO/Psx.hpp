#pragma once

#include "FunctionFwd.hpp"
#include "Psx_common.hpp"

using TPsxEmuCallBack = std::add_pointer<int(DWORD)>::type;

EXPORT void CC PSX_PutDispEnv_495D30(PSX_DISPENV* pDispEnv);

EXPORT int CC PSX_DrawSync_496750(int mode);

EXPORT int CC PSX_VSync_496620(int mode);

EXPORT void CC PSX_SetDefDrawEnv_495EF0(PSX_DRAWENV* pDrawEnv, __int16 x, __int16 y, __int16 w, __int16 h);

EXPORT void CC PSX_PutDrawEnv_495DD0(const PSX_DRAWENV* pDrawEnv);

EXPORT void CC PSX_SetDefDispEnv_4959D0(PSX_DISPENV* pOutEnv, __int16 x, __int16 y, __int16 w, __int16 h);

EXPORT void CC PSX_PutDispEnv_495CE0(PSX_DISPENV* pDispEnv);

EXPORT int CC PSX_LoadImage16_4962A0(const PSX_RECT* pRect, const BYTE* pData);

EXPORT int PSX_LoadImage_496480(const PSX_RECT* pRect, const BYTE* pData);

EXPORT signed int CC PSX_StoreImage_496320(const PSX_RECT* rect, WORD* pData);

EXPORT CdlLOC* CC PSX_Pos_To_CdLoc_49B340(int pos, CdlLOC* pLoc);

EXPORT int CC PSX_CD_File_Seek_49B670(char mode, CdlLOC* pLoc);

EXPORT int CC PSX_CD_File_Read_49B8B0(int numSectors, void* pBuffer);

EXPORT int CC PSX_CD_FileIOWait_49B900(int bASync);

EXPORT int CC PSX_ResetGraph_4987E0(int mode);

EXPORT int CC PSX_StopCallBack_49AFC0();

EXPORT int CC PSX_CdControlB_49BB40(int, int, int);

EXPORT void CC PSX_SetDispMask_4989D0(int);

EXPORT void CC PSX_SetGraphDebug_4989F0(int);

EXPORT int CC PSX_SetDispMask_49AE80(int a1);

EXPORT void CC PSX_Prevent_Rendering_44FFB0();

EXPORT int PSX_ResetCallBack_49AFB0();

EXPORT int CC PSX_MoveImage_4961A0(const PSX_RECT* pRect, int xpos, int ypos);

EXPORT signed int CC PSX_EMU_Set_Cd_Emulation_Paths_49B000(const char* pPath1, const char* pPath2, const char* pPath3);

EXPORT void CC PSX_EMU_Init_49A1D0(char bShowVRam);

EXPORT signed int PSX_EMU_VideoAlloc_49A2B0();

EXPORT void CC PSX_EMU_SetCallBack_499920(int callBackType, TPsxEmuCallBack fnPtr);

EXPORT void CC PSX_EMU_Set_screen_mode_499910(char a1);

EXPORT void CC PSX_DispEnv_Set_48D900(int mode);

EXPORT void PSX_EMU_VideoDeAlloc_49A550();

EXPORT CdlFILE* CC PSX_CdSearchFile_49B930(CdlFILE*, const char*);

EXPORT int CC PSX_CdLoc_To_Pos_49B3B0(const CdlLOC* pLoc);

int CC PSX_CD_OpenFile(const char* pFileName, int bTryAllPaths);
