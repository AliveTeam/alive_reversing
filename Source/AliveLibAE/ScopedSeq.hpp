#pragma once

#include "FunctionFwd.hpp"

enum class CameraPos : __int16;

class ScopedSeq
{
public:
    EXPORT ScopedSeq* ctor_4CB210(char ambianceId, CameraPos direction);

    virtual ScopedSeq* VDestructor(signed int flags);

private:
    EXPORT ScopedSeq* vdtor_4CB410(signed int flags);

    EXPORT void dtor_4CB440();


private:
    __int16 field_4_seq_id;
    __int16 field_6_padding;
    int field_8_channel_mask;
};
ALIVE_ASSERT_SIZEOF(ScopedSeq, 0xC);
