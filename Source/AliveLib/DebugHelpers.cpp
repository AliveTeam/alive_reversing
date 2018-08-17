#include "stdafx.h"
#include "DebugHelpers.hpp"
#include "stdlib.hpp"
#include <iomanip>
#include "Function.hpp"
#include "Map.hpp"
#include "PathData.hpp"
#include "PsxDisplay.hpp"
#include "ScreenManager.hpp"
#include "ResourceManager.hpp"
#include "Font.hpp"
#include "DDCheat.hpp"
#include "Resources.hpp"
#include "SwitchStates.hpp"
#include "Events.hpp"
#include "MainMenu.hpp"
#include "Abe.hpp"
#include "Midi.hpp"
#include "Sfx.hpp"
#include "Sys.hpp"

bool sDebugEnabled_VerboseEvents = false;

class ObjectDebugger : public BaseGameObject
{
public:
    ObjectDebugger()
    {
        DisableVTableHack disableHack;

        BaseGameObject_ctor_4DBFA0(1, 1);
        field_6_flags |= BaseGameObject::eDrawable | 0x100;
        field_4_typeId = (BaseGameObject::Types)1001;

        mFontContext.LoadFontTypeCustom(reinterpret_cast<File_Font*>(sDebugFont), reinterpret_cast<Font_AtlasEntry*>(sDebugFontAtlas), mFontPalette);
        mFont.ctor_433590(512, reinterpret_cast<BYTE*>(mFontPalette), &mFontContext);

        gObjList_drawables_5C1124->Push_Back(this);
    }

    ~ObjectDebugger()
    {
    }

    static bool Enabled;

    void Destruct()
    {
        BaseGameObject_dtor_4DBEC0();
        gObjList_drawables_5C1124->Remove_Item(this);
    }

    virtual void VDestructor(signed int flags) override
    {
        Destruct();
        if (flags & 1)
        {
            Mem_Free_495540(this);
        }
    }

    virtual void VUpdate() override
    {
        mFontPIndex = 0;
    }

    virtual void VScreenChanged() override
    {
        // Dont kill!
    }

    static bool IsInAnimationList(Animation* toFind)
    {
        for (int i = 0; i < gObjList_animations_5C1A24->Size(); i++)
        {
            if (gObjList_animations_5C1A24->ItemAt(i) == toFind)
            {
                return true;
            }
        }
        return false;
    }

    void DrawObjectIDs(int**pOrderingTable)
    {
        /*struct MainMenuButton
        {
        __int16 field_0;
        __int16 field_2_x;
        __int16 field_4_y;
        __int16 field_6;
        __int16 field_8;
        __int16 field_A;
        };

        auto list = reinterpret_cast<MainMenuButton *>(0x5610B8);

        while (list->field_0)
        {
        pIndex = mFont.DrawString_4337D0(pOrderingTable, "O", list->field_2_x, list->field_4_y, 0, 1, 0, 40, 255, 0, 0, pIndex, FP_FromDouble(1.0), 640, 0);
        list++;
        }*/

        for (int baseObjIdx = 0; baseObjIdx < gBaseGameObject_list_BB47C4->Size(); baseObjIdx++)
        {
            BaseGameObject* pBaseGameObject = gBaseGameObject_list_BB47C4->ItemAt(baseObjIdx);

            if (!pBaseGameObject)
            {
                break;
            }

            if (pBaseGameObject->field_6_flags & BaseGameObject::eIsBaseAnimatedWithPhysicsObj)
            {
                auto aliveObj = ((BaseAnimatedWithPhysicsGameObject*)pBaseGameObject);
                int x = aliveObj->field_B8_xpos.GetExponent() - gMap_5C3030.field_24_camera_offset.field_0_x.GetExponent();
                int y = aliveObj->field_BC_ypos.GetExponent() - gMap_5C3030.field_24_camera_offset.field_4_y.GetExponent();

                if (IsInAnimationList(&aliveObj->field_20_animation))
                {
                    if (aliveObj->field_20_animation.field_92_current_frame != -1)
                    {
                        auto framePtr = aliveObj->field_20_animation.Get_FrameHeader_40B730(aliveObj->field_20_animation.field_92_current_frame);
                        if (framePtr != nullptr)
                        {
                            y += (framePtr->mBottomRight.y * aliveObj->field_CC_sprite_scale.GetDouble());
                        }
                    }
                }

                std::string text = std::to_string(pBaseGameObject->field_4_typeId);

                mFontPIndex = mFont.DrawString_4337D0(pOrderingTable, text.c_str(), x - (mFont.MeasureWidth_433700(text.c_str()) / 2) + 1, y + 1, 0, 0, 0, 39, 0, 0, 0, mFontPIndex, FP_FromDouble(1.0), 640, 0);
                mFontPIndex = mFont.DrawString_4337D0(pOrderingTable, text.c_str(), x - (mFont.MeasureWidth_433700(text.c_str()) / 2), y, 0, 1, 0, 40, 255, 255, 255, mFontPIndex, FP_FromDouble(1.0), 640, 0);
            }
        }
    }

    float Vec2Distance(float x1, float y1, float x2, float y2)
    {
        // This takes into account the fact that the height of the screen is / 2
        return sqrt(pow(x1 - x2, 2) + pow((y1 - y2) * 2, 2));
    }

    bool isDragging = false;
    BaseAnimatedWithPhysicsGameObject * mDragObject;

    void DrawUI(int** pOrderingTable)
    {
        HWND windowHandle = Sys_GetWindowHandle_4EE180();
        POINT mousePos;
        bool mouseLeftDown = GetAsyncKeyState(VK_LBUTTON);
        RECT r;
        GetClientRect(windowHandle, &r);
        GetCursorPos(&mousePos);
        ScreenToClient(windowHandle, &mousePos);
        mousePos.y /= 2;
        
        for (int baseObjIdx = 0; baseObjIdx < gBaseGameObject_list_BB47C4->Size(); baseObjIdx++)
        {
            BaseGameObject* pBaseGameObject = gBaseGameObject_list_BB47C4->ItemAt(baseObjIdx);

            if (!pBaseGameObject)
            {
                break;
            }

            if (pBaseGameObject->field_6_flags & BaseGameObject::eIsBaseAnimatedWithPhysicsObj)
            {
                auto aliveObj = ((BaseAnimatedWithPhysicsGameObject*)pBaseGameObject);

                int x = (aliveObj->field_B8_xpos.GetExponent() - gMap_5C3030.field_24_camera_offset.field_0_x.GetExponent()) / 0.575;
                int y = (aliveObj->field_BC_ypos.GetExponent() - gMap_5C3030.field_24_camera_offset.field_4_y.GetExponent());

                if (Vec2Distance(x, y, mousePos.x, mousePos.y) < 10 && !isDragging && mouseLeftDown)
                {
                    isDragging = true;
                    mDragObject = aliveObj;
                }

                if (Vec2Distance(x, y, mousePos.x, mousePos.y) > 100)
                    continue;

                if (IsInAnimationList(&aliveObj->field_20_animation))
                {
                    if (aliveObj->field_20_animation.field_92_current_frame != -1)
                    {
                        auto framePtr = aliveObj->field_20_animation.Get_FrameHeader_40B730(aliveObj->field_20_animation.field_92_current_frame);
                        if (framePtr != nullptr)
                        {
                            double scale = aliveObj->field_CC_sprite_scale.GetDouble();
                            y += (framePtr->mBottomRight.y * aliveObj->field_CC_sprite_scale.GetDouble());
                        }
                    }
                }

                sFontDrawScreenSpace_5CA4B4 = 1;
                mFontPIndex = mFont.DrawString_4337D0(pOrderingTable, "Test", x, y, 0, 0, 0, 32, 255, 255, 255, mFontPIndex, FP_FromDouble(1.0), 640, 0);
                sFontDrawScreenSpace_5CA4B4 = 0;
            }

            if (isDragging && !mouseLeftDown)
            {
                isDragging = false;
            }

            if (isDragging)
            {
                mDragObject->field_B8_xpos = FP(gMap_5C3030.field_24_camera_offset.field_0_x.GetExponent() + (mousePos.x * 0.575));
                mDragObject->field_BC_ypos = FP(gMap_5C3030.field_24_camera_offset.field_4_y.GetExponent() + mousePos.y);
            }
        }
    }

    virtual void VRender(int** pOrderingTable) override
    {
        if (!Enabled)
            return;

        DrawUI(pOrderingTable);

        DrawObjectIDs(pOrderingTable);
    }

    int mFontPIndex = 0;
    Font mFont;
    char mFontPalette[32];
    Font_Context mFontContext;
};
bool ObjectDebugger::Enabled = false;

class DebugPathRenderer : public BaseGameObject
{
public:
    DebugPathRenderer()
    {
        DisableVTableHack disableHack;

        BaseGameObject_ctor_4DBFA0(1, 1);
        field_6_flags |= BaseGameObject::eDrawable | 0x100;
        field_4_typeId = (BaseGameObject::Types)1001;

        mFontContext.LoadFontTypeCustom(reinterpret_cast<File_Font*>(sDebugFont), reinterpret_cast<Font_AtlasEntry*>(sDebugFontAtlas), mFontPalette);
        mFont.ctor_433590(128, reinterpret_cast<BYTE*>(mFontPalette), &mFontContext);

        gObjList_drawables_5C1124->Push_Back(this);
    }

    ~DebugPathRenderer()
    {
    }

    static bool Enabled;
    static bool GridEnabled;

    void Destruct()
    {
        BaseGameObject_dtor_4DBEC0();
        gObjList_drawables_5C1124->Remove_Item(this);
    }

    virtual void VDestructor(signed int flags) override
    {
        Destruct();
        if (flags & 1)
        {
            Mem_Free_495540(this);
        }
    }

    virtual void VUpdate() override
    {

    }

    virtual void VScreenChanged() override
    {
        // Dont kill!
    }

    struct LineColor { int r; int g; int b; };
    std::map<int, LineColor> mLineColors = { 
        { 0,{ 255, 0, 0 } }, // Floor
        { 1,{ 0, 0, 255 } }, // Left Wall
        { 3,{ 255, 0, 0 } }, // Ceiling
        { 2,{ 0, 0, 255 } }, // Right Wall
        { 4,{ 100, 0, 0 } }, // Background Floor
        { 5,{ 0, 0, 100 } }, // Background Left Wall
        { 6,{ 0, 0, 100 } }, // Background Right Wall
        { 8,{ 0, 255, 0 } }, // Path Group
        { 9,{ 255, 255, 255 } }, // Comment lines
        { 10,{ 128, 128, 128 } }, // Bullet blocking walls
        { 17,{ 255, 255, 0 } }, // Flying Slig Collision
        { 32,{ 255, 255, 100 } }, // Lift Platform
    };

    virtual void VRender(int** pOrderingTable) override
    {
        int pIndex = 0;

        bool gridSemiTrans = true;

        if (GridEnabled)
        {
            const float gridX = 25 / 0.575f;
            const int gridY = 20;
            const int layer = 22;

            for (int y = 0; y < 12; y++)
            {
                for (int x = 0; x < 15; x++)
                {
                    char c = ((x + y) % 2 == 0) ? 200 : 127;
                    for (int i = -1; i < 2; i++)
                    {
                        DEV::DebugDrawLine(pOrderingTable, layer, (x * gridX) + (gridX / 2.0f) + i, y * gridY, (x * gridX) + (gridX / 2.0f) + i, (y * gridY) + (gridY / 4.0f), 255, 255, 255, false, gridSemiTrans);
                    }

                    if (x == 0)
                    {
                        DEV::DebugDrawLine(pOrderingTable, layer, 0, y * gridY, 640, y * gridY, 255, 255, 255, false, gridSemiTrans);
                    }

                    if (y == 0)
                    {
                        DEV::DebugDrawLine(pOrderingTable, layer, x * gridX, 0, x * gridX, 240, 255, 255, 255, false, gridSemiTrans);
                        DEV::DebugDrawLine(pOrderingTable, layer, (x * gridX) - 1, 0, (x * gridX) - 1, 240, 255, 255, 255, false, gridSemiTrans);
                    }

                    DEV::DebugFillRect(pOrderingTable, layer, x * gridX, y * gridY, gridX, gridY, c, c, c, false, gridSemiTrans);
                }
            }
        }

        if (Enabled)
        {
            for (int i = 0; i < sCollisions_DArray_5C1128->field_C_max_count; i++)
            {
                auto l = &sCollisions_DArray_5C1128->field_0_pArray[i];
                LineColor color = { 255, 0, 255 };
                int mode = l->field_8_mode;

                if (mLineColors.find(mode) != mLineColors.end())
                {
                    color = mLineColors[mode];
                }

                int layer = 40;
                if (mode == 4 || mode == 5 || mode == 6)
                {
                    layer = 23;
                }
                DEV::DebugDrawLine(pOrderingTable, layer, l->field_0_x1, l->field_2_y1, l->field_4_x2, l->field_6_y2, color.r, color.g, color.b, true, false);

                int id_x = l->field_0_x1 - gMap_5C3030.field_24_camera_offset.field_0_x.GetExponent();
                int id_y = l->field_2_y1 - gMap_5C3030.field_24_camera_offset.field_4_y.GetExponent();

                if (id_x > 0 && id_x <= 640 && id_y > 0 && id_y <= 240)
                {
                    pIndex = mFont.DrawString_4337D0(pOrderingTable, std::to_string(mode).c_str(), id_x, id_y, 0, 0, 0, layer + 1, 255, 255, 255, pIndex, FP_FromDouble(1.0), 640, 0);
                    pIndex = mFont.DrawString_4337D0(pOrderingTable, std::to_string(mode).c_str(), id_x + 1, id_y + 1, 0, 0, 0, layer + 1, 0, 0, 0, pIndex, FP_FromDouble(1.0), 640, 0);
                }
            }
        }
    }

    Font mFont;
    char mFontPalette[32];
    Font_Context mFontContext;
};
bool DebugPathRenderer::Enabled = false;
bool DebugPathRenderer::GridEnabled = false;

struct DebugConsoleMessage
{
    std::string message;
    int time;
    float y;
    char r, g, b;
};

static std::vector<DebugConsoleMessage> sDebugConsoleMessages;

void ShowDebugConsoleMessage(std::string message, float duration, char r, char g, char b)
{
    auto lines = SplitString(message, '\n');
    
    for (auto l : lines)
    {
        sDebugConsoleMessages.insert(sDebugConsoleMessages.begin(), { l, static_cast<int>(30 * duration), 250, r, g, b });
    }
}

void ShowDebugConsoleMessage(std::string message, float duration)
{
    ShowDebugConsoleMessage(message, duration, 255, 255, 255);
}

struct FakeObjStruct
{
    int *field_0_VTbl;
    __int16 field_4_typeId;
    __int16 field_6_flags;
    int field_8_flags_ex;
    int field_C_objectId;
    int field_10_resources_array;
    int field_1C_update_delay;
};

struct DebugConsoleCommand
{
    std::string command;
    int paramsCount;
    std::function<void(std::vector<std::string> args)> callback;
    std::string helpText;
};

extern std::vector<DebugConsoleCommand> sDebugConsoleCommands;

void Command_Help(std::vector<std::string> args)
{
    DEV_CONSOLE_MESSAGE("You can call the following: ", 6);
    for (auto c : sDebugConsoleCommands)
    {
        DEV_CONSOLE_MESSAGE(c.command + " - " + c.helpText, 6);
    }
}

void Command_Test(std::vector<std::string> args)
{
    DEV_CONSOLE_MESSAGE("You called this with " + std::to_string(args.size()) + " arguments", 5);
}

void Command_Die(std::vector<std::string> args)
{
    FakeObjStruct fake;
    fake.field_4_typeId = 30;
    // Dont judge me on the line bellow paul :P
    // It calls to vtable offset 19
    ((void(__fastcall*)(BaseGameObject *, int eax, BaseGameObject *))(*(int*)(*(int*)(sControlledCharacter_5C1B8C)+76)))(sControlledCharacter_5C1B8C, 0, reinterpret_cast<BaseGameObject*>(&fake));
}

void Command_Murder(std::vector<std::string> args)
{
    FakeObjStruct fake;
    fake.field_4_typeId = 30;

    for (int baseObjIdx = 0; baseObjIdx < gBaseGameObject_list_BB47C4->Size(); baseObjIdx++)
    {
        BaseGameObject* pBaseGameObject = gBaseGameObject_list_BB47C4->ItemAt(baseObjIdx);

        if (!pBaseGameObject)
        {
            break;
        }

        if (pBaseGameObject == sActiveHero_5C1B68)
        {
            continue;
        }

        if (pBaseGameObject->field_6_flags & BaseGameObject::eIsBaseAliveGameObject)
        {
            auto aliveObj = ((BaseAliveGameObject*)pBaseGameObject);
            ((void(__fastcall*)(BaseGameObject *, int eax, BaseGameObject *))(*(int*)(*(int*)(aliveObj)+76)))(aliveObj, 0, reinterpret_cast<BaseGameObject*>(&fake));
        }
    }
}

bool sHasTeleported = false;
void Command_HelperUpdate()
{
    if (sHasTeleported)
    {
        PSX_Point pos;
        gMap_5C3030.GetCurrentCamCoords_480680(&pos);

        sActiveHero_5C1B68->field_B8_xpos = FP(pos.field_0_x + 184);
        sActiveHero_5C1B68->field_BC_ypos = FP(pos.field_2_y + 60);
        sHasTeleported = false;
        sActiveHero_5C1B68->field_106_animation_num = 3;
        sActiveHero_5C1B68->field_1AC |= 0x40;
        sActiveHero_5C1B68->field_C2_lvl_number = gMap_5C3030.sCurrentLevelId_5C3030;
        sActiveHero_5C1B68->field_C0_path_number = gMap_5C3030.sCurrentPathId_5C3032;
        sActiveHero_5C1B68->field_100_pCollisionLine = nullptr;
        sActiveHero_5C1B68->field_F8 = sActiveHero_5C1B68->field_BC_ypos;
        sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
    }
}

void Command_ToggleBool(bool * var, std::string varName)
{
    *var = !*var;
    DEV_CONSOLE_MESSAGE(varName + " is now " + std::string(((*var) ? "On" : "Off")), 6);
}

void Command_Teleport(std::vector<std::string> args)
{
    int level = std::stoi(args[0]);
    int path = std::stoi(args[1]);
    int cam = std::stoi(args[2]);
    gMap_5C3030.SetActiveCam_480D30(level, path, cam, 5, 0, 0);
    

    sHasTeleported = true;

    DEV_CONSOLE_MESSAGE("Teleported", 6);
}

void Command_Event(std::vector<std::string> args)
{
    int eventId = std::stoi(args[0]);
    if (eventId >= Event::kEventMax)
    {
        DEV_CONSOLE_MESSAGE_C("Invalid event id", 6, 127, 0, 0);
        return;
    }
    Event_Broadcast_422BC0(static_cast<Event>(eventId), sControlledCharacter_5C1B8C);
}

void Command_Menu(std::vector<std::string> args)
{
    int menuCam = std::stoi(args[0]);
    if (MainMenuController::gMainMenuController != nullptr)
    {
        MainMenuController::gMainMenuController->field_21A_target_cam = static_cast<short>(menuCam);
        MainMenuController::gMainMenuController->field_218_target_page_index = static_cast<short>(MainMenuController::GetPageIndexFromCam_4D05A0(menuCam));
        MainMenuController::gMainMenuController->field_21C = static_cast<short>(menuCam);
        MainMenuController::gMainMenuController->field_23C_T80 |= 0x200000;
    }
}

void Command_Midi1(std::vector<std::string> args)
{
    int arg1 = std::stoi(args[0]);

    SFX_Play_46FA90(arg1, 0, 0x10000);

    DEV_CONSOLE_MESSAGE("Played Midi1", 6);
}

std::vector<DebugConsoleCommand> sDebugConsoleCommands = {
    { "help", -1, Command_Help, "Shows what you're looking at" },
    { "test", -1, Command_Test, "Is this thing on?" },
    { "die", -1, Command_Die, "Kills you." },
    { "murder", -1, Command_Murder, "Kill everything around you." },
    { "ddcheat", -1, [](std::vector<std::string> args) { Command_ToggleBool(&sCommandLine_DDCheatEnabled_5CA4B5, "DDCheat"); }, "Toggle DDCheat" },
    { "object_id", -1, [](std::vector<std::string> args) { Command_ToggleBool(&ObjectDebugger::Enabled, "Object ID Debugger"); }, "Shows object id's on screen" },
    { "no_frame_skip", -1, [](std::vector<std::string> args) { Command_ToggleBool(&sCommandLine_NoFrameSkip_5CA4D1, "No Frame Skip"); }, "Toggle No Frame Skip" },
    { "fps", -1, [](std::vector<std::string> args) { Command_ToggleBool(&sCommandLine_ShowFps_5CA4D0, "FPS"); }, "Toggle FPS" },
    { "verbose_events", -1, [](std::vector<std::string> args) { Command_ToggleBool(&sDebugEnabled_VerboseEvents, "Verbose Events"); }, "Toggle Verbose Events" },
    { "open_doors", -1, [](std::vector<std::string> args) { Cheat_OpenAllDoors(); }, "Open all doors." },
    { "teleport", 3, Command_Teleport, "Teleport to a cam. (LEVEL, PATH, CAM)" },
    { "event", 1, Command_Event, "Broadcast's an event (EVENT ID)" },
    { "menu", 1, Command_Menu, "Changes to given menu cam" },
    { "midi1", 1, Command_Midi1, "Play sound using midi func 1" },
    { "path_lines", -1, [](std::vector<std::string> args) { Command_ToggleBool(&DebugPathRenderer::Enabled, "Path Lines"); }, "Renders path lines on screen" },
    { "grid", -1, [](std::vector<std::string> args) { Command_ToggleBool(&DebugPathRenderer::GridEnabled, "Grid"); }, "Renders grid on screen" },
};

//

class DebugConsole : public BaseGameObject
{
public:
    DebugConsole()
    {
        DisableVTableHack disableHack;

        BaseGameObject_ctor_4DBFA0(1, 1);
        field_6_flags |= BaseGameObject::eDrawable | 0x100;
        field_4_typeId = (BaseGameObject::Types)1002;
        
        mFontContext.LoadFontTypeCustom(reinterpret_cast<File_Font*>(sDebugFont), reinterpret_cast<Font_AtlasEntry*>(sDebugFontAtlas), mFontPalette);
        mFont.ctor_433590(512, reinterpret_cast<BYTE*>(mFontPalette), &mFontContext);

        gObjList_drawables_5C1124->Push_Back(this);
    }

    void Destruct()
    {
        BaseGameObject_dtor_4DBEC0();
        gObjList_drawables_5C1124->Remove_Item(this);
    }

    virtual void VDestructor(signed int flags) override
    {
        Destruct();
        if (flags & 1)
        {
            Mem_Free_495540(this);
        }
    }

    void ParseCommand(std::string command)
    {
        command = StringToLower(command);
        auto commandSplit = SplitString(command, ' ');

        if (commandSplit.size() > 0)
        {
            for (auto c : sDebugConsoleCommands)
            {
                if (commandSplit[0] == c.command)
                {
                    commandSplit.erase(commandSplit.begin());

                    if (c.paramsCount == -1 || c.paramsCount == commandSplit.size())
                    {
                        c.callback(commandSplit);
                    }
                    else
                    {
                        DEV_CONSOLE_MESSAGE_C("Command '" + c.command + "' was expecting " + std::to_string(c.paramsCount) + " args but got " + std::to_string(commandSplit.size()), 6, 127, 0, 0);
                    }
                    
                    return;
                }
            }

            DEV_CONSOLE_MESSAGE_C("Unknown command '" + command + "' Type help for more info.", 6, 127, 0, 0);
        }
    }

    std::string GetAutoComplete(std::string s)
    {
        s = StringToLower(s);
        for (auto c : sDebugConsoleCommands)
        {
            if (StringStartsWith(c.command, s))
            {
                return c.command;
            }
        }

        return "";
    }

    virtual void VUpdate() override
    {
        Command_HelperUpdate();

        auto key = Input_ReadKey_492610();

        if (GetAsyncKeyState(VK_OEM_3) & 0x1 || GetAsyncKeyState(VK_F9) & 0x1)
        {
            mCommandLineEnabled = !mCommandLineEnabled;

            if (mCommandLineEnabled)
                Input_DisableInput_4EDDC0();
            else
                Input_EnableInput_4EDDD0();
        }

        const char * allowedChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 !-+@#$%^&*()_";

        if (GetAsyncKeyState(VK_UP) & 0x1 && mCommandLineEnabled)
        {
            mCommandLineInput = mLastCommand;
        }

        if (mCommandLineEnabled && key > 0)
        {
            switch (key)
            {
            case VK_BACK:
                if (mCommandLineInput.size() > 0)
                {
                    mCommandLineInput.pop_back();
                }
                break;
            case VK_RETURN:
                if (mCommandLineInput.size() > 0)
                {
                    ParseCommand(mCommandLineInput);
                    mLastCommand = mCommandLineInput;
                    mCommandLineInput.clear();
                }

                mCommandLineEnabled = false;
                Input_EnableInput_4EDDD0();
                Input_Reset_492660();
                break;
            case VK_TAB:
                if (mAutoComplete.size() > 0)
                {
                    mCommandLineInput = mAutoComplete;
                }
                break;
            default:
                if (strspn(&key, allowedChars))
                {
                    mCommandLineInput += key;
                }
                break;
            }

            if (mCommandLineInput.size() > 0)
            {
                mAutoComplete = GetAutoComplete(mCommandLineInput);
            }
            else
            {
                mAutoComplete = "";
            }
        }
    }

    virtual void VScreenChanged() override
    {
        // Dont kill!
    }

    virtual void VRender(int** pOrderingTable) override
    {
        int pIndex = 0;

        int i = 0;
        for (std::vector<DebugConsoleMessage>::iterator it = sDebugConsoleMessages.begin();
            it != sDebugConsoleMessages.end();
            /*it++*/)
        {
            auto message = it;
            //char color = static_cast<char>(max(0, min(message->time * 10, 255)));
            int targetY = 232 - (i * 9) - 9;

            //message->y += (targetY - message->y) * 0.2f; // Smooth
            message->y = static_cast<float>(targetY);

            pIndex = mFont.DrawString_4337D0(pOrderingTable, message->message.c_str(), 0, static_cast<short>(message->y), 0, 1, 0, 40, message->r, message->g, message->b, pIndex, FP_FromDouble(1.0), 640, 0);
        
            message->time--;

            if (message->time <= 0 || i > 15)
            {
                it = sDebugConsoleMessages.erase(it);
            }
            else
            {
                i++;
                ++it;
            }
        }

        if (mCommandLineEnabled)
        {
            std::string trail = (sGnFrame_5C1B84 % 10 < 5) ? "" : "_";
            pIndex = mFont.DrawString_4337D0(pOrderingTable, (">" + mCommandLineInput + trail).c_str(), 0, 232, 0, 1, 0, 40, 255, 255, 255, pIndex, FP_FromDouble(1.0), 640, 0);
            pIndex = mFont.DrawString_4337D0(pOrderingTable, (" " + mAutoComplete).c_str(), 0, 232, 0, 1, 0, 40, 30, 30, 30, pIndex, FP_FromDouble(1.0), 640, 0);
        }
    }

    Font mFont;
    char mFontPalette[32];
    Font_Context mFontContext;

    bool mCommandLineEnabled = false;
    std::string mCommandLineInput;
    std::string mAutoComplete;
    std::string mLastCommand;
};

class RenderTest : public BaseGameObject
{
public:
    RenderTest()
    {
        // Don't hack the vtable else our virtuals won't get called and we can't hack the correct one back since we don't know the address of our vtable.
        DisableVTableHack disableHack;

        BaseGameObject_ctor_4DBFA0(1, 1);

        InitTestRender();

        field_6_flags |= BaseGameObject::eDrawable;

        gObjList_drawables_5C1124->Push_Back(this);
    }

    virtual void VDestructor(signed int flags) override
    {
        Destruct();
        if (flags & 1)
        {
            Mem_Free_495540(this);
        }
    }

    virtual void VRender(int** pOrderingTable) override
    {
        static PSX_Pos16 xy = {};
        static short ypos = 0;
        ypos++;
        if (ypos > 30)
        {
            ypos = 0;
        }
        xy.x = ypos;
        xy.y = ypos;
        InitType_ScreenOffset_4F5BB0(&mScreenOffset, &xy);
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mScreenOffset.mBase);

        static PSX_RECT clipRect = {};
        clipRect.x = 80;
        clipRect.y = 50;
        clipRect.w = 640 - 300;
        clipRect.h = 480 - 200;

        Init_PrimClipper_4F5B80(&mPrimClipper, &clipRect);
        // OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mPrimClipper.field_0_header);

        // Tiles

        // Sprites

        // Lines
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mLineF2.mBase.header);
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mLineF3.mBase.header);
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mLineF4.mBase.header);

        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mLineG2.mBase.header);
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mLineG3.mBase.header);
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mLineG4.mBase.header);

        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mPolyFT4.mBase.header);
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mPolyF4.mBase.header);
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mPolyG4.mBase.header);
        // TG

        // Polys
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mPolyF3.mBase.header);
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mPolyG3.mBase.header);
        // T
        // TG


    }

    void Destruct()
    {
        gObjList_drawables_5C1124->Remove_Item(this);
    }

private:
    void InitTestRender()
    {
        {
            PolyG3_Init_4F8890(&mPolyG3);

            SetRGB0(&mPolyG3, 255, 255, 255);
            SetRGB1(&mPolyG3, 0, 0, 255);
            SetRGB2(&mPolyG3, 255, 0, 255);

            SetXY0(&mPolyG3, 50, 50);
            SetXY1(&mPolyG3, 200, 50);
            SetXY2(&mPolyG3, 150, 100);
        }

        {
            PolyF3_Init(&mPolyF3);
            
            SetRGB0(&mPolyF3, 255, 255, 0);

            SetXY0(&mPolyF3, 180 + 50, 50);
            SetXY1(&mPolyF3, 180 + 200, 50);
            SetXY2(&mPolyF3, 180 + 150, 100);
        }

        {
            PolyF4_Init_4F8830(&mPolyF4);

            SetRGB0(&mPolyF4, 255, 255, 255);

            SetXY0(&mPolyF4, 150, 150);
            SetXY1(&mPolyF4, 150, 200);
            SetXY2(&mPolyF4, 200, 150);
            SetXY3(&mPolyF4, 200, 200);
        }

        {
            PolyFT4_Init_4F8870(&mPolyFT4);

            SetRGB0(&mPolyFT4, 127, 127, 127);

            int xpos = 20;
            int ypos = 30;
            int w = 128*2;
            int h = 128;

            SetXY0(&mPolyFT4, xpos, ypos);
            SetXY1(&mPolyFT4, xpos, ypos + h);
            SetXY2(&mPolyFT4, xpos + w, ypos);
            SetXY3(&mPolyFT4, xpos + w, ypos + h);

            /* TODO: Use a smaller hard coded array with the image data, also test using pallete
            std::vector<WORD> buffer(128 * 128);
            FILE* f = fopen("AbeSpa.raw", "rb");
            fread((BYTE*)buffer.data(), 1, 128 * 128 * 2, f);
            fclose(f);

            std::reverse(buffer.begin(), buffer.end());

            PSX_RECT rect = { 640, 0, 128, 128 };
            PSX_LoadImage_4F5FB0(&rect, (BYTE*)buffer.data());
            */

            Poly_Set_Blending_4F8A20(&mPolyFT4.mBase.header, 0);
            Poly_Set_SemiTrans_4F8A60(&mPolyFT4.mBase.header, 0);

            SetTPage(&mPolyFT4, PSX_getTPage_4F60E0(2, 0, 640, 0));
            SetClut(&mPolyFT4, 0);
            //SetClut(&mPolyFT4, PSX_getClut_4F6350(0, 0));

            SetUV0(&mPolyFT4, 0, 0);
            SetUV1(&mPolyFT4, 0, 128);
            SetUV2(&mPolyFT4, 128, 0);
            SetUV3(&mPolyFT4, 128, 128);

        }

        

        {
            PolyG4_Init_4F88B0(&mPolyG4);

            SetRGB0(&mPolyG4, 255, 0, 0);
            SetRGB1(&mPolyG4, 0, 0, 255);
            SetRGB2(&mPolyG4, 0, 255, 0);
            SetRGB3(&mPolyG4, 255, 0, 255);

            SetXY0(&mPolyG4, 150 + 100, 150 + 10);
            SetXY1(&mPolyG4, 150 + 100, 200 + 10);
            SetXY2(&mPolyG4, 200 + 100, 150 + 10);
            SetXY3(&mPolyG4, 200 + 100, 200 + 10);
        }

        {
            Line_F2_Init(&mLineF2);
            SetRGB0(&mLineF2, 255, 55, 55);
            SetXY0(&mLineF2, 40, 50);
            SetXY1(&mLineF2, 200, 200);
        }

        {
            Line_F3_Init(&mLineF3);
            SetRGB0(&mLineF3, 55, 255, 55);
            SetXY0(&mLineF3, 30, 30);
            SetXY1(&mLineF3, 100, 200);
            SetXY2(&mLineF3, 120, 180);
        }

        {
            Line_F4_Init(&mLineF4);
            SetRGB0(&mLineF4, 55, 155, 155);
            SetXY0(&mLineF4, 20, 40);
            SetXY1(&mLineF4, 110, 230);
            SetXY2(&mLineF4, 120, 140);
            SetXY3(&mLineF4, 140, 170);
        }

        {
            LineG2_Init(&mLineG2);
            SetRGB0(&mLineG2, 255, 255, 0);
            SetRGB1(&mLineG2, 255, 0, 255);

            SetXY0(&mLineG2, 250, 80);
            SetXY1(&mLineG2, 350, 110);
        }

        {
            LineG3_Init(&mLineG3);

            SetRGB0(&mLineG3, 55, 55, 90);
            SetRGB1(&mLineG3, 155, 60, 255);
            SetRGB2(&mLineG3, 50, 255, 255);
  
            SetXY1(&mLineG3, 100, 150);
            SetXY0(&mLineG3, 150, 200);
            SetXY2(&mLineG3, 200, 150);
        }

        {
            LineG4_Init(&mLineG4);

            SetRGB0(&mLineG4, 255, 255, 0);
            SetRGB1(&mLineG4, 255, 0, 255);
            SetRGB2(&mLineG4, 255, 255, 0);
            SetRGB3(&mLineG4, 255, 0, 255);

            SetXY1(&mLineG4, 300, 150);
            SetXY0(&mLineG4, 280, 120);
            SetXY2(&mLineG4, 20, 20);
            SetXY3(&mLineG4, 200, 50);
        }
    }

    Line_F2 mLineF2 = {};
    Line_F3 mLineF3 = {};
    Line_F4 mLineF4 = {};

    Line_G2 mLineG2 = {};
    Line_G3 mLineG3 = {};
    Line_G4 mLineG4 = {};

    Poly_G3 mPolyG3 = {};
    Poly_F3 mPolyF3 = {};
    //Poly_FT3 a;
    //Poly_GT3 b;

    Poly_G4 mPolyG4 = {};
    Poly_F4 mPolyF4 = {};
    Poly_FT4 mPolyFT4 = {};
    //Poly_GT4 b;

    Prim_ScreenOffset mScreenOffset = {};
    Prim_PrimClipper mPrimClipper = {};

    // TODO: Test SetTPage
    // TODO: Test Prim_Sprt
    // TODO: Test Prim_Tile
};

void DebugHelpers_Init() 
{
    if (GetKeyState(VK_LCONTROL) < 0)
    {
        return;
    }

    if (IsAlive())
    {
        alive_new<ObjectDebugger>();
        alive_new<DebugConsole>();
        alive_new<DebugPathRenderer>();

        DEV_CONSOLE_MESSAGE_C("Debug Console Active. Open with ~ (Tilde)", 7, 0, 150, 255);
    }

    // Test rendering diff prim types
    alive_new<RenderTest>(); // Will get nuked at LVL/Path change
}

std::vector<BYTE> FS::ReadFile(std::string filePath)
{
    std::ifstream f(filePath, std::ios::binary);
    std::vector<BYTE> v(std::istreambuf_iterator<char>{f}, {});
    return v;
}

std::vector<std::string> SplitString(const std::string& s, char seperator)
{
    std::vector<std::string> output;

    std::string::size_type prev_pos = 0, pos = 0;

    while ((pos = s.find(seperator, pos)) != std::string::npos)
    {
        std::string substring(s.substr(prev_pos, pos - prev_pos));

        output.push_back(substring);

        prev_pos = ++pos;
    }

    output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word

    return output;
}

bool StringStartsWith(std::string mainStr, std::string toMatch)
{
    // std::string::find returns 0 if toMatch is found at starting
    if (mainStr.find(toMatch) == 0)
        return true;
    else
        return false;
}

std::string StringToLower(std::string s)
{
    std::string r;
    for (auto c : s)
    {
        char l = c;
        if (l >= 65 && l <= 90)
        {
            l += 32;
        }

        r += l;
    }

    return r;
}

std::string IntToHexString(int v)
{
    std::stringstream ss;
    ss << "0x" << std::hex << v;
    return ss.str();
}

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

BaseGameObject * FindObjectOfType(int id)
{
    for (int baseObjIdx = 0; baseObjIdx < gBaseGameObject_list_BB47C4->Size(); baseObjIdx++)
    {
        BaseGameObject* pBaseGameObject = gBaseGameObject_list_BB47C4->ItemAt(baseObjIdx);

        if (!pBaseGameObject)
        {
            break;
        }

        if (pBaseGameObject->field_4_typeId == id)
            return pBaseGameObject;
    }
    return nullptr;
}

void Cheat_OpenAllDoors()
{
    for (int i = 0; i < 256; i++)
    {
        SwitchStates_Set_465FF0(static_cast<unsigned short>(i), 1);
    }

    DEV_CONSOLE_MESSAGE("(CHEAT) All doors opened", 4);
}

int sNextLinePrim = 0;
int sNextPolyF4Prim = 0;
Line_G2 sLinePrimBuffer[1024];
Poly_F4 sPolyF4PrimBuffer[1024];

void DEV::DebugFillRect(int ** ot, int layer, int x, int y, int width, int height, char r, char g, char b, bool worldspace, bool semiTransparent)
{
    Poly_F4 * mPolyF4 = &sPolyF4PrimBuffer[++sNextPolyF4Prim];
    *mPolyF4 = {};
    PolyF4_Init_4F8830(mPolyF4);

    const auto camOffset = gMap_5C3030.field_24_camera_offset;

    if (worldspace)
    {
        x -= camOffset.field_0_x.GetExponent();
        y -= camOffset.field_4_y.GetExponent();

        x = static_cast<int>(x / 0.575);
        y = static_cast<int>(y / 0.575);
        width = static_cast<int>(width / 0.575);
        height = static_cast<int>(height / 0.575);
    }

    SetRGB0(mPolyF4, r, g, b);

    struct XY { short x; short y; };
    XY points[4] =
    {
        { x, y },
        { x, y + height },
        { x + width, y },
        { x + width, y + height },
    };

    SetXY0(mPolyF4, points[0].x, points[0].y);
    SetXY1(mPolyF4, points[1].x, points[1].y);
    SetXY2(mPolyF4, points[2].x, points[2].y);
    SetXY3(mPolyF4, points[3].x, points[3].y);

    Poly_Set_SemiTrans_4F8A60(&mPolyF4->mBase.header, semiTransparent);

    OrderingTable_Add_4F8AA0(&ot[layer], &mPolyF4->mBase.header);
    pScreenManager_5BB5F4->InvalidateRect_40EC10(0, 0, 640, 240);
}

void DEV::DebugDrawRect(int ** ot, int layer, int x, int y, int width, int height, char r, char g, char b, bool worldspace, bool semiTransparent)
{
    DebugDrawLine(ot, layer, x, y, x + width, y, r, g, b, worldspace, semiTransparent);
    DebugDrawLine(ot, layer, x + width, y, x + width, y + height, r, g, b, worldspace, semiTransparent);
    DebugDrawLine(ot, layer, x + width, y + height, x, y + height, r, g, b, worldspace, semiTransparent);
    DebugDrawLine(ot, layer, x, y + height, x, y, r, g, b, worldspace, semiTransparent);
}

void DEV::DebugDrawLine(int ** ot, int layer, int x1, int y1, int x2, int y2, char r, char g, char b, bool worldspace, bool semiTransparent)
{
    Line_G2 * mLineG2 = &sLinePrimBuffer[++sNextLinePrim];
    LineG2_Init(mLineG2);

    const auto camOffset = gMap_5C3030.field_24_camera_offset;

    if (worldspace)
    {
        x1 -= camOffset.field_0_x.GetExponent();
        y1 -= camOffset.field_4_y.GetExponent();

        x2 -= camOffset.field_0_x.GetExponent();
        y2 -= camOffset.field_4_y.GetExponent();

        x1 = static_cast<int>(x1 / 0.575);
        x2 = static_cast<int>(x2 / 0.575);
    }

    // TODO: Might as well use Line_F2 here
    SetRGB0(mLineG2, r, g, b);
    SetXY0(mLineG2, static_cast<short>(x1), static_cast<short>(y1));

    SetRGB1(mLineG2, r, g, b);
    SetXY1(mLineG2, static_cast<short>(x2), static_cast<short>(y2));

    Poly_Set_SemiTrans_4F8A60(&mLineG2->mBase.header, semiTransparent);

    OrderingTable_Add_4F8AA0(&ot[layer], &mLineG2->mBase.header);
    pScreenManager_5BB5F4->InvalidateRect_40EC10(0, 0, 640, 240);
}

void DEV::DebugOnFrameDraw()
{
    sNextLinePrim = 0;
    sNextPolyF4Prim = 0;
}
