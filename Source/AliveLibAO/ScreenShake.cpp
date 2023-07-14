#include "stdafx_ao.h"
#include "ScreenShake.hpp"
#include "../relive_lib/Events.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/Primitives.hpp"

namespace AO {

ScreenShake::ScreenShake(bool enableShakeEvent)
    : BaseGameObject(true, 0)
{
    SetType(ReliveTypes::ScreenShake);

    SetDrawable(true);

    mShakeNumber = 16;
    mEnableShakeEvent = enableShakeEvent;

    gObjListDrawables->Push_Back(this);

    if (mEnableShakeEvent)
    {
        EventBroadcast(kEventScreenShake, this);
    }
}

ScreenShake::~ScreenShake()
{
    gObjListDrawables->Remove_Item(this);
}

void ScreenShake::VUpdate()
{
    if (mEnableShakeEvent)
    {
        EventBroadcast(kEventScreenShake, this);
    }

    if (mShakeNumber > 0)
    {
        mShakeNumber--;
    }
}

static const FP_Point sShakeOffsets[16] = {
    {FP_FromInteger(0), FP_FromInteger(-1)},
    {FP_FromInteger(-1), FP_FromInteger(0)},
    {FP_FromInteger(1), FP_FromInteger(-1)},
    {FP_FromInteger(-1), FP_FromInteger(1)},
    {FP_FromInteger(0), FP_FromInteger(-2)},
    {FP_FromInteger(-2), FP_FromInteger(0)},
    {FP_FromInteger(2), FP_FromInteger(-2)},
    {FP_FromInteger(-2), FP_FromInteger(2)},
    {FP_FromInteger(0), FP_FromInteger(-3)},
    {FP_FromInteger(-3), FP_FromInteger(0)},
    {FP_FromInteger(3), FP_FromInteger(-3)},
    {FP_FromInteger(-3), FP_FromInteger(3)},
    {FP_FromInteger(0), FP_FromInteger(-9)},
    {FP_FromInteger(-9), FP_FromInteger(0)},
    {FP_FromInteger(9), FP_FromInteger(-9)},
    {FP_FromInteger(-9), FP_FromInteger(9)}};


void ScreenShake::VRender(OrderingTable& ot)
{
    if (mShakeNumber < 14)
    {
        const s16 xoff = FP_GetExponent(sShakeOffsets[mShakeNumber].x); // TODO: Div 16 ??
        const s16 yoff = FP_GetExponent(sShakeOffsets[mShakeNumber].y);

        PSX_Pos16 offset = {};
        offset.x = PsxToPCX(xoff); // TODO + 11 ?
        offset.y = yoff;

        mScreenOffset.SetOffset(offset.x, offset.y);
        ot.Add(Layer::eLayer_0, &mScreenOffset);

        if (mShakeNumber == 0)
        {
            SetDead(true);
        }
    }
}

} // namespace AO
