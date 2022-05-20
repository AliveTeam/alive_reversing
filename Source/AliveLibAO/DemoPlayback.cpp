#include "stdafx_ao.h"
#include "DDCheat.hpp"
#include "DemoPlayback.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "ResourceManager.hpp"
#include "SaveGame.hpp"
#include "stdlib.hpp"
#include "Input.hpp"
#include "Abe.hpp"
#include "MainMenu.hpp"

namespace AO {

ALIVE_VAR_EXTERN(u8, sRandomSeed_50A228); //Math.cpp

EXPORT DemoPlayback* DemoPlayback::ctor_4517B0(u8** ppPlaybackData, s16 bFromHandStone)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BBF98);

    mFlags.Clear(Options::eDrawable_Bit4);
    mFlags.Set(Options::eSurviveDeathReset_Bit9);
    field_4_typeId = Types::eDemoPlayback;
    field_1C_bFromHandStone = bFromHandStone;
    sDDCheat_FlyingEnabled_50771C = 0;
    if (gAttract_507698 == 0)
    {
        field_18_ppRes = ResourceManager::Allocate_New_Locked_Resource_454F80(ResourceManager::Resource_Play, 1, sizeof(SaveData));
        if (!field_18_ppRes)
        {
            mFlags.Clear(Options::eDead);
        }
        SaveGame::SaveToMemory_459490(reinterpret_cast<SaveData*>(*field_18_ppRes));
    }
    else
    {
        field_18_ppRes = nullptr;
    }

    auto pd = reinterpret_cast<PlaybackData*>(*ppPlaybackData);
    ResourceManager::Set_Header_Flags_4557D0(ppPlaybackData, ResourceManager::ResourceHeaderFlags::eLocked);
    SaveGame::LoadFromMemory_459970(&pd->saveData, 1);
    sRandomSeed_50A228 = pd->randomSeed;
    field_10_state = States::eState_0_Init;
    field_14_ppDemoRes = ppPlaybackData;
    field_8_update_delay = 1;
    return this;
}

BaseGameObject* DemoPlayback::VDestructor(s32 flags)
{
    return Vdtor_451F50(flags);
}

BaseGameObject* DemoPlayback::dtor_4518D0()
{
    SetVTable(this, 0x4BBF98);

    //nullsub_59(nullsub_59);

    ResourceManager::Clear_Header_Flags_4557F0(field_14_ppDemoRes, ResourceManager::ResourceHeaderFlags::eLocked);
    if (field_18_ppRes)
    {
        ResourceManager::FreeResource_455550(field_18_ppRes);
    }

    ResourceManager::FreeResource_455550(field_14_ppDemoRes);

    //nullsub_59(nullsub_59);

    return dtor_487DF0();
}

DemoPlayback* DemoPlayback::Vdtor_451F50(s32 flags)
{
    dtor_4518D0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
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
    VUpdate_451960();
}

void DemoPlayback::VUpdate_451960()
{
    //nullsub_59(nullsub_59);

    switch (field_10_state)
    {
        case States::eState_0_Init:
            sActiveHero_507678->mFlags.Set(Options::eDrawable_Bit4);
            sActiveHero_507678->field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render);

            Input().SetDemoRes_433470(reinterpret_cast<u32**>(field_14_ppDemoRes));

            mFlags.Set(Options::eDrawable_Bit4);
            field_10_state = States::eState_1_Playing;
            break;

        case States::eState_1_Playing:
            if (!Input().IsDemoPlaying_4334A0())
            {
                if (gAttract_507698)
                {
                    gAttract_507698 = 0;

                    // To the next demo of 30
                    sJoyResId_50769C++;

                    if (sJoyResId_50769C >= 30)
                    {
                        sJoyResId_50769C = 0;
                    }
                    gMap.SetActiveCam_444660(LevelIds::eMenu_0, 1, CameraIds::Menu::eMainMenu_1, CameraSwapEffects::eInstantChange_0, 0, 0);
                    gMap.field_DC_free_all_anim_and_palts = 1;
                }
                else
                {
                    SaveGame::LoadFromMemory_459970(&reinterpret_cast<PlaybackData*>(*field_18_ppRes)->saveData, 1);
                }

                field_10_state = States::eState_2_Done;
                field_8_update_delay = 2;
                mFlags.Clear(BaseGameObject::eDrawable_Bit4);
                mFlags.Set(BaseGameObject::eDead);
            }
            break;

        case States::eState_2_Done:
            mFlags.Set(BaseGameObject::eDead);
            break;
    }
}

} // namespace AO
