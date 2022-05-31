#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../AliveLibCommon/Psx_common.hpp"
#include "../AliveLibCommon/AddPointer.hpp"

namespace AO {

using TPsxEmuCallBack = AddPointer_t<s32(u32)>;

void PSX_PutDispEnv_495D30(PSX_DISPENV* pDispEnv);

s32 PSX_DrawSync_496750(s32 mode);

s32 PSX_VSync_496620(s32 mode);

void PSX_SetDefDrawEnv_495EF0(PSX_DRAWENV* pDrawEnv, s16 x, s16 y, s16 w, s16 h);

void PSX_PutDrawEnv_495DD0(const PSX_DRAWENV* pDrawEnv);

void PSX_SetDefDispEnv_4959D0(PSX_DISPENV* pOutEnv, s16 x, s16 y, s16 w, s16 h);

void PSX_PutDispEnv_495CE0(PSX_DISPENV* pDispEnv);

s32 PSX_LoadImage16_4962A0(const PSX_RECT* pRect, const u8* pData);

s32 PSX_LoadImage_496480(const PSX_RECT* pRect, const u8* pData);

s32 PSX_StoreImage_496320(const PSX_RECT* rect, u16* pData);

CdlLOC* PSX_Pos_To_CdLoc_49B340(s32 pos, CdlLOC* pLoc);

s32 PSX_CD_File_Seek_49B670(s8 mode, CdlLOC* pLoc);

s32 PSX_CD_File_Read_49B8B0(s32 numSectors, void* pBuffer);

s32 PSX_CD_FileIOWait_49B900(s32 bASync);

s32 PSX_ResetGraph_4987E0(s32 mode);

s32 PSX_StopCallBack_49AFC0();

s32 PSX_CdControlB_49BB40(s32, s32, s32);

void PSX_SetDispMask_4989D0(s32);

void PSX_SetGraphDebug_4989F0(s32);

s32 PSX_SetDispMask_49AE80(s32 a1);

void PSX_Prevent_Rendering_44FFB0();

s32 PSX_ResetCallBack_49AFB0();

s32 PSX_MoveImage_4961A0(const PSX_RECT* pRect, s32 xpos, s32 ypos);

s32 PSX_EMU_Set_Cd_Emulation_Paths_49B000(const char_type* pPath1, const char_type* pPath2, const char_type* pPath3);

void PSX_EMU_Init_49A1D0(bool bShowVRam);

s32 PSX_EMU_VideoAlloc_49A2B0();

void PSX_EMU_SetCallBack_499920(s32 callBackType, TPsxEmuCallBack fnPtr);

void PSX_EMU_Set_screen_mode_499910(s8 a1);

void PSX_DispEnv_Set_48D900(s32 mode);

void PSX_EMU_VideoDeAlloc_49A550();

CdlFILE* PSX_CdSearchFile_49B930(CdlFILE*, const char_type*);

s32 PSX_CdLoc_To_Pos_49B3B0(const CdlLOC* pLoc);

void Init_VGA_AndPsxVram();


s32 PSX_CD_OpenFile(const char_type* pFileName, s32 bTryAllPaths);

} // namespace AO
