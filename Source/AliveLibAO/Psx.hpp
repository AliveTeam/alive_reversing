#pragma once

#include "FunctionFwd.hpp"
#include "Psx_common.hpp"

EXPORT void CC PSX_PutDispEnv_495D30(PSX_DISPENV* pDispEnv);

EXPORT int CC PSX_DrawSync_496750(int /*mode*/);

EXPORT int CC PSX_VSync_496620(int);

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

EXPORT int CC PSX_CD_FileIOWait_49B900(int);

EXPORT int CC PSX_ClearImage_496020(const PSX_RECT* pRect, __int16 r, unsigned __int8 g, unsigned __int8 b);

EXPORT int CC PSX_ResetGraph_4987E0(int mode);

EXPORT int CC PSX_StopCallBack_49AFC0();

EXPORT int CC PSX_CdControlB_49BB40(int, int, int);

EXPORT void CC PSX_SetDispMask_4989D0(int);

EXPORT void CC PSX_SetGraphDebug_4989F0(int);

EXPORT int CC PSX_SetDispMask_49AE80(int a1);

EXPORT void CC PSX_Prevent_Rendering_44FFB0();

EXPORT int PSX_ResetCallBack_49AFB0();

EXPORT int CC PSX_MoveImage_4961A0(const PSX_RECT* pRect, int xpos, int ypos);