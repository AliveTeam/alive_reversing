#pragma once

enum class CameraPos : s16;

class ScopedSeq final
{
public:
    // TODO: ambience id enum
    ScopedSeq(s32 ambianceId, CameraPos direction);
    ~ScopedSeq();
private:
    s16 mSeqId = 0;
    s32 mChannelMask = 0;
};
