#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Primitives.hpp"
#include "FixedPoint.hpp"

START_NS_AO

class ThrowableTotalIndicator : public BaseGameObject
{
public:
    EXPORT ThrowableTotalIndicator* ctor_41B520(FP xpos, FP ypos, __int16 layer, FP scale, __int16 count, __int16 bFade);

    EXPORT BaseGameObject* dtor_41B610();


    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT BaseGameObject* Vdtor_41BB80(signed int flags);


    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_41BB70();

    // TODO: Render/update

    FP field_10_xpos;
    FP field_14_ypos;
    FP field_18_cur_xpos;
    FP field_1C_cur_ypos;
    FP field_20_xspeed;
    FP field_24_yspeed;
    FP field_28_scale;
    FP field_2C_scale_speed;
    __int16 field_30_layer;
    __int16 field_32_r;
    __int16 field_34_g;
    __int16 field_36_b;
    __int16 field_38_num_to_show;
    __int16 field_3A_pad;

    // TODO: This data structure is probably wrong
    Line_G2 field_3C[2][6];
    Prim_SetTPage field_15C[2];
    // end wrongness

    Prim_SetTPage field_17C_tPage[2];
    __int16 field_19C_state;
    __int16 field_19E_bFade;
};
ALIVE_ASSERT_SIZEOF(ThrowableTotalIndicator, 0x1A0);

END_NS_AO

