#include "stdafx_ao.h"
#include "BackgroundMusic.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "Midi.hpp"

namespace AO {

ALIVE_VAR(1, 0x4CFFF8, s16, sBackgroundMusic_seq_id_4CFFF8, -1);

BackgroundMusic* BackgroundMusic::ctor_476370(s16 musicId)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BCD30);
    field_10_music_id = musicId;
    sBackgroundMusic_seq_id_4CFFF8 = -1;
    return this;
}


BaseGameObject* BackgroundMusic::dtor_4763A0()
{
    SetVTable(this, 0x4BCD30);
    return dtor_487DF0();
}

BaseGameObject* BackgroundMusic::VDestructor(s32 flags)
{
    return Vdtor_476A80(flags);
}

BackgroundMusic* BackgroundMusic::Vdtor_476A80(s32 flags)
{
    dtor_4763A0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void BackgroundMusic::VUpdate()
{
    VUpdate_4763B0();
}


void BackgroundMusic::VUpdate_4763B0()
{
    if (field_10_music_id >= 0)
    {
        if (gMap.mCurrentLevel != LevelIds::eNone
            || gMap.mCurrentPath != 1
            || gMap.field_4_current_camera != 10)
        {
            SND_SEQ_PlaySeq_4775A0(static_cast<SeqId>(field_10_music_id), 0, 0);
        }
        sBackgroundMusic_seq_id_4CFFF8 = field_10_music_id;
    }
    mFlags.Set(BaseGameObject::eDead);
}

void CC BackgroundMusic::Stop_476290()
{
    if (sBackgroundMusic_seq_id_4CFFF8 >= 0)
    {
        SND_Seq_Stop_477A60(static_cast<SeqId>(sBackgroundMusic_seq_id_4CFFF8));
    }
}

void CC BackgroundMusic::Play_4762B0()
{
    if (sBackgroundMusic_seq_id_4CFFF8 >= 0)
    {
        SND_SEQ_PlaySeq_4775A0(static_cast<SeqId>(sBackgroundMusic_seq_id_4CFFF8), 0, 0);
    }
}

} // namespace AO
