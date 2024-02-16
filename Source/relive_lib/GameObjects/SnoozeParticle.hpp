#pragma once

#include "BaseGameObject.hpp"
#include "../Primitives.hpp"
#include "../FixedPoint.hpp"
#include "../Layer.hpp"
#include "../RGB16.hpp"

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
    virtual void VRender(OrderingTable& ot) override;
    virtual void VUpdate() override;

private:
    FP mStartY = {};
    FP mXPos = {};
    FP mYPos = {};
    FP mDestX = {};
    FP mDestY = {};
    FP mSpriteScale = {};
    FP mScaleDx = {};
    Layer mOtLayer = Layer::eLayer_0;
    RGB16 mRGB;
    u16 mIdx = 0;
    bool mBlowUp = false;
    Line_G4 mG4Line = {};
    Line_G2 mG2Lines[6] = {};

public:
    SnoozeParticleState mState = SnoozeParticleState::eRising_0;
};
