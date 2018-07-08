#include "stdafx.h"
#include "PauseMenu.hpp"
#include "stdlib.hpp"
#include "Function.hpp"


void PauseMenu_ForceLink() { }

EXPORT void CC String_FormatString_4969D0(char *src, char *dst, int a3, char a4)
{
    NOT_IMPLEMENTED();
}

void PauseMenu::VDestructor(signed int flags)
{
    dtor_48FCB0(flags);
}

void PauseMenu::VUpdate()
{
    Update_48FD80();
}

PauseMenu * PauseMenu::ctor_48FB80()
{
    NOT_IMPLEMENTED();
    return nullptr;
}

void PauseMenu::dtor_48FCE0()
{
    NOT_IMPLEMENTED();
}

void PauseMenu::dtor_48FCB0(signed int flags)
{
    NOT_IMPLEMENTED();
}

void PauseMenu::Update_48FD80()
{
    NOT_IMPLEMENTED();
}

void PauseMenu::Page_Base_Render_490A50(int ** ot, PauseMenuPage * mp)
{
    int i = 0;
    PauseMenuPageEntry * e = &mp->field_8_menu_items->entries[i];
    
    while(e->field_6_text)
    {
        char textFormatted[128];
        String_FormatString_4969D0(e->field_6_text, textFormatted, 128, 1);

        auto glow = 0;
        if (mp->field_C_selected_index == i)
        {
            glow = field_12E_selected_glow;
        }

        auto x = e->field_0_x;

        if (e->field_D_alignment != 0)
        {
            auto textMeasure = field_F4_font.MeasureWidth_433700(textFormatted);

            if (textMeasure >= 608)
            {
                x = 16;
            }
            else
            {
                if (e->field_D_alignment != 2)
                {
                    textMeasure /= 2;
                }
                x -= textMeasure;
            }
        }

        field_142 = field_F4_font.DrawString_4337D0(
            ot,
            textFormatted,
            x, // X
            e->field_2_y, // Y
            0,
            1,
            0,
            41,
            glow + e->field_A_r,
            glow + e->field_B_g,
            glow + e->field_C_b,
            field_142,
            0x10000,
            640,
            0);

        e = &mp->field_8_menu_items->entries[++i];
    }
}

int Font::DrawString_4337D0(int ** ot, char * text, int x, __int16 y, char abr, int bSemiTrans, int a2, int a9, char r, char g, char b, int a13, signed int scale, int a15, __int16 colorRandomRange)
{
    NOT_IMPLEMENTED();
    return 0;
}

int Font::MeasureWidth_433700(char * text)
{
    NOT_IMPLEMENTED();
    return 0;
}

ALIVE_VAR(1, 0x5C9300, PauseMenu*, pPauseMenu_5C9300, nullptr);