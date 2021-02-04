#include "stdafx.h"
#include "ScreenClipper.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "PsxDisplay.hpp"

ScreenClipper* ScreenClipper::ctor_416D60(PSX_Point xy, PSX_Point wh, Layer layer)
{
    BaseGameObject_ctor_4DBFA0(1, 0);

    SetVTable(this, 0x5445C4);
    field_4_typeId = Types::eScreenClipper_114;

    field_6_flags.Set(BaseGameObject::eUpdateDuringCamSwap_Bit10);
    field_6_flags.Set(BaseGameObject::eDrawable_Bit4);

    field_40_rect.x = xy.field_0_x;
    field_40_rect.y = xy.field_2_y;
    field_40_rect.w = wh.field_0_x;
    field_40_rect.h = wh.field_2_y;

    field_48_ot_layer = layer;

    gObjList_drawables_5C1124->Push_Back(this);

    // This actually matters here due how its called
    return this;
}

BaseGameObject* ScreenClipper::VDestructor(signed int flags)
{
    return vdtor_416E00(flags);
}

void ScreenClipper::VRender(PrimHeader** ppOt)
{
    vRender_416F70(ppOt);
}

void ScreenClipper::VScreenChanged()
{
    // Empty
}

void ScreenClipper::VUpdate()
{
    // Empty
}

void ScreenClipper::vRender_416F70(PrimHeader** ot)
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

void ScreenClipper::dtor_416E30()
{
    SetVTable(this, 0x5445C4);
    gObjList_drawables_5C1124->Remove_Item(this);
    BaseGameObject_dtor_4DBEC0();
}

BaseGameObject* ScreenClipper::vdtor_416E00(signed int flags)
{
    dtor_416E30();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void ScreenClipper::Update_Clip_Rect_416EB0(PSX_Point xy, PSX_Point wh)
{
    field_40_rect.x = std::min(xy.field_0_x, field_40_rect.x);
    field_40_rect.y = std::min(xy.field_2_y, field_40_rect.y);

    field_40_rect.w = std::max(wh.field_0_x, field_40_rect.w);
    field_40_rect.h = std::max(wh.field_2_y, field_40_rect.h);
}
