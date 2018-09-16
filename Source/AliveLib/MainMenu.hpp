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

enum MenuResIds
{
    eAbeSpeak = 0,
    eAbeSpeak2 = 1,
    eAbeIntro = 2,
    eDoor = 3,
    eOptionFlare = 4,
    eUnknown = 5,
    eResHighLite = 6,
    eSligSpeak = 7,
    eGlukkonSpeak = 8,
    eScrabSpeak = 9,
    eParamiteSpeak = 10,
};

struct MainMenuControllerResources
{
    BYTE** field_0_resources[11];
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
    __int16 field_8_anim_enum;
    __int16 field_A_frame_number;
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

    enum AnimIds
    {
        eAbe_Idle = 1,
        eAbe_GoodBye = 6,
        eAbe_Hello = 3,
        eAbe_OK = 4,
        eAbe_FollowMe = 5,
        eAbe_Chant = 12,
        eAbe_Wait = 7,
        eAbe_Work = 14,
        eAbe_Anger = 8,
        eAbe_AllYa = 15,
        eAbe_Sympathy = 17,
        eAbe_StopIt = 16,

        eSlig_Idle = 18,
        eSlig_Hi = 26,
        eSlig_HereBoy = 21,
        eSlig_Freeze = 19,
        eSlig_GetEm = 20,
        eSlig_SmoBs = 25,
        eSlig_Bs = 24,
        eSlig_LookOut = 23,
        eSlig_Laugh = 22,

        eGlukkon_Idle = 27,
        eGlukkon_Hey = 35,
        eGlukkon_Commere = 32,
        eGlukkon_StayHere = 31,
        eGlukkon_DoIt = 33,
        eGlukkon_KillEm = 29,
        eGlukkon_AllOYa = 36,
        eGlukkon_Help = 34,
        eGlukkon_Laugh = 30,

        eScrab_ShredPower = 39,
        eScrab_Howl = 40,
        eScrab_Idle = 37,

        eParamite_Howdy = 44,
        eParamite_Cmon = 45,
        eParamite_Stay = 46,
        eParamite_DoIt = 47,
        eParamite_Attack = 43,
        eParamite_AllAYa = 48,
        eParamite_Idle = 41,
    };

    EXPORT void Set_Anim_4D05E0(__int16 a2, __int16 a3 = 0);
    EXPORT void Load_Anim_Pal_4D06A0(AnimationEx* pAnim);
    EXPORT signed int sub_4CF640();
    EXPORT void sub_4CFE80();
    EXPORT void UpdateHighliteGlow_4D0630();
    EXPORT static void callback_4D06E0(MainMenuController *a1);
    EXPORT static int DrawMenuText_4D20D0(MainMenuText *array, int **ot, Font *font, int *polyIndex, char a5);

    unsigned int    MainMenuPageUpdateTemplate(int input);  // Returns target cam/page info.
    int             MainMenuPageRenderTemplate(int **ot);   // Returns poly index for text rendering.
    
    // Page Functions
    EXPORT void t_Render_Abe_Speak_4D2060(int** ot);
    EXPORT signed int t_Input_Abe_Speak_4D2D20(DWORD input_held);

    EXPORT void t_Render_Slig_Speak_4D2370(int** ot);
    EXPORT signed int t_Input_Slig_Speak_4D3280(DWORD input_held);
    EXPORT void t_Unload_Slig_Speak_4D3170();
    EXPORT void t_Load_Slig_Speak_4D3090();

    EXPORT void t_Render_Glukkon_Speak_4D23C0(int** ot);
    EXPORT signed int t_Input_Glukkon_Speak_4D3670(DWORD input_held);
    EXPORT void t_Unload_Glukkon_Speak_4D3560();
    EXPORT void t_Load_Glukkon_Speak_4D3480();

    EXPORT void t_Render_Scrab_Speak_4D2410(int** ot);
    EXPORT signed int t_Input_Scrab_Speak_4D3A60(DWORD input_held);
    EXPORT void t_Unload_Scrab_Speak_4D3950();
    EXPORT void t_Load_Scrab_Speak_4D3870();

    EXPORT void t_Render_Paramite_Speak_4D2460(int** ot);
    EXPORT signed int t_Input_Paramite_Speak_4D3D60(DWORD input_held);
    EXPORT void t_Unload_Paramite_Speak_4D3C50();
    EXPORT void t_Load_Paramite_Speak_4D3B70();

    void Unload_AbeSpeakResources();
    void Load_AbeSpeakResources();

    // Front End
    EXPORT unsigned int Page_Front_Update_4D0720(unsigned int input);
    EXPORT int Page_Front_Render_4D24B0(int **ot);

    static MainMenuController * gMainMenuController;
private:
    signed int HandleGameSpeakInput(DWORD input_held, std::function<signed int(InputCommands cmd)> fnOnGameSpeak);

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
    void* field_210_pUnknown;
    __int16 field_214_page_index;
    __int16 field_216;
    __int16 field_218_target_page_index;
    __int16 field_21A_target_cam;
    __int16 field_21C_bDoScreenTransistionEffect;
    __int16 field_21E_bChangeScreen;
    __int16 field_220_frame_table_idx;
    char field_222;
    int field_224;
    __int16 field_228_res_idx;
    __int16 field_22A_anim_frame_num;
    int field_22C_T80;
    __int16 field_230_fmv_level_index;
    __int16 field_232;
    const char* field_234_pStr;
    __int16 field_238;
    __int16 field_23A;
    int field_23C_T80;
    int field_240_credits_current_cam;
    __int16 field_244_lvl_id;
    __int16 field_246_path_id;
    __int16 field_248;
    __int16 field_24A_abeXOff;
    __int16 field_24C_abeYOff;
    __int16 field_24E_start_scale;
    __int16 field_250;
    __int16 field_252;
    int field_254;
    int field_258;
    __int16 field_25C;
    __int16 field_25E;
private:
    void Unload_Resource(BYTE**& res);
};
ALIVE_ASSERT_SIZEOF(MainMenuController, 0x260);

using t_MainMenuPage_Update = decltype(&MainMenuController::MainMenuPageUpdateTemplate);

struct MainMenuPage
{
    __int16 field_0_cam_id;
    __int16 field_2;
    int field_4;
    __int16 field_8;
    __int16 field_A_bDoScreenTransistionEffect;
    __int16 field_C_target_camera;
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