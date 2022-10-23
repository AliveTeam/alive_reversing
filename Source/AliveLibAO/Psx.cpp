#include "stdafx_ao.h"
#include "Psx.hpp"
#include "Function.hpp"
#include "PsxRender.hpp"
#include "../relive_lib/Error.hpp"
#include "../AliveLibAE/Psx.hpp"
#include "../AliveLibAE/Sys.hpp"
#include "../AliveLibAE/VGA.hpp"

namespace AO {

void PSX_PutDispEnv_495D30(PSX_DISPENV* pDispEnv)
{
    PSX_PutDispEnv_4F58E0(pDispEnv);
}

void PSX_VSync_496620(s32 mode)
{
    PSX_VSync_4F6170(mode);
}

void PSX_SetDefDrawEnv_495EF0(PSX_DRAWENV* pDrawEnv, s16 x, s16 y, s16 w, s16 h)
{
    PSX_SetDefDrawEnv_4F5AA0(pDrawEnv, x, y, w, h);
}

void PSX_PutDrawEnv_495DD0(const PSX_DRAWENV* pDrawEnv)
{
    PSX_PutDrawEnv_4F5980(pDrawEnv);
}

void PSX_SetDefDispEnv_4959D0(PSX_DISPENV* pOutEnv, s16 x, s16 y, s16 w, s16 h)
{
    PSX_SetDefDispEnv_4F55A0(pOutEnv, x, y, w, h);
}

void PSX_PutDispEnv_495CE0(PSX_DISPENV* pDispEnv)
{
    PSX_PutDispEnv_4F5890(pDispEnv);
}

void PSX_Prevent_Rendering_44FFB0()
{
    PSX_Prevent_Rendering_4945B0();
}

void PSX_EMU_SetCallBack_499920(s32 callBackType, TPsxEmuCallBack fnPtr)
{
    PSX_EMU_SetCallBack_4F9430(callBackType, fnPtr);
}

} // namespace AO
