#pragma once

#include "FunctionFwd.hpp"
#include "Animation.hpp"
#include "FixedPoint.hpp"
#include "BitField.hpp"

START_NS_AO

class Shadow
{
public:
    EXPORT Shadow* ctor_461FB0();

    __int16 field_0;
    __int16 field_2;
    __int16 field_4;
    __int16 field_6;
    FP field_8_xpos;
    FP field_C_ypos;
    FP field_10_scale;
    enum Flags
    {
        eBit1_ShadowAtBottom = 0x1,
        eBit2_Enabled = 0x2,
    };
    BitField32<Flags> field_14_flags;
    Animation field_18_animation;
};
ALIVE_ASSERT_SIZEOF(Shadow, 0xB0);

END_NS_AO

