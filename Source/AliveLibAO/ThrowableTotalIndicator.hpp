#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Primitives.hpp"
#include "FixedPoint.hpp"
#include "Layer.hpp"

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

    EXPORT void VScreenChanged_41BB70();

    virtual void VUpdate() override;

    EXPORT void VUpdate_41B690();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_41B810(PrimHeader** ppOt);

    FP field_10_xpos;
    FP field_14_ypos;
    FP field_18_cur_xpos;
    FP field_1C_cur_ypos;
    FP field_20_xspeed;
    FP field_24_yspeed;
    FP field_28_scale;
    FP field_2C_scale_speed;
    Layer field_30_layer;
    s16 field_32_r;
    s16 field_34_g;
    s16 field_36_b;
    s16 field_38_num_to_show;
    s16 field_3A_pad;

    Line_F2 field_3C_lines[2][8];
    Prim_SetTPage field_17C_tPage[2];
    ThrowableTotalIndicatorState field_19C_state;
    s16 field_19E_bFade;
};
ALIVE_ASSERT_SIZEOF(ThrowableTotalIndicator, 0x1A0);

ALIVE_VAR_EXTERN(u16, bThrowableIndicatorExists_504C70);

} // namespace AO
