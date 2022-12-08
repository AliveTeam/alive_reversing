#include "stdafx.h"
#include "PauseMenu.hpp"
#include "stdlib.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "StringFormatters.hpp"
#include "Abe.hpp"
#include "Sfx.hpp"
#include "Sound/Sound.hpp"
#include "MainMenu.hpp"
#include "ThrowableArray.hpp"
#include "QuikSave.hpp"
#include "Mudokon.hpp"
#include "Sys.hpp"
#include "PathDataExtensions.hpp"
#include "GameAutoPlayer.hpp"
#include "EvilFart.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "DDCheat.hpp"
#include "Sound/Midi.hpp"
#include "ResourceManager.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "Game.hpp"

s8 sQuicksave_SaveNextFrame_5CA4D8 = 0;
s8 sQuicksave_LoadNextFrame_5CA4D9 = 0;

char_type sScreenStringBuffer_5C92F0[16] = {0};


char_type sSaveString_5C931C[32] = {};

// Menu Text Data

PauseMenuPageEntry PauseMenu__PageEntryList_Main_55E1C8[11] = {
    {2, 184, 48, 0, "continue", 128u, 16u, 255u, 1u},
    {2, 184, 70, 0, "quiksave", 128u, 16u, 255u, 1u},
    {2, 184, 92, 0, "controls", 128u, 16u, 255u, 1u},
    {2, 184, 114, 0, "status", 128u, 16u, 255u, 1u},
    {2, 184, 136, 0, "save", 128u, 16u, 255u, 1u},
    {2, 184, 158, 0, "load", 128u, 16u, 255u, 1u},
    {2, 184, 180, 0, "restart path", 128u, 16u, 255u, 1u},
    {2, 184, 202, 0, "quit", 128u, 16u, 255u, 1u},
    {1, 184, 16, 0, "paused", 128u, 16u, 255u, 1u},
    {1, 280, 16, 0, sScreenStringBuffer_5C92F0, 128u, 16u, 255u, 0u},
    {0, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u}};


PauseMenuPageEntry PauseMenu__PageEntryList_ControlActions_55d820[17] = {
    {2, 184, 205, 0, "more", 128, 16, 255, Centre},
    {1, 184, 20, 0, "Actions", 127, 127, 127, Centre},
    {1, 180, 50, 0, kRun "+" kLeft kRight, 160, 160, 160, Right},
    {1, 180, 70, 0, kSneak "+" kLeft kRight, 160, 160, 160, Right},
    {1, 180, 90, 0, kJump, 160, 160, 160, Right},
    {1, 180, 110, 0, kThrow "+" kDPad, 160, 160, 160, Right},
    {1, 180, 135, 0, kDown, 160, 160, 160, Right},
    {1, 180, 150, 0, kAction, 160, 160, 160, Right},
    {1, 180, 173, 0, kUp, 160, 160, 160, Right},
    {1, 200, 50, 0, "run", 128, 16, 255, Left},
    {1, 200, 70, 0, "sneak", 128, 16, 255, Left},
    {1, 200, 90, 0, "jump", 128, 16, 255, Left},
    {1, 200, 110, 0, "throw", 128, 16, 255, Left},
    {1, 200, 130, 0, "crouch", 128, 16, 255, Left},
    {1, 200, 150, 0, "action", 128, 16, 255, Left},
    {1, 200, 170, 0, "hoist " kHoistZTurn " zturn", 128, 16, 255, Left},
    {1, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u}};

PauseMenuPageEntry PauseMenu__PageEntryList_GameSpeak_55d930[21] = {
    {2, 184, 205, 0, "more", 128, 16, 255, Centre},
    {1, 184, 20, 0, "GameSpeak", 127, 127, 127, Centre},
    {1, 184, 55, 0, kChant, 160, 160, 160, Centre},
    {1, 184, 75, 0, "chant", 128, 16, 255, Centre},
    {1, 110, 104, 0, "hello", 128, 16, 255, Left},
    {1, 110, 126, 0, "follow me", 128, 16, 255, Left},
    {1, 110, 148, 0, "wait", 128, 16, 255, Left},
    {1, 110, 170, 0, "work", 128, 16, 255, Left},
    {1, 278, 104, 0, "anger", 128, 16, 255, Left},
    {1, 278, 126, 0, "all 'ya", 128, 16, 255, Left},
    {1, 278, 148, 0, "sympathy", 128, 16, 255, Left},
    {1, 278, 170, 0, "stop it!", 128, 16, 255, Left},
    {1, 90, 104, 0, kHello, 160, 160, 160, Right},
    {1, 90, 126, 0, kFollowMe, 160, 160, 160, Right},
    {1, 90, 148, 0, kWait, 160, 160, 160, Right},
    {1, 90, 170, 0, kWork, 160, 160, 160, Right},
    {1, 260, 104, 0, kAnger, 160, 160, 160, Right},
    {1, 260, 126, 0, kAllYa, 160, 160, 160, Right},
    {1, 260, 148, 0, kSorry, 160, 160, 160, Right},
    {1, 260, 170, 0, kStopIt, 160, 160, 160, Right},
    {1, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u}};

PauseMenuPageEntry PauseMenu__PageEntryList_SligSpeak_55da80[19] = {
    {2, 184, 205, 0, "more", 128, 16, 255, Centre},
    {1, 184, 20, 0, "SligSpeak", 127, 127, 127, Centre},
    {1, 110, 104, 0, "hi", 128, 16, 255, Left},
    {1, 110, 126, 0, "here boy", 128, 16, 255, Left},
    {1, 110, 148, 0, "freeze", 128, 16, 255, Left},
    {1, 110, 170, 0, "get 'im", 128, 16, 255, Left},
    {1, 278, 104, 0, "s'mo bs", 128, 16, 255, Left},
    {1, 278, 126, 0, "bs", 128, 16, 255, Left},
    {1, 278, 148, 0, "look out", 128, 16, 255, Left},
    {1, 278, 170, 0, "laugh", 128, 16, 255, Left},
    {1, 90, 104, 0, kHello, 160, 160, 160, Right},
    {1, 90, 126, 0, kFollowMe, 160, 160, 160, Right},
    {1, 90, 148, 0, kWait, 160, 160, 160, Right},
    {1, 90, 170, 0, kWork, 160, 160, 160, Right},
    {1, 260, 104, 0, kAnger, 160, 160, 160, Right},
    {1, 260, 126, 0, kAllYa, 160, 160, 160, Right},
    {1, 260, 148, 0, kSorry, 160, 160, 160, Right},
    {1, 260, 170, 0, kStopIt, 160, 160, 160, Right},
    {1, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u}};

PauseMenuPageEntry PauseMenu__PageEntryList_GlukkonSpeak_55dbb0[19] = {
    {2, 184, 205, 0, "more", 128, 16, 255, Centre},
    {1, 184, 20, 0, "GlukkonSpeak", 127, 127, 127, Centre},
    {1, 110, 104, 0, "hey!", 128, 16, 255, Left},
    {1, 110, 126, 0, "commere", 128, 16, 255, Left},
    {1, 110, 148, 0, "stay here", 128, 16, 255, Left},
    {1, 110, 170, 0, "do it!", 128, 16, 255, Left},
    {1, 278, 104, 0, "kill'em!", 128, 16, 255, Left},
    {1, 278, 126, 0, "all o'ya", 128, 16, 255, Left},
    {1, 278, 148, 0, "help!", 128, 16, 255, Left},
    {1, 278, 170, 0, "laugh", 128, 16, 255, Left},
    {1, 90, 104, 0, kHello, 160, 160, 160, Right},
    {1, 90, 126, 0, kFollowMe, 160, 160, 160, Right},
    {1, 90, 148, 0, kWait, 160, 160, 160, Right},
    {1, 90, 170, 0, kWork, 160, 160, 160, Right},
    {1, 260, 104, 0, kAnger, 160, 160, 160, Right},
    {1, 260, 126, 0, kAllYa, 160, 160, 160, Right},
    {1, 260, 148, 0, kSorry, 160, 160, 160, Right},
    {1, 260, 170, 0, kStopIt, 160, 160, 160, Right},
    {1, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u}};

PauseMenuPageEntry PauseMenu__PageEntryList_ParamiteSpeak_55dce0[15] = {
    {2, 184, 205, 0, "more", 128, 16, 255, Centre},
    {1, 184, 20, 0, "ParamiteSpeak", 127, 127, 127, Centre},
    {1, 110, 104, 0, "howdy", 128, 16, 255, Left},
    {1, 110, 126, 0, "c'mon", 128, 16, 255, Left},
    {1, 110, 148, 0, "stay", 128, 16, 255, Left},
    {1, 110, 170, 0, "do it", 128, 16, 255, Left},
    {1, 278, 104, 0, "attack", 128, 16, 255, Left},
    {1, 278, 126, 0, "all a ya!", 128, 16, 255, Left},
    {1, 90, 104, 0, kHello, 160, 160, 160, Right},
    {1, 90, 126, 0, kFollowMe, 160, 160, 160, Right},
    {1, 90, 148, 0, kWait, 160, 160, 160, Right},
    {1, 90, 170, 0, kWork, 160, 160, 160, Right},
    {1, 260, 104, 0, kAnger, 160, 160, 160, Right},
    {1, 260, 126, 0, kAllYa, 160, 160, 160, Right},
    {1, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u}};

PauseMenuPageEntry PauseMenu__PageEntryList_ScrabSpeak_55ddd0[7] = {
    {2, 184, 205, 0, "exit", 128, 16, 255, Centre},
    {1, 184, 20, 0, "ScrabSpeak", 127, 127, 127, Centre},
    {1, 110, 104, 0, "shred power", 128, 16, 255, Left},
    {1, 110, 126, 0, "howl", 128, 16, 255, Left},
    {1, 90, 104, 0, kHello, 160, 160, 160, Right},
    {1, 90, 126, 0, kFollowMe, 160, 160, 160, Right},
    {1, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u}};

PauseMenuPageEntry PauseMenu__PageEntryList_Save_55e4c8[4] = {
    {1, 184, 120, 0, sSaveString_5C931C, 128, 16, 255, Centre},
    {1, 184, 180, 0, "Enter   Save", 128, 16, 255, Centre},
    {1, 184, 205, 0, "Esc   Cancel", 128, 16, 255, Centre},
    {1, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u}};

PauseMenuPageEntry PauseMenu__PageEntryList_ReallyQuit_55e278[3] = {
    {1, 184, 110, 0, "REALLY QUIT?", 128, 16, 255, Centre},
    {1, 184, 135, 0, "Enter yes   Esc no", 160, 160, 160, Centre},
    {1, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u}};

PauseMenuPageEntry PauseMenu__PageEntryList_Status_55e738[1] = {
    {1, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u}};

PauseMenuPageEntry PauseMenu__PageEntryList_Load_55e3a0[9] = {
    {1, 184, 5, 0, "", 128, 16, 255, Centre},
    {1, 184, 30, 0, "", 128, 16, 255, Centre},
    {1, 184, 55, 0, "", 128, 16, 255, Centre},
    {1, 184, 80, 0, "", 128, 16, 255, Centre},
    {1, 184, 105, 0, "", 128, 16, 255, Centre},
    {1, 184, 130, 0, "", 128, 16, 255, Centre},
    {1, 184, 188, 0, "", 128, 16, 255, Centre},
    {1, 184, 213, 0, "Esc  Cancel        F6  Load QuikSave", 128, 16, 255, Centre},
    {1, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u}};


// Menu Pages

PauseMenu::PauseMenuPage sPM_Page_Main_5465B0 = {
    &PauseMenu::Page_Main_Update,
    &PauseMenu::Page_Base_Render,
    PauseMenu__PageEntryList_Main_55E1C8,
    0,
    100u,
    100u,
    100u,
    0u,
    0u,
    0u};

PauseMenu::PauseMenuPage sPM_Page_Controls_Actions_546610 = {
    &PauseMenu::Page_ControlsActions_Update,
    &PauseMenu::Page_Base_Render,
    PauseMenu__PageEntryList_ControlActions_55d820,
    0,
    100u,
    100u,
    100u,
    0u,
    0u,
    0u};

PauseMenu::PauseMenuPage sPM_Page_ReallyQuit_5465E0 = {
    &PauseMenu::Page_ReallyQuit_Update,
    &PauseMenu::Page_Base_Render,
    PauseMenu__PageEntryList_ReallyQuit_55e278,
    -1,
    100u,
    100u,
    100u,
    0u,
    0u,
    0u};

PauseMenu::PauseMenuPage sPM_Page_Load_546628 = {
    &PauseMenu::Page_Load_Update,
    &PauseMenu::Page_Load_Render,
    PauseMenu__PageEntryList_Load_55e3a0,
    0,
    static_cast<s8>(160u),
    static_cast<s8>(160u),
    static_cast<s8>(160u),
    0u,
    0u,
    0u};

PauseMenuPageEntry PauseMenu__PageEntryList_Save_55E4C8[4] = {
    {1, 184, 120, 0, sSaveString_5C931C, 128u, 16u, 255u, 1u},
    {1, 184, 180, 0, "Enter   Save", 128u, 16u, 255u, 1u},
    {1, 184, 205, 0, "Esc   Cancel", 128u, 16u, 255u, 1u},
    {1, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u}};

PauseMenuPageEntry PauseMenu__PageEntryList_Save_Overwrite_Confirm_55E508[2] = {
    {1, 184, 180, 0, "Enter Overwrite  Esc Cancel", 160u, 160u, 160u, 1u},
    {1, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u}};

PauseMenu::PauseMenuPage sPM_Page_Save_5465C8 = {
    &PauseMenu::Page_Save_Update,
    &PauseMenu::Page_Save_Render,
    &PauseMenu__PageEntryList_Save_55E4C8[0],
    0,
    static_cast<s8>(160u),
    static_cast<s8>(160u),
    static_cast<s8>(160u),
    0,
    0,
    0};

char_type sPauseMenu_Of300Mudokons_55E718[32] = {};
char_type sHasBeenTerminated_55E738[56] = {};

PauseMenuPageEntry sStatusEntries_55E758[6] = {
    {2, 184, 205, 0, "EXIT", 128u, 16u, 255u, 1u},
    {1, 184, 20, 0, "ABE'S STATUS", 127u, 127u, 127u, 1u},
    {1, 184, 145, 0, sPauseMenu_Of300Mudokons_55E718, 128u, 16u, 255u, 1u},
    {1, 184, 170, 0, sHasBeenTerminated_55E738, 128u, 16u, 255u, 1u},
    {1, 184, 120, 0, "YOU HAVE RESCUED", 128u, 16u, 255u, 1u},
    {0, 0, 0, 0, nullptr, 0u, 0u, 0u, 0u}};

PauseMenu::PauseMenuPage sPM_Page_Status_5465F8 = {
    &PauseMenu::Page_Status_Update,
    &PauseMenu::Page_Status_Render,
    &sStatusEntries_55E758[0],
    0,
    100u,
    100u,
    100u,
    0u,
    0u,
    0u};

PauseMenuPageEntry* sControlActionsPages_55DE40[6] =
{
    PauseMenu__PageEntryList_ControlActions_55d820,
    PauseMenu__PageEntryList_GameSpeak_55d930,
    PauseMenu__PageEntryList_SligSpeak_55da80,
    PauseMenu__PageEntryList_GlukkonSpeak_55dbb0,
    PauseMenu__PageEntryList_ParamiteSpeak_55dce0,
    PauseMenu__PageEntryList_ScrabSpeak_55ddd0
};

void PauseMenu::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::NormalMudIcon));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::AngryMudIcon));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::HappyMudIcon));
}

PauseMenu::PauseMenu()
    : BaseAnimatedWithPhysicsGameObject(0)
{
    sQuicksave_SaveNextFrame_5CA4D8 = 0;
    sQuicksave_LoadNextFrame_5CA4D9 = 0;

    LoadAnimations();

    SetType(ReliveTypes::ePauseMenu);
    mBaseGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);
    SetSurviveDeathReset(true);
    word12C_flags &= ~0xE;
    word12C_flags &= ~1u;

    SetUpdateDelay(25);

    gObjListDrawables->Push_Back(this);

    field_136_unused = 0;
    field_138_control_action_page_index = 0;
    field_13A_unused = 0;
    field_13C_save_state = SaveState::ReadingInput_0;
    field_13E_unused = 0;
    field_140_unused = 0;

    mFontContext.LoadFontType(FontType::PauseMenu);
    mPal = ResourceManagerWrapper::LoadPal(PalId::MainMenuFont_PauseMenu);
    field_F4_font.Load(256, mPal, &mFontContext);

    Init();

    sDisableFontFlicker = 0;
}

PauseMenu::~PauseMenu()
{
    mBaseGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);

    gObjListDrawables->Remove_Item(this);
    field_158_animation.VCleanUp();
}

void PauseMenu::Init()
{
    if (field_158_animation.Init(GetAnimRes(AnimId::NormalMudIcon), this))
    {
        this->field_158_animation.SetRenderLayer(GetAnimation().GetRenderLayer());
        this->field_158_animation.SetSpriteScale(GetSpriteScale());
        this->field_158_animation.SetRGB(127, 127, 127);
    }
    else
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eListAddFailed_Bit1);
    }
}

void PauseMenu::VRender(PrimHeader** ot)
{
    field_142_poly_offset = 0;

    // Render the page
    (this->*field_144_active_menu.field_4_fn_render)(ot, &field_144_active_menu);

    // Draw a full screen polygon that "dims" out the screen while paused
    Prim_SetTPage* pTPage = &field_1F0_primitives[gPsxDisplay.mBufferIndex];
    Poly_G4* pPolys = &field_210_polygons[gPsxDisplay.mBufferIndex];
    PolyG4_Init(pPolys);
    Poly_Set_SemiTrans(&pPolys->mBase.header, true);
    Poly_Set_Blending(&pPolys->mBase.header, false);
    SetRGB0(pPolys, field_144_active_menu.field_E_background_r, field_144_active_menu.field_F_background_g, field_144_active_menu.field_10_background_b);
    SetRGB1(pPolys, field_144_active_menu.field_E_background_r, field_144_active_menu.field_F_background_g, field_144_active_menu.field_10_background_b);
    SetRGB2(pPolys, field_144_active_menu.field_E_background_r, field_144_active_menu.field_F_background_g, field_144_active_menu.field_10_background_b);
    SetXY0(pPolys, 0, 0);
    SetXY1(pPolys, 640, 0);
    SetXY2(pPolys, 0, 240);
    SetXY3(pPolys, 640, 240);
    Init_SetTPage(pTPage, 0, 0, PSX_getTPage(TPageAbr::eBlend_2));
    OrderingTable_Add(OtLayer(ot, Layer::eLayer_Menu_41), &pPolys->mBase.header);
    OrderingTable_Add(OtLayer(ot, Layer::eLayer_Menu_41), &pTPage->mBase);
}

void PauseMenu::VScreenChanged()
{
    if (gMap.mNextLevel == EReliveLevelIds::eCredits)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void PauseMenu::Page_Base_Render(PrimHeader** ot, PauseMenu::PauseMenuPage* mp)
{
    s32 i = 0;
    PauseMenuPageEntry* e = &mp->field_8_menu_items[i];

    while (e->field_8_text)
    {
        char_type textFormatted[128];
        String_FormatString(e->field_8_text, textFormatted, 128, 1);

        auto glow = 0;
        if (mp->field_C_selected_index == i)
        {
            glow = field_12E_selected_glow;
        }

        auto x = e->field_2_x;

        if (e->field_F_alignment != 0)
        {
            s16 textMeasure = static_cast<s16>(field_F4_font.MeasureTextWidth(textFormatted));

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

        field_142_poly_offset = static_cast<s16>(field_F4_font.DrawString(
            ot,
            textFormatted,
            x,            // X
            e->y, // Y
            TPageAbr::eBlend_0,
            1,
            0,
            Layer::eLayer_Menu_41,
            static_cast<s8>(glow + e->field_C_r),
            static_cast<s8>(glow + e->field_D_g),
            static_cast<s8>(glow + e->field_E_b),
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

    Quicksave_SaveSwitchResetterStates();

    gSwitchStates = sActiveQuicksaveData.field_35C_restart_path_switch_states;

    Abe::CreateFromSaveState(sActiveQuicksaveData.field_284_restart_path_abe_state);
    Quicksave_ReadWorldInfo(&sActiveQuicksaveData.field_244_restart_path_world_info);

    gMap.SetActiveCam(
        MapWrapper::FromAE(sActiveQuicksaveData.field_244_restart_path_world_info.field_4_level),
        sActiveQuicksaveData.field_244_restart_path_world_info.field_6_path,
        sActiveQuicksaveData.field_244_restart_path_world_info.field_8_cam,
        CameraSwapEffects::eInstantChange_0,
        1,
        1);

    gMap.mForceLoad = true;
    if (sActiveHero->mBaseThrowableCount)
    {
        LoadRockTypes_49AB30(
            MapWrapper::FromAE(sActiveQuicksaveData.field_244_restart_path_world_info.field_4_level),
            sActiveQuicksaveData.field_244_restart_path_world_info.field_6_path);

        if (!gpThrowableArray)
        {
            gpThrowableArray = relive_new ThrowableArray();
        }

        gpThrowableArray->Add(sActiveHero->mBaseThrowableCount);
    }

    word12C_flags &= ~1;
    SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 3400);
    GetSoundAPI().SND_Restart();
}

const char_type kArrowChar = 3;
char_type sArrowStr_55E398[2] = {kArrowChar, 0};

void PauseMenu::Page_Main_Update()
{
    if (Input().isHeld(InputCommands::Enum::eDown))
    {
        if (++field_134_index_main > MainPages::ePage_Quit_7)
        {
            field_134_index_main = MainPages::ePage_Continue_0;
        }
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
    }

    if (Input().isHeld(InputCommands::Enum::eUp))
    {
        if (--field_134_index_main < MainPages::ePage_Continue_0)
        {
            field_134_index_main = MainPages::ePage_Quit_7;
        }
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
    }

    field_144_active_menu.field_C_selected_index = field_134_index_main;

    if (Input().isHeld(InputCommands::Enum::eBack))
    {
        word12C_flags &= ~1u;
        SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
        GetSoundAPI().SND_Restart();
    }
    else if (Input().isHeld(InputCommands::Enum::eUnPause_OrConfirm))
    {
        switch (field_134_index_main)
        {
            case MainPages::ePage_Continue_0:
                word12C_flags &= ~1u;
                SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
                GetSoundAPI().SND_Restart();
                return;

            case MainPages::ePage_QuickSave_1:
                word12C_flags &= ~1u;
                SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
                GetSoundAPI().SND_Restart();
                DoQuicksave();
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
                SfxPlayMono(relive::SoundEffects::IngameTransition, 90);
                field_13C_save_state = SaveState::ReadingInput_0;
                word12C_flags &= ~0xA;
                field_13E_unused = -1;
                word12C_flags |= 0x400;
                field_13A_unused = 0;
                DoQuicksave();
                // Set the default save name to be the current level/path/camera
                Path_Format_CameraName(
                    sSaveString_5C931C,
                    gMap.mCurrentLevel,
                    gMap.mCurrentPath,
                    gMap.mCurrentCamera);
                // Null terminate it
                sSaveString_5C931C[8] = 0;
                // Append the editor arrow s8
                strcat(sSaveString_5C931C, sArrowStr_55E398);

#if ORIGINAL_PS1_BEHAVIOR // OG Change - Allow for exiting save menu using controller
                setSaveMenuOpen(true); // Sets saveMenuOpen bool to true, instead of disabling input
#else
                Input_DisableInputForPauseMenuAndDebug_4EDDC0();
#endif
                return;

            case MainPages::ePage_Load_5:
                Quicksave_FindSaves();
                field_136_unused = 4;
                field_144_active_menu = sPM_Page_Load_546628;
                SfxPlayMono(relive::SoundEffects::IngameTransition, 90);
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

        SfxPlayMono(relive::SoundEffects::IngameTransition, 90);
    }
}

void PauseMenu::Page_ControlsActions_Update()
{
    if (Input().isHeld(InputCommands::Enum::eBack))
    {
        field_136_unused = 0;
        field_144_active_menu = sPM_Page_Main_5465B0;
        SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
    }

    if (Input().isHeld(0x100000))
    {
        const s32 prev = ++field_138_control_action_page_index;
        if (prev < 6)
        {
            field_144_active_menu.field_8_menu_items = sControlActionsPages_55DE40[prev];
            SfxPlayMono(relive::SoundEffects::IngameTransition, 90);
        }
        else
        {
            field_138_control_action_page_index = 0;
            field_136_unused = 0;
            field_144_active_menu = sPM_Page_Main_5465B0;
            SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
        }
    }
}

void PauseMenu::Page_ReallyQuit_Update()
{
    if (Input().isHeld(InputCommands::Enum::eBack))
    {
        field_136_unused = 0;
        field_144_active_menu = sPM_Page_Main_5465B0;
        SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
    }

    if (Input().isHeld(0x100000))
    {
        word12C_flags &= ~1u;
        SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);

        if (gPauseMenu && gPauseMenu == this)
        {
            gPauseMenu->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
        else
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }

        gPauseMenu = 0;
        gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, 1, CameraSwapEffects::eInstantChange_0, 0, 0);
        gMap.mFreeAllAnimAndPalts = 1;
        sCurrentControllerIndex = 0;
    }
}

void PauseMenu::Page_Save_Update()
{
    static bool bWriteSaveFile_5C937C = false;

    char_type newInput[2] = {};
    char_type savFileName[40] = {};
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
                fwrite(&sActiveQuicksaveData, sizeof(Quicksave), 1u, hFile);
                fclose(hFile);
                sSavedGameToLoadIdx_BB43FC = 0;
            }
            word12C_flags &= ~1u;
            SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
            GetSoundAPI().SND_Restart();
        }
        else
        {
            field_13C_save_state = SaveState::SaveConfirmOverwrite_8;
        }
    }
    else if (field_13C_save_state == SaveState::SaveConfirmOverwrite_8)
    {
        if (Input().isHeld(InputCommands::Enum::eUnPause_OrConfirm))
        {
            // Enter - do the save and don't return to the confirm overwrite
            SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
            field_13C_save_state = SaveState::DoSave_4;
            bWriteSaveFile_5C937C = true;
        }
        else if (Input().isHeld(InputCommands::Enum::eBack))
        {
            // Escape - cancel save
            SfxPlayMono(relive::SoundEffects::IngameTransition, 90);
            field_13C_save_state = SaveState::ReadingInput_0;
            strcat(sSaveString_5C931C, sArrowStr_55E398);
#if ORIGINAL_PS1_BEHAVIOR // OG Change - Allow for exiting save menu using controller
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
        u32 lastPressed = Input_GetLastPressedKey_492610();

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
        const u32 lastPressed = Input_GetLastPressedKey_492610();
#endif

        if (!lastPressed)
        {
            return;
        }

        newInput[0] = static_cast<s8>(lastPressed);
        newInput[1] = 0;

        const size_t stringLen = strlen(sSaveString_5C931C);

        switch (lastPressed)
        {
            case VK_SHIFT:
                return;
            // Escape - cancel
            case VK_ESCAPE:
                SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
                field_136_unused = 0;
                field_144_active_menu = sPM_Page_Main_5465B0;
                Input_Reset_492660();
                return;

            // Enter - save
            case VK_RETURN:
                if (stringLen > 1)
                {
                    // Replace arrow with null terminator
                    SfxPlayMono(relive::SoundEffects::IngameTransition, 90);
                    sSaveString_5C931C[stringLen - 1] = 0;
                    field_13C_save_state = SaveState::DoSave_4;
                    Input_Reset_492660();
                    return;
                }
                SfxPlayMono(relive::SoundEffects::ElectricZap, 0);
                return;

            // Backspace - delete
            case VK_BACK:
                if (stringLen > 1)
                {
                    // Replace last s8 with arrow
                    sSaveString_5C931C[stringLen - 2] = kArrowChar;
                    sSaveString_5C931C[stringLen - 1] = 0;
                    SfxPlayMono(relive::SoundEffects::PickupItem, 0);
                    return;
                }
                SfxPlayMono(relive::SoundEffects::ElectricZap, 0);
                return;
        }

        // Otherwise edit the file name if its an allowed s8
        if (strspn(newInput, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 !-"))
        {
            // Don't allow space at the start of the name, and don't allow 2 constitutive spaces.
            if (newInput[0] == ' ' && (stringLen == 1 || sSaveString_5C931C[stringLen - 2] == newInput[0]))
            {
                SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 30, 2600);
            }
            // Also don't allow the name length to be over 20 chars
            else if (stringLen > 20)
            {
                SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 30, 2400);
            }
            // Append new input s8
            else
            {
                sSaveString_5C931C[stringLen - 1] = newInput[0]; // Replace arrow with input
                sSaveString_5C931C[stringLen] = kArrowChar;      // Replace null with arrow
                sSaveString_5C931C[stringLen + 1] = 0;           // Append new null
                SfxPlayMono(relive::SoundEffects::RockBounce, 0);
            }
        }
        else
        {
            SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 70, 2200);
        }
    }
}

PauseMenu::PauseMenuPage sOverwriteSaveConfirmPage_55E560 = {
    &PauseMenu::Page_Save_Update,
    &PauseMenu::Page_Save_Render,
    PauseMenu__PageEntryList_Save_Overwrite_Confirm_55E508,
    -1,
    0u,
    0u,
    0u,
    0u,
    0u,
    0u};


void PauseMenu::Page_Save_Render(PrimHeader** ot, PauseMenuPage* pPage)
{
    PauseMenuPage* pPageToRender = &sOverwriteSaveConfirmPage_55E560;
    if (field_13C_save_state != SaveState::SaveConfirmOverwrite_8)
    {
        pPageToRender = pPage;
    }

    Page_Base_Render(ot, pPageToRender);
}

void PauseMenu::Page_Status_Update()
{
    if (Input().isHeld(0x300000))
    {
        // Go back to the main page
        field_136_unused = 0;
        field_144_active_menu = sPM_Page_Main_5465B0;
        SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
    }
}

void PauseMenu::Page_Status_Render(PrimHeader** ot, PauseMenuPage* pPage)
{
    // Render the status icon
    field_158_animation.SetRenderLayer(Layer::eLayer_Menu_41);
    field_158_animation.VRender(180, 100, ot, 0, 0);

    // Render the text
    Page_Base_Render(ot, pPage);
}

void PauseMenu::Page_Load_Update()
{
    char_type saveFileName[40] = {};

    // When F6 is pressed
    if (sQuicksave_LoadNextFrame_5CA4D9)
    {
        Quicksave_LoadActive();
        sQuicksave_LoadNextFrame_5CA4D9 = 0;
        word12C_flags &= ~1u;
    }

    const u32 inputHeld = Input().mPads[sCurrentControllerIndex].mHeld;

    // Up one save
    if (inputHeld & InputCommands::Enum::eUp)
    {
        // Don't underflow
        if (sSavedGameToLoadIdx_BB43FC > 0)
        {
            sSavedGameToLoadIdx_BB43FC--;
        }
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 35, 400);
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
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 35, 400);
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

        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 35, 400);
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
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 35, 400);
        return;
    }

    // Load save (enter)
    if (inputHeld & InputCommands::Enum::eUnPause_OrConfirm)
    {
        field_136_unused = 0;
        
        field_144_active_menu = sPM_Page_Main_5465B0;

        if (sTotalSaveFilesCount_BB43E0)
        {
            strcpy(saveFileName, sSaveFileRecords_BB31D8[sSavedGameToLoadIdx_BB43FC].field_0_fileName);
            strcat(saveFileName, ".sav");
            FILE* hFile = ::fopen(saveFileName, "rb");
            if (hFile)
            {
                ::fread(&sActiveQuicksaveData, sizeof(Quicksave), 1u, hFile);
                sActiveHero->mXPos = FP_FromInteger(0);
                sActiveHero->mYPos = FP_FromInteger(0);
                Quicksave_LoadActive();
                word12C_flags &= ~1u;
                // TODO: OG bug, file handle is leaked
                ::fclose(hFile);
            }
            SfxPlayMono(relive::SoundEffects::IngameTransition, 90);
        }
    }
    // Go back (esc)
    else if (inputHeld & InputCommands::Enum::eBack)
    {
        field_136_unused = 0;
        field_144_active_menu = sPM_Page_Main_5465B0;
        SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
    }
    // Delete (del)
    else if (inputHeld & 0x10000000)
    {
        if (sTotalSaveFilesCount_BB43E0)
        {
            strcpy(saveFileName, sSaveFileRecords_BB31D8[sSavedGameToLoadIdx_BB43FC].field_0_fileName);
            strcat(saveFileName, ".sav");
            relive_remove(saveFileName);
            Quicksave_FindSaves();
        }
    }
}

void PauseMenu::Page_Load_Render(PrimHeader** ot, PauseMenuPage* mp)
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

    Page_Base_Render(ot, mp);
}

u16 sub_4A2B70()
{
    
    return 1;
}

void PauseMenu::VUpdate()
{
    Abe* pHero = sActiveHero;
    IBaseAliveGameObject* pControlledChar = nullptr;

    if (!pHero)
    {
        return;
    }

    if (sActiveHero->mHealth <= FP_FromInteger(0) || sActiveHero->mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eElectrocuted))
    {
        pControlledChar = sControlledCharacter;
    }
    else
    {
        pControlledChar = sControlledCharacter;
        if (!(sControlledCharacter->mBaseAliveGameObjectFlags.Get(eTeleporting)))
        {
            const s16 heroState = sActiveHero->mCurrentMotion;
            if (heroState != eAbeMotions::Motion_86_HandstoneBegin
                && heroState != eAbeMotions::Motion_119_ToShrykull
                && heroState != eAbeMotions::Motion_120_EndShrykull
                && heroState != eAbeMotions::Motion_75_JumpIntoWell_45C7B0
                && heroState != eAbeMotions::Motion_76_ToInsideOfAWellLocal_45CA40
                && heroState != eAbeMotions::Motion_77_ToWellShotOut_45D130
                && heroState != eAbeMotions::Motion_78_WellBegin_45C810
                && heroState != eAbeMotions::Motion_79_InsideWellLocal_45CA60
                && heroState != eAbeMotions::Motion_80_WellShotOut_45D150
                && heroState != eAbeMotions::jMotion_81_WellBegin_45C7F0
                && heroState != eAbeMotions::Motion_82_InsideWellExpress_45CC80
                && heroState != eAbeMotions::Motion_83_WellExpressShotOut_45CF70
                && !(sControlledCharacter->Type() == ReliveTypes::eEvilFart && static_cast<EvilFart*>(sControlledCharacter)->mState == FartStates::eDechanting_2)
                && sActiveHero->mBirdPortalId == Guid{})
            {
                if (sQuicksave_SaveNextFrame_5CA4D8)
                {
                    DoQuicksave();
                    pHero = sActiveHero;
                    pControlledChar = sControlledCharacter;
                    sQuicksave_SaveNextFrame_5CA4D8 = 0;
                    sQuicksave_LoadNextFrame_5CA4D9 = 0;
                }
                else if (sQuicksave_LoadNextFrame_5CA4D9)
                {
                    Quicksave_LoadActive();
                    SND_SEQ_Stop(SeqId::MudokonChant1_10);
                    pHero = sActiveHero;
                    pControlledChar = sControlledCharacter;
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

    if (Input().mPads[sCurrentControllerIndex].mHeld & InputCommands::Enum::ePause)
    {
        if (pHero->mHealth > FP_FromInteger(0)
            && !(pHero->mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eElectrocuted))
            && !(pControlledChar->mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eTeleporting)))
        {
            const s16 heroState = pHero->mCurrentMotion;
            if (heroState != eAbeMotions::Motion_86_HandstoneBegin
                && heroState != eAbeMotions::Motion_119_ToShrykull
                && heroState != eAbeMotions::Motion_120_EndShrykull
                && heroState != eAbeMotions::Motion_75_JumpIntoWell_45C7B0
                && heroState != eAbeMotions::Motion_76_ToInsideOfAWellLocal_45CA40
                && heroState != eAbeMotions::Motion_77_ToWellShotOut_45D130
                && heroState != eAbeMotions::Motion_78_WellBegin_45C810
                && heroState != eAbeMotions::Motion_79_InsideWellLocal_45CA60
                && heroState != eAbeMotions::Motion_80_WellShotOut_45D150
                && heroState != eAbeMotions::jMotion_81_WellBegin_45C7F0
                && heroState != eAbeMotions::Motion_82_InsideWellExpress_45CC80
                && heroState != eAbeMotions::Motion_83_WellExpressShotOut_45CF70
                && (pControlledChar->Type() != ReliveTypes::eEvilFart || LOWORD(static_cast<Abe*>(pControlledChar)->field_124_timer) != 2) // TODO: Why LOWORD only ?? TODO: Cast seems wrong, missing intermediate base class??
                && pHero->mBirdPortalId == Guid{})
            {
                SND_StopAll();
                SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
                sub_4A2B70();
                mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);
                field_134_index_main = MainPages::ePage_Continue_0;
                field_136_unused = 0;
                word12C_flags = (word12C_flags & ~8) | 1;
                field_12E_selected_glow = 40;
                field_130_selected_glow_counter = 8;
                Path_Format_CameraName(
                    sScreenStringBuffer_5C92F0,
                    gMap.mCurrentLevel,
                    gMap.mCurrentPath,
                    gMap.mCurrentCamera);

                // TODO: never read ??
                //byte_5C92F8 = 0;

                for (size_t i = 0; i < strlen(sScreenStringBuffer_5C92F0); i++)
                {
                    s8 currentCamChar = sScreenStringBuffer_5C92F0[i];
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
                        sScreenStringBuffer_5C92F0[i] = static_cast<s8>(::tolower(currentCamChar));
                    }
                }

                sprintf(sPauseMenu_Of300Mudokons_55E718, "%d OF %d MUDOKONS", sRescuedMudokons, Path_GetTotalMuds(gMap.mCurrentLevel, gMap.mCurrentPath));
                sprintf(sHasBeenTerminated_55E738, "%d HA%s BEEN TERMINATED", sKilledMudokons, (sKilledMudokons != 1) ? "VE" : "S");

                if (sActiveHero->field_128.mMood == Mud_Emotion::eNormal_0)
                {
                    field_158_animation.Set_Animation_Data(GetAnimRes(AnimId::NormalMudIcon));
                }
                else if (sActiveHero->field_128.mMood == Mud_Emotion::eSad_3)
                {
                    field_158_animation.Set_Animation_Data(GetAnimRes(AnimId::AngryMudIcon));
                }
                else if (sActiveHero->field_128.mMood == Mud_Emotion::eHappy_5)
                {
                    field_158_animation.Set_Animation_Data(GetAnimRes(AnimId::HappyMudIcon));
                }

                field_158_animation.ReloadPal();

                sDisableFontFlicker = 1;
                field_144_active_menu = sPM_Page_Main_5465B0;

                // Start pause menu update/render loop
                while (word12C_flags & 1)
                {
                    if (pResourceManager)
                    {
                        pResourceManager->VUpdate();
                    }

                    SYS_EventsPump();

                    for (s32 i = 0; i < gObjListDrawables->Size(); i++)
                    {
                        BaseGameObject* pObj = gObjListDrawables->ItemAt(i);
                        if (!pObj)
                        {
                            break;
                        }

                        if (!(pObj->mBaseGameObjectFlags.Get(BaseGameObject::eDead)))
                        {
                            if (pObj->mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4))
                            {
                                pObj->VRender(gPsxDisplay.mDrawEnvs[gPsxDisplay.mBufferIndex].mOrderingTable);
                            }
                        }
                    }

                    pScreenManager->VRender(gPsxDisplay.mDrawEnvs[gPsxDisplay.mBufferIndex].mOrderingTable);

                    gPsxDisplay.RenderOrderingTable();
                    Input().Update(GetGameAutoPlayer());

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
                }

                // This call seems redundant as the calle will also update input right after this too
                Input().Update(GetGameAutoPlayer());

                mBaseGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);
            }
        }
    }
    sDisableFontFlicker = 0;
}

PauseMenu* gPauseMenu = nullptr;
