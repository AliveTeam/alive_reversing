#include "stdafx_ao.h"
#include "RollingBallShaker.hpp"
#include "Function.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/Primitives.hpp"
#include "../relive_lib/Primitives.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

const static PSX_Pos16 sRollingBallShakerScreenOffsets[18] = {
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

    SetType(ReliveTypes::eRollingBallStopperShaker);
    mShakeTableIdx = 0;
    mStopShaking = false; // Set externally
    gObjListDrawables->Push_Back(this);
}

RollingBallShaker::~RollingBallShaker()
{
    gObjListDrawables->Remove_Item(this);
}

void RollingBallShaker::VUpdate()
{
    mShakeTableIdx++;

    if (mShakeTableIdx >= ALIVE_COUNTOF(sRollingBallShakerScreenOffsets))
    {
        mShakeTableIdx = 0;
    }
}

void RollingBallShaker::VRender(PrimHeader** ppOt)
{
    Prim_ScreenOffset* pPrim = &mPrimScreenOffset[gPsxDisplay.mBufferIndex + 1];
    if (mStopShaking)
    {
        // Unshake the screen
        PSX_Pos16 screenOff = {};
        if (gPsxDisplay.mBufferIndex)
        {
            screenOff.y = gPsxDisplay.mHeight;
        }
        InitType_ScreenOffset(pPrim, &screenOff);
        OrderingTable_Add(OtLayer(ppOt, Layer::eLayer_0), &pPrim->mBase);

        // Kill yourself
        mBaseGameObjectFlags.Set(Options::eDead);
    }
    else
    {
        PSX_Pos16 screenOff = sRollingBallShakerScreenOffsets[mShakeTableIdx];
        if (gPsxDisplay.mBufferIndex)
        {
            screenOff.y += gPsxDisplay.mHeight;
        }

        InitType_ScreenOffset(pPrim, &screenOff);
        OrderingTable_Add(OtLayer(ppOt, Layer::eLayer_0), &pPrim->mBase);
    }
}

} // namespace AO
