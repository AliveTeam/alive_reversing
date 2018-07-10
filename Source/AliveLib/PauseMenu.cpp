#include "stdafx.h"
#include "PauseMenu.hpp"
#include "stdlib.hpp"
#include "Function.hpp"
#include <iomanip>

// MACROS

#define MENU_OVERRIDE(name) memcpy(reinterpret_cast<void*>(_MenuAddress##name##), &sPauseMenuList_##name, sizeof(sPauseMenuList_##name));
#define MENU_VAR(addr, name) ALIVE_VAR(1, 0x##addr, t_sPauseMenuList_##name, PageEntryList_##name##_##addr, sPauseMenuList_##name);

#define MENU_BEGIN(name) struct t_sPauseMenuList_##name{
#define MENU_ENTRY(text, x, y, align) PauseMenuPageEntry Menu_##x##y##align = { 2, x, y, 0, text, 0x80, 0x10, 0xFF, align};
#define MENU_ENTRY_EX(text, x, y, align, r, g, b) PauseMenuPageEntry Menu_##x##y##align##r##g##b = { 2, x, y, 0, text, r, g, b, align};
#define MENU_END(addr, name) PauseMenuPageEntry MenuEnd = {  0, 0, 0, 0, nullptr, 0, 0, 0, 0 };}; t_sPauseMenuList_##name sPauseMenuList_##name;\
MENU_VAR(addr, name);\
const int _MenuAddress##name = 0x##addr;

//

ALIVE_ARY(1, 0x5bc5c8, byte, 32, sFont1VRAM_5BC5C8, {});
ALIVE_ARY(1, 0x5BC5D8, byte, 32, sFont2VRAM_5BC5D8, {});

ALIVE_VAR(1, 0x5ca4d8, char, sQuicksave_SaveNextFrame_5CA4D8, 0);
ALIVE_VAR(1, 0x5ca4d9, char, sQuicksave_LoadNextFrame_5CA4D9, 0);
ALIVE_VAR(1, 0x5c9304, char, sDisableFontFlicker_5C9304, 0);
ALIVE_ARY(1, 0x5C92F0, char, 16, sScreenStringBuffer_5C92F0, { 0 });

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
MENU_ENTRY("alive menu", 184, 92, Centre);
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

struct DumpEntry
{
    int address;
    std::string name;
};

std::string EscapeUnknownCharacters(std::string text)
{
    std::stringstream output;

    for (auto c : text)
    {
        if (c >= 32 && c <= 126)
        {
            output << c;
        }
        else
        {
            output << "\\x" << std::setfill('0') << std::setw(2) << std::hex << (int)c;
        }
    }

    return output.str();
}

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

void PauseMenu_ForceLink() {

    if (IsAlive())
    {
        DumpMenus();

        // Overwrites game menu lists with ours, so we can see if everything is the same.
        MENU_OVERRIDE(MainMenu);
        MENU_OVERRIDE(Menu_ControlActions);
        MENU_OVERRIDE(Menu_GameSpeak);
        MENU_OVERRIDE(Menu_SligSpeak);
        MENU_OVERRIDE(Menu_GlukkonSpeak);
        MENU_OVERRIDE(Menu_ParamiteSpeak);
        MENU_OVERRIDE(Menu_ScrabSpeak);
        //MENU_OVERRIDE(Menu_Save);
        MENU_OVERRIDE(Menu_ReallyQuit);
        MENU_OVERRIDE(Menu_Status);
        //MENU_OVERRIDE(Menu_Load);
    }
}

EXPORT signed __int16 sub_4A2BC0()
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT void CC String_FormatString_4969D0(char *src, char *dst, int a3, char a4)
{
    NOT_IMPLEMENTED();
}

void PauseMenu::VDestructor(signed int flags)
{
    dtor_48FCB0(flags);
}

void PauseMenu::VUpdate()
{
    Update_48FD80();
}

PauseMenu * PauseMenu::ctor_48FB80()
{
    BaseAnimatedWithPhysicsGameObject::BaseAnimatedWithPhysicsGameObject_ctor_424930(0);

    SetVTable(&field_158_animation, 0x544290);
    SetVTable(this, 0x546658);

    sQuicksave_SaveNextFrame_5CA4D8 = 0;
    sQuicksave_LoadNextFrame_5CA4D9 = 0;

    field_4_typeId = 95;
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

    field_F4_font.ctor_433590(256, pal_554474, sFont1VRAM_5BC5C8);
    Init_491760();
    sub_4A2BC0();
    
    sDisableFontFlicker_5C9304 = 0;
    
    return this;
}

void PauseMenu::dtor_48FCE0()
{
    NOT_IMPLEMENTED();
}

void PauseMenu::dtor_48FCB0(signed int flags)
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

void PauseMenu::Update_48FD80()
{
    NOT_IMPLEMENTED();
}

void PauseMenu::Render_490BD0(int ** ot)
{
    NOT_IMPLEMENTED();
}

void PauseMenu::Page_Base_Render_490A50(int ** ot, PauseMenuPage * mp)
{
    int i = 0;
    PauseMenuPageEntry * e = &mp->field_8_menu_items[i];
    
    while(e->field_8_text)
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
            auto textMeasure = field_F4_font.MeasureWidth_433700(textFormatted);

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

        field_142 = field_F4_font.DrawString_4337D0(
            ot,
            textFormatted,
            x, // X
            e->field_4_y, // Y
            0,
            1,
            0,
            41,
            glow + e->field_C_r,
            glow + e->field_D_g,
            glow + e->field_E_b,
            field_142,
            0x10000,
            640,
            0);

        e = &mp->field_8_menu_items[++i];
    }
}

void Font::ctor_433590(int blockSize, BYTE * palette, byte * vramBuffer)
{
    NOT_IMPLEMENTED();
}

int Font::DrawString_4337D0(int ** ot, char * text, int x, __int16 y, char abr, int bSemiTrans, int a2, int a9, char r, char g, char b, int a13, signed int scale, int a15, __int16 colorRandomRange)
{
    NOT_IMPLEMENTED();
    return 0;
}

int Font::MeasureWidth_433700(char * text)
{
    NOT_IMPLEMENTED();
    return 0;
}

ALIVE_VAR(1, 0x5C9300, PauseMenu*, pPauseMenu_5C9300, nullptr);