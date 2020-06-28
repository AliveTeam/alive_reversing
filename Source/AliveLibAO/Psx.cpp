#include "stdafx_ao.h"
#include "Psx.hpp"
#include "Function.hpp"

EXPORT int CC PSX_DrawSync_496750(int /*mode*/)
{
    return 0;
}

EXPORT int CC PSX_VSync_496620(int)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT void CC PSX_SetDefDrawEnv_495EF0(PSX_DRAWENV* /*pDrawEnv*/, __int16 /*x*/, __int16 /*y*/, __int16 /*w*/, __int16 /*h*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC PSX_PutDrawEnv_495DD0(const PSX_DRAWENV* /*pDrawEnv*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC PSX_SetDefDispEnv_4959D0(PSX_DISPENV* /*pOutEnv*/, __int16 /*x*/, __int16 /*y*/, __int16 /*w*/, __int16 /*h*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC PSX_PutDispEnv_495CE0(PSX_DISPENV* /*pDispEnv*/)
{
    NOT_IMPLEMENTED();
}

EXPORT int CC PSX_LoadImage16_4962A0(const PSX_RECT* /*pRect*/, BYTE* /*pData*/)
{
    NOT_IMPLEMENTED();
    return 0;
}
