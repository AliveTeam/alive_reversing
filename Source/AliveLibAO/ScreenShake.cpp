#include "stdafx_ao.h"
#include "Function.hpp"
#include "ScreenShake.hpp"
#include "Events.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "PsxDisplay.hpp"
#include "../relive_lib/Primitives.hpp"
#include "ScreenManager.hpp"
#include "PsxRender.hpp"

namespace AO {

ScreenShake::ScreenShake(bool enableShakeEvent)
    : BaseGameObject(TRUE, 0)
{
    mBaseGameObjectTypeId = ReliveTypes::ScreenShake;

    mBaseGameObjectFlags.Set(Options::eDrawable_Bit4);

    field_30_shakeNumber = 16;
    field_32_enableShakeEvent = enableShakeEvent;

    gObjListDrawables->Push_Back(this);

    if (field_32_enableShakeEvent)
    {
        EventBroadcast(kEventScreenShake, this);
    }
}

void ScreenShake::VUpdate()
{
    if (field_32_enableShakeEvent)
    {
        EventBroadcast(kEventScreenShake, this);
    }

    if (field_30_shakeNumber > 0)
    {
        field_30_shakeNumber--;
    }
}

const FP_Point stru_4CF850[16] = {
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


void ScreenShake::VRender(PrimHeader** ppOt)
{
    if (field_30_shakeNumber < 14)
    {
        Prim_ScreenOffset* pPrim = &field_10_screenOffset[gPsxDisplay.mBufferIndex];

        s16 xoff = 0;
        s16 yoff = 0;
        xoff = FP_GetExponent(stru_4CF850[field_30_shakeNumber].x); // TODO: Div 16 ??
        yoff = FP_GetExponent(stru_4CF850[field_30_shakeNumber].y);

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

        InitType_ScreenOffset(pPrim, &offset);
        OrderingTable_Add(OtLayer(ppOt, Layer::eLayer_0), &pPrim->mBase);

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
            PSX_ClearImage_496020(&clearRect, 0, 0, 0);
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
            PSX_ClearImage_496020(&clearRect, 0, 0, 0);
        }

        if (!field_30_shakeNumber)
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }


    pScreenManager->InvalidateRectCurrentIdx(0, 0, 640, 240);
}

ScreenShake::~ScreenShake()
{
    gObjListDrawables->Remove_Item(this);
}

} // namespace AO
