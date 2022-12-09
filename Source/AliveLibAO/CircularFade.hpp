#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class CircularFade final : public BaseAnimatedWithPhysicsGameObject
{
public:
    CircularFade(FP xpos, FP ypos, FP scale, s16 direction, s8 destroyOnDone);

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    // New virtuals
    virtual void VFadeIn(u8 direction, s8 destroyOnDone);
    virtual s32 VDone();

private:
    bool mFadeIn = false;
    bool mDone = false;
    bool mDestroyOnDone = false;
    Poly_G4 field_E8[2] = {};
    Poly_G4 field_110[2] = {};
    Poly_G4 field_138[2] = {};
    Poly_G4 field_160[2] = {};
    Prim_SetTPage field_188_tPage[2] = {};
    s16 field_1A8_fade_colour = 0;
    s16 field_1AA_speed = 0;
};

} // namespace AO
