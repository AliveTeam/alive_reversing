#include "stdafx_ao.h"
#include "Function.hpp"
#include "Input.hpp"
#include "Game.hpp"
#include "Midi.hpp"
#include "PauseMenu.hpp"
#include "Psx.hpp"
#include "PsxDisplay.hpp"
#include "ResourceManager.hpp"
#include "SaveGame.hpp"
#include "ScreenManager.hpp"
#include "Sound.hpp"
#include "stdlib.hpp"
#include "Sfx.hpp"
#include "Sys.hpp"
#include "Map.hpp"

START_NS_AO;

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

ALIVE_VAR(1, 0xA8A600, DWORD, sIsAKeyDown_A8A600, 0);
ALIVE_VAR(1, 0xA8A604, DWORD, sLastPressedKey_A8A604, 0);

ALIVE_ARY(1, 0x5080C6, char, 26, saveNameBuffer_5080C6, {});

EXPORT int Sys_IsAnyKeyDown_48E6C0()
{
    return sIsAKeyDown_A8A600;
}

EXPORT char Input_GetLastPressedKey_44F2C0()
{
    if (!Sys_IsAnyKeyDown_48E6C0())
    {
        return 0;
    }

    const char result = static_cast<char>(sLastPressedKey_A8A604);
    sIsAKeyDown_A8A600 = 0;
    sLastPressedKey_A8A604 = 0;
    return result;
}

ALIVE_VAR(1, 0x9F7710, BYTE, sInputEnabled_9F7710, 0);

EXPORT int Input_Enable_48E6A0()
{
    sInputEnabled_9F7710 = 1;
    return 0;
}

EXPORT void Input_Reset_44F2F0()
{
    Input_Enable_48E6A0();
    Input_InitKeyStateArray_48E5F0();
}

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

void PauseMenu::VUpdate_44DFB0()
{
    enum PauseMenuPages
    {
        ePause_0 = 0,
        eSave_1 = 1,
        eControls_2 = 2,
        eQuit_3 = 3
    };

    if (sInputObject_5009E8.IsHeld(0x800))
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
            sInputObject_5009E8.Update_433250();

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
                    if (sInputObject_5009E8.IsHeld(0x100))
                    {
                        field_124++;
                        if (field_124 > 3)
                        {
                            field_124 = 0;
                        }
                        SFX_Play_43AE60(SoundEffect::MenuNavigation_61, 45, 400, 0);
                    }
                    if (sInputObject_5009E8.IsHeld(0x1000))
                    {
                        field_124--;
                        if (field_124 < 0)
                        {
                            field_124 = 3;
                        }
                        SFX_Play_43AE60(SoundEffect::MenuNavigation_61, 45, 400, 0);
                    }
                    if (sInputObject_5009E8.IsHeld(0x4000))
                    {
                        field_124++;
                        if (field_124 > 3)
                        {
                            field_124 = 0;
                        }
                        SFX_Play_43AE60(SoundEffect::MenuNavigation_61, 45, 400, 0);
                    }
                    if (sInputObject_5009E8.IsHeld(0x8F0))
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
                                    };
                                }

                                auto curPathId = gMap_507BA8.field_2_current_path;
                                char v22[12] = {};

                                strncpy(&saveNameBuffer_5080C6[2], gLevelNames_4CE1D4[tmp], 19);
                                if (tmp != 12 && tmp != 14 && tmp != 15)
                                {
                                    strcat(&saveNameBuffer_5080C6[2], " ");
                                    if (strlen(&saveNameBuffer_5080C6[2]) < 18)
                                    {
                                        strcat(&saveNameBuffer_5080C6[2], "p");
                                    }
                                    itoa(curPathId, v22, 10);
                                    strncat(&saveNameBuffer_5080C6[2], v22, 19u);
                                }

                                const char aux[2] = { 18, 0 };
                                strncat(&saveNameBuffer_5080C6[2], aux, 19u);
                                Input_DisableInput_48E690();
                                break;
                            }
                            case Page1Selectables::eControls_2:
                            {
                                field_126_page = PauseMenuPages::eControls_2;
                                field_128 = 0;
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
                                SaveGame::WriteSave_45A110(&saveNameBuffer_5080C6[2]);
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
                    if (!last_pressed)
                    {
                        break;
                    }
                    auto string_len_no_nullterminator = strlen(&saveNameBuffer_5080C6[2]);
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
                            saveNameBuffer_5080C6[string_len_no_nullterminator + 1] = 0;
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
                            saveNameBuffer_5080C6[string_len_no_nullterminator] = 18;
                            saveNameBuffer_5080C6[string_len_no_nullterminator + 1] = 0;
                            SFX_Play_43AD70(SoundEffect::PickupItem_33, 0, 0);
                            break;
                        }
                        default:
                        {
                            if (strspn(lastPressedKeyNT, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 !-"))
                            {
                                if (lastPressedKeyNT[0] != 32 || string_len_no_nullterminator != 1 && saveNameBuffer_5080C6[string_len_no_nullterminator] != lastPressedKeyNT[0])
                                {
                                    if (string_len_no_nullterminator > 19)
                                    {
                                        SFX_Play_43AD70(SoundEffect::SackWobble_34, 0, 0);
                                    }
                                    else
                                    {
                                        saveNameBuffer_5080C6[string_len_no_nullterminator + 1] = lastPressedKeyNT[0];
                                        saveNameBuffer_5080C6[string_len_no_nullterminator + 2] = 18;
                                        saveNameBuffer_5080C6[string_len_no_nullterminator + 3] = 0;
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
                    if (sInputObject_5009E8.IsHeld(0x810))
                    {
                        field_126_page = 0;
                        SFX_Play_43AE60(SoundEffect::PossessEffect_21, 40, 2400, 0);
                    }
                    if (sInputObject_5009E8.IsHeld(0xF1E0))
                    {
                        field_128++;
                        if (field_128 < 2)
                        {
                            SFX_Play_43AD70(SoundEffect::IngameTransition_107, 90, 0);
                        }
                        else
                        {
                            field_128 = 0;
                            field_126_page = PauseMenuPages::ePause_0;
                            SFX_Play_43AE60(SoundEffect::PossessEffect_21, 40, 2400, 0);
                        }
                    }
                    break;
                }
                case PauseMenuPages::eQuit_3:
                {
                    if (sInputObject_5009E8.IsHeld(0x810))
                    {
                        field_126_page = 0;
                        SFX_Play_43AE60(SoundEffect::PossessEffect_21, 40, 2400, 0);
                    }
                    if (sInputObject_5009E8.IsHeld(0xE0))
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
                        sCurrentControllerIndex_5076B8 = 0;
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
                sInputObject_5009E8.Update_433250();
                field_6_flags.Clear(Options::eDrawable_Bit4);
                break;
            }
        }

        sDisableFontFlicker_5080E4 = 0;
    }
}

void PauseMenu::VRender(int** ppOt)
{
    VRender_44E6F0(ppOt);
}

void PauseMenu::VRender_44E6F0(int** /*ppOt*/)
{
    NOT_IMPLEMENTED();
}

END_NS_AO
