#pragma once

#include "FunctionFwd.hpp"

START_NS_AO

enum class CameraPos : __int16;

class ScopedSeq
{
public:
    EXPORT ScopedSeq* ctor_476400(char ambianceId, CameraPos direction);

    virtual ScopedSeq* VDtor(int flags);

    __int16 field_4_seq_id;
    __int16 field_6_pad;
    int field_8_channel_mask;
};
ALIVE_ASSERT_SIZEOF(ScopedSeq, 0xC);

END_NS_AO
