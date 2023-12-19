#include "stdafx.h"
#include "ScopedSeq.hpp"
#include "MapWrapper.hpp"
#include "Sound/Midi.hpp"
#include "../AliveLibAO/Midi.hpp"
#include "GameType.hpp"

static inline void PlaySeq(u16 seqId, s16 repeatCount, s16 volLeft, s16 volRight)
{
    if (GetGameType() == GameType::eAo)
    {
        AO::SND_SEQ_Play(static_cast<AO::SeqId>(seqId), repeatCount, volLeft, volRight);
    }
    else
    {
        SND_SEQ_Play(seqId, repeatCount, volLeft, volRight);
    }
}

ScopedSeq::ScopedSeq(AmbienceId ambianceId, CameraPos direction)
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
        volLeft = direction != CameraPos::eCamRight_4;
        volRight = 1;
    }

    mSeqId = -1;
    mChannelMask = 0;

    switch (ambianceId)
    {
        case AmbienceId::eSligSleeping:
        {
            mSeqId = GetGameType() == GameType::eAe ? 22 : 27;
            PlaySeq(mSeqId, 0, 40 * volLeft + 15, 40 * volRight + 15);
            break;
        }

        case AmbienceId::eSligPatrol:
        {
            mSeqId = GetGameType() == GameType::eAe ? 21 : 26;
            PlaySeq(mSeqId, 0, 40 * volLeft + 15, 40 * volRight + 15);
            break;
        }

        case AmbienceId::eSlogSleeping:
        {
            mSeqId = GetGameType() == GameType::eAe ? 24 : 29;
            s16 volMultiplier = GetGameType() == GameType::eAe ? 40 : 35;
            PlaySeq(mSeqId, 0, volMultiplier * volLeft + 15, volMultiplier * volRight + 15);
            break;
        }
        case AmbienceId::eSlogAwake:
        {
            mSeqId = GetGameType() == GameType::eAe ? 23 : 28;
            s16 volMultiplier = GetGameType() == GameType::eAe ? 40 : 50;
            PlaySeq(mSeqId, 0, volMultiplier * volLeft + 15, volMultiplier * volRight + 15);
            break;
        }
        case AmbienceId::eParamite:
        {
            mSeqId = GetGameType() == GameType::eAe ? 25 : 30;
            s16 volMultiplier = GetGameType() == GameType::eAe ? 40 : 25;
            s16 volAdd = GetGameType() == GameType::eAe ? 15 : 10;
            PlaySeq(mSeqId, 0, volMultiplier * volLeft + volAdd, volMultiplier * volRight + volAdd);
            break;
        }
        case AmbienceId::eMeatSaw:
        {
            mSeqId = 31;
            PlaySeq(mSeqId, 0, 35 * volLeft + 15, 35 * volRight + 15);
            break;
        }
        case AmbienceId::eFleechAsleep:
        {
            mSeqId = 26;
            PlaySeq(mSeqId, 0, 35 * volLeft + 15, 35 * volRight + 15);
            break;
        }

        default:
            break;
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
