#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Layer.hpp"

class Particle final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Particle(FP xpos, FP ypos, AnimResource& res, bool explosionSizeHack = false);
    ~Particle();
    virtual void VUpdate() override;
public:
    FP field_F4_scale_amount = {};
};

Particle* New_DestroyOrCreateObject_Particle(FP xpos, FP ypos, FP scale);
Particle* New_Orb_Particle(FP xpos, FP ypos, FP velX, FP velY, FP scale, Layer layer, RGB16 rgb);
Particle* New_TintChant_Particle(FP xpos, FP ypos, FP scale, Layer layer);
void New_Smoke_Particles(FP xpos, FP ypos, FP scale, s16 count, RGB16 rgb);
void New_RandomizedChant_Particle(BaseAnimatedWithPhysicsGameObject* pObj);
Particle* New_TintShiny_Particle(FP xpos, FP ypos, FP scale, Layer layer);
void New_ShootingZFire_Particle(FP xpos, FP ypos, FP scale);
void New_ShootingFire_Particle(FP xpos, FP ypos, s8 direction, FP scale);
