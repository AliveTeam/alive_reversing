#include "stdafx.h"
#include "ScreenClipper.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "PsxDisplay.hpp"

ScreenClipper::ScreenClipper(PSX_Point xy, PSX_Point wh, Layer layer)
    : BaseGameObject(TRUE, 0)
{
    SetType(ReliveTypes::eScreenClipper);

    mBaseGameObjectFlags.Set(BaseGameObject::eUpdateDuringCamSwap_Bit10);
    mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);

    field_40_rect.x = xy.x;
    field_40_rect.y = xy.y;
    field_40_rect.w = wh.x;
    field_40_rect.h = wh.y;

    field_48_ot_layer = layer;

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

    clipRect.x = field_40_rect.x;
    clipRect.y = field_40_rect.y;
    clipRect.w = field_40_rect.w - field_40_rect.x;
    clipRect.h = field_40_rect.h - field_40_rect.y;

    if (gPsxDisplay.mBufferIndex)
    {
        // Move to the lower buffer if not using the top buffer
        clipRect.y += 256;
    }

    Prim_PrimClipper* pClipper = &field_20_clippers[gPsxDisplay.mBufferIndex];
    Init_PrimClipper(pClipper, &clipRect);
    OrderingTable_Add(OtLayer(ot, field_48_ot_layer), &pClipper->mBase);
}

ScreenClipper::~ScreenClipper()
{
    gObjListDrawables->Remove_Item(this);
}

void ScreenClipper::Update_Clip_Rect(PSX_Point xy, PSX_Point wh)
{
    field_40_rect.x = std::min(xy.x, field_40_rect.x);
    field_40_rect.y = std::min(xy.y, field_40_rect.y);

    field_40_rect.w = std::max(wh.x, field_40_rect.w);
    field_40_rect.h = std::max(wh.y, field_40_rect.h);
}
