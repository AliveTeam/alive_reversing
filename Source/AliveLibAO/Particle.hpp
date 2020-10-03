#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FunctionFwd.hpp"

START_NS_AO

class Particle : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Particle* ctor_478880(FP xpos, FP ypos, int animFrameTableOffset, int maxW, int maxH, BYTE** ppAnimData);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VUpdate() override;

    EXPORT void VUpdate_478930();

    int field_D4[4];
    FP field_E4_scale_amount;
};
ALIVE_ASSERT_SIZEOF(Particle, 0xE8);

EXPORT Particle* CC New_DestroyOrCreateObject_Particle_419D00(FP xpos, FP ypos, FP scale);

EXPORT void CC New_Particles_419A80(FP xpos, FP ypos, FP scale, __int16 count, __int16 type);

EXPORT void CC New_Chant_Particle_4198E0(FP xpos, FP ypos, FP scale, __int16 layer);

EXPORT void CC New_Shiny_Particle_4199A0(FP xpos, FP ypos, FP scale, __int16 layer);

EXPORT void CC New_ShootingZFire_Particle_419810(FP xpos, FP ypos, FP scale);

END_NS_AO
