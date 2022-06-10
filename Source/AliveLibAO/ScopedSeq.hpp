#pragma once

#include "../AliveLibCommon/Function.hpp"

namespace AO {

enum class CameraPos : s16;

class ScopedSeq final
{
public:
    ScopedSeq(s32 ambianceId, CameraPos direction);
    ~ScopedSeq();

    s16 field_4_seq_id;
    s32 field_8_channel_mask;
};
//ALIVE_ASSERT_SIZEOF(ScopedSeq, 0xC);

} // namespace AO
