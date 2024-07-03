#include "stdafx.h"
#include "RollingBallShaker.hpp"
#include "../Function.hpp"
#include "../PsxDisplay.hpp"
#include "../Primitives.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

// NOTE: this is the AO code and hasn't been compared since it's unused in original AE levels

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

void RollingBallShaker::VRender(OrderingTable& ot)
{
    if (mStopShaking)
    {
        // Unshake the screen
        mPrimScreenOffset.SetOffset(0, 0);
        ot.Add(Layer::eLayer_0, &mPrimScreenOffset);

        // Kill yourself
        SetDead(true);
    }
    else
    {
        mPrimScreenOffset.SetOffset(sRollingBallShakerScreenOffsets[mShakeTableIdx].x, sRollingBallShakerScreenOffsets[mShakeTableIdx].y);
        ot.Add(Layer::eLayer_0, &mPrimScreenOffset);
    }
}
