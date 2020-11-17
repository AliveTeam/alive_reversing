#pragma once

#include "FunctionFwd.hpp"
#include "Animation.hpp"
#include "FixedPoint.hpp"
#include "BitField.hpp"

namespace AO {

class Shadow
{
public:
    EXPORT Shadow* ctor_461FB0();

    EXPORT void dtor_462030();

    EXPORT void Calculate_Position_462040(FP xpos, FP ypos, PSX_RECT* pRect, FP scale);

    EXPORT void Render_462410(int** ppOt);

    __int16 field_0_x1;
    __int16 field_2_y1;
    __int16 field_4_x2;
    __int16 field_6_y2;
    FP field_8_xpos;
    FP field_C_ypos;
    FP field_10_scale;
    enum Flags
    {
        eBit1_ShadowAtBottom = 0x1,
        eBit2_Enabled = 0x2,
    };
    BitField32<Flags> field_14_flags;
    Animation field_18_anim;
};
ALIVE_ASSERT_SIZEOF(Shadow, 0xB0);

}

