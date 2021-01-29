#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"
#include "Primitives.hpp"

enum class ThrowableTotalIndicatorState : __int16
{
    eCreated_0 = 0,
    eFading_1 = 1,
    eVanishing_2 = 2
};

class ThrowableTotalIndicator : public BaseGameObject
{
public:
    EXPORT ThrowableTotalIndicator* ctor_431CB0(FP xpos, FP ypos, __int16 layer, FP scale, __int16 count, __int16 bFade);
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

private:
    EXPORT void dtor_431E10();
    EXPORT BaseGameObject* vdtor_431DE0(signed int flags);
    EXPORT void vScreenChanged_4323E0();
    EXPORT void vUpdate_431EA0();
    EXPORT void vRender_432070(PrimHeader** ppOt);

private:
    FP field_20_xpos;
    FP field_24_ypos;
    FP field_28_cur_xpos;
    FP field_2C_cur_ypos;
    FP field_30_xspeed;
    FP field_34_yspeed;
    FP field_38_scale;
    FP field_3C_scale_speed;
    __int16 field_40_layer;
    __int16 field_42_r;
    __int16 field_44_g;
    __int16 field_46_b;
    __int16 field_48_num_to_show;
    Line_G2 field_4C_lines[2][6];
    Prim_SetTPage field_16C_tPage[2];
    ThrowableTotalIndicatorState field_18C_state;
    __int16 field_18E_bFade;
};
ALIVE_ASSERT_SIZEOF(ThrowableTotalIndicator, 0x190);

ALIVE_VAR_EXTERN(WORD, bThrowableIndicatorExists_5C112C);
