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
    FP field_8_xpos;
    FP field_C_ypos;
    FP field_10_scale;
public:
    enum Flags
    {
        eBit1_ShadowAtBottom = 0x1,
        eBit2_Enabled = 0x2,
    };
    BitField32<Flags> field_14_flags;
    Animation field_18_animation;
};
ALIVE_ASSERT_SIZEOF(Shadow, 0xB0);
