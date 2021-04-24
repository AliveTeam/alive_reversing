#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FunctionFwd.hpp"

namespace AO {

class Particle : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Particle* ctor_478880(FP xpos, FP ypos, s32 animFrameTableOffset, s32 maxW, s32 maxH, u8** ppAnimData);

    virtual BaseGameObject* VDestructor(s32 flags) override;

    virtual void VUpdate() override;

    EXPORT void VUpdate_478930();

    s32 field_D4[4];
    FP field_E4_scale_amount;
};
ALIVE_ASSERT_SIZEOF(Particle, 0xE8);

EXPORT Particle* CC New_DestroyOrCreateObject_Particle_419D00(FP xpos, FP ypos, FP scale);

EXPORT void CC New_Smoke_Particles_419A80(FP xpos, FP ypos, FP scale, s16 count, s16 type);

EXPORT void CC New_Chant_Particle_4198E0(FP xpos, FP ypos, FP scale, Layer layer);

EXPORT void CC New_Shiny_Particle_4199A0(FP xpos, FP ypos, FP scale, Layer layer);

EXPORT void CC New_ShootingZFire_Particle_419810(FP xpos, FP ypos, FP scale);

EXPORT void CC New_ShootingFire_Particle_419720(FP xpos, FP ypos, char direction, FP scale);

}
