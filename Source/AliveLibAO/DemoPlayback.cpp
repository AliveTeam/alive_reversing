#include "stdafx_ao.h"
#include "DDCheat.hpp"
#include "DemoPlayback.hpp"
#include "../relive_lib/Function.hpp"
#include "Game.hpp"
#include "SaveGame.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Input.hpp"
#include "Abe.hpp"
#include "MainMenu.hpp"

extern u8 sRandomSeed; //Math.cpp

namespace AO {

DemoPlayback::DemoPlayback(u8** ppPlaybackData)
    : BaseGameObject(true, 0)
{
    SetDrawable(false);
    SetSurviveDeathReset(true);
    SetType(ReliveTypes::eDemoPlayback);
    gDDCheat_FlyingEnabled = false;
    if (gAttract == 0)
    {
        mSaveData = relive_new SaveData();
        if (!mSaveData)
        {
            SetDead(false);
        }
        SaveGame::SaveToMemory(mSaveData);
    }
    else
    {
        mSaveData = nullptr;
    }

    auto pd = reinterpret_cast<PlaybackData*>(*ppPlaybackData);
    SaveGame::LoadFromMemory(&pd->saveData, 1);
    sRandomSeed = pd->randomSeed;
    mState = States::eInit_0;
    mDemoRes = ppPlaybackData;
    SetUpdateDelay(1);
}

DemoPlayback::~DemoPlayback()
{
    relive_delete mSaveData;
}

void DemoPlayback::VScreenChanged()
{
    // Keep alive
}

void DemoPlayback::VRender(PrimHeader** /*ppOt*/)
{
    // Render nothing
}

void DemoPlayback::VUpdate()
{
    //nullsub_59(nullsub_59);

    switch (mState)
    {
        case States::eInit_0:
            sActiveHero->SetDrawable(true);
            sActiveHero->GetAnimation().SetRender(true);

            Input().SetDemoRes(reinterpret_cast<u32**>(mDemoRes));

            SetDrawable(true);
            mState = States::ePlaying_1;
            break;

        case States::ePlaying_1:
            if (!Input().IsDemoPlaying())
            {
                if (gAttract)
                {
                    gAttract = 0;

                    // To the next demo of 30
                    gJoyResId++;

                    if (gJoyResId >= 30)
                    {
                        gJoyResId = 0;
                    }
                    gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eMainMenu_1, CameraSwapEffects::eInstantChange_0, 0, 0);
                    gMap.mFreeAllAnimAndPalts = true;
                }
                else
                {
                    SaveGame::LoadFromMemory(mSaveData, 1);
                }

                mState = States::eDone_2;
                SetUpdateDelay(2);
                SetDrawable(false);
                SetDead(true);
            }
            break;

        case States::eDone_2:
            SetDead(true);
            break;
    }
}

} // namespace AO
