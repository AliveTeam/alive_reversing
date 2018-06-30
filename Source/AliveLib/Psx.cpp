#include "stdafx.h"
#include "Psx.hpp"
#include "Function.hpp"
#include "Error.hpp"
#include "bmp.hpp"
#include "Midi.hpp"
#include "PsxDisplay.hpp"
#include "VGA.hpp"
#include "stdlib.hpp"
#include <timeapi.h>
#include <gmock/gmock.h>

void Psx_ForceLink() {}

ALIVE_VAR(1, 0x578325, char, sVSync_Unused_578325, 0);
ALIVE_VAR(1, 0xBD0F2C, int, sVSyncLastMillisecond_BD0F2C, 0);
ALIVE_VAR(1, 0xBD0F24, int, sLastFrameTimestampMilliseconds_BD0F24, 0);

ALIVE_VAR(1, 0xC3D080, PSX_DRAWENV, sPSX_EMU_DrawEnvState_C3D080, {});
ALIVE_VAR(1, 0x578E88, int, sConst_1000_578E88, 1000);
ALIVE_VAR(1, 0xBD1464, BYTE, byte_BD1464, 0);

ALIVE_VAR(1, 0xBDCD40, int, sPsx_drawenv_clipx_BDCD40, 0);
ALIVE_VAR(1, 0xBDCD44, int, sPsx_drawenv_clipy_BDCD44, 0);
ALIVE_VAR(1, 0xBDCD48, int, sPsx_drawenv_clipw_BDCD48, 0);
ALIVE_VAR(1, 0xBDCD4C, int, sPsx_drawenv_cliph_BDCD4C, 0);
ALIVE_VAR(1, 0xBDCD50, int, sPsx_drawenv_k500_BDCD50, 0);
ALIVE_VAR(1, 0xBDCD54, BYTE*, sPsx_drawenv_buffer_BDCD54, nullptr);
ALIVE_VAR(1, 0xBD1465, BYTE, sPsxEMU_show_vram_BD1465, 0);

ALIVE_VAR(1, 0xC1D160, Bitmap, sPsxVram_C1D160, {});
ALIVE_VAR(1, 0xC1D1A0, Bitmap, stru_C1D1A0, {}); // Note: never used?

ALIVE_VAR(1, 0xC2D060, PSX_DISPENV, sLastDispEnv_C2D060, {});
ALIVE_VAR(1, 0xBD146D, BYTE, sScreenMode_BD146D, 0);
ALIVE_VAR(1, 0xBD0F20, BYTE, byte_BD0F20, 0);
ALIVE_VAR(1, 0x578324, BYTE, byte_578324, 0);

using TPsxEmuCallBack = std::add_pointer<int(DWORD)>::type;

ALIVE_VAR(1, 0xC1D184, TPsxEmuCallBack, sPsxEmu_put_disp_env_callback_C1D184, nullptr);
ALIVE_VAR(1, 0xC1D17C, TPsxEmuCallBack, sPsxEmu_EndFrameFnPtr_C1D17C, nullptr);
ALIVE_VAR(1, 0xBD0F21, BYTE, sPsxDontChangeDispEnv_BD0F21, 0);
ALIVE_VAR(1, 0xBD145C, bool, sbBitmapsAllocated_BD145C, false);


struct OtUnknown
{
    int** field_0_pOtStart;
    int** field_4;
    int** field_8_pOt_End;
};

ALIVE_ARY(1, 0xBD0D88, OtUnknown, 32, sOt_Stack_BD0D88, {});
ALIVE_VAR(1, 0xBD0C08, int, sOtIdxRollOver_BD0C08, 0);

EXPORT void CC sub_4EDAB0(Bitmap* /*pBmp*/, int /*left*/, int /*top*/, int /*width*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC sub_4ED9E0()
{
    NOT_IMPLEMENTED();
}

EXPORT void CC PSX_DrawOTag_4F6540(int** /*pOT*/)
{
    NOT_IMPLEMENTED();
}

ALIVE_ARY(1, 0xC19160, float, 4096, sPsxEmu_float_table_C19160, {});
ALIVE_ARY(1, 0xC1D5C0, int, 4096, sPsxEmu_fixed_point_table_C1D5C0, {});

EXPORT void CC PSX_EMU_Init_4F9CD0(bool bShowVRam)
{
    memset(&sPsxVram_C1D160, 0, sizeof(sPsxVram_C1D160));
    memset(&stru_C1D1A0, 0, sizeof(stru_C1D1A0));

    sPsxEmu_EndFrameFnPtr_C1D17C = nullptr;
    sPsxEmu_put_disp_env_callback_C1D184 = nullptr;
    sPsxEMU_show_vram_BD1465 = bShowVRam;

    for (int i = 0; i < 4096; i++)
    {
        if (i == 0)
        {
            sPsxEmu_float_table_C19160[i] = 0;
            sPsxEmu_fixed_point_table_C1D5C0[i] = 0;
        }
        else
        {
            sPsxEmu_float_table_C19160[i] = 1.0f / static_cast<float>(i);
            sPsxEmu_fixed_point_table_C1D5C0[i] = 0x10000 / (i);
        }
    }

    // Note: sPsxEmu_BD1454 removed
}

ALIVE_VAR(1, 0xBD1468, int, sVGA_DisplayType_BD1468, 0);

EXPORT int CC PSX_EMU_SetDispType_4F9960(int dispType)
{
    NOT_IMPLEMENTED();
}

EXPORT int CC PSX_ResetCallBack_4FAA20()
{
    return 0;
}

EXPORT int CC PSX_CdInit_4FB2C0()
{
    return 1;
}

EXPORT int CC PSX_CdSetDebug_4FB330(int /*mode*/)
{
    return 1;
}

EXPORT int CC PSX_EMU_VideoAlloc_4F9D70()
{
    if (!sbBitmapsAllocated_BD145C)
    {
        if (sVGA_DisplayType_BD1468 == 1)
        {
            sVGA_DisplayType_BD1468 = 4;
            if (BMP_New_4F1990(&sPsxVram_C1D160, 1024, 512, 15, 1))
            {
                Error_PushErrorRecord_4F2920("C:\\abe2\\code\\PSXEmu\\PSXEMU.C", 405, -1, "PSXEMU_VideoAlloc: can't alloc PSX-VRAM");
                return -1;
            }

            PSX_EMU_SetDispType_4F9960(sVGA_DisplayType_BD1468);
        }
        else
        {
            int pixelFormat = 0;
            switch (sVGA_DisplayType_BD1468)
            {
            case 2:
                pixelFormat = 16;
                break;
            case 3:
                pixelFormat = 116;
                break;
            case 4:
                pixelFormat = 15;
                break;
            case 5:
                pixelFormat = 115;
                break;
            default:
                pixelFormat = 0;
                break;
            }

            if (BMP_New_4F1990(&sPsxVram_C1D160, 1024, 512, pixelFormat, 1))
            {
                Error_PushErrorRecord_4F2920("C:\\abe2\\code\\PSXEmu\\PSXEMU.C", 414, -1, "PSXEMU_VideoAlloc: can't alloc PSX-VRAM");
                return -1;
            }
        }

        byte_BD1464 = 0;
        sbBitmapsAllocated_BD145C = 1;
    }

    PSX_RECT rect = {};
    rect.x = 0;
    rect.y = 0;
    rect.w = 1024;
    rect.h = 512;
    PSX_ClearImage_4F5BD0(&rect, 0, 0, 0);
    return 0;
}

EXPORT void CC PSX_EMU_VideoDeAlloc_4FA010()
{
    if (sbBitmapsAllocated_BD145C)
    {
        Bmp_Free_4F1950(&sPsxVram_C1D160);
        if (byte_BD1464)
        {
            Bmp_Free_4F1950(&stru_C1D1A0);
            byte_BD1464 = 0;
        }
        sbBitmapsAllocated_BD145C = false;
    }
}

EXPORT void CC PSX_OrderingTable_4F62C0(int** otBuffer, int otBufferSize)
{
    int otIdx = 0;
    for (otIdx = 0; otIdx < 32; otIdx++)
    {
        if (otBuffer == sOt_Stack_BD0D88[otIdx].field_0_pOtStart)
        {
            break;
        }
    }

    if (otIdx == 32)
    {
        sOtIdxRollOver_BD0C08 = (sOtIdxRollOver_BD0C08 & 31);
        otIdx = sOtIdxRollOver_BD0C08;
    }

    sOt_Stack_BD0D88[otIdx].field_0_pOtStart = otBuffer;
    sOt_Stack_BD0D88[otIdx].field_4 = otBuffer;
    sOt_Stack_BD0D88[otIdx].field_8_pOt_End = &otBuffer[otBufferSize];
}


EXPORT void CC PSX_PutDispEnv_Impl_4F5640(const PSX_DISPENV* pDispEnv, char a2)
{
    if (!pDispEnv)
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\PSXEmu\\LIBGPU.C", 217, -1, "PutDispEnv(): env == NULL");
        return;
    }

    MIDI_UpdatePlayer_4FDC80();
    memcpy(&sLastDispEnv_C2D060, pDispEnv, sizeof(sLastDispEnv_C2D060));
    if (sPsxVram_C1D160.field_4_pLockedPixels)
    {
        BMP_unlock_4F2100(&sPsxVram_C1D160);
    }

    if (!byte_BD0F20 && byte_578324)
    {
        if (sPsxEMU_show_vram_BD1465)
        {
            VGA_CopyToFront_4F3710(&sPsxVram_C1D160, nullptr);
            MIDI_UpdatePlayer_4FDC80();
            return;
        }

        Bitmap* pBmp = nullptr;
        RECT rect = {};
        if (byte_BD1464)
        {
            rect.top = 0;
            rect.left = 0;
            rect.right = sLastDispEnv_C2D060.disp.w;
            rect.bottom = sLastDispEnv_C2D060.disp.h;
            pBmp = &stru_C1D1A0;
        }
        else
        {
            rect.left = sLastDispEnv_C2D060.disp.x;
            rect.top = sLastDispEnv_C2D060.disp.y;
            rect.right = sLastDispEnv_C2D060.disp.x + sLastDispEnv_C2D060.disp.w;
            rect.bottom = sLastDispEnv_C2D060.disp.y + sLastDispEnv_C2D060.disp.h;
            pBmp = &sPsxVram_C1D160;
        }

        PSX_DrawDebugTextBuffers(pBmp, rect);

        if (a2 && VGA_IsWindowMode_4F31E0())
        {
            sub_4EDAB0(pBmp, rect.left, rect.top, rect.right - rect.left);
            sub_4ED9E0();
        }
        else
        {
            VGA_CopyToFront_4F3EB0(pBmp, &rect, sScreenMode_BD146D);
        }

        // TODO: Removed dead increment here
    }
    MIDI_UpdatePlayer_4FDC80();
}

EXPORT int CC PSX_ResetGraph_4F8800(int)
{
    return 0;
}

EXPORT int CC PSX_SetVideoMode_4FA8F0()
{
    return 0;
}

EXPORT int CC PSX_SetGraphDebug_4F8A10(int)
{
    return 0;
}

EXPORT void CC PSX_PutDispEnv_4F58E0(const PSX_DISPENV* pDispEnv)
{
    if (!sPsxEmu_put_disp_env_callback_C1D184 || !sPsxEmu_put_disp_env_callback_C1D184(0))
    {
        if (!sPsxDontChangeDispEnv_BD0F21)
        {
            if (sVGA_Bmp1_BD2A20.field_8_width != 320 || pDispEnv->disp.w != 640)
            {
                PSX_PutDispEnv_Impl_4F5640(pDispEnv, 1);
            }
            else
            {
                PSX_PutDispEnv_Impl_4F5640(pDispEnv, 0);
            }
        }

        if (sPsxEmu_put_disp_env_callback_C1D184)
        {
            sPsxEmu_put_disp_env_callback_C1D184(1);
        }
    }
}

EXPORT void CC PSX_SetDefDrawEnv_4F5AA0(PSX_DRAWENV* pDrawEnv, __int16 x, __int16 y, __int16 w, __int16 h)
{
    PSX_DRAWENV drawEnv = {};
    drawEnv.field_0_clip.x = x;
    drawEnv.field_0_clip.y = y;

    drawEnv.field_8_ofs[0] = x;
    drawEnv.field_8_ofs[1] = y;

    drawEnv.field_0_clip.w = w;
    drawEnv.field_0_clip.h = h;

    drawEnv.field_16_dtd = 1;
    drawEnv.field_17_dfe = 1;

    drawEnv.field_C_tw.x = 0;
    drawEnv.field_C_tw.y = 0;

    drawEnv.field_C_tw.w = 640;
    drawEnv.field_C_tw.h = 0;

    drawEnv.field_14_tpage = 0;

    drawEnv.field_18_isbg = 0;
    drawEnv.field_19_r0 = 0;
    drawEnv.field_1A_g0 = 0;
    drawEnv.field_1B_b0 = 0;

    if (pDrawEnv)
    {
        memcpy(pDrawEnv, &drawEnv, sizeof(PSX_DRAWENV));
    }
    else
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\PSXEmu\\LIBGPU.C", 442, -1, "SetDefDrawEnv(): env == NULL");
    }
}

EXPORT void CC PSX_SetDefDispEnv_4F55A0(PSX_DISPENV* pOutEnv, __int16 x, __int16 y, __int16 w, __int16 h)
{
    if (!pOutEnv)
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\PSXEmu\\LIBGPU.C", 180, -1, "SetDefDispEnv(): env == NULL");
        return;
    }

    PSX_DISPENV defEnv = {};
    defEnv.disp.x = x;
    defEnv.disp.y = y;
    defEnv.disp.h = h;
    defEnv.disp.w = w;
    defEnv.screen.w = 256;
    defEnv.screen.h = 240;
    memcpy(pOutEnv, &defEnv, sizeof(PSX_DISPENV));
}

EXPORT void CC PSX_PutDispEnv_4F5890(PSX_DISPENV* pDispEnv)
{
    auto pFn = sPsxEmu_put_disp_env_callback_C1D184;
    if (sPsxEmu_put_disp_env_callback_C1D184)
    {
        if (sPsxEmu_put_disp_env_callback_C1D184(0))
        {
            return;
        }
        pFn = sPsxEmu_put_disp_env_callback_C1D184;
    }

    if (!sPsxDontChangeDispEnv_BD0F21)
    {
        PSX_PutDispEnv_Impl_4F5640(pDispEnv, 0);
        pFn = sPsxEmu_put_disp_env_callback_C1D184;
    }

    if (pFn)
    {
        pFn(1);
    }
}

EXPORT int CC PSX_SetDispMask_4F89F0(int /*mode*/)
{
    return 0;
}

EXPORT void CC PSX_ClearOTag_4F6290(int** otBuffer, int otBufferSize)
{
    PSX_OrderingTable_4F62C0(otBuffer, otBufferSize);

    // Set each element to point to the next
    int i = 0;
    for (i = 0; i < otBufferSize - 1; i++)
    {
        otBuffer[i] = reinterpret_cast<int*>(&otBuffer[i + 1]);
    }

    // Terminate the list
    otBuffer[i] = reinterpret_cast<int*>(0xFFFFFFFF);
}

EXPORT bool CC PSX_Rect_IsInFrameBuffer_4FA050(const PSX_RECT* pRect)
{
    return 
        pRect->x >= 0 && pRect->x < sPsxVram_C1D160.field_8_width
        && pRect->y >= 0
        && pRect->y < sPsxVram_C1D160.field_C_height
        && pRect->w + pRect->x - 1 >= 0
        && pRect->w + pRect->x - 1 < sPsxVram_C1D160.field_8_width
        && pRect->h + pRect->y - 1 >= 0
        && pRect->h + pRect->y - 1 < sPsxVram_C1D160.field_C_height;
}

EXPORT int CC PSX_LoadImage_4F5FB0(const PSX_RECT* pRect, BYTE* pData)
{
    if (!PSX_Rect_IsInFrameBuffer_4FA050(pRect))
    {
        return 0;
    }

    if (!BMP_Lock_4F1FF0(&sPsxVram_C1D160))
    {
        Error_PushErrorRecord_4F2920(
            "C:\\abe2\\code\\PSXEmu\\LIBGPU.C",
            678,
            -1,
            "LoadImage: can't lock the _psxemu_videomem");
        return 1;
    }

    // TODO: Clean up more, treat as 1024x512 16bit array
    const unsigned int bytesPerPixel = sPsxVram_C1D160.field_14_bpp / 8;
    unsigned int srcWidthInBytes = pRect->w * bytesPerPixel;
    BYTE* pDst = (BYTE *)sPsxVram_C1D160.field_4_pLockedPixels + bytesPerPixel * (pRect->x + (pRect->y * sPsxVram_C1D160.field_8_width));
    BYTE* pDataEnd = &pData[srcWidthInBytes * pRect->h];
    BYTE* pDataIter = pData;

    while (pDataIter < pDataEnd)
    {
        memcpy(pDst, pDataIter, srcWidthInBytes);
        pDataIter += srcWidthInBytes;
        pDst += (sPsxVram_C1D160.field_8_width * bytesPerPixel);
    }

    BMP_unlock_4F2100(&sPsxVram_C1D160);
    return 1;

    // Note: Removed width == 32 optimization case.
}

EXPORT void CC PSX_Pal_Conversion_4F98D0(WORD* pDataToConvert, WORD* pConverted, unsigned int size)
{
    NOT_IMPLEMENTED();
}

EXPORT int CC PSX_LoadImage16_4F5E20(const PSX_RECT* pRect, BYTE* pData)
{
    const unsigned int pixelCount = pRect->w * pRect->h;
    WORD* pConversionBuffer = reinterpret_cast<WORD*>(malloc_4F4E60(pixelCount * (sPsxVram_C1D160.field_14_bpp / 8)));
    if (!pConversionBuffer)
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\PSXEmu\\LIBGPU.C", 579, 0, "LoadImage16: can't do color conversion.");
        return PSX_LoadImage_4F5FB0(pRect, pData);
    }

    PSX_Pal_Conversion_4F98D0(reinterpret_cast<WORD*>(pData), pConversionBuffer, pixelCount);
    const auto loadImageRet = PSX_LoadImage_4F5FB0(pRect, reinterpret_cast<BYTE*>(pConversionBuffer));
    mem_free_4F4EA0(pConversionBuffer);
    return loadImageRet;
}

EXPORT void CC PSX_SetDrawEnv_Impl_4FE420(int x, int y, int w, int h, int unknown, BYTE* pBuffer)
{
    sPsx_drawenv_clipx_BDCD40 = x;
    sPsx_drawenv_clipy_BDCD44 = y;
    sPsx_drawenv_clipw_BDCD48 = w;
    sPsx_drawenv_cliph_BDCD4C = h;
    sPsx_drawenv_k500_BDCD50 = unknown;
    sPsx_drawenv_buffer_BDCD54 = pBuffer;
}

EXPORT void CC PSX_PutDrawEnv_4F5980(const PSX_DRAWENV* pDrawEnv)
{
    if (pDrawEnv)
    {
        memcpy(&sPSX_EMU_DrawEnvState_C3D080, pDrawEnv, sizeof(sPSX_EMU_DrawEnvState_C3D080));
        if (byte_BD1464)
        {
            PSX_SetDrawEnv_Impl_4FE420(
                0,
                0,
                16 * sPSX_EMU_DrawEnvState_C3D080.field_0_clip.w - 16,
                16 * sPSX_EMU_DrawEnvState_C3D080.field_0_clip.h - 16,
                sConst_1000_578E88 / 2,
                nullptr);
        }
        else
        {
            PSX_SetDrawEnv_Impl_4FE420(
                16 * sPSX_EMU_DrawEnvState_C3D080.field_0_clip.x,
                16 * sPSX_EMU_DrawEnvState_C3D080.field_0_clip.y,
                16 * (sPSX_EMU_DrawEnvState_C3D080.field_0_clip.x + sPSX_EMU_DrawEnvState_C3D080.field_0_clip.w) - 16,
                16 * (sPSX_EMU_DrawEnvState_C3D080.field_0_clip.y + sPSX_EMU_DrawEnvState_C3D080.field_0_clip.h) - 16,
                sConst_1000_578E88 / 2,
                nullptr);
        }
    }
    else
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\PSXEmu\\LIBGPU.C", 371, -1, "PutDrawEnv(): env == NULL");
    }
}

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

EXPORT signed int CC PSX_ClearImage_4F5BD0(PSX_RECT* /*pRect*/, unsigned __int8 /*r*/, unsigned __int8 /*g*/, __int16 /*b*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int CC PSX_DrawSync_4F6280(int /*mode*/)
{
    return 0;
}

namespace Test
{
    static void Test_PSX_ClearOTag_4F6290()
    {
        int* ot[5] = {};
        PSX_ClearOTag_4F6290(ot, 5);
        ASSERT_EQ(ot[0], (int*)&ot[1]);
        ASSERT_EQ(ot[4], (int*)0xFFFFFFFF);
    }

    void PsxTests()
    {
        Test_PSX_ClearOTag_4F6290();
    }
}
