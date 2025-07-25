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
#include "Map.hpp"
#include "../relive_lib/data_conversion/file_system.hpp"
#include "nlohmann/json.hpp"
#include "../relive_lib/data_conversion/AOSaveSerialization.hpp"

extern u8 sRandomSeed; //Math.cpp

namespace AO {

char_type gActiveDemoName[32];

DemoPlayback::DemoPlayback()
    : BaseGameObject(true, 0)
{
    SetDrawable(false);
    SetSurviveDeathReset(true);
    SetType(ReliveTypes::eDemoPlayback);
    gDDCheat_FlyingEnabled = false;
    mSaveData = relive_new SaveData();
    if (gAttract == 0)
    {
        if (!mSaveData)
        {
            SetDead(false);
        }
        SaveGame::SaveToMemory(mSaveData);
    }

    //auto pd = reinterpret_cast<PlaybackData*>(*ppPlaybackData);

    FileSystem fs;
    char_type fileName[32];
    sprintf(fileName, "ATTR%04d.SAV.json", gJoyResId);
    auto jsonStr = fs.LoadToString(fileName);

    nlohmann::json j = nlohmann::json::parse(jsonStr);
    from_json(j, *mSaveData);
    SaveGame::LoadFromMemory(mSaveData, 1);
    // TODO: can probably be removed since rng in relive won't be in sync with OG anyway?
    //sRandomSeed = pd->randomSeed;
    mState = States::eInit_0;
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

void DemoPlayback::VRender(OrderingTable& /*ot*/)
{
    // Render nothing
}

void DemoPlayback::VUpdate()
{
    //nullsub_59(nullsub_59);

    switch (mState)
    {
        case States::eInit_0:
            gAbe->SetDrawable(true);
            gAbe->GetAnimation().SetRender(true);

            char_type fileName[32];
            sprintf(fileName, "PLAYBK%02d.JOY", gJoyResId);
            Input().InitDemo(fileName);

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
