#pragma once

struct PSX_DISPENV;
struct PSX_RECT;

using TPsxEmuCallBack = AddPointer_t<s32(u32)>;

void PSX_VSync_4F6170(s32 mode);
void PSX_SetDefDispEnv_4F55A0(PSX_DISPENV* pOutEnv);
void PSX_PutDispEnv_4F5890();
void PSX_PutDispEnv_4F58E0();
void PSX_EMU_SetCallBack_4F9430(TPsxEmuCallBack fnPtr);

bool PSX_Rects_overlap_no_adjustment(const PSX_RECT* pRect1, const PSX_RECT* pRect2);

bool PSX_Rects_overlap_4FA0B0(const PSX_RECT* pRect1, const PSX_RECT* pRect2);

void PSX_Prevent_Rendering_4945B0();

extern u8 turn_off_rendering_BD0F20;
