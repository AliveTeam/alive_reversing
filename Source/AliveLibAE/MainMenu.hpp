#pragma once

#include "FunctionFwd.hpp"
#include "DynamicArray.hpp"
#include "BaseGameObject.hpp"
#include "Animation.hpp"
#include "Font.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Factory.hpp"
#include "Input.hpp"
#include "BitField.hpp"

void MainMenu_ForceLink();

enum class LevelIds : s16;

struct MenuFMV final
{
    const char_type* field_0_name;
    LevelIds field_4_level_id;
    s16 field_6;
    s16 field_8;
    s16 field_A_fmv_id;
    s16 field_C;
    s16 field_E;
};

enum MainMenuButtonType
{
    eNoButton = 0, // signifies the end of the BtnArray
    eCircularSelectableButton = 1,
    eUnused_2 = 2,
    eUnused_3 = 3
};

struct MainMenuButton final
{
    s16 field_0_type;
    s16 field_2_x;
    s16 field_4_y;
    s16 field_6;
    s32 field_8_frameTableOffset;
};

enum MenuResIds
{
    eAbeSpeak = 0,
    eAbeSpeak2 = 1,
    eAbeIntro = 2,
    eMenuDoor = 3,
    eOptionFlare = 4,
    eUnknown = 5,
    eResHighLite = 6,
    eSligSpeak = 7,
    eGlukkonSpeak = 8,
    eScrabSpeak = 9,
    eParamiteSpeak = 10,
};

struct MainMenuControllerResources final
{
    u8** field_0_resources[11];
};

struct MainMenuText final
{
    s32 field_0_x;
    s32 field_4_y;
    const char_type* field_8_text;
    u8 field_C_align;
    u8 field_D;
    u8 field_E;
    u8 field_F;
    f32 field_10_scale;
    u8 field_14;
    u8 field_15;
    u8 field_16;
    u8 field_17;
};

struct MainMenuFrameTable final
{
    AnimId field_0_animation;
    s16 field_4_menu_res_id;
    s16 field_6_sound;
    s16 field_8_anim_enum;
    s16 field_A_frame_number;
};

extern PerLvlData gPerLvlData_561700[17];

struct Path_TLV;
class MainMenuTransition;

enum MainMenuCams : u16
{
    eNoChange = 0,
    eMainMenuCam = 1,
    eGamespeakCharacterSelectionCam = 2,
    eOptionsCam = 3,
    eLoadGameMenuCam = 4,
    eRemapInputsCam = 5,
    eCheatMenu_SelectFMVCam = 6,
    eUnknown7Cam = 7,
    eUnknown8Cam = 8,
    eAbesMotionKeysCam = 9,
    eUnknown10Cam = 10,
    eDummyBlankCam = 11,
    eBackstory_Or_NewGameCam = 12,
    eGameIsLoading_ShaddapCam = 13,
    eAbeGamespeakCam = 14,
    eSligGamespeakCam = 15,
    eGlukkonGamespeakCam = 16,
    eScrabGamespeakCam = 17,
    eParamiteGamespeakCam = 18,
    eUnknown19Cam = 19,
    eUnknown20Cam = 20,
    eUnknown21Cam = 21,
    eDemoIsLoading_ShaddapCam = 22,
    eControllerSelectionCam = 23,
    eUnknown24Cam = 24,
    eGameBootCopyrightSplashCam = 25,
    eUnknown26Cam = 26,
    eUnknown27Cam = 27,
    eUnknown28Cam = 28,
    eUnknown29Cam = 29,
    eDemoSelectionCam = 30,
    eCheatMenu_SelectLevelCam = 31,
};

enum camTransEffectState : s16
{
    eDone_0 = 0,
    eConstructed_1 = 1,
    eDone_2 = 2,
    eTopToBottom_3 = 3,
    eBottomToTop_4 = 4,
    eLeftToRight_5 = 5,
    eRightToLeft_6 = 6,
    eUnknown_7 = 7
};

struct MainMenuNextCam final
{
    s16 page_update_camera;      // LOWORD
    s16 button_idx_to_highlight; // HIWORD - by default, always the first button on the given page should be highlighted

    // ctor is needed for gcc/clang to not complain about inline struct definitions in return statements
    MainMenuNextCam(s16 pageUpdateCam, s16 btnIdx = 0x0000)
    {
        page_update_camera = pageUpdateCam;
        button_idx_to_highlight = btnIdx;
    }
};

// Indicates a button index value on a given page where there is no button to animate with a glowing outline
const s16 NO_SELECTABLE_BUTTONS = -1;

class MainMenuController final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT MainMenuController* ctor_4CE9A0(Path_TLV* pTlv, TlvItemInfoUnion tlvOffsetLevelIdPathId);
    EXPORT BaseGameObject* vdtor_4CEF00(s32 flags);
    EXPORT void dtor_4CEF30();

    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override
    { }

    EXPORT void Render_4CF4C0(PrimHeader** ppOt);
    EXPORT void Update_4CF010();

    EXPORT static s32 CCSTD GetPageIndexFromCam_4D05A0(s32 camId);

    enum MainMenuGamespeakAnimIds
    {
        eAbe_Idle = 1,
        eAbe_IdleBlinking = 2,
        eAbe_EnterThroughDoor = 9,
        eAbe_EnterThroughDoor2 = 10,
        eAbe_EnterThroughDoorHello = 11,
        eAbe_GoodBye = 6,
        eAbe_Hello = 3,
        eAbe_OK = 4,
        eAbe_FollowMe = 5,
        eAbe_Chant = 12,
        eAbe_ChantEnd = 13,
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
        eGlukkon_IdleVariation = 28,
        eGlukkon_Hey = 35,
        eGlukkon_Commere = 32,
        eGlukkon_StayHere = 31,
        eGlukkon_DoIt = 33,
        eGlukkon_KillEm = 29,
        eGlukkon_AllOYa = 36,
        eGlukkon_Help = 34,
        eGlukkon_Laugh = 30,

        eScrab_IdleVariation = 38,
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
        eParamite_IdleVariation = 42, // Randomly moves its tenticles every so often
    };

    EXPORT void Set_Anim_4D05E0(s16 a2, s16 a3 = 0);
    EXPORT void Load_Anim_Pal_4D06A0(Animation* pAnim);
    EXPORT s32 ChangeScreenAndIntroLogic_4CF640();
    EXPORT void AnimationAndSoundLogic_4CFE80();
    EXPORT void UpdateHighliteGlow_4D0630();
    EXPORT static void callback_4D06E0(MainMenuController* a1);
    EXPORT static void DrawMenuText_4D20D0(const MainMenuText* array, PrimHeader** ot, Alive::Font* font, s32* polyIndex, s8 a5);

    // Page Functions
    EXPORT void AbeSpeak_Render_4D2060(PrimHeader** ot);
    EXPORT MainMenuNextCam AbeSpeak_Update_4D2D20(u32 input_held);

    EXPORT void SligSpeak_Render_4D2370(PrimHeader** ot);
    EXPORT MainMenuNextCam SligSpeak_Update_4D3280(u32 input_held);
    EXPORT void SligSpeak_Unload_4D3170();
    EXPORT void SligSpeak_Load_4D3090();

    EXPORT void GlukkonSpeak_Render_4D23C0(PrimHeader** ot);
    EXPORT MainMenuNextCam GlukkonSpeak_Update_4D3670(u32 input_held);
    EXPORT void GlukkonSpeak_Unload_4D3560();
    EXPORT void GlukkonSpeak_Load_4D3480();

    EXPORT void ScrabSpeak_Render_4D2410(PrimHeader** ot);
    EXPORT MainMenuNextCam ScrabSpeak_Update_4D3A60(u32 input_held);
    EXPORT void ScrabSpeak_Unload_4D3950();
    EXPORT void ScrabSpeak_Load_4D3870();

    EXPORT void ParamiteSpeak_Render_4D2460(PrimHeader** ot);
    EXPORT MainMenuNextCam ParamiteSpeak_Update_4D3D60(u32 input_held);
    EXPORT void ParamiteSpeak_Unload_4D3C50();
    EXPORT void ParamiteSpeak_Load_4D3B70();

    EXPORT MainMenuNextCam Gamespeak_Update_4D1FC0(u32 input_held);

    void RenderOnScreenTextHelper(PrimHeader** ot, const MainMenuText* menuTextArray, s32 count, s8 isSingleChar);

    EXPORT void AbeMotions_Render_4D25E0(PrimHeader** ot);
    EXPORT void Gamespeak_Render_4D24F0(PrimHeader** ot);
    EXPORT void Options_Render_4D2540(PrimHeader** ot);
    EXPORT void BackStory_Or_NewGame_Render_4D2630(PrimHeader** ot);

    EXPORT void ControllerMenu_Render_Text_4D26C0(PrimHeader** pOt);

    EXPORT void Demo_And_FMV_List_Render_4D4F30(PrimHeader** ppOt);

    EXPORT void t_Unload_AbeSpeak_Res_4D49F0();

    EXPORT void t_Load_AbeSpeak_Res_4D4A20();

    EXPORT MainMenuNextCam Page_FMV_Level_Update_4D4AB0(u32 input_held);

    void Unload_AbeSpeakResources();
    void Load_AbeSpeakResources();

    // Front End
    EXPORT MainMenuNextCam Page_Front_Update_4D0720(u32 input);
    EXPORT void Page_Front_Render_4D24B0(PrimHeader** ot);

    EXPORT MainMenuNextCam LoadNewGame_Update_4D0920(u32 input);

    EXPORT MainMenuNextCam BackStory_Or_NewGame_Update_4D1C60(u32 input_held);
    EXPORT void BackStory_Or_NewGame_Load_4D1BA0();
    EXPORT void BackStory_Or_NewGame_Unload_4D1BE0();

    EXPORT MainMenuNextCam LoadDemo_Update_4D1040(u32 input);
    EXPORT MainMenuNextCam DemoSelect_Update_4D0E10(u32 input);
    EXPORT MainMenuNextCam Options_Update_4D1AB0(u32 input);

    EXPORT MainMenuNextCam AbeMotions_Update_4D1F50(u32 input);

    EXPORT MainMenuNextCam PSX_Cooperative_Mode_Update_4D49B0(u32 input);

    EXPORT MainMenuNextCam PSX_Gamemode_Selection_Update_4D48C0(u32 input);

    EXPORT void RemapInput_Load_4D17E0();

    EXPORT void ControllerMenu_Load_4D16B0();

    EXPORT void RemapInput_Render_4D2A10(PrimHeader** ot);

    EXPORT MainMenuNextCam ControllerMenu_Update_4D16D0(u32 input);

    EXPORT MainMenuNextCam RemapInput_Update_4D1820(u32 input);

    EXPORT MainMenuNextCam tLoadGame_Input_4D3EF0(u32 input);
    EXPORT void tLoadGame_Render_4D44D0(PrimHeader** pOt);

    EXPORT void tLoadGame_Load_4D42F0();
    EXPORT static s8 CC checkIfDemoFileExists_4D1430(char_type* path);
    EXPORT static void CC remove_ISO9660_Suffix_4D1660(char_type* out, const char_type* in);

    EXPORT void tLoadGame_Unload_4D4360();

    // "Quit" page load
    EXPORT void Game_Force_Quit_Load_4D1A90();

    static MainMenuController* gMainMenuController;

private:
    MainMenuNextCam HandleGameSpeakInput(u32 input_held, std::function<MainMenuNextCam(InputCommands::Enum cmd)> fnOnGameSpeak);

    void HandleCreditsControllerUpdate();
    void HandleMainMenuUpdate();

public:
    enum Flags
    {
        eBit1_padding = 0x1,
        eBit2_padding = 0x2,
        eBit3_padding = 0x4,
        eBit4_padding = 0x8,
        eBit5_padding = 0x10,
        eBit6_padding = 0x20,
        eBit7_padding = 0x40,
        eBit8_padding = 0x80,
        eBit9_padding = 0x100,
        eBit10_padding = 0x200,
        eBit11_padding = 0x400,
        eBit12_padding = 0x800,
        eBit13_padding = 0x1000,
        eBit14_padding = 0x2000,
        eBit15_unused = 0x4000,
        eBit16_padding = 0x8000,
        eBit17_bDisableChangingSelection = 0x10000,
        eBit18_Loading = 0x20000,
        eBit19_unused = 0x40000,
        eBit20_padding = 0x80000,
        eBit21_LoadingSave = 0x100000,
        eBit22_GameSpeakPlaying = 0x200000,
        eBit23_unused = 0x400000,
        eBit24_Chant_Seq_Playing = 0x800000,
        eBit25_CheatLevelSelectLoading = 0x1000000,
    };


    MainMenuControllerResources field_F4_resources;
    Alive::Font field_120_font;
    Animation field_158_animation;
    s32 field_1F0_tlvOffsetLevelIdPathId;
    s32 field_1F4_credits_next_frame;
    s32 field_1F8_page_timeout; // Timer for starting demos automatically / backing out of menus
    s16 field_1FC_button_index;
    s16 field_1FE_highlite_alpha;
    s16 field_200_highlite_glow_speed;
    s16 field_202_input_hold_down_timer;
    u32 field_204_prev_pressed;
    MainMenuTransition* field_208_transition_obj;

    // These appear to never be used
    BaseGameObject* field_20C_pUnused;
    BaseGameObject* field_210_pUnused;

    s16 field_214_page_index;
    s16 field_216_previous_page_index;
    s16 field_218_target_page_index;
    s16 field_21A_target_button_index;
    camTransEffectState field_21C_camSwapEffectState;
    s16 field_21E_changeScreenState;
    s16 field_220_frame_table_idx;
    s8 field_222_unused;
    s32 field_224_timer_anim_delay;
    s16 field_228_res_idx;
    s16 field_22A_anim_frame_num;
    s32 field_22C_T80_animation_delay;
    s16 field_230_target_entry_index;
    s16 field_232_unused;
    const char_type* field_234_pStr;
    s16 field_238_unused;
    s16 field_23A_Inside_LoadGame_Screen;
    BitField32<Flags> field_23C_T80;
    s32 field_240_credits_current_cam;
    LevelIds field_244_lvl_id;
    s16 field_246_path_id;
    s16 field_248_camera;
    s16 field_24A_abeXOff;
    s16 field_24C_abeYOff;
    s16 field_24E_start_scale;
    s16 field_250_selected_entry_index;
    s16 field_252_unused;
    FP field_254;
    FP field_258;
    s16 field_25C_Inside_FMV_Screen;
    s16 field_25E_Inside_CheatLevelSelect_Screen;

private:
    void Unload_Resource(u8**& res);
};
ALIVE_ASSERT_SIZEOF(MainMenuController, 0x260);

using TFnInput = MainMenuNextCam (MainMenuController::*)(u32);
using TFnRender = void (MainMenuController::*)(PrimHeader**);
using TFnLoad = void (MainMenuController::*)();
using TFnUnLoad = void (MainMenuController::*)();


struct MainMenuPage final
{
    s16 field_0_cam_id;
    s16 field_2_pad;
    s32 field_4_time_out;
    s16 field_8_next_idx;
    s16 field_A_transition_effect;
    s16 field_C_initial_button_idx_to_highlight; // which button index to put the cursor on upon page opening
    s16 field_E_show_character;
    TFnInput field_10_fn_update;
    TFnRender field_14_fn_render;
    MainMenuButton* field_18_buttons;
    TFnLoad field_1C_fn_on_load;
    TFnUnLoad field_20_fn_on_free;
};

struct PerPathMudStats final
{
    s8 mData[21];
};

ALIVE_VAR_EXTERN(s32, sMainMenuObjectCounter_BB4400);

ALIVE_VAR_EXTERN(s8, sEnableCheatLevelSelect_5C1BEE);
ALIVE_VAR_EXTERN(s8, sEnableCheatFMV_5C1BEC);
ALIVE_VAR_EXTERN(PerPathMudStats, sSavedKilledMudsPerPath_5C1B50);
ALIVE_VAR_EXTERN(u32, sLevelId_dword_5CA408);
ALIVE_VAR_EXTERN(s32, sGameStartedFrame_5C1B88);
