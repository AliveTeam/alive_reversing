#pragma once

#include <FunctionFwd.hpp>
#include "Animation.hpp"
#include "BitField.hpp"

class Shadow
{
public:
    EXPORT Shadow* ctor_4AC990();
    EXPORT void dtor_4ACA30();
    EXPORT void Calculate_Position_4ACA50(FP xpos, FP ypos, PSX_RECT* frameRect, FP spriteScale, __int16 scale);
    EXPORT void Render_4ACE60(int** pOt);
private:
    __int16 field_0_x1;
    __int16 field_2_y1;
    __int16 field_4_x2;
    __int16 field_6_y2;
    int field_8_xpos;
    int field_C_ypos;
    int field_10_scale;
public:
    enum Flags
    {
        eBit1 = 0x1,
        eBit2 = 0x2,
    };
    BitField32<Flags> field_14_flags;
    Animation field_18_animation;
};
ALIVE_ASSERT_SIZEOF(Shadow, 0xB0);
