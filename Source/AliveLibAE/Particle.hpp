#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Layer.hpp"

class Particle final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    Particle(FP xpos, FP ypos, AnimId animId, u8** ppAnimData, bool explosionSizeHack = false);
    virtual void VUpdate() override;
public:
    FP field_F4_scale_amount = {};
};
ALIVE_ASSERT_SIZEOF(Particle, 0xF8);

Particle* New_DestroyOrCreateObject_Particle(FP xpos, FP ypos, FP scale);
Particle* New_Orb_Particle(FP xpos, FP ypos, FP velX, FP velY, FP scale, Layer layer, u8 r, u8 b, u8 g);
Particle* New_TintChant_Particle(FP xpos, FP ypos, FP scale, Layer layer);
void New_Smoke_Particles(FP xpos, FP ypos, FP scale, s16 count, u8 r, u8 g, u8 b);
void New_RandomizedChant_Particle(class BaseAliveGameObject* pObj);
Particle* New_TintShiny_Particle(FP xpos, FP ypos, FP scale, Layer layer);
void New_ShootingZFire_Particle(FP xpos, FP ypos, FP scale);
void New_ShootingFire_Particle(FP xpos, FP ypos, s8 direction, FP scale);
