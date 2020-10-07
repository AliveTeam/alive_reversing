#include "stdafx_ao.h"
#include "Psx.hpp"
#include "Function.hpp"


EXPORT void CC PSX_PutDispEnv_495D30(PSX_DISPENV* /*pDispEnv*/)
{
    NOT_IMPLEMENTED();
}

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

EXPORT int CC PSX_LoadImage16_4962A0(const PSX_RECT* /*pRect*/, const BYTE* /*pData*/)
{
    NOT_IMPLEMENTED();
    return 0;
}


EXPORT int PSX_LoadImage_496480(const PSX_RECT* /*pRect*/, const BYTE* /*pData*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT signed int CC PSX_StoreImage_496320(const PSX_RECT* /*rect*/, WORD* /*pData*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT CdlLOC* CC PSX_Pos_To_CdLoc_49B340(int /*pos*/, CdlLOC* /*pLoc*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

EXPORT int CC PSX_CD_File_Seek_49B670(char /*mode*/, CdlLOC* /*pLoc*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int CC PSX_CD_File_Read_49B8B0(int /*numSectors*/, void* /*pBuffer*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int CC PSX_CD_FileIOWait_49B900(int)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int CC PSX_ClearImage_496020(const PSX_RECT* /*pRect*/, __int16 /*r*/, unsigned __int8 /*g*/, unsigned __int8 /*b*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int CC PSX_ResetGraph_4987E0(int /*mode*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int CC PSX_StopCallBack_49AFC0()
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int CC PSX_CdControlB_49BB40(int, int, int)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT void CC PSX_SetDispMask_4989D0(int)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC PSX_SetGraphDebug_4989F0(int)
{
    NOT_IMPLEMENTED();
}

EXPORT int CC PSX_SetDispMask_49AE80(int)
{
    NOT_IMPLEMENTED();
    return 0;
}

ALIVE_VAR(1, 0xA08628, unsigned __int8, byte_A08628, 0);

EXPORT void CC PSX_Prevent_Rendering_44FFB0()
{
    byte_A08628 = 1;
}
