#include "stdafx.h"
#include "Psx.hpp"
#include "Function.hpp"
#include "Error.hpp"
#include "bmp.hpp"

void Psx_ForceLink() {}

#define PSX_IMPL true

struct PSX_RECT
{
    short x, y, w, h;
};
ALIVE_ASSERT_SIZEOF(PSX_RECT, 8);

bool CC PSX_Rect_IsInFrameBuffer_4FA050(const PSX_RECT* pRect)
{
    return 
        pRect->x >= 0 && pRect->x < 1024 
        && pRect->y >= 0
        && pRect->y < 512
        && pRect->w + pRect->x - 1 >= 0
        && pRect->w + pRect->x - 1 < 1024
        && pRect->h + pRect->y - 1 >= 0
        && pRect->h + pRect->y - 1 < 512;
}
ALIVE_FUNC_IMPLEX(0x4FA050, PSX_Rect_IsInFrameBuffer_4FA050, PSX_IMPL);

ALIVE_VAR(1, 0xC1D160, Bitmap, sPsxVram_C1D160, {});

signed int CC PSX_MoveImage_4F5D50(const PSX_RECT* pRect, int xpos, int ypos)
{
    if (PSX_Rect_IsInFrameBuffer_4FA050(pRect))
    {
        RECT rect = {};
        rect.left = pRect->x;
        rect.top = pRect->y;
        rect.right = pRect->x + pRect->w;
        rect.bottom = pRect->y + pRect->h;
        BMP_Blt_4F1E50(&sPsxVram_C1D160, xpos, ypos, &sPsxVram_C1D160, &rect, 0);
        return 0;
    }

    Error_DisplayMessageBox_4F2C80("C:\\abe2\\code\\PSXEmu\\LIBGPU.C", 531, "MoveImage: BAD SRC RECT !!!");
    return -1;
}
ALIVE_FUNC_IMPLEX(0x4F5D50, PSX_MoveImage_4F5D50, PSX_IMPL);
