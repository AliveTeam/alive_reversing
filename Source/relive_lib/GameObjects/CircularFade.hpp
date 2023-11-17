#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"

class CircularFade final : public BaseAnimatedWithPhysicsGameObject
{
public:
    CircularFade(FP xpos, FP ypos, FP scale, bool fadeIn, s8 destroyOnDone);
    ~CircularFade();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;

    // New virtuals
    virtual void VFadeIn(bool fadeIn, s8 destroyOnDone);
    virtual s32 VDone();

private:
    bool mFadeIn = false;
    bool mDone = false;
    bool mDestroyOnDone = false;
    Poly_G4 mTile1 = {};
    Poly_G4 mTile2 = {};
    Poly_G4 mTile3 = {};
    Poly_G4 mTile4 = {};
    s16 mFadeColour = 0;
    s16 mSpeed = 0;
};

CircularFade* Make_Circular_Fade(FP xpos, FP ypos, FP scale, bool fadeIn, s8 destroyOnDone, bool surviveDeathReset);