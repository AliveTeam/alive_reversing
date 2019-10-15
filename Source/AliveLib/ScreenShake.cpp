#include "stdafx.h"
#include "ScreenShake.hpp"
#include "Function.hpp"
#include "Events.hpp"
#include "stdlib.hpp"
#include "PsxDisplay.hpp"
#include "ScreenManager.hpp"
#include "PsxRender.hpp"
#include "Game.hpp"

EXPORT ScreenShake* ScreenShake::ctor_4ACF70(__int16 a2, __int16 a3)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    field_6_flags.Set(BaseGameObject::eDrawable);

    SetVTable(this, 0x547070); // vTbl_ScreenShake_547070

    field_4_typeId = Types::eScreenShake_118;
    field_44 = a3;
    field_40 = 16;
    field_42 = a2;

    gObjList_drawables_5C1124->Push_Back(this);

    if (!field_42)
    {
        return this;
    }

    if (!field_44)
    {
        Event_Broadcast_422BC0(kEventScreenShake, this);
    }

    return this;
}

EXPORT void ScreenShake::dtor_4AD060()
{
    SetVTable(this, 0x547070); // vTbl_ScreenShake_547070

    gObjList_drawables_5C1124->Remove_Item(this);
    BaseGameObject_dtor_4DBEC0();
}

EXPORT void ScreenShake::vUpdate_4AD0E0()
{
    if (field_42)
    {
        if (!field_44)
        {
            Event_Broadcast_422BC0(kEventScreenShake, this);
        }
    }

    if (field_40 > 0)
    {
        field_40--;
    }
}

EXPORT BaseGameObject* ScreenShake::vdtor_4AD030(signed int flags)
{
    dtor_4AD060();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

struct ScreenOffset
{
    signed char field_0_x;
    signed char field_1_y;
};

const ScreenOffset sShakeOffsets_560388[16] =
{
    { 0, -1 },
    { -1, 0 },
    { 1, -1 },
    { -1, 1 },
    { 0, -2 },
    { -2, 0 },
    { 2, -2 },
    { -2, 2 },
    { 0, -3 },
    { -3, 0 },
    { 3, -3 },
    { -3, 3 },
    { 0, -9 },
    { -9, 0 },
    { 9, -9 },
    { -9, 9 },
};

EXPORT void ScreenShake::vRender_4AD120(int** pOt)
{
    Prim_ScreenOffset* pPrim = &field_20[gPsxDisplay_5C1130.field_C_buffer_index];
    if (field_40 < 14)
    {
        short xoff = 0;
        short yoff = 0;

        if (field_44)
        {
            xoff = sShakeOffsets_560388[field_40].field_0_x / 2;
            yoff = sShakeOffsets_560388[field_40].field_1_y / 2;
        }
        else
        {
            xoff = sShakeOffsets_560388[field_40].field_0_x;
            yoff = sShakeOffsets_560388[field_40].field_1_y;
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
        OrderingTable_Add_4F8AA0(pOt, &pPrim->mBase);

        // Always true
        //if (dword_55EF94)
        {

            if (offset.y > 0)
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
                clearRect.w = 640;
                PSX_ClearImage_4F5BD0(&clearRect, 0, 0, 0);
            }

            if (offset.x > 0)
            {
                PSX_RECT clearRect = {};
                if (offset.x < 0)
                {
                    clearRect.x = offset.x + 640;
                    clearRect.w = -offset.x;
                }
                else if (offset.x > 0)
                {
                    clearRect.y = 0;
                    clearRect.h = gPsxDisplay_5C1130.field_2_height;
                }

                clearRect.x = 0;
                clearRect.w = offset.x;
                PSX_ClearImage_4F5BD0(&clearRect, 0, 0, 0);
            }
        }

        if (!field_40)
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }
    }

    // Always true
    //if (!dword_55EF94)
    {
        pScreenManager_5BB5F4->InvalidateRect_40EC10(0, 0, 640, 240);
    }

}

BaseGameObject* ScreenShake::VDestructor(signed int flags)
{
    return vdtor_4AD030(flags);
}

void ScreenShake::VUpdate()
{
    vUpdate_4AD0E0();
}

void ScreenShake::VRender(int** pOt)
{
    vRender_4AD120(pOt);
}
