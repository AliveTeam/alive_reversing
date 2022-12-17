#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/Primitives.hpp"
#include "../relive_lib/Layer.hpp"

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
    FP mStartY = {};
    FP field_28_x = {};
    FP field_2C_y = {};
    FP field_30_dx = {};
    FP field_34_dy = {};
    FP mSpriteScale = {};
    FP field_3C_scale_dx = {};
    Layer mOtLayer = Layer::eLayer_0;
    RGB16 mRGB;
    u16 field_48_idx = 0;
    s16 field_4A_count_down = 0;
    Line_G4 field_4C_G4_lines[2] = {};
    Line_G2 field_A4_G2_lines[2][6] = {};
    Prim_SetTPage field_1C4_tPage[2] = {};

public:
    SnoozeParticleState mState = SnoozeParticleState::eRising_0;
};
