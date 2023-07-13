#include "stdafx_ao.h"
#include "MainMenuTransition.hpp"
#include "Map.hpp"
#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"
#include "Sfx.hpp"
#include "Math.hpp"
#include "../relive_lib/PsxDisplay.hpp"

namespace AO
{

struct MainMenu_TransitionData final
{
    u16 field_0;
    u16 field_2;
    u16 field_4;
    s16 field_6;
};
ALIVE_ASSERT_SIZEOF(MainMenu_TransitionData, 0x8);

const MainMenu_TransitionData stru_55C038[24] = // 3 x 8's ?
{
    {65528U, 65528U, 384, 1},
    {0, 65504U, 256, 1},
    {8, 65528U, 384, 1},
    {32, 0, 256, 1},
    {8, 8, 384, 1},
    {0, 32, 256, 1},
    {65528U, 8, 384, 1},
    {65504U, 0, 256, 1},

    {65520U, 65520U, 256, 1},
    {0, 65513U, 256, 1},
    {16, 65520U, 256, 1},
    {23, 0, 256, 1},
    {16, 16, 256, 1},
    {0, 23, 256, 1},
    {65520U, 16, 256, 1},

    {65513U, 0, 256, 1},
    {65520U, 65520U, 256, 1},
    {0, 65520U, 256, 1},
    {16, 65520U, 256, 1},
    {16, 0, 256, 1},
    {16, 16, 256, 1},
    {0, 16, 256, 1},
    {65520U, 16, 256, 1},
    {65520U, 0, 256, 1},
};

void MainMenuTransition::VScreenChanged()
{
    if (gMap.LevelChanged() || gMap.PathChanged())
    {
        SetDead(true);
    }
}

void MainMenuTransition::VUpdate()
{
    if (!field_16_bDone)
    {
        field_10_current_Value += field_12_change_by_speed;
        if (field_14_fade_direction)
        {
            if (field_10_current_Value > 255)
            {
                field_10_current_Value = 255;
                field_246_colour_fade_value--;
                return;
            }
        }
        else if (field_10_current_Value < 0)
        {
            field_10_current_Value = 0;
        }
        field_246_colour_fade_value--;
    }
}

MainMenuTransition::MainMenuTransition(Layer layer, s32 fadeDirection, s32 bKillWhenDone, s32 speed, relive::TBlendModes blendMode)
    : BaseGameObject(true, 0)
{
    SetType(ReliveTypes::eFade);

    gObjListDrawables->Push_Back(this);

    SetDrawable(true);


    for (s32 i = 0; i < 8; i++)
    {
        field_1C_polys[0].field_0_polys[i].SetSemiTransparent(true);
        field_1C_polys[0].field_0_polys[i].SetBlendMode(blendMode);

        field_1C_polys[1].field_0_polys[i].SetSemiTransparent(true);
        field_1C_polys[1].field_0_polys[i].SetBlendMode(blendMode);
    }

    field_23C_layer = layer;

    if (fadeDirection)
    {
        field_10_current_Value = 0;
    }
    else
    {
        field_10_current_Value = 255;
    }

    field_242_idx = 0;
    field_246_colour_fade_value = 0;
    field_23E_width = 320;
    field_240_k120 = 120;
    StartTrans_436560(layer, static_cast<s16>(fadeDirection), static_cast<s16>(bKillWhenDone), static_cast<s16>(speed));
}

MainMenuTransition::~MainMenuTransition()
{
    gObjListDrawables->Remove_Item(this);
}

void MainMenuTransition::StartTrans_436560(Layer layer, s16 fadeDirection, s16 bKillWhenDone, s16 speed)
{
    field_23C_layer = layer;
    field_14_fade_direction = fadeDirection;
    field_16_bDone = 0;

    field_18_bKillOnDone = bKillWhenDone;

    if (!fadeDirection)
    {
        field_12_change_by_speed = -2 * speed;
    }
    else
    {
        field_12_change_by_speed = 2 * speed;
    }

    if (fadeDirection)
    {
        SfxPlayMono(relive::SoundEffects::MenuTransition, 0);
    }
}

void MainMenuTransition::VRender(OrderingTable& ot)
{
    // TODO: The fixed point math/var needs cleaning up/refactoring in here
    s32 currentValue = field_10_current_Value;
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
    s32 val1 = Math_Cosine(field_246_colour_fade_value).fpValue;
    s32 val2 = Math_Sine(field_246_colour_fade_value).fpValue;
    s32 r0g0 = -64 / ((v5 >> 2) + 1);
    for (s32 i = 0; i < 8; i++)
    {
        s32 idx = i + (8 * field_242_idx);
        s32 v8 = stru_55C038[idx].field_4 << 8;
        s32 v9 = stru_55C038[idx].field_2 << 16;
        s32 v10 = static_cast<s32>(stru_55C038[idx].field_0 << 16);
        s32 v11 = Math_FixedPoint_Multiply(v9, val1);
        s32 v12 = Math_FixedPoint_Multiply(v10, val2) - v11;
        s32 v13 = Math_FixedPoint_Multiply(op1, v8);
        s16 x0 = field_23E_width + 640 * ((s32)Math_FixedPoint_Multiply(v12, v13) >> 16) / 368;
        s32 v14 = Math_FixedPoint_Multiply(v9, val2);
        s32 v15 = Math_FixedPoint_Multiply(v10, val1) + v14;
        s32 v16 = Math_FixedPoint_Multiply(op1, v8);
        s16 y0 = field_240_k120 + (Math_FixedPoint_Multiply(v15, v16) >> 16);

        s32 v17 = 0;
        if (i < 7)
        {
            v17 = i + 1;
        }
        else
        {
            v17 = 0;
        }

        s32 idx2 = (8 * field_242_idx);
        s32 v36 = static_cast<s32>(stru_55C038[idx2 + v17].field_0 << 16);
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

        s32 v23 = Math_FixedPoint_Multiply(v20, val1);
        s32 x1 = Math_FixedPoint_Multiply(v36, val2) - v23;
        s32 v25 = Math_FixedPoint_Multiply(op1, y1);
        // TODO: Use PsxToPCX
        x1 = field_23E_width + 40 * ((s32)Math_FixedPoint_Multiply(x1, v25) >> 16) / 23; // LOWORD
        s32 v26 = Math_FixedPoint_Multiply(v38, val2);
        s32 v27 = v26 + Math_FixedPoint_Multiply(v36, val1);
        s32 v28 = Math_FixedPoint_Multiply(op1, y1);
        y1 = field_240_k120 + (Math_FixedPoint_Multiply(v27, v28) >> 16); // LOWORD
        Poly_G3* pPoly = &field_1C_polys[gPsxDisplay.mBufferIndex].field_0_polys[i];

        pPoly->SetRGB0(static_cast<u8>(r0g0), static_cast<u8>(r0g0), 255);
        pPoly->SetRGB1(static_cast<u8>(rgValue), static_cast<u8>(rgValue), static_cast<u8>(bValue));
        pPoly->SetRGB2(static_cast<u8>(rgValue), static_cast<u8>(rgValue), static_cast<u8>(bValue));

        pPoly->SetXY0(field_23E_width, field_240_k120);
        pPoly->SetXY1(x0, y0);
        pPoly->SetXY2(static_cast<s16>(x1), static_cast<s16>(y1));

        OrderingTable_Add(OtLayer(ppOt, field_23C_layer), pPoly);
    }

    if ((field_10_current_Value == 255 && field_14_fade_direction) || (field_10_current_Value == 0 && !field_14_fade_direction))
    {
        field_16_bDone = 1;
        if (field_18_bKillOnDone)
        {
            SetDead(true);
        }
    }
}

} // namespace AO