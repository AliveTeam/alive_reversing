#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Psx_common.hpp"

struct PrimHeader;

namespace AO {


void PSX_ClearOTag_496760(PrimHeader** otBuffer, s32 otBufferSize);

void PSX_DrawOTag_4969F0(PrimHeader** ppOt);

s32 PSX_EMU_SetDispType_499E60(s32 dispType);

s32 PSX_ClearImage_496020(const PSX_RECT* pRect, u8 r, u8 g, u8 b);

} // namespace AO
