#include "stdafx.h"
#include "Psx.hpp"
#include "Function.hpp"
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

static s32 sVSyncLastMillisecond_BD0F2C = 0;
static s32 sLastFrameTimestampMilliseconds_BD0F24 = 0;
static TPsxEmuCallBack sPsxEmu_put_disp_env_callback_C1D184 = nullptr;

PSX_DRAWENV sPSX_EMU_DrawEnvState_C3D080 = {};
u8 turn_off_rendering_BD0F20 = 0;

void PSX_EMU_SetCallBack_4F9430(TPsxEmuCallBack fnPtr)
{
    sPsxEmu_put_disp_env_callback_C1D184 = fnPtr;
}

static void PSX_PutDispEnv_Impl_4F5640()
{
    SsSeqCalledTbyT_4FDC80();

    if (!turn_off_rendering_BD0F20)
    {
        PSX_DrawDebugTextBuffers();
        VGA_EndFrame();
    }

    SsSeqCalledTbyT_4FDC80();
}

void PSX_PutDispEnv_4F58E0()
{
    if (!sPsxEmu_put_disp_env_callback_C1D184 || !sPsxEmu_put_disp_env_callback_C1D184(0))
    {
        PSX_PutDispEnv_Impl_4F5640();

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
        ALIVE_FATAL("SetDefDrawEnv(): env == NULL");
    }
}

void PSX_SetDefDispEnv_4F55A0(PSX_DISPENV* pOutEnv, s16 x, s16 y, s16 w, s16 h)
{
    if (!pOutEnv)
    {
        ALIVE_FATAL("SetDefDispEnv(): env == NULL");
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

void PSX_PutDispEnv_4F5890()
{
    if (sPsxEmu_put_disp_env_callback_C1D184)
    {
        if (sPsxEmu_put_disp_env_callback_C1D184(0))
        {
            return;
        }
    }

    PSX_PutDispEnv_Impl_4F5640();
    if (sPsxEmu_put_disp_env_callback_C1D184)
    {
        sPsxEmu_put_disp_env_callback_C1D184(1);
    }
}

void PSX_PutDrawEnv_4F5980(const PSX_DRAWENV* pDrawEnv)
{
    if (pDrawEnv)
    {
        memcpy(&sPSX_EMU_DrawEnvState_C3D080, pDrawEnv, sizeof(sPSX_EMU_DrawEnvState_C3D080));
    }
    else
    {
        ALIVE_FATAL("PutDrawEnv(): env == NULL");
    }
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
        ALIVE_FATAL("VSync(): negative param unsupported");
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
