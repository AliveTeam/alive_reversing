#pragma once

#include "FunctionFwd.hpp"
#include "DynamicArray.hpp"
#include "BaseGameObject.hpp"
#include "Animation.hpp"
#include "Font.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Factory.hpp"
#include "Input.hpp"

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
    int field_8_anim_frame_offset;
};

struct MainMenuControllerResources
{
    BYTE** field_0_res_abespeak;
    BYTE** field_4_res_abespek2;
    BYTE** field_8_res_abeintro;
    BYTE** field_C_res_door;
    BYTE** field_10_res_optflare;
    BYTE** field_14;
    BYTE** field_18_res_highlite;
    BYTE** field_1C_res_slgspeak;
    BYTE** field_20_res_glkspeak;
    BYTE** field_24_res_scrspeak;
    BYTE** field_28_res_parspeak;
};

struct MainMenuText
{
    int field_0_x;
    int field_4_y;
    const char *field_8_text;
    BYTE field_C_align;
    BYTE field_D;
    BYTE field_E;
    BYTE field_F;
    float field_10_scale;
    BYTE field_14;
    BYTE field_15;
    BYTE field_16;
    BYTE field_17;
};

struct MainMenuFrameTable
{
    int field_0_frame_offset;
    __int16 field_4;
    __int16 field_6;
    __int16 field_8;
    __int16 field_A;
};

extern PerLvlData gPerLvlData_561700[17];

struct Path_TLV;

class MainMenuController : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT void ctor_4CE9A0(Path_TLV* pTlv, TlvItemInfoUnion tlvOffsetLevelIdPathId);

    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;

    EXPORT void Render_4CF4C0(int **ot);
    EXPORT void Update_4CF010();

    EXPORT static int __stdcall GetPageIndexFromCam_4D05A0(int camId);
    EXPORT void sub_4D05E0(__int16 a2, __int16 a3);
    EXPORT void Load_Anim_Pal_4D06A0(AnimationEx* pAnim);
    EXPORT signed int sub_4CF640();
    EXPORT void sub_4CFE80();
    EXPORT void UpdateHighliteGlow_4D0630();
    EXPORT static void callback_4D06E0(MainMenuController *a1);
    EXPORT static int DrawMenuText_4D20D0(MainMenuText *array, int **ot, Font *font, int *polyIndex, char a5);

    unsigned int    MainMenuPageUpdateTemplate(int input);  // Returns target cam/page info.
    int             MainMenuPageRenderTemplate(int **ot);   // Returns poly index for text rendering.
    
    // Page Functions
    
    // Front End
    EXPORT unsigned int Page_Front_Update_4D0720(InputCommands input);
    EXPORT int Page_Front_Render_4D24B0(int **ot);

    static MainMenuController * gMainMenuController;
private:
    void HandleCreditsControllerUpdate();
    void HandleMainMenuUpdate();
public:
    int field_E4[3];
    int field_F0;
    MainMenuControllerResources field_F4_resources;
    Font field_120_font;
    AnimationEx field_158_animation;
    int field_1F0;
    int field_1F4_credits_next_frame;
    int field_1F8_page_timeout; // Timer for starting demos automatically / backing out of menus
    __int16 field_1FC_button_index;
    __int16 field_1FE_highlite_alpha;
    __int16 field_200_highlite_glow_speed;
    __int16 field_202_input_hold_down_timer;
    int field_204_prev_pressed;
    int field_208;
    int field_20C;
    int field_210;
    __int16 field_214_page_index;
    __int16 field_216;
    __int16 field_218_target_page_index;
    __int16 field_21A_target_cam;
    __int16 field_21C;
    __int16 field_21E;
    __int16 field_220;
    char field_222;
    int field_224;
    __int16 field_228;
    __int16 field_22A;
    int field_22C_T80;
    __int16 field_230_fmv_level_index;
    __int16 field_232;
    const char *field_234;
    __int16 field_238;
    __int16 field_23A;
    int field_23C_T80;
    int field_240_credits_current_cam;
    __int16 field_244;
    __int16 field_246;
    __int16 field_248;
    __int16 field_24A;
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

using t_MainMenuPage_Update = decltype(&MainMenuController::MainMenuPageUpdateTemplate);

struct MainMenuPage
{
    __int16 field_0_cam_id;
    __int16 field_2;
    int field_4;
    __int16 field_8;
    __int16 field_A;
    __int16 field_C;
    __int16 field_E_show_character;
    t_MainMenuPage_Update field_10_fn_update;
    void(__thiscall *field_14_fn_render)(MainMenuController *, int **);
    MainMenuButton *field_18_buttons;
    int(__thiscall *field_1C_fn_on_load)(MainMenuController *);
    int(__thiscall *field_20_fn_on_free)(MainMenuController *);
};


ALIVE_VAR_EXTERN(int, sMainMenuObjectCounter_BB4400);

ALIVE_VAR_EXTERN(char, sEnableCheatLevelSelect_5C1BEE);
ALIVE_VAR_EXTERN(char, sEnableCheatFMV_5C1BEC);