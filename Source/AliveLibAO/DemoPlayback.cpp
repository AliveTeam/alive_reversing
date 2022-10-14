#include "stdafx_ao.h"
#include "DDCheat.hpp"
#include "DemoPlayback.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "ResourceManager.hpp"
#include "SaveGame.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Input.hpp"
#include "Abe.hpp"
#include "MainMenu.hpp"

extern u8 sRandomSeed; //Math.cpp

namespace AO {

DemoPlayback::DemoPlayback(u8** ppPlaybackData, s32 bFromHandStone)
    : BaseGameObject(TRUE, 0)
{
    mBaseGameObjectFlags.Clear(Options::eDrawable_Bit4);
    mBaseGameObjectFlags.Set(Options::eSurviveDeathReset_Bit9);
    SetType(ReliveTypes::eDemoPlayback);
    field_1C_bFromHandStone = static_cast<s16>(bFromHandStone);
    sDDCheat_FlyingEnabled_50771C = 0;
    if (gAttract_507698 == 0)
    {
        field_18_ppRes = relive_new SaveData();
        if (!field_18_ppRes)
        {
            mBaseGameObjectFlags.Clear(Options::eDead);
        }
        SaveGame::SaveToMemory(field_18_ppRes);
    }
    else
    {
        field_18_ppRes = nullptr;
    }

    auto pd = reinterpret_cast<PlaybackData*>(*ppPlaybackData);
    SaveGame::LoadFromMemory(&pd->saveData, 1);
    sRandomSeed = pd->randomSeed;
    field_10_state = States::eState_0_Init;
    field_14_ppDemoRes = ppPlaybackData;
    SetUpdateDelay(1);
}

DemoPlayback::~DemoPlayback()
{
    relive_delete field_18_ppRes;
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

    switch (field_10_state)
    {
        case States::eState_0_Init:
            sActiveHero->mBaseGameObjectFlags.Set(Options::eDrawable_Bit4);
            sActiveHero->GetAnimation().mFlags.Set(AnimFlags::eRender);

            Input().SetDemoRes(reinterpret_cast<u32**>(field_14_ppDemoRes));

            mBaseGameObjectFlags.Set(Options::eDrawable_Bit4);
            field_10_state = States::eState_1_Playing;
            break;

        case States::eState_1_Playing:
            if (!Input().IsDemoPlaying())
            {
                if (gAttract_507698)
                {
                    gAttract_507698 = 0;

                    // To the next demo of 30
                    gJoyResId++;

                    if (gJoyResId >= 30)
                    {
                        gJoyResId = 0;
                    }
                    gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eMainMenu_1, CameraSwapEffects::eInstantChange_0, 0, 0);
                    gMap.field_DC_free_all_anim_and_palts = 1;
                }
                else
                {
                    SaveGame::LoadFromMemory(field_18_ppRes, 1);
                }

                field_10_state = States::eState_2_Done;
                SetUpdateDelay(2);
                mBaseGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            break;

        case States::eState_2_Done:
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            break;
    }
}

} // namespace AO
