#pragma once

#include "../relive_lib/Function.hpp"
#include "../relive_lib/Animation.hpp"
#include "../relive_lib/Function.hpp"
#include "Font.hpp"
#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"


class PauseMenu;
struct PauseMenuPage;

enum PauseMenuAlign
{
    Left = 0,
    Centre = 1,
    Right = 2,
};

struct PauseMenuPageEntry final
{
    s16 mX;
    s16 y;
    const char_type* mText;
    u8 mRed;
    u8 mGreen;
    u8 mBlue;
    u8 mAlignment;
};

class PauseMenu final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    struct PauseMenuPage;

    PauseMenu();
    ~PauseMenu();

    void LoadAnimations();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

    void Init();

    void Page_Main_Update();
    void Page_Base_Render(PrimHeader** ot, PauseMenuPage* pPage);

    void Page_ControlsActions_Update();
    void Page_QuitConfirmation_Update();

    void Page_Save_Update();
    void Page_Save_Render(PrimHeader** ot, PauseMenuPage* pPage);

    void Page_Status_Update();
    void Page_Status_Render(PrimHeader** ot, PauseMenuPage* pPage);

    void Page_Load_Update();
    void Page_Load_Render(PrimHeader** ot, PauseMenuPage* pPage);

    using t_PmPage_Update = decltype(&PauseMenu::Page_Main_Update);
    using t_PmPage_Render = decltype(&PauseMenu::Page_Base_Render);

    struct PauseMenuPage final
    {
        t_PmPage_Update mFnUpdate;
        t_PmPage_Render mFnRender;
        PauseMenuPageEntry* mMenuItems;
        s16 mSelectedIndex;
        s8 mBgRed;
        s8 mBgGreen;
        s8 mBgBlue;
    };

private:
    void RestartPath();

public:
    FontContext mFontContext;
    PalResource mPal;
    AliveFont mFont = {};
    s16 mSelectedGlow = 0;
    s16 mSelectedGlowCounter = 0;

    enum MainPages : s16
    {
        ePage_Continue_0 = 0,
        ePage_QuickSave_1 = 1,
        ePage_Controls_2 = 2,
        ePage_Status_3 = 3,
        ePage_Save_4 = 4,
        ePage_Load_5 = 5,
        ePage_RestartPath_6 = 6,
        ePage_Quit_7 = 7,
    };

    /*MainPages*/ s16 mMainPage = 0;
    s16 mControlActionPageIndex = 0;

    enum class SaveState : s16
    {
        ReadingInput_0 = 0,
        DoSave_4 = 4,
        SaveConfirmOverwrite_8 = 8,
    };
    SaveState mSaveState = SaveState::ReadingInput_0;

    s16 mPolyOffset = 0;
    PauseMenu::PauseMenuPage mActiveMenu = {};
    Animation mMudIconAnim = {};
    Prim_SetTPage mPrimitives[2] = {};
    Poly_G4 mPolyG4s[2] = {};
    bool mPauseRenderLoop = false;
};

extern PauseMenu* gPauseMenu;
extern bool gQuicksave_SaveNextFrame;
extern bool gQuicksave_LoadNextFrame;
