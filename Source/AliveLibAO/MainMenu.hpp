#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Primitives.hpp"
#include "Font.hpp"

namespace AO {

class MainMenuFade : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT MainMenuFade* ctor_42A5A0(s16 xpos, s16 ypos, u16 idx_1, s16 bDestroyOnDone);

    virtual void VUpdate() override;

    EXPORT void VUpdate_42A730();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_42A7A0(PrimHeader** ppOt);

    virtual BaseGameObject* VDestructor(s32 flags) override;

    s32 field_D4[4];
    s16 field_E4;
    s16 field_E6;
    s16 field_E8_bDestroyOnDone;
    s16 field_EA;
};
ALIVE_ASSERT_SIZEOF(MainMenuFade, 0xEC);

struct MainMenuTransition_Polys
{
    Poly_G3 field_0_polys[8];
};
ALIVE_ASSERT_SIZEOF(MainMenuTransition_Polys, 0x100);

// TODO: Move to own file
class MainMenuTransition : public BaseGameObject
{
public:
    EXPORT MainMenuTransition* ctor_436370(Layer layer, s16 fadeDirection, s16 bKillWhenDone, s16 speed, TPageAbr abr);

    EXPORT BaseGameObject* dtor_436500();

    virtual void VUpdate() override;

    EXPORT void VUpdate_4365C0();

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_4369D0();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT MainMenuTransition* Vdtor_4369F0(s32 flags);

    EXPORT void StartTrans_436560(Layer layer, s16 fadeDirection, s16 bKillWhenDone, s16 speed);

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_436610(PrimHeader** ppOt);

    s16 field_10_current_Value;
    s16 field_12_change_by_speed;
    s16 field_14_fade_direction;
    s16 field_16_bDone;
    s16 field_18_bKillOnDone;
    s16 field_1A;
    MainMenuTransition_Polys field_1C_polys[2];
    Prim_SetTPage field_21C_tPage[2];
    Layer field_23C_layer;
    s16 field_23E_width;
    s16 field_240_k120;
    s16 field_242_idx;
    s8 field_244;
    s8 field_245;
    s8 field_246_colour_fade_value;
    s8 field_247;
};
ALIVE_ASSERT_SIZEOF(MainMenuTransition, 0x248);

struct Path_TLV;
enum class LevelIds : s16;

enum class MainMenuOptions : s16
{
    eGameSpeak_0 = 0,
    eBegin_1 = 1,
    eQuit_2 = 2,
    eLoad_3 = 3,
    eOptions_4 = 4
};

enum class GameSpeakOptions : s16
{
    eWait_0 = 0,
    eHello_1 = 1,
    eFollowMe_2 = 2,
    eAngry_3 = 3,
    eWhistleHigh_4 = 4,
    eWhistleLow_5 = 5,
    eLaugh_6 = 6,
    eFart_7 = 7,
    eChant_8 = 8,
    eMainMenu_9 = 9
};

enum class OptionsMenuOptions : s16
{
    eController_0 = 0,
    eSound_1 = 1,
    eMainMenu_2 = 2
};

enum class RemapOptions : s16
{
    eRun_0 = 0,
    eSneak_1 = 1,
    eJump_2 = 2,
    eSpeakLeft_3 = 3,
    eAction_4 = 4,
    eThrow_5 = 5,
    eCrouch_6 = 6,
    eSpeakRight_7 = 7
};

enum class MotionsOptions : s16
{
    eMotions_0 = 0,
    eGameSpeak_1 = 1,
    eExit_2 = 2
};

enum class SoundOptions : s16
{
    eStereo_0 = 0,
    eMono_1 = 1,
    eExit_2 = 2
};

union MenuOption
{
    MainMenuOptions mainmenu;
    GameSpeakOptions gamespeak_menu;
    OptionsMenuOptions options_menu;
    RemapOptions remap_menu;
    MotionsOptions motions_menu;
    SoundOptions sound_menu;
    s16 raw;
};

class Menu : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Menu* ctor_47A6F0(Path_TLV* pTlv, s32 tlvInfo);

    EXPORT BaseGameObject* dtor_47AAB0();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT Menu* Vdtor_47FC40(s32 flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_47ABB0();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_47AC00(PrimHeader** ppOt);

    virtual void VScreenChanged() override;

    void ProgressInProgressFilesLoading();


    EXPORT void WaitForDoorToOpen_47B550();
    EXPORT void WaitForAbesHeadPoppingThroughDoor_47B5E0();
    EXPORT void AbePopThroughDoor_47B620();
    EXPORT void CopyRight_Update_47B4C0();
    EXPORT void FMV_Select_Update_47E8D0();
    EXPORT void SayHelloWaitForLoading_47B690();
    EXPORT void WaitForAbeSayHello_47B770();
    EXPORT void MainScreen_Update_47AF60();
    EXPORT void GoToSelectedMenuPage_47BC50();
    EXPORT void WaitForSpeakFinishAndStartChangeEffect_47BB90();
    EXPORT void ToNextMenuPage_47BD80();
    EXPORT void ToLoading_47B7E0();
    EXPORT void ToGameSpeak_Update_47D620();
    EXPORT void To_FMV_Or_Level_Select_Update_47EC30();
    EXPORT void Update_NoRefs_47E3C0();
    EXPORT void To_Load_Update_47D8E0();
    EXPORT void To_Options_Update_47C250();
    EXPORT void Loading_Update_47B870();
    EXPORT void NewGameStart_47B9C0();
    EXPORT void Options_Update_47BF90();
    EXPORT void Options_WaitForAbeSpeak_Update_47C280();
    EXPORT void Option_GoTo_Selected_Update_47C2C0();
    EXPORT void Options_To_Selected_After_Cam_Change_Update_47C330();
    EXPORT void To_Options_Controller_Update_47F2E0();
    EXPORT void To_Options_Sound_Update_47C6F0();
    EXPORT void To_MainScreen_Update_47BB60();
    EXPORT void Options_Sound_Update_47C420();
    EXPORT void Options_WaitForAbeSayOK_Update_47C720();
    EXPORT void Options_WaitForScreenTrans_Update_47C760();
    EXPORT void To_MainOptions_Screen_After_Camera_Change_Update_47C7A0();
    EXPORT void GameSpeak_Update_47CBD0();
    EXPORT void FMV_Or_Level_Select_To_Back_Update_47EC70();
    EXPORT void To_Credits_Update_47F140();
    EXPORT void Level_Cheat_To_Loading_Update_47ED50();
    EXPORT void Options_Controller_Update_47F210();
    EXPORT void GoTo_ControllerConfigure_Or_Back_AfterScreenTrans_Update_47F330();
    EXPORT void Goto_ConfigureController_OrSave_SettingIni_Update_47F380();
    EXPORT void To_ButtonRemap_Update_47F860();
    EXPORT void ButtonRemap_Update_47F6F0();
    EXPORT void To_LoadSave_Update_47DB10();
    EXPORT void LoadSave_Update_47DB40();
    EXPORT void SaveLoadFailed_Update_47DCD0();
    EXPORT void To_ShowAbeMotions_ChangeCamera_Update_47F8A0();
    EXPORT void To_ShowAbeMotions_SaveSettings_Update_47F8E0();
    EXPORT void To_ToggleMotions_Update_47C9E0();
    EXPORT void Credits_Update_47F190();
    EXPORT void CreditsEnd_BackTo_FMV_Or_Level_List_Update_47F170();

    EXPORT void Load_Update_47D760();
    void CycleGameSpeakIdleAnims();

    EXPORT static s32 CC StringsEqual_47DA20(const void* pStr1, const void* pStr2);

    // Compared via address
    EXPORT void ToggleMotions_Update_47C800();
    EXPORT void Toggle_Motions_Screens_Update_47C8F0();

    EXPORT void MotionsScreen_Back_Update_47CA10();

    EXPORT void Motions_ToOptions_Update_47CA50();

    EXPORT void To_MainScreenOrLoad_Update_47DA90();

    EXPORT void Load_BackToMainScreen_Update_47DA40();

    EXPORT void GameSpeakBack_WaitForAbeGoodbye_Update_47D5E0();

    EXPORT void GamespeakBack_WaitForScreenTrans_Update_47D650();

    EXPORT void GameSpeak_To_MainScreen_Update_47D690();

    EXPORT void ToggleMotions_Render_47CAB0(PrimHeader** ppOt);
    EXPORT void SaveLoadFailed_Render_47DCF0(PrimHeader** ppOt);
    EXPORT void ButtonRemap_Render_47F940(PrimHeader** ppOt);
    EXPORT void Options_Sound_Render_47C630(PrimHeader** ppOt);
    EXPORT void Options_Controller_Render_47F430(PrimHeader** ppOt);
    EXPORT void Empty_Render_47AC80(PrimHeader** ppOt);
    EXPORT void FMV_Or_Level_Select_Render_47EEA0(PrimHeader** ppOt);
    EXPORT void MainScreen_Render_47BED0(PrimHeader** ppOt);
    EXPORT void GameSpeak_Render_47D700(PrimHeader** ppOt);
    EXPORT void Render_NoRefs_47E5B0(PrimHeader** ppOt);
    EXPORT void Load_Render_47DDA0(PrimHeader** ppOt);
    EXPORT void Options_Render_47C190(PrimHeader** ppOt);

    EXPORT void FMV_Or_Level_Select_Back_Update_47ECB0();

    EXPORT static void CC OnResourceLoaded_47ADA0(Menu* pMenu);

    EXPORT static void CC RenderElement_47A4E0(s32 xpos, s32 ypos, s32 input_command, PrimHeader** ot, AliveFont* pFont, s32* pPolyOffset);

    s32 field_D4[4];
    u8** field_E4_res_array[6];
    AliveFont field_FC_font;
    Animation field_134_anim;

    using TUpdateFn = decltype(&Menu::WaitForDoorToOpen_47B550);

    TUpdateFn field_1CC_fn_update;

    using TRenderFn = decltype(&Menu::Empty_Render_47AC80);

    TRenderFn field_1D0_fn_render;

    s32 field_1D4_tlvInfo;
    s32 field_1D8_timer;
    s32 field_1DC_idle_input_counter;
    MenuOption field_1E0_selected_index;
    s16 field_1E2_rgb;
    s16 field_1E4_colour_counter;
    s16 field_1E6;
    MainMenuTransition* field_1E8_pMenuTrans;
    MainMenuFade* field_1EC_pObj1;
    MainMenuFade* field_1F0_pObj2;
    const s8* field_1F4_text;
    s16 field_1F8;
    s16 field_1FA;
    s16 field_1FC;
    s16 field_1FE;
    s16 field_200;
    s16 field_202;
    s16 field_204_flags;
    s16 field_206;
    s32 field_208_camera;
    s16 field_20C_bStartInSpecificMap;
    LevelIds field_20E_level;
    s16 field_210_path;
    s16 field_212_camera;
    s16 field_214_abe_xpos;
    s16 field_216_abe_ypos;
    s16 field_218_previous_fmv_or_level_selection;
    s16 field_21A;
    FP field_21C;
    FP field_220;
    s16 field_224_bToFmvSelect;
    s16 field_226_bToLevelSelect;
    FP field_228;
    FP field_22C;
    s32 field_230_bGoBack;
};
ALIVE_ASSERT_SIZEOF(Menu, 0x234);

ALIVE_VAR_EXTERN(s32, gMainMenuInstanceCount_9F2DE0);
ALIVE_VAR_EXTERN(u8, sJoyResId_50769C);

} // namespace AO
