#include "stdafx.h"
#include "ScreenClipper.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "../relive_lib/PsxDisplay.hpp"

ScreenClipper::ScreenClipper(PSX_Point xy, PSX_Point wh, Layer layer)
    : BaseGameObject(TRUE, 0)
{
    SetType(ReliveTypes::eScreenClipper);

    mBaseGameObjectFlags.Set(BaseGameObject::eUpdateDuringCamSwap_Bit10);
    mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);

    mRect.x = xy.x;
    mRect.y = xy.y;
    mRect.w = wh.x;
    mRect.h = wh.y;

    mOtLayer = layer;

    gObjListDrawables->Push_Back(this);
}

void ScreenClipper::VScreenChanged()
{
    // Empty
}

void ScreenClipper::VUpdate()
{
    // Empty
}

void ScreenClipper::VRender(PrimHeader** ot)
{
    PSX_RECT clipRect = {};

    clipRect.x = mRect.x;
    clipRect.y = mRect.y;
    clipRect.w = mRect.w - mRect.x;
    clipRect.h = mRect.h - mRect.y;

    if (gPsxDisplay.mBufferIndex)
    {
        // Move to the lower buffer if not using the top buffer
        clipRect.y += 256;
    }

    Prim_PrimClipper* pClipper = &mClippers[gPsxDisplay.mBufferIndex];
    Init_PrimClipper(pClipper, &clipRect);
    OrderingTable_Add(OtLayer(ot, mOtLayer), &pClipper->mBase);
}

ScreenClipper::~ScreenClipper()
{
    gObjListDrawables->Remove_Item(this);
}

void ScreenClipper::Update_Clip_Rect(PSX_Point xy, PSX_Point wh)
{
    mRect.x = std::min(xy.x, mRect.x);
    mRect.y = std::min(xy.y, mRect.y);

    mRect.w = std::max(wh.x, mRect.w);
    mRect.h = std::max(wh.y, mRect.h);
}
