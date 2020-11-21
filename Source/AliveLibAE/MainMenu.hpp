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

enum class LevelIds : __int16;

struct MenuFMV
{
    const char* field_0_name;
    LevelIds field_4_level_id;
    __int16 field_6;
    __int16 field_8;
    __int16 field_A_fmv_id;
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
    __int16 field_4_menu_res_id;
    __int16 field_6_sound;
    __int16 field_8_anim_enum;
    __int16 field_A_frame_number;
};

extern PerLvlData gPerLvlData_561700[17];

struct Path_TLV;
class MainMenuTransition;

class MainMenuController : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT MainMenuController* ctor_4CE9A0(Path_TLV* pTlv, TlvItemInfoUnion tlvOffsetLevelIdPathId);
    EXPORT BaseGameObject* vdtor_4CEF00(signed int flags);
    EXPORT void dtor_4CEF30();

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VScreenChanged() override { }

    EXPORT void Render_4CF4C0(int **ot);
    EXPORT void Update_4CF010();

    EXPORT static int CCSTD GetPageIndexFromCam_4D05A0(int camId);

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
        eParamite_Idle2 = 42, // Randomly moves its tenticles every so often

    };

    EXPORT void Set_Anim_4D05E0(__int16 a2, __int16 a3 = 0);
    EXPORT void Load_Anim_Pal_4D06A0(Animation* pAnim);
    EXPORT signed int ChangeScreenAndIntroLogic_4CF640();
    EXPORT void AnimationAndSoundLogic_4CFE80();
    EXPORT void UpdateHighliteGlow_4D0630();
    EXPORT static void callback_4D06E0(MainMenuController *a1);
    EXPORT static void DrawMenuText_4D20D0(const MainMenuText *array, int **ot, Alive::Font *font, int *polyIndex, char a5);

    // Page Functions
    EXPORT void AbeSpeak_Render_4D2060(int** ot);
    EXPORT signed int AbeSpeak_Update_4D2D20(DWORD input_held);

    EXPORT void SligSpeak_Render_4D2370(int** ot);
    EXPORT signed int SligSpeak_Update_4D3280(DWORD input_held);
    EXPORT void SligSpeak_Unload_4D3170();
    EXPORT void SligSpeak_Load_4D3090();

    EXPORT void GlukkonSpeak_Render_4D23C0(int** ot);
    EXPORT signed int GlukkonSpeak_Update_4D3670(DWORD input_held);
    EXPORT void GlukkonSpeak_Unload_4D3560();
    EXPORT void GlukkonSpeak_Load_4D3480();

    EXPORT void ScrabSpeak_Render_4D2410(int** ot);
    EXPORT signed int ScrabSpeak_Update_4D3A60(DWORD input_held);
    EXPORT void ScrabSpeak_Unload_4D3950();
    EXPORT void ScrabSpeak_Load_4D3870();

    EXPORT void ParamiteSpeak_Render_4D2460(int** ot);
    EXPORT signed int ParamiteSpeak_Update_4D3D60(DWORD input_held);
    EXPORT void ParamiteSpeak_Unload_4D3C50();
    EXPORT void ParamiteSpeak_Load_4D3B70();

    EXPORT signed int Gamespeak_Update_4D1FC0(DWORD input_held);

    void RenderOnScreenTextHelper(int ** ot, const MainMenuText * menuTextArray, int count, char isSingleChar);

    EXPORT void AbeMotions_Render_4D25E0(int **ot);
    EXPORT void Gamespeak_Render_4D24F0(int** ot);
    EXPORT void Options_Render_4D2540(int** ot);
    EXPORT void BackStory_Or_NewGame_Render_4D2630(int** ot);

    EXPORT void ControllerMenu_Render_Text_4D26C0(int **pOt);

    EXPORT void Demo_And_FMV_List_Render_4D4F30(int** pOt);

    EXPORT void t_Unload_AbeSpeak_Res_4D49F0();

    EXPORT void t_Load_AbeSpeak_Res_4D4A20();

    EXPORT signed int Page_FMV_Level_Update_4D4AB0(DWORD input_held);

    void Unload_AbeSpeakResources();
    void Load_AbeSpeakResources();

    // Front End
    EXPORT signed int Page_Front_Update_4D0720(DWORD input);
    EXPORT void Page_Front_Render_4D24B0(int **ot);

    EXPORT signed int LoadNewGame_Update_4D0920(DWORD input);

    EXPORT signed int BackStory_Or_NewGame_Update_4D1C60(DWORD input_held);
    EXPORT void BackStory_Or_NewGame_Load_4D1BA0();
    EXPORT void BackStory_Or_NewGame_Unload_4D1BE0();

    EXPORT signed int LoadDemo_Update_4D1040(DWORD input);
    EXPORT signed int DemoSelect_Update_4D0E10(DWORD input);
    EXPORT signed int Options_Update_4D1AB0(DWORD input);
    
    EXPORT signed int AbeMotions_Update_4D1F50(DWORD input);

    EXPORT signed int tsub_Input_Check_4D49B0(DWORD input);

    EXPORT signed int tsub_4D48C0(DWORD input);

    EXPORT void RemapInput_Load_4D17E0();

    EXPORT void ControllerMenu_Load_4D16B0();

    EXPORT void RemapInput_Render_4D2A10(int** ot);

    EXPORT signed int ControllerMenu_Update_4D16D0(DWORD input);

    EXPORT signed int RemapInput_Update_4D1820(DWORD input);

    EXPORT signed int tLoadGame_Input_4D3EF0(DWORD input);
    EXPORT void tLoadGame_Render_4D44D0( int **pOt);
    
    EXPORT void tLoadGame_Load_4D42F0();
    EXPORT static char CC checkIfDemoFileExists_4D1430(char *path);
    EXPORT static void CC remove_ISO9660_Suffix_4D1660(char* out, char* in);

    EXPORT void tLoadGame_Unload_4D4360();

    // "Quit" page load
    EXPORT void Game_Force_Quit_Load_4D1A90();

    static MainMenuController * gMainMenuController;
private:
    signed int HandleGameSpeakInput(DWORD input_held, std::function<signed int(InputCommands cmd)> fnOnGameSpeak);

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
    int field_1F0_tlvOffsetLevelIdPathId;
    int field_1F4_credits_next_frame;
    int field_1F8_page_timeout; // Timer for starting demos automatically / backing out of menus
    __int16 field_1FC_button_index;
    __int16 field_1FE_highlite_alpha;
    __int16 field_200_highlite_glow_speed;
    __int16 field_202_input_hold_down_timer;
    unsigned int field_204_prev_pressed;
    MainMenuTransition* field_208_transition_obj;

    // These appear to never be used
    BaseGameObject* field_20C_pUnused;
    BaseGameObject* field_210_pUnused;

    __int16 field_214_page_index;
    __int16 field_216_previous_page_index;
    __int16 field_218_target_page_index;
    __int16 field_21A_target_cam;
    __int16 field_21C_bDoScreenTransistionEffect;
    __int16 field_21E_bChangeScreen;
    __int16 field_220_frame_table_idx;
    char field_222_unused;
    int field_224_timer_anim_delay;
    __int16 field_228_res_idx;
    __int16 field_22A_anim_frame_num;
    int field_22C_T80_animation_delay;
    __int16 field_230_target_entry_index;
    __int16 field_232_unused;
    const char* field_234_pStr;
    __int16 field_238_unused;
    __int16 field_23A_Inside_LoadGame_Screen;
    BitField32<Flags> field_23C_T80;
    int field_240_credits_current_cam;
    LevelIds field_244_lvl_id;
    __int16 field_246_path_id;
    __int16 field_248_camera;
    __int16 field_24A_abeXOff;
    __int16 field_24C_abeYOff;
    __int16 field_24E_start_scale;
    __int16 field_250_selected_entry_index;
    __int16 field_252_unused;
    FP field_254;
    FP field_258;
    __int16 field_25C_Inside_FMV_Screen;
    __int16 field_25E_Inside_CheatLevelSelect_Screen;
private:
    void Unload_Resource(BYTE**& res);
};
ALIVE_ASSERT_SIZEOF(MainMenuController, 0x260);

using TFnInput = signed int (MainMenuController::*)(DWORD);
using TFnRender = void (MainMenuController::*)(int**);
using TFnLoad = void (MainMenuController::*)();
using TFnUnLoad = void (MainMenuController::*)();

struct MainMenuPage
{
    __int16 field_0_cam_id;
    __int16 field_2_pad;
    int field_4_time_out;
    __int16 field_8_next_idx;
    __int16 field_A_transistion_effect;
    __int16 field_C_target_camera;
    __int16 field_E_show_character;
    TFnInput field_10_fn_update;
    TFnRender field_14_fn_render;
    MainMenuButton *field_18_buttons;
    TFnLoad field_1C_fn_on_load;
    TFnUnLoad field_20_fn_on_free;
};

struct PerPathMudStats
{
    char mData[21];
};

ALIVE_VAR_EXTERN(int, sMainMenuObjectCounter_BB4400);

ALIVE_VAR_EXTERN(char, sEnableCheatLevelSelect_5C1BEE);
ALIVE_VAR_EXTERN(char, sEnableCheatFMV_5C1BEC);
ALIVE_VAR_EXTERN(PerPathMudStats, sSavedKilledMudsPerPath_5C1B50);
ALIVE_VAR_EXTERN(DWORD, sLevelId_dword_5CA408);
ALIVE_VAR_EXTERN(int, sGameStartedFrame_5C1B88);
