#include "stdafx.h"
#include "PauseMenu.hpp"
#include "stdlib.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "DebugHelpers.hpp"
#include "SwitchStates.hpp"
#include "StringFormatters.hpp"
#include "Abe.hpp"


// Used by the level skip cheat/ui/menu
const static PerLvlData gPerLvlData_561700[17] =
{
    { "Mines", 1, 1, 4, 65535u, 2712, 1300 },
    { "Mines Ender", 1, 6, 10, 65535u, 2935, 2525 },
    { "Necrum", 2, 2, 1, 65535u, 2885, 1388 },
    { "Mudomo Vault", 3, 1, 1, 65535u, 110, 917 },
    { "Mudomo Vault Ender", 11, 13, 1, 65535u, 437, 454 },
    { "Mudanchee Vault", 4, 6, 3, 65535u, 836, 873 },
    { "Mudanchee Vault Ender", 7, 9, 4, 65534u, 1600, 550 },
    { "FeeCo Depot", 5, 1, 1, 65535u, 4563, 972 },
    { "FeeCo Depot Ender", 12, 11, 5, 65535u, 1965, 1650 },
    { "Barracks", 6, 1, 4, 65535u, 1562, 1651 },
    { "Barracks Ender", 13, 11, 5, 65535u, 961, 1132 },
    { "Bonewerkz", 8, 1, 1, 65535u, 813, 451 },
    { "Bonewerkz Ender", 14, 14, 10, 65535u, 810, 710 },
    { "Brewery", 9, 16, 6, 65535u, 1962, 1232 },
    { "Game Ender", 10, 1, 1, 65535u, 460, 968 },
    { "Credits", 16, 1, 1, 65535u, 0, 0 },
    { "Menu", 0, 1, 1, 65535u, 0, 0 }
};

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

ALIVE_VAR_EXTERN(__int16, sRescuedMudokons_5C1BC2);
ALIVE_VAR(1, 0x5c1bc0, __int16, sKilledMudokons_5C1BC0, 0);

ALIVE_ARY(1, 0x5C931C, char, 32, sSaveString_5C931C, {});

ALIVE_VAR(1, 0x5d1e2c, BaseGameObject *, class_0x30_dword_5D1E2C, 0);

ALIVE_ARY(1, 0x554474, byte, 32, pal_554474, {
    0x00, 0x00, 0x21, 0x84, 0x42, 0x88, 0x63, 0x8C, 0x84, 0x90,
    0xA5, 0x14, 0xE7, 0x1C, 0x08, 0x21, 0x29, 0x25, 0x4A, 0x29,
    0x6B, 0x2D, 0x8C, 0x31, 0xAD, 0x35, 0xEF, 0x3D, 0x10, 0x42,
    0x73, 0x4E });

// MENUS

MENU_BEGIN(MainMenu);
MENU_ENTRY("continue", 184, 48, Centre);
MENU_ENTRY("quicksave", 184, 70, Centre);
#ifdef DEVELOPER_MODE
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

// TODO: SET VALUES
ALIVE_VAR(1, 0x5465B0, PauseMenu::PauseMenuPage, sPM_Page_Main_5465B0, {});
ALIVE_VAR(1, 0x546610, PauseMenu::PauseMenuPage, sPM_Page_Controls_Actions_546610, {});
ALIVE_VAR(1, 0x546628, PauseMenu::PauseMenuPage, sPM_Page_Load_546628, {});
ALIVE_VAR(1, 0x5465F8, PauseMenu::PauseMenuPage, sPM_Page_Status_5465F8, {});
ALIVE_VAR(1, 0x5465E0, PauseMenu::PauseMenuPage, sPM_Page_ReallyQuit_5465E0, {});
ALIVE_VAR(1, 0x5465C8, PauseMenu::PauseMenuPage, sPM_Page_Save_5465C8, {});

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

void PauseMenu::VDestructor(signed int flags)
{
    dtor_48FCB0(flags);
}

void PauseMenu::VUpdate()
{
    Update_48FD80();
}

PauseMenu::PauseMenu()
{

}

PauseMenu * PauseMenu::ctor_48FB80()
{
    BaseAnimatedWithPhysicsGameObject::BaseAnimatedWithPhysicsGameObject_ctor_424930(0);

    SetVTable(&field_158_animation, 0x544290);
    SetVTable(this, 0x546658);

    sQuicksave_SaveNextFrame_5CA4D8 = 0;
    sQuicksave_LoadNextFrame_5CA4D9 = 0;

    field_4_typeId = Types::ePauseMenu;
    field_6_flags &= ~0x8;
    field_6_flags |= 0x100;
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
    NOT_IMPLEMENTED();
}

void PauseMenu::dtor_48FCB0(signed int /*flags*/)
{
    NOT_IMPLEMENTED();
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
        field_6_flags |= 1u; // Todo: check this
    }
}

EXPORT void CC sub_4CB0E0()
{
    NOT_IMPLEMENTED();
}

EXPORT void CC LoadRockTypes_49AB30(unsigned __int16 /*a1*/, unsigned __int16 /*a2*/)
{
    NOT_IMPLEMENTED();
}

EXPORT BaseGameObject * sub_49A630(BaseGameObject * /*ptr*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void __fastcall sub_4A1F20(int /*a1*/, int /*a2*/)
{
    NOT_IMPLEMENTED();
}

EXPORT __int16 __fastcall sub_49A7A0(BaseGameObject * /*thisPtr*/, __int16 /*a2*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC sub_4C9870()
{
    NOT_IMPLEMENTED();
}

void PauseMenu::Render_490BD0(int ** /*ot*/)
{
    NOT_IMPLEMENTED();
}


#ifdef DEVELOPER_MODE
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
        MIDI_46FBA0(0x11u, 40, 2400, 0x10000);
        sub_4CB0E0();
        customMenuStack.clear();
    }

    void Update(PauseMenu * pm)
    {
        auto input = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;
        if (input & eDown)
        {
            if (++index >= static_cast<int>(entries->size()))
                index = 0;
            MIDI_46FBA0(0x34u, 45, 400, 0x10000);

            CompileEntries();
        }
        else if (input & eUp)
        {
            if (--index < 0)
                index = entries->size() - 1;
            MIDI_46FBA0(0x34u, 45, 400, 0x10000);

            CompileEntries();
        }
        else if (input & 0x200000)
        {
            MIDI_46FBA0(0x11u, 40, 2400, 0x10000);
            GoBack(pm);
        }
        else if (input & eUnPause)
        {
            (*entries)[index].callback(this);
            MIDI_46FA90(0x54u, 90, 0x10000);
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
        if (!pObj || pObj == pPauseMenu_5C9300 || !(pObj->field_6_flags & BaseGameObject::eIsBaseAliveGameObject))
        {
            continue;
        }

        if (pObj->field_4_typeId != 69)
        {
            pObj->field_6_flags |= BaseGameObject::eDead;
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

        if (pObj->field_4_typeId != 69)
        {
            pObj->field_6_flags |= BaseGameObject::eDead;
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
    if (customMenuStack.size() > 0)
        customMenuStack[customMenuStack.size() - 1]->Update(this);
}

///// END CUSTOM CODE!!! ///

#endif

void PauseMenu_ForceLink() {

#ifdef DEVELOPER_MODE
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
                sActiveHero_5C1B68->field_106_animation_num = 3;
                sActiveHero_5C1B68->field_1AC |= 0x40;
                sActiveHero_5C1B68->field_C2_lvl_number = levelSelectEntry.field_4_level;
                sActiveHero_5C1B68->field_C0_path_number = levelSelectEntry.field_6_path;
                sActiveHero_5C1B68->field_100_pCollisionLine = nullptr;
                sActiveHero_5C1B68->field_F8 = sActiveHero_5C1B68->field_BC_ypos;
                gMap_5C3030.SetActiveCam_480D30(levelSelectEntry.field_4_level, levelSelectEntry.field_6_path, levelSelectEntry.field_8_camera, 0, 0, 0);
                sActiveHero_5C1B68->field_B8_xpos = FP(levelSelectEntry.field_C_abe_x_off - Path_Get_Bly_Record_460F30(levelSelectEntry.field_4_level, levelSelectEntry.field_6_path)->field_4_pPathData->field_1A_abe_start_xpos);
                sActiveHero_5C1B68->field_BC_ypos = FP(levelSelectEntry.field_E_abe_y_off - Path_Get_Bly_Record_460F30(levelSelectEntry.field_4_level, levelSelectEntry.field_6_path)->field_4_pPathData->field_1C_abe_start_ypos);
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
        MIDI_46FBA0(0x34u, 45, 400, 0x10000);
    }
    if (inputHeld & eUp)
    {
        if (--field_134_Index_Main < 0)
        {
            field_134_Index_Main = 7;
        }
        MIDI_46FBA0(0x34u, 45, 400, 0x10000);
    }

    field_144_active_menu.field_C_selected_index = field_134_Index_Main;

    if (inputHeld & 0x200000)
    {
        word12C_flags &= 0xFFFEu;
        MIDI_46FBA0(0x11u, 40, 2400, 0x10000);
        sub_4CB0E0();
    }
    else if (inputHeld & eUnPause)
    {
        switch (field_134_Index_Main)
        {
        case 0:
            word12C_flags &= 0xFFFEu;
            MIDI_46FBA0(0x11u, 40, 2400, 0x10000);
            sub_4CB0E0();
            return;
        case 1:
            word12C_flags &= 0xFFFEu;
            MIDI_46FBA0(0x11u, 40, 2400, 0x10000);
            sub_4CB0E0();
            Quicksave_4C90D0();
            return;
        case 2:
#ifdef DEVELOPER_MODE
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
            MIDI_46FA90(0x54u, 90, 0x10000);
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
            MIDI_46FA90(0x54u, 90, 0x10000);
            word12C_flags &= 0xFFF5;
            field_13C = 0;
            word12C_flags |= 0x400;
            field_13E = -1;
            field_13A = 0;
            return;
        case 6:
            sub_4A1F20(3 * sCurrentControllerIndex_5C1BBE, 3 * sCurrentControllerIndex_5C1BBE);
            sub_4C9870();
            memcpy(sSwitchStates_5C1A28,sActiveQuicksaveData_BAF7F8.field_35C_restart_path_switch_states,sizeof(sSwitchStates_5C1A28));
            Abe::CreateFromSaveState_44D4F0(sActiveQuicksaveData_BAF7F8.field_284_restart_path_abe_state);
            Quicksave_ReadWorldInfo_4C9490(&sActiveQuicksaveData_BAF7F8.field_244_restart_path_world_info);
            
            gMap_5C3030.SetActiveCam_480D30(
                sActiveQuicksaveData_BAF7F8.field_244_restart_path_world_info.field_4_level,
                sActiveQuicksaveData_BAF7F8.field_244_restart_path_world_info.field_6_path,
                sActiveQuicksaveData_BAF7F8.field_244_restart_path_world_info.field_8_cam,
                0,
                1,
                1);
            gMap_5C3030.field_8 = 1;
            if (sActiveHero_5C1B68->field_1A2_rock_or_bone_count)
            {
                LoadRockTypes_49AB30(
                    sActiveQuicksaveData_BAF7F8.field_244_restart_path_world_info.field_4_level,
                    sActiveQuicksaveData_BAF7F8.field_244_restart_path_world_info.field_6_path);
                if (!class_0x30_dword_5D1E2C)
                {
                    auto v6 = (BaseGameObject *)malloc_4954D0(0x30u);
                    sub_49A630(v6);
                    class_0x30_dword_5D1E2C = v6;
                }
                sub_49A7A0(class_0x30_dword_5D1E2C, sActiveHero_5C1B68->field_1A2_rock_or_bone_count);
            }
            word12C_flags &= 0xFFFEu;
            MIDI_46FBA0(0x11u, 40, 3400, 0x10000);
            sub_4CB0E0();
        case 7:
            field_136 = 2;
            memcpy(&field_144_active_menu, &sPM_Page_ReallyQuit_5465E0, sizeof(field_144_active_menu));
            field_134_Index_Main = 0;
            break;
        default:
            return;
        }

        MIDI_46FA90(0x54u, 90, 0x10000);
    }
}

void PauseMenu::Update_48FD80()
{
    NOT_IMPLEMENTED();
}

ALIVE_VAR(1, 0x5C9300, PauseMenu*, pPauseMenu_5C9300, nullptr);