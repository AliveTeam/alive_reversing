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

    mSeqId = -1;
    mChannelMask = 0;

    switch (ambianceId)
    {
        case 0:
            mSeqId = 22;
            break;
        case 1:
            mSeqId = 21;
            break;
        case 2:
            mSeqId = 23;
            break;
        case 3:
            mSeqId = 24;
            break;
        case 4:
            mSeqId = 26;
            break;
        case 5:
            mSeqId = 25;
            break;
        default:
            break;
    }

    if (mSeqId != -1)
    {
        SND_SEQ_Play(mSeqId, 0, 40 * leftVol + 15, 40 * rightVol + 15);
    }
}

ScopedSeq::~ScopedSeq()
{
    if (mSeqId >= 0)
    {
        SND_SEQ_Stop(mSeqId);
    }

    if (mChannelMask)
    {
        SND_Stop_Channels_Mask(mChannelMask);
    }
}
