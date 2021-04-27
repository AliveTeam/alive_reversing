#pragma once

#include "FunctionFwd.hpp"

namespace AO {

enum class CameraPos : s16;

class ScopedSeq
{
public:
    EXPORT ScopedSeq* ctor_476400(s8 ambianceId, CameraPos direction);

    virtual ScopedSeq* VDestructor(s32 flags);

    s16 field_4_seq_id;
    s16 field_6_pad;
    s32 field_8_channel_mask;
};
ALIVE_ASSERT_SIZEOF(ScopedSeq, 0xC);

} // namespace AO
