#include "stdafx.h"
#include "PauseMenu.hpp"
#include "stdlib.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "DebugHelpers.hpp"
#include "SwitchStates.hpp"
#include "StringFormatters.hpp"
#include "Abe.hpp"
#include "Sfx.hpp"
#include "Sound/Sound.hpp"
#include "MainMenu.hpp"
#include "ThrowableArray.hpp"
#include "Io.hpp"
#include "QuikSave.hpp"
#include "VGA.hpp"
#include "Mudokon.hpp"
#include "Sys.hpp"

ALIVE_VAR(1, 0x5ca4d8, char, sQuicksave_SaveNextFrame_5CA4D8, 0);
ALIVE_VAR(1, 0x5ca4d9, char, sQuicksave_LoadNextFrame_5CA4D9, 0);

ALIVE_ARY(1, 0x5C92F0, char, 16, sScreenStringBuffer_5C92F0, { 0 });


ALIVE_ARY(1, 0x5C931C, char, 32, sSaveString_5C931C, {});


ALIVE_ARY(1, 0x554474, BYTE, 32, pal_554474, {
    0x00, 0x00, 0x21, 0x84, 0x42, 0x88, 0x63, 0x8C, 0x84, 0x90,
    0xA5, 0x14, 0xE7, 0x1C, 0x08, 0x21, 0x29, 0x25, 0x4A, 0x29,
    0x6B, 0x2D, 0x8C, 0x31, 0xAD, 0x35, 0xEF, 0x3D, 0x10, 0x42,
    0x73, 0x4E });

// MENUS

//MENU_BEGIN(MainMenu);
//MENU_ENTRY("continue", 184, 48, Centre);
//MENU_ENTRY("quicksave", 184, 70, Centre);
//#if DEVELOPER_MODE
//MENU_ENTRY_EX("developer", 184, 92, Centre, 33, 127, 33);
//#else
//MENU_ENTRY("controls", 184, 92, Centre);
//#endif
//MENU_ENTRY("status", 184, 114, Centre);
//MENU_ENTRY("save", 184, 136, Centre);
//MENU_ENTRY("load", 184, 158, Centre);
//MENU_ENTRY("restart path", 184, 180, Centre);
//MENU_ENTRY("quit", 184, 202, Centre);
//MENU_ENTRY("paused", 184, 16, Centre);
//MENU_ENTRY(sScreenStringBuffer_5C92F0, 280, 16, Left);
//MENU_END(55E1C8, MainMenu);

// Menu Text Data

PauseMenuPageEntry PauseMenu__PageEntryList_Main_55E1C8[11] =
{
    { 2, 184, 48, 0, "continue", 128u, 16u, 255u, 1u },
    { 2, 184, 70, 0, "quiksave", 128u, 16u, 255u, 1u },
#if DEVELOPER_MODE
    { 2, 184, 92, 0, "developer", 33u, 127u, 33u, 1u },
#else
    { 2, 184, 92, 0, "controls", 128u, 16u, 255u, 1u },
#endif
    { 2, 184, 114, 0, "status", 128u, 16u, 255u, 1u },
    { 2, 184, 136, 0, "save", 128u, 16u, 255u, 1u },
    { 2, 184, 158, 0, "load", 128u, 16u, 255u, 1u },
    { 2, 184, 180, 0, "restart path", 128u, 16u, 255u, 1u },
    { 2, 184, 202, 0, "quit", 128u, 16u, 255u, 1u },
    { 1, 184, 16, 0, "paused", 128u, 16u, 255u, 1u },
    { 1, 280, 16, 0, sScreenStringBuffer_5C92F0, 128u, 16u, 255u, 0u },
    { 0, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u }
};


PauseMenuPageEntry PauseMenu__PageEntryList_ControlActions_55d820[17] =
{
    { 2, 184, 205, 0, "more", 128, 16, 255, Centre },
    { 1, 184, 20, 0, "Actions", 127, 127, 127, Centre },
    { 1, 180, 50, 0, kRun "+" kLeft kRight, 160, 160, 160, Right },
    { 1, 180, 70, 0, kSneak "+" kLeft kRight, 160, 160, 160, Right },
    { 1, 180, 90, 0, kJump, 160, 160, 160, Right },
    { 1, 180, 110, 0, kThrow "+" kDPad, 160, 160, 160, Right },
    { 1, 180, 135, 0, kDown, 160, 160, 160, Right },
    { 1, 180, 150, 0, kAction, 160, 160, 160, Right },
    { 1, 180, 173, 0, kUp, 160, 160, 160, Right },
    { 1, 200, 50, 0, "run", 128, 16, 255, Left },
    { 1, 200, 70, 0, "sneak", 128, 16, 255, Left },
    { 1, 200, 90, 0, "jump", 128, 16, 255, Left },
    { 1, 200, 110, 0, "throw", 128, 16, 255, Left },
    { 1, 200, 130, 0, "crouch", 128, 16, 255, Left },
    { 1, 200, 150, 0, "action", 128, 16, 255, Left },
    { 1, 200, 170, 0, "hoist " kHoistZTurn " zturn", 128, 16, 255, Left },
    { 1, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u }
};

PauseMenuPageEntry PauseMenu__PageEntryList_GameSpeak_55d930[21] =
{
    { 2, 184, 205, 0, "more", 128, 16, 255, Centre },
    { 1, 184, 20, 0, "GameSpeak", 127, 127, 127, Centre },
    { 1, 184, 55, 0, kChant, 160, 160, 160, Centre },
    { 1, 184, 75, 0, "chant", 128, 16, 255, Centre },
    { 1, 110, 104, 0, "hello", 128, 16, 255, Left },
    { 1, 110, 126, 0, "follow me", 128, 16, 255, Left },
    { 1, 110, 148, 0, "wait", 128, 16, 255, Left },
    { 1, 110, 170, 0, "work", 128, 16, 255, Left },
    { 1, 278, 104, 0, "anger", 128, 16, 255, Left },
    { 1, 278, 126, 0, "all 'ya", 128, 16, 255, Left },
    { 1, 278, 148, 0, "sympathy", 128, 16, 255, Left },
    { 1, 278, 170, 0, "stop it!", 128, 16, 255, Left },
    { 1, 90, 104, 0, kHello, 160, 160, 160, Right },
    { 1, 90, 126, 0, kFollowMe, 160, 160, 160, Right },
    { 1, 90, 148, 0, kWait, 160, 160, 160, Right },
    { 1, 90, 170, 0, kWork, 160, 160, 160, Right },
    { 1, 260, 104, 0, kAnger, 160, 160, 160, Right },
    { 1, 260, 126, 0, kAllYa, 160, 160, 160, Right },
    { 1, 260, 148, 0, kSorry, 160, 160, 160, Right },
    { 1, 260, 170, 0, kStopIt, 160, 160, 160, Right },
    { 1, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u }
};

PauseMenuPageEntry PauseMenu__PageEntryList_SligSpeak_55da80[19] =
{
    { 2, 184, 205, 0, "more", 128, 16, 255, Centre },
    { 1, 184, 20, 0, "SligSpeak", 127, 127, 127, Centre },
    { 1, 110, 104, 0, "hi", 128, 16, 255, Left },
    { 1, 110, 126, 0, "here boy", 128, 16, 255, Left },
    { 1, 110, 148, 0, "freeze", 128, 16, 255, Left },
    { 1, 110, 170, 0, "get 'im", 128, 16, 255, Left },
    { 1, 278, 104, 0, "s'mo bs", 128, 16, 255, Left },
    { 1, 278, 126, 0, "bs", 128, 16, 255, Left },
    { 1, 278, 148, 0, "look out", 128, 16, 255, Left },
    { 1, 278, 170, 0, "laugh", 128, 16, 255, Left },
    { 1, 90, 104, 0, kHello, 160, 160, 160, Right },
    { 1, 90, 126, 0, kFollowMe, 160, 160, 160, Right },
    { 1, 90, 148, 0, kWait, 160, 160, 160, Right },
    { 1, 90, 170, 0, kWork, 160, 160, 160, Right },
    { 1, 260, 104, 0, kAnger, 160, 160, 160, Right },
    { 1, 260, 126, 0, kAllYa, 160, 160, 160, Right },
    { 1, 260, 148, 0, kSorry, 160, 160, 160, Right },
    { 1, 260, 170, 0, kStopIt, 160, 160, 160, Right },
    { 1, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u }
};

PauseMenuPageEntry PauseMenu__PageEntryList_GlukkonSpeak_55dbb0[19] =
{
    { 2, 184, 205, 0, "more", 128, 16, 255, Centre },
    { 1, 184, 20, 0, "GlukkonSpeak", 127, 127, 127, Centre },
    { 1, 110, 104, 0, "hey!", 128, 16, 255, Left },
    { 1, 110, 126, 0, "commere", 128, 16, 255, Left },
    { 1, 110, 148, 0, "stay here", 128, 16, 255, Left },
    { 1, 110, 170, 0, "do it!", 128, 16, 255, Left },
    { 1, 278, 104, 0, "kill'em!", 128, 16, 255, Left },
    { 1, 278, 126, 0, "all o'ya", 128, 16, 255, Left },
    { 1, 278, 148, 0, "help!", 128, 16, 255, Left },
    { 1, 278, 170, 0, "laugh", 128, 16, 255, Left },
    { 1, 90, 104, 0, kHello, 160, 160, 160, Right },
    { 1, 90, 126, 0, kFollowMe, 160, 160, 160, Right },
    { 1, 90, 148, 0, kWait, 160, 160, 160, Right },
    { 1, 90, 170, 0, kWork, 160, 160, 160, Right },
    { 1, 260, 104, 0, kAnger, 160, 160, 160, Right },
    { 1, 260, 126, 0, kAllYa, 160, 160, 160, Right },
    { 1, 260, 148, 0, kSorry, 160, 160, 160, Right },
    { 1, 260, 170, 0, kStopIt, 160, 160, 160, Right },
    { 1, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u }
};

PauseMenuPageEntry PauseMenu__PageEntryList_ParamiteSpeak_55dce0[15] =
{
    { 2, 184, 205, 0, "more", 128, 16, 255, Centre },
    { 1, 184, 20, 0, "ParamiteSpeak", 127, 127, 127, Centre },
    { 1, 110, 104, 0, "howdy", 128, 16, 255, Left },
    { 1, 110, 126, 0, "c'mon", 128, 16, 255, Left },
    { 1, 110, 148, 0, "stay", 128, 16, 255, Left },
    { 1, 110, 170, 0, "do it", 128, 16, 255, Left },
    { 1, 278, 104, 0, "attack", 128, 16, 255, Left },
    { 1, 278, 126, 0, "all a ya!", 128, 16, 255, Left },
    { 1, 90, 104, 0, kHello, 160, 160, 160, Right },
    { 1, 90, 126, 0, kFollowMe, 160, 160, 160, Right },
    { 1, 90, 148, 0, kWait, 160, 160, 160, Right },
    { 1, 90, 170, 0, kWork, 160, 160, 160, Right },
    { 1, 260, 104, 0, kAnger, 160, 160, 160, Right },
    { 1, 260, 126, 0, kAllYa, 160, 160, 160, Right },
    { 1, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u }
};

PauseMenuPageEntry PauseMenu__PageEntryList_ScrabSpeak_55ddd0[7] =
{
    { 2, 184, 205, 0, "exit", 128, 16, 255, Centre },
    { 1, 184, 20, 0, "ScrabSpeak", 127, 127, 127, Centre },
    { 1, 110, 104, 0, "shred power", 128, 16, 255, Left },
    { 1, 110, 126, 0, "howl", 128, 16, 255, Left },
    { 1, 90, 104, 0, kHello, 160, 160, 160, Right },
    { 1, 90, 126, 0, kFollowMe, 160, 160, 160, Right },
    { 1, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u }
};

PauseMenuPageEntry PauseMenu__PageEntryList_Save_55e4c8[4] =
{
    { 1, 184, 120, 0, sSaveString_5C931C, 128, 16, 255, Centre },
    { 1, 184, 180, 0, "Enter   Save", 128, 16, 255, Centre },
    { 1, 184, 205, 0, "Esc   Cancel", 128, 16, 255, Centre },
    { 1, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u }
};

PauseMenuPageEntry PauseMenu__PageEntryList_ReallyQuit_55e278[3] =
{
    { 1, 184, 110, 0, "REALLY QUIT?", 128, 16, 255, Centre },
    { 1, 184, 135, 0, "Enter yes   Esc no", 160, 160, 160, Centre },
    { 1, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u }
};

PauseMenuPageEntry PauseMenu__PageEntryList_Status_55e738[1] =
{
    { 1, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u }
};

PauseMenuPageEntry PauseMenu__PageEntryList_Load_55e3a0[9] =
{
    { 1, 184, 5, 0, "", 128, 16, 255, Centre },
    { 1, 184, 30, 0, "", 128, 16, 255, Centre },
    { 1, 184, 55, 0, "", 128, 16, 255, Centre },
    { 1, 184, 80, 0, "", 128, 16, 255, Centre },
    { 1, 184, 105, 0, "", 128, 16, 255, Centre },
    { 1, 184, 130, 0, "", 128, 16, 255, Centre },
    { 1, 184, 188, 0, "", 128, 16, 255, Centre },
    { 1, 184, 213, 0, "Esc  Cancel        F6  Load QuikSave", 128, 16, 255, Centre },
    { 1, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u }
};


// Menu Pages

PauseMenu::PauseMenuPage sPM_Page_Main_5465B0 =
{
    &PauseMenu::Page_Main_Update_4903E0,
    &PauseMenu::Page_Base_Render_490A50,
    PauseMenu__PageEntryList_Main_55E1C8,
    0,
    100u,
    100u,
    100u,
    0u,
    0u,
    0u
};

PauseMenu::PauseMenuPage sPM_Page_Controls_Actions_546610 =
{
    &PauseMenu::Page_ControlsActions_Update_48FA60,
    &PauseMenu::Page_Base_Render_490A50,
    PauseMenu__PageEntryList_ControlActions_55d820,
    0,
    100u,
    100u,
    100u,
    0u,
    0u,
    0u
};

PauseMenu::PauseMenuPage sPM_Page_ReallyQuit_5465E0 =
{
    &PauseMenu::Page_ReallyQuit_Update_490930,
    &PauseMenu::Page_Base_Render_490A50,
    PauseMenu__PageEntryList_ReallyQuit_55e278,
    -1,
    100u,
    100u,
    100u,
    0u,
    0u,
    0u
};

PauseMenu::PauseMenuPage sPM_Page_Load_546628 =
{
    &PauseMenu::Page_Load_Update_490D50,
    &PauseMenu::Page_Load_Render_4910A0,
    PauseMenu__PageEntryList_Load_55e3a0,
    0,
    static_cast<char>(160u),
    static_cast<char>(160u),
    static_cast<char>(160u),
    0u,
    0u,
    0u
};

PauseMenuPageEntry PauseMenu__PageEntryList_Save_55E4C8[4] =
{
    { 1, 184, 120, 0, sSaveString_5C931C, 128u, 16u, 255u, 1u },
    { 1, 184, 180, 0, "Enter   Save", 128u, 16u, 255u, 1u },
    { 1, 184, 205, 0, "Esc   Cancel", 128u, 16u, 255u, 1u },
    { 1, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u }
};

PauseMenuPageEntry PauseMenu__PageEntryList_Save_Overwrite_Confirm_55E508[2] =
{
    { 1, 184, 180, 0, "Enter Overwrite  Esc Cancel", 160u, 160u, 160u, 1u },
    { 1, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u }
};

PauseMenu::PauseMenuPage sPM_Page_Save_5465C8 =
{
    &PauseMenu::Page_Save_Update_491210,
    &PauseMenu::Page_Save_Render_491660,
    &PauseMenu__PageEntryList_Save_55E4C8[0],
    0,
    static_cast<char>(160u),
    static_cast<char>(160u),
    static_cast<char>(160u),
    0,
    0,
    0
};

ALIVE_ARY(1, 0x55e718, char, 32, sPauseMenu_Of300Mudokons_55E718, {});
ALIVE_ARY(1, 0x55e738, char, 56, sHasBeenTerminated_55E738, {});

PauseMenuPageEntry sStatusEntries_55E758[6] =
{
    { 2, 184, 205, 0, "EXIT", 128u, 16u, 255u, 1u },
    { 1, 184, 20, 0, "ABE'S STATUS", 127u, 127u, 127u, 1u },
    { 1, 184, 145, 0, sPauseMenu_Of300Mudokons_55E718, 128u, 16u, 255u, 1u },
    { 1, 184, 170, 0, sHasBeenTerminated_55E738, 128u, 16u, 255u, 1u },
    { 1, 184, 120, 0, "YOU HAVE RESCUED", 128u, 16u, 255u, 1u },
    { 0, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u }
};

PauseMenu::PauseMenuPage sPM_Page_Status_5465F8 =
{
    &PauseMenu::Page_Status_Update_4916A0,
    &PauseMenu::Page_Status_Render_491710,
    &sStatusEntries_55E758[0],
    0,
    100u,
    100u,
    100u,
    0u,
    0u,
    0u
};

ALIVE_ARY(1, 0x55DE40, PauseMenuPageEntry*, 6, sControlActionsPages_55DE40,
{
    PauseMenu__PageEntryList_ControlActions_55d820,
    PauseMenu__PageEntryList_GameSpeak_55d930,
    PauseMenu__PageEntryList_SligSpeak_55da80,
    PauseMenu__PageEntryList_GlukkonSpeak_55dbb0,
    PauseMenu__PageEntryList_ParamiteSpeak_55dce0,
    PauseMenu__PageEntryList_ScrabSpeak_55ddd0
});

struct DumpEntry
{
    void* address;
    std::string name;
};

void DumpMenus()
{
    std::vector<DumpEntry> menuListArrays;
    menuListArrays.push_back({ (void*)0x55D820, "ControlActions" });
    menuListArrays.push_back({ (void*)0x55D930, "GameSpeak" });
    menuListArrays.push_back({ (void*)0x55DA80, "SligSpeak" });
    menuListArrays.push_back({ (void*)0x55DBB0, "GlukkonSpeak" });
    menuListArrays.push_back({ (void*)0x55DCE0, "ParamiteSpeak" });
    menuListArrays.push_back({ (void*)0x55DDD0, "ScrabSpeak" });

    menuListArrays.push_back({ (void*)0x55E4C8, "Save" });
    menuListArrays.push_back({ (void*)0x55E278, "ReallyQuit" });
    menuListArrays.push_back({ (void*)0x55E738, "Status" });
    menuListArrays.push_back({ (void*)0x55E3A0, "Load" });

    // menuListArrays.push_back({ 0x55DDD0, "" });

    std::stringstream output;
    std::stringstream output_override;

    for (auto a : menuListArrays)
    {
        std::stringstream menuAddr;
        menuAddr << std::hex << a.address;

        s32 count = 0;
        PauseMenuPageEntry* c = reinterpret_cast<PauseMenuPageEntry*>(a.address);

        while (c->field_8_text)
        {
            count++;
            c++;
        }

        output << "PauseMenuPageEntry PauseMenu__PageEntryList_" + a.name + "_" + menuAddr.str() + "[" + std::to_string(count + 1) + "] =\n{\n";

        PauseMenuPageEntry* e = reinterpret_cast<PauseMenuPageEntry*>(a.address);

        while (e->field_8_text)
        {
            std::string alignment;
            switch (e->field_F_alignment)
            {
            case 0:
                alignment = "Left";
                break;
            case 1:
                alignment = "Centre";
                break;
            case 2:
                alignment = "Right";
                break;
            default:
                alignment = "UnknownAlignment";
                break;
            }

            output << "\t{ ";
            output << e->field_0_unknown2 << ", ";
            output << e->field_2_x << ", ";
            output << e->field_4_y << ", ";
            output << e->field_6_unknown << ", ";
            output << "\"" << EscapeUnknownCharacters(e->field_8_text) << "\"" << ", ";
            output << (s32)e->field_C_r << ", ";
            output << (s32)e->field_D_g << ", ";
            output << (s32)e->field_E_b << ", ";
            output << alignment;
            output << " },\n";

            e++;
        }

        output << "\t{ 1, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u }\n};\n\n";
    }

    std::ofstream fileOut("menu_dump.h");
    fileOut << output.rdbuf() << "\n\n" << output_override.rdbuf();
}

EXPORT s16 sub_4A2BC0()
{
    NOT_IMPLEMENTED();
    return 0;
}

BaseGameObject* PauseMenu::VDestructor(s32 flags)
{
    return vdtor_48FCB0(flags);
}

void PauseMenu::VUpdate()
{
    Update_48FD80();
}

void PauseMenu::VRender(PrimHeader** ppOt)
{
    Render_490BD0(ppOt);
}

void PauseMenu::VScreenChanged()
{
    Remove_At_Credits_Screen_490D30();
}

PauseMenu::PauseMenu()
{

}

PauseMenu * PauseMenu::ctor_48FB80()
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);

    SetVTable(&field_158_animation, 0x544290);
    SetVTable(this, 0x546658);

    sQuicksave_SaveNextFrame_5CA4D8 = 0;
    sQuicksave_LoadNextFrame_5CA4D9 = 0;

    field_4_typeId = AETypes::ePauseMenu_95;
    field_6_flags.Clear(BaseGameObject::eDrawable_Bit4);
    field_6_flags.Set(BaseGameObject::eSurviveDeathReset_Bit9);
    word12C_flags &= ~0xE;
    word12C_flags &= ~1u;

    field_1C_update_delay = 25;

    gObjList_drawables_5C1124->Push_Back(this);

    field_136_unused = 0;
    field_138_control_action_page_index = 0;
    field_13A_unused = 0;
    field_13C_save_state = SaveState::ReadingInput_0;
    field_13E_unused = 0;
    field_140_unused = 0;

    field_F4_font.ctor_433590(256, pal_554474, &sFont1Context_5BC5C8);

    Init_491760();
    sub_4A2BC0();

    sDisableFontFlicker_5C9304 = 0;

    return this;
}

void PauseMenu::dtor_48FCE0()
{
    SetVTable(this, 0x546658);

    field_6_flags.Clear(BaseGameObject::eDrawable_Bit4);

    gObjList_drawables_5C1124->Remove_Item(this);
    field_158_animation.vCleanUp_40C630();
    field_F4_font.dtor_433540();
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

BaseGameObject* PauseMenu::vdtor_48FCB0(s32 flags)
{
    dtor_48FCE0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void PauseMenu::Init_491760()
{
    ResourceManager::LoadResourceFile_49C170("EMOHAP.BAN", 0);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kHappyiconResID);
    ResourceManager::LoadResourceFile_49C170("EMOANGRY.BAN", 0);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kAngryiconResID);
    ResourceManager::LoadResourceFile_49C170("EMONORM.BAN", 0);
    BYTE **ppAnimData = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kNormaliconResID);

    if (field_158_animation.Init_40A030(1248, gObjList_animations_5C1A24, this, 54, 47, ppAnimData, 1, 0, 0))
    {
        this->field_158_animation.field_C_render_layer = field_20_animation.field_C_render_layer;
        this->field_158_animation.field_14_scale = field_CC_sprite_scale;
        this->field_158_animation.field_8_r = 127;
        this->field_158_animation.field_9_g = 127;
        this->field_158_animation.field_A_b = 127;
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eListAddFailed_Bit1);
    }
}

void PauseMenu::Render_490BD0(PrimHeader** ot)
{
    field_142_poly_offset = 0;

    // Render the page
    (this->*field_144_active_menu.field_4_fn_render)(ot, &field_144_active_menu);

    // Draw a full screen polygon that "dims" out the screen while paused
    Prim_SetTPage* pTPage = &field_1F0_primitives[gPsxDisplay_5C1130.field_C_buffer_index];
    Poly_F4* pPolys = &field_210_polygons[gPsxDisplay_5C1130.field_C_buffer_index];
    PolyF4_Init_4F8830(pPolys);
    Poly_Set_SemiTrans_4F8A60(&pPolys->mBase.header, TRUE);
    Poly_Set_Blending_4F8A20(&pPolys->mBase.header, FALSE);
    SetRGB0(pPolys,
        field_144_active_menu.field_E_background_r,
        field_144_active_menu.field_F_background_g,
        field_144_active_menu.field_10_background_b);
    SetXY0(pPolys, 0, 0);
    SetXY1(pPolys, 640, 0);
    SetXY2(pPolys, 0, 240);
    SetXY3(pPolys, 640, 240);
    Init_SetTPage_4F5B60(pTPage, 0, 0, PSX_getTPage_4F60E0(TPageMode::e4Bit_0, TPageAbr::eBlend_2, 0, 0));
    OrderingTable_Add_4F8AA0(OtLayer(ot, Layer::eLayer_41), &pPolys->mBase.header);
    OrderingTable_Add_4F8AA0(OtLayer(ot, Layer::eLayer_41), &pTPage->mBase);
    pScreenManager_5BB5F4->InvalidateRect_40EC90(0, 0, 640, 240, pScreenManager_5BB5F4->field_3A_idx);
}

EXPORT void PauseMenu::Remove_At_Credits_Screen_490D30()
{
    if (gMap_5C3030.field_A_level == LevelIds::eCredits_16)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

#if DEVELOPER_MODE
// CUSTOM PAUSE MENU

class CustomPauseMenu;
struct CustomPauseMenuItem
{
    const char* text;
    std::function<void(CustomPauseMenu*)> callback;
};

std::vector<CustomPauseMenu*> customMenuStack;

class CustomPauseMenu
{
public:
    CustomPauseMenu(std::vector<CustomPauseMenuItem>* items, const char* titleStr)
    {
        entries = items;
        title = std::string(titleStr);
        CompileEntries();
    }

    virtual void CompileEntries()
    {
        compiledEntries.clear();

        if (index > maxScrollDown)
        {
            scrollDownIndex = index - maxScrollDown;
        }
        else
        {
            scrollDownIndex = 0;
        }

        s32 o = 0;
        s32 size = 0;
        if (entries != nullptr)
        {
            size = static_cast<s32>(entries->size());
            for (s32 i = scrollDownIndex; i < std::min(size, scrollDownIndex + 7); i++)
            {
                const auto item = (*entries)[i];
                compiledEntries.push_back({ 0, 184, (short)(57 + (22 * o)), 0, (char*)item.text, 0x80, 0x10, 0xFF, Centre });
                o++;
            }
        }
        compiledEntries.push_back({ 0, 184, 16, 0, (char*)title.c_str(), 127, 127, 127, Centre });
        if (size > 7 && index < size - 2)
        {
            compiledEntries.push_back({ 0, 184, 210, 0, kDown, 127, 127, 127, Centre });
        }
        if (index > 5)
        {
            compiledEntries.push_back({ 0, 184, 42, 0, kUp, 127, 127, 127, Centre });
        }
        compiledEntries.push_back({ 0, 0, 0, 0, nullptr, 0, 0, 0, 0 });
        mMenuPage.field_0_fn_update = &PauseMenu::CustomPauseMenuUpdate;
        mMenuPage.field_4_fn_render = &PauseMenu::Page_Base_Render_490A50;
        mMenuPage.field_C_selected_index = 0;
        mMenuPage.field_8_menu_items = compiledEntries.data();
        mMenuPage.field_E_background_r = 127;
        mMenuPage.field_F_background_g = 127;
        mMenuPage.field_10_background_b = 127;
    }

    void SetText(char* text)
    {
        (*entries)[index].text = text;
        compiledEntries[index].field_8_text = text;
    }

    void ClosePauseMenu()
    {
        pPauseMenu_5C9300->word12C_flags &= ~1;
        SFX_Play_46FBA0(SoundEffect::PossessEffect_17, 40, 2400);
        GetSoundAPI().SND_Restart();
        customMenuStack.clear();
    }

    void Update(PauseMenu* pm)
    {
        auto input = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;
        if (input & InputCommands::Enum::eDown)
        {
            if (++index >= static_cast<s32>(entries->size()))
            {
                index = 0;
            }
            SFX_Play_46FBA0(SoundEffect::MenuNavigation_52, 45, 400);

            CompileEntries();
        }
        else if (input & InputCommands::Enum::eUp)
        {
            if (--index < 0)
            {
                index = static_cast<s32>(entries->size()) - 1;
            }
            SFX_Play_46FBA0(SoundEffect::MenuNavigation_52, 45, 400);

            CompileEntries();
        }
        else if (input & InputCommands::Enum::eBack)
        {
            SFX_Play_46FBA0(SoundEffect::PossessEffect_17, 40, 2400);
            GoBack(pm);
        }
        else if (input & InputCommands::Enum::eUnPause_OrConfirm)
        {
            (*entries)[index].callback(this);
            SFX_Play_46FA90(SoundEffect::IngameTransition_84, 90);
            CompileEntries();
        }

        pm->field_144_active_menu.field_C_selected_index = static_cast<short>(index - scrollDownIndex);
    }

    void Activate()
    {
        CompileEntries();
        customMenuStack.push_back(this);
        SetLastPageStack(pPauseMenu_5C9300);
    }

    void GoBack(PauseMenu* pm)
    {
        customMenuStack.pop_back();
        SetLastPageStack(pm);
    }

    void SetLastPageStack(PauseMenu* pm)
    {
        if (customMenuStack.size() > 0)
            memcpy(&pm->field_144_active_menu, &customMenuStack[customMenuStack.size() - 1]->mMenuPage, sizeof(pm->field_144_active_menu));
        else
            memcpy(&pm->field_144_active_menu, &sPM_Page_Main_5465B0, sizeof(pm->field_144_active_menu));
    }

public:
    std::vector<CustomPauseMenuItem>* entries;
    s32 index = 0;
    s32 scrollDownIndex = 0;
    s32 maxScrollDown = 5;
    std::string title;
    std::vector<PauseMenuPageEntry> compiledEntries;
    PauseMenu::PauseMenuPage mMenuPage;
};

std::vector<CustomPauseMenuItem> optionMenuItems({});

std::string optionMenuStr_SoundStereo;
std::string optionMenuStr_SoundReverb;
std::string optionMenuStr_WindowMode;
std::string optionMenuStr_AspectRatio;
std::string optionMenuStr_WindowFilterMode;

class OptionsMenu : public CustomPauseMenu
{
public:
    OptionsMenu() : CustomPauseMenu(&optionMenuItems, "Options")
    {
    }

    virtual void CompileEntries() override
    {
        optionMenuItems.clear();

#if USE_SDL2_SOUND
        optionMenuStr_SoundStereo = "Sound Mode: " + std::string((gAudioStereo) ? "Stereo" : "Mono");
        optionMenuStr_SoundReverb = "Reverb: " + std::string((gReverbEnabled) ? "On" : "Off");
#endif
        optionMenuStr_AspectRatio = "Aspect Ratio: " + std::string((s_VGA_KeepAspectRatio) ? "Keep" : "Stretch");
        optionMenuStr_WindowFilterMode = "Screen Filter: " + std::string((s_VGA_FilterScreen) ? "On" : "Off");

#if USE_SDL2_SOUND
        optionMenuItems.push_back({ optionMenuStr_SoundStereo.c_str(), [](CustomPauseMenu *) { gAudioStereo = !gAudioStereo; Input_SaveSettingsIni_492840();  } });
        optionMenuItems.push_back({ optionMenuStr_SoundReverb.c_str(), [](CustomPauseMenu *) { gReverbEnabled = !gReverbEnabled; Input_SaveSettingsIni_492840(); } });
#endif
        //optionMenuItems.push_back({ optionMenuStr_WindowMode.c_str(), [](CustomPauseMenu *) {} });
        optionMenuItems.push_back({ optionMenuStr_AspectRatio.c_str(), [](CustomPauseMenu *) { s_VGA_KeepAspectRatio = !s_VGA_KeepAspectRatio; Input_SaveSettingsIni_492840(); } });
        optionMenuItems.push_back({ optionMenuStr_WindowFilterMode.c_str(), [](CustomPauseMenu *) { s_VGA_FilterScreen = !s_VGA_FilterScreen; Input_SaveSettingsIni_492840(); } });


        CustomPauseMenu::CompileEntries();
    }
};

std::vector<CustomPauseMenuItem> devTeleportMenuItems;
CustomPauseMenu devTeleportMenu(&devTeleportMenuItems, "Level Select");

void DestroyAliveObjects()
{
    for (s32 i = 0; i < gObjList_drawables_5C1124->Size(); i++)
    {
        BaseGameObject* pObj = gObjList_drawables_5C1124->ItemAt(i);
        if (!pObj || pObj == pPauseMenu_5C9300 || pObj->field_6_flags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6) == false)
        {
            continue;
        }

        if (pObj->field_4_typeId != AETypes::eAbe_69)
        {
            pObj->field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }

    sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
}

void DestroyAllObjects()
{
    for (s32 i = 0; i < gObjList_drawables_5C1124->Size(); i++)
    {
        BaseGameObject* pObj = gObjList_drawables_5C1124->ItemAt(i);
        if (!pObj || pObj == pPauseMenu_5C9300)
        {
            continue;
        }

        if (pObj->field_4_typeId != AETypes::eAbe_69)
        {
            pObj->field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }

    sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
}

std::vector<CustomPauseMenuItem> devCheatsMenuItems({
    { "Destroy Alive Objs", [](CustomPauseMenu* pm) { DestroyAliveObjects(); pm->ClosePauseMenu(); DEV_CONSOLE_MESSAGE("Destroyed Alive Objects", 4); } },
    { "Destroy All Objects", [](CustomPauseMenu* pm) { DestroyAllObjects(); pm->ClosePauseMenu(); DEV_CONSOLE_MESSAGE("Destroyed All Objects", 4); } },
    { "Save All Mudokons", [](CustomPauseMenu*) { sRescuedMudokons_5C1BC2 = 300; sKilledMudokons_5C1BC0 = 0; DEV_CONSOLE_MESSAGE("(CHEAT) Saved all Mudokons", 4); } },
    { "Kill All Mudokons", [](CustomPauseMenu*) { sRescuedMudokons_5C1BC2 = 0; sKilledMudokons_5C1BC0 = 300; DEV_CONSOLE_MESSAGE("(CHEAT) Killed all Mudokons", 4); }  },
    { "Give Rocks", [](CustomPauseMenu*) { sActiveHero_5C1B68->field_1A2_throwable_count = 99; DEV_CONSOLE_MESSAGE("(CHEAT) Got Bones", 4); }  },
    { "Open All Doors", [](CustomPauseMenu* pm) {  pm->ClosePauseMenu(); Cheat_OpenAllDoors(); } },
    { "Invisible", [](CustomPauseMenu* pm) { DEV_CONSOLE_MESSAGE("(CHEAT) Invisibility!", 4);  pm->ClosePauseMenu(); sActiveHero_5C1B68->field_170_invisible_timer = 65535; sActiveHero_5C1B68->field_114_flags.Set(Flags_114::e114_Bit9_RestoredFromQuickSave);  sActiveHero_5C1B68->field_114_flags.Set(Flags_114::e114_Bit8_bInvisible); } },
    { "Give Explosive Fart", [](CustomPauseMenu * pm) {
        DEV_CONSOLE_MESSAGE("(CHEAT) Oh man that stinks.", 4);
        pm->ClosePauseMenu();
        sActiveHero_5C1B68->field_198_has_evil_fart = true;
        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kEvilFartResID, FALSE, FALSE))
        {
            ResourceManager::LoadResourceFile_49C170("EVILFART.BAN", nullptr);
        }
        if (!sActiveHero_5C1B68->field_10_resources_array.ItemAt(22))
        {
            sActiveHero_5C1B68->field_10_resources_array.SetAt(22, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kEvilFartResID, TRUE, FALSE));
        }
    } },
});


CustomPauseMenu devCheatsMenu(&devCheatsMenuItems, "Cheats");

OptionsMenu optionsMenu;

std::vector<CustomPauseMenuItem> devMenuItems({
    { "Options", [](CustomPauseMenu *) { optionsMenu.Activate(); } },
    { "Cheats", [](CustomPauseMenu * ) { devCheatsMenu.Activate(); } },
    { "Level Select", [](CustomPauseMenu * ) { devTeleportMenu.Activate(); } },
});
CustomPauseMenu devMenu(&devMenuItems, "Developer Menu");


void PauseMenu::CustomPauseMenuUpdate()
{
#if DEVELOPER_MODE
    if (customMenuStack.size() > 0)
        customMenuStack[customMenuStack.size() - 1]->Update(this);
#endif
}

///// END CUSTOM CODE!!! ///

#endif

void PauseMenu_ForceLink() {

#if DEVELOPER_MODE
    if (RunningAsInjectedDll())
    {
        //DumpMenus();
    }

    devTeleportMenuItems.clear();

    for (s32 i = 0; i < 17; i++)
    {
        devTeleportMenuItems.push_back({ gPerLvlData_561700[i].field_0_display_name, [](CustomPauseMenu* pm) {
            const auto levelSelectEntry = gPerLvlData_561700[pm->index];
            pm->ClosePauseMenu();
            sActiveHero_5C1B68->field_106_current_motion = eAbeStates::State_3_Fall_459B60;
            sActiveHero_5C1B68->field_1AC_flags.Set(Abe::e1AC_Bit7_land_softly);
            sActiveHero_5C1B68->field_C2_lvl_number = levelSelectEntry.field_4_level;
            sActiveHero_5C1B68->field_C0_path_number = levelSelectEntry.field_6_path;
            sActiveHero_5C1B68->field_100_pCollisionLine = nullptr;

            if (levelSelectEntry.field_A_id & 1)
            {
                sActiveHero_5C1B68->field_D6_scale = 1;
                sActiveHero_5C1B68->field_CC_sprite_scale = FP_FromDouble(1.0);
            }
            else
            {
                sActiveHero_5C1B68->field_D6_scale = 0;
                sActiveHero_5C1B68->field_CC_sprite_scale = FP_FromDouble(0.5);
            }

            sActiveHero_5C1B68->field_F8_LastLineYPos = sActiveHero_5C1B68->field_BC_ypos;
            gMap_5C3030.SetActiveCam_480D30(levelSelectEntry.field_4_level, levelSelectEntry.field_6_path, levelSelectEntry.field_8_camera, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
            sActiveHero_5C1B68->field_B8_xpos = FP_FromInteger(levelSelectEntry.field_C_abe_x_off - Path_Get_Bly_Record_460F30(levelSelectEntry.field_4_level, levelSelectEntry.field_6_path)->field_4_pPathData->field_1A_abe_start_xpos);
            sActiveHero_5C1B68->field_BC_ypos = FP_FromInteger(levelSelectEntry.field_E_abe_y_off - Path_Get_Bly_Record_460F30(levelSelectEntry.field_4_level, levelSelectEntry.field_6_path)->field_4_pPathData->field_1C_abe_start_ypos);
            DEV_CONSOLE_MESSAGE("Teleported to " + std::string(levelSelectEntry.field_0_display_name), 6);
        } });
    }

    //devTeleportMenu = CustomPauseMenu(&devTeleportMenuItems, "Level Select");
#endif
}

void PauseMenu::Page_Base_Render_490A50(PrimHeader** ot, PauseMenu::PauseMenuPage* mp)
{
    s32 i = 0;
    PauseMenuPageEntry* e = &mp->field_8_menu_items[i];

    while (e->field_8_text)
    {
        char textFormatted[128];
        String_FormatString_4969D0(e->field_8_text, textFormatted, 128, 1);

        auto glow = 0;
        if (mp->field_C_selected_index == i)
        {
            glow = field_12E_selected_glow;
        }

        auto x = e->field_2_x;

        if (e->field_F_alignment != 0)
        {
            short textMeasure = static_cast<short>(field_F4_font.MeasureWidth_433700(textFormatted));

            if (textMeasure >= 608)
            {
                x = 16;
            }
            else
            {
                if (e->field_F_alignment != 2)
                {
                    textMeasure /= 2;
                }
                x -= textMeasure;
            }
        }

        field_142_poly_offset = static_cast<short>(field_F4_font.DrawString_4337D0(
            ot,
            textFormatted,
            x, // X
            e->field_4_y, // Y
            TPageAbr::eBlend_0,
            1,
            0,
            Layer::eLayer_41,
            static_cast<char>(glow + e->field_C_r),
            static_cast<char>(glow + e->field_D_g),
            static_cast<char>(glow + e->field_E_b),
            field_142_poly_offset,
            FP_FromDouble(1.0),
            640,
            0));

        e = &mp->field_8_menu_items[++i];
    }
}

void PauseMenu::RestartPath()
{
    DestroyObjects_4A1F20();

    Quicksave_SaveSwitchResetterStates_4C9870();

    sSwitchStates_5C1A28 = sActiveQuicksaveData_BAF7F8.field_35C_restart_path_switch_states;

    Abe::CreateFromSaveState_44D4F0(sActiveQuicksaveData_BAF7F8.field_284_restart_path_abe_state);
    Quicksave_ReadWorldInfo_4C9490(&sActiveQuicksaveData_BAF7F8.field_244_restart_path_world_info);

    gMap_5C3030.SetActiveCam_480D30(
        sActiveQuicksaveData_BAF7F8.field_244_restart_path_world_info.field_4_level,
        sActiveQuicksaveData_BAF7F8.field_244_restart_path_world_info.field_6_path,
        sActiveQuicksaveData_BAF7F8.field_244_restart_path_world_info.field_8_cam,
        CameraSwapEffects::eEffect0_InstantChange,
        1,
        1);

    gMap_5C3030.field_8_force_load = TRUE;
    if (sActiveHero_5C1B68->field_1A2_throwable_count)
    {
        LoadRockTypes_49AB30(
            sActiveQuicksaveData_BAF7F8.field_244_restart_path_world_info.field_4_level,
            sActiveQuicksaveData_BAF7F8.field_244_restart_path_world_info.field_6_path);

        if (!gpThrowableArray_5D1E2C)
        {
            gpThrowableArray_5D1E2C = ae_new<ThrowableArray>();
            gpThrowableArray_5D1E2C->ctor_49A630();
        }

        gpThrowableArray_5D1E2C->Add_49A7A0(sActiveHero_5C1B68->field_1A2_throwable_count);
    }

    word12C_flags &= ~1;
    SFX_Play_46FBA0(SoundEffect::PossessEffect_17, 40, 3400);
    GetSoundAPI().SND_Restart();
}

const char kArrowChar = 3;
ALIVE_ARY(1, 0x55E398, char, 2, sArrowStr_55E398, { kArrowChar, 0 });

void PauseMenu::Page_Main_Update_4903E0()
{
    if (sInputObject_5BD4E0.isHeld(InputCommands::Enum::eDown))
    {
        if (++field_134_index_main > MainPages::ePage_Quit_7)
        {
            field_134_index_main = MainPages::ePage_Continue_0;
        }
        SFX_Play_46FBA0(SoundEffect::MenuNavigation_52, 45, 400);
    }

    if (sInputObject_5BD4E0.isHeld(InputCommands::Enum::eUp))
    {
        if (--field_134_index_main < MainPages::ePage_Continue_0)
        {
            field_134_index_main = MainPages::ePage_Quit_7;
        }
        SFX_Play_46FBA0(SoundEffect::MenuNavigation_52, 45, 400);
    }

    field_144_active_menu.field_C_selected_index = field_134_index_main;

    if (sInputObject_5BD4E0.isHeld(InputCommands::Enum::eBack))
    {
        word12C_flags &= ~1u;
        SFX_Play_46FBA0(SoundEffect::PossessEffect_17, 40, 2400);
        GetSoundAPI().SND_Restart();
    }
    else if (sInputObject_5BD4E0.isHeld(InputCommands::Enum::eUnPause_OrConfirm))
    {
        switch (field_134_index_main)
        {
        case MainPages::ePage_Continue_0:
            word12C_flags &= ~1u;
            SFX_Play_46FBA0(SoundEffect::PossessEffect_17, 40, 2400);
            GetSoundAPI().SND_Restart();
            return;

        case MainPages::ePage_QuickSave_1:
            word12C_flags &= ~1u;
            SFX_Play_46FBA0(SoundEffect::PossessEffect_17, 40, 2400);
            GetSoundAPI().SND_Restart();
            Quicksave_4C90D0();
            return;

        case MainPages::ePage_Controls_2:
#if DEVELOPER_MODE
            devMenu.Activate();
#else
            field_136_unused = 1;
            field_144_active_menu = sPM_Page_Controls_Actions_546610;
            field_138_control_action_page_index = 0;
#endif
            break;

        case MainPages::ePage_Status_3:
            field_136_unused = 3;
            field_144_active_menu = sPM_Page_Status_5465F8;
            break;

        case MainPages::ePage_Save_4:
            field_136_unused = 5;
            field_144_active_menu = sPM_Page_Save_5465C8;
            SFX_Play_46FA90(SoundEffect::IngameTransition_84, 90);
            field_13C_save_state = SaveState::ReadingInput_0;
            word12C_flags &= ~0xA;
            field_13E_unused = -1;
            word12C_flags |= 0x400;
            field_13A_unused = 0;
            Quicksave_4C90D0();
            // Set the default save name to be the current level/path/camera
            Path_Format_CameraName_460FB0(
                sSaveString_5C931C,
                gMap_5C3030.field_0_current_level,
                gMap_5C3030.field_2_current_path,
                gMap_5C3030.field_4_current_camera);
            // Null terminate it
            sSaveString_5C931C[8] = 0;
            // Append the editor arrow char
            strcat(sSaveString_5C931C, sArrowStr_55E398);

#if ORIGINAL_PS1_BEHAVIOR  // OG Change - Allow for exiting save menu using controller
            setSaveMenuOpen(true); // Sets saveMenuOpen bool to true, instead of disabling input
#else
            Input_DisableInputForPauseMenuAndDebug_4EDDC0();
#endif
            return;

        case MainPages::ePage_Load_5:
            Quicksave_FindSaves_4D4150();
            field_136_unused = 4;
            field_144_active_menu = sPM_Page_Load_546628;
            SFX_Play_46FA90(SoundEffect::IngameTransition_84, 90);
            word12C_flags &= ~0xA;
            field_13C_save_state = SaveState::ReadingInput_0;
            word12C_flags |= 0x400;
            field_13E_unused = -1;
            field_13A_unused = 0;
            return;

        case MainPages::ePage_RestartPath_6:
            RestartPath();
            return;

        case MainPages::ePage_Quit_7:
            field_136_unused = 2;
            field_144_active_menu = sPM_Page_ReallyQuit_5465E0;
            field_134_index_main = MainPages::ePage_Continue_0;
            break;

        default:
            return;
        }

        SFX_Play_46FA90(SoundEffect::IngameTransition_84, 90);
    }
}

void PauseMenu::Page_ControlsActions_Update_48FA60()
{
    if (sInputObject_5BD4E0.isHeld(InputCommands::Enum::eBack))
    {
        field_136_unused = 0;
        field_144_active_menu = sPM_Page_Main_5465B0;
        SFX_Play_46FBA0(SoundEffect::PossessEffect_17, 40, 2400);
    }

    if (sInputObject_5BD4E0.isHeld(0x100000))
    {
        const s32 prev = ++field_138_control_action_page_index;
        if (prev < 6)
        {
            field_144_active_menu.field_8_menu_items = sControlActionsPages_55DE40[prev];
            SFX_Play_46FA90(SoundEffect::IngameTransition_84, 90);
        }
        else
        {
            field_138_control_action_page_index = 0;
            field_136_unused = 0;
            field_144_active_menu = sPM_Page_Main_5465B0;
            SFX_Play_46FBA0(SoundEffect::PossessEffect_17, 40, 2400);
        }
    }
}

void PauseMenu::Page_ReallyQuit_Update_490930()
{
    if (sInputObject_5BD4E0.isHeld(InputCommands::Enum::eBack))
    {
        field_136_unused = 0;
        field_144_active_menu = sPM_Page_Main_5465B0;
        SFX_Play_46FBA0(SoundEffect::PossessEffect_17, 40, 2400);
    }

    if (sInputObject_5BD4E0.isHeld(0x100000))
    {
        word12C_flags &= ~1u;
        SFX_Play_46FBA0(SoundEffect::PossessEffect_17, 40, 2400);

        if (pPauseMenu_5C9300 && pPauseMenu_5C9300 == this)
        {
            pPauseMenu_5C9300->field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
        else
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }

        pPauseMenu_5C9300 = 0;
        gMap_5C3030.SetActiveCam_480D30(LevelIds::eMenu_0, 1, 1, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
        gMap_5C3030.field_CE_free_all_anim_and_palts = 1;
        sCurrentControllerIndex_5C1BBE = 0;
    }
}

void PauseMenu::Page_Save_Update_491210()
{
    static bool bWriteSaveFile_5C937C = false;

    char newInput[2] = {};
    char savFileName[40] = {};
    if (field_13C_save_state == SaveState::DoSave_4)
    {
        strcpy(savFileName, sSaveString_5C931C);
        strcat(savFileName, ".sav");
        if (access_impl(savFileName, 4) || bWriteSaveFile_5C937C) // check file is writable
        {
            bWriteSaveFile_5C937C = false;
            FILE* hFile = fopen(savFileName, "wb");
            if (hFile)
            {
                fwrite(&sActiveQuicksaveData_BAF7F8, sizeof(Quicksave), 1u, hFile);
                fclose(hFile);
                sSavedGameToLoadIdx_BB43FC = 0;
            }
            word12C_flags &= ~1u;
            SFX_Play_46FBA0(SoundEffect::PossessEffect_17, 40, 2400);
            GetSoundAPI().SND_Restart();
        }
        else
        {
            field_13C_save_state = SaveState::SaveConfirmOverwrite_8;
        }
    }
    else if (field_13C_save_state == SaveState::SaveConfirmOverwrite_8)
    {
        if (sInputObject_5BD4E0.isHeld(InputCommands::Enum::eUnPause_OrConfirm))
        {
            // Enter - do the save and don't return to the confirm overwrite
            SFX_Play_46FBA0(SoundEffect::PossessEffect_17, 40, 2400);
            field_13C_save_state = SaveState::DoSave_4;
            bWriteSaveFile_5C937C = true;
        }
        else if (sInputObject_5BD4E0.isHeld(InputCommands::Enum::eBack))
        {
            // Escape - cancel save
            SFX_Play_46FA90(SoundEffect::IngameTransition_84, 90);
            field_13C_save_state = SaveState::ReadingInput_0;
            strcat(sSaveString_5C931C, sArrowStr_55E398);
#if ORIGINAL_PS1_BEHAVIOR  // OG Change - Allow for exiting save menu using controller
            setSaveMenuOpen(true); // Sets saveMenuOpen bool to true, instead of disabling input
#else
            Input_DisableInputForPauseMenuAndDebug_4EDDC0();
#endif
        }
        return;
    }
    else if (field_13C_save_state == SaveState::ReadingInput_0)
    {
#if ORIGINAL_PS1_BEHAVIOR // OG Change - Exit save menu using controller
        DWORD lastPressed = Input_GetLastPressedKey_492610();

        if (lastPressed == VK_ESCAPE || lastPressed == VK_RETURN) // Keyboard ESC or ENTER
        {
            setSaveMenuOpen(false);
        }
        else if (Input().isHeld(InputCommands::eBack)) // Triangle
        {
            lastPressed = VK_ESCAPE;
            setSaveMenuOpen(false);
        }
        else if (Input().isHeld(InputCommands::eUnPause_OrConfirm)) // Cross or START
        {
            lastPressed = VK_RETURN;
            setSaveMenuOpen(false);
        }
#else
        const DWORD lastPressed = Input_GetLastPressedKey_492610();
#endif

        if (!lastPressed)
        {
            return;
        }

        newInput[0] = static_cast<char>(lastPressed);
        newInput[1] = 0;

        const size_t stringLen = strlen(sSaveString_5C931C);

        switch (lastPressed)
        {
        case VK_SHIFT:
            return;
        // Escape - cancel
        case VK_ESCAPE:
            SFX_Play_46FBA0(SoundEffect::PossessEffect_17, 40, 2400);
            field_136_unused = 0;
            field_144_active_menu = sPM_Page_Main_5465B0;
            Input_Reset_492660();
            return;

        // Enter - save
        case VK_RETURN:
            if (stringLen > 1)
            {
                // Replace arrow with null terminator
                SFX_Play_46FA90(SoundEffect::IngameTransition_84, 90);
                sSaveString_5C931C[stringLen -1] = 0;
                field_13C_save_state = SaveState::DoSave_4;
                Input_Reset_492660();
                return;
            }
            SFX_Play_46FA90(SoundEffect::ElectricZap_39, 0);
            return;

        // Backspace - delete
        case VK_BACK:
            if (stringLen > 1)
            {
                // Replace last char with arrow
                sSaveString_5C931C[stringLen -2] = kArrowChar;
                sSaveString_5C931C[stringLen -1] = 0;
                SFX_Play_46FA90(SoundEffect::PickupItem_28, 0);
                return;
            }
            SFX_Play_46FA90(SoundEffect::ElectricZap_39, 0);
            return;
        }

        // Otherwise edit the file name if its an allowed char
        if (strspn(newInput, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 !-"))
        {
            // Don't allow space at the start of the name, and don't allow 2 constitutive spaces.
            if (newInput[0] == ' ' && (stringLen == 1 || sSaveString_5C931C[stringLen -2] == newInput[0]))
            {
                SFX_Play_46FBA0(SoundEffect::PossessEffect_17, 30, 2600);
            }
            // Also don't allow the name length to be over 20 chars
            else if (stringLen > 20)
            {
                SFX_Play_46FBA0(SoundEffect::PossessEffect_17, 30, 2400);
            }
            // Append new input char
            else
            {
                sSaveString_5C931C[stringLen -1] = newInput[0]; // Replace arrow with input
                sSaveString_5C931C[stringLen] = kArrowChar;    // Replace null with arrow
                sSaveString_5C931C[stringLen +1] = 0;           // Append new null
                SFX_Play_46FA90(SoundEffect::RockBounce_26, 0);
            }
        }
        else
        {
            SFX_Play_46FBA0(SoundEffect::PossessEffect_17, 70, 2200);
        }
    }
}

PauseMenu::PauseMenuPage sOverwriteSaveConfirmPage_55E560 =
{
    &PauseMenu::Page_Save_Update_491210,
    &PauseMenu::Page_Save_Render_491660,
    PauseMenu__PageEntryList_Save_Overwrite_Confirm_55E508,
    -1,
    0u,
    0u,
    0u,
    0u,
    0u,
    0u
};


void PauseMenu::Page_Save_Render_491660(PrimHeader** ot, PauseMenuPage* pPage)
{
    PauseMenuPage* pPageToRender = &sOverwriteSaveConfirmPage_55E560;
    if (field_13C_save_state != SaveState::SaveConfirmOverwrite_8)
    {
        pPageToRender = pPage;
    }

    Page_Base_Render_490A50(ot, pPageToRender);
}

void PauseMenu::Page_Status_Update_4916A0()
{
    if (sInputObject_5BD4E0.isHeld(0x300000))
    {
        // Go back to the main page
        field_136_unused = 0;
        field_144_active_menu = sPM_Page_Main_5465B0;
        SFX_Play_46FBA0(SoundEffect::PossessEffect_17, 40, 2400);
    }
}

void PauseMenu::Page_Status_Render_491710(PrimHeader** ot, PauseMenuPage* pPage)
{
    // Render the status icon
    field_158_animation.field_C_render_layer = Layer::eLayer_41;
    field_158_animation.vRender_40B820(180, 100, ot, 0, 0);

    // Render the text
    Page_Base_Render_490A50(ot, pPage);
}

void PauseMenu::Page_Load_Update_490D50()
{
    CHAR saveFileName[40] = {};

    // When F6 is pressed
    if (sQuicksave_LoadNextFrame_5CA4D9)
    {
        Quicksave_LoadActive_4C9170();
        sQuicksave_LoadNextFrame_5CA4D9 = 0;
        word12C_flags &= ~1u;
    }

    const DWORD inputHeld = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;

    // Up one save
    if (inputHeld & InputCommands::Enum::eUp)
    {
        // Don't underflow
        if (sSavedGameToLoadIdx_BB43FC > 0)
        {
            sSavedGameToLoadIdx_BB43FC--;
        }
        SFX_Play_46FBA0(SoundEffect::MenuNavigation_52, 35, 400);
        return;
    }

    // Down one save
    if (inputHeld & InputCommands::Enum::eDown)
    {
        // Don't overflow
        if (sSavedGameToLoadIdx_BB43FC < sTotalSaveFilesCount_BB43E0 - 1)
        {
            sSavedGameToLoadIdx_BB43FC++;
        }
        SFX_Play_46FBA0(SoundEffect::MenuNavigation_52, 35, 400);
        return;
    }

    // Page up saves
    if (inputHeld & 0x20000000)
    {
        sSavedGameToLoadIdx_BB43FC -= 4;

        // Don't underflow
        if (sSavedGameToLoadIdx_BB43FC < 0)
        {
            sSavedGameToLoadIdx_BB43FC = 0;
        }

        SFX_Play_46FBA0(SoundEffect::MenuNavigation_52, 35, 400);
        return;
    }

    // Page down saves
    if (inputHeld & 0x40000000)
    {
        // Don't overflow
        sSavedGameToLoadIdx_BB43FC += 4;
        if (sSavedGameToLoadIdx_BB43FC > sTotalSaveFilesCount_BB43E0 - 1)
        {
            sSavedGameToLoadIdx_BB43FC = sTotalSaveFilesCount_BB43E0 - 1;
        }
        SFX_Play_46FBA0(SoundEffect::MenuNavigation_52, 35, 400);
        return;
    }

    // Load save (enter)
    if (inputHeld & InputCommands::Enum::eUnPause_OrConfirm)
    {
        field_136_unused = 0;
        memcpy(&field_144_active_menu, &sPM_Page_Main_5465B0, sizeof(field_144_active_menu));
        if (sTotalSaveFilesCount_BB43E0)
        {
            strcpy(saveFileName, sSaveFileRecords_BB31D8[sSavedGameToLoadIdx_BB43FC].field_0_fileName);
            strcat(saveFileName, ".sav");
            FILE* hFile = ae_fopen_520C64(saveFileName, "rb");
            if (hFile)
            {
                ae_fread_520B5C(&sActiveQuicksaveData_BAF7F8, sizeof(Quicksave), 1u, hFile);
                sActiveHero_5C1B68->field_B8_xpos = FP_FromInteger(0);
                sActiveHero_5C1B68->field_BC_ypos = FP_FromInteger(0);
                Quicksave_LoadActive_4C9170();
                word12C_flags &= ~1u;
                // TODO: OG bug, file handle is leaked
                ae_fclose_520CBE(hFile);
            }
            SFX_Play_46FA90(SoundEffect::IngameTransition_84, 90);
        }
    }
    // Go back (esc)
    else if (inputHeld & InputCommands::Enum::eBack)
    {
        field_136_unused = 0;
        memcpy(&field_144_active_menu, &sPM_Page_Main_5465B0, sizeof(field_144_active_menu));
        SFX_Play_46FBA0(SoundEffect::PossessEffect_17, 40, 2400);
    }
    // Delete (del)
    else if (inputHeld & 0x10000000)
    {
        if (sTotalSaveFilesCount_BB43E0)
        {
            strcpy(saveFileName, sSaveFileRecords_BB31D8[sSavedGameToLoadIdx_BB43FC].field_0_fileName);
            strcat(saveFileName, ".sav");
            ae_remove_520B27(saveFileName);
            Quicksave_FindSaves_4D4150();
        }
    }
}

void PauseMenu::Page_Load_Render_4910A0(PrimHeader** ot, PauseMenuPage* mp)
{
    s32 saveIdx = sSavedGameToLoadIdx_BB43FC - 2;
    for (s32 i = 0; i < 6; i++)
    {
        if (saveIdx < 0 || saveIdx >= sTotalSaveFilesCount_BB43E0)
        {
            // When at the top of the list set the first 2 entries to a blank, and when at the end of the list set the
            // remaining save text slots to blank.
            PauseMenu__PageEntryList_Load_55e3a0[i].field_8_text = "";
        }
        else
        {
            PauseMenu__PageEntryList_Load_55e3a0[i].field_8_text = sSaveFileRecords_BB31D8[saveIdx].field_0_fileName;
        }
        saveIdx++;
    }

    if (sTotalSaveFilesCount_BB43E0 > 0)
    {
        // This has to be set every time there is at least one save,
        // otherwise a call to this function with no saves hides this row forever
        PauseMenu__PageEntryList_Load_55e3a0[6].field_8_text = kUp " " kDown " Select    Enter Load    Del Delete";
    }
    else
    {
        PauseMenu__PageEntryList_Load_55e3a0[4].field_8_text = "No Saved Games";

        // hide row that displays hotkeys for selecting/loading/deleting saves, since there are no saves
        PauseMenu__PageEntryList_Load_55e3a0[6].field_8_text = "";
    }

    mp->field_C_selected_index = 2;

    Page_Base_Render_490A50(ot, mp);
}

EXPORT WORD CC sub_4A2B70()
{
    NOT_IMPLEMENTED();
    return 1;
}

void PauseMenu::Update_48FD80()
{
    Abe* pHero = sActiveHero_5C1B68;
    BaseAliveGameObject* pControlledChar = nullptr;

    if (sActiveHero_5C1B68->field_10C_health <= FP_FromInteger(0) || sActiveHero_5C1B68->field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        pControlledChar = sControlledCharacter_5C1B8C;
    }
    else
    {
        pControlledChar = sControlledCharacter_5C1B8C;
        if (!(sControlledCharacter_5C1B8C->field_114_flags.Get(e114_Bit10_Teleporting)))
        {
            const s16 heroState = sActiveHero_5C1B68->field_106_current_motion;
            if (heroState != eAbeStates::State_86_HandstoneBegin_45BD00
                && heroState != eAbeStates::State_119_ToShrykull_45A990
                && heroState != eAbeStates::State_120_EndShrykull_45AB00
                && heroState != eAbeStates::State_75_JumpIntoWell_45C7B0
                && heroState != eAbeStates::State_76_ToInsideOfAWellLocal_45CA40
                && heroState != eAbeStates::State_77_ToWellShotOut_45D130
                && heroState != eAbeStates::State_78_WellBegin_45C810
                && heroState != eAbeStates::State_79_InsideWellLocal_45CA60
                && heroState != eAbeStates::State_80_WellShotOut_45D150
                && heroState != eAbeStates::jState_81_WellBegin_45C7F0
                && heroState != eAbeStates::State_82_InsideWellExpress_45CC80
                && heroState != eAbeStates::State_83_WellExpressShotOut_45CF70
                && (sControlledCharacter_5C1B8C->field_4_typeId != AETypes::eEvilFart_45 || LOWORD(static_cast<Abe*>(sControlledCharacter_5C1B8C)->field_124_timer) != 2) // TODO: Cast seems wrong, missing intermediate base class??
                && sActiveHero_5C1B68->field_1A8_portal_id == -1)
            {
                if (sQuicksave_SaveNextFrame_5CA4D8)
                {
                    Quicksave_4C90D0();
                    pHero = sActiveHero_5C1B68;
                    pControlledChar = sControlledCharacter_5C1B8C;
                    sQuicksave_SaveNextFrame_5CA4D8 = 0;
                    sQuicksave_LoadNextFrame_5CA4D9 = 0;
                }
                else if (sQuicksave_LoadNextFrame_5CA4D9)
                {
                    Quicksave_LoadActive_4C9170();
                    SND_SEQ_Stop_4CAE60(SeqId::MudokonChant1_10);
                    pHero = sActiveHero_5C1B68;
                    pControlledChar = sControlledCharacter_5C1B8C;
                    sQuicksave_SaveNextFrame_5CA4D8 = 0;
                    sQuicksave_LoadNextFrame_5CA4D9 = 0;
                }
                else
                {
                    sQuicksave_SaveNextFrame_5CA4D8 = 0;
                    sQuicksave_LoadNextFrame_5CA4D9 = 0;
                }
            }
        }
    }

    if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held & InputCommands::Enum::ePause)
    {
        if (pHero->field_10C_health > FP_FromInteger(0)
            && !(pHero->field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted))
            && !(pControlledChar->field_114_flags.Get(Flags_114::e114_Bit10_Teleporting)))
        {
            const short heroState = pHero->field_106_current_motion;
            if (heroState != eAbeStates::State_86_HandstoneBegin_45BD00
                && heroState != eAbeStates::State_119_ToShrykull_45A990
                && heroState != eAbeStates::State_120_EndShrykull_45AB00
                && heroState != eAbeStates::State_75_JumpIntoWell_45C7B0
                && heroState != eAbeStates::State_76_ToInsideOfAWellLocal_45CA40
                && heroState != eAbeStates::State_77_ToWellShotOut_45D130
                && heroState != eAbeStates::State_78_WellBegin_45C810
                && heroState != eAbeStates::State_79_InsideWellLocal_45CA60
                && heroState != eAbeStates::State_80_WellShotOut_45D150
                && heroState != eAbeStates::jState_81_WellBegin_45C7F0
                && heroState != eAbeStates::State_82_InsideWellExpress_45CC80
                && heroState != eAbeStates::State_83_WellExpressShotOut_45CF70
                && (pControlledChar->field_4_typeId != AETypes::eEvilFart_45 || LOWORD(static_cast<Abe*>(pControlledChar)->field_124_timer) != 2) // TODO: Why LOWORD only ?? TODO: Cast seems wrong, missing intermediate base class??
                && pHero->field_1A8_portal_id == -1)
            {
                SND_StopAll_4CB060();
                SFX_Play_46FBA0(SoundEffect::PossessEffect_17, 40, 2400);
                sub_4A2B70();
                field_6_flags.Set(BaseGameObject::eDrawable_Bit4);
                field_134_index_main = MainPages::ePage_Continue_0;
                field_136_unused = 0;
                word12C_flags = (word12C_flags & ~8) | 1;
                field_12E_selected_glow = 40;
                field_130_selected_glow_counter = 8;
                Path_Format_CameraName_460FB0(
                    sScreenStringBuffer_5C92F0,
                    gMap_5C3030.field_0_current_level,
                    gMap_5C3030.field_2_current_path,
                    gMap_5C3030.field_4_current_camera);

                // TODO: never read ??
                //byte_5C92F8 = 0;

                for (size_t i = 0; i < strlen(sScreenStringBuffer_5C92F0); i++)
                {
                    char currentCamChar = sScreenStringBuffer_5C92F0[i];
                    if (currentCamChar == '.') // Chop off .CAM
                    {
                        sScreenStringBuffer_5C92F0[i] = 0;
                        break;
                    }

                    if (currentCamChar >= '0' && currentCamChar <= '9')
                    {
                        // "Lower" case numbers in the font atlas
                        sScreenStringBuffer_5C92F0[i] = currentCamChar - 58;
                    }
                    else
                    {
                        sScreenStringBuffer_5C92F0[i] =  static_cast<char>(::tolower(currentCamChar));
                    }
                }

                // TODO: How do these ever get read ??
                sprintf(sPauseMenu_Of300Mudokons_55E718, "%d OF 300 MUDOKONS", sRescuedMudokons_5C1BC2);
                sprintf(sHasBeenTerminated_55E738, "%d HA%s BEEN TERMINATED", sKilledMudokons_5C1BC0, (sKilledMudokons_5C1BC0 != 1) ? "VE" : "S");

                if (sActiveHero_5C1B68->field_128.field_12_mood == Mud_Emotion::eNormal_0)
                {
                    field_158_animation.Set_Animation_Data_409C80(1248, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kNormaliconResID, 1u, 0));
                }
                else if (sActiveHero_5C1B68->field_128.field_12_mood == Mud_Emotion::eSad_3)
                {
                    field_158_animation.Set_Animation_Data_409C80(1076, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAngryiconResID, 1u, 0));
                }
                else if (sActiveHero_5C1B68->field_128.field_12_mood == Mud_Emotion::eHappy_5)
                {
                    field_158_animation.Set_Animation_Data_409C80(1252, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kHappyiconResID, 1u, 0));
                }

                FrameInfoHeader* pFrameInfoHeader = field_158_animation.Get_FrameHeader_40B730(0);
                FrameHeader* pHeader = reinterpret_cast<FrameHeader*>(&(*field_158_animation.field_20_ppBlock)[pFrameInfoHeader->field_0_frame_header_offset]);

                field_158_animation.Load_Pal_40A530(field_158_animation.field_20_ppBlock, pHeader->field_0_clut_offset);
                sDisableFontFlicker_5C9304 = 1;
                memcpy(&field_144_active_menu, &sPM_Page_Main_5465B0, sizeof(field_144_active_menu));
                if (word12C_flags & 1)
                {
                    // Start pause menu update/render loop
                    do
                    {
                        if (pResourceManager_5C1BB0)
                        {
                            pResourceManager_5C1BB0->VUpdate();
                        }

                        SYS_EventsPump_494580();

                        for (s32 i = 0; i < gObjList_drawables_5C1124->Size(); i++)
                        {
                            BaseGameObject* pObj = gObjList_drawables_5C1124->ItemAt(i);
                            if (!pObj)
                            {
                                break;
                            }

                            if (!(pObj->field_6_flags.Get(BaseGameObject::eDead_Bit3)))
                            {
                                if (pObj->field_6_flags.Get(BaseGameObject::eDrawable_Bit4))
                                {
                                    pObj->VRender(gPsxDisplay_5C1130.field_10_drawEnv[gPsxDisplay_5C1130.field_C_buffer_index].field_70_ot_buffer);
                                }
                            }
                        }

                        pScreenManager_5BB5F4->VRender(gPsxDisplay_5C1130.field_10_drawEnv[gPsxDisplay_5C1130.field_C_buffer_index].field_70_ot_buffer);

                        PSX_DrawSync_4F6280(0);
                        ResourceManager::Reclaim_Memory_49C470(500000);
                        gPsxDisplay_5C1130.PSX_Display_Render_OT_41DDF0();
                        sInputObject_5BD4E0.Update_45F040();

                        if (field_130_selected_glow_counter > 0)
                        {
                            field_12E_selected_glow += 8;
                        }

                        if (field_12E_selected_glow <= 120 || field_130_selected_glow_counter <= 0)
                        {
                            if (field_130_selected_glow_counter <= 0)
                            {
                                field_12E_selected_glow -= 8;
                                if (field_12E_selected_glow < 40)
                                {
                                    field_130_selected_glow_counter = -field_130_selected_glow_counter;
                                    field_12E_selected_glow += field_130_selected_glow_counter;
                                }
                            }
                        }
                        else
                        {
                            field_130_selected_glow_counter = -field_130_selected_glow_counter;
                            field_12E_selected_glow += field_130_selected_glow_counter;
                        }

                        (this->*field_144_active_menu.field_0_fn_update)();
                    } while (word12C_flags & 1);
                }
                sInputObject_5BD4E0.Update_45F040();
                field_6_flags.Clear(BaseGameObject::eDrawable_Bit4);
            }
        }
    }
    sDisableFontFlicker_5C9304 = 0;
}

ALIVE_VAR(1, 0x5C9300, PauseMenu*, pPauseMenu_5C9300, nullptr);
