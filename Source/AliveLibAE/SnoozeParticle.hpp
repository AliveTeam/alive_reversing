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
    virtual void VRender(BasePrimitive** ppOt) override;
    virtual void VUpdate() override;

    enum class SnoozeParticleState : s16
    {
        eRising_0 = 0,
        eUnused_1 = 1,
        eBlowingUp_2 = 2
    };

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
    Line_G4 mG4Lines[2] = {};
    Line_G2 mG2Lines[2][6] = {};

public:
    SnoozeParticleState mState = SnoozeParticleState::eRising_0;
};
