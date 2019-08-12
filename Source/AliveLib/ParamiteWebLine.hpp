#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FunctionFwd.hpp"

class ParamiteWebLine : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT void Wobble_4E29D0(short ypos);

    virtual void VScreenChanged() override;

private:

    EXPORT void vScreenChanged_4E2BC0();

    __int16 field_F4_anim_segment_count;
    __int16 field_F6_piece_length;
    __int16 field_F8_top;
    __int16 field_FA_bottom;
    AnimationUnknown* field_FC;
    int field_100;
    __int16 field_104;
    __int16 field_106_wobble_pos;
    Animation field_108_anim_flare;
    __int16 field_1A0;
    __int16 field_1A2;
    __int16 field_1A4_delay_counter;
    __int16 field_1A6;
    __int16 field_1A8;
    __int16 field_1AA;
};
ALIVE_ASSERT_SIZEOF(ParamiteWebLine, 0x1AC);
