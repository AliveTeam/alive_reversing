#include "stdafx_ao.h"
#include "Function.hpp"
#include "Input.hpp"
#include "Midi.hpp"
#include "PauseMenu.hpp"
#include "../relive_lib/Primitives.hpp"
#include "Psx.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "ResourceManager.hpp"
#include "SaveGame.hpp"
#include "StringFormatters.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "Sound.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Sfx.hpp"
#include "Sys.hpp"
#include "Map.hpp"
#include "GameAutoPlayer.hpp"

#if ORIGINAL_PS1_BEHAVIOR
    #include "../AliveLibAE/Sys.hpp"
#endif

namespace AO {

ALIVE_VAR(1, 0x5080E0, PauseMenu*, pPauseMenu_5080E0, nullptr);

const u8 byte_4C5EE8[32] = {
    0u,
    0u,
    33u,
    132u,
    66u,
    136u,
    99u,
    140u,
    132u,
    144u,
    165u,
    20u,
    231u,
    28u,
    8u,
    33u,
    41u,
    37u,
    74u,
    41u,
    107u,
    45u,
    140u,
    49u,
    173u,
    53u,
    239u,
    61u,
    16u,
    66u,
    115u,
    78u};


PauseMenu::PauseMenu()
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::ePauseMenu);
    SetUpdateDelay(25);

    mBaseGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);
    mBaseGameObjectFlags.Set(BaseGameObject::eSurviveDeathReset_Bit9);

    gObjListDrawables->Push_Back(this);
    field_E4_font.Load(175, byte_4C5EE8, &sFontContext_4FFD68);
    field_130 = 0;
    field_11C = 0;
    sDisableFontFlicker_5080E4 = FALSE;
}

PauseMenu::~PauseMenu()
{
    mBaseGameObjectFlags.Clear(Options::eDrawable_Bit4);
    gObjListDrawables->Remove_Item(this);
}

void PauseMenu::VScreenChanged()
{
    if (gMap.mNextLevel == EReliveLevelIds::eCredits)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

ALIVE_VAR(1, 0x9F1188, s16, word_9F1188, 0);
ALIVE_VAR(1, 0x504620, s16, word_504620, 0);
ALIVE_VAR(1, 0x504622, s16, word_504622, 0);
ALIVE_VAR(1, 0x9F0E60, u16, word_9F0E60, 0);
ALIVE_VAR(1, 0x504624, u16, word_504624, 0);
ALIVE_VAR(1, 0x504626, u16, word_504626, 0);

s16 Reset_Unknown_45A5B0()
{
    word_9F1188 = -1;
    word_504620 = -1;
    word_504622 = -1;
    word_9F0E60 = 0;
    word_504624 = 0;
    word_504626 = 0;
    return 1;
}

struct saveName final
{
    char_type characters[26];
};
ALIVE_VAR(1, 0x5080C6, saveName, saveNameBuffer_5080C6, {});

const char_type* gLevelNames_4CE1D4[20] = {
    "¸",
    "RuptureFarms",
    "Monsaic Lines",
    "Paramonia",
    "Paramonian Temple",
    "Stockyard Escape",
    "Stockyards",
    "",
    "Scrabania",
    "Scrabanian Temple",
    "",
    "",
    "The Boardroom",
    "RuptureFarms II",
    "Paramonian Nest",
    "Scrabanian Nest",
    "Rescue Zulag 1",
    "Rescue Zulag 2",
    "Rescue Zulag 3",
    "Rescue Zulag 4"};

enum PauseMenuPages
{
    ePause_0 = 0,
    eSave_1 = 1,
    eControls_2 = 2,
    eQuit_3 = 3
};

void PauseMenu::VUpdate()
{
    if (Input().IsAnyHeld(InputCommands::ePause))
    {
        SND_StopAll_4762D0();
        SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
        mBaseGameObjectFlags.Set(Options::eDrawable_Bit4);
        field_11C = 1;
        field_124 = 0;
        field_126_page = PauseMenuPages::ePause_0;
        Reset_Unknown_45A5B0();
        field_132_always_0 = 0;
        field_11E_selected_glow = 52;
        field_120_selected_glow_counter = 8;

        // This is bad, let's nuke it later :)
        while (1)
        {
            sDisableFontFlicker_5080E4 = 1;
            SYS_EventsPump_44FF90();

            for (s32 idx = 0; idx < gObjListDrawables->Size(); idx++)
            {
                auto pObjIter = gObjListDrawables->ItemAt(idx);
                if (!pObjIter)
                {
                    break;
                }
                if (!pObjIter->mBaseGameObjectFlags.Get(Options::eDead))
                {
                    if (pObjIter->mBaseGameObjectFlags.Get(Options::eDrawable_Bit4))
                    {
                        pObjIter->VRender(gPsxDisplay.mDrawEnvs[gPsxDisplay.mBufferIndex].mOrderingTable);
                    }
                }
            }
            pScreenManager->VRender(
                gPsxDisplay.mDrawEnvs[gPsxDisplay.mBufferIndex].mOrderingTable);
            PSX_DrawSync_496750(0);
            gPsxDisplay.RenderOrderingTable();
            Input().Update(GetGameAutoPlayer());

            if (field_120_selected_glow_counter > 0)
            {
                field_11E_selected_glow += 8;
            }

            if (field_11E_selected_glow <= 100 || field_120_selected_glow_counter <= 0)
            {
                if (field_120_selected_glow_counter <= 0)
                {
                    field_11E_selected_glow -= 8;
                    if (field_11E_selected_glow < 52)
                    {
                        field_120_selected_glow_counter = -field_120_selected_glow_counter;
                        field_11E_selected_glow += field_120_selected_glow_counter;
                    }
                }
            }
            else
            {
                field_120_selected_glow_counter = -field_120_selected_glow_counter;
                field_11E_selected_glow += field_120_selected_glow_counter;
            }

            enum Page1Selectables
            {
                eContinue_0 = 0,
                eSave_1 = 1,
                eControls_2 = 2,
                eQuit_3 = 3
            };

            switch (field_126_page)
            {
                case PauseMenuPages::ePause_0:
                {
                    if (Input().IsAnyHeld(InputCommands::eCheatMode | InputCommands::eDown))
                    {
                        field_124++;
                        if (field_124 > 3)
                        {
                            field_124 = 0;
                        }
                        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
                    }

                    if (Input().IsAnyHeld(InputCommands::eUp))
                    {
                        field_124--;
                        if (field_124 < 0)
                        {
                            field_124 = 3;
                        }
                        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
                    }

#if ORIGINAL_PS1_BEHAVIOR // OG Change - Pause Menu controls like PS1
                    if (Input().IsAnyHeld(InputCommands::ePause))
                    {
                        field_11C = 0;
                        SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
                        SND_Restart();
                        break;
                    }

                    const bool optionClicked = Input().IsAnyHeld(InputCommands::eUnPause_OrConfirm);
#else
                    const bool optionClicked = Input().IsAnyHeld(
                        InputCommands::eHop | InputCommands::eThrowItem | InputCommands::eUnPause_OrConfirm | InputCommands::eDoAction | InputCommands::eBack);
#endif
                    if (optionClicked)
                    {
                        switch (field_124)
                        {
                            case Page1Selectables::eContinue_0:
                            {
                                field_11C = 0;
                                SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
                                SND_Restart();
                                break;
                            }
                            case Page1Selectables::eSave_1:
                            {
                                field_126_page = PauseMenuPages::eSave_1;
                                field_12C = 0;
                                field_12E = 0;
                                field_134 = 1;
                                SfxPlayMono(relive::SoundEffects::IngameTransition, 90);
                                s32 tmp = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));
                                if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
                                {
                                    s16 row = 0;
                                    auto pathId = SaveGame::GetPathId(gMap.mCurrentPath, &row);

                                    if (pathId != -1)
                                    {
                                        tmp += row + 3;
                                    }
                                }

                                auto curPathId = gMap.mCurrentPath;
                                char_type curPathIdNumBuf[12] = {};

                                strncpy(&saveNameBuffer_5080C6.characters[2], gLevelNames_4CE1D4[tmp], 19);
                                if (tmp != 12 && tmp != 14 && tmp != 15)
                                {
                                    strcat(&saveNameBuffer_5080C6.characters[2], " ");
                                    if (strlen(&saveNameBuffer_5080C6.characters[2]) < 18)
                                    {
                                        strcat(&saveNameBuffer_5080C6.characters[2], "p");
                                    }
                                    sprintf(curPathIdNumBuf, "%d", curPathId);
                                    strncat(&saveNameBuffer_5080C6.characters[2], curPathIdNumBuf, 19u);
                                }

                                const char_type aux[2] = {18, 0};
                                strncat(&saveNameBuffer_5080C6.characters[2], aux, 19u);
#if ORIGINAL_PS1_BEHAVIOR // OG Change - Allow for exiting save menu using controller
                                setSaveMenuOpen(true); // Sets saveMenuOpen bool to true, instead of disabling input
#else
                                Input_DisableInput();
#endif
                                break;
                            }
                            case Page1Selectables::eControls_2:
                            {
                                field_126_page = PauseMenuPages::eControls_2;
                                field_128_controller_id = 0;
                                SfxPlayMono(relive::SoundEffects::IngameTransition, 90);
                                break;
                            }
                            case Page1Selectables::eQuit_3:
                            {
                                field_126_page = PauseMenuPages::eQuit_3;
                                field_124 = 0;
                                SfxPlayMono(relive::SoundEffects::IngameTransition, 90);
                                break;
                            }
                            default:
                            {
                                break;
                            }
                        }
                        break;
                    }
                    break;
                }
                case PauseMenuPages::eSave_1:
                {
                    if (field_12C)
                    {
                        if (field_12C == 4)
                        {
                            if (field_134)
                            {
                                field_134 = 0;
                            }
                            else
                            {
                                SaveGame::SaveToFile(&saveNameBuffer_5080C6.characters[2]);
                                field_12C = 5;
                                field_12A = 13;
                                field_122 = 120;
                            }
                        }
                        else if (field_12C == 5)
                        {
                            field_11C = 0;
                            SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
                            SND_Restart();
                        }
                        break;
                    }

                    auto last_pressed = static_cast<char_type>(Input_GetLastPressedKey());
                    char_type lastPressedKeyNT[2] = {last_pressed, 0};

#if ORIGINAL_PS1_BEHAVIOR // OG Change - Exit save menu using controller
                    if (last_pressed == VK_ESCAPE || last_pressed == VK_RETURN) // Keyboard ESC or ENTER
                    {
                        setSaveMenuOpen(false);
                    }
                    else if (Input().IsAnyHeld(InputCommands::eBack)) // Triangle
                    {
                        last_pressed = VK_ESCAPE;
                        setSaveMenuOpen(false);
                    }
                    else if (Input().IsAnyHeld(InputCommands::eUnPause_OrConfirm)) // Cross or Start
                    {
                        last_pressed = VK_RETURN;
                        setSaveMenuOpen(false);
                    }
#endif

                    if (!last_pressed)
                    {
                        break;
                    }
                    auto string_len_no_nullterminator = strlen(&saveNameBuffer_5080C6.characters[2]);
                    switch (last_pressed)
                    {
                        case VK_ESCAPE:
                        {
                            SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
                            field_126_page = 0;
                            Input_Reset();
                            break;
                        }
                        case VK_RETURN:
                        {
                            if (string_len_no_nullterminator <= 1)
                            {
                                SfxPlayMono(relive::SoundEffects::ElectricZap, 0);
                                break;
                            }
                            SfxPlayMono(relive::SoundEffects::IngameTransition, 90);
                            saveNameBuffer_5080C6.characters[string_len_no_nullterminator + 1] = 0;
                            field_12C = 4;
                            field_12A = 11;
                            field_134 = 1;
                            Input_Reset();
                            break;
                        }
                        case VK_BACK:
                        {
                            if (string_len_no_nullterminator <= 1)
                            {
                                SfxPlayMono(relive::SoundEffects::ElectricZap, 0);
                                break;
                            }
                            saveNameBuffer_5080C6.characters[string_len_no_nullterminator] = 18;
                            saveNameBuffer_5080C6.characters[string_len_no_nullterminator + 1] = 0;
                            SfxPlayMono(relive::SoundEffects::PickupItem, 0);
                            break;
                        }
                        default:
                        {
                            if (strspn(lastPressedKeyNT, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 !-"))
                            {
                                if (lastPressedKeyNT[0] != 32 || (string_len_no_nullterminator != 1 && saveNameBuffer_5080C6.characters[string_len_no_nullterminator] != lastPressedKeyNT[0]))
                                {
                                    if (string_len_no_nullterminator > 19)
                                    {
                                        SfxPlayMono(relive::SoundEffects::SackWobble, 0);
                                    }
                                    else
                                    {
                                        saveNameBuffer_5080C6.characters[string_len_no_nullterminator + 1] = lastPressedKeyNT[0];
                                        saveNameBuffer_5080C6.characters[string_len_no_nullterminator + 2] = 18;
                                        saveNameBuffer_5080C6.characters[string_len_no_nullterminator + 3] = 0;
                                        SfxPlayMono(relive::SoundEffects::RockBounce, 0);
                                    }
                                }
                                else
                                {
                                    SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 30, 2600);
                                }
                            }
                            else
                            {
                                SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 70, 2200);
                            }
                            break;
                        }
                    }
                    break;
                }
                case PauseMenuPages::eControls_2:
                {
                    if (Input().IsAnyHeld(InputCommands::eBack | InputCommands::eHop))
                    {
                        field_126_page = 0;
                        SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
                    }

                    if (Input().IsAnyHeld(
                            InputCommands::eThrowItem | InputCommands::eUnPause_OrConfirm | InputCommands::eDoAction | InputCommands::eCheatMode | InputCommands::eUp | InputCommands::eRight | InputCommands::eDown | InputCommands::eLeft))
                    {
                        field_128_controller_id++;
                        if (field_128_controller_id < 2)
                        {
                            SfxPlayMono(relive::SoundEffects::IngameTransition, 90);
                        }
                        else
                        {
                            field_128_controller_id = 0;
                            field_126_page = PauseMenuPages::ePause_0;
                            SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
                        }
                    }
                    break;
                }
                case PauseMenuPages::eQuit_3:
                {
                    if (Input().IsAnyHeld(InputCommands::eBack | InputCommands::eHop))
                    {
                        field_126_page = 0;
                        SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
                    }

                    if (Input().IsAnyHeld(InputCommands::eThrowItem | InputCommands::eUnPause_OrConfirm | InputCommands::eDoAction))
                    {
                        field_11C = 0;
                        SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
                        if (pPauseMenu_5080E0 && pPauseMenu_5080E0 == this)
                        {
                            pPauseMenu_5080E0->mBaseGameObjectFlags.Set(Options::eDead);
                        }
                        else
                        {
                            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                        }
                        pPauseMenu_5080E0 = 0;
                        gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eMainMenu_1, CameraSwapEffects::eInstantChange_0, 0, 0);
                        gMap.field_DC_free_all_anim_and_palts = 1;
                        Input().SetCurrentController(InputObject::PadIndex::First);
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }

            if (!field_11C)
            {
                Input().Update(GetGameAutoPlayer());
                mBaseGameObjectFlags.Clear(Options::eDrawable_Bit4);
                break;
            }
        }

        sDisableFontFlicker_5080E4 = 0;
    }
}

ALIVE_VAR(1, 0xA88B90, s8, byte_A88B90, 0);

PauseMenu::PauseEntry pauseEntries_4CDE50[6] = {
    {184, 85, "CONTINUE", 128u, 16u, 255u, '\x01'},
    {184, 110, "SAVE", 128u, 16u, 255u, '\x01'},
    {184, 135, "CONTROLS", 128u, 16u, 255u, '\x01'},
    {184, 160, "QUIT", 128u, 16u, 255u, '\x01'},
    {184, 42, "- paused -", 228u, 116u, 99u, '\x01'},
    {0, 0, nullptr, 0u, 0u, 0u, '\0'}};

PauseMenu::PauseEntry PauseEntry2_4CDE98[2] = {
    {184, 205, "r1p01c01", 128u, 16u, 255u, '\x01'},
    {0, 0, nullptr, 0u, 0u, 0u, '\0'}};

PauseMenu::PauseEntry quitEntries_4CDEA8[3] = {
    {184, 110, "REALLY QUIT?", 128u, 16u, 255u, '\x01'},
    {184, 135, kAO_ConfirmContinue " yes   " kAO_Esc " no", 160u, 160u, 160u, '\x01'},
    {0, 0, nullptr, 0u, 0u, 0u, '\0'}};

PauseMenu::PauseEntry saveEntries_4CDED0[4] = {
    {184, 120, "DUMMY_TEXT", 128u, 16u, 255u, '\x01'},
    {184, 180, "enter   save", 160u, 160u, 160u, '\x01'},
    {184, 205, "esc   cancel", 160u, 160u, 160u, '\x01'},
    {0, 0, nullptr, 0u, 0u, 0u, '\0'}};

PauseMenu::PauseEntry controlsPageOne_4CDF00[17] = {
    {184, 205, kAO_ConfirmContinue " more  " kAO_Esc " exit", 128u, 16u, 255u, '\x01'},
    {184, 20, "Actions", 127u, 127u, 127u, '\x01'},
    {80, 50, kAO_Run " + " kAO_Left " " kAO_Right, 160u, 160u, 160u, '\0'},
    {80, 70, kAO_Sneak " + " kAO_Left " " kAO_Right, 160u, 160u, 160u, '\0'},
    {80, 90, kAO_Jump_Or_Hello " " kAO_Or " " kAO_Up, 160u, 160u, 160u, '\0'},
    {80, 110, kAO_Crouch " " kAO_Or " " kAO_Down, 160u, 160u, 160u, '\0'},
    {80, 130, kAO_Throw " + " kAO_DirectionalButtons, 160u, 160u, 160u, '\0'},
    {80, 150, kAO_Action, 160u, 160u, 160u, '\0'},
    {80, 170, kAO_Up, 160u, 160u, 160u, '\0'},
    {200, 50, "run", 128u, 16u, 255u, '\0'},
    {200, 70, "sneak", 128u, 16u, 255u, '\0'},
    {200, 90, "jump", 128u, 16u, 255u, '\0'},
    {200, 110, "crouch", 128u, 16u, 255u, '\0'},
    {200, 130, "throw", 128u, 16u, 255u, '\0'},
    {200, 150, "action", 128u, 16u, 255u, '\0'},
    {200, 170, "mount " kAO_Or " zturn", 128u, 16u, 255u, '\0'},
    {0, 0, nullptr, 0u, 0u, 0u, '\0'}};

PauseMenu::PauseEntry gamepadGameSpeak_4CDFD0[21] = {
    {184, 205, kAO_Esc " exit", 128u, 16u, 255u, '\x01'},
    {184, 20, "GameSpeak", 127u, 127u, 127u, '\x01'},
    {184, 55, kAO_Speak1 " + " kAO_Speak2, 160u, 160u, 160u, '\x01'},
    {184, 75, "chant", 128u, 16u, 255u, '\x01'},
    {100, 104, "hello", 128u, 16u, 255u, '\0'},
    {100, 126, "angry", 128u, 16u, 255u, '\0'},
    {100, 148, "wait", 128u, 16u, 255u, '\0'},
    {100, 170, "follow me", 128u, 16u, 255u, '\0'},
    {290, 104, "whistle ", 128u, 16u, 255u, '\0'},
    {290, 126, "fart", 128u, 16u, 255u, '\0'},
    {290, 148, "whistle ", 128u, 16u, 255u, '\0'},
    {290, 170, "laugh", 128u, 16u, 255u, '\0'},
    {2, 104, kAO_Speak1 "+" kAO_Jump_Or_Hello, 160u, 160u, 160u, '\0'},
    {2, 126, kAO_Speak1 "+" kAO_Throw, 160u, 160u, 160u, '\0'},
    {2, 148, kAO_Speak1 "+" kAO_Crouch, 160u, 160u, 160u, '\0'},
    {2, 170, kAO_Speak1 "+" kAO_Action, 160u, 160u, 160u, '\0'},
    {192, 104, kAO_Speak2 "+" kAO_Jump_Or_Hello, 160u, 160u, 160u, '\0'},
    {192, 126, kAO_Speak2 "+" kAO_Throw, 160u, 160u, 160u, '\0'},
    {192, 148, kAO_Speak2 "+" kAO_Crouch, 160u, 160u, 160u, '\0'},
    {192, 170, kAO_Speak2 "+" kAO_Action, 160u, 160u, 160u, '\0'},
    {0, 0, nullptr, 0u, 0u, 0u, '\0'}};

PauseMenu::PauseEntry keyboardGameSpeak_4CE0D0[21] = {
    {184, 205, kAO_Esc " exit", 128u, 16u, 255u, '\x01'},
    {184, 20, "GameSpeak", 127u, 127u, 127u, '\x01'},
    {184, 55, "0", 160u, 160u, 160u, '\x01'},
    {184, 75, "chant", 128u, 16u, 255u, '\x01'},
    {90, 104, "hello", 128u, 16u, 255u, '\0'},
    {90, 126, "follow me", 128u, 16u, 255u, '\0'},
    {90, 148, "wait", 128u, 16u, 255u, '\0'},
    {90, 170, "angry", 128u, 16u, 255u, '\0'},
    {240, 104, "laugh", 128u, 16u, 255u, '\0'},
    {240, 126, "whistle ", 128u, 16u, 255u, '\0'},
    {240, 148, "fart", 128u, 16u, 255u, '\0'},
    {240, 170, "whistle ", 128u, 16u, 255u, '\0'},
    {52, 104, "1", 160u, 160u, 160u, '\0'},
    {52, 126, "2", 160u, 160u, 160u, '\0'},
    {52, 148, "3", 160u, 160u, 160u, '\0'},
    {52, 170, "4", 160u, 160u, 160u, '\0'},
    {202, 104, "5", 160u, 160u, 160u, '\0'},
    {202, 126, "6", 160u, 160u, 160u, '\0'},
    {202, 148, "7", 160u, 160u, 160u, '\0'},
    {202, 170, "8", 160u, 160u, 160u, '\0'},
    {0, 0, nullptr, 0u, 0u, 0u, '\0'}};

void PauseMenu::DrawEntries(PrimHeader** ppOt, PauseEntry* entry, s16 selectedEntryId, s32 polyOffset = 0)
{
    for (s16 entryId = 0; entry[entryId].field_4_strBuf; ++entryId)
    {
        s16 colourOffset;
        if (entryId == selectedEntryId && (field_126_page != 1 || field_132_always_0))
        {
            colourOffset = field_11E_selected_glow;
        }
        else
        {
            colourOffset = 0;
        }
        const char_type* stringBuffer;
        if (&entry[entryId] == &saveEntries_4CDED0[0])
        {
            stringBuffer = &saveNameBuffer_5080C6.characters[2];
        }
        else
        {
            stringBuffer = entry[entryId].field_4_strBuf;
        }
        if (!stringBuffer)
        {
            break;
        }
        char_type formattedString[128] = {};
        String_FormatString(stringBuffer, formattedString);
        s16 clampedFontWidth;
        if (entry[entryId].field_B == 1)
        {
            s16 font_width_2 = static_cast<s16>(field_E4_font.MeasureTextWidth(formattedString));
            clampedFontWidth = font_width_2 >= 608 ? 16 : (entry[entryId].x - font_width_2 / 2);
        }
        else
        {
            clampedFontWidth = entry[entryId].x;
        }
        polyOffset = field_E4_font.DrawString(
            ppOt,
            formattedString,
            clampedFontWidth,
            entry[entryId].y,
            TPageAbr::eBlend_0,
            1,
            0,
            Layer::eLayer_Menu_41,
            static_cast<u8>(colourOffset + entry[entryId].mRed),
            static_cast<u8>(colourOffset + entry[entryId].mGreen),
            static_cast<u8>(colourOffset + entry[entryId].mBlue),
            polyOffset,
            FP_FromInteger(1),
            640,
            0);
    }
    Poly_F4* pPrim = &field_158[gPsxDisplay.mBufferIndex];
    PolyF4_Init(pPrim);
    Poly_Set_SemiTrans(&pPrim->mBase.header, 1);
    Poly_Set_Blending(&pPrim->mBase.header, 0);
    u8 color = 0x64;
    if (field_126_page != PauseMenuPages::ePause_0)
    {
        color = 160;
    }
    SetRGB0(pPrim, color, color, color);
    SetXY0(pPrim, 0, 0);
    SetXY1(pPrim, 640, 0);
    SetXY2(pPrim, 0, 240);
    SetXY3(pPrim, 640, 240);
    Prim_SetTPage* prim_tpage = &field_138_tPage[gPsxDisplay.mBufferIndex];
    Init_SetTPage(prim_tpage, 0, 0, PSX_getTPage(TPageMode::e4Bit_0, TPageAbr::eBlend_2, 0, 0));
    OrderingTable_Add(OtLayer(ppOt, Layer::eLayer_Menu_41), &pPrim->mBase.header);
    OrderingTable_Add(OtLayer(ppOt, Layer::eLayer_Menu_41), &prim_tpage->mBase);
}

void PauseMenu::VRender(PrimHeader** ppOt)
{
    switch (field_126_page)
    {
        case PauseMenuPages::ePause_0:
        case PauseMenuPages::eQuit_3:
        {
            PauseEntry* entries = &quitEntries_4CDEA8[0];
            if (field_126_page != PauseMenuPages::eQuit_3)
            {
                entries = &pauseEntries_4CDE50[0];
            }
            char_type cameraNameBuffer[48] = {};
            Path_Format_CameraName_4346B0(
                cameraNameBuffer,
                gMap.mCurrentLevel,
                gMap.mCurrentPath,
                gMap.mCurrentCamera);
            cameraNameBuffer[8] = 0;
            if (strlen(cameraNameBuffer) != 0)
            {
                for (unsigned idx = 0; idx < strlen(cameraNameBuffer); idx++)
                {
                    s8 letter = cameraNameBuffer[idx];
                    s8 letterCandidate = 0;
                    if (letter < 'A' || letter > 'Z')
                    {
                        if (letter < '0' || letter > '9')
                        {
                            continue;
                        }
                        letterCandidate = letter - 26;
                    }
                    else
                    {
                        letterCandidate = letter | ' ';
                    }
                    cameraNameBuffer[idx] = letterCandidate;
                }
            }
            auto polyOffset = field_E4_font.DrawString(
                ppOt,
                cameraNameBuffer,
                static_cast<s16>(PauseEntry2_4CDE98[0].x - field_E4_font.MeasureTextWidth(cameraNameBuffer) / 2),
                PauseEntry2_4CDE98[0].y,
                TPageAbr::eBlend_0,
                1,
                0,
                Layer::eLayer_Menu_41,
                128,
                16,
                255,
                0,
                FP_FromInteger(1),
                640,
                0);
            DrawEntries(ppOt, entries, field_124, polyOffset);
            break;
        }
        case PauseMenuPages::eSave_1:
        {
            DrawEntries(ppOt, &saveEntries_4CDED0[0], -1);
            break;
        }
        case PauseMenuPages::eControls_2:
        {
            PauseEntry* entries = nullptr;
            if (field_128_controller_id == 1)
            {
                if (Input().JoyStickEnabled())
                {
                    entries = &gamepadGameSpeak_4CDFD0[0];
                }
                else
                {
                    entries = &keyboardGameSpeak_4CE0D0[0];
                }
            }
            else
            {
                entries = &controlsPageOne_4CDF00[0] + field_128_controller_id;
            }
            DrawEntries(ppOt, entries, 0);
            break;
        }
        default:
        {
            ALIVE_FATAL("Unknown menu page!");
        }
    }
}

} // namespace AO
