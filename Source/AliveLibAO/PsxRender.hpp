#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "Psx_common.hpp"

struct PrimHeader;

namespace AO {


EXPORT void CC PSX_ClearOTag_496760(PrimHeader** otBuffer, s32 otBufferSize);

EXPORT void CC PSX_DrawOTag_4969F0(PrimHeader** ppOt);

EXPORT s32 CC PSX_EMU_SetDispType_499E60(s32 dispType);

EXPORT s32 CC PSX_ClearImage_496020(const PSX_RECT* pRect, u8 r, u8 g, u8 b);

} // namespace AO
