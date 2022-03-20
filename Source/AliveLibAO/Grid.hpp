#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "../AliveLibCommon/FixedPoint_common.hpp"

namespace AO {

s32 CC XGrid_Index_To_XPos_41FA60(FP scale, s32 xGridIndex);

FP CC ScaleToGridSize_41FA30(FP scale);

EXPORT s32 CC SnapToXGrid_41FAA0(FP scale, s32 a2);


EXPORT FP CC CamX_VoidSkipper_418590(FP xpos, FP xvel, s16 xMargin, u16* pResult);
EXPORT FP CC CamY_VoidSkipper_418690(FP ypos, FP yvel, s16 yMargin, u16* pResult);


} // namespace AO