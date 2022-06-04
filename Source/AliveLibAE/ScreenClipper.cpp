#include "stdafx.h"
#include "ScreenClipper.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "PsxDisplay.hpp"

ScreenClipper::ScreenClipper(PSX_Point xy, PSX_Point wh, Layer layer)
    : BaseGameObject(TRUE, 0)
{
    SetType(AETypes::eScreenClipper_114);

    mBaseGameObjectFlags.Set(BaseGameObject::eUpdateDuringCamSwap_Bit10);
    mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);

    field_40_rect.x = xy.field_0_x;
    field_40_rect.y = xy.field_2_y;
    field_40_rect.w = wh.field_0_x;
    field_40_rect.h = wh.field_2_y;

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

    if (gPsxDisplay_5C1130.field_C_buffer_index)
    {
        // Move to the lower buffer if not using the top buffer
        clipRect.y += 256;
    }

    Prim_PrimClipper* pClipper = &field_20_clippers[gPsxDisplay_5C1130.field_C_buffer_index];
    Init_PrimClipper_4F5B80(pClipper, &clipRect);
    OrderingTable_Add_4F8AA0(OtLayer(ot, field_48_ot_layer), &pClipper->mBase);
}

ScreenClipper::~ScreenClipper()
{
    gObjListDrawables->Remove_Item(this);
}

void ScreenClipper::Update_Clip_Rect(PSX_Point xy, PSX_Point wh)
{
    field_40_rect.x = std::min(xy.field_0_x, field_40_rect.x);
    field_40_rect.y = std::min(xy.field_2_y, field_40_rect.y);

    field_40_rect.w = std::max(wh.field_0_x, field_40_rect.w);
    field_40_rect.h = std::max(wh.field_2_y, field_40_rect.h);
}
