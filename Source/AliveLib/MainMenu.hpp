#pragma once

#include "FunctionFwd.hpp"
#include "DynamicArray.hpp"
#include "BaseGameObject.hpp"
#include "Animation.hpp"
#include "Font.hpp"

void MainMenu_ForceLink();

struct MenuFMV
{
    const char *field_0_name;
    __int16 field_4;
    __int16 field_6;
    __int16 field_8;
    __int16 field_A;
    __int16 field_C;
    __int16 field_E;
};


struct MainMenuButton
{
    __int16 field_0;
    __int16 field_2_x;
    __int16 field_4_y;
    __int16 field_6;
    __int16 field_8;
    __int16 field_A;
};

struct MainMenuControllerResources
{
    AnimHeader *field_0_res_abespeak;
    AnimHeader *field_4_res_abespek2;
    AnimHeader *field_8_res_abeintro;
    AnimHeader *field_C_res_door;
    AnimHeader *field_10_res_optflare;
    AnimHeader *field_14;
    AnimHeader *field_18_res_highlite;
    AnimHeader *field_1C_res_slgspeak;
    AnimHeader *field_20_res_glkspeak;
    AnimHeader *field_24_res_scrspeak;
    AnimHeader *field_28_res_parspeak;
};


class MainMenuController : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT MainMenuController * ctor_4CE9A0(int a2, int a3);

    EXPORT void Render_4CF4C0(int **ot);

    static EXPORT int __stdcall MainMenuController::GetPageIndexFromCam_4D05A0(int camId);
    EXPORT void sub_4D05E0(__int16 a2, __int16 a3);
    EXPORT void sub_4D06A0(AnimationEx *a3);
    EXPORT static void callback_4D06E0(MainMenuController *a1);
private:
    int field_E4[3];
    int field_F0;
    MainMenuControllerResources field_F4_resources;
    Font field_120_font;
    AnimationEx field_158_animation;
    int field_1F0;
    int field_1F4;
    int field_1F8;
    __int16 field_1FC_button_index;
    __int16 field_1FE;
    __int16 field_200;
    __int16 field_202;
    int field_204;
    int field_208;
    int field_20C;
    int field_210;
    __int16 field_214_page_index;
    __int16 field_216;
    __int16 field_218;
    __int16 field_21A;
    __int16 field_21C;
    __int16 field_21E;
    __int16 field_220;
    char field_222;
    int field_224;
    __int16 field_228;
    __int16 field_22A;
    int field_22C_T80;
    int field_230;
    const char *field_234;
    __int16 field_238;
    __int16 field_23A;
    int field_23C_T80;
    int field_240;
    int field_244;
    int field_248;
    __int16 field_24C_unknown;
    __int16 field_24E;
    __int16 field_250;
    __int16 field_252;
    int field_254;
    int field_258;
    __int16 field_25C;
    __int16 field_25E;
};
ALIVE_ASSERT_SIZEOF(MainMenuController, 0x260);


struct MainMenuPage
{
    __int16 field_0_cam_id;
    __int16 field_2;
    int field_4;
    __int16 field_8;
    __int16 field_A;
    __int16 field_C;
    __int16 field_E_show_character;
    int(__thiscall *field_10_fn_update)(MainMenuController *);
    void(__thiscall *field_14_fn_render)(MainMenuController *, int **);
    MainMenuButton *field_18_buttons;
    int field_1C_fn;
    int field_20;
};
