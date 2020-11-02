#include "stdafx_ao.h"
#include "Ddcheat.hpp"
#include "DemoPlayback.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "ResourceManager.hpp"
#include "SaveGame.hpp"
#include "stdlib.hpp"

START_NS_AO

ALIVE_VAR_EXTERN(BYTE, sRandomSeed_50A228); //Math.cpp

EXPORT DemoPlayback* DemoPlayback::ctor_4517B0(BYTE** ppPlaybackData, __int16 a3)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BBF98);

    field_6_flags.Clear(Options::eDrawable_Bit4);
    field_6_flags.Set(Options::eSurviveDeathReset_Bit9);
    field_4_typeId = Types::eDemoPlayback;
    field_1C = a3;
    sDDCheat_FlyingEnabled_50771C = 0;
    if (gAttract_507698 == 0)
    {
        field_18_ppRes = ResourceManager::Allocate_New_Locked_Resource_454F80(ResourceManager::Resource_Play, 1, sizeof(SaveData));
        if (!field_18_ppRes)
        {
            field_6_flags.Clear(Options::eDead_Bit3);
        }
        SaveGame::Save_459490(reinterpret_cast<SaveData*>(*field_18_ppRes));
    }
    else
    {
        field_18_ppRes = nullptr;
    }

    auto pd = reinterpret_cast<PlaybackData*>(*ppPlaybackData);
    ResourceManager::Set_Header_Flags_4557D0(ppPlaybackData, ResourceManager::ResourceHeaderFlags::eLocked);
    SaveGame::Load_459970(&pd->saveData, 1);
    sRandomSeed_50A228 = pd->randomSeed;
    field_10 = 0;
    field_14 = ppPlaybackData;
    field_8_update_delay = 1;
    return this;
}

BaseGameObject* DemoPlayback::VDestructor(signed int flags)
{
    return Vdtor_451F50(flags);
}

BaseGameObject* DemoPlayback::dtor_4518D0()
{
    NOT_IMPLEMENTED();
    return this;
}

DemoPlayback* DemoPlayback::Vdtor_451F50(signed int flags)
{
    dtor_4518D0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

END_NS_AO
