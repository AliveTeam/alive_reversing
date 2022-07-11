#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/Primitives.hpp"
#include "FixedPoint.hpp"
#include "../relive_lib/Layer.hpp"

namespace AO {

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

    FP field_10_xpos = {};
    FP field_14_ypos = {};
    FP field_18_cur_xpos = {};
    FP field_1C_cur_ypos = {};
    FP field_20_xspeed = {};
    FP field_24_yspeed = {};
    FP field_28_scale = {};
    FP field_2C_scale_speed = {};
    Layer field_30_layer = Layer::eLayer_0;
    s16 field_32_r = 0;
    s16 field_34_g = 0;
    s16 field_36_b = 0;
    s16 field_38_num_to_show = 0;

    Line_F2 field_3C_lines[2][8] = {};
    Prim_SetTPage field_17C_tPage[2] = {};
    ThrowableTotalIndicatorState field_19C_state = ThrowableTotalIndicatorState::eCreated_0;
    s16 field_19E_bFade = 0;
};
ALIVE_ASSERT_SIZEOF(ThrowableTotalIndicator, 0x1A0);

ALIVE_VAR_EXTERN(u16, bThrowableIndicatorExists_504C70);

} // namespace AO
