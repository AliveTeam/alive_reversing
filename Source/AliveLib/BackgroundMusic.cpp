#include "stdafx.h"
#include "BackgroundMusic.hpp"
#include "Sound/Midi.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "PathData.hpp"

ALIVE_VAR(1, 0x560f78, short, sBackgroundMusic_seq_id_560F78, -1);

void CC BackgroundMusic::Stop_4CB000()
{
    if (sBackgroundMusic_seq_id_560F78 >= 0)
    {
        SND_SEQ_Stop_4CAE60(sBackgroundMusic_seq_id_560F78);
    }
}

void CC BackgroundMusic::Play_4CB030()
{
    if (sBackgroundMusic_seq_id_560F78 >= 0)
    {
        SND_SEQ_PlaySeq_4CA960(sBackgroundMusic_seq_id_560F78, 0, 0);
    }
}

BackgroundMusic* BackgroundMusic::ctor_4CB110(__int16 musicId)
{
    BaseGameObject_ctor_4DBFA0(1, 0);
    SetVTable(this, 0x54781C); //  vTbl_BackgroundMusic_54781C
    field_20_music_id = musicId;
    sBackgroundMusic_seq_id_560F78 = -1;
    return this;
}

BaseGameObject* BackgroundMusic::VDestructor(signed int flags)
{
    return vdtor_4CB150(flags);
}

BaseGameObject* BackgroundMusic::vdtor_4CB150(signed int flags)
{
    dtor_4CB180();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void BackgroundMusic::dtor_4CB180()
{
    BaseGameObject_dtor_4DBEC0();
}

void BackgroundMusic::VUpdate()
{
    Update_4CB1A0();
}

void BackgroundMusic::Update_4CB1A0()
{
    if (field_20_music_id >= 0)
    {
        if (gMap_5C3030.sCurrentLevelId_5C3030 != LevelIds::eNone || gMap_5C3030.sCurrentPathId_5C3032 != 1 || gMap_5C3030.sCurrentCamId_5C3034 != 10)
        {
            SND_SEQ_PlaySeq_4CA960(field_20_music_id, 0, 0);
        }
        sBackgroundMusic_seq_id_560F78 = field_20_music_id;
    }
    field_6_flags.Set(BaseGameObject::eDead);
}
