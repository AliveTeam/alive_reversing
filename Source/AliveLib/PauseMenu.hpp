#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Animation.hpp"
#include "Game.hpp"
#include "Psx.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"
#include "PsxDisplay.hpp"
#include "Input.hpp"
#include "ScreenManager.hpp"
#include "Map.hpp"
#include "PathData.hpp"
#include "Quicksave.hpp"
#include "DDCheat.hpp"

void PauseMenu_ForceLink();

class PauseMenu;
struct PauseMenuPage;

ALIVE_ARY_EXTERN(byte, 32, sFont1VRAM_5BC5C8);
ALIVE_ARY_EXTERN(byte, 32, sFont2VRAM_5BC5D8);

enum PauseMenuAlign
{
    Left = 0,
    Centre = 1,
    Right = 2,
};

class Font
{
public:
    EXPORT void ctor_433590(int blockSize, BYTE *palette, byte *vramBuffer);
    EXPORT int DrawString_4337D0(int **ot, char *text, int x, __int16 y, char abr, int bSemiTrans, int a2, int a9, char r, char g, char b, int a13, signed int scale, int a15, __int16 colorRandomRange);
    EXPORT int MeasureWidth_433700(char * text);
public:
    byte gap0[32];
    BYTE **field_20;
    byte *field_24;
    PSX_RECT field_28_palette_rect;
    int field_30_block_size;
    byte *field_34;
};
ALIVE_ASSERT_SIZEOF(Font, 0x38);

struct PauseMenuPageEntry
{
    __int16 field_0_unknown2;
    __int16 field_2_x;
    __int16 field_4_y;
    __int16 field_6_unknown;
    char *field_8_text;
    unsigned char field_C_r;
    unsigned char field_D_g;
    unsigned char field_E_b;
    unsigned char field_F_alignment;
};
ALIVE_ASSERT_SIZEOF(PauseMenuPageEntry, 0x10);

class PauseMenu : public BaseAnimatedWithPhysicsGameObject
{
    struct PauseMenuPage;
public:
    virtual void VDestructor(signed int flags);
    virtual void VUpdate() override;

    EXPORT PauseMenu * ctor_48FB80();
    EXPORT void dtor_48FCE0();
    EXPORT void dtor_48FCB0(signed int flags);

    EXPORT void Init_491760();

    EXPORT void Update_48FD80();
    EXPORT void Render_490BD0(int **ot);

    EXPORT void Page_Main_Update_4903E0();
    EXPORT void Page_Base_Render_490A50(int **ot, PauseMenuPage *mp);

    using t_PmPage_Update = decltype(&PauseMenu::Page_Main_Update_4903E0);
    using t_PmPage_Render = decltype(&PauseMenu::Page_Base_Render_490A50);

#ifdef DEVELOPER_MODE
    // DEVELOPER MODE STUFF
    void CustomPauseMenuUpdate();
    ///////////////////////
#endif

    struct PauseMenuPage
    {
        t_PmPage_Update field_0_fn_update;
        t_PmPage_Render field_4_fn_render;
        PauseMenuPageEntry *field_8_menu_items;
        __int16 field_C_selected_index;
        char field_E_background_r;
        char field_F_background_g;
        char field_10_background_b;
        char field_11;
        char field_12;
        char field_13;
    };
    ALIVE_ASSERT_SIZEOF(PauseMenu::PauseMenuPage, 0x14);

public:
    char gapE4;
    byte gapE5[15];
    Font field_F4_font;
    __int16 word12C_flags;
    __int16 field_12E_selected_glow;
    __int16 field_130;
    __int16 field_132;
    __int16 field_134_Index_Main;
    __int16 field_136;
    __int16 field_138;
    __int16 field_13A;
    __int16 field_13C;
    __int16 field_13E;
    __int16 field_140;
    __int16 field_142;
    PauseMenu::PauseMenuPage field_144_active_menu;
    AnimationEx field_158_animation;
    int field_1F0;
    Poly_F4 field_1F4[4];
    int field_264;
};
//ALIVE_ASSERT_SIZEOF(PauseMenu, 0x268);



ALIVE_VAR_EXTERN(PauseMenu*, pPauseMenu_5C9300);