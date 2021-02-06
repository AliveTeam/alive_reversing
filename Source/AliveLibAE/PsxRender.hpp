#pragma once

#include "FunctionFwd.hpp"

struct PSX_RECT;
struct PrimHeader;

namespace Test
{
    void PsxRenderTests();
}
union PrimAny;

void DrawOTag_Render_SPRT(PrimAny& any, __int16 drawEnv_of0, __int16 drawEnv_of1, short width, short height);
EXPORT void CC PSX_RenderLaughingGasEffect_4F7B80(int xpos, int ypos, int width, int height, WORD* pData);
void DrawOTag_Render_TILE(PrimAny& any, short x, short y, short w, short h);
EXPORT void CC PSX_Render_Line_Prim_4F7D90(void* pOtPrim, int offX, int offY);

struct OT_Prim;
EXPORT OT_Prim* CC PSX_Render_Convert_Polys_To_Internal_Format_4F7110(void* pData, int xoff, int yoff);
EXPORT void CC PSX_Render_Internal_Format_Polygon_4F7960(OT_Prim* prim, int xoff, int yoff);

EXPORT void CC PSX_DrawOTag_4F6540(PrimHeader** ppOt);
EXPORT void CC PSX_OrderingTable_SaveRecord_4F62C0(int** otBuffer, int otBufferSize);
EXPORT void CC PSX_ClearOTag_4F6290(PrimHeader** otBuffer, int otBufferSize);

EXPORT void CC PSX_TPage_Change_4F6430(__int16 tPage);
EXPORT int CC PSX_EMU_SetDispType_4F9960(int dispType);

EXPORT void CC PSX_EMU_Render_SPRT_51EF90(__int16 x, __int16 y, int minX, int minY, BYTE r, BYTE g, BYTE b, __int16 w, __int16 h, WORD clut, int semiTrans);
EXPORT int CC PSX_ClearImage_4F5BD0(const PSX_RECT* pRect, unsigned __int8 r, unsigned __int8 g, unsigned __int8 b);
EXPORT void CC PSX_Pal_Conversion_4F98D0(const WORD* pDataToConvert, WORD* pConverted, unsigned int size);

EXPORT void CC Add_Dirty_Area_4ED970(int x, int y, int w, int h);

void Psx_Render_Float_Table_Init();

ALIVE_VAR_EXTERN(int, sScreenXOffSet_BD30E4);
ALIVE_VAR_EXTERN(int, sScreenYOffset_BD30A4);

ALIVE_VAR_EXTERN(DWORD, sSemiTransShift_C215C0);
ALIVE_VAR_EXTERN(DWORD, sRedShift_C215C4);
ALIVE_VAR_EXTERN(DWORD, sGreenShift_C1D180);
ALIVE_VAR_EXTERN(DWORD, sBlueShift_C19140);