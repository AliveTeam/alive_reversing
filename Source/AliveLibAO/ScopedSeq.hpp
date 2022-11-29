#pragma once

enum class CameraPos : s16;

namespace AO {


class ScopedSeq final
{
public:
    ScopedSeq(s32 ambianceId, CameraPos direction);
    ~ScopedSeq();

    s16 field_4_seq_id = 0;
    s32 field_8_channel_mask = 0;
};

} // namespace AO
