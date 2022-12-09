#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

class CircularFade final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    CircularFade(FP xpos, FP ypos, FP scale, s16 direction, s8 destroyOnDone);
    ~CircularFade();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    // New virtuals
    virtual void VFadeIn(s16 direction, s8 destroyOnDone);
    virtual s32 VDone();

private:
    bool mFadeIn = false;
    bool mDone = false;
    bool mDestroyOnDone = false;
    Poly_G4 field_F8_tile1[2] = {};
    Poly_G4 field_120_tile2[2] = {};
    Poly_G4 field_148_tile3[2] = {};
    Poly_G4 field_170_tile4[2] = {};
    Prim_SetTPage field_198_tPages[2] = {};
    s16 field_1B8_fade_colour = 0;
    s16 field_1BA_speed = 0;
};

CircularFade* Make_Circular_Fade_4CE8C0(FP xpos, FP ypos, FP scale, s16 direction, s8 destroyOnDone, bool surviveDeathReset);
