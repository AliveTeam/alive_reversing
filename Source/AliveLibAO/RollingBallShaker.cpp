#include "stdafx_ao.h"
#include "RollingBallShaker.hpp"
#include "../relive_lib/Function.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/Primitives.hpp"
#include "../relive_lib/Primitives.hpp"
#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"

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
    : BaseGameObject(true, 0)
{
    SetDrawable(true);

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
        SetDead(true);
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
