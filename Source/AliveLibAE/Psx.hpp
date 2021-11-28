#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "bmp.hpp"
#include "FixedPoint.hpp"
#include "../AliveLibCommon/Psx_common.hpp"
#include "../AliveLibCommon/AddPointer.hpp"

void Psx_ForceLink();

using TPsxEmuCallBack = AddPointer_t<s32(u32)>;

EXPORT void CC PSX_DispEnv_Set_4ED960(s32 mode);
EXPORT s32 CC PSX_VSync_4F6170(s32 mode);
EXPORT s32 CC PSX_DrawSync_4F6280(s32 mode);
EXPORT bool CC PSX_Rect_IsInFrameBuffer_4FA050(const PSX_RECT* pRect);
EXPORT s32 CC PSX_SetDispMask_4F89F0(s32 mode);
EXPORT s32 CC PSX_SetVideoMode_4FA8F0();
EXPORT s32 CC PSX_ResetGraph_4F8800(s32);
EXPORT s32 CC PSX_SetGraphDebug_4F8A10(s32);
EXPORT void CC PSX_SetDefDrawEnv_4F5AA0(PSX_DRAWENV* pDrawEnv, s16 x, s16 y, s16 w, s16 h);
EXPORT void CC PSX_SetDefDispEnv_4F55A0(PSX_DISPENV* pOutEnv, s16 x, s16 y, s16 w, s16 h);
EXPORT void CC PSX_PutDrawEnv_4F5980(const PSX_DRAWENV* pDrawEnv);
EXPORT void CC PSX_PutDispEnv_4F5890(PSX_DISPENV* pDispEnv);
EXPORT void CC PSX_PutDispEnv_4F58E0(const PSX_DISPENV* pDispEnv);
EXPORT void CC PSX_EMU_VideoDeAlloc_4FA010();
EXPORT s32 CC PSX_ResetCallBack_4FAA20();
EXPORT s32 CC PSX_CdInit_4FB2C0();
EXPORT s32 CC PSX_CdSetDebug_4FB330(s32 mode);
EXPORT s32 CC PSX_CdControlB_4FB320(s32, s32, s32);
EXPORT s32 CC PSX_StopCallBack_4FAA30();
EXPORT s32 CC PSX_EMU_VideoAlloc_4F9D70();
EXPORT void CC Init_VGA_AndPsxVram_494690();
EXPORT void CC PSX_EMU_Init_4F9CD0(bool bShowVRam);
EXPORT void CC PSX_EMU_SetCallBack_4F9430(s32 callBackType, TPsxEmuCallBack fnPtr);
EXPORT s32 CC PSX_EMU_Set_Cd_Emulation_Paths_4FAA70(const char_type* pPath1, const char_type* pPath2, const char_type* pPath3);
EXPORT void CC PSX_EMU_Set_screen_mode_4F9420(s8 mode);
EXPORT s32 CC PSX_MoveImage_4F5D50(const PSX_RECT* pRect, s32 xpos, s32 ypos);
EXPORT s32 CC PSX_LoadImage16_4F5E20(const PSX_RECT* pRect, const u8* pData);
EXPORT s32 CC PSX_LoadImage_4F5FB0(const PSX_RECT* pRect, const u8* pData);
EXPORT s32 CC PSX_StoreImage_4F5E90(const PSX_RECT* rect, u16* pData);

BOOL CC PSX_Rects_overlap_no_adjustment(const PSX_RECT* pRect1, const PSX_RECT* pRect2);

EXPORT BOOL CC PSX_Rects_overlap_4FA0B0(const PSX_RECT* pRect1, const PSX_RECT* pRect2);
EXPORT void CC PSX_SetDrawEnv_Impl_4FE420(s32 x, s32 y, s32 w, s32 h, s32 unknown, u8* pBuffer);

EXPORT void CC PSX_CD_Normalize_FileName_4FAD90(char_type* pNormalized, const char_type* pFileName);
EXPORT s32 CC PSX_CD_OpenFile_4FAE80(const char_type* pFileName, s32 bTryAllPaths);

EXPORT void CC PSX_Prevent_Rendering_4945B0();


EXPORT CdlLOC* CC PSX_Pos_To_CdLoc_4FADD0(s32 pos, CdlLOC* pLoc);
EXPORT s32 CC PSX_CdLoc_To_Pos_4FAE40(const CdlLOC* pLoc);
EXPORT s32 CC PSX_CD_File_Seek_4FB1E0(s8 mode, const CdlLOC* pLoc);
EXPORT s32 CC PSX_CD_File_Read_4FB210(s32 numSectors, void* pBuffer);
EXPORT s32 CC PSX_CD_FileIOWait_4FB260(s32 bASync);

EXPORT Bitmap& GetPsxVram();

ALIVE_VAR_EXTERN(Bitmap, sPsxVram_C1D160);
ALIVE_VAR_EXTERN(u8, turn_off_rendering_BD0F20);
ALIVE_VAR_EXTERN(PSX_DRAWENV, sPSX_EMU_DrawEnvState_C3D080);
ALIVE_VAR_EXTERN(u8, sPsxEMU_show_vram_BD1465);
ALIVE_VAR_EXTERN(Bitmap*, spBitmap_C2D038);

ALIVE_VAR_EXTERN(TPsxEmuCallBack, sPsxEmu_EndFrameFnPtr_C1D17C);
ALIVE_VAR_EXTERN(u8, bDontUseXYOffsetInRender_BD1464);
ALIVE_VAR_EXTERN(Bitmap, sBitmap_C1D1A0);
ALIVE_VAR_EXTERN(s32, sVGA_DisplayType_BD1468);

ALIVE_ARY_EXTERN(char_type, 128, sCdEmu_Path1_C14620);
ALIVE_ARY_EXTERN(char_type, 128, sCdEmu_Path2_C144C0);
ALIVE_ARY_EXTERN(char_type, 128, sCdEmu_Path3_C145A0);

ALIVE_VAR_EXTERN(s32, sPsx_drawenv_clipx_BDCD40);
ALIVE_VAR_EXTERN(s32, sPsx_drawenv_clipy_BDCD44);
ALIVE_VAR_EXTERN(s32, sPsx_drawenv_clipw_BDCD48);
ALIVE_VAR_EXTERN(s32, sPsx_drawenv_cliph_BDCD4C);


namespace AETest::TestsPsx {
void PsxTests();
}
