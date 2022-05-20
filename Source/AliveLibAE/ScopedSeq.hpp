#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"

enum class CameraPos : s16;

class ScopedSeq final
{
public:
    ScopedSeq(s32 ambianceId, CameraPos direction);
    ~ScopedSeq();
private:
    s16 field_4_seq_id;
    s16 field_6_padding;
    s32 field_8_channel_mask;
};
