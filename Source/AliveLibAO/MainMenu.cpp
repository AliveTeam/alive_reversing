#include "stdafx_ao.h"
#include "Function.hpp"
#include "MainMenu.hpp"
#include "ResourceManager.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "../relive_lib/Sfx.hpp"
#include "DDCheat.hpp"
#include "CheatController.hpp"
#include "Abe.hpp"
#include "Math.hpp"
#include "CameraSwapper.hpp"
#include "PauseMenu.hpp"
#include "DemoPlayback.hpp"
#include "Grenade.hpp"
#include "Movie.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "CreditsController.hpp"
#include "SaveGame.hpp"
#include "Midi.hpp"
#include "Sound.hpp"
#include "../relive_lib/Particle.hpp"
#include "../relive_lib/Camera.hpp"
#include "../AliveLibAE/Io.hpp"

namespace AO {

static const AnimId sButtonAnimIds[4] = {
    AnimId::MenuHighlight_Circle,
    AnimId::MenuHighlight_Square,
    AnimId::MenuHighlight_Triangle,
    AnimId::None};

// TODO: Move out
static s16 sSoundMono = 0;

// TODO: Move out
void SND_Set_Mono()
{
    //nullsub_63();
    sSoundMono = 1;
}

// TODO: Move out
void SND_Set_Stereo()
{
    //nullsub_64();
    sSoundMono = 0;
}

s16 bWaitingForRemapInput_9F2DE8 = 0;

struct Buttons final
{
    s32 buttons[2][8] = {
        {// keyboard
         InputCommands::eRun,
         InputCommands::eSneak,
         InputCommands::eHop,
         InputCommands::eLeftGamespeak,
         InputCommands::eDoAction,
         InputCommands::eThrowItem,
         InputCommands::eCrouchOrRoll,
         InputCommands::eRightGameSpeak},
        {
            // gamepad
            // NOTE: diversion from OG!
            // the sneak - speak1 and run - speak2 pairs are now decoupled
            // so that they can each be remapped to separate buttons
            InputCommands::eRun,
            InputCommands::eSneak,
            InputCommands::eHop,
            InputCommands::eLeftGamespeak,
            InputCommands::eDoAction,
            InputCommands::eThrowItem,
            InputCommands::eCrouchOrRoll,
            InputCommands::eRightGameSpeak,
        }};
};
static const Buttons sRemappableInputs = {};

struct MenuButtonAnimInfo final
{
    s16 xpos;
    s16 ypos;
    AnimId animId;
};

static const MenuButtonAnimInfo sMainScreenButtons[5] = {
    {33, 64, AnimId::MenuHighlight_Circle},
    {33, 85, AnimId::MenuHighlight_Circle},
    {33, 107, AnimId::MenuHighlight_Circle},
    {335, 215, AnimId::MenuHighlight_Circle},
    {335, 240, AnimId::MenuHighlight_Circle}};

static const MenuButtonAnimInfo sOptionsButtons[3] = {
    {33, 66, AnimId::MenuHighlight_Circle},
    {33, 87, AnimId::MenuHighlight_Circle},
    {288, 238, AnimId::MenuHighlight_Circle}};

static const MenuButtonAnimInfo sSoundOptionsButtons[3] = {
    {33, 66, AnimId::MenuHighlight_Circle},
    {33, 87, AnimId::MenuHighlight_Circle},
    {289, 238, AnimId::MenuHighlight_Circle}};

static const MenuButtonAnimInfo sAbeMotionsButtons[3] = {
    {116, 251, AnimId::MenuHighlight_Circle},
    {116, 251, AnimId::MenuHighlight_Circle},
    {308, 240, AnimId::MenuHighlight_Circle}};

static const MenuButtonAnimInfo sLoadButtons[2] = {
    {62, 204, AnimId::MenuHighlight_Circle},
    {293, 204, AnimId::MenuHighlight_Circle}};

static const MenuButtonAnimInfo sGameSpeakButtons[13] = {
    {50, 151, AnimId::MenuHighlight_Circle},
    {38, 81, AnimId::MenuHighlight_Circle},
    {65, 190, AnimId::MenuHighlight_Circle},
    {43, 118, AnimId::MenuHighlight_Circle},
    {322, 81, AnimId::MenuHighlight_Circle},
    {302, 146, AnimId::MenuHighlight_Circle},
    {279, 185, AnimId::MenuHighlight_Circle},
    {307, 116, AnimId::MenuHighlight_Circle},
    {157, 29, AnimId::MenuHighlight_Circle},
    {308, 205, AnimId::MenuHighlight_Circle},
    {96, 32, AnimId::MenuHighlight_Circle},
    {205, 29, AnimId::MenuHighlight_Circle},
    {305, 32, AnimId::MenuHighlight_Circle}};

struct MenuButtonInputInfo final
{
    s32 xpos;
    s32 ypos;
    s32 inputCommand;
};

static const MenuButtonInputInfo sMainScreenInputs[1] = {
    {146, 205, InputCommands::eUnPause_OrConfirm}
};

static const MenuButtonInputInfo sOptionsInputs[2] = {
    {145, 204, InputCommands::eUnPause_OrConfirm},
    {286, 202, InputCommands::eBack}
};

static const MenuButtonInputInfo sAbeMotionsInputs[11] = {
    {144, 205, InputCommands::eUnPause_OrConfirm},
    {288, 203, InputCommands::eBack},
    {112, 87, InputCommands::eCrouchOrRoll},       // Crouch
    {283, 87, InputCommands::eRun},                // Run
    {89, 118, InputCommands::eRun},                // Run jump: run
    {124, 118, InputCommands::eHop},               // Run jump: jump
    {293, 119, InputCommands::eSneak},             // Sneak
    {264, 151, InputCommands::eThrowItem},         // Throw
    {273, 183, InputCommands::eDoAction},          // Action
    {115, 216, InputCommands::eUnPause_OrConfirm}, // Go to Gamespeak page
    {307, 203, InputCommands::eBack},              // Exit
};

static const MenuButtonInputInfo sGameSpeakInputs[33] = {
    {115, 216, InputCommands::eUnPause_OrConfirm}, // Back to Abe Motions
    {307, 203, InputCommands::eBack},              // Exit
    {90, 53, InputCommands::eLeftGamespeak},       // Chant first button
    {125, 53, InputCommands::eRightGameSpeak},     // Chant second button
    {81, 80, InputCommands::eLeftGamespeak},       // Hello shoulder button
    {125, 80, InputCommands::eHop},                // Hello action button
    {79, 112, InputCommands::eLeftGamespeak},      // Angry shoulder button
    {122, 112, InputCommands::eThrowItem},         // Angry action button
    {78, 147, InputCommands::eLeftGamespeak},      // Wait shoulder button
    {119, 147, InputCommands::eCrouchOrRoll},      // Wait action button
    {82, 180, InputCommands::eLeftGamespeak},      // Follow me shoulder button
    {120, 180, InputCommands::eDoAction},          // Follow me action button
    {258, 62, InputCommands::eRightGameSpeak},     // Whistle 1 shoulder button
    {295, 62, InputCommands::eHop},                // Whistle 1 action button
    {253, 98, InputCommands::eRightGameSpeak},     // Fart shoulder button
    {294, 98, InputCommands::eThrowItem},          // Fart action button
    {267, 139, InputCommands::eRightGameSpeak},    // Whistle 2 shoulder button
    {307, 139, InputCommands::eCrouchOrRoll},      // Whistle 2 action button
    {259, 178, InputCommands::eRightGameSpeak},    // Laugh shoulder button
    {296, 178, InputCommands::eDoAction},          // Laugh action button
    // ----
    // these below seem unused - maybe they are part of a different MenuButtonInputInfo struct?
    {308, 205, InputCommands::eBack}, // end
    {96, 32, InputCommands::eLeftGamespeak},
    {305, 32, InputCommands::eRightGameSpeak},
    {157, 29, InputCommands::eLeftGamespeak},
    {205, 29, InputCommands::eRightGameSpeak},
    {38, 81, InputCommands::eHop},
    {42, 118, InputCommands::eThrowItem},
    {49, 151, InputCommands::eUnPause_OrConfirm},
    {65, 190, InputCommands::eDoAction},
    {322, 82, InputCommands::eHop},
    {307, 117, InputCommands::eThrowItem},
    {301, 146, InputCommands::eUnPause_OrConfirm},
    {278, 184, InputCommands::eDoAction}, // 1st
};

static const MenuButtonInputInfo sLoadInputs[2] = {
    {62, 204, InputCommands::eUnPause_OrConfirm},
    {293, 205, InputCommands::eBack}};

struct SaveName final
{
    char_type field_0_mName[32];
};

SaveName sSaveNames_9F1DD8[128] = {}; // Got more than 128 saves? Hard luck mate

static s16 sDemoPlay = 0;
u8 gJoyResId = 0;

s32 gMainMenuInstanceCount = 0;
static s16 sListCount = -1;

// The total number of valid controllers - includes the keyboard as well
static s32 sAvailableControllers = 0;
s32 gJoystickAvailable = 0;

static s32 sSelectedSaveIdx = 0;
static s32 sSaveIdx = 0;


struct MenuFMV;

static const MenuFMV* sActiveList = nullptr;

struct MenuFMV final
{
    const char_type* mMenuEntryName;
    EReliveLevelIds mLevel;
    s16 mPath;
    s16 mCamera;
    s16 mFmvId;
    s16 mAbeSpawnX;
    s16 mAbeSpawnY;
};

static const MenuFMV sFmvList[13] = {
    {"Oddworld Intro", EReliveLevelIds::eMenu, -1, -1, 1, -1, -1},
    {"This is RuptureFarms", EReliveLevelIds::eRuptureFarms, -1, -1, 1, -1, -1},
    {"Barrel Ride", EReliveLevelIds::eStockYards, -1, -1, 4, -1, -1},
    {"Abe's Moon", EReliveLevelIds::eStockYards, -1, -1, 2, -1, -1},
    {"Paramite Scar", EReliveLevelIds::eLines, -1, -1, 3, -1, -1},
    {"Scrab Scar", EReliveLevelIds::eLines, -1, -1, 4, -1, -1},
    {"Shrykull Revealed", EReliveLevelIds::eLines, -1, -1, 16, -1, -1},
    {"Mullock Watches", EReliveLevelIds::eRuptureFarmsReturn, -1, -1, 2, -1, -1},
    {"The Factory Halts", EReliveLevelIds::eRuptureFarmsReturn, -1, -1, 1, -1, -1},
    {"What a Drag", EReliveLevelIds::eRuptureFarmsReturn, -1, -1, 3, -1, -1},
    {"Bad Ending", EReliveLevelIds::eRuptureFarmsReturn, -1, -1, 4, -1, -1},
    {"Good Ending", EReliveLevelIds::eRuptureFarmsReturn, -1, -1, 16, -1, -1},
    {"Credits", EReliveLevelIds::eCredits, -1, -1, -1, -1, -1}};

struct MenuLevel final
{
    const char_type* field_0_name;
    EReliveLevelIds field_4_level_id;
    s16 field_6;
    s16 field_8;
    s16 field_A;
    s16 field_C;
    s16 field_E;
};

// TODO: Should be MenuFMV or there is a generic structure for "lists" of things?
static const MenuFMV sLevelList[15] = {
    {"RuptureFarms", EReliveLevelIds::eRuptureFarms, 15, 1, -1, 1390, 245},
    {"Stockyard Escape", EReliveLevelIds::eStockYards, 6, 6, -1, 5656, 133},
    {"Monsaic Lines", EReliveLevelIds::eLines, 1, 14, -1, 11810, 681},
    {"Paramonia", EReliveLevelIds::eForest, 1, 1, -1, 330, 660},
    {"Paramonian Temple", EReliveLevelIds::eForestTemple, 1, 1, -1, 565, 170},
    {"Paramonian Nests", EReliveLevelIds::eForestChase, 9, 1, -1, 2439, 2621},
    {"Scrabania", EReliveLevelIds::eDesert, 1, 1, -1, 4677, 750},
    {"Scrabanian Temple", EReliveLevelIds::eDesertTemple, 1, 1, -1, 4410, 203},
    {"Scrabanian Nests", EReliveLevelIds::eDesertEscape, 11, 1, -1, 466, 2124},
    {"Stockyards", EReliveLevelIds::eStockYardsReturn, 4, 7, -1, 540, 640},
    {"Rescue Zulag 1", EReliveLevelIds::eRuptureFarmsReturn, 19, 3, -1, 2589, 193},
    {"Rescue Zulag 2", EReliveLevelIds::eRuptureFarmsReturn, 1, 1, -1, 1514, 705},
    {"Rescue Zulag 3", EReliveLevelIds::eRuptureFarmsReturn, 13, 1, -1, 1389, 694},
    {"Rescue Zulag 4", EReliveLevelIds::eRuptureFarmsReturn, 14, 1, -1, 1390, 700},
    {"The Boardroom", EReliveLevelIds::eBoardRoom, 6, 1, -1, 592, 157}};

void MainMenuFade::LoadAnimations()
{
    for (auto& animId : sButtonAnimIds)
    {
        if (animId != AnimId::None)
        {
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
        }
    }
}

MainMenuFade::MainMenuFade(s32 xpos, s32 ypos, buttonType buttonType, s32 bDestroyOnDone)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::MainMenuFade);

    mRGB.SetRGB(128, 128, 128);

    LoadAnimations();
    Animation_Init(GetAnimRes(sButtonAnimIds[buttonType]));

    GetAnimation().SetRenderMode(TPageAbr::eBlend_1);
    mXPos = FP_FromInteger(xpos);
    mYPos = FP_FromInteger(ypos);
    field_E8_bDestroyOnDone = static_cast<s16>(bDestroyOnDone);
    mRgbValue = 40;
    mRgbModifier = 8;

    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }
        if (pObj->Type() == ReliveTypes::MainMenuFade && pObj != this && static_cast<BaseAnimatedWithPhysicsGameObject*>(pObj)->mXPos == mXPos && static_cast<BaseAnimatedWithPhysicsGameObject*>(pObj)->mYPos == mYPos)
        {
            pObj->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }
}

void MainMenuFade::VUpdate()
{
    mRgbValue += mRgbModifier;

    if (mRgbValue < 40)
    {
        mRgbValue += -mRgbModifier;
        mRgbModifier = -mRgbModifier;

        if (field_E8_bDestroyOnDone)
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }

    if (mRgbValue > 127)
    {
        mRgbValue -= mRgbModifier;
        mRgbModifier = -mRgbModifier;
    }

    GetAnimation().SetRGB(mRgbValue, mRgbValue, mRgbValue);
}

void MainMenuFade::VRender(PrimHeader** ppOt)
{
    GetAnimation().VRender(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        ppOt,
        0,
        0);

    PSX_RECT rect = {};
    GetAnimation().Get_Frame_Rect(&rect);
}

struct MainMenu_TransitionData final
{
    u16 field_0;
    u16 field_2;
    u16 field_4;
    s16 field_6;
};
ALIVE_ASSERT_SIZEOF(MainMenu_TransitionData, 0x8);

const MainMenu_TransitionData stru_55C038[24] = // 3 x 8's ?
    {
        {65528U, 65528U, 384, 1},
        {0, 65504U, 256, 1},
        {8, 65528U, 384, 1},
        {32, 0, 256, 1},
        {8, 8, 384, 1},
        {0, 32, 256, 1},
        {65528U, 8, 384, 1},
        {65504U, 0, 256, 1},

        {65520U, 65520U, 256, 1},
        {0, 65513U, 256, 1},
        {16, 65520U, 256, 1},
        {23, 0, 256, 1},
        {16, 16, 256, 1},
        {0, 23, 256, 1},
        {65520U, 16, 256, 1},

        {65513U, 0, 256, 1},
        {65520U, 65520U, 256, 1},
        {0, 65520U, 256, 1},
        {16, 65520U, 256, 1},
        {16, 0, 256, 1},
        {16, 16, 256, 1},
        {0, 16, 256, 1},
        {65520U, 16, 256, 1},
        {65520U, 0, 256, 1},
};

void MainMenuTransition::VScreenChanged()
{
    if (gMap.LevelChanged() || gMap.PathChanged())
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void MainMenuTransition::VUpdate()
{
    if (!field_16_bDone)
    {
        field_10_current_Value += field_12_change_by_speed;
        if (field_14_fade_direction)
        {
            if (field_10_current_Value > 255)
            {
                field_10_current_Value = 255;
                field_246_colour_fade_value--;
                return;
            }
        }
        else if (field_10_current_Value < 0)
        {
            field_10_current_Value = 0;
        }
        field_246_colour_fade_value--;
    }
}

MainMenuTransition::MainMenuTransition(Layer layer, s32 fadeDirection, s32 bKillWhenDone, s32 speed, TPageAbr abr)
    : BaseGameObject(true, 0)
{
    SetType(ReliveTypes::eFade);

    gObjListDrawables->Push_Back(this);

    mBaseGameObjectFlags.Set(Options::eDrawable_Bit4);

    for (s32 i = 0; i < 2; i++)
    {
        Init_SetTPage(&field_21C_tPage[i], 0, 1, PSX_getTPage(abr));
    }

    for (s32 i = 0; i < 8; i++)
    {
        PolyG3_Init(&field_1C_polys[0].field_0_polys[i]);
        Poly_Set_SemiTrans(&field_1C_polys[0].field_0_polys[i].mBase.header, 1);

        PolyG3_Init(&field_1C_polys[1].field_0_polys[i]);
        Poly_Set_SemiTrans(&field_1C_polys[1].field_0_polys[i].mBase.header, 1);
    }

    field_23C_layer = layer;

    if (fadeDirection)
    {
        field_10_current_Value = 0;
    }
    else
    {
        field_10_current_Value = 255;
    }

    field_242_idx = 0;
    field_246_colour_fade_value = 0;
    field_23E_width = 320;
    field_240_k120 = 120;
    StartTrans_436560(layer, static_cast<s16>(fadeDirection), static_cast<s16>(bKillWhenDone), static_cast<s16>(speed));
}

MainMenuTransition::~MainMenuTransition()
{
    gObjListDrawables->Remove_Item(this);
}

void MainMenuTransition::StartTrans_436560(Layer layer, s16 fadeDirection, s16 bKillWhenDone, s16 speed)
{
    field_23C_layer = layer;
    field_14_fade_direction = fadeDirection;
    field_16_bDone = 0;

    field_18_bKillOnDone = bKillWhenDone;

    if (!fadeDirection)
    {
        field_12_change_by_speed = -2 * speed;
    }
    else
    {
        field_12_change_by_speed = 2 * speed;
    }

    if (fadeDirection)
    {
        SfxPlayMono(relive::SoundEffects::MenuTransition, 0);
    }
}

void MainMenuTransition::VRender(PrimHeader** ppOt)
{
    // TODO: The fixed point math/var needs cleaning up/refactoring in here
    s32 currentValue = field_10_current_Value;
    s32 v4 = (currentValue + 1) >> 4;
    s32 v5 = v4 * v4 * v4 * v4 >> 8;

    s32 bValue = v4 * v4 * v4 * v4 >> 8;
    if (v5 > 255)
    {
        bValue = -1; // LOBYTE
        v5 = 255;
    }

    s32 rgValue = v5 * v5 >> 8;
    if (rgValue > 255)
    {
        rgValue = -1; // LOBYTE
    }

    s32 op1 = currentValue << 12;
    s32 val1 = Math_Cosine(field_246_colour_fade_value).fpValue;
    s32 val2 = Math_Sine(field_246_colour_fade_value).fpValue;
    s32 r0g0 = -64 / ((v5 >> 2) + 1);
    for (s32 i = 0; i < 8; i++)
    {
        s32 idx = i + (8 * field_242_idx);
        s32 v8 = stru_55C038[idx].field_4 << 8;
        s32 v9 = stru_55C038[idx].field_2 << 16;
        s32 v10 = (s16) stru_55C038[idx].field_0 << 16;
        s32 v11 = Math_FixedPoint_Multiply(v9, val1);
        s32 v12 = Math_FixedPoint_Multiply(v10, val2) - v11;
        s32 v13 = Math_FixedPoint_Multiply(op1, v8);
        s16 x0 = field_23E_width + 640 * ((s32) Math_FixedPoint_Multiply(v12, v13) >> 16) / 368;
        s32 v14 = Math_FixedPoint_Multiply(v9, val2);
        s32 v15 = Math_FixedPoint_Multiply(v10, val1) + v14;
        s32 v16 = Math_FixedPoint_Multiply(op1, v8);
        s16 y0 = field_240_k120 + (Math_FixedPoint_Multiply(v15, v16) >> 16);

        s32 v17 = 0;
        if (i < 7)
        {
            v17 = i + 1;
        }
        else
        {
            v17 = 0;
        }

        s32 idx2 = (8 * field_242_idx);
        s32 v36 = (s16) stru_55C038[idx2 + v17].field_0 << 16;
        s32 v19 = 0;
        if (i < 7)
        {
            v19 = i + 1;
        }
        else
        {
            v19 = 0;
        }

        s32 v20 = stru_55C038[idx2 + v19].field_2 << 16;
        s32 v38 = v20;
        s32 v21 = 0;
        if (i < 7)
        {
            v21 = i + 1;
        }
        else
        {
            v21 = 0;
        }
        s32 y1 = stru_55C038[v21 + idx2].field_4 << 8;

        s32 v23 = Math_FixedPoint_Multiply(v20, val1);
        s32 x1 = Math_FixedPoint_Multiply(v36, val2) - v23;
        s32 v25 = Math_FixedPoint_Multiply(op1, y1);
        // TODO: Use PsxToPCX
        x1 = field_23E_width + 40 * ((s32) Math_FixedPoint_Multiply(x1, v25) >> 16) / 23; // LOWORD
        s32 v26 = Math_FixedPoint_Multiply(v38, val2);
        s32 v27 = v26 + Math_FixedPoint_Multiply(v36, val1);
        s32 v28 = Math_FixedPoint_Multiply(op1, y1);
        y1 = field_240_k120 + (Math_FixedPoint_Multiply(v27, v28) >> 16); // LOWORD
        Poly_G3* pPoly = &field_1C_polys[gPsxDisplay.mBufferIndex].field_0_polys[i];

        SetRGB0(pPoly, static_cast<u8>(r0g0), static_cast<u8>(r0g0), 255);
        SetRGB1(pPoly, static_cast<u8>(rgValue), static_cast<u8>(rgValue), static_cast<u8>(bValue));
        SetRGB2(pPoly, static_cast<u8>(rgValue), static_cast<u8>(rgValue), static_cast<u8>(bValue));

        SetXY0(pPoly, field_23E_width, field_240_k120);
        SetXY1(pPoly, x0, y0);
        SetXY2(pPoly, static_cast<s16>(x1), static_cast<s16>(y1));

        OrderingTable_Add(OtLayer(ppOt, field_23C_layer), &pPoly->mBase.header);
    }

    OrderingTable_Add(OtLayer(ppOt, field_23C_layer), &field_21C_tPage[gPsxDisplay.mBufferIndex].mBase);

    if ((field_10_current_Value == 255 && field_14_fade_direction) || (field_10_current_Value == 0 && !field_14_fade_direction))
    {
        field_16_bDone = 1;
        if (field_18_bKillOnDone)
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }
}

void Menu::LoadAnimations()
{
    static AnimId animIds[] =
    {
        AnimId::MenuHighlight_ButtonRemapSquare,
        AnimId::MenuHighlight_Circle,
        AnimId::MenuHighlight_Square,
        AnimId::MenuHighlight_Triangle,
        AnimId::MenuDoor,
        AnimId::AbeIntro,
        AnimId::MenuAbeSpeak_Laugh,
        AnimId::MenuAbeSpeak_WhistleHigh,
        AnimId::MenuAbeSpeak_WhistleLow,
        AnimId::MenuAbeSpeak_Fart,
        AnimId::MenuAbeSpeak_Hello,
        AnimId::MenuAbeSpeak_Idle,
        AnimId::MenuAbeSpeak_IdleBlink,
        AnimId::MenuAbeSpeak_Ok,
        AnimId::MenuAbeSpeak_FollowMe,
        AnimId::MenuAbeSpeak_Wait,
        AnimId::MenuAbeSpeak_Anger,
        AnimId::MenuAbeSpeak_Chant,
        AnimId::MenuAbeSpeak_ChantEnd,
        AnimId::MenuAbeSpeak_Goodbye,
        AnimId::OptionChantOrb_Particle
    };

    for (auto& animId : animIds)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
    }
}

Menu::Menu(relive::Path_TLV* /*pTlv*/, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    LoadAnimations();
    mLoadedPals.push_back(ResourceManagerWrapper::LoadPal(PalId::WhiteHighlite));

    gMainMenuInstanceCount++;

    mFontContext.LoadFontType(FontType::PauseMenu);
    mPal = ResourceManagerWrapper::LoadPal(PalId::MainMenuFont_MainMenu);
    mFont.Load(240, mPal, &mFontContext);
    
    // 30 = fmv select
    if (gMap.mCurrentCamera == 30)
    {
        Animation_Init(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
    }
    else
    {
        Animation_Init(GetAnimRes(AnimId::MenuDoor));
    }

    GetAnimation().SetRGB(127, 127, 127);

    mButtonAnim.Init(GetAnimRes(AnimId::MenuHighlight_Circle), this);

    mButtonAnim.mFlags.Clear(AnimFlags::eBlending);
    mButtonAnim.mFlags.Set(AnimFlags::eSemiTrans);

    mButtonAnim.SetRenderLayer(Layer::eLayer_MainMenuButtonBees_38);
    mButtonAnim.SetSpriteScale(GetSpriteScale());
    mButtonAnim.SetRenderMode(TPageAbr::eBlend_1);
    mButtonAnim.LoadPal(GetPalRes(PalId::WhiteHighlite));
    field_204_flags &= ~6u;
    mButtonRgb = 40;
    mButtonRgbModifier = -8;
    field_1D4_tlvInfo = tlvId;
    field_1D8_timer = sGnFrame + 150;

    // copy right boot screen
    mFnUpdate = &Menu::CopyRight_Update;


    // 1 == abe hello screen
    if (gMap.mCurrentCamera == 1)
    {
        mFnUpdate = &Menu::WaitForDoorToOpen;
        field_204_flags |= 2;
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuDoor));
    }

    mFnRender = &Menu::Empty_Render;
    mSelectedButtonIndex.mainmenu = MainMenuOptions::eBegin_1;
    mMenuTrans = 0;
    mMenuFade1 = 0;
    mMenuFade2 = 0;
    field_1F4_text = "";
    mLoadSave = false;
    mToFmvSelect = false;
    mToLevelSelect = false;
    mUsingLvlSelectCheat = false;

    gEnableFartGasCheat = 0;
    gVoiceCheat = 0;
    gEnableCheatFMV = 0;
    gEnableCheatLevelSelect = 0;
    sKilledMudokons = 0;
    sRescuedMudokons = 0;
    gRestartRuptureFarmsKilledMuds = 0;
    gRestartRuptureFarmsSavedMuds = 0;

    // 30 = fmv select
    if (gMap.mCurrentCamera == 30)
    {
        field_204_flags &= ~2u;
        mToFmvSelect = true;
        field_21C = FP_FromInteger(0);
        mFnUpdate = &Menu::FMV_Select_Update;
        mFnRender = &Menu::FMV_Or_Level_Select_Render;
        mSelectedButtonIndex.raw = 0;
        field_218_previous_fmv_or_level_selection = 0;
        field_220 = FP_FromInteger(0);

        sActiveList = sFmvList;
        sListCount = ALIVE_COUNTOF(sFmvList);

        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));

        GetAnimation().ReloadPal();
    }

    sAvailableControllers = (gJoystickAvailable != 0) + 1;
}

Menu::~Menu()
{
    Path::TLV_Reset(field_1D4_tlvInfo, -1, 0, 0);
    mButtonAnim.VCleanUp();

    /*
    for (s32 i = 0; i < ALIVE_COUNTOF(field_E4_res_array); i++)
    {
        if (mAnim.field_20_ppBlock != field_E4_res_array[i])
        {
            ResourceManager::FreeResource_455550(field_E4_res_array[i]);
        }
    }*/

    if (mMenuTrans)
    {
        mMenuTrans->mBaseGameObjectFlags.Set(Options::eDead);
        mMenuTrans->mBaseGameObjectRefCount--;
        mMenuTrans = nullptr;
    }

    if (mMenuFade1)
    {
        mMenuFade1->mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (mMenuFade2)
    {
        mMenuFade2->mBaseGameObjectFlags.Set(Options::eDead);
    }

    gMainMenuInstanceCount--;
}

void Menu::VRender(PrimHeader** ppOt)
{
    if ((field_204_flags >> 1) & 1)
    {
        GetAnimation().VRender(184, 162, ppOt, 0, 0);

        PSX_RECT rect = {};
        GetAnimation().Get_Frame_Rect(&rect);
    }
    (this->*mFnRender)(ppOt);
}

void Menu::VScreenChanged()
{
    // Empty
}

void Menu::VUpdate()
{
    mButtonRgb += mButtonRgbModifier;

    if (mButtonRgb < 40 || mButtonRgb > 80)
    {
        mButtonRgb -= mButtonRgbModifier;
        mButtonRgbModifier = -mButtonRgbModifier;
    }

    mButtonAnim.SetRGB(mButtonRgb, mButtonRgb, mButtonRgb);

    (this->*mFnUpdate)();
}

// Wait for the door that abes head comes out of to open
void Menu::WaitForDoorToOpen()
{
    field_204_flags |= 2u;
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::AbeIntro));
        GetAnimation().ReloadPal();
        mFnUpdate = &Menu::WaitForAbesHeadPoppingThroughDoor;
       // ResourceManager::FreeResource_455550(field_E4_res_array[3]);
        field_204_flags &= ~2u;
        //field_E4_res_array[3] = nullptr;
        field_1D8_timer = sGnFrame + 15;
    }
}

void Menu::WaitForAbesHeadPoppingThroughDoor()
{
    if (field_1D8_timer <= static_cast<s32>(sGnFrame))
    {
        field_204_flags |= 2u;
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::AbeIntro));
        mFnUpdate = &Menu::AbePopThroughDoor;
    }
}

void Menu::AbePopThroughDoor()
{
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        // Put abe into the bug eyed idle loop
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_IdleBlink));
        mFnUpdate = &Menu::SayHelloWaitForLoading;
        mIdleInputCounter = 0;
        /*
        ResourceManager::FreeResource_455550(field_E4_res_array[2]);
        field_E4_res_array[2] = nullptr;
        ResourceManager::LoadResourceFile("ABESPEAK.BAN", Menu::OnResourceLoaded, this);*/
    }
}

void Menu::CopyRight_Update()
{
    if (gMap.mCurrentCamera == 23)
    {
        if (static_cast<s32>(sGnFrame) > field_1D8_timer)
        {
            field_1D8_timer = sGnFrame + 150;
            gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eCopyright_10, CameraSwapEffects::eInstantChange_0, 0, 0);
        }
    }
    else
    {
        if (static_cast<s32>(sGnFrame) > field_1D8_timer || gMap.mCurrentCamera != 10)
        {
            gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eMainMenu_1, CameraSwapEffects::ePlay1FMV_5, 30102, 0);
            mFnUpdate = &Menu::WaitForDoorToOpen;
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuDoor));
        }
    }
}

void Menu::FMV_Select_Update()
{
    gEnableCheatFMV = 0;
    gEnableCheatLevelSelect = 0;

    if (sMovie_ref_count_9F309C == 0)
    {
        if (Input().IsAnyPressed(InputObject::PadIndex::First, InputCommands::eUp)) // TODO: Input constants
        {
            if (mSelectedButtonIndex.raw > 0 && field_21C == FP_FromInteger(0))
            {
                mSelectedButtonIndex.raw--;
                SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
            }
        }
        else if (Input().IsAnyPressed(InputObject::PadIndex::First, InputCommands::eDown | InputCommands::eCheatMode)) // TODO: Input constants
        {
            if (mSelectedButtonIndex.raw < (sListCount - 1) && field_21C == FP_FromInteger(0))
            {
                mSelectedButtonIndex.raw++;
                SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
            }
        }

        if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eBack | InputCommands::eHop)) // TODO: Input constants
        {
            // Go back to main screen
            mUsingLvlSelectCheat = false;

            if (mMenuTrans)
            {
                mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
            }
            else
            {
                mMenuTrans = relive_new MainMenuTransition(Layer::eLayer_FadeFlash_40, 1, 0, 16, TPageAbr::eBlend_1);
            }
            mFnUpdate = &Menu::FMV_Or_Level_Select_To_Back_Update;
        }

        if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eUnPause_OrConfirm)) // TODO: Input constants
        {
            if (mToFmvSelect)
            {
                if (sActiveList[mSelectedButtonIndex.raw].mFmvId >= 0)
                {
                    SND_StopAll();

                    const FmvInfo* pFmvRec = Path_Get_FMV_Record_434680(sActiveList[mSelectedButtonIndex.raw].mLevel, sActiveList[mSelectedButtonIndex.raw].mFmvId);
                    relive_new Movie(pFmvRec->field_0_pName);

                    while (sMovie_ref_count_9F309C)
                    {
                        for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
                        {
                            BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
                            if (!pObj)
                            {
                                break;
                            }

                            if (pObj->Type() == ReliveTypes::eMovie)
                            {
                                if (pObj->mBaseGameObjectFlags.Get(BaseGameObject::eUpdatable_Bit2))
                                {
                                    if (!pObj->mBaseGameObjectFlags.Get(BaseGameObject::eDead) && (!gNumCamSwappers || pObj->mBaseGameObjectFlags.Get(BaseGameObject::eUpdateDuringCamSwap_Bit10)))
                                    {
                                        pObj->VUpdate();
                                    }
                                }
                            }
                        }
                    }

                    gPsxDisplay.PutCurrentDispEnv();
                    pScreenManager->DecompressCameraToVRam(gMap.field_2C_camera_array[0]->field_C_pCamRes);
                    pScreenManager->EnableRendering();
                    SND_Restart();
                }
                else
                {
                    // "Credits" FMV
                    gCreditsControllerExists = 1;

                    // The credits are re-done in this class rather than using CreditsController... go to the Sherry credit screen
                    mCurrentCreditsCamera = 1;
                    gMap.SetActiveCam(EReliveLevelIds::eCredits, 1, static_cast<s16>(mCurrentCreditsCamera), CameraSwapEffects::eInstantChange_0, 0, 0);
                    mFnUpdate = &Menu::To_Credits_Update;
                    mFnRender = &Menu::Empty_Render;
                }
            }
            else
            {
                const MenuFMV* pRec = &sActiveList[mSelectedButtonIndex.raw];

                mLvlSelectCheatLevel = pRec->mLevel;
                mLvlSelectCheatPath = pRec->mPath;
                mLvlSelectCheatCamera = pRec->mCamera;

                mLvlSelectCheatAbeSpawnX = pRec->mAbeSpawnX;
                mLvlSelectCheatAbeSpawnY = pRec->mAbeSpawnY;

                mUsingLvlSelectCheat = true;

                mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
                mFnUpdate = &Menu::Level_Cheat_To_Loading_Update;
            }
        }
    }
}

void Menu::Empty_Render(PrimHeader**)
{
    // Draw nothing
}

static const MenuButtonAnimInfo sFmvOrLevelSelectButtons = {34, 65, AnimId::MenuHighlight_Circle};
const MenuButtonInputInfo x_and_esc_4D0660[] = {{33, 29, InputCommands::eUnPause_OrConfirm}, {304, 165, InputCommands::eBack}};


void NavigateBetweenTwoPoints(FP& a, FP& b)
{
    if (a > FP_FromInteger(0))
    {
        a -= b;
        if (a <= FP_FromInteger(0))
        {
            a = FP_FromInteger(0);
        }
        else
        {
            b -= FP_FromDouble(0.2);
            if (b <= FP_FromInteger(0))
            {
                b = FP_FromInteger(0);
            }
        }
    }
    else if (a < FP_FromInteger(0))
    {
        a += b;
        if (a < FP_FromInteger(0))
        {
            b -= FP_FromDouble(0.2);
            if (b <= FP_FromInteger(0))
            {
                b = FP_FromInteger(0);
            }
        }
        else
        {
            a = FP_FromInteger(0);
        }
    }
}

void Menu::FMV_Or_Level_Select_Render(PrimHeader** ppOt)
{
    // Glow hilight
    mButtonAnim.VRender(
        sFmvOrLevelSelectButtons.xpos,
        sFmvOrLevelSelectButtons.ypos,
        ppOt,
        0,
        0);

    PSX_RECT rect = {};
    mButtonAnim.Get_Frame_Rect(&rect);

    if (mSelectedButtonIndex.raw != field_218_previous_fmv_or_level_selection)
    {
        if (field_21C != FP_FromInteger(0))
        {
            mSelectedButtonIndex.raw = field_218_previous_fmv_or_level_selection;
        }
        else
        {
            if (mSelectedButtonIndex.raw <= field_218_previous_fmv_or_level_selection)
            {
                if (mSelectedButtonIndex.raw < field_218_previous_fmv_or_level_selection)
                {
                    field_21C = FP_FromInteger(-1) * FP_FromInteger(26);
                }
            }
            else
            {
                field_21C = FP_FromInteger(26);
            }
            field_220 = FP_FromDouble(4.5);
            field_218_previous_fmv_or_level_selection = mSelectedButtonIndex.raw;
        }
    }

    NavigateBetweenTwoPoints(field_21C, field_220);

    s32 polyOffset = 0;

    s32 idxStart = -1;
    s32 textYOff = -26;
    s32 v24 = -1;
    s32 v25 = -26;
    do
    {
        s32 itemIdx = mSelectedButtonIndex.raw + idxStart;
        if (itemIdx >= 0 && itemIdx <= sListCount - 1)
        {
            field_1F4_text = sActiveList[itemIdx].mMenuEntryName;
            const s32 textWidth = mFont.MeasureScaledTextWidth(field_1F4_text, FP_FromInteger(1));
            s16 textXOff = 0;
            if (textWidth >= 336)
            {
                textXOff = 16;
            }
            else
            {
                textXOff = static_cast<s16>((368 - textWidth) / 2);
            }

            const s16 textYPos = static_cast<s16>((FP_GetExponent(field_21C + FP_FromDouble(0.5))) + textYOff + 114);
            const s16 textXPos = textXOff + 6;
            polyOffset = mFont.DrawString(
                ppOt,
                field_1F4_text,
                textXPos,
                textYPos,
                TPageAbr::eBlend_0,
                1,
                0,
                Layer::eLayer_AbeMenu_32,
                210,
                150,
                80,
                polyOffset,
                FP_FromInteger(1),
                640,
                0);

            polyOffset = mFont.DrawString(
                ppOt,
                field_1F4_text,
                textXPos + 2,
                textYPos + 2,
                TPageAbr::eBlend_0,
                1,
                0,
                Layer::eLayer_AbeMenu_32,
                0,
                0,
                0,
                polyOffset,
                FP_FromInteger(1),
                640,
                0);
            textYOff = v25;
        }
        textYOff += 26;
        idxStart = v24++ + 1;
        v25 = textYOff;
    }
    while (textYOff < 52);


    for (s32 i = 0; i < 2; i++)
    {
        RenderElement(x_and_esc_4D0660[i].xpos, x_and_esc_4D0660[i].ypos, x_and_esc_4D0660[i].inputCommand, ppOt, &mFont, &polyOffset);
    }
}

void Menu::SayHelloWaitForLoading()
{
    // After 1 idle anim loop
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        // Wait for in progress loading (gamespeak ban) to finish
        /*
        if (!field_E4_res_array[0])
        {
            ProgressInProgressFilesLoading();
        }*/
        Mudokon_SFX(MudSounds::eHello_3, 0, 0, 0);
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Hello));
        mFnUpdate = &Menu::WaitForAbeSayHello;
    }
}

void Menu::WaitForAbeSayHello()
{
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        // Abe has finished saying hello, go to main menu handler
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
        GetAnimation().SetFrame(7u);
        mFnUpdate = &Menu::MainScreen_Update;
        mFnRender = &Menu::MainScreen_Render;
        field_1D8_timer = sGnFrame + Math_RandomRange(300, 450);
    }
}

void Menu::ProgressInProgressFilesLoading()
{
    TRACE_ENTRYEXIT;
    /*
    do
    {
        for (s32 i = 0; i < gLoadingFiles->Size(); i++)
        {
            auto pObj = gLoadingFiles->ItemAt(i);

            if (pObj->mBaseGameObjectFlags.Get(BaseGameObject::eDead) && pObj->mBaseGameObjectRefCount == 0)
            {
                i = gLoadingFiles->RemoveAt(i);
                relive_delete pObj;
            }
            else
            {
                pObj->VUpdate();
            }
        }
    }
    while (!gLoadingFiles->IsEmpty());
    */
}

void Menu::MainScreen_Render(PrimHeader** ppOt)
{
    mButtonAnim.VRender(
        sMainScreenButtons[mSelectedButtonIndex.raw].xpos,
        sMainScreenButtons[mSelectedButtonIndex.raw].ypos,
        ppOt,
        0,
        0);

    PSX_RECT rect = {};
    mButtonAnim.Get_Frame_Rect(&rect);

    s32 polyOffset = 0;
    for (const auto& element : sMainScreenInputs)
    {
        RenderElement(element.xpos, element.ypos, element.inputCommand, ppOt, &mFont, &polyOffset);
    }
}

void Menu::MainScreen_Update()
{
    // Calculate idle timers for playing game play demos
    s32 bSmallerTimeout = 0;
    if (Input().Pressed(InputObject::PadIndex::First))
    {
        bSmallerTimeout = 0;
        mIdleInputCounter = 0;
        sDemoPlay = 0;
    }
    else
    {
        mIdleInputCounter++;
        bSmallerTimeout = sDemoPlay;
    }

    // Backwards menu button
    if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eLeft | InputCommands::eUp))
    {
        mSelectedButtonIndex.raw--;
        if (mSelectedButtonIndex.mainmenu < MainMenuOptions::eGameSpeak_0)
        {
            mSelectedButtonIndex.mainmenu = MainMenuOptions::eOptions_4; // TODO: Why isn't count of stru_4D00B0 ??
        }

        mButtonAnim.Set_Animation_Data(GetAnimRes(sMainScreenButtons[mSelectedButtonIndex.raw].animId));
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
        bSmallerTimeout = sDemoPlay;
    }

    // Forward menu button
    if (Input().IsAnyHeld(InputObject::PadIndex::First, (InputCommands::eDown | InputCommands::eRight | InputCommands::eCheatMode)))
    {
        mSelectedButtonIndex.raw++;
        if (mSelectedButtonIndex.mainmenu > MainMenuOptions::eOptions_4) // TODO: Why isn't count of stru_4D00B0 ??
        {
            mSelectedButtonIndex.mainmenu = MainMenuOptions::eGameSpeak_0;
        }

        mButtonAnim.Set_Animation_Data(GetAnimRes(sMainScreenButtons[mSelectedButtonIndex.raw].animId));
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
        bSmallerTimeout = sDemoPlay;
    }
    const s32 idleMax = bSmallerTimeout != 0 ? 300 : 1500;
    if (Input().IsAnyHeld(InputObject::PadIndex::First, (InputCommands::eThrowItem | InputCommands::eUnPause_OrConfirm | InputCommands::eDoAction | InputCommands::eCheatMode | InputCommands::eBack)) || mIdleInputCounter > idleMax)
    {
        if (mIdleInputCounter <= idleMax)
        {
            //if (field_E4_res_array[0])
            {
                field_204_flags |= 1u;

                if (mSelectedButtonIndex.mainmenu == MainMenuOptions::eBegin_1)
                {
                    // Begin/new game
                    Mudokon_SFX(MudSounds::eFollowMe_4, 0, 0, 0);
                    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_FollowMe));
                }
                else if (mSelectedButtonIndex.mainmenu == MainMenuOptions::eQuit_2)
                {
                    // Quit
                    Mudokon_SFX(MudSounds::eGoodbye_12, 0, 0, 0);
                    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Goodbye));
                }
                else
                {
                    // 0 = game speak,
                    // 3 = load
                    // 4 = options
                    Mudokon_SFX(MudSounds::eOkay_13, 0, 0, 0);
                    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Ok));
                }

                mFnUpdate = &Menu::WaitForSpeakFinishAndStartChangeEffect;
            }
            /*
            * // TODO Binned
            else
            {
                if (mMenuTrans)
                {
                    mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
                }
                else
                {
                    mMenuTrans = relive_new MainMenuTransition(Layer::eLayer_FadeFlash_40, 1, 0, 16, TPageAbr::eBlend_1);
                    if (mMenuTrans)
                    {
                        mMenuTrans->mBaseGameObjectRefCount++;
                    }
                }

                mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
                mFnUpdate = &Menu::GoToSelectedMenuPage;
            }*/
        }
        else
        {
            // Play a demo
            sDemoPlay = 1;
            mSelectedButtonIndex.mainmenu = MainMenuOptions::eBegin_1;
            gAttract = 1;

            char_type fileNameBuf[20] = {};
            sprintf(fileNameBuf, "PLAYBK%02d.JOY", gJoyResId);
            //ResourceManager::LoadResourceFile_4551E0(fileNameBuf, 0, 0, 0);

            if (mMenuTrans)
            {
                mMenuTrans->mBaseGameObjectRefCount--;
                mMenuTrans->mBaseGameObjectFlags.Set(Options::eDead);
            }

            mMenuTrans = relive_new MainMenuTransition(Layer::eLayer_FadeFlash_40, 1, 0, 16, TPageAbr::eBlend_1);
            if (mMenuTrans)
            {
                mMenuTrans->mBaseGameObjectRefCount++;
            }
            mFnUpdate = &Menu::GoToSelectedMenuPage;
        }
    }

    if (gEnableCheatFMV)
    {
        gEnableCheatFMV = 0;
        mToFmvSelect = true;
        sActiveList = sFmvList;
        sListCount = ALIVE_COUNTOF(sFmvList);
       // if (field_E4_res_array[0])
        {
            Mudokon_SFX(MudSounds::eOkay_13, 0, 0, 0);
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Ok));
            mFnUpdate = &Menu::WaitForSpeakFinishAndStartChangeEffect;
        }
        // TODO: probably binned now
        /*
        else
        {
            if (mMenuTrans)
            {
                mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
            }
            else
            {
                mMenuTrans = relive_new MainMenuTransition(Layer::eLayer_FadeFlash_40, 1, 0, 16, TPageAbr::eBlend_1);
                if (mMenuTrans)
                {
                    mMenuTrans->mBaseGameObjectRefCount++;
                }
            }

            mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
            mFnUpdate = &Menu::GoToSelectedMenuPage;
        }*/
    }

    if (gEnableCheatLevelSelect)
    {
        gEnableCheatLevelSelect = 0;
        mToLevelSelect = true;
        sActiveList = sLevelList;
        sListCount = ALIVE_COUNTOF(sLevelList);
       // if (field_E4_res_array[0])
        {
            Mudokon_SFX(MudSounds::eOkay_13, 0, 0, 0);
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Ok));
            mFnUpdate = &Menu::WaitForSpeakFinishAndStartChangeEffect;
        }
        /*
        // TODO: prob binned
        else
        {
            if (mMenuTrans)
            {
                mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
            }
            else
            {
                mMenuTrans = relive_new MainMenuTransition(Layer::eLayer_FadeFlash_40, 1, 0, 16, TPageAbr::eBlend_1);
                if (mMenuTrans)
                {
                    mMenuTrans->mBaseGameObjectRefCount++;
                }
            }

            mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
            mFnUpdate = &Menu::GoToSelectedMenuPage;
        }*/
    }

    // Some sort of idle anim toggling
    if (((field_204_flags) >> 2) & 1)
    {
        if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
        {
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
            field_204_flags &= ~4u;
            field_1D8_timer = sGnFrame + Math_RandomRange(120, 450);
        }
    }
    else if (field_1D8_timer <= static_cast<s32>(sGnFrame))
    {
        if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
        {
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_IdleBlink));
            field_204_flags |= 4u;
        }
    }
}

// After fade out go to gamespeak/options/load/whatever
void Menu::GoToSelectedMenuPage()
{
    if (mMenuTrans == nullptr || mMenuTrans->field_16_bDone)
    {
        if (mToFmvSelect)
        {
            gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eFmvSelect_30, CameraSwapEffects::eInstantChange_0, 0, 0);
            mFnUpdate = &Menu::ToNextMenuPage;
            return;
        }

        if (mToLevelSelect)
        {
            gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eLvlSelect_31, CameraSwapEffects::eInstantChange_0, 0, 0);
            mFnUpdate = &Menu::ToNextMenuPage;
            return;
        }

        switch (mSelectedButtonIndex.mainmenu)
        {
                // Gamespeak
            case MainMenuOptions::eGameSpeak_0:
                field_204_flags &= ~1u;

                // Diff cam depending on input method ?
                if (Input_JoyStickEnabled())
                {
                    gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eGamespeakGamepad_3, CameraSwapEffects::eInstantChange_0, 0, 0);
                }
                else
                {
                    gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eGamespeakKeyboard_33, CameraSwapEffects::eInstantChange_0, 0, 0);
                }

                mFnUpdate = &Menu::ToNextMenuPage;

                mButtonAnim.SetRGB(127, 127, 127);
                break;

            // Begin
            case MainMenuOptions::eBegin_1:
                mFnUpdate = &Menu::ToLoading;
                break;

            // Quit
            case MainMenuOptions::eQuit_2:
                sBreakGameLoop = 1;
                exit(0);
                break;

            // Load
            case MainMenuOptions::eLoad_3:
                gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eLoad_6, CameraSwapEffects::eInstantChange_0, 0, 0);
                mFnUpdate = &Menu::ToNextMenuPage;
                break;

            // Options
            case MainMenuOptions::eOptions_4:
                gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eOptions_2, CameraSwapEffects::eInstantChange_0, 0, 0);
                mFnUpdate = &Menu::ToNextMenuPage;
                break;

            default:
                LOG_ERROR("Unknown menu item %d", mSelectedButtonIndex.raw);
                break;
        }
    }
}

void Menu::WaitForSpeakFinishAndStartChangeEffect()
{
    // Abe finished speaking?
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        if (mMenuTrans)
        {
            mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
        }
        else
        {
            mMenuTrans = relive_new MainMenuTransition(Layer::eLayer_FadeFlash_40, 1, 0, 16, TPageAbr::eBlend_1);
            if (mMenuTrans)
            {
                mMenuTrans->mBaseGameObjectRefCount++;
            }
        }

        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
        mFnUpdate = &Menu::GoToSelectedMenuPage;
    }
}

void Menu::ToNextMenuPage()
{
    if (gNumCamSwappers <= 0)
    {
        if (mToFmvSelect || mToLevelSelect)
        {
            field_204_flags &= ~2u;
            field_21C = FP_FromInteger(0);
            mFnUpdate = &Menu::To_FMV_Or_Level_Select_Update;
            mFnRender = &Menu::FMV_Or_Level_Select_Render;
            mSelectedButtonIndex.raw = 0;
            field_218_previous_fmv_or_level_selection = 0;
            field_220 = FP_FromInteger(0);
        }
        else
        {
            switch (mSelectedButtonIndex.mainmenu)
            {
                case MainMenuOptions::eGameSpeak_0:
                {
                    mButtonAnim.ReloadPal();
                    mFnUpdate = &Menu::ToGameSpeak_Update;
                    mFnRender = &Menu::GameSpeak_Render;
                    mSelectedButtonIndex.gamespeak_menu = GameSpeakOptions::eWait_0;
                    break;
                }

                // ?? leads to 2 options, one starts the game, the other leads to
                // another screen with 1 option that only starts the game
                case MainMenuOptions::eBegin_1: // Never reached?
                    mFnUpdate = &Menu::Update_NoRefs;
                    mFnRender = &Menu::Render_NoRefs;
                    mSelectedButtonIndex.raw = 0;
                    break;

                case MainMenuOptions::eLoad_3:
                    field_204_flags &= ~2u;
                    mFnUpdate = &Menu::To_Load_Update;
                    mFnRender = &Menu::Load_Render;
                    mSelectedButtonIndex.raw = 0;
                    break;

                case MainMenuOptions::eOptions_4:
                {
                    mFnUpdate = &Menu::To_Options_Update;
                    mFnRender = &Menu::Options_Render;
                    mButtonAnim.Set_Animation_Data(GetAnimRes(sOptionsButtons[0].animId));
                    mSelectedButtonIndex.options_menu = OptionsMenuOptions::eController_0;
                    break;
                }

                default:
                    LOG_ERROR("Unknown menu item %d", mSelectedButtonIndex.raw);
                    break;
            }
        }
        mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 0, 0, 16);
    }
}

void Menu::ToLoading()
{
    if (mMenuTrans)
    {
        if (mMenuTrans->field_16_bDone)
        {
            mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 0, 0, 16);
            gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eLoading_21, CameraSwapEffects::eInstantChange_0, 0, 0);
            field_204_flags &= ~2u;
            mFnUpdate = &Menu::Loading_Update;
            mFnRender = &Menu::Empty_Render;
        }
    }
}


void Menu::ToGameSpeak_Update()
{
    if (mMenuTrans)
    {
        if (mMenuTrans->field_16_bDone)
        {
            mFnUpdate = &Menu::GameSpeak_Update;
            mIdleInputCounter = 0;
        }
    }
}


void Menu::GameSpeak_Render(PrimHeader** ppOt)
{
#if AUTO_SWITCH_CONTROLLER // OG Change - Automatically switch between Gamepad/Keyboard GameSpeak Menu if joystick is added/removed
    if (Input_JoyStickEnabled() && gMap.mNextCamera == CameraIds::Menu::eGamespeakKeyboard_33)
    {
        gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eGamespeakGamepad_3, CameraSwapEffects::eInstantChange_0, 0, 0);
    }
    else if (!Input_JoyStickEnabled() && gMap.mNextCamera == CameraIds::Menu::eGamespeakGamepad_3)
    {
        gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eGamespeakKeyboard_33, CameraSwapEffects::eInstantChange_0, 0, 0);
    }
#endif

    // Only renders exit and keys
    s32 polyOffset = 0;
    const s32 count = Input_JoyStickEnabled() != 0 ? 13 : 1;
    for (s32 i = 20; i < 20 + count; i++)
    {
        RenderElement(
            sGameSpeakInputs[i].xpos,
            sGameSpeakInputs[i].ypos,
            sGameSpeakInputs[i].inputCommand,
            ppOt,
            &mFont,
            &polyOffset);
    }
}

void Menu::To_FMV_Or_Level_Select_Update()
{
    if (mMenuTrans->field_16_bDone)
    {
        //ResourceManager::FreeResource_455550(field_E4_res_array[0]);
        //field_E4_res_array[0] = nullptr;
        mFnUpdate = &Menu::FMV_Select_Update;
    }
}


void Menu::Update_NoRefs()
{
    
}


void Menu::Render_NoRefs(PrimHeader** /*ppOt*/)
{
    
}


void Menu::To_Load_Update()
{
    sSelectedSaveIdx = 0;

    mSelectedButtonIndex.raw = 0;
    field_230_bGoBack = 0;
    field_228 = FP_FromInteger(0);

    if (mMenuTrans)
    {
        if (mMenuTrans->field_16_bDone)
        {
            sSaveIdx = 0;
            IO_EnumerateDirectory("*.sav", [](const char_type* fileName, u32 /*lastWriteTime*/)
                                  {
                                      if (sSaveIdx < 128) // TODO: Array len
                                      {
                                          size_t saveNameLen = strlen(fileName) - 4;

                                          // Limit length to prevent buffer overflow
                                          if (saveNameLen > 19)
                                          {
                                              saveNameLen = 19;
                                          }

                                          if (saveNameLen > 0)
                                          {
                                              memcpy(sSaveNames_9F1DD8[sSaveIdx].field_0_mName, fileName, saveNameLen);
                                              sSaveNames_9F1DD8[sSaveIdx].field_0_mName[saveNameLen] = 0;
                                              sSaveIdx++;
                                          }
                                      }
                                  });
            qsort(sSaveNames_9F1DD8, sSaveIdx, sizeof(SaveName), &Menu::StringsEqual);

            mIdleInputCounter = 0;
            mFnUpdate = &Menu::Load_Update;
        }
    }
}


void Menu::Load_Render(PrimHeader** ppOt)
{
    if (field_230_bGoBack || !sSaveIdx)
    {
        mButtonAnim.VRender(sLoadButtons[1].xpos, sLoadButtons[1].ypos + 36, ppOt, 0, 0);
        PSX_RECT rect = {};
        mButtonAnim.Get_Frame_Rect(&rect);
    }
    else
    {
        mButtonAnim.VRender(sLoadButtons[0].xpos, sLoadButtons[0].ypos + 36, ppOt, 0, 0);
        PSX_RECT rect = {};
        mButtonAnim.Get_Frame_Rect(&rect);
    }

    if (mSelectedButtonIndex.raw != sSelectedSaveIdx)
    {
        if (field_228 != FP_FromInteger(0))
        {
            mSelectedButtonIndex.raw = static_cast<s16>(sSelectedSaveIdx);
        }
        else
        {
            if (mSelectedButtonIndex.raw < sSelectedSaveIdx)
            {
                field_228 = FP_FromInteger(-26);
                field_22C = FP_FromDouble(4.5);
                sSelectedSaveIdx = mSelectedButtonIndex.raw;
            }
            else if (mSelectedButtonIndex.raw > sSelectedSaveIdx)
            {
                field_228 = FP_FromInteger(26);
                field_22C = FP_FromDouble(4.5);
                sSelectedSaveIdx = mSelectedButtonIndex.raw;
            }
        }
    }

    NavigateBetweenTwoPoints(field_228, field_22C);

    s32 start = 0;
    s32 end = 0;
    if (field_228 == FP_FromInteger(0))
    {
        start = -2;
        end = 2;
    }
    else if (field_228 < FP_FromInteger(0))
    {
        start = -2;
        end = 3;
    }
    else
    {
        start = -3;
        end = 2;
    }

    s32 polyOffset = 0;
    while (start <= end)
    {
        const auto curIdx = start + mSelectedButtonIndex.raw;
        if (curIdx >= 0 && curIdx < sSaveIdx)
        {
            field_1F4_text = sSaveNames_9F1DD8[curIdx].field_0_mName;

            const auto name_width = mFont.MeasureScaledTextWidth(field_1F4_text, FP_FromInteger(1));
            s16 text_x = 0;
            if (name_width >= 336)
            {
                text_x = 16;
            }
            else
            {
                text_x = static_cast<s16>((368 - name_width) / 2);
            }

            const s32 yAdjust = (FP_GetExponent(field_228 + FP_FromDouble(0.5))) + 26 * start + 120;
            const s16 text_y = static_cast<s16>((yAdjust + FP_GetExponent(FP_FromInteger(-7) * FP_FromInteger(1))) - 1);

            u8 r = 210;
            u8 g = 150;
            u8 b = 80;

            // Draw the "selected" item in another colour so you can see its selected
            if (start == 0)
            {
                r = 255;
                g = 218;
                b = 140;
            }

            const s32 nextPolyOff = mFont.DrawString(
                ppOt,
                field_1F4_text,
                text_x,
                text_y,
                TPageAbr::eBlend_0,
                1,
                0,
                Layer::eLayer_AbeMenu_32,
                r,
                g,
                b,
                polyOffset,
                FP_FromInteger(1),
                640,
                0);

            polyOffset = mFont.DrawString(
                ppOt,
                field_1F4_text,
                text_x + 2,
                text_y + 2,
                TPageAbr::eBlend_0,
                1,
                0,
                Layer::eLayer_AbeMenu_32,
                0,
                0,
                0,
                nextPolyOff,
                FP_FromInteger(1),
                640,
                0);
        }
        start++;
    }

    for (s32 i = 0; i < 2; i++)
    {
        RenderElement(
            sLoadInputs[i].xpos,
            sLoadInputs[i].ypos,
            sLoadInputs[i].inputCommand,
            ppOt,
            &mFont,
            &polyOffset);
    }
}

void Menu::To_Options_Update()
{
    if (mMenuTrans)
    {
        if (mMenuTrans->field_16_bDone)
        {
            mFnUpdate = &Menu::Options_Update;
            mIdleInputCounter = 0;
        }
    }
}

void Menu::Options_Render(PrimHeader** ppOt)
{
    mButtonAnim.VRender(
        sOptionsButtons[mSelectedButtonIndex.raw].xpos,
        sOptionsButtons[mSelectedButtonIndex.raw].ypos,
        ppOt,
        0,
        0);

    PSX_RECT rect = {};
    mButtonAnim.Get_Frame_Rect(&rect);

    s32 polyOff = 0;
    for (const auto& element : sOptionsInputs)
    {
        RenderElement(element.xpos, element.ypos, element.inputCommand, ppOt, &mFont, &polyOff);
    }
}

void Menu::FMV_Or_Level_Select_Back_Update()
{
    if (gNumCamSwappers <= 0)
    {
        mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 0, 0, 16);
        
        mSelectedButtonIndex.mainmenu = MainMenuOptions::eBegin_1;

        mButtonAnim.Set_Animation_Data(GetAnimRes(sMainScreenButtons[1].animId));
        field_204_flags |= 2u;
        mToFmvSelect = false;
        mToLevelSelect = false;
        mFnUpdate = &Menu::To_MainScreen_Update;
        mFnRender = &Menu::MainScreen_Render;
    }
}

void Menu::Loading_Update()
{
    if (!gAttract)
    {
        if (mMenuTrans)
        {
            if (mMenuTrans->field_16_bDone)
            {
                if (gAttract)
                {
                    char_type buffer[92] = {};
                    sprintf(buffer, "loading Joy # %d\n", gJoyResId);
                    // Never used ??
                    LOG_INFO(buffer);
                }

                mMenuTrans->mBaseGameObjectRefCount--;
                mMenuTrans->mBaseGameObjectFlags.Set(Options::eDead);
                mMenuTrans = nullptr;

                /*
                if (!field_E4_res_array[0])
                {
                    ProgressInProgressFilesLoading();
                }*/

                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
                //ResourceManager::FreeResource_455550(field_E4_res_array[0]);
                //field_E4_res_array[0] = nullptr;
                //ResourceManager::Reclaim_Memory_455660(0);
                mFnUpdate = &Menu::NewGameStart;
            }
        }
    }
}

void Menu::NewGameStart()
{
    if (!sActiveHero)
    {
        sActiveHero = relive_new Abe();
    }

    if (gAttract)
    {
        // OG bug fix: the demo will load a save which will call Kill_Objects_451720 which will delete this object
        // resulting in a crash when we try access any member vars at the end. Bump the ref count so we can kill ourselves instead.
        mBaseGameObjectRefCount++;
        // TODO: The ctor of the playback should load the demo res itself
        u8** ppRes = nullptr; //ResourceManager::GetLoadedResource(ResourceManager::Resource_Plbk, gJoyResId, 1, 0);
        relive_new DemoPlayback(ppRes, 0);
        mBaseGameObjectRefCount--;
    }
    else
    {
        if (!gPauseMenu)
        {
            gPauseMenu = relive_new PauseMenu();
        }

        if (mUsingLvlSelectCheat)
        {
            mUsingLvlSelectCheat = false;
            gMap.SetActiveCam(mLvlSelectCheatLevel, mLvlSelectCheatPath, mLvlSelectCheatCamera, CameraSwapEffects::eInstantChange_0, 0, 0);
            sActiveHero->mXPos = FP_FromInteger(mLvlSelectCheatAbeSpawnX);
            sActiveHero->mYPos = FP_FromInteger(mLvlSelectCheatAbeSpawnY);
        }
        else
        {
            // Start the game in the biggest meat processing plant
            gInfiniteGrenades = false;
            gMap.SetActiveCam(EReliveLevelIds::eRuptureFarms, 15, 1, CameraSwapEffects::ePlay1FMV_5, 102, 0);

            // What if someone made a level editor and wanted to change where abe spawns on the first map? Well... hard luck pal
            sActiveHero->mXPos = FP_FromInteger(1378);
            sActiveHero->mYPos = FP_FromInteger(83);
        }
    }
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void Menu::Options_Update()
{
    // Idle time calculate
    if (Input().Pressed(InputObject::PadIndex::First))
    {
        mIdleInputCounter = 0;
    }
    else
    {
        mIdleInputCounter++;
    }

    // Menu backwards
    if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eUp))
    {
        if (mSelectedButtonIndex.options_menu <= OptionsMenuOptions::eController_0)
        {
            mSelectedButtonIndex.options_menu = OptionsMenuOptions::eSound_1;
        }
        else
        {
            mSelectedButtonIndex.raw--;
        }

        mButtonAnim.Set_Animation_Data(GetAnimRes(sOptionsButtons[mSelectedButtonIndex.raw].animId));
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
    }

    // Menu forwards
    if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eDown | InputCommands::eCheatMode))
    {
        if (mSelectedButtonIndex.options_menu >= OptionsMenuOptions::eSound_1)
        {
            mSelectedButtonIndex.options_menu = OptionsMenuOptions::eController_0;
        }
        else
        {
            mSelectedButtonIndex.raw++;
        }

        mButtonAnim.Set_Animation_Data(GetAnimRes(sOptionsButtons[mSelectedButtonIndex.raw].animId));
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
    }

    if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eUnPause_OrConfirm | InputCommands::eDoAction))
    {
        Mudokon_SFX(MudSounds::eOkay_13, 0, 0, 0);
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Ok));
        mFnUpdate = &Menu::Options_WaitForAbeSpeak_Update;
    }

    if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eBack | InputCommands::eHop) || mIdleInputCounter > 900)
    {
        // Back to main menu
        mSelectedButtonIndex.options_menu = OptionsMenuOptions::eMainMenu_2;
        mButtonAnim.Set_Animation_Data(GetAnimRes(sOptionsButtons[2].animId));
        Mudokon_SFX(MudSounds::eOkay_13, 0, 0, 0);
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Ok));
        mFnUpdate = &Menu::Options_WaitForAbeSpeak_Update;
    }

    // Some sort of idle anim toggle?
    if (((field_204_flags) >> 2) & 1)
    {
        if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
        {
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
            field_204_flags &= ~4u;
            field_1D8_timer = sGnFrame + Math_RandomRange(120, 450);
        }
    }
    else if (field_1D8_timer <= static_cast<s32>(sGnFrame))
    {
        if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
        {
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_IdleBlink));
            field_204_flags |= 4u;
        }
    }
}

void Menu::Options_WaitForAbeSpeak_Update()
{
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
        mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
        mFnUpdate = &Menu::Option_GoTo_Selected_Update;
    }
}

void Menu::Option_GoTo_Selected_Update()
{
    if (mMenuTrans)
    {
        if (mMenuTrans->field_16_bDone)
        {
            switch (mSelectedButtonIndex.options_menu)
            {
                // Controller
                case OptionsMenuOptions::eController_0:
                    gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eController_40, CameraSwapEffects::eInstantChange_0, 0, 0);
                    break;

                // Sound
                case OptionsMenuOptions::eSound_1:
                    gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eSound_5, CameraSwapEffects::eInstantChange_0, 0, 0);
                    break;

                // Back to main menu screen
                case OptionsMenuOptions::eMainMenu_2:
                    gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eMainMenu_1, CameraSwapEffects::eInstantChange_0, 0, 0);
                    break;

                default:
                    LOG_ERROR("Unknown menu idx %d", mSelectedButtonIndex.raw);
                    break;
            }

            mFnUpdate = &Menu::Options_To_Selected_After_Cam_Change_Update;
        }
    }
}

void Menu::Options_To_Selected_After_Cam_Change_Update()
{
    if (gNumCamSwappers <= 0)
    {
        switch (mSelectedButtonIndex.options_menu)
        {
            // To controller options
            case OptionsMenuOptions::eController_0:
                field_204_flags &= ~2u;
                field_228 = FP_FromInteger(0);
                mFnUpdate = &Menu::To_Options_Controller_Update;
                mFnRender = &Menu::Options_Controller_Render;
                mSelectedButtonIndex.raw = static_cast<s16>(Input_JoyStickEnabled());
                field_230_bGoBack = -1;
                field_22C = FP_FromInteger(0);
                break;

            // To sound options
            case OptionsMenuOptions::eSound_1:
            {
                mFnUpdate = &Menu::To_Options_Sound_Update;
                mFnRender = &Menu::Options_Sound_Render;
                mSelectedButtonIndex.raw = sSoundMono;
                mButtonAnim.Set_Animation_Data(GetAnimRes(sSoundOptionsButtons[sSoundMono].animId));
                break;
            }

            // Back to main menu
            case OptionsMenuOptions::eMainMenu_2:
            {
                mFnUpdate = &Menu::To_MainScreen_Update;
                mFnRender = &Menu::MainScreen_Render;
                mSelectedButtonIndex.mainmenu = MainMenuOptions::eOptions_4;
                mButtonAnim.Set_Animation_Data(GetAnimRes(sMainScreenButtons[4].animId));
                break;
            }

            default:
                LOG_ERROR("Unknown menu idx %d", mSelectedButtonIndex.raw);
                break;
        }

        mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 0, 0, 16);
    }
}

void Menu::To_Options_Controller_Update()
{
    field_230_bGoBack = -1;
    if (mMenuTrans)
    {
        if (mMenuTrans->field_16_bDone)
        {
            mFnUpdate = &Menu::Options_Controller_Update;
            mFnRender = &Menu::Options_Controller_Render;
            mIdleInputCounter = 0;
            mSelectedButtonIndex.raw = static_cast<s16>(Input_JoyStickEnabled());
            field_228 = FP_FromInteger(0);
        }
    }
}

static const MenuButtonAnimInfo sControlledSelectButtons[2] = {
    {44, 236, AnimId::MenuHighlight_Circle},
    {290, 236, AnimId::MenuHighlight_Circle}};

static const MenuButtonInputInfo controllerSelectElements_4D0678[2] = {
    {43, 200, InputCommands::eUnPause_OrConfirm},
    {289, 200, InputCommands::eBack}};

void Menu::Options_Controller_Render(PrimHeader** ppOt)
{
    if (field_230_bGoBack != -1)
    {
        mButtonAnim.VRender(
            sControlledSelectButtons[field_230_bGoBack].xpos,
            sControlledSelectButtons[field_230_bGoBack].ypos,
            ppOt,
            0,
            0);
    }

    PSX_RECT rect = {};
    mButtonAnim.Get_Frame_Rect(&rect);

    if (mSelectedButtonIndex.raw != (Input_JoyStickEnabled() ? 1 : 0))
    {
        if (field_228 > FP_FromInteger(0))
        {
            mSelectedButtonIndex.raw = static_cast<s16>(Input_JoyStickEnabled());
        }
        else
        {
            if (mSelectedButtonIndex.raw < (Input_JoyStickEnabled() ? 1 : 0))
            {
                field_228 = FP_FromInteger(-1) * FP_FromInteger(26);
                field_22C = FP_FromDouble(4.5);
                Input_SetJoyStickEnabled(mSelectedButtonIndex.raw ? true : false);
            }
            else if (mSelectedButtonIndex.raw > (Input_JoyStickEnabled() ? 1 : 0))
            {
                field_228 = FP_FromInteger(26);
                field_22C = FP_FromDouble(4.5);
                Input_SetJoyStickEnabled(mSelectedButtonIndex.raw ? true : false);
            }
        }
    }

    NavigateBetweenTwoPoints(field_228, field_22C);

    s32 yOffset = -26;
    s32 polyOffset = 0;
    for (s32 i = 0; i < 3; i++)
    {
        s32 selection = mSelectedButtonIndex.raw + i - 1;
        if (selection >= 0 && selection < sAvailableControllers)
        {
            if (selection == 0)
            {
                field_1F4_text = "Keyboard";
            }
            else
            {
                field_1F4_text = "Gamepad";
            }

            const s16 fontWidth = static_cast<s16>(mFont.MeasureScaledTextWidth(field_1F4_text, FP_FromInteger(1)));
            const s16 x = static_cast<s16>(fontWidth >= 336 ? 16 : (368 - fontWidth) / 2);
            const s16 y = static_cast<s16>(FP_GetExponent(field_228 + FP_FromDouble(0.5)) + yOffset + 112);

            polyOffset = mFont.DrawString(
                ppOt,
                field_1F4_text,
                x,
                y,
                TPageAbr::eBlend_0,
                1,
                0,
                Layer::eLayer_AbeMenu_32,
                210,
                150,
                80,
                polyOffset,
                FP_FromInteger(1),
                640,
                0);
            polyOffset = mFont.DrawString(
                ppOt,
                field_1F4_text,
                x + 2,
                y + 2,
                TPageAbr::eBlend_0,
                1,
                0,
                Layer::eLayer_AbeMenu_32,
                0,
                0,
                0,
                polyOffset,
                FP_FromInteger(1),
                640,
                0);
        }
        yOffset += 26;
    };

    for (s32 i = 0; i < ALIVE_COUNTOF(controllerSelectElements_4D0678); i++)
    {
        const auto& menuElement = controllerSelectElements_4D0678[i];
        Menu::RenderElement(
            menuElement.xpos,
            menuElement.ypos,
            menuElement.inputCommand,
            ppOt,
            &mFont,
            &polyOffset);
    }
}

void Menu::To_Options_Sound_Update()
{
    if (mMenuTrans)
    {
        if (mMenuTrans->field_16_bDone)
        {
            mFnUpdate = &Menu::Options_Sound_Update;
            mIdleInputCounter = 0;
        }
    }
}

void Menu::To_MainScreen_Update()
{
    if (mMenuTrans)
    {
        if (mMenuTrans->field_16_bDone)
        {
            mFnUpdate = &Menu::MainScreen_Update;
            mIdleInputCounter = 0;
        }
    }
}

void Menu::Options_Sound_Render(PrimHeader** ppOt)
{
    mButtonAnim.VRender(
        sSoundOptionsButtons[mSelectedButtonIndex.raw].xpos,
        sSoundOptionsButtons[mSelectedButtonIndex.raw].ypos,
        ppOt,
        0,
        0);

    PSX_RECT rect = {};
    mButtonAnim.Get_Frame_Rect(&rect);

    s32 polyOffset = 0;
    for (s32 i = 0; i < 2; i++)
    {
        RenderElement(sAbeMotionsInputs[i].xpos, sAbeMotionsInputs[i].ypos, sAbeMotionsInputs[i].inputCommand, ppOt, &mFont, &polyOffset);
    }
}

void Menu::Options_Sound_Update()
{
    if (Input().Pressed(InputObject::PadIndex::First))
    {
        mIdleInputCounter = 0;
    }
    else
    {
        mIdleInputCounter++;
    }

    if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eUp)) // TODO: Input constants
    {
        if (mSelectedButtonIndex.sound_menu <= SoundOptions::eStereo_0)
        {
            mSelectedButtonIndex.sound_menu = SoundOptions::eMono_1;
        }
        else
        {
            mSelectedButtonIndex.raw--;
        }

        mButtonAnim.Set_Animation_Data(GetAnimRes(sSoundOptionsButtons[mSelectedButtonIndex.raw].animId));
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
    }

    if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eDown | InputCommands::eCheatMode))
    {
        if (mSelectedButtonIndex.sound_menu >= SoundOptions::eMono_1)
        {
            mSelectedButtonIndex.sound_menu = SoundOptions::eStereo_0;
        }
        else
        {
            mSelectedButtonIndex.raw++;
        }

        mButtonAnim.Set_Animation_Data(GetAnimRes(sSoundOptionsButtons[mSelectedButtonIndex.raw].animId));
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
    }

    if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eUnPause_OrConfirm))
    {
        if (mSelectedButtonIndex.raw)
        {
            SND_Set_Mono();
        }
        else
        {
            SND_Set_Stereo();
        }

        Mudokon_SFX(MudSounds::eOkay_13, 0, 0, nullptr);

        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Ok));
        mFnUpdate = &Menu::Options_WaitForAbeSayOK_Update;
    }

    if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eBack | InputCommands::eHop) || mIdleInputCounter > 900)
    {
        mSelectedButtonIndex.sound_menu = SoundOptions::eExit_2;
        mButtonAnim.Set_Animation_Data(GetAnimRes(sSoundOptionsButtons[2].animId));
        Mudokon_SFX(MudSounds::eOkay_13, 0, 0, nullptr);
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Ok));
        mFnUpdate = &Menu::Options_WaitForAbeSayOK_Update;
    }

    // Idle anim toggle ?
    if (((field_204_flags) >> 2) & 1)
    {
        if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
        {
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
            field_204_flags &= ~4u;
            field_1D8_timer = sGnFrame + Math_RandomRange(120, 450);
        }
    }
    else if (field_1D8_timer <= static_cast<s32>(sGnFrame))
    {
        if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
        {
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_IdleBlink));
            field_204_flags |= 4u;
        }
    }
}

void Menu::Options_WaitForAbeSayOK_Update()
{
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
        mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
        mFnUpdate = &Menu::Options_WaitForScreenTrans_Update;
    }
}

void Menu::Options_WaitForScreenTrans_Update()
{
    if (mMenuTrans)
    {
        if (mMenuTrans->field_16_bDone)
        {
            gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eOptions_2, CameraSwapEffects::eInstantChange_0, 0, 0);
            mFnUpdate = &Menu::To_MainOptions_Screen_After_Camera_Change_Update;
        }
    }
}

void Menu::To_MainOptions_Screen_After_Camera_Change_Update()
{
    if (gNumCamSwappers <= 0)
    {
        mFnUpdate = &Menu::To_Options_Update;
        mFnRender = &Menu::Options_Render;
        mSelectedButtonIndex.options_menu = OptionsMenuOptions::eSound_1;
        mButtonAnim.Set_Animation_Data(GetAnimRes(sOptionsButtons[1].animId));
        mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 0, 0, 16);
    }
}


void Menu::GameSpeak_Update()
{
    if (Input().Pressed(InputObject::PadIndex::First))
    {
        mIdleInputCounter = 0;
    }
    else
    {
        mIdleInputCounter++;
    }

    if (field_204_flags & 1)
    {
        if (mSelectedButtonIndex.gamespeak_menu == GameSpeakOptions::eChant_8 && !(sGnFrame % 8))
        {
            const FP screen_y = pScreenManager->mCamPos->y - FP_FromInteger(pScreenManager->mCamYOff);
            const FP screen_x = pScreenManager->mCamPos->x - FP_FromInteger(pScreenManager->mCamXOff);

            const FP x = screen_x + (FP_FromInteger(Math_RandomRange(-40, 40) + 184));
            const FP y = screen_y + (FP_FromInteger(162 - Math_RandomRange(30, 90)));
            auto pParticle = relive_new Particle(
                x,
                y,
                GetAnimRes(AnimId::OptionChantOrb_Particle));
            if (pParticle)
            {
                pParticle->GetAnimation().SetRenderMode(TPageAbr::eBlend_1);
                pParticle->GetAnimation().SetRenderLayer(Layer::eLayer_Above_FG1_39);
            }
        }

        if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
        {
            if (mSelectedButtonIndex.gamespeak_menu == GameSpeakOptions::eChant_8)
            {
                if (!Input_IsChanting())
                {
                    SND_Seq_Stop_477A60(SeqId::eMudokonChant1_11);

                    mMenuFade1->field_E8_bDestroyOnDone = 1;
                    mMenuFade1 = nullptr;

                    if (mMenuFade2)
                    {
                        mMenuFade2->field_E8_bDestroyOnDone = 1;
                        mMenuFade2 = nullptr;
                    }

                    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_ChantEnd));
                    mSelectedButtonIndex.gamespeak_menu = GameSpeakOptions::eHello_1;
                }
            }
            else
            {
                field_204_flags &= ~1u;
                if (mSelectedButtonIndex.gamespeak_menu == GameSpeakOptions::eLaugh_6 || mSelectedButtonIndex.gamespeak_menu == GameSpeakOptions::eHello_1)
                {
                    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_IdleBlink));
                    GetAnimation().SetFrame(7u);
                }
                else
                {
                    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
                }
            }
        }
        CycleGameSpeakIdleAnims();
        return;
    }

    if (Input_IsChanting())
    {
        SND_SEQ_PlaySeq_4775A0(SeqId::eMudokonChant1_11, 0, 1);

        field_204_flags |= 1u;
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Chant));
        mSelectedButtonIndex.gamespeak_menu = GameSpeakOptions::eChant_8;

        if (!mMenuFade1)
        {
            if (Input_JoyStickEnabled())
            {
                mMenuFade1 = relive_new MainMenuFade(sGameSpeakButtons[8].xpos, sGameSpeakButtons[8].ypos + 36, buttonType::eCircle_0, 0);
            }
            else
            {
                mMenuFade1 = relive_new MainMenuFade(181, sGameSpeakButtons[8].ypos + 36, buttonType::eCircle_0, 0);
            }
        }

        if (mMenuFade2)
        {
            mMenuFade2->field_E8_bDestroyOnDone = 1;
        }

        if (Input_JoyStickEnabled())
        {
            mMenuFade2 = relive_new MainMenuFade(sGameSpeakButtons[11].xpos, sGameSpeakButtons[11].ypos + 36, buttonType::eCircle_0, 0);
        }
        else
        {
            mMenuFade2 = nullptr;
        }
        CycleGameSpeakIdleAnims();
        return;
    }

    if (Input().IsAnyPressed(InputObject::PadIndex::First, sInputKey_LeftGameSpeakEnabler))
    {
        if (mMenuFade2)
        {
            const FP xpos = FP_FromInteger(sGameSpeakButtons[10].xpos);
            if (mMenuFade2->mXPos != xpos)
            {
                mMenuFade2->mXPos = xpos;
                mMenuFade2->mYPos = FP_FromInteger(sGameSpeakButtons[10].ypos + 36);
            }
        }
        else if (Input_JoyStickEnabled())
        {
            mMenuFade2 = relive_new MainMenuFade(sGameSpeakButtons[10].xpos, sGameSpeakButtons[10].ypos + 36, buttonType::eCircle_0, 0);
        }

        if (Input().IsAnyHeld(InputObject::PadIndex::First, sInputKey_GameSpeak2))
        {
            Mudokon_SFX(MudSounds::eFollowMe_4, 0, 0, 0);
            field_204_flags |= 1u;
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_FollowMe));
            mSelectedButtonIndex.gamespeak_menu = GameSpeakOptions::eFollowMe_2;
            relive_new MainMenuFade(sGameSpeakButtons[2].xpos, sGameSpeakButtons[2].ypos + 36, buttonType::eCircle_0, 1);
        }
        else if (Input().IsAnyHeld(InputObject::PadIndex::First, sInputKey_GameSpeak4))
        {
            Mudokon_SFX(MudSounds::eWait_6, 0, 0, 0);
            field_204_flags |= 1u;
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Wait));
            mSelectedButtonIndex.gamespeak_menu = GameSpeakOptions::eWait_0;
            relive_new MainMenuFade(sGameSpeakButtons[0].xpos, sGameSpeakButtons[0].ypos + 36, buttonType::eCircle_0, 1);
        }
        else if (Input().IsAnyHeld(InputObject::PadIndex::First, sInputKey_GameSpeak1))
        {
            Mudokon_SFX(MudSounds::eHello_3, 0, 0, 0);
            field_204_flags |= 1u;
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Hello));
            mSelectedButtonIndex.gamespeak_menu = GameSpeakOptions::eHello_1;
            relive_new MainMenuFade(sGameSpeakButtons[1].xpos, sGameSpeakButtons[1].ypos + 36, buttonType::eCircle_0, 1);
        }
        else if (Input().IsAnyPressed(InputObject::PadIndex::First, sInputKey_GameSpeak3))
        {
            Mudokon_SFX(MudSounds::eAngry_5, 0, 0, 0);
            field_204_flags |= 1u;
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Anger));
            mSelectedButtonIndex.gamespeak_menu = GameSpeakOptions::eAngry_3;
            relive_new MainMenuFade(sGameSpeakButtons[3].xpos, sGameSpeakButtons[3].ypos + 36, buttonType::eCircle_0, 1);
        }
        CycleGameSpeakIdleAnims();
        return;
    }

    if (Input().IsAnyPressed(InputObject::PadIndex::First, sInputKey_RightGameSpeakEnabler))
    {
        if (mMenuFade2)
        {
            const FP xpos = FP_FromInteger(sGameSpeakButtons[12].xpos);
            if (mMenuFade2->mXPos != xpos)
            {
                mMenuFade2->mXPos = xpos;
                mMenuFade2->mYPos = FP_FromInteger(sGameSpeakButtons[12].ypos + 36);
            }
        }
        else if (Input_JoyStickEnabled())
        {
            mMenuFade2 = relive_new MainMenuFade(sGameSpeakButtons[12].xpos, sGameSpeakButtons[12].ypos + 36, buttonType::eCircle_0, 0);
        }

        if (Input().IsAnyHeld(InputObject::PadIndex::First, sInputKey_GameSpeak6))
        {
            Mudokon_SFX(MudSounds::eWhistleHigh_1, 0, 0, 0);
            field_204_flags |= 1u;
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_WhistleHigh));
            mSelectedButtonIndex.gamespeak_menu = GameSpeakOptions::eWhistleHigh_4;
            relive_new MainMenuFade(sGameSpeakButtons[4].xpos, sGameSpeakButtons[4].ypos + 36, buttonType::eCircle_0, 1);
        }
        else if (Input().IsAnyHeld(InputObject::PadIndex::First, sInputKey_GameSpeak5))
        {
            Mudokon_SFX(MudSounds::eWhistleLow_2, 0, 0, 0);
            field_204_flags |= 1u;
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_WhistleLow));
            mSelectedButtonIndex.gamespeak_menu = GameSpeakOptions::eWhistleLow_5;
            relive_new MainMenuFade(sGameSpeakButtons[5].xpos, sGameSpeakButtons[5].ypos + 36, buttonType::eCircle_0, 1);
        }
        else if (Input().IsAnyHeld(InputObject::PadIndex::First, sInputKey_GameSpeak8))
        {
            Mudokon_SFX(MudSounds::eLaugh2_11, 0, 0, 0);
            field_204_flags |= 1u;
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Laugh));
            mSelectedButtonIndex.gamespeak_menu = GameSpeakOptions::eLaugh_6;
            relive_new MainMenuFade(sGameSpeakButtons[6].xpos, sGameSpeakButtons[6].ypos + 36, buttonType::eCircle_0, 1);
        }
        else if (Input().IsAnyHeld(InputObject::PadIndex::First, sInputKey_GameSpeak7))
        {
            Mudokon_SFX(MudSounds::eFart_7, 0, 0, 0);
            field_204_flags |= 1u;
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Fart));
            mSelectedButtonIndex.gamespeak_menu = GameSpeakOptions::eFart_7;
            relive_new MainMenuFade(sGameSpeakButtons[7].xpos, sGameSpeakButtons[7].ypos + 36, buttonType::eCircle_0, 1);
        }
        CycleGameSpeakIdleAnims();
        return;
    }

    if (!Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eBack) && mIdleInputCounter <= 1600)
    {
        if (mMenuFade2)
        {
            mMenuFade2->field_E8_bDestroyOnDone = 1;
            mMenuFade2 = nullptr;
        }
        CycleGameSpeakIdleAnims();
        return;
    }

    Mudokon_SFX(MudSounds::eGoodbye_12, 0, 0, 0);

    field_204_flags |= 1u;
    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Goodbye));
    mSelectedButtonIndex.gamespeak_menu = GameSpeakOptions::eMainMenu_9;

    relive_new MainMenuFade(sGameSpeakButtons[9].xpos, sGameSpeakButtons[9].ypos + 36, buttonType::eCircle_0, 1);

    mFnUpdate = &Menu::GameSpeakBack_WaitForAbeGoodbye_Update;

    if (mMenuFade1)
    {
        mMenuFade1->field_E8_bDestroyOnDone = 1;
        mMenuFade1 = nullptr;
    }

    if (mMenuFade2)
    {
        mMenuFade2->field_E8_bDestroyOnDone = 1;
        mMenuFade2 = nullptr;
    }

    CycleGameSpeakIdleAnims();
}

void Menu::CycleGameSpeakIdleAnims()
{
    if (!(field_204_flags & 1))
    {
        if ((field_204_flags >> 2) & 1)
        {
            if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
            {
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
                field_204_flags &= ~4u;
                field_1D8_timer = sGnFrame + Math_RandomRange(120, 450);
            }
        }
        else
        {
            if (field_1D8_timer <= static_cast<s32>(sGnFrame))
            {
                if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
                {
                    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_IdleBlink));
                    field_204_flags |= 4u;
                }
            }
        }
    }
}

void Menu::FMV_Or_Level_Select_To_Back_Update()
{
    if (mMenuTrans->field_16_bDone)
    {
        gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eMainMenu_1, CameraSwapEffects::eInstantChange_0, 0, 0);
        mFnUpdate = &Menu::FMV_Or_Level_Select_Back_Update;
    }
}

void Menu::To_Credits_Update()
{
    mFnUpdate = &Menu::Credits_Update;
    mFnRender = &Menu::Empty_Render;
    field_1D8_timer = sGnFrame + 60;
}

void Menu::Level_Cheat_To_Loading_Update()
{
    if (mMenuTrans->field_16_bDone)
    {
        mFnUpdate = &Menu::ToLoading;
    }
}

void Menu::Options_Controller_Update()
{
    if (Input().IsAnyPressed(InputObject::PadIndex::First, InputCommands::eUp))
    {
        if (mSelectedButtonIndex.raw > 0 && field_228 == FP_FromInteger(0))
        {
            mSelectedButtonIndex.raw--;
            SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
        }
    }
    else if (Input().IsAnyPressed(InputObject::PadIndex::First, InputCommands::eDown | InputCommands::eCheatMode))
    {
        if (mSelectedButtonIndex.raw < sAvailableControllers - 1 && field_228 == FP_FromInteger(0))
        {
            mSelectedButtonIndex.raw++;
            SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
        }
    }

    if (Input().IsAnyPressed(InputObject::PadIndex::First, InputCommands::eBack | InputCommands::eHop))
    {
        field_230_bGoBack = 1;
        mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
        mFnUpdate = &Menu::GoTo_ControllerConfigure_Or_Back_AfterScreenTrans_Update;
    }

    if (Input().IsAnyHeld(InputObject::PadIndex::First, (InputCommands::eThrowItem | InputCommands::eUnPause_OrConfirm | InputCommands::eDoAction)))
    {
        field_230_bGoBack = 0;
        mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
        mFnUpdate = &Menu::GoTo_ControllerConfigure_Or_Back_AfterScreenTrans_Update;
    }
}

void Menu::GoTo_ControllerConfigure_Or_Back_AfterScreenTrans_Update()
{
    if (mMenuTrans)
    {
        if (mMenuTrans->field_16_bDone)
        {
            switch (field_230_bGoBack)
            {
                // Goto controller configuration
                case 0:
                    gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eControllerConfig_41, CameraSwapEffects::eInstantChange_0, 0, 0);
                    break;

                // Back to main options (sound/controller)
                case 1:
                    gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eOptions_2, CameraSwapEffects::eInstantChange_0, 0, 0);
                    break;
            }

            mFnUpdate = &Menu::Goto_ConfigureController_OrSave_SettingIni_Update;
        }
    }
}

void Menu::Goto_ConfigureController_OrSave_SettingIni_Update()
{
    if (gNumCamSwappers <= 0)
    {
        switch (field_230_bGoBack)
        {
            // Goto controller configuration
            case 0:
                field_204_flags &= ~2u;
                mFnUpdate = &Menu::To_ButtonRemap_Update;
                mFnRender = &Menu::ButtonRemap_Render;
                mSelectedButtonIndex.remap_menu = RemapOptions::eRun_0;
                field_230_bGoBack = -1;
                break;

            // Back to main options (sound/controller)
            case 1:
            {
                Input_SaveSettingsIni();
                mFnUpdate = &Menu::To_Options_Update;
                mFnRender = &Menu::Options_Render;
                mSelectedButtonIndex.options_menu = OptionsMenuOptions::eController_0;
                mButtonAnim.Set_Animation_Data(GetAnimRes(sMainScreenButtons[0].animId));
                field_204_flags |= 2u;
                break;
            }

        }

        mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 0, 0, 16);
    }
}

const MenuButtonInputInfo chooseAndExitRemapButtons_4D0690[2] = {
    {40, 34, InputCommands::eUnPause_OrConfirm},
    {301, 163, InputCommands::eBack}};

static const MenuButtonAnimInfo sRemapScreenButtons[10] = {
    {172, 78, AnimId::MenuHighlight_ButtonRemapSquare},  // Run
    {172, 109, AnimId::MenuHighlight_ButtonRemapSquare}, // Sneak
    {172, 138, AnimId::MenuHighlight_ButtonRemapSquare}, // Jump
    {172, 169, AnimId::MenuHighlight_ButtonRemapSquare}, // Speak 1
    {258, 77, AnimId::MenuHighlight_ButtonRemapSquare},  // Action
    {258, 109, AnimId::MenuHighlight_ButtonRemapSquare}, // Throw
    {258, 138, AnimId::MenuHighlight_ButtonRemapSquare}, // Crouch
    {258, 169, AnimId::MenuHighlight_ButtonRemapSquare}, // Speak 2
    {41, 69, AnimId::MenuHighlight_Circle},  // Choose button
    {302, 199, AnimId::MenuHighlight_Circle} // Exit
};

const char_type* inputActions_4D0070[8] = {
    "Run", "Sneak", "Jump", "Speak 1", "Action", "Throw", "Crouch", "Speak 2"};


void Menu::ButtonRemap_Render(PrimHeader** ppOt)
{
    AnimId animId = AnimId::None;
    if (field_230_bGoBack == -1)
    {
        animId = sRemapScreenButtons[mSelectedButtonIndex.raw].animId;
    }
    else
    {
        animId = sRemapScreenButtons[field_230_bGoBack].animId;
    }
    mButtonAnim.Set_Animation_Data(GetAnimRes(animId));

    s32 polyOffset = 0;
    for (s32 i = 0; i < ALIVE_COUNTOF(sRemapScreenButtons) - 2; i++)
    {
        RenderElement(
            sRemapScreenButtons[i].xpos,
            sRemapScreenButtons[i].ypos,
            sRemappableInputs.buttons[Input_JoyStickEnabled() != 0][i],
            ppOt,
            &mFont,
            &polyOffset);
    }

    if (field_230_bGoBack == -1)
    {
        mButtonAnim.VRender(
            sRemapScreenButtons[mSelectedButtonIndex.raw].xpos - 3,
            sRemapScreenButtons[mSelectedButtonIndex.raw].ypos + 1,
            ppOt,
            0,
            0);
    }
    else
    {
        mButtonAnim.VRender(
            sRemapScreenButtons[field_230_bGoBack].xpos,
            sRemapScreenButtons[field_230_bGoBack].ypos,
            ppOt,
            0,
            0);
    }
    if (field_230_bGoBack == 8)
    {
        const s32 maxFontWidth = 336;
        if (Input_JoyStickEnabled())
        {
            field_1F4_text = "Press button to use";
        }
        else
        {
            field_1F4_text = "Press key to use";
        }
        s32 fontWidth = mFont.MeasureScaledTextWidth(field_1F4_text, FP_FromInteger(1));
        s16 calculatedXposBasedOnWidth = 0;
        if (fontWidth >= maxFontWidth)
        {
            calculatedXposBasedOnWidth = 16;
        }
        else
        {
            calculatedXposBasedOnWidth = static_cast<s16>((368 - fontWidth) / 2);
        }
        s32 drawnStringOffset = mFont.DrawString(
            ppOt,
            field_1F4_text,
            calculatedXposBasedOnWidth,
            88,
            TPageAbr::eBlend_0,
            1,
            0,
            Layer::eLayer_Menu_41,
            40,
            20,
            0,
            polyOffset,
            FP_FromInteger(1),
            640,
            0);
        polyOffset = drawnStringOffset;

        char_type buffer[40] = {};
        sprintf(buffer, "for %s", inputActions_4D0070[mSelectedButtonIndex.raw]);
        field_1F4_text = buffer;
        auto fontWidth2 = mFont.MeasureScaledTextWidth(buffer, FP_FromInteger(1));
        s16 calculatedXposBasedOnWidth2 = 0;
        if (fontWidth2 >= maxFontWidth)
        {
            calculatedXposBasedOnWidth2 = 16;
        }
        else
        {
            calculatedXposBasedOnWidth2 = static_cast<s16>((368 - fontWidth2) / 2);
        }
        polyOffset = mFont.DrawString(
            ppOt,
            field_1F4_text,
            calculatedXposBasedOnWidth2,
            120,
            TPageAbr::eBlend_0,
            1,
            0,
            Layer::eLayer_Menu_41,
            40,
            20,
            0,
            polyOffset,
            FP_FromInteger(1),
            640,
            0);
        field_1F4_text = "or Esc for none";
        auto fontWidth3 = mFont.MeasureScaledTextWidth("or Esc for none", FP_FromInteger(1));
        s16 calculatedXposBasedOnWidth3 = 0;
        if (fontWidth3 >= maxFontWidth)
        {
            calculatedXposBasedOnWidth3 = 16;
        }
        else
        {
            calculatedXposBasedOnWidth3 = static_cast<s16>((368 - fontWidth3) / 2);
        }
        polyOffset = mFont.DrawString(
            ppOt,
            field_1F4_text,
            calculatedXposBasedOnWidth3,
            152,
            TPageAbr::eBlend_0,
            1,
            0,
            Layer::eLayer_Menu_41,
            40,
            20,
            0,
            polyOffset,
            FP_FromInteger(1),
            640,
            0);
    }

    PSX_RECT pRect = {};
    mButtonAnim.Get_Frame_Rect(&pRect);

    for (s32 i = 0; i < ALIVE_COUNTOF(chooseAndExitRemapButtons_4D0690); i++)
    {
        RenderElement(
            chooseAndExitRemapButtons_4D0690[i].xpos,
            chooseAndExitRemapButtons_4D0690[i].ypos,
            chooseAndExitRemapButtons_4D0690[i].inputCommand,
            ppOt,
            &mFont,
            &polyOffset);
    }
}

void Menu::To_ButtonRemap_Update()
{
    field_230_bGoBack = -1;

    if (mMenuTrans)
    {
        if (mMenuTrans->field_16_bDone)
        {
            mFnUpdate = &Menu::ButtonRemap_Update;
            mFnRender = &Menu::ButtonRemap_Render;
            mIdleInputCounter = 0;
        }
    }
}

void Menu::ButtonRemap_Update()
{
    if (bWaitingForRemapInput_9F2DE8)
    {
        if (!Input().Pressed(InputObject::PadIndex::First))
        {
            bWaitingForRemapInput_9F2DE8 = 0;
        }
        return;
    }

    if (field_230_bGoBack == 8)
    {
        if (!Input_Remap(static_cast<InputCommands>(sRemappableInputs.buttons[Input_JoyStickEnabled() ? 1 : 0][mSelectedButtonIndex.raw])))
        {
            return;
        }

        field_230_bGoBack = -1;
        mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 0, 0, 16);
        bWaitingForRemapInput_9F2DE8 = 1;
        return;
    }

    if (Input().IsAnyPressed(InputObject::PadIndex::First, InputCommands::eLeft))
    {
        if (mSelectedButtonIndex.remap_menu >= RemapOptions::eAction_4)
        {
            mSelectedButtonIndex.raw -= 4;
        }
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400); // TODO: Input constants
        bWaitingForRemapInput_9F2DE8 = 1;
    }
    else if (Input().IsAnyPressed(InputObject::PadIndex::First, InputCommands::eRight))
    {
        if (mSelectedButtonIndex.remap_menu < RemapOptions::eAction_4)
        {
            mSelectedButtonIndex.raw += 4;
        }
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400); // TODO: Input constants
        bWaitingForRemapInput_9F2DE8 = 1;
    }
    else if (Input().IsAnyPressed(InputObject::PadIndex::First, InputCommands::eUp))
    {
        mSelectedButtonIndex.raw--;
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400); // TODO: Input constants
        bWaitingForRemapInput_9F2DE8 = 1;

#if MISC_PC_MENU_FIXES // OG Change - Prevent users from changing Speak1 and Speak2 keys on keyboard
        if (!Input_JoyStickEnabled())
        {
            if (mSelectedButtonIndex.remap_menu < RemapOptions::eRun_0)
            {
                mSelectedButtonIndex.remap_menu = RemapOptions::eCrouch_6;
            }
            else if (mSelectedButtonIndex.remap_menu == RemapOptions::eSpeakLeft_3 || mSelectedButtonIndex.remap_menu == RemapOptions::eSpeakRight_7)
            {
                mSelectedButtonIndex.raw--;
            }
        }
#endif
    }
    else if (Input().IsAnyPressed(InputObject::PadIndex::First, InputCommands::eDown | InputCommands::eCheatMode))
    {
        mSelectedButtonIndex.raw++;
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400); // TODO: Input constants
        bWaitingForRemapInput_9F2DE8 = 1;

#if MISC_PC_MENU_FIXES // OG Change - Prevent users from changing Speak1 and Speak2 keys on keyboard
        if (!Input_JoyStickEnabled())
        {
            if (mSelectedButtonIndex.remap_menu == RemapOptions::eSpeakLeft_3 || mSelectedButtonIndex.remap_menu == RemapOptions::eSpeakRight_7)
            {
                mSelectedButtonIndex.raw++;
            }
        }
#endif
    }

    if (mSelectedButtonIndex.remap_menu < RemapOptions::eRun_0)
    {
        mSelectedButtonIndex.remap_menu = RemapOptions::eSpeakRight_7;
    }

    if (mSelectedButtonIndex.remap_menu > RemapOptions::eSpeakRight_7)
    {
        mSelectedButtonIndex.remap_menu = RemapOptions::eRun_0;
    }

    if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eBack | InputCommands::eHop))
    {
        // Show abe motions screen
        mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
        field_230_bGoBack = 9;
        mFnUpdate = &Menu::To_ShowAbeMotions_ChangeCamera_Update;
    }

    if (Input().IsAnyHeld(InputObject::PadIndex::First, (InputCommands::eThrowItem | InputCommands::eUnPause_OrConfirm | InputCommands::eDoAction)))
    {
        // Rebind a key (in that horrible white blinding screen)
        mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
        field_230_bGoBack = 8;
        bWaitingForRemapInput_9F2DE8 = 1;
    }
}

void Menu::To_LoadSave_Update()
{
    if (mMenuTrans)
    {
        if (mMenuTrans->field_16_bDone)
        {
            mFnUpdate = &Menu::LoadSave_Update;
            mFnRender = &Menu::Empty_Render;
        }
    }
}

void Menu::LoadSave_Update()
{
    if (mMenuTrans)
    {
        mMenuTrans->mBaseGameObjectRefCount--;
        mMenuTrans->mBaseGameObjectFlags.Set(Options::eDead);
        mMenuTrans = nullptr;
    }

    /*
    if (!field_E4_res_array[0])
    {
        ProgressInProgressFilesLoading();
    }*/

    if (!gPauseMenu)
    {
        gPauseMenu = relive_new PauseMenu();
    }


    if (!sActiveHero)
    {
        sActiveHero = relive_new Abe();
    }

    if (!SaveGame::LoadFromFile(sSaveNames_9F1DD8[mSelectedButtonIndex.raw].field_0_mName))
    {
        mFnUpdate = &Menu::SaveLoadFailed_Update;
        mFnRender = &Menu::SaveLoadFailed_Render;
        sActiveHero->mBaseGameObjectFlags.Set(Options::eDead);
    }
}

void Menu::SaveLoadFailed_Update()
{
    // Kill the pause menu and stay in this state - have to force restart the game when a save fails to load :)
    if (gPauseMenu)
    {
        gPauseMenu->mBaseGameObjectFlags.Set(Options::eDead);
        gPauseMenu = nullptr;
    }
}

void Menu::SaveLoadFailed_Render(PrimHeader** ppOt)
{
    // Note: This string in OG was just "Error" which is completely useless, changed to at least
    // give people a clue about what broke.
    const char_type* kErrStr = "Error loading save file";

    s16 xpos = 16;
    const s32 drawWidth = mFont.DrawString(
        ppOt,
        kErrStr,
        xpos,
        210,
        TPageAbr::eBlend_0,
        1,
        0,
        Layer::eLayer_FG1_37,
        210,
        150,
        80,
        0,
        FP_FromInteger(1),
        640,
        0);

    mFont.DrawString(
        ppOt,
        kErrStr,
        xpos + 2,
        212,
        TPageAbr::eBlend_0,
        1,
        0,
        Layer::eLayer_FG1_37,
        0,
        0,
        0,
        drawWidth,
        FP_FromInteger(1),
        640,
        0);
}

void Menu::To_ShowAbeMotions_ChangeCamera_Update()
{
    if (mMenuTrans)
    {
        if (mMenuTrans->field_16_bDone)
        {
            if (field_230_bGoBack == 9)
            {
                gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eMotions_4, CameraSwapEffects::eInstantChange_0, 0, 0);
                mFnUpdate = &Menu::To_ShowAbeMotions_SaveSettings_Update;
            }
        }
    }
}

void Menu::To_ShowAbeMotions_SaveSettings_Update()
{
    if (gNumCamSwappers <= 0)
    {
        mButtonAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuHighlight_Circle));
        Input_SaveSettingsIni();
        field_204_flags &= ~2u;
        mFnUpdate = &Menu::To_ToggleMotions_Update;
        mFnRender = &Menu::ToggleMotions_Render;
        mSelectedButtonIndex.motions_menu = MotionsOptions::eMotions_0;
        mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 0, 0, 16);
    }
}

void Menu::To_ToggleMotions_Update()
{
    if (mMenuTrans)
    {
        if (mMenuTrans->field_16_bDone)
        {
            mFnUpdate = &Menu::ToggleMotions_Update;
            mIdleInputCounter = 0;
        }
    }
}

void Menu::Credits_Update()
{
    if (field_1D8_timer <= static_cast<s32>(sGnFrame))
    {
        mCurrentCreditsCamera++;

        if (mCurrentCreditsCamera > 24)
        {
            // Credits done
            gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eFmvSelect_30, CameraSwapEffects::eInstantChange_0, 0, 0);
            mFnUpdate = &Menu::CreditsEnd_BackTo_FMV_Or_Level_List_Update;
            gCreditsControllerExists = 0;
        }
        else
        {
            // Next credits screen
            gMap.SetActiveCam(EReliveLevelIds::eCredits, 1, static_cast<s16>(mCurrentCreditsCamera), CameraSwapEffects::eTopToBottom_3, 0, 0);
            field_1D8_timer = sGnFrame + 60;
        }
    }
}

void Menu::CreditsEnd_BackTo_FMV_Or_Level_List_Update()
{
    mFnUpdate = &Menu::FMV_Select_Update;
    mFnRender = &Menu::FMV_Or_Level_Select_Render;
}

void Menu::Load_Update()
{
    if (Input().Pressed(InputObject::PadIndex::First))
    {
        mIdleInputCounter = 0;
    }
    else
    {
        mIdleInputCounter++;
    }

    if ((Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eBack | InputCommands::eHop))
        || mIdleInputCounter > 1000)
    {
#if !MISC_PC_MENU_FIXES // OG Change - Fixes automatic scroll when exiting Load Menu
        field_1E0_selected_index.raw = 1; // This line causes the issue
#endif
        mButtonAnim.Set_Animation_Data(GetAnimRes(sLoadButtons[1].animId));
        mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
        mFnUpdate = &Menu::Load_BackToMainScreen_Update;
        mLoadSave = false;
    }

    if (Input().IsAnyPressed(InputObject::PadIndex::First, InputCommands::eUp))
    {
        if (mSelectedButtonIndex.raw > 0 && field_228 == FP_FromInteger(0))
        {
            mSelectedButtonIndex.raw--;

            SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);

            if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eBack | InputCommands::eHop))
            {
                field_230_bGoBack = 1;
                mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
                mFnUpdate = &Menu::Load_BackToMainScreen_Update;
                mLoadSave = false;
                return;
            }
        }
    }
    else
    {
        if (Input().IsAnyPressed(InputObject::PadIndex::First, InputCommands::eDown | InputCommands::eCheatMode))
        {
            if (mSelectedButtonIndex.raw < (sSaveIdx - 1) && field_228 == FP_FromInteger(0))
            {
                mSelectedButtonIndex.raw++;

                SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);

                if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eBack | InputCommands::eHop))
                {
                    field_230_bGoBack = 1;
                    mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
                    mFnUpdate = &Menu::Load_BackToMainScreen_Update;
                    mLoadSave = false;
                    return;
                }
            }
        }
    }

    if (Input().IsAnyHeld(InputObject::PadIndex::First, (InputCommands::eThrowItem | InputCommands::eUnPause_OrConfirm | InputCommands::eDoAction)))
    {
        if (sSaveIdx)
        {
            field_230_bGoBack = 0;
            mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
            mFnUpdate = &Menu::Load_BackToMainScreen_Update;
            mLoadSave = true;
        }
        else
        {
            field_230_bGoBack = 1;
            mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
            mFnUpdate = &Menu::Load_BackToMainScreen_Update;
            mLoadSave = false;
        }
    }
}

s32 Menu::StringsEqual(const void* pStr1, const void* pStr2)
{
    return _strcmpi(static_cast<const char_type*>(pStr1), static_cast<const char_type*>(pStr2));
}

void Menu::ToggleMotions_Render(PrimHeader** ppOt)
{
    mButtonAnim.VRender(
        sAbeMotionsButtons[mSelectedButtonIndex.raw].xpos,
        sAbeMotionsButtons[mSelectedButtonIndex.raw].ypos,
        ppOt,
        0,
        0);

    if (mFnUpdate == &Menu::ToggleMotions_Update)
    {
        s32 polyOffset = 0;
        for (s32 i = 2; i <= 10; i++)
        {
            RenderElement(sAbeMotionsInputs[i].xpos, sAbeMotionsInputs[i].ypos, sAbeMotionsInputs[i].inputCommand, ppOt, &mFont, &polyOffset);
        }
    }
    else if (mFnUpdate == &Menu::Toggle_Motions_Screens_Update)
    {
        s32 polyOffset = 0;
        if (Input_JoyStickEnabled())
        {
            for (s32 i = 0; i < 20; i++)
            {
                RenderElement(
                    sGameSpeakInputs[i].xpos,
                    sGameSpeakInputs[i].ypos,
                    sGameSpeakInputs[i].inputCommand,
                    ppOt,
                    &mFont,
                    &polyOffset);
            }
        }
        else
        {
            for (s32 i = 9; i < 11; i++)
            {
                RenderElement(sAbeMotionsInputs[i].xpos, sAbeMotionsInputs[i].ypos, sAbeMotionsInputs[i].inputCommand, ppOt, &mFont, &polyOffset);
            }
        }
    }

    PSX_RECT rect = {};
    mButtonAnim.Get_Frame_Rect(&rect);
}

void Menu::ToggleMotions_Update()
{
    if (Input().Pressed(InputObject::PadIndex::First))
    {
        mIdleInputCounter = 0;
    }
    else
    {
        mIdleInputCounter++;
    }

    if (gNumCamSwappers <= 0)
    {
        if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eUnPause_OrConfirm | InputCommands::eDoAction | InputCommands::eCheatMode))
        {
            if (Input_JoyStickEnabled())
            {
                gMap.SetActiveCameraDelayed(MapDirections::eMapBottom_3, 0, -1);
            }
            else
            {
                gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eMotionsGamespeakKeyboard_37, CameraSwapEffects::eTopToBottom_3, 0, 0);
            }

            // Go to game speak toggle
            mFnUpdate = &Menu::Toggle_Motions_Screens_Update;
            mSelectedButtonIndex.motions_menu = MotionsOptions::eGameSpeak_1;
            PSX_Prevent_Rendering_4945B0();
            SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
        }

        if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eBack | InputCommands::eHop) || mIdleInputCounter > 1600)
        {
            // Back to options
            mSelectedButtonIndex.motions_menu = MotionsOptions::eExit_2;
            mButtonAnim.Set_Animation_Data(GetAnimRes(sAbeMotionsButtons[2].animId));
            mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
            mFnUpdate = &Menu::MotionsScreen_Back_Update;
        }
    }
}

void Menu::Toggle_Motions_Screens_Update()
{
    if (Input().Pressed(InputObject::PadIndex::First))
    {
        mIdleInputCounter = 0;
    }
    else
    {
        mIdleInputCounter++;
    }

    if (gNumCamSwappers <= 0)
    {
        if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eUnPause_OrConfirm | InputCommands::eDoAction | InputCommands::eCheatMode))
        {
            if (Input_JoyStickEnabled())
            {
                gMap.SetActiveCameraDelayed(MapDirections::eMapTop_2, 0, -1);
            }
            else
            {
                gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eMotions_4, CameraSwapEffects::eBottomToTop_4, 0, 0);
            }

            mFnUpdate = &Menu::ToggleMotions_Update;
            mSelectedButtonIndex.motions_menu = MotionsOptions::eMotions_0;
            PSX_Prevent_Rendering_4945B0();
            SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
        }

        if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eBack | InputCommands::eHop) || mIdleInputCounter > 1600)
        {
            mSelectedButtonIndex.motions_menu = MotionsOptions::eExit_2;
            mButtonAnim.Set_Animation_Data(GetAnimRes(sAbeMotionsButtons[2].animId));
            mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
            mFnUpdate = &Menu::MotionsScreen_Back_Update;
        }
    }
}

void Menu::MotionsScreen_Back_Update()
{
    if (mMenuTrans)
    {
        if (mMenuTrans->field_16_bDone)
        {
            gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eOptions_2, CameraSwapEffects::eInstantChange_0, 0, 0);
            mFnUpdate = &Menu::Motions_ToOptions_Update;
        }
    }
}

void Menu::Motions_ToOptions_Update()
{
    if (gNumCamSwappers <= 0)
    {
        field_204_flags |= 2u;
        mFnUpdate = &Menu::To_Options_Update;
        mFnRender = &Menu::Options_Render;
        mSelectedButtonIndex.options_menu = OptionsMenuOptions::eController_0;
        mButtonAnim.Set_Animation_Data(GetAnimRes(sOptionsButtons[0].animId));
        mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 0, 0, 16);
    }
}

void Menu::To_MainScreenOrLoad_Update()
{
    if (gNumCamSwappers <= 0)
    {
        mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 0, 0, 16);
        if (mLoadSave)
        {
            mFnUpdate = &Menu::To_LoadSave_Update;
            mFnRender = &Menu::Empty_Render;
        }
        else
        {
            mSelectedButtonIndex.mainmenu = MainMenuOptions::eLoad_3;
            mButtonAnim.Set_Animation_Data(GetAnimRes(sMainScreenButtons[3].animId));
            field_204_flags |= 2u;
            mFnUpdate = &Menu::To_MainScreen_Update;
            mFnRender = &Menu::MainScreen_Render;
        }
    }
}

void Menu::Load_BackToMainScreen_Update()
{
    if (mMenuTrans)
    {
        if (mMenuTrans->field_16_bDone)
        {
            if (mLoadSave == false)
            {
                gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eMainMenu_1, CameraSwapEffects::eInstantChange_0, 0, 0);
            }
            else
            {
                gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eLoading_21, CameraSwapEffects::eInstantChange_0, 0, 0);
            }
            mFnUpdate = &Menu::To_MainScreenOrLoad_Update;
        }
    }
}

void Menu::GameSpeakBack_WaitForAbeGoodbye_Update()
{
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
        mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
        mFnUpdate = &Menu::GamespeakBack_WaitForScreenTrans_Update;
    }
}

void Menu::GamespeakBack_WaitForScreenTrans_Update()
{
    if (mMenuTrans)
    {
        if (mMenuTrans->field_16_bDone)
        {
            field_204_flags &= ~1u;
            gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eMainMenu_1, CameraSwapEffects::eInstantChange_0, 0, 0);
            mFnUpdate = &Menu::GameSpeak_To_MainScreen_Update;
        }
    }
}

void Menu::GameSpeak_To_MainScreen_Update()
{
    if (gNumCamSwappers <= 0)
    {
        mFnUpdate = &Menu::To_MainScreen_Update;
        mFnRender = &Menu::MainScreen_Render;
        mSelectedButtonIndex.mainmenu = MainMenuOptions::eGameSpeak_0;
        mButtonAnim.Set_Animation_Data(GetAnimRes(sMainScreenButtons[0].animId));
        mMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 0, 0, 16);
        mButtonAnim.LoadPal(GetPalRes(PalId::WhiteHighlite));
    }
}

void Menu::OnResourceLoaded(Menu* /*pMenu*/)
{
    
}

void Menu::RenderElement(s32 xpos, s32 ypos, s32 input_command, PrimHeader** ot, AliveFont* pFont, s32* pPolyOffset)
{
    char_type text[32] = {};
    strcpy(text, Input_GetButtonString(static_cast<InputCommands>(input_command), false)); // TODO: Strongly type all the way back to the button structure
    const FP scale_fp = strlen(text) > 1 ? FP_FromDouble(0.64) : FP_FromDouble(0.84);

    if (text[0])
    {
        char_type* pTextIter = &text[0];
        do
        {
            *pTextIter = static_cast<char_type>(tolower(*pTextIter));
            pTextIter++;
        }
        while (*pTextIter);
    }

    if (text[0] < ' ')
    {
        // NOTE: diversion from OG!
        // Used to be:
        // text[0] += 'Y';
        text[0] = '-';
    }

    const s32 text_width = pFont->MeasureScaledTextWidth(text, scale_fp);
    const s16 text_y = static_cast<s16>(ypos + FP_GetExponent((FP_FromInteger(-9) * scale_fp)) + 1);
    const s16 converted_x = static_cast<s16>(PsxToPCX(xpos - text_width / 2, 11));

    const u8 bOldValue = sFontDrawScreenSpace;
    sFontDrawScreenSpace = 1;

    s32 offset = pFont->DrawString(
        ot,
        text,
        converted_x,
        text_y,
        TPageAbr::eBlend_0,
        1,
        0,
        Layer::eLayer_Above_FG1_39,
        0,
        0,
        0,
        *pPolyOffset,
        scale_fp,
        640,
        0);
    *pPolyOffset = offset;

    offset = pFont->DrawString(
        ot,
        text,
        converted_x - 1,
        text_y,
        TPageAbr::eBlend_0,
        1,
        0,
        Layer::eLayer_Above_FG1_39,
        58,
        40,
        41,
        offset,
        scale_fp,
        640,
        0);
    *pPolyOffset = offset;

    *pPolyOffset = pFont->DrawString(
        ot,
        text,
        converted_x + 1,
        text_y,
        TPageAbr::eBlend_3,
        1,
        0,
        Layer::eLayer_Above_FG1_39,
        115,
        98,
        99,
        offset,
        scale_fp,
        640,
        0);

    sFontDrawScreenSpace = bOldValue;
}

} // namespace AO
