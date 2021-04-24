#pragma once

#include "BaseGameObject.hpp"
#include "Primitives.hpp"
#include "FixedPoint.hpp"
#include "FunctionFwd.hpp"
#include "Layer.hpp"

class SnoozeParticle : public BaseGameObject
{

public:
    EXPORT SnoozeParticle* ctor_4B06F0(FP xpos, FP ypos, Layer layer, FP scale);

    BaseGameObject* VDestructor(s32 flags) override
    {
        return vdtor_4B08D0(flags);
    }

    virtual void VScreenChanged() override
    {
        vScreenChanged_4B1300();
    }

    virtual void VRender(PrimHeader** ppOt) override
    {
        Render_4B0AF0(ppOt);
    }

    virtual void VUpdate() override
    {
        Update_4B0980();
    }

    enum class SnoozeParticleState : s16
    {
        Rising_0 = 0,
        Unused_1 = 1,
        BlowingUp_2 = 2
    };

private:

    EXPORT void dtor_4B0900();

    EXPORT SnoozeParticle* vdtor_4B08D0(s32 flags);

    EXPORT void vScreenChanged_4B1300();

    EXPORT void Update_4B0980();

    EXPORT void Render_4B0AF0(PrimHeader** ppOt);

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
private:
    s16 field_1E6_padding;
};
ALIVE_ASSERT_SIZEOF(SnoozeParticle, 0x1E8);
