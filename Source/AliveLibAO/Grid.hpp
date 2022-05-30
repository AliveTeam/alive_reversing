#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "../AliveLibCommon/FixedPoint_common.hpp"

namespace AO {

s32 XGrid_Index_To_XPos(FP scale, s32 xGridIndex);

FP ScaleToGridSize(FP scale);

s32 SnapToXGrid(FP scale, s32 a2);


FP CamX_VoidSkipper(FP xpos, FP xvel, s16 xMargin, u16* pResult);
FP CamY_VoidSkipper(FP ypos, FP yvel, s16 yMargin, u16* pResult);


} // namespace AO