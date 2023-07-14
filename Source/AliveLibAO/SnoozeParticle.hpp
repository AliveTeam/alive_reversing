#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/Primitives.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "../relive_lib/Layer.hpp"

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
    virtual void VRender(OrderingTable& ot) override;

    FP mStartY = {};
    FP mXPos = {};
    FP mYPos = {};
    FP mDestX = {};
    FP mDestY = {};
    FP mSpriteScale = {};
    FP mScaleDx = {};
    Layer mOtLayer = Layer::eLayer_0;
    RGB16 mRGB;
    s16 mIdx = 0;
    bool mBlowUp = false;
    Line_G4 mG4Line = {};
    Line_G2 mG2Lines[6] = {};
    SnoozeParticleState mState = SnoozeParticleState::eRising_0;
};


} // namespace AO
