#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Primitives.hpp"
#include "FixedPoint.hpp"

START_NS_AO

class SnoozeParticle : public BaseGameObject
{
public:
    EXPORT SnoozeParticle* ctor_464320(FP xpos, FP ypos, __int16 layer, FP scale);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_464CD0();

    EXPORT BaseGameObject* dtor_4644A0();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT SnoozeParticle* Vdtor_464CE0(signed int flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_464500();

    virtual void VRender(int** ppOt) override;

    EXPORT void VRender_464620(int** ppOt);

    FP field_10_x_start;
    FP field_14_y_start;
    FP field_18_x;
    FP field_1C_y;
    FP field_20_dx;
    FP field_24_dy;
    FP field_28_scale;
    FP field_2C_scale_dx;
    __int16 field_30_layer;
    __int16 field_32_r;
    __int16 field_34_g;
    __int16 field_36_b;
    __int16 field_38_idx;
    __int16 field_3A_count_down;
    Line_G4 field_3C_lines[2];
    Line_G2 field_94_lines[12];
    Prim_SetTPage field_1B4_tPage[2];
    __int16 field_1D4_state;
    __int16 field_1D6_pad;
};
ALIVE_ASSERT_SIZEOF(SnoozeParticle, 0x1D8);


END_NS_AO
