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
#include "Midi.hpp"
#include "Abe.hpp"
#include "Psx.hpp"
#include "Particle.hpp"
#include "CameraSwapper.hpp"
#include "Movie.hpp"
#include "MainMenuTransition.hpp"
#include "Text.hpp"
#include "Sound.hpp"
#include "Path.hpp"
#include "Abe.hpp"
#include "PauseMenu.hpp"

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

ALIVE_VAR(1, 0x5C1BEC, short, word_5C1BEC, {});
ALIVE_VAR(1, 0x5C1BC6, short, word_5C1BC6, {});
ALIVE_VAR(1, 0x5C1BC8, short, word_5C1BC8, {});
ALIVE_VAR(1, 0x5c1b9e, __int16, word_5C1B9E, 0);
ALIVE_VAR(1, 0x5C1B64, byte, byte_5C1B64, {});

ALIVE_VAR(1, 0x561538, short, sMenuItemCount_561538, 0);
ALIVE_VAR(1, 0x5C1B50, PerPathMudStats, sSavedKilledMudsPerPath_5C1B50, {});
ALIVE_VAR(1, 0xbb4414, void *, pDemosOrFmvs_BB4414, 0);
ALIVE_VAR(1, 0x5c2f68, const char, byte_5C2F68, 0);

MainMenuButton sBtnArray_FrontEnd_560FA0[6] =
{
    { 1, 33, 82, 0, 13912 },
    { 1, 32, 104, 0, 13912 },
    { 1, 337, 219, 0, 13912 },
    { 1, 337, 239, 0, 13912 },
    { 1, 33, 62, 0, 13912 },
    { 0, 0, 0, 0, 0 }
};

// HACK HACK FIX ME - all of these buttons are in one contiguous array in the real game
// we need to replicate this as the game will access this array with the index of the PREVIOUS screen
// which will be out of bounds
MainMenuButton sBtnArray_Game_BackStory_Or_NewGame_561420[40] = 
{
    { 1, 59, 199, 0, 13912 },
    { 1, 248, 199, 0, 13912 },
    { 3, 180, 240, 0, 13912 },
    { 0, 0, 0, 0, 0 }
};

MainMenuButton sBtnArray_Gamespeak_561310[7] =
{
    { 1, 153, 122, 0, 13912 },
    { 1, 153, 143, 0, 13912 },
    { 1, 153, 164, 0, 13912 },
    { 1, 153, 184, 0, 13912 },
    { 1, 153, 205, 0, 13912 },
    { 3, 313, 240, 0, 13924 },
    { 0, 0, 0, 0, 0 }
};

MainMenuButton sBtnArray_Options_561368[4] =
{
    { 1, 33, 62, 0, 13912 },
    { 1, 336, 61, 0, 13912 },
    { 3, 332, 240, 0, 13912 },
    { 0, 0, 0, 0, 0 }
};

MainMenuButton sBtnArray_LoadGame_561450[19] =
{
    { 1, 32, 62, 0, 13912 },
    { 3, 331, 239, 0, 13912 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 2, 127, 74 },
    { 13936, 0, 2, 127, 96 },
    { 13936, 0, 2, 127, 117 },
    { 13936, 0, 2, 127, 141 },
    { 13936, 0, 2, 165, 74 },
    { 13936, 0, 2, 165, 96 },
    { 13936, 0, 2, 165, 117 },
    { 13936, 0, 2, 165, 141 },
    { 13936, 0, 2, 201, 74 },
    { 13936, 0, 2, 201, 96 },
    { 13936, 0, 2, 201, 117 },
    { 13936, 0, 2, 201, 141 },
    { 13936, 0, 2, 240, 74 },
    { 13936, 0, 2, 240, 96 },
    { 13936, 0, 2, 240, 117 },
    { 13936, 0, 0, 0, 0 }
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
    {
        23,       0,        0,        0,        -1,        0,        0,
        nullptr, // &MainMenuController::sub_JoyStick_4D16D0,
        nullptr, //&MainMenuController::sub_Render_4D26C0,
        NULL,
        nullptr, //&MainMenuController::tsub_Set_JoyStick_4D16B0,
        NULL
    },
    {
        5,        0,        0,        0,        -1,        0,        0,
        nullptr, //&MainMenuController::tsub_4D1820,
        nullptr, //&MainMenuController::t_Remap_Input_Render_4D2A10,
        NULL,
        nullptr, //&MainMenuController::tsub_4D17E0,
        NULL
    },
    {
        11,        0,        0,        0,        -1,        0,       0,
        NULL,        NULL,        NULL,
        &MainMenuController::Game_Force_Quit_Load_4D1A90,
        NULL
    },
    {
        // Main menu screen where abe says hello
        1,        0,        0,        0,        -1,        -1,    1,
        &MainMenuController::Page_Front_Update_4D0720,
        &MainMenuController::Page_Front_Render_4D24B0,
        sBtnArray_FrontEnd_560FA0,
        NULL,
        NULL
    },
    { // Gamespeak Page
        2,        0,        900,        1,        0,        4,        0,
        &MainMenuController::t_Input_Gamespeak_4D1FC0,
        &MainMenuController::t_Render_All_Text_4D24F0,
        sBtnArray_Gamespeak_561310,
        NULL,
        NULL
    },
    { // Options Page
        3,        0,        900,        1,        0,        3,        1,
        nullptr, //&MainMenuController::tsub_DemoMenu_4D1AB0,
        &MainMenuController::t_Render_All_Text_4D2540,
        sBtnArray_Options_561368,
        NULL,
        NULL
    },
    {
        9,        0,        1600,        3,        0,        0,        0,
        nullptr, //&MainMenuController::tsub_4D1F50,
        nullptr, //&MainMenuController::t_Render_All_Text_4D25E0,
        NULL,
        NULL,
        NULL
    },
    { // Load game page
        4,        0,        1000,        1,        0,        2,        0,
        &MainMenuController::tLoadGame_Input_4D3EF0,
        &MainMenuController::tLoadGame_Render_4D44D0,
        sBtnArray_LoadGame_561450,
        &MainMenuController::tLoadGame_Load_4D42F0,
        &MainMenuController::tLoadGame_Unload_4D4360
    },
    {
        5,        0,        900,        1,        0,        0,        1,
        nullptr, //&MainMenuController::tsub_4D48C0,
        NULL,
        nullptr, //&sBtnArray_5613C8,
        NULL,
        NULL
    },
    {
        11,        0,        1800,        5,        0,        1,        0,
        nullptr, //&MainMenuController::tsub_Input_Check_4D49B0,
        NULL,
        nullptr, //&sBtnArray_5613F8,
        NULL,
        NULL
    },
    {
        6,        0,        0,        1,        0,        0,        0,
        nullptr, //&MainMenuController::Page_FMV_Level_Update_4D4AB0,
        nullptr, //&MainMenuController::tsub_Render_4D4F30,
        nullptr, //&sBtnArray_560FE8,
        nullptr, //&MainMenuController::t_Unload_AbeSpeak_Res_4D49F0,
        nullptr, //&MainMenuController::t_Load_AbeSpeak_Res_4D4A20
    },
    {
        31,        0,        0,        1,        0,        0,        0,
        nullptr, //&MainMenuController::Page_FMV_Level_Update_4D4AB0,
        nullptr, //&MainMenuController::tsub_Render_4D4F30,
        nullptr, //&sBtnArray_560FE8,
        NULL,
        NULL
    },
    {
        30,        0,        0,        1,        0,        1,        0,
        nullptr, //&MainMenuController::tsub_4D0E10,
        nullptr, //&MainMenuController::tsub_Render_4D4F30,
        NULL,
        NULL,
        NULL
    },
    {
        13,        0,        0,        0,        -1,        -1,        0,
        &MainMenuController::tLoad_New_Game_Input_4D0920,
        NULL,
        NULL,
        NULL,
        NULL
    },
    {
        22,        0,        0,        0,        -1,        -1,        0,
        &MainMenuController::tsub_LoadSave_Input_4D1040,
        NULL,
        NULL,
        NULL,
        NULL
    },
    { 
        20, 0, 150, 25, 1, -1, 0, NULL, NULL, NULL, NULL, NULL
    },
    { 
        // Copy right boot screen
        25, 0, 150, 1, 7, 0, 0, NULL, NULL, NULL, NULL, NULL 
    },
    { 
        23, 0, 150, 25, 1, -1, 0, NULL, NULL, NULL, NULL, NULL 
    },
    {
        14,        0,        1600,        2,        0,        0,        1,
        &MainMenuController::t_Input_Abe_Speak_4D2D20,
        &MainMenuController::t_Render_Abe_Speak_4D2060,
        NULL,
        NULL,
        NULL
    },
    {
        15,        0,        1600,        2,        0,        1,        1,
        &MainMenuController::t_Input_Slig_Speak_4D3280,
        &MainMenuController::t_Render_Slig_Speak_4D2370,
        NULL,
        &MainMenuController::t_Load_Slig_Speak_4D3090,
        &MainMenuController::t_Unload_Slig_Speak_4D3170
    },
    {
        16,        0,        1600,        2,        0,        2,        1,
        &MainMenuController::t_Input_Glukkon_Speak_4D3670,
        &MainMenuController::t_Render_Glukkon_Speak_4D23C0,
        NULL,
        &MainMenuController::t_Load_Glukkon_Speak_4D3480,
        &MainMenuController::t_Unload_Glukkon_Speak_4D3560
    },
    {
        17,        0,        1600,        2,        0,        3,        1,
        &MainMenuController::t_Input_Scrab_Speak_4D3A60,
        &MainMenuController::t_Render_Scrab_Speak_4D2410,
        NULL,
        &MainMenuController::t_Load_Scrab_Speak_4D3870,
        &MainMenuController::t_Unload_Scrab_Speak_4D3950
    },
    {
        18,        0,        1600,        2,        0,        4,        1,
        &MainMenuController::t_Input_Paramite_Speak_4D3D60,
        &MainMenuController::t_Render_Paramite_Speak_4D2460,
        NULL,
        &MainMenuController::t_Load_Paramite_Speak_4D3B70,
        &MainMenuController::t_Unload_Paramite_Speak_4D3C50
    },
    {
        12,        0,        900,        1,        0,        0,        0,
        &MainMenuController::tGame_BackStory_Or_NewGame_Input_4D1C60,
        &MainMenuController::tGame_BackStory_Or_NewGame_Render_4D2630,
        sBtnArray_Game_BackStory_Or_NewGame_561420,
        &MainMenuController::tGame_BackStory_Or_NewGame_Load_4D1BA0,
        &MainMenuController::tGame_BackStory_Or_NewGame_Unload_4D1BE0
    }
});

inline std::int16_t operator "" _s(unsigned long long value)
{
    return static_cast<std::int16_t>(value);
}

MenuFMV sFmvs_561540[28] =
{
    { "GT Logo", 0_s, 65535_s, 65535_s, 3_s, 65535_s, 65535_s },
    { "Oddworld Intro", 0_s, 65535_s, 65535_s, 1, 65535_s, 65535_s },
    { "Abe's Exoddus", 0_s, 65535_s, 65535_s, 5, 65535_s, 65535_s },
    { "Backstory", 0_s, 65535_s, 65535_s, 4, 65535_s, 65535_s },
    { "Prophecy", 1_s, 65535_s, 65535_s, 1, 65535_s, 65535_s },
    { "Vision", 1_s, 65535_s, 65535_s, 24, 65535_s, 65535_s },
    { "Game Opening", 1_s, 65535_s, 65535_s, 2, 65535_s, 65535_s },
    { "Brew", 1_s, 65535_s, 65535_s, 26, 65535_s, 65535_s },
    { "Brew Transition", 1_s, 65535_s, 65535_s, 31, 65535_s, 65535_s },
    { "Escape", 1_s, 65535_s, 65535_s, 25, 65535_s, 65535_s },
    { "Reward", 2_s, 65535_s, 65535_s, 22, 65535_s, 65535_s },
    { "FeeCo", 5_s, 65535_s, 65535_s, 4, 65535_s, 65535_s },
    { "Information Booth", 5_s, 65535_s, 65535_s, 3, 65535_s, 65535_s },
    { "Train 1", 6_s, 65535_s, 65535_s, 5, 65535_s, 65535_s },
    { "Train 2", 9_s, 65535_s, 65535_s, 15, 65535_s, 65535_s },
    { "Train 3", 8_s, 65535_s, 65535_s, 6, 65535_s, 65535_s },
    { "Aslik Info", 5_s, 65535_s, 65535_s, 2, 65535_s, 65535_s },
    { "Aslik Explodes", 5_s, 65535_s, 65535_s, 1, 65535_s, 65535_s },
    { "Dripek Info", 6_s, 65535_s, 65535_s, 4, 65535_s, 65535_s },
    { "Dripek Explodes", 6_s, 65535_s, 65535_s, 3, 65535_s, 65535_s },
    { "Phleg Info", 8_s, 65535_s, 65535_s, 4, 65535_s, 65535_s },
    { "Phleg Explodes", 8_s, 65535_s, 65535_s, 5, 65535_s, 65535_s },
    { "Soulstorm Info", 9_s, 65535_s, 65535_s, 14, 65535_s, 65535_s },
    { "Ingredient", 9_s, 65535_s, 65535_s, 16, 65535_s, 65535_s },
    { "Conference", 9_s, 65535_s, 65535_s, 13, 65535_s, 65535_s },
    { "Happy Ending", 9_s, 65535_s, 65535_s, 17, 65535_s, 65535_s },
    { "Sad Ending", 9_s, 65535_s, 65535_s, 18, 65535_s, 65535_s },
    { "Credits", 16_s, 65535_s, 65535_s, 65535_s, 65535_s, 65535_s }
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

    ++sMainMenuObjectCounter_BB4400;
    field_6_flags.Set(BaseGameObject::eUpdateDuringCamSwap);

    gMainMenuController = this;

    if (!sMainMenuFontLoaded_5C1B92)
    {
        sFont1Context_5BC5C8.LoadFontType_433400(1);
        sMainMenuFontLoaded_5C1B92 = 1;
    }

    field_120_font.ctor_433590(240, pal_560F80, &sFont1Context_5BC5C8);
    memset(&field_F4_resources, 0, sizeof(field_F4_resources));

    field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak2] = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, kAbespek2ResID, TRUE, FALSE);
    field_F4_resources.field_0_resources[MenuResIds::eOptionFlare] = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, kOptflareResID, TRUE, FALSE);

    field_F4_resources.field_0_resources[MenuResIds::eUnknown] = nullptr;
    field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak] = nullptr;

    if (gMap_5C3030.sCurrentCamId_5C3034 == 6)
    {
        field_F4_resources.field_0_resources[MenuResIds::eAbeIntro] = nullptr;
        field_F4_resources.field_0_resources[MenuResIds::eDoor] = nullptr;
        Animation_Init_424E10(247808, 233, 0x7Du, field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak2], 1, 1);
    }
    else
    {
        ResourceManager::LoadResourceFile_49C170("ABEINTRO.BAN", nullptr);
        field_F4_resources.field_0_resources[MenuResIds::eAbeIntro] = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, kAbeintroResID, TRUE, FALSE);
        ResourceManager::LoadResourceFile_49C170("STDOOR.BAN", nullptr);
        field_F4_resources.field_0_resources[MenuResIds::eDoor] = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, kDoorResID, TRUE, FALSE);
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
    field_1F0 = tlvOffsetLevelIdPathId.all; // TODO: Should probably be using the same types here, depending on how it gets used

    field_214_page_index = static_cast<short>(GetPageIndexFromCam_4D05A0(gMap_5C3030.sCurrentCamId_5C3034));
    field_21C_bDoScreenTransistionEffect = 1;
    field_21E_bChangeScreen = 0;
    field_1F8_page_timeout = 0;
    field_220_frame_table_idx = 1;
    field_228_res_idx = 0;
    field_22A_anim_frame_num = 0;
    field_22C_T80 = 0;
    field_224 = 0;
    field_202_input_hold_down_timer = 15;
    field_204_prev_pressed = 0;
    field_230_fmv_level_index = 0; // Double check

    field_23C_T80.Clear(Flags::eBit17_bDisableChangingSelection);
    field_23C_T80.Clear(Flags::eBit18);
    field_23C_T80.Clear(Flags::eBit22);
    field_23C_T80.Clear(Flags::eBit23);
    field_23C_T80.Clear(Flags::eBit24_Chant_Seq_Playing);

    if (gMap_5C3030.sCurrentCamId_5C3034 == 1)
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

    field_23C_T80.Clear(Flags::eBit19);
    field_23C_T80.Clear(Flags::eBit21);
    field_23C_T80.Clear(Flags::eBit25);

    field_1FC_button_index = 0;
    field_208_transition_obj = 0;
    field_20C = 0;
    field_210_pUnknown = nullptr;
    field_23A = 0;
    field_234_pStr = &byte_5C2F68;
    field_238 = 0;
    field_25C = 0;
    field_25E = 0;

    memset(&sSavedKilledMudsPerPath_5C1B50.mData, 0, sizeof(sSavedKilledMudsPerPath_5C1B50.mData));

    word_5C1BEC = 0;
    sEnableCheatLevelSelect_5C1BEE = 0;
    sKilledMudokons_5C1BC0 = 0;
    sRescuedMudokons_5C1BC2 = 0;
    word_5C1BA0 = 0;
    byte_5C1B64 = 0;
    word_5C1BC6 = 0;
    word_5C1BC8 = 0;

    if (gMap_5C3030.sCurrentCamId_5C3034 == 6)
    {
        field_1FC_button_index = 0;
        field_250 = 0;
        field_254 = 0;
        field_258 = 0;
        field_25C = 1;
        pDemosOrFmvs_BB4414 = &sFmvs_561540;
        sMenuItemCount_561538 = 28;
        field_20_animation.Set_Animation_Data_409C80(247808, field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak2]);
        Load_Anim_Pal_4D06A0(&field_20_animation);
        return this;
    }

    if (gMap_5C3030.sCurrentCamId_5C3034 == 30)
    {
        ResourceManager::Reclaim_Memory_49C470(0);
        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, kAbespeakResID, 0, 0))
        {
            ResourceManager::LoadResourceFile_49C130("ABESPEAK.BAN", reinterpret_cast<ResourceManager::TLoaderFn>(callback_4D06E0), reinterpret_cast<Camera *>(this), 0);
        }
        
        pResourceManager_5C1BB0->LoadingLoop_465590(false);
        field_1FC_button_index = 0;
        field_250 = word_5C1B9E;
        field_254 = 0;
        field_258 = 0;
        pDemosOrFmvs_BB4414 = &sDemos_5617F0;
        sMenuItemCount_561538 = 23;
        field_230_fmv_level_index = word_5C1B9E;
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
        Mem_Free_495540(this);
    }
    return this;
}

void MainMenuController::dtor_4CEF30()
{
    SetVTable(this, 0x547958);

    Path::TLV_Reset_4DB8E0(field_1F0, -1, 0, 0);
    field_158_animation.vCleanUp_40C630();

    for (auto& res : field_F4_resources.field_0_resources)
    {
        ResourceManager::FreeResource_49C330(res);
    }

    --sMainMenuObjectCounter_BB4400;

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

MainMenuText sMMT_FrontPage_5623A0 = { 35, 205, "x", 3u, 0u, 0u, 0u,  0.75, 0u, 0u, 0u, 0u };

void MainMenuController::t_Render_Abe_Speak_4D2060(int** /*ot*/)
{
    NOT_IMPLEMENTED();
}

signed int MainMenuController::t_Input_Abe_Speak_4D2D20(DWORD input_held)
{
    // 8 is when returning to previous screen
    if (field_230_fmv_level_index != 8 && field_23C_T80.Get(Flags::eBit24_Chant_Seq_Playing))
    {
        // Only 1 when chanting
        if (field_230_fmv_level_index == 1 && (sGnFrame_5C1B84 % 8) == 0)
        {
            // Spawn chant star/flare particle at random locations around abes head
            field_F4_resources.field_0_resources[MenuResIds::eOptionFlare] = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kOptflareResID, FALSE, FALSE);
            Particle* pParticle = alive_new<Particle>();
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
        return 0;
    }

    return HandleGameSpeakInput(input_held, [&](InputCommands cmd)
    {
        switch (cmd)
        {
        case InputCommands::eChant: Set_Anim_4D05E0(AnimIds::eAbe_Chant); return 0;
        case InputCommands::eGameSpeak1: Set_Anim_4D05E0(AnimIds::eAbe_Hello); return 0;
        case InputCommands::eGameSpeak2: Set_Anim_4D05E0(AnimIds::eAbe_FollowMe); return 0;
        case InputCommands::eGameSpeak3: Set_Anim_4D05E0(AnimIds::eAbe_Wait); return 0;
        case InputCommands::eGameSpeak4: Set_Anim_4D05E0(AnimIds::eAbe_Work); return 0;
        case InputCommands::eGameSpeak5: Set_Anim_4D05E0(AnimIds::eAbe_Anger); return 0;
        case InputCommands::eGameSpeak6: Set_Anim_4D05E0(AnimIds::eAbe_AllYa); return 0;
        case InputCommands::eGameSpeak7: Set_Anim_4D05E0(AnimIds::eAbe_Sympathy); return 0;
        case InputCommands::eGameSpeak8: Set_Anim_4D05E0(AnimIds::eAbe_StopIt); return 0;
        case InputCommands::eBack: 
            Set_Anim_4D05E0(AnimIds::eAbe_GoodBye);
            // Stop chanting music
            SND_SEQ_Stop_4CAE60(10u);

            // TODO: Extra case for Abe - recover the type
            if (field_20C)
            {
                *(WORD *)(field_20C + 248) = 1;
                this->field_20C = 0;
            }
            return 0x00002;
        }
        return 0;
    });
}

void MainMenuController::t_Render_Slig_Speak_4D2370(int** /*ot*/)
{
    // TODO: Render the button text
    NOT_IMPLEMENTED();
}

signed int MainMenuController::t_Input_Slig_Speak_4D3280(DWORD input_held)
{
    return HandleGameSpeakInput(input_held, [&](InputCommands cmd)
    {
        switch (cmd)
        {
        case InputCommands::eGameSpeak1: Set_Anim_4D05E0(AnimIds::eSlig_Hi); return 0;
        case InputCommands::eGameSpeak2: Set_Anim_4D05E0(AnimIds::eSlig_HereBoy); return 0;
        case InputCommands::eGameSpeak3: Set_Anim_4D05E0(AnimIds::eSlig_Freeze); return 0;
        case InputCommands::eGameSpeak4: Set_Anim_4D05E0(AnimIds::eSlig_GetEm); return 0;
        case InputCommands::eGameSpeak5: Set_Anim_4D05E0(AnimIds::eSlig_SmoBs); return 0;
        case InputCommands::eGameSpeak6: Set_Anim_4D05E0(AnimIds::eSlig_Bs); return 0;
        case InputCommands::eGameSpeak7: Set_Anim_4D05E0(AnimIds::eSlig_LookOut); return 0;
        case InputCommands::eGameSpeak8: Set_Anim_4D05E0(AnimIds::eSlig_Laugh); return 0;
        case InputCommands::eBack: Set_Anim_4D05E0(AnimIds::eSlig_Laugh); return 0x10002;
        }
        return 0;
    });
}

void MainMenuController::t_Unload_Slig_Speak_4D3170()
{
    Unload_Resource(field_F4_resources.field_0_resources[MenuResIds::eSligSpeak]);
    Load_AbeSpeakResources();
    Set_Anim_4D05E0(AnimIds::eAbe_Idle);
}

void MainMenuController::t_Load_Slig_Speak_4D3090()
{
    Unload_AbeSpeakResources();

    ResourceManager::LoadResourceFile_49C170("SLGSPEAK.BAN", nullptr);
    field_F4_resources.field_0_resources[MenuResIds::eSligSpeak] = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 140, TRUE, FALSE);
    field_20_animation.Set_Animation_Data_409C80(0x10DF18, field_F4_resources.field_0_resources[MenuResIds::eSligSpeak]);
    Set_Anim_4D05E0(AnimIds::eSlig_Idle);
}

void MainMenuController::t_Render_Glukkon_Speak_4D23C0(int** /*ot*/)
{
    NOT_IMPLEMENTED();
}

signed int MainMenuController::t_Input_Glukkon_Speak_4D3670(DWORD input_held)
{
    return HandleGameSpeakInput(input_held, [&](InputCommands cmd)
    {
        switch (cmd)
        {
        case InputCommands::eGameSpeak1: Set_Anim_4D05E0(AnimIds::eGlukkon_Hey); return 0;
        case InputCommands::eGameSpeak2: Set_Anim_4D05E0(AnimIds::eGlukkon_Commere); return 0;
        case InputCommands::eGameSpeak3: Set_Anim_4D05E0(AnimIds::eGlukkon_StayHere); return 0;
        case InputCommands::eGameSpeak4: Set_Anim_4D05E0(AnimIds::eGlukkon_DoIt); return 0;
        case InputCommands::eGameSpeak5: Set_Anim_4D05E0(AnimIds::eGlukkon_KillEm); return 0;
        case InputCommands::eGameSpeak6: Set_Anim_4D05E0(AnimIds::eGlukkon_AllOYa); return 0;
        case InputCommands::eGameSpeak7: Set_Anim_4D05E0(AnimIds::eGlukkon_Help); return 0;
        case InputCommands::eGameSpeak8: Set_Anim_4D05E0(AnimIds::eGlukkon_Laugh); return 0;
        case InputCommands::eBack: Set_Anim_4D05E0(AnimIds::eGlukkon_Laugh); return 0x20002;
        }
        return 0;
    });
}

void MainMenuController::t_Unload_Glukkon_Speak_4D3560()
{
    Unload_Resource(field_F4_resources.field_0_resources[MenuResIds::eGlukkonSpeak]);
    Load_AbeSpeakResources();
    Set_Anim_4D05E0(AnimIds::eAbe_Idle);
}

void MainMenuController::t_Load_Glukkon_Speak_4D3480()
{
    Unload_AbeSpeakResources();

    ResourceManager::LoadResourceFile_49C170("GLKSPEAK.BAN", nullptr);
    field_F4_resources.field_0_resources[MenuResIds::eGlukkonSpeak] = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 150, TRUE, FALSE);
    field_20_animation.Set_Animation_Data_409C80(0x14EE50, field_F4_resources.field_0_resources[MenuResIds::eGlukkonSpeak]);
    Set_Anim_4D05E0(AnimIds::eGlukkon_Idle);
}

void MainMenuController::t_Render_Scrab_Speak_4D2410(int** /*ot*/)
{
    NOT_IMPLEMENTED();
}

signed int MainMenuController::t_Input_Scrab_Speak_4D3A60(DWORD input_held)
{
    return HandleGameSpeakInput(input_held, [&](InputCommands cmd)
    {
        switch (cmd)
        {
        case InputCommands::eGameSpeak1: Set_Anim_4D05E0(AnimIds::eScrab_ShredPower); return 0;
        case InputCommands::eGameSpeak2: Set_Anim_4D05E0(AnimIds::eScrab_Howl); return 0;
        case InputCommands::eGameSpeak3: return 0; // Scrabs don't have much to say.
        case InputCommands::eGameSpeak4: return 0;
        case InputCommands::eGameSpeak5: return 0;
        case InputCommands::eGameSpeak6: return 0;
        case InputCommands::eGameSpeak7: return 0;
        case InputCommands::eGameSpeak8: return 0;
        case InputCommands::eBack: Set_Anim_4D05E0(AnimIds::eScrab_ShredPower); return 0x30002;
        }
        return 0;
    });
}

void MainMenuController::t_Unload_Scrab_Speak_4D3950()
{
    Unload_Resource(field_F4_resources.field_0_resources[MenuResIds::eScrabSpeak]);
    Load_AbeSpeakResources();
    Set_Anim_4D05E0(AnimIds::eAbe_Idle);
}

void MainMenuController::t_Load_Scrab_Speak_4D3870()
{
    Unload_AbeSpeakResources();

    ResourceManager::LoadResourceFile_49C170("SCRSPEAK.BAN", nullptr);
    field_F4_resources.field_0_resources[MenuResIds::eScrabSpeak] = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 170, TRUE, FALSE);
    field_20_animation.Set_Animation_Data_409C80(0x7B8BC, field_F4_resources.field_0_resources[MenuResIds::eScrabSpeak]);
    Set_Anim_4D05E0(AnimIds::eScrab_Idle);
}

void MainMenuController::t_Render_Paramite_Speak_4D2460(int** /*ot*/)
{
    NOT_IMPLEMENTED();
}

signed int MainMenuController::t_Input_Paramite_Speak_4D3D60(DWORD input_held)
{
    return HandleGameSpeakInput(input_held, [&](InputCommands cmd)
    {
        switch (cmd)
        {
        case InputCommands::eGameSpeak1: Set_Anim_4D05E0(AnimIds::eParamite_Howdy); return 0;
        case InputCommands::eGameSpeak2: Set_Anim_4D05E0(AnimIds::eParamite_Cmon); return 0;
        case InputCommands::eGameSpeak3: Set_Anim_4D05E0(AnimIds::eParamite_Stay); return 0;
        case InputCommands::eGameSpeak4: Set_Anim_4D05E0(AnimIds::eParamite_DoIt); return 0;
        case InputCommands::eGameSpeak5: Set_Anim_4D05E0(AnimIds::eParamite_Attack); return 0;
        case InputCommands::eGameSpeak6: Set_Anim_4D05E0(AnimIds::eParamite_AllAYa); return 0;
        case InputCommands::eGameSpeak7: return 0; // Paramites don't have as much to say.
        case InputCommands::eGameSpeak8: return 0;
        case InputCommands::eBack: Set_Anim_4D05E0(AnimIds::eParamite_Attack); return 0x40002;
        }
        return 0;
    });
}

void MainMenuController::t_Unload_Paramite_Speak_4D3C50()
{
    Unload_Resource(field_F4_resources.field_0_resources[MenuResIds::eParamiteSpeak]);
    Load_AbeSpeakResources();
    Set_Anim_4D05E0(AnimIds::eAbe_Idle);
}

void MainMenuController::t_Load_Paramite_Speak_4D3B70()
{
    Unload_AbeSpeakResources();

    ResourceManager::LoadResourceFile_49C170("PARSPEAK.BAN", nullptr);
    field_F4_resources.field_0_resources[MenuResIds::eParamiteSpeak] = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 160, TRUE, FALSE);
    field_20_animation.Set_Animation_Data_409C80(0xD8E80, field_F4_resources.field_0_resources[MenuResIds::eParamiteSpeak]);
    Set_Anim_4D05E0(AnimIds::eParamite_Idle);
}

signed int MainMenuController::t_Input_Gamespeak_4D1FC0(DWORD input_held)
{
    field_230_fmv_level_index = 0;

    if (input_held & InputCommands::eBack)
    {
        return 1;
    }

    if (!(input_held & InputCommands::eUnPause))
    {
        return 0;
    }

    switch (field_1FC_button_index)
    {
    case 0:
        return 0xFFFF000E;
        break;
    case 1:
        return 0xFFFF000F;
        break;
    case 2:
        return 0xFFFF0010;
        break;
    case 3:
        return 0xFFFF0011;
        break;
    case 4:
        return 0xFFFF0012;
        break;
    default:
        return 0;
    }
}

void MainMenuController::RenderOnScreenTextHelper(int ** ot, MainMenuText * menuTextArray, int count)
{
    int polyIndex = 0;

    for (int i = 0; i < count; i++)
    {
        MainMenuController::DrawMenuText_4D20D0(&menuTextArray[i], ot, &field_120_font, &polyIndex, 1);
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

void MainMenuController::t_Render_All_Text_4D24F0(int ** ot)
{
    RenderOnScreenTextHelper(ot, sMTxt_Gamespeak_5623B8, 2);
}

void MainMenuController::t_Render_All_Text_4D2540(int ** ot)
{
    RenderOnScreenTextHelper(ot, sMTxt_Gamespeak_5623E8, 2);
}

void MainMenuController::tGame_BackStory_Or_NewGame_Render_4D2630(int ** ot)
{
    RenderOnScreenTextHelper(ot, sMTxt_NewGame_562598, 2);
}

signed int MainMenuController::Page_Front_Update_4D0720(DWORD input)
{
#if DEVELOPER_MODE
    static bool first = true;
    if (first && gBootToLoadScreen)
    {
        first = false;
        // Force enter pressed
        input |= eUnPause;
        // Force load game selected
        field_1FC_button_index = 2;
    }

#endif
    // Reset time out if any input detected
    if (sInputObject_5BD4E0.field_0_pads[0].field_0_pressed)
    {
        field_1F8_page_timeout = 0;
        word_5C1B9A = 0;
    }

    // Go to loading a demo screen if no input after time out, after one demo plays the next time out is lower if input isn't pressed
    if (field_1F8_page_timeout > (word_5C1B9A != 0 ? 300 : 1500))
    {
        word_5C1B9A = 1;
        field_1FC_button_index = 0;
        return 0xFFFF0016;
    }

    // Enter pressed on selected menu item?
    if (input & eUnPause)
    {
        if (field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak])
        {
            Set_Anim_4D05E0(AnimIds::eAbe_OK);
        }

        switch (field_1FC_button_index)
        {
        case 0:
            // Begin
            return 12;
        case 1:
            // Quit
            Set_Anim_4D05E0(AnimIds::eAbe_GoodBye);
            return 11;
        case 2:
            // Load
            field_230_fmv_level_index = 0;
            return 0xFFFF0004;
        case 3:
            // Options
            return 3;
        case 4:
            // Game speak
            field_230_fmv_level_index = 0;
            return 2;
        }
    }

    if (sEnableCheatFMV_5C1BEC)
    {
        // To FMV list menu
        sEnableCheatFMV_5C1BEC = 0;
        field_25C = 1;
        pDemosOrFmvs_BB4414 = sFmvs_561540;
        sMenuItemCount_561538 = 28;
        field_230_fmv_level_index = 0;
        field_250 = 0;
        field_254 = 0;
        field_258 = 0;
        return 0xFFFF0006;
    }
    
    if (sEnableCheatLevelSelect_5C1BEE)
    {
        // To level select menu
        sEnableCheatLevelSelect_5C1BEE = 0;
        field_25E = 1;
        pDemosOrFmvs_BB4414 = gPerLvlData_561700;
        sMenuItemCount_561538 = 15;
        field_230_fmv_level_index = 0;
        field_250 = 0;
        field_254 = 0;
        field_258 = 0;
        return 0xFFFF001F;
    }

    return 0;
}

void MainMenuController::Page_Front_Render_4D24B0(int** ot)
{
    int notUsed = 0;
    DrawMenuText_4D20D0(&sMMT_FrontPage_5623A0, ot, &field_120_font, &notUsed, 1);
}

ALIVE_VAR(1, 0xbb43dc, short, word_BB43DC, 0);
ALIVE_VAR(1, 0x5c1b88, int, dword_5C1B88, 0);

signed int MainMenuController::tLoad_New_Game_Input_4D0920(DWORD /*input*/)
{
    // TODO: De-dupe the big parts of duplicated code in here
    if (field_23C_T80.Get(Flags::eBit21))
    {
        if (field_23C_T80.Get(Flags::eBit18))
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
                pPauseMenu_5C9300 = alive_new<PauseMenu>();
                if (pPauseMenu_5C9300)
                {
                    pPauseMenu_5C9300->ctor_48FB80();
                }
            }

            if (sActiveHero_5C1B68 == spAbe_554D5C)
            {
                sActiveHero_5C1B68 = alive_new<Abe>();
                if (sActiveHero_5C1B68)
                {
                    sActiveHero_5C1B68->ctor_44AD10(58808, 85, 57, 55);
                }
            }

            if (field_208_transition_obj)
            {
                field_208_transition_obj->field_6_flags.Set(BaseGameObject::eDead);
            }

            if (field_20C)
            {
                field_20C->field_6_flags.Set(BaseGameObject::eDead);
            }

            if (field_210_pUnknown)
            {
                field_210_pUnknown->field_6_flags.Set(BaseGameObject::eDead);
            }

            field_6_flags.Set(BaseGameObject::eDead);
            
            sActiveHero_5C1B68->field_B8_xpos = FP_FromInteger(0);
            sActiveHero_5C1B68->field_BC_ypos = FP_FromInteger(0);
            
            Quicksave_LoadActive_4C9170();

            return 0;
        }

        field_23C_T80.Set(Flags::eBit18);
        return 0;
    }

    if (!field_23C_T80.Get(Flags::eBit18))
    {
        field_23C_T80.Set(Flags::eBit18);
        return 0;
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
        pPauseMenu_5C9300 = alive_new<PauseMenu>();
        if (pPauseMenu_5C9300)
        {
            pPauseMenu_5C9300->ctor_48FB80();
        }
    }

    if (sActiveHero_5C1B68 == spAbe_554D5C)
    {
        sActiveHero_5C1B68 = alive_new<Abe>();
        if (sActiveHero_5C1B68)
        {
            sActiveHero_5C1B68->ctor_44AD10(58808, 85, 57, 55);
        }
    }

    if (field_23C_T80.Get(Flags::eBit25))
    {
        field_23C_T80.Clear(Flags::eBit25);

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
        sActiveHero_5C1B68->field_F8 = FP_FromInteger(1400);
    }

    if (field_208_transition_obj)
    {
        field_208_transition_obj->field_6_flags.Set(BaseGameObject::eDead);
    }

    if (field_20C)
    {
        field_20C->field_6_flags.Set(BaseGameObject::eDead);
    }

    if (field_210_pUnknown)
    {
        field_210_pUnknown->field_6_flags.Set(BaseGameObject::eDead);
    }

    field_6_flags.Set(BaseGameObject::eDead);

    return 0;
}

EXPORT signed int MainMenuController::tGame_BackStory_Or_NewGame_Input_4D1C60(DWORD input_held)
{
    if (input_held & 0x100000) // TODO: start button ?
    {
        if (field_1FC_button_index == 0) // Back story
        {
            FmvInfo* pFmvRecord = Path_Get_FMV_Record_460F70(LevelIds::eMenu_0, 4u);
            while (Input_IsVKPressed_4EDD40(VK_RETURN))
            {
                SYS_EventsPump_494580();
            }

            DWORD fmvSector = 0;
            Get_fmvs_sectors_494460(pFmvRecord->field_0_pName, nullptr, nullptr, &fmvSector, 0, 0);
            sLevelId_dword_5CA408 = 0;

            auto pMovie = alive_new<Movie>();
            if (pMovie)
            {
                pMovie->ctor_4DFDE0(
                    pFmvRecord->field_4_id,
                    input_held,
                    pFmvRecord->field_6_flags & 1,
                    pFmvRecord->field_8,
                    pFmvRecord->field_A_volume);
            }

            while (sMovie_ref_count_BB4AE4)
            {
                if (pMovie->field_6_flags.Get(BaseGameObject::eUpdatable))
                {
                    if (!(pMovie->field_6_flags.Get(BaseGameObject::eDead) && (!sNum_CamSwappers_5C1B66 || (pMovie->field_6_flags.Get(BaseGameObject::eUpdateDuringCamSwap)))))
                    {
                        pMovie->VUpdate();
                    }
                }
            }

            gPsxDisplay_5C1130.PutCurrentDispEnv_41DFA0();
            pScreenManager_5BB5F4->DecompressToVRam_40EF60((WORD**)gMap_5C3030.field_2C_5C305C_camera_array[0]->field_C_pCamRes);
            pScreenManager_5BB5F4->MoveImage_40EB70();
            pScreenManager_5BB5F4->field_40_flags |= 0x10000; // Render enable flag
            SND_Restart_4CB0E0();
            field_1FC_button_index = 1; // Select start game
            return 0;
        }
        else if (field_1FC_button_index == 1) // Start game
        {
            sCurrentControllerIndex_5C1BBE = 0;
            dword_5C1B88 = sGnFrame_5C1B84;
            return 0xFFFF000D;
        }
    }
    else if (input_held & 0x200000) // Escape/back
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
        field_23C_T80.Clear(Flags::eBit25);
        return 1;
    }

    return 0;
}

void MainMenuController::tGame_BackStory_Or_NewGame_Load_4D1BA0()
{
    ResourceManager::FreeResource_49C330(field_F4_resources.field_0_resources[MenuResIds::eAbeIntro]);
    field_F4_resources.field_0_resources[MenuResIds::eAbeIntro] = nullptr;
    word_BB43DC = 0;
}

void MainMenuController::tGame_BackStory_Or_NewGame_Unload_4D1BE0()
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


signed int MainMenuController::tsub_LoadSave_Input_4D1040(DWORD /*input*/)
{
    NOT_IMPLEMENTED();

    return 0;
}

signed int MainMenuController::tsub_4D0E10(DWORD /*input*/)
{
    NOT_IMPLEMENTED();

    return 0;
}

ALIVE_VAR(1, 0xbb43f0, FP, sSaveGameTextYPos_BB43F0, {});
ALIVE_VAR(1, 0xbb43e8, int, sSelectedSavedGameIdx_BB43E8, 0);

EXPORT signed int MainMenuController::tLoadGame_Input_4D3EF0(DWORD input)
{
    bool indexChanged = false;

    // Escape ?
    if (input & 0x200000)
    {
        // Go back to start page
        field_23C_T80.Clear(Flags::eBit21);
        field_23A = 0;
        return 0x20001;
    }
    // Up a single save
    else if (input & eUp)
    {
        if (sSelectedSavedGameIdx_BB43E8 > 0 && !sSaveGameTextYPos_BB43F0.fpValue)
        {
            sSelectedSavedGameIdx_BB43E8--;
            indexChanged = true;
        }
    }
    // Down a single save
    else if (input & eDown)
    {
        if (sSelectedSavedGameIdx_BB43E8 < sTotalSaveFilesCount_BB43E0 - 1 && !sSaveGameTextYPos_BB43F0.fpValue)
        {
            sSelectedSavedGameIdx_BB43E8++;
            indexChanged = true;
        }
    }
    else if (input & 0x20000000)
    {
        // Page up underflow
        if (sSelectedSavedGameIdx_BB43E8 >= 3 && !sSaveGameTextYPos_BB43F0.fpValue)
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
    else if (input & 0x40000000)
    {
        // Page down overflow
        if (sSelectedSavedGameIdx_BB43E8 < sTotalSaveFilesCount_BB43E0 - 3 && !sSaveGameTextYPos_BB43F0.fpValue)
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
        SFX_Play_46FBA0(0x34u, 35, 400, 0x10000);
    }

    if (input & eUnPause)
    {
        // No save to load, go back
        if (sTotalSaveFilesCount_BB43E0 == 0)
        {
            // Go back to start page
            field_23C_T80.Clear(Flags::eBit21);
            field_23A = 0;
            return 0x20001;
        }

        // Load selected save
        char filename[40] = {};
        strcpy(filename, sSaveFileRecords_BB31D8[sSavedGameToLoadIdx_BB43FC].field_0_fileName);
        strcat(filename, ".sav");

        FILE* hFile = fopen_520C64(filename, "rb");
        if (!hFile)
        {
            return 0;
        }
        fread_520B5C(&sActiveQuicksaveData_BAF7F8, sizeof(Quicksave), 1u, hFile);
        fclose_520CBE(hFile);

        field_23C_T80.Set(Flags::eBit21);
        return 0xFFFF000D;
    }
    else
    {
        return 0;
    }
}

MainMenuText sLoadButtonGraphics[2] =
{
    {32, 27, "x", 3u, 0u, 0u, 0u,  0.75, 0u, 0u, 0u, 0u},
    { 331, 204, "esc", 3u, 0u, 0u, 0u,  0.75, 0u, 0u, 0u, 0u }
};

ALIVE_VAR(1, 0xbb43e4, FP, dword_BB43E4, {});

void MainMenuController::tLoadGame_Render_4D44D0(int** pOt)
{
    NOT_IMPLEMENTED();

    // TODO: Animated moving of menu text
    // this code is crazy and probably easier to just re-do from scratch

    /*
    int maxSaveIdx; // esi
    signed int start; // ebx
    signed int recIdx; // ecx
    __int16 textWidth; // ax
    signed int textX; // ebp
    int ypos; // ebx
    int textY; // ebx
    int v18; // eax
    int **v19; // ebx
    int v20; // eax
    int end; // [esp+18h] [ebp-8h]
    int i; // [esp+1Ch] [ebp-4h]

    maxSaveIdx = sSelectedSavedGameIdx_BB43E8; // the selected index with the input
    FP textRowsHeight = sSaveGameTextYPos_BB43F0;
    
    int polyIdx = 0;

    if (sSelectedSavedGameIdx_BB43E8 != sSavedGameToLoadIdx_BB43FC && sSaveGameTextYPos_BB43F0 > FP_FromInteger(0))
    {
        maxSaveIdx = sSavedGameToLoadIdx_BB43FC;
        sSelectedSavedGameIdx_BB43E8 = sSavedGameToLoadIdx_BB43FC;
        goto LABEL_4;
    }

    if (sSelectedSavedGameIdx_BB43E8 > sSavedGameToLoadIdx_BB43FC)
    {
        textRowsHeight = FP_FromInteger(26);
        dword_BB43E4 = FP_FromDouble(4.5);
        sSavedGameToLoadIdx_BB43FC = sSelectedSavedGameIdx_BB43E8;
        sSaveGameTextYPos_BB43F0 = textRowsHeight - FP_FromDouble(4.5);

        if (sSaveGameTextYPos_BB43F0 > FP_FromInteger(0))
        {
            dword_BB43E4 = FP_FromDouble(4.5) - FP_FromDouble(0.2);
            if (dword_BB43E4 > FP_FromInteger(0))
            {
                goto LABEL_18;
            }
            else
            {
                dword_BB43E4 = FP_FromInteger(0);
                goto LABEL_18;
            }
        }
        else
        {
            sSaveGameTextYPos_BB43F0 = FP_FromInteger(0);
            start = -2;
            end = 2;
            goto render_text_rows;
        }
    }

    if (sSelectedSavedGameIdx_BB43E8 >= sSavedGameToLoadIdx_BB43FC)
    {
    LABEL_4:
        if (sSaveGameTextYPos_BB43F0 < FP_FromInteger(0))
        {
            goto LABEL_6;
        }

        if (sSaveGameTextYPos_BB43F0 <= FP_FromInteger(0))
        {
            if (sSaveGameTextYPos_BB43F0 < FP_FromInteger(0))
            {
                start = -2;
                end = 3;
                goto render_text_rows;
            }
            else
            {
                start = -2;
                end = 2;
                goto render_text_rows;
            }
        }

        sSaveGameTextYPos_BB43F0 = textRowsHeight - dword_BB43E4;
        if (sSaveGameTextYPos_BB43F0 > FP_FromInteger(0))
        {
            dword_BB43E4 = dword_BB43E4 - FP_FromDouble(0.2);
            if (dword_BB43E4 > FP_FromInteger(0))
            {
                goto LABEL_18;
            }
            else
            {
                dword_BB43E4 = FP_FromInteger(0);
                goto LABEL_18;
            }
        }
        else
        {
            sSaveGameTextYPos_BB43F0 = FP_FromInteger(0);
            start = -2;
            end = 2;
            goto render_text_rows;
        }
    }

    textRowsHeight = FP_FromInteger(-26);
    dword_BB43E4 = FP_FromDouble(4.5);
    sSavedGameToLoadIdx_BB43FC = sSelectedSavedGameIdx_BB43E8;

LABEL_6:
    sSaveGameTextYPos_BB43F0 = FP_FromDouble(4.5) + textRowsHeight;

    if (sSaveGameTextYPos_BB43F0 >= FP_FromInteger(0))
    {
        sSaveGameTextYPos_BB43F0 = FP_FromInteger(0);
        start = -2;
        end = 2;
        goto render_text_rows;
    }

    dword_BB43E4 = FP_FromDouble(4.5) - FP_FromDouble(0.2);
    if (dword_BB43E4 > FP_FromInteger(0))
    {
        goto LABEL_18;
    }
    else
    {
        dword_BB43E4 = FP_FromInteger(0);
    }

LABEL_18:

    if (sSaveGameTextYPos_BB43F0 <= FP_FromInteger(0))
    {
        if (sSaveGameTextYPos_BB43F0 > FP_FromInteger(0))
        {
            start = -2;
            end = 3;
        }
        else
        {
            start = -2;
            end = 2;
        }
    }
    else
    {
        start = -3;
        end = 2;
    }

render_text_rows:
    */
    int start = -3;
    int end = 2;
    int i = 0;
    int maxSaveIdx = 2;
    int polyIdx = 0;

    sSavedGameToLoadIdx_BB43FC = sSelectedSavedGameIdx_BB43E8;

    for (i = start; start <= end; i = start)
    {
        int recIdx = maxSaveIdx + start;
        if (maxSaveIdx + start >= 0 && recIdx < sTotalSaveFilesCount_BB43E0)
        {
            SaveFileRec* pSaveRec = &sSaveFileRecords_BB31D8[recIdx];
            field_234_pStr = pSaveRec->field_0_fileName;

            int textWidth = (short)field_120_font.MeasureWidth_4336C0(sSaveFileRecords_BB31D8[recIdx].field_0_fileName, FP_FromInteger(1));
            int textX;
            if (textWidth >= 336)
            {
                textX = 16;
            }
            else
            {
                textX = (368 - textWidth) / 2;
            }
            int ypos = (FP_GetExponent(sSaveGameTextYPos_BB43F0 + FP_FromDouble(0.5))) + (26 * start) + 120;
            int textY = ypos + -7  / 1 - 1;
            int v18;
            if (i) // Draw as selected idx
            {
                v18 = field_120_font.DrawString_4337D0(
                    pOt,
                    this->field_234_pStr,
                    (short)textX,
                    (short)textY,
                    0,
                    1,
                    0,
                    32,
                    210,
                    150,
                    80,
                    polyIdx,
                    FP_FromInteger(1),
                    640,
                    0);
            }
            else
            {
                v18 = field_120_font.DrawString_4337D0(
                    pOt,
                    this->field_234_pStr,
                    (short)textX,
                    (short)textY,
                    0,
                    1,
                    0,
                    32,
                    255,
                    218,
                    140,
                    polyIdx,
                    FP_FromInteger(1),
                    640,
                    0);
            }
            
            v18 = field_120_font.DrawString_4337D0(
                pOt,
                this->field_234_pStr,
                (short)(textX + 2),
                (short)(textY + 2),
                0,
                1,
                0,
                32,
                0,
                0,
                0,
                v18,
                FP_FromInteger(1),
                640,
                0);
                
            maxSaveIdx = sSelectedSavedGameIdx_BB43E8;
            start = i;
            polyIdx = v18;
        }
        ++start;
    }

    if (sTotalSaveFilesCount_BB43E0 <= 0) // max save files count
    {
        int v5 = 0;
        auto v20 = field_120_font.DrawString_4337D0(
            pOt,
            "No Saved Games",
            120,
            110,
            0,
            1,
            0,
            32,
            255,
            218,
            140,
            v5,
            FP_FromInteger(1),
            640,
            0);

        polyIdx = field_120_font.DrawString_4337D0(
            pOt,
            "No Saved Games",
            122,
            112,
            0,
            1,
            0,
            32,
            0,
            0,
            0,
            v20,
            FP_FromInteger(1),
            640,
            0);
    }

    for (auto& menuGraphic : sLoadButtonGraphics)
    {
        DrawMenuText_4D20D0(&menuGraphic, pOt, &field_120_font, &polyIdx, 1);
    }
}

void MainMenuController::tLoadGame_Load_4D42F0()
{
    field_23A = 6;
    field_230_fmv_level_index = 0;
    field_1FC_button_index = -1;
    Quicksave_FindSaves_4D4150();
    sSelectedSavedGameIdx_BB43E8 = sSavedGameToLoadIdx_BB43FC;
    field_23C_T80.Clear(Flags::eBit15);
    field_1F4_credits_next_frame = 0;
}

EXPORT void sub_4A2D40()
{
    NOT_IMPLEMENTED();
}

void MainMenuController::tLoadGame_Unload_4D4360()
{
    field_23C_T80.Clear(Flags::eBit15);

    sub_4A2D40();

    if (field_23C_T80.Get(Flags::eBit21))
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

signed int MainMenuController::HandleGameSpeakInput(DWORD input_held, std::function<signed int(InputCommands cmd)> fnOnGameSpeak)
{
    field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
    field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);

    if (field_230_fmv_level_index == 8)
    {
        return 0;
    }

    if (Input_IsChanting_45F260())
    {
        field_230_fmv_level_index = 1;
        return fnOnGameSpeak(InputCommands::eChant);
    }
    // Hi
    else if (input_held & InputCommands::eGameSpeak1) // 0x400
    {
        field_230_fmv_level_index = 0;
        return fnOnGameSpeak(InputCommands::eGameSpeak1);
    }
    // Git 'im
    else if (input_held & InputCommands::eGameSpeak4) // 0x2000
    {
        field_230_fmv_level_index = 1;
        return fnOnGameSpeak(InputCommands::eGameSpeak4);
    }
    // Freeze
    else if (input_held & InputCommands::eGameSpeak3) // 0x1000
    {
        field_230_fmv_level_index = 2;
        return fnOnGameSpeak(InputCommands::eGameSpeak3);
    }
    // Here boy
    else if (input_held & InputCommands::eGameSpeak2) // 0x800
    {
        field_230_fmv_level_index = 3;
        return fnOnGameSpeak(InputCommands::eGameSpeak2);
    }
    // Bs
    else if (input_held & InputCommands::eGameSpeak6) // 0x8000
    {
        field_230_fmv_level_index = 4;
        return fnOnGameSpeak(InputCommands::eGameSpeak6);
    }
    // Look out
    else if (input_held & InputCommands::eGameSpeak7) // 0x10000
    {
        field_230_fmv_level_index = 5;
        return fnOnGameSpeak(InputCommands::eGameSpeak7);
    }
    // S'mo bs
    else if (input_held & InputCommands::eGameSpeak5) // 0x4000
    {
        field_230_fmv_level_index = 6;
        return fnOnGameSpeak(InputCommands::eGameSpeak5);
    }
    // Laugh
    else if (input_held & InputCommands::eGameSpeak8) // 0x20000
    {
        field_230_fmv_level_index = 7;
        return fnOnGameSpeak(InputCommands::eGameSpeak8);
    }
    else if (input_held & InputCommands::eBack) // 0x200000
    {
        // Exit
      
        field_230_fmv_level_index = 8;
        field_1FC_button_index = -1;

        if (field_210_pUnknown)
        {
            // TODO: Recover type
            WORD* pUnknown = (WORD *)field_210_pUnknown;
            pUnknown[124] = 1;
            field_210_pUnknown = nullptr;
        }

        return fnOnGameSpeak(InputCommands::eBack);
    }
    else
    {
        if (field_210_pUnknown)
        {
            // TODO: Recover type
            WORD* pUnknown = (WORD *)field_210_pUnknown;
            pUnknown[124] = 1;
            field_210_pUnknown = nullptr;
        }
        return 0;
    }
}

void MainMenuController::HandleCreditsControllerUpdate()
{
    if (sInputObject_5BD4E0.field_0_pads[0].field_0_pressed & 0x200000)
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
        if (gMap_5C3030.sCurrentPathId_5C3032 == 2)
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
    if (gMap_5C3030.sCurrentCamId_5C3034 == 5 || gMap_5C3030.sCurrentCamId_5C3034 == 11 || gMap_5C3030.sCurrentCamId_5C3034 == 4)
    {
        MusicController::sub_47FD60(4, this, 0, 0);
    }
    else if (gMap_5C3030.sCurrentCamId_5C3034 == 12 || gMap_5C3030.sCurrentCamId_5C3034 == 13)
    {
        MusicController::sub_47FD60(8, this, 0, 0);
    }
    else
    {
        MusicController::sub_47FD60(0, this, 0, 0);
    }

    UpdateHighliteGlow_4D0630();

    if (sub_4CF640())
    {
        sub_4CFE80();
        return;
    }

    sub_4CFE80();

    if (field_23C_T80.Get(Flags::eBit17_bDisableChangingSelection))
    {
        return;
    }

    const auto currentCamId = sMainMenuPages_561960[field_214_page_index].field_0_cam_id;
    if (sInputObject_5BD4E0.field_0_pads[0].field_0_pressed && currentCamId != 25  && currentCamId != 20 && currentCamId != 23)
    {
        field_1F8_page_timeout = 0;
    }
    else
    {
        ++field_1F8_page_timeout;
    }

    auto v8 = 0;

    if (sMainMenuPages_561960[field_214_page_index].field_4_time_out <= 0 ||
        sMainMenuPages_561960[field_214_page_index].field_8_next_idx <= 0 ||
        field_1F8_page_timeout <= sMainMenuPages_561960[field_214_page_index].field_4_time_out)
    {
        auto pageBtns = sMainMenuPages_561960[field_214_page_index].field_18_buttons;
        auto inputHeld = sInputObject_5BD4E0.field_0_pads[0].field_C_held;

        if (pageBtns)
        {
            auto btnArray = sMainMenuPages_561960[field_214_page_index].field_18_buttons;

            if (field_1FC_button_index != -1)
            {
                if (inputHeld & (eLeft | eUp))
                {
                    if (sMainMenuPages_561960[field_214_page_index].field_0_cam_id != 4)
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

                    field_158_animation.Set_Animation_Data_409C80(
                        sMainMenuPages_561960[field_214_page_index].field_18_buttons[field_1FC_button_index].field_8_anim_frame_offset, 0);

                    SFX_Play_46FBA0(0x34u, 35, 400, 0x10000);
                }

                if (inputHeld & (eRight | eDown))
                {
                    if (sMainMenuPages_561960[field_214_page_index].field_0_cam_id != 4)
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

                    field_158_animation.Set_Animation_Data_409C80(
                        sMainMenuPages_561960[field_214_page_index].field_18_buttons[field_1FC_button_index].field_8_anim_frame_offset,
                        0);
                    SFX_Play_46FBA0(0x34u, 35, 400, 0x10000);
                }
            }
        }

        if (field_21E_bChangeScreen)
        {
            return;
        }
        
        if (field_23C_T80.Get(Flags::eBit22))
        {
            return;
        }

        if (!sMainMenuPages_561960[field_214_page_index].field_10_fn_update)
        {
            return;
        }

        // Todo: change all field_10_fn_update function types to unsigned.
        const unsigned int pageUpdateRet = (this->*(sMainMenuPages_561960[field_214_page_index].field_10_fn_update))(inputHeld);

        if (pageUpdateRet <= 0 || (pageUpdateRet & 0xFF) == static_cast<unsigned int>(gMap_5C3030.sCurrentCamId_5C3034))
        {
            return;
        }

        field_218_target_page_index = static_cast<short>(GetPageIndexFromCam_4D05A0(pageUpdateRet & 0xFF));

        // The return variable of page update seems to have multiple bits of data masked.
        auto v19 = (pageUpdateRet >> 16) & 0xFF;
        field_21A_target_cam = static_cast<short>(v19);
        if (v19 == 255)
        {
            field_21A_target_cam = -1;
        }
        v8 = BYTE1(pageUpdateRet);
    }
    else
    {
        field_1F8_page_timeout = 0;
        field_218_target_page_index = static_cast<short>(GetPageIndexFromCam_4D05A0(sMainMenuPages_561960[field_214_page_index].field_8_next_idx));
        field_21A_target_cam = sMainMenuPages_561960[field_214_page_index].field_C_target_camera;
        v8 = sMainMenuPages_561960[field_214_page_index].field_A_transistion_effect;
    }

    field_21C_bDoScreenTransistionEffect = static_cast<short>(v8);
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

void MainMenuController::Load_Anim_Pal_4D06A0(AnimationEx* pAnim)
{
    // TODO: If this is duplicated a lot it can be moved to a helper
    const FrameInfoHeader* pFrameInfo = pAnim->Get_FrameHeader_40B730(0);
    const FrameHeader* pFrameHeader = reinterpret_cast<const FrameHeader*>(&(*pAnim->field_20_ppBlock)[pFrameInfo->field_0_frame_header_offset]);
    pAnim->Load_Pal_40A530(pAnim->field_20_ppBlock, pFrameHeader->field_0_clut_offset);
}

ALIVE_VAR(1, 0x5ca408, DWORD, sLevelId_dword_5CA408, 0);

signed int MainMenuController::sub_4CF640()
{
    NOT_IMPLEMENTED();

    if (field_21E_bChangeScreen == 0 || field_23C_T80.Get(Flags::eBit22) || field_228_res_idx != 0)
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

            field_208_transition_obj = alive_new<MainMenuTransition>();
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

            field_208_transition_obj = alive_new<MainMenuTransition>();
            if (field_208_transition_obj)
            {
                field_208_transition_obj->ctor_464110(40, 1, 0, 16, 1);
                field_21E_bChangeScreen = 2;
                return 1;
            }
        }
        // alive_new failure case
        field_21E_bChangeScreen = 2;
        return 1;

    case 2:
        if (sMainMenuPages_561960[field_214_page_index].field_A_transistion_effect == 7)
        {
            char buffer[256] = {};

            // Use path 2
            strcpy(buffer, sCdEmu_Path2_C144C0);
            strcat(buffer, "movies");

            WIN32_FIND_DATA sFindData = {};
            HANDLE hFind = FindFirstFile(buffer, &sFindData);
            if (hFind == INVALID_HANDLE_VALUE)
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
                hFind = FindFirstFile(buffer, &sFindData);
                if (hFind != INVALID_HANDLE_VALUE)
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
                Display_Full_Screen_Message_Blocking_465820(1, 3);
            }

            if (hFind != INVALID_HANDLE_VALUE)
            {
                FindClose(hFind);
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
            auto pMovie = alive_new<Movie>();
            if (pMovie)
            {
                pMovie->ctor_4DFDE0(
                    pFmvRecord->field_4_id,
                    v34,
                    pFmvRecord->field_6_flags & 1,
                    pFmvRecord->field_8,
                    pFmvRecord->field_A_volume);
            }

            // Run the movie till its done
            while (sMovie_ref_count_BB4AE4 > 0)
            {
                if (pMovie->field_6_flags.Get(BaseGameObject::eUpdatable))
                {
                    if (pMovie->field_6_flags.Get(BaseGameObject::eDead) == false && (!sNum_CamSwappers_5C1B66 || pMovie->field_6_flags.Get(BaseGameObject::eUpdateDuringCamSwap)))
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
            pMovie = alive_new<Movie>();

            if (pMovie)
            {
                pMovie->ctor_4DFDE0(
                    pFmvRecord->field_4_id,
                    v34,
                    pFmvRecord->field_6_flags & 1,
                    pFmvRecord->field_8,
                    pFmvRecord->field_A_volume);
            }

            // Run the movie till its done
            while (sMovie_ref_count_BB4AE4 > 0)
            {
                if (pMovie->field_6_flags.Get(BaseGameObject::eUpdatable))
                {
                    if (pMovie->field_6_flags.Get(BaseGameObject::eDead) == false && (!sNum_CamSwappers_5C1B66 || pMovie->field_6_flags.Get(BaseGameObject::eUpdateDuringCamSwap)))
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
        if (sNum_CamSwappers_5C1B66 > 0)
        {
            return 1;
        }

        if (field_21C_bDoScreenTransistionEffect == 0 || field_21C_bDoScreenTransistionEffect == 2)
        {
            field_208_transition_obj->StartTrans_464370(40, 0, 0, 16);
        }

        field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
        field_216 = field_214_page_index;
        field_214_page_index = field_218_target_page_index;
        field_21E_bChangeScreen = 5;

        if (sMainMenuPages_561960[field_218_target_page_index].field_18_buttons == 0)
        {
            goto LABEL_76;
        }
        
        field_1FC_button_index = field_21A_target_cam;

        if (field_21A_target_cam != -1)
        {
            goto LABEL_74;
        }

        if (sMainMenuPages_561960[field_218_target_page_index].field_18_buttons->field_0 == 1)
        {
            field_1FC_button_index = 0;
            goto LABEL_74;
        }
        break;
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

    /*
    while (1)
    {
        ++v29;
        ++buttons;
        if (!v30)
        {
            break;
        }
        v30 = buttons->field_0;
        if (v30 == 1)
        {
        LABEL_73:
            this->field_1FC_button_index = v29;
            break;
        }
    }*/

LABEL_74:
    if (field_1FC_button_index != -1)
    {
        field_158_animation.Set_Animation_Data_409C80(
            sMainMenuPages_561960[field_218_target_page_index].field_18_buttons[field_218_target_page_index].field_8_anim_frame_offset,
            nullptr);
    }

LABEL_76:
    if (sMainMenuPages_561960[field_214_page_index].field_1C_fn_on_load)
    {
        (this->*sMainMenuPages_561960[field_214_page_index].field_1C_fn_on_load)();
    }
    return 1;
}



void MainMenuController::sub_4CFE80()
{
    NOT_IMPLEMENTED();

    if (!sMainMenuPages_561960[field_214_page_index].field_E_show_character)
    {
        field_22C_T80 = 0;
        return;
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame) && !field_22C_T80)
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
            if (field_224 <= static_cast<int>(sGnFrame_5C1B84))
            {
                Set_Anim_4D05E0(2, 0);
                field_224 = sGnFrame_5C1B84 + Math_RandomRange_496AB0(120, 450);
            }
            break;

        case 9: // ??
            field_6_flags.Clear(BaseGameObject::eUpdateDuringCamSwap);
            field_22C_T80 = 15;
            
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
            if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 130, 0, 0))
            {
                // TODO: Fix the types
                ResourceManager::LoadResourceFile_49C130("ABESPEAK.BAN",
                    reinterpret_cast<ResourceManager::TLoaderFn>(callback_4D06E0),
                    reinterpret_cast<Camera *>(this), nullptr);
            }
            field_23C_T80.Clear(Flags::eBit17_bDisableChangingSelection);
            field_224 = sGnFrame_5C1B84 + Math_RandomRange_496AB0(300, 450);
            break;

        case AnimIds::eAbe_Chant:
            if (Input_IsChanting_45F260())
            {
                Set_Anim_4D05E0(0);
            }
            else
            {
                SND_SEQ_Stop_4CAE60(0xAu);
                /*
                if (field_20C)
                {
                    *(WORD *)(field_20C + 248) = 1;
                }
                field_20C = 0;

                if (field_210_pUnknown)
                {
                    field_210_pUnknown[124] = 1;
                }
                field_210_pUnknown = 0;
                */

                field_23C_T80.Clear(Flags::eBit24_Chant_Seq_Playing);
                Set_Anim_4D05E0(13);
            }
            break;

        case AnimIds::eSlig_Idle:
            break;

        case AnimIds::eGlukkon_Idle:
            if (field_224 <= static_cast<int>(sGnFrame_5C1B84))
            {
                Set_Anim_4D05E0(28); // ??
                field_224 = sGnFrame_5C1B84 + Math_RandomRange_496AB0(120, 450);
            }
            break;

        case AnimIds::eScrab_Idle:
            if (field_224 <= static_cast<int>(sGnFrame_5C1B84))
            {
                Set_Anim_4D05E0(38); // ??
                field_224 = sGnFrame_5C1B84 + Math_RandomRange_496AB0(120, 450);
            }
            break;

        case AnimIds::eParamite_Idle:
            if (field_224 <= static_cast<int>(sGnFrame_5C1B84))
            {
                Set_Anim_4D05E0(42); // ??
                field_224 = sGnFrame_5C1B84 + Math_RandomRange_496AB0(120, 450);
            }
            break;

        default:
            field_23C_T80.Clear(Flags::eBit22);
            break;
        }
    }
    
    if (sMainMenuFrameTable_561CC8[field_220_frame_table_idx].field_4_menu_res_id == 10
        // TODO: What the ?
        && field_20_animation.field_92_current_frame == *((unsigned __int8 *)&sMainMenuPages_561960[22].field_1C_fn_on_load
            + field_220_frame_table_idx
            + 1)
        && field_220_frame_table_idx != eParamite_Idle
        && field_220_frame_table_idx != 42)
    {
        if (field_220_frame_table_idx == 48)
        {
            SFX_SfxDefinition_Play_4CA700(&stru_55D7C0[sMainMenuFrameTable_561CC8[field_220_frame_table_idx].field_6_sound], 127, 127, 64, 64);
        }
        else
        {
            if (sMainMenuFrameTable_561CC8[field_220_frame_table_idx].field_6_sound == 9)
            {
                SND_SEQ_Play_4CAB10(0x14u, 1, 127, 127);
            }
            else
            {
                SFX_SfxDefinition_Play_4CA700(&stru_55D7C0[sMainMenuFrameTable_561CC8[field_220_frame_table_idx].field_6_sound], 127, 127, 0x7FFF, 0x7FFF);
            }
        }
    }

    if (field_22C_T80)
    {
        field_22C_T80 = field_22C_T80 - 1;
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
        if (!field_22C_T80)
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
                            Abe_SFX_457EC0(static_cast<BYTE>(sMainMenuFrameTable_561CC8[field_228_res_idx].field_6_sound), 0, 0, nullptr);
                            //BYTE2(this->field_23C_T80) |= 0x20u; // Gamespeak sound playing flag
                            break;

                        case eSligSpeak:
                            //sub_4C04F0(sMainMenuFrameTable_561CC8[field_228_res_idx].field_6_sound, 0, 0, 0);
                            //BYTE2(this->field_23C_T80) |= 0x20u;
                            break;

                        case eGlukkonSpeak:
                            //Glukkon::sub_444AF0(sMainMenuFrameTable_561CC8[field_228_res_idx].field_6_sound, 0, 0, 0);
                            //BYTE2(this->field_23C_T80) |= 0x20u;
                            break;

                        case eScrabSpeak:
                            /*
                            SFX_SfxDefinition_Play_4CA700(
                                &stru_560330[sound],
                                (char)stru_560330[sound].field_3_default_volume,
                                (char)stru_560330[sound].field_3_default_volume,
                                0x7FFF,
                                0x7FFF);
                            */
                            //BYTE2(this->field_23C_T80) |= 0x20u;
                            break;

                        case eParamiteSpeak:
                            //BYTE2(this->field_23C_T80) |= 0x20u;
                            break;

                        default:
                            break;
                        }
                    }

                    if (field_228_res_idx > 0)
                    {
                        if (field_228_res_idx == eAbe_Chant)
                        {
                            SND_SEQ_PlaySeq_4CA960(10, 0, 1);
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
                            SND_SEQ_Stop_4CAE60(10);
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
            ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, kAbespeakResID, TRUE, FALSE);
}

const char byte_55EE00[2] = { '\x18', '\0' };
const char byte_55EDEC[2] = { '\x19', '\0' };

void MainMenuController::DrawMenuText_4D20D0(MainMenuText* array, int** ot, Alive::Font* font, int* polyIndex, char op2)
{
    const bool bCancel = strstr(array->field_8_text, byte_55EE00) != 0; // cancel
    const bool bEndOfMedium = strstr(array->field_8_text, byte_55EDEC) != 0; // end of medium

    char textBuffer[32] = {};
    String_FormatString_4969D0(array->field_8_text, textBuffer, _countof(textBuffer), array->field_14 == 0);
    
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

    field_F4_resources.field_0_resources[MenuResIds::eAbeSpeak] = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 130, TRUE, FALSE);
}