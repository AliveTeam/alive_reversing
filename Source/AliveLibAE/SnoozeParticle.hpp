#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "Primitives.hpp"
#include "FixedPoint.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "Layer.hpp"

class SnoozeParticle final : public BaseGameObject
{
public:
    SnoozeParticle(FP xpos, FP ypos, Layer layer, FP scale);
    ~SnoozeParticle();

    virtual void VScreenChanged() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VUpdate() override;

    enum class SnoozeParticleState : s16
    {
        eRising_0 = 0,
        eUnused_1 = 1,
        eBlowingUp_2 = 2
    };

private:
    FP field_20_x_start;
    FP field_24_y_start;
    FP field_28_x;
    FP field_2C_y;
    FP field_30_dx;
    FP field_34_dy;
    FP field_38_scale;
    FP field_3C_scale_dx;
    Layer field_40_layer;
    s16 field_42_r;
    s16 field_44_g;
    s16 field_46_b;
    u16 field_48_idx;
    s16 field_4A_count_down;
    Line_G4 field_4C_G4_lines[2];
    Line_G2 field_A4_G2_lines[2][6];
    Prim_SetTPage field_1C4_tPage[2];

public:
    SnoozeParticleState field_1E4_state;
};
ALIVE_ASSERT_SIZEOF(SnoozeParticle, 0x1E8);
