#include "stdafx.h"
#include "MainMenuTransition.hpp"
#include "Function.hpp"
#include "PsxDisplay.hpp"
#include "FixedPoint.hpp"
#include "stdlib.hpp"
#include "ScreenManager.hpp"
#include "Game.hpp"
#include "Map.hpp"

struct MainMenu_TransitionData final
{
    s16 field_0;
    s16 field_2;
    s16 field_4;
    s16 field_6;
};
ALIVE_ASSERT_SIZEOF(MainMenu_TransitionData, 0x8);

const MainMenu_TransitionData stru_55C038[24] = // 3 x 8's ?
    {
        {-8, -8, 384, 1},
        {0, -32, 256, 1},
        {8, -8, 384, 1},
        {32, 0, 256, 1},
        {8, 8, 384, 1},
        {0, 32, 256, 1},
        {-8, 8, 384, 1},
        {-32, 0, 256, 1},

        {-16, -16, 256, 1},
        {0, -23, 256, 1},
        {16, -16, 256, 1},
        {23, 0, 256, 1},
        {16, 16, 256, 1},
        {0, 23, 256, 1},
        {-16, 16, 256, 1},

        {-23, 0, 256, 1},
        {-16, -16, 256, 1},
        {0, -16, 256, 1},
        {16, -16, 256, 1},
        {16, 0, 256, 1},
        {16, 16, 256, 1},
        {0, 16, 256, 1},
        {-16, 16, 256, 1},
        {-16, 0, 256, 1},
};

MainMenuTransition* MainMenuTransition::ctor_464110(Layer layer, s16 fadeDirection, s16 bKillWhenDone, s16 fadeSpeed, TPageAbr abr)
{
    BaseGameObject_ctor_4DBFA0(1, 0);

    SetVTable(this, 0x545EA0); // vTbl_MainMenuTransition_545EA0

    SetType(AETypes::eMainMenuTransistion_116);

    gObjList_drawables_5C1124->Push_Back(this);

    field_6_flags.Set(BaseGameObject::eDrawable_Bit4);

    Init_SetTPage_4F5B60(&field_22C_tPage[0], 0, 1, PSX_getTPage_4F60E0(TPageMode::e16Bit_2, abr, 0, 0));
    Init_SetTPage_4F5B60(&field_22C_tPage[1], 0, 1, PSX_getTPage_4F60E0(TPageMode::e16Bit_2, abr, 0, 0));

    for (s32 i = 0; i < 8; i++)
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
    StartTrans_464370(layer, fadeDirection, bKillWhenDone, fadeSpeed);

    return this;
}

void MainMenuTransition::StartTrans_464370(Layer layer, s16 fadeDirection, s16 bKillWhenDone, s16 speed)
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
        SFX_Play_46FA90(SoundEffect::MenuTransition_7, 0);
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

void MainMenuTransition::Render_464470(PrimHeader** ppOt)
{
    // TODO: The fixed point math/var needs cleaning up/refactoring in here

    s32 currentValue = field_20_current_value;
    s32 v4 = (currentValue + 1) >> 4;
    s32 v5 = v4 * v4 * v4 * v4 >> 8;

    s32 bValue = v4 * v4 * v4 * v4 >> 8;
    if (v5 > 255)
    {
        bValue = -1; // LOBYTE
        v5 = 255;
    }

    s32 rgValue = v5 * v5 >> 8;
    if (rgValue > 255)
    {
        rgValue = -1; // LOBYTE
    }

    s32 op1 = currentValue << 12;
    s32 val1 = Math_Cosine_496CD0(field_colour_fade_value).fpValue;
    s32 val2 = Math_Sine_496DD0(field_colour_fade_value).fpValue;
    s32 r0g0 = -64 / ((v5 >> 2) + 1);
    for (s32 i = 0; i < 8; i++)
    {
        s32 idx = i + (8 * this->field_252_tbl_idx);
        s32 v8 = stru_55C038[idx].field_4 << 8;
        s32 v9 = stru_55C038[idx].field_2 << 16;
        s32 v10 = (s16) stru_55C038[idx].field_0 << 16;
        s32 v11 = Math_FixedPoint_Multiply_496C50(v9, val1);
        s32 v12 = Math_FixedPoint_Multiply_496C50(v10, val2) - v11;
        s32 v13 = Math_FixedPoint_Multiply_496C50(op1, v8);
        s16 x0 = this->field_24E_width + 640 * ((s32) Math_FixedPoint_Multiply_496C50(v12, v13) >> 16) / 368;
        s32 v14 = Math_FixedPoint_Multiply_496C50(v9, val2);
        s32 v15 = Math_FixedPoint_Multiply_496C50(v10, val1) + v14;
        s32 v16 = Math_FixedPoint_Multiply_496C50(op1, v8);
        s16 y0 = this->field_250_k120 + (Math_FixedPoint_Multiply_496C50(v15, v16) >> 16);

        s32 v17 = 0;
        if (i < 7)
        {
            v17 = i + 1;
        }
        else
        {
            v17 = 0;
        }

        s32 idx2 = (8 * this->field_252_tbl_idx);
        s32 v36 = (s16) stru_55C038[idx2 + v17].field_0 << 16;
        s32 v19 = 0;
        if (i < 7)
        {
            v19 = i + 1;
        }
        else
        {
            v19 = 0;
        }

        s32 v20 = stru_55C038[idx2 + v19].field_2 << 16;
        s32 v38 = v20;
        s32 v21 = 0;
        if (i < 7)
        {
            v21 = i + 1;
        }
        else
        {
            v21 = 0;
        }
        s32 y1 = stru_55C038[v21 + idx2].field_4 << 8;

        s32 v23 = Math_FixedPoint_Multiply_496C50(v20, val1);
        s32 x1 = Math_FixedPoint_Multiply_496C50(v36, val2) - v23;
        s32 v25 = Math_FixedPoint_Multiply_496C50(op1, y1);
        // TODO: Use PsxToPCX
        x1 = this->field_24E_width + 40 * ((s32) Math_FixedPoint_Multiply_496C50(x1, v25) >> 16) / 23; // LOWORD
        s32 v26 = Math_FixedPoint_Multiply_496C50(v38, val2);
        s32 v27 = v26 + Math_FixedPoint_Multiply_496C50(v36, val1);
        s32 v28 = Math_FixedPoint_Multiply_496C50(op1, y1);
        y1 = this->field_250_k120 + (Math_FixedPoint_Multiply_496C50(v27, v28) >> 16); // LOWORD
        Poly_G3* pPoly = &field_2C_polys[gPsxDisplay_5C1130.field_C_buffer_index].field_0_polys[i];

        SetRGB0(pPoly, static_cast<u8>(r0g0), static_cast<u8>(r0g0), 255);
        SetRGB1(pPoly, static_cast<u8>(rgValue), static_cast<u8>(rgValue), static_cast<u8>(bValue));
        SetRGB2(pPoly, static_cast<u8>(rgValue), static_cast<u8>(rgValue), static_cast<u8>(bValue));

        SetXY0(pPoly, field_24E_width, field_250_k120);
        SetXY1(pPoly, x0, y0);
        SetXY2(pPoly, static_cast<s16>(x1), static_cast<s16>(y1));

        OrderingTable_Add_4F8AA0(OtLayer(ppOt, field_24C_layer), &pPoly->mBase.header);
    }

    OrderingTable_Add_4F8AA0(OtLayer(ppOt, field_24C_layer), &field_22C_tPage[gPsxDisplay_5C1130.field_C_buffer_index].mBase);

    pScreenManager_5BB5F4->InvalidateRect_40EC10(0, 0, gPsxDisplay_5C1130.field_0_width, gPsxDisplay_5C1130.field_2_height);

    if ((field_20_current_value == 255 && field_24_fade_direction) || (field_20_current_value == 0 && !field_24_fade_direction))
    {
        field_26_bDone = 1;
        if (field_28_bKillOnDone)
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }
}

void MainMenuTransition::dtor_4642F0()
{
    SetVTable(this, 0x545EA0); // vTbl_MainMenuTransition_545EA0
    gObjList_drawables_5C1124->Remove_Item(this);
    BaseGameObject_dtor_4DBEC0();
}

BaseGameObject* MainMenuTransition::vdtor_4642C0(s32 flags)
{
    dtor_4642F0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void MainMenuTransition::vScreenChanged_4648D0()
{
    if (gMap_5C3030.field_0_current_level != gMap_5C3030.field_A_level || gMap_5C3030.field_2_current_path != gMap_5C3030.field_C_path)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void MainMenuTransition::VRender(PrimHeader** ppOt)
{
    Render_464470(ppOt);
}

void MainMenuTransition::VUpdate()
{
    Update_464400();
}

BaseGameObject* MainMenuTransition::VDestructor(s32 flags)
{
    return vdtor_4642C0(flags);
}

void MainMenuTransition::VScreenChanged()
{
    vScreenChanged_4648D0();
}
