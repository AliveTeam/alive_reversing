#include "stdafx.h"
#include "ScopedSeq.hpp"
#include "Map.hpp"
#include "Sound/Midi.hpp"
#include "stdlib.hpp"

ScopedSeq::ScopedSeq(s32 ambianceId, CameraPos direction)
{
    s16 leftVol = 0;
    s16 rightVol = 0;

    if (direction == CameraPos::eCamLeft_3)
    {
        leftVol = 1;
        rightVol = 0;
    }
    else
    {
        leftVol = direction != CameraPos::eCamRight_4;
        rightVol = 1;
    }

    field_4_seq_id = -1;
    field_8_channel_mask = 0;

    switch (ambianceId)
    {
        case 0:
            field_4_seq_id = 22;
            break;
        case 1:
            field_4_seq_id = 21;
            break;
        case 2:
            field_4_seq_id = 23;
            break;
        case 3:
            field_4_seq_id = 24;
            break;
        case 4:
            field_4_seq_id = 26;
            break;
        case 5:
            field_4_seq_id = 25;
            break;
        default:
            break;
    }

    if (field_4_seq_id != -1)
    {
        SND_SEQ_Play(field_4_seq_id, 0, 40 * leftVol + 15, 40 * rightVol + 15);
    }
}

ScopedSeq::~ScopedSeq()
{
    if (field_4_seq_id >= 0)
    {
        SND_SEQ_Stop(field_4_seq_id);
    }

    if (field_8_channel_mask)
    {
        SND_Stop_Channels_Mask(field_8_channel_mask);
    }
}
