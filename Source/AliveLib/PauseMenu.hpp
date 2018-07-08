#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Animation.hpp"
#include "Game.hpp"
#include "Psx.hpp"

void PauseMenu_ForceLink();

class PauseMenu;

class Font
{
public:
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

#pragma pack(push)
#pragma pack(2)
struct PauseMenuPageEntry
{
    __int16 field_0_x;
    __int16 field_2_y;
    __int16 field_4_unknown;
    char *field_6_text;
    char field_A_r;
    char field_B_g;
    char field_C_b;
    char field_D_alignment;
    __int16 field_E_unknown2;
};
#pragma pack(pop)

#pragma pack(push)
#pragma pack(2)
struct PauseMenuPageEntryList
{
    __int16 unknown;
    PauseMenuPageEntry entries[1];
};
#pragma pack(pop)

struct PauseMenuPage
{
    void(__thiscall *field_0_fn_update)(PauseMenu *);
    void(__thiscall *field_4_fn_render)(PauseMenu *, int **, PauseMenuPage *);
    PauseMenuPageEntryList *field_8_menu_items;
    __int16 field_C_selected_index;
    char field_E_background_r;
    char field_F_background_g;
    char field_10_background_b;
    char field_11;
    char field_12;
    char field_13;
};
ALIVE_ASSERT_SIZEOF(PauseMenuPage, 0x14);

class PauseMenu : public BaseAnimatedWithPhysicsGameObject
{
public:
    virtual void VDestructor(signed int flags);
    virtual void VUpdate() override;

    EXPORT PauseMenu * ctor_48FB80();
    EXPORT void dtor_48FCE0();
    EXPORT void dtor_48FCB0(signed int flags);

    EXPORT void Update_48FD80();

    EXPORT void Page_Base_Render_490A50(int **ot, PauseMenuPage *mp);

private:
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
    PauseMenuPage field_144_active_menu;
    AnimationEx field_158_animation;
    int field_1F0;
    Poly_F4 field_1F4[4];
    int field_264;
};
ALIVE_ASSERT_SIZEOF(PauseMenu, 0x268);

ALIVE_VAR_EXTERN(PauseMenu*, pPauseMenu_5C9300);