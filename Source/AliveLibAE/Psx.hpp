#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "FixedPoint.hpp"
#include "../AliveLibCommon/Psx_common.hpp"
#include "../AliveLibCommon/AddPointer.hpp"


using TPsxEmuCallBack = AddPointer_t<s32(u32)>;

void PSX_VSync_4F6170(s32 mode);
void PSX_SetDefDrawEnv_4F5AA0(PSX_DRAWENV* pDrawEnv, s16 x, s16 y, s16 w, s16 h);
void PSX_SetDefDispEnv_4F55A0(PSX_DISPENV* pOutEnv, s16 x, s16 y, s16 w, s16 h);
void PSX_PutDrawEnv_4F5980(const PSX_DRAWENV* pDrawEnv);
void PSX_PutDispEnv_4F5890(PSX_DISPENV* pDispEnv);
void PSX_PutDispEnv_4F58E0(const PSX_DISPENV* pDispEnv);
void PSX_EMU_SetCallBack_4F9430(s32 callBackType, TPsxEmuCallBack fnPtr);

bool PSX_Rects_overlap_no_adjustment(const PSX_RECT* pRect1, const PSX_RECT* pRect2);

bool PSX_Rects_overlap_4FA0B0(const PSX_RECT* pRect1, const PSX_RECT* pRect2);
void PSX_SetDrawEnv_Impl_4FE420(s32 x, s32 y, s32 w, s32 h, s32 unknown, u8* pBuffer);

void PSX_Prevent_Rendering_4945B0();


extern u8 turn_off_rendering_BD0F20;
extern PSX_DRAWENV sPSX_EMU_DrawEnvState_C3D080;

extern TPsxEmuCallBack sPsxEmu_EndFrameFnPtr_C1D17C;

extern s32 sPsx_drawenv_clipx_BDCD40;
extern s32 sPsx_drawenv_clipy_BDCD44;
extern s32 sPsx_drawenv_clipw_BDCD48;
extern s32 sPsx_drawenv_cliph_BDCD4C;
