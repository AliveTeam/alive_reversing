#include "stdafx.h"
#include "MainMenuTransition.hpp"
#include "Function.hpp"
#include "PsxDisplay.hpp"
#include "FixedPoint.hpp"
#include "stdlib.hpp"
#include "ScreenManager.hpp"
#include "Game.hpp"

struct MainMenu_TransitionData
{
    __int16 field_0;
    __int16 field_2;
    __int16 field_4;
    __int16 field_6;
};
ALIVE_ASSERT_SIZEOF(MainMenu_TransitionData, 0x8);

const MainMenu_TransitionData stru_55C038[24] = // 3 x 8's ?
{
    { -8,       -8,         384, 1 },
    { 0,       -32,         256, 1 },
    { 8,       -8,          384, 1 },
    { 32,       0,          256, 1 },
    { 8,        8,          384, 1 },
    { 0,        32,         256, 1 },
    { -8,       8,          384, 1 },
    { -32,      0,          256, 1 },

    { -16,      -16,        256, 1 },
    { 0,       -23,         256, 1 },
    { 16,      -16,         256, 1 },
    { 23,      0,           256, 1 },
    { 16,      16,          256, 1 },
    { 0,       23,          256, 1 },
    { -16,      16,         256, 1 },

    { -23,      0,          256, 1 },
    { -16,      -16,        256, 1 },
    { 0,        -16,        256, 1 },
    { 16,       -16,        256, 1 },
    { 16,       0,          256, 1 },
    { 16,       16,         256, 1 },
    { 0,        16,         256, 1 },
    { -16,      16,         256, 1 },
    { -16,       0,         256, 1 },
};

EXPORT int CC sub_496DD0(char /*a1*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int CC sub_496CD0(char /*a1*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

MainMenuTransition* MainMenuTransition::ctor_464110(__int16 layer, __int16 fadeDirection, __int16 bKillWhenDone, __int16 fadeSpeed, char abr)
{
    BaseGameObject_ctor_4DBFA0(1, 0);

    SetVTable(this, 0x545EA0); // vTbl_MainMenuTransition_545EA0

    field_4_typeId = Types::eMainMenuTransistion;

    gObjList_drawables_5C1124->Push_Back(this);

    field_6_flags.Set(BaseGameObject::eDrawable);

    Init_SetTPage_4F5B60(&field_22C_tPage[0], 0, 1, PSX_getTPage_4F60E0(2, abr, 0, 0));
    Init_SetTPage_4F5B60(&field_22C_tPage[1], 0, 1, PSX_getTPage_4F60E0(2, abr, 0, 0));

    for (int i = 0; i < 8; i++)
    {
        PolyG3_Init_4F8890(&field_2C_polys[0].field_0_polys[i]);
        Poly_Set_SemiTrans_4F8A60(&field_2C_polys[0].field_0_polys[i].mBase.header, 1);

        PolyG3_Init_4F8890(&field_2C_polys[1].field_0_polys[i]);
        Poly_Set_SemiTrans_4F8A60(&field_2C_polys[1].field_0_polys[i].mBase.header, 1);

    }

    field_24C_layer = layer;

    if (fadeDirection)
    {
        field_20_current_value = 0;
    }
    else
    {
        field_20_current_value = 255;
    }

    field_252_tbl_idx = 0;
    field_colour_fade_value = 0;
    field_24E_width = 320;
    field_250_k120 = 120;
    sub_464370(layer, fadeDirection, bKillWhenDone, fadeSpeed);

    return this;
}

void MainMenuTransition::sub_464370(__int16 layer, __int16 fadeDirection, __int16 bKillWhenDone, __int16 speed)
{
    field_24C_layer = layer;
    field_24_fade_direction = fadeDirection;
    field_26_bDone = 0;

    if (speed)
    {
        field_2A = 0;
    }
    else
    {
        field_2A = 1;
    }

    field_28_bKillOnDone = bKillWhenDone;

    if (fadeDirection)
    {
        field_22_change_by_speed = speed;
        SFX_Play_46FA90(7u, 0, 0x10000);
    }
    else
    {
        field_22_change_by_speed = -speed;
    }
}

void MainMenuTransition::Update_464400()
{
    if (!field_26_bDone && !field_2A)
    {
        field_20_current_value += field_22_change_by_speed;
        if (field_24_fade_direction)
        {
            if (field_20_current_value > 255)
            {
                field_colour_fade_value--;
                field_20_current_value = 255;
                return;
            }
        }
        else if (field_20_current_value < 0)
        {
            field_20_current_value = 0;
        }
        field_colour_fade_value--;
    }
}

void MainMenuTransition::Render_464470(int** ot)
{
    NOT_IMPLEMENTED(); // TODO

    int currentValue = field_20_current_value;
    int v4 = (currentValue + 1) >> 4;
    int v5 = v4 * v4 * v4 * v4 >> 8;

    int bValue = v4 * v4 * v4 * v4 >> 8;
    if (v5 > 255)
    {
        bValue = -1; // LOBYTE
        v5 = 255;
    }

    int rgValue = v5 * v5 >> 8;
    if (rgValue > 255)
    {
        rgValue = -1; // LOBYTE
    }

    int op1 = currentValue << 12;
    int val1 = sub_496CD0(field_colour_fade_value);
    int val2 = sub_496DD0(field_colour_fade_value);
    int r0g0 = -64 / ((v5 >> 2) + 1);
    for (int i = 0; i < 8; i++)
    {
        int idx = i + (8 * this->field_252_tbl_idx);
        int v8 = stru_55C038[idx].field_4 << 8;
        int v9 = stru_55C038[idx].field_2 << 16;
        int v10 = (signed __int16)stru_55C038[idx].field_0 << 16;
        int v11 = Math_FixedPoint_Multiply_496C50(v9, val1);
        int v12 = Math_FixedPoint_Multiply_496C50(v10, val2) - v11;
        int v13 = Math_FixedPoint_Multiply_496C50(op1, v8);
        __int16 x0 = this->field_24E_width + 640 * ((signed int)Math_FixedPoint_Multiply_496C50(v12, v13) >> 16) / 368;
        int v14 = Math_FixedPoint_Multiply_496C50(v9, val2);
        int v15 = Math_FixedPoint_Multiply_496C50(v10, val1) + v14;
        int v16 = Math_FixedPoint_Multiply_496C50(op1, v8);
        __int16 y0 = this->field_250_k120 + (Math_FixedPoint_Multiply_496C50(v15, v16) >> 16);

        int v17 = 0;
        if (i < 7)
        {
            v17 = i + 1;
        }
        else
        {
            v17 = 0;
        }

        int idx2 = (8 * this->field_252_tbl_idx);
        int v36 = (signed __int16)stru_55C038[idx2 + v17].field_0 << 16;
        int v19 = 0;
        if (i < 7)
        {
            v19 = i + 1;
        }
        else
        {
            v19 = 0;
        }

        int v20 = stru_55C038[idx2 + v19].field_2 << 16;
        int v38 = v20;
        int v21 = 0;
        if (i < 7)
        {
            v21 = i + 1;
        }
        else
        {
            v21 = 0;
        }
        int y1 = stru_55C038[v21 + idx2].field_4 << 8;

        int v23 = Math_FixedPoint_Multiply_496C50(v20, val1);
        int x1 = Math_FixedPoint_Multiply_496C50(v36, val2) - v23;
        int v25 = Math_FixedPoint_Multiply_496C50(op1, y1);
        x1 = this->field_24E_width + 40 * ((signed int)Math_FixedPoint_Multiply_496C50(x1, v25) >> 16) / 23; // LOWORD
        int v26 = Math_FixedPoint_Multiply_496C50(v38, val2);
        int v27 = v26 + Math_FixedPoint_Multiply_496C50(v36, val1);
        int v28 = Math_FixedPoint_Multiply_496C50(op1, y1);
        y1 = this->field_250_k120 + (Math_FixedPoint_Multiply_496C50(v27, v28) >> 16); // LOWORD
        Poly_G3* pPoly = &field_2C_polys[gPsxDisplay_5C1130.field_C_buffer_index].field_0_polys[i];

        SetRGB0(pPoly, static_cast<BYTE>(r0g0), static_cast<BYTE>(r0g0), 255);
        SetRGB1(pPoly, static_cast<BYTE>(rgValue), static_cast<BYTE>(rgValue), static_cast<BYTE>(bValue));
        SetRGB2(pPoly, static_cast<BYTE>(rgValue), static_cast<BYTE>(rgValue), static_cast<BYTE>(bValue));

        SetXY0(pPoly, field_24E_width, field_250_k120);
        SetXY1(pPoly, x0, y0);
        SetXY2(pPoly, static_cast<short>(x1), static_cast<short>(y1));

        OrderingTable_Add_4F8AA0(&ot[field_24C_layer], &pPoly->mBase.header);
    }

    OrderingTable_Add_4F8AA0(&ot[field_24C_layer], &field_22C_tPage[gPsxDisplay_5C1130.field_C_buffer_index].mBase);

    pScreenManager_5BB5F4->InvalidateRect_40EC10(0, 0, gPsxDisplay_5C1130.field_0_width, gPsxDisplay_5C1130.field_2_height);

    if ((field_20_current_value == 255 && field_24_fade_direction) || (field_20_current_value == 0 && !field_24_fade_direction))
    {
        field_26_bDone = 1;
        if (field_28_bKillOnDone)
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }
    }
}

void MainMenuTransition::dtor_4642F0()
{
    SetVTable(this, 0x545EA0); // vTbl_MainMenuTransition_545EA0
    gObjList_drawables_5C1124->Remove_Item(this);
    BaseGameObject_dtor_4DBEC0();
}

void MainMenuTransition::vdtor_4642C0(signed int flags)
{
    dtor_4642F0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
}

void MainMenuTransition::VRender(int** ot)
{
    Render_464470(ot);
}

void MainMenuTransition::VUpdate()
{
    Update_464400();
}

void MainMenuTransition::VDestructor(signed int flags)
{
    vdtor_4642C0(flags);
}
