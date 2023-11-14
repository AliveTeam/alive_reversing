#include "stdafx.h"
#include "ScreenShake.hpp"
#include "../relive_lib/Events.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/Primitives.hpp"

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
        offset.y = yoff;

        mScreenOffset.SetOffset(offset.x, offset.y);
        ot.Add(Layer::eLayer_0, &mScreenOffset);

        if (mShakeNumber == 0)
        {
            SetDead(true);
        }
    }
}
