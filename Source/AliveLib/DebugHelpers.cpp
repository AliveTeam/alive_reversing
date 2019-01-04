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
#include "Sound.hpp"
#include "RenderingTestTimData.hpp"
#include "PsxRender.hpp"
#include "LvlArchive.hpp"
#include "UXB.hpp"
#include "Movie.hpp"
#include "Text.hpp"

char _devConsoleBuffer[1000];

bool sDebugEnabled_VerboseEvents = false;

Font g_DebugGlobalFont;
int g_DebugGlobalFontPolyIndex = 0;
char g_DebugGlobalFontPalette[32];
Font_Context g_DebugGlobalFontContext;
bool g_DebugGlobalFontIsInit = false;
bool g_EnabledRaycastRendering = false;

std::vector<RaycastDebug> g_RaycastDebugList;

void DebugAddRaycast(RaycastDebug rc)
{
    g_RaycastDebugList.push_back(rc);
}

void InitDebugFont()
{
    if (!g_DebugGlobalFontIsInit)
    {
        g_DebugGlobalFontContext.LoadFontTypeCustom(reinterpret_cast<File_Font*>(sDebugFont), reinterpret_cast<Font_AtlasEntry*>(sDebugFontAtlas), g_DebugGlobalFontPalette);
        g_DebugGlobalFont.ctor_433590(1024, reinterpret_cast<BYTE*>(g_DebugGlobalFontPalette), &g_DebugGlobalFontContext);
        g_DebugGlobalFontIsInit = true;
    }
}

class ObjectDebugger : public BaseGameObject
{
public:
    ObjectDebugger()
    {
        DisableVTableHack disableHack;

        BaseGameObject_ctor_4DBFA0(1, 1);

        field_6_flags.Set(BaseGameObject::eDrawable);
        field_6_flags.Set(BaseGameObject::eSurviveDeathReset);
        field_4_typeId = BaseGameObject::Types::eDebugHelper_1001;

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

    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        Destruct();
        if (flags & 1)
        {
            Mem_Free_495540(this);
        }
        return this;
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

            if (pBaseGameObject->field_6_flags.Get(BaseGameObject::eIsBaseAnimatedWithPhysicsObj))
            {
                auto aliveObj = ((BaseAnimatedWithPhysicsGameObject*)pBaseGameObject);
                short x = FP_GetExponent(aliveObj->field_B8_xpos) - FP_GetExponent(gMap_5C3030.field_24_camera_offset.field_0_x);
                short y = FP_GetExponent(aliveObj->field_BC_ypos) - FP_GetExponent(gMap_5C3030.field_24_camera_offset.field_4_y);

                if (IsInAnimationList(&aliveObj->field_20_animation))
                {
                    if (aliveObj->field_20_animation.field_92_current_frame != -1)
                    {
                        FrameInfoHeader* framePtr = aliveObj->field_20_animation.Get_FrameHeader_40B730(aliveObj->field_20_animation.field_92_current_frame);
                        if (framePtr != nullptr)
                        {
                            y += static_cast<short>(framePtr->field_8_data.offsetAndRect.mMax.y * FP_GetDouble(aliveObj->field_CC_sprite_scale));
                        }
                    }
                }

                std::string text = std::to_string(static_cast<int>(pBaseGameObject->field_4_typeId));

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

    void DrawUI(int** /*pOrderingTable*/)
    {
        POINT mousePos = Sys_GetScreenMousePos();
        bool mouseLeftDown = GetAsyncKeyState(VK_LBUTTON) ? true : false;
        mousePos.y /= 2;

        for (int baseObjIdx = 0; baseObjIdx < gBaseGameObject_list_BB47C4->Size(); baseObjIdx++)
        {
            BaseGameObject* pBaseGameObject = gBaseGameObject_list_BB47C4->ItemAt(baseObjIdx);

            if (!pBaseGameObject)
            {
                break;
            }

            if (pBaseGameObject->field_6_flags.Get(BaseGameObject::eIsBaseAnimatedWithPhysicsObj))
            {
                auto aliveObj = ((BaseAnimatedWithPhysicsGameObject*)pBaseGameObject);

                int x = static_cast<int>((FP_GetExponent(aliveObj->field_B8_xpos) - FP_GetExponent(gMap_5C3030.field_24_camera_offset.field_0_x)) / 0.575);
                int y = static_cast<int>((FP_GetExponent(aliveObj->field_BC_ypos) - FP_GetExponent(gMap_5C3030.field_24_camera_offset.field_4_y)));

                if (Vec2Distance(static_cast<float>(x), static_cast<float>(y), static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)) < 10 && !isDragging && mouseLeftDown)
                {
                    isDragging = true;
                    mDragObject = aliveObj;
                }

                if (Vec2Distance(static_cast<float>(x), static_cast<float>(y), static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)) > 100)
                {
                    continue;
                }

                if (IsInAnimationList(&aliveObj->field_20_animation))
                {
                    if (aliveObj->field_20_animation.field_92_current_frame != -1)
                    {
                        FrameInfoHeader* framePtr = aliveObj->field_20_animation.Get_FrameHeader_40B730(aliveObj->field_20_animation.field_92_current_frame);
                        if (framePtr != nullptr)
                        {
                            y += static_cast<int>(framePtr->field_8_data.offsetAndRect.mMax.y * FP_GetDouble(aliveObj->field_CC_sprite_scale));
                        }
                    }
                }

                sFontDrawScreenSpace_5CA4B4 = 1;
                //mFontPIndex = mFont.DrawString_4337D0(pOrderingTable, "Test", x, y, 0, 0, 0, 32, 255, 255, 255, mFontPIndex, FP_FromDouble(1.0), 640, 0);
                sFontDrawScreenSpace_5CA4B4 = 0;
            }

            if (isDragging && !mouseLeftDown)
            {
                isDragging = false;
            }

            if (isDragging)
            {
                mDragObject->field_B8_xpos = FP_FromInteger(static_cast<int>(FP_GetExponent(gMap_5C3030.field_24_camera_offset.field_0_x) + (mousePos.x * 0.575)));
                mDragObject->field_BC_ypos = FP_FromInteger(static_cast<int>(FP_GetExponent(gMap_5C3030.field_24_camera_offset.field_4_y) + mousePos.y));
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
        field_6_flags.Set(BaseGameObject::eDrawable);
        field_6_flags.Set(BaseGameObject::eSurviveDeathReset);
        field_4_typeId = BaseGameObject::Types::eDebugPathRenderer_1003;

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

    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        Destruct();
        if (flags & 1)
        {
            Mem_Free_495540(this);
        }
        return this;
    }

    virtual void VUpdate() override
    {

    }

    virtual void VScreenChanged() override
    {
        // Dont kill!
    }

    struct LineColor { BYTE r; BYTE g; BYTE b; };
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
            const int gridX = 25 / 2;
            const int gridY = 20;
            const int layer = 22;

            for (int y = 0; y < 12; y++)
            {
                for (int x = 0; x < 15; x++)
                {
                    char c = ((x + y) % 2 == 0) ? 200 : 127;
                    for (int i = -1; i < 2; i++)
                    {
                        DEV::DebugDrawLine(pOrderingTable, layer, (x * gridX) + (gridX / 2) + i, y * gridY, (x * gridX) + (gridX / 2) + i, (y * gridY) + (gridY / 4), 255, 255, 255, false, gridSemiTrans);
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
                int mode = l->field_8_type;

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

                short id_x = l->field_0_x1 - FP_GetExponent(gMap_5C3030.field_24_camera_offset.field_0_x);
                short id_y = l->field_2_y1 - FP_GetExponent(gMap_5C3030.field_24_camera_offset.field_4_y);

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
    BYTE r, g, b;
};

static std::vector<DebugConsoleMessage> sDebugConsoleMessages;

void ShowDebugConsoleMessage(std::string message, float duration, BYTE r, BYTE g, BYTE b)
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

struct DebugConsoleCommand
{
    std::string command;
    int paramsCount;
    std::function<void(const std::vector<std::string>& args)> callback;
    std::string helpText;
};

extern std::vector<DebugConsoleCommand> sDebugConsoleCommands;

void Command_Help(const std::vector<std::string>& /*args*/)
{
    DEV_CONSOLE_MESSAGE("You can call the following: ", 6);
    for (auto c : sDebugConsoleCommands)
    {
        DEV_CONSOLE_MESSAGE(c.command + " - " + c.helpText, 6);
    }
}

void Command_Test(const std::vector<std::string>& args)
{
    DEV_CONSOLE_MESSAGE("You called this with " + std::to_string(args.size()) + " arguments", 5);
}

class FakeMeatGrinder : public UXB
{
public:
    FakeMeatGrinder()
    {
        field_4_typeId = BaseGameObject::Types::eGrinder_30;
        field_4_typeId = BaseGameObject::Types::eExplosion_109;
    }
};

void Command_Die(const std::vector<std::string>& /*args*/)
{
    FakeMeatGrinder fakeGrinder;
    sControlledCharacter_5C1B8C->VTakeDamage_408730(&fakeGrinder);
}

void Command_Murder(const std::vector<std::string>& /*args*/)
{
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

        if (pBaseGameObject->field_6_flags.Get(BaseGameObject::eIsBaseAliveGameObject))
        {
            auto aliveObj = ((BaseAliveGameObject*)pBaseGameObject);

            auto explosion = alive_new<BaseBomb>();
            if (explosion)
            {
                explosion->ctor_423E70(aliveObj->field_B8_xpos, aliveObj->field_BC_ypos, 0, aliveObj->field_CC_sprite_scale);
            }
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

        sActiveHero_5C1B68->field_B8_xpos = FP_FromInteger(pos.field_0_x + 184);
        sActiveHero_5C1B68->field_BC_ypos = FP_FromInteger(pos.field_2_y + 60);
        sHasTeleported = false;
        sActiveHero_5C1B68->field_106_current_state = 3;
        sActiveHero_5C1B68->field_1AC_flags.Set(Abe::Flags_1AC::e1AC_Bit7);
        sActiveHero_5C1B68->field_C2_lvl_number = gMap_5C3030.sCurrentLevelId_5C3030;
        sActiveHero_5C1B68->field_C0_path_number = gMap_5C3030.sCurrentPathId_5C3032;
        sActiveHero_5C1B68->field_100_pCollisionLine = nullptr;
        sActiveHero_5C1B68->field_F8 = sActiveHero_5C1B68->field_BC_ypos;
        sActiveHero_5C1B68->field_CC_sprite_scale = FP_FromDouble(1.0);
        sActiveHero_5C1B68->field_D6_scale = 1;
        FP rX = FP_FromInteger(0);
        FP rY = FP_FromInteger(0);
        PathLine* rUnk = nullptr;
        
        float subDevide = 368 / 10.0f;
        for (int i = 0; i < 10; i++)
        {
            int centerIndex = ((i + 5) % 10);
            FP xOffset = FP_FromDouble(pos.field_0_x + (subDevide * centerIndex));
            FP yOffset = FP_FromDouble(pos.field_2_y);
            if (sCollisions_DArray_5C1128->Raycast_417A60(xOffset, yOffset,
                xOffset, yOffset + FP_FromDouble(240), &rUnk, &rX, &rY, 1))
            {
                sActiveHero_5C1B68->field_CC_sprite_scale = FP_FromDouble(1.0);
                sActiveHero_5C1B68->field_D6_scale = 1;
                sActiveHero_5C1B68->field_B8_xpos = rX;
                sActiveHero_5C1B68->field_BC_ypos = rY;
                break;
            }
            else if (sCollisions_DArray_5C1128->Raycast_417A60(xOffset, yOffset,
                xOffset, yOffset + FP_FromDouble(240), &rUnk, &rX, &rY, 1 << 4))
            {
                sActiveHero_5C1B68->field_CC_sprite_scale = FP_FromDouble(0.5);
                sActiveHero_5C1B68->field_D6_scale = 0;
                sActiveHero_5C1B68->field_B8_xpos = rX;
                sActiveHero_5C1B68->field_BC_ypos = rY;
                break;
            }
        }
        
        sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
    }
}

void Command_ToggleBool(bool * var, std::string varName)
{
    *var = !*var;
    DEV_CONSOLE_MESSAGE(varName + " is now " + std::string(((*var) ? "On" : "Off")), 6);
}

void Command_Teleport(const std::vector<std::string>& args)
{
    short level = 0;
    if (IsStringNumber(args[0]))
    {
        level = static_cast<short>(std::stoi(args[0]));
    }
    else
    {
        bool found = false;
        for (short i = 0; i < sizeof(sPathData_559660.paths) / sizeof(PathRoot);i++)
        {
            if (!strcmpi(sPathData_559660.paths[i].field_18_lvl_name, args[0].c_str()))
            {
                level = i;
                found = true;
                break;
            }
        }
        
        if (!found)
        {
            DEV_CONSOLE_MESSAGE("Cannot find level specified!", 6);
        }
    }
    short path = static_cast<short>(std::stoi(args[1]));
    short cam = static_cast<short>(std::stoi(args[2]));
    gMap_5C3030.SetActiveCam_480D30(static_cast<LevelIds>(level), path, cam, CameraSwapEffects::eEffect5_1_FMV, 0, 0);
    

    sHasTeleported = true;

    DEV_CONSOLE_MESSAGE("Teleported", 6);
}

void Command_Event(const std::vector<std::string>& args)
{
    int eventId = std::stoi(args[0]);
    if (eventId >= Event::kEventMax)
    {
        DEV_CONSOLE_MESSAGE_C("Invalid event id", 6, 127, 0, 0);
        return;
    }
    Event_Broadcast_422BC0(static_cast<Event>(eventId), sControlledCharacter_5C1B8C);
}

void Command_Menu(const std::vector<std::string>& args)
{
    int menuCam = std::stoi(args[0]);
    if (MainMenuController::gMainMenuController != nullptr)
    {
        MainMenuController::gMainMenuController->field_21A_target_cam = static_cast<short>(menuCam);
        MainMenuController::gMainMenuController->field_218_target_page_index = static_cast<short>(MainMenuController::GetPageIndexFromCam_4D05A0(menuCam));
        MainMenuController::gMainMenuController->field_21C_bDoScreenTransistionEffect = static_cast<short>(menuCam);
        MainMenuController::gMainMenuController->field_23C_T80.Set(MainMenuController::Flags::eBit22);
    }
}

void Command_Midi1(const std::vector<std::string>& args)
{
    const unsigned __int8 arg1 = static_cast<unsigned __int8>(std::stoi(args[0]));

    SFX_Play_46FA90(arg1, 0, 0x10000);

    DEV_CONSOLE_MESSAGE("Played Midi1", 6);
}

void Command_DDV(const std::vector<std::string>& args)
{
    SND_StopAll_4CB060();

    std::string filePath = args[0] + ".STR";
    while (!DDV_Play_493210(filePath.c_str()))
    {
        if (word_5C1BA0)
        {
            break;
        }

        if (!Display_Full_Screen_Message_Blocking_465820(sPathData_559660.paths[sLevelId_dword_5CA408].field_1C_unused, 1))
        {
            break;
        }
    }
}

void Command_SetState(const std::vector<std::string>& args)
{
    if (sControlledCharacter_5C1B8C->field_4_typeId != BaseGameObject::Types::eType_Abe_69)
    {
        DEV_CONSOLE_MESSAGE_C("Setting state not supported on this object (only allowed for abe)!", 6, 255, 0, 0);
        return;
    }

    __int16  state = static_cast<__int16 >(std::stoi(args[0]));
    Abe* pAbe = static_cast<Abe*>(sControlledCharacter_5C1B8C);
    auto resource = pAbe->StateToAnimResource_44AAB0(state);

    if (resource != nullptr)
    {
        pAbe->field_106_current_state = state;
        pAbe->field_20_animation.Set_Animation_Data_409C80(sAbeFrameOffsetTable_554B18[state], resource);
        DEV_CONSOLE_PRINTF("Set state to %i", state);
    }
    else
    {
        DEV_CONSOLE_PRINTF("Cannot set state to %i! Resource NULL", state);
    }
}

std::vector<DebugConsoleCommand> sDebugConsoleCommands = {
    { "help", -1, Command_Help, "Shows what you're looking at" },
    { "test", -1, Command_Test, "Is this thing on?" },
    { "die", -1, Command_Die, "Kills you." },
    { "murder", -1, Command_Murder, "Kill everything around you." },
    { "ddcheat", -1, [](const std::vector<std::string>& /*args*/) { Command_ToggleBool(&sCommandLine_DDCheatEnabled_5CA4B5, "DDCheat"); }, "Toggle DDCheat" },
    { "object_id", -1, [](const std::vector<std::string>& /*args*/) { Command_ToggleBool(&ObjectDebugger::Enabled, "Object ID Debugger"); }, "Shows object id's on screen" },
    { "no_frame_skip", -1, [](const std::vector<std::string>& /*args*/) { Command_ToggleBool(&sCommandLine_NoFrameSkip_5CA4D1, "No Frame Skip"); }, "Toggle No Frame Skip" },
    { "fps", -1, [](const std::vector<std::string>& /*args*/) { Command_ToggleBool(&sCommandLine_ShowFps_5CA4D0, "FPS"); }, "Toggle FPS" },
    { "reverb", -1, [](const std::vector<std::string>& /*args*/) { Command_ToggleBool(&gReverbEnabled, "Reverb"); }, "Toggle Reverb (New Sound Engine)" },
    { "raycast", -1, [](const std::vector<std::string>& /*args*/) { Command_ToggleBool(&g_EnabledRaycastRendering, "Raycast Debug"); }, "Toggle Raycast Debug" },
    { "verbose_events", -1, [](const std::vector<std::string>& /*args*/) { Command_ToggleBool(&sDebugEnabled_VerboseEvents, "Verbose Events"); }, "Toggle Verbose Events" },
    { "open_doors", -1, [](const std::vector<std::string>& /*args*/) { Cheat_OpenAllDoors(); }, "Open all doors." },
    { "teleport", 3, Command_Teleport, "Teleport to a cam. (LEVEL, PATH, CAM)" },
    { "event", 1, Command_Event, "Broadcast's an event (EVENT ID)" },
    //{ "menu", 1, Command_Menu, "Changes to given menu cam" },
    { "state", 1, Command_SetState, "Sets currently controlled objects state." },
    { "ddv", 1, Command_DDV, "Plays a ddv" },
    { "midi1", 1, Command_Midi1, "Play sound using midi func 1" },
    { "path_lines", -1, [](const std::vector<std::string>& /*args*/) { Command_ToggleBool(&DebugPathRenderer::Enabled, "Path Lines"); }, "Renders path lines on screen" },
    { "grid", -1, [](const std::vector<std::string>& /*args*/) { Command_ToggleBool(&DebugPathRenderer::GridEnabled, "Grid"); }, "Renders grid on screen" },
    { "pcopen", -1, [](const std::vector<std::string>& /*args*/) { Command_ToggleBool(reinterpret_cast<bool*>(&sbEnable_PCOpen_5CA4B0), "PCOpen"); }, "Toggles PCOpen" },
};

//

class DebugConsole : public BaseGameObject
{
public:
    DebugConsole()
    {
        DisableVTableHack disableHack;

        BaseGameObject_ctor_4DBFA0(1, 1);
        field_6_flags.Set(BaseGameObject::eDrawable);
        field_6_flags.Set(BaseGameObject::eSurviveDeathReset);
        field_4_typeId = BaseGameObject::Types::eDebugConsole_1002;
        
        mFontContext.LoadFontTypeCustom(reinterpret_cast<File_Font*>(sDebugFont), reinterpret_cast<Font_AtlasEntry*>(sDebugFontAtlas), mFontPalette);
        mFont.ctor_433590(4096 * 2, reinterpret_cast<BYTE*>(mFontPalette), &mFontContext);

        gObjList_drawables_5C1124->Push_Back(this);

        InitDebugFont();

        Map::LoadResource_4DBE00("ABEBLOW.BAN", ResourceManager::Resource_Animation, kAbeblowResID, 0, 0);
        Map::LoadResource_4DBE00("EXPLODE.BND", ResourceManager::Resource_Animation, kAbebombResID, 0, 0);
        Map::LoadResource_4DBE00("EXPLODE.BND", ResourceManager::Resource_Animation, kDebrisID00, 0, 0);
        Map::LoadResource_4DBE00("EXPLODE.BND", ResourceManager::Resource_Animation, kBgexpldResID, 0, 0);
    }

    void Destruct()
    {
        BaseGameObject_dtor_4DBEC0();
        gObjList_drawables_5C1124->Remove_Item(this);
    }

    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        Destruct();
        if (flags & 1)
        {
            Mem_Free_495540(this);
        }
        return this;
    }

    void ParseCommand(std::string command)
    {
        command = StringToLower(command);
        auto commandSplit = SplitString(command, ' ');

        if (!commandSplit.empty())
        {
            for (const auto& c : sDebugConsoleCommands)
            {
                if (commandSplit[0] == c.command)
                {
                    commandSplit.erase(commandSplit.begin());

                    if (c.paramsCount == -1 || c.paramsCount == static_cast<int>(commandSplit.size()))
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
        char titleBuffer[1000];
        char camBuffer[32];
        Path_Format_CameraName_460FB0(
            camBuffer,
            gMap_5C3030.sCurrentLevelId_5C3030,
            gMap_5C3030.sCurrentPathId_5C3032,
            gMap_5C3030.sCurrentCamId_5C3034);
        sprintf(titleBuffer, "Oddworld Abe's Exoddus DEV MODE - %s", camBuffer);
        Sys_SetWindowText(Sys_GetHWnd_4F2C70(), titleBuffer);

        Command_HelperUpdate();

        const char key = static_cast<char>(Input_GetLastPressedKey_492610());
        if (Input_IsVKPressed_4EDD40(VK_OEM_3))
        {
            mCommandLineEnabled = !mCommandLineEnabled;

            if (mCommandLineEnabled)
            {
                Input_DisableInput_4EDDC0();
            }
            else
            {
                Input_EnableInput_4EDDD0();
            }
        }

        const char* allowedChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 !-+@#$%^&*()_";

        if (Input_IsVKPressed_4EDD40(VK_UP) && mCommandLineEnabled)
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
            int targetY = 232 - (i * 9) - 9;

            message->y = static_cast<float>(targetY);

            pIndex = mFont.DrawString_4337D0(pOrderingTable, message->message.c_str(), 0, static_cast<short>(message->y), 0, 1, 0, 40, message->r, message->g, message->b, pIndex, FP_FromDouble(1.0), 640, 0);
            pIndex = mFont.DrawString_4337D0(pOrderingTable, message->message.c_str(), 1, static_cast<short>(message->y) + 1, 0, 1, 0, 40, 0, 0, 0, pIndex, FP_FromDouble(1.0), 640, 0);
        
            message->time--;

            if (message->time <= 0 || i > 64)
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

struct PsxTimHeader
{
    DWORD mMagic;   // 0x10
    DWORD mFlag;    // 0x08 4BPP, 0x09 8BPP, 0x02 16BPP
    DWORD mUnknown;
    WORD mClutX;
    WORD mClutY;
    WORD mNumClutColours;
    WORD mClutCount;
};

struct PsxTimImageHeader
{
    DWORD mSizeInBytes; // includes header size
    PSX_RECT mImageRect;
};

struct TimInfo
{
    short mRenderWidth;
    short mHeight;
    WORD mTPage;
    WORD mClut;
};

static void LoadTIM(TimInfo* pInfo, const BYTE* timBuffer, char abr)
{
    const PsxTimHeader* pHeader = reinterpret_cast<const PsxTimHeader*>(timBuffer);
    const PsxTimImageHeader* pImgHeader = nullptr;
    if (pHeader->mFlag == 2) // 16 bit
    {
        pImgHeader = reinterpret_cast<const PsxTimImageHeader*>(timBuffer + (sizeof(DWORD) * 2)); // skip magic marker and flags
        pInfo->mClut = 0;
    }
    else
    {
        int clutSkip = pHeader->mClutCount * pHeader->mNumClutColours * 2;
        pImgHeader = reinterpret_cast<const PsxTimImageHeader*>(timBuffer + sizeof(PsxTimHeader) + clutSkip);

        PSX_RECT clutRect = { static_cast<short>(pHeader->mClutX), static_cast<short>(pHeader->mClutY), static_cast<short>(pHeader->mNumClutColours), static_cast<short>(1) };
        PSX_LoadImage16_4F5E20(&clutRect, (BYTE*)&pHeader[1]);

        pInfo->mClut = static_cast<WORD>(PSX_getClut_4F6350(pHeader->mClutX, pHeader->mClutY));
    }

    if (pHeader->mFlag == 2) // 16 bit
    {
        // Raw pixel data, convert it
        PSX_LoadImage16_4F5E20(&pImgHeader->mImageRect, (BYTE*)&pImgHeader[1]);
    }
    else
    {
        // Bytes or nibbles of pal indices, don't convert it
        PSX_LoadImage_4F5FB0(&pImgHeader->mImageRect, (BYTE*)&pImgHeader[1]);
    }

    int mode = 2;
    if (pHeader->mFlag == 2) // 16 bit
    {
        mode = 2;
    }
    else if (pHeader->mFlag == 9) // 8 bit
    {
        mode = 1;
    }
    else if (pHeader->mFlag == 8) // 4 bit
    {
        mode = 0;
    }
    else
    {
        ALIVE_FATAL("Unknown TIM flags");
    }

    int widthMultipler = 1;
    if (mode == 0)
    {
        widthMultipler = 4;
    }
    else if (mode == 1)
    {
        widthMultipler = 2;
    }

    pInfo->mRenderWidth = static_cast<short>(pImgHeader->mImageRect.w * widthMultipler);
    pInfo->mHeight = pImgHeader->mImageRect.h;
    pInfo->mTPage = static_cast<WORD>(PSX_getTPage_4F60E0(static_cast<char>(mode), abr, pImgHeader->mImageRect.x, pImgHeader->mImageRect.y));
}

class RenderTest_AllPrims
{
public:
    RenderTest_AllPrims()
    {
        InitTestRender();
    }

    void Render(int** pOrderingTable)
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
        for (int i = 0; i < 10; i++)
        {
            OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mTiles[i].mBase.header);
        }
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mTile8.mBase.header);
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mTile16.mBase.header);
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mTile.mBase.header);

        // Sprites
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mSprt8.mBase.header);
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mSprt8_TPage.mBase);

        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mSprt16.mBase.header);
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mSprt16_TPage.mBase);

        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mSprt.mBase.header);
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mSprt_TPage.mBase);

        // Lines
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mLineF2.mBase.header);
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mLineF3.mBase.header);
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mLineF4.mBase.header);

        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mLineG2.mBase.header);
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mLineG3.mBase.header);
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mLineG4.mBase.header);

        for (int i = 0; i < 4; i++)
        {
            OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mPolyFT4[i].mBase.header);
        }

        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mPolyGT4.mBase.header);
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mPolyF4.mBase.header);
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mPolyG4.mBase.header);

        // Polys
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mPolyF3.mBase.header);
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mPolyG3.mBase.header);
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mPolyFT3.mBase.header);
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mPolyGT3.mBase.header);
    }

private:

    void InitTestRender()
    {
        {
            PolyG3_Init_4F8890(&mPolyG3);

            SetRGB0(&mPolyG3, 255, 0, 0);
            SetRGB1(&mPolyG3, 0, 255, 0);
            SetRGB2(&mPolyG3, 0, 0, 255);

            SetXY0(&mPolyG3, 180, 190);
            SetXY1(&mPolyG3, 180, 190 + 20);
            SetXY2(&mPolyG3, 180 + (20 * 2), 190);
        }

        {
            PolyF3_Init(&mPolyF3);

            SetRGB0(&mPolyF3, 255, 255, 0);

            SetXY0(&mPolyF3, 240, 190);
            SetXY1(&mPolyF3, 240, 190 + 20);
            SetXY2(&mPolyF3, 240 + (20 * 2), 190);
        }

        {
            PolyFT3_Init(&mPolyFT3);

            TimInfo timInfo = {};
            LoadTIM(&timInfo, &tim_16_bit[0], 3);

            SetRGB0(&mPolyFT3, 127, 127, 127);

            Poly_Set_Blending_4F8A20(&mPolyFT3.mBase.header, 1);
            Poly_Set_SemiTrans_4F8A60(&mPolyFT3.mBase.header, 1);
            SetTPage(&mPolyFT3, timInfo.mTPage);
            SetClut(&mPolyFT3, timInfo.mClut);

            const short xpos = 30;
            const short ypos = 160;
            const short w = timInfo.mRenderWidth * 2; // All width doubled due to PC doubling the render width
            const short h = timInfo.mHeight;

            SetXY0(&mPolyFT3, xpos, ypos);
            SetXY1(&mPolyFT3, xpos, ypos + h);
            SetXY2(&mPolyFT3, xpos + w, ypos);

            // This assumes the texture data is at 0,0 in the active texture page
            SetUV0(&mPolyFT3, 0, 0);
            SetUV1(&mPolyFT3, 0, static_cast<BYTE>(timInfo.mHeight));
            SetUV2(&mPolyFT3, static_cast<BYTE>(timInfo.mRenderWidth), 0);
        }

        {
            PolyGT3_Init(&mPolyGT3);

            TimInfo timInfo = {};
            LoadTIM(&timInfo, &tim_16_bit[0], 0);

            SetRGB0(&mPolyGT3, 255, 0, 255);
            // Much like PolyGT4 these have no effect, so same behavior as PolyFT3
            SetRGB1(&mPolyGT3, 255, 0, 0);
            SetRGB2(&mPolyGT3, 0, 255, 0);

            Poly_Set_Blending_4F8A20(&mPolyGT3.mBase.header, 0);
            Poly_Set_SemiTrans_4F8A60(&mPolyGT3.mBase.header, 0);
            SetTPage(&mPolyGT3, timInfo.mTPage);
            SetClut(&mPolyGT3, timInfo.mClut);

            const short xpos = 180;
            const short ypos = 90;
            const short w = timInfo.mRenderWidth * 2; // All width doubled due to PC doubling the render width
            const short h = timInfo.mHeight;

            SetXY0(&mPolyGT3, xpos, ypos);
            SetXY1(&mPolyGT3, xpos, ypos + h);
            SetXY2(&mPolyGT3, xpos + w, ypos);

            // This assumes the texture data is at 0,0 in the active texture page
            SetUV0(&mPolyGT3, 0, 0);
            SetUV1(&mPolyGT3, 0, static_cast<BYTE>(timInfo.mHeight));
            SetUV2(&mPolyGT3, static_cast<BYTE>(timInfo.mRenderWidth), 0);
        }

        {
            PolyF4_Init_4F8830(&mPolyF4);

            SetRGB0(&mPolyF4, 255, 255, 255);

            SetXY0(&mPolyF4, 180, 160);
            SetXY1(&mPolyF4, 180, 160 + 20);
            SetXY2(&mPolyF4, 180 + (20 * 2), 160);
            SetXY3(&mPolyF4, 180 + (20 * 2), 160 + 20);
        }

        {
            for (short i = 0; i < 4; i++)
            {
                PolyFT4_Init_4F8870(&mPolyFT4[i]);

                TimInfo timInfo = {};
                if (i == 0)
                {
                    LoadTIM(&timInfo, &tim_16_bit[0], 0);
                    Poly_Set_Blending_4F8A20(&mPolyFT4[i].mBase.header, 0);
                    Poly_Set_SemiTrans_4F8A60(&mPolyFT4[i].mBase.header, 0);
                }
                else if (i == 1)
                {
                    LoadTIM(&timInfo, &tim_8_bit[0], 1);
                    Poly_Set_Blending_4F8A20(&mPolyFT4[i].mBase.header, 1);
                    Poly_Set_SemiTrans_4F8A60(&mPolyFT4[i].mBase.header, 1);
                }
                else if (i == 2)
                {
                    LoadTIM(&timInfo, &tim_8_bit2[0], 2);
                    Poly_Set_Blending_4F8A20(&mPolyFT4[i].mBase.header, 1);
                    Poly_Set_SemiTrans_4F8A60(&mPolyFT4[i].mBase.header, 1);
                }
                else
                {
                    LoadTIM(&timInfo, &tim_4_bit[0], 3);
                    Poly_Set_Blending_4F8A20(&mPolyFT4[i].mBase.header, 1);
                    Poly_Set_SemiTrans_4F8A60(&mPolyFT4[i].mBase.header, 1);
                }

                SetRGB0(&mPolyFT4[i], 127, 127, 127);
                SetTPage(&mPolyFT4[i], timInfo.mTPage);
                SetClut(&mPolyFT4[i], timInfo.mClut);

                const short xpos = 30 + (150 * i);
                const short ypos = 20;
                const short w = timInfo.mRenderWidth * 2; // All width doubled due to PC doubling the render width
                const short h = timInfo.mHeight;

                SetXY0(&mPolyFT4[i], xpos, ypos);
                SetXY1(&mPolyFT4[i], xpos, ypos + h);
                SetXY2(&mPolyFT4[i], xpos + w, ypos);
                SetXY3(&mPolyFT4[i], xpos + w, ypos + h);

                // This assumes the texture data is at 0,0 in the active texture page
                SetUV0(&mPolyFT4[i], 0, 0);
                SetUV1(&mPolyFT4[i], 0, static_cast<BYTE>(timInfo.mHeight));
                SetUV2(&mPolyFT4[i], static_cast<BYTE>(timInfo.mRenderWidth), 0);
                SetUV3(&mPolyFT4[i], static_cast<BYTE>(timInfo.mRenderWidth), static_cast<BYTE>(timInfo.mHeight));
            }
        }

        {
            PolyGT4_Init(&mPolyGT4);

            TimInfo timInfo = {};
            LoadTIM(&timInfo, &tim_16_bit[0], 0);

            // So it appears that only RGB0 changes the colour, so GT4 behaves the same
            // as FT4.
            SetRGB0(&mPolyGT4, 0, 255, 0);
            SetRGB1(&mPolyGT4, 0, 0, 0);
            SetRGB2(&mPolyGT4, 0, 0, 0);
            SetRGB3(&mPolyGT4, 0, 0, 0);

            Poly_Set_Blending_4F8A20(&mPolyGT4.mBase.header, 0);
            Poly_Set_SemiTrans_4F8A60(&mPolyGT4.mBase.header, 1);
            SetTPage(&mPolyGT4, timInfo.mTPage);
            SetClut(&mPolyGT4, timInfo.mClut);

            const short xpos = 30;
            const short ypos = 90;
            const short w = timInfo.mRenderWidth * 2; // All width doubled due to PC doubling the render width
            const short h = timInfo.mHeight;

            SetXY0(&mPolyGT4, xpos, ypos);
            SetXY1(&mPolyGT4, xpos, ypos + h);
            SetXY2(&mPolyGT4, xpos + w, ypos);
            SetXY3(&mPolyGT4, xpos + w, ypos + h);

            // This assumes the texture data is at 0,0 in the active texture page
            SetUV0(&mPolyGT4, 0, 0);
            SetUV1(&mPolyGT4, 0, static_cast<BYTE>(timInfo.mHeight));
            SetUV2(&mPolyGT4, static_cast<BYTE>(timInfo.mRenderWidth), 0);
            SetUV3(&mPolyGT4, static_cast<BYTE>(timInfo.mRenderWidth), static_cast<BYTE>(timInfo.mHeight));
        }

        {
            PolyG4_Init_4F88B0(&mPolyG4);

            SetRGB0(&mPolyG4, 255, 0, 0);
            SetRGB1(&mPolyG4, 0, 0, 255);
            SetRGB2(&mPolyG4, 0, 255, 0);
            SetRGB3(&mPolyG4, 255, 0, 255);

            SetXY0(&mPolyG4, 240, 160);
            SetXY1(&mPolyG4, 240, 160 + 20);
            SetXY2(&mPolyG4, 240 + (20 * 2), 160);
            SetXY3(&mPolyG4, 240 + (20 * 2), 160 + 20);
        }

        {
            Line_F2_Init(&mLineF2);
            SetRGB0(&mLineF2, 0, 0, 255);
            SetXY0(&mLineF2, 30, 9);
            SetXY1(&mLineF2, 30 + 80, 9);
        }

        {
            Line_F3_Init(&mLineF3);
            SetRGB0(&mLineF3, 55, 255, 55);
            SetXY0(&mLineF3, 30, 11);
            SetXY1(&mLineF3, 30 + 80, 11);
            SetXY2(&mLineF3, 30 + 80, 15);
        }

        {
            Line_F4_Init(&mLineF4);
            SetRGB0(&mLineF4, 55, 155, 155);

            SetXY0(&mLineF4, 30, 18);
            SetXY1(&mLineF4, 30 + 140, 18);
            SetXY2(&mLineF4, 30 + 140, 85);
            SetXY3(&mLineF4, 30, 85);
        }

        {
            LineG2_Init(&mLineG2);
            SetRGB0(&mLineG2, 255, 255, 0);
            SetRGB1(&mLineG2, 255, 0, 255);

            SetXY0(&mLineG2, 320, 90);
            SetXY1(&mLineG2, 420, 90);
        }

        {
            LineG3_Init(&mLineG3);

            SetRGB0(&mLineG3, 55, 55, 90);
            SetRGB1(&mLineG3, 155, 60, 255);
            SetRGB2(&mLineG3, 50, 255, 255);

            SetXY0(&mLineG3, 320, 120);
            SetXY1(&mLineG3, 420, 120);
            SetXY2(&mLineG3, 320, 100);
        }

        {
            LineG4_Init(&mLineG4);

            SetRGB0(&mLineG4, 255, 0, 0);
            SetRGB1(&mLineG4, 0, 255, 0);
            SetRGB2(&mLineG4, 0, 0, 255);
            SetRGB3(&mLineG4, 255, 0, 255);

            SetXY0(&mLineG4, 320, 140);
            SetXY1(&mLineG4, 420, 140);
            SetXY2(&mLineG4, 320, 180);
            SetXY3(&mLineG4, 420, 180);
        }

        {
            for (BYTE i = 0; i < 10; i++)
            {
                Init_Tile1(&mTiles[i]);
                SetRGB0(&mTiles[i], 255, i * 12, i * 12);
                SetXY0(&mTiles[i], 30 + (i * 5), 10);
            }
        }

        {
            Init_Tile8(&mTile8);
            SetRGB0(&mTile8, 55, 80, 55);
            SetXY0(&mTile8, 180, 5);
        }

        {
            Init_Tile16(&mTile16);
            SetRGB0(&mTile16, 100, 255, 0);
            SetXY0(&mTile16, 200, 2);
        }

        {
            Init_Tile(&mTile);
            SetRGB0(&mTile, 30, 50, 90);
            SetXY0(&mTile, 450, 80);
            mTile.field_14_w = 50;
            mTile.field_16_h = 90;
        }

        {
            Init_Sprt_8(&mSprt8);
            SetRGB0(&mSprt8, 127, 127, 127);
            SetXY0(&mSprt8, 520, 80);

            TimInfo timInfo = {};
            LoadTIM(&timInfo, &tim_16_bit[0], 0);

            SetClut(&mSprt8, timInfo.mClut);
            SetUV0(&mSprt8, 30, 30);

            Init_SetTPage_4F5B60(&mSprt8_TPage, 0, 0, timInfo.mTPage);
        }

        {
            Init_Sprt_16(&mSprt16);
            SetRGB0(&mSprt16, 127, 127, 127);
            SetXY0(&mSprt16, 520, 120);

            TimInfo timInfo = {};
            LoadTIM(&timInfo, &tim_16_bit[0], 0);

            SetClut(&mSprt8, timInfo.mClut);
            SetUV0(&mSprt16, 30, 30);

            Init_SetTPage_4F5B60(&mSprt16_TPage, 0, 0, timInfo.mTPage);
        }

        {
            Sprt_Init_4F8910(&mSprt);
            SetRGB0(&mSprt, 255, 127, 127);
            SetXY0(&mSprt, 550, 120);

            TimInfo timInfo = {};
            LoadTIM(&timInfo, &tim_16_bit[0], 0);

            SetClut(&mSprt, timInfo.mClut);
            SetUV0(&mSprt, 10, 10);

            mSprt.field_14_w = 50;
            mSprt.field_16_h = 50;

            Init_SetTPage_4F5B60(&mSprt_TPage, 0, 0, timInfo.mTPage);
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
    Poly_FT3 mPolyFT3 = {};
    Poly_GT3 mPolyGT3 = {};

    Poly_G4 mPolyG4 = {};
    Poly_F4 mPolyF4 = {};
    Poly_FT4 mPolyFT4[4] = {};
    Poly_GT4 mPolyGT4 = {};

    Prim_Tile_1 mTiles[10] = {};
    Prim_Tile_8 mTile8 = {};
    Prim_Tile_16 mTile16 = {};
    Prim_Tile mTile = {};

    Prim_SetTPage mSprt8_TPage = {};
    Prim_Sprt_8 mSprt8 = {};

    Prim_SetTPage mSprt16_TPage = {};
    Prim_Sprt_16 mSprt16 = {};

    Prim_SetTPage mSprt_TPage = {};
    Prim_Sprt mSprt = {};

    // TODO: Type 2
    // TODO: Type 0x83 (move image?)
    // TODO: Type 0x84 (used in gas rendering)

    Prim_ScreenOffset mScreenOffset = {};
    Prim_PrimClipper mPrimClipper = {};
};

class Poly_F3_Test
{
public:
    Poly_F3_Test()
    {
        for (auto& poly : mPolys)
        {
            PolyF3_Init(&poly);
        }
        Update();

        SetRGB0(&mPolys[0], 127, 127, 127);
        SetRGB0(&mPolys[1], 255, 0, 0);
        SetRGB0(&mPolys[2], 0, 255, 0);
        SetRGB0(&mPolys[3], 0, 0, 255);

        Poly_Set_SemiTrans_4F8A60(&mPolys[2].mBase.header, TRUE);
        Poly_Set_SemiTrans_4F8A60(&mPolys[3].mBase.header, TRUE);

        Poly_Set_Blending_4F8A20(&mPolys[0].mBase.header, TRUE);
        Poly_Set_Blending_4F8A20(&mPolys[1].mBase.header, TRUE);
    }

    void Render(int** pOrderingTable)
    {
        for (int i = 0; i < 4; i++)
        {
            OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mPolys[i].mBase.header);
        }
    }

    void Update()
    {
        mWidth++;
        if (mWidth > 100)
        {
            mWidth = 60;
        }

        short xpos = mXPos;
        for (auto& poly : mPolys)
        {
            SetXY0(&poly, xpos, mYPos);
            SetXY1(&poly, xpos, mYPos + mHeight);
            SetXY2(&poly, xpos + (mWidth * 2), mYPos);

            xpos += (mWidth * 2) - 60;
        }
    }

private:
    short mWidth = 60;
    short mHeight = 150;
    short mXPos = 50;
    short mYPos = 50/2;
    Poly_F3 mPolys[4];
};


class Poly_F_Test
{
public:
    Poly_F_Test()
    {
        PolyF3_Init(&mPoly_F3);
        SetRGB0(&mPoly_F3, 255, 0, 0);
        Poly_Set_SemiTrans_4F8A60(&mPoly_F3.mBase.header, FALSE);
        Poly_Set_Blending_4F8A20(&mPoly_F3.mBase.header, FALSE);

        SetXY0(&mPoly_F3, 50, 80);
        SetXY1(&mPoly_F3, 300, 200);
        SetXY2(&mPoly_F3, 200, 100);

        PolyF4_Init_4F8830(&mPoly_F4);
        SetRGB0(&mPoly_F4, 255, 0, 255);
        Poly_Set_SemiTrans_4F8A60(&mPoly_F4.mBase.header, FALSE);
        Poly_Set_Blending_4F8A20(&mPoly_F4.mBase.header, FALSE);

        SetXY0(&mPoly_F4, 350, 100);
        SetXY1(&mPoly_F4, 550, 100);
        SetXY2(&mPoly_F4, 300, 200);
        SetXY3(&mPoly_F4, 500, 50);

        for (int i = 0; i < 4; i++)
        {
            Init_Tile8(&mPoly_F4_Verts[i]);
        }
        SetRGB0(&mPoly_F4_Verts[0], 255, 0, 0);
        SetXY0(&mPoly_F4_Verts[0], 350, 100);

        SetRGB0(&mPoly_F4_Verts[1], 0, 255, 0);
        SetXY0(&mPoly_F4_Verts[1], 550, 100);

        SetRGB0(&mPoly_F4_Verts[2], 0, 0, 255);
        SetXY0(&mPoly_F4_Verts[2], 300, 200);

        SetRGB0(&mPoly_F4_Verts[3], 255, 255, 255);
        SetXY0(&mPoly_F4_Verts[3], 500, 50);
    }

    void Render(int** pOrderingTable)
    {
        for (int i = 0; i < 4; i++)
        {
            OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mPoly_F4_Verts[i].mBase.header);
        }

        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mPoly_F3.mBase.header);
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mPoly_F4.mBase.header);
    }

    void Update()
    {

    }

private:
    Poly_F3 mPoly_F3;
    Poly_F4 mPoly_F4;
    Prim_Tile_8 mPoly_F4_Verts[4];
};


class RenderTest : public BaseGameObject
{
public:
    RenderTest()
    {
        // Don't hack the vtable else our virtuals won't get called and we can't hack the correct one back since we don't know the address of our vtable.
        DisableVTableHack disableHack;

        BaseGameObject_ctor_4DBFA0(1, 1);

        field_6_flags.Set(BaseGameObject::eDrawable);
        field_6_flags.Set(BaseGameObject::eUpdatable);

        gObjList_drawables_5C1124->Push_Back(this);
    }

    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        Destruct();
        if (flags & 1)
        {
            Mem_Free_495540(this);
        }
        return this;
    }

    virtual void VUpdate() override
    {
        field_1C_update_delay = 4;
        mPoly_F3_Test.Update();
        mPoly_F_Test.Update();
    }

    virtual void VRender(int** pOrderingTable) override
    {
        PSX_RECT screen = { 0, 0, 640, 240 };
        PSX_ClearImage_4F5BD0(&screen, 0, 0, 0);

        //pScreenManager_5BB5F4->InvalidateRect_40EC10(0, 0, 640, 240);

        mPoly_F_Test.Render(pOrderingTable);

        mPoly_F3_Test.Render(pOrderingTable);

        mAllPrims.Render(pOrderingTable);
    }

    void Destruct()
    {
        gObjList_drawables_5C1124->Remove_Item(this);
    }

private:
    RenderTest_AllPrims mAllPrims;
    Poly_F3_Test mPoly_F3_Test;
    Poly_F_Test mPoly_F_Test;
};

#include "ScreenManager.hpp"
#include "VRam.hpp"

static WORD RGB888toRGB565(unsigned int r, unsigned int g, unsigned int b)
{
    return static_cast<WORD>((r >> 3 << 11) + (g >> 2 << 5) + (b >> 3));
}

const WORD kTestImagePal[16] =
{
    RGB888toRGB565(237, 28,  36),
    RGB888toRGB565(255, 255, 255),
    RGB888toRGB565(127, 127, 127),
    RGB888toRGB565(63,  72,  204),
    RGB888toRGB565(34,  177, 76),
    RGB888toRGB565(255, 174, 201),
    RGB888toRGB565(255, 127, 39),
    RGB888toRGB565(163, 73,  164),
};

const BYTE kTestImg[4][8] =
{
    { 0, 0, 1, 2, 2, 2, 1, 3 },
    { 0, 0, 1, 1, 2, 1, 1, 3 },
    { 4, 4, 5, 1, 2, 1, 6, 7 },
    { 4, 4, 1, 5, 2, 6, 1, 7 },
};

inline static BYTE AsByte(BYTE nibble1, BYTE nibble2)
{
    return (nibble2 << 4) | nibble1;
}

// Pack kTestImg nibbles into bytes
const BYTE kTestImage[4][4] =
{
    { AsByte(kTestImg[0][0],kTestImg[0][1]), AsByte(kTestImg[0][2],kTestImg[0][3]), AsByte(kTestImg[0][4],kTestImg[0][5]), AsByte(kTestImg[0][6],kTestImg[0][7]) },
    { AsByte(kTestImg[1][0],kTestImg[1][1]), AsByte(kTestImg[1][2],kTestImg[1][3]), AsByte(kTestImg[1][4],kTestImg[1][5]), AsByte(kTestImg[1][6],kTestImg[1][7]) },
    { AsByte(kTestImg[2][0],kTestImg[2][1]), AsByte(kTestImg[2][2],kTestImg[2][3]), AsByte(kTestImg[2][4],kTestImg[2][5]), AsByte(kTestImg[2][6],kTestImg[2][7]) },
    { AsByte(kTestImg[3][0],kTestImg[3][1]), AsByte(kTestImg[3][2],kTestImg[3][3]), AsByte(kTestImg[3][4],kTestImg[3][5]), AsByte(kTestImg[3][6],kTestImg[3][7]) },
};

// Pack kTestImage bytes into a RLE compressed buffer
const BYTE kTestImageCompressed[] =
{
    8, 0,               // u16 width
    4, 0,               // u16 height
    AsByte(0, 8),       // (black pixel count, direct copy count) both are limited to max 16,16 since 4 bits each
    kTestImage[0][0],
    kTestImage[0][1],
    kTestImage[0][2],
    kTestImage[0][3],

    AsByte(0, 8),
    kTestImage[1][0],
    kTestImage[1][1],
    kTestImage[1][2],
    kTestImage[1][3],

    AsByte(0, 8),
    kTestImage[2][0],
    kTestImage[2][1],
    kTestImage[2][2],
    kTestImage[2][3],

    AsByte(0, 8),
    kTestImage[3][0],
    kTestImage[3][1],
    kTestImage[3][2],
    kTestImage[3][3],
};

class AnimRenderTest : public BaseGameObject
{
public:
  
    AnimRenderTest()
    {
        DisableVTableHack disableHack;

        BaseGameObject_ctor_4DBFA0(1, 1);

        field_6_flags.Set(BaseGameObject::eDrawable);
        field_6_flags.Set(BaseGameObject::eUpdatable);

        gObjList_drawables_5C1124->Push_Back(this);

        Init();
    }

    virtual void VRender(int** ot) override
    {
        mAnim[0].vRender_40B820(40 + (1 * 85), 40 + (1 * 90), ot, 0, 0);
        mAnim[1].vRender_40B820(40 + (2 * 85), 40 + (1 * 90), ot, 0, 0);
        mAnim[2].vRender_40B820(40 + (1 * 85), 40 + (2 * 90), ot, 0, 0);
        mAnim[3].vRender_40B820(40 + (2 * 85), 40 + (2 * 90), ot, 0, 0);
        mAnim[4].vRender_40B820(180 + 90, 170 + 45, ot, 0, 0);

        OrderingTable_Add_4F8AA0(&ot[30], &mPolyFT4[0].mBase.header);

        pScreenManager_5BB5F4->InvalidateRect_40EC10(0, 0, 640, 240);
    }

    virtual void VUpdate() override
    {

    }

    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        Destruct();
        if (flags & 1)
        {
            Mem_Free_495540(this);
        }
        return this;
    }

    void Destruct()
    {
        gObjList_drawables_5C1124->Remove_Item(this);
    }
private:
    void Init()
    {
        ResourceManager::LoadResourceFile_49C170("LOADING.BAN", nullptr);
        ResourceManager::LoadResourceFile_49C170("ABEBLOW.BAN", nullptr);

        for (int i = 0; i < 5; i++)
        {

            SetVTable(&mAnim[i], 0x544290);

            if (i < 2)
            {
                // 4 bit
                mAnimRes[i] = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, kLoadingResID, TRUE, FALSE);
                mAnim[i].Init_40A030(900, gObjList_animations_5C1A24, this, 150, 0x41u, mAnimRes[i], 1, 0, 0);
            }
            else
            {
                // 8 bit
                mAnimRes[i] = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeblowResID, TRUE, FALSE);
                mAnim[i].Init_40A030(7812, gObjList_animations_5C1A24, this, 50, 25, mAnimRes[i], 1, 0, 0);
            }
            // No 16 bit test case because there are simply no 16bit sprites at all in the game data
           
            mAnim[i].field_4_flags.Clear(AnimFlags::eBit16_bBlending);
            mAnim[i].field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);

            mAnim[i].field_14_scale = FP_FromDouble(2.0);

            mAnim[i].field_C_render_layer = 38;
            mAnim[i].field_B_render_mode = 1;

            mAnim[i].field_A_b = 127;
            mAnim[i].field_9_g = 127;
            mAnim[i].field_8_r = 127;
        }

        // 4 bit o 
        mAnim[0].field_4_flags.Clear(AnimFlags::eBit16_bBlending);
        mAnim[0].field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
        mAnim[0].field_4_flags.Set(AnimFlags::eBit5_FlipX);
        mAnim[0].field_14_scale = FP_FromDouble(1.0);

        // 4 bit s
        mAnim[1].field_4_flags.Clear(AnimFlags::eBit16_bBlending);
        mAnim[1].field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
        mAnim[1].field_14_scale = FP_FromDouble(2.0);

        // 8 bit o
        mAnim[2].field_4_flags.Clear(AnimFlags::eBit16_bBlending);
        mAnim[2].field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
        mAnim[2].field_14_scale = FP_FromDouble(1.0);

        // 8 bit s
        mAnim[3].field_4_flags.Clear(AnimFlags::eBit16_bBlending);
        mAnim[3].field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

        PSX_RECT pr = {};
        Pal_Allocate_483110(&pr, 16);
        pr.w = 16;
        pr.h = 1;
        PSX_LoadImage16_4F5E20(&pr, (BYTE*)&kTestImagePal[0]);

        for (short i = 0; i < 1; i++)
        {
            PolyFT4_Init_4F8870(&mPolyFT4[i]);

            SetRGB0(&mPolyFT4[i], 127, 127, 127);
            SetTPage(&mPolyFT4[i], 0);
            SetClut(&mPolyFT4[i], static_cast<short>(PSX_getClut_4F6350(pr.x, pr.y)));

            const short xpos = 20 + (150 * i);
            const short ypos = 10;
            const short w = 255; // All width doubled due to PC doubling the render width
            const short h = 80;

            SetXY0(&mPolyFT4[i], xpos, ypos);
            SetXY1(&mPolyFT4[i], xpos, ypos + h);
            SetXY2(&mPolyFT4[i], xpos , ypos);
            SetXY3(&mPolyFT4[i], xpos + w, ypos + h);

            // This assumes the texture data is at 0,0 in the active texture page
            SetUV0(&mPolyFT4[i], 6, 0);
            SetUV1(&mPolyFT4[i], 2, 0);
            SetUV2(&mPolyFT4[i], 2, 0);
            SetUV3(&mPolyFT4[i], 2, 4);

            const void* ptr = &kTestImageCompressed[0];
            mPolyFT4[i].mVerts[1].mUv.tpage_clut_pad = (WORD)(unsigned int)ptr >> 0;
            mPolyFT4[i].mVerts[2].mUv.tpage_clut_pad = (unsigned int)ptr >> 16;
        }
    }

    BYTE** mAnimRes[5];
    AnimationEx mAnim[5];
    Poly_FT4 mPolyFT4[1];
};

void DebugHelpers_Init() 
{
#if DEVELOPER_MODE
    if (GetKeyState(VK_LCONTROL) < 0)
    {
        return;
    }

    //if (IsAlive())
    {
        alive_new<ObjectDebugger>();
        alive_new<DebugConsole>();
        alive_new<DebugPathRenderer>();

        DEV_CONSOLE_MESSAGE_C("Debug Console Active. Open with ~ (Tilde)", 7, 0, 150, 255);
    }
#endif

//#if RENDER_TEST
    // Test rendering diff prim types
   // alive_new<RenderTest>(); // Will get nuked at LVL/Path change
   //alive_new<AnimRenderTest>();
//#endif
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

BaseGameObject* FindObjectOfType(BaseGameObject::Types id)
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

void DEV::DebugFillRect(int ** ot, int layer, int x, int y, int width, int height, BYTE r, BYTE g, BYTE b, bool worldspace, bool semiTransparent)
{
    Poly_F4 * mPolyF4 = &sPolyF4PrimBuffer[++sNextPolyF4Prim];
    *mPolyF4 = {};
    PolyF4_Init_4F8830(mPolyF4);

    const auto camOffset = gMap_5C3030.field_24_camera_offset;

    if (worldspace)
    {
        x -= FP_GetExponent(camOffset.field_0_x);
        y -= FP_GetExponent(camOffset.field_4_y);

        x = static_cast<int>(x / 0.575);
        y = static_cast<int>(y / 0.575);
        width = static_cast<int>(width / 0.575);
        height = static_cast<int>(height / 0.575);
    }

    SetRGB0(mPolyF4, r, g, b);

    SetXY0(mPolyF4, static_cast<short>(x), static_cast<short>(y));
    SetXY1(mPolyF4, static_cast<short>(x), static_cast<short>(y + height));
    SetXY2(mPolyF4, static_cast<short>(x + width), static_cast<short>(y));
    SetXY3(mPolyF4, static_cast<short>(x + width), static_cast<short>(y + height));

    Poly_Set_SemiTrans_4F8A60(&mPolyF4->mBase.header, semiTransparent);

    OrderingTable_Add_4F8AA0(&ot[layer], &mPolyF4->mBase.header);
    pScreenManager_5BB5F4->InvalidateRect_40EC10(0, 0, 640, 240);
}

void DEV::DebugDrawRect(int ** ot, int layer, int x, int y, int width, int height, BYTE r, BYTE g, BYTE b, bool worldspace, bool semiTransparent)
{
    DebugDrawLine(ot, layer, x, y, x + width, y, r, g, b, worldspace, semiTransparent);
    DebugDrawLine(ot, layer, x + width, y, x + width, y + height, r, g, b, worldspace, semiTransparent);
    DebugDrawLine(ot, layer, x + width, y + height, x, y + height, r, g, b, worldspace, semiTransparent);
    DebugDrawLine(ot, layer, x, y + height, x, y, r, g, b, worldspace, semiTransparent);
}

void DEV::DebugDrawLine(int ** ot, int layer, int x1, int y1, int x2, int y2, BYTE r, BYTE g, BYTE b, bool worldspace, bool semiTransparent)
{
    Line_G2 * mLineG2 = &sLinePrimBuffer[++sNextLinePrim];
    LineG2_Init(mLineG2);

    const auto camOffset = gMap_5C3030.field_24_camera_offset;

    if (worldspace)
    {
        x1 -= FP_GetExponent(camOffset.field_0_x);
        y1 -= FP_GetExponent(camOffset.field_4_y);

        x2 -= FP_GetExponent(camOffset.field_0_x);
        y2 -= FP_GetExponent(camOffset.field_4_y);

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

void DEV::DebugDrawText(int ** ot, int layer, std::string & text, int x, int y, BYTE r, BYTE g, BYTE b, bool worldspace, bool semiTransparent)
{
    const auto camOffset = gMap_5C3030.field_24_camera_offset;

    if (worldspace)
    {
        x -= FP_GetExponent(camOffset.field_0_x);
        y -= FP_GetExponent(camOffset.field_4_y);
    }

    g_DebugGlobalFontPolyIndex = g_DebugGlobalFont.DrawString_4337D0(ot, text.c_str(), x - (g_DebugGlobalFont.MeasureWidth_433700(text.c_str()) / 2), static_cast<short>(y), semiTransparent, 0, 0, layer, r, g, b, g_DebugGlobalFontPolyIndex, FP_FromDouble(1.0), 640, 0);
    g_DebugGlobalFontPolyIndex = g_DebugGlobalFont.DrawString_4337D0(ot, text.c_str(), x - (g_DebugGlobalFont.MeasureWidth_433700(text.c_str()) / 2) + 1, static_cast<short>(y + 1), semiTransparent, 0, 0, layer - 1, 0, 0, 0, g_DebugGlobalFontPolyIndex, FP_FromDouble(1.0), 640, 0);
}

void DEV::DebugOnFrameDraw(int ** pOt)
{
    sNextLinePrim = 0;
    sNextPolyF4Prim = 0;
    g_DebugGlobalFontPolyIndex = 0;

    if (g_EnabledRaycastRendering)
    {
        for (auto rc : g_RaycastDebugList)
        {
            if (rc.pLine != nullptr)
            {
                const int hitX = FP_GetExponent(rc.hitX);
                const int hitY = FP_GetExponent(rc.hitY);

                DEV::DebugDrawLine(pOt, 38, FP_GetExponent(rc.x1), FP_GetExponent(rc.y1), hitX, hitY, 255, 255, 0, true, true);


                DEV::DebugDrawLine(pOt, 38, hitX - 1, hitY - 1, hitX + 1, hitY + 1, 255, 255, 255, true, true);
                DEV::DebugDrawLine(pOt, 38, hitX + 1, hitY - 1, hitX - 1, hitY + 1, 255, 255, 255, true, true);
            }
            else
            {
                DEV::DebugDrawLine(pOt, 38, FP_GetExponent(rc.x1), FP_GetExponent(rc.y1), FP_GetExponent(rc.x2), FP_GetExponent(rc.y2), 0, 255, 0, true, true);
            }
        }
    }

    g_RaycastDebugList.clear();
}

bool IsStringNumber(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}