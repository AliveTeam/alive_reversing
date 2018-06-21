#include "stdafx.h"
#include "Psx.hpp"
#include "Function.hpp"
#include "Error.hpp"
#include "bmp.hpp"
#include "Midi.hpp"

#include <timeapi.h>

void Psx_ForceLink() {}

ALIVE_VAR(1, 0x578325, char, sVSync_Unused_578325, 0);
ALIVE_VAR(1, 0xBD0F2C, int, sVSyncLastMillisecond_BD0F2C, 0);
ALIVE_VAR(1, 0xBD0F24, int, sLastFrameTimestampMilliseconds_BD0F24, 0);

EXPORT bool CC PSX_Rect_IsInFrameBuffer_4FA050(const PSX_RECT* pRect)
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

ALIVE_VAR(1, 0xC1D160, Bitmap, sPsxVram_C1D160, {});

EXPORT signed int CC PSX_MoveImage_4F5D50(const PSX_RECT* pRect, int xpos, int ypos)
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

// If mode is 1, game doesn't frame cap at all. If it is greater than 1, then it caps to (60 / mode) fps.
EXPORT int CC PSX_VSync_4F6170(int mode)
{
    sVSync_Unused_578325 = 0;
    MIDI_UpdatePlayer_4FDC80();

    const int currentTime = timeGetTime();

    if (!sVSyncLastMillisecond_BD0F2C)
    {
        sVSyncLastMillisecond_BD0F2C = currentTime;
    }

    if (mode == 1) // Ignore Frame cap
    {
        sVSync_Unused_578325 = 1;
        const int v3 = (signed int)((unsigned __int64)(1172812403i64 * (signed int)(240 * (currentTime - sVSyncLastMillisecond_BD0F2C))) >> 32) >> 14;
        return (v3 >> 31) + v3;
    }
    else if (mode < 0) // Nope.
    {
        sVSync_Unused_578325 = 1;
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\PSXEmu\\LIBGPU.C", 756, -1, "VSync(): negative param unsupported");
        return 0;
    }
    else
    {
        int frameTimeInMilliseconds = currentTime - sVSyncLastMillisecond_BD0F2C;
        if (mode > 0 && frameTimeInMilliseconds < 1000 * mode / 60)
        {
            int timeSinceLastFrame = 0;
            sVSync_Unused_578325 = 1;

            do
            {
                timeSinceLastFrame = timeGetTime() - sVSyncLastMillisecond_BD0F2C;
                MIDI_UpdatePlayer_4FDC80();
            } while (timeSinceLastFrame < 1000 * mode / 60);

            frameTimeInMilliseconds = 1000 * mode / 60;
        }

        sVSyncLastMillisecond_BD0F2C += frameTimeInMilliseconds;
        sLastFrameTimestampMilliseconds_BD0F24 = currentTime + frameTimeInMilliseconds;

        return 240 * frameTimeInMilliseconds / 60000;
    }
}
