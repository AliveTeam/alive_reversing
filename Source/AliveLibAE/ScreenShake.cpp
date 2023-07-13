#include "stdafx.h"
#include "ScreenShake.hpp"
#include "../relive_lib/Events.hpp"
#include "stdlib.hpp"
#include "../relive_lib/PsxDisplay.hpp"

 ScreenShake::ScreenShake(bool enableShakeEvent, bool softerShakes)
     : BaseGameObject(true, 0)
 {
    SetType(ReliveTypes::eScreenShake);

    SetDrawable(true);

    mSofterShakes = softerShakes;
    mShakeNumber = 16;
    mEnableShakeEvent = enableShakeEvent;

    gObjListDrawables->Push_Back(this);

    if (mEnableShakeEvent && !mSofterShakes)
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
    if (mEnableShakeEvent && !mSofterShakes)
    {
        EventBroadcast(kEventScreenShake, this);
    }

    if (mShakeNumber > 0)
    {
        mShakeNumber--;
    }
}

struct ScreenOffset final
{
    s8 x;
    s8 y;
};

static const ScreenOffset sShakeOffsets[16] = {
    {0, -1},
    {-1, 0},
    {1, -1},
    {-1, 1},
    {0, -2},
    {-2, 0},
    {2, -2},
    {-2, 2},
    {0, -3},
    {-3, 0},
    {3, -3},
    {-3, 3},
    {0, -9},
    {-9, 0},
    {9, -9},
    {-9, 9},
};

void ScreenShake::VRender(OrderingTable& ot)
{
    Prim_ScreenOffset* pPrim = &mScreenOffset[gPsxDisplay.mBufferIndex];
    if (mShakeNumber < 14)
    {
        s16 xoff = 0;
        s16 yoff = 0;

        if (mSofterShakes)
        {
            xoff = sShakeOffsets[mShakeNumber].x / 2;
            yoff = sShakeOffsets[mShakeNumber].y / 2;
        }
        else
        {
            xoff = sShakeOffsets[mShakeNumber].x;
            yoff = sShakeOffsets[mShakeNumber].y;
        }

        PSX_Pos16 offset = {};
        offset.x = PsxToPCX(xoff); // TODO + 11 ?
        if (gPsxDisplay.mBufferIndex)
        {
            offset.y = yoff + 256;
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
            //PSX_ClearImage_4F5BD0(&clearRect, 0, 0, 0);
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
            //PSX_ClearImage_4F5BD0(&clearRect, 0, 0, 0);
        }

        if (!mShakeNumber)
        {
            SetDead(true);
        }
    }
}
