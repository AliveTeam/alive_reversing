#include "FixedPoint.hpp"

FP ScaleToGridSize(FP scale);

FP CamX_VoidSkipper(FP xpos, FP xvel, s16 xMargin, u16* pResult);
FP CamY_VoidSkipper(FP ypos, FP yvel, s16 yMargin, u16* pResult);

s32 XGrid_Index_To_XPos_AO(FP scale, s32 xGridIndex);
s32 XGrid_Index_To_XPos_AE(FP scale, s32 xGridIndex);

s32 SnapToXGrid_AO(FP scale, s32 x);
s32 SnapToXGrid_AE(FP scale, s32 x);

s32 SnapToXGrid(FP scale, s32 x, bool isAo);
s32 XGrid_Index_To_XPos(FP scale, s32 xGridIndex, bool isAo);
