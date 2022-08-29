#include "stdafx_ao.h"
#include "BackgroundMusic.hpp"
#include "Function.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Map.hpp"
#include "Midi.hpp"

namespace AO {

ALIVE_VAR(1, 0x4CFFF8, s16, sBackgroundMusic_seq_id_4CFFF8, -1);

BackgroundMusic::BackgroundMusic(s16 musicId)
    : BaseGameObject(TRUE, 0)
{
    field_10_music_id = musicId;
    sBackgroundMusic_seq_id_4CFFF8 = -1;
}

void BackgroundMusic::VUpdate()
{
    if (field_10_music_id >= 0)
    {
        if (gMap.mCurrentLevel != EReliveLevelIds::eNone
            || gMap.mCurrentPath != 1
            || gMap.mCurrentCamera != 10)
        {
            SND_SEQ_PlaySeq_4775A0(static_cast<SeqId>(field_10_music_id), 0, 0);
        }
        sBackgroundMusic_seq_id_4CFFF8 = field_10_music_id;
    }
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void BackgroundMusic::Stop()
{
    if (sBackgroundMusic_seq_id_4CFFF8 >= 0)
    {
        SND_Seq_Stop_477A60(static_cast<SeqId>(sBackgroundMusic_seq_id_4CFFF8));
    }
}

void BackgroundMusic::Play()
{
    if (sBackgroundMusic_seq_id_4CFFF8 >= 0)
    {
        SND_SEQ_PlaySeq_4775A0(static_cast<SeqId>(sBackgroundMusic_seq_id_4CFFF8), 0, 0);
    }
}

} // namespace AO
