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

//#ifdef DEVELOPER_MODE
//static Font_Context debugFont;
//static ;
//debugFont.LoadFontTypeFromFile("Debug.Font", "Debug.Font_Atlas", debugFontPalette);
//field_F4_font.ctor_433590(256, reinterpret_cast<BYTE*>(debugFontPalette), &debugFont);
//#else
//
//#endif

class ObjectDebugger : public BaseGameObject
{
public:
    ObjectDebugger()
    {
        DisableVTableHack disableHack;

        BaseGameObject_ctor_4DBFA0(1, 1);
        field_6_flags |= BaseGameObject::eDrawable | 0x100;
        field_4_typeId = (BaseGameObject::Types)1001;
        if (!mFontContext.LoadFontTypeFromFile("Debug.Font", "Debug.Font_Atlas", mFontPalette))
        {
            field_6_flags = BaseGameObject::eDead;
            return;
        }

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
        for (int baseObjIdx = 0; baseObjIdx < gBaseGameObject_list_BB47C4->Size(); baseObjIdx++)
        {
            BaseGameObject* pBaseGameObject = gBaseGameObject_list_BB47C4->ItemAt(baseObjIdx);

            if (!pBaseGameObject)
            {
                break;
            }

            if (pBaseGameObject->field_6_flags & BaseGameObject::eBit04)
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
};

static std::vector<DebugConsoleMessage> sDebugConsoleMessages;

void ShowDebugConsoleMessage(std::string message, float duration)
{
    auto lines = SplitString(message, '\n');
    
    for (auto l : lines)
    {
        sDebugConsoleMessages.insert(sDebugConsoleMessages.begin(), { l, static_cast<int>(30 * duration), 250 });
    }
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

void Command_DDCheat(std::vector<std::string> args)
{
    sCommandLine_DDCheatEnabled_5CA4B5 = !sCommandLine_DDCheatEnabled_5CA4B5;
    DEV_CONSOLE_MESSAGE("DDCheat is now " + std::string(((sCommandLine_DDCheatEnabled_5CA4B5) ? "On" : "Off")), 6);
}

void Command_ObjectId(std::vector<std::string> args)
{
    ObjectDebugger::Enabled = !ObjectDebugger::Enabled;
    DEV_CONSOLE_MESSAGE("Object ID Debugger is now " + std::string(((ObjectDebugger::Enabled) ? "On" : "Off")), 6);
}

std::vector<DebugConsoleCommand> sDebugConsoleCommands = {
    { "help", -1, Command_Help, "Shows what you're looking at" },
    { "test", -1, Command_Test, "Is this thing on?" },
    { "die", -1, Command_Die, "Kills you." },
    { "murder", -1, Command_Murder, "Kill everything around you." },
    { "ddcheat", -1, Command_DDCheat, "Toggle DDCheat" },
    { "object_id", -1, Command_ObjectId, "Shows object id's on screen" },
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
        if (!mFontContext.LoadFontTypeFromFile("Debug.Font", "Debug.Font_Atlas", mFontPalette))
        {
            field_6_flags = BaseGameObject::eDead;
            return;
        }

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
                        DEV_CONSOLE_MESSAGE("Command '" + c.command + "' was expecting " + std::to_string(c.paramsCount) + " args but got " + std::to_string(commandSplit.size()), 6);
                    }
                    
                    return;
                }
            }

            DEV_CONSOLE_MESSAGE("Unknown command '" + command + "' Type help for more info.", 6);
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
        auto key = Input_ReadKey_492610();

        if (GetAsyncKeyState(VK_OEM_3) & 0x1)
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
                ParseCommand(mCommandLineInput);
                mLastCommand = mCommandLineInput;
                mCommandLineInput.clear();
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
            char color = max(0, min(message->time * 10, 255));
            int targetY = 232 - (i * 9) - 9;

            message->y += (targetY - message->y) * 0.2f;

            pIndex = mFont.DrawString_4337D0(pOrderingTable, message->message.c_str(), 0, static_cast<int>(message->y), 0, 1, 0, 40, color, color, color, pIndex, FP_FromDouble(1.0), 640, 0);
        
            message->time--;

            if (message->time <= 0)
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

void DebugHelpers_Init() {
    if (IsAlive())
    {
        alive_new<ObjectDebugger>();
        alive_new<DebugConsole>();

        DEV_CONSOLE_MESSAGE("Debug Console Active. Open with ~ (Tilde)", 7);
    }
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
