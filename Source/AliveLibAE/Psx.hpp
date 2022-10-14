#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "FixedPoint.hpp"
#include "../AliveLibCommon/Psx_common.hpp"
#include "../AliveLibCommon/AddPointer.hpp"

struct Bitmap;

using TPsxEmuCallBack = AddPointer_t<s32(u32)>;

void PSX_VSync_4F6170(s32 mode);
s32 PSX_SetDispMask_4F89F0(s32 mode);
s32 PSX_SetVideoMode_4FA8F0();
s32 PSX_ResetGraph_4F8800(s32);
s32 PSX_SetGraphDebug_4F8A10(s32);
void PSX_SetDefDrawEnv_4F5AA0(PSX_DRAWENV* pDrawEnv, s16 x, s16 y, s16 w, s16 h);
void PSX_SetDefDispEnv_4F55A0(PSX_DISPENV* pOutEnv, s16 x, s16 y, s16 w, s16 h);
void PSX_PutDrawEnv_4F5980(const PSX_DRAWENV* pDrawEnv);
void PSX_PutDispEnv_4F5890(PSX_DISPENV* pDispEnv);
void PSX_PutDispEnv_4F58E0(const PSX_DISPENV* pDispEnv);
void PSX_EMU_VideoDeAlloc_4FA010();
s32 PSX_ResetCallBack_4FAA20();
s32 PSX_StopCallBack_4FAA30();
s32 PSX_EMU_VideoAlloc_4F9D70();
void Init_VGA_AndPsxVram_494690();
void PSX_EMU_Init_4F9CD0();
void PSX_EMU_SetCallBack_4F9430(s32 callBackType, TPsxEmuCallBack fnPtr);
s32 PSX_MoveImage(const PSX_RECT* pRect, s32 xpos, s32 ypos);
s32 PSX_LoadImage_4F5FB0(const PSX_RECT* pRect, const u8* pData);

bool PSX_Rects_overlap_no_adjustment(const PSX_RECT* pRect1, const PSX_RECT* pRect2);

bool PSX_Rects_overlap_4FA0B0(const PSX_RECT* pRect1, const PSX_RECT* pRect2);
void PSX_SetDrawEnv_Impl_4FE420(s32 x, s32 y, s32 w, s32 h, s32 unknown, u8* pBuffer);

void PSX_CD_Normalize_FileName_4FAD90(char_type* pNormalized, const char_type* pFileName);

void PSX_Prevent_Rendering_4945B0();


CdlLOC* PSX_Pos_To_CdLoc_4FADD0(s32 pos, CdlLOC* pLoc);
s32 PSX_CdLoc_To_Pos_4FAE40(const CdlLOC* pLoc);
s32 PSX_CD_File_Seek_4FB1E0(s8 mode, const CdlLOC* pLoc);
s32 PSX_CD_File_Read_4FB210(s32 numSectors, void* pBuffer);
s32 PSX_CD_FileIOWait_4FB260(s32 bASync);

extern u8 turn_off_rendering_BD0F20;
extern PSX_DRAWENV sPSX_EMU_DrawEnvState_C3D080;
extern Bitmap* spBitmap_C2D038;

extern TPsxEmuCallBack sPsxEmu_EndFrameFnPtr_C1D17C;
extern Bitmap sBitmap_C1D1A0;
extern s32 sVGA_DisplayType_BD1468;

extern s32 sPsx_drawenv_clipx_BDCD40;
extern s32 sPsx_drawenv_clipy_BDCD44;
extern s32 sPsx_drawenv_clipw_BDCD48;
extern s32 sPsx_drawenv_cliph_BDCD4C;
