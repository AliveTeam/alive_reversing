#include "stdafx.h"
#include "Psx.hpp"
#include "Function.hpp"
#include "../relive_lib/Error.hpp"
#include "../relive_lib/bmp.hpp"
#include "Sound/PsxSpuApi.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "VGA.hpp"
#include "stdlib.hpp"
#include "Io.hpp"
#include "PsxRender.hpp"
#include "PsxRender.hpp"
#include "Renderer/IRenderer.hpp"
#include "GameAutoPlayer.hpp"
#include <gmock/gmock.h>

extern bool gLatencyHack;

s32 sVSyncLastMillisecond_BD0F2C = 0;
s32 sLastFrameTimestampMilliseconds_BD0F24 = 0;

PSX_DRAWENV sPSX_EMU_DrawEnvState_C3D080 = {};
s32 sConst_1000_578E88 = 1000;

s32 sPsx_drawenv_clipx_BDCD40 = 0;
s32 sPsx_drawenv_clipy_BDCD44 = 0;
s32 sPsx_drawenv_clipw_BDCD48 = 0;
s32 sPsx_drawenv_cliph_BDCD4C = 0;
s32 sPsx_drawenv_k500_BDCD50 = 0;
u8* sPsx_drawenv_buffer_BDCD54 = nullptr;
u8 sPsxEMU_show_vram_BD1465 = 0;

Bitmap sBitmap_C1D1A0 = {}; // Note: never used?

PSX_DISPENV sLastDispEnv_C2D060 = {};
u8 sScreenMode_BD146D = 0;
u8 turn_off_rendering_BD0F20 = 0;
u8 byte_578324 = 1;


IO_Handle* sCdFileHandle_BD1CC4 = nullptr;
s32 sCdReadPos_BD1894 = 0;

TPsxEmuCallBack sPsxEmu_put_disp_env_callback_C1D184 = nullptr;
TPsxEmuCallBack sPsxEmu_EndFrameFnPtr_C1D17C = nullptr;
u8 sPsxDontChangeDispEnv_BD0F21 = 0;

Bitmap* spBitmap_C2D038 = nullptr;

void PSX_EMU_Init_4F9CD0()
{
    memset(&sBitmap_C1D1A0, 0, sizeof(sBitmap_C1D1A0));

    sPsxEmu_EndFrameFnPtr_C1D17C = nullptr;
    sPsxEmu_put_disp_env_callback_C1D184 = nullptr;

    // Note: sPsxEmu_BD1454 removed
}

s32 sVGA_DisplayType_BD1468 = 0;

void PSX_EMU_SetCallBack_4F9430(s32 callBackType, TPsxEmuCallBack fnPtr)
{
    if (callBackType)
    {
        if (callBackType == 1)
        {
            sPsxEmu_put_disp_env_callback_C1D184 = fnPtr;
        }
    }
    else
    {
        sPsxEmu_EndFrameFnPtr_C1D17C = fnPtr;
    }
}


#if _WIN32
const char_type kDirChar[] = "\\";
#else
const char_type kDirChar[] = "/";
#endif


void Init_VGA_AndPsxVram_494690()
{
    VGA_DisplaySet_4F32C0(640u, 480u, 16u, 2u, 0);
}


void PSX_PutDispEnv_Impl_4F5640(const PSX_DISPENV* pDispEnv)
{
    if (!pDispEnv)
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\PSXEmu\\LIBGPU.C", 217, -1, "PutDispEnv(): env == NULL");
        return;
    }

    SsSeqCalledTbyT_4FDC80();
    memcpy(&sLastDispEnv_C2D060, pDispEnv, sizeof(sLastDispEnv_C2D060));

    if (!turn_off_rendering_BD0F20 && byte_578324)
    {
        RECT rect = {};
        rect.left = sLastDispEnv_C2D060.disp.x;
        rect.top = sLastDispEnv_C2D060.disp.y;
        rect.right = sLastDispEnv_C2D060.disp.x + sLastDispEnv_C2D060.disp.w;
        rect.bottom = sLastDispEnv_C2D060.disp.y + sLastDispEnv_C2D060.disp.h;

        PSX_DrawDebugTextBuffers(nullptr, rect);

        // Clear/end frame
        VGA_CopyToFront(&rect);
    }
    SsSeqCalledTbyT_4FDC80();
}

void PSX_PutDispEnv_4F58E0(const PSX_DISPENV* pDispEnv)
{
    if (!sPsxEmu_put_disp_env_callback_C1D184 || !sPsxEmu_put_disp_env_callback_C1D184(0))
    {
        if (!sPsxDontChangeDispEnv_BD0F21)
        {
            PSX_PutDispEnv_Impl_4F5640(pDispEnv);
        }

        if (sPsxEmu_put_disp_env_callback_C1D184)
        {
            sPsxEmu_put_disp_env_callback_C1D184(1);
        }
    }
}

void PSX_SetDefDrawEnv_4F5AA0(PSX_DRAWENV* pDrawEnv, s16 x, s16 y, s16 w, s16 h)
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

void PSX_SetDefDispEnv_4F55A0(PSX_DISPENV* pOutEnv, s16 x, s16 y, s16 w, s16 h)
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

void PSX_PutDispEnv_4F5890(PSX_DISPENV* pDispEnv)
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
        PSX_PutDispEnv_Impl_4F5640(pDispEnv);
        pFn = sPsxEmu_put_disp_env_callback_C1D184;
    }

    if (pFn)
    {
        pFn(1);
    }
}

void PSX_SetDrawEnv_Impl_4FE420(s32 x, s32 y, s32 w, s32 h, s32 unknown, u8* pBuffer)
{
    sPsx_drawenv_clipx_BDCD40 = x;
    sPsx_drawenv_clipy_BDCD44 = y;
    sPsx_drawenv_clipw_BDCD48 = w;
    sPsx_drawenv_cliph_BDCD4C = h;
    sPsx_drawenv_k500_BDCD50 = unknown;
    sPsx_drawenv_buffer_BDCD54 = pBuffer;
}

void PSX_PutDrawEnv_4F5980(const PSX_DRAWENV* pDrawEnv)
{
    if (pDrawEnv)
    {
        memcpy(&sPSX_EMU_DrawEnvState_C3D080, pDrawEnv, sizeof(sPSX_EMU_DrawEnvState_C3D080));

        PSX_SetDrawEnv_Impl_4FE420(
            16 * sPSX_EMU_DrawEnvState_C3D080.field_0_clip.x,
            16 * sPSX_EMU_DrawEnvState_C3D080.field_0_clip.y,
            16 * (sPSX_EMU_DrawEnvState_C3D080.field_0_clip.x + sPSX_EMU_DrawEnvState_C3D080.field_0_clip.w) - 16,
            16 * (sPSX_EMU_DrawEnvState_C3D080.field_0_clip.y + sPSX_EMU_DrawEnvState_C3D080.field_0_clip.h) - 16,
            sConst_1000_578E88 / 2,
            nullptr);
    }
    else
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\PSXEmu\\LIBGPU.C", 371, -1, "PutDrawEnv(): env == NULL");
    }
}

void PSX_CD_Normalize_FileName_4FAD90(char_type* pNormalized, const char_type* pFileName)
{
    const char_type* fileNameIter = pFileName;
    char_type* pNormalizedIter = pNormalized;
    while (*fileNameIter)
    {
        if (*fileNameIter == ';')
        {
            break;
        }

        *pNormalizedIter = static_cast<s8>(tolower(*fileNameIter));

        ++fileNameIter;
        ++pNormalizedIter;
    }
    *pNormalizedIter = 0;
}

bool PSX_Rects_overlap_4FA0B0(const PSX_RECT* pRect1, const PSX_RECT* pRect2)
{
    return pRect1->x < (pRect2->x + pRect2->w)
        && pRect1->y < (pRect2->y + pRect2->h)
        && pRect2->x < (pRect1->x + pRect1->w)
        && pRect2->y < (pRect1->y + pRect1->h);
}


void PSX_Prevent_Rendering_4945B0()
{
    turn_off_rendering_BD0F20 = 1;
}

// If mode is 1, game doesn't frame cap at all. If it is greater than 1, then it caps to (60 / mode) fps.
void PSX_VSync_4F6170(s32 mode)
{
    SsSeqCalledTbyT_4FDC80();

    const s32 currentTime = SYS_GetTicks();

    if (!sVSyncLastMillisecond_BD0F2C)
    {
        sVSyncLastMillisecond_BD0F2C = currentTime;
    }

    if (GetGameAutoPlayer().IsPlaying() && GetGameAutoPlayer().NoFpsLimitPlayBack())
    {
        // Uncapped playback
        return;
    }

    if (mode == 1) // Ignore Frame cap
    {
        // Do nothing
    }
    else if (mode < 0) // Error
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\PSXEmu\\LIBGPU.C", 756, -1, "VSync(): negative param unsupported");
    }
    else
    {
        s32 frameTimeInMilliseconds = currentTime - sVSyncLastMillisecond_BD0F2C;
        if (mode > 0 && frameTimeInMilliseconds < 1000 * mode / 60)
        {
            s32 timeSinceLastFrame = 0;

            do
            {
                timeSinceLastFrame = SYS_GetTicks() - sVSyncLastMillisecond_BD0F2C;
                // During recording or playback do not call SsSeqCalledTbyT_4FDC80 an undeterminate
                // amount of times as this can leak to de-syncs.
                if (!GetGameAutoPlayer().IsRecording() && !GetGameAutoPlayer().IsPlaying())
                {
                    SsSeqCalledTbyT_4FDC80();

                    // Prevent max CPU usage, will probably cause stuttering on weaker machines
                    if (gLatencyHack)
                    {
                        SDL_Delay(1);
                    }
                }
            }
            while (timeSinceLastFrame < 1000 * mode / 60);

            frameTimeInMilliseconds = 1000 * mode / 60;
        }

        sVSyncLastMillisecond_BD0F2C += frameTimeInMilliseconds;
        sLastFrameTimestampMilliseconds_BD0F24 = currentTime + frameTimeInMilliseconds;
    }
}
