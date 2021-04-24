#include "stdafx_ao.h"
#include "Function.hpp"
#include "Input.hpp"
#include "Game.hpp"
#include "Midi.hpp"
#include "PauseMenu.hpp"
#include "Primitives.hpp"
#include "Psx.hpp"
#include "PsxDisplay.hpp"
#include "ResourceManager.hpp"
#include "SaveGame.hpp"
#include "StringFormatters.hpp"
#include "ScreenManager.hpp"
#include "Sound.hpp"
#include "stdlib.hpp"
#include "Sfx.hpp"
#include "Sys.hpp"
#include "Map.hpp"

#if ORIGINAL_PS1_BEHAVIOR
#include "../AliveLibAE/Sys.hpp"
#endif

namespace AO {

ALIVE_VAR(1, 0x5080E0, PauseMenu*, pPauseMenu_5080E0, nullptr);

const BYTE byte_4C5EE8[32] =
{
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
    78u
};


PauseMenu* PauseMenu::ctor_44DEA0()
{
    ctor_417C10();
    SetVTable(this, 0x4BBD68);

    field_4_typeId = Types::ePauseMenu_61;
    field_8_update_delay = 25;

    field_6_flags.Clear(BaseGameObject::eDrawable_Bit4);
    field_6_flags.Set(BaseGameObject::eSurviveDeathReset_Bit9);

    gObjList_drawables_504618->Push_Back(this);
    field_E4_font.ctor_41C170(175, byte_4C5EE8, &sFontContext_4FFD68);
    field_130 = 0;
    field_11C = 0;
    sDisableFontFlicker_5080E4 = FALSE;
    return this;
}

BaseGameObject* PauseMenu::dtor_44DF40()
{
    SetVTable(this, 0x4BBD68);

    field_6_flags.Clear(Options::eDrawable_Bit4);
    gObjList_drawables_504618->Remove_Item(this);
    field_E4_font.dtor_41C130();

    return dtor_417D10();
}

BaseGameObject* PauseMenu::VDestructor(signed int flags)
{
    return Vdtor_44EAA0(flags);
}

PauseMenu* PauseMenu::Vdtor_44EAA0(signed int flags)
{
    dtor_44DF40();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void PauseMenu::VScreenChanged()
{
    VScreenChange_44EA90();
}

void PauseMenu::VScreenChange_44EA90()
{
    if (gMap_507BA8.field_A_level == LevelIds::eCredits_10)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void PauseMenu::VUpdate()
{
    VUpdate_44DFB0();
}

ALIVE_VAR(1, 0x9F1188, short, word_9F1188, 0);
ALIVE_VAR(1, 0x504620, short, word_504620, 0);
ALIVE_VAR(1, 0x504622, short, word_504622, 0);
ALIVE_VAR(1, 0x9F0E60, WORD, word_9F0E60, 0);
ALIVE_VAR(1, 0x504624, WORD, word_504624, 0);
ALIVE_VAR(1, 0x504626, WORD, word_504626, 0);

EXPORT signed __int16 Reset_Unknown_45A5B0()
{
    word_9F1188 = -1;
    word_504620 = -1;
    word_504622 = -1;
    word_9F0E60 = 0;
    word_504624 = 0;
    word_504626 = 0;
    return 1;
}

struct saveName
{
    char characters[26];
};
ALIVE_VAR(1, 0x5080C6, saveName, saveNameBuffer_5080C6, {});

const char *gLevelNames_4CE1D4[20] =
{
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
    "Rescue Zulag 4"
};

enum PauseMenuPages
{
    ePause_0 = 0,
    eSave_1 = 1,
    eControls_2 = 2,
    eQuit_3 = 3
};

void PauseMenu::VUpdate_44DFB0()
{
    if (Input().IsAnyHeld(InputCommands::ePause))
    {
        SND_StopAll_4762D0();
        SFX_Play_43AE60(SoundEffect::PossessEffect_21, 40, 2400, 0);
        field_6_flags.Set(Options::eDrawable_Bit4);
        field_11C = 1;
        field_124 = 0;
        field_126_page = PauseMenuPages::ePause_0;
        Reset_Unknown_45A5B0();
        field_132 = 0;
        field_11E = 52;
        field_120 = 8;

        // This is bad, let's nuke it later :)
        while (1)
        {
            sDisableFontFlicker_5080E4 = 1;
            SYS_EventsPump_44FF90();

            for(int idx = 0; idx < gObjList_drawables_504618->Size(); idx++)
            {
                auto pObjIter = gObjList_drawables_504618->ItemAt(idx);
                if (!pObjIter)
                {
                    break;
                }
                if (!pObjIter->field_6_flags.Get(Options::eDead_Bit3))
                {
                    if (pObjIter->field_6_flags.Get(Options::eDrawable_Bit4))
                    {
                        pObjIter->VRender(gPsxDisplay_504C78.field_C_drawEnv[gPsxDisplay_504C78.field_A_buffer_index].field_70_ot_buffer);
                    }
                }
            }
            pScreenManager_4FF7C8->VRender(
                gPsxDisplay_504C78.field_C_drawEnv[gPsxDisplay_504C78.field_A_buffer_index].field_70_ot_buffer
            );
            PSX_DrawSync_496750(0);
            ResourceManager::Reclaim_Memory_455660(500000);
            gPsxDisplay_504C78.PSX_Display_Render_OT_40DD20();
            Input().Update_433250();

            if (field_120 > 0)
            {
                field_11E += 8;
            }

            if (field_11E <= 100 || field_120 <= 0)
            {
                if (field_120 <= 0)
                {
                    field_11E -= 8;
                    if (field_11E < 52)
                    {
                        field_120 = -field_120;
                        field_11E += field_120;
                    }
                }
            }
            else
            {
                field_120 = -field_120;
                field_11E += field_120;
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
                        SFX_Play_43AE60(SoundEffect::MenuNavigation_61, 45, 400, 0);
                    }

                    if (Input().IsAnyHeld(InputCommands::eUp))
                    {
                        field_124--;
                        if (field_124 < 0)
                        {
                            field_124 = 3;
                        }
                        SFX_Play_43AE60(SoundEffect::MenuNavigation_61, 45, 400, 0);
                    }

                    if (Input().IsAnyHeld(
                        InputCommands::eHop |
                        InputCommands::eThrowItem |
                        InputCommands::eUnPause_OrConfirm |
                        InputCommands::eDoAction |
                        InputCommands::eBack
                    ))
                    {
                        switch (field_124)
                        {
                            case Page1Selectables::eContinue_0:
                            {
                                field_11C = 0;
                                SFX_Play_43AE60(SoundEffect::PossessEffect_21, 40, 2400, 0);
                                SND_Restart_476340();
                                break;
                            }
                            case Page1Selectables::eSave_1:
                            {
                                field_126_page = PauseMenuPages::eSave_1;
                                field_12C = 0;
                                field_12E = 0;
                                field_134 = 1;
                                SFX_Play_43AD70(SoundEffect::IngameTransition_107, 90, 0);
                                int tmp = static_cast<int>(gMap_507BA8.field_0_current_level);
                                if (gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarmsReturn_13)
                                {
                                    short row = 0;
                                    auto pathId = SaveGame::GetPathId(gMap_507BA8.field_2_current_path, &row);

                                    if (pathId != -1)
                                    {
                                        tmp += row + 3;
                                    }
                                }

                                auto curPathId = gMap_507BA8.field_2_current_path;
                                char curPathIdNumBuf[12] = {};

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

                                const char aux[2] = { 18, 0 };
                                strncat(&saveNameBuffer_5080C6.characters[2], aux, 19u);
#if ORIGINAL_PS1_BEHAVIOR  // OG Change - Allow for exiting save menu using controller
                                setSaveMenuOpen(true); // Sets saveMenuOpen bool to true, instead of disabling input
#else
                                Input_DisableInput_48E690();
#endif
                                break;
                            }
                            case Page1Selectables::eControls_2:
                            {
                                field_126_page = PauseMenuPages::eControls_2;
                                field_128_controller_id = 0;
                                SFX_Play_43AD70(SoundEffect::IngameTransition_107, 90, 0);
                                break;
                            }
                            case Page1Selectables::eQuit_3:
                            {
                                field_126_page = PauseMenuPages::eQuit_3;
                                field_124 = 0;
                                SFX_Play_43AD70(SoundEffect::IngameTransition_107, 90, 0);
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
                                SaveGame::SaveToFile_45A110(&saveNameBuffer_5080C6.characters[2]);
                                field_12C = 5;
                                field_12A = 13;
                                field_122 = 120;
                            }
                        }
                        else if (field_12C == 5)
                        {
                            field_11C = 0;
                            SFX_Play_43AE60(SoundEffect::PossessEffect_21, 40, 2400, 0);
                            SND_Restart_476340();
                        }
                        break;
                    }

                    auto last_pressed = Input_GetLastPressedKey_44F2C0();
                    char lastPressedKeyNT[2] = { last_pressed, 0 };

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
                            SFX_Play_43AE60(SoundEffect::PossessEffect_21, 40, 2400, 0);
                            field_126_page = 0;
                            Input_Reset_44F2F0();
                            break;
                        }
                        case VK_RETURN:
                        {
                            if (string_len_no_nullterminator <= 1)
                            {
                                SFX_Play_43AD70(SoundEffect::ElectricZap_46, 0, 0);
                                break;
                            }
                            SFX_Play_43AD70(SoundEffect::IngameTransition_107, 90, 0);
                            saveNameBuffer_5080C6.characters[string_len_no_nullterminator + 1] = 0;
                            field_12C = 4;
                            field_12A = 11;
                            field_134 = 1;
                            Input_Reset_44F2F0();
                            break;
                        }
                        case VK_BACK:
                        {
                            if (string_len_no_nullterminator <= 1)
                            {
                                SFX_Play_43AD70(SoundEffect::ElectricZap_46, 0, 0);
                                break;
                            }
                            saveNameBuffer_5080C6.characters[string_len_no_nullterminator] = 18;
                            saveNameBuffer_5080C6.characters[string_len_no_nullterminator + 1] = 0;
                            SFX_Play_43AD70(SoundEffect::PickupItem_33, 0, 0);
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
                                        SFX_Play_43AD70(SoundEffect::SackWobble_34, 0, 0);
                                    }
                                    else
                                    {
                                        saveNameBuffer_5080C6.characters[string_len_no_nullterminator + 1] = lastPressedKeyNT[0];
                                        saveNameBuffer_5080C6.characters[string_len_no_nullterminator + 2] = 18;
                                        saveNameBuffer_5080C6.characters[string_len_no_nullterminator + 3] = 0;
                                        SFX_Play_43AD70(SoundEffect::RockBounce_31, 0, 0);
                                    }
                                }
                                else
                                {
                                    SFX_Play_43AE60(SoundEffect::PossessEffect_21, 30, 2600, 0);
                                }
                            }
                            else
                            {
                                SFX_Play_43AE60(SoundEffect::PossessEffect_21, 70, 2200, 0);
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
                        SFX_Play_43AE60(SoundEffect::PossessEffect_21, 40, 2400, 0);
                    }

                    if (Input().IsAnyHeld(
                        InputCommands::eThrowItem |
                        InputCommands::eUnPause_OrConfirm |
                        InputCommands::eDoAction |
                        InputCommands::eCheatMode |
                        InputCommands::eUp |
                        InputCommands::eRight |
                        InputCommands::eDown |
                        InputCommands::eLeft
                    ))
                    {
                        field_128_controller_id++;
                        if (field_128_controller_id < 2)
                        {
                            SFX_Play_43AD70(SoundEffect::IngameTransition_107, 90, 0);
                        }
                        else
                        {
                            field_128_controller_id = 0;
                            field_126_page = PauseMenuPages::ePause_0;
                            SFX_Play_43AE60(SoundEffect::PossessEffect_21, 40, 2400, 0);
                        }
                    }
                    break;
                }
                case PauseMenuPages::eQuit_3:
                {
                    if (Input().IsAnyHeld(InputCommands::eBack | InputCommands::eHop))
                    {
                        field_126_page = 0;
                        SFX_Play_43AE60(SoundEffect::PossessEffect_21, 40, 2400, 0);
                    }

                    if (Input().IsAnyHeld(InputCommands::eThrowItem | InputCommands::eUnPause_OrConfirm | InputCommands::eDoAction))
                    {
                        field_11C = 0;
                        SFX_Play_43AE60(SoundEffect::PossessEffect_21, 40, 2400, 0);
                        if (pPauseMenu_5080E0 && pPauseMenu_5080E0 == this)
                        {
                            pPauseMenu_5080E0->field_6_flags.Set(Options::eDead_Bit3);
                        }
                        else
                        {
                            field_6_flags.Set(BaseGameObject::eDead_Bit3);
                        }
                        pPauseMenu_5080E0 = 0;
                        gMap_507BA8.SetActiveCam_444660(LevelIds::eMenu_0, 1, 1, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
                        gMap_507BA8.field_DC_free_all_anim_and_palts = 1;
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
                Input().Update_433250();
                field_6_flags.Clear(Options::eDrawable_Bit4);
                break;
            }
        }

        sDisableFontFlicker_5080E4 = 0;
    }
}

void PauseMenu::VRender(PrimHeader** ppOt)
{
    VRender_44E6F0(ppOt);
}

ALIVE_VAR(1, 0xA88B90, char, byte_A88B90, 0);

PauseMenu::PauseEntry pauseEntries_4CDE50[6] =
{
    { 184, 85, "CONTINUE", 128u, 16u, 255u, '\x01' },
    { 184, 110, "SAVE", 128u, 16u, 255u, '\x01' },
    { 184, 135, "CONTROLS", 128u, 16u, 255u, '\x01' },
    { 184, 160, "QUIT", 128u, 16u, 255u, '\x01' },
    { 184, 42, "- paused -", 228u, 116u, 99u, '\x01' },
    { 0, 0, nullptr, 0u, 0u, 0u, '\0' }
};

PauseMenu::PauseEntry PauseEntry2_4CDE98[2] =
{
    { 184, 205, "r1p01c01", 128u, 16u, 255u, '\x01' },
    { 0, 0, nullptr, 0u, 0u, 0u, '\0' }
};

PauseMenu::PauseEntry quitEntries_4CDEA8[3] =
{
    { 184, 110, "REALLY QUIT?", 128u, 16u, 255u, '\x01' },
    { 184, 135, kAO_ConfirmContinue " yes   " kAO_Esc " no", 160u, 160u, 160u, '\x01' },
    { 0, 0, nullptr, 0u, 0u, 0u, '\0' }
};

PauseMenu::PauseEntry saveEntries_4CDED0[4] =
{
    { 184, 120, "DUMMY_TEXT", 128u, 16u, 255u, '\x01' },
    { 184, 180, "enter   save", 160u, 160u, 160u, '\x01' },
    { 184, 205, "esc   cancel", 160u, 160u, 160u, '\x01' },
    { 0, 0, nullptr, 0u, 0u, 0u, '\0' }
};

PauseMenu::PauseEntry controlsPageOne_4CDF00[17] =
{
    { 184, 205, kAO_ConfirmContinue " more  " kAO_Esc " exit", 128u, 16u, 255u, '\x01' },
    { 184, 20, "Actions", 127u, 127u, 127u, '\x01' },
    { 80, 50, kAO_Run " + " kAO_Left " " kAO_Right, 160u, 160u, 160u, '\0' },
    { 80, 70, kAO_Sneak " + " kAO_Left " " kAO_Right, 160u, 160u, 160u, '\0' },
    { 80, 90, kAO_Jump_Or_Hello " + " kAO_Or " " kAO_Up, 160u, 160u, 160u, '\0' },
    { 80, 110, kAO_Crouch " + " kAO_Or " " kAO_Down, 160u, 160u, 160u, '\0' },
    { 80, 130, kAO_Throw " + " kAO_DirectionalButtons, 160u, 160u, 160u, '\0' },
    { 80, 150, kAO_Action, 160u, 160u, 160u, '\0' },
    { 80, 170, kAO_Up, 160u, 160u, 160u, '\0' },
    { 200, 50, "run", 128u, 16u, 255u, '\0' },
    { 200, 70, "sneak", 128u, 16u, 255u, '\0' },
    { 200, 90, "jump", 128u, 16u, 255u, '\0' },
    { 200, 110, "crouch", 128u, 16u, 255u, '\0' },
    { 200, 130, "throw", 128u, 16u, 255u, '\0' },
    { 200, 150, "action", 128u, 16u, 255u, '\0' },
    { 200, 170, "mount " kAO_Or " zturn", 128u, 16u, 255u, '\0' },
    { 0, 0, nullptr, 0u, 0u, 0u, '\0' }
};

PauseMenu::PauseEntry gamepadGameSpeak_4CDFD0[21] =
{
    { 184, 205, kAO_Esc " exit", 128u, 16u, 255u, '\x01' },
    { 184, 20, "GameSpeak", 127u, 127u, 127u, '\x01' },
    { 184, 55, kAO_Speak1 " + " kAO_Speak2, 160u, 160u, 160u, '\x01' },
    { 184, 75, "chant", 128u, 16u, 255u, '\x01' },
    { 100, 104, "hello", 128u, 16u, 255u, '\0' },
    { 100, 126, "angry", 128u, 16u, 255u, '\0' },
    { 100, 148, "wait", 128u, 16u, 255u, '\0' },
    { 100, 170, "follow me", 128u, 16u, 255u, '\0' },
    { 290, 104, "whistle ", 128u, 16u, 255u, '\0' },
    { 290, 126, "fart", 128u, 16u, 255u, '\0' },
    { 290, 148, "whistle ", 128u, 16u, 255u, '\0' },
    { 290, 170, "laugh", 128u, 16u, 255u, '\0' },
    { 2, 104, kAO_Speak1 "+" kAO_Jump_Or_Hello, 160u, 160u, 160u, '\0' },
    { 2, 126, kAO_Speak1 "+" kAO_Throw, 160u, 160u, 160u, '\0' },
    { 2, 148, kAO_Speak1 "+" kAO_Crouch, 160u, 160u, 160u, '\0' },
    { 2, 170, kAO_Speak1 "+" kAO_Action, 160u, 160u, 160u, '\0' },
    { 192, 104, kAO_Speak2 "+" kAO_Jump_Or_Hello, 160u, 160u, 160u, '\0' },
    { 192, 126, kAO_Speak2 "+" kAO_Throw, 160u, 160u, 160u, '\0' },
    { 192, 148, kAO_Speak2 "+" kAO_Crouch, 160u, 160u, 160u, '\0' },
    { 192, 170, kAO_Speak2 "+" kAO_Action, 160u, 160u, 160u, '\0' },
    { 0, 0, nullptr, 0u, 0u, 0u, '\0' }
};

PauseMenu::PauseEntry keyboardGameSpeak_4CE0D0[21] =
{
    { 184, 205, kAO_Esc " exit", 128u, 16u, 255u, '\x01' },
    { 184, 20, "GameSpeak", 127u, 127u, 127u, '\x01' },
    { 184, 55, "0", 160u, 160u, 160u, '\x01' },
    { 184, 75, "chant", 128u, 16u, 255u, '\x01' },
    { 90, 104, "hello", 128u, 16u, 255u, '\0' },
    { 90, 126, "follow me", 128u, 16u, 255u, '\0' },
    { 90, 148, "wait", 128u, 16u, 255u, '\0' },
    { 90, 170, "angry", 128u, 16u, 255u, '\0' },
    { 240, 104, "laugh", 128u, 16u, 255u, '\0' },
    { 240, 126, "whistle ", 128u, 16u, 255u, '\0' },
    { 240, 148, "fart", 128u, 16u, 255u, '\0' },
    { 240, 170, "whistle ", 128u, 16u, 255u, '\0' },
    { 52, 104, "1", 160u, 160u, 160u, '\0' },
    { 52, 126, "2", 160u, 160u, 160u, '\0' },
    { 52, 148, "3", 160u, 160u, 160u, '\0' },
    { 52, 170, "4", 160u, 160u, 160u, '\0' },
    { 202, 104, "5", 160u, 160u, 160u, '\0' },
    { 202, 126, "6", 160u, 160u, 160u, '\0' },
    { 202, 148, "7", 160u, 160u, 160u, '\0' },
    { 202, 170, "8", 160u, 160u, 160u, '\0' },
    { 0, 0, nullptr, 0u, 0u, 0u, '\0' }
};

void PauseMenu::DrawEntries(PrimHeader** ppOt, PauseEntry *entry, short selectedEntryId, int polyOffset = 0)
{
    for (short entryId = 0; entry[entryId].field_4_strBuf; ++entryId)
    {
        __int16 colourOffset;
        if (entryId == selectedEntryId && (field_126_page != 1 || field_132))
        {
            colourOffset = field_11E;
        }
        else
        {
            colourOffset = 0;
        }
        const char *stringBuffer;
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
        char formattedString[128] = {};
        String_FormatString_450DC0(stringBuffer, formattedString);
        signed __int16 clampedFontWidth;
        if (entry[entryId].field_B == 1)
        {
            __int16 font_width_2 = static_cast<short>(field_E4_font.MeasureWidth_41C2B0(formattedString));
            clampedFontWidth = font_width_2 >= 608 ? 16 : (entry[entryId].field_0_x - font_width_2 / 2);
        }
        else
        {
            clampedFontWidth = entry[entryId].field_0_x;
        }
        polyOffset = field_E4_font.DrawString_41C360(
            ppOt,
            formattedString,
            clampedFontWidth,
            entry[entryId].field_2_y,
            TPageAbr::eBlend_0,
            1,
            0,
            Layer::eLayer_41,
            static_cast<BYTE>(colourOffset + entry[entryId].field_8_r),
            static_cast<BYTE>(colourOffset + entry[entryId].field_9_g),
            static_cast<BYTE>(colourOffset + entry[entryId].field_A_b),
            polyOffset,
            FP_FromInteger(1),
            640,
            0
        );
    }
    Poly_F4 *pPrim = &field_158[gPsxDisplay_504C78.field_A_buffer_index];
    PolyF4_Init(pPrim);
    Poly_Set_SemiTrans_498A40(&pPrim->mBase.header, 1);
    Poly_Set_Blending_498A00(&pPrim->mBase.header, 0);
    BYTE color = 0x64;
    if (field_126_page != PauseMenuPages::ePause_0)
    {
        color = 160;
    }
    SetRGB0(pPrim, color, color, color);
    SetXY0(pPrim, 0, 0);
    SetXY1(pPrim, 640, 0);
    SetXY2(pPrim, 0, 240);
    SetXY3(pPrim, 640, 240);
    Prim_SetTPage *prim_tpage = &field_138_tPage[gPsxDisplay_504C78.field_A_buffer_index];
    Init_SetTPage_495FB0(prim_tpage, 0, 0, PSX_getTPage_4965D0(TPageMode::e4Bit_0, TPageAbr::eBlend_2, 0, 0));
    OrderingTable_Add_498A80(OtLayer(ppOt, Layer::eLayer_41), &pPrim->mBase.header);
    OrderingTable_Add_498A80(OtLayer(ppOt, Layer::eLayer_41), &prim_tpage->mBase);
    pScreenManager_4FF7C8->InvalidateRect_406E40(
        0,
        0,
        640,
        240,
        pScreenManager_4FF7C8->field_2E_idx
    );
}

void PauseMenu::VRender_44E6F0(PrimHeader** ppOt)
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
            char cameraNameBuffer[48] = {};
            Path_Format_CameraName_4346B0(
                cameraNameBuffer,
                gMap_507BA8.field_0_current_level,
                gMap_507BA8.field_2_current_path,
                gMap_507BA8.field_4_current_camera
            );
            cameraNameBuffer[8] = 0;
            if (strlen(cameraNameBuffer) != 0)
            {
                for(unsigned idx = 0; idx < strlen(cameraNameBuffer); idx++)
                {
                    char letter = cameraNameBuffer[idx];
                    char letterCandidate = 0;
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
            auto polyOffset = field_E4_font.DrawString_41C360(
                ppOt,
                cameraNameBuffer,
                static_cast<signed __int16>(PauseEntry2_4CDE98[0].field_0_x - field_E4_font.MeasureWidth_41C2B0(cameraNameBuffer) / 2),
                PauseEntry2_4CDE98[0].field_2_y,
                TPageAbr::eBlend_0,
                1,
                0,
                Layer::eLayer_41,
                128,
                16,
                255,
                0,
                FP_FromInteger(1),
                640,
                0
            );
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
            PauseEntry *entries = nullptr;
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

}
