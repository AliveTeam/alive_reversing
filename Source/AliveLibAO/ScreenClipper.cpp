#include "stdafx_ao.h"
#include "ScreenClipper.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/Primitives.hpp"
#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"

#undef min
#undef max

#include <algorithm>

namespace AO {

ScreenClipper::~ScreenClipper()
{
    gObjListDrawables->Remove_Item(this);
}

void ScreenClipper::VRender(BasePrimitive** ppOt)
{
    PSX_RECT clipRect = {};

    clipRect.x = mRect.x;
    clipRect.y = mRect.y;
    clipRect.w = mRect.w - mRect.x;
    clipRect.h = mRect.h - mRect.y;

    if (gPsxDisplay.mBufferIndex)
    {
        // Move to the lower buffer if not using the top buffer
        clipRect.y += gPsxDisplay.mHeight;
    }

    Prim_ScissorRect* pClipper = &mClippers[gPsxDisplay.mBufferIndex];
    pClipper->SetRect(clipRect);
    OrderingTable_Add(OtLayer(ppOt, mOtLayer), pClipper);
}

void ScreenClipper::VUpdate()
{
    // Empty
}

void ScreenClipper::VScreenChanged()
{
    // Empty
}

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

void ScreenClipper::Update_Clip_Rect(PSX_Point xy, PSX_Point wh)
{
    mRect.x = std::min(xy.x, mRect.x);
    mRect.y = std::min(xy.y, mRect.y);

    mRect.w = std::max(wh.x, mRect.w);
    mRect.h = std::max(wh.y, mRect.h);
}

} // namespace AO
