#pragma once

#include "../AliveLibCommon/Primitives_common.hpp"

void PSX_DrawOTag(PrimHeader** ppOt);
void PSX_ClearOTag(PrimHeader** otBuffer, s32 otBufferSize);

extern s32 sScreenXOffSet_BD30E4;
extern s32 sScreenYOffset_BD30A4;
