#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Animation.hpp"
#include "FixedPoint.hpp"
#include "BitField.hpp"

namespace AO {

class Shadow final
{
public:
    Shadow();
    ~Shadow();

    void Calculate_Position(FP xpos, FP ypos, PSX_RECT* pRect, FP scale);
    void Render(PrimHeader** ppOt);

    s16 field_0_x1;
    s16 field_2_y1;
    s16 field_4_x2;
    s16 field_6_y2;
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

} // namespace AO
