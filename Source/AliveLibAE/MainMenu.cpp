#include "stdafx.h"
#include "MainMenu.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "PathData.hpp"
#include "DebugHelpers.hpp"
#include "StringFormatters.hpp"
#include "CreditsController.hpp"
#include "MusicController.hpp"
#include "Sfx.hpp"
#include "Sound/Midi.hpp"
#include "Abe.hpp"
#include "Psx.hpp"
#include "../relive_lib/Particle.hpp"
#include "CameraSwapper.hpp"
#include "Movie.hpp"
#include "MainMenuTransition.hpp"
#include "Text.hpp"
#include "Sound/Sound.hpp"
#include "Path.hpp"
#include "Abe.hpp"
#include "PauseMenu.hpp"
#include "Io.hpp"
#include "GameEnderController.hpp"
#include "Glukkon.hpp"
#include "LvlArchive.hpp"
#include "Sys.hpp"

MainMenuController* MainMenuController::gMainMenuController = nullptr;

ALIVE_VAR(1, 0xbb4400, s32, sMainMenuObjectCounter_BB4400, 0);
ALIVE_VAR(1, 0x5c1b92, s16, sMainMenuFontLoaded_5C1B92, 0);

u8 pal_560F80[] = {
    0x00, 0x00, 0x21, 0x84, 0x42, 0x88, 0x63, 0x8C, 0x84, 0x90,
    0xA5, 0x94, 0xE7, 0x9C, 0x08, 0x21, 0x29, 0x25, 0x4A, 0x29,
    0x6B, 0x2D, 0x8C, 0x31, 0xAD, 0x35, 0xEF, 0x3D, 0x10, 0x42,
    0x73, 0x4E};

ALIVE_VAR(1, 0x5c1bee, s8, sEnableCheatLevelSelect_5C1BEE, 0);
ALIVE_VAR(1, 0x5c1bec, s8, sEnableCheatFMV_5C1BEC, 0);

ALIVE_VAR(1, 0x5c1b9e, s16, sDemoIdChosenFromDemoMenu_5C1B9E, 0);

ALIVE_VAR(1, 0x561538, s16, sMenuItemCount_561538, 0);
ALIVE_VAR(1, 0x5C1B50, PerPathMudStats, sSavedKilledMudsPerZulag_5C1B50, {});

union DemoOrFmv
{
    PerLvlData* mDemoRec;
    MenuFMV* mFmvRec;
};

ALIVE_VAR(1, 0xbb4414, DemoOrFmv, pDemosOrFmvs_BB4414, {});
ALIVE_VAR(1, 0x5c2f68, const char_type, byte_5C2F68, 0);

// HACK HACK FIX ME - all of these buttons are in one contiguous array in the real game
// we need to replicate this as the game will access this array with the index of the PREVIOUS screen
// which will be out of bounds

MainMenuButton sBtnArray_FrontEnd_560FA0[] = {
    {MainMenuButtonType::eCircularSelectableButton, 33, 82, 0, AnimId::MenuHighlight_Circle},   // Begin
    {MainMenuButtonType::eCircularSelectableButton, 32, 104, 0, AnimId::MenuHighlight_Circle},  // Quit
    {MainMenuButtonType::eCircularSelectableButton, 337, 219, 0, AnimId::MenuHighlight_Circle}, // Load
    {MainMenuButtonType::eCircularSelectableButton, 337, 239, 0, AnimId::MenuHighlight_Circle}, // Options
    {MainMenuButtonType::eCircularSelectableButton, 33, 62, 0, AnimId::MenuHighlight_Circle},   // Gamespeak
    {0, 0, 0, 0, AnimId::None}};

MainMenuButton sBtnArray_Cheat_Level_Or_FMVSelection_560FE8[] = {
    {MainMenuButtonType::eUnused_3, 34, 65, 0, AnimId::MenuHighlight_Circle},
    {0, 0, 0, 0, AnimId::None}};

MainMenuButton sBtnArray_InputKeys_561000[] = {
    // first column
    {MainMenuButtonType::eUnused_2, 51, 119, 0, AnimId::MenuHighlight_Circle}, // Run
    {MainMenuButtonType::eUnused_2, 46, 151, 0, AnimId::MenuHighlight_Circle}, // Sneak
    {MainMenuButtonType::eUnused_2, 46, 182, 0, AnimId::MenuHighlight_Circle}, // Jump
    {MainMenuButtonType::eUnused_2, 55, 211, 0, AnimId::MenuHighlight_Circle}, // Speak I
    // second column
    {MainMenuButtonType::eUnused_2, 314, 119, 0, AnimId::MenuHighlight_Circle}, // Action
    {MainMenuButtonType::eUnused_2, 323, 151, 0, AnimId::MenuHighlight_Circle}, // Throw
    {MainMenuButtonType::eUnused_2, 323, 182, 0, AnimId::MenuHighlight_Circle}, // Fart/Roll
    {MainMenuButtonType::eUnused_2, 317, 211, 0, AnimId::MenuHighlight_Circle}, // Speak II
    {MainMenuButtonType::eUnused_3, 166, 240, 0, AnimId::MenuHighlight_Triangle},
    {0, 0, 0, 0, AnimId::None}};

MainMenuButton sBtnArray_Unknown_5610C0[] = {
    {MainMenuButtonType::eUnused_2, 150, 26, 0, AnimId::MenuHighlight_Square},
    {MainMenuButtonType::eUnused_2, 198, 26, 0, AnimId::MenuHighlight_Square},
    {MainMenuButtonType::eUnused_2, 292, 35, 0, AnimId::MenuHighlight_Square},
    {0, 0, 0, 0, AnimId::None}};

MainMenuButton sBtnArray_Game_BackStory_Or_NewGame_561420[] = {
    {MainMenuButtonType::eCircularSelectableButton, 59, 199, 0, AnimId::MenuHighlight_Circle},  // Show me what happened
    {MainMenuButtonType::eCircularSelectableButton, 248, 199, 0, AnimId::MenuHighlight_Circle}, // Just start the game
    {MainMenuButtonType::eUnused_3, 180, 240, 0, AnimId::MenuHighlight_Circle},
    {0, 0, 0, 0, AnimId::None}};

MainMenuButton sBtnArray_Gamespeak_561310[] = {
    {MainMenuButtonType::eCircularSelectableButton, 153, 122, 0, AnimId::MenuHighlight_Circle}, // Abe
    {MainMenuButtonType::eCircularSelectableButton, 153, 143, 0, AnimId::MenuHighlight_Circle}, // Slig
    {MainMenuButtonType::eCircularSelectableButton, 153, 164, 0, AnimId::MenuHighlight_Circle}, // Glukkon
    {MainMenuButtonType::eCircularSelectableButton, 153, 184, 0, AnimId::MenuHighlight_Circle}, // Scrab
    {MainMenuButtonType::eCircularSelectableButton, 153, 205, 0, AnimId::MenuHighlight_Circle}, // Paramite
    {MainMenuButtonType::eUnused_3, 313, 240, 0, AnimId::MenuHighlight_Triangle},
    {0, 0, 0, 0, AnimId::None}};

MainMenuButton sBtnArray_Options_561368[] = {
    {MainMenuButtonType::eCircularSelectableButton, 33, 62, 0, AnimId::MenuHighlight_Circle},  // Controls
    {MainMenuButtonType::eCircularSelectableButton, 336, 61, 0, AnimId::MenuHighlight_Circle}, // Demos
    {MainMenuButtonType::eUnused_3, 332, 240, 0, AnimId::MenuHighlight_Circle},
    {0, 0, 0, 0, AnimId::None}};

MainMenuButton sBtnArray_PSX_1Player_Or_2Player_NewGame_5613C8[] = {
    {MainMenuButtonType::eCircularSelectableButton, 33, 62, 0, AnimId::MenuHighlight_Circle},  // One player
    {MainMenuButtonType::eCircularSelectableButton, 336, 60, 0, AnimId::MenuHighlight_Circle}, // Two players
    {MainMenuButtonType::eUnused_3, 331, 240, 0, AnimId::MenuHighlight_Circle},
    {0, 0, 0, 0, AnimId::None}};

MainMenuButton sBtnArray_Cooperative_Mode_Prompt_5613F8[] = {
    {MainMenuButtonType::eCircularSelectableButton, 31, 62, 0, AnimId::MenuHighlight_Circle}, // Begin
    {MainMenuButtonType::eUnused_3, 331, 240, 0, AnimId::MenuHighlight_Circle},
    {0, 0, 0, 0, AnimId::None}};

MainMenuButton sBtnArray_LoadGame_561450[] = {
    {MainMenuButtonType::eCircularSelectableButton, 32, 62, 0, AnimId::MenuHighlight_Circle}, // probably used to be the select button, isn't actually rendered
    {MainMenuButtonType::eUnused_3, 331, 239, 0, AnimId::MenuHighlight_Circle},
    {0, 0, 0, 0, AnimId::None},
};



// Sfx Arrays

// TODO: repetition with `Paramite.cpp`
relive::SfxDefinition mainMenu_stru_55D7C0[] = {
    {0u, 5u, 60u, 90u, -520, -520},
    {0u, 5u, 61u, 75u, -520, -520},
    {0u, 5u, 62u, 50u, -520, -520},
    {0u, 5u, 64u, 40u, -520, -520},
    {0u, 5u, 65u, 40u, -520, -520},
    {0u, 5u, 66u, 30u, -520, -520},
    {0u, 5u, 66u, 50u, 0, 0},
    {0u, 5u, 67u, 40u, -520, -520},
    {0u, 5u, 68u, 30u, -520, -520},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0}};

ALIVE_ARY(1, 0x561960, MainMenuPage, 24, sMainMenuPages_561960,
          {{// Page 0: Controller selection menu
            MainMenuCams::eControllerSelectionCam,
            0, 0, 0, -1, 0,
            FALSE,
            &MainMenuController::ControllerMenu_Update_4D16D0,
            &MainMenuController::ControllerMenu_Render_Text_4D26C0,
            nullptr,
            &MainMenuController::ControllerMenu_Load_4D16B0,
            nullptr},
           {// Page 1: Input remapping menu
            MainMenuCams::eRemapInputsCam,
            0, 0, 0, -1, 0,
            FALSE,
            &MainMenuController::RemapInput_Update_4D1820,
            &MainMenuController::RemapInput_Render_4D2A10,
            nullptr,
            &MainMenuController::RemapInput_Load_4D17E0,
            nullptr},
           {// Page 2: Dummy blank screen for quitting
            MainMenuCams::eDummyBlankCam,
            0, 0, 0, -1, 0,
            FALSE,
            nullptr,
            nullptr,
            nullptr,
            &MainMenuController::Game_Force_Quit_Load_4D1A90,
            nullptr},
           {// Page 3: Main menu screen where abe says hello
            MainMenuCams::eMainMenuCam,
            0, 0, 0, -1, NO_SELECTABLE_BUTTONS,
            TRUE,
            &MainMenuController::Page_Front_Update_4D0720,
            &MainMenuController::Page_Front_Render_4D24B0,
            sBtnArray_FrontEnd_560FA0,
            nullptr,
            nullptr},
           {// Page 4: Gamespeak character selection screen
            MainMenuCams::eGamespeakCharacterSelectionCam,
            0, 900, 1, 0, 4,
            FALSE,
            &MainMenuController::Gamespeak_Update_4D1FC0,
            &MainMenuController::Gamespeak_Render_4D24F0,
            sBtnArray_Gamespeak_561310,
            nullptr,
            nullptr},
           {// Page 5: Options
            MainMenuCams::eOptionsCam,
            0, 900, 1, 0, 3,
            TRUE,
            &MainMenuController::Options_Update_4D1AB0,
            &MainMenuController::Options_Render_4D2540,
            sBtnArray_Options_561368,
            nullptr,
            nullptr},
           {// Page 6: Abe's motions
            MainMenuCams::eAbesMotionKeysCam,
            0, 1600, 3, 0, 0,
            FALSE,
            &MainMenuController::AbeMotions_Update_4D1F50,
            &MainMenuController::AbeMotions_Render_4D25E0,
            nullptr,
            nullptr,
            nullptr},
           {// Page 7: Load game menu
            MainMenuCams::eLoadGameMenuCam,
            0, 1000, 1, 0, 2,
            FALSE,
            &MainMenuController::tLoadGame_Input_4D3EF0,
            &MainMenuController::tLoadGame_Render_4D44D0,
            sBtnArray_LoadGame_561450,
            &MainMenuController::tLoadGame_Load_4D42F0,
            &MainMenuController::tLoadGame_Unload_4D4360},
           {// Page 8: Solo or Co-op newgame selection (PSX only, unused on PC)
            MainMenuCams::eRemapInputsCam,
            0, 900, 1, 0, 0,
            TRUE,
            &MainMenuController::PSX_Gamemode_Selection_Update_4D48C0,
            nullptr,
            sBtnArray_PSX_1Player_Or_2Player_NewGame_5613C8,
            nullptr,
            nullptr},
           {// Page 9: Cooperative mode (2-player) description screen (PSX only, unused on PC)
            MainMenuCams::eDummyBlankCam,
            0, 1800, 5, 0, 1,
            FALSE,
            &MainMenuController::PSX_Cooperative_Mode_Update_4D49B0,
            nullptr,
            sBtnArray_Cooperative_Mode_Prompt_5613F8,
            nullptr,
            nullptr},
           {// Page 10: FMV selection cheat menu
            MainMenuCams::eCheatMenu_SelectFMVCam,
            0, 0, 1, 0, 0,
            FALSE,
            &MainMenuController::Page_FMV_Level_Update_4D4AB0,
            &MainMenuController::Demo_And_FMV_List_Render_4D4F30,
            sBtnArray_Cheat_Level_Or_FMVSelection_560FE8,
            &MainMenuController::t_Unload_AbeSpeak_Res_4D49F0,
            &MainMenuController::t_Load_AbeSpeak_Res_4D4A20},
           {// Page 11: Level selection cheat menu
            MainMenuCams::eCheatMenu_SelectLevelCam,
            0, 0, 1, 0, 0,
            FALSE,
            &MainMenuController::Page_FMV_Level_Update_4D4AB0,
            &MainMenuController::Demo_And_FMV_List_Render_4D4F30,
            sBtnArray_Cheat_Level_Or_FMVSelection_560FE8,
            nullptr,
            nullptr},
           {// Page 12: Demo selection menu
            MainMenuCams::eDemoSelectionCam,
            0, 0, 1, 0, 1,
            FALSE,
            &MainMenuController::DemoSelect_Update_4D0E10,
            &MainMenuController::Demo_And_FMV_List_Render_4D4F30,
            nullptr,
            nullptr,
            nullptr},
           {// Page 13: "Game is loading" splash screen
            MainMenuCams::eGameIsLoading_ShaddapCam,
            0, 0, 0, -1, NO_SELECTABLE_BUTTONS,
            FALSE,
            &MainMenuController::LoadNewGame_Update_4D0920,
            nullptr,
            nullptr,
            nullptr,
            nullptr},
           {// Page 14: "Demo is loading" splash screen
            MainMenuCams::eDemoIsLoading_ShaddapCam,
            0, 0, 0, -1, NO_SELECTABLE_BUTTONS,
            FALSE,
            &MainMenuController::LoadDemo_Update_4D1040,
            nullptr,
            nullptr,
            nullptr,
            nullptr},
           {// Page 15: Unknown, possibly startup-related?
            MainMenuCams::eUnknown20Cam,
            0, 150, 25, 1, NO_SELECTABLE_BUTTONS, FALSE, nullptr, nullptr, nullptr, nullptr, nullptr},
           {// Page 16: Copyright boot screen
            MainMenuCams::eGameBootCopyrightSplashCam,
            0, 150, 1, 7, 0, FALSE, nullptr, nullptr, nullptr, nullptr, nullptr},
           {// Page 17: Unknown, possibly startup-related?
            MainMenuCams::eControllerSelectionCam,
            0, 150, 25, 1, NO_SELECTABLE_BUTTONS, FALSE, nullptr, nullptr, nullptr, nullptr, nullptr},
           {// Page 18: Abe Gamespeak screen
            MainMenuCams::eAbeGamespeakCam,
            0, 1600, 2, 0, 0,
            TRUE,
            &MainMenuController::AbeSpeak_Update_4D2D20,
            &MainMenuController::AbeSpeak_Render_4D2060,
            nullptr,
            nullptr,
            nullptr},
           {// Page 19: Slig Gamespeak screen
            MainMenuCams::eSligGamespeakCam,
            0, 1600, 2, 0, 1,
            TRUE,
            &MainMenuController::SligSpeak_Update_4D3280,
            &MainMenuController::SligSpeak_Render_4D2370,
            nullptr,
            &MainMenuController::SligSpeak_Load_4D3090,
            &MainMenuController::SligSpeak_Unload_4D3170},
           {// Page 20: Glukkon Gamespeak screen
            MainMenuCams::eGlukkonGamespeakCam,
            0, 1600, 2, 0, 2,
            TRUE,
            &MainMenuController::GlukkonSpeak_Update_4D3670,
            &MainMenuController::GlukkonSpeak_Render_4D23C0,
            nullptr,
            &MainMenuController::GlukkonSpeak_Load_4D3480,
            &MainMenuController::GlukkonSpeak_Unload_4D3560},
           {// Page 21: Scrab Gamespeak screen
            MainMenuCams::eScrabGamespeakCam,
            0, 1600, 2, 0, 3,
            TRUE,
            &MainMenuController::ScrabSpeak_Update_4D3A60,
            &MainMenuController::ScrabSpeak_Render_4D2410,
            nullptr,
            &MainMenuController::ScrabSpeak_Load_4D3870,
            &MainMenuController::ScrabSpeak_Unload_4D3950},
           {// Page 22: Paramite Gamespeak screen
            MainMenuCams::eParamiteGamespeakCam,
            0, 1600, 2, 0, 4,
            TRUE,
            &MainMenuController::ParamiteSpeak_Update_4D3D60,
            &MainMenuController::ParamiteSpeak_Render_4D2460,
            nullptr,
            &MainMenuController::ParamiteSpeak_Load_4D3B70,
            &MainMenuController::ParamiteSpeak_Unload_4D3C50},
           {// Page 23: Backstory Or Just Start a New Game screen
            MainMenuCams::eBackstory_Or_NewGameCam,
            0, 900, 1, 0, 0,
            FALSE,
            &MainMenuController::BackStory_Or_NewGame_Update_4D1C60,
            &MainMenuController::BackStory_Or_NewGame_Render_4D2630,
            sBtnArray_Game_BackStory_Or_NewGame_561420,
            &MainMenuController::BackStory_Or_NewGame_Load_4D1BA0,
            &MainMenuController::BackStory_Or_NewGame_Unload_4D1BE0}});

inline std::int16_t operator"" _s(unsigned long long value)
{
    return static_cast<std::int16_t>(value);
}

MenuFMV sFmvs_561540[28] = {
    {"GT Logo", EReliveLevelIds::eMenu, 65535_s, 65535_s, 3_s, 65535_s, 65535_s},
    {"Oddworld Intro", EReliveLevelIds::eMenu, 65535_s, 65535_s, 1, 65535_s, 65535_s},
    {"Abe's Exoddus", EReliveLevelIds::eMenu, 65535_s, 65535_s, 5, 65535_s, 65535_s},
    {"Backstory", EReliveLevelIds::eMenu, 65535_s, 65535_s, 4, 65535_s, 65535_s},
    {"Prophecy", EReliveLevelIds::eMines, 65535_s, 65535_s, 1, 65535_s, 65535_s},
    {"Vision", EReliveLevelIds::eMines, 65535_s, 65535_s, 24, 65535_s, 65535_s},
    {"Game Opening", EReliveLevelIds::eMines, 65535_s, 65535_s, 2, 65535_s, 65535_s},
    {"Brew", EReliveLevelIds::eMines, 65535_s, 65535_s, 26, 65535_s, 65535_s},
    {"Brew Transition", EReliveLevelIds::eMines, 65535_s, 65535_s, 31, 65535_s, 65535_s},
    {"Escape", EReliveLevelIds::eMines, 65535_s, 65535_s, 25, 65535_s, 65535_s},
    {"Reward", EReliveLevelIds::eNecrum, 65535_s, 65535_s, 22, 65535_s, 65535_s},
    {"FeeCo", EReliveLevelIds::eFeeCoDepot, 65535_s, 65535_s, 4, 65535_s, 65535_s},
    {"Information Booth", EReliveLevelIds::eFeeCoDepot, 65535_s, 65535_s, 3, 65535_s, 65535_s},
    {"Train 1", EReliveLevelIds::eBarracks, 65535_s, 65535_s, 5, 65535_s, 65535_s},
    {"Train 2", EReliveLevelIds::eBrewery, 65535_s, 65535_s, 15, 65535_s, 65535_s},
    {"Train 3", EReliveLevelIds::eBonewerkz, 65535_s, 65535_s, 6, 65535_s, 65535_s},
    {"Aslik Info", EReliveLevelIds::eFeeCoDepot, 65535_s, 65535_s, 2, 65535_s, 65535_s},
    {"Aslik Explodes", EReliveLevelIds::eFeeCoDepot, 65535_s, 65535_s, 1, 65535_s, 65535_s},
    {"Dripek Info", EReliveLevelIds::eBarracks, 65535_s, 65535_s, 4, 65535_s, 65535_s},
    {"Dripek Explodes", EReliveLevelIds::eBarracks, 65535_s, 65535_s, 3, 65535_s, 65535_s},
    {"Phleg Info", EReliveLevelIds::eBonewerkz, 65535_s, 65535_s, 4, 65535_s, 65535_s},
    {"Phleg Explodes", EReliveLevelIds::eBonewerkz, 65535_s, 65535_s, 5, 65535_s, 65535_s},
    {"Soulstorm Info", EReliveLevelIds::eBrewery, 65535_s, 65535_s, 14, 65535_s, 65535_s},
    {"Ingredient", EReliveLevelIds::eBrewery, 65535_s, 65535_s, 16, 65535_s, 65535_s},
    {"Conference", EReliveLevelIds::eBrewery, 65535_s, 65535_s, 13, 65535_s, 65535_s},
    {"Happy Ending", EReliveLevelIds::eBrewery, 65535_s, 65535_s, 17, 65535_s, 65535_s},
    {"Sad Ending", EReliveLevelIds::eBrewery, 65535_s, 65535_s, 18, 65535_s, 65535_s},
    {"Credits", EReliveLevelIds::eCredits, 65535_s, 65535_s, 65535_s, 65535_s, 65535_s}};

// Used by the level skip cheat/ui/menu
PerLvlData gPerLvlData_561700[17] = {
    {"Mines", EReliveLevelIds::eMines, 1, 4, 65535u, 2712, 1300},
    {"Mines Ender", EReliveLevelIds::eMines, 6, 10, 65535u, 2935, 2525},
    {"Necrum", EReliveLevelIds::eNecrum, 2, 1, 65535u, 2885, 1388},
    {"Mudomo Vault", EReliveLevelIds::eMudomoVault, 1, 1, 65535u, 110, 917},
    {"Mudomo Vault Ender", EReliveLevelIds::eMudomoVault_Ender, 13, 1, 65535u, 437, 454},
    {"Mudanchee Vault", EReliveLevelIds::eMudancheeVault, 6, 3, 65535u, 836, 873},
    {"Mudanchee Vault Ender", EReliveLevelIds::eMudancheeVault_Ender, 9, 4, 65534u, 1600, 550},
    {"FeeCo Depot", EReliveLevelIds::eFeeCoDepot, 1, 1, 65535u, 4563, 972},
    {"FeeCo Depot Ender", EReliveLevelIds::eFeeCoDepot_Ender, 11, 5, 65535u, 1965, 1650},
    {"Barracks", EReliveLevelIds::eBarracks, 1, 4, 65535u, 1562, 1651},
    {"Barracks Ender", EReliveLevelIds::eBarracks_Ender, 11, 5, 65535u, 961, 1132},
    {"Bonewerkz", EReliveLevelIds::eBonewerkz, 1, 1, 65535u, 813, 451},
    {"Bonewerkz Ender", EReliveLevelIds::eBonewerkz_Ender, 14, 10, 65535u, 810, 710},
    {"Brewery", EReliveLevelIds::eBrewery, 16, 6, 65535u, 1962, 1232},
    {"Game Ender", EReliveLevelIds::eBrewery_Ender, 1, 1, 65535u, 460, 968},
    {"Credits", EReliveLevelIds::eCredits, 1, 1, 65535u, 0, 0},
    {"Menu", EReliveLevelIds::eMenu, 1, 1, 65535u, 0, 0}};

PerLvlData sDemos_5617F0[23] = {
    {"Mudokons 1", EReliveLevelIds::eMines, 8, 5, 0, 0, 0},
    {"Mudokons 2", EReliveLevelIds::eMines, 8, 32, 1, 0, 0},
    {"Mudokons 3", EReliveLevelIds::eMines, 8, 21, 2, 0, 0},
    {"Flying Slig", EReliveLevelIds::eMines, 9, 18, 4, 0, 0},
    {"Blind Mudokons 1", EReliveLevelIds::eMines, 11, 27, 5, 0, 0},
    {"Blind Mudokons 2", EReliveLevelIds::eMines, 11, 22, 3, 0, 0},
    {"Minecar", EReliveLevelIds::eMines, 12, 2, 6, 0, 0},
    {"Fleeches", EReliveLevelIds::eNecrum, 7, 1, 7, 0, 0},
    {"Paramite Chase", EReliveLevelIds::eMudomoVault, 2, 13, 8, 0, 0},
    {"Paramites Talk", EReliveLevelIds::eMudomoVault, 6, 8, 27, 0, 0},
    {"Scrab and Fleeches", EReliveLevelIds::eMudancheeVault, 12, 2, 9, 0, 0},
    {"Invisibility", EReliveLevelIds::eMudancheeVault, 13, 5, 10, 0, 0},
    {"Farts-a-poppin'", EReliveLevelIds::eFeeCoDepot, 6, 3, 12, 0, 0},
    {"Flying Sligs 2", EReliveLevelIds::eFeeCoDepot, 12, 1, 14, 0, 0},
    {"Baggage Claim", EReliveLevelIds::eFeeCoDepot_Ender, 13, 1, 15, 0, 0},
    {"Shrykull", EReliveLevelIds::eBarracks, 3, 10, 16, 0, 0},
    {"Crawling Sligs", EReliveLevelIds::eBarracks, 4, 6, 17, 0, 0},
    {"Slogs Attack", EReliveLevelIds::eBonewerkz, 11, 7, 18, 0, 0},
    {"Glukkon", EReliveLevelIds::eBonewerkz_Ender, 13, 9, 19, 0, 0},
    {"Angry Mudokons", EReliveLevelIds::eBrewery, 13, 10, 22, 0, 0},
    {"Sligs", EReliveLevelIds::eBrewery, 26, 4, 23, 0, 0},
    {"Tortured Mudokons", EReliveLevelIds::eBrewery, 27, 7, 24, 0, 0},
    {"Greeters Go Boom", EReliveLevelIds::eBrewery, 28, 4, 25, 0, 0}};

MainMenuFrameTable sMainMenuFrameTable_561CC8[49] = {
    {AnimId::None, 0, 0, 0, 0},
    {AnimId::MenuAbeSpeak_Idle, 1, -1, 1, 0},
    {AnimId::MenuAbeSpeak_IdleBlink, 1, -1, 1, 0},
    {AnimId::MenuAbeSpeak_Hello, 1, 3, 1, 0},
    {AnimId::MenuAbeSpeak_Ok, 1, 12, 1, 14},
    {AnimId::MenuAbeSpeak_FollowMe, 0, 4, 1, 0},
    {AnimId::MenuAbeSpeak_Goodbye, 0, 11, 1, 14},
    {AnimId::MenuAbeSpeak_Wait, 0, 6, 1, 0},
    {AnimId::MenuAbeSpeak_Anger, 0, 5, 1, 14},
    {AnimId::MenuDoor, 3, -1, 10, 0},
    {AnimId::AbeIntro, 2, -1, 11, 0},
    {AnimId::MenuAbeSpeak_IdleBlink, 1, -1, 3, 0},
    {AnimId::MenuAbeSpeak_Chant, 0, -1, 0, 0},
    {AnimId::MenuAbeSpeak_ChantEnd, 0, -1, 1, 0},
    {AnimId::MenuAbeSpeak_Work, 0, 25, 1, 0},
    {AnimId::MenuAbeSpeak_AllYa, 0, 17, 1, 0},
    {AnimId::MenuAbeSpeak_StopIt, 0, 26, 1, 0},
    {AnimId::MenuAbeSpeak_Sympathy, 0, 27, 1, 14},
    {AnimId::MenuSligSpeak_Idle, 7, -1, 18, 0},
    {AnimId::MenuSligSpeak_Freeze, 7, 8, 18, 0},
    {AnimId::MenuSligSpeak_GetEm, 7, 2, 18, 0},
    {AnimId::MenuSligSpeak_HereBoy, 7, 1, 18, 0},
    {AnimId::MenuSligSpeak_Laugh, 7, 3, 18, 14},
    {AnimId::MenuSligSpeak_LookOut, 7, 6, 18, 14},
    {AnimId::MenuSligSpeak_Bs, 7, 5, 18, 0},
    {AnimId::MenuSligSpeak_SmoBs, 7, 7, 18, 0},
    {AnimId::MenuSligSpeak_Hi, 7, 0, 18, 0},
    {AnimId::MenuGlukkonSpeak_Idle, 8, -1, 27, 0},
    {AnimId::MenuGlukkonSpeak_IdleVariation, 8, -1, 27, 0},
    {AnimId::MenuGlukkonSpeak_KillEm, 8, 8, 27, 17},
    {AnimId::MenuGlukkonSpeak_Laugh, 8, 7, 27, 0},
    {AnimId::MenuGlukkonSpeak_StayHere, 8, 2, 27, 0},
    {AnimId::MenuGlukkonSpeak_Commere, 8, 3, 27, 17},
    {AnimId::MenuGlukkonSpeak_DoIt, 8, 1, 27, 17},
    {AnimId::MenuGlukkonSpeak_Help, 8, 6, 27, 17},
    {AnimId::MenuGlukkonSpeak_Hey, 8, 0, 27, 0},
    {AnimId::MenuGlukkonSpeak_AllOYa, 8, 4, 27, 0},
    {AnimId::MenuScrabSpeak_Idle, 9, -1, 37, 0},
    {AnimId::MenuScrabSpeak_IdleVariation, 9, -1, 37, 0},
    {AnimId::MenuScrabSpeak_ShredPower, 9, 8, 37, 0},
    {AnimId::MenuScrabSpeak_Howl, 9, 0, 37, 0},
    {AnimId::MenuParamiteSpeak_Idle, 10, -1, 41, 0},
    {AnimId::MenuParamiteSpeak_IdleVariation, 10, -1, 41, 0},
    {AnimId::MenuParamiteSpeak_Attack, 10, 9, 41, 0},
    {AnimId::MenuParamiteSpeak_Howdy, 10, 5, 41, 14},
    {AnimId::MenuParamiteSpeak_Cmon, 10, 0, 41, 14},
    {AnimId::MenuParamiteSpeak_Stay, 10, 1, 41, 14},
    {AnimId::MenuParamiteSpeak_DoIt, 10, 2, 41, 14},
    {AnimId::MenuParamiteSpeak_AllAYa, 10, 0, 41, 14}};

#if DEVELOPER_MODE
bool gBootToLoadScreen = false;
#endif

void MainMenuController::LoadAnimations()
{
    for (auto& info : sMainMenuFrameTable_561CC8)
    {
        if (info.field_0_animation != AnimId::None)
        {
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(info.field_0_animation));
        }
    }

    const AnimId kMenuAnims[] = {
        AnimId::MenuHighlight_Circle,
        AnimId::MenuHighlight_ButtonRemapSquare,
        AnimId::MenuHighlight_Square,
        AnimId::MenuHighlight_Triangle,
        AnimId::OptionChantOrb_Particle
    };
    for (const auto& animId : kMenuAnims)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
    }

}

MainMenuController::MainMenuController(relive::Path_TLV* /*pTlv*/, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    sMainMenuObjectCounter_BB4400++;
    mBaseGameObjectFlags.Set(BaseGameObject::eUpdateDuringCamSwap_Bit10);

    LoadAnimations();

    gMainMenuController = this;

    if (!sMainMenuFontLoaded_5C1B92)
    {
        sFont1Context_5BC5C8.LoadFontType_433400(FontType::PauseMenu);
        sMainMenuFontLoaded_5C1B92 = 1;
    }

    field_120_font.ctor_433590(240, pal_560F80, &sFont1Context_5BC5C8);

    if (gMap.mCurrentCamera == MainMenuCams::eCheatMenu_SelectFMVCam)
    {
        Animation_Init(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
    }
    else
    {
        Animation_Init(GetAnimRes(AnimId::MenuDoor));
    }

    mAnim.mBlue = 127;
    mAnim.mGreen = 127;
    mAnim.mRed = 127;

    field_158_animation.Init(GetAnimRes(AnimId::MenuHighlight_Circle), this);

    field_158_animation.field_14_scale = mSpriteScale;

    field_158_animation.mFlags.Clear(AnimFlags::eBit16_bBlending);
    field_158_animation.mFlags.Set(AnimFlags::eBit15_bSemiTrans);

    field_158_animation.mRenderLayer = Layer::eLayer_MainMenuButtonBees_38;
    field_158_animation.mRenderMode = TPageAbr::eBlend_1;

    field_1FE_highlite_alpha = 40;
    field_200_highlite_glow_speed = -8;
    mTlvId = tlvId;

    field_214_page_index = static_cast<s16>(GetPageIndexFromCam_4D05A0(gMap.mCurrentCamera));
    field_21C_camSwapEffectState = camTransEffectState::eConstructed_1;
    field_21E_changeScreenState = 0;
    field_1F8_page_timeout = 0;
    field_220_frame_table_idx = 1;
    field_228_res_idx = 0;
    field_22A_anim_frame_num = 0;
    field_22C_T80_animation_delay = 0;
    field_224_timer_anim_delay = 0;
    field_202_input_hold_down_timer = 15;
    field_204_prev_pressed = 0;
    field_230_target_entry_index = 0; // Double check

    field_23C_T80.Clear(Flags::eBit17_bDisableChangingSelection);
    field_23C_T80.Clear(Flags::eBit18_Loading);
    field_23C_T80.Clear(Flags::eBit22_GameSpeakPlaying);
    field_23C_T80.Clear(Flags::eBit23_unused);
    field_23C_T80.Clear(Flags::eBit24_Chant_Seq_Playing);

    if (gMap.mCurrentCamera == MainMenuCams::eMainMenuCam)
    {
        MainMenuController::Set_Anim_4D05E0(eAbe_EnterThroughDoor, 0);
        field_23C_T80.Set(Flags::eBit17_bDisableChangingSelection);
#if DEVELOPER_MODE
        if (gBootToLoadScreen)
        {
            // So we don't have to wait all year for abe to mSay hello
            field_23C_T80.Clear(Flags::eBit17_bDisableChangingSelection);
        }
#endif
    }

    field_23C_T80.Clear(Flags::eBit19_unused);
    field_23C_T80.Clear(Flags::eBit21_LoadingSave);
    field_23C_T80.Clear(Flags::eBit25_CheatLevelSelectLoading);

    field_1FC_button_index = 0;
    field_208_transition_obj = 0;
    field_20C_pUnused = nullptr;
    field_210_pUnused = nullptr;
    field_23A_Inside_LoadGame_Screen = 0;
    field_234_pStr = &byte_5C2F68;
    field_238_unused = 0;
    field_25C_Inside_FMV_Screen = 0;
    field_25E_Inside_CheatLevelSelect_Screen = 0;

    sSavedKilledMudsPerZulag_5C1B50 = {};

    sEnableCheatFMV_5C1BEC = 0;
    sEnableCheatLevelSelect_5C1BEE = 0;
    sKilledMudokons_5C1BC0 = 0;
    sRescuedMudokons_5C1BC2 = 0;
    gAttract_5C1BA0 = 0;
    sSavedKilledMudsPerZulag_5C1B50.mData[ALIVE_COUNTOF(sSavedKilledMudsPerZulag_5C1B50.mData) - 1] = 0;
    sFeeco_Restart_KilledMudCount_5C1BC6 = 0;
    sFeecoRestart_SavedMudCount_5C1BC8 = 0;

    if (gMap.mCurrentCamera == MainMenuCams::eCheatMenu_SelectFMVCam)
    {
        field_1FC_button_index = 0;
        field_250_selected_entry_index = 0;
        field_254 = FP_FromInteger(0);
        field_258 = FP_FromInteger(0);
        field_25C_Inside_FMV_Screen = 1;
        pDemosOrFmvs_BB4414.mFmvRec = &sFmvs_561540[0];
        sMenuItemCount_561538 = ALIVE_COUNTOF(sFmvs_561540);
        mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
        Load_Anim_Pal_4D06A0(&mAnim);
        return;
    }

    if (gMap.mCurrentCamera == MainMenuCams::eDemoSelectionCam)
    {
        ResourceManager::Reclaim_Memory_49C470(0);
        pResourceManager_5C1BB0->LoadingLoop_465590(false);
        field_1FC_button_index = 0;
        field_250_selected_entry_index = sDemoIdChosenFromDemoMenu_5C1B9E;
        field_254 = FP_FromInteger(0);
        field_258 = FP_FromInteger(0);
        pDemosOrFmvs_BB4414.mDemoRec = &sDemos_5617F0[0];
        sMenuItemCount_561538 = ALIVE_COUNTOF(sDemos_5617F0);
        field_230_target_entry_index = sDemoIdChosenFromDemoMenu_5C1B9E;
        mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
        Load_Anim_Pal_4D06A0(&mAnim);
    }
}

MainMenuController::~MainMenuController()
{
    Path::TLV_Reset(mTlvId, -1, 0, 0);
    field_158_animation.VCleanUp();

    sMainMenuObjectCounter_BB4400--;

    field_120_font.dtor_433540();
}

void MainMenuController::VRender(PrimHeader** ppOt)
{
    if (mAnim.mFlags.Get(AnimFlags::eBit2_Animate)
        && sMainMenuPages_561960[field_214_page_index].field_E_show_character
        && mAnim.mFlags.Get(AnimFlags::eBit3_Render))
    {
        mAnim.VRender(184, 162, ppOt, 0, 0);
        PSX_RECT pRect = {};
        mAnim.Get_Frame_Rect(&pRect);
        pScreenManager->InvalidateRectCurrentIdx(pRect.x, pRect.y, pRect.w, pRect.h);
    }

    const MainMenuButton* pButtons = sMainMenuPages_561960[field_214_page_index].field_18_buttons;
    if (pButtons)
    {
        if (!field_23C_T80.Get(Flags::eBit17_bDisableChangingSelection))
        {
            if (field_1FC_button_index != NO_SELECTABLE_BUTTONS)
            {
                field_158_animation.VRender(pButtons[field_1FC_button_index].field_2_x, pButtons[field_1FC_button_index].y, ppOt, 0, 0);
                PSX_RECT rect = {};
                field_158_animation.Get_Frame_Rect(&rect);
                pScreenManager->InvalidateRectCurrentIdx(rect.x, rect.y, rect.w, rect.h);
            }
        }
    }

    auto renderFunc = sMainMenuPages_561960[field_214_page_index].field_14_fn_render;
    if (renderFunc)
    {
        (this->*renderFunc)(ppOt);
    }
}

// Main Menu Text Data
MainMenuText sMMT_FrontPage_5623A0[1] = {{35, 205, "x", 3u, 0u, 0u, 0u, 0.75, 0u, 0u, 0u, 0u}};

// Paramite speak text data
const MainMenuText sParamiteSpeak_5622C8[9] = {
    {153, 212, "esc", 3u, 0u, 0u, 0u, 0.75, 0u, 0u, 0u, 0u},
    {75, 55, kSpeak1, 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {240, 57, kSpeak2, 1u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {62, 85, kHello, 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {57, 117, kFollowMe, 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {57, 148, kWait, 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {66, 175, kWork, 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {308, 85, kAnger, 1u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {317, 115, kAllYa, 1u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u}};

const MainMenuText sAbeGameSpeak_561F38[12] = {
    {184, 25, kChant, 3u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {153, 212, "esc", 3u, 0u, 0u, 0u, 0.75, 0u, 0u, 0u, 0u},
    {75, 57, kSpeak1, 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {240, 57, kSpeak2, 1u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {55, 85, kHello, 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {52, 112, kFollowMe, 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {55, 151, kWait, 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {64, 177, kWork, 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {307, 85, kAnger, 1u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {314, 115, kAllYa, 1u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {319, 145, kSorry, 1u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {306, 175, kStopIt, 1u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u}};

const MainMenuText sSligGameSpeak_562058[11] = {
    {153, 212, "esc", 3u, 0u, 0u, 0u, 0.75, 0u, 0u, 0u, 0u},
    {75, 55, kSpeak1, 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {240, 57, kSpeak2, 1u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {55, 82, kHello, 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {51, 105, kFollowMe, 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {51, 144, kWait, 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {51, 171, kWork, 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {317, 84, kAnger, 1u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {317, 108, kAllYa, 1u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {317, 131, kSorry, 1u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {317, 173, kStopIt, 1u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u}};

const MainMenuText sGlukkonGameSpeak_562160[11] = {
    {153, 212, "esc", 3u, 0u, 0u, 0u, 0.75, 0u, 0u, 0u, 0u},
    {75, 55, kSpeak1, 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {240, 57, kSpeak2, 1u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {55, 80, kHello, 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {42, 107, kFollowMe, 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {43, 137, kWait, 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {53, 177, kWork, 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {317, 85, kAnger, 1u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {325, 117, kAllYa, 1u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {321, 147, kSorry, 1u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {313, 178, kStopIt, 1u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u}};

const MainMenuText sScrabGameSpeak_562268[4] = {
    {153, 215, "esc", 3u, 0u, 0u, 0u, 0.75, 0u, 0u, 0u, 0u},
    {75, 55, kSpeak1, 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {60, 86, kHello, 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u},
    {60, 133, kFollowMe, 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u}};

const MainMenuText sAbeMotions_562448[15] = {
    {55, 182, "X", 3u, 0u, 0u, 0u, 0.75, 0u, 0u, 0u, 0u},
    {153, 211, "esc", 3u, 0u, 0u, 0u, 0.75, 0u, 0u, 0u, 0u},
    {175, 60, kRight, 2u, 0u, 0u, 0u, 0.88f, 0u, 0u, 0u, 0u},             //walk right arrow        //rebindables are in []
    {175, 79, kSneak "+ " kRight, 2u, 0u, 0u, 0u, 0.88f, 0u, 0u, 0u, 0u}, //sneak [alt] + right arrow
    {175, 98, kRun "+ " kRight, 2u, 0u, 0u, 0u, 0.88f, 0u, 0u, 0u, 0u},   //run [shift] + right arrow
    {175, 117, kJump, 2u, 0u, 0u, 0u, 0.88f, 0u, 0u, 0u, 0u},             //jump [space]
    {175, 136, kRun "+" kJump, 2u, 0u, 0u, 0u, 0.88f, 0u, 0u, 0u, 0u},    //runjump [shift]+[space]
    {175, 158, kDown, 2u, 0u, 0u, 0u, 0.88f, 0u, 0u, 0u, 0u},             //crouch down arrow
    {330, 60, kRight, 2u, 0u, 0u, 0u, 0.88f, 0u, 0u, 0u, 0u},             //roll from crouch right arrow
    {330, 79, kUp, 2u, 0u, 0u, 0u, 0.88f, 0u, 0u, 0u, 0u},                //zturn up arrow
    {330, 98, kAction, 2u, 0u, 0u, 0u, 0.88f, 0u, 0u, 0u, 0u},            //action [ctrl]
    {334, 117, "Z+" kDPad, 2u, 0u, 0u, 0u, 0.88f, 0u, 0u, 0u, 0u},        //throw [z] + Dpad
    {330, 136, kFart, 2u, 0u, 0u, 0u, 0.88f, 0u, 0u, 0u, 0u},             //fart [x]
    {330, 155, kSorry, 2u, 0u, 0u, 0u, 0.88f, 0u, 0u, 0u, 0u},            //sympathy [7]
    {330, 174, kAnger, 2u, 0u, 0u, 0u, 0.88f, 0u, 0u, 0u, 0u},            //angry slap [5]
};

void MainMenuController::AbeSpeak_Render_4D2060(PrimHeader** ot)
{
    RenderOnScreenTextHelper(ot, &sAbeGameSpeak_561F38[0], ALIVE_COUNTOF(sAbeGameSpeak_561F38), 1);
}

MainMenuNextCam MainMenuController::AbeSpeak_Update_4D2D20(u32 input_held)
{
    // 8 is when returning to previous screen
    if (field_230_target_entry_index != 8 && field_23C_T80.Get(Flags::eBit24_Chant_Seq_Playing))
    {
        // Only 1 when chanting
        if (field_230_target_entry_index == 1 && (sGnFrame % 8) == 0)
        {
            // Spawn chant star/flare particle at random locations around abes head
            const s16 randX = Math_RandomRange(-40, 40) + 184;
            const s16 randY = Math_RandomRange(30, 90);

            const FP xpos = pScreenManager->CamXPos() + FP_FromDouble(randX);
            FP ypos = pScreenManager->CamYPos() + FP_FromDouble(randY);
            ypos.fpValue += 0x44D60C; // TODO: 68.83 ??
            Particle* pParticle = relive_new Particle(xpos,
                    ypos, GetAnimRes(AnimId::OptionChantOrb_Particle));

            if (pParticle)
            {
                pParticle->mAnim.mRenderMode = TPageAbr::eBlend_1;
                pParticle->mAnim.mRenderLayer = Layer::eLayer_Above_FG1_39;
            }
        }
        return MainMenuNextCam(MainMenuCams::eNoChange);
    }

    return HandleGameSpeakInput(input_held, [&](InputCommands::Enum cmd)
                                {
                                    switch (cmd)
                                    {
                                        case InputCommands::Enum::eChant:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eAbe_Chant);
                                            break;
                                        case InputCommands::Enum::eGameSpeak1:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eAbe_Hello);
                                            break;
                                        case InputCommands::Enum::eGameSpeak2:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eAbe_FollowMe);
                                            break;
                                        case InputCommands::Enum::eGameSpeak3:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eAbe_Wait);
                                            break;
                                        case InputCommands::Enum::eGameSpeak4:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eAbe_Work);
                                            break;
                                        case InputCommands::Enum::eGameSpeak5:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eAbe_Anger);
                                            break;
                                        case InputCommands::Enum::eGameSpeak6:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eAbe_AllYa);
                                            break;
                                        case InputCommands::Enum::eGameSpeak7:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eAbe_Sympathy);
                                            break;
                                        case InputCommands::Enum::eGameSpeak8:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eAbe_StopIt);
                                            break;
                                        case InputCommands::Enum::eBack:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eAbe_GoodBye);
                                            // Stop chanting music
                                            SND_SEQ_Stop(SeqId::MudokonChant1_10);

                                            // TODO: Extra case for Abe - recover the type
                                            if (field_20C_pUnused)
                                            {
                                                ALIVE_FATAL("Never expected to be used");
                                            }
                                            return MainMenuNextCam(MainMenuCams::eGamespeakCharacterSelectionCam);

                                        default:
                                            return MainMenuNextCam(MainMenuCams::eNoChange);
                                    }

                                    return MainMenuNextCam(MainMenuCams::eNoChange);
                                });
}

void MainMenuController::SligSpeak_Render_4D2370(PrimHeader** ot)
{
    RenderOnScreenTextHelper(ot, &sSligGameSpeak_562058[0], ALIVE_COUNTOF(sSligGameSpeak_562058), 1);
}

void MainMenuController::GlukkonSpeak_Render_4D23C0(PrimHeader** ot)
{
    RenderOnScreenTextHelper(ot, &sGlukkonGameSpeak_562160[0], ALIVE_COUNTOF(sGlukkonGameSpeak_562160), 1);
}

void MainMenuController::ScrabSpeak_Render_4D2410(PrimHeader** ot)
{
    RenderOnScreenTextHelper(ot, &sScrabGameSpeak_562268[0], ALIVE_COUNTOF(sScrabGameSpeak_562268), 1);
}

MainMenuNextCam MainMenuController::SligSpeak_Update_4D3280(u32 input_held)
{
    return HandleGameSpeakInput(input_held, [&](InputCommands::Enum cmd)
                                {
                                    switch (cmd)
                                    {
                                        case InputCommands::Enum::eGameSpeak1:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eSlig_Hi);
                                            break;
                                        case InputCommands::Enum::eGameSpeak2:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eSlig_HereBoy);
                                            break;
                                        case InputCommands::Enum::eGameSpeak3:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eSlig_Freeze);
                                            break;
                                        case InputCommands::Enum::eGameSpeak4:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eSlig_GetEm);
                                            break;
                                        case InputCommands::Enum::eGameSpeak5:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eSlig_SmoBs);
                                            break;
                                        case InputCommands::Enum::eGameSpeak6:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eSlig_Bs);
                                            break;
                                        case InputCommands::Enum::eGameSpeak7:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eSlig_LookOut);
                                            break;
                                        case InputCommands::Enum::eGameSpeak8:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eSlig_Laugh);
                                            break;
                                        case InputCommands::Enum::eBack:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eSlig_Laugh);
                                            return MainMenuNextCam(MainMenuCams::eGamespeakCharacterSelectionCam, 1);

                                        default:
                                            return MainMenuNextCam(MainMenuCams::eNoChange);
                                    }

                                    return MainMenuNextCam(MainMenuCams::eNoChange);
                                });
}

void MainMenuController::SligSpeak_Unload_4D3170()
{
    Unload_Resource(AnimId::MenuSligSpeak_Idle);
    Game_ShowLoadingIcon_482D80();
    Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eAbe_Idle);
}

void MainMenuController::SligSpeak_Load_4D3090()
{
    Game_ShowLoadingIcon_482D80();
    mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuSligSpeak_Idle));
    Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eSlig_Idle);
}

MainMenuNextCam MainMenuController::GlukkonSpeak_Update_4D3670(u32 input_held)
{
    return HandleGameSpeakInput(input_held, [&](InputCommands::Enum cmd)
                                {
                                    switch (cmd)
                                    {
                                        case InputCommands::Enum::eGameSpeak1:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eGlukkon_Hey);
                                            break;
                                        case InputCommands::Enum::eGameSpeak2:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eGlukkon_Commere);
                                            break;
                                        case InputCommands::Enum::eGameSpeak3:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eGlukkon_StayHere);
                                            break;
                                        case InputCommands::Enum::eGameSpeak4:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eGlukkon_DoIt);
                                            break;
                                        case InputCommands::Enum::eGameSpeak5:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eGlukkon_KillEm);
                                            break;
                                        case InputCommands::Enum::eGameSpeak6:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eGlukkon_AllOYa);
                                            break;
                                        case InputCommands::Enum::eGameSpeak7:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eGlukkon_Help);
                                            break;
                                        case InputCommands::Enum::eGameSpeak8:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eGlukkon_Laugh);
                                            break;
                                        case InputCommands::Enum::eBack:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eGlukkon_Laugh);
                                            return MainMenuNextCam(MainMenuCams::eGamespeakCharacterSelectionCam, 2);

                                        default:
                                            return MainMenuNextCam(MainMenuCams::eNoChange);
                                    }

                                    return MainMenuNextCam(MainMenuCams::eNoChange);
                                });
}

void MainMenuController::GlukkonSpeak_Unload_4D3560()
{
    Game_ShowLoadingIcon_482D80();
    Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eAbe_Idle);
}

void MainMenuController::GlukkonSpeak_Load_4D3480()
{
    Game_ShowLoadingIcon_482D80();
    mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuGlukkonSpeak_Idle));
    Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eGlukkon_Idle);
}

MainMenuNextCam MainMenuController::ScrabSpeak_Update_4D3A60(u32 input_held)
{
    return HandleGameSpeakInput(input_held, [&](InputCommands::Enum cmd)
                                {
                                    switch (cmd)
                                    {
                                        case InputCommands::Enum::eGameSpeak1:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eScrab_ShredPower);
                                            break;
                                        case InputCommands::Enum::eGameSpeak2:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eScrab_Howl);
                                            break;
                                        case InputCommands::Enum::eGameSpeak3:
                                            break; // Scrabs don't have much to mSay.
                                        case InputCommands::Enum::eGameSpeak4:
                                            break;
                                        case InputCommands::Enum::eGameSpeak5:
                                            break;
                                        case InputCommands::Enum::eGameSpeak6:
                                            break;
                                        case InputCommands::Enum::eGameSpeak7:
                                            break;
                                        case InputCommands::Enum::eGameSpeak8:
                                            break;
                                        case InputCommands::Enum::eBack:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eScrab_ShredPower);
                                            return MainMenuNextCam(MainMenuCams::eGamespeakCharacterSelectionCam, 3);

                                        default:
                                            return MainMenuNextCam(MainMenuCams::eNoChange);
                                    }

                                    return MainMenuNextCam(MainMenuCams::eNoChange);
                                });
}

void MainMenuController::ScrabSpeak_Unload_4D3950()
{
    Game_ShowLoadingIcon_482D80();
    Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eAbe_Idle);
}

void MainMenuController::ScrabSpeak_Load_4D3870()
{
    Game_ShowLoadingIcon_482D80();
    Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eScrab_Idle);
}

void MainMenuController::ParamiteSpeak_Render_4D2460(PrimHeader** ot)
{
    RenderOnScreenTextHelper(ot, &sParamiteSpeak_5622C8[0], ALIVE_COUNTOF(sParamiteSpeak_5622C8), 1);
}

MainMenuNextCam MainMenuController::ParamiteSpeak_Update_4D3D60(u32 input_held)
{
    return HandleGameSpeakInput(input_held, [&](InputCommands::Enum cmd)
                                {
                                    switch (cmd)
                                    {
                                        case InputCommands::Enum::eGameSpeak1:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eParamite_Howdy);
                                            break;
                                        case InputCommands::Enum::eGameSpeak2:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eParamite_Cmon);
                                            break;
                                        case InputCommands::Enum::eGameSpeak3:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eParamite_Stay);
                                            break;
                                        case InputCommands::Enum::eGameSpeak4:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eParamite_DoIt);
                                            break;
                                        case InputCommands::Enum::eGameSpeak5:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eParamite_Attack);
                                            break;
                                        case InputCommands::Enum::eGameSpeak6:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eParamite_AllAYa);
                                            break;
                                        case InputCommands::Enum::eGameSpeak7:
                                            break; // Paramites don't have as much to mSay.
                                        case InputCommands::Enum::eGameSpeak8:
                                            break;
                                        case InputCommands::Enum::eBack:
                                            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eParamite_Attack);
                                            return MainMenuNextCam(MainMenuCams::eGamespeakCharacterSelectionCam, 4);

                                        default:
                                            return MainMenuNextCam(MainMenuCams::eNoChange);
                                    }

                                    return MainMenuNextCam(MainMenuCams::eNoChange);
                                });
}

void MainMenuController::ParamiteSpeak_Unload_4D3C50()
{
    Game_ShowLoadingIcon_482D80();
    Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eAbe_Idle);
}

void MainMenuController::ParamiteSpeak_Load_4D3B70()
{
    Game_ShowLoadingIcon_482D80();
    mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuParamiteSpeak_Idle));
    Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eParamite_Idle);
}

static s32 DrawMenuStringWithShadow(PrimHeader** ppOt, Alive::Font& field_120_font, const char_type* text, s16 x, s16 y, u8 r, u8 g, u8 b, s32& polyOffset)
{
    s32 polyOff = polyOffset;
    polyOff = field_120_font.DrawString_4337D0(
        ppOt,
        text,
        x,
        y,
        TPageAbr::eBlend_0,
        1,
        0,
        Layer::eLayer_AbeMenu_32,
        r,
        g,
        b,
        polyOff,
        FP_FromInteger(1),
        640,
        0);

    polyOffset = field_120_font.DrawString_4337D0(
        ppOt,
        text,
        x + 2,
        y + 2,
        TPageAbr::eBlend_0,
        1,
        0,
        Layer::eLayer_AbeMenu_32,
        0,
        0,
        0,
        polyOff,
        FP_FromInteger(1),
        640,
        0);
    return polyOffset;
}

ALIVE_VAR(1, 0xBB43F0, FP, sTextYPos_BB43F0, {});
ALIVE_VAR(1, 0xBB43E4, FP, dword_BB43E4, {});

static void RenderScrollableTextEntries(
    PrimHeader** ot, s32& targetEntry, s32& selectedEntry, s32 totalItemsCount,
    FP& TextYPos, FP& TextYPos2, const char_type* field_234_pStr, const SaveFileRec* stringList, Alive::Font& field_120_font, s32& polyOffset)
{
    s32 i_start = 0;
    s32 i_end = 0;
    s32 entryPicker = selectedEntry;

    if (selectedEntry != targetEntry && TextYPos > FP_FromInteger(0))
    {
        entryPicker = targetEntry;
        selectedEntry = targetEntry;
    }

    if (selectedEntry == targetEntry)
    {
        if (TextYPos == FP_FromInteger(0))
        {
            i_start = -2;
            i_end = 2;
        }
        else if (TextYPos < FP_FromInteger(0))
        {
            TextYPos += TextYPos2;
            if (TextYPos >= FP_FromInteger(0))
            {
                TextYPos = FP_FromInteger(0);
                i_start = -2;
                i_end = 2;
            }
            else
            {
                TextYPos2 -= FP_FromDouble(0.2);
                if (TextYPos2 <= FP_FromInteger(0))
                {
                    TextYPos2 = FP_FromInteger(0);
                }
                i_start = -2;
                i_end = 3;
            }
        }
        else if (TextYPos - TextYPos2 > FP_FromInteger(0))
        {
            TextYPos -= TextYPos2;
            TextYPos2 -= FP_FromDouble(0.2);
            if (TextYPos2 <= FP_FromInteger(0))
            {
                TextYPos2 = FP_FromInteger(0);
            }
            i_start = -3;
            i_end = 2;
        }
        else
        {
            TextYPos = FP_FromInteger(0);
            i_start = -2;
            i_end = 2;
        }
    }
    else if (selectedEntry > targetEntry)
    {
        targetEntry = selectedEntry;
        TextYPos = FP_FromInteger(26) - FP_FromDouble(4.5);
        TextYPos2 = FP_FromDouble(4.5) - FP_FromDouble(0.2);
        i_start = -3;
        i_end = 2;
    }
    else if (selectedEntry < targetEntry)
    {
        targetEntry = selectedEntry;
        TextYPos = FP_FromDouble(4.5) - FP_FromInteger(26);
        TextYPos2 = FP_FromDouble(4.5) - FP_FromDouble(0.2);
        i_start = -2;
        i_end = 3;
    }

    for (s32 i = i_start; i <= i_end; i++)
    {
        s32 v9 = entryPicker + i;
        if (v9 >= 0 && v9 < totalItemsCount)
        {
            field_234_pStr = stringList[v9].field_0_fileName;
            s32 currEntryWidth = field_120_font.MeasureScaledTextWidth(field_234_pStr, FP_FromInteger(1));

            //Entry X alignment (of questionable quality) for long words
            s16 x = currEntryWidth >= 336 ? 16 : (368 - static_cast<s16>(currEntryWidth)) / 2;
            //TextYPos is entry height?
            s16 y = FP_GetExponent(TextYPos + FP_FromDouble(0.5)) + static_cast<s16>(26 * i) + 112;
            if (i)
            {
                //Draw non-highlighted entry
                polyOffset = DrawMenuStringWithShadow(ot, field_120_font, field_234_pStr, x, y, 210, 150, 80, polyOffset);
            }
            else
            {
                //Draw highlighted entry
                polyOffset = DrawMenuStringWithShadow(ot, field_120_font, field_234_pStr, x, y, 255, 218, 140, polyOffset);
            }
            entryPicker = selectedEntry;
        }
    }
}

const MainMenuText sControllerMenuButtonCaptions_Text_562628[3] = {
    {34, 27, "x", 3u, 0u, 0u, 0u, 0.75, 0u, 0u, 0u, 0u},
    {331, 204, "esc", 3u, 0u, 0u, 0u, 0.75, 0u, 0u, 0u, 0u},
    {32, 203, "c", 3u, 0u, 0u, 0u, 0.75, 0u, 0u, 0u, 0u}};

const SaveFileRec aKeyboard_1[2] = //used SaveFileRec as workaround for RenderScrollableTextEntries()
    {
        {"Keyboard", 0},
        {"Game Pad", 0}};

ALIVE_VAR(1, 0xBB43D8, s32, sControllerEntryToSelect_BB43D8, 0);
ALIVE_VAR(1, 0xBB43F4, s32, sSelectedControllerEntry_BB43F4, 0);
ALIVE_VAR(1, 0x55E838, s32, sControllerCount_55E838, ALIVE_COUNTOF(aKeyboard_1));

void MainMenuController::ControllerMenu_Render_Text_4D26C0(PrimHeader** ot)
{
    s32 polyOffset = 0;
    RenderScrollableTextEntries(ot, sControllerEntryToSelect_BB43D8, sSelectedControllerEntry_BB43F4, sControllerCount_55E838, sTextYPos_BB43F0, dword_BB43E4, field_234_pStr, aKeyboard_1, field_120_font, polyOffset);

    for (s32 i = 0; i < ALIVE_COUNTOF(sControllerMenuButtonCaptions_Text_562628); i++)
    {
        MainMenuController::DrawMenuText_4D20D0(&sControllerMenuButtonCaptions_Text_562628[i], ot, &field_120_font, &polyOffset, 1);
    }
}

ALIVE_VAR(1, 0xBB4418, s16, word_BB4418, 0);

void MainMenuController::Demo_And_FMV_List_Render_4D4F30(PrimHeader** ppOt)
{
    if (word_BB4418)
    {
        word_BB4418 = gCreditsControllerExists;
        return;
    }

    word_BB4418 = gCreditsControllerExists;

    if (field_230_target_entry_index != field_250_selected_entry_index)
    {
        if (field_254 != FP_FromInteger(0))
        {
            field_230_target_entry_index = field_250_selected_entry_index;
        }
        else if (field_230_target_entry_index <= field_250_selected_entry_index)
        {
            if (field_230_target_entry_index < field_250_selected_entry_index)
            {
                field_254 = FP_FromInteger(-26);
                field_258 = FP_FromInteger(6);
                field_250_selected_entry_index = field_230_target_entry_index;
            }
        }
        else
        {
            field_254 = FP_FromInteger(26);
            field_258 = FP_FromInteger(6);
            field_250_selected_entry_index = field_230_target_entry_index;
        }
    }

    if (field_254 > FP_FromInteger(0))
    {
        field_254 -= field_258;

        if (field_254 <= FP_FromInteger(0))
        {
            field_254 = FP_FromInteger(0);
        }
        else
        {
            field_258 -= FP_FromDouble(0.2);
            if (field_258 <= FP_FromInteger(0))
            {
                field_258 = FP_FromInteger(0);
            }
        }
    }
    else if (field_254 < FP_FromInteger(0))
    {
        field_254 += field_258;

        if (field_254 >= FP_FromInteger(0))
        {
            field_254 = FP_FromInteger(0);
        }
        else
        {
            field_258 -= FP_FromDouble(0.2);
            if (field_258 <= FP_FromInteger(0))
            {
                field_258 = FP_FromInteger(0);
            }
        }
    }

    const MainMenuText stru_5625F8[2] = {
        {32, 27, "x", 3u, 0u, 0u, 0u, 0.75f, 0u, 0u, 0u, 0u},
        {331, 204, "esc", 3u, 0u, 0u, 0u, 0.75f, 0u, 0u, 0u, 0u}};

    s32 polyIndex = 0;
    for (const MainMenuText& text : stru_5625F8)
    {
        DrawMenuText_4D20D0(&text, ppOt, &field_120_font, &polyIndex, 1);
    }

    s32 loopCount = -2;
    do
    {
        s32 idx = field_230_target_entry_index + loopCount;
        if (idx >= 0 && idx <= sMenuItemCount_561538 - 1)
        {
            field_234_pStr = pDemosOrFmvs_BB4414.mDemoRec[idx].field_0_display_name;
            s32 textWidth = field_120_font.MeasureScaledTextWidth(field_234_pStr, FP_FromInteger(1));
            s16 nextTextXPos = 0;
            if (textWidth >= 336)
            {
                nextTextXPos = 16;
            }
            else
            {
                nextTextXPos = static_cast<s16>((368 - textWidth) / 2);
            }

            const s16 textYPos = static_cast<s16>((FP_GetExponent(field_254 + FP_FromDouble(0.5))) + 26 * loopCount + 117);
            if (loopCount)
            {
                polyIndex = field_120_font.DrawString_4337D0(ppOt, field_234_pStr, nextTextXPos, textYPos, TPageAbr::eBlend_0, 1, 0, Layer::eLayer_AbeMenu_32, 210, 150, 80, polyIndex, FP_FromInteger(1), 640, 0);
            }
            else
            {
                polyIndex = field_120_font.DrawString_4337D0(ppOt, field_234_pStr, nextTextXPos, textYPos, TPageAbr::eBlend_0, 1, 0, Layer::eLayer_AbeMenu_32, 255, 218, 140, polyIndex, FP_FromInteger(1), 640, 0);
            }
            polyIndex = field_120_font.DrawString_4337D0(ppOt, field_234_pStr, nextTextXPos + 2, textYPos + 2, TPageAbr::eBlend_0, 1, 0, Layer::eLayer_AbeMenu_32, 0, 0, 0, polyIndex, FP_FromInteger(1), 640, 0);
        }
        loopCount++;
    }
    while (loopCount < 2);
}

void MainMenuController::t_Unload_AbeSpeak_Res_4D49F0()
{ 
    /*
    ResourceManager::FreeResource_49C330(field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak]);
    field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak] = nullptr;
    */
}


void MainMenuController::t_Load_AbeSpeak_Res_4D4A20()
{
    ResourceManager::Reclaim_Memory_49C470(0);
    field_25C_Inside_FMV_Screen = 0;
    field_25E_Inside_CheatLevelSelect_Screen = 0;
}

ALIVE_VAR(1, 0x55C128, s32, dword_55C128, 0);

MainMenuNextCam MainMenuController::Page_FMV_Level_Update_4D4AB0(u32 input_held)
{
    sEnableCheatFMV_5C1BEC = 0;
    sEnableCheatLevelSelect_5C1BEE = 0;

    if (sMovie_ref_count_BB4AE4 > 0)
    {
        // Do nothing if a movie is playing
        return MainMenuNextCam(MainMenuCams::eNoChange);
    }

    s32 inputToUse = 0;
    if (field_204_prev_pressed && field_204_prev_pressed == Input().mPads[sCurrentControllerIndex].mPressed)
    {
        field_202_input_hold_down_timer--;
        if (field_202_input_hold_down_timer == 0)
        {
            inputToUse = field_204_prev_pressed;
            field_202_input_hold_down_timer = 4;
        }
        else
        {
            inputToUse = input_held;
        }
    }
    else
    {
        field_202_input_hold_down_timer = 15;
        field_204_prev_pressed = Input().mPads[sCurrentControllerIndex].mPressed;
        inputToUse = input_held;
    }

    if (inputToUse & (InputCommands::Enum::eLeft | InputCommands::Enum::eUp))
    {
        if (field_230_target_entry_index > 0 && field_254 == FP_FromInteger(0))
        {
            field_230_target_entry_index--;
            SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 35, 400);
        }
    }
    else if (inputToUse & (InputCommands::Enum::eRight | InputCommands::Enum::eDown))
    {
        if (field_230_target_entry_index < sMenuItemCount_561538 - 1 && field_254 == FP_FromInteger(0))
        {
            field_230_target_entry_index++;
            SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 35, 400);
        }
    }

    if (inputToUse & InputCommands::Enum::ePageUp)
    {
        if (field_254 == FP_FromInteger(0))
        {
            if (field_230_target_entry_index <= 3)
            {
                field_230_target_entry_index = 0;
            }
            else
            {
                field_230_target_entry_index -= 3;
            }
            SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 35, 400);
        }
    }
    else if (inputToUse & InputCommands::Enum::ePageDown)
    {
        if (field_254 == FP_FromInteger(0))
        {
            if (field_230_target_entry_index >= sMenuItemCount_561538 - 3)
            {
                field_230_target_entry_index = sMenuItemCount_561538 - 1;
            }
            else
            {
                field_230_target_entry_index += 3;
            }
            SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 35, 400);
        }
    }

    if (inputToUse & InputCommands::Enum::eBack)
    {
        field_23C_T80.Clear(Flags::eBit25_CheatLevelSelectLoading);
        return MainMenuNextCam(MainMenuCams::eMainMenuCam);
    }

    if (!(inputToUse & InputCommands::Enum::eUnPause_OrConfirm))
    {
        return MainMenuNextCam(MainMenuCams::eNoChange);
    }

    if (field_25C_Inside_FMV_Screen)
    {
        MenuFMV* pMenuFMV = &pDemosOrFmvs_BB4414.mFmvRec[field_230_target_entry_index];
        if (pMenuFMV->field_A_fmv_id >= 0)
        {
            FmvInfo* pFmvRecord = Path_Get_FMV_Record(pMenuFMV->field_4_level_id, pMenuFMV->field_A_fmv_id);
            Get_fmvs_sectors(pFmvRecord->field_0_pName, 0, 0, &input_held, 0, 0);
            sLevelId_dword_5CA408 = static_cast<u32>(MapWrapper::ToAE(pMenuFMV->field_4_level_id));

            dword_55C128 = -1;

            auto pMovie = relive_new Movie(pFmvRecord->field_4_id, input_held, pFmvRecord->field_6_flags & 1, pFmvRecord->field_8_flags, pFmvRecord->field_A_volume);

            while (sMovie_ref_count_BB4AE4)
            {
                if (pMovie->mBaseGameObjectFlags.Get(BaseGameObject::eUpdatable_Bit2))
                {
                    if (!pMovie->mBaseGameObjectFlags.Get(BaseGameObject::eDead) && (!sNum_CamSwappers_5C1B66 || pMovie->mBaseGameObjectFlags.Get(BaseGameObject::eUpdateDuringCamSwap_Bit10)))
                    {
                        pMovie->VUpdate();
                    }
                }
            }
            stru_5C3110.Free_433130();
            gPsxDisplay.PutCurrentDispEnv();
            pScreenManager->DecompressCameraToVRam(gMap.field_2C_camera_array[0]->field_C_pCamRes);
            pScreenManager->MoveImage();
            pScreenManager->EnableRendering();
            GetSoundAPI().SND_Restart();
        }
        else
        {
            gCreditsControllerExists = 1;
            field_240_credits_current_cam = 1;
            field_1F4_credits_next_frame = sGnFrame + 160;
            gMap.SetActiveCam(EReliveLevelIds::eCredits, 1, 1, CameraSwapEffects::eInstantChange_0, 0, 0);
        }
        return MainMenuNextCam(MainMenuCams::eNoChange);
    }

    field_23C_T80.Set(Flags::eBit25_CheatLevelSelectLoading);

    field_244_lvl_id = pDemosOrFmvs_BB4414.mDemoRec[field_230_target_entry_index].field_4_level;
    field_246_path_id = pDemosOrFmvs_BB4414.mDemoRec[field_230_target_entry_index].field_6_path;
    field_248_camera = pDemosOrFmvs_BB4414.mDemoRec[field_230_target_entry_index].field_8_camera;
    field_24A_abeXOff = pDemosOrFmvs_BB4414.mDemoRec[field_230_target_entry_index].field_C_abe_x_off;
    field_24C_abeYOff = pDemosOrFmvs_BB4414.mDemoRec[field_230_target_entry_index].field_E_abe_y_off;
    field_24E_start_scale = pDemosOrFmvs_BB4414.mDemoRec[field_230_target_entry_index].field_A_id; // some how id and scale ??

    return MainMenuNextCam(MainMenuCams::eGameIsLoading_ShaddapCam, NO_SELECTABLE_BUTTONS);
}

MainMenuText sLoadButtonGraphics[2] = {
    {32, 27, "x", 3u, 0u, 0u, 0u, 0.75, 0u, 0u, 0u, 0u},
    {331, 204, "esc", 3u, 0u, 0u, 0u, 0.75, 0u, 0u, 0u, 0u}};

ALIVE_VAR(1, 0xBB43E8, s32, sSelectedSavedGameIdx_BB43E8, 0);

void MainMenuController::tLoadGame_Render_4D44D0(PrimHeader** ot)
{
    s32 polyOffset = 0;
    RenderScrollableTextEntries(
        ot, sSavedGameToLoadIdx_BB43FC, sSelectedSavedGameIdx_BB43E8, sTotalSaveFilesCount_BB43E0,
        sTextYPos_BB43F0, dword_BB43E4, field_234_pStr, sSaveFileRecords_BB31D8, field_120_font, polyOffset);

    if (sTotalSaveFilesCount_BB43E0 <= 0) // max save files count
    {
        DrawMenuStringWithShadow(ot, field_120_font, "No Saved Games", 120, 110, 255, 218, 140, polyOffset);
    }
    for (auto& menuGraphic : sLoadButtonGraphics)
    {
        DrawMenuText_4D20D0(&menuGraphic, ot, &field_120_font, &polyOffset, 1);
    }
}

void MainMenuController::AbeMotions_Render_4D25E0(PrimHeader** ot)
{
    RenderOnScreenTextHelper(ot, &sAbeMotions_562448[0], ALIVE_COUNTOF(sAbeMotions_562448), 0);
}

MainMenuNextCam MainMenuController::Gamespeak_Update_4D1FC0(u32 input_held)
{
    field_230_target_entry_index = 0;

    if (input_held & InputCommands::Enum::eBack)
    {
        return MainMenuNextCam(MainMenuCams::eMainMenuCam);
    }

    if (!(input_held & InputCommands::Enum::eUnPause_OrConfirm))
    {
        return MainMenuNextCam(MainMenuCams::eNoChange);
    }

    switch (field_1FC_button_index)
    {
        case 0:
            return MainMenuNextCam(MainMenuCams::eAbeGamespeakCam, NO_SELECTABLE_BUTTONS);
        case 1:
            return MainMenuNextCam(MainMenuCams::eSligGamespeakCam, NO_SELECTABLE_BUTTONS);
        case 2:
            return MainMenuNextCam(MainMenuCams::eGlukkonGamespeakCam, NO_SELECTABLE_BUTTONS);
        case 3:
            return MainMenuNextCam(MainMenuCams::eScrabGamespeakCam, NO_SELECTABLE_BUTTONS);
        case 4:
            return MainMenuNextCam(MainMenuCams::eParamiteGamespeakCam, NO_SELECTABLE_BUTTONS);
        default:
            return MainMenuNextCam(MainMenuCams::eNoChange);
    }
}

void MainMenuController::RenderOnScreenTextHelper(PrimHeader** ot, const MainMenuText* menuTextArray, s32 count, s8 isSingleChar)
{
    s32 polyIndex = 0;

    for (s32 i = 0; i < count; i++)
    {
        MainMenuController::DrawMenuText_4D20D0(&menuTextArray[i], ot, &field_120_font, &polyIndex, isSingleChar);
    }
}

MainMenuText sMTxt_Gamespeak_5623B8[2] = {
    {330, 204, "esc", 3u, 0u, 0u, 0u, 0.75f, 0u, 0u, 0u, 0u},
    {35, 205, "x", 3u, 0u, 0u, 0u, 0.75f, 0u, 0u, 0u, 0u},
};

MainMenuText sMTxt_Gamespeak_5623E8[2] = {
    {35, 205, "x", 3u, 0u, 0u, 0u, 0.75f, 0u, 0u, 0u, 0u},
    {331, 205, "esc", 3u, 0u, 0u, 0u, 0.75f, 0u, 0u, 0u, 0u},
};

MainMenuText sMTxt_NewGame_562598[3] = {
    {165, 211, "esc", 3u, 0u, 0u, 0u, 0.75, 0u, 0u, 0u, 0u},
    {161, 164, "x", 3u, 0u, 0u, 0u, 0.75, 0u, 0u, 0u, 0u},
};

void MainMenuController::Gamespeak_Render_4D24F0(PrimHeader** ot)
{
    RenderOnScreenTextHelper(ot, sMTxt_Gamespeak_5623B8, 2, 1);
}

void MainMenuController::Options_Render_4D2540(PrimHeader** ot)
{
    RenderOnScreenTextHelper(ot, sMTxt_Gamespeak_5623E8, 2, 1);
}

void MainMenuController::BackStory_Or_NewGame_Render_4D2630(PrimHeader** ot)
{
    RenderOnScreenTextHelper(ot, sMTxt_NewGame_562598, 2, 1);
}

MainMenuNextCam MainMenuController::Page_Front_Update_4D0720(u32 input)
{
#if DEVELOPER_MODE
    static bool first = true;
    if (first && gBootToLoadScreen)
    {
        first = false;
        // Force enter pressed
        input |= InputCommands::Enum::eUnPause_OrConfirm;
        // Force load game selected
        field_1FC_button_index = 2;
    }

#endif
    // Reset time out if any input detected
    if (Input().mPads[0].mPressed)
    {
        field_1F8_page_timeout = 0;
        bLongerTimeoutToNextDemo = 0;
    }

    // Go to loading a demo screen if no input after time out, after one demo plays the next time out is lower if input isn't pressed
    if (field_1F8_page_timeout > (bLongerTimeoutToNextDemo ? 300 : 1500))
    {
        bLongerTimeoutToNextDemo = 1;
        field_1FC_button_index = 0;
        return MainMenuNextCam(MainMenuCams::eDemoIsLoading_ShaddapCam, NO_SELECTABLE_BUTTONS);
    }

    // Enter pressed on selected menu item?
    if (input & InputCommands::Enum::eUnPause_OrConfirm)
    {
        Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eAbe_OK);

        switch (field_1FC_button_index)
        {
            case 0:
                // Begin
                return MainMenuNextCam(MainMenuCams::eBackstory_Or_NewGameCam);
            case 1:
                // Quit
                Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eAbe_GoodBye);
                return MainMenuNextCam(MainMenuCams::eDummyBlankCam);
            case 2:
                // Load
                field_230_target_entry_index = 0;
                return MainMenuNextCam(MainMenuCams::eLoadGameMenuCam, NO_SELECTABLE_BUTTONS);
            case 3:
                // Options
                return MainMenuNextCam(MainMenuCams::eOptionsCam);
            case 4:
                // Game speak
                field_230_target_entry_index = 0;
                return MainMenuNextCam(MainMenuCams::eGamespeakCharacterSelectionCam);
        }
    }

    if (sEnableCheatFMV_5C1BEC)
    {
        // To FMV list menu
        sEnableCheatFMV_5C1BEC = 0;
        field_25C_Inside_FMV_Screen = 1;
        pDemosOrFmvs_BB4414.mFmvRec = &sFmvs_561540[0];
        sMenuItemCount_561538 = ALIVE_COUNTOF(sFmvs_561540);
        field_230_target_entry_index = 0;
        field_250_selected_entry_index = 0;
        field_254 = FP_FromInteger(0);
        field_258 = FP_FromInteger(0);
        return MainMenuNextCam(MainMenuCams::eCheatMenu_SelectFMVCam, NO_SELECTABLE_BUTTONS);
    }

    if (sEnableCheatLevelSelect_5C1BEE)
    {
        // To level select menu
        sEnableCheatLevelSelect_5C1BEE = 0;
        field_25E_Inside_CheatLevelSelect_Screen = 1;
        pDemosOrFmvs_BB4414.mDemoRec = &gPerLvlData_561700[0];
        sMenuItemCount_561538 = ALIVE_COUNTOF(gPerLvlData_561700) - 2; // exclude menu and credits levels
        field_230_target_entry_index = 0;
        field_250_selected_entry_index = 0;
        field_254 = FP_FromInteger(0);
        field_258 = FP_FromInteger(0);
        return MainMenuNextCam(MainMenuCams::eCheatMenu_SelectLevelCam, NO_SELECTABLE_BUTTONS);
    }

    return MainMenuNextCam(MainMenuCams::eNoChange);
}

void MainMenuController::Page_Front_Render_4D24B0(PrimHeader** ot)
{
    RenderOnScreenTextHelper(ot, &sMMT_FrontPage_5623A0[0], ALIVE_COUNTOF(sMMT_FrontPage_5623A0), 1);
}

ALIVE_VAR(1, 0xbb43dc, s16, word_BB43DC, 0);
ALIVE_VAR(1, 0x5c1b88, s32, sGameStartedFrame_5C1B88, 0);

MainMenuNextCam MainMenuController::LoadNewGame_Update_4D0920(u32 /*input*/)
{
    // TODO: De-dupe the big parts of duplicated code in here
    if (field_23C_T80.Get(Flags::eBit21_LoadingSave))
    {
        if (field_23C_T80.Get(Flags::eBit18_Loading))
        {
            /*
            // Wait for load to complete
            if (!field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak])
            {
                pResourceManager_5C1BB0->LoadingLoop_465590(FALSE);
            }
            mAnim.Set_Animation_Data(AnimId::MenuAbeSpeak_Idle, field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak2]);

            ResourceManager::FreeResource_49C330(field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak]);
            field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak] = nullptr;

            ResourceManager::Reclaim_Memory_49C470(0);
            */
            mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));

            if (!pPauseMenu_5C9300)
            {
                pPauseMenu_5C9300 = relive_new PauseMenu();
            }

            if (!sActiveHero)
            {
                sActiveHero = relive_new Abe();
            }

            if (field_208_transition_obj)
            {
                field_208_transition_obj->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }

            if (field_20C_pUnused)
            {
                field_20C_pUnused->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }

            if (field_210_pUnused)
            {
                field_210_pUnused->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }

            mBaseGameObjectFlags.Set(BaseGameObject::eDead);

            sActiveHero->mXPos = FP_FromInteger(0);
            sActiveHero->mYPos = FP_FromInteger(0);

            Quicksave_LoadActive();

            return MainMenuNextCam(MainMenuCams::eNoChange);
        }

        field_23C_T80.Set(Flags::eBit18_Loading);
        return MainMenuNextCam(MainMenuCams::eNoChange);
    }

    if (!field_23C_T80.Get(Flags::eBit18_Loading))
    {
        field_23C_T80.Set(Flags::eBit18_Loading);
        return MainMenuNextCam(MainMenuCams::eNoChange);
    }

    /*
    if (!field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak])
    {
        pResourceManager_5C1BB0->LoadingLoop_465590(FALSE);
    }

    mAnim.Set_Animation_Data(AnimId::MenuAbeSpeak_Idle, field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak2]);

    ResourceManager::FreeResource_49C330(field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak]);
    field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak] = nullptr;
    ResourceManager::Reclaim_Memory_49C470(0);
    */
    mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));

    if (!pPauseMenu_5C9300)
    {
        pPauseMenu_5C9300 = relive_new PauseMenu();
    }

    if (!sActiveHero)
    {
        sActiveHero = relive_new Abe();
    }

    if (field_23C_T80.Get(Flags::eBit25_CheatLevelSelectLoading))
    {
        field_23C_T80.Clear(Flags::eBit25_CheatLevelSelectLoading);

        sActiveHero->SetUpdateDelay(1);
        gMap.SetActiveCam(field_244_lvl_id, field_246_path_id, field_248_camera, CameraSwapEffects::eInstantChange_0, 0, 0);

        const PathBlyRec* pPathData = Path_Get_Bly_Record(field_244_lvl_id, field_246_path_id);
        sActiveHero->mXPos = FP_FromInteger(field_24A_abeXOff - pPathData->field_4_pPathData->field_1A_abe_start_xpos);
        sActiveHero->mYPos = FP_FromInteger(field_24C_abeYOff - pPathData->field_4_pPathData->field_1C_abe_start_ypos);

        if (field_24E_start_scale == -1)
        {
            sActiveHero->mSpriteScale = FP_FromDouble(1.0);
            sActiveHero->mScale = Scale::Fg;
            sActiveHero->mAnim.mRenderLayer = Layer::eLayer_AbeMenu_32;
        }
        else if (field_24E_start_scale == -2)
        {
            sActiveHero->mSpriteScale = FP_FromDouble(0.5);
            sActiveHero->mScale = Scale::Bg;
            sActiveHero->mAnim.mRenderLayer = Layer::eLayer_AbeMenu_Half_13;
        }
    }
    else
    {
        gMap.SetActiveCam(EReliveLevelIds::eMines, 1, 4, CameraSwapEffects::ePlay1FMV_5, 12402, 0);
        sActiveHero->mXPos = FP_FromInteger(837);
        sActiveHero->mYPos = FP_FromInteger(1300);
        sActiveHero->BaseAliveGameObjectLastLineYPos = FP_FromInteger(1400);
    }

    if (field_208_transition_obj)
    {
        field_208_transition_obj->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (field_20C_pUnused)
    {
        field_20C_pUnused->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (field_210_pUnused)
    {
        field_210_pUnused->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    mBaseGameObjectFlags.Set(BaseGameObject::eDead);

    return MainMenuNextCam(MainMenuCams::eNoChange);
}

MainMenuNextCam MainMenuController::BackStory_Or_NewGame_Update_4D1C60(u32 input_held)
{
    if (input_held & InputCommands::Enum::eUnPause_OrConfirm)
    {
        if (field_1FC_button_index == 0) // Show backstory
        {
            FmvInfo* pFmvRecord = Path_Get_FMV_Record(EReliveLevelIds::eMenu, 4u);
            while (Input_IsVKPressed_4EDD40(VK_RETURN))
            {
                SYS_EventsPump_494580();
            }

            u32 fmvSector = 0;
            Get_fmvs_sectors(pFmvRecord->field_0_pName, nullptr, nullptr, &fmvSector, 0, 0);
            sLevelId_dword_5CA408 = 0;

            auto pMovie = relive_new Movie(pFmvRecord->field_4_id,
                                        input_held,
                                        pFmvRecord->field_6_flags & 1,
                                        pFmvRecord->field_8_flags,
                                        pFmvRecord->field_A_volume);

            while (sMovie_ref_count_BB4AE4)
            {
                if (pMovie->mBaseGameObjectFlags.Get(BaseGameObject::eUpdatable_Bit2))
                {
                    if (!(pMovie->mBaseGameObjectFlags.Get(BaseGameObject::eDead) && (!sNum_CamSwappers_5C1B66 || (pMovie->mBaseGameObjectFlags.Get(BaseGameObject::eUpdateDuringCamSwap_Bit10)))))
                    {
                        pMovie->VUpdate();
                    }
                }
            }

            gPsxDisplay.PutCurrentDispEnv();
            pScreenManager->DecompressCameraToVRam(gMap.field_2C_camera_array[0]->field_C_pCamRes);
            pScreenManager->MoveImage();
            pScreenManager->EnableRendering();
            GetSoundAPI().SND_Restart();
            field_1FC_button_index = 1; // Select start game
            return MainMenuNextCam(MainMenuCams::eNoChange);
        }
        else if (field_1FC_button_index == 1) // Start game
        {
            sCurrentControllerIndex = 0;
            sGameStartedFrame_5C1B88 = sGnFrame;
            return MainMenuNextCam(MainMenuCams::eGameIsLoading_ShaddapCam, NO_SELECTABLE_BUTTONS);
        }
    }
    else if (input_held & InputCommands::Enum::eBack) // Escape/back
    {
        ResourceManager::Reclaim_Memory_49C470(0);
        word_BB43DC = 1;
        field_23C_T80.Clear(Flags::eBit25_CheatLevelSelectLoading);
        return MainMenuNextCam(MainMenuCams::eMainMenuCam);
    }

    return MainMenuNextCam(MainMenuCams::eNoChange);
}

void MainMenuController::BackStory_Or_NewGame_Load_4D1BA0()
{
   // ResourceManager::FreeResource_49C330(field_F4_resources.field_0_resources[MenuResIds::eAbeIntro]);
//    field_F4_resources.field_0_resources[MenuResIds::eAbeIntro] = nullptr;
    word_BB43DC = 0;
}

void MainMenuController::BackStory_Or_NewGame_Unload_4D1BE0()
{
    if (!word_BB43DC)
    {
        ResourceManager::Reclaim_Memory_49C470(0);
    }
    pResourceManager_5C1BB0->LoadingLoop_465590(FALSE);
}

void MainMenuController::remove_ISO9660_Suffix_4D1660(char_type* out, const char_type* in)
{
    while (*in)
    {
        if (*in == ';')
        {
            *out = '\0';
            break;
        }

        *out = static_cast<s8>(tolower(*in));
        in++;
        out++;
    }
}

s8 MainMenuController::checkIfDemoFileExists_4D1430(char_type* input)
{
    char_type* inputMod = input;
    if (inputMod[0] == '\\')
    {
        inputMod++;
    }
    char_type buffer[256] = {};
    MainMenuController::remove_ISO9660_Suffix_4D1660(buffer, inputMod);

    char_type fileName[256] = {};
    strcpy(fileName, sCdEmu_Path1_C14620);
    strcat(fileName, buffer);
    if (access_impl(fileName, 0) == 0)
    {
        return 1;
    }
    strcpy(fileName, sCdEmu_Path2_C144C0);
    strcat(fileName, buffer);
    if (access_impl(fileName, 0) == 0)
    {
        return 1;
    }

    if (!sCdRomDrives_5CA488[0])
    {
        return 0;
    }
    strcpy(fileName, sCdEmu_Path3_C145A0);
    strcat(fileName, buffer);
    fileName[0] = sCdRomDrives_5CA488[0];

    int idx = 1;
    while (access_impl(fileName, 0) != 0)
    {
        fileName[0] = sCdRomDrives_5CA488[idx++];
        if (!fileName[0])
        {
            return 0;
        }
    }
    if (fileName[0])
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// true if demo was started manually from the demos menu,
// false if demo was started automatically due to idling
ALIVE_VAR(1, 0x5C1B9C, s16, gIsDemoStartedManually_5C1B9C, FALSE);

ALIVE_VAR(1, 0x5C1BA2, u8, sCurrentDemoIdForIdlingDemoPlayback_5C1BA2, 0);
MainMenuNextCam MainMenuController::LoadDemo_Update_4D1040(u32)
{
    const s32 maxDemoId = ALIVE_COUNTOF(sDemos_5617F0);

    if (field_23C_T80.Get(Flags::eBit18_Loading))
    {
        s16 demoId = sDemoIdChosenFromDemoMenu_5C1B9E;
        if (!gIsDemoStartedManually_5C1B9C)
        {
            demoId = sCurrentDemoIdForIdlingDemoPlayback_5C1BA2;
        }
        if (demoId >= maxDemoId)
        {
            demoId = 0;
        }
        const s32 levelId = static_cast<s32>(sDemos_5617F0[demoId].field_4_level);
        char_type lvFilename[256] = {};
        strcpy(lvFilename, "ATTRACT");
        memset(&lvFilename[8], 0, 0xF8u);
        strcpy(&lvFilename[7], CdLvlName((sDemos_5617F0[demoId].field_4_level)));
        auto lvFilenameNoPrefix = &lvFilename[7];

        while (!MainMenuController::checkIfDemoFileExists_4D1430(lvFilenameNoPrefix) && !MainMenuController::checkIfDemoFileExists_4D1430(lvFilename))
        {
            sLevelId_dword_5CA408 = levelId;
            if (gIsDemoStartedManually_5C1B9C)
            {
                dword_55C128 = -1;
            }
            if (!Display_Full_Screen_Message_Blocking(Path_Get_Unknown(sDemos_5617F0[demoId].field_4_level), MessageType::eSkipDemo_2))
            {
                field_1F8_page_timeout = 0;
                if (gIsDemoStartedManually_5C1B9C)
                {
                    return MainMenuNextCam(MainMenuCams::eDemoSelectionCam);
                }
                else
                {
                    return MainMenuNextCam(MainMenuCams::eMainMenuCam);
                }
            }
        }

        /*
        if (field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak] == 0)
        {
            pResourceManager_5C1BB0->LoadingLoop_465590(0);
        }
        mAnim.Set_Animation_Data(AnimId::MenuAbeSpeak_Idle, field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak2]);
        ResourceManager::FreeResource_49C330(field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak]);
        field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak] = nullptr;
        ResourceManager::Reclaim_Memory_49C470(0);
        */
        mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));

        if (!sActiveHero)
        {
            auto abe = relive_new Abe();
            if (abe)
            {
                sActiveHero = abe;
                sActiveHero->mXPos = FP_FromInteger(0);
                sActiveHero->mYPos = FP_FromInteger(0);
            }
            else
            {
                sActiveHero = nullptr;
            }
        }
        
        gAttract_5C1BA0 = 1;

        if (field_208_transition_obj)
        {
            field_208_transition_obj->mBaseGameObjectFlags.Set(Options::eDead);
        }
        if (field_20C_pUnused)
        {
            field_20C_pUnused->mBaseGameObjectFlags.Set(Options::eDead);
        }
        if (field_210_pUnused)
        {
            field_210_pUnused->mBaseGameObjectFlags.Set(Options::eDead);
        }
        mBaseGameObjectFlags.Set(Options::eDead);

        demoId = sDemoIdChosenFromDemoMenu_5C1B9E;
        if (gIsDemoStartedManually_5C1B9C)
        {
            // play the manually picked demo
            demoId = sDemoIdChosenFromDemoMenu_5C1B9E;
        }
        else
        {
            if (sCurrentDemoIdForIdlingDemoPlayback_5C1BA2 > maxDemoId)
            {
                // all the "idling" demos have been played, wrap around
                sCurrentDemoIdForIdlingDemoPlayback_5C1BA2 = 0;
            }
            demoId = sCurrentDemoIdForIdlingDemoPlayback_5C1BA2++; // take the pre-incremented value
        }

        char_type file[32] = {};
        sprintf(file, "ATTR%04d.SAV", sDemos_5617F0[demoId].field_A_id);
        ResourceManager::LoadResourceFile_49C170(file, nullptr);
        u8** resource = ResourceManager::GetLoadedResource(ResourceManager::ResourceType::Resource_NxtP, AEResourceID::kUnknownResID_0, 1, 0);
        sActiveQuicksaveData = *(reinterpret_cast<Quicksave*>(*resource));
        ResourceManager::FreeResource_49C330(resource);

        if (gIsDemoStartedManually_5C1B9C)
        {
            sActiveQuicksaveData.field_200_accumulated_obj_count = 1024;
        }
        Quicksave_LoadActive();
    }
    else
    {
        field_23C_T80.Set(Flags::eBit18_Loading);
    }
    return MainMenuNextCam(MainMenuCams::eNoChange);
}

MainMenuNextCam MainMenuController::DemoSelect_Update_4D0E10(u32 input)
{
    gAttract_5C1BA0 = 0;
    gIsDemoStartedManually_5C1B9C = FALSE;

    s32 input_or_field_204 = input;
    if (field_204_prev_pressed && field_204_prev_pressed == Input().mPads[sCurrentControllerIndex].mPressed)
    {
        field_202_input_hold_down_timer--;
        if (field_202_input_hold_down_timer == 0)
        {
            input_or_field_204 = field_204_prev_pressed;
            field_202_input_hold_down_timer = 4;
        }
    }
    else
    {
        field_202_input_hold_down_timer = 15;
        field_204_prev_pressed = Input().mPads[sCurrentControllerIndex].mPressed;
    }

    if (input_or_field_204 & (InputCommands::Enum::eUp | InputCommands::Enum::eLeft))
    {
        // move to previous entry
        if (field_230_target_entry_index > 0 && field_254 == FP_FromInteger(0))
        {
            field_230_target_entry_index--;
            SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 35, 400);
        }
    }
    else if (input_or_field_204 & (InputCommands::Enum::eDown | InputCommands::Enum::eRight))
    {
        // move to next entry
        if (field_230_target_entry_index < sMenuItemCount_561538 - 1 && field_254 == FP_FromInteger(0))
        {
            field_230_target_entry_index++;
            SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 35, 400);
        }
    }

    if (input_or_field_204 & InputCommands::Enum::ePageUp)
    {
        // move one whole page back
        if (field_230_target_entry_index > 0 && field_254 == FP_FromInteger(0))
        {
            field_230_target_entry_index -= 3;
            if (field_230_target_entry_index < 0)
            {
                field_230_target_entry_index = 0;
            }
            SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 35, 400);
        }
    }
    else if (input_or_field_204 & InputCommands::Enum::ePageDown)
    {
        // move one whole page forward
        if (field_230_target_entry_index < sMenuItemCount_561538 - 1 && field_254 == FP_FromInteger(0))
        {
            field_230_target_entry_index += 3;
            if (field_230_target_entry_index > sMenuItemCount_561538 - 1)
            {
                field_230_target_entry_index = sMenuItemCount_561538 - 1;
            }
            SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 35, 400);
        }
    }

    if (input_or_field_204 & InputCommands::Enum::eBack)
    {
        return MainMenuNextCam(MainMenuCams::eOptionsCam, 1); //Esc Pressed
    }

    if (input_or_field_204 & InputCommands::Enum::eUnPause_OrConfirm)
    {
        // selected a demo for playing
        gIsDemoStartedManually_5C1B9C = TRUE;
        sDemoIdChosenFromDemoMenu_5C1B9E = field_230_target_entry_index;
        return MainMenuNextCam(MainMenuCams::eDemoIsLoading_ShaddapCam, NO_SELECTABLE_BUTTONS); //Enter Pressed
    }

    return MainMenuNextCam(MainMenuCams::eNoChange); // Nothing Pressed
}

MainMenuNextCam MainMenuController::tLoadGame_Input_4D3EF0(u32 input)
{
    bool indexChanged = false;

    // Escape ?
    if (input & InputCommands::Enum::eBack)
    {
        // Go back to start page
        field_23C_T80.Clear(Flags::eBit21_LoadingSave);
        field_23A_Inside_LoadGame_Screen = 0;
        return MainMenuNextCam(MainMenuCams::eMainMenuCam, 2);
    }
    // Up a single save
    else if (input & InputCommands::Enum::eUp)
    {
        if (sSelectedSavedGameIdx_BB43E8 > 0 && !sTextYPos_BB43F0.fpValue)
        {
            sSelectedSavedGameIdx_BB43E8--;
            indexChanged = true;
        }
    }
    // Down a single save
    else if (input & InputCommands::Enum::eDown)
    {
        if (sSelectedSavedGameIdx_BB43E8 < sTotalSaveFilesCount_BB43E0 - 1 && !sTextYPos_BB43F0.fpValue)
        {
            sSelectedSavedGameIdx_BB43E8++;
            indexChanged = true;
        }
    }
    else if (input & InputCommands::Enum::ePageUp)
    {
        // Page up underflow
        if (sSelectedSavedGameIdx_BB43E8 >= 3 && !sTextYPos_BB43F0.fpValue)
        {
            sSelectedSavedGameIdx_BB43E8 -= 3;
            indexChanged = true;
        }
        else
        {
            sSelectedSavedGameIdx_BB43E8 = 0;
            indexChanged = true;
        }
    }
    else if (input & InputCommands::Enum::ePageDown)
    {
        // Page down overflow
        if (sSelectedSavedGameIdx_BB43E8 < sTotalSaveFilesCount_BB43E0 - 3 && !sTextYPos_BB43F0.fpValue)
        {
            sSelectedSavedGameIdx_BB43E8 += 3;
            indexChanged = true;
        }
        else
        {
            sSelectedSavedGameIdx_BB43E8 = sTotalSaveFilesCount_BB43E0 - 1;
            indexChanged = true;
        }
    }

    if (indexChanged)
    {
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 35, 400);
    }

    if (input & InputCommands::Enum::eUnPause_OrConfirm)
    {
        // No save to load, go back
        if (sTotalSaveFilesCount_BB43E0 == 0)
        {
            // Go back to start page
            field_23C_T80.Clear(Flags::eBit21_LoadingSave);
            field_23A_Inside_LoadGame_Screen = 0;
            return MainMenuNextCam(MainMenuCams::eMainMenuCam, 2);
        }

        // Load selected save
        char_type filename[40] = {};
        strcpy(filename, sSaveFileRecords_BB31D8[sSavedGameToLoadIdx_BB43FC].field_0_fileName);
        strcat(filename, ".sav");

        std::string strPath = FS::GetPrefPath() + filename;
        IO_FileHandleType hFile = IO_Open(strPath.c_str(), "rb");

        if (!hFile)
        {
            return MainMenuNextCam(MainMenuCams::eNoChange);
        }

        IO_Read(hFile, &sActiveQuicksaveData, sizeof(Quicksave), 1u);
        IO_Close(hFile);

        field_23C_T80.Set(Flags::eBit21_LoadingSave);
        return MainMenuNextCam(MainMenuCams::eGameIsLoading_ShaddapCam, NO_SELECTABLE_BUTTONS);
    }

    return MainMenuNextCam(MainMenuCams::eNoChange);
}

void MainMenuController::tLoadGame_Load_4D42F0()
{
    field_23A_Inside_LoadGame_Screen = 6;
    field_230_target_entry_index = 0;
    field_1FC_button_index = NO_SELECTABLE_BUTTONS;
    Quicksave_FindSaves();
    sSelectedSavedGameIdx_BB43E8 = sSavedGameToLoadIdx_BB43FC;
    field_23C_T80.Clear(Flags::eBit15_unused);
    field_1F4_credits_next_frame = 0;
}

void sub_4A2D40()
{
    
}


MainMenuNextCam MainMenuController::Options_Update_4D1AB0(u32 input)
{
    if (!(input & InputCommands::Enum::eUnPause_OrConfirm))
    {
        if (input & InputCommands::Enum::eBack)
        {
            Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eAbe_OK, 0);
            return MainMenuNextCam(MainMenuCams::eMainMenuCam);
        }
        else
        {
            return MainMenuNextCam(MainMenuCams::eNoChange);
        }
    }
    Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eAbe_OK, 0);

    switch (field_1FC_button_index)
    {
        case 0:
        {
            return MainMenuNextCam(MainMenuCams::eControllerSelectionCam);
        }
        case 1:
        {
            field_250_selected_entry_index = 0;
            field_254 = FP_FromInteger(0);
            field_258 = FP_FromInteger(0);
            pDemosOrFmvs_BB4414.mDemoRec = &sDemos_5617F0[0];
            sMenuItemCount_561538 = ALIVE_COUNTOF(sDemos_5617F0);
            mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
            Load_Anim_Pal_4D06A0(&mAnim);
            field_230_target_entry_index = 0;
            return MainMenuNextCam(MainMenuCams::eDemoSelectionCam, NO_SELECTABLE_BUTTONS);
        }
        default:
        {
            if (input & InputCommands::Enum::eBack)
            {
                Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eAbe_OK, 0);
                return MainMenuNextCam(MainMenuCams::eMainMenuCam);
            }
            else
            {
                return MainMenuNextCam(MainMenuCams::eNoChange);
            }
        }
    }
}

MainMenuNextCam MainMenuController::AbeMotions_Update_4D1F50(u32 input)
{
    if (sNum_CamSwappers_5C1B66 > 0)
    {
        // Camera is changing - stay in this screen
        return MainMenuNextCam(MainMenuCams::eNoChange);
    }

    // To the game speak screen
    if (input & InputCommands::Enum::eUnPause_OrConfirm)
    {
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 35, 400);
        return MainMenuNextCam(MainMenuCams::eGamespeakCharacterSelectionCam);
    }

    // Return to previous screen
    if (input & InputCommands::Enum::eBack)
    {
        return MainMenuNextCam(MainMenuCams::eOptionsCam);
    }
    else
    {
        // No input - stay in this screen
        return MainMenuNextCam(MainMenuCams::eNoChange);
    }
}

MainMenuNextCam MainMenuController::PSX_Cooperative_Mode_Update_4D49B0(u32 /*input*/)
{
    const u32 held = Input().mPads[0].mHeld;
    if (held & (InputCommands::Enum::eUnPause_OrConfirm | InputCommands::Enum::eBack))
    {
        return MainMenuNextCam(MainMenuCams::eBackstory_Or_NewGameCam);
    }

    return MainMenuNextCam(MainMenuCams::eNoChange);
}

MainMenuNextCam MainMenuController::PSX_Gamemode_Selection_Update_4D48C0(u32 input)
{
    if (input & InputCommands::Enum::eUnPause_OrConfirm)
    {
        sGameStartedFrame_5C1B88 = sGnFrame;
        sCurrentControllerIndex = 0;
        const bool twoPlayerModeSelected = field_1FC_button_index == 1;

        MainMenuController::Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eAbe_FollowMe);
        if (field_23C_T80.Get(Flags::eBit25_CheatLevelSelectLoading))
        {
            return MainMenuNextCam(MainMenuCams::eGameIsLoading_ShaddapCam);
        }

        // apparently cam 11 was originally for the cooperative mode
        // description screen on the PSX
        return twoPlayerModeSelected ? MainMenuNextCam(MainMenuCams::eDummyBlankCam) : MainMenuNextCam(MainMenuCams::eBackstory_Or_NewGameCam);
    }
    else if (input & InputCommands::Enum::eBack)
    {
        if (field_23C_T80.Get(Flags::eBit25_CheatLevelSelectLoading))
        {
            return MainMenuNextCam(MainMenuCams::eCheatMenu_SelectLevelCam);
        }

        MainMenuController::Set_Anim_4D05E0(MainMenuGamespeakAnimIds::eAbe_OK);
        return MainMenuNextCam(MainMenuCams::eMainMenuCam);
    }

    return MainMenuNextCam(MainMenuCams::eNoChange);
}

ALIVE_VAR(1, 0xBB43F8, s32, dword_BB43F8, 0);
ALIVE_VAR(1, 0xBB43EC, s32, sButtonToRemapIdx_BB43EC, 0);

void MainMenuController::RemapInput_Load_4D17E0()
{
    field_158_animation.Set_Animation_Data(GetAnimRes(AnimId::MenuHighlight_ButtonRemapSquare));
    sButtonToRemapIdx_BB43EC = 0;
    dword_BB43F8 = 0;
}

void MainMenuController::ControllerMenu_Load_4D16B0()
{
    sControllerEntryToSelect_BB43D8 = sJoystickEnabled;
    sSelectedControllerEntry_BB43F4 = sJoystickEnabled;
}

const char_type* sInputButtonNames_562790[8] = {
    "Run",
    "Sneak",
    "Jump",
    "Speak 1",
    "Action",
    "Throw",
    "Fart/Roll",
    "Speak 2"};


const MainMenuText kTextInfo_5626A0[10] = {
    {35, 34, "x", 3u, 0u, 0u, 0u, 0.75, 0u, 0u, 0u, 0u},
    {331, 204, "esc", 3u, 0u, 0u, 0u, 0.75, 0u, 0u, 0u, 0u},
    {152, 75, kRun, 3u, 0u, 0u, 0u, 0.88f, 1u, 0u, 0u, 0u},
    {152, 107, kSneak, 3u, 0u, 0u, 0u, 0.88f, 1u, 0u, 0u, 0u},
    {152, 140, kJump, 3u, 0u, 0u, 0u, 0.88f, 1u, 0u, 0u, 0u},
    {152, 172, kSpeak1, 3u, 0u, 0u, 0u, 0.88f, 1u, 0u, 0u, 0u},
    {250, 75, kAction, 3u, 0u, 0u, 0u, 0.88f, 1u, 0u, 0u, 0u},
    {250, 107, kThrow, 3u, 0u, 0u, 0u, 0.88f, 1u, 0u, 0u, 0u},
    {250, 140, kFart, 3u, 0u, 0u, 0u, 0.88f, 1u, 0u, 0u, 0u},
    {250, 172, kSpeak2, 3u, 0u, 0u, 0u, 0.88f, 1u, 0u, 0u, 0u}};

void MainMenuController::RemapInput_Render_4D2A10(PrimHeader** ppOt)
{
    s32 polyIndex = 0;
    if (dword_BB43F8 == 3)
    {
        if (sJoystickEnabled)
        {
            field_234_pStr = "Press button to use";
        }
        else
        {
            field_234_pStr = "Press key to use";
        }

        s32 textWidth = field_120_font.MeasureScaledTextWidth(field_234_pStr, FP_FromInteger(1));
        s32 nextTextXPos = 0;
        if (textWidth >= 336)
        {
            nextTextXPos = 16;
        }
        else
        {
            nextTextXPos = (368 - textWidth) / 2;
        }
        polyIndex = field_120_font.DrawString_4337D0(ppOt, field_234_pStr, nextTextXPos, 88, TPageAbr::eBlend_0, 1, 0, Layer::eLayer_Menu_41, 40, 20, 0, polyIndex, FP_FromInteger(1), 640, 0);
        char_type buffer[512] = {};
        sprintf(buffer, "for %s", sInputButtonNames_562790[sButtonToRemapIdx_BB43EC]);
        field_234_pStr = buffer;
        textWidth = field_120_font.MeasureScaledTextWidth(buffer, FP_FromInteger(1));
        if (textWidth >= 336)
        {
            nextTextXPos = 16;
        }
        else
        {
            nextTextXPos = (368 - textWidth) / 2;
        }
        polyIndex = field_120_font.DrawString_4337D0(ppOt, field_234_pStr, nextTextXPos, 120, TPageAbr::eBlend_0, 1, 0, Layer::eLayer_Menu_41, 40, 20, 0, polyIndex, FP_FromInteger(1), 640, 0);
        field_234_pStr = "Press BackSpace for none, Esc to exit";
        textWidth = field_120_font.MeasureScaledTextWidth(field_234_pStr, FP_FromInteger(1));
        if (textWidth >= 336)
        {
            nextTextXPos = 16;
        }
        else
        {
            nextTextXPos = (368 - textWidth) / 2;
        }
        field_120_font.DrawString_4337D0(ppOt, field_234_pStr, nextTextXPos, 152, TPageAbr::eBlend_0, 1, 0, Layer::eLayer_Menu_41, 40, 20, 0, polyIndex, FP_FromInteger(1), 640, 0);
    }
    else
    {
        if (!sJoystickEnabled)
        {
            // Speak 1
            const MainMenuText speak1Stru_562760 = {152, 174, "-", 3u, 0u, 0u, 0u, 0.88f, 0u, 0u, 0u, 0u};

            // Speak 2
            const MainMenuText speak2Stru_562778 = {250, 174, "-", 3u, 0u, 0u, 0u, 0.88f, 0u, 0u, 0u, 0u};

            DrawMenuText_4D20D0(&speak1Stru_562760, ppOt, &field_120_font, &polyIndex, 1);
            DrawMenuText_4D20D0(&speak2Stru_562778, ppOt, &field_120_font, &polyIndex, 1);
        }

        // Note: OG renders this list backwards
        for (const MainMenuText& item : kTextInfo_5626A0)
        {
            DrawMenuText_4D20D0(&item, ppOt, &field_120_font, &polyIndex, 1);
        }

        // Render "glowing" box around the selected buttons
        u8 rgb = static_cast<u8>(3 * field_1FE_highlite_alpha);
        field_158_animation.mRed = rgb;
        field_158_animation.mBlue = rgb;
        field_158_animation.mGreen = rgb;
        field_158_animation.VRender(
            kTextInfo_5626A0[sButtonToRemapIdx_BB43EC + 2].x - 18,
            kTextInfo_5626A0[sButtonToRemapIdx_BB43EC + 2].y + 8,
            ppOt, 0, 0);
    }
}

MainMenuNextCam MainMenuController::ControllerMenu_Update_4D16D0(u32 input)
{
    if (input & InputCommands::Enum::eUp)
    {
        if (sSelectedControllerEntry_BB43F4 > 0 && sTextYPos_BB43F0 == FP_FromInteger(0))
        {
            sSelectedControllerEntry_BB43F4--;
            SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
        }
    }
    else if (input & InputCommands::Enum::eDown)
    {
        if (sSelectedControllerEntry_BB43F4 < sControllerCount_55E838 - 1 && sTextYPos_BB43F0 == FP_FromInteger(0))
        {
            sSelectedControllerEntry_BB43F4++;
            SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
        }
    }

    if (input & InputCommands::Enum::eBack)
    {
        return MainMenuNextCam(MainMenuCams::eOptionsCam);
    }

    // Enter - set active input device
    if (input & InputCommands::Enum::eUnPause_OrConfirm)
    {
        sJoystickEnabled = sControllerEntryToSelect_BB43D8;
        Input_Init_Names_491870();
        Input_SaveSettingsIni_492840();
        return MainMenuNextCam(MainMenuCams::eOptionsCam);
    }

#if MISC_PC_MENU_FIXES // OG Change - Allows player to select "Configure Controller" using their gamepad (Square/Circle Button)
    const bool configButtonPressed = input & (InputCommands::Enum::eConfigure | InputCommands::Enum::eDoAction | InputCommands::Enum::eThrowItem);
#else
    const bool configButtonPressed = input & InputCommands::Enum::eConfigure;
#endif

    if (configButtonPressed)
    {
        // c configure controller
        sJoystickEnabled = sControllerEntryToSelect_BB43D8;
        Input_Init_Names_491870();
        return MainMenuNextCam(MainMenuCams::eRemapInputsCam);
    }
    else
    {
        // Nothing pressed, stay on this screen
        return MainMenuNextCam(MainMenuCams::eNoChange);
    }
}

const InputCommands::Enum kIdxToInput_561F14[8] = {
    InputCommands::Enum::eRun,
    InputCommands::Enum::eSneak,
    InputCommands::Enum::eHop,
    InputCommands::Enum::eSpeak1,
    InputCommands::Enum::eDoAction,
    InputCommands::Enum::eThrowItem,
    InputCommands::Enum::eFartOrRoll,
    InputCommands::Enum::eSpeak2};

MainMenuNextCam MainMenuController::RemapInput_Update_4D1820(u32 input)
{
    if (dword_BB43F8)
    {
        if (dword_BB43F8 == 1 && Input().IsReleased(InputCommands::Enum::eUnPause_OrConfirm))
        {
            dword_BB43F8 = 2;
            return MainMenuNextCam(MainMenuCams::eNoChange);
        }

        if (field_208_transition_obj->field_26_bDone)
        {
            dword_BB43F8 = 3;
            if (Input_Remap_492680(kIdxToInput_561F14[sButtonToRemapIdx_BB43EC]))
            {
                dword_BB43F8 = 0;
                field_208_transition_obj->StartTrans(Layer::eLayer_FadeFlash_40, 0, 0, 16);
            }
        }
    }
    else
    {
        // prepare animating the blinking outline of the rectangular input slots
        field_158_animation.Set_Animation_Data(GetAnimRes(AnimId::MenuHighlight_ButtonRemapSquare));

        if (input & InputCommands::Enum::eUp)
        {
            sButtonToRemapIdx_BB43EC--;

            if (sButtonToRemapIdx_BB43EC < 0)
            {
                sButtonToRemapIdx_BB43EC = 7;
            }

            if (!sJoystickEnabled && (sButtonToRemapIdx_BB43EC == 7 || sButtonToRemapIdx_BB43EC == 3))
            {
                sButtonToRemapIdx_BB43EC--;
            }

            SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
        }

        if (input & InputCommands::Enum::eDown)
        {
            sButtonToRemapIdx_BB43EC++;

            if (!sJoystickEnabled && (sButtonToRemapIdx_BB43EC == 7 || sButtonToRemapIdx_BB43EC == 3))
            {
                sButtonToRemapIdx_BB43EC++;
            }

            if (sButtonToRemapIdx_BB43EC > 7)
            {
                sButtonToRemapIdx_BB43EC = 0;
            }
            SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
        }

        if (input & InputCommands::Enum::eLeft)
        {
            if (sButtonToRemapIdx_BB43EC >= 4)
            {
                sButtonToRemapIdx_BB43EC -= 4;
            }
            SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
        }

        if (input & InputCommands::Enum::eRight)
        {
            if (sButtonToRemapIdx_BB43EC < 4)
            {
                sButtonToRemapIdx_BB43EC += 4;
            }
            SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
        }

        if (input & InputCommands::Enum::eBack)
        {
            Input_SaveSettingsIni_492840();
            field_1FC_button_index = NO_SELECTABLE_BUTTONS;
            return MainMenuNextCam(MainMenuCams::eAbesMotionKeysCam);
        }

        if (input & InputCommands::Enum::eUnPause_OrConfirm)
        {
            field_208_transition_obj->StartTrans(Layer::eLayer_FadeFlash_40, 1, 0, 16);
            dword_BB43F8 = 1;
            return MainMenuNextCam(MainMenuCams::eNoChange);
        }
    }
    return MainMenuNextCam(MainMenuCams::eNoChange);
}

void MainMenuController::tLoadGame_Unload_4D4360()
{
    field_23C_T80.Clear(Flags::eBit15_unused);

    sub_4A2D40();

    if (field_23C_T80.Get(Flags::eBit21_LoadingSave))
    {
        /*
        for (auto& ppRes : field_F4_resources.field_0_resources)
        {
            if (mAnim.field_20_ppBlock != ppRes)
            {
                ResourceManager::FreeResource_49C330(ppRes);
                ppRes = nullptr;
            }
        }*/
    }
}

void MainMenuController::Game_Force_Quit_Load_4D1A90()
{
    sBreakGameLoop_5C2FE0 = TRUE;
}

MainMenuNextCam MainMenuController::HandleGameSpeakInput(u32 input_held, std::function<MainMenuNextCam(InputCommands::Enum cmd)> fnOnGameSpeak)
{
    mAnim.mFlags.Set(AnimFlags::eBit3_Render);
    mAnim.mFlags.Set(AnimFlags::eBit2_Animate);

    if (field_230_target_entry_index == 8)
    {
        return MainMenuNextCam(MainMenuCams::eNoChange);
    }

    if (Input_IsChanting_45F260())
    {
        field_230_target_entry_index = 1;
        return fnOnGameSpeak(InputCommands::Enum::eChant);
    }
    // Hi
    else if (input_held & InputCommands::Enum::eGameSpeak1) // 0x400
    {
        field_230_target_entry_index = 0;
        return fnOnGameSpeak(InputCommands::Enum::eGameSpeak1);
    }
    // Git 'im
    else if (input_held & InputCommands::Enum::eGameSpeak4) // 0x2000
    {
        field_230_target_entry_index = 1;
        return fnOnGameSpeak(InputCommands::Enum::eGameSpeak4);
    }
    // Freeze
    else if (input_held & InputCommands::Enum::eGameSpeak3) // 0x1000
    {
        field_230_target_entry_index = 2;
        return fnOnGameSpeak(InputCommands::Enum::eGameSpeak3);
    }
    // Here boy
    else if (input_held & InputCommands::Enum::eGameSpeak2) // 0x800
    {
        field_230_target_entry_index = 3;
        return fnOnGameSpeak(InputCommands::Enum::eGameSpeak2);
    }
    // Bs
    else if (input_held & InputCommands::Enum::eGameSpeak6) // 0x8000
    {
        field_230_target_entry_index = 4;
        return fnOnGameSpeak(InputCommands::Enum::eGameSpeak6);
    }
    // Look out
    else if (input_held & InputCommands::Enum::eGameSpeak7) // 0x10000
    {
        field_230_target_entry_index = 5;
        return fnOnGameSpeak(InputCommands::Enum::eGameSpeak7);
    }
    // S'mo bs
    else if (input_held & InputCommands::Enum::eGameSpeak5) // 0x4000
    {
        field_230_target_entry_index = 6;
        return fnOnGameSpeak(InputCommands::Enum::eGameSpeak5);
    }
    // Laugh
    else if (input_held & InputCommands::Enum::eGameSpeak8) // 0x20000
    {
        field_230_target_entry_index = 7;
        return fnOnGameSpeak(InputCommands::Enum::eGameSpeak8);
    }
    else if (input_held & InputCommands::Enum::eBack) // 0x200000
    {
        // Exit

        field_230_target_entry_index = 8;
        field_1FC_button_index = NO_SELECTABLE_BUTTONS;

        if (field_210_pUnused)
        {
            // TODO: Recover type
            u16* pUnknown = (u16*) field_210_pUnused;
            pUnknown[124] = 1;
            field_210_pUnused = nullptr;
        }

        return fnOnGameSpeak(InputCommands::Enum::eBack);
    }
    else
    {
        if (field_210_pUnused)
        {
            // TODO: Recover type
            u16* pUnknown = (u16*) field_210_pUnused;
            pUnknown[124] = 1;
            field_210_pUnused = nullptr;
        }
        return MainMenuNextCam(MainMenuCams::eNoChange);
    }
}

void MainMenuController::HandleCreditsControllerUpdate()
{
    if (Input().isPressed(InputCommands::Enum::eBack))
    {
        gCreditsControllerExists = 0;
        gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, 6, CameraSwapEffects::eInstantChange_0, 0, 0);
        return;
    }

    if (field_1F4_credits_next_frame <= static_cast<s32>(sGnFrame))
    {
        const auto currentCam = field_240_credits_current_cam + 1;
        field_240_credits_current_cam = currentCam;
        field_1F4_credits_next_frame = sGnFrame + 160;
        if (gMap.mCurrentPath == 2)
        {
            if (currentCam > 22)
            {
                field_240_credits_current_cam = 1;
                gMap.SetActiveCam(EReliveLevelIds::eCredits, 1, static_cast<s16>(field_240_credits_current_cam), CameraSwapEffects::eTopToBottom_3, 0, 0);
            }
            else
            {
                gMap.SetActiveCam(EReliveLevelIds::eCredits, 2, static_cast<s16>(currentCam), CameraSwapEffects::eTopToBottom_3, 0, 0);
            }
        }
        else
        {
            if (currentCam > 36)
            {
                gCreditsControllerExists = 0;
                gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, 6, CameraSwapEffects::eInstantChange_0, 0, 0);
                return;
            }
            gMap.SetActiveCam(EReliveLevelIds::eCredits, 1, static_cast<s16>(field_240_credits_current_cam), CameraSwapEffects::eTopToBottom_3, 0, 0);
        }
    }
}

// Todo: needs cleanup
void MainMenuController::HandleMainMenuUpdate()
{
    if (gMap.mCurrentCamera == MainMenuCams::eRemapInputsCam || gMap.mCurrentCamera == MainMenuCams::eDummyBlankCam || gMap.mCurrentCamera == MainMenuCams::eLoadGameMenuCam)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, this, 0, 0);
    }
    else if (gMap.mCurrentCamera == MainMenuCams::eBackstory_Or_NewGameCam || gMap.mCurrentCamera == MainMenuCams::eGameIsLoading_ShaddapCam)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
    }
    else
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
    }

    UpdateHighliteGlow_4D0630();

    if (ChangeScreenAndIntroLogic_4CF640())
    {
        AnimationAndSoundLogic_4CFE80();
        return;
    }

    AnimationAndSoundLogic_4CFE80();

    if (field_23C_T80.Get(Flags::eBit17_bDisableChangingSelection))
    {
        return;
    }

    MainMenuPage* pPage = &sMainMenuPages_561960[field_214_page_index];

    const auto currentCamId = pPage->field_0_cam_id;
    if (Input().mPads[0].mPressed && currentCamId != MainMenuCams::eGameBootCopyrightSplashCam && currentCamId != MainMenuCams::eUnknown20Cam && currentCamId != MainMenuCams::eControllerSelectionCam)
    {
        field_1F8_page_timeout = 0;
    }
    else
    {
        ++field_1F8_page_timeout;
    }

    if (pPage->field_4_time_out <= 0 || pPage->field_8_next_idx <= 0 || field_1F8_page_timeout <= pPage->field_4_time_out)
    {
        const MainMenuButton* btnArray = pPage->field_18_buttons;
        const u32 inputHeld = Input().mPads[0].mHeld;

        if (btnArray)
        {
            if (field_1FC_button_index != NO_SELECTABLE_BUTTONS)
            {
                if (inputHeld & (InputCommands::Enum::eLeft | InputCommands::Enum::eUp))
                {
                    if (pPage->field_0_cam_id != MainMenuCams::eLoadGameMenuCam)
                    {
                        for (;;)
                        {
                            field_1FC_button_index--;
                            if (field_1FC_button_index < 0)
                            {
                                s16 lastIndex = 0;
                                while (btnArray[lastIndex].field_0_type == MainMenuButtonType::eCircularSelectableButton)
                                {
                                    lastIndex++;
                                }
                                field_1FC_button_index = lastIndex;
                            }
                            else if (btnArray[field_1FC_button_index].field_0_type == MainMenuButtonType::eCircularSelectableButton)
                            {
                                break;
                            }
                        }
                    }

                    field_158_animation.Set_Animation_Data(GetAnimRes(pPage->field_18_buttons[field_1FC_button_index].field_8_anim_id));

                    SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 35, 400);
                }

                if (inputHeld & (InputCommands::Enum::eRight | InputCommands::Enum::eDown))
                {
                    if (pPage->field_0_cam_id != MainMenuCams::eLoadGameMenuCam)
                    {
                        for (;;)
                        {
                            field_1FC_button_index++;
                            if (btnArray[field_1FC_button_index].field_0_type == MainMenuButtonType::eNoButton)
                            {
                                field_1FC_button_index = 0;
                                break;
                            }
                            else if (btnArray[field_1FC_button_index].field_0_type == MainMenuButtonType::eCircularSelectableButton)
                            {
                                break;
                            }
                        }
                    }

                    field_158_animation.Set_Animation_Data(GetAnimRes(pPage->field_18_buttons[field_1FC_button_index].field_8_anim_id));
                    SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 35, 400);
                }
            }
        }

        if (field_21E_changeScreenState)
        {
            return;
        }

        if (field_23C_T80.Get(Flags::eBit22_GameSpeakPlaying))
        {
            return;
        }

        if (!pPage->field_10_fn_update)
        {
            return;
        }

        const MainMenuNextCam pageUpdateReturnedCam = (this->*(pPage->field_10_fn_update))(inputHeld);

        if (pageUpdateReturnedCam.page_update_camera == MainMenuCams::eNoChange || pageUpdateReturnedCam.page_update_camera == gMap.mCurrentCamera)
        {
            // stay on the same screen
            return;
        }

        field_218_target_page_index = static_cast<s16>(GetPageIndexFromCam_4D05A0(pageUpdateReturnedCam.page_update_camera));
        field_21A_target_button_index = pageUpdateReturnedCam.button_idx_to_highlight;

        // Originally was:
        //field_21C_bDoScreenTransistionEffect = BYTE1(pageUpdateReturnedCam.page_update_camera);
        // No need to read BYTE1 of the cam though, as it's always 0.
        field_21C_camSwapEffectState = camTransEffectState::eDone_0;
    }
    else
    {
        field_1F8_page_timeout = 0;
        field_218_target_page_index = static_cast<s16>(GetPageIndexFromCam_4D05A0(pPage->field_8_next_idx));
        field_21A_target_button_index = pPage->field_C_initial_button_idx_to_highlight;
        field_21C_camSwapEffectState = static_cast<camTransEffectState>(pPage->field_A_transition_effect);
    }

    field_21E_changeScreenState = 1;
}

void MainMenuController::Unload_Resource(AnimId /*res*/)
{
    // TODO: Re-implement dynamic game speak anim swapping
    // if loading all of them upfront is too heavy on perf/memory

    /*
    // Resource might be pending, wait for it to load if so
    if (!res)
    {
        pResourceManager_5C1BB0->LoadingLoop_465590(FALSE);
    }

    // Now we can free it
    ResourceManager::FreeResource_49C330(res);
    res = nullptr;
    */

    // Prevent animation since its now unloaded
    mAnim.mFlags.Clear(AnimFlags::eBit2_Animate);
}

void MainMenuController::VUpdate()
{
    if (gCreditsControllerExists)
    {
        HandleCreditsControllerUpdate();
    }
    else
    {
        HandleMainMenuUpdate();
    }
}

s32 MainMenuController::GetPageIndexFromCam_4D05A0(s32 camId)
{
    for (s32 i = 0; i < 24; i++)
    {
        if (camId == sMainMenuPages_561960[i].field_0_cam_id)
        {
            return i;
        }
    }

    return 0;
}

void MainMenuController::Set_Anim_4D05E0(s16 frameTableIdx, s16 animFrameNum)
{
    if (frameTableIdx != this->field_220_frame_table_idx || sMainMenuFrameTable_561CC8[frameTableIdx].field_8_anim_enum == frameTableIdx)
    {
        this->field_228_res_idx = frameTableIdx;
        this->field_22A_anim_frame_num = animFrameNum;
    }
}

void MainMenuController::Load_Anim_Pal_4D06A0(Animation* pAnim)
{
    pAnim->ReloadPal();
}

ALIVE_VAR(1, 0x5ca408, u32, sLevelId_dword_5CA408, 0);

s32 MainMenuController::ChangeScreenAndIntroLogic_4CF640()
{
    if (field_21E_changeScreenState == 0 || field_23C_T80.Get(Flags::eBit22_GameSpeakPlaying) || field_228_res_idx != 0)
    {
        return 0;
    }

    CameraSwapEffects screenChangeEffect = CameraSwapEffects::eInstantChange_0;

    switch (field_21E_changeScreenState)
    {
        case 1:
            if (field_21C_camSwapEffectState != camTransEffectState::eDone_0)
            {
                if (field_21C_camSwapEffectState != camTransEffectState::eDone_2)
                {
                    field_21E_changeScreenState = 2;
                    return 1;
                }

                if (field_208_transition_obj)
                {
                    field_208_transition_obj->StartTrans(Layer::eLayer_FadeFlash_40, 1, 0, 16);
                    field_21E_changeScreenState = 2;
                    return 1;
                }

                field_208_transition_obj = relive_new MainMenuTransition(Layer::eLayer_FadeFlash_40, 1, 0, 16, TPageAbr::eBlend_2);
                if (field_208_transition_obj)
                {
                    field_21E_changeScreenState = 2;
                    return 1;
                }
            }
            else
            {
                if (field_208_transition_obj)
                {
                    field_208_transition_obj->StartTrans(Layer::eLayer_FadeFlash_40, 1, 0, 16);
                    field_21E_changeScreenState = 2;
                    return 1;
                }

                field_208_transition_obj = relive_new MainMenuTransition(Layer::eLayer_FadeFlash_40, 1, 0, 16, TPageAbr::eBlend_1);
                if (field_208_transition_obj)
                {
                    field_21E_changeScreenState = 2;
                    return 1;
                }
            }
            // ae_new failure case
            field_21E_changeScreenState = 2;
            return 1;

        case 2:
            if (sMainMenuPages_561960[field_214_page_index].field_A_transition_effect == camTransEffectState::eUnknown_7)
            {
                char_type buffer[256] = {};
                // Use path 2
                strcpy(buffer, sCdEmu_Path2_C144C0);
                strcat(buffer, "movies");

                if (!IO_DirectoryExists(buffer))
                {
                    // Can't enumerate anything at all in path 2, try path 3
                    strcpy(buffer, sCdEmu_Path3_C145A0);
                    strcat(buffer, "movies");
                }

                s32 i = 0;
                for (;;) // TODO: Switch to using the len/size of sCdRomDrives_5CA488 when reversed
                {
                    if (!sCdRomDrives_5CA488[i])
                    {
                        // Out of CD drives to try
                        buffer[0] = 0;
                        break;
                    }

                    buffer[0] = sCdRomDrives_5CA488[i];
                    if (IO_DirectoryExists(buffer))
                    {
                        // Found a valid drive
                        break;
                    }

                    i++;
                }

                if (!buffer[0])
                {
                    // Displays the "Abes Exoddus" full screen message you see on boot.
                    // You will probably always see this given that the CD drive with the game in it
                    // usually isn't there.
                    Display_Full_Screen_Message_Blocking(1, MessageType::eShortTitle_3);
                }

                // Find the record for GTILOGO.STR
                FmvInfo* pFmvRecord = Path_Get_FMV_Record(EReliveLevelIds::eMenu, 3u);
                while (Input_IsVKPressed_4EDD40(VK_RETURN))
                {
                    SYS_EventsPump_494580();
                }

                u32 pos = 0; // Gets set to 0x11111111
                Get_fmvs_sectors(pFmvRecord->field_0_pName, 0, 0, &pos, 0, 0);
                sLevelId_dword_5CA408 = 0;

                // Create a movie object for the GTI logo
                auto pMovie = relive_new Movie(pFmvRecord->field_4_id,
                                            pos,
                                            pFmvRecord->field_6_flags & 1,
                                            pFmvRecord->field_8_flags,
                                            pFmvRecord->field_A_volume);

                // Run the movie till its done
                while (sMovie_ref_count_BB4AE4 > 0)
                {
                    if (pMovie->mBaseGameObjectFlags.Get(BaseGameObject::eUpdatable_Bit2))
                    {
                        if (pMovie->mBaseGameObjectFlags.Get(BaseGameObject::eDead) == false && (!sNum_CamSwappers_5C1B66 || pMovie->mBaseGameObjectFlags.Get(BaseGameObject::eUpdateDuringCamSwap_Bit10)))
                        {
                            pMovie->VUpdate();
                        }
                    }
                }

                while (Input_IsVKPressed_4EDD40(VK_RETURN))
                {
                    SYS_EventsPump_494580();
                }

                // Create movie object for the DD logo
                Get_fmvs_sectors("DDLOGO.STR", 0, 0, &pos, 0, 0);
                sLevelId_dword_5CA408 = 0;
                pMovie = relive_new Movie(pFmvRecord->field_4_id,
                                       pos,
                                       pFmvRecord->field_6_flags & 1,
                                       pFmvRecord->field_8_flags,
                                       pFmvRecord->field_A_volume);

                // Run the movie till its done
                while (sMovie_ref_count_BB4AE4 > 0)
                {
                    if (pMovie->mBaseGameObjectFlags.Get(BaseGameObject::eUpdatable_Bit2))
                    {
                        if (pMovie->mBaseGameObjectFlags.Get(BaseGameObject::eDead) == false && (!sNum_CamSwappers_5C1B66 || pMovie->mBaseGameObjectFlags.Get(BaseGameObject::eUpdateDuringCamSwap_Bit10)))
                        {
                            pMovie->VUpdate();
                        }
                    }
                }

                gPsxDisplay.PutCurrentDispEnv();

                gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, 1, CameraSwapEffects::ePlay1FMV_5, 10502, 0);
                mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuDoor));
                Load_Anim_Pal_4D06A0(&mAnim);

                field_23C_T80.Set(Flags::eBit17_bDisableChangingSelection);
                field_220_frame_table_idx = 9;
                field_228_res_idx = 0;
                field_21E_changeScreenState = 3;
                return 1;
            }

            switch (field_21C_camSwapEffectState)
            {
            case camTransEffectState::eDone_0:
            case camTransEffectState::eDone_2:
                if (field_208_transition_obj->field_26_bDone)
                {
                    break;
                }
                return 1;

            case camTransEffectState::eTopToBottom_3:
                screenChangeEffect = CameraSwapEffects::eTopToBottom_3;
                break;

            case camTransEffectState::eBottomToTop_4:
                screenChangeEffect = CameraSwapEffects::eBottomToTop_4;
                break;

            case camTransEffectState::eLeftToRight_5:
                screenChangeEffect = CameraSwapEffects::eLeftToRight_1;
                break;

            case camTransEffectState::eRightToLeft_6:
                screenChangeEffect = CameraSwapEffects::eRightToLeft_2;
                break;

            default:
                break;
            }

            gMap.SetActiveCam(
                EReliveLevelIds::eMenu,
                1,
                sMainMenuPages_561960[field_218_target_page_index].field_0_cam_id,
                screenChangeEffect,
                0,
                0);

            mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
            mAnim.mFlags.Clear(AnimFlags::eBit2_Animate);
            field_21E_changeScreenState = 3;
            return 1;

        case 3:
            if (sMainMenuPages_561960[field_214_page_index].field_20_fn_on_free)
            {
                (this->*sMainMenuPages_561960[field_214_page_index].field_20_fn_on_free)();
            }
            field_21E_changeScreenState = 4;
            return 1;

        case 4:
        {
            if (sNum_CamSwappers_5C1B66 > 0)
            {
                return 1;
            }

            if (field_21C_camSwapEffectState == camTransEffectState::eDone_0 || field_21C_camSwapEffectState == camTransEffectState::eDone_2)
            {
                field_208_transition_obj->StartTrans(Layer::eLayer_FadeFlash_40, 0, 0, 16);
            }

            mAnim.mFlags.Set(AnimFlags::eBit3_Render);
            field_216_previous_page_index = field_214_page_index;
            field_214_page_index = field_218_target_page_index;
            field_21E_changeScreenState = 5;
            const MainMenuPage targetPage = sMainMenuPages_561960[field_218_target_page_index];

            if (targetPage.field_18_buttons != nullptr)
            {
                field_1FC_button_index = field_21A_target_button_index;

                // PSX leftover:
                // This logic only seems to have been used on the PSX loadgame page,
                // as that's the only page which has NO_SELECTABLE_BUTTONS set,
                // yet still has a button with mRingObjectType == eCircularSelectableButton in its btnArray.
                //
                // What it does is it searches for the first highlightable button
                // on the loadgame page, if any (i.e. when there are valid saves on the memory card).
                //
                // Useless on PC as the loadgame page layout is different and does not have
                // any highlightable/selectable buttons, only static ones.
                //
                //if (field_1FC_button_index == NO_SELECTABLE_BUTTONS)
                //{
                //MainMenuButton* pButtonsIter = targetPage.field_18_buttons;
                //s16 useButtonIdx = 0;
                //for (;;)
                //{
                //    // Last row of buttons, don't look further
                //    if (pButtonsIter->mRingObjectType == MainMenuButtonType::eNoButton)
                //    {
                //        break;
                //    }
                //
                //    // Found button to highlight
                //    if (pButtonsIter->mRingObjectType == MainMenuButtonType::eCircularSelectableButton)
                //    {
                //        field_1FC_button_index = useButtonIdx;
                //        break;
                //    }
                //
                //    useButtonIdx++;
                //    pButtonsIter++;
                //}
                //}

                if (field_1FC_button_index != NO_SELECTABLE_BUTTONS)
                {
                    // prepare for animating the blinking outline of the currently highlighted button
                    field_158_animation.Set_Animation_Data(
                        GetAnimRes(targetPage.field_18_buttons[field_1FC_button_index].field_8_anim_id));
                }
            }


            if (sMainMenuPages_561960[field_214_page_index].field_1C_fn_on_load)
            {
                (this->*targetPage.field_1C_fn_on_load)();
            }

            return 1;
        }

        case 5:
            if (field_21C_camSwapEffectState != camTransEffectState::eUnknown_7)
            {
                mAnim.mFlags.Set(AnimFlags::eBit2_Animate);
                mAnim.mFlags.Set(AnimFlags::eBit3_Render);
            }

            if ((field_21C_camSwapEffectState == camTransEffectState::eDone_0 || field_21C_camSwapEffectState == camTransEffectState::eDone_2) 
                && !field_208_transition_obj->field_26_bDone)
            {
                return 1;
            }

            field_21E_changeScreenState = 0;
            return 0;
        default:
            return 0;
    }
}

// TODO: repetition with `Scrab.cpp`
const relive::SfxDefinition mainMenu_sScrabSfx_560330[10] = {
    {0u, 4u, 60u, 55u, 0, 0},
    {0u, 4u, 61u, 70u, 0, 0},
    {0u, 4u, 62u, 80u, 0, 0},
    {0u, 4u, 63u, 80u, 0, 0},
    {0u, 4u, 64u, 60u, -127, 127},
    {0u, 4u, 66u, 90u, 0, 0},
    {0u, 4u, 67u, 50u, -511, 0},
    {0u, 4u, 67u, 50u, 0, 511},
    {0u, 4u, 68u, 110u, -1791, -1791},
    {0u, 0u, 0u, 0u, 0, 0}};



void MainMenuController::AnimationAndSoundLogic_4CFE80()
{
    if (!sMainMenuPages_561960[field_214_page_index].field_E_show_character)
    {
        field_22C_T80_animation_delay = 0;
        return;
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame) && !field_22C_T80_animation_delay)
    {
        if (!field_228_res_idx)
        {
            Set_Anim_4D05E0(
                sMainMenuFrameTable_561CC8[field_220_frame_table_idx].field_8_anim_enum,
                sMainMenuFrameTable_561CC8[field_220_frame_table_idx].field_A_frame_number);
        }

        switch (field_220_frame_table_idx)
        {
            case MainMenuGamespeakAnimIds::eAbe_Idle:
                if (field_224_timer_anim_delay <= static_cast<s32>(sGnFrame))
                {
                    Set_Anim_4D05E0(eAbe_IdleBlinking, 0);
                    field_224_timer_anim_delay = sGnFrame + Math_RandomRange(120, 450);
                }
                break;

            case MainMenuGamespeakAnimIds::eAbe_EnterThroughDoor:
                mBaseGameObjectFlags.Clear(BaseGameObject::eUpdateDuringCamSwap_Bit10);
                field_22C_T80_animation_delay = 15;
                [[fallthrough]];
            case MainMenuGamespeakAnimIds::eAbe_EnterThroughDoor2:
                break;

            case MainMenuGamespeakAnimIds::eAbe_EnterThroughDoorHello:
                ResourceManager::Reclaim_Memory_49C470(0);
                field_23C_T80.Clear(Flags::eBit17_bDisableChangingSelection);
                field_224_timer_anim_delay = sGnFrame + Math_RandomRange(300, 450);
                break;

            case MainMenuGamespeakAnimIds::eAbe_Chant:
                if (Input_IsChanting_45F260())
                {
                    Set_Anim_4D05E0(0); // what??
                }
                else
                {
                    SND_SEQ_Stop(SeqId::MudokonChant1_10);

                    if (field_20C_pUnused)
                    {
                        ALIVE_FATAL("Never expected field_20C to be used");
                    }

                    if (field_210_pUnused)
                    {
                        ALIVE_FATAL("Never expected field_210_pUnknown to be used");
                    }


                    field_23C_T80.Clear(Flags::eBit24_Chant_Seq_Playing);
                    Set_Anim_4D05E0(eAbe_ChantEnd);
                }
                break;

            case MainMenuGamespeakAnimIds::eSlig_Idle:
                break;

            case MainMenuGamespeakAnimIds::eGlukkon_Idle:
                if (field_224_timer_anim_delay <= static_cast<s32>(sGnFrame))
                {
                    Set_Anim_4D05E0(eGlukkon_IdleVariation);
                    field_224_timer_anim_delay = sGnFrame + Math_RandomRange(120, 450);
                }
                break;

            case MainMenuGamespeakAnimIds::eScrab_Idle:
                if (field_224_timer_anim_delay <= static_cast<s32>(sGnFrame))
                {
                    Set_Anim_4D05E0(eScrab_IdleVariation);
                    field_224_timer_anim_delay = sGnFrame + Math_RandomRange(120, 450);
                }
                break;

            case MainMenuGamespeakAnimIds::eParamite_Idle:
                if (field_224_timer_anim_delay <= static_cast<s32>(sGnFrame))
                {
                    Set_Anim_4D05E0(eParamite_IdleVariation);
                    field_224_timer_anim_delay = sGnFrame + Math_RandomRange(120, 450);
                }
                break;

            default:
                field_23C_T80.Clear(Flags::eBit22_GameSpeakPlaying);
                break;
        }
    }

    if (sMainMenuFrameTable_561CC8[field_220_frame_table_idx].field_4_menu_res_id == eParamiteSpeak)
    {
        if (field_220_frame_table_idx != eParamite_Idle && field_220_frame_table_idx != eParamite_IdleVariation)
        {
            s32 doSpeakSoundFrame = 0;
            switch (field_220_frame_table_idx)
            {
                case eParamite_Attack:
                    doSpeakSoundFrame = 0;
                    break;

                case eParamite_Howdy:
                    doSpeakSoundFrame = 0;
                    break;

                case eParamite_Cmon:
                    doSpeakSoundFrame = 10;
                    break;

                case eParamite_Stay:
                    doSpeakSoundFrame = 12;
                    break;

                case eParamite_DoIt:
                    doSpeakSoundFrame = 4;
                    break;

                case eParamite_AllAYa:
                    doSpeakSoundFrame = 10;
                    break;
            }

            if (mAnim.mCurrentFrame == doSpeakSoundFrame)
            {
                if (field_220_frame_table_idx == eParamite_AllAYa)
                {
                    SFX_SfxDefinition_Play_Stereo(mainMenu_stru_55D7C0[sMainMenuFrameTable_561CC8[field_220_frame_table_idx].field_6_sound], 127, 127, 64, 64);
                }
                // Attack
                else if (sMainMenuFrameTable_561CC8[field_220_frame_table_idx].field_6_sound == 9)
                {
                    SND_SEQ_Play(SeqId::MainMenuParamiteAttack_20, 1, 127, 127);
                }
                // All other Paramite speak
                else
                {
                    SFX_SfxDefinition_Play_Stereo(mainMenu_stru_55D7C0[sMainMenuFrameTable_561CC8[field_220_frame_table_idx].field_6_sound], 127, 127, 0x7FFF, 0x7FFF);
                }
            }
        }
    }

    if (field_22C_T80_animation_delay)
    {
        field_22C_T80_animation_delay--;
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame)
        || field_220_frame_table_idx == eAbe_Idle
        || field_220_frame_table_idx == eAbe_IdleBlinking
        || field_220_frame_table_idx == eSlig_Idle
        || field_220_frame_table_idx == eGlukkon_Idle
        || field_220_frame_table_idx == eGlukkon_IdleVariation
        || field_220_frame_table_idx == eScrab_Idle
        || field_220_frame_table_idx == eScrab_IdleVariation
        || field_220_frame_table_idx == eParamite_Idle
        || field_220_frame_table_idx == eParamite_IdleVariation)
    {
        if (!field_22C_T80_animation_delay)
        {
            // TODO: Always loaded now
            //if (field_F4_resources.field_0_resources[sMainMenuFrameTable_561CC8[field_228_res_idx].field_4_menu_res_id])
            {
                if (field_228_res_idx > 0)
                {
                    const s16 sound = sMainMenuFrameTable_561CC8[field_228_res_idx].field_6_sound;
                    if (sound != -1)
                    {
                        switch (sMainMenuFrameTable_561CC8[field_228_res_idx].field_4_menu_res_id)
                        {
                            case eAbeSpeak:
                            case eAbeSpeak2:
                                Mudokon_SFX(static_cast<MudSounds>(sMainMenuFrameTable_561CC8[field_228_res_idx].field_6_sound), 0, 0, nullptr);
                                field_23C_T80.Set(Flags::eBit22_GameSpeakPlaying); // BYTE2(0x20)
                                break;

                            case eSligSpeak:
                                Slig_GameSpeak_SFX_4C04F0(static_cast<SligSpeak>(sMainMenuFrameTable_561CC8[field_228_res_idx].field_6_sound), 0, 0, 0);
                                field_23C_T80.Set(Flags::eBit22_GameSpeakPlaying);
                                break;

                            case eGlukkonSpeak:
                                Glukkon::PlaySound_GameSpeak(static_cast<GlukkonSpeak>(sMainMenuFrameTable_561CC8[field_228_res_idx].field_6_sound), 0, 0, 0);
                                field_23C_T80.Set(Flags::eBit22_GameSpeakPlaying);
                                break;

                            case eScrabSpeak:
                                SFX_SfxDefinition_Play_Stereo(
                                    mainMenu_sScrabSfx_560330[sound],
                                    mainMenu_sScrabSfx_560330[sound].field_C_default_volume,
                                    mainMenu_sScrabSfx_560330[sound].field_C_default_volume,
                                    0x7FFF,
                                    0x7FFF);
                                field_23C_T80.Set(Flags::eBit22_GameSpeakPlaying);
                                break;

                            case eParamiteSpeak:
                                field_23C_T80.Set(Flags::eBit22_GameSpeakPlaying);
                                break;

                            default:
                                break;
                        }
                    }

                    if (field_228_res_idx > 0)
                    {

                        if (field_228_res_idx == eAbe_Chant)
                        {
                            SND_SEQ_PlaySeq(SeqId::MudokonChant1_10, 0, 1);
                            field_23C_T80.Set(Flags::eBit24_Chant_Seq_Playing);
                        }

                        mAnim.mFlags.Set(AnimFlags::eBit2_Animate);

                        mAnim.Set_Animation_Data(
                            GetAnimRes(sMainMenuFrameTable_561CC8[field_228_res_idx].field_0_animation));

                        if (sMainMenuFrameTable_561CC8[field_228_res_idx].field_4_menu_res_id != sMainMenuFrameTable_561CC8[field_220_frame_table_idx].field_4_menu_res_id)
                        {
                            Load_Anim_Pal_4D06A0(&mAnim);
                        }

                        mAnim.SetFrame(field_22A_anim_frame_num);

                        if (field_228_res_idx != eAbe_Chant && field_23C_T80.Get(Flags::eBit24_Chant_Seq_Playing))
                        {
                            SND_SEQ_Stop(SeqId::MudokonChant1_10);
                            field_23C_T80.Clear(Flags::eBit24_Chant_Seq_Playing);
                        }

                        field_220_frame_table_idx = field_228_res_idx;
                        field_228_res_idx = 0;
                        field_22A_anim_frame_num = 0;
                    }
                }
            }
        }
    }
}

void MainMenuController::UpdateHighliteGlow_4D0630()
{
    field_1FE_highlite_alpha += field_200_highlite_glow_speed;

    // Invert glow speed to make ping pong effect.
    if (field_1FE_highlite_alpha < 40 || field_1FE_highlite_alpha > 80)
    {
        field_1FE_highlite_alpha += -field_200_highlite_glow_speed;
        field_200_highlite_glow_speed = -field_200_highlite_glow_speed;
    }

    field_158_animation.mRed = static_cast<u8>(field_1FE_highlite_alpha);
    field_158_animation.mBlue = static_cast<u8>(field_1FE_highlite_alpha);
    field_158_animation.mGreen = static_cast<u8>(field_1FE_highlite_alpha);
}

void MainMenuController::DrawMenuText_4D20D0(const MainMenuText* array, PrimHeader** ot, Alive::Font* font, s32* polyIndex, s8 op2)
{
    const bool bSpeak1 = strstr(array->field_8_text, kSpeak1) != 0;
    const bool bSpeak2 = strstr(array->field_8_text, kSpeak2) != 0;

    char_type textBuffer[32] = {};
    String_FormatString(array->field_8_text, textBuffer, ALIVE_COUNTOF(textBuffer), array->field_14 == 0);

    if (op2)
    {
        char_type* plusSignIx = strchr(textBuffer, '+');
        if (plusSignIx)
        {
            strcpy(textBuffer, plusSignIx + 1);
        }
    }

    if (!array->field_14)
    {
        if (textBuffer[0])
        {
            if (bSpeak1 != bSpeak2)
            {
                strcat(textBuffer, "+");
            }
        }
    }

    FP fontScale;
    if (array->field_10_scale == 0.0)
    {
        fontScale = FP_FromDouble(0.84f); // 0xD70A
    }
    else
    {
        fontScale = FP_FromDouble(array->field_10_scale);
    }

    const s32 array_field_x = array->x;
    const s32 array_field_y = array->y;

    s16 text_xpos = 0;
    if (array->field_C_align == 1)
    {
        text_xpos = static_cast<s16>(PsxToPCX(array_field_x));
    }
    else
    {
        s32 fontWidth = font->MeasureScaledTextWidth(textBuffer, fontScale);
        s32 halfFontWidth = fontWidth / -2;
        if (array->field_C_align == 2)
        {
            halfFontWidth = -fontWidth;
        }
        text_xpos = static_cast<s16>(strlen(textBuffer) + PsxToPCX(halfFontWidth + array_field_x));
    }

    const FP text_ypos = FP_FromInteger(-10) * fontScale; // -655360

    const u8 oldDrawInScreenSpace = sFontDrawScreenSpace_5CA4B4;
    sFontDrawScreenSpace_5CA4B4 = 1;

    *polyIndex = font->DrawString_4337D0(
        ot,
        textBuffer,
        text_xpos,
        static_cast<s16>(FP_GetExponent(text_ypos) + array_field_y + 1),
        TPageAbr::eBlend_0,
        1,
        0,
        Layer::eLayer_Above_FG1_39,
        0,
        255,
        0,
        *polyIndex,
        fontScale,
        640,
        0);

    sFontDrawScreenSpace_5CA4B4 = oldDrawInScreenSpace;
}
