#pragma once

#include "../AliveLibCommon/Function.hpp"

struct PSX_RECT;
struct PrimHeader;

namespace AETest::TestsPsxRender {
void PsxRenderTests();
}
union PrimAny;

void DrawOTag_Render_SPRT(PrimAny& any, s16 drawEnv_of0, s16 drawEnv_of1, s16 width, s16 height);
void PSX_RenderLaughingGasEffect_4F7B80(s32 xpos, s32 ypos, s32 width, s32 height, u16* pData);
void DrawOTag_Render_TILE(PrimAny& any, s16 x, s16 y, s16 w, s16 h);
void PSX_Render_Line_Prim_4F7D90(void* pOtPrim, s32 offX, s32 offY);

struct OT_Prim;
OT_Prim* PSX_Render_Convert_Polys_To_Internal_Format_4F7110(void* pData, s32 xoff, s32 yoff);
void PSX_Render_Internal_Format_Polygon_4F7960(OT_Prim* prim, s32 xoff, s32 yoff);

void PSX_DrawOTag_4F6540(PrimHeader** ppOt);
void PSX_OrderingTable_SaveRecord_4F62C0(s32** otBuffer, s32 otBufferSize);
void PSX_ClearOTag_4F6290(PrimHeader** otBuffer, s32 otBufferSize);

void PSX_TPage_Change_4F6430(s16 tPage);
s32 PSX_EMU_SetDispType_4F9960(s32 dispType);

void PSX_EMU_Render_SPRT_51EF90(s16 x, s16 y, s32 minX, s32 minY, u8 r, u8 g, u8 b, s16 w, s16 h, u16 clut, s32 semiTrans);
s32 PSX_ClearImage_4F5BD0(const PSX_RECT* pRect, u8 r, u8 g, u8 b);
void PSX_Pal_Conversion_4F98D0(const u16* pDataToConvert, u16* pConverted, u32 size);

void Add_Dirty_Area_4ED970(s32 x, s32 y, s32 w, s32 h);

void Psx_Render_Float_Table_Init();

ALIVE_VAR_EXTERN(s32, sScreenXOffSet_BD30E4);
ALIVE_VAR_EXTERN(s32, sScreenYOffset_BD30A4);

ALIVE_VAR_EXTERN(u32, sSemiTransShift_C215C0);
ALIVE_VAR_EXTERN(u32, sRedShift_C215C4);
ALIVE_VAR_EXTERN(u32, sGreenShift_C1D180);
ALIVE_VAR_EXTERN(u32, sBlueShift_C19140);
