#pragma once

enum class CameraPos : s16;

namespace AO {


class ScopedSeq final
{
public:
    // TODO: ambience id enum
    ScopedSeq(s32 ambianceId, CameraPos direction);
    ~ScopedSeq();

    s16 mSeqId = 0;
    s32 mChannelMask = 0;
};

} // namespace AO
