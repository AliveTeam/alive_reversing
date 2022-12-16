#pragma once

#include "../relive_lib/Primitives_common.hpp"

void PSX_DrawOTag(PrimHeader** ppOt);
void PSX_ClearOTag(PrimHeader** otBuffer, s32 otBufferSize);

extern s32 gScreenXOffset;
extern s32 gScreenYOffset;
