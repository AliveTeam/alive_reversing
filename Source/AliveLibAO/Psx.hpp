#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../AliveLibCommon/Psx_common.hpp"
#include "../AliveLibCommon/AddPointer.hpp"

namespace AO {

using TPsxEmuCallBack = AddPointer_t<s32(u32)>;

void PSX_PutDispEnv_495D30(PSX_DISPENV* pDispEnv);

void PSX_VSync_496620(s32 mode);

void PSX_SetDefDrawEnv_495EF0(PSX_DRAWENV* pDrawEnv, s16 x, s16 y, s16 w, s16 h);

void PSX_PutDrawEnv_495DD0(const PSX_DRAWENV* pDrawEnv);

void PSX_SetDefDispEnv_4959D0(PSX_DISPENV* pOutEnv, s16 x, s16 y, s16 w, s16 h);

void PSX_PutDispEnv_495CE0(PSX_DISPENV* pDispEnv);

void PSX_Prevent_Rendering_44FFB0();

void PSX_EMU_SetCallBack_499920(s32 callBackType, TPsxEmuCallBack fnPtr);

} // namespace AO
