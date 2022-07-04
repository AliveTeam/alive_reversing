#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"

namespace AO {

class Particle final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Particle(FP xpos, FP ypos, AnimId animId, u8** ppAnimData);

    virtual void VUpdate() override;

    FP field_E4_scale_amount = {};
};
ALIVE_ASSERT_SIZEOF(Particle, 0xE8);

Particle* New_DestroyOrCreateObject_Particle_419D00(FP xpos, FP ypos, FP scale);
void New_Smoke_Particles_419A80(FP xpos, FP ypos, FP scale, s16 count, s16 type);
void New_Chant_Particle_4198E0(FP xpos, FP ypos, FP scale, Layer layer);
void New_Shiny_Particle_4199A0(FP xpos, FP ypos, FP scale, Layer layer);
void New_ShootingZFire_Particle_419810(FP xpos, FP ypos, FP scale);
void New_ShootingFire_Particle_419720(FP xpos, FP ypos, s8 direction, FP scale);

} // namespace AO
