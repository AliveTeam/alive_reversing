#include "stdafx_ao.h"
#include "Function.hpp"
#include "ScreenShake.hpp"
#include "Events.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "PsxDisplay.hpp"
#include "Primitives.hpp"
#include "ScreenManager.hpp"
#include "PsxRender.hpp"

namespace AO {

ScreenShake* ScreenShake::ctor_4624D0(__int16 enableShakeEvent)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BC950);

    field_4_typeId = Types::ScreenShake_82;

    field_6_flags.Set(Options::eDrawable_Bit4);

    field_30_shakeNumber = 16;
    field_32_enableShakeEvent = enableShakeEvent;

    gObjList_drawables_504618->Push_Back(this);

    if (field_32_enableShakeEvent)
    {
        Event_Broadcast_417220(kEventScreenShake_19, this);
    }

    return this;
}

void ScreenShake::VUpdate()
{
    VUpdate_4625B0();
}

void ScreenShake::VUpdate_4625B0()
{
    if (field_32_enableShakeEvent)
    {
        Event_Broadcast_417220(kEventScreenShake_19, this);
    }

    if (field_30_shakeNumber > 0)
    {
        field_30_shakeNumber--;
    }
}

BaseGameObject* ScreenShake::VDestructor(signed int flags)
{
    return Vdtor_462770(flags);
}

ScreenShake* ScreenShake::Vdtor_462770(signed int flags)
{
    dtor_462550();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}


void ScreenShake::VRender(PrimHeader** ppOt)
{
    VRender_4625E0(ppOt);
}

const FP_Point stru_4CF850[16] =
{
    { FP_FromInteger(0),  FP_FromInteger(-1) },
    { FP_FromInteger(-1), FP_FromInteger(0)  },
    { FP_FromInteger(1),  FP_FromInteger(-1) },
    { FP_FromInteger(-1), FP_FromInteger(1)  },
    { FP_FromInteger(0),  FP_FromInteger(-2) },
    { FP_FromInteger(-2), FP_FromInteger(0)  },
    { FP_FromInteger(2),  FP_FromInteger(-2) },
    { FP_FromInteger(-2), FP_FromInteger(2)  },
    { FP_FromInteger(0),  FP_FromInteger(-3) },
    { FP_FromInteger(-3), FP_FromInteger(0)  },
    { FP_FromInteger(3),  FP_FromInteger(-3) },
    { FP_FromInteger(-3), FP_FromInteger(3)  },
    { FP_FromInteger(0),  FP_FromInteger(-9) },
    { FP_FromInteger(-9), FP_FromInteger(0)  },
    { FP_FromInteger(9),  FP_FromInteger(-9) },
    { FP_FromInteger(-9), FP_FromInteger(9)  }
};


void ScreenShake::VRender_4625E0(PrimHeader** ppOt)
{
    if (field_30_shakeNumber < 14)
    {
        Prim_ScreenOffset* pPrim = &field_10_screenOffset[gPsxDisplay_504C78.field_A_buffer_index];

        short xoff = 0;
        short yoff = 0;
        xoff = FP_GetExponent(stru_4CF850[field_30_shakeNumber].field_0_x); // TODO: Div 16 ??
        yoff = FP_GetExponent(stru_4CF850[field_30_shakeNumber].field_4_y);

        if (gPsxDisplay_504C78.field_A_buffer_index)
        {
            yoff += gPsxDisplay_504C78.field_2_height;
        }

        PSX_Pos16 offset = {};
        offset.x = PsxToPCX(xoff); // TODO + 11 ?
        if (gPsxDisplay_504C78.field_A_buffer_index)
        {
            offset.y = yoff + gPsxDisplay_504C78.field_2_height;
        }
        else
        {
            offset.y = yoff;
        }

        InitType_ScreenOffset_496000(pPrim, &offset);
        OrderingTable_Add_498A80(ppOt, &pPrim->mBase);

        if (offset.y != 0)
        {
            PSX_RECT clearRect = {};
            if (offset.y < 0)
            {
                clearRect.y = offset.y + gPsxDisplay_504C78.field_2_height;
                clearRect.h = -offset.y;
            }
            else if (offset.y > 0)
            {
                clearRect.y = 0;
                clearRect.h = offset.y;
            }

            clearRect.x = 0;
            clearRect.w = 640; // Could probably replace with `gPsxDisplay_5C1130.field_0_width`
            PSX_ClearImage_496020(&clearRect, 0, 0, 0);
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
            clearRect.h = gPsxDisplay_504C78.field_2_height;
            PSX_ClearImage_496020(&clearRect, 0, 0, 0);
        }

        if (!field_30_shakeNumber)
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }


    pScreenManager_4FF7C8->InvalidateRect_406CC0(0, 0, 640, 240);
}

BaseGameObject* ScreenShake::dtor_462550()
{
    SetVTable(this, 0x4BC950);
    gObjList_drawables_504618->Remove_Item(this);
    return dtor_487DF0();
}

}
