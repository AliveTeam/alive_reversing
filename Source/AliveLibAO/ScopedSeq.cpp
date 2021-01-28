#include "stdafx_ao.h"
#include "Function.hpp"
#include "ScopedSeq.hpp"
#include "Midi.hpp"
#include "stdlib.hpp"
#include "Midi.hpp"
#include "Map.hpp"

namespace AO {

ScopedSeq* ScopedSeq::ctor_476400(char ambianceId, CameraPos direction)
{
    SetVTable(this, 0x4BCD44);

    short volLeft = 0;
    short volRight = 0;

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

    field_4_seq_id = -1;
    field_8_channel_mask = 0;

    switch (ambianceId)
    {
    case 0:
        field_4_seq_id = 27;
        SND_SEQ_Play_477760(SeqId::Unknown_27, 0, 40 * volLeft + 15, 40 * volRight + 15);
        break;

    case 1:
        field_4_seq_id = 26;
        SND_SEQ_Play_477760(SeqId::Unknown_26, 0, 40 * volLeft + 15, 40 * volRight + 15);
        break;

    case 2:
        field_4_seq_id = 28;
        SND_SEQ_Play_477760(SeqId::Unknown_28, 0, 50 * volLeft + 15, 50 * volRight + 15);
        break;

    case 3:
        field_4_seq_id = 29;
        SND_SEQ_Play_477760(SeqId::Unknown_29, 0, 35 * volLeft + 15, 35 * volRight + 15);
        break;

    case 4:
        field_4_seq_id = 30;
        SND_SEQ_Play_477760(SeqId::eParamiteNearby_30, 0, 25 * volLeft + 10, 25 * volRight + 10);
        break;

    case 6:
        field_4_seq_id = 31;
        SND_SEQ_Play_477760(SeqId::Unknown_31, 0, 35 * volLeft + 15, 35 * volRight + 15);
        break;

    default:
        break;
    }
    return this;
}

ScopedSeq* ScopedSeq::VDestructor(int flags)
{
    SetVTable(this, 0x4BCD44);

    if (field_4_seq_id >= 0)
    {
        SND_Seq_Stop_477A60(static_cast<SeqId>(field_4_seq_id));
    }

    if (field_8_channel_mask)
    {
        SND_Stop_Channels_Mask_4774A0(field_8_channel_mask);
    }

    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }

    return this;
}

}
