#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "FixedPoint.hpp"
#include "../relive_lib/Primitives.hpp"
#include "../relive_lib/Layer.hpp"

enum class ThrowableTotalIndicatorState : s16
{
    eCreated_0 = 0,
    eFading_1 = 1,
    eVanishing_2 = 2
};

class ThrowableTotalIndicator final : public BaseGameObject
{
public:
    ThrowableTotalIndicator(FP xpos, FP ypos, Layer layer, FP scale, s32 count, bool bFade);
    ~ThrowableTotalIndicator();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

private:
    FP field_20_xpos = {};
    FP field_24_ypos = {};
    FP field_28_cur_xpos = {};
    FP field_2C_cur_ypos = {};
    FP field_30_xspeed = {};
    FP field_34_yspeed = {};
    FP field_38_scale = {};
    FP field_3C_scale_speed = {};
    Layer field_40_layer = Layer::eLayer_0;
    s16 field_42_r = 0;
    s16 field_44_g = 0;
    s16 field_46_b = 0;
    s16 field_48_num_to_show = 0;
    Line_G2 field_4C_lines[2][6] = {};
    Prim_SetTPage field_16C_tPage[2] = {};
    ThrowableTotalIndicatorState field_18C_state = ThrowableTotalIndicatorState::eCreated_0;
    s16 field_18E_bFade = 0;
};
ALIVE_ASSERT_SIZEOF(ThrowableTotalIndicator, 0x190);

ALIVE_VAR_EXTERN(u16, bThrowableIndicatorExists_5C112C);
