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

} // namespace AO
