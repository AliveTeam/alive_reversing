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
#include "QuikSave.hpp"
#include "DDCheat.hpp"
#include "Font.hpp"
#include "FixedPoint.hpp"
#include "Midi.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

void PauseMenu_ForceLink();


class PauseMenu;
struct PauseMenuPage;

enum PauseMenuAlign
{
    Left = 0,
    Centre = 1,
    Right = 2,
};

struct PauseMenuPageEntry
{
    __int16 field_0_unknown2;
    __int16 field_2_x;
    __int16 field_4_y;
    __int16 field_6_unknown;
    const char *field_8_text;
    unsigned char field_C_r;
    unsigned char field_D_g;
    unsigned char field_E_b;
    unsigned char field_F_alignment;
};
ALIVE_ASSERT_SIZEOF(PauseMenuPageEntry, 0x10);

class PauseMenu : public BaseAnimatedWithPhysicsGameObject
{
public:
    struct PauseMenuPage;

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VScreenChanged() override;

    PauseMenu();

    EXPORT PauseMenu* ctor_48FB80();
    EXPORT void dtor_48FCE0();
    EXPORT BaseGameObject* vdtor_48FCB0(signed int flags);

    EXPORT void Init_491760();

    EXPORT void Update_48FD80();
    EXPORT void Render_490BD0(int** ot);

    EXPORT void vsub_490D30();

    EXPORT void Page_Main_Update_4903E0();
    EXPORT void Page_Base_Render_490A50(int** ot, PauseMenuPage* mp);

    EXPORT void Page_ControlsActions_Update_48FA60();
    EXPORT void Page_ReallyQuit_Update_490930();

    EXPORT void Page_Save_Update_491210();
    EXPORT void Page_Save_Render_491660(int** ot, PauseMenuPage* pPage);

    EXPORT void Page_Status_Update_4916A0();
    EXPORT void Page_Status_Render_491710(int** ot, PauseMenuPage* pPage);

    EXPORT void Page_Load_Update_490D50();
    EXPORT void Page_Load_Render_4910A0(int** ot, PauseMenuPage *mp);

    using t_PmPage_Update = decltype(&PauseMenu::Page_Main_Update_4903E0);
    using t_PmPage_Render = decltype(&PauseMenu::Page_Base_Render_490A50);

    // DEVELOPER MODE STUFF
    void CustomPauseMenuUpdate();
    ///////////////////////

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

private:
    void RestartPath();

public:
    Alive::Font field_F4_font;
    __int16 word12C_flags;
    __int16 field_12E_selected_glow;
    __int16 field_130;
    __int16 field_132;

    enum MainPages : __int16
    {
        ePage_Continue_0 = 0,
        ePage_QuickSave_1 = 1,
        ePage_2 = 2,
        ePage_Status_3 = 3,
        ePage_Save_4 = 4,
        ePage_Load_5 = 5,
        ePage_RestartPath_6 = 6,
        ePage_Quit_7 = 7,
    };

    /*MainPages*/ __int16 field_134_Index_Main;
    __int16 field_136;
    __int16 field_138;
    __int16 field_13A;
    __int16 field_13C;
    __int16 field_13E;
    __int16 field_140;
    __int16 field_142;
    PauseMenu::PauseMenuPage field_144_active_menu;
    Animation field_158_animation;
    Prim_SetTPage field_1F0[2];
    Poly_F4 field_210[2];
    Prim_SetTPage field_248[2]; // Not used ??
};
ALIVE_ASSERT_SIZEOF(PauseMenu, 0x268);

ALIVE_VAR_EXTERN(PauseMenu*, pPauseMenu_5C9300);
ALIVE_VAR_EXTERN(char, sQuicksave_SaveNextFrame_5CA4D8);
ALIVE_VAR_EXTERN(char, sQuicksave_LoadNextFrame_5CA4D9);
ALIVE_ARY_EXTERN(BYTE, 32, pal_554474);
