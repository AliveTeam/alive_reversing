#include "stdafx_ao.h"
#include "RollingBallShaker.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "PsxDisplay.hpp"
#include "ScreenManager.hpp"
#include "../AliveLibAE/Primitives.hpp"

void RollingBallShaker_ForceLink()
{ }

namespace AO {

const static PSX_Pos16 sRollingBallShakerScreenOffsets_4BB740[18] = {
    {1, 0},
    {0, 0},
    {-1, 1},
    {0, 0},
    {-1, -1},
    {0, 0},
    {1, -1},
    {0, 0},
    {0, 1},
    {0, 0},
    {1, 0},
    {0, 0},
    {1, 1},
    {0, 0},
    {-1, -1},
    {0, 0},
    {0, -1},
    {0, 0}};

RollingBallShaker::RollingBallShaker()
    : BaseGameObject(TRUE, 0)
{
    mBaseGameObjectFlags.Set(Options::eDrawable_Bit4);

    mBaseGameObjectTypeId = ReliveTypes::eRollingBallStopperShaker;
    field_30_shake_table_idx = 0;
    field_32_bKillMe = 0; // Set externally
    gObjListDrawables->Push_Back(this);
}

RollingBallShaker::~RollingBallShaker()
{
    gObjListDrawables->Remove_Item(this);
}

void RollingBallShaker::VUpdate()
{
    field_30_shake_table_idx++;

    if (field_30_shake_table_idx >= ALIVE_COUNTOF(sRollingBallShakerScreenOffsets_4BB740))
    {
        field_30_shake_table_idx = 0;
    }
}

void RollingBallShaker::VRender(PrimHeader** ppOt)
{
    Prim_ScreenOffset* pPrim = &field_10_prim_screen_offset[gPsxDisplay.mBufferIndex + 1];
    if (field_32_bKillMe != 0)
    {
        // Unshake the screen
        PSX_Pos16 screenOff = {};
        if (gPsxDisplay.mBufferIndex)
        {
            screenOff.y = gPsxDisplay.mHeight;
        }
        InitType_ScreenOffset_496000(pPrim, &screenOff);
        OrderingTable_Add(OtLayer(ppOt, Layer::eLayer_0), &pPrim->mBase);

        // Kill yourself
        mBaseGameObjectFlags.Set(Options::eDead);
    }
    else
    {
        PSX_Pos16 screenOff = sRollingBallShakerScreenOffsets_4BB740[field_30_shake_table_idx];
        if (gPsxDisplay.mBufferIndex)
        {
            screenOff.y += gPsxDisplay.mHeight;
        }

        InitType_ScreenOffset_496000(pPrim, &screenOff);
        OrderingTable_Add(OtLayer(ppOt, Layer::eLayer_0), &pPrim->mBase);
    }
    pScreenManager->InvalidateRectCurrentIdx(0, 0, 640, gPsxDisplay.mHeight);
}

} // namespace AO
