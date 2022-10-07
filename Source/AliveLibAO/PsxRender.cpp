#include "stdafx_ao.h"
#include "PsxRender.hpp"
#include "../AliveLibAE/PsxRender.hpp"

namespace AO {

void PSX_ClearOTag_496760(PrimHeader** otBuffer, s32 otBufferSize)
{
    PSX_ClearOTag_4F6290(otBuffer, otBufferSize);
}

void PSX_DrawOTag_4969F0(PrimHeader** ppOt)
{
    PSX_DrawOTag_4F6540(ppOt);
}

s32 PSX_EMU_SetDispType_499E60(s32 dispType)
{
    return PSX_EMU_SetDispType_4F9960(dispType);
}

s32 PSX_ClearImage_496020(const PSX_RECT* pRect, u8 r, u8 g, u8 b)
{
    return PSX_ClearImage_4F5BD0(reinterpret_cast<const ::PSX_RECT*>(pRect), r, g, b);
}

} // namespace AO
