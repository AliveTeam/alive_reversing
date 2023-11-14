#include "stdafx.h"
#include "ScreenClipper.hpp"
#include "../../AliveLibAE/stdlib.hpp"
#include "../PsxDisplay.hpp"
#include "../Primitives.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

#undef min
#undef max

#include <algorithm>

ScreenClipper::ScreenClipper(PSX_Point xy, PSX_Point wh, Layer layer)
    : BaseGameObject(true, 0)
{
    SetType(ReliveTypes::eScreenClipper);

    SetUpdateDuringCamSwap(true);
    SetDrawable(true);

    mRect.x = xy.x;
    mRect.y = xy.y;

    mRect.w = wh.x;
    mRect.h = wh.y;

    mOtLayer = layer;

    gObjListDrawables->Push_Back(this);
}

ScreenClipper::~ScreenClipper()
{
    gObjListDrawables->Remove_Item(this);
}

void ScreenClipper::VRender(OrderingTable& ot)
{
    PSX_RECT clipRect = {};

    clipRect.x = mRect.x;
    clipRect.y = mRect.y;
    clipRect.w = mRect.w - mRect.x;
    clipRect.h = mRect.h - mRect.y;

    mClipper.SetRect(clipRect);
    ot.Add(mOtLayer, &mClipper);
}

void ScreenClipper::VUpdate()
{
    // Empty
}

void ScreenClipper::VScreenChanged()
{
    // Empty
}

void ScreenClipper::Update_Clip_Rect(PSX_Point xy, PSX_Point wh)
{
    mRect.x = std::min(xy.x, mRect.x);
    mRect.y = std::min(xy.y, mRect.y);

    mRect.w = std::max(wh.x, mRect.w);
    mRect.h = std::max(wh.y, mRect.h);
}
