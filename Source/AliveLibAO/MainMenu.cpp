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
#include "Camera.hpp"
#include "../AliveLibAE/Io.hpp"

namespace AO {

const AnimId buttonAnimIds_4BB1B8[4] = {
    AnimId::MenuHighlight_Circle,
    AnimId::MenuHighlight_Square,
    AnimId::MenuHighlight_Triangle,
    AnimId::None};

// TODO: Move out
ALIVE_VAR(1, 0x507690, s16, sSoundMono_507690, 0);

// TODO: Move out
void SND_Set_Mono_477020()
{
    //nullsub_63();
    sSoundMono_507690 = 1;
}

// TODO: Move out
void SND_Set_Stereo_477030()
{
    //nullsub_64();
    sSoundMono_507690 = 0;
}

ALIVE_VAR(1, 0x9F2DE8, s16, bWaitingForRemapInput_9F2DE8, 0);

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
ALIVE_VAR(1, 0x4D0030, const Buttons, sRemappableInputs_4D0030, {});

struct Menu_Button final
{
    s16 field_0_xpos;
    s16 field_2_ypos;
    AnimId field_4_anim_id;
};

const Menu_Button sMainScreenButtons_4D00B0[5] = {
    {33, 64, AnimId::MenuHighlight_Circle},
    {33, 85, AnimId::MenuHighlight_Circle},
    {33, 107, AnimId::MenuHighlight_Circle},
    {335, 215, AnimId::MenuHighlight_Circle},
    {335, 240, AnimId::MenuHighlight_Circle}};

const Menu_Button sOptionsButtons_4D0148[3] = {
    {33, 66, AnimId::MenuHighlight_Circle},
    {33, 87, AnimId::MenuHighlight_Circle},
    {288, 238, AnimId::MenuHighlight_Circle}};

const Menu_Button sSoundOptionsButtons_4D01C0[3] = {
    {33, 66, AnimId::MenuHighlight_Circle},
    {33, 87, AnimId::MenuHighlight_Circle},
    {289, 238, AnimId::MenuHighlight_Circle}};

const Menu_Button stru_4D01D8[3] = {
    {116, 251, AnimId::MenuHighlight_Circle},
    {116, 251, AnimId::MenuHighlight_Circle},
    {308, 240, AnimId::MenuHighlight_Circle}};

const Menu_Button stru_4D01F0[2] = {
    {62, 204, AnimId::MenuHighlight_Circle},
    {293, 204, AnimId::MenuHighlight_Circle}};

const Menu_Button stru_4D00E0[13] = {
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

struct Menu_Element final
{
    s32 field_0_xpos;
    s32 field_4_ypos;
    s32 field_8_input_command;
};

const Menu_Element sBtnArray_MainMenuStaticBtn_4D03F0[1] = {{146, 205, InputCommands::eUnPause_OrConfirm}};
const Menu_Element sBtnArray_Options_4D0400[2] = {{145, 204, InputCommands::eUnPause_OrConfirm}, {286, 202, InputCommands::eBack}};
const Menu_Element sBtnArray_AbeMotionsMenuButtons_4D0418[11] = {
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

const Menu_Element sBtnArray_AbeGamespeakMenuButtons_4D04A0[33] = {
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
    // these below seem unused - maybe they are part of a different Menu_Element struct?
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

const Menu_Element sBtnArray_LoadGameMenuButtons_4D0630[2] = {
    {62, 204, InputCommands::eUnPause_OrConfirm},
    {293, 205, InputCommands::eBack}};

struct SaveName final
{
    char_type field_0_mName[32];
};

ALIVE_ARY(1, 0x9F1DD8, SaveName, 128, sSaveNames_9F1DD8, {}); // Got more than 128 saves? Hard luck mate

ALIVE_VAR(1, 0x507694, s16, gDemoPlay_507694, 0);
ALIVE_VAR(1, 0x50769C, u8, sJoyResId_50769C, 0);

ALIVE_VAR(1, 0x9F2DE0, s32, gMainMenuInstanceCount_9F2DE0, 0);
ALIVE_VAR(1, 0x507688, s16, sFontLoaded_507688, 0);
ALIVE_VAR(1, 0x4D0228, s16, sListCount_4D0228, -1);

// The total number of valid controllers - includes the keyboard as well
ALIVE_VAR(1, 0x4CE598, s32, sAvailableControllers_4CE598, 0);
ALIVE_VAR(1, 0x5079A4, s32, gJoystickAvailable_5079A4, 0);

ALIVE_VAR(1, 0x9F2DDC, s32, sSelectedSaveIdx_9F2DDC, 0);
ALIVE_VAR(1, 0x9F2DD8, s32, sSaveIdx_9F2DD8, 0);


struct MenuFMV;

ALIVE_VAR(1, 0x9F2DE4, const MenuFMV*, sActiveList_9F2DE4, nullptr);

const u8 sFontPal_4D0090[32] = {
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
    148u,
    231u,
    156u,
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
    78u};

struct MenuFMV final
{
    const char_type* field_0_name;
    EReliveLevelIds field_4_level_id;
    s16 field_6;
    s16 field_8;
    s16 field_A_fmv_id;
    s16 field_C;
    s16 field_E;
};

const MenuFMV gFmvs_4D0230[13] = {
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
const MenuFMV sLevelList_4D0300[15] = {
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
    for (auto& animId : buttonAnimIds_4BB1B8)
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
    Animation_Init(GetAnimRes(buttonAnimIds_4BB1B8[buttonType]));

    mAnim.mRenderMode = TPageAbr::eBlend_1;
    mXPos = FP_FromInteger(xpos);
    mYPos = FP_FromInteger(ypos);
    field_E8_bDestroyOnDone = static_cast<s16>(bDestroyOnDone);
    field_E4 = 40;
    field_E6 = 8;

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
    field_E4 += field_E6;

    if (field_E4 < 40)
    {
        field_E4 += -field_E6;
        field_E6 = -field_E6;

        if (field_E8_bDestroyOnDone)
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }

    if (field_E4 > 127)
    {
        field_E4 -= field_E6;
        field_E6 = -field_E6;
    }

    mAnim.mBlue = static_cast<u8>(field_E4);
    mAnim.mGreen = static_cast<u8>(field_E4);
    mAnim.mRed = static_cast<u8>(field_E4);
}

void MainMenuFade::VRender(PrimHeader** ppOt)
{
    mAnim.VRender(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        ppOt,
        0,
        0);

    PSX_RECT rect = {};
    mAnim.Get_Frame_Rect(&rect);
}

struct MainMenu_TransitionData final
{
    s16 field_0;
    s16 field_2;
    s16 field_4;
    s16 field_6;
};
ALIVE_ASSERT_SIZEOF(MainMenu_TransitionData, 0x8);

const MainMenu_TransitionData stru_55C038[24] = // 3 x 8's ?
    {
        {-8, -8, 384, 1},
        {0, -32, 256, 1},
        {8, -8, 384, 1},
        {32, 0, 256, 1},
        {8, 8, 384, 1},
        {0, 32, 256, 1},
        {-8, 8, 384, 1},
        {-32, 0, 256, 1},

        {-16, -16, 256, 1},
        {0, -23, 256, 1},
        {16, -16, 256, 1},
        {23, 0, 256, 1},
        {16, 16, 256, 1},
        {0, 23, 256, 1},
        {-16, 16, 256, 1},

        {-23, 0, 256, 1},
        {-16, -16, 256, 1},
        {0, -16, 256, 1},
        {16, -16, 256, 1},
        {16, 0, 256, 1},
        {16, 16, 256, 1},
        {0, 16, 256, 1},
        {-16, 16, 256, 1},
        {-16, 0, 256, 1},
};

void MainMenuTransition::VScreenChanged()
{
    if (gMap.mOverlayId != gMap.GetOverlayId())
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
    : BaseGameObject(TRUE, 0)
{
    SetType(ReliveTypes::eDeathFadeOut);

    gObjListDrawables->Push_Back(this);

    mBaseGameObjectFlags.Set(Options::eDrawable_Bit4);

    for (s32 i = 0; i < 2; i++)
    {
        Init_SetTPage(&field_21C_tPage[i], 0, 1, PSX_getTPage(TPageMode::e16Bit_2, abr, 0, 0));
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
    s32 val1 = Math_Cosine_4510A0(field_246_colour_fade_value).fpValue;
    s32 val2 = Math_Sine_451110(field_246_colour_fade_value).fpValue;
    s32 r0g0 = -64 / ((v5 >> 2) + 1);
    for (s32 i = 0; i < 8; i++)
    {
        s32 idx = i + (8 * field_242_idx);
        s32 v8 = stru_55C038[idx].field_4 << 8;
        s32 v9 = stru_55C038[idx].field_2 << 16;
        s32 v10 = (s16) stru_55C038[idx].field_0 << 16;
        s32 v11 = Math_FixedPoint_Multiply_451040(v9, val1);
        s32 v12 = Math_FixedPoint_Multiply_451040(v10, val2) - v11;
        s32 v13 = Math_FixedPoint_Multiply_451040(op1, v8);
        s16 x0 = field_23E_width + 640 * ((s32) Math_FixedPoint_Multiply_451040(v12, v13) >> 16) / 368;
        s32 v14 = Math_FixedPoint_Multiply_451040(v9, val2);
        s32 v15 = Math_FixedPoint_Multiply_451040(v10, val1) + v14;
        s32 v16 = Math_FixedPoint_Multiply_451040(op1, v8);
        s16 y0 = field_240_k120 + (Math_FixedPoint_Multiply_451040(v15, v16) >> 16);

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

        s32 v23 = Math_FixedPoint_Multiply_451040(v20, val1);
        s32 x1 = Math_FixedPoint_Multiply_451040(v36, val2) - v23;
        s32 v25 = Math_FixedPoint_Multiply_451040(op1, y1);
        // TODO: Use PsxToPCX
        x1 = field_23E_width + 40 * ((s32) Math_FixedPoint_Multiply_451040(x1, v25) >> 16) / 23; // LOWORD
        s32 v26 = Math_FixedPoint_Multiply_451040(v38, val2);
        s32 v27 = v26 + Math_FixedPoint_Multiply_451040(v36, val1);
        s32 v28 = Math_FixedPoint_Multiply_451040(op1, y1);
        y1 = field_240_k120 + (Math_FixedPoint_Multiply_451040(v27, v28) >> 16); // LOWORD
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

    gMainMenuInstanceCount_9F2DE0++;

    if (sFontLoaded_507688 == 0)
    {
        sFontContext_4FFD68.LoadFontType(FontType::PauseMenu);
        sFontLoaded_507688 = 1;
    }

    field_FC_font.Load(240, sFontPal_4D0090, &sFontContext_4FFD68);
    
    // 30 = fmv select
    if (gMap.mCurrentCamera == 30)
    {
        Animation_Init(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
    }
    else
    {
        Animation_Init(GetAnimRes(AnimId::MenuDoor));
    }

    mAnim.mBlue = 127;
    mAnim.mGreen = 127;
    mAnim.mRed = 127;

    field_134_anim.Init(GetAnimRes(AnimId::MenuHighlight_Circle), this);

    field_134_anim.mFlags.Clear(AnimFlags::eBlending);
    field_134_anim.mFlags.Set(AnimFlags::eSemiTrans);

    field_134_anim.mRenderLayer = Layer::eLayer_MainMenuButtonBees_38;
    field_134_anim.field_14_scale = mSpriteScale;
    field_134_anim.mRenderMode = TPageAbr::eBlend_1;
    field_134_anim.LoadPal(GetPalRes(PalId::WhiteHighlite));
    field_204_flags &= ~6u;
    field_1E2_rgb = 40;
    field_1E4_colour_counter = -8;
    field_1D4_tlvInfo = tlvId;
    field_1D8_timer = sGnFrame + 150;

    // copy right boot screen
    field_1CC_fn_update = &Menu::CopyRight_Update_47B4C0;


    // 1 == abe hello screen
    if (gMap.mCurrentCamera == 1)
    {
        field_1CC_fn_update = &Menu::WaitForDoorToOpen_47B550;
        field_204_flags |= 2;
        mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuDoor));
    }

    field_1D0_fn_render = &Menu::Empty_Render_47AC80;
    field_1E0_selected_index.mainmenu = MainMenuOptions::eBegin_1;
    field_1E8_pMenuTrans = 0;
    field_1EC_pObj1 = 0;
    field_1F0_pObj2 = 0;
    field_200 = 0;
    field_1FA = 0;
    field_1F4_text = "";
    field_1F8 = 0;
    field_202 = 0;
    field_224_bToFmvSelect = 0;
    field_226_bToLevelSelect = 0;
    field_20C_bStartInSpecificMap = 0;

    sEnableFartGasCheat_507704 = 0;
    sVoiceCheat_507708 = 0;
    sEnableCheatFMV_50770C = 0;
    sEnableCheatLevelSelect_507710 = 0;
    sKilledMudokons_5076BC = 0;
    sRescuedMudokons_5076C0 = 0;
    gRestartRuptureFarmsKilledMuds_5076C4 = 0;
    gRestartRuptureFarmsSavedMuds_5076C8 = 0;

    // 30 = fmv select
    if (gMap.mCurrentCamera == 30)
    {
        field_204_flags &= ~2u;
        field_224_bToFmvSelect = 1;
        field_21C = FP_FromInteger(0);
        field_1CC_fn_update = &Menu::FMV_Select_Update_47E8D0;
        field_1D0_fn_render = &Menu::FMV_Or_Level_Select_Render_47EEA0;
        field_1E0_selected_index.raw = 0;
        field_218_previous_fmv_or_level_selection = 0;
        field_220 = FP_FromInteger(0);

        sActiveList_9F2DE4 = gFmvs_4D0230;
        sListCount_4D0228 = ALIVE_COUNTOF(gFmvs_4D0230);

        mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));

        mAnim.ReloadPal();
    }

    sAvailableControllers_4CE598 = (gJoystickAvailable_5079A4 != 0) + 1;
}

Menu::~Menu()
{
    Path::TLV_Reset(field_1D4_tlvInfo, -1, 0, 0);
    field_134_anim.VCleanUp();

    /*
    for (s32 i = 0; i < ALIVE_COUNTOF(field_E4_res_array); i++)
    {
        if (mAnim.field_20_ppBlock != field_E4_res_array[i])
        {
            ResourceManager::FreeResource_455550(field_E4_res_array[i]);
        }
    }*/

    if (field_1E8_pMenuTrans)
    {
        field_1E8_pMenuTrans->mBaseGameObjectFlags.Set(Options::eDead);
        field_1E8_pMenuTrans->mBaseGameObjectRefCount--;
        field_1E8_pMenuTrans = nullptr;
    }

    if (field_1EC_pObj1)
    {
        field_1EC_pObj1->mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (field_1F0_pObj2)
    {
        field_1F0_pObj2->mBaseGameObjectFlags.Set(Options::eDead);
    }

    gMainMenuInstanceCount_9F2DE0--;
}

void Menu::VRender(PrimHeader** ppOt)
{
    if ((field_204_flags >> 1) & 1)
    {
        mAnim.VRender(184, 162, ppOt, 0, 0);

        PSX_RECT rect = {};
        mAnim.Get_Frame_Rect(&rect);
    }
    (this->*field_1D0_fn_render)(ppOt);
}

void Menu::VScreenChanged()
{
    // Empty
}

void Menu::VUpdate()
{
    field_1E2_rgb += field_1E4_colour_counter;

    if (field_1E2_rgb < 40 || field_1E2_rgb > 80)
    {
        field_1E2_rgb -= field_1E4_colour_counter;
        field_1E4_colour_counter = -field_1E4_colour_counter;
    }

    field_134_anim.mRed = static_cast<u8>(field_1E2_rgb);
    field_134_anim.mBlue = static_cast<u8>(field_1E2_rgb);
    field_134_anim.mGreen = static_cast<u8>(field_1E2_rgb);

    (this->*field_1CC_fn_update)();
}

// Wait for the door that abes head comes out of to open
void Menu::WaitForDoorToOpen_47B550()
{
    field_204_flags |= 2u;
    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        mAnim.Set_Animation_Data(GetAnimRes(AnimId::AbeIntro));
        mAnim.ReloadPal();
        field_1CC_fn_update = &Menu::WaitForAbesHeadPoppingThroughDoor_47B5E0;
       // ResourceManager::FreeResource_455550(field_E4_res_array[3]);
        field_204_flags &= ~2u;
        //field_E4_res_array[3] = nullptr;
        field_1D8_timer = sGnFrame + 15;
    }
}

void Menu::WaitForAbesHeadPoppingThroughDoor_47B5E0()
{
    if (field_1D8_timer <= static_cast<s32>(sGnFrame))
    {
        field_204_flags |= 2u;
        mAnim.Set_Animation_Data(GetAnimRes(AnimId::AbeIntro));
        field_1CC_fn_update = &Menu::AbePopThroughDoor_47B620;
    }
}

void Menu::AbePopThroughDoor_47B620()
{
    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        // Put abe into the bug eyed idle loop
        mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_IdleBlink));
        field_1CC_fn_update = &Menu::SayHelloWaitForLoading_47B690;
        field_1DC_idle_input_counter = 0;
        /*
        ResourceManager::FreeResource_455550(field_E4_res_array[2]);
        field_E4_res_array[2] = nullptr;
        ResourceManager::LoadResourceFile("ABESPEAK.BAN", Menu::OnResourceLoaded_47ADA0, this);*/
    }
}

void Menu::CopyRight_Update_47B4C0()
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
            field_1CC_fn_update = &Menu::WaitForDoorToOpen_47B550;
            mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuDoor));
        }
    }
}

void Menu::FMV_Select_Update_47E8D0()
{
    sEnableCheatFMV_50770C = 0;
    sEnableCheatLevelSelect_507710 = 0;

    if (sMovie_ref_count_9F309C == 0)
    {
        if (Input().IsAnyPressed(InputObject::PadIndex::First, InputCommands::eUp)) // TODO: Input constants
        {
            if (field_1E0_selected_index.raw > 0 && field_21C == FP_FromInteger(0))
            {
                field_1E0_selected_index.raw--;
                SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
            }
        }
        else if (Input().IsAnyPressed(InputObject::PadIndex::First, InputCommands::eDown | InputCommands::eCheatMode)) // TODO: Input constants
        {
            if (field_1E0_selected_index.raw < (sListCount_4D0228 - 1) && field_21C == FP_FromInteger(0))
            {
                field_1E0_selected_index.raw++;
                SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
            }
        }

        if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eBack | InputCommands::eHop)) // TODO: Input constants
        {
            // Go back to main screen
            field_20C_bStartInSpecificMap = 0;

            if (field_1E8_pMenuTrans)
            {
                field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
            }
            else
            {
                field_1E8_pMenuTrans = relive_new MainMenuTransition(Layer::eLayer_FadeFlash_40, 1, 0, 16, TPageAbr::eBlend_1);
            }
            field_1CC_fn_update = &Menu::FMV_Or_Level_Select_To_Back_Update_47EC70;
        }

        if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eUnPause_OrConfirm)) // TODO: Input constants
        {
            if (field_224_bToFmvSelect)
            {
                if (sActiveList_9F2DE4[field_1E0_selected_index.raw].field_A_fmv_id >= 0)
                {
                    SND_StopAll_4762D0();

                    const FmvInfo* pFmvRec = Path_Get_FMV_Record_434680(sActiveList_9F2DE4[field_1E0_selected_index.raw].field_4_level_id, sActiveList_9F2DE4[field_1E0_selected_index.raw].field_A_fmv_id);
                    u32 movie1Sector = 0;
                    Get_fmvs_sectors_44FEB0(pFmvRec->field_0_pName, 0, 0, &movie1Sector, 0, 0);

                    relive_new Movie(pFmvRec->field_4_id, movie1Sector, static_cast<s8>(pFmvRec->field_6), pFmvRec->field_A, pFmvRec->field_C_volume);

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
                                    if (!pObj->mBaseGameObjectFlags.Get(BaseGameObject::eDead) && (!sNumCamSwappers_507668 || pObj->mBaseGameObjectFlags.Get(BaseGameObject::eUpdateDuringCamSwap_Bit10)))
                                    {
                                        pObj->VUpdate();
                                    }
                                }
                            }
                        }
                    }

                    gPsxDisplay.PutCurrentDispEnv();
                    pScreenManager->DecompressCameraToVRam(gMap.field_34_camera_array[0]->field_C_ppBits);
                    pScreenManager->EnableRendering();
                    SND_Restart();
                }
                else
                {
                    // "Credits" FMV
                    gCreditsControllerExists = 1;

                    // The credits are re-done in this class rather than using CreditsController... go to the Sherry credit screen
                    field_208_camera = 1;
                    gMap.SetActiveCam(EReliveLevelIds::eCredits, 1, static_cast<s16>(field_208_camera), CameraSwapEffects::eInstantChange_0, 0, 0);
                    field_1CC_fn_update = &Menu::To_Credits_Update_47F140;
                    field_1D0_fn_render = &Menu::Empty_Render_47AC80;
                }
            }
            else
            {
                const MenuFMV* pRec = &sActiveList_9F2DE4[field_1E0_selected_index.raw];

                field_20E_level = pRec->field_4_level_id;
                field_210_path = pRec->field_6;
                field_212_camera = pRec->field_8;

                field_214_abe_xpos = pRec->field_C;
                field_216_abe_ypos = pRec->field_E;

                field_20C_bStartInSpecificMap = 1;

                field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
                field_1CC_fn_update = &Menu::Level_Cheat_To_Loading_Update_47ED50;
            }
        }
    }
}

void Menu::Empty_Render_47AC80(PrimHeader**)
{
    // Draw nothing
}

const Menu_Button stru_4D00D8 = {34, 65, AnimId::MenuHighlight_Circle};
const Menu_Element x_and_esc_4D0660[] = {{33, 29, InputCommands::eUnPause_OrConfirm}, {304, 165, InputCommands::eBack}};


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

void Menu::FMV_Or_Level_Select_Render_47EEA0(PrimHeader** ppOt)
{
    // Glow hilight
    field_134_anim.VRender(
        stru_4D00D8.field_0_xpos,
        stru_4D00D8.field_2_ypos,
        ppOt,
        0,
        0);

    PSX_RECT rect = {};
    field_134_anim.Get_Frame_Rect(&rect);

    if (field_1E0_selected_index.raw != field_218_previous_fmv_or_level_selection)
    {
        if (field_21C != FP_FromInteger(0))
        {
            field_1E0_selected_index.raw = field_218_previous_fmv_or_level_selection;
        }
        else
        {
            if (field_1E0_selected_index.raw <= field_218_previous_fmv_or_level_selection)
            {
                if (field_1E0_selected_index.raw < field_218_previous_fmv_or_level_selection)
                {
                    field_21C = FP_FromInteger(-1) * FP_FromInteger(26);
                }
            }
            else
            {
                field_21C = FP_FromInteger(26);
            }
            field_220 = FP_FromDouble(4.5);
            field_218_previous_fmv_or_level_selection = field_1E0_selected_index.raw;
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
        s32 itemIdx = field_1E0_selected_index.raw + idxStart;
        if (itemIdx >= 0 && itemIdx <= sListCount_4D0228 - 1)
        {
            field_1F4_text = sActiveList_9F2DE4[itemIdx].field_0_name;
            const s32 textWidth = field_FC_font.MeasureScaledTextWidth(field_1F4_text, FP_FromInteger(1));
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
            polyOffset = field_FC_font.DrawString(
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

            polyOffset = field_FC_font.DrawString(
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
        RenderElement_47A4E0(x_and_esc_4D0660[i].field_0_xpos, x_and_esc_4D0660[i].field_4_ypos, x_and_esc_4D0660[i].field_8_input_command, ppOt, &field_FC_font, &polyOffset);
    }
}

void Menu::SayHelloWaitForLoading_47B690()
{
    // After 1 idle anim loop
    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        // Wait for in progress loading (gamespeak ban) to finish
        /*
        if (!field_E4_res_array[0])
        {
            ProgressInProgressFilesLoading();
        }*/
        Mudokon_SFX(MudSounds::eHello_3, 0, 0, 0);
        mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Hello));
        field_1CC_fn_update = &Menu::WaitForAbeSayHello_47B770;
    }
}

void Menu::WaitForAbeSayHello_47B770()
{
    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        // Abe has finished saying hello, go to main menu handler
        mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
        mAnim.SetFrame(7u);
        field_1CC_fn_update = &Menu::MainScreen_Update_47AF60;
        field_1D0_fn_render = &Menu::MainScreen_Render_47BED0;
        field_1D8_timer = sGnFrame + Math_RandomRange(300, 450);
    }
}

void Menu::ProgressInProgressFilesLoading()
{
    TRACE_ENTRYEXIT;
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
    while (!gLoadingFiles->Empty());
}

void Menu::MainScreen_Render_47BED0(PrimHeader** ppOt)
{
    field_134_anim.VRender(
        sMainScreenButtons_4D00B0[field_1E0_selected_index.raw].field_0_xpos,
        sMainScreenButtons_4D00B0[field_1E0_selected_index.raw].field_2_ypos,
        ppOt,
        0,
        0);

    PSX_RECT rect = {};
    field_134_anim.Get_Frame_Rect(&rect);

    s32 polyOffset = 0;
    for (const auto& element : sBtnArray_MainMenuStaticBtn_4D03F0)
    {
        RenderElement_47A4E0(element.field_0_xpos, element.field_4_ypos, element.field_8_input_command, ppOt, &field_FC_font, &polyOffset);
    }
}

void Menu::MainScreen_Update_47AF60()
{
    // Calculate idle timers for playing game play demos
    s32 bSmallerTimeout = 0;
    if (Input().Pressed(InputObject::PadIndex::First))
    {
        bSmallerTimeout = 0;
        field_1DC_idle_input_counter = 0;
        gDemoPlay_507694 = 0;
    }
    else
    {
        field_1DC_idle_input_counter++;
        bSmallerTimeout = gDemoPlay_507694;
    }

    // Backwards menu button
    if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eLeft | InputCommands::eUp))
    {
        field_1E0_selected_index.raw--;
        if (field_1E0_selected_index.mainmenu < MainMenuOptions::eGameSpeak_0)
        {
            field_1E0_selected_index.mainmenu = MainMenuOptions::eOptions_4; // TODO: Why isn't count of stru_4D00B0 ??
        }

        field_134_anim.Set_Animation_Data(GetAnimRes(sMainScreenButtons_4D00B0[field_1E0_selected_index.raw].field_4_anim_id));
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
        bSmallerTimeout = gDemoPlay_507694;
    }

    // Forward menu button
    if (Input().IsAnyHeld(InputObject::PadIndex::First, (InputCommands::eDown | InputCommands::eRight | InputCommands::eCheatMode)))
    {
        field_1E0_selected_index.raw++;
        if (field_1E0_selected_index.mainmenu > MainMenuOptions::eOptions_4) // TODO: Why isn't count of stru_4D00B0 ??
        {
            field_1E0_selected_index.mainmenu = MainMenuOptions::eGameSpeak_0;
        }

        field_134_anim.Set_Animation_Data(GetAnimRes(sMainScreenButtons_4D00B0[field_1E0_selected_index.raw].field_4_anim_id));
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
        bSmallerTimeout = gDemoPlay_507694;
    }
    const s32 idleMax = bSmallerTimeout != 0 ? 300 : 1500;
    if (Input().IsAnyHeld(InputObject::PadIndex::First, (InputCommands::eThrowItem | InputCommands::eUnPause_OrConfirm | InputCommands::eDoAction | InputCommands::eCheatMode | InputCommands::eBack)) || field_1DC_idle_input_counter > idleMax)
    {
        if (field_1DC_idle_input_counter <= idleMax)
        {
            //if (field_E4_res_array[0])
            {
                field_204_flags |= 1u;

                if (field_1E0_selected_index.mainmenu == MainMenuOptions::eBegin_1)
                {
                    // Begin/new game
                    Mudokon_SFX(MudSounds::eFollowMe_4, 0, 0, 0);
                    mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_FollowMe));
                }
                else if (field_1E0_selected_index.mainmenu == MainMenuOptions::eQuit_2)
                {
                    // Quit
                    Mudokon_SFX(MudSounds::eGoodbye_12, 0, 0, 0);
                    mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Goodbye));
                }
                else
                {
                    // 0 = game speak,
                    // 3 = load
                    // 4 = options
                    Mudokon_SFX(MudSounds::eOkay_13, 0, 0, 0);
                    mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Ok));
                }

                field_1CC_fn_update = &Menu::WaitForSpeakFinishAndStartChangeEffect_47BB90;
            }
            /*
            * // TODO Binned
            else
            {
                if (field_1E8_pMenuTrans)
                {
                    field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
                }
                else
                {
                    field_1E8_pMenuTrans = relive_new MainMenuTransition(Layer::eLayer_FadeFlash_40, 1, 0, 16, TPageAbr::eBlend_1);
                    if (field_1E8_pMenuTrans)
                    {
                        field_1E8_pMenuTrans->mBaseGameObjectRefCount++;
                    }
                }

                mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
                field_1CC_fn_update = &Menu::GoToSelectedMenuPage_47BC50;
            }*/
        }
        else
        {
            // Play a demo
            gDemoPlay_507694 = 1;
            field_1E0_selected_index.mainmenu = MainMenuOptions::eBegin_1;
            gAttract_507698 = 1;

            char_type fileNameBuf[20] = {};
            sprintf(fileNameBuf, "PLAYBK%02d.JOY", sJoyResId_50769C);
            //ResourceManager::LoadResourceFile_4551E0(fileNameBuf, 0, 0, 0);

            if (field_1E8_pMenuTrans)
            {
                field_1E8_pMenuTrans->mBaseGameObjectRefCount--;
                field_1E8_pMenuTrans->mBaseGameObjectFlags.Set(Options::eDead);
            }

            field_1E8_pMenuTrans = relive_new MainMenuTransition(Layer::eLayer_FadeFlash_40, 1, 0, 16, TPageAbr::eBlend_1);
            if (field_1E8_pMenuTrans)
            {
                field_1E8_pMenuTrans->mBaseGameObjectRefCount++;
            }
            field_1CC_fn_update = &Menu::GoToSelectedMenuPage_47BC50;
        }
    }

    if (sEnableCheatFMV_50770C)
    {
        sEnableCheatFMV_50770C = 0;
        field_224_bToFmvSelect = 1;
        sActiveList_9F2DE4 = gFmvs_4D0230;
        sListCount_4D0228 = ALIVE_COUNTOF(gFmvs_4D0230);
       // if (field_E4_res_array[0])
        {
            Mudokon_SFX(MudSounds::eOkay_13, 0, 0, 0);
            mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Ok));
            field_1CC_fn_update = &Menu::WaitForSpeakFinishAndStartChangeEffect_47BB90;
        }
        // TODO: probably binned now
        /*
        else
        {
            if (field_1E8_pMenuTrans)
            {
                field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
            }
            else
            {
                field_1E8_pMenuTrans = relive_new MainMenuTransition(Layer::eLayer_FadeFlash_40, 1, 0, 16, TPageAbr::eBlend_1);
                if (field_1E8_pMenuTrans)
                {
                    field_1E8_pMenuTrans->mBaseGameObjectRefCount++;
                }
            }

            mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
            field_1CC_fn_update = &Menu::GoToSelectedMenuPage_47BC50;
        }*/
    }

    if (sEnableCheatLevelSelect_507710)
    {
        sEnableCheatLevelSelect_507710 = 0;
        field_226_bToLevelSelect = 1;
        sActiveList_9F2DE4 = sLevelList_4D0300;
        sListCount_4D0228 = ALIVE_COUNTOF(sLevelList_4D0300);
       // if (field_E4_res_array[0])
        {
            Mudokon_SFX(MudSounds::eOkay_13, 0, 0, 0);
            mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Ok));
            field_1CC_fn_update = &Menu::WaitForSpeakFinishAndStartChangeEffect_47BB90;
        }
        /*
        // TODO: prob binned
        else
        {
            if (field_1E8_pMenuTrans)
            {
                field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
            }
            else
            {
                field_1E8_pMenuTrans = relive_new MainMenuTransition(Layer::eLayer_FadeFlash_40, 1, 0, 16, TPageAbr::eBlend_1);
                if (field_1E8_pMenuTrans)
                {
                    field_1E8_pMenuTrans->mBaseGameObjectRefCount++;
                }
            }

            mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
            field_1CC_fn_update = &Menu::GoToSelectedMenuPage_47BC50;
        }*/
    }

    // Some sort of idle anim toggling
    if (((field_204_flags) >> 2) & 1)
    {
        if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
        {
            mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
            field_204_flags &= ~4u;
            field_1D8_timer = sGnFrame + Math_RandomRange(120, 450);
        }
    }
    else if (field_1D8_timer <= static_cast<s32>(sGnFrame))
    {
        if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
        {
            mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_IdleBlink));
            field_204_flags |= 4u;
        }
    }
}

// After fade out go to gamespeak/options/load/whatever
void Menu::GoToSelectedMenuPage_47BC50()
{
    if (field_1E8_pMenuTrans == nullptr || field_1E8_pMenuTrans->field_16_bDone)
    {
        if (field_224_bToFmvSelect)
        {
            gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eFmvSelect_30, CameraSwapEffects::eInstantChange_0, 0, 0);
            field_1CC_fn_update = &Menu::ToNextMenuPage_47BD80;
            return;
        }

        if (field_226_bToLevelSelect)
        {
            gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eLvlSelect_31, CameraSwapEffects::eInstantChange_0, 0, 0);
            field_1CC_fn_update = &Menu::ToNextMenuPage_47BD80;
            return;
        }

        switch (field_1E0_selected_index.mainmenu)
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

                field_1CC_fn_update = &Menu::ToNextMenuPage_47BD80;

                field_134_anim.mBlue = 127;
                field_134_anim.mGreen = 127;
                field_134_anim.mRed = 127;
                break;

            // Begin
            case MainMenuOptions::eBegin_1:
                field_1CC_fn_update = &Menu::ToLoading_47B7E0;
                break;

            // Quit
            case MainMenuOptions::eQuit_2:
                sBreakGameLoop_507B78 = 1;
                exit(0);
                break;

            // Load
            case MainMenuOptions::eLoad_3:
                gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eLoad_6, CameraSwapEffects::eInstantChange_0, 0, 0);
                field_1CC_fn_update = &Menu::ToNextMenuPage_47BD80;
                break;

            // Options
            case MainMenuOptions::eOptions_4:
                gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eOptions_2, CameraSwapEffects::eInstantChange_0, 0, 0);
                field_1CC_fn_update = &Menu::ToNextMenuPage_47BD80;
                break;

            default:
                LOG_ERROR("Unknown menu item " << field_1E0_selected_index.raw);
                break;
        }
    }
}

void Menu::WaitForSpeakFinishAndStartChangeEffect_47BB90()
{
    // Abe finished speaking?
    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        if (field_1E8_pMenuTrans)
        {
            field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
        }
        else
        {
            field_1E8_pMenuTrans = relive_new MainMenuTransition(Layer::eLayer_FadeFlash_40, 1, 0, 16, TPageAbr::eBlend_1);
            if (field_1E8_pMenuTrans)
            {
                field_1E8_pMenuTrans->mBaseGameObjectRefCount++;
            }
        }

        mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
        field_1CC_fn_update = &Menu::GoToSelectedMenuPage_47BC50;
    }
}

void Menu::ToNextMenuPage_47BD80()
{
    if (sNumCamSwappers_507668 <= 0)
    {
        if (field_224_bToFmvSelect || field_226_bToLevelSelect)
        {
            field_204_flags &= ~2u;
            field_21C = FP_FromInteger(0);
            field_1CC_fn_update = &Menu::To_FMV_Or_Level_Select_Update_47EC30;
            field_1D0_fn_render = &Menu::FMV_Or_Level_Select_Render_47EEA0;
            field_1E0_selected_index.raw = 0;
            field_218_previous_fmv_or_level_selection = 0;
            field_220 = FP_FromInteger(0);
        }
        else
        {
            switch (field_1E0_selected_index.mainmenu)
            {
                case MainMenuOptions::eGameSpeak_0:
                {
                    field_134_anim.ReloadPal();
                    field_1CC_fn_update = &Menu::ToGameSpeak_Update_47D620;
                    field_1D0_fn_render = &Menu::GameSpeak_Render_47D700;
                    field_1E0_selected_index.gamespeak_menu = GameSpeakOptions::eWait_0;
                    break;
                }

                // ?? leads to 2 options, one starts the game, the other leads to
                // another screen with 1 option that only starts the game
                case MainMenuOptions::eBegin_1: // Never reached?
                    field_1CC_fn_update = &Menu::Update_NoRefs_47E3C0;
                    field_1D0_fn_render = &Menu::Render_NoRefs_47E5B0;
                    field_1E0_selected_index.raw = 0;
                    break;

                case MainMenuOptions::eLoad_3:
                    field_204_flags &= ~2u;
                    field_1CC_fn_update = &Menu::To_Load_Update_47D8E0;
                    field_1D0_fn_render = &Menu::Load_Render_47DDA0;
                    field_1E0_selected_index.raw = 0;
                    break;

                case MainMenuOptions::eOptions_4:
                {
                    field_1CC_fn_update = &Menu::To_Options_Update_47C250;
                    field_1D0_fn_render = &Menu::Options_Render_47C190;
                    field_134_anim.Set_Animation_Data(GetAnimRes(sOptionsButtons_4D0148[0].field_4_anim_id));
                    field_1E0_selected_index.options_menu = OptionsMenuOptions::eController_0;
                    break;
                }

                default:
                    LOG_ERROR("Unknown menu item " << field_1E0_selected_index.raw);
                    break;
            }
        }
        field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 0, 0, 16);
    }
}

void Menu::ToLoading_47B7E0()
{
    if (field_1E8_pMenuTrans)
    {
        if (field_1E8_pMenuTrans->field_16_bDone)
        {
            field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 0, 0, 16);
            gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eLoading_21, CameraSwapEffects::eInstantChange_0, 0, 0);
            field_204_flags &= ~2u;
            field_1CC_fn_update = &Menu::Loading_Update_47B870;
            field_1D0_fn_render = &Menu::Empty_Render_47AC80;
        }
    }
}


void Menu::ToGameSpeak_Update_47D620()
{
    if (field_1E8_pMenuTrans)
    {
        if (field_1E8_pMenuTrans->field_16_bDone)
        {
            field_1CC_fn_update = &Menu::GameSpeak_Update_47CBD0;
            field_1DC_idle_input_counter = 0;
        }
    }
}


void Menu::GameSpeak_Render_47D700(PrimHeader** ppOt)
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
        RenderElement_47A4E0(
            sBtnArray_AbeGamespeakMenuButtons_4D04A0[i].field_0_xpos,
            sBtnArray_AbeGamespeakMenuButtons_4D04A0[i].field_4_ypos,
            sBtnArray_AbeGamespeakMenuButtons_4D04A0[i].field_8_input_command,
            ppOt,
            &field_FC_font,
            &polyOffset);
    }
}

void Menu::To_FMV_Or_Level_Select_Update_47EC30()
{
    if (field_1E8_pMenuTrans->field_16_bDone)
    {
        //ResourceManager::FreeResource_455550(field_E4_res_array[0]);
        //field_E4_res_array[0] = nullptr;
        field_1CC_fn_update = &Menu::FMV_Select_Update_47E8D0;
    }
}


void Menu::Update_NoRefs_47E3C0()
{
    
}


void Menu::Render_NoRefs_47E5B0(PrimHeader** /*ppOt*/)
{
    
}


void Menu::To_Load_Update_47D8E0()
{
    sSelectedSaveIdx_9F2DDC = 0;

    field_1E0_selected_index.raw = 0;
    field_230_bGoBack = 0;
    field_228 = FP_FromInteger(0);

    if (field_1E8_pMenuTrans)
    {
        if (field_1E8_pMenuTrans->field_16_bDone)
        {
            sSaveIdx_9F2DD8 = 0;
            IO_EnumerateDirectory("*.sav", [](const char_type* fileName, u32 /*lastWriteTime*/)
                                  {
                                      if (sSaveIdx_9F2DD8 < 128) // TODO: Array len
                                      {
                                          size_t saveNameLen = strlen(fileName) - 4;

                                          // Limit length to prevent buffer overflow
                                          if (saveNameLen > 19)
                                          {
                                              saveNameLen = 19;
                                          }

                                          if (saveNameLen > 0)
                                          {
                                              memcpy(sSaveNames_9F1DD8[sSaveIdx_9F2DD8].field_0_mName, fileName, saveNameLen);
                                              sSaveNames_9F1DD8[sSaveIdx_9F2DD8].field_0_mName[saveNameLen] = 0;
                                              sSaveIdx_9F2DD8++;
                                          }
                                      }
                                  });
            qsort(sSaveNames_9F1DD8, sSaveIdx_9F2DD8, sizeof(SaveName), &Menu::StringsEqual_47DA20);

            field_1DC_idle_input_counter = 0;
            field_1FE = 0;
            field_1CC_fn_update = &Menu::Load_Update_47D760;
        }
    }
}


void Menu::Load_Render_47DDA0(PrimHeader** ppOt)
{
    if (field_230_bGoBack || !sSaveIdx_9F2DD8)
    {
        field_134_anim.VRender(stru_4D01F0[1].field_0_xpos, stru_4D01F0[1].field_2_ypos + 36, ppOt, 0, 0);
        PSX_RECT rect = {};
        field_134_anim.Get_Frame_Rect(&rect);
    }
    else
    {
        field_134_anim.VRender(stru_4D01F0[0].field_0_xpos, stru_4D01F0[0].field_2_ypos + 36, ppOt, 0, 0);
        PSX_RECT rect = {};
        field_134_anim.Get_Frame_Rect(&rect);
    }

    if (field_1E0_selected_index.raw != sSelectedSaveIdx_9F2DDC)
    {
        if (field_228 != FP_FromInteger(0))
        {
            field_1E0_selected_index.raw = static_cast<s16>(sSelectedSaveIdx_9F2DDC);
        }
        else
        {
            if (field_1E0_selected_index.raw < sSelectedSaveIdx_9F2DDC)
            {
                field_228 = FP_FromInteger(-26);
                field_22C = FP_FromDouble(4.5);
                sSelectedSaveIdx_9F2DDC = field_1E0_selected_index.raw;
            }
            else if (field_1E0_selected_index.raw > sSelectedSaveIdx_9F2DDC)
            {
                field_228 = FP_FromInteger(26);
                field_22C = FP_FromDouble(4.5);
                sSelectedSaveIdx_9F2DDC = field_1E0_selected_index.raw;
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
        const auto curIdx = start + field_1E0_selected_index.raw;
        if (curIdx >= 0 && curIdx < sSaveIdx_9F2DD8)
        {
            field_1F4_text = sSaveNames_9F1DD8[curIdx].field_0_mName;

            const auto name_width = field_FC_font.MeasureScaledTextWidth(field_1F4_text, FP_FromInteger(1));
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

            const s32 nextPolyOff = field_FC_font.DrawString(
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

            polyOffset = field_FC_font.DrawString(
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
        RenderElement_47A4E0(
            sBtnArray_LoadGameMenuButtons_4D0630[i].field_0_xpos,
            sBtnArray_LoadGameMenuButtons_4D0630[i].field_4_ypos,
            sBtnArray_LoadGameMenuButtons_4D0630[i].field_8_input_command,
            ppOt,
            &field_FC_font,
            &polyOffset);
    }
}

void Menu::To_Options_Update_47C250()
{
    if (field_1E8_pMenuTrans)
    {
        if (field_1E8_pMenuTrans->field_16_bDone)
        {
            field_1CC_fn_update = &Menu::Options_Update_47BF90;
            field_1DC_idle_input_counter = 0;
        }
    }
}

void Menu::Options_Render_47C190(PrimHeader** ppOt)
{
    field_134_anim.VRender(
        sOptionsButtons_4D0148[field_1E0_selected_index.raw].field_0_xpos,
        sOptionsButtons_4D0148[field_1E0_selected_index.raw].field_2_ypos,
        ppOt,
        0,
        0);

    PSX_RECT rect = {};
    field_134_anim.Get_Frame_Rect(&rect);

    s32 polyOff = 0;
    for (const auto& element : sBtnArray_Options_4D0400)
    {
        RenderElement_47A4E0(element.field_0_xpos, element.field_4_ypos, element.field_8_input_command, ppOt, &field_FC_font, &polyOff);
    }
}

void Menu::FMV_Or_Level_Select_Back_Update_47ECB0()
{
    if (sNumCamSwappers_507668 <= 0)
    {
        field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 0, 0, 16);
        
        field_1E0_selected_index.mainmenu = MainMenuOptions::eBegin_1;

        field_134_anim.Set_Animation_Data(GetAnimRes(sMainScreenButtons_4D00B0[1].field_4_anim_id));
        field_204_flags |= 2u;
        field_224_bToFmvSelect = 0;
        field_226_bToLevelSelect = 0;
        field_1CC_fn_update = &Menu::To_MainScreen_Update_47BB60;
        field_1D0_fn_render = &Menu::MainScreen_Render_47BED0;
    }
}

void Menu::Loading_Update_47B870()
{
    if (!gAttract_507698)
    {
        if (field_1E8_pMenuTrans)
        {
            if (field_1E8_pMenuTrans->field_16_bDone)
            {
                if (gAttract_507698)
                {
                    char_type buffer[92] = {};
                    sprintf(buffer, "loading Joy # %d\n", sJoyResId_50769C);
                    // Never used ??
                    LOG_INFO(buffer);
                }

                field_1E8_pMenuTrans->mBaseGameObjectRefCount--;
                field_1E8_pMenuTrans->mBaseGameObjectFlags.Set(Options::eDead);
                field_1E8_pMenuTrans = nullptr;

                /*
                if (!field_E4_res_array[0])
                {
                    ProgressInProgressFilesLoading();
                }*/

                mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
                //ResourceManager::FreeResource_455550(field_E4_res_array[0]);
                //field_E4_res_array[0] = nullptr;
                //ResourceManager::Reclaim_Memory_455660(0);
                field_1CC_fn_update = &Menu::NewGameStart_47B9C0;
            }
        }
    }
}

void Menu::NewGameStart_47B9C0()
{
    if (!sActiveHero)
    {
        sActiveHero = relive_new Abe();
    }

    if (gAttract_507698)
    {
        // OG bug fix: the demo will load a save which will call Kill_Objects_451720 which will delete this object
        // resulting in a crash when we try access any member vars at the end. Bump the ref count so we can kill ourselves instead.
        mBaseGameObjectRefCount++;
        // TODO: The ctor of the playback should load the demo res itself
        u8** ppRes = nullptr; //ResourceManager::GetLoadedResource(ResourceManager::Resource_Plbk, sJoyResId_50769C, 1, 0);
        relive_new DemoPlayback(ppRes, 0);
        mBaseGameObjectRefCount--;
    }
    else
    {
        if (!pPauseMenu_5080E0)
        {
            pPauseMenu_5080E0 = relive_new PauseMenu();
        }

        if (field_20C_bStartInSpecificMap)
        {
            field_20C_bStartInSpecificMap = FALSE;
            gMap.SetActiveCam(field_20E_level, field_210_path, field_212_camera, CameraSwapEffects::eInstantChange_0, 0, 0);
            sActiveHero->mXPos = FP_FromInteger(field_214_abe_xpos);
            sActiveHero->mYPos = FP_FromInteger(field_216_abe_ypos);
        }
        else
        {
            // Start the game in the biggest meat processing plant
            gInfiniteGrenades_5076EC = FALSE;
            gMap.SetActiveCam(EReliveLevelIds::eRuptureFarms, 15, 1, CameraSwapEffects::ePlay1FMV_5, 102, 0);

            // What if someone made a level editor and wanted to change where abe spawns on the first map? Well... hard luck pal
            sActiveHero->mXPos = FP_FromInteger(1378);
            sActiveHero->mYPos = FP_FromInteger(83);
        }
    }
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void Menu::Options_Update_47BF90()
{
    // Idle time calculate
    if (Input().Pressed(InputObject::PadIndex::First))
    {
        field_1DC_idle_input_counter = 0;
    }
    else
    {
        field_1DC_idle_input_counter++;
    }

    // Menu backwards
    if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eUp))
    {
        if (field_1E0_selected_index.options_menu <= OptionsMenuOptions::eController_0)
        {
            field_1E0_selected_index.options_menu = OptionsMenuOptions::eSound_1;
        }
        else
        {
            field_1E0_selected_index.raw--;
        }

        field_134_anim.Set_Animation_Data(GetAnimRes(sOptionsButtons_4D0148[field_1E0_selected_index.raw].field_4_anim_id));
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
    }

    // Menu forwards
    if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eDown | InputCommands::eCheatMode))
    {
        if (field_1E0_selected_index.options_menu >= OptionsMenuOptions::eSound_1)
        {
            field_1E0_selected_index.options_menu = OptionsMenuOptions::eController_0;
        }
        else
        {
            field_1E0_selected_index.raw++;
        }

        field_134_anim.Set_Animation_Data(GetAnimRes(sOptionsButtons_4D0148[field_1E0_selected_index.raw].field_4_anim_id));
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
    }

    if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eUnPause_OrConfirm | InputCommands::eDoAction))
    {
        Mudokon_SFX(MudSounds::eOkay_13, 0, 0, 0);
        mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Ok));
        field_1CC_fn_update = &Menu::Options_WaitForAbeSpeak_Update_47C280;
    }

    if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eBack | InputCommands::eHop) || field_1DC_idle_input_counter > 900)
    {
        // Back to main menu
        field_1E0_selected_index.options_menu = OptionsMenuOptions::eMainMenu_2;
        field_134_anim.Set_Animation_Data(GetAnimRes(sOptionsButtons_4D0148[2].field_4_anim_id));
        Mudokon_SFX(MudSounds::eOkay_13, 0, 0, 0);
        mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Ok));
        field_1CC_fn_update = &Menu::Options_WaitForAbeSpeak_Update_47C280;
    }

    // Some sort of idle anim toggle?
    if (((field_204_flags) >> 2) & 1)
    {
        if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
        {
            mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
            field_204_flags &= ~4u;
            field_1D8_timer = sGnFrame + Math_RandomRange(120, 450);
        }
    }
    else if (field_1D8_timer <= static_cast<s32>(sGnFrame))
    {
        if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
        {
            mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_IdleBlink));
            field_204_flags |= 4u;
        }
    }
}

void Menu::Options_WaitForAbeSpeak_Update_47C280()
{
    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
        field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
        field_1CC_fn_update = &Menu::Option_GoTo_Selected_Update_47C2C0;
    }
}

void Menu::Option_GoTo_Selected_Update_47C2C0()
{
    if (field_1E8_pMenuTrans)
    {
        if (field_1E8_pMenuTrans->field_16_bDone)
        {
            switch (field_1E0_selected_index.options_menu)
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
                    LOG_ERROR("Unknown menu idx " << field_1E0_selected_index.raw);
                    break;
            }

            field_1CC_fn_update = &Menu::Options_To_Selected_After_Cam_Change_Update_47C330;
        }
    }
}

void Menu::Options_To_Selected_After_Cam_Change_Update_47C330()
{
    if (sNumCamSwappers_507668 <= 0)
    {
        switch (field_1E0_selected_index.options_menu)
        {
            // To controller options
            case OptionsMenuOptions::eController_0:
                field_204_flags &= ~2u;
                field_228 = FP_FromInteger(0);
                field_1CC_fn_update = &Menu::To_Options_Controller_Update_47F2E0;
                field_1D0_fn_render = &Menu::Options_Controller_Render_47F430;
                field_1E0_selected_index.raw = static_cast<s16>(Input_JoyStickEnabled());
                field_230_bGoBack = -1;
                field_22C = FP_FromInteger(0);
                break;

            // To sound options
            case OptionsMenuOptions::eSound_1:
            {
                field_1CC_fn_update = &Menu::To_Options_Sound_Update_47C6F0;
                field_1D0_fn_render = &Menu::Options_Sound_Render_47C630;
                field_1E0_selected_index.raw = sSoundMono_507690;
                field_134_anim.Set_Animation_Data(GetAnimRes(sSoundOptionsButtons_4D01C0[sSoundMono_507690].field_4_anim_id));
                break;
            }

            // Back to main menu
            case OptionsMenuOptions::eMainMenu_2:
            {
                field_1CC_fn_update = &Menu::To_MainScreen_Update_47BB60;
                field_1D0_fn_render = &Menu::MainScreen_Render_47BED0;
                field_1E0_selected_index.mainmenu = MainMenuOptions::eOptions_4;
                field_134_anim.Set_Animation_Data(GetAnimRes(sMainScreenButtons_4D00B0[4].field_4_anim_id));
                break;
            }

            default:
                LOG_ERROR("Unknown menu idx " << field_1E0_selected_index.raw);
                break;
        }

        field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 0, 0, 16);
    }
}

void Menu::To_Options_Controller_Update_47F2E0()
{
    field_230_bGoBack = -1;
    if (field_1E8_pMenuTrans)
    {
        if (field_1E8_pMenuTrans->field_16_bDone)
        {
            field_1CC_fn_update = &Menu::Options_Controller_Update_47F210;
            field_1D0_fn_render = &Menu::Options_Controller_Render_47F430;
            field_1DC_idle_input_counter = 0;
            field_1E0_selected_index.raw = static_cast<s16>(Input_JoyStickEnabled());
            field_228 = FP_FromInteger(0);
        }
    }
}

const Menu_Button controllerSelectButtons_4D0160[2] = {
    {44, 236, AnimId::MenuHighlight_Circle},
    {290, 236, AnimId::MenuHighlight_Circle}};

const Menu_Element controllerSelectElements_4D0678[2] = {
    {43, 200, InputCommands::eUnPause_OrConfirm},
    {289, 200, InputCommands::eBack}};

void Menu::Options_Controller_Render_47F430(PrimHeader** ppOt)
{
    if (field_230_bGoBack != -1)
    {
        field_134_anim.VRender(
            controllerSelectButtons_4D0160[field_230_bGoBack].field_0_xpos,
            controllerSelectButtons_4D0160[field_230_bGoBack].field_2_ypos,
            ppOt,
            0,
            0);
    }

    PSX_RECT rect = {};
    field_134_anim.Get_Frame_Rect(&rect);

    if (field_1E0_selected_index.raw != (Input_JoyStickEnabled() ? 1 : 0))
    {
        if (field_228 > FP_FromInteger(0))
        {
            field_1E0_selected_index.raw = static_cast<s16>(Input_JoyStickEnabled());
        }
        else
        {
            if (field_1E0_selected_index.raw < (Input_JoyStickEnabled() ? 1 : 0))
            {
                field_228 = FP_FromInteger(-1) * FP_FromInteger(26);
                field_22C = FP_FromDouble(4.5);
                Input_SetJoyStickEnabled(field_1E0_selected_index.raw ? true : false);
            }
            else if (field_1E0_selected_index.raw > (Input_JoyStickEnabled() ? 1 : 0))
            {
                field_228 = FP_FromInteger(26);
                field_22C = FP_FromDouble(4.5);
                Input_SetJoyStickEnabled(field_1E0_selected_index.raw ? true : false);
            }
        }
    }

    NavigateBetweenTwoPoints(field_228, field_22C);

    s32 yOffset = -26;
    s32 polyOffset = 0;
    for (s32 i = 0; i < 3; i++)
    {
        s32 selection = field_1E0_selected_index.raw + i - 1;
        if (selection >= 0 && selection < sAvailableControllers_4CE598)
        {
            if (selection == 0)
            {
                field_1F4_text = "Keyboard";
            }
            else
            {
                field_1F4_text = "Gamepad";
            }

            const s16 fontWidth = static_cast<s16>(field_FC_font.MeasureScaledTextWidth(field_1F4_text, FP_FromInteger(1)));
            const s16 x = static_cast<s16>(fontWidth >= 336 ? 16 : (368 - fontWidth) / 2);
            const s16 y = static_cast<s16>(FP_GetExponent(field_228 + FP_FromDouble(0.5)) + yOffset + 112);

            polyOffset = field_FC_font.DrawString(
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
            polyOffset = field_FC_font.DrawString(
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
        Menu::RenderElement_47A4E0(
            menuElement.field_0_xpos,
            menuElement.field_4_ypos,
            menuElement.field_8_input_command,
            ppOt,
            &field_FC_font,
            &polyOffset);
    }
}

void Menu::To_Options_Sound_Update_47C6F0()
{
    if (field_1E8_pMenuTrans)
    {
        if (field_1E8_pMenuTrans->field_16_bDone)
        {
            field_1CC_fn_update = &Menu::Options_Sound_Update_47C420;
            field_1DC_idle_input_counter = 0;
        }
    }
}

void Menu::To_MainScreen_Update_47BB60()
{
    if (field_1E8_pMenuTrans)
    {
        if (field_1E8_pMenuTrans->field_16_bDone)
        {
            field_1CC_fn_update = &Menu::MainScreen_Update_47AF60;
            field_1DC_idle_input_counter = 0;
        }
    }
}

void Menu::Options_Sound_Render_47C630(PrimHeader** ppOt)
{
    field_134_anim.VRender(
        sSoundOptionsButtons_4D01C0[field_1E0_selected_index.raw].field_0_xpos,
        sSoundOptionsButtons_4D01C0[field_1E0_selected_index.raw].field_2_ypos,
        ppOt,
        0,
        0);

    PSX_RECT rect = {};
    field_134_anim.Get_Frame_Rect(&rect);

    s32 polyOffset = 0;
    for (s32 i = 0; i < 2; i++)
    {
        RenderElement_47A4E0(sBtnArray_AbeMotionsMenuButtons_4D0418[i].field_0_xpos, sBtnArray_AbeMotionsMenuButtons_4D0418[i].field_4_ypos, sBtnArray_AbeMotionsMenuButtons_4D0418[i].field_8_input_command, ppOt, &field_FC_font, &polyOffset);
    }
}

void Menu::Options_Sound_Update_47C420()
{
    if (Input().Pressed(InputObject::PadIndex::First))
    {
        field_1DC_idle_input_counter = 0;
    }
    else
    {
        field_1DC_idle_input_counter++;
    }

    if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eUp)) // TODO: Input constants
    {
        if (field_1E0_selected_index.sound_menu <= SoundOptions::eStereo_0)
        {
            field_1E0_selected_index.sound_menu = SoundOptions::eMono_1;
        }
        else
        {
            field_1E0_selected_index.raw--;
        }

        field_134_anim.Set_Animation_Data(GetAnimRes(sSoundOptionsButtons_4D01C0[field_1E0_selected_index.raw].field_4_anim_id));
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
    }

    if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eDown | InputCommands::eCheatMode))
    {
        if (field_1E0_selected_index.sound_menu >= SoundOptions::eMono_1)
        {
            field_1E0_selected_index.sound_menu = SoundOptions::eStereo_0;
        }
        else
        {
            field_1E0_selected_index.raw++;
        }

        field_134_anim.Set_Animation_Data(GetAnimRes(sSoundOptionsButtons_4D01C0[field_1E0_selected_index.raw].field_4_anim_id));
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
    }

    if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eUnPause_OrConfirm))
    {
        if (field_1E0_selected_index.raw)
        {
            SND_Set_Mono_477020();
        }
        else
        {
            SND_Set_Stereo_477030();
        }

        Mudokon_SFX(MudSounds::eOkay_13, 0, 0, nullptr);

        mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Ok));
        field_1CC_fn_update = &Menu::Options_WaitForAbeSayOK_Update_47C720;
    }

    if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eBack | InputCommands::eHop) || field_1DC_idle_input_counter > 900)
    {
        field_1E0_selected_index.sound_menu = SoundOptions::eExit_2;
        field_134_anim.Set_Animation_Data(GetAnimRes(sSoundOptionsButtons_4D01C0[2].field_4_anim_id));
        Mudokon_SFX(MudSounds::eOkay_13, 0, 0, nullptr);
        mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Ok));
        field_1CC_fn_update = &Menu::Options_WaitForAbeSayOK_Update_47C720;
    }

    // Idle anim toggle ?
    if (((field_204_flags) >> 2) & 1)
    {
        if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
        {
            mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
            field_204_flags &= ~4u;
            field_1D8_timer = sGnFrame + Math_RandomRange(120, 450);
        }
    }
    else if (field_1D8_timer <= static_cast<s32>(sGnFrame))
    {
        if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
        {
            mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_IdleBlink));
            field_204_flags |= 4u;
        }
    }
}

void Menu::Options_WaitForAbeSayOK_Update_47C720()
{
    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
        field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
        field_1CC_fn_update = &Menu::Options_WaitForScreenTrans_Update_47C760;
    }
}

void Menu::Options_WaitForScreenTrans_Update_47C760()
{
    if (field_1E8_pMenuTrans)
    {
        if (field_1E8_pMenuTrans->field_16_bDone)
        {
            gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eOptions_2, CameraSwapEffects::eInstantChange_0, 0, 0);
            field_1CC_fn_update = &Menu::To_MainOptions_Screen_After_Camera_Change_Update_47C7A0;
        }
    }
}

void Menu::To_MainOptions_Screen_After_Camera_Change_Update_47C7A0()
{
    if (sNumCamSwappers_507668 <= 0)
    {
        field_1CC_fn_update = &Menu::To_Options_Update_47C250;
        field_1D0_fn_render = &Menu::Options_Render_47C190;
        field_1E0_selected_index.options_menu = OptionsMenuOptions::eSound_1;
        field_134_anim.Set_Animation_Data(GetAnimRes(sOptionsButtons_4D0148[1].field_4_anim_id));
        field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 0, 0, 16);
    }
}


void Menu::GameSpeak_Update_47CBD0()
{
    if (Input().Pressed(InputObject::PadIndex::First))
    {
        field_1DC_idle_input_counter = 0;
    }
    else
    {
        field_1DC_idle_input_counter++;
    }

    if (field_204_flags & 1)
    {
        if (field_1E0_selected_index.gamespeak_menu == GameSpeakOptions::eChant_8 && !(sGnFrame % 8))
        {
            const FP screen_y = pScreenManager->mCamPos->y - FP_FromInteger(pScreenManager->mCamYOff);
            const FP screen_x = pScreenManager->mCamPos->x - FP_FromInteger(pScreenManager->mCamXOff);

            auto pParticle = relive_new Particle(
                screen_x + (FP_FromInteger(Math_RandomRange(-40, 40) + 184)),
                screen_y + (FP_FromInteger(162 - Math_RandomRange(30, 90))),
                GetAnimRes(AnimId::OptionChantOrb_Particle));
            if (pParticle)
            {
                pParticle->mAnim.mRenderMode = TPageAbr::eBlend_1;
                pParticle->mAnim.mRenderLayer = Layer::eLayer_Above_FG1_39;
            }
        }

        if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
        {
            if (field_1E0_selected_index.gamespeak_menu == GameSpeakOptions::eChant_8)
            {
                if (!Input_IsChanting())
                {
                    SND_Seq_Stop_477A60(SeqId::eMudokonChant1_11);

                    field_1EC_pObj1->field_E8_bDestroyOnDone = 1;
                    field_1EC_pObj1 = nullptr;

                    if (field_1F0_pObj2)
                    {
                        field_1F0_pObj2->field_E8_bDestroyOnDone = 1;
                        field_1F0_pObj2 = nullptr;
                    }

                    mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_ChantEnd));
                    field_1E0_selected_index.gamespeak_menu = GameSpeakOptions::eHello_1;
                }
            }
            else
            {
                field_204_flags &= ~1u;
                if (field_1E0_selected_index.gamespeak_menu == GameSpeakOptions::eLaugh_6 || field_1E0_selected_index.gamespeak_menu == GameSpeakOptions::eHello_1)
                {
                    mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_IdleBlink));
                    mAnim.SetFrame(7u);
                }
                else
                {
                    mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
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
        mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Chant));
        field_1E0_selected_index.gamespeak_menu = GameSpeakOptions::eChant_8;

        if (!field_1EC_pObj1)
        {
            if (Input_JoyStickEnabled())
            {
                field_1EC_pObj1 = relive_new MainMenuFade(stru_4D00E0[8].field_0_xpos, stru_4D00E0[8].field_2_ypos + 36, buttonType::eCircle_0, 0);
            }
            else
            {
                field_1EC_pObj1 = relive_new MainMenuFade(181, stru_4D00E0[8].field_2_ypos + 36, buttonType::eCircle_0, 0);
            }
        }

        if (field_1F0_pObj2)
        {
            field_1F0_pObj2->field_E8_bDestroyOnDone = 1;
        }

        if (Input_JoyStickEnabled())
        {
            field_1F0_pObj2 = relive_new MainMenuFade(stru_4D00E0[11].field_0_xpos, stru_4D00E0[11].field_2_ypos + 36, buttonType::eCircle_0, 0);
        }
        else
        {
            field_1F0_pObj2 = nullptr;
        }
        CycleGameSpeakIdleAnims();
        return;
    }

    if (Input().IsAnyPressed(InputObject::PadIndex::First, sInputKey_LeftGameSpeakEnabler))
    {
        if (field_1F0_pObj2)
        {
            const FP xpos = FP_FromInteger(stru_4D00E0[10].field_0_xpos);
            if (field_1F0_pObj2->mXPos != xpos)
            {
                field_1F0_pObj2->mXPos = xpos;
                field_1F0_pObj2->mYPos = FP_FromInteger(stru_4D00E0[10].field_2_ypos + 36);
            }
        }
        else if (Input_JoyStickEnabled())
        {
            field_1F0_pObj2 = relive_new MainMenuFade(stru_4D00E0[10].field_0_xpos, stru_4D00E0[10].field_2_ypos + 36, buttonType::eCircle_0, 0);
        }

        if (Input().IsAnyHeld(InputObject::PadIndex::First, sInputKey_GameSpeak2))
        {
            Mudokon_SFX(MudSounds::eFollowMe_4, 0, 0, 0);
            field_204_flags |= 1u;
            mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_FollowMe));
            field_1E0_selected_index.gamespeak_menu = GameSpeakOptions::eFollowMe_2;
            relive_new MainMenuFade(stru_4D00E0[2].field_0_xpos, stru_4D00E0[2].field_2_ypos + 36, buttonType::eCircle_0, 1);
        }
        else if (Input().IsAnyHeld(InputObject::PadIndex::First, sInputKey_GameSpeak4))
        {
            Mudokon_SFX(MudSounds::eWait_6, 0, 0, 0);
            field_204_flags |= 1u;
            mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Wait));
            field_1E0_selected_index.gamespeak_menu = GameSpeakOptions::eWait_0;
            relive_new MainMenuFade(stru_4D00E0[0].field_0_xpos, stru_4D00E0[0].field_2_ypos + 36, buttonType::eCircle_0, 1);
        }
        else if (Input().IsAnyHeld(InputObject::PadIndex::First, sInputKey_GameSpeak1))
        {
            Mudokon_SFX(MudSounds::eHello_3, 0, 0, 0);
            field_204_flags |= 1u;
            mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Hello));
            field_1E0_selected_index.gamespeak_menu = GameSpeakOptions::eHello_1;
            relive_new MainMenuFade(stru_4D00E0[1].field_0_xpos, stru_4D00E0[1].field_2_ypos + 36, buttonType::eCircle_0, 1);
        }
        else if (Input().IsAnyPressed(InputObject::PadIndex::First, sInputKey_GameSpeak3))
        {
            Mudokon_SFX(MudSounds::eAngry_5, 0, 0, 0);
            field_204_flags |= 1u;
            mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Anger));
            field_1E0_selected_index.gamespeak_menu = GameSpeakOptions::eAngry_3;
            relive_new MainMenuFade(stru_4D00E0[3].field_0_xpos, stru_4D00E0[3].field_2_ypos + 36, buttonType::eCircle_0, 1);
        }
        CycleGameSpeakIdleAnims();
        return;
    }

    if (Input().IsAnyPressed(InputObject::PadIndex::First, sInputKey_RightGameSpeakEnabler))
    {
        if (field_1F0_pObj2)
        {
            const FP xpos = FP_FromInteger(stru_4D00E0[12].field_0_xpos);
            if (field_1F0_pObj2->mXPos != xpos)
            {
                field_1F0_pObj2->mXPos = xpos;
                field_1F0_pObj2->mYPos = FP_FromInteger(stru_4D00E0[12].field_2_ypos + 36);
            }
        }
        else if (Input_JoyStickEnabled())
        {
            field_1F0_pObj2 = relive_new MainMenuFade(stru_4D00E0[12].field_0_xpos, stru_4D00E0[12].field_2_ypos + 36, buttonType::eCircle_0, 0);
        }

        if (Input().IsAnyHeld(InputObject::PadIndex::First, sInputKey_GameSpeak6))
        {
            Mudokon_SFX(MudSounds::eWhistleHigh_1, 0, 0, 0);
            field_204_flags |= 1u;
            mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_WhistleHigh));
            field_1E0_selected_index.gamespeak_menu = GameSpeakOptions::eWhistleHigh_4;
            relive_new MainMenuFade(stru_4D00E0[4].field_0_xpos, stru_4D00E0[4].field_2_ypos + 36, buttonType::eCircle_0, 1);
        }
        else if (Input().IsAnyHeld(InputObject::PadIndex::First, sInputKey_GameSpeak5))
        {
            Mudokon_SFX(MudSounds::eWhistleLow_2, 0, 0, 0);
            field_204_flags |= 1u;
            mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_WhistleLow));
            field_1E0_selected_index.gamespeak_menu = GameSpeakOptions::eWhistleLow_5;
            relive_new MainMenuFade(stru_4D00E0[5].field_0_xpos, stru_4D00E0[5].field_2_ypos + 36, buttonType::eCircle_0, 1);
        }
        else if (Input().IsAnyHeld(InputObject::PadIndex::First, sInputKey_GameSpeak8))
        {
            Mudokon_SFX(MudSounds::eLaugh2_11, 0, 0, 0);
            field_204_flags |= 1u;
            mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Laugh));
            field_1E0_selected_index.gamespeak_menu = GameSpeakOptions::eLaugh_6;
            relive_new MainMenuFade(stru_4D00E0[6].field_0_xpos, stru_4D00E0[6].field_2_ypos + 36, buttonType::eCircle_0, 1);
        }
        else if (Input().IsAnyHeld(InputObject::PadIndex::First, sInputKey_GameSpeak7))
        {
            Mudokon_SFX(MudSounds::eFart_7, 0, 0, 0);
            field_204_flags |= 1u;
            mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Fart));
            field_1E0_selected_index.gamespeak_menu = GameSpeakOptions::eFart_7;
            relive_new MainMenuFade(stru_4D00E0[7].field_0_xpos, stru_4D00E0[7].field_2_ypos + 36, buttonType::eCircle_0, 1);
        }
        CycleGameSpeakIdleAnims();
        return;
    }

    if (!Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eBack) && field_1DC_idle_input_counter <= 1600)
    {
        if (field_1F0_pObj2)
        {
            field_1F0_pObj2->field_E8_bDestroyOnDone = 1;
            field_1F0_pObj2 = nullptr;
        }
        CycleGameSpeakIdleAnims();
        return;
    }

    Mudokon_SFX(MudSounds::eGoodbye_12, 0, 0, 0);

    field_204_flags |= 1u;
    mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Goodbye));
    field_1E0_selected_index.gamespeak_menu = GameSpeakOptions::eMainMenu_9;

    relive_new MainMenuFade(stru_4D00E0[9].field_0_xpos, stru_4D00E0[9].field_2_ypos + 36, buttonType::eCircle_0, 1);

    field_1CC_fn_update = &Menu::GameSpeakBack_WaitForAbeGoodbye_Update_47D5E0;

    if (field_1EC_pObj1)
    {
        field_1EC_pObj1->field_E8_bDestroyOnDone = 1;
        field_1EC_pObj1 = nullptr;
    }

    if (field_1F0_pObj2)
    {
        field_1F0_pObj2->field_E8_bDestroyOnDone = 1;
        field_1F0_pObj2 = nullptr;
    }

    CycleGameSpeakIdleAnims();
}

void Menu::CycleGameSpeakIdleAnims()
{
    if (!(field_204_flags & 1))
    {
        if ((field_204_flags >> 2) & 1)
        {
            if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
            {
                mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
                field_204_flags &= ~4u;
                field_1D8_timer = sGnFrame + Math_RandomRange(120, 450);
            }
        }
        else
        {
            if (field_1D8_timer <= static_cast<s32>(sGnFrame))
            {
                if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
                {
                    mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_IdleBlink));
                    field_204_flags |= 4u;
                }
            }
        }
    }
}

void Menu::FMV_Or_Level_Select_To_Back_Update_47EC70()
{
    if (field_1E8_pMenuTrans->field_16_bDone)
    {
        gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eMainMenu_1, CameraSwapEffects::eInstantChange_0, 0, 0);
        field_1CC_fn_update = &Menu::FMV_Or_Level_Select_Back_Update_47ECB0;
    }
}

void Menu::To_Credits_Update_47F140()
{
    field_1CC_fn_update = &Menu::Credits_Update_47F190;
    field_1D0_fn_render = &Menu::Empty_Render_47AC80;
    field_1D8_timer = sGnFrame + 60;
}

void Menu::Level_Cheat_To_Loading_Update_47ED50()
{
    if (field_1E8_pMenuTrans->field_16_bDone)
    {
        field_1CC_fn_update = &Menu::ToLoading_47B7E0;
    }
}

void Menu::Options_Controller_Update_47F210()
{
    if (Input().IsAnyPressed(InputObject::PadIndex::First, InputCommands::eUp))
    {
        if (field_1E0_selected_index.raw > 0 && field_228 == FP_FromInteger(0))
        {
            field_1E0_selected_index.raw--;
            SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
        }
    }
    else if (Input().IsAnyPressed(InputObject::PadIndex::First, InputCommands::eDown | InputCommands::eCheatMode))
    {
        if (field_1E0_selected_index.raw < sAvailableControllers_4CE598 - 1 && field_228 == FP_FromInteger(0))
        {
            field_1E0_selected_index.raw++;
            SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
        }
    }

    if (Input().IsAnyPressed(InputObject::PadIndex::First, InputCommands::eBack | InputCommands::eHop))
    {
        field_230_bGoBack = 1;
        field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
        field_1CC_fn_update = &Menu::GoTo_ControllerConfigure_Or_Back_AfterScreenTrans_Update_47F330;
    }

    if (Input().IsAnyHeld(InputObject::PadIndex::First, (InputCommands::eThrowItem | InputCommands::eUnPause_OrConfirm | InputCommands::eDoAction)))
    {
        field_230_bGoBack = 0;
        field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
        field_1CC_fn_update = &Menu::GoTo_ControllerConfigure_Or_Back_AfterScreenTrans_Update_47F330;
    }
}

void Menu::GoTo_ControllerConfigure_Or_Back_AfterScreenTrans_Update_47F330()
{
    if (field_1E8_pMenuTrans)
    {
        if (field_1E8_pMenuTrans->field_16_bDone)
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

            field_1CC_fn_update = &Menu::Goto_ConfigureController_OrSave_SettingIni_Update_47F380;
        }
    }
}

void Menu::Goto_ConfigureController_OrSave_SettingIni_Update_47F380()
{
    if (sNumCamSwappers_507668 <= 0)
    {
        switch (field_230_bGoBack)
        {
            // Goto controller configuration
            case 0:
                field_204_flags &= ~2u;
                field_1CC_fn_update = &Menu::To_ButtonRemap_Update_47F860;
                field_1D0_fn_render = &Menu::ButtonRemap_Render_47F940;
                field_1E0_selected_index.remap_menu = RemapOptions::eRun_0;
                field_230_bGoBack = -1;
                break;

            // Back to main options (sound/controller)
            case 1:
            {
                Input_SaveSettingsIni();
                field_1CC_fn_update = &Menu::To_Options_Update_47C250;
                field_1D0_fn_render = &Menu::Options_Render_47C190;
                field_1E0_selected_index.options_menu = OptionsMenuOptions::eController_0;
                field_134_anim.Set_Animation_Data(GetAnimRes(sMainScreenButtons_4D00B0[0].field_4_anim_id));
                field_204_flags |= 2u;
                break;
            }

        }

        field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 0, 0, 16);
    }
}

const Menu_Element chooseAndExitRemapButtons_4D0690[2] = {
    {40, 34, InputCommands::eUnPause_OrConfirm},
    {301, 163, InputCommands::eBack}};

const Menu_Button sRemapScreenButtons_4D0170[10] = {
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


void Menu::ButtonRemap_Render_47F940(PrimHeader** ppOt)
{
    AnimId animId = AnimId::None;
    if (field_230_bGoBack == -1)
    {
        animId = sRemapScreenButtons_4D0170[field_1E0_selected_index.raw].field_4_anim_id;
    }
    else
    {
        animId = sRemapScreenButtons_4D0170[field_230_bGoBack].field_4_anim_id;
    }
    field_134_anim.Set_Animation_Data(GetAnimRes(animId));

    s32 polyOffset = 0;
    for (s32 i = 0; i < ALIVE_COUNTOF(sRemapScreenButtons_4D0170) - 2; i++)
    {
        RenderElement_47A4E0(
            sRemapScreenButtons_4D0170[i].field_0_xpos,
            sRemapScreenButtons_4D0170[i].field_2_ypos,
            sRemappableInputs_4D0030.buttons[Input_JoyStickEnabled() != 0][i],
            ppOt,
            &field_FC_font,
            &polyOffset);
    }

    if (field_230_bGoBack == -1)
    {
        field_134_anim.VRender(
            sRemapScreenButtons_4D0170[field_1E0_selected_index.raw].field_0_xpos - 3,
            sRemapScreenButtons_4D0170[field_1E0_selected_index.raw].field_2_ypos + 1,
            ppOt,
            0,
            0);
    }
    else
    {
        field_134_anim.VRender(
            sRemapScreenButtons_4D0170[field_230_bGoBack].field_0_xpos,
            sRemapScreenButtons_4D0170[field_230_bGoBack].field_2_ypos,
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
        s32 fontWidth = field_FC_font.MeasureScaledTextWidth(field_1F4_text, FP_FromInteger(1));
        s16 calculatedXposBasedOnWidth = 0;
        if (fontWidth >= maxFontWidth)
        {
            calculatedXposBasedOnWidth = 16;
        }
        else
        {
            calculatedXposBasedOnWidth = static_cast<s16>((368 - fontWidth) / 2);
        }
        s32 drawnStringOffset = field_FC_font.DrawString(
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
        sprintf(buffer, "for %s", inputActions_4D0070[field_1E0_selected_index.raw]);
        field_1F4_text = buffer;
        auto fontWidth2 = field_FC_font.MeasureScaledTextWidth(buffer, FP_FromInteger(1));
        s16 calculatedXposBasedOnWidth2 = 0;
        if (fontWidth2 >= maxFontWidth)
        {
            calculatedXposBasedOnWidth2 = 16;
        }
        else
        {
            calculatedXposBasedOnWidth2 = static_cast<s16>((368 - fontWidth2) / 2);
        }
        polyOffset = field_FC_font.DrawString(
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
        auto fontWidth3 = field_FC_font.MeasureScaledTextWidth("or Esc for none", FP_FromInteger(1));
        s16 calculatedXposBasedOnWidth3 = 0;
        if (fontWidth3 >= maxFontWidth)
        {
            calculatedXposBasedOnWidth3 = 16;
        }
        else
        {
            calculatedXposBasedOnWidth3 = static_cast<s16>((368 - fontWidth3) / 2);
        }
        polyOffset = field_FC_font.DrawString(
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
    field_134_anim.Get_Frame_Rect(&pRect);

    for (s32 i = 0; i < ALIVE_COUNTOF(chooseAndExitRemapButtons_4D0690); i++)
    {
        RenderElement_47A4E0(
            chooseAndExitRemapButtons_4D0690[i].field_0_xpos,
            chooseAndExitRemapButtons_4D0690[i].field_4_ypos,
            chooseAndExitRemapButtons_4D0690[i].field_8_input_command,
            ppOt,
            &field_FC_font,
            &polyOffset);
    }
}

void Menu::To_ButtonRemap_Update_47F860()
{
    field_230_bGoBack = -1;

    if (field_1E8_pMenuTrans)
    {
        if (field_1E8_pMenuTrans->field_16_bDone)
        {
            field_1CC_fn_update = &Menu::ButtonRemap_Update_47F6F0;
            field_1D0_fn_render = &Menu::ButtonRemap_Render_47F940;
            field_1DC_idle_input_counter = 0;
        }
    }
}

void Menu::ButtonRemap_Update_47F6F0()
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
        if (!Input_Remap(static_cast<InputCommands>(sRemappableInputs_4D0030.buttons[Input_JoyStickEnabled() ? 1 : 0][field_1E0_selected_index.raw])))
        {
            return;
        }

        field_230_bGoBack = -1;
        field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 0, 0, 16);
        bWaitingForRemapInput_9F2DE8 = 1;
        return;
    }

    if (Input().IsAnyPressed(InputObject::PadIndex::First, InputCommands::eLeft))
    {
        if (field_1E0_selected_index.remap_menu >= RemapOptions::eAction_4)
        {
            field_1E0_selected_index.raw -= 4;
        }
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400); // TODO: Input constants
        bWaitingForRemapInput_9F2DE8 = 1;
    }
    else if (Input().IsAnyPressed(InputObject::PadIndex::First, InputCommands::eRight))
    {
        if (field_1E0_selected_index.remap_menu < RemapOptions::eAction_4)
        {
            field_1E0_selected_index.raw += 4;
        }
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400); // TODO: Input constants
        bWaitingForRemapInput_9F2DE8 = 1;
    }
    else if (Input().IsAnyPressed(InputObject::PadIndex::First, InputCommands::eUp))
    {
        field_1E0_selected_index.raw--;
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400); // TODO: Input constants
        bWaitingForRemapInput_9F2DE8 = 1;

#if MISC_PC_MENU_FIXES // OG Change - Prevent users from changing Speak1 and Speak2 keys on keyboard
        if (!Input_JoyStickEnabled())
        {
            if (field_1E0_selected_index.remap_menu < RemapOptions::eRun_0)
            {
                field_1E0_selected_index.remap_menu = RemapOptions::eCrouch_6;
            }
            else if (field_1E0_selected_index.remap_menu == RemapOptions::eSpeakLeft_3 || field_1E0_selected_index.remap_menu == RemapOptions::eSpeakRight_7)
            {
                field_1E0_selected_index.raw--;
            }
        }
#endif
    }
    else if (Input().IsAnyPressed(InputObject::PadIndex::First, InputCommands::eDown | InputCommands::eCheatMode))
    {
        field_1E0_selected_index.raw++;
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400); // TODO: Input constants
        bWaitingForRemapInput_9F2DE8 = 1;

#if MISC_PC_MENU_FIXES // OG Change - Prevent users from changing Speak1 and Speak2 keys on keyboard
        if (!Input_JoyStickEnabled())
        {
            if (field_1E0_selected_index.remap_menu == RemapOptions::eSpeakLeft_3 || field_1E0_selected_index.remap_menu == RemapOptions::eSpeakRight_7)
            {
                field_1E0_selected_index.raw++;
            }
        }
#endif
    }

    if (field_1E0_selected_index.remap_menu < RemapOptions::eRun_0)
    {
        field_1E0_selected_index.remap_menu = RemapOptions::eSpeakRight_7;
    }

    if (field_1E0_selected_index.remap_menu > RemapOptions::eSpeakRight_7)
    {
        field_1E0_selected_index.remap_menu = RemapOptions::eRun_0;
    }

    if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eBack | InputCommands::eHop))
    {
        // Show abe motions screen
        field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
        field_230_bGoBack = 9;
        field_1CC_fn_update = &Menu::To_ShowAbeMotions_ChangeCamera_Update_47F8A0;
    }

    if (Input().IsAnyHeld(InputObject::PadIndex::First, (InputCommands::eThrowItem | InputCommands::eUnPause_OrConfirm | InputCommands::eDoAction)))
    {
        // Rebind a key (in that horrible white blinding screen)
        field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
        field_230_bGoBack = 8;
        bWaitingForRemapInput_9F2DE8 = 1;
    }
}

void Menu::To_LoadSave_Update_47DB10()
{
    if (field_1E8_pMenuTrans)
    {
        if (field_1E8_pMenuTrans->field_16_bDone)
        {
            field_1CC_fn_update = &Menu::LoadSave_Update_47DB40;
            field_1D0_fn_render = &Menu::Empty_Render_47AC80;
        }
    }
}

void Menu::LoadSave_Update_47DB40()
{
    if (field_1E8_pMenuTrans)
    {
        field_1E8_pMenuTrans->mBaseGameObjectRefCount--;
        field_1E8_pMenuTrans->mBaseGameObjectFlags.Set(Options::eDead);
        field_1E8_pMenuTrans = nullptr;
    }

    /*
    if (!field_E4_res_array[0])
    {
        ProgressInProgressFilesLoading();
    }*/

    if (!pPauseMenu_5080E0)
    {
        pPauseMenu_5080E0 = relive_new PauseMenu();
    }


    if (!sActiveHero)
    {
        sActiveHero = relive_new Abe();
    }

    if (!SaveGame::LoadFromFile(sSaveNames_9F1DD8[field_1E0_selected_index.raw].field_0_mName))
    {
        field_1CC_fn_update = &Menu::SaveLoadFailed_Update_47DCD0;
        field_1D0_fn_render = &Menu::SaveLoadFailed_Render_47DCF0;
        sActiveHero->mBaseGameObjectFlags.Set(Options::eDead);
    }
}

void Menu::SaveLoadFailed_Update_47DCD0()
{
    // Kill the pause menu and stay in this state - have to force restart the game when a save fails to load :)
    if (pPauseMenu_5080E0)
    {
        pPauseMenu_5080E0->mBaseGameObjectFlags.Set(Options::eDead);
        pPauseMenu_5080E0 = nullptr;
    }
}

void Menu::SaveLoadFailed_Render_47DCF0(PrimHeader** ppOt)
{
    // Note: This string in OG was just "Error" which is completely useless, changed to at least
    // give people a clue about what broke.
    const char_type* kErrStr = "Error loading save file";

    s16 xpos = 16;
    const s32 drawWidth = field_FC_font.DrawString(
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

    field_FC_font.DrawString(
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

void Menu::To_ShowAbeMotions_ChangeCamera_Update_47F8A0()
{
    if (field_1E8_pMenuTrans)
    {
        if (field_1E8_pMenuTrans->field_16_bDone)
        {
            if (field_230_bGoBack == 9)
            {
                gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eMotions_4, CameraSwapEffects::eInstantChange_0, 0, 0);
                field_1CC_fn_update = &Menu::To_ShowAbeMotions_SaveSettings_Update_47F8E0;
            }
        }
    }
}

void Menu::To_ShowAbeMotions_SaveSettings_Update_47F8E0()
{
    if (sNumCamSwappers_507668 <= 0)
    {
        field_134_anim.Set_Animation_Data(GetAnimRes(AnimId::MenuHighlight_Circle));
        Input_SaveSettingsIni();
        field_204_flags &= ~2u;
        field_1CC_fn_update = &Menu::To_ToggleMotions_Update_47C9E0;
        field_1D0_fn_render = &Menu::ToggleMotions_Render_47CAB0;
        field_1E0_selected_index.motions_menu = MotionsOptions::eMotions_0;
        field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 0, 0, 16);
    }
}

void Menu::To_ToggleMotions_Update_47C9E0()
{
    if (field_1E8_pMenuTrans)
    {
        if (field_1E8_pMenuTrans->field_16_bDone)
        {
            field_1CC_fn_update = &Menu::ToggleMotions_Update_47C800;
            field_1DC_idle_input_counter = 0;
        }
    }
}

void Menu::Credits_Update_47F190()
{
    if (field_1D8_timer <= static_cast<s32>(sGnFrame))
    {
        field_208_camera++;

        if (field_208_camera > 24)
        {
            // Credits done
            gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eFmvSelect_30, CameraSwapEffects::eInstantChange_0, 0, 0);
            field_1CC_fn_update = &Menu::CreditsEnd_BackTo_FMV_Or_Level_List_Update_47F170;
            gCreditsControllerExists = 0;
        }
        else
        {
            // Next credits screen
            gMap.SetActiveCam(EReliveLevelIds::eCredits, 1, static_cast<s16>(field_208_camera), CameraSwapEffects::eTopToBottom_3, 0, 0);
            field_1D8_timer = sGnFrame + 60;
        }
    }
}

void Menu::CreditsEnd_BackTo_FMV_Or_Level_List_Update_47F170()
{
    field_1CC_fn_update = &Menu::FMV_Select_Update_47E8D0;
    field_1D0_fn_render = &Menu::FMV_Or_Level_Select_Render_47EEA0;
}

void Menu::Load_Update_47D760()
{
    if (Input().Pressed(InputObject::PadIndex::First))
    {
        field_1DC_idle_input_counter = 0;
    }
    else
    {
        field_1DC_idle_input_counter++;
    }

    if ((Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eBack | InputCommands::eHop)
         && !field_1FE)
        || field_1DC_idle_input_counter > 1000)
    {
        field_1FA = 0;
#if !MISC_PC_MENU_FIXES // OG Change - Fixes automatic scroll when exiting Load Menu
        field_1E0_selected_index.raw = 1; // This line causes the issue
#endif
        field_134_anim.Set_Animation_Data(GetAnimRes(stru_4D01F0[1].field_4_anim_id));
        field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
        field_1CC_fn_update = &Menu::Load_BackToMainScreen_Update_47DA40;
        field_202 = 0;
    }

    if (Input().IsAnyPressed(InputObject::PadIndex::First, InputCommands::eUp))
    {
        if (field_1E0_selected_index.raw > 0 && field_228 == FP_FromInteger(0))
        {
            field_1E0_selected_index.raw--;

            SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);

            if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eBack | InputCommands::eHop))
            {
                field_230_bGoBack = 1;
                field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
                field_1CC_fn_update = &Menu::Load_BackToMainScreen_Update_47DA40;
                field_202 = 0;
                return;
            }
        }
    }
    else
    {
        if (Input().IsAnyPressed(InputObject::PadIndex::First, InputCommands::eDown | InputCommands::eCheatMode))
        {
            if (field_1E0_selected_index.raw < (sSaveIdx_9F2DD8 - 1) && field_228 == FP_FromInteger(0))
            {
                field_1E0_selected_index.raw++;

                SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);

                if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eBack | InputCommands::eHop))
                {
                    field_230_bGoBack = 1;
                    field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
                    field_1CC_fn_update = &Menu::Load_BackToMainScreen_Update_47DA40;
                    field_202 = 0;
                    return;
                }
            }
        }
    }

    if (Input().IsAnyHeld(InputObject::PadIndex::First, (InputCommands::eThrowItem | InputCommands::eUnPause_OrConfirm | InputCommands::eDoAction)))
    {
        if (sSaveIdx_9F2DD8)
        {
            field_230_bGoBack = 0;
            field_1FC = field_1E0_selected_index.raw;
            field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
            field_1CC_fn_update = &Menu::Load_BackToMainScreen_Update_47DA40;
            field_202 = 1;
        }
        else
        {
            field_230_bGoBack = 1;
            field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
            field_1CC_fn_update = &Menu::Load_BackToMainScreen_Update_47DA40;
            field_202 = 0;
        }
    }
}

s32 Menu::StringsEqual_47DA20(const void* pStr1, const void* pStr2)
{
    return _strcmpi(static_cast<const char_type*>(pStr1), static_cast<const char_type*>(pStr2));
}

void Menu::ToggleMotions_Render_47CAB0(PrimHeader** ppOt)
{
    field_134_anim.VRender(
        stru_4D01D8[field_1E0_selected_index.raw].field_0_xpos,
        stru_4D01D8[field_1E0_selected_index.raw].field_2_ypos,
        ppOt,
        0,
        0);

    if (field_1CC_fn_update == &Menu::ToggleMotions_Update_47C800)
    {
        s32 polyOffset = 0;
        for (s32 i = 2; i <= 10; i++)
        {
            RenderElement_47A4E0(sBtnArray_AbeMotionsMenuButtons_4D0418[i].field_0_xpos, sBtnArray_AbeMotionsMenuButtons_4D0418[i].field_4_ypos, sBtnArray_AbeMotionsMenuButtons_4D0418[i].field_8_input_command, ppOt, &field_FC_font, &polyOffset);
        }
    }
    else if (field_1CC_fn_update == &Menu::Toggle_Motions_Screens_Update_47C8F0)
    {
        s32 polyOffset = 0;
        if (Input_JoyStickEnabled())
        {
            for (s32 i = 0; i < 20; i++)
            {
                RenderElement_47A4E0(
                    sBtnArray_AbeGamespeakMenuButtons_4D04A0[i].field_0_xpos,
                    sBtnArray_AbeGamespeakMenuButtons_4D04A0[i].field_4_ypos,
                    sBtnArray_AbeGamespeakMenuButtons_4D04A0[i].field_8_input_command,
                    ppOt,
                    &field_FC_font,
                    &polyOffset);
            }
        }
        else
        {
            for (s32 i = 9; i < 11; i++)
            {
                RenderElement_47A4E0(sBtnArray_AbeMotionsMenuButtons_4D0418[i].field_0_xpos, sBtnArray_AbeMotionsMenuButtons_4D0418[i].field_4_ypos, sBtnArray_AbeMotionsMenuButtons_4D0418[i].field_8_input_command, ppOt, &field_FC_font, &polyOffset);
            }
        }
    }

    PSX_RECT rect = {};
    field_134_anim.Get_Frame_Rect(&rect);
}

void Menu::ToggleMotions_Update_47C800()
{
    if (Input().Pressed(InputObject::PadIndex::First))
    {
        field_1DC_idle_input_counter = 0;
    }
    else
    {
        field_1DC_idle_input_counter++;
    }

    if (sNumCamSwappers_507668 <= 0)
    {
        if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eUnPause_OrConfirm | InputCommands::eDoAction | InputCommands::eCheatMode))
        {
            if (Input_JoyStickEnabled())
            {
                gMap.SetActiveCameraDelayed(Map::MapDirections::eMapBottom_3, 0, -1);
            }
            else
            {
                gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eMotionsGamespeakKeyboard_37, CameraSwapEffects::eTopToBottom_3, 0, 0);
            }

            // Go to game speak toggle
            field_1CC_fn_update = &Menu::Toggle_Motions_Screens_Update_47C8F0;
            field_1E0_selected_index.motions_menu = MotionsOptions::eGameSpeak_1;
            PSX_Prevent_Rendering_44FFB0();
            SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
        }

        if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eBack | InputCommands::eHop) || field_1DC_idle_input_counter > 1600)
        {
            // Back to options
            field_1E0_selected_index.motions_menu = MotionsOptions::eExit_2;
            field_134_anim.Set_Animation_Data(GetAnimRes(stru_4D01D8[2].field_4_anim_id));
            field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
            field_1CC_fn_update = &Menu::MotionsScreen_Back_Update_47CA10;
        }
    }
}

void Menu::Toggle_Motions_Screens_Update_47C8F0()
{
    if (Input().Pressed(InputObject::PadIndex::First))
    {
        field_1DC_idle_input_counter = 0;
    }
    else
    {
        field_1DC_idle_input_counter++;
    }

    if (sNumCamSwappers_507668 <= 0)
    {
        if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eUnPause_OrConfirm | InputCommands::eDoAction | InputCommands::eCheatMode))
        {
            if (Input_JoyStickEnabled())
            {
                gMap.SetActiveCameraDelayed(Map::MapDirections::eMapTop_2, 0, -1);
            }
            else
            {
                gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eMotions_4, CameraSwapEffects::eBottomToTop_4, 0, 0);
            }

            field_1CC_fn_update = &Menu::ToggleMotions_Update_47C800;
            field_1E0_selected_index.motions_menu = MotionsOptions::eMotions_0;
            PSX_Prevent_Rendering_44FFB0();
            SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
        }

        if (Input().IsAnyHeld(InputObject::PadIndex::First, InputCommands::eBack | InputCommands::eHop) || field_1DC_idle_input_counter > 1600)
        {
            field_1E0_selected_index.motions_menu = MotionsOptions::eExit_2;
            field_134_anim.Set_Animation_Data(GetAnimRes(stru_4D01D8[2].field_4_anim_id));
            field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
            field_1CC_fn_update = &Menu::MotionsScreen_Back_Update_47CA10;
        }
    }
}

void Menu::MotionsScreen_Back_Update_47CA10()
{
    if (field_1E8_pMenuTrans)
    {
        if (field_1E8_pMenuTrans->field_16_bDone)
        {
            gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eOptions_2, CameraSwapEffects::eInstantChange_0, 0, 0);
            field_1CC_fn_update = &Menu::Motions_ToOptions_Update_47CA50;
        }
    }
}

void Menu::Motions_ToOptions_Update_47CA50()
{
    if (sNumCamSwappers_507668 <= 0)
    {
        field_204_flags |= 2u;
        field_1CC_fn_update = &Menu::To_Options_Update_47C250;
        field_1D0_fn_render = &Menu::Options_Render_47C190;
        field_1E0_selected_index.options_menu = OptionsMenuOptions::eController_0;
        field_134_anim.Set_Animation_Data(GetAnimRes(sOptionsButtons_4D0148[0].field_4_anim_id));
        field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 0, 0, 16);
    }
}

void Menu::To_MainScreenOrLoad_Update_47DA90()
{
    if (sNumCamSwappers_507668 <= 0)
    {
        field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 0, 0, 16);
        if (field_202)
        {
            field_1CC_fn_update = &Menu::To_LoadSave_Update_47DB10;
            field_1D0_fn_render = &Menu::Empty_Render_47AC80;
        }
        else
        {
            field_1E0_selected_index.mainmenu = MainMenuOptions::eLoad_3;
            field_134_anim.Set_Animation_Data(GetAnimRes(sMainScreenButtons_4D00B0[3].field_4_anim_id));
            field_204_flags |= 2u;
            field_1CC_fn_update = &Menu::To_MainScreen_Update_47BB60;
            field_1D0_fn_render = &Menu::MainScreen_Render_47BED0;
        }
    }
}

void Menu::Load_BackToMainScreen_Update_47DA40()
{
    if (field_1E8_pMenuTrans)
    {
        if (field_1E8_pMenuTrans->field_16_bDone)
        {
            if (field_202 == 0)
            {
                gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eMainMenu_1, CameraSwapEffects::eInstantChange_0, 0, 0);
            }
            else
            {
                gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eLoading_21, CameraSwapEffects::eInstantChange_0, 0, 0);
            }
            field_200 = 0;
            field_1CC_fn_update = &Menu::To_MainScreenOrLoad_Update_47DA90;
        }
    }
}

void Menu::GameSpeakBack_WaitForAbeGoodbye_Update_47D5E0()
{
    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        mAnim.Set_Animation_Data(GetAnimRes(AnimId::MenuAbeSpeak_Idle));
        field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 1, 0, 16);
        field_1CC_fn_update = &Menu::GamespeakBack_WaitForScreenTrans_Update_47D650;
    }
}

void Menu::GamespeakBack_WaitForScreenTrans_Update_47D650()
{
    if (field_1E8_pMenuTrans)
    {
        if (field_1E8_pMenuTrans->field_16_bDone)
        {
            field_204_flags &= ~1u;
            gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eMainMenu_1, CameraSwapEffects::eInstantChange_0, 0, 0);
            field_1CC_fn_update = &Menu::GameSpeak_To_MainScreen_Update_47D690;
        }
    }
}

void Menu::GameSpeak_To_MainScreen_Update_47D690()
{
    if (sNumCamSwappers_507668 <= 0)
    {
        field_1CC_fn_update = &Menu::To_MainScreen_Update_47BB60;
        field_1D0_fn_render = &Menu::MainScreen_Render_47BED0;
        field_1E0_selected_index.mainmenu = MainMenuOptions::eGameSpeak_0;
        field_134_anim.Set_Animation_Data(GetAnimRes(sMainScreenButtons_4D00B0[0].field_4_anim_id));
        field_1E8_pMenuTrans->StartTrans_436560(Layer::eLayer_FadeFlash_40, 0, 0, 16);
        field_134_anim.LoadPal(GetPalRes(PalId::WhiteHighlite));
    }
}

void Menu::OnResourceLoaded_47ADA0(Menu* /*pMenu*/)
{
    
}

void Menu::RenderElement_47A4E0(s32 xpos, s32 ypos, s32 input_command, PrimHeader** ot, AliveFont* pFont, s32* pPolyOffset)
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

    const u8 bOldValue = sFontDrawScreenSpace_508BF4;
    sFontDrawScreenSpace_508BF4 = 1;

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

    sFontDrawScreenSpace_508BF4 = bOldValue;
}

} // namespace AO
