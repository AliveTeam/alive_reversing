#include "stdafx.h"
#include "ScreenShake.hpp"
#include "Function.hpp"
#include "Events.hpp"
#include "stdlib.hpp"
#include "PsxDisplay.hpp"
#include "ScreenManager.hpp"
#include "PsxRender.hpp"
#include "Game.hpp"

 ScreenShake::ScreenShake(bool enableShakeEvent, bool softerShakes)
     : BaseGameObject(TRUE, 0)
 {
    SetType(AETypes::eScreenShake_118);

    mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);

    field_44_softerShakes = softerShakes;
    field_40_shakeNumber = 16;
    field_42_enableShakeEvent = enableShakeEvent;

    gObjList_drawables_5C1124->Push_Back(this);

    if (field_42_enableShakeEvent && !field_44_softerShakes)
    {
        Event_Broadcast(kEventScreenShake, this);
    }
}

ScreenShake::~ScreenShake()
{
    gObjList_drawables_5C1124->Remove_Item(this);
}

void ScreenShake::VUpdate()
{
    if (field_42_enableShakeEvent && !field_44_softerShakes)
    {
        Event_Broadcast(kEventScreenShake, this);
    }

    if (field_40_shakeNumber > 0)
    {
        field_40_shakeNumber--;
    }
}

struct ScreenOffset final
{
    s8 field_0_x;
    s8 field_1_y;
};

const ScreenOffset sShakeOffsets_560388[16] = {
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

void ScreenShake::VRender(PrimHeader** ppOt)
{
    Prim_ScreenOffset* pPrim = &field_20_screenOffset[gPsxDisplay_5C1130.field_C_buffer_index];
    if (field_40_shakeNumber < 14)
    {
        s16 xoff = 0;
        s16 yoff = 0;

        if (field_44_softerShakes)
        {
            xoff = sShakeOffsets_560388[field_40_shakeNumber].field_0_x / 2;
            yoff = sShakeOffsets_560388[field_40_shakeNumber].field_1_y / 2;
        }
        else
        {
            xoff = sShakeOffsets_560388[field_40_shakeNumber].field_0_x;
            yoff = sShakeOffsets_560388[field_40_shakeNumber].field_1_y;
        }

        PSX_Pos16 offset = {};
        offset.x = PsxToPCX(xoff); // TODO + 11 ?
        if (gPsxDisplay_5C1130.field_C_buffer_index)
        {
            offset.y = yoff + 256;
        }
        else
        {
            offset.y = yoff;
        }

        InitType_ScreenOffset_4F5BB0(pPrim, &offset);
        OrderingTable_Add_4F8AA0(OtLayer(ppOt, Layer::eLayer_0), &pPrim->mBase);

        if (offset.y != 0)
        {
            PSX_RECT clearRect = {};
            if (offset.y < 0)
            {
                clearRect.y = offset.y + gPsxDisplay_5C1130.field_2_height;
                clearRect.h = -offset.y;
            }
            else if (offset.y > 0)
            {
                clearRect.y = 0;
                clearRect.h = offset.y;
            }

            clearRect.x = 0;
            clearRect.w = 640; // Could probably replace with `gPsxDisplay_5C1130.field_0_width`
            PSX_ClearImage_4F5BD0(&clearRect, 0, 0, 0);
        }

        if (offset.x != 0)
        {
            PSX_RECT clearRect = {};
            if (offset.x < 0)
            {
                clearRect.x = offset.x + 640; // Could probably replace with `gPsxDisplay_5C1130.field_0_width`
                clearRect.w = -offset.x;
            }
            else if (offset.x > 0)
            {
                clearRect.x = 0;
                clearRect.w = offset.x;
            }

            clearRect.y = 0;
            clearRect.h = gPsxDisplay_5C1130.field_2_height;
            PSX_ClearImage_4F5BD0(&clearRect, 0, 0, 0);
        }

        if (!field_40_shakeNumber)
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }

    pScreenManager_5BB5F4->InvalidateRect_40EC10(0, 0, 640, 240);
}
