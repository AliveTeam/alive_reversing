#include "stdafx_ao.h"
#include "Function.hpp"
#include "ScreenClipper.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "PsxDisplay.hpp"

#undef min
#undef max

namespace AO {

ScreenClipper::~ScreenClipper()
{
    gObjList_drawables_504618->Remove_Item(this);
}

void ScreenClipper::VRender_40BEB0(PrimHeader** ppOt)
{
    PSX_RECT clipRect = {};

    clipRect.x = field_30_rect.x;
    clipRect.y = field_30_rect.y;
    clipRect.w = field_30_rect.w - field_30_rect.x;
    clipRect.h = field_30_rect.h - field_30_rect.y;

    if (gPsxDisplay_504C78.field_A_buffer_index)
    {
        // Move to the lower buffer if not using the top buffer
        clipRect.y += gPsxDisplay_504C78.field_2_height;
    }

    Prim_PrimClipper* pClipper = &field_10_clippers[gPsxDisplay_504C78.field_A_buffer_index];
    Init_PrimClipper_495FD0(pClipper, &clipRect);
    OrderingTable_Add_498A80(OtLayer(ppOt, field_38_ot_layer), &pClipper->mBase);
}

void ScreenClipper::VUpdate()
{
    // Empty
}

void ScreenClipper::VScreenChanged()
{
    // Empty
}

void ScreenClipper::VRender(PrimHeader** ppOt)
{
    VRender_40BEB0(ppOt);
}

ScreenClipper::ScreenClipper(PSX_Point xy, PSX_Point wh, Layer layer)
    : BaseGameObject(1)
{
    field_4_typeId = Types::eScreenClipper_78;

    mFlags.Set(Options::eUpdateDuringCamSwap_Bit10);
    mFlags.Set(Options::eDrawable_Bit4);

    field_30_rect.x = xy.field_0_x;
    field_30_rect.y = xy.field_2_y;

    field_30_rect.w = wh.field_0_x;
    field_30_rect.h = wh.field_2_y;

    field_38_ot_layer = layer;

    gObjList_drawables_504618->Push_Back(this);
}

void ScreenClipper::Update_Clip_Rect_40BE40(PSX_Point xy, PSX_Point wh)
{
    field_30_rect.x = std::min(xy.field_0_x, field_30_rect.x);
    field_30_rect.y = std::min(xy.field_2_y, field_30_rect.y);

    field_30_rect.w = std::max(wh.field_0_x, field_30_rect.w);
    field_30_rect.h = std::max(wh.field_2_y, field_30_rect.h);
}

} // namespace AO
