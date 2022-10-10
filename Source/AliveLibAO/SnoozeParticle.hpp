#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/Primitives.hpp"
#include "FixedPoint.hpp"
#include "../relive_lib/Layer.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class SnoozeParticle final : public ::BaseGameObject
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

    FP mStartY = {};
    FP mXPos = {};
    FP mYPos = {};
    FP field_20_dx = {};
    FP field_24_dy = {};
    FP mSpriteScale = {};
    FP field_2C_scale_dx = {};
    Layer mOtLayer = Layer::eLayer_0;
    RGB16 mRGB;
    s16 field_38_idx = 0;
    s16 field_3A_count_down = 0;
    Line_G4 field_3C_lines[2] = {};
    Line_G2 field_94_lines[2][6] = {};
    Prim_SetTPage field_1B4_tPage[2] = {};
    SnoozeParticleState mState = SnoozeParticleState::eRising_0;
};


} // namespace AO
