#include "stdafx.h"
#include "Psx.hpp"
#include "Sound/PsxSpuApi.hpp"
#include "PsxDisplay.hpp"
#include "../AliveLibAE/VGA.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../AliveLibAE/GameAutoPlayer.hpp"
#include "Sys.hpp"

extern bool gLatencyHack;

static s32 sVSyncLastMillisecond = 0;
static s32 sLastFrameTimestampMilliseconds_BD0F24 = 0;
static TPsxEmuCallBack sPsxEmu_put_disp_env_callback_C1D184 = nullptr;

bool gTurnOffRendering = false;

void PSX_EMU_SetCallBack_4F9430(TPsxEmuCallBack fnPtr)
{
    sPsxEmu_put_disp_env_callback_C1D184 = fnPtr;
}

static void PSX_PutDispEnv_Impl_4F5640()
{
    SsSeqCalledTbyT();

    if (!gTurnOffRendering)
    {
        PSX_DrawDebugTextBuffers();
        VGA_EndFrame();
    }

    SsSeqCalledTbyT();
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

bool PSX_Rects_overlap_4FA0B0(const PSX_RECT* pRect1, const PSX_RECT* pRect2)
{
    return pRect1->x < (pRect2->x + pRect2->w)
        && pRect1->y < (pRect2->y + pRect2->h)
        && pRect2->x < (pRect1->x + pRect1->w)
        && pRect2->y < (pRect1->y + pRect1->h);
}


void PSX_Prevent_Rendering()
{
    gTurnOffRendering = true;
}


// If mode is 1, game doesn't frame cap at all. If it is greater than 1, then it caps to (60 / mode) fps.
void PSX_VSync(VSyncMode mode)
{
    SsSeqCalledTbyT();

    const s32 currentTime = SYS_GetTicks();

    if (!sVSyncLastMillisecond)
    {
        sVSyncLastMillisecond = currentTime;
    }

    if (GetGameAutoPlayer().IsPlaying() && GetGameAutoPlayer().NoFpsLimitPlayBack())
    {
        // Uncapped playback
        return;
    }

    s32 frameTimeInMilliseconds = currentTime - sVSyncLastMillisecond;
    if (mode == VSyncMode::LimitTo30Fps && frameTimeInMilliseconds < (1000 * 2) / 60)
    {
        s32 timeSinceLastFrame = 0;
        do
        {
            timeSinceLastFrame = SYS_GetTicks() - sVSyncLastMillisecond;

            // During recording or playback do not call SsSeqCalledTbyT an undeterminate
            // amount of times as this can leak to de-syncs.
            if (!GetGameAutoPlayer().IsRecording() && !GetGameAutoPlayer().IsPlaying())
            {
                SsSeqCalledTbyT();

                // Prevent max CPU usage, will probably cause stuttering on weaker machines
                if (gLatencyHack)
                {
                    SDL_Delay(1);
                }
            }
        }
        while (timeSinceLastFrame < (1000 * 2) / 60);

        frameTimeInMilliseconds = (1000 * 2) / 60;
    }

    sVSyncLastMillisecond += frameTimeInMilliseconds;
    sLastFrameTimestampMilliseconds_BD0F24 = currentTime + frameTimeInMilliseconds;
}
