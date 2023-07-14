#include "stdafx.h"
#include "MainMenuTransition.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"

struct MainMenu_TransitionData final
{
    u16 field_0;
    u16 field_2;
    u16 field_4;
    u16 field_6;
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

MainMenuTransition::MainMenuTransition(Layer layer, s32 fadeDirection, bool killWhenDone, s32 fadeSpeed, relive::TBlendModes blendMode)
    : BaseGameObject(true, 0)
{
    SetType(ReliveTypes::eMainMenuTransistion);

    gObjListDrawables->Push_Back(this);

    SetDrawable(true);

    for (s32 i = 0; i < 8; i++)
    {
        field_2C_polys.mPolys[i].SetSemiTransparent(true);
        field_2C_polys.mPolys[i].SetBlendMode(blendMode);
    }

    mLayer = layer;

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
    StartTrans(layer, static_cast<s16>(fadeDirection), killWhenDone, static_cast<s16>(fadeSpeed));
}

void MainMenuTransition::StartTrans(Layer layer, s16 fadeDirection, bool killWhenDone, s16 speed)
{
    mLayer = layer;
    field_24_fade_direction = fadeDirection;
    mDone = 0;

    if (speed)
    {
        field_2A = 0;
    }
    else
    {
        field_2A = 1;
    }

    mKillWhenDone = killWhenDone;

    if (fadeDirection)
    {
        field_22_change_by_speed = speed;
        SfxPlayMono(relive::SoundEffects::MenuTransition, 0);
    }
    else
    {
        field_22_change_by_speed = -speed;
    }
}

void MainMenuTransition::VUpdate()
{
    if (!mDone && !field_2A)
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

void MainMenuTransition::VRender(OrderingTable& ot)
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
    s32 val1 = Math_Cosine(field_colour_fade_value).fpValue;
    s32 val2 = Math_Sine(field_colour_fade_value).fpValue;
    s32 r0g0 = -64 / ((v5 >> 2) + 1);
    for (s32 i = 0; i < 8; i++)
    {
        s32 idx = i + (8 * this->field_252_tbl_idx);
        s32 v8 = stru_55C038[idx].field_4 << 8;
        s32 v9 = stru_55C038[idx].field_2 << 16;
        s32 v10 = static_cast<s32>(stru_55C038[idx].field_0 << 16);
        s32 v11 = Math_FixedPoint_Multiply(v9, val1);
        s32 v12 = Math_FixedPoint_Multiply(v10, val2) - v11;
        s32 v13 = Math_FixedPoint_Multiply(op1, v8);
        s16 x0 = this->field_24E_width + 640 * ((s32) Math_FixedPoint_Multiply(v12, v13) >> 16) / 368;
        s32 v14 = Math_FixedPoint_Multiply(v9, val2);
        s32 v15 = Math_FixedPoint_Multiply(v10, val1) + v14;
        s32 v16 = Math_FixedPoint_Multiply(op1, v8);
        s16 y0 = this->field_250_k120 + (Math_FixedPoint_Multiply(v15, v16) >> 16);

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
        x1 = this->field_24E_width + 40 * ((s32) Math_FixedPoint_Multiply(x1, v25) >> 16) / 23; // LOWORD
        s32 v26 = Math_FixedPoint_Multiply(v38, val2);
        s32 v27 = v26 + Math_FixedPoint_Multiply(v36, val1);
        s32 v28 = Math_FixedPoint_Multiply(op1, y1);
        y1 = this->field_250_k120 + (Math_FixedPoint_Multiply(v27, v28) >> 16); // LOWORD
        Poly_G3* pPoly = &field_2C_polys.mPolys[i];

        pPoly->SetRGB0(static_cast<u8>(r0g0), static_cast<u8>(r0g0), 255);
        pPoly->SetRGB1(static_cast<u8>(rgValue), static_cast<u8>(rgValue), static_cast<u8>(bValue));
        pPoly->SetRGB2(static_cast<u8>(rgValue), static_cast<u8>(rgValue), static_cast<u8>(bValue));

        pPoly->SetXY0( field_24E_width, field_250_k120);
        pPoly->SetXY1( x0, y0);
        pPoly->SetXY2( static_cast<s16>(x1), static_cast<s16>(y1));

        ot.Add(mLayer, pPoly);
    }

    if ((field_20_current_value == 255 && field_24_fade_direction) || (field_20_current_value == 0 && !field_24_fade_direction))
    {
        mDone = 1;
        if (mKillWhenDone)
        {
            SetDead(true);
        }
    }
}

MainMenuTransition::~MainMenuTransition()
{
    gObjListDrawables->Remove_Item(this);
}

void MainMenuTransition::VScreenChanged()
{
    if (gMap.LevelChanged() || gMap.PathChanged())
    {
        SetDead(true);
    }
}
