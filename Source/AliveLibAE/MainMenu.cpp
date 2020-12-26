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
#include "Particle.hpp"
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

MainMenuController * MainMenuController::gMainMenuController = nullptr;

ALIVE_VAR(1, 0xbb4400, int, sMainMenuObjectCounter_BB4400, 0);
ALIVE_VAR(1, 0x5c1b92, __int16, sMainMenuFontLoaded_5C1B92, 0);

unsigned char pal_560F80[] =
{
    0x00, 0x00, 0x21, 0x84, 0x42, 0x88, 0x63, 0x8C, 0x84, 0x90,
    0xA5, 0x94, 0xE7, 0x9C, 0x08, 0x21, 0x29, 0x25, 0x4A, 0x29,
    0x6B, 0x2D, 0x8C, 0x31, 0xAD, 0x35, 0xEF, 0x3D, 0x10, 0x42,
    0x73, 0x4E
};

ALIVE_VAR(1, 0x5c1bee, char, sEnableCheatLevelSelect_5C1BEE, 0);
ALIVE_VAR(1, 0x5c1bec, char, sEnableCheatFMV_5C1BEC, 0);

ALIVE_VAR(1, 0x5c1b9e, __int16, sDemoIdChosenFromDemoMenu_5C1B9E, 0);

ALIVE_VAR(1, 0x561538, short, sMenuItemCount_561538, 0);
ALIVE_VAR(1, 0x5C1B50, PerPathMudStats, sSavedKilledMudsPerPath_5C1B50, {});

union DemoOrFmv
{
    PerLvlData* mDemoRec;
    MenuFMV* mFmvRec;
};

ALIVE_VAR(1, 0xbb4414, DemoOrFmv, pDemosOrFmvs_BB4414, {});
ALIVE_VAR(1, 0x5c2f68, const char, byte_5C2F68, 0);

// HACK HACK FIX ME - all of these buttons are in one contiguous array in the real game
// we need to replicate this as the game will access this array with the index of the PREVIOUS screen
// which will be out of bounds

MainMenuButton sBtnArray_FrontEnd_560FA0[] =
{
    { 1, 33, 82, 0, 13912 },
    { 1, 32, 104, 0, 13912 },
    { 1, 337, 219, 0, 13912 },
    { 1, 337, 239, 0, 13912 },
    { 1, 33, 62, 0, 13912 },
    { 0, 0, 0, 0, 0 }
};

MainMenuButton sBtnArray_Cheat_Level_Or_FMVSelection_560FE8[] =
{
    { 3, 34, 65, 0, 13912 },
    { 0, 0, 0, 0, 0 },
    { 2, 51, 119, 0, 13912 },
    { 2, 46, 151, 0, 13912 },
    { 2, 46, 182, 0, 13912 },
    { 2, 55, 211, 0, 13912 },
    { 2, 314, 119, 0, 13912 },
    { 2, 323, 151, 0, 13912 },
    { 2, 323, 182, 0, 13912 },
    { 2, 317, 211, 0, 13912 },
    { 3, 166, 240, 0, 13924 },
    { 0, 0, 0, 0, 0 }
};

MainMenuButton sBtnArray_Game_BackStory_Or_NewGame_561420[] = 
{
    { 1, 59, 199, 0, 13912 },
    { 1, 248, 199, 0, 13912 },
    { 3, 180, 240, 0, 13912 },
    { 0, 0, 0, 0, 0 }
};

MainMenuButton sBtnArray_Gamespeak_561310[] =
{
    { 1, 153, 122, 0, 13912 },
    { 1, 153, 143, 0, 13912 },
    { 1, 153, 164, 0, 13912 },
    { 1, 153, 184, 0, 13912 },
    { 1, 153, 205, 0, 13912 },
    { 3, 313, 240, 0, 13924 },
    { 0, 0, 0, 0, 0 }
};

MainMenuButton sBtnArray_Options_561368[] =
{
    { 1, 33, 62, 0, 13912 },
    { 1, 336, 61, 0, 13912 },
    { 3, 332, 240, 0, 13912 },
    { 0, 0, 0, 0, 0 }
};

MainMenuButton sBtnArray_PSX_1Player_Or_2Player_NewGame_5613C8[] =
{
    { 1, 33, 62, 0, 13912 },
    { 1, 336, 60, 0, 13912 },
    { 3, 331, 240, 0, 13912 },
    { 0, 0, 0, 0, 0 }
};

MainMenuButton sBtnArray_Cooperative_Mode_Prompt_5613F8[] =
{ 
    { 1, 31, 62, 0, 13912 },
    { 3, 331, 240, 0, 13912 },
    { 0, 0, 0, 0, 0 }
};

MainMenuButton sBtnArray_LoadGame_561450[] =
{
    { 1, 32, 62, 0, 13912 },
    { 3, 331, 239, 0, 13912 },
    { 0, 0, 0, 0, 0 },
};



// Sfx Arrays

SfxDefinition stru_55D7C0[] =
{
    { 0u, 5u, 60u, 90u, -520, -520 },
    { 0u, 5u, 61u, 75u, -520, -520 },
    { 0u, 5u, 62u, 50u, -520, -520 },
    { 0u, 5u, 64u, 40u, -520, -520 },
    { 0u, 5u, 65u, 40u, -520, -520 },
    { 0u, 5u, 66u, 30u, -520, -520 },
    { 0u, 5u, 66u, 50u, 0, 0 },
    { 0u, 5u, 67u, 40u, -520, -520 },
    { 0u, 5u, 68u, 30u, -520, -520 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 }
};

ALIVE_ARY(1, 0x561960, MainMenuPage, 24, sMainMenuPages_561960, 
{
    { // Page 0: Controller selection menu
        MainMenuCams::eControllerSelectionCam,
        0,        0,        0,       -1,        0,        0,
        &MainMenuController::ControllerMenu_Update_4D16D0,
        &MainMenuController::ControllerMenu_Render_Text_4D26C0,
        nullptr,
        &MainMenuController::ControllerMenu_Load_4D16B0,
        nullptr
    }, 
    { // Page 1: Input remapping menu
        MainMenuCams::eRemapInputsCam,
        0,        0,        0,       -1,        0,        0,
        &MainMenuController::RemapInput_Update_4D1820,
        &MainMenuController::RemapInput_Render_4D2A10,
        nullptr,
        &MainMenuController::RemapInput_Load_4D17E0,
        nullptr
    }, 
    { // Page 2: Dummy blank screen for quitting
        MainMenuCams::eDummyBlankCam,
        0,        0,        0,       -1,        0,        0,
        nullptr,
        nullptr,
        nullptr,
        &MainMenuController::Game_Force_Quit_Load_4D1A90,
        nullptr
    },
    { // Page 3: Main menu screen where abe says hello
        MainMenuCams::eMainMenuCam,
        0,        0,        0,       -1,       -1,        1,
        &MainMenuController::Page_Front_Update_4D0720,
        &MainMenuController::Page_Front_Render_4D24B0,
        sBtnArray_FrontEnd_560FA0,
        nullptr,
        nullptr
    },
    { // Page 4: Gamespeak character selection screen
        MainMenuCams::eGamespeakCharacterSelectionCam,
        0,        900,      1,        0,        4,        0,
        &MainMenuController::Gamespeak_Update_4D1FC0,
        &MainMenuController::Gamespeak_Render_4D24F0,
        sBtnArray_Gamespeak_561310,
        nullptr,
        nullptr
    },
    { // Page 5: Options 
        MainMenuCams::eOptionsCam,
        0,        900,      1,        0,        3,        1,
        &MainMenuController::Options_Update_4D1AB0,
        &MainMenuController::Options_Render_4D2540,
        sBtnArray_Options_561368,
        nullptr,
        nullptr
    },
    { // Page 6: Abe's motions 
        MainMenuCams::eAbesMotionKeysCam,
        0,        1600,     3,        0,        0,        0,
        &MainMenuController::AbeMotions_Update_4D1F50,
        &MainMenuController::AbeMotions_Render_4D25E0,
        nullptr,
        nullptr,
        nullptr
    },
    { // Page 7: Load game menu
        MainMenuCams::eLoadGameMenuCam,
        0,        1000,     1,        0,        2,        0,
        &MainMenuController::tLoadGame_Input_4D3EF0,
        &MainMenuController::tLoadGame_Render_4D44D0,
        sBtnArray_LoadGame_561450,
        &MainMenuController::tLoadGame_Load_4D42F0,
        &MainMenuController::tLoadGame_Unload_4D4360
    },
    { // Page 8: Solo or Co-op newgame selection (PSX only, unused on PC)
        MainMenuCams::eRemapInputsCam,
        0,        900,      1,        0,        0,        1,
        &MainMenuController::PSX_Gamemode_Selection_Update_4D48C0,
        nullptr,
        sBtnArray_PSX_1Player_Or_2Player_NewGame_5613C8,
        nullptr,
        nullptr
    },
    { // Page 9: Cooperative mode (2-player) description screen (PSX only, unused on PC)
        MainMenuCams::eDummyBlankCam,
        0,        1800,     5,        0,        1,        0,
        &MainMenuController::PSX_Cooperative_Mode_Update_4D49B0,
        nullptr,
        sBtnArray_Cooperative_Mode_Prompt_5613F8,
        nullptr,
        nullptr
    },
    { // Page 10: FMV selection cheat menu
        MainMenuCams::eCheatMenu_SelectFMVCam,
        0,        0,        1,        0,        0,        0,
        &MainMenuController::Page_FMV_Level_Update_4D4AB0,
        &MainMenuController::Demo_And_FMV_List_Render_4D4F30,
        sBtnArray_Cheat_Level_Or_FMVSelection_560FE8,
        &MainMenuController::t_Unload_AbeSpeak_Res_4D49F0,
        &MainMenuController::t_Load_AbeSpeak_Res_4D4A20
    },
    { // Page 11: Level selection cheat menu
        MainMenuCams::eCheatMenu_SelectLevelCam,
        0,        0,        1,        0,        0,        0,
        &MainMenuController::Page_FMV_Level_Update_4D4AB0,
        &MainMenuController::Demo_And_FMV_List_Render_4D4F30,
        sBtnArray_Cheat_Level_Or_FMVSelection_560FE8,
        nullptr,
        nullptr
    },
    { // Page 12: Demo selection menu
        MainMenuCams::eDemoSelectionCam,
        0,        0,        1,        0,        1,        0,
        &MainMenuController::DemoSelect_Update_4D0E10,
        &MainMenuController::Demo_And_FMV_List_Render_4D4F30,
        nullptr,
        nullptr,
        nullptr
    },
    { // Page 13: "Game is loading" splash screen
        MainMenuCams::eGameIsLoading_ShaddapCam,
        0,        0,        0,       -1,       -1,        0,
        &MainMenuController::LoadNewGame_Update_4D0920,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    },
    { // Page 14: "Demo is loading" splash screen
        MainMenuCams::eDemoIsLoading_ShaddapCam,
        0,        0,        0,       -1,       -1,        0,
        &MainMenuController::LoadDemo_Update_4D1040,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    },
    { // Page 15: Unknown, possibly startup-related?
        MainMenuCams::eUnknown20Cam,
        0, 150, 25, 1, -1, 0, nullptr, nullptr, nullptr, nullptr, nullptr
    },
    { // Page 16: Copyright boot screen
        MainMenuCams::eGameBootCopyrightSplashCam,
        0, 150, 1, 7, 0, 0, nullptr, nullptr, nullptr, nullptr, nullptr
    },
    { // Page 17: Unknown, possibly startup-related?
        MainMenuCams::eControllerSelectionCam,
        0, 150, 25, 1, -1, 0, nullptr, nullptr, nullptr, nullptr, nullptr
    },
    { // Page 18: Abe Gamespeak screen
        MainMenuCams::eAbeGamespeakCam,
        0,        1600,     2,        0,        0,        1,
        &MainMenuController::AbeSpeak_Update_4D2D20,
        &MainMenuController::AbeSpeak_Render_4D2060,
        nullptr,
        nullptr,
        nullptr
    },
    { // Page 19: Slig Gamespeak screen
        MainMenuCams::eSligGamespeakCam,
        0,        1600,     2,        0,        1,        1,
        &MainMenuController::SligSpeak_Update_4D3280,
        &MainMenuController::SligSpeak_Render_4D2370,
        nullptr,
        &MainMenuController::SligSpeak_Load_4D3090,
        &MainMenuController::SligSpeak_Unload_4D3170
    },
    { // Page 20: Glukkon Gamespeak screen
        MainMenuCams::eGlukkonGamespeakCam,
        0,        1600,     2,        0,        2,        1,
        &MainMenuController::GlukkonSpeak_Update_4D3670,
        &MainMenuController::GlukkonSpeak_Render_4D23C0,
        nullptr,
        &MainMenuController::GlukkonSpeak_Load_4D3480,
        &MainMenuController::GlukkonSpeak_Unload_4D3560
    },
    { // Page 21: Scrab Gamespeak screen
        MainMenuCams::eScrabGamespeakCam,
        0,        1600,     2,        0,        3,        1,
        &MainMenuController::ScrabSpeak_Update_4D3A60,
        &MainMenuController::ScrabSpeak_Render_4D2410,
        nullptr,
        &MainMenuController::ScrabSpeak_Load_4D3870,
        &MainMenuController::ScrabSpeak_Unload_4D3950
    },
    { // Page 22: Paramite Gamespeak screen
        MainMenuCams::eParamiteGamespeakCam,
        0,        1600,     2,        0,        4,        1,
        &MainMenuController::ParamiteSpeak_Update_4D3D60,
        &MainMenuController::ParamiteSpeak_Render_4D2460,
        nullptr,
        &MainMenuController::ParamiteSpeak_Load_4D3B70,
        &MainMenuController::ParamiteSpeak_Unload_4D3C50
    },
    { // Page 23: Backstory Or Just Start a New Game screen
        MainMenuCams::eBackstory_Or_NewGameCam,
        0,        900,      1,        0,        0,        0,
        &MainMenuController::BackStory_Or_NewGame_Update_4D1C60,
        &MainMenuController::BackStory_Or_NewGame_Render_4D2630,
        sBtnArray_Game_BackStory_Or_NewGame_561420,
        &MainMenuController::BackStory_Or_NewGame_Load_4D1BA0,
        &MainMenuController::BackStory_Or_NewGame_Unload_4D1BE0
    }
});

const char byte_55EE00[2] = { '\x18', '\0' };
const char byte_55EDEC[2] = { '\x19', '\0' };

inline std::int16_t operator "" _s(unsigned long long value)
{
    return static_cast<std::int16_t>(value);
}

MenuFMV sFmvs_561540[28] =
{
    { "GT Logo", LevelIds::eMenu_0, 65535_s, 65535_s, 3_s, 65535_s, 65535_s },
    { "Oddworld Intro", LevelIds::eMenu_0, 65535_s, 65535_s, 1, 65535_s, 65535_s },
    { "Abe's Exoddus", LevelIds::eMenu_0, 65535_s, 65535_s, 5, 65535_s, 65535_s },
    { "Backstory", LevelIds::eMenu_0, 65535_s, 65535_s, 4, 65535_s, 65535_s },
    { "Prophecy", LevelIds::eMines_1, 65535_s, 65535_s, 1, 65535_s, 65535_s },
    { "Vision", LevelIds::eMines_1, 65535_s, 65535_s, 24, 65535_s, 65535_s },
    { "Game Opening", LevelIds::eMines_1, 65535_s, 65535_s, 2, 65535_s, 65535_s },
    { "Brew", LevelIds::eMines_1, 65535_s, 65535_s, 26, 65535_s, 65535_s },
    { "Brew Transition", LevelIds::eMines_1, 65535_s, 65535_s, 31, 65535_s, 65535_s },
    { "Escape", LevelIds::eMines_1, 65535_s, 65535_s, 25, 65535_s, 65535_s },
    { "Reward", LevelIds::eNecrum_2, 65535_s, 65535_s, 22, 65535_s, 65535_s },
    { "FeeCo", LevelIds::eFeeCoDepot_5, 65535_s, 65535_s, 4, 65535_s, 65535_s },
    { "Information Booth", LevelIds::eFeeCoDepot_5, 65535_s, 65535_s, 3, 65535_s, 65535_s },
    { "Train 1", LevelIds::eBarracks_6, 65535_s, 65535_s, 5, 65535_s, 65535_s },
    { "Train 2", LevelIds::eBrewery_9, 65535_s, 65535_s, 15, 65535_s, 65535_s },
    { "Train 3", LevelIds::eBonewerkz_8, 65535_s, 65535_s, 6, 65535_s, 65535_s },
    { "Aslik Info", LevelIds::eFeeCoDepot_5, 65535_s, 65535_s, 2, 65535_s, 65535_s },
    { "Aslik Explodes", LevelIds::eFeeCoDepot_5, 65535_s, 65535_s, 1, 65535_s, 65535_s },
    { "Dripek Info", LevelIds::eBarracks_6, 65535_s, 65535_s, 4, 65535_s, 65535_s },
    { "Dripek Explodes", LevelIds::eBarracks_6, 65535_s, 65535_s, 3, 65535_s, 65535_s },
    { "Phleg Info", LevelIds::eBonewerkz_8, 65535_s, 65535_s, 4, 65535_s, 65535_s },
    { "Phleg Explodes", LevelIds::eBonewerkz_8, 65535_s, 65535_s, 5, 65535_s, 65535_s },
    { "Soulstorm Info", LevelIds::eBrewery_9, 65535_s, 65535_s, 14, 65535_s, 65535_s },
    { "Ingredient", LevelIds::eBrewery_9, 65535_s, 65535_s, 16, 65535_s, 65535_s },
    { "Conference", LevelIds::eBrewery_9, 65535_s, 65535_s, 13, 65535_s, 65535_s },
    { "Happy Ending", LevelIds::eBrewery_9, 65535_s, 65535_s, 17, 65535_s, 65535_s },
    { "Sad Ending", LevelIds::eBrewery_9, 65535_s, 65535_s, 18, 65535_s, 65535_s },
    { "Credits", LevelIds::eCredits_16, 65535_s, 65535_s, 65535_s, 65535_s, 65535_s }
};

// Used by the level skip cheat/ui/menu
PerLvlData gPerLvlData_561700[17] =
{
    { "Mines", LevelIds::eMines_1, 1, 4, 65535u, 2712, 1300 },
    { "Mines Ender", LevelIds::eMines_1, 6, 10, 65535u, 2935, 2525 },
    { "Necrum", LevelIds::eNecrum_2, 2, 1, 65535u, 2885, 1388 },
    { "Mudomo Vault", LevelIds::eMudomoVault_3, 1, 1, 65535u, 110, 917 },
    { "Mudomo Vault Ender", LevelIds::eMudomoVault_Ender_11, 13, 1, 65535u, 437, 454 },
    { "Mudanchee Vault", LevelIds::eMudancheeVault_4, 6, 3, 65535u, 836, 873 },
    { "Mudanchee Vault Ender", LevelIds::eMudancheeVault_Ender_7, 9, 4, 65534u, 1600, 550 },
    { "FeeCo Depot", LevelIds::eFeeCoDepot_5, 1, 1, 65535u, 4563, 972 },
    { "FeeCo Depot Ender", LevelIds::eFeeCoDepot_Ender_12, 11, 5, 65535u, 1965, 1650 },
    { "Barracks", LevelIds::eBarracks_6, 1, 4, 65535u, 1562, 1651 },
    { "Barracks Ender", LevelIds::eBarracks_Ender_13, 11, 5, 65535u, 961, 1132 },
    { "Bonewerkz", LevelIds::eBonewerkz_8, 1, 1, 65535u, 813, 451 },
    { "Bonewerkz Ender", LevelIds::eBonewerkz_Ender_14, 14, 10, 65535u, 810, 710 },
    { "Brewery", LevelIds::eBrewery_9, 16, 6, 65535u, 1962, 1232 },
    { "Game Ender", LevelIds::eBrewery_Ender_10, 1, 1, 65535u, 460, 968 },
    { "Credits", LevelIds::eCredits_16, 1, 1, 65535u, 0, 0 },
    { "Menu", LevelIds::eMenu_0, 1, 1, 65535u, 0, 0 }
};

PerLvlData sDemos_5617F0[23] =
{
    { "Mudokons 1", LevelIds::eMines_1, 8, 5, 0, 0, 0 },
    { "Mudokons 2", LevelIds::eMines_1, 8, 32, 1, 0, 0 },
    { "Mudokons 3", LevelIds::eMines_1, 8, 21, 2, 0, 0 },
    { "Flying Slig", LevelIds::eMines_1, 9, 18, 4, 0, 0 },
    { "Blind Mudokons 1", LevelIds::eMines_1, 11, 27, 5, 0, 0 },
    { "Blind Mudokons 2", LevelIds::eMines_1, 11, 22, 3, 0, 0 },
    { "Minecar", LevelIds::eMines_1, 12, 2, 6, 0, 0 },
    { "Fleeches", LevelIds::eNecrum_2, 7, 1, 7, 0, 0 },
    { "Paramite Chase", LevelIds::eMudomoVault_3, 2, 13, 8, 0, 0 },
    { "Paramites Talk", LevelIds::eMudomoVault_3, 6, 8, 27, 0, 0 },
    { "Scrab and Fleeches", LevelIds::eMudancheeVault_4, 12, 2, 9, 0, 0 },
    { "Invisibility", LevelIds::eMudancheeVault_4, 13, 5, 10, 0, 0 },
    { "Farts-a-poppin'", LevelIds::eFeeCoDepot_5, 6, 3, 12, 0, 0 },
    { "Flying Sligs 2", LevelIds::eFeeCoDepot_5, 12, 1, 14, 0, 0 },
    { "Baggage Claim", LevelIds::eFeeCoDepot_Ender_12, 13, 1, 15, 0, 0 },
    { "Shrykull", LevelIds::eBarracks_6, 3, 10, 16, 0, 0 },
    { "Crawling Sligs", LevelIds::eBarracks_6, 4, 6, 17, 0, 0 },
    { "Slogs Attack", LevelIds::eBonewerkz_8, 11, 7, 18, 0, 0 },
    { "Glukkon", LevelIds::eBonewerkz_Ender_14, 13, 9, 19, 0, 0 },
    { "Angry Mudokons", LevelIds::eBrewery_9, 13, 10, 22, 0, 0 },
    { "Sligs", LevelIds::eBrewery_9, 26, 4, 23, 0, 0 },
    { "Tortured Mudokons", LevelIds::eBrewery_9, 27, 7, 24, 0, 0 },
    { "Greeters Go Boom", LevelIds::eBrewery_9, 28, 4, 25, 0, 0 }
};

MainMenuFrameTable sMainMenuFrameTable_561CC8[49] =
{
    { 0, 0, 0, 0, 0 },
    { 247808, 1, -1, 1, 0 },
    { 247684, 1, -1, 1, 0 },
    { 247620, 1, 3, 1, 0 },
    { 247924, 1, 12, 1, 14 },
    { 693672, 0, 4, 1, 0 },
    { 693504, 0, 11, 1, 14 },
    { 693912, 0, 6, 1, 0 },
    { 693556, 0, 5, 1, 14 },
    { 50212, 3, -1, 10, 0 },
    { 25888, 2, -1, 11, 0 },
    { 247684, 1, -1, 3, 0 },
    { 693772, 0, -1, 0, 0 },
    { 693748, 0, -1, 1, 0 },
    { 694084, 0, 25, 1, 0 },
    { 694132, 0, 17, 1, 0 },
    { 694200, 0, 26, 1, 0 },
    { 693956, 0, 27, 1, 14 },
    { 1105688, 7, -1, 18, 0 },
    { 1105420, 7, 8, 18, 0 },
    { 1105516, 7, 2, 18, 0 },
    { 1105600, 7, 1, 18, 0 },
    { 1105816, 7, 3, 18, 14 },
    { 1105884, 7, 6, 18, 14 },
    { 1105964, 7, 5, 18, 0 },
    { 1106020, 7, 7, 18, 0 },
    { 1106072, 7, 0, 18, 0 },
    { 1371728, 8, -1, 27, 0 },
    { 1371876, 8, -1, 27, 0 },
    { 1372024, 8, 8, 27, 17 },
    { 1372108, 8, 7, 27, 0 },
    { 1372192, 8, 2, 27, 0 },
    { 1371452, 8, 3, 27, 17 },
    { 1371520, 8, 1, 27, 17 },
    { 1371580, 8, 6, 27, 17 },
    { 1371660, 8, 0, 27, 0 },
    { 1372292, 8, 4, 27, 0 },
    { 506044, 9, -1, 37, 0 },
    { 506160, 9, -1, 37, 0 },
    { 505836, 9, 8, 37, 0 },
    { 505756, 9, 0, 37, 0 },
    { 888448, 10, -1, 41, 0 },
    { 888572, 10, -1, 41, 0 },
    { 888936, 10, 9, 41, 0 },
    { 888240, 10, 5, 41, 14 },
    { 888308, 10, 0, 41, 14 },
    { 888696, 10, 1, 41, 14 },
    { 888824, 10, 2, 41, 14 },
    { 889016, 10, 0, 41, 14 }
};

#if DEVELOPER_MODE
bool gBootToLoadScreen = false;
#endif

MainMenuController* MainMenuController::ctor_4CE9A0(Path_TLV* /*pTlv*/, TlvItemInfoUnion tlvOffsetLevelIdPathId)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);

 
    SetVTable(this, 0x547958);
    SetVTable(&field_158_animation, 0x544290);

    sMainMenuObjectCounter_BB4400++;
    field_6_flags.Set(BaseGameObject::eUpdateDuringCamSwap_Bit10);

    gMainMenuController = this;

    if (!sMainMenuFontLoaded_5C1B92)
    {
        sFont1Context_5BC5C8.LoadFontType_433400(1);
        sMainMenuFontLoaded_5C1B92 = 1;
    }

    field_120_font.ctor_433590(240, pal_560F80, &sFont1Context_5BC5C8);
    field_F4_resources = {};

    field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak2] = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbespek2ResID, TRUE, FALSE);
    field_F4_resources.field_0_resources[MenuResIds::eOptionFlare] = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kOptflareResID, TRUE, FALSE);

    field_F4_resources.field_0_resources[MenuResIds::eUnknown] = nullptr;
    field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak] = nullptr;

    if (gMap_5C3030.field_4_current_camera == MainMenuCams::eCheatMenu_SelectFMVCam)
    {
        field_F4_resources.field_0_resources[MenuResIds::eAbeIntro] = nullptr;
        field_F4_resources.field_0_resources[MenuResIds::eDoor] = nullptr;
        Animation_Init_424E10(247808, 233, 0x7Du, field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak2], 1, 1);
    }
    else
    {
        ResourceManager::LoadResourceFile_49C170("ABEINTRO.BAN", nullptr);
        field_F4_resources.field_0_resources[MenuResIds::eAbeIntro] = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeintroResID, TRUE, FALSE);
        ResourceManager::LoadResourceFile_49C170("STDOOR.BAN", nullptr);
        field_F4_resources.field_0_resources[MenuResIds::eDoor] = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kDoorResID, TRUE, FALSE);
        Animation_Init_424E10(50212, 233, 0x7Du, field_F4_resources.field_0_resources[MenuResIds::eDoor], 1, 1);
    }

    field_20_animation.field_A_b = 127;
    field_20_animation.field_9_g = 127;
    field_20_animation.field_8_r = 127;

    
    field_F4_resources.field_0_resources[MenuResIds::eResHighLite] = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, kHighliteResID, TRUE, FALSE);
    field_158_animation.Init_40A030(13912, gObjList_animations_5C1A24, this, 150, 0x41u, field_F4_resources.field_0_resources[MenuResIds::eResHighLite], 1, 0, 0);

    field_158_animation.field_14_scale = field_CC_sprite_scale;

    field_158_animation.field_4_flags.Clear(AnimFlags::eBit16_bBlending);
    field_158_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

    field_158_animation.field_C_render_layer = 38;
    field_158_animation.field_B_render_mode = 1;

    field_1FE_highlite_alpha = 40;
    field_200_highlite_glow_speed = -8;
    field_1F0_tlvOffsetLevelIdPathId = tlvOffsetLevelIdPathId.all; // TODO: Should probably be using the same types here, depending on how it gets used

    field_214_page_index = static_cast<short>(GetPageIndexFromCam_4D05A0(gMap_5C3030.field_4_current_camera));
    field_21C_bDoScreenTransistionEffect = 1;
    field_21E_bChangeScreen = 0;
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

    if (gMap_5C3030.field_4_current_camera == MainMenuCams::eMainMenuCam)
    {
        MainMenuController::Set_Anim_4D05E0(9, 0);
        field_23C_T80.Set(Flags::eBit17_bDisableChangingSelection);
#if DEVELOPER_MODE
        if (gBootToLoadScreen)
        {
            // So we don't have to wait all year for abe to say hello
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

    sSavedKilledMudsPerPath_5C1B50 = {};

    sEnableCheatFMV_5C1BEC = 0;
    sEnableCheatLevelSelect_5C1BEE = 0;
    sKilledMudokons_5C1BC0 = 0;
    sRescuedMudokons_5C1BC2 = 0;
    gAttract_5C1BA0 = 0;
    sSavedKilledMudsPerPath_5C1B50.mData[ALIVE_COUNTOF(sSavedKilledMudsPerPath_5C1B50.mData)-1]= 0;
    sFeeco_Restart_KilledMudCount_5C1BC6 = 0;
    sFeecoRestart_SavedMudCount_5C1BC8 = 0;

    if (gMap_5C3030.field_4_current_camera == MainMenuCams::eCheatMenu_SelectFMVCam)
    {
        field_1FC_button_index = 0;
        field_250_selected_entry_index = 0;
        field_254 = FP_FromInteger(0);
        field_258 = FP_FromInteger(0);
        field_25C_Inside_FMV_Screen = 1;
        pDemosOrFmvs_BB4414.mFmvRec = &sFmvs_561540[0];
        sMenuItemCount_561538 = ALIVE_COUNTOF(sFmvs_561540);
        field_20_animation.Set_Animation_Data_409C80(247808, field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak2]);
        Load_Anim_Pal_4D06A0(&field_20_animation);
        return this;
    }

    if (gMap_5C3030.field_4_current_camera == MainMenuCams::eDemoSelectionCam)
    {
        ResourceManager::Reclaim_Memory_49C470(0);
        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbespeakResID, 0, 0))
        {
            ResourceManager::LoadResourceFile_49C130("ABESPEAK.BAN", reinterpret_cast<ResourceManager::TLoaderFn>(callback_4D06E0), reinterpret_cast<Camera *>(this), 0);
        }
        
        pResourceManager_5C1BB0->LoadingLoop_465590(false);
        field_1FC_button_index = 0;
        field_250_selected_entry_index = sDemoIdChosenFromDemoMenu_5C1B9E;
        field_254 = FP_FromInteger(0);
        field_258 = FP_FromInteger(0);
        pDemosOrFmvs_BB4414.mDemoRec = &sDemos_5617F0[0];
        sMenuItemCount_561538 = ALIVE_COUNTOF(sDemos_5617F0);
        field_230_target_entry_index = sDemoIdChosenFromDemoMenu_5C1B9E;
        field_20_animation.Set_Animation_Data_409C80(247808, field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak2]);
        Load_Anim_Pal_4D06A0(&field_20_animation);
    }
    return this;
}

BaseGameObject* MainMenuController::vdtor_4CEF00(signed int flags)
{
    dtor_4CEF30();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void MainMenuController::dtor_4CEF30()
{
    SetVTable(this, 0x547958);

    Path::TLV_Reset_4DB8E0(field_1F0_tlvOffsetLevelIdPathId, -1, 0, 0);
    field_158_animation.vCleanUp_40C630();

    for (auto& res : field_F4_resources.field_0_resources)
    {
        ResourceManager::FreeResource_49C330(res);
    }

    sMainMenuObjectCounter_BB4400--;

    field_120_font.dtor_433540();
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

BaseGameObject* MainMenuController::VDestructor(signed int flags)
{
    return vdtor_4CEF00(flags);
}

void MainMenuController::VUpdate()
{
    Update_4CF010();
}

void MainMenuController::VRender(int** pOrderingTable)
{
    Render_4CF4C0(pOrderingTable);
}

void MainMenuController::Render_4CF4C0(int** ot)
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit2_Animate)
        && sMainMenuPages_561960[field_214_page_index].field_E_show_character 
        && field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
    {
        field_20_animation.vRender_40B820(184, 162, ot, 0, 0);
        PSX_RECT pRect = {};
        field_20_animation.Get_Frame_Rect_409E10(&pRect);
        pScreenManager_5BB5F4->InvalidateRect_40EC90(pRect.x, pRect.y, pRect.w, pRect.h, pScreenManager_5BB5F4->field_3A_idx);
    }

    const MainMenuButton* pButtons = sMainMenuPages_561960[field_214_page_index].field_18_buttons;
    if (pButtons)
    {
        if (!field_23C_T80.Get(Flags::eBit17_bDisableChangingSelection))
        {
            if (field_1FC_button_index != -1)
            {
                field_158_animation.vRender_40B820(pButtons[field_1FC_button_index].field_2_x, pButtons[field_1FC_button_index].field_4_y, ot, 0, 0);
                PSX_RECT rect = {};
                field_158_animation.Get_Frame_Rect_409E10(&rect);
                pScreenManager_5BB5F4->InvalidateRect_40EC90(rect.x, rect.y, rect.w, rect.h, pScreenManager_5BB5F4->field_3A_idx);
            }
        }
    }

    auto renderFunc = sMainMenuPages_561960[field_214_page_index].field_14_fn_render;
    if (renderFunc)
    {
        (this->*renderFunc)(ot);
    }
}

// Main Menu Text Data
MainMenuText sMMT_FrontPage_5623A0[1] = { 35, 205, "x", 3u, 0u, 0u, 0u,  0.75, 0u, 0u, 0u, 0u };

// Paramite speak text data
const MainMenuText sParamiteSpeak_5622C8[9] =
{
    { 153, 212, "esc", 3u, 0u, 0u, 0u,  0.75, 0u, 0u, 0u, 0u },
    { 75, 55, "\x18", 2u, 0u, 0u, 0u,  0.0, 0u, 0u, 0u, 0u },
    { 240, 57, "\x19", 1u, 0u, 0u, 0u,  0.0, 0u, 0u, 0u, 0u },
    { 62, 85,"\v", 2u, 0u, 0u, 0u,  0.0, 0u, 0u, 0u, 0u },
    { 57, 117, "\f" , 2u, 0u, 0u, 0u,  0.0, 0u, 0u, 0u, 0u },
    { 57, 148, "\r", 2u, 0u, 0u, 0u,  0.0, 0u, 0u, 0u, 0u },
    { 66, 175, "\x0E", 2u, 0u, 0u, 0u,  0.0, 0u, 0u, 0u, 0u },
    { 308, 85, "\x0F" , 1u, 0u, 0u, 0u,  0.0, 0u, 0u, 0u, 0u },
    { 317, 115, "\x10" , 1u, 0u, 0u, 0u,  0.0, 0u, 0u, 0u, 0u }
};

const MainMenuText sAbeGameSpeak_561F38[12] =
{
    { 184, 25, "\x13", 3u, 0u, 0u, 0u,  0.0, 0u, 0u, 0u, 0u },
    { 153, 212, "esc", 3u, 0u, 0u, 0u,  0.75, 0u, 0u, 0u, 0u },
    { 75, 57, "\x18", 2u, 0u, 0u, 0u,  0.0, 0u, 0u, 0u, 0u },
    { 240, 57, "\x19", 1u, 0u, 0u, 0u,  0.0, 0u, 0u, 0u, 0u },
    { 55, 85, "\v", 2u, 0u, 0u, 0u,  0.0, 0u, 0u, 0u, 0u },
    { 52, 112, "\f", 2u, 0u, 0u, 0u,  0.0, 0u, 0u, 0u, 0u },
    { 55, 151, "\r", 2u, 0u, 0u, 0u,  0.0, 0u, 0u, 0u, 0u },
    { 64, 177, "\x0E", 2u, 0u, 0u, 0u,  0.0, 0u, 0u, 0u, 0u },
    { 307, 85, "\x0F", 1u, 0u, 0u, 0u,  0.0, 0u, 0u, 0u, 0u },
    { 314, 115, "\x10", 1u, 0u, 0u, 0u,  0.0, 0u, 0u, 0u, 0u },
    { 319, 145, "\x11", 1u, 0u, 0u, 0u,  0.0, 0u, 0u, 0u, 0u },
    { 306, 175, "\x12", 1u, 0u, 0u, 0u,  0.0, 0u, 0u, 0u, 0u }
};

const MainMenuText sSligGameSpeak_562058[11] =
{
    { 153, 212, "esc", 3u, 0u, 0u, 0u, 0.75, 0u, 0u, 0u, 0u },
    { 75, 55, "\x18", 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u },
    { 240, 57, "\x19", 1u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u },
    { 55, 82, "\v", 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u },
    { 51, 105, "\f", 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u },
    { 51, 144, "\r", 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u },
    { 51, 171, "\x0E", 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u },
    { 317, 84, "\x0F", 1u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u },
    { 317, 108, "\x10", 1u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u },
    { 317, 131, "\x11", 1u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u },
    { 317, 173, "\x12", 1u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u }
};

const MainMenuText sGlukkonGameSpeak_562160[11] =
{
    { 153, 212, "esc", 3u, 0u, 0u, 0u, 0.75, 0u, 0u, 0u, 0u },
    { 75, 55, "\x18", 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u },
    { 240, 57, "\x19", 1u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u },
    { 55, 80, "\v", 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u },
    { 42, 107, "\f", 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u },
    { 43, 137, "\r", 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u },
    { 53, 177, "\x0E", 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u },
    { 317, 85, "\x0F", 1u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u },
    { 325, 117, "\x10", 1u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u },
    { 321, 147, "\x11", 1u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u },
    { 313, 178, "\x12", 1u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u }
};

const MainMenuText sScrabGameSpeak_562268[4] =
{
    { 153, 215, "esc", 3u, 0u, 0u, 0u, 0.75, 0u, 0u, 0u, 0u },
    { 75, 55, "\x18", 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u },
    { 60, 86, "\v", 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u },
    { 60, 133, "\f", 2u, 0u, 0u, 0u, 0.0, 0u, 0u, 0u, 0u }
};

const MainMenuText sAbeMotions_562448[15] =
{
    { 55, 182, "X", 3u, 0u, 0u, 0u, 0.75, 0u, 0u, 0u, 0u },
    { 153, 211, "esc", 3u, 0u, 0u, 0u, 0.75, 0u, 0u, 0u, 0u },            
    { 175, 60, "\x4", 2u, 0u, 0u, 0u, 0.88f, 0u, 0u, 0u, 0u },            //walk right arrow        //rebindables are in []
    { 175, 79, "\x7+ \x4", 2u, 0u, 0u, 0u, 0.88f, 0u, 0u, 0u, 0u },       //sneak [alt] + right arrow 
    { 175, 98, "\x5+ \x4", 2u, 0u, 0u, 0u, 0.88f, 0u, 0u, 0u, 0u },       //run [shift] + right arrow
    { 175, 117, "\t", 2u, 0u, 0u, 0u, 0.88f, 0u, 0u, 0u, 0u },            //jump [space]
    { 175, 136, "\x5+\t", 2u, 0u, 0u, 0u, 0.88f, 0u, 0u, 0u, 0u },        //runjump [shift]+[space]
    { 175, 158, "\x2", 2u, 0u, 0u, 0u, 0.88f, 0u, 0u, 0u, 0u },           //crouch down arrow
    { 330, 60, "\x4", 2u, 0u, 0u, 0u, 0.88f, 0u, 0u, 0u, 0u },            //roll from crouch right arrow
    { 330, 79, "\x1", 2u, 0u, 0u, 0u, 0.88f, 0u, 0u, 0u, 0u },            //zturn up arrow
    { 330, 98, "\x6", 2u, 0u, 0u, 0u, 0.88f, 0u, 0u, 0u, 0u },            //action [ctrl]
    { 334, 117, "Z+\x1A", 2u, 0u, 0u, 0u, 0.88f, 0u, 0u, 0u, 0u },        //throw [z] + Dpad
    { 330, 136, "\n", 2u, 0u, 0u, 0u, 0.88f, 0u, 0u, 0u, 0u },            //fart [x]
    { 330, 155, "\x11", 2u, 0u, 0u, 0u, 0.88f, 0u, 0u, 0u, 0u },          //sympathy [7]
    { 330, 174, "\x0F", 2u, 0u, 0u, 0u, 0.88f, 0u, 0u, 0u, 0u },          //angry slap [5]
};

void MainMenuController::AbeSpeak_Render_4D2060(int** ot)
{
    RenderOnScreenTextHelper(ot, &sAbeGameSpeak_561F38[0], ALIVE_COUNTOF(sAbeGameSpeak_561F38), 1);
}

MainMenuNextCam MainMenuController::AbeSpeak_Update_4D2D20(DWORD input_held)
{
    // 8 is when returning to previous screen
    if (field_230_target_entry_index != 8 && field_23C_T80.Get(Flags::eBit24_Chant_Seq_Playing))
    {
        // Only 1 when chanting
        if (field_230_target_entry_index == 1 && (sGnFrame_5C1B84 % 8) == 0)
        {
            // Spawn chant star/flare particle at random locations around abes head
            field_F4_resources.field_0_resources[MenuResIds::eOptionFlare] = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kOptflareResID, FALSE, FALSE);
            Particle* pParticle = ae_new<Particle>();
            if (pParticle)
            {
                const short randX = Math_RandomRange_496AB0(-40, 40) + 184;
                const short randY = Math_RandomRange_496AB0(30, 90);

                const FP xpos = pScreenManager_5BB5F4->field_20_pCamPos->field_0_x + FP_FromDouble(randX);
                FP ypos = pScreenManager_5BB5F4->field_20_pCamPos->field_4_y + FP_FromDouble(randY);
                ypos.fpValue += 0x44D60C;  // TODO: 68.83 ??

                pParticle->ctor_4CC4C0(
                    xpos,
                    ypos,
                    4176,   // frame table
                    92,     // max w
                    47,     // max h
                    field_F4_resources.field_0_resources[MenuResIds::eOptionFlare]);

                if (pParticle)
                {
                    pParticle->field_20_animation.field_B_render_mode = 1;
                    pParticle->field_20_animation.field_C_render_layer = 39;
                }
            }
        }
        return { MainMenuCams::eNoChange };
    }

    return HandleGameSpeakInput(input_held, [&](InputCommands cmd)
    {
        switch (cmd)
        {
        case InputCommands::eChant: Set_Anim_4D05E0(AnimIds::eAbe_Chant); break;
        case InputCommands::eGameSpeak1: Set_Anim_4D05E0(AnimIds::eAbe_Hello); break;
        case InputCommands::eGameSpeak2: Set_Anim_4D05E0(AnimIds::eAbe_FollowMe); break;
        case InputCommands::eGameSpeak3: Set_Anim_4D05E0(AnimIds::eAbe_Wait); break;
        case InputCommands::eGameSpeak4: Set_Anim_4D05E0(AnimIds::eAbe_Work); break;
        case InputCommands::eGameSpeak5: Set_Anim_4D05E0(AnimIds::eAbe_Anger); break;
        case InputCommands::eGameSpeak6: Set_Anim_4D05E0(AnimIds::eAbe_AllYa); break;
        case InputCommands::eGameSpeak7: Set_Anim_4D05E0(AnimIds::eAbe_Sympathy); break;
        case InputCommands::eGameSpeak8: Set_Anim_4D05E0(AnimIds::eAbe_StopIt); break;
        case InputCommands::eBack: 
            Set_Anim_4D05E0(AnimIds::eAbe_GoodBye);
            // Stop chanting music
            SND_SEQ_Stop_4CAE60(SeqId::MudokonChant1_10);

            // TODO: Extra case for Abe - recover the type
            if (field_20C_pUnused)
            {
                ALIVE_FATAL("Never expected to be used");
            }
            return MainMenuNextCam{ MainMenuCams::eGamespeakCharacterSelectionCam };

        default: return MainMenuNextCam{ MainMenuCams::eNoChange };
        }

        return MainMenuNextCam{ MainMenuCams::eNoChange };
    });
}

void MainMenuController::SligSpeak_Render_4D2370(int** ot)
{
    RenderOnScreenTextHelper(ot, &sSligGameSpeak_562058[0], ALIVE_COUNTOF(sSligGameSpeak_562058), 1);
}

void MainMenuController::GlukkonSpeak_Render_4D23C0(int** ot)
{
    RenderOnScreenTextHelper(ot, &sGlukkonGameSpeak_562160[0], ALIVE_COUNTOF(sGlukkonGameSpeak_562160), 1);
}

void MainMenuController::ScrabSpeak_Render_4D2410(int** ot)
{
    RenderOnScreenTextHelper(ot, &sScrabGameSpeak_562268[0], ALIVE_COUNTOF(sScrabGameSpeak_562268), 1);
}

MainMenuNextCam MainMenuController::SligSpeak_Update_4D3280(DWORD input_held)
{
    return HandleGameSpeakInput(input_held, [&](InputCommands cmd)
    {
        switch (cmd)
        {
        case InputCommands::eGameSpeak1: Set_Anim_4D05E0(AnimIds::eSlig_Hi); break;
        case InputCommands::eGameSpeak2: Set_Anim_4D05E0(AnimIds::eSlig_HereBoy); break;
        case InputCommands::eGameSpeak3: Set_Anim_4D05E0(AnimIds::eSlig_Freeze); break;
        case InputCommands::eGameSpeak4: Set_Anim_4D05E0(AnimIds::eSlig_GetEm); break;
        case InputCommands::eGameSpeak5: Set_Anim_4D05E0(AnimIds::eSlig_SmoBs); break;
        case InputCommands::eGameSpeak6: Set_Anim_4D05E0(AnimIds::eSlig_Bs); break;
        case InputCommands::eGameSpeak7: Set_Anim_4D05E0(AnimIds::eSlig_LookOut); break;
        case InputCommands::eGameSpeak8: Set_Anim_4D05E0(AnimIds::eSlig_Laugh); break;
        case InputCommands::eBack:
            Set_Anim_4D05E0(AnimIds::eSlig_Laugh);
            return MainMenuNextCam{ MainMenuCams::eGamespeakCharacterSelectionCam, 1 };

        default: return MainMenuNextCam{ MainMenuCams::eNoChange };
        }

        return MainMenuNextCam{ MainMenuCams::eNoChange };
    });
}

void MainMenuController::SligSpeak_Unload_4D3170()
{
    Unload_Resource(field_F4_resources.field_0_resources[MenuResIds::eSligSpeak]);
    Load_AbeSpeakResources();
    Set_Anim_4D05E0(AnimIds::eAbe_Idle);
}

void MainMenuController::SligSpeak_Load_4D3090()
{
    Unload_AbeSpeakResources();

    ResourceManager::LoadResourceFile_49C170("SLGSPEAK.BAN", nullptr);
    field_F4_resources.field_0_resources[MenuResIds::eSligSpeak] = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kSligSpeakResID, TRUE, FALSE);
    field_20_animation.Set_Animation_Data_409C80(0x10DF18, field_F4_resources.field_0_resources[MenuResIds::eSligSpeak]);
    Set_Anim_4D05E0(AnimIds::eSlig_Idle);
}

MainMenuNextCam MainMenuController::GlukkonSpeak_Update_4D3670(DWORD input_held)
{
    return HandleGameSpeakInput(input_held, [&](InputCommands cmd)
    {
        switch (cmd)
        {
        case InputCommands::eGameSpeak1: Set_Anim_4D05E0(AnimIds::eGlukkon_Hey); break;
        case InputCommands::eGameSpeak2: Set_Anim_4D05E0(AnimIds::eGlukkon_Commere); break;
        case InputCommands::eGameSpeak3: Set_Anim_4D05E0(AnimIds::eGlukkon_StayHere); break;
        case InputCommands::eGameSpeak4: Set_Anim_4D05E0(AnimIds::eGlukkon_DoIt); break;
        case InputCommands::eGameSpeak5: Set_Anim_4D05E0(AnimIds::eGlukkon_KillEm); break;
        case InputCommands::eGameSpeak6: Set_Anim_4D05E0(AnimIds::eGlukkon_AllOYa); break;
        case InputCommands::eGameSpeak7: Set_Anim_4D05E0(AnimIds::eGlukkon_Help); break;
        case InputCommands::eGameSpeak8: Set_Anim_4D05E0(AnimIds::eGlukkon_Laugh); break;
        case InputCommands::eBack:
            Set_Anim_4D05E0(AnimIds::eGlukkon_Laugh);
            return MainMenuNextCam{ MainMenuCams::eGamespeakCharacterSelectionCam, 2 };

        default: MainMenuNextCam{ MainMenuCams::eNoChange };
        }

        return MainMenuNextCam{ MainMenuCams::eNoChange };
    });
}

void MainMenuController::GlukkonSpeak_Unload_4D3560()
{
    Unload_Resource(field_F4_resources.field_0_resources[MenuResIds::eGlukkonSpeak]);
    Load_AbeSpeakResources();
    Set_Anim_4D05E0(AnimIds::eAbe_Idle);
}

void MainMenuController::GlukkonSpeak_Load_4D3480()
{
    Unload_AbeSpeakResources();

    ResourceManager::LoadResourceFile_49C170("GLKSPEAK.BAN", nullptr);
    field_F4_resources.field_0_resources[MenuResIds::eGlukkonSpeak] = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kGlukkonSpeakResID, TRUE, FALSE);
    field_20_animation.Set_Animation_Data_409C80(0x14EE50, field_F4_resources.field_0_resources[MenuResIds::eGlukkonSpeak]);
    Set_Anim_4D05E0(AnimIds::eGlukkon_Idle);
}

MainMenuNextCam MainMenuController::ScrabSpeak_Update_4D3A60(DWORD input_held)
{
    return HandleGameSpeakInput(input_held, [&](InputCommands cmd)
    {
        switch (cmd)
        {
        case InputCommands::eGameSpeak1: Set_Anim_4D05E0(AnimIds::eScrab_ShredPower); break;
        case InputCommands::eGameSpeak2: Set_Anim_4D05E0(AnimIds::eScrab_Howl); break;
        case InputCommands::eGameSpeak3: break; // Scrabs don't have much to say.
        case InputCommands::eGameSpeak4: break;
        case InputCommands::eGameSpeak5: break;
        case InputCommands::eGameSpeak6: break;
        case InputCommands::eGameSpeak7: break;
        case InputCommands::eGameSpeak8: break;
        case InputCommands::eBack:
            Set_Anim_4D05E0(AnimIds::eScrab_ShredPower);
            return MainMenuNextCam{ MainMenuCams::eGamespeakCharacterSelectionCam, 3 };

        default: return MainMenuNextCam{ MainMenuCams::eNoChange };
        }

        return MainMenuNextCam{ MainMenuCams::eNoChange };
    });
}

void MainMenuController::ScrabSpeak_Unload_4D3950()
{
    Unload_Resource(field_F4_resources.field_0_resources[MenuResIds::eScrabSpeak]);
    Load_AbeSpeakResources();
    Set_Anim_4D05E0(AnimIds::eAbe_Idle);
}

void MainMenuController::ScrabSpeak_Load_4D3870()
{
    Unload_AbeSpeakResources();

    ResourceManager::LoadResourceFile_49C170("SCRSPEAK.BAN", nullptr);
    field_F4_resources.field_0_resources[MenuResIds::eScrabSpeak] = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kScrabSpeakResID, TRUE, FALSE);
    field_20_animation.Set_Animation_Data_409C80(0x7B8BC, field_F4_resources.field_0_resources[MenuResIds::eScrabSpeak]);
    Set_Anim_4D05E0(AnimIds::eScrab_Idle);
}

void MainMenuController::ParamiteSpeak_Render_4D2460(int** ot)
{
    RenderOnScreenTextHelper(ot, &sParamiteSpeak_5622C8[0], ALIVE_COUNTOF(sParamiteSpeak_5622C8), 1);
}

MainMenuNextCam MainMenuController::ParamiteSpeak_Update_4D3D60(DWORD input_held)
{
    return HandleGameSpeakInput(input_held, [&](InputCommands cmd)
    {
        switch (cmd)
        {
        case InputCommands::eGameSpeak1: Set_Anim_4D05E0(AnimIds::eParamite_Howdy); break;
        case InputCommands::eGameSpeak2: Set_Anim_4D05E0(AnimIds::eParamite_Cmon); break;
        case InputCommands::eGameSpeak3: Set_Anim_4D05E0(AnimIds::eParamite_Stay); break;
        case InputCommands::eGameSpeak4: Set_Anim_4D05E0(AnimIds::eParamite_DoIt); break;
        case InputCommands::eGameSpeak5: Set_Anim_4D05E0(AnimIds::eParamite_Attack); break;
        case InputCommands::eGameSpeak6: Set_Anim_4D05E0(AnimIds::eParamite_AllAYa); break;
        case InputCommands::eGameSpeak7: break; // Paramites don't have as much to say.
        case InputCommands::eGameSpeak8: break;
        case InputCommands::eBack:
            Set_Anim_4D05E0(AnimIds::eParamite_Attack);
            return MainMenuNextCam{ MainMenuCams::eGamespeakCharacterSelectionCam, 4 };

        default: return MainMenuNextCam{ MainMenuCams::eNoChange };
        }

        return MainMenuNextCam{ MainMenuCams::eNoChange };
    });
}

void MainMenuController::ParamiteSpeak_Unload_4D3C50()
{
    Unload_Resource(field_F4_resources.field_0_resources[MenuResIds::eParamiteSpeak]);
    Load_AbeSpeakResources();
    Set_Anim_4D05E0(AnimIds::eAbe_Idle);
}

void MainMenuController::ParamiteSpeak_Load_4D3B70()
{
    Unload_AbeSpeakResources();

    ResourceManager::LoadResourceFile_49C170("PARSPEAK.BAN", nullptr);
    field_F4_resources.field_0_resources[MenuResIds::eParamiteSpeak] = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kParamiteSpeakResID, TRUE, FALSE);
    field_20_animation.Set_Animation_Data_409C80(0xD8E80, field_F4_resources.field_0_resources[MenuResIds::eParamiteSpeak]);
    Set_Anim_4D05E0(AnimIds::eParamite_Idle);
}

static int DrawMenuStringWithShadow(int** ot, Alive::Font& field_120_font, const char* text, __int16 x, __int16 y, BYTE r, BYTE g, BYTE b, int& polyOffset)
{
    int polyOff = polyOffset;
    polyOff = field_120_font.DrawString_4337D0(
        ot,
        text,
        x,
        y,
        0,
        1,
        0,
        32,
        r,
        g,
        b,
        polyOff,
        FP_FromInteger(1),
        640,
        0
    );

    polyOffset = field_120_font.DrawString_4337D0(
        ot,
        text,
        x+2,
        y+2,
        0,
        1,
        0,
        32,
        0,
        0,
        0,
        polyOff,
        FP_FromInteger(1),
        640,
        0
    );
    return polyOffset;
}

ALIVE_VAR(1, 0xBB43F0, FP, sTextYPos_BB43F0, {});
ALIVE_VAR(1, 0xBB43E4, FP, dword_BB43E4, {});

static void RenderScrollableTextEntries(
    int** ot, int& targetEntry, int& selectedEntry, int totalItemsCount,
    FP& TextYPos, FP& TextYPos2, const char* field_234_pStr, const SaveFileRec* stringList, Alive::Font& field_120_font, int& polyOffset
)
{
    int i_start = 0;
    int i_end = 0;
    int entryPicker = selectedEntry;

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

    for (int i = i_start; i <= i_end; i++)
    {
        int v9 = entryPicker + i;
        if (v9 >= 0 && v9 < totalItemsCount)
        {
            field_234_pStr = stringList[v9].field_0_fileName;
            int currEntryWidth = field_120_font.MeasureWidth_4336C0(field_234_pStr, FP_FromInteger(1));

            //Entry X alignment (of questionable quality) for long words
            __int16 x = currEntryWidth >= 336 ? 16 : (368 - static_cast<__int16>(currEntryWidth)) / 2;
            //TextYPos is entry height?
            __int16 y = FP_GetExponent(TextYPos + FP_FromDouble(0.5)) + static_cast<__int16>(26 * i) + 112;
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

const MainMenuText sControllerMenuButtonCaptions_Text_562628[3] =
{
    { 34, 27, "x", 3u, 0u, 0u, 0u, 0.75, 0u, 0u, 0u, 0u },
    { 331, 204, "esc", 3u, 0u, 0u, 0u, 0.75, 0u, 0u, 0u, 0u },
    { 32, 203, "c", 3u, 0u, 0u, 0u, 0.75, 0u, 0u, 0u, 0u }
};

const SaveFileRec aKeyboard_1[2] = //used SaveFileRec as workaround for RenderScrollableTextEntries()
{
    { "Keyboard", 0 },
    { "Game Pad", 0 }
};

ALIVE_VAR(1, 0xBB43D8, int, controllerEntryToSelect_BB43D8, 0);
ALIVE_VAR(1, 0xBB43F4, int, selectedControllerEntry_BB43F4, 0);
ALIVE_VAR(1, 0x55E838, int, controllerCount_55E838, ALIVE_COUNTOF(aKeyboard_1));

void MainMenuController::ControllerMenu_Render_Text_4D26C0(int ** ot)
{
    int polyOffset = 0;
    RenderScrollableTextEntries(ot, controllerEntryToSelect_BB43D8, selectedControllerEntry_BB43F4, controllerCount_55E838, sTextYPos_BB43F0, dword_BB43E4, field_234_pStr, aKeyboard_1, field_120_font, polyOffset);

    for (int i = 0; i < ALIVE_COUNTOF(sControllerMenuButtonCaptions_Text_562628); i++)
    {
        MainMenuController::DrawMenuText_4D20D0(&sControllerMenuButtonCaptions_Text_562628[i], ot, &field_120_font, &polyOffset, 1);
    }
}

ALIVE_VAR(1, 0xBB4418, short, word_BB4418, 0);

void MainMenuController::Demo_And_FMV_List_Render_4D4F30(int** pOt)
{
    if (word_BB4418)
    {
        word_BB4418 = sDoesCreditsControllerExist_5C1B90;
        return;
    }

    word_BB4418 = sDoesCreditsControllerExist_5C1B90;

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

    const MainMenuText stru_5625F8[2] = 
    {
        { 32, 27, "x", 3u, 0u, 0u, 0u,  0.75f, 0u, 0u, 0u, 0u },
        { 331, 204, "esc", 3u, 0u, 0u, 0u,  0.75f, 0u, 0u, 0u, 0u }
    };

    int polyIndex = 0;
    for (const MainMenuText& text : stru_5625F8)
    {
        DrawMenuText_4D20D0(&text, pOt, &field_120_font, &polyIndex, 1);
    }

    int loopCount = -2;
    do
    {
        int idx = field_230_target_entry_index + loopCount;
        if (idx >= 0 && idx <= sMenuItemCount_561538 - 1)
        {
            field_234_pStr = pDemosOrFmvs_BB4414.mDemoRec[idx].field_0_display_name;
            int textWidth = field_120_font.MeasureWidth_4336C0(field_234_pStr, FP_FromInteger(1));
            short nextTextXPos = 0;
            if (textWidth >= 336)
            {
                nextTextXPos = 16;
            }
            else
            {
                nextTextXPos = static_cast<short>((368 - textWidth) / 2);
            }

            const short textYPos = static_cast<short>((FP_GetExponent(field_254 + FP_FromDouble(0.5))) + 26 * loopCount + 117);
            if (loopCount)
            {
                polyIndex = field_120_font.DrawString_4337D0(pOt, field_234_pStr, nextTextXPos, textYPos, 0, 1, 0, 32, 210, 150, 80, polyIndex, FP_FromInteger(1), 640, 0);
            }
            else
            {
                polyIndex = field_120_font.DrawString_4337D0(pOt, field_234_pStr, nextTextXPos, textYPos, 0, 1, 0, 32, 255, 218, 140, polyIndex, FP_FromInteger(1), 640, 0);
            }
            polyIndex = field_120_font.DrawString_4337D0(pOt, field_234_pStr, nextTextXPos + 2, textYPos + 2, 0, 1, 0, 32, 0, 0, 0, polyIndex, FP_FromInteger(1), 640, 0);
        }
        loopCount++;
    } while (loopCount < 2);
}

void MainMenuController::t_Unload_AbeSpeak_Res_4D49F0()
{
    ResourceManager::FreeResource_49C330(field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak]);
    field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak] = nullptr;
}


void MainMenuController::t_Load_AbeSpeak_Res_4D4A20()
{
    ResourceManager::Reclaim_Memory_49C470(0);

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbespeakResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABESPEAK.BAN", 0);
    }

    field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak] = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbespeakResID, TRUE, FALSE);

    field_25C_Inside_FMV_Screen = 0;
    field_25E_Inside_CheatLevelSelect_Screen = 0;
}

ALIVE_VAR(1, 0x55C128, int, dword_55C128, 0);

MainMenuNextCam MainMenuController::Page_FMV_Level_Update_4D4AB0(DWORD input_held)
{
    sEnableCheatFMV_5C1BEC = 0;
    sEnableCheatLevelSelect_5C1BEE = 0;

    if (sMovie_ref_count_BB4AE4 > 0)
    {
        // Do nothing if a movie is playing
        return { MainMenuCams::eNoChange };
    }

    int inputToUse = 0;
    if (field_204_prev_pressed && field_204_prev_pressed == sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)
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
        field_204_prev_pressed = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
        inputToUse = input_held;
    }

    if (inputToUse & (InputCommands::eLeft | InputCommands::eUp))
    {
        if (field_230_target_entry_index > 0 && field_254 == FP_FromInteger(0))
        {
            field_230_target_entry_index--;
            SFX_Play_46FBA0(SoundEffect::MenuNavigation_52, 35, 400);
        }
    }
    else if (inputToUse & (InputCommands::eRight | InputCommands::eDown))
    {
        if (field_230_target_entry_index < sMenuItemCount_561538 - 1 && field_254 == FP_FromInteger(0))
        {
            field_230_target_entry_index++;
            SFX_Play_46FBA0(SoundEffect::MenuNavigation_52, 35, 400);
        }
    }

    if (inputToUse & InputCommands::ePageUp)
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
            SFX_Play_46FBA0(SoundEffect::MenuNavigation_52, 35, 400);
        }
    }
    else if (inputToUse & InputCommands::ePageDown)
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
            SFX_Play_46FBA0(SoundEffect::MenuNavigation_52, 35, 400);
        }
    }

    if (inputToUse & InputCommands::eBack)
    {
        field_23C_T80.Clear(Flags::eBit25_CheatLevelSelectLoading);
        return { MainMenuCams::eMainMenuCam };
    }

    if (!(inputToUse & InputCommands::eUnPause_OrConfirm))
    {
        return { MainMenuCams::eNoChange };
    }

    if (field_25C_Inside_FMV_Screen)
    {
        MenuFMV* v12 = &pDemosOrFmvs_BB4414.mFmvRec[field_230_target_entry_index];
        if (v12->field_A_fmv_id >= 0)
        {
            FmvInfo* pFmvRecord = Path_Get_FMV_Record_460F70(v12->field_4_level_id, v12->field_A_fmv_id);
            Get_fmvs_sectors_494460(pFmvRecord->field_0_pName, 0, 0, &input_held, 0, 0);
            sLevelId_dword_5CA408 = static_cast<DWORD>(v12->field_4_level_id);
            
            dword_55C128 = -1;

            auto pMovie = ae_new<Movie>();
            if (pMovie)
            {
                pMovie->ctor_4DFDE0(pFmvRecord->field_4_id, input_held, pFmvRecord->field_6_flags & 1, pFmvRecord->field_8_flags, pFmvRecord->field_A_volume);
            }

            while (sMovie_ref_count_BB4AE4)
            {
                if (pMovie->field_6_flags.Get(BaseGameObject::eUpdatable_Bit2))
                {
                    if (!pMovie->field_6_flags.Get(BaseGameObject::eDead_Bit3) &&
                       (!sNum_CamSwappers_5C1B66 || pMovie->field_6_flags.Get(BaseGameObject::eUpdateDuringCamSwap_Bit10)))
                    {
                        pMovie->VUpdate();
                    }
                }
            }
            stru_5C3110.Free_433130();
            gPsxDisplay_5C1130.PutCurrentDispEnv_41DFA0();
            pScreenManager_5BB5F4->DecompressCameraToVRam_40EF60(reinterpret_cast<WORD**>(gMap_5C3030.field_2C_camera_array[0]->field_C_pCamRes));
            pScreenManager_5BB5F4->MoveImage_40EB70();
            pScreenManager_5BB5F4->field_40_flags |= 0x10000; // Render enable flag
            GetSoundAPI().SND_Restart();
        }
        else
        {
            sDoesCreditsControllerExist_5C1B90 = 1;
            field_240_credits_current_cam = 1;
            field_1F4_credits_next_frame = sGnFrame_5C1B84 + 160;
            gMap_5C3030.SetActiveCam_480D30(LevelIds::eCredits_16, 1, 1, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
        }
        return { MainMenuCams::eNoChange };
    }

    field_23C_T80.Set(Flags::eBit25_CheatLevelSelectLoading);

    field_244_lvl_id = pDemosOrFmvs_BB4414.mDemoRec[field_230_target_entry_index].field_4_level;
    field_246_path_id = pDemosOrFmvs_BB4414.mDemoRec[field_230_target_entry_index].field_6_path;
    field_248_camera = pDemosOrFmvs_BB4414.mDemoRec[field_230_target_entry_index].field_8_camera;
    field_24A_abeXOff = pDemosOrFmvs_BB4414.mDemoRec[field_230_target_entry_index].field_C_abe_x_off;
    field_24C_abeYOff = pDemosOrFmvs_BB4414.mDemoRec[field_230_target_entry_index].field_E_abe_y_off;
    field_24E_start_scale = pDemosOrFmvs_BB4414.mDemoRec[field_230_target_entry_index].field_A_id; // some how id and scale ??

    return { MainMenuCams::eGameIsLoading_ShaddapCam, -1 };
}

MainMenuText sLoadButtonGraphics[2] =
{
    { 32, 27, "x", 3u, 0u, 0u, 0u, 0.75, 0u, 0u, 0u, 0u },
    { 331, 204, "esc", 3u, 0u, 0u, 0u, 0.75, 0u, 0u, 0u, 0u }
};

ALIVE_VAR(1, 0xBB43E8, int, sSelectedSavedGameIdx_BB43E8, 0);

void MainMenuController::tLoadGame_Render_4D44D0(int** ot)
{
    int polyOffset = 0;
    RenderScrollableTextEntries(
        ot, sSavedGameToLoadIdx_BB43FC, sSelectedSavedGameIdx_BB43E8, sTotalSaveFilesCount_BB43E0,
        sTextYPos_BB43F0, dword_BB43E4, field_234_pStr, sSaveFileRecords_BB31D8, field_120_font, polyOffset
    );

    if (sTotalSaveFilesCount_BB43E0 <= 0) // max save files count
    {
        DrawMenuStringWithShadow(ot, field_120_font, "No Saved Games", 120, 110, 255, 218, 140, polyOffset);
    }
    for (auto& menuGraphic : sLoadButtonGraphics)
    {
        DrawMenuText_4D20D0(&menuGraphic, ot, &field_120_font, &polyOffset, 1);
    }
}

void MainMenuController::AbeMotions_Render_4D25E0(int ** ot)
{
    RenderOnScreenTextHelper(ot, &sAbeMotions_562448[0], ALIVE_COUNTOF(sAbeMotions_562448), 0);
}

MainMenuNextCam MainMenuController::Gamespeak_Update_4D1FC0(DWORD input_held)
{
    field_230_target_entry_index = 0;

    if (input_held & InputCommands::eBack)
    {
        return { MainMenuCams::eMainMenuCam };
    }

    if (!(input_held & InputCommands::eUnPause_OrConfirm))
    {
        return { MainMenuCams::eNoChange };
    }

    switch (field_1FC_button_index)
    {
    case 0:
        return { MainMenuCams::eAbeGamespeakCam, -1 };
        break;
    case 1:
        return { MainMenuCams::eSligGamespeakCam, -1 };
        break;
    case 2:
        return { MainMenuCams::eGlukkonGamespeakCam, -1 };
        break;
    case 3:
        return { MainMenuCams::eScrabGamespeakCam, -1 };
        break;
    case 4:
        return { MainMenuCams::eParamiteGamespeakCam, -1 };
        break;
    default:
        return { MainMenuCams::eNoChange };
    }
}

void MainMenuController::RenderOnScreenTextHelper(int ** ot, const MainMenuText * menuTextArray, int count, char isSingleChar)
{
    int polyIndex = 0;

    for (int i = 0; i < count; i++)
    {
        MainMenuController::DrawMenuText_4D20D0(&menuTextArray[i], ot, &field_120_font, &polyIndex, isSingleChar);
    }
}

MainMenuText sMTxt_Gamespeak_5623B8[2] = {
    { 330, 204, "esc", 3u, 0u, 0u, 0u,  0.75f, 0u, 0u, 0u, 0u },
    { 35, 205, "x", 3u, 0u, 0u, 0u,  0.75f, 0u, 0u, 0u, 0u },
};

MainMenuText sMTxt_Gamespeak_5623E8[2] = {
    { 35, 205, "x", 3u, 0u, 0u, 0u,  0.75f, 0u, 0u, 0u, 0u },
    { 331, 205, "esc", 3u, 0u, 0u, 0u,  0.75f, 0u, 0u, 0u, 0u },
};

MainMenuText sMTxt_NewGame_562598[3] = {
    { 165, 211, "esc", 3u, 0u, 0u, 0u,  0.75, 0u, 0u, 0u, 0u },
    { 161, 164, "x", 3u, 0u, 0u, 0u,  0.75, 0u, 0u, 0u, 0u },
};

void MainMenuController::Gamespeak_Render_4D24F0(int ** ot)
{
    RenderOnScreenTextHelper(ot, sMTxt_Gamespeak_5623B8, 2, 1);
}

void MainMenuController::Options_Render_4D2540(int ** ot)
{
    RenderOnScreenTextHelper(ot, sMTxt_Gamespeak_5623E8, 2, 1);
}

void MainMenuController::BackStory_Or_NewGame_Render_4D2630(int ** ot)
{
    RenderOnScreenTextHelper(ot, sMTxt_NewGame_562598, 2, 1);
}

MainMenuNextCam MainMenuController::Page_Front_Update_4D0720(DWORD input)
{
#if DEVELOPER_MODE
    static bool first = true;
    if (first && gBootToLoadScreen)
    {
        first = false;
        // Force enter pressed
        input |= InputCommands::eUnPause_OrConfirm;
        // Force load game selected
        field_1FC_button_index = 2;
    }

#endif
    // Reset time out if any input detected
    if (sInputObject_5BD4E0.field_0_pads[0].field_0_pressed)
    {
        field_1F8_page_timeout = 0;
        bLongerTimeoutToNextDemo_5C1B9A = 0;
    }

    // Go to loading a demo screen if no input after time out, after one demo plays the next time out is lower if input isn't pressed
    if (field_1F8_page_timeout > (bLongerTimeoutToNextDemo_5C1B9A ? 1500 : 300))
    {
        bLongerTimeoutToNextDemo_5C1B9A = 1;
        field_1FC_button_index = 0;
        return { MainMenuCams::eDemoIsLoading_ShaddapCam, -1 };
    }

    // Enter pressed on selected menu item?
    if (input & InputCommands::eUnPause_OrConfirm)
    {
        if (field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak])
        {
            Set_Anim_4D05E0(AnimIds::eAbe_OK);
        }

        switch (field_1FC_button_index)
        {
        case 0:
            // Begin
            return { MainMenuCams::eBackstory_Or_NewGameCam };
        case 1:
            // Quit
            Set_Anim_4D05E0(AnimIds::eAbe_GoodBye);
            return { MainMenuCams::eDummyBlankCam };
        case 2:
            // Load
            field_230_target_entry_index = 0;
            return { MainMenuCams::eLoadGameMenuCam, -1 };
        case 3:
            // Options
            return { MainMenuCams::eOptionsCam };
        case 4:
            // Game speak
            field_230_target_entry_index = 0;
            return { MainMenuCams::eGamespeakCharacterSelectionCam };
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
        return { MainMenuCams::eCheatMenu_SelectFMVCam, -1 };
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
        return { MainMenuCams::eCheatMenu_SelectLevelCam, -1 };
    }

    return { MainMenuCams::eNoChange };
}

void MainMenuController::Page_Front_Render_4D24B0(int** ot)
{
    RenderOnScreenTextHelper(ot, &sMMT_FrontPage_5623A0[0], ALIVE_COUNTOF(sMMT_FrontPage_5623A0), 1);
}

ALIVE_VAR(1, 0xbb43dc, short, word_BB43DC, 0);
ALIVE_VAR(1, 0x5c1b88, int, sGameStartedFrame_5C1B88, 0);

MainMenuNextCam MainMenuController::LoadNewGame_Update_4D0920(DWORD /*input*/)
{
    // TODO: De-dupe the big parts of duplicated code in here
    if (field_23C_T80.Get(Flags::eBit21_LoadingSave))
    {
        if (field_23C_T80.Get(Flags::eBit18_Loading))
        {
            // Wait for load to complete
            if (!field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak])
            {
                pResourceManager_5C1BB0->LoadingLoop_465590(FALSE);
            }

            field_20_animation.Set_Animation_Data_409C80(0x3C800, field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak2]);

            ResourceManager::FreeResource_49C330(field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak]);
            field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak] = nullptr;
            
            ResourceManager::Reclaim_Memory_49C470(0);
            
            if (!pPauseMenu_5C9300)
            {
                pPauseMenu_5C9300 = ae_new<PauseMenu>();
                if (pPauseMenu_5C9300)
                {
                    pPauseMenu_5C9300->ctor_48FB80();
                }
            }

            if (sActiveHero_5C1B68 == spAbe_554D5C)
            {
                sActiveHero_5C1B68 = ae_new<Abe>();
                if (sActiveHero_5C1B68)
                {
                    sActiveHero_5C1B68->ctor_44AD10(58808, 85, 57, 55);
                }
            }

            if (field_208_transition_obj)
            {
                field_208_transition_obj->field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }

            if (field_20C_pUnused)
            {
                field_20C_pUnused->field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }

            if (field_210_pUnused)
            {
                field_210_pUnused->field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }

            field_6_flags.Set(BaseGameObject::eDead_Bit3);
            
            sActiveHero_5C1B68->field_B8_xpos = FP_FromInteger(0);
            sActiveHero_5C1B68->field_BC_ypos = FP_FromInteger(0);
            
            Quicksave_LoadActive_4C9170();

            return { MainMenuCams::eNoChange };
        }

        field_23C_T80.Set(Flags::eBit18_Loading);
        return { MainMenuCams::eNoChange };
    }

    if (!field_23C_T80.Get(Flags::eBit18_Loading))
    {
        field_23C_T80.Set(Flags::eBit18_Loading);
        return { MainMenuCams::eNoChange };
    }

    if (!field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak])
    {
        pResourceManager_5C1BB0->LoadingLoop_465590(FALSE);
    }

    field_20_animation.Set_Animation_Data_409C80(247808, field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak2]);

    ResourceManager::FreeResource_49C330(field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak]);
    field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak] = nullptr;
    ResourceManager::Reclaim_Memory_49C470(0);

    if (!pPauseMenu_5C9300)
    {
        pPauseMenu_5C9300 = ae_new<PauseMenu>();
        if (pPauseMenu_5C9300)
        {
            pPauseMenu_5C9300->ctor_48FB80();
        }
    }

    if (sActiveHero_5C1B68 == spAbe_554D5C)
    {
        sActiveHero_5C1B68 = ae_new<Abe>();
        if (sActiveHero_5C1B68)
        {
            sActiveHero_5C1B68->ctor_44AD10(58808, 85, 57, 55);
        }
    }

    if (field_23C_T80.Get(Flags::eBit25_CheatLevelSelectLoading))
    {
        field_23C_T80.Clear(Flags::eBit25_CheatLevelSelectLoading);

        sActiveHero_5C1B68->field_1C_update_delay = 1;
        gMap_5C3030.SetActiveCam_480D30(field_244_lvl_id, field_246_path_id, field_248_camera, CameraSwapEffects::eEffect0_InstantChange, 0, 0);

        const PathBlyRec* pPathData = Path_Get_Bly_Record_460F30(field_244_lvl_id, field_246_path_id);
        sActiveHero_5C1B68->field_B8_xpos = FP_FromInteger(field_24A_abeXOff - pPathData->field_4_pPathData->field_1A_abe_start_xpos);
        sActiveHero_5C1B68->field_BC_ypos = FP_FromInteger(field_24C_abeYOff - pPathData->field_4_pPathData->field_1C_abe_start_ypos);

        if (field_24E_start_scale == -1)
        {
            sActiveHero_5C1B68->field_CC_sprite_scale = FP_FromDouble(1.0);
            sActiveHero_5C1B68->field_D6_scale = 1;
            sActiveHero_5C1B68->field_20_animation.field_C_render_layer = 32;
        }
        else if (field_24E_start_scale == -2)
        {
            sActiveHero_5C1B68->field_CC_sprite_scale = FP_FromDouble(0.5);
            sActiveHero_5C1B68->field_D6_scale = 0;
            sActiveHero_5C1B68->field_20_animation.field_C_render_layer = 13;
        }

    }
    else
    {
        gMap_5C3030.SetActiveCam_480D30(LevelIds::eMines_1, 1, 4, CameraSwapEffects::eEffect5_1_FMV, 12402, 0);
        sActiveHero_5C1B68->field_B8_xpos = FP_FromInteger(837);
        sActiveHero_5C1B68->field_BC_ypos = FP_FromInteger(1300);
        sActiveHero_5C1B68->field_F8_LastLineYPos = FP_FromInteger(1400);
    }

    if (field_208_transition_obj)
    {
        field_208_transition_obj->field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (field_20C_pUnused)
    {
        field_20C_pUnused->field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (field_210_pUnused)
    {
        field_210_pUnused->field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    field_6_flags.Set(BaseGameObject::eDead_Bit3);

    return { MainMenuCams::eNoChange };
}

EXPORT MainMenuNextCam MainMenuController::BackStory_Or_NewGame_Update_4D1C60(DWORD input_held)
{
    if (input_held & InputCommands::eUnPause_OrConfirm)
    {
        if (field_1FC_button_index == 0) // Show backstory
        {
            FmvInfo* pFmvRecord = Path_Get_FMV_Record_460F70(LevelIds::eMenu_0, 4u);
            while (Input_IsVKPressed_4EDD40(VK_RETURN))
            {
                SYS_EventsPump_494580();
            }

            DWORD fmvSector = 0;
            Get_fmvs_sectors_494460(pFmvRecord->field_0_pName, nullptr, nullptr, &fmvSector, 0, 0);
            sLevelId_dword_5CA408 = 0;

            auto pMovie = ae_new<Movie>();
            if (pMovie)
            {
                pMovie->ctor_4DFDE0(
                    pFmvRecord->field_4_id,
                    input_held,
                    pFmvRecord->field_6_flags & 1,
                    pFmvRecord->field_8_flags,
                    pFmvRecord->field_A_volume);
            }

            while (sMovie_ref_count_BB4AE4)
            {
                if (pMovie->field_6_flags.Get(BaseGameObject::eUpdatable_Bit2))
                {
                    if (!(pMovie->field_6_flags.Get(BaseGameObject::eDead_Bit3) && (!sNum_CamSwappers_5C1B66 || (pMovie->field_6_flags.Get(BaseGameObject::eUpdateDuringCamSwap_Bit10)))))
                    {
                        pMovie->VUpdate();
                    }
                }
            }

            gPsxDisplay_5C1130.PutCurrentDispEnv_41DFA0();
            pScreenManager_5BB5F4->DecompressCameraToVRam_40EF60((WORD**)gMap_5C3030.field_2C_camera_array[0]->field_C_pCamRes);
            pScreenManager_5BB5F4->MoveImage_40EB70();
            pScreenManager_5BB5F4->field_40_flags |= 0x10000; // Render enable flag
            GetSoundAPI().SND_Restart();
            field_1FC_button_index = 1; // Select start game
            return { MainMenuCams::eNoChange };
        }
        else if (field_1FC_button_index == 1) // Start game
        {
            sCurrentControllerIndex_5C1BBE = 0;
            sGameStartedFrame_5C1B88 = sGnFrame_5C1B84;
            return { MainMenuCams::eGameIsLoading_ShaddapCam, -1 };
        }
    }
    else if (input_held & InputCommands::eBack) // Escape/back
    {
        ResourceManager::Reclaim_Memory_49C470(0);
        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbespeakResID, FALSE, FALSE))
        {
            // TODO: Fix the types
            ResourceManager::LoadResourceFile_49C130("ABESPEAK.BAN",
                reinterpret_cast<ResourceManager::TLoaderFn>(callback_4D06E0),
                reinterpret_cast<Camera *>(this), nullptr);
        }
        word_BB43DC = 1;
        field_23C_T80.Clear(Flags::eBit25_CheatLevelSelectLoading);
        return { MainMenuCams::eMainMenuCam };
    }

    return { MainMenuCams::eNoChange };
}

void MainMenuController::BackStory_Or_NewGame_Load_4D1BA0()
{
    ResourceManager::FreeResource_49C330(field_F4_resources.field_0_resources[MenuResIds::eAbeIntro]);
    field_F4_resources.field_0_resources[MenuResIds::eAbeIntro] = nullptr;
    word_BB43DC = 0;
}

void MainMenuController::BackStory_Or_NewGame_Unload_4D1BE0()
{
    if (!word_BB43DC)
    {
        ResourceManager::Reclaim_Memory_49C470(0);
        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbespeakResID, FALSE, FALSE))
        {
            // TODO: Fix the types
            ResourceManager::LoadResourceFile_49C130("ABESPEAK.BAN",
                reinterpret_cast<ResourceManager::TLoaderFn>(callback_4D06E0),
                reinterpret_cast<Camera *>(this), nullptr);
        }
    }
    pResourceManager_5C1BB0->LoadingLoop_465590(FALSE);
}

void MainMenuController::remove_ISO9660_Suffix_4D1660(char* out, char *in)
{
    while (*in)
    {
        if (*in == ';')
        {
            *out = '\0';
            break;
        }

        *out = (char) tolower(*in);
        in++;
        out++;
    }
}

char MainMenuController::checkIfDemoFileExists_4D1430(char* input)
{
    char* inputMod = input;
    if (inputMod[0] == '\\')
    {
        inputMod++;
    }
    char buffer[256] = {};
    MainMenuController::remove_ISO9660_Suffix_4D1660(buffer, inputMod);

    char fileName[256] = {};
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

    while (access_impl(fileName, 0) != 0)
    {
        fileName[0] = (sCdRomDrives_5CA488++)[1];
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
ALIVE_VAR(1, 0x5C1B9C, short, gIsDemoStartedManually_5C1B9C, FALSE);

ALIVE_VAR(1, 0x5C1BA2, BYTE, sCurrentDemoIdForIdlingDemoPlayback_5C1BA2, 0);
MainMenuNextCam MainMenuController::LoadDemo_Update_4D1040(DWORD)
{
    const int maxDemoId = ALIVE_COUNTOF(sDemos_5617F0);

    if (field_23C_T80.Get(Flags::eBit18_Loading))
    {
        __int16 demoId = sDemoIdChosenFromDemoMenu_5C1B9E;
        if (!gIsDemoStartedManually_5C1B9C)
        {
            demoId = sCurrentDemoIdForIdlingDemoPlayback_5C1BA2;
        }
        if (demoId >= maxDemoId)
        {
            demoId = 0;
        }
        int levelId = static_cast<int>(sDemos_5617F0[demoId].field_4_level);
        char lvFilename[256] = {};
        strcpy(lvFilename, "ATTRACT");
        memset(&lvFilename[8], 0, 0xF8u);
        strcpy(&lvFilename[7], sPathData_559660.paths[levelId].field_20_lvl_name_cd);
        auto lvFilenameNoPrefix = &lvFilename[7];

        while (!MainMenuController::checkIfDemoFileExists_4D1430(lvFilenameNoPrefix) && !MainMenuController::checkIfDemoFileExists_4D1430(lvFilename))
        {
            sLevelId_dword_5CA408 = levelId;
            if (gIsDemoStartedManually_5C1B9C)
            {
                dword_55C128 = -1;
            }
            if (!Display_Full_Screen_Message_Blocking_465820(sPathData_559660.paths[levelId].field_1A_unused, MessageType::eSkipDemo_2))
            {
                field_1F8_page_timeout = 0;
                if (gIsDemoStartedManually_5C1B9C)
                {
                    return { MainMenuCams::eDemoSelectionCam };
                }
                else
                {
                    return { MainMenuCams::eMainMenuCam };
                }
            }
        }

        if (field_F4_resources.field_0_resources[0] == 0)
        {
            pResourceManager_5C1BB0->LoadingLoop_465590(0);
        }
        field_20_animation.Set_Animation_Data_409C80(247808, field_F4_resources.field_0_resources[1]);
        ResourceManager::FreeResource_49C330(field_F4_resources.field_0_resources[0]);
        field_F4_resources.field_0_resources[0] = nullptr;
        ResourceManager::Reclaim_Memory_49C470(0);

        if (sActiveHero_5C1B68 == spAbe_554D5C)
        {
            auto abe = ae_new<Abe>();
            if (abe)
            {
                sActiveHero_5C1B68 = abe->ctor_44AD10(58808, 85, 57, 55);
                sActiveHero_5C1B68->field_B8_xpos = FP_FromInteger(0);
                sActiveHero_5C1B68->field_BC_ypos = FP_FromInteger(0);
            }
            else
            {
                sActiveHero_5C1B68 = nullptr;
            }
        }

        gAttract_5C1BA0 = 1;

        if (field_208_transition_obj)
        {
            field_208_transition_obj->field_6_flags.Set(Options::eDead_Bit3);
        }
        if (field_20C_pUnused)
        {
            field_20C_pUnused->field_6_flags.Set(Options::eDead_Bit3);
        }
        if (field_210_pUnused)
        {
            field_210_pUnused->field_6_flags.Set(Options::eDead_Bit3);
        }
        field_6_flags.Set(Options::eDead_Bit3);

        demoId = sDemoIdChosenFromDemoMenu_5C1B9E;
        if (!gIsDemoStartedManually_5C1B9C)
        {
            sCurrentDemoIdForIdlingDemoPlayback_5C1BA2++;

            if (sCurrentDemoIdForIdlingDemoPlayback_5C1BA2 > maxDemoId)
            {
                // all the "idling" demos have been played, wrap around
                sCurrentDemoIdForIdlingDemoPlayback_5C1BA2 = 1;
            }
            demoId = sCurrentDemoIdForIdlingDemoPlayback_5C1BA2;
        }

        char file[32] = {};
        sprintf(file, "ATTR%04d.SAV", sDemos_5617F0[demoId].field_A_id);
        ResourceManager::LoadResourceFile_49C170(file, nullptr);
        BYTE **resource = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::ResourceType::Resource_NxtP, ResourceID::kUnknownResID_0, 1, 0);
        sActiveQuicksaveData_BAF7F8 = *(reinterpret_cast<Quicksave*>(*resource));
        ResourceManager::FreeResource_49C330(resource);

        if (gIsDemoStartedManually_5C1B9C)
        {
            sActiveQuicksaveData_BAF7F8.field_200_accumulated_obj_count = 1024;
        }
        Quicksave_LoadActive_4C9170();
    }
    else
    {
        field_23C_T80.Set(Flags::eBit18_Loading);
    }
    return { MainMenuCams::eNoChange };
}

MainMenuNextCam MainMenuController::DemoSelect_Update_4D0E10(DWORD input)
{
    gAttract_5C1BA0 = 0;
    gIsDemoStartedManually_5C1B9C = FALSE;

    int input_or_field_204 = input;
    if (field_204_prev_pressed && field_204_prev_pressed == sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)
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
        field_204_prev_pressed = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
    }

    if (input_or_field_204 & (InputCommands::eUp | InputCommands::eLeft))
    {
        // move to previous entry
        if (field_230_target_entry_index > 0 && field_254 == FP_FromInteger(0))
        {
            field_230_target_entry_index--;
            SFX_Play_46FBA0(SoundEffect::MenuNavigation_52, 35, 400);
        }
    }
    else if (input_or_field_204 & (InputCommands::eDown | InputCommands::eRight))
    {
        // move to next entry
        if (field_230_target_entry_index < sMenuItemCount_561538 - 1 && field_254 == FP_FromInteger(0))
        {
            field_230_target_entry_index++;
            SFX_Play_46FBA0(SoundEffect::MenuNavigation_52, 35, 400);
        }
    }

    if (input_or_field_204 & InputCommands::ePageUp)
    {
        // move one whole page back
        if (field_230_target_entry_index > 0 && field_254 == FP_FromInteger(0))
        {
            field_230_target_entry_index -= 3;
            if (field_230_target_entry_index < 0)
            {
                field_230_target_entry_index = 0;
            }
            SFX_Play_46FBA0(SoundEffect::MenuNavigation_52, 35, 400);
        }
    }
    else if (input_or_field_204 & InputCommands::ePageDown)
    {
        // move one whole page forward
        if (field_230_target_entry_index < sMenuItemCount_561538 - 1 && field_254 == FP_FromInteger(0))
        {
            field_230_target_entry_index += 3;
            if (field_230_target_entry_index > sMenuItemCount_561538 - 1)
            {
                field_230_target_entry_index = sMenuItemCount_561538 - 1;
            }
            SFX_Play_46FBA0(SoundEffect::MenuNavigation_52, 35, 400);
        }
    }

    if (input_or_field_204 & InputCommands::eBack)
    {
        return { MainMenuCams::eOptionsCam, 1 }; //Esc Pressed
    }

    if (input_or_field_204 & InputCommands::eUnPause_OrConfirm)
    {
        // selected a demo for playing
        gIsDemoStartedManually_5C1B9C = TRUE;
        sDemoIdChosenFromDemoMenu_5C1B9E = field_230_target_entry_index;
        return { MainMenuCams::eDemoIsLoading_ShaddapCam, -1 }; //Enter Pressed
    }

    return { MainMenuCams::eNoChange }; // Nothing Pressed
}

EXPORT MainMenuNextCam MainMenuController::tLoadGame_Input_4D3EF0(DWORD input)
{
    bool indexChanged = false;

    // Escape ?
    if (input & InputCommands::eBack)
    {
        // Go back to start page
        field_23C_T80.Clear(Flags::eBit21_LoadingSave);
        field_23A_Inside_LoadGame_Screen = 0;
        return { MainMenuCams::eMainMenuCam, 2 };
    }
    // Up a single save
    else if (input & InputCommands::eUp)
    {
        if (sSelectedSavedGameIdx_BB43E8 > 0 && !sTextYPos_BB43F0.fpValue)
        {
            sSelectedSavedGameIdx_BB43E8--;
            indexChanged = true;
        }
    }
    // Down a single save
    else if (input & InputCommands::eDown)
    {
        if (sSelectedSavedGameIdx_BB43E8 < sTotalSaveFilesCount_BB43E0 - 1 && !sTextYPos_BB43F0.fpValue)
        {
            sSelectedSavedGameIdx_BB43E8++;
            indexChanged = true;
        }
    }
    else if (input & InputCommands::ePageUp)
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
    else if (input & InputCommands::ePageDown)
    {
        // Page down overflow
        if (sSelectedSavedGameIdx_BB43E8 < sTotalSaveFilesCount_BB43E0 - 3 && !sTextYPos_BB43F0.fpValue)
        {
            sSelectedSavedGameIdx_BB43E8 += 3;
            indexChanged = true;
        }
        else
        {
            sSelectedSavedGameIdx_BB43E8 = sTotalSaveFilesCount_BB43E0 -1;
            indexChanged = true;
        }
    }

    if (indexChanged)
    {
        SFX_Play_46FBA0(SoundEffect::MenuNavigation_52, 35, 400);
    }

    if (input & InputCommands::eUnPause_OrConfirm)
    {
        // No save to load, go back
        if (sTotalSaveFilesCount_BB43E0 == 0)
        {
            // Go back to start page
            field_23C_T80.Clear(Flags::eBit21_LoadingSave);
            field_23A_Inside_LoadGame_Screen = 0;
            return { MainMenuCams::eMainMenuCam, 2 };
        }

        // Load selected save
        char filename[40] = {};
        strcpy(filename, sSaveFileRecords_BB31D8[sSavedGameToLoadIdx_BB43FC].field_0_fileName);
        strcat(filename, ".sav");

        std::string strPath = FS::GetPrefPath() + filename;
        IO_FileHandleType hFile = IO_Open(strPath.c_str(), "rb");

        if (!hFile)
        {
            return { MainMenuCams::eNoChange };
        }

        IO_Read(hFile, &sActiveQuicksaveData_BAF7F8, sizeof(Quicksave), 1u);
        IO_Close(hFile);

        field_23C_T80.Set(Flags::eBit21_LoadingSave);
        return { MainMenuCams::eGameIsLoading_ShaddapCam, -1 };
    }

    return { MainMenuCams::eNoChange };
}

void MainMenuController::tLoadGame_Load_4D42F0()
{
    field_23A_Inside_LoadGame_Screen = 6;
    field_230_target_entry_index = 0;
    field_1FC_button_index = -1;
    Quicksave_FindSaves_4D4150();
    sSelectedSavedGameIdx_BB43E8 = sSavedGameToLoadIdx_BB43FC;
    field_23C_T80.Clear(Flags::eBit15_unused);
    field_1F4_credits_next_frame = 0;
}

EXPORT void sub_4A2D40()
{
    NOT_IMPLEMENTED();
}


MainMenuNextCam MainMenuController::Options_Update_4D1AB0(DWORD input)
{
    if (!(input & InputCommands::eUnPause_OrConfirm))
    {
        if (input & InputCommands::eBack)
        {
            Set_Anim_4D05E0(AnimIds::eAbe_OK, 0);
            return { MainMenuCams::eMainMenuCam };
        }
        else
        {
            return { MainMenuCams::eNoChange };
        }
    }
    Set_Anim_4D05E0(AnimIds::eAbe_OK, 0);

    switch (field_1FC_button_index)
    {
        case 0:
        {
            return { MainMenuCams::eControllerSelectionCam };
        }
        case 1:
        {
            field_250_selected_entry_index = 0;
            field_254 = FP_FromInteger(0);
            field_258 = FP_FromInteger(0);
            pDemosOrFmvs_BB4414.mDemoRec = &sDemos_5617F0[0];
            sMenuItemCount_561538 = ALIVE_COUNTOF(sDemos_5617F0);
            field_20_animation.Set_Animation_Data_409C80(247808, field_F4_resources.field_0_resources[1]);
            Load_Anim_Pal_4D06A0(&field_20_animation);
            field_230_target_entry_index = 0;
            return { MainMenuCams::eDemoSelectionCam, -1 };
        }
        default:
        {
            if (input & InputCommands::eBack)
            {
                Set_Anim_4D05E0(AnimIds::eAbe_OK, 0);
                return { MainMenuCams::eMainMenuCam };
            }
            else
            {
                return { MainMenuCams::eNoChange };
            }
        }
    }
}

MainMenuNextCam MainMenuController::AbeMotions_Update_4D1F50(DWORD input)
{
    if (sNum_CamSwappers_5C1B66 > 0)
    {
        // Camera is changing - stay in this screen
        return { MainMenuCams::eNoChange };
    }

    // To the game speak screen
    if (input & InputCommands::eUnPause_OrConfirm)
    {
        SFX_Play_46FBA0(SoundEffect::MenuNavigation_52, 35, 400);
        return { MainMenuCams::eGamespeakCharacterSelectionCam };
    }

    // Return to previous screen
    if (input & InputCommands::eBack)
    {
        return { MainMenuCams::eOptionsCam };
    }
    else
    {
        // No input - stay in this screen
        return { MainMenuCams::eNoChange };
    }
}

MainMenuNextCam MainMenuController::PSX_Cooperative_Mode_Update_4D49B0(DWORD /*input*/)
{
    const DWORD held = sInputObject_5BD4E0.field_0_pads[0].field_C_held;
    if ( held & (InputCommands::eUnPause_OrConfirm | InputCommands::eBack))
    {
        return { MainMenuCams::eBackstory_Or_NewGameCam };
    }

    return { MainMenuCams::eNoChange };
}

MainMenuNextCam MainMenuController::PSX_Gamemode_Selection_Update_4D48C0(DWORD input)
{
    if (input & InputCommands::eUnPause_OrConfirm)
    {
        sGameStartedFrame_5C1B88 = sGnFrame_5C1B84;
        sCurrentControllerIndex_5C1BBE = 0;
        const BOOL twoPlayerModeSelected = field_1FC_button_index == 1;

        MainMenuController::Set_Anim_4D05E0(AnimIds::eAbe_FollowMe);
        if (field_23C_T80.Get(Flags::eBit25_CheatLevelSelectLoading))
        {
            return { MainMenuCams::eGameIsLoading_ShaddapCam };
        }

        // apparently cam 11 was originally for the cooperative mode
        // description screen on the PSX
        return twoPlayerModeSelected ?
            MainMenuNextCam{MainMenuCams::eDummyBlankCam} : MainMenuNextCam{MainMenuCams::eBackstory_Or_NewGameCam};
    }
    else if (input & InputCommands::eBack)
    {
        if (field_23C_T80.Get(Flags::eBit25_CheatLevelSelectLoading))
        {
            return { MainMenuCams::eCheatMenu_SelectLevelCam };
        }

        MainMenuController::Set_Anim_4D05E0(AnimIds::eAbe_OK);
        return { MainMenuCams::eMainMenuCam };
    }

    return { MainMenuCams::eNoChange };
}

ALIVE_VAR(1, 0xBB43F8, int, dword_BB43F8, 0);
ALIVE_VAR(1, 0xBB43EC, int, sButtonToRemapIdx_BB43EC, 0);

void MainMenuController::RemapInput_Load_4D17E0()
{
    field_158_animation.Set_Animation_Data_409C80(13936, nullptr);
    sButtonToRemapIdx_BB43EC = 0;
    dword_BB43F8 = 0;
}

void MainMenuController::ControllerMenu_Load_4D16B0()
{
    controllerEntryToSelect_BB43D8 = sJoystickEnabled_5C9F70;
    selectedControllerEntry_BB43F4 = sJoystickEnabled_5C9F70;
}

const char* sButtonNames_562790[8] =
{ 
    "Run",
    "Sneak",
    "Jump",
    "Speak 1",
    "Action",
    "Throw",
    "Fart/Roll",
    "Speak 2"
};


const MainMenuText stru_5626A0[10] =
{
    { 35, 34, "x", 3u, 0u, 0u, 0u,  0.75, 0u, 0u, 0u, 0u },
    { 331, 204, "esc", 3u, 0u, 0u, 0u,  0.75, 0u, 0u, 0u, 0u },
    { 152, 75, "\x5", 3u, 0u, 0u, 0u,  0.88f, 1u, 0u, 0u, 0u },
    { 152, 107, "\a", 3u, 0u, 0u, 0u,  0.88f, 1u, 0u, 0u, 0u },
    { 152, 140, "\t", 3u, 0u, 0u, 0u,  0.88f, 1u, 0u, 0u, 0u },
    { 152, 172, "\x18", 3u, 0u, 0u, 0u,  0.88f, 1u, 0u, 0u, 0u },
    { 250, 75, "\x6", 3u, 0u, 0u, 0u,  0.88f, 1u, 0u, 0u, 0u },
    { 250, 107, "\b", 3u, 0u, 0u, 0u,  0.88f, 1u, 0u, 0u, 0u },
    { 250, 140, "\n", 3u, 0u, 0u, 0u,  0.88f, 1u, 0u, 0u, 0u },
    { 250, 172, byte_55EDEC, 3u, 0u, 0u, 0u,  0.88f, 1u, 0u, 0u, 0u }
};

void MainMenuController::RemapInput_Render_4D2A10(int** ot)
{
    int polyIndex = 0;
    if (dword_BB43F8 == 3)
    {
        if (sJoystickEnabled_5C9F70)
        {
            field_234_pStr = "Press button to use";
        }
        else
        {
            field_234_pStr = "Press key to use";
        }

        int textWidth = field_120_font.MeasureWidth_4336C0(field_234_pStr, FP_FromInteger(1));
        int nextTextXPos = 0;
        if (textWidth >= 336)
        {
            nextTextXPos = 16;
        }
        else
        {
            nextTextXPos = (368 - textWidth) / 2;
        }
        polyIndex = field_120_font.DrawString_4337D0(ot, field_234_pStr, nextTextXPos, 88, 0, 1, 0, 41, 40, 20, 0, polyIndex, FP_FromInteger(1), 640, 0);
        char buffer[512] = {};
        sprintf(buffer, "for %s", sButtonNames_562790[sButtonToRemapIdx_BB43EC]);
        field_234_pStr = buffer;
        textWidth = field_120_font.MeasureWidth_4336C0(buffer, FP_FromInteger(1));
        if (textWidth >= 336)
        {
            nextTextXPos = 16;
        }
        else
        {
            nextTextXPos = (368 - textWidth) / 2;
        }
        polyIndex = field_120_font.DrawString_4337D0(ot, field_234_pStr, nextTextXPos, 120, 0, 1, 0, 41, 40, 20, 0, polyIndex, FP_FromInteger(1), 640, 0);
        field_234_pStr = "Press BackSpace for none, Esc to exit";
        textWidth = field_120_font.MeasureWidth_4336C0(field_234_pStr, FP_FromInteger(1));
        if (textWidth >= 336)
        {
            nextTextXPos = 16;
        }
        else
        {
            nextTextXPos = (368 - textWidth) / 2;
        }
        field_120_font.DrawString_4337D0(ot, field_234_pStr, nextTextXPos, 152, 0, 1, 0, 41, 40, 20, 0, polyIndex, FP_FromInteger(1), 640, 0);
    }
    else
    {
        if (!sJoystickEnabled_5C9F70)
        {
            // Speak 1
            const MainMenuText stru_562760 = { 152, 174, "-", 3u, 0u, 0u, 0u, 0.88f, 0u, 0u, 0u, 0u };

            // Speak 2
            const MainMenuText stru_562778 = { 250, 174, "-", 3u, 0u, 0u, 0u, 0.88f, 0u, 0u, 0u, 0u };

            DrawMenuText_4D20D0(&stru_562760, ot, &field_120_font, &polyIndex, 1);
            DrawMenuText_4D20D0(&stru_562778, ot, &field_120_font, &polyIndex, 1);
        }

        // Note: OG renders this list backwards
        for (const MainMenuText& item : stru_5626A0)
        {
            DrawMenuText_4D20D0(&item, ot, &field_120_font, &polyIndex, 1);
        }

        // Render "glowing" box around the selected buttons
        BYTE rgb = static_cast<BYTE>(3 * field_1FE_highlite_alpha);
        field_158_animation.field_8_r = rgb;
        field_158_animation.field_A_b = rgb;
        field_158_animation.field_9_g = rgb;
        field_158_animation.vRender_40B820(
            stru_5626A0[sButtonToRemapIdx_BB43EC + 2].field_0_x - 18,
            stru_5626A0[sButtonToRemapIdx_BB43EC + 2].field_4_y + 8,
            ot, 0, 0);
    }
}

MainMenuNextCam MainMenuController::ControllerMenu_Update_4D16D0(DWORD input)
{
    if (input & InputCommands::eUp)
    {
        if (selectedControllerEntry_BB43F4 > 0 && sTextYPos_BB43F0 == FP_FromInteger(0))
        {
            selectedControllerEntry_BB43F4--;
            SFX_Play_46FBA0(SoundEffect::MenuNavigation_52, 45, 400);
        }
    }
    else if (input & InputCommands::eDown)
    {
        if (selectedControllerEntry_BB43F4 < controllerCount_55E838 - 1 && sTextYPos_BB43F0 == FP_FromInteger(0))
        {
            selectedControllerEntry_BB43F4++;
            SFX_Play_46FBA0(SoundEffect::MenuNavigation_52, 45, 400);
        }
    }

    if (input & InputCommands::eBack)
    {
        return { MainMenuCams::eOptionsCam };
    }

    // Enter - set active input device
    if (input & InputCommands::eUnPause_OrConfirm)
    {
        sJoystickEnabled_5C9F70 = controllerEntryToSelect_BB43D8;
        Input_Init_Names_491870();
        Input_SaveSettingsIni_492840();
        return { MainMenuCams::eOptionsCam };
    }
    
    if (input & InputCommands::eConfigure)
    {
        // c configure controller
        sJoystickEnabled_5C9F70 = controllerEntryToSelect_BB43D8;
        Input_Init_Names_491870();
        return { MainMenuCams::eRemapInputsCam };
    }
    else
    {
        // Nothing pressed, stay on this screen
        return { MainMenuCams::eNoChange };
    }
}

const InputCommands kIdxToInput_561F14[8] =
{
    InputCommands::eRun,
    InputCommands::eSneak,
    InputCommands::eHop,
    InputCommands::eSpeak1,
    InputCommands::eDoAction,
    InputCommands::eThrowItem,
    InputCommands::eFartOrRoll,
    InputCommands::eSpeak2
};

MainMenuNextCam MainMenuController::RemapInput_Update_4D1820(DWORD input)
{
    if (dword_BB43F8)
    {
        if (dword_BB43F8 == 1 && sInputObject_5BD4E0.IsReleased(InputCommands::eUnPause_OrConfirm))
        {
            dword_BB43F8 = 2;
            return { MainMenuCams::eNoChange };
        }

        if (field_208_transition_obj->field_26_bDone)
        {
            dword_BB43F8 = 3;
            if (Input_Remap_492680(kIdxToInput_561F14[sButtonToRemapIdx_BB43EC]))
            {
                dword_BB43F8 = 0;
                field_208_transition_obj->StartTrans_464370(40, 0, 0, 16);
            }
        }
    }
    else
    {
        field_158_animation.Set_Animation_Data_409C80(13936, nullptr);

        if (input & InputCommands::eUp)
        {
            sButtonToRemapIdx_BB43EC--;

            if (sButtonToRemapIdx_BB43EC < 0)
            {
                sButtonToRemapIdx_BB43EC = 7;
            }

            if (!sJoystickEnabled_5C9F70 && (sButtonToRemapIdx_BB43EC == 7 || sButtonToRemapIdx_BB43EC == 3))
            {
                sButtonToRemapIdx_BB43EC--;
            }

            SFX_Play_46FBA0(SoundEffect::MenuNavigation_52, 45, 400);
        }

        if (input & InputCommands::eDown)
        {
            sButtonToRemapIdx_BB43EC++;

            if (!sJoystickEnabled_5C9F70 && (sButtonToRemapIdx_BB43EC == 7 || sButtonToRemapIdx_BB43EC == 3))
            {
                sButtonToRemapIdx_BB43EC++;
            }

            if (sButtonToRemapIdx_BB43EC > 7)
            {
                sButtonToRemapIdx_BB43EC = 0;
            }
            SFX_Play_46FBA0(SoundEffect::MenuNavigation_52, 45, 400);
        }

        if (input & InputCommands::eLeft)
        {
            if (sButtonToRemapIdx_BB43EC >= 4)
            {
                sButtonToRemapIdx_BB43EC -= 4;
            }
            SFX_Play_46FBA0(SoundEffect::MenuNavigation_52, 45, 400);
        }

        if (input & InputCommands::eRight)
        {
            if (sButtonToRemapIdx_BB43EC < 4)
            {
                sButtonToRemapIdx_BB43EC += 4;
            }
            SFX_Play_46FBA0(SoundEffect::MenuNavigation_52, 45, 400);
        }

        if (input & InputCommands::eBack)
        {
            Input_SaveSettingsIni_492840();
            field_1FC_button_index = -1;
            return { MainMenuCams::eAbesMotionKeysCam };
        }

        if (input & InputCommands::eUnPause_OrConfirm)
        {
            field_208_transition_obj->StartTrans_464370(40, 1, 0, 16);
            dword_BB43F8 = 1;
            return { MainMenuCams::eNoChange };
        }
    }
    return { MainMenuCams::eNoChange };
}

void MainMenuController::tLoadGame_Unload_4D4360()
{
    field_23C_T80.Clear(Flags::eBit15_unused);

    sub_4A2D40();

    if (field_23C_T80.Get(Flags::eBit21_LoadingSave))
    {
        for (auto& ppRes : field_F4_resources.field_0_resources)
        {
            if (field_20_animation.field_20_ppBlock != ppRes)
            {
                ResourceManager::FreeResource_49C330(ppRes);
                ppRes = nullptr;
            }
        }
    }
}

void MainMenuController::Game_Force_Quit_Load_4D1A90()
{
    sBreakGameLoop_5C2FE0 = TRUE;
}

MainMenuNextCam MainMenuController::HandleGameSpeakInput(DWORD input_held, std::function<MainMenuNextCam(InputCommands cmd)> fnOnGameSpeak)
{
    field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
    field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);

    if (field_230_target_entry_index == 8)
    {
        return { MainMenuCams::eNoChange };
    }

    if (Input_IsChanting_45F260())
    {
        field_230_target_entry_index = 1;
        return fnOnGameSpeak(InputCommands::eChant);
    }
    // Hi
    else if (input_held & InputCommands::eGameSpeak1) // 0x400
    {
        field_230_target_entry_index = 0;
        return fnOnGameSpeak(InputCommands::eGameSpeak1);
    }
    // Git 'im
    else if (input_held & InputCommands::eGameSpeak4) // 0x2000
    {
        field_230_target_entry_index = 1;
        return fnOnGameSpeak(InputCommands::eGameSpeak4);
    }
    // Freeze
    else if (input_held & InputCommands::eGameSpeak3) // 0x1000
    {
        field_230_target_entry_index = 2;
        return fnOnGameSpeak(InputCommands::eGameSpeak3);
    }
    // Here boy
    else if (input_held & InputCommands::eGameSpeak2) // 0x800
    {
        field_230_target_entry_index = 3;
        return fnOnGameSpeak(InputCommands::eGameSpeak2);
    }
    // Bs
    else if (input_held & InputCommands::eGameSpeak6) // 0x8000
    {
        field_230_target_entry_index = 4;
        return fnOnGameSpeak(InputCommands::eGameSpeak6);
    }
    // Look out
    else if (input_held & InputCommands::eGameSpeak7) // 0x10000
    {
        field_230_target_entry_index = 5;
        return fnOnGameSpeak(InputCommands::eGameSpeak7);
    }
    // S'mo bs
    else if (input_held & InputCommands::eGameSpeak5) // 0x4000
    {
        field_230_target_entry_index = 6;
        return fnOnGameSpeak(InputCommands::eGameSpeak5);
    }
    // Laugh
    else if (input_held & InputCommands::eGameSpeak8) // 0x20000
    {
        field_230_target_entry_index = 7;
        return fnOnGameSpeak(InputCommands::eGameSpeak8);
    }
    else if (input_held & InputCommands::eBack) // 0x200000
    {
        // Exit
      
        field_230_target_entry_index = 8;
        field_1FC_button_index = -1;

        if (field_210_pUnused)
        {
            // TODO: Recover type
            WORD* pUnknown = (WORD *)field_210_pUnused;
            pUnknown[124] = 1;
            field_210_pUnused = nullptr;
        }

        return fnOnGameSpeak(InputCommands::eBack);
    }
    else
    {
        if (field_210_pUnused)
        {
            // TODO: Recover type
            WORD* pUnknown = (WORD *)field_210_pUnused;
            pUnknown[124] = 1;
            field_210_pUnused = nullptr;
        }
        return { MainMenuCams::eNoChange };
    }
}

void MainMenuController::HandleCreditsControllerUpdate()
{
    if (sInputObject_5BD4E0.isPressed(InputCommands::eBack))
    {
        sDoesCreditsControllerExist_5C1B90 = 0;
        gMap_5C3030.SetActiveCam_480D30(LevelIds::eMenu_0, 1, 6, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
        return;
    }

    if (field_1F4_credits_next_frame <= static_cast<int>(sGnFrame_5C1B84))
    {
        const auto currentCam = field_240_credits_current_cam + 1;
        field_240_credits_current_cam = currentCam;
        field_1F4_credits_next_frame = sGnFrame_5C1B84 + 160;
        if (gMap_5C3030.field_2_current_path == 2)
        {
            if (currentCam > 22)
            {
                field_240_credits_current_cam = 1;
                gMap_5C3030.SetActiveCam_480D30(LevelIds::eCredits_16, 1, static_cast<short>(field_240_credits_current_cam), CameraSwapEffects::eEffect3_TopToBottom, 0, 0);
            }
            else
            {
                gMap_5C3030.SetActiveCam_480D30(LevelIds::eCredits_16, 2, static_cast<short>(currentCam), CameraSwapEffects::eEffect3_TopToBottom, 0, 0);
            }
        }
        else
        {
            if (currentCam > 36)
            {
                sDoesCreditsControllerExist_5C1B90 = 0;
                gMap_5C3030.SetActiveCam_480D30(LevelIds::eMenu_0, 1, 6, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
                return;
            }
            gMap_5C3030.SetActiveCam_480D30(LevelIds::eCredits_16, 1, static_cast<short>(field_240_credits_current_cam), CameraSwapEffects::eEffect3_TopToBottom, 0, 0);
        }
    }
}

// Todo: needs cleanup
void MainMenuController::HandleMainMenuUpdate()
{
    if (gMap_5C3030.field_4_current_camera == MainMenuCams::eRemapInputsCam ||
        gMap_5C3030.field_4_current_camera == MainMenuCams::eDummyBlankCam ||
        gMap_5C3030.field_4_current_camera == MainMenuCams::eLoadGameMenuCam)
    {
        MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eTension_4, this, 0, 0);
    }
    else if (gMap_5C3030.field_4_current_camera == MainMenuCams::eBackstory_Or_NewGameCam ||
        gMap_5C3030.field_4_current_camera == MainMenuCams::eGameIsLoading_ShaddapCam)
    {
        MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eChase_8, this, 0, 0);
    }
    else
    {
        MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eNone_0, this, 0, 0);
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
    if (sInputObject_5BD4E0.field_0_pads[0].field_0_pressed &&
        currentCamId != MainMenuCams::eGameBootCopyrightSplashCam &&
        currentCamId != MainMenuCams::eUnknown20Cam &&
        currentCamId != MainMenuCams::eControllerSelectionCam)
    {
        field_1F8_page_timeout = 0;
    }
    else
    {
        ++field_1F8_page_timeout;
    }

    auto isScreenTransitionNecessary = 0;

    if (pPage->field_4_time_out <= 0 ||
        pPage->field_8_next_idx <= 0 ||
        field_1F8_page_timeout <= pPage->field_4_time_out)
    {
        const MainMenuButton* btnArray = pPage->field_18_buttons;
        const DWORD inputHeld = sInputObject_5BD4E0.field_0_pads[0].field_C_held;

        if (btnArray)
        {
            if (field_1FC_button_index != -1)
            {
                if (inputHeld & (eLeft | eUp))
                {
                    if (pPage->field_0_cam_id != MainMenuCams::eLoadGameMenuCam)
                    {
                        for (;;)
                        {
                            field_1FC_button_index--;
                            if (field_1FC_button_index < 0)
                            {
                                short lastIndex = 0;
                                while (btnArray[lastIndex].field_0 == 1)
                                {
                                    lastIndex++;
                                }
                                field_1FC_button_index = lastIndex;
                            }
                            else if (btnArray[field_1FC_button_index].field_0 == 1)
                            {
                                break;
                            }
                        }
                    }

                    field_158_animation.Set_Animation_Data_409C80(pPage->field_18_buttons[field_1FC_button_index].field_8_anim_frame_offset, 0);

                    SFX_Play_46FBA0(SoundEffect::MenuNavigation_52, 35, 400);
                }

                if (inputHeld & (InputCommands::eRight | InputCommands::eDown))
                {
                    if (pPage->field_0_cam_id != MainMenuCams::eLoadGameMenuCam)
                    {
                        for (;;)
                        {
                            field_1FC_button_index++;
                            if (!btnArray[field_1FC_button_index].field_0)
                            {
                                field_1FC_button_index = 0;
                                break;
                            }
                            else if (btnArray[field_1FC_button_index].field_0 == 1)
                            {
                                break;
                            }
                        }
                    }

                    field_158_animation.Set_Animation_Data_409C80(pPage->field_18_buttons[field_1FC_button_index].field_8_anim_frame_offset, 0);
                    SFX_Play_46FBA0(SoundEffect::MenuNavigation_52, 35, 400);
                }
            }
        }

        if (field_21E_bChangeScreen)
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

        if (pageUpdateReturnedCam.page_update_camera == MainMenuCams::eNoChange || 
            pageUpdateReturnedCam.page_update_camera == gMap_5C3030.field_4_current_camera)
        {
            // stay on the same screen
            return;
        }

        field_218_target_page_index = static_cast<short>(GetPageIndexFromCam_4D05A0(pageUpdateReturnedCam.page_update_camera));
        field_21A_target_cam = pageUpdateReturnedCam.button_idx_to_highlight;
        isScreenTransitionNecessary = BYTE1(pageUpdateReturnedCam.page_update_camera);
    }
    else
    {
        field_1F8_page_timeout = 0;
        field_218_target_page_index = static_cast<short>(GetPageIndexFromCam_4D05A0(pPage->field_8_next_idx));
        field_21A_target_cam = pPage->field_C_target_camera;
        isScreenTransitionNecessary = pPage->field_A_transistion_effect;
    }

    field_21C_bDoScreenTransistionEffect = static_cast<short>(isScreenTransitionNecessary);
    field_21E_bChangeScreen = 1;
}

void MainMenuController::Unload_Resource(BYTE**& res)
{
    // Resource might be pending, wait for it to load if so
    if (!res)
    {
        pResourceManager_5C1BB0->LoadingLoop_465590(FALSE);
    }

    // Now we can free it
    ResourceManager::FreeResource_49C330(res);
    res = nullptr;

    // Prevent animation since its now unloaded
    field_20_animation.field_4_flags.Clear(AnimFlags::eBit2_Animate);
}

void MainMenuController::Update_4CF010()
{
    if (sDoesCreditsControllerExist_5C1B90)
    {
        HandleCreditsControllerUpdate();
    }
    else
    {
        HandleMainMenuUpdate();
    }
}

int CCSTD MainMenuController::GetPageIndexFromCam_4D05A0(int camId)
{
    for (int i = 0; i < 24; i++)
    {
        if (camId == sMainMenuPages_561960[i].field_0_cam_id)
        {
            return i;
        }
    }

    return 0;
}

void MainMenuController::Set_Anim_4D05E0(__int16 a2, __int16 a3)
{
    if (a2 != this->field_220_frame_table_idx || sMainMenuFrameTable_561CC8[a2].field_8_anim_enum == a2)
    {
        this->field_228_res_idx = a2;
        this->field_22A_anim_frame_num = a3;
    }
}

void MainMenuController::Load_Anim_Pal_4D06A0(Animation* pAnim)
{
    // TODO: If this is duplicated a lot it can be moved to a helper
    const FrameInfoHeader* pFrameInfo = pAnim->Get_FrameHeader_40B730(0);
    const FrameHeader* pFrameHeader = reinterpret_cast<const FrameHeader*>(&(*pAnim->field_20_ppBlock)[pFrameInfo->field_0_frame_header_offset]);
    pAnim->Load_Pal_40A530(pAnim->field_20_ppBlock, pFrameHeader->field_0_clut_offset);
}

ALIVE_VAR(1, 0x5ca408, DWORD, sLevelId_dword_5CA408, 0);

signed int MainMenuController::ChangeScreenAndIntroLogic_4CF640()
{
    if (field_21E_bChangeScreen == 0 || field_23C_T80.Get(Flags::eBit22_GameSpeakPlaying) || field_228_res_idx != 0)
    {
        return 0;
    }

    CameraSwapEffects screenChangeEffect = CameraSwapEffects::eEffect0_InstantChange;

    switch (field_21E_bChangeScreen)
    {
    case 1:
        if (field_21C_bDoScreenTransistionEffect != 0)
        {
            if (field_21C_bDoScreenTransistionEffect != 2)
            {
                field_21E_bChangeScreen = 2;
                return 1;
            }

            if (field_208_transition_obj)
            {
                field_208_transition_obj->StartTrans_464370(40, 1, 0, 16);
                field_21E_bChangeScreen = 2;
                return 1;
            }

            field_208_transition_obj = ae_new<MainMenuTransition>();
            if (field_208_transition_obj)
            {
                field_208_transition_obj->ctor_464110(40, 1, 0, 16, 2);
                field_21E_bChangeScreen = 2;
                return 1;
            }
        }
        else
        {
            if (field_208_transition_obj)
            {
                field_208_transition_obj->StartTrans_464370(40, 1, 0, 16);
                field_21E_bChangeScreen = 2;
                return 1;
            }

            field_208_transition_obj = ae_new<MainMenuTransition>();
            if (field_208_transition_obj)
            {
                field_208_transition_obj->ctor_464110(40, 1, 0, 16, 1);
                field_21E_bChangeScreen = 2;
                return 1;
            }
        }
        // ae_new failure case
        field_21E_bChangeScreen = 2;
        return 1;

    case 2:
        if (sMainMenuPages_561960[field_214_page_index].field_A_transistion_effect == 7)
        {
            char buffer[256] = {};

            // Use path 2
            strcpy(buffer, sCdEmu_Path2_C144C0);
            strcat(buffer, "movies");

            if (!IO_DirectoryExists(buffer))
            {
                // Can't enumerate anything at all in path 2, try path 3
                strcpy(buffer, sCdEmu_Path3_C145A0);
                strcat(buffer, "movies");
            }

            int i = 0;
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
                Display_Full_Screen_Message_Blocking_465820(1, MessageType::eShortTitle_3);
            }

            // Find the record for GTILOGO.STR
            FmvInfo* pFmvRecord = Path_Get_FMV_Record_460F70(LevelIds::eMenu_0, 3u);
            while (Input_IsVKPressed_4EDD40(VK_RETURN))
            {
                SYS_EventsPump_494580();
            }

            DWORD v34 = 0; // Gets set to 0x11111111
            Get_fmvs_sectors_494460(pFmvRecord->field_0_pName, 0, 0, &v34, 0, 0);
            sLevelId_dword_5CA408 = 0;

            // Create a movie object for the GTI logo
            auto pMovie = ae_new<Movie>();
            if (pMovie)
            {
                pMovie->ctor_4DFDE0(
                    pFmvRecord->field_4_id,
                    v34,
                    pFmvRecord->field_6_flags & 1,
                    pFmvRecord->field_8_flags,
                    pFmvRecord->field_A_volume);
            }

            // Run the movie till its done
            while (sMovie_ref_count_BB4AE4 > 0)
            {
                if (pMovie->field_6_flags.Get(BaseGameObject::eUpdatable_Bit2))
                {
                    if (pMovie->field_6_flags.Get(BaseGameObject::eDead_Bit3) == false && (!sNum_CamSwappers_5C1B66 || pMovie->field_6_flags.Get(BaseGameObject::eUpdateDuringCamSwap_Bit10)))
                    {
                        pMovie->VUpdate();
                    }
                }
            }

            while (Input_IsVKPressed_4EDD40(13))
            {
                SYS_EventsPump_494580();
            }

            // Create movie object for the DD logo
            Get_fmvs_sectors_494460("DDLOGO.STR", 0, 0, &v34, 0, 0);
            sLevelId_dword_5CA408 = 0;
            pMovie = ae_new<Movie>();

            if (pMovie)
            {
                pMovie->ctor_4DFDE0(
                    pFmvRecord->field_4_id,
                    v34,
                    pFmvRecord->field_6_flags & 1,
                    pFmvRecord->field_8_flags,
                    pFmvRecord->field_A_volume);
            }

            // Run the movie till its done
            while (sMovie_ref_count_BB4AE4 > 0)
            {
                if (pMovie->field_6_flags.Get(BaseGameObject::eUpdatable_Bit2))
                {
                    if (pMovie->field_6_flags.Get(BaseGameObject::eDead_Bit3) == false && (!sNum_CamSwappers_5C1B66 || pMovie->field_6_flags.Get(BaseGameObject::eUpdateDuringCamSwap_Bit10)))
                    {
                        pMovie->VUpdate();
                    }
                }
            }

            gPsxDisplay_5C1130.PutCurrentDispEnv_41DFA0();

            gMap_5C3030.SetActiveCam_480D30(LevelIds::eMenu_0, 1, 1, CameraSwapEffects::eEffect5_1_FMV, 10502, 0);

            field_20_animation.Set_Animation_Data_409C80(0xC424, field_F4_resources.field_0_resources[MenuResIds::eDoor]);
            Load_Anim_Pal_4D06A0(&field_20_animation);

            field_23C_T80.Set(Flags::eBit17_bDisableChangingSelection);
            field_220_frame_table_idx = 9;
            field_228_res_idx = 0;
            field_21E_bChangeScreen = 3;
            return 1;
        }

        switch (field_21C_bDoScreenTransistionEffect)
        {
        case 0:
        case 2:
            if (field_208_transition_obj->field_26_bDone)
            {
                break;
            }
            return 1;
        case 3:
            screenChangeEffect = CameraSwapEffects::eEffect3_TopToBottom;
            break;
        case 4:
            screenChangeEffect = CameraSwapEffects::eEffect4_BottomToTop;
            break;
        case 5:
            screenChangeEffect = CameraSwapEffects::eEffect1_LeftToRight;
            break;
        case 6:
            screenChangeEffect = CameraSwapEffects::eEffect2_RightToLeft;
            break;
        default:
            break;
        }

        gMap_5C3030.SetActiveCam_480D30(
            LevelIds::eMenu_0,
            1,
            sMainMenuPages_561960[field_218_target_page_index].field_0_cam_id,
            screenChangeEffect,
            0,
            0);

        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit2_Animate);
        field_21E_bChangeScreen = 3;
        return 1;

    case 3:
        if (sMainMenuPages_561960[field_214_page_index].field_20_fn_on_free)
        {
            (this->*sMainMenuPages_561960[field_214_page_index].field_20_fn_on_free)();
        }
        field_21E_bChangeScreen = 4;
        return 1;

    case 4:
    {
        if (sNum_CamSwappers_5C1B66 > 0)
        {
            return 1;
        }

        if (field_21C_bDoScreenTransistionEffect == 0 || field_21C_bDoScreenTransistionEffect == 2)
        {
            field_208_transition_obj->StartTrans_464370(40, 0, 0, 16);
        }

        field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
        field_216_previous_page_index = field_214_page_index;
        field_214_page_index = field_218_target_page_index;
        field_21E_bChangeScreen = 5;

        if (sMainMenuPages_561960[field_218_target_page_index].field_18_buttons != nullptr)
        {
            field_1FC_button_index = field_21A_target_cam;
            if (field_21A_target_cam == -1)
            {
                MainMenuButton* pButtonsIter = sMainMenuPages_561960[field_218_target_page_index].field_18_buttons;
                short useButtonIdx = 0;
                for (;;)
                {
                    // Last item ?
                    if (pButtonsIter->field_0 == 0)
                    {
                        break;
                    }

                    // "Pressed" item ?
                    if (pButtonsIter->field_0 == 1)
                    {
                        field_1FC_button_index = useButtonIdx;
                        break;
                    }

                    useButtonIdx++;
                    pButtonsIter++;
                }
            }

            if (field_1FC_button_index != -1)
            {
                field_158_animation.Set_Animation_Data_409C80(
                    sMainMenuPages_561960[field_218_target_page_index].field_18_buttons[field_1FC_button_index].field_8_anim_frame_offset,
                    nullptr);
            }
        }


        if (sMainMenuPages_561960[field_214_page_index].field_1C_fn_on_load)
        {
            (this->*sMainMenuPages_561960[field_214_page_index].field_1C_fn_on_load)();
        }

        return 1;
    }

    case 5:
        if (field_21C_bDoScreenTransistionEffect != 7)
        {
            field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);
            field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
        }

        if ((field_21C_bDoScreenTransistionEffect == 0 || field_21C_bDoScreenTransistionEffect == 2) && !field_208_transition_obj->field_26_bDone)
        {
            return 1;
        }

        field_21E_bChangeScreen = 0;
        return 0;
    default:
        return 0;
    }
}

const SfxDefinition sScrabSfx_560330[10] =
{
    { 0u, 4u, 60u, 55u, 0, 0 },
    { 0u, 4u, 61u, 70u, 0, 0 },
    { 0u, 4u, 62u, 80u, 0, 0 },
    { 0u, 4u, 63u, 80u, 0, 0 },
    { 0u, 4u, 64u, 60u, -127, 127 },
    { 0u, 4u, 66u, 90u, 0, 0 },
    { 0u, 4u, 67u, 50u, -511, 0 },
    { 0u, 4u, 67u, 50u, 0, 511 },
    { 0u, 4u, 68u, 110u, -1791, -1791 },
    { 0u, 0u, 0u, 0u, 0, 0 }
};



void MainMenuController::AnimationAndSoundLogic_4CFE80()
{
    if (!sMainMenuPages_561960[field_214_page_index].field_E_show_character)
    {
        field_22C_T80_animation_delay = 0;
        return;
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame) && !field_22C_T80_animation_delay)
    {
        if (!field_228_res_idx)
        {
            Set_Anim_4D05E0(
                sMainMenuFrameTable_561CC8[field_220_frame_table_idx].field_8_anim_enum,
                sMainMenuFrameTable_561CC8[field_220_frame_table_idx].field_A_frame_number);
        }

        switch (field_220_frame_table_idx)
        {
        case AnimIds::eAbe_Idle:
            if (field_224_timer_anim_delay <= static_cast<int>(sGnFrame_5C1B84))
            {
                Set_Anim_4D05E0(2, 0);
                field_224_timer_anim_delay = sGnFrame_5C1B84 + Math_RandomRange_496AB0(120, 450);
            }
            break;

        case 9: // ??
            field_6_flags.Clear(BaseGameObject::eUpdateDuringCamSwap_Bit10);
            field_22C_T80_animation_delay = 15;
            
            if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbespeakResID, FALSE, FALSE))
            {
                // TODO: Fix the types
                ResourceManager::LoadResourceFile_49C130("ABESPEAK.BAN", 
                    reinterpret_cast<ResourceManager::TLoaderFn>(callback_4D06E0), 
                    reinterpret_cast<Camera *>(this), nullptr);
            }
            // Fall through to next case
        case 10: // ??
            ResourceManager::FreeResource_49C330(field_F4_resources.field_0_resources[sMainMenuFrameTable_561CC8[field_220_frame_table_idx].field_4_menu_res_id]);
            field_F4_resources.field_0_resources[sMainMenuFrameTable_561CC8[field_220_frame_table_idx].field_4_menu_res_id] = nullptr;
            break;

        case 11: // ??
            ResourceManager::Reclaim_Memory_49C470(0);
            if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbespeakResID, 0, 0))
            {
                // TODO: Fix the types
                ResourceManager::LoadResourceFile_49C130("ABESPEAK.BAN",
                    reinterpret_cast<ResourceManager::TLoaderFn>(callback_4D06E0),
                    reinterpret_cast<Camera *>(this), nullptr);
            }
            field_23C_T80.Clear(Flags::eBit17_bDisableChangingSelection);
            field_224_timer_anim_delay = sGnFrame_5C1B84 + Math_RandomRange_496AB0(300, 450);
            break;

        case AnimIds::eAbe_Chant:
            if (Input_IsChanting_45F260())
            {
                Set_Anim_4D05E0(0);
            }
            else
            {
                SND_SEQ_Stop_4CAE60(SeqId::MudokonChant1_10);

                if (field_20C_pUnused)
                {
                    ALIVE_FATAL("Never expected field_20C to be used");
                }

                if (field_210_pUnused)
                {
                    ALIVE_FATAL("Never expected field_210_pUnknown to be used");
                }


                field_23C_T80.Clear(Flags::eBit24_Chant_Seq_Playing);
                Set_Anim_4D05E0(13);
            }
            break;

        case AnimIds::eSlig_Idle:
            break;

        case AnimIds::eGlukkon_Idle:
            if (field_224_timer_anim_delay <= static_cast<int>(sGnFrame_5C1B84))
            {
                Set_Anim_4D05E0(28); // ??
                field_224_timer_anim_delay = sGnFrame_5C1B84 + Math_RandomRange_496AB0(120, 450);
            }
            break;

        case AnimIds::eScrab_Idle:
            if (field_224_timer_anim_delay <= static_cast<int>(sGnFrame_5C1B84))
            {
                Set_Anim_4D05E0(38); // ??
                field_224_timer_anim_delay = sGnFrame_5C1B84 + Math_RandomRange_496AB0(120, 450);
            }
            break;

        case AnimIds::eParamite_Idle:
            if (field_224_timer_anim_delay <= static_cast<int>(sGnFrame_5C1B84))
            {
                Set_Anim_4D05E0(42); // ??
                field_224_timer_anim_delay = sGnFrame_5C1B84 + Math_RandomRange_496AB0(120, 450);
            }
            break;

        default:
            field_23C_T80.Clear(Flags::eBit22_GameSpeakPlaying);
            break;
        }
    }

    if (sMainMenuFrameTable_561CC8[field_220_frame_table_idx].field_4_menu_res_id == eParamiteSpeak)
    {
        if (field_220_frame_table_idx != eParamite_Idle && field_220_frame_table_idx != eParamite_Idle2)
        {
            int doSpeakSoundFrame = 0;
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

            if (field_20_animation.field_92_current_frame == doSpeakSoundFrame)
            {
                if (field_220_frame_table_idx == eParamite_AllAYa)
                {
                    SFX_SfxDefinition_Play_4CA700(&stru_55D7C0[sMainMenuFrameTable_561CC8[field_220_frame_table_idx].field_6_sound], 127, 127, 64, 64);
                }
                // Attack
                else if (sMainMenuFrameTable_561CC8[field_220_frame_table_idx].field_6_sound == 9)
                {
                    SND_SEQ_Play_4CAB10(SeqId::MainMenuParamiteAttack_20, 1, 127, 127);
                }
                // All other Paramite speak
                else
                {
                    SFX_SfxDefinition_Play_4CA700(&stru_55D7C0[sMainMenuFrameTable_561CC8[field_220_frame_table_idx].field_6_sound], 127, 127, 0x7FFF, 0x7FFF);
                }
            }
        }
    }

    if (field_22C_T80_animation_delay)
    {
        field_22C_T80_animation_delay--;
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)
        || field_220_frame_table_idx == eAbe_Idle
        || field_220_frame_table_idx == 2
        || field_220_frame_table_idx == eSlig_Idle
        || field_220_frame_table_idx == eGlukkon_Idle
        || field_220_frame_table_idx == 28
        || field_220_frame_table_idx == eScrab_Idle
        || field_220_frame_table_idx == 38
        || field_220_frame_table_idx == eParamite_Idle
        || field_220_frame_table_idx == 42)
    {
        if (!field_22C_T80_animation_delay)
        {
            if (field_F4_resources.field_0_resources[sMainMenuFrameTable_561CC8[field_228_res_idx].field_4_menu_res_id])
            {
                if (field_228_res_idx > 0)
                {
                    const __int16 sound = sMainMenuFrameTable_561CC8[field_228_res_idx].field_6_sound;
                    if (sound != -1)
                    {
                        switch (sMainMenuFrameTable_561CC8[field_228_res_idx].field_4_menu_res_id)
                        {
                        case eAbeSpeak:
                        case eAbeSpeak2:
                            Mudokon_SFX_457EC0(static_cast<MudSounds>(sMainMenuFrameTable_561CC8[field_228_res_idx].field_6_sound), 0, 0, nullptr);
                            field_23C_T80.Set(Flags::eBit22_GameSpeakPlaying); // BYTE2(0x20)
                            break;

                        case eSligSpeak:
                            Slig_GameSpeak_SFX_4C04F0(static_cast<SligSpeak>(sMainMenuFrameTable_561CC8[field_228_res_idx].field_6_sound), 0, 0, 0);
                            field_23C_T80.Set(Flags::eBit22_GameSpeakPlaying);
                            break;

                        case eGlukkonSpeak:
                            Glukkon::PlaySound_GameSpeak_444AF0(static_cast<GlukkonSpeak>(sMainMenuFrameTable_561CC8[field_228_res_idx].field_6_sound), 0, 0, 0);
                            field_23C_T80.Set(Flags::eBit22_GameSpeakPlaying);
                            break;

                        case eScrabSpeak:
                            SFX_SfxDefinition_Play_4CA700(
                                &sScrabSfx_560330[sound],
                                sScrabSfx_560330[sound].field_3_default_volume,
                                sScrabSfx_560330[sound].field_3_default_volume,
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
                            SND_SEQ_PlaySeq_4CA960(SeqId::MudokonChant1_10, 0, 1);
                            field_23C_T80.Set(Flags::eBit24_Chant_Seq_Playing);
                        }

                        field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);

                        field_20_animation.Set_Animation_Data_409C80(
                            sMainMenuFrameTable_561CC8[field_228_res_idx].field_0_frame_offset,
                            field_F4_resources.field_0_resources[sMainMenuFrameTable_561CC8[field_228_res_idx].field_4_menu_res_id]);

                        if (sMainMenuFrameTable_561CC8[field_228_res_idx].field_4_menu_res_id != sMainMenuFrameTable_561CC8[field_220_frame_table_idx].field_4_menu_res_id)
                        {
                            Load_Anim_Pal_4D06A0(&field_20_animation);
                        }

                        field_20_animation.SetFrame_409D50(field_22A_anim_frame_num);

                        if (field_228_res_idx != eAbe_Chant && field_23C_T80.Get(Flags::eBit24_Chant_Seq_Playing))
                        {
                            SND_SEQ_Stop_4CAE60(SeqId::MudokonChant1_10);
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

    field_158_animation.field_8_r = static_cast<BYTE>(field_1FE_highlite_alpha);
    field_158_animation.field_A_b = static_cast<BYTE>(field_1FE_highlite_alpha);
    field_158_animation.field_9_g = static_cast<BYTE>(field_1FE_highlite_alpha);
}

void MainMenuController::callback_4D06E0(MainMenuController* pMenu)
{
    pMenu->field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak] = 
            ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbespeakResID, TRUE, FALSE);
}

void MainMenuController::DrawMenuText_4D20D0(const MainMenuText* array, int** ot, Alive::Font* font, int* polyIndex, char op2)
{
    const bool bCancel = strstr(array->field_8_text, byte_55EE00) != 0; // cancel
    const bool bEndOfMedium = strstr(array->field_8_text, byte_55EDEC) != 0; // end of medium

    char textBuffer[32] = {};
    String_FormatString_4969D0(array->field_8_text, textBuffer, ALIVE_COUNTOF(textBuffer), array->field_14 == 0);
    
    if (op2)
    {
        char* plusSignIx = strchr(textBuffer, '+');
        if (plusSignIx)
        {
            strcpy(textBuffer, plusSignIx + 1);
        }
    }

    if (!array->field_14)
    {
        if (textBuffer[0])
        {
            if (bCancel != bEndOfMedium)
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

    const int array_field_x = array->field_0_x;
    const int array_field_y = array->field_4_y;

    short text_xpos = 0;
    if (array->field_C_align == 1)
    {
        text_xpos = static_cast<short>(PsxToPCX(array_field_x));
    }
    else
    {
        int fontWidth = font->MeasureWidth_4336C0(textBuffer, fontScale);
        int halfFontWidth = fontWidth / -2;
        if (array->field_C_align == 2)
        {
            halfFontWidth = -fontWidth;
        }
        text_xpos = static_cast<short>(strlen(textBuffer) + PsxToPCX(halfFontWidth + array_field_x));
    }

    const FP text_ypos = FP_FromInteger(-10) * fontScale; // -655360

    const BYTE oldDrawInScreenSpace = sFontDrawScreenSpace_5CA4B4;
    sFontDrawScreenSpace_5CA4B4 = 1;

    *polyIndex = font->DrawString_4337D0(
        ot,
        textBuffer,
        text_xpos,
        static_cast<short>(FP_GetExponent(text_ypos) + array_field_y + 1),
        0,
        1,
        0,
        39,
        0,
        255,
        0,
        *polyIndex,
        fontScale,
        640,
        0);

    sFontDrawScreenSpace_5CA4B4 = oldDrawInScreenSpace;
}

void MainMenu_ForceLink()
{
}

void MainMenuController::Unload_AbeSpeakResources()
{
    // If they are currently loading wait for them to finish
    if (!field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak] || !field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak2])
    {
        pResourceManager_5C1BB0->LoadingLoop_465590(FALSE);
    }

    // And then bin them off to make room for new resources
    ResourceManager::FreeResource_49C330(field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak]);
    ResourceManager::FreeResource_49C330(field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak2]);

    field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak2] = nullptr;
    field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak] = nullptr;

    // Prevent animation since its now unloaded
    field_20_animation.field_4_flags.Clear(AnimFlags::eBit2_Animate);

    // Compact the heap
    ResourceManager::Reclaim_Memory_49C470(0);

    Game_ShowLoadingIcon_482D80();
}

void MainMenuController::Load_AbeSpeakResources()
{
    // Compact heap + load file to memory
    ResourceManager::Reclaim_Memory_49C470(0);
    ResourceManager::LoadResourceFile_49C170("ABESPEK2.BAN", nullptr);

    // Get the resource, if we don't have it compact heap again
    field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak2] = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbespek2ResID, TRUE, FALSE);
    ResourceManager::Reclaim_Memory_49C470(0);

    // And then try to load it
    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbespeakResID, FALSE, FALSE))
    {
        Game_ShowLoadingIcon_482D80();
        ResourceManager::LoadResourceFile_49C170("ABESPEAK.BAN", nullptr);
    }

    field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak] = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbespeakResID, TRUE, FALSE);
}
