#include "stdafx.h"
#include "BackgroundMusic.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../MapWrapper.hpp"
#include "../Sound/Midi.hpp"

s16 gBackgroundMusicSeqId = -1;

BackgroundMusic::BackgroundMusic(s32 musicId)
    : BaseGameObject(true, 0)
{
    mMusicId = static_cast<s16>(musicId);
    gBackgroundMusicSeqId = -1;
}

void BackgroundMusic::VUpdate()
{
    if (mMusicId >= 0)
    {
        if (GetMap().mCurrentLevel != EReliveLevelIds::eNone
            || GetMap().mCurrentPath != 1
            || GetMap().mCurrentCamera != 10)
        {
            SND_SEQ_PlaySeq(mMusicId, 0, 0);
        }
        gBackgroundMusicSeqId = mMusicId;
    }
    SetDead(true);
}

// In order of functions in the orignal game these are not part of this object
void BackgroundMusic::Stop()
{
    if (gBackgroundMusicSeqId >= 0)
    {
        SND_SEQ_Stop(gBackgroundMusicSeqId);
    }
}

void BackgroundMusic::Play()
{
    if (gBackgroundMusicSeqId >= 0)
    {
        SND_SEQ_PlaySeq(gBackgroundMusicSeqId, 0, 0);
    }
}
