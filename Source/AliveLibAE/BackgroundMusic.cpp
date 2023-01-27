#include "stdafx.h"
#include "BackgroundMusic.hpp"
#include "../relive_lib/Sound/Midi.hpp"
#include "Map.hpp"
#include "stdlib.hpp"

static s16 sBackgroundMusicSeqId = -1;

BackgroundMusic::BackgroundMusic(s32 musicId)
    : BaseGameObject(true, 0)
{
    mMusicId = static_cast<s16>(musicId);
    sBackgroundMusicSeqId = -1;
}

void BackgroundMusic::VUpdate()
{
    if (mMusicId >= 0)
    {
        if (gMap.mCurrentLevel != EReliveLevelIds::eNone || gMap.mCurrentPath != 1 || gMap.mCurrentCamera != 10)
        {
            SND_SEQ_PlaySeq(mMusicId, 0, 0);
        }
        sBackgroundMusicSeqId = mMusicId;
    }
    SetDead(true);
}

// In order of functions in the orignal game these are not part of this object
void BackgroundMusic::Stop()
{
    if (sBackgroundMusicSeqId >= 0)
    {
        SND_SEQ_Stop(sBackgroundMusicSeqId);
    }
}

void BackgroundMusic::Play()
{
    if (sBackgroundMusicSeqId >= 0)
    {
        SND_SEQ_PlaySeq(sBackgroundMusicSeqId, 0, 0);
    }
}
