#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Psx_common.hpp"

struct PrimHeader;

namespace AO {


void PSX_ClearOTag_496760(PrimHeader** otBuffer, s32 otBufferSize);

void PSX_DrawOTag_4969F0(PrimHeader** ppOt);


} // namespace AO
