#include "stdafx.h"
#include "DebugHelpers.hpp"
#include "stdlib.hpp"
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

    }

    virtual void vsub_4DC0A0() override
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

    virtual void VRender(int** pOrderingTable) override
    {
        if (!Enabled)
            return;

        int pIndex = 0;

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
                int x = aliveObj->field_B8_xpos.GetExponent() - gMap_5C3030.field_24_camera_offset_x.GetExponent();
                int y = aliveObj->field_BC_ypos.GetExponent() - gMap_5C3030.field_28_camera_offset_y.GetExponent();

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
                
                pIndex = mFont.DrawString_4337D0(pOrderingTable, text.c_str(), x - (mFont.MeasureWidth_433700(text.c_str()) / 2) + 1, y + 1, 0, 0, 0, 39, 0, 0, 0, pIndex, FP_FromDouble(1.0), 640, 0);
                pIndex = mFont.DrawString_4337D0(pOrderingTable, text.c_str(), x - (mFont.MeasureWidth_433700(text.c_str()) / 2), y, 0, 1, 0, 40, 255, 255, 255, pIndex, FP_FromDouble(1.0), 640, 0);
            }
            
        }
    }

    Font mFont;
    char mFontPalette[32];
    Font_Context mFontContext;
};
bool ObjectDebugger::Enabled = false;

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
        POINT16 pos;
        gMap_5C3030.GetCurrentCamCoords_480680(&pos);

        sActiveHero_5C1B68->field_B8_xpos = FP(pos.x + 184);
        sActiveHero_5C1B68->field_BC_ypos = FP(pos.y + 60);
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
    gMap_5C3030.sub_480D30(level, path, cam, 5, 0, 0);
    

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
        MainMenuController::gMainMenuController->field_21A = menuCam;
        MainMenuController::gMainMenuController->field_218 = MainMenuController::GetPageIndexFromCam_4D05A0(menuCam);
        MainMenuController::gMainMenuController->field_21C = menuCam;
        MainMenuController::gMainMenuController->field_23C_T80 |= 0x200000;
    }
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
};

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

        if (mCommandLineEnabled && GetAsyncKeyState(VK_UP) & 0x1)
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

    virtual void vsub_4DC0A0() override
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
            message->y = targetY;

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
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mPolyG3.field_0_header.field_0_tag); // 30 being the "layer"
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mPolyF4.field_0_header.field_0_tag);
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mPolyG4.field_0_header.field_0_tag);

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
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mScreenOffset.field_0_header.field_0_tag);

        static PSX_RECT clipRect = {};
        clipRect.x = 80;
        clipRect.y = 50;
        clipRect.w = 640 - 300;
        clipRect.h = 480 - 200;

        Init_PrimClipper_4F5B80(&mPrimClipper, &clipRect);
        // OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mPrimClipper.field_0_header.field_0_tag);

        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mLineG2.field_0_header.field_0_tag);

        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mPolyF3.field_0_header.field_0_tag);

        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mLineG4.field_0_header.field_0_tag);


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

            mPolyG3.field_0_header.field_8_r0 = 255;
            mPolyG3.field_0_header.field_9_g0 = 255;
            mPolyG3.field_0_header.field_A_b0 = 255;
            mPolyG3.field_C_x0 = 50;
            mPolyG3.field_E_y0 = 50;

            mPolyG3.field_10_r1 = 0;
            mPolyG3.field_11_g1 = 0;
            mPolyG3.field_12_b1 = 255;
            mPolyG3.field_14_x1 = 200;
            mPolyG3.field_16_y1 = 50;

            mPolyG3.field_18_r2 = 255;
            mPolyG3.field_19_g2 = 0;
            mPolyG3.field_1A_b2 = 255;
            mPolyG3.field_1C_x2 = 150;
            mPolyG3.field_1E_y2 = 100;
        }

        {
            PolyF3_Init(&mPolyF3);

            mPolyF3.field_0_header.field_8_r0 = 255;
            mPolyF3.field_0_header.field_9_g0 = 255;
            mPolyF3.field_0_header.field_A_b0 = 0;

            mPolyF3.field_C_x0 = 180 + 50;
            mPolyF3.field_E_y0 = 50;

            mPolyF3.field_10_x1 = 180 + 200;
            mPolyF3.field_12_y1 = 50;

            mPolyF3.field_14_x2 = 180 + 150;
            mPolyF3.field_16_y2 = 100;
        }

        {
            PolyF4_Init_4F8830(&mPolyF4);
            mPolyF4.field_0_header.field_8_r0 = 255;
            mPolyF4.field_0_header.field_9_g0 = 255;
            mPolyF4.field_0_header.field_A_b0 = 255;

            struct XY { short x; short y; };
            XY points[4] =
            {
                { 150, 150 },
                { 150, 200 },
                { 200, 150 },
                { 200, 200 },
            };

            mPolyF4.field_C_x0 = points[0].x;
            mPolyF4.field_E_y0 = points[0].y;

            mPolyF4.field_10_x1 = points[1].x;
            mPolyF4.field_12_y1 = points[1].y;

            mPolyF4.field_14_x2 = points[2].x;
            mPolyF4.field_16_y2 = points[2].y;

            mPolyF4.field_18_x3 = points[3].x;
            mPolyF4.field_1A_y3 = points[3].y;
        }

        {
            PolyG4_Init_4F88B0(&mPolyG4);
            struct XY { short x; short y; };
            XY points[4] =
            {
                { 150 + 100, 150 + 10 },
                { 150 + 100, 200 + 10 },
                { 200 + 100, 150 + 10 },
                { 200 + 100, 200 + 10 },
            };

            mPolyG4.field_C_x0 = points[0].x;
            mPolyG4.field_E_y0 = points[0].y;

            mPolyG4.field_0_header.field_8_r0 = 255;
            mPolyG4.field_0_header.field_9_g0 = 0;
            mPolyG4.field_0_header.field_A_b0 = 0;


            mPolyG4.field_14_x1 = points[1].x;
            mPolyG4.field_16_y1 = points[1].y;

            mPolyG4.field_10_r1 = 0;
            mPolyG4.field_11_g1 = 0;
            mPolyG4.field_12_b1 = 255;


            mPolyG4.field_1C_x2 = points[2].x;
            mPolyG4.field_1E_y2 = points[2].y;

            mPolyG4.field_18_r2 = 0;
            mPolyG4.field_19_g2 = 255;
            mPolyG4.field_1A_b2 = 0;

            mPolyG4.field_24_x3 = points[3].x;
            mPolyG4.field_26_y3 = points[3].y;

            mPolyG4.field_20_r3 = 255;
            mPolyG4.field_21_g3 = 0;
            mPolyG4.field_22_b3 = 255;
        }

        {
            LineG2_Init(&mLineG2);

            mLineG2.field_0_header.field_8_r0 = 255;
            mLineG2.field_0_header.field_9_g0 = 255;
            mLineG2.field_0_header.field_A_b0 = 0;

            mLineG2.field_C_x0 = 250;
            mLineG2.field_E_y0 = 80;

            mLineG2.field_10_r1 = 255;
            mLineG2.field_11_g1 = 0;
            mLineG2.field_12_b1 = 255;


            mLineG2.field_14_x1 = 350;
            mLineG2.field_16_y1 = 110;
        }

        {
            LineG4_Init(&mLineG4);

            mLineG4.field_0_header.field_8_r0 = 255;
            mLineG4.field_0_header.field_9_g0 = 255;
            mLineG4.field_0_header.field_A_b0 = 0;
            mLineG4.field_C_x0 = 280;
            mLineG4.field_E_y0 = 120;

            mLineG4.field_10_r1 = 255;
            mLineG4.field_11_g1 = 0;
            mLineG4.field_12_b1 = 255;
            mLineG4.field_14_x1 = 300;
            mLineG4.field_16_y1 = 150;


            mLineG4.field_1C_x2 = 20;
            mLineG4.field_1E_y2 = 20;
            mLineG4.field_18_r2 = 255;
            mLineG4.field_19_g2 = 255;
            mLineG4.field_22_b3 = 0;

            mLineG4.field_24_x3 = 200;
            mLineG4.field_26_y3 = 50;
            mLineG4.field_20_r3 = 255;
            mLineG4.field_21_g3 = 0;
            mLineG4.field_22_b3 = 50;
        }
    }

    Line_G2 mLineG2 = {};
    Line_G4 mLineG4 = {};

    Poly_G3 mPolyG3 = {};
    Poly_F3 mPolyF3 = {};

    Poly_G4 mPolyG4 = {};
    Poly_F4 mPolyF4 = {};

    Prim_ScreenOffset mScreenOffset = {};
    Prim_PrimClipper mPrimClipper = {};

    // TODO: Test SetTPage
    // TODO: Test Prim_Sprt
    // TODO: Test Prim_Tile
    // TODO: Test Poly_FT4
};

void DebugHelpers_Init() 
{
    if (IsAlive())
    {
        alive_new<ObjectDebugger>();
        alive_new<DebugConsole>();

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
