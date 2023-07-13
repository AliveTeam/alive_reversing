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
        Prim_ScreenOffset* pPrim = &mScreenOffset[gPsxDisplay.mBufferIndex];

        s16 xoff = 0;
        s16 yoff = 0;
        xoff = FP_GetExponent(sShakeOffsets[mShakeNumber].x); // TODO: Div 16 ??
        yoff = FP_GetExponent(sShakeOffsets[mShakeNumber].y);

        if (gPsxDisplay.mBufferIndex)
        {
            yoff += gPsxDisplay.mHeight;
        }

        PSX_Pos16 offset = {};
        offset.x = PsxToPCX(xoff); // TODO + 11 ?
        if (gPsxDisplay.mBufferIndex)
        {
            offset.y = yoff + gPsxDisplay.mHeight;
        }
        else
        {
            offset.y = yoff;
        }

        pPrim->SetOffset(offset.x, offset.y);
        OrderingTable_Add(OtLayer(ppOt, Layer::eLayer_0), pPrim);

        if (offset.y != 0)
        {
            PSX_RECT clearRect = {};
            if (offset.y < 0)
            {
                clearRect.y = offset.y + gPsxDisplay.mHeight;
                clearRect.h = -offset.y;
            }
            else if (offset.y > 0)
            {
                clearRect.y = 0;
                clearRect.h = offset.y;
            }

            clearRect.x = 0;
            clearRect.w = 640; // Could probably replace with `gPsxDisplay.mWidth`
            //PSX_ClearImage_496020(&clearRect, 0, 0, 0);
        }

        if (offset.x != 0)
        {
            PSX_RECT clearRect = {};
            if (offset.x < 0)
            {
                clearRect.x = offset.x + 640; // Could probably replace with `gPsxDisplay.mWidth`
                clearRect.w = -offset.x;
            }
            else if (offset.x > 0)
            {
                clearRect.x = 0;
                clearRect.w = offset.x;
            }

            clearRect.y = 0;
            clearRect.h = gPsxDisplay.mHeight;
            //PSX_ClearImage_496020(&clearRect, 0, 0, 0);
        }

        if (!mShakeNumber)
        {
            SetDead(true);
        }
    }
}

} // namespace AO
