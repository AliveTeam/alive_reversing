#include "stdafx_ao.h"
#include "ScopedSeq.hpp"
#include "Midi.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Midi.hpp"
#include "Map.hpp"

namespace AO {

ScopedSeq::ScopedSeq(s32 ambianceId, CameraPos direction)
{
    s16 volLeft = 0;
    s16 volRight = 0;

    if (direction == CameraPos::eCamLeft_3)
    {
        volLeft = 1;
        volRight = 0;
    }
    else
    {
        volLeft = direction != CameraPos::eCamRight_4 ? 1 : 0;
        volRight = 1;
    }

    mSeqId = -1;
    mChannelMask = 0;

    switch (ambianceId)
    {
        case 0:
            mSeqId = 27;
            SND_SEQ_Play(SeqId::Unknown_27, 0, 40 * volLeft + 15, 40 * volRight + 15);
            break;

        case 1:
            mSeqId = 26;
            SND_SEQ_Play(SeqId::Unknown_26, 0, 40 * volLeft + 15, 40 * volRight + 15);
            break;

        case 2:
            mSeqId = 28;
            SND_SEQ_Play(SeqId::Unknown_28, 0, 50 * volLeft + 15, 50 * volRight + 15);
            break;

        case 3:
            mSeqId = 29;
            SND_SEQ_Play(SeqId::Unknown_29, 0, 35 * volLeft + 15, 35 * volRight + 15);
            break;

        case 4:
            mSeqId = 30;
            SND_SEQ_Play(SeqId::eParamiteNearby_30, 0, 25 * volLeft + 10, 25 * volRight + 10);
            break;

        case 6:
            mSeqId = 31;
            SND_SEQ_Play(SeqId::Unknown_31, 0, 35 * volLeft + 15, 35 * volRight + 15);
            break;

        default:
            break;
    }
}

ScopedSeq::~ScopedSeq()
{
    if (mSeqId >= 0)
    {
        SND_Seq_Stop(static_cast<SeqId>(mSeqId));
    }

    if (mChannelMask)
    {
        SND_Stop_Channels_Mask(mChannelMask);
    }
}

} // namespace AO
