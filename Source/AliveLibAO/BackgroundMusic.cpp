#include "stdafx_ao.h"
#include "BackgroundMusic.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Map.hpp"
#include "Midi.hpp"

namespace AO {

s16 gBackgroundMusicSeqId = -1;

BackgroundMusic::BackgroundMusic(s16 musicId)
    : BaseGameObject(true, 0)
{
    mMusicId = musicId;
    gBackgroundMusicSeqId = -1;
}

void BackgroundMusic::VUpdate()
{
    if (mMusicId >= 0)
    {
        if (gMap.mCurrentLevel != EReliveLevelIds::eNone
            || gMap.mCurrentPath != 1
            || gMap.mCurrentCamera != 10)
        {
            SND_SEQ_PlaySeq(static_cast<SeqId>(mMusicId), 0, 0);
        }
        gBackgroundMusicSeqId = mMusicId;
    }
    SetDead(true);
}

void BackgroundMusic::Stop()
{
    if (gBackgroundMusicSeqId >= 0)
    {
        SND_Seq_Stop(static_cast<SeqId>(gBackgroundMusicSeqId));
    }
}

void BackgroundMusic::Play()
{
    if (gBackgroundMusicSeqId >= 0)
    {
        SND_SEQ_PlaySeq(static_cast<SeqId>(gBackgroundMusicSeqId), 0, 0);
    }
}

} // namespace AO
