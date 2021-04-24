#pragma once

#include "FunctionFwd.hpp"

namespace AO {

enum class CameraPos : s16;

class ScopedSeq
{
public:
    EXPORT ScopedSeq* ctor_476400(char ambianceId, CameraPos direction);

    virtual ScopedSeq* VDestructor(int flags);

    s16 field_4_seq_id;
    s16 field_6_pad;
    int field_8_channel_mask;
};
ALIVE_ASSERT_SIZEOF(ScopedSeq, 0xC);

}
