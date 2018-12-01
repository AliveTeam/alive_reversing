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
#include "Sound.hpp"
#include "MainMenu.hpp"
#include "ThrowableArray.hpp"

// MACROS

#define MENU_OVERRIDE(name) memcpy(_MenuAddress##name##, &sPauseMenuList_##name, sizeof(sPauseMenuList_##name));
#define MENU_VAR(addr, name) ALIVE_VAR(1, 0x##addr, t_sPauseMenuList_##name, PageEntryList_##name##_##addr, sPauseMenuList_##name);

#define MENU_BEGIN(name) struct t_sPauseMenuList_##name{
#define MENU_ENTRY(text, x, y, align) PauseMenuPageEntry Menu_##x##y##align = { 2, x, y, 0, text, 0x80, 0x10, 0xFF, align};
#define MENU_ENTRY_EX(text, x, y, align, r, g, b) PauseMenuPageEntry Menu_##x##y##align##r##g##b = { 2, x, y, 0, text, r, g, b, align};
#define MENU_END(addr, name) PauseMenuPageEntry MenuEnd = {  0, 0, 0, 0, nullptr, 0, 0, 0, 0 };}; t_sPauseMenuList_##name sPauseMenuList_##name;\
MENU_VAR(addr, name);\
void * _MenuAddress##name = reinterpret_cast<void*>(0x##addr);

//

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

MENU_BEGIN(MainMenu);
MENU_ENTRY("continue", 184, 48, Centre);
MENU_ENTRY("quicksave", 184, 70, Centre);
#if DEVELOPER_MODE
MENU_ENTRY_EX("developer", 184, 92, Centre, 33, 127, 33);
#else
MENU_ENTRY("controls", 184, 92, Centre);
#endif
MENU_ENTRY("status", 184, 114, Centre);
MENU_ENTRY("save", 184, 136, Centre);
MENU_ENTRY("load", 184, 158, Centre);
MENU_ENTRY("restart path", 184, 180, Centre);
MENU_ENTRY("quit", 184, 202, Centre);
MENU_ENTRY("paused", 184, 16, Centre);
MENU_ENTRY(sScreenStringBuffer_5C92F0, 280, 16, Left);
MENU_END(55E1C8, MainMenu);

MENU_BEGIN(Menu_ControlActions);
MENU_ENTRY_EX("more", 184, 205, Centre, 128, 16, 255);
MENU_ENTRY_EX("Actions", 184, 20, Centre, 127, 127, 127);
MENU_ENTRY_EX("\x05+\x03\x04", 180, 50, Right, 160, 160, 160);
MENU_ENTRY_EX("\x07+\x03\x04", 180, 70, Right, 160, 160, 160);
MENU_ENTRY_EX("\x09", 180, 90, Right, 160, 160, 160);
MENU_ENTRY_EX("\x08+\x1a", 180, 110, Right, 160, 160, 160);
MENU_ENTRY_EX("\x02", 180, 135, Right, 160, 160, 160);
MENU_ENTRY_EX("\x06", 180, 150, Right, 160, 160, 160);
MENU_ENTRY_EX("\x01", 180, 173, Right, 160, 160, 160);
MENU_ENTRY_EX("run", 200, 50, Left, 128, 16, 255);
MENU_ENTRY_EX("sneak", 200, 70, Left, 128, 16, 255);
MENU_ENTRY_EX("jump", 200, 90, Left, 128, 16, 255);
MENU_ENTRY_EX("throw", 200, 110, Left, 128, 16, 255);
MENU_ENTRY_EX("crouch", 200, 130, Left, 128, 16, 255);
MENU_ENTRY_EX("action", 200, 150, Left, 128, 16, 255);
MENU_ENTRY_EX("hoist \x1b zturn", 200, 170, Left, 128, 16, 255);
MENU_END(55d820, Menu_ControlActions);

MENU_BEGIN(Menu_GameSpeak);
MENU_ENTRY_EX("more", 184, 205, Centre, 128, 16, 255);
MENU_ENTRY_EX("GameSpeak", 184, 20, Centre, 127, 127, 127);
MENU_ENTRY_EX("\x13", 184, 55, Centre, 160, 160, 160);
MENU_ENTRY_EX("chant", 184, 75, Centre, 128, 16, 255);
MENU_ENTRY_EX("hello", 110, 104, Left, 128, 16, 255);
MENU_ENTRY_EX("follow me", 110, 126, Left, 128, 16, 255);
MENU_ENTRY_EX("wait", 110, 148, Left, 128, 16, 255);
MENU_ENTRY_EX("work", 110, 170, Left, 128, 16, 255);
MENU_ENTRY_EX("anger", 278, 104, Left, 128, 16, 255);
MENU_ENTRY_EX("all 'ya", 278, 126, Left, 128, 16, 255);
MENU_ENTRY_EX("sympathy", 278, 148, Left, 128, 16, 255);
MENU_ENTRY_EX("stop it!", 278, 170, Left, 128, 16, 255);
MENU_ENTRY_EX("\x0b", 90, 104, Right, 160, 160, 160);
MENU_ENTRY_EX("\x0c", 90, 126, Right, 160, 160, 160);
MENU_ENTRY_EX("\x0d", 90, 148, Right, 160, 160, 160);
MENU_ENTRY_EX("\x0e", 90, 170, Right, 160, 160, 160);
MENU_ENTRY_EX("\x0f", 260, 104, Right, 160, 160, 160);
MENU_ENTRY_EX("\x10", 260, 126, Right, 160, 160, 160);
MENU_ENTRY_EX("\x11", 260, 148, Right, 160, 160, 160);
MENU_ENTRY_EX("\x12", 260, 170, Right, 160, 160, 160);
MENU_END(55d930, Menu_GameSpeak);

MENU_BEGIN(Menu_SligSpeak);
MENU_ENTRY_EX("more", 184, 205, Centre, 128, 16, 255);
MENU_ENTRY_EX("SligSpeak", 184, 20, Centre, 127, 127, 127);
MENU_ENTRY_EX("hi", 110, 104, Left, 128, 16, 255);
MENU_ENTRY_EX("here boy", 110, 126, Left, 128, 16, 255);
MENU_ENTRY_EX("freeze", 110, 148, Left, 128, 16, 255);
MENU_ENTRY_EX("get 'im", 110, 170, Left, 128, 16, 255);
MENU_ENTRY_EX("s'mo bs", 278, 104, Left, 128, 16, 255);
MENU_ENTRY_EX("bs", 278, 126, Left, 128, 16, 255);
MENU_ENTRY_EX("look out", 278, 148, Left, 128, 16, 255);
MENU_ENTRY_EX("laugh", 278, 170, Left, 128, 16, 255);
MENU_ENTRY_EX("\x0b", 90, 104, Right, 160, 160, 160);
MENU_ENTRY_EX("\x0c", 90, 126, Right, 160, 160, 160);
MENU_ENTRY_EX("\x0d", 90, 148, Right, 160, 160, 160);
MENU_ENTRY_EX("\x0e", 90, 170, Right, 160, 160, 160);
MENU_ENTRY_EX("\x0f", 260, 104, Right, 160, 160, 160);
MENU_ENTRY_EX("\x10", 260, 126, Right, 160, 160, 160);
MENU_ENTRY_EX("\x11", 260, 148, Right, 160, 160, 160);
MENU_ENTRY_EX("\x12", 260, 170, Right, 160, 160, 160);
MENU_END(55da80, Menu_SligSpeak);

MENU_BEGIN(Menu_GlukkonSpeak);
MENU_ENTRY_EX("more", 184, 205, Centre, 128, 16, 255);
MENU_ENTRY_EX("GlukkonSpeak", 184, 20, Centre, 127, 127, 127);
MENU_ENTRY_EX("hey!", 110, 104, Left, 128, 16, 255);
MENU_ENTRY_EX("commere", 110, 126, Left, 128, 16, 255);
MENU_ENTRY_EX("stay here", 110, 148, Left, 128, 16, 255);
MENU_ENTRY_EX("do it!", 110, 170, Left, 128, 16, 255);
MENU_ENTRY_EX("kill'em!", 278, 104, Left, 128, 16, 255);
MENU_ENTRY_EX("all o'ya", 278, 126, Left, 128, 16, 255);
MENU_ENTRY_EX("help!", 278, 148, Left, 128, 16, 255);
MENU_ENTRY_EX("laugh", 278, 170, Left, 128, 16, 255);
MENU_ENTRY_EX("\x0b", 90, 104, Right, 160, 160, 160);
MENU_ENTRY_EX("\x0c", 90, 126, Right, 160, 160, 160);
MENU_ENTRY_EX("\x0d", 90, 148, Right, 160, 160, 160);
MENU_ENTRY_EX("\x0e", 90, 170, Right, 160, 160, 160);
MENU_ENTRY_EX("\x0f", 260, 104, Right, 160, 160, 160);
MENU_ENTRY_EX("\x10", 260, 126, Right, 160, 160, 160);
MENU_ENTRY_EX("\x11", 260, 148, Right, 160, 160, 160);
MENU_ENTRY_EX("\x12", 260, 170, Right, 160, 160, 160);
MENU_END(55dbb0, Menu_GlukkonSpeak);

MENU_BEGIN(Menu_ParamiteSpeak);
MENU_ENTRY_EX("more", 184, 205, Centre, 128, 16, 255);
MENU_ENTRY_EX("ParamiteSpeak", 184, 20, Centre, 127, 127, 127);
MENU_ENTRY_EX("howdy", 110, 104, Left, 128, 16, 255);
MENU_ENTRY_EX("c'mon", 110, 126, Left, 128, 16, 255);
MENU_ENTRY_EX("stay", 110, 148, Left, 128, 16, 255);
MENU_ENTRY_EX("do it", 110, 170, Left, 128, 16, 255);
MENU_ENTRY_EX("attack", 278, 104, Left, 128, 16, 255);
MENU_ENTRY_EX("all a ya!", 278, 126, Left, 128, 16, 255);
MENU_ENTRY_EX("\x0b", 90, 104, Right, 160, 160, 160);
MENU_ENTRY_EX("\x0c", 90, 126, Right, 160, 160, 160);
MENU_ENTRY_EX("\x0d", 90, 148, Right, 160, 160, 160);
MENU_ENTRY_EX("\x0e", 90, 170, Right, 160, 160, 160);
MENU_ENTRY_EX("\x0f", 260, 104, Right, 160, 160, 160);
MENU_ENTRY_EX("\x10", 260, 126, Right, 160, 160, 160);
MENU_END(55dce0, Menu_ParamiteSpeak);

MENU_BEGIN(Menu_ScrabSpeak);
MENU_ENTRY_EX("exit", 184, 205, Centre, 128, 16, 255);
MENU_ENTRY_EX("ScrabSpeak", 184, 20, Centre, 127, 127, 127);
MENU_ENTRY_EX("shred power", 110, 104, Left, 128, 16, 255);
MENU_ENTRY_EX("howl", 110, 126, Left, 128, 16, 255);
MENU_ENTRY_EX("\x0b", 90, 104, Right, 160, 160, 160);
MENU_ENTRY_EX("\x0c", 90, 126, Right, 160, 160, 160);
MENU_END(55ddd0, Menu_ScrabSpeak);

MENU_BEGIN(Menu_Save);
MENU_ENTRY_EX("", 184, 120, Centre, 128, 16, 255);
MENU_ENTRY_EX("Enter   Save", 184, 180, Centre, 128, 16, 255);
MENU_ENTRY_EX("Esc   Cancel", 184, 205, Centre, 128, 16, 255);
MENU_END(55e4c8, Menu_Save);

MENU_BEGIN(Menu_ReallyQuit);
MENU_ENTRY_EX("REALLY QUIT?", 184, 110, Centre, 128, 16, 255);
MENU_ENTRY_EX("Enter yes   Esc no", 184, 135, Centre, 160, 160, 160);
MENU_END(55e278, Menu_ReallyQuit);

MENU_BEGIN(Menu_Status);
MENU_END(55e738, Menu_Status);

MENU_BEGIN(Menu_Load);
MENU_ENTRY_EX("", 184, 5, Centre, 128, 16, 255);
MENU_ENTRY_EX("", 184, 30, Centre, 128, 16, 255);
MENU_ENTRY_EX("", 184, 55, Centre, 128, 16, 255);
MENU_ENTRY_EX("", 184, 80, Centre, 128, 16, 255);
MENU_ENTRY_EX("", 184, 105, Centre, 128, 16, 255);
MENU_ENTRY_EX("", 184, 130, Centre, 128, 16, 255);
MENU_ENTRY_EX("\x01 \x02 Select    Enter Load    Del Delete", 184, 188, Centre, 128, 16, 255);
MENU_ENTRY_EX("Esc  Cancel        F6  Load QuikSave", 184, 213, Centre, 128, 16, 255);
MENU_END(55e3a0, Menu_Load);

//ALIVE_VAR(1, 0x, t_sPauseMenuList_MainMenu, PageEntryList_MainMenu_55E1C8, sPauseMenuList_MainMenu);
//ALIVE_VAR(1, 0x, t_sPauseMenuList_ReallyQuit, PageEntryList_ReallyQuit_55E278, sPauseMenuList_ReallyQuit);

PauseMenuPageEntry PauseMenu__PageEntryList_Main_55E1C8[11] =
{
    { 2, 184, 48, 0, "continue", 128u, 16u, 255u, 1u },
    { 2, 184, 70, 0, "quiksave", 128u, 16u, 255u, 1u },
    { 2, 184, 92, 0, "controls", 128u, 16u, 255u, 1u },
    { 2, 184, 114, 0, "status", 128u, 16u, 255u, 1u },
    { 2, 184, 136, 0, "save", 128u, 16u, 255u, 1u },
    { 2, 184, 158, 0, "load", 128u, 16u, 255u, 1u },
    { 2, 184, 180, 0, "restart path", 128u, 16u, 255u, 1u },
    { 2, 184, 202, 0, "quit", 128u, 16u, 255u, 1u },
    { 1, 184, 16, 0, "paused", 128u, 16u, 255u, 1u },
    { 1, 280, 16, 0, sScreenStringBuffer_5C92F0, 128u, 16u, 255u, 0u },
    { 0, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u }
};


PauseMenu::PauseMenuPage sPM_Page_Main = 
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

PauseMenuPageEntry PauseMenu__PageEntryList_ControlsActions_55D820[17] =
{
    { 2, 184, 205, 0, "more", 128u, 16u, 255u, 1u },
    { 1, 184, 20, 0, "Actions", 127u, 127u, 127u, 1u },
    { 1, 180, 50, 0, "\x5\x2B\x3\x4", 160u, 160u, 160u, 2u },
    { 1, 180, 70, 0, "\x7\x2B\x3\x4", 160u, 160u, 160u, 2u },
    { 1, 180, 90, 0, "\t", 160u, 160u, 160u, 2u },
    { 1, 180, 110, 0, "\x8\x2B\x1A", 160u, 160u, 160u, 2u },
    { 1, 180, 135, 0, "\x2", 160u, 160u, 160u, 2u },
    { 1, 180, 150, 0, "\x6", 160u, 160u, 160u, 2u },
    { 1, 180, 173, 0, "\x1", 160u, 160u, 160u, 2u },
    { 1, 200, 50, 0, "run", 128u, 16u, 255u, 0u },
    { 1, 200, 70, 0, "sneak", 128u, 16u, 255u, 0u },
    { 1, 200, 90, 0, "jump", 128u, 16u, 255u, 0u },
    { 1, 200, 110, 0, "throw", 128u, 16u, 255u, 0u },
    { 1, 200, 130, 0, "crouch", 128u, 16u, 255u, 0u },
    { 1, 200, 150, 0, "action", 128u, 16u, 255u, 0u },
    { 1, 200, 170, 0, "hoist \x1B zturn", 128u, 16u, 255u, 0u },
    { 0, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u }
};

PauseMenu::PauseMenuPage sPM_Page_Controls_Actions =
{
    &PauseMenu::Page_ControlsActions_Update_48FA60,
    &PauseMenu::Page_Base_Render_490A50,
    PauseMenu__PageEntryList_ControlsActions_55D820,
    0,
    100u,
    100u,
    100u,
    0u,
    0u,
    0u
};

PauseMenuPageEntry PauseMenu__PageEntryList_Save_55E4C8 = { 1, 184, 120, 0, sSaveString_5C931C, 128u, 16u, 255u, 1u };

/*
PauseMenu::PauseMenuPage sPM_Page_Save =
{
    &PauseMenu::Page_Save_Update_491210,
    &PauseMenu::tsub_491660,
    PauseMenu__PageEntryList_Save_55E4C8,
    0,
    160u,
    160u,
    160u,
    0u,
    0u,
    0u
};
*/

PauseMenuPageEntry PauseMenu__PageEntryList_ReallyQuit_55E278[3] =
{
    { 1, 184, 110, 0, "REALLY QUIT?", 128u, 16u, 255u, 1u },
    { 1, 184, 135, 0, "Enter yes   Esc no", 160u, 160u, 160u, 1u },
    { 0, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u }
};


PauseMenu::PauseMenuPage sPM_Page_ReallyQuit =
{
    &PauseMenu::Page_ReallyQuit_Update_490930,
    &PauseMenu::Page_Base_Render_490A50,
    PauseMenu__PageEntryList_ReallyQuit_55E278,
    -1,
    100u,
    100u,
    100u,
    0u,
    0u,
    0u
};

// TODO: SET VALUES
ALIVE_VAR(1, 0x5465B0, PauseMenu::PauseMenuPage, sPM_Page_Main_5465B0, { sPM_Page_Main });
ALIVE_VAR(1, 0x546610, PauseMenu::PauseMenuPage, sPM_Page_Controls_Actions_546610, { sPM_Page_Controls_Actions });
ALIVE_VAR(1, 0x546628, PauseMenu::PauseMenuPage, sPM_Page_Load_546628, {});
ALIVE_VAR(1, 0x5465F8, PauseMenu::PauseMenuPage, sPM_Page_Status_5465F8, {});
ALIVE_VAR(1, 0x5465E0, PauseMenu::PauseMenuPage, sPM_Page_ReallyQuit_5465E0, { sPM_Page_ReallyQuit });
ALIVE_VAR(1, 0x5465C8, PauseMenu::PauseMenuPage, sPM_Page_Save_5465C8, { });

// TODO: Populate
ALIVE_ARY(1, 0x55DE40, PauseMenuPageEntry*, 6, sControlActionsPages_55DE40, {});

struct DumpEntry
{
    int address;
    std::string name;
};

void DumpMenus()
{
    std::vector<DumpEntry> menuListArrays;
    menuListArrays.push_back({ 0x55D820, "ControlActions" });
    menuListArrays.push_back({ 0x55D930, "GameSpeak" });
    menuListArrays.push_back({ 0x55DA80, "SligSpeak" });
    menuListArrays.push_back({ 0x55DBB0, "GlukkonSpeak" });
    menuListArrays.push_back({ 0x55DCE0, "ParamiteSpeak" });
    menuListArrays.push_back({ 0x55DDD0, "ScrabSpeak" });

    menuListArrays.push_back({ 0x55E4C8, "Save" });
    menuListArrays.push_back({ 0x55E278, "ReallyQuit" });
    menuListArrays.push_back({ 0x55E738, "Status" });
    menuListArrays.push_back({ 0x55E3A0, "Load" });

    // menuListArrays.push_back({ 0x55DDD0, "" });

    std::stringstream output;
    std::stringstream output_override;

    for (auto a : menuListArrays)
    {
        std::stringstream menuAddr;
        menuAddr << std::hex << a.address;
        
        output << "MENU_BEGIN(Menu_" + a.name + ");\n";
        
        PauseMenuPageEntry * e = reinterpret_cast<PauseMenuPageEntry *>(a.address);

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
            output << "MENU_ENTRY_EX(\"" << EscapeUnknownCharacters(e->field_8_text) << "\", " << e->field_2_x << ", " << e->field_4_y << ", " + alignment + ", " << (int)e->field_C_r << ", " << (int)e->field_D_g << ", " << (int)e->field_E_b << ");\n";
            e++;
        }

        output << "MENU_END(" + menuAddr.str() + ", Menu_" + a.name + ");\n\n";

        output_override << "MENU_OVERRIDE(Menu_" + a.name + ");\n";
    }

    std::ofstream fileOut("menu_dump.h");
    fileOut << output.rdbuf() << "\n\n" << output_override.rdbuf();
}

EXPORT signed __int16 sub_4A2BC0()
{
    NOT_IMPLEMENTED();
    return 0;
}

BaseGameObject* PauseMenu::VDestructor(signed int flags)
{
    return vdtor_48FCB0(flags);
}

void PauseMenu::VUpdate()
{
    Update_48FD80();
}

void PauseMenu::VRender(int** pOrderingTable)
{
    Render_490BD0(pOrderingTable);
}

void PauseMenu::VScreenChanged()
{
    vsub_490D30();
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

    field_4_typeId = Types::ePauseMenu_95;
    field_6_flags.Clear(BaseGameObject::eDrawable);
    field_6_flags.Set(BaseGameObject::eBit08);
    word12C_flags &= ~0xE;
    word12C_flags &= ~1u;

    field_1C_update_delay = 25;

    gObjList_drawables_5C1124->Push_Back(this);

    field_136 = 0;
    field_138 = 0;
    field_13A = 0;
    field_13C = 0;
    field_13E = 0;
    field_140 = 0;

    field_F4_font.ctor_433590(256, pal_554474, &sFont1Context_5BC5C8);
    
    Init_491760();
    sub_4A2BC0();
    
    sDisableFontFlicker_5C9304 = 0;
    
    return this;
}

void PauseMenu::dtor_48FCE0()
{
    SetVTable(this, 0x546658);

    field_6_flags.Clear(BaseGameObject::eDrawable);

    gObjList_drawables_5C1124->Remove_Item(this);
    field_158_animation.vCleanUp_40C630();
    field_F4_font.dtor_433540();
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

BaseGameObject* PauseMenu::vdtor_48FCB0(signed int flags)
{
    dtor_48FCE0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void PauseMenu::Init_491760()
{
    ResourceManager::LoadResourceFile_49C170("EMOHAP.BAN", 0);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, 201);
    ResourceManager::LoadResourceFile_49C170("EMOANGRY.BAN", 0);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, 202);
    ResourceManager::LoadResourceFile_49C170("EMONORM.BAN", 0);
    BYTE **v2 = Add_Resource_4DC130(ResourceManager::Resource_Animation, 203);

    if (field_158_animation.Init_40A030(1248, gObjList_animations_5C1A24, this, 54, 47, v2, 1, 0, 0))
    {
        this->field_158_animation.field_C_render_layer = field_20_animation.field_C_render_layer;
        this->field_158_animation.field_14_scale = field_CC_sprite_scale;
        this->field_158_animation.field_8_r = 127;
        this->field_158_animation.field_9_g = 127;
        this->field_158_animation.field_A_b = 127;
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eListAddFailed);
    }
}

EXPORT void CC sub_4C9870()
{
    NOT_IMPLEMENTED();
}

void PauseMenu::Render_490BD0(int ** ot)
{
    field_142 = 0;

    // Render the page
    (this->*field_144_active_menu.field_4_fn_render)(ot, &field_144_active_menu);
    
    // Draw a full screen polygon that "dims" out the screen while paused
    Prim_SetTPage* pTPage = &field_1F0[gPsxDisplay_5C1130.field_C_buffer_index];
    Poly_F4* pPolys = &field_210[gPsxDisplay_5C1130.field_C_buffer_index];
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
    Init_SetTPage_4F5B60(pTPage, 0, 0, PSX_getTPage_4F60E0(0, 2, 0, 0));
    OrderingTable_Add_4F8AA0(&ot[41], &pPolys->mBase.header);
    OrderingTable_Add_4F8AA0(&ot[41], &pTPage->mBase);
    pScreenManager_5BB5F4->InvalidateRect_40EC90(0, 0, 640, 240, pScreenManager_5BB5F4->field_3A_idx);
}

EXPORT void PauseMenu::vsub_490D30()
{
    if (gMap_5C3030.field_A_5C303A_levelId == 16)
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}

#if DEVELOPER_MODE
// CUSTOM PAUSE MENU

class CustomPauseMenu;
struct CustomPauseMenuItem
{
    const char * text;
    std::function<void(CustomPauseMenu *)> callback;
};

std::vector<CustomPauseMenu *> customMenuStack;

class CustomPauseMenu
{
public:
    CustomPauseMenu(std::vector<CustomPauseMenuItem> * items, char * titleStr)
    {
        entries = items;
        title = std::string(titleStr);
        CompileEntries();
    }

    void CompileEntries()
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

        int o = 0;
        int size = entries->size();
        for (int i = scrollDownIndex; i < min(size, scrollDownIndex + 7); i++)
        {
            const auto item = (*entries)[i];
            compiledEntries.push_back({ 0, 184, (short)(57 + (22 * o)), 0, (char*)item.text, 0x80, 0x10, 0xFF, Centre });
            o++;
        }
        compiledEntries.push_back({ 0, 184, 16, 0, (char*)title.c_str(), 127, 127, 127, Centre });
        if (size > 7 && index < size - 2)
        {
            compiledEntries.push_back({ 0, 184, 210, 0, "\x02", 127, 127, 127, Centre });
        }
        if (index > 5)
        {
            compiledEntries.push_back({ 0, 184, 42, 0, "\x01", 127, 127, 127, Centre });
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

    void SetText(char * text)
    {
        (*entries)[index].text = text;
        compiledEntries[index].field_8_text = text;
    }

    void ClosePauseMenu()
    {
        pPauseMenu_5C9300->word12C_flags &= ~1;
        SFX_Play_46FBA0(0x11u, 40, 2400, 0x10000);
        SND_Restart_4CB0E0();
        customMenuStack.clear();
    }

    void Update(PauseMenu * pm)
    {
        auto input = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;
        if (input & eDown)
        {
            if (++index >= static_cast<int>(entries->size()))
                index = 0;
            SFX_Play_46FBA0(0x34u, 45, 400, 0x10000);

            CompileEntries();
        }
        else if (input & eUp)
        {
            if (--index < 0)
                index = entries->size() - 1;
            SFX_Play_46FBA0(0x34u, 45, 400, 0x10000);

            CompileEntries();
        }
        else if (input & 0x200000)
        {
            SFX_Play_46FBA0(0x11u, 40, 2400, 0x10000);
            GoBack(pm);
        }
        else if (input & eUnPause)
        {
            (*entries)[index].callback(this);
            SFX_Play_46FA90(0x54u, 90, 0x10000);
        }

        pm->field_144_active_menu.field_C_selected_index = static_cast<short>(index - scrollDownIndex);
    }

    void Activate()
    {
        CompileEntries();
        customMenuStack.push_back(this);
        SetLastPageStack(pPauseMenu_5C9300);
    }

    void GoBack(PauseMenu * pm)
    {
        customMenuStack.pop_back();
        SetLastPageStack(pm);
    }

    void SetLastPageStack(PauseMenu * pm)
    {
        if (customMenuStack.size() > 0)
            memcpy(&pm->field_144_active_menu, &customMenuStack[customMenuStack.size() - 1]->mMenuPage, sizeof(pm->field_144_active_menu));
        else
            memcpy(&pm->field_144_active_menu, &sPM_Page_Main_5465B0, sizeof(pm->field_144_active_menu));
    }

public:
    std::vector<CustomPauseMenuItem> * entries;
    int index = 0;
    int scrollDownIndex = 0;
    int maxScrollDown = 5;
    std::string title;
    std::vector<PauseMenuPageEntry> compiledEntries;
    PauseMenu::PauseMenuPage mMenuPage;
};

std::vector<CustomPauseMenuItem> devTeleportMenuItems;
CustomPauseMenu devTeleportMenu(&devTeleportMenuItems, "Level Select");

void DestroyAliveObjects()
{
    for (int i = 0; i < gObjList_drawables_5C1124->Size(); i++)
    {
        BaseGameObject* pObj = gObjList_drawables_5C1124->ItemAt(i);
        if (!pObj || pObj == pPauseMenu_5C9300 || pObj->field_6_flags.Get(BaseGameObject::eIsBaseAliveGameObject) == false)
        {
            continue;
        }

        if (pObj->field_4_typeId != BaseGameObject::eType_Abe_69)
        {
            pObj->field_6_flags.Set(BaseGameObject::eDead);
        }
    }

    sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
}

void DestroyAllObjects()
{
    for (int i = 0; i < gObjList_drawables_5C1124->Size(); i++)
    {
        BaseGameObject* pObj = gObjList_drawables_5C1124->ItemAt(i);
        if (!pObj || pObj == pPauseMenu_5C9300)
        {
            continue;
        }

        if (pObj->field_4_typeId != BaseGameObject::Types::eType_Abe_69)
        {
            pObj->field_6_flags.Set(BaseGameObject::eDead);
        }
    }

    sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
}

std::vector<CustomPauseMenuItem> devCheatsMenuItems({
    { "Destroy Alive Objs", [](CustomPauseMenu * pm ) { DestroyAliveObjects(); pm->ClosePauseMenu(); DEV_CONSOLE_MESSAGE("Destroyed Alive Objects", 4); } },
    { "Destroy All Objects", [](CustomPauseMenu * pm) { DestroyAllObjects(); pm->ClosePauseMenu(); DEV_CONSOLE_MESSAGE("Destroyed All Objects", 4); } },
    { "Save All Mudokons", [](CustomPauseMenu * ) { sRescuedMudokons_5C1BC2 = 300; sKilledMudokons_5C1BC0 = 0; DEV_CONSOLE_MESSAGE("(CHEAT) Saved all Mudokons", 4); } },
    { "Kill All Mudokons", [](CustomPauseMenu * ) { sRescuedMudokons_5C1BC2 = 0; sKilledMudokons_5C1BC0 = 300; DEV_CONSOLE_MESSAGE("(CHEAT) Killed all Mudokons", 4); }  },
    { "Give Rocks", [](CustomPauseMenu * ) { sActiveHero_5C1B68->field_1A2_rock_or_bone_count = 99; DEV_CONSOLE_MESSAGE("(CHEAT) Got Bones", 4); }  },
    { "Open All Doors", [](CustomPauseMenu * pm) {  pm->ClosePauseMenu(); Cheat_OpenAllDoors(); } },
});
CustomPauseMenu devCheatsMenu(&devCheatsMenuItems, "Cheats");

std::vector<CustomPauseMenuItem> devMenuItems({
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
    if (IsAlive())
    {
        //DumpMenus();

        // Overwrites game menu lists with ours, so we can see if everything is the same.
        MENU_OVERRIDE(MainMenu);
        //MENU_OVERRIDE(Menu_ControlActions);
        //MENU_OVERRIDE(Menu_GameSpeak);
        //MENU_OVERRIDE(Menu_SligSpeak);
        //MENU_OVERRIDE(Menu_GlukkonSpeak);
        //MENU_OVERRIDE(Menu_ParamiteSpeak);
        //MENU_OVERRIDE(Menu_ScrabSpeak);
        //MENU_OVERRIDE(Menu_Save);
        //MENU_OVERRIDE(Menu_ReallyQuit);
        //MENU_OVERRIDE(Menu_Status);
        //MENU_OVERRIDE(Menu_Load);

        devTeleportMenuItems.clear();

        for (int i = 0; i < 17; i++)
        {
            devTeleportMenuItems.push_back({ gPerLvlData_561700[i].field_0_display_name, [](CustomPauseMenu * pm) {
                const auto levelSelectEntry = gPerLvlData_561700[pm->index];
                pm->ClosePauseMenu();
                sActiveHero_5C1B68->field_106_current_state = 3;
                sActiveHero_5C1B68->field_1AC_flags.Set(Abe::e1AC_Bit7);;
                sActiveHero_5C1B68->field_C2_lvl_number = levelSelectEntry.field_4_level;
                sActiveHero_5C1B68->field_C0_path_number = levelSelectEntry.field_6_path;
                sActiveHero_5C1B68->field_100_pCollisionLine = nullptr;
                sActiveHero_5C1B68->field_F8 = sActiveHero_5C1B68->field_BC_ypos;
                gMap_5C3030.SetActiveCam_480D30(levelSelectEntry.field_4_level, levelSelectEntry.field_6_path, levelSelectEntry.field_8_camera, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
                sActiveHero_5C1B68->field_B8_xpos = FP_FromInteger(levelSelectEntry.field_C_abe_x_off - Path_Get_Bly_Record_460F30(levelSelectEntry.field_4_level, levelSelectEntry.field_6_path)->field_4_pPathData->field_1A_abe_start_xpos);
                sActiveHero_5C1B68->field_BC_ypos = FP_FromInteger(levelSelectEntry.field_E_abe_y_off - Path_Get_Bly_Record_460F30(levelSelectEntry.field_4_level, levelSelectEntry.field_6_path)->field_4_pPathData->field_1C_abe_start_ypos);
                DEV_CONSOLE_MESSAGE("Teleported to " + std::string(levelSelectEntry.field_0_display_name), 6);
            } });
        }

        //devTeleportMenu = CustomPauseMenu(&devTeleportMenuItems, "Level Select");
    }
#endif
}

void PauseMenu::Page_Base_Render_490A50(int ** ot, PauseMenu::PauseMenuPage * mp)
{
    int i = 0;
    PauseMenuPageEntry * e = &mp->field_8_menu_items[i];

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

        field_142 = static_cast<short>(field_F4_font.DrawString_4337D0(
            ot,
            textFormatted,
            x, // X
            e->field_4_y, // Y
            static_cast<char>(0),
            1,
            0,
            41,
            static_cast<char>(glow + e->field_C_r),
            static_cast<char>(glow + e->field_D_g),
            static_cast<char>(glow + e->field_E_b),
            field_142,
            FP_FromDouble(1.0),
            640,
            0));

        e = &mp->field_8_menu_items[++i];
    }
}


void PauseMenu::Page_Main_Update_4903E0()
{
    //NOT_IMPLEMENTED();

    auto inputHeld = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;

    if (inputHeld & eDown)
    {
        if (++field_134_Index_Main > 7)
        {
            field_134_Index_Main = 0;
        }
        SFX_Play_46FBA0(0x34u, 45, 400, 0x10000);
    }
    if (inputHeld & eUp)
    {
        if (--field_134_Index_Main < 0)
        {
            field_134_Index_Main = 7;
        }
        SFX_Play_46FBA0(0x34u, 45, 400, 0x10000);
    }

    field_144_active_menu.field_C_selected_index = field_134_Index_Main;

    if (inputHeld & 0x200000)
    {
        word12C_flags &= 0xFFFEu;
        SFX_Play_46FBA0(0x11u, 40, 2400, 0x10000);
        SND_Restart_4CB0E0();
    }
    else if (inputHeld & eUnPause)
    {
        switch (field_134_Index_Main)
        {
        case 0:
            word12C_flags &= 0xFFFEu;
            SFX_Play_46FBA0(0x11u, 40, 2400, 0x10000);
            SND_Restart_4CB0E0();
            return;
        case 1:
            word12C_flags &= 0xFFFEu;
            SFX_Play_46FBA0(0x11u, 40, 2400, 0x10000);
            SND_Restart_4CB0E0();
            Quicksave_4C90D0();
            return;
        case 2:
#if DEVELOPER_MODE
            devMenu.Activate();
#else
            field_136 = 1;
            memcpy(&field_144_active_menu, &sPM_Page_Controls_Actions_546610, sizeof(field_144_active_menu));
            field_138 = 0;
#endif
            break;
        case 3:
            field_136 = 3;
            memcpy(&field_144_active_menu, &sPM_Page_Status_5465F8, sizeof(field_144_active_menu));
            break;
        case 4:
            field_136 = 5;
            memcpy(&field_144_active_menu, &sPM_Page_Save_5465C8, sizeof(field_144_active_menu));
            SFX_Play_46FA90(0x54u, 90, 0x10000);
            field_13C = 0;
            word12C_flags &= 0xFFF5u;
            field_13E = -1;
            word12C_flags |= 0x400;
            field_13A = 0;
            Quicksave_4C90D0();
            Path_Format_CameraName_460FB0(
                sSaveString_5C931C,
                gMap_5C3030.sCurrentLevelId_5C3030,
                gMap_5C3030.sCurrentPathId_5C3032,
                gMap_5C3030.sCurrentCamId_5C3034);
            sSaveString_5C931C[8] = 0;
            strcat(sSaveString_5C931C, "\x03");
            Input_DisableInput_4EDDC0();
            return;
        case 5:
            Quicksave_FindSaves_4D4150();
            field_136 = 4;
            memcpy(&field_144_active_menu, &sPM_Page_Load_546628, sizeof(field_144_active_menu));
            SFX_Play_46FA90(0x54u, 90, 0x10000);
            word12C_flags &= 0xFFF5;
            field_13C = 0;
            word12C_flags |= 0x400;
            field_13E = -1;
            field_13A = 0;
            return;
        case 6:
            DestroyObjects_4A1F20();
            sub_4C9870();
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
            gMap_5C3030.field_8 = 1;
            if (sActiveHero_5C1B68->field_1A2_rock_or_bone_count)
            {
                LoadRockTypes_49AB30(
                    sActiveQuicksaveData_BAF7F8.field_244_restart_path_world_info.field_4_level,
                    sActiveQuicksaveData_BAF7F8.field_244_restart_path_world_info.field_6_path);

                if (!gpThrowableArray_5D1E2C)
                {
                    gpThrowableArray_5D1E2C = alive_new<ThrowableArray>();
                    gpThrowableArray_5D1E2C->ctor_49A630();
                }

                gpThrowableArray_5D1E2C->Add_49A7A0(sActiveHero_5C1B68->field_1A2_rock_or_bone_count);
            }
            word12C_flags &= 0xFFFEu;
            SFX_Play_46FBA0(0x11u, 40, 3400, 0x10000);
            SND_Restart_4CB0E0();
        case 7:
            field_136 = 2;
            memcpy(&field_144_active_menu, &sPM_Page_ReallyQuit_5465E0, sizeof(field_144_active_menu));
            field_134_Index_Main = 0;
            break;
        default:
            return;
        }

        SFX_Play_46FA90(0x54u, 90, 0x10000);
    }
}

void PauseMenu::Page_ControlsActions_Update_48FA60()
{
    if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held & 0x200000)
    {
        field_136 = 0;
        memcpy(&field_144_active_menu, &sPM_Page_Main_5465B0, sizeof(field_144_active_menu));
        SFX_Play_46FBA0(17u, 40, 2400, 0x10000);
    }

    if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held & 0x100000)
    {
        const int prev = ++field_138;
        if (prev < 6)
        {
            field_144_active_menu.field_8_menu_items = sControlActionsPages_55DE40[prev];
            SFX_Play_46FA90(84u, 90, 0x10000);
        }
        else
        {
            field_138 = 0;
            field_136 = 0;
            memcpy(&field_144_active_menu, &sPM_Page_Main_5465B0, sizeof(field_144_active_menu));
            SFX_Play_46FBA0(17u, 40, 2400, 0x10000);
        }
    }
}

EXPORT void PauseMenu::Page_ReallyQuit_Update_490930()
{
    if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held & 0x200000)
    {
        field_136 = 0;
        memcpy(&field_144_active_menu, &sPM_Page_Main_5465B0, sizeof(field_144_active_menu));
        SFX_Play_46FBA0(17u, 40, 2400, 0x10000);
    }

    if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held & 0x100000)
    {
        word12C_flags &= ~1u;
        SFX_Play_46FBA0(17u, 40, 2400, 0x10000);

        if (pPauseMenu_5C9300 && pPauseMenu_5C9300 == this)
        {
            pPauseMenu_5C9300->field_6_flags.Set(BaseGameObject::eDead);
        }
        else
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }

        pPauseMenu_5C9300 = 0;
        gMap_5C3030.SetActiveCam_480D30(0, 1, 1, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
        gMap_5C3030.field_CE = 1;
        sCurrentControllerIndex_5C1BBE = 0;
    }
}

EXPORT WORD CC sub_4A2B70()
{
    NOT_IMPLEMENTED();
    return 1;
}
ALIVE_ARY(1, 0x55e718, char, 32, sPauseMenu_Of300Mudokons_55E718, {});
ALIVE_ARY(1, 0x55e738, char, 56, sHasBeenTerminated_55E738, {});

void PauseMenu::Update_48FD80()
{
    Abe* pHero = sActiveHero_5C1B68;
    Abe* pControlledChar = nullptr;
    
    if (sActiveHero_5C1B68->field_10C_health <= FP_FromInteger(0) || sActiveHero_5C1B68->field_114_flags.Get(Flags_114::e114_Bit7))
    {
        pControlledChar = sControlledCharacter_5C1B8C;
    }
    else
    {
        pControlledChar = sControlledCharacter_5C1B8C;
        if (!(sControlledCharacter_5C1B8C->field_114_flags.Get(e114_Bit10)))
        {
            const __int16 heroState = sActiveHero_5C1B68->field_106_current_state;
            if (heroState != 86
                && heroState != 119
                && heroState != 120
                && heroState != 75
                && heroState != 76
                && heroState != 77
                && heroState != 78
                && heroState != 79
                && heroState != 80
                && heroState != 81
                && heroState != 82
                && heroState != 83
                && (sControlledCharacter_5C1B8C->field_4_typeId != Types::eType_45 || LOWORD(sControlledCharacter_5C1B8C->field_124_gnFrame) != 2)
                && sActiveHero_5C1B68->field_1A8 == -1)
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
                    SND_SEQ_Stop_4CAE60(0xAu);
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

    if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held & InputCommands::ePause)
    {
        if (pHero->field_10C_health > FP_FromInteger(0)
            && !(pHero->field_114_flags.Get(Flags_114::e114_Bit7))
            && !(pControlledChar->field_114_flags.Get(Flags_114::e114_Bit10)))
        {
            const short heroState = pHero->field_106_current_state;
            if (heroState != 86
                && heroState != 119
                && heroState != 120
                && heroState != 75
                && heroState != 76
                && heroState != 77
                && heroState != 78
                && heroState != 79
                && heroState != 80
                && heroState != 81
                && heroState != 82
                && heroState != 83
                && (pControlledChar->field_4_typeId != BaseGameObject::eType_45 || LOWORD(pControlledChar->field_124_gnFrame) != 2) // TODO: Why LOWORD only ??
                && pHero->field_1A8 == -1)
            {
                SND_StopAll_4CB060();
                SFX_Play_46FBA0(0x11u, 40, 2400, 0x10000);
                sub_4A2B70();
                field_6_flags.Set(BaseGameObject::eDrawable);
                field_134_Index_Main = 0;
                field_136 = 0;
                word12C_flags = word12C_flags & ~8 | 1;
                field_12E_selected_glow = 40;
                field_130 = 8;
                Path_Format_CameraName_460FB0(
                    sScreenStringBuffer_5C92F0,
                    gMap_5C3030.sCurrentLevelId_5C3030,
                    gMap_5C3030.sCurrentPathId_5C3032,
                    gMap_5C3030.sCurrentCamId_5C3034);

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
                sprintf(sHasBeenTerminated_55E738, "%d HA%s BEEN TERMINATED", sKilledMudokons_5C1BC0, (sKilledMudokons_5C1BC0 != 0) ? "VE" : "S");

                if (sActiveHero_5C1B68->field_128.field_12_mood == 0)
                {
                    field_158_animation.Set_Animation_Data_409C80(1248, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 203, 1u, 0));
                }
                else if (sActiveHero_5C1B68->field_128.field_12_mood == 3)
                {
                    field_158_animation.Set_Animation_Data_409C80(1076, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 202, 1u, 0));
                }
                else if (sActiveHero_5C1B68->field_128.field_12_mood == 5)
                {
                    field_158_animation.Set_Animation_Data_409C80(1252, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 201, 1u, 0));
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

                        for (int i = 0; i < gObjList_drawables_5C1124->Size(); i++)
                        {
                            BaseGameObject* pObj = gObjList_drawables_5C1124->ItemAt(i);
                            if (!pObj)
                            {
                                break;
                            }

                            if (!(pObj->field_6_flags.Get(BaseGameObject::eDead)))
                            {
                                if (pObj->field_6_flags.Get(BaseGameObject::eDrawable))
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

                        if (field_130 > 0)
                        {
                            field_12E_selected_glow += 8;
                        }

                        if (field_12E_selected_glow <= 120 || field_130 <= 0)
                        {
                            if (field_130 <= 0)
                            {
                                field_12E_selected_glow -= 8;
                                if (field_12E_selected_glow < 40)
                                {
                                    field_130 = -field_130;
                                    field_12E_selected_glow += field_130;
                                }
                            }
                        }
                        else
                        {
                            field_130 = -field_130;
                            field_12E_selected_glow += field_130;
                        }

                        (this->*field_144_active_menu.field_0_fn_update)();
                    } while (word12C_flags & 1);
                }
                sInputObject_5BD4E0.Update_45F040();
                field_6_flags.Clear(BaseGameObject::eDrawable);
            }
        }
    }
    sDisableFontFlicker_5C9304 = 0;
}

ALIVE_VAR(1, 0x5C9300, PauseMenu*, pPauseMenu_5C9300, nullptr);