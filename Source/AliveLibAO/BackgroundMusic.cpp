#include "stdafx_ao.h"
#include "BackgroundMusic.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "Midi.hpp"

namespace AO {

ALIVE_VAR(1, 0x4CFFF8, s16, sBackgroundMusic_seq_id_4CFFF8, -1);

BackgroundMusic::BackgroundMusic(s16 musicId)
    : BaseGameObject(1)
{
    field_10_music_id = musicId;
    sBackgroundMusic_seq_id_4CFFF8 = -1;
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
