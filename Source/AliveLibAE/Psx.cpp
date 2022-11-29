#include "stdafx.h"
#include "Psx.hpp"
#include "Sound/PsxSpuApi.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "VGA.hpp"
#include "stdlib.hpp"
#include "GameAutoPlayer.hpp"
#include <gmock/gmock.h>
#include "Sys.hpp"

extern bool gLatencyHack;

static s32 sVSyncLastMillisecond_BD0F2C = 0;
static s32 sLastFrameTimestampMilliseconds_BD0F24 = 0;

u8 turn_off_rendering_BD0F20 = 0;

s32 Game_End_Frame_4950F0(u32 flags);

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
    if (!Game_End_Frame_4950F0(0))
    {
        PSX_PutDispEnv_Impl_4F5640();

        Game_End_Frame_4950F0(1);
    }
}


void PSX_SetDefDispEnv_4F55A0(PSX_DISPENV* pOutEnv)
{
    if (!pOutEnv)
    {
        ALIVE_FATAL("SetDefDispEnv(): env == NULL");
        return;
    }

    PSX_DISPENV defEnv = {};
    defEnv.screen.w = 256;
    defEnv.screen.h = 240;
    memcpy(pOutEnv, &defEnv, sizeof(PSX_DISPENV));
}

void PSX_PutDispEnv_4F5890()
{
    if (Game_End_Frame_4950F0(0))
    {
        return;
    }

    PSX_PutDispEnv_Impl_4F5640();
    Game_End_Frame_4950F0(1);

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
