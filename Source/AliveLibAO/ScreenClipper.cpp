#include "stdafx_ao.h"
#include "Function.hpp"
#include "ScreenClipper.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "PsxDisplay.hpp"
#include "../AliveLibAE/Primitives.hpp"

#undef min
#undef max

namespace AO {

ScreenClipper::~ScreenClipper()
{
    gObjListDrawables->Remove_Item(this);
}

void ScreenClipper::VRender(PrimHeader** ppOt)
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
    OrderingTable_Add(OtLayer(ppOt, field_38_ot_layer), &pClipper->mBase);
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
    : BaseGameObject(TRUE, 0)
{
    mBaseGameObjectTypeId = ReliveTypes::eScreenClipper;

    mBaseGameObjectFlags.Set(Options::eUpdateDuringCamSwap_Bit10);
    mBaseGameObjectFlags.Set(Options::eDrawable_Bit4);

    field_30_rect.x = xy.x;
    field_30_rect.y = xy.y;

    field_30_rect.w = wh.x;
    field_30_rect.h = wh.y;

    field_38_ot_layer = layer;

    gObjListDrawables->Push_Back(this);
}

void ScreenClipper::Update_Clip_Rect(PSX_Point xy, PSX_Point wh)
{
    field_30_rect.x = std::min(xy.x, field_30_rect.x);
    field_30_rect.y = std::min(xy.y, field_30_rect.y);

    field_30_rect.w = std::max(wh.x, field_30_rect.w);
    field_30_rect.h = std::max(wh.y, field_30_rect.h);
}

} // namespace AO
