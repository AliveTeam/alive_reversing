#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "Primitives.hpp"
#include "FixedPoint.hpp"
#include "Layer.hpp"

namespace AO {

class SnoozeParticle final : public BaseGameObject
{
public:
    enum class SnoozeParticleState : s16
    {
        eRising_0 = 0,
        eUnused_1 = 1,
        eBlowingUp_2 = 2
    };

    SnoozeParticle(FP xpos, FP ypos, Layer layer, FP scale);
    ~SnoozeParticle();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    FP field_10_x_start;
    FP field_14_y_start;
    FP field_18_x;
    FP field_1C_y;
    FP field_20_dx;
    FP field_24_dy;
    FP field_28_scale;
    FP field_2C_scale_dx;
    Layer field_30_layer;
    s16 field_32_r;
    s16 field_34_g;
    s16 field_36_b;
    s16 field_38_idx;
    s16 field_3A_count_down;
    Line_G4 field_3C_lines[2];
    Line_G2 field_94_lines[2][6];
    Prim_SetTPage field_1B4_tPage[2];
    SnoozeParticleState field_1D4_state;
    s16 field_1D6_pad;
};
ALIVE_ASSERT_SIZEOF(SnoozeParticle, 0x1D8);


} // namespace AO
