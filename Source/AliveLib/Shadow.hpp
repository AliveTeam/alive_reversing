#pragma once

#include <FunctionFwd.hpp>
#include "Animation.hpp"

class Shadow
{
public:
    EXPORT Shadow* ctor_4AC990();
    EXPORT void dtor_4ACA30();
    EXPORT void Calculate_Position_4ACA50(FP xpos, FP ypos, PSX_RECT* frameRect, FP spriteScale, __int16 scale);
    EXPORT void Render_4ACE60(int** pOt);
private:
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
public:
    int field_14_flags;
    AnimationEx field_18_animation;
};
ALIVE_ASSERT_SIZEOF(Shadow, 0xB0);
