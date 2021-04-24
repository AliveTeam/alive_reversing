#pragma once

#include "FunctionFwd.hpp"

enum class CameraPos : s16;

class ScopedSeq
{
public:
    EXPORT ScopedSeq* ctor_4CB210(char ambianceId, CameraPos direction);

    virtual ScopedSeq* VDestructor(s32 flags);

private:
    EXPORT ScopedSeq* vdtor_4CB410(s32 flags);

    EXPORT void dtor_4CB440();


private:
    s16 field_4_seq_id;
    s16 field_6_padding;
    s32 field_8_channel_mask;
};
ALIVE_ASSERT_SIZEOF(ScopedSeq, 0xC);
