#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/Layer.hpp"

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
Particle* New_Orb_Particle(FP xpos, FP ypos, FP velX, FP velY, FP scale, Layer layer, u8 r, u8 b, u8 g);
Particle* New_TintChant_Particle(FP xpos, FP ypos, FP scale, Layer layer);
void New_Smoke_Particles(FP xpos, FP ypos, FP scale, s16 count, u8 r, u8 g, u8 b);
void New_RandomizedChant_Particle(BaseAnimatedWithPhysicsGameObject* pObj);
Particle* New_TintShiny_Particle(FP xpos, FP ypos, FP scale, Layer layer);
void New_ShootingZFire_Particle(FP xpos, FP ypos, FP scale);
void New_ShootingFire_Particle(FP xpos, FP ypos, s8 direction, FP scale);
