#include "stdafx.h"
#include "PauseMenu.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Function.hpp"
#include "Map.hpp"
#include "StringFormatters.hpp"
#include "Abe.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Sound/Sound.hpp"
#include "MainMenu.hpp"
#include "ThrowableArray.hpp"
#include "QuikSave.hpp"
#include "Mudokon.hpp"
#include "../relive_lib/Sys.hpp"
#include "PathDataExtensions.hpp"
#include "GameAutoPlayer.hpp"
#include "EvilFart.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "DDCheat.hpp"
#include "../relive_lib/Sound/Midi.hpp"
#include "ResourceManager.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "Game.hpp"
#include "nlohmann/json.hpp"
#include "../relive_lib/data_conversion/AESaveSerialization.hpp"
#include "../relive_lib/data_conversion/data_conversion.hpp"

bool gQuicksave_SaveNextFrame = false;
bool gQuicksave_LoadNextFrame = false;

static char_type sScreenStringBuffer[16] = {0};


static char_type sSaveString[32] = {};

// Menu Text Data

static PauseMenuPageEntry sMainPageEntries[11] = {
    {184, 48, "continue", 128u, 16u, 255u, 1u},
    {184, 70, "quiksave", 128u, 16u, 255u, 1u},
    {184, 92, "controls", 128u, 16u, 255u, 1u},
    {184, 114, "status", 128u, 16u, 255u, 1u},
    {184, 136, "save", 128u, 16u, 255u, 1u},
    {184, 158, "load", 128u, 16u, 255u, 1u},
    {184, 180, "restart path", 128u, 16u, 255u, 1u},
    {184, 202, "quit", 128u, 16u, 255u, 1u},
    {184, 16, "paused", 128u, 16u, 255u, 1u},
    {280, 16, sScreenStringBuffer, 128u, 16u, 255u, 0u},
    {0, 0, nullptr, 0u, 0u, 0u, 0u}};


static PauseMenuPageEntry sControlActionsEntries[17] = {
    {184, 205, "more", 128, 16, 255, Centre},
    {184, 20, "Actions", 127, 127, 127, Centre},
    {180, 50, kRun "+" kLeft kRight, 160, 160, 160, Right},
    {180, 70, kSneak "+" kLeft kRight, 160, 160, 160, Right},
    {180, 90, kJump, 160, 160, 160, Right},
    {180, 110, kThrow "+" kDPad, 160, 160, 160, Right},
    {180, 135, kDown, 160, 160, 160, Right},
    {180, 150, kAction, 160, 160, 160, Right},
    {180, 173, kUp, 160, 160, 160, Right},
    {200, 50, "run", 128, 16, 255, Left},
    {200, 70, "sneak", 128, 16, 255, Left},
    {200, 90, "jump", 128, 16, 255, Left},
    {200, 110, "throw", 128, 16, 255, Left},
    {200, 130, "crouch", 128, 16, 255, Left},
    {200, 150, "action", 128, 16, 255, Left},
    {200, 170, "hoist " kHoistZTurn " zturn", 128, 16, 255, Left},
    {0, 0, nullptr, 0u, 0u, 0u, 0u}};

static PauseMenuPageEntry sAbeGameSpeakEntries[21] = {
    {184, 205, "more", 128, 16, 255, Centre},
    {184, 20, "GameSpeak", 127, 127, 127, Centre},
    {184, 55, kChant, 160, 160, 160, Centre},
    {184, 75, "chant", 128, 16, 255, Centre},
    {110, 104, "hello", 128, 16, 255, Left},
    {110, 126, "follow me", 128, 16, 255, Left},
    {110, 148, "wait", 128, 16, 255, Left},
    {110, 170, "work", 128, 16, 255, Left},
    {278, 104, "anger", 128, 16, 255, Left},
    {278, 126, "all 'ya", 128, 16, 255, Left},
    {278, 148, "sympathy", 128, 16, 255, Left},
    {278, 170, "stop it!", 128, 16, 255, Left},
    {90, 104, kHello, 160, 160, 160, Right},
    {90, 126, kFollowMe, 160, 160, 160, Right},
    {90, 148, kWait, 160, 160, 160, Right},
    {90, 170, kWork, 160, 160, 160, Right},
    {260, 104, kAnger, 160, 160, 160, Right},
    {260, 126, kAllYa, 160, 160, 160, Right},
    {260, 148, kSorry, 160, 160, 160, Right},
    {260, 170, kStopIt, 160, 160, 160, Right},
    {0, 0, nullptr, 0u, 0u, 0u, 0u}};

static PauseMenuPageEntry sSligGameSpeakEntries[19] = {
    {184, 205, "more", 128, 16, 255, Centre},
    {184, 20, "SligSpeak", 127, 127, 127, Centre},
    {110, 104, "hi", 128, 16, 255, Left},
    {110, 126, "here boy", 128, 16, 255, Left},
    {110, 148, "freeze", 128, 16, 255, Left},
    {110, 170, "get 'im", 128, 16, 255, Left},
    {278, 104, "s'mo bs", 128, 16, 255, Left},
    {278, 126, "bs", 128, 16, 255, Left},
    {278, 148, "look out", 128, 16, 255, Left},
    {278, 170, "laugh", 128, 16, 255, Left},
    {90, 104, kHello, 160, 160, 160, Right},
    {90, 126, kFollowMe, 160, 160, 160, Right},
    {90, 148, kWait, 160, 160, 160, Right},
    {90, 170, kWork, 160, 160, 160, Right},
    {260, 104, kAnger, 160, 160, 160, Right},
    {260, 126, kAllYa, 160, 160, 160, Right},
    {260, 148, kSorry, 160, 160, 160, Right},
    {260, 170, kStopIt, 160, 160, 160, Right},
    {0, 0, nullptr, 0u, 0u, 0u, 0u}};

static PauseMenuPageEntry sGlukkonGameSpeakEntries[19] = {
    {184, 205, "more", 128, 16, 255, Centre},
    {184, 20, "GlukkonSpeak", 127, 127, 127, Centre},
    {110, 104, "hey!", 128, 16, 255, Left},
    {110, 126, "commere", 128, 16, 255, Left},
    {110, 148, "stay here", 128, 16, 255, Left},
    {110, 170, "do it!", 128, 16, 255, Left},
    {278, 104, "kill'em!", 128, 16, 255, Left},
    {278, 126, "all o'ya", 128, 16, 255, Left},
    {278, 148, "help!", 128, 16, 255, Left},
    {278, 170, "laugh", 128, 16, 255, Left},
    {90, 104, kHello, 160, 160, 160, Right},
    {90, 126, kFollowMe, 160, 160, 160, Right},
    {90, 148, kWait, 160, 160, 160, Right},
    {90, 170, kWork, 160, 160, 160, Right},
    {260, 104, kAnger, 160, 160, 160, Right},
    {260, 126, kAllYa, 160, 160, 160, Right},
    {260, 148, kSorry, 160, 160, 160, Right},
    {260, 170, kStopIt, 160, 160, 160, Right},
    {0, 0, nullptr, 0u, 0u, 0u, 0u}};

static PauseMenuPageEntry sParamiteGameSpeakEntries[15] = {
    {184, 205, "more", 128, 16, 255, Centre},
    {184, 20, "ParamiteSpeak", 127, 127, 127, Centre},
    {110, 104, "howdy", 128, 16, 255, Left},
    {110, 126, "c'mon", 128, 16, 255, Left},
    {110, 148, "stay", 128, 16, 255, Left},
    {110, 170, "do it", 128, 16, 255, Left},
    {278, 104, "attack", 128, 16, 255, Left},
    {278, 126, "all a ya!", 128, 16, 255, Left},
    {90, 104, kHello, 160, 160, 160, Right},
    {90, 126, kFollowMe, 160, 160, 160, Right},
    {90, 148, kWait, 160, 160, 160, Right},
    {90, 170, kWork, 160, 160, 160, Right},
    {260, 104, kAnger, 160, 160, 160, Right},
    {260, 126, kAllYa, 160, 160, 160, Right},
    {0, 0, nullptr, 0u, 0u, 0u, 0u}};

static PauseMenuPageEntry sScrabGameSpeakEntries[7] = {
    {184, 205, "exit", 128, 16, 255, Centre},
    {184, 20, "ScrabSpeak", 127, 127, 127, Centre},
    {110, 104, "shred power", 128, 16, 255, Left},
    {110, 126, "howl", 128, 16, 255, Left},
    {90, 104, kHello, 160, 160, 160, Right},
    {90, 126, kFollowMe, 160, 160, 160, Right},
    {0, 0, nullptr, 0u, 0u, 0u, 0u}};

static PauseMenuPageEntry sSavePageEntries[4] = {
    {184, 120, sSaveString, 128, 16, 255, Centre},
    {184, 180, "Enter   Save", 128, 16, 255, Centre},
    {184, 205, "Esc   Cancel", 128, 16, 255, Centre},
    {0, 0, nullptr, 0u, 0u, 0u, 0u}};

static PauseMenuPageEntry sQuitConfirmationEntries[3] = {
    {184, 110, "REALLY QUIT?", 128, 16, 255, Centre},
    {184, 135, "Enter yes   Esc no", 160, 160, 160, Centre},
    {0, 0, nullptr, 0u, 0u, 0u, 0u}};

static PauseMenuPageEntry sStatusPageEntries[1] = {
    {0, 0, nullptr, 0u, 0u, 0u, 0u}};

static PauseMenuPageEntry sLoadPageEntries[9] = {
    {184, 5, "", 128, 16, 255, Centre},
    {184, 30, "", 128, 16, 255, Centre},
    {184, 55, "", 128, 16, 255, Centre},
    {184, 80, "", 128, 16, 255, Centre},
    {184, 105, "", 128, 16, 255, Centre},
    {184, 130, "", 128, 16, 255, Centre},
    {184, 188, "", 128, 16, 255, Centre},
    {184, 213, "Esc  Cancel        F6  Load QuikSave", 128, 16, 255, Centre},
    {0, 0, nullptr, 0u, 0u, 0u, 0u}};


// Menu Pages

static const PauseMenu::PauseMenuPage sMainMenuPage = {
    &PauseMenu::Page_Main_Update,
    &PauseMenu::Page_Base_Render,
    sMainPageEntries,
    0,
    100u,
    100u,
    100u};

static const PauseMenu::PauseMenuPage sControlActionsMenuPage = {
    &PauseMenu::Page_ControlsActions_Update,
    &PauseMenu::Page_Base_Render,
    sControlActionsEntries,
    0,
    100u,
    100u,
    100u};

static const PauseMenu::PauseMenuPage sQuitConfirmationMenuPage = {
    &PauseMenu::Page_QuitConfirmation_Update,
    &PauseMenu::Page_Base_Render,
    sQuitConfirmationEntries,
    -1,
    100u,
    100u,
    100u};

static const PauseMenu::PauseMenuPage sLoadMenuPage = {
    &PauseMenu::Page_Load_Update,
    &PauseMenu::Page_Load_Render,
    sLoadPageEntries,
    0,
    static_cast<s8>(160u),
    static_cast<s8>(160u),
    static_cast<s8>(160u)};

static PauseMenuPageEntry sOverwriteSaveEntries[2] = {
    {184, 180, "Enter Overwrite  Esc Cancel", 160u, 160u, 160u, 1u},
    {0, 0, nullptr, 0u, 0u, 0u, 0u}};

static const PauseMenu::PauseMenuPage sSaveMenuPage = {
    &PauseMenu::Page_Save_Update,
    &PauseMenu::Page_Save_Render,
    &sSavePageEntries[0],
    0,
    static_cast<s8>(160u),
    static_cast<s8>(160u),
    static_cast<s8>(160u)};

static char_type sSavedMudokonsText[32] = {};
static char_type sTerminatedMudokonsText[56] = {};

static PauseMenuPageEntry sStatusEntries[6] = {
    {184, 205, "EXIT", 128u, 16u, 255u, 1u},
    {184, 20, "ABE'S STATUS", 127u, 127u, 127u, 1u},
    {184, 145, sSavedMudokonsText, 128u, 16u, 255u, 1u},
    {184, 170, sTerminatedMudokonsText, 128u, 16u, 255u, 1u},
    {184, 120, "YOU HAVE RESCUED", 128u, 16u, 255u, 1u},
    {0, 0, nullptr, 0u, 0u, 0u, 0u}};

PauseMenu::PauseMenuPage sStatusMenuPage = {
    &PauseMenu::Page_Status_Update,
    &PauseMenu::Page_Status_Render,
    &sStatusEntries[0],
    0,
    100u,
    100u,
    100u};

static PauseMenuPageEntry* sAllControlEntries[6] =
{
    sControlActionsEntries,
    sAbeGameSpeakEntries,
    sSligGameSpeakEntries,
    sGlukkonGameSpeakEntries,
    sParamiteGameSpeakEntries,
    sScrabGameSpeakEntries
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
    gQuicksave_SaveNextFrame = false;
    gQuicksave_LoadNextFrame = false;

    LoadAnimations();

    SetType(ReliveTypes::ePauseMenu);
    SetDrawable(false);
    SetSurviveDeathReset(true);
    mPauseRenderLoop = false;

    SetUpdateDelay(25);

    gObjListDrawables->Push_Back(this);

    mControlActionPageIndex = 0;
    mSaveState = SaveState::ReadingInput_0;

    mFontContext.LoadFontType(FontType::PauseMenu);
    mPal = ResourceManagerWrapper::LoadPal(PalId::MainMenuFont_PauseMenu);
    mFont.Load(256, mPal, &mFontContext);

    Init();

    gDisableFontFlicker = false;
}

PauseMenu::~PauseMenu()
{
    SetDrawable(false);

    gObjListDrawables->Remove_Item(this);
    mMudIconAnim.VCleanUp();
}

void PauseMenu::Init()
{
    if (mMudIconAnim.Init(GetAnimRes(AnimId::NormalMudIcon), this))
    {
        mMudIconAnim.SetRenderLayer(GetAnimation().GetRenderLayer());
        mMudIconAnim.SetSpriteScale(GetSpriteScale());
        mMudIconAnim.SetRGB(127, 127, 127);
    }
    else
    {
        SetListAddFailed(true);
    }
}

void PauseMenu::VRender(BasePrimitive** ot)
{
    mPolyOffset = 0;

    // Render the page
    (this->*mActiveMenu.mFnRender)(ot, &mActiveMenu);

    // Draw a full screen polygon that "dims" out the screen while paused
    Poly_G4* pPolys = &mPolyG4s[gPsxDisplay.mBufferIndex];
    pPolys->SetSemiTransparent(true);
    pPolys->DisableBlending(false);
    pPolys->SetRGB0(mActiveMenu.mBgRed, mActiveMenu.mBgGreen, mActiveMenu.mBgBlue);
    pPolys->SetRGB1(mActiveMenu.mBgRed, mActiveMenu.mBgGreen, mActiveMenu.mBgBlue);
    pPolys->SetRGB2(mActiveMenu.mBgRed, mActiveMenu.mBgGreen, mActiveMenu.mBgBlue);
    pPolys->SetXY0(0, 0);
    pPolys->SetXY1(640, 0);
    pPolys->SetXY2(0, 240);
    pPolys->SetXY3(640, 240);
    pPolys->SetBlendMode(relive::TBlendModes::eBlend_2);
    OrderingTable_Add(OtLayer(ot, Layer::eLayer_Menu_41), pPolys);
}

void PauseMenu::VScreenChanged()
{
    if (gMap.mNextLevel == EReliveLevelIds::eCredits)
    {
        SetDead(true);
    }
}

void PauseMenu::Page_Base_Render(BasePrimitive** ot, PauseMenu::PauseMenuPage* pPage)
{
    s32 i = 0;
    PauseMenuPageEntry* e = &pPage->mMenuItems[i];

    while (e->mText)
    {
        char_type textFormatted[128];
        String_FormatString(e->mText, textFormatted, 1);

        auto glow = 0;
        if (pPage->mSelectedIndex == i)
        {
            glow = mSelectedGlow;
        }

        auto x = e->mX;

        if (e->mAlignment != 0)
        {
            s16 textMeasure = static_cast<s16>(mFont.MeasureTextWidth(textFormatted));

            if (textMeasure >= 608)
            {
                x = 16;
            }
            else
            {
                if (e->mAlignment != 2)
                {
                    textMeasure /= 2;
                }
                x -= textMeasure;
            }
        }

        mPolyOffset = static_cast<s16>(mFont.DrawString(
            ot,
            textFormatted,
            x,            // X
            e->y, // Y
            relive::TBlendModes::eBlend_0,
            1,
            0,
            Layer::eLayer_Menu_41,
            static_cast<s8>(glow + e->mRed),
            static_cast<s8>(glow + e->mGreen),
            static_cast<s8>(glow + e->mBlue),
            mPolyOffset,
            FP_FromDouble(1.0),
            640,
            0));

        e = &pPage->mMenuItems[++i];
    }
}

void PauseMenu::RestartPath()
{
    DestroyObjects();

    gSwitchStates = QuikSave::gActiveQuicksaveData.mRestartPathSwitchStates;

    Abe::CreateFromSaveState(QuikSave::gActiveQuicksaveData.mRestartPathAbeState);
    QuikSave::RestoreWorldInfo(QuikSave::gActiveQuicksaveData.mRestartPathWorldInfo);

    gMap.SetActiveCam(
        QuikSave::gActiveQuicksaveData.mRestartPathWorldInfo.mLevel,
        QuikSave::gActiveQuicksaveData.mRestartPathWorldInfo.mPath,
        QuikSave::gActiveQuicksaveData.mRestartPathWorldInfo.mCam,
        CameraSwapEffects::eInstantChange_0,
        1,
        1);

    gMap.mForceLoad = true;
    if (sActiveHero->mBaseThrowableCount)
    {
        LoadRockTypes(
            QuikSave::gActiveQuicksaveData.mRestartPathWorldInfo.mLevel,
            QuikSave::gActiveQuicksaveData.mRestartPathWorldInfo.mPath);

        if (!gThrowableArray)
        {
            gThrowableArray = relive_new ThrowableArray();
        }

        gThrowableArray->Add(sActiveHero->mBaseThrowableCount);
    }

    mPauseRenderLoop = false;
    SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 3400);
    GetSoundAPI().mSND_Restart();
}

const char_type kArrowChar = 3;
char_type sArrowStr_55E398[2] = {kArrowChar, 0};

void PauseMenu::Page_Main_Update()
{
    if (Input().IsAnyPressed(InputCommands::eDown))
    {
        if (++mMainPage > MainPages::ePage_Quit_7)
        {
            mMainPage = MainPages::ePage_Continue_0;
        }
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
    }

    if (Input().IsAnyPressed(InputCommands::eUp))
    {
        if (--mMainPage < MainPages::ePage_Continue_0)
        {
            mMainPage = MainPages::ePage_Quit_7;
        }
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
    }

    mActiveMenu.mSelectedIndex = mMainPage;

    if (Input().IsAnyPressed(InputCommands::eBack))
    {
        mPauseRenderLoop = false;
        SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
        GetSoundAPI().mSND_Restart();
    }
    else if (Input().IsAnyPressed(InputCommands::eUnPause_OrConfirm))
    {
        switch (mMainPage)
        {
            case MainPages::ePage_Continue_0:
                mPauseRenderLoop = false;
                SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
                GetSoundAPI().mSND_Restart();
                return;

            case MainPages::ePage_QuickSave_1:
                mPauseRenderLoop = false;
                SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
                GetSoundAPI().mSND_Restart();
                QuikSave::DoQuicksave();
                return;

            case MainPages::ePage_Controls_2:
#if DEVELOPER_MODE
                devMenu.Activate();
#else
                mActiveMenu = sControlActionsMenuPage;
                mControlActionPageIndex = 0;
#endif
                break;

            case MainPages::ePage_Status_3:
                mActiveMenu = sStatusMenuPage;
                break;

            case MainPages::ePage_Save_4:
                mActiveMenu = sSaveMenuPage;
                SfxPlayMono(relive::SoundEffects::IngameTransition, 90);
                mSaveState = SaveState::ReadingInput_0;
                QuikSave::DoQuicksave();
                // Set the default save name to be the current level/path/camera
                Path_Format_CameraName(
                    sSaveString,
                    gMap.mCurrentLevel,
                    gMap.mCurrentPath,
                    gMap.mCurrentCamera);
                // Null terminate it
                sSaveString[8] = 0;
                // Append the editor arrow s8
                strcat(sSaveString, sArrowStr_55E398);

#if ORIGINAL_PS1_BEHAVIOR // OG Change - Allow for exiting save menu using controller
                setSaveMenuOpen(true); // Sets saveMenuOpen bool to true, instead of disabling input
#else
                Input_DisableInputForPauseMenuAndDebug_4EDDC0();
#endif
                return;

            case MainPages::ePage_Load_5:
                QuikSave::FindSaves();
                mActiveMenu = sLoadMenuPage;
                SfxPlayMono(relive::SoundEffects::IngameTransition, 90);
                mSaveState = SaveState::ReadingInput_0;
                return;

            case MainPages::ePage_RestartPath_6:
                RestartPath();
                return;

            case MainPages::ePage_Quit_7:
                mActiveMenu = sQuitConfirmationMenuPage;
                mMainPage = MainPages::ePage_Continue_0;
                break;

            default:
                return;
        }

        SfxPlayMono(relive::SoundEffects::IngameTransition, 90);
    }
}

void PauseMenu::Page_ControlsActions_Update()
{
    if (Input().IsAnyPressed(InputCommands::eBack))
    {
        mActiveMenu = sMainMenuPage;
        SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
    }

    if (Input().IsAnyPressed(InputCommands::eUnPause_OrConfirm))
    {
        const s32 prev = ++mControlActionPageIndex;
        if (prev < 6)
        {
            mActiveMenu.mMenuItems = sAllControlEntries[prev];
            SfxPlayMono(relive::SoundEffects::IngameTransition, 90);
        }
        else
        {
            mControlActionPageIndex = 0;
            mActiveMenu = sMainMenuPage;
            SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
        }
    }
}

void PauseMenu::Page_QuitConfirmation_Update()
{
    if (Input().IsAnyPressed(InputCommands::eBack))
    {
        mActiveMenu = sMainMenuPage;
        SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
    }

    if (Input().IsAnyPressed(InputCommands::eUnPause_OrConfirm))
    {
        mPauseRenderLoop = false;
        SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);

        if (gPauseMenu && gPauseMenu == this)
        {
            gPauseMenu->SetDead(true);
        }
        else
        {
            SetDead(true);
        }

        gPauseMenu = nullptr;
        gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, 1, CameraSwapEffects::eInstantChange_0, 0, 0);
        gMap.mFreeAllAnimAndPalts = true;
        sCurrentControllerIndex = 0;
    }
}

void PauseMenu::Page_Save_Update()
{
    static bool bWriteSaveFile_5C937C = false;

    char_type newInput[2] = {};
    char_type savFileName[40] = {};
    if (mSaveState == SaveState::DoSave_4)
    {
        strcpy(savFileName, sSaveString);
        strcat(savFileName, ".json");
        if (access_impl(savFileName, 4) || bWriteSaveFile_5C937C) // check file is writable
        {
            bWriteSaveFile_5C937C = false;
            nlohmann::json j;

            to_json(j, QuikSave::gActiveQuicksaveData);
            FileSystem fs;
            SaveJson(j, fs, &savFileName[0]);
            QuikSave::gSavedGameToLoadIdx = 0;

            mPauseRenderLoop = false;
            SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
            GetSoundAPI().mSND_Restart();
        }
        else
        {
            mSaveState = SaveState::SaveConfirmOverwrite_8;
        }
    }
    else if (mSaveState == SaveState::SaveConfirmOverwrite_8)
    {
        if (Input().IsAnyPressed(InputCommands::eUnPause_OrConfirm))
        {
            // Enter - do the save and don't return to the confirm overwrite
            SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
            mSaveState = SaveState::DoSave_4;
            bWriteSaveFile_5C937C = true;
        }
        else if (Input().IsAnyPressed(InputCommands::eBack))
        {
            // Escape - cancel save
            SfxPlayMono(relive::SoundEffects::IngameTransition, 90);
            mSaveState = SaveState::ReadingInput_0;
            strcat(sSaveString, sArrowStr_55E398);
#if ORIGINAL_PS1_BEHAVIOR // OG Change - Allow for exiting save menu using controller
            setSaveMenuOpen(true); // Sets saveMenuOpen bool to true, instead of disabling input
#else
            Input_DisableInputForPauseMenuAndDebug_4EDDC0();
#endif
        }
        return;
    }
    else if (mSaveState == SaveState::ReadingInput_0)
    {
#if ORIGINAL_PS1_BEHAVIOR // OG Change - Exit save menu using controller
        u32 lastPressed = Input_GetLastPressedKey_492610();

        if (lastPressed == VK_ESCAPE || lastPressed == VK_RETURN) // Keyboard ESC or ENTER
        {
            setSaveMenuOpen(false);
        }
        else if (Input().IsAnyPressed(InputCommands::eBack)) // Triangle
        {
            lastPressed = VK_ESCAPE;
            setSaveMenuOpen(false);
        }
        else if (Input().IsAnyPressed(InputCommands::eUnPause_OrConfirm)) // Cross or START
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

        const size_t stringLen = strlen(sSaveString);

        switch (lastPressed)
        {
            case VK_SHIFT:
                return;
            // Escape - cancel
            case VK_ESCAPE:
                SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
                mActiveMenu = sMainMenuPage;
                Input_Reset_492660();
                return;

            // Enter - save
            case VK_RETURN:
                if (stringLen > 1)
                {
                    // Replace arrow with null terminator
                    SfxPlayMono(relive::SoundEffects::IngameTransition, 90);
                    sSaveString[stringLen - 1] = 0;
                    mSaveState = SaveState::DoSave_4;
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
                    sSaveString[stringLen - 2] = kArrowChar;
                    sSaveString[stringLen - 1] = 0;
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
            if (newInput[0] == ' ' && (stringLen == 1 || sSaveString[stringLen - 2] == newInput[0]))
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
                sSaveString[stringLen - 1] = newInput[0]; // Replace arrow with input
                sSaveString[stringLen] = kArrowChar;      // Replace null with arrow
                sSaveString[stringLen + 1] = 0;           // Append new null
                SfxPlayMono(relive::SoundEffects::RockBounce, 0);
            }
        }
        else
        {
            SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 70, 2200);
        }
    }
}

static PauseMenu::PauseMenuPage sOverwriteSaveMenuPage = {
    &PauseMenu::Page_Save_Update,
    &PauseMenu::Page_Save_Render,
    sOverwriteSaveEntries,
    -1,
    0u,
    0u,
    0u};


void PauseMenu::Page_Save_Render(BasePrimitive** ot, PauseMenuPage* pPage)
{
    PauseMenuPage* pPageToRender = &sOverwriteSaveMenuPage;
    if (mSaveState != SaveState::SaveConfirmOverwrite_8)
    {
        pPageToRender = pPage;
    }

    Page_Base_Render(ot, pPageToRender);
}

void PauseMenu::Page_Status_Update()
{
    if (Input().IsAnyPressed(InputCommands::eBack | InputCommands::eUnPause_OrConfirm))
    {
        // Go back to the main page
        mActiveMenu = sMainMenuPage;
        SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
    }
}

void PauseMenu::Page_Status_Render(BasePrimitive** ot, PauseMenuPage* pPage)
{
    // Render the status icon
    mMudIconAnim.SetRenderLayer(Layer::eLayer_Menu_41);
    mMudIconAnim.VRender(180, 100, ot, 0, 0);

    // Render the text
    Page_Base_Render(ot, pPage);
}

void PauseMenu::Page_Load_Update()
{
    char_type saveFileName[40] = {};

    // When F6 is pressed
    if (gQuicksave_LoadNextFrame)
    {
        QuikSave::LoadActive();
        gQuicksave_LoadNextFrame = false;
        mPauseRenderLoop = false;
    }

    const u32 pressed = Input().mPads[sCurrentControllerIndex].mPressed;

    // Up one save
    if (pressed & InputCommands::eUp)
    {
        // Don't underflow
        if (QuikSave::gSavedGameToLoadIdx > 0)
        {
            QuikSave::gSavedGameToLoadIdx--;
        }
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 35, 400);
        return;
    }

    // Down one save
    if (pressed & InputCommands::eDown)
    {
        // Don't overflow
        if (QuikSave::gSavedGameToLoadIdx < QuikSave::gTotalSaveFilesCount - 1)
        {
            QuikSave::gSavedGameToLoadIdx++;
        }
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 35, 400);
        return;
    }

    // Page up saves
    if (pressed & InputCommands::ePageUp)
    {
        QuikSave::gSavedGameToLoadIdx -= 4;

        // Don't underflow
        if (QuikSave::gSavedGameToLoadIdx < 0)
        {
            QuikSave::gSavedGameToLoadIdx = 0;
        }

        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 35, 400);
        return;
    }

    // Page down saves
    if (pressed & InputCommands::ePageDown)
    {
        // Don't overflow
        QuikSave::gSavedGameToLoadIdx += 4;
        if (QuikSave::gSavedGameToLoadIdx > QuikSave::gTotalSaveFilesCount - 1)
        {
            QuikSave::gSavedGameToLoadIdx = QuikSave::gTotalSaveFilesCount - 1;
        }
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 35, 400);
        return;
    }

    // Load save (enter)
    if (pressed & InputCommands::eUnPause_OrConfirm)
    {
        mActiveMenu = sMainMenuPage;

        if (QuikSave::gTotalSaveFilesCount)
        {
            strcpy(saveFileName, QuikSave::gSaveFileRecords[QuikSave::gSavedGameToLoadIdx].mFileName);
            strcat(saveFileName, ".json");

            FileSystem fs;
            std::string jsonStr = fs.LoadToString(saveFileName);

            if (jsonStr.empty())
            {
                ALIVE_FATAL("Save file is empty");
            }

            nlohmann::json j = nlohmann::json::parse(jsonStr);
            QuikSave::gActiveQuicksaveData = {};
            from_json(j, QuikSave::gActiveQuicksaveData);

            sActiveHero->mXPos = FP_FromInteger(0);
            sActiveHero->mYPos = FP_FromInteger(0);
            QuikSave::LoadActive();
            mPauseRenderLoop = false;
            SfxPlayMono(relive::SoundEffects::IngameTransition, 90);
        }
    }
    // Go back (esc)
    else if (pressed & InputCommands::eBack)
    {
        mActiveMenu = sMainMenuPage;
        SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
    }
    // Delete (del)
    else if (pressed & InputCommands::eDelete)
    {
        if (QuikSave::gTotalSaveFilesCount)
        {
            strcpy(saveFileName, QuikSave::gSaveFileRecords[QuikSave::gSavedGameToLoadIdx].mFileName);
            strcat(saveFileName, ".json");
            relive_remove(saveFileName);
            QuikSave::FindSaves();
        }
    }
}

void PauseMenu::Page_Load_Render(BasePrimitive** ot, PauseMenuPage* pPage)
{
    s32 saveIdx = QuikSave::gSavedGameToLoadIdx - 2;
    for (s32 i = 0; i < 6; i++)
    {
        if (saveIdx < 0 || saveIdx >= QuikSave::gTotalSaveFilesCount)
        {
            // When at the top of the list set the first 2 entries to a blank, and when at the end of the list set the
            // remaining save text slots to blank.
            sLoadPageEntries[i].mText = "";
        }
        else
        {
            sLoadPageEntries[i].mText = QuikSave::gSaveFileRecords[saveIdx].mFileName;
        }
        saveIdx++;
    }

    if (QuikSave::gTotalSaveFilesCount > 0)
    {
        // This has to be set every time there is at least one save,
        // otherwise a call to this function with no saves hides this row forever
        sLoadPageEntries[6].mText = kUp " " kDown " Select    Enter Load    Del Delete";
    }
    else
    {
        sLoadPageEntries[4].mText = "No Saved Games";

        // hide row that displays hotkeys for selecting/loading/deleting saves, since there are no saves
        sLoadPageEntries[6].mText = "";
    }

    pPage->mSelectedIndex = 2;

    Page_Base_Render(ot, pPage);
}

void PauseMenu::VUpdate()
{
    Abe* pHero = sActiveHero;
    IBaseAliveGameObject* pControlledChar = nullptr;

    if (!pHero)
    {
        return;
    }

    if (sActiveHero->mHealth <= FP_FromInteger(0) || sActiveHero->GetElectrocuted())
    {
        pControlledChar = sControlledCharacter;
    }
    else
    {
        pControlledChar = sControlledCharacter;
        if (!sControlledCharacter->GetTeleporting())
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
                if (gQuicksave_SaveNextFrame)
                {
                    QuikSave::DoQuicksave();
                    pHero = sActiveHero;
                    pControlledChar = sControlledCharacter;
                    gQuicksave_SaveNextFrame = false;
                    gQuicksave_LoadNextFrame = false;
                }
                else if (gQuicksave_LoadNextFrame)
                {
                    QuikSave::LoadActive();
                    SND_SEQ_Stop(SeqId::MudokonChant1_10);
                    pHero = sActiveHero;
                    pControlledChar = sControlledCharacter;
                    gQuicksave_SaveNextFrame = false;
                    gQuicksave_LoadNextFrame = false;
                }
                else
                {
                    gQuicksave_SaveNextFrame = false;
                    gQuicksave_LoadNextFrame = false;
                }
            }
        }
    }

    if (Input().mPads[sCurrentControllerIndex].mPressed & InputCommands::ePause)
    {
        if (pHero->mHealth > FP_FromInteger(0)
            && !(pHero->GetElectrocuted())
            && !(pControlledChar->GetTeleporting()))
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
                SetDrawable(true);
                mMainPage = MainPages::ePage_Continue_0;
                mPauseRenderLoop = true;
                mSelectedGlow = 40;
                mSelectedGlowCounter = 8;
                Path_Format_CameraName(
                    sScreenStringBuffer,
                    gMap.mCurrentLevel,
                    gMap.mCurrentPath,
                    gMap.mCurrentCamera);

                for (size_t i = 0; i < strlen(sScreenStringBuffer); i++)
                {
                    s8 currentCamChar = sScreenStringBuffer[i];
                    if (currentCamChar == '.') // Chop off .CAM
                    {
                        sScreenStringBuffer[i] = 0;
                        break;
                    }

                    if (currentCamChar >= '0' && currentCamChar <= '9')
                    {
                        // "Lower" case numbers in the font atlas
                        sScreenStringBuffer[i] = currentCamChar - 58;
                    }
                    else
                    {
                        sScreenStringBuffer[i] = static_cast<s8>(::tolower(currentCamChar));
                    }
                }

                sprintf(sSavedMudokonsText, "%d OF %d MUDOKONS", gRescuedMudokons, Path_GetTotalMuds(gMap.mCurrentLevel, gMap.mCurrentPath));
                sprintf(sTerminatedMudokonsText, "%d HA%s BEEN TERMINATED", gKilledMudokons, (gKilledMudokons != 1) ? "VE" : "S");

                if (sActiveHero->mMood == Mud_Emotion::eNormal_0)
                {
                    mMudIconAnim.Set_Animation_Data(GetAnimRes(AnimId::NormalMudIcon));
                }
                else if (sActiveHero->mMood == Mud_Emotion::eSad_3)
                {
                    mMudIconAnim.Set_Animation_Data(GetAnimRes(AnimId::AngryMudIcon));
                }
                else if (sActiveHero->mMood == Mud_Emotion::eHappy_5)
                {
                    mMudIconAnim.Set_Animation_Data(GetAnimRes(AnimId::HappyMudIcon));
                }

                mMudIconAnim.ReloadPal();

                gDisableFontFlicker = true;
                mActiveMenu = sMainMenuPage;

                // Start pause menu update/render loop
                while (mPauseRenderLoop)
                {
                    SYS_EventsPump();

                    for (s32 i = 0; i < gObjListDrawables->Size(); i++)
                    {
                        BaseGameObject* pObj = gObjListDrawables->ItemAt(i);
                        if (!pObj)
                        {
                            break;
                        }

                        if (!(pObj->GetDead()))
                        {
                            if (pObj->GetDrawable())
                            {
                                pObj->VRender(gPsxDisplay.mDrawEnvs[gPsxDisplay.mBufferIndex].mOrderingTable.mOrderingTable);
                            }
                        }
                    }

                    gScreenManager->VRender(gPsxDisplay.mDrawEnvs[gPsxDisplay.mBufferIndex].mOrderingTable.mOrderingTable);

                    gPsxDisplay.RenderOrderingTable();
                    Input().Update(GetGameAutoPlayer());

                    if (mSelectedGlowCounter > 0)
                    {
                        mSelectedGlow += 8;
                    }

                    if (mSelectedGlow <= 120 || mSelectedGlowCounter <= 0)
                    {
                        if (mSelectedGlowCounter <= 0)
                        {
                            mSelectedGlow -= 8;
                            if (mSelectedGlow < 40)
                            {
                                mSelectedGlowCounter = -mSelectedGlowCounter;
                                mSelectedGlow += mSelectedGlowCounter;
                            }
                        }
                    }
                    else
                    {
                        mSelectedGlowCounter = -mSelectedGlowCounter;
                        mSelectedGlow += mSelectedGlowCounter;
                    }

                    (this->*mActiveMenu.mFnUpdate)();
                }

                // This call seems redundant as the calle will also update input right after this too
                Input().Update(GetGameAutoPlayer());

                SetDrawable(false);
            }
        }
    }
    gDisableFontFlicker = false;
}

PauseMenu* gPauseMenu = nullptr;
