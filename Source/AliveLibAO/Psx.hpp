#pragma once

#include "FunctionFwd.hpp"
#include "Psx_common.hpp"
#include "AddPointer.hpp"

namespace AO {

using TPsxEmuCallBack = AddPointer_t<s32(u32)>;

EXPORT void CC PSX_PutDispEnv_495D30(PSX_DISPENV* pDispEnv);

EXPORT s32 CC PSX_DrawSync_496750(s32 mode);

EXPORT s32 CC PSX_VSync_496620(s32 mode);

EXPORT void CC PSX_SetDefDrawEnv_495EF0(PSX_DRAWENV* pDrawEnv, s16 x, s16 y, s16 w, s16 h);

EXPORT void CC PSX_PutDrawEnv_495DD0(const PSX_DRAWENV* pDrawEnv);

EXPORT void CC PSX_SetDefDispEnv_4959D0(PSX_DISPENV* pOutEnv, s16 x, s16 y, s16 w, s16 h);

EXPORT void CC PSX_PutDispEnv_495CE0(PSX_DISPENV* pDispEnv);

EXPORT s32 CC PSX_LoadImage16_4962A0(const PSX_RECT* pRect, const u8* pData);

EXPORT s32 PSX_LoadImage_496480(const PSX_RECT* pRect, const u8* pData);

EXPORT s32 CC PSX_StoreImage_496320(const PSX_RECT* rect, u16* pData);

EXPORT CdlLOC* CC PSX_Pos_To_CdLoc_49B340(s32 pos, CdlLOC* pLoc);

EXPORT s32 CC PSX_CD_File_Seek_49B670(s8 mode, CdlLOC* pLoc);

EXPORT s32 CC PSX_CD_File_Read_49B8B0(s32 numSectors, void* pBuffer);

EXPORT s32 CC PSX_CD_FileIOWait_49B900(s32 bASync);

EXPORT s32 CC PSX_ResetGraph_4987E0(s32 mode);

EXPORT s32 CC PSX_StopCallBack_49AFC0();

EXPORT s32 CC PSX_CdControlB_49BB40(s32, s32, s32);

EXPORT void CC PSX_SetDispMask_4989D0(s32);

EXPORT void CC PSX_SetGraphDebug_4989F0(s32);

EXPORT s32 CC PSX_SetDispMask_49AE80(s32 a1);

EXPORT void CC PSX_Prevent_Rendering_44FFB0();

EXPORT s32 PSX_ResetCallBack_49AFB0();

EXPORT s32 CC PSX_MoveImage_4961A0(const PSX_RECT* pRect, s32 xpos, s32 ypos);

EXPORT s32 CC PSX_EMU_Set_Cd_Emulation_Paths_49B000(const s8* pPath1, const s8* pPath2, const s8* pPath3);

EXPORT void CC PSX_EMU_Init_49A1D0(bool bShowVRam);

EXPORT s32 PSX_EMU_VideoAlloc_49A2B0();

EXPORT void CC PSX_EMU_SetCallBack_499920(s32 callBackType, TPsxEmuCallBack fnPtr);

EXPORT void CC PSX_EMU_Set_screen_mode_499910(s8 a1);

EXPORT void CC PSX_DispEnv_Set_48D900(s32 mode);

EXPORT void PSX_EMU_VideoDeAlloc_49A550();

EXPORT CdlFILE* CC PSX_CdSearchFile_49B930(CdlFILE*, const s8*);

EXPORT s32 CC PSX_CdLoc_To_Pos_49B3B0(const CdlLOC* pLoc);

void Init_VGA_AndPsxVram();


s32 CC PSX_CD_OpenFile(const s8* pFileName, s32 bTryAllPaths);

} // namespace AO
