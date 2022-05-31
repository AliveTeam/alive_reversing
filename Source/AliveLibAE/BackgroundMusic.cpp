#include "stdafx.h"
#include "BackgroundMusic.hpp"
#include "Sound/Midi.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "PathData.hpp"

ALIVE_VAR(1, 0x560f78, s16, sBackgroundMusic_seq_id_560F78, -1);


BackgroundMusic::BackgroundMusic(s32 musicId)
    : BaseGameObject(TRUE, 0)
{
    field_20_music_id = static_cast<s16>(musicId);
    sBackgroundMusic_seq_id_560F78 = -1;
}

void BackgroundMusic::VUpdate()
{
    if (field_20_music_id >= 0)
    {
        if (gMap.mCurrentLevel != LevelIds::eNone || gMap.mCurrentPath != 1 || gMap.field_4_current_camera != 10)
        {
            SND_SEQ_PlaySeq_4CA960(field_20_music_id, 0, 0);
        }
        sBackgroundMusic_seq_id_560F78 = field_20_music_id;
    }
    mFlags.Set(BaseGameObject::eDead);
}

// In order of functions in the orignal game these are not part of this object
void BackgroundMusic::Stop_4CB000()
{
    if (sBackgroundMusic_seq_id_560F78 >= 0)
    {
        SND_SEQ_Stop_4CAE60(sBackgroundMusic_seq_id_560F78);
    }
}

void BackgroundMusic::Play_4CB030()
{
    if (sBackgroundMusic_seq_id_560F78 >= 0)
    {
        SND_SEQ_PlaySeq_4CA960(sBackgroundMusic_seq_id_560F78, 0, 0);
    }
}
