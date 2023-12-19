#pragma once

enum class CameraPos : s16;

enum AmbienceId
{
    eNone,
    eSligSleeping,
    eSligPatrol,
    eSlogSleeping,
    eSlogAwake,
    eParamite,
    eMeatSaw,
    eFleechAsleep,
};

class ScopedSeq final
{
public:
    ScopedSeq(AmbienceId ambianceId, CameraPos direction);
    ~ScopedSeq();
private:
    s16 mSeqId = 0;
    s32 mChannelMask = 0;
};
