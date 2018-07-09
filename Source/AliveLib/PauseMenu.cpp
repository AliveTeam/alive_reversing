#include "stdafx.h"
#include "PauseMenu.hpp"
#include "stdlib.hpp"
#include "Function.hpp"


void PauseMenu_ForceLink() { }

ALIVE_ARY(1, 0x5bc5c8, byte, 32, sFont1VRAM_5BC5C8, {});
ALIVE_ARY(1, 0x5BC5D8, byte, 32, sFont2VRAM_5BC5D8, {});

ALIVE_VAR(1, 0x5ca4d8, char, sQuicksave_SaveNextFrame_5CA4D8, 0);
ALIVE_VAR(1, 0x5ca4d9, char, sQuicksave_LoadNextFrame_5CA4D9, 0);
ALIVE_VAR(1, 0x5c9304, char, sDisableFontFlicker_5C9304, 0);


ALIVE_ARY(1, 0x554474, byte, 32, pal_554474,{
    0x00, 0x00, 0x21, 0x84, 0x42, 0x88, 0x63, 0x8C, 0x84, 0x90,
    0xA5, 0x14, 0xE7, 0x1C, 0x08, 0x21, 0x29, 0x25, 0x4A, 0x29,
    0x6B, 0x2D, 0x8C, 0x31, 0xAD, 0x35, 0xEF, 0x3D, 0x10, 0x42,
    0x73, 0x4E });


EXPORT signed __int16 sub_4A2BC0()
{
    NOT_IMPLEMENTED();
    return 0;
}

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
    BaseAnimatedWithPhysicsGameObject::BaseAnimatedWithPhysicsGameObject_ctor_424930(0);

    SetVTable(&field_158_animation, 0x544290);
    SetVTable(this, 0x546658);

    sQuicksave_SaveNextFrame_5CA4D8 = 0;
    sQuicksave_LoadNextFrame_5CA4D9 = 0;

    field_4_typeId = 95;
    field_6_flags &= ~0x8;
    field_6_flags |= 0x100;
    word12C_flags &= ~0xE;
    word12C_flags &= ~1u;

    field_1C_update_delay = 25;

    gObjList_drawables_5C1124->Push_Back(this);

    field_136 = 0;
    field_138 = 0;
    field_13A = 0;
    field_13C = 0;
    field_13E = 0;
    field_140 = 0;

    field_F4_font.ctor_433590(256, pal_554474, sFont1VRAM_5BC5C8);
    Init_491760();
    sub_4A2BC0();
    
    sDisableFontFlicker_5C9304 = 0;
    
    return this;
}

void PauseMenu::dtor_48FCE0()
{
    NOT_IMPLEMENTED();
}

void PauseMenu::dtor_48FCB0(signed int flags)
{
    NOT_IMPLEMENTED();
}

void PauseMenu::Init_491760()
{
    ResourceManager::LoadResourceFile_49C170("EMOHAP.BAN", 0);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, 201);
    ResourceManager::LoadResourceFile_49C170("EMOANGRY.BAN", 0);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, 202);
    ResourceManager::LoadResourceFile_49C170("EMONORM.BAN", 0);
    BYTE **v2 = Add_Resource_4DC130(ResourceManager::Resource_Animation, 203);

    if (field_158_animation.Init_40A030(1248, gObjList_animations_5C1A24, this, 54, 47, v2, 1, 0, 0))
    {
        this->field_158_animation.field_C_render_layer = field_20_animation.field_C_render_layer;
        this->field_158_animation.field_14_scale = field_CC_sprite_scale;
        this->field_158_animation.field_8_r = -128;
        this->field_158_animation.field_9_g = -128;
        this->field_158_animation.field_A_b = -128;
    }
    else
    {
        field_6_flags |= 1u; // Todo: check this
    }
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

void Font::ctor_433590(int blockSize, BYTE * palette, byte * vramBuffer)
{
    NOT_IMPLEMENTED();
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