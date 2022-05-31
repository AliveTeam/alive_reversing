#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Layer.hpp"

class Particle final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    Particle(FP xpos, FP ypos, s32 animFrameTableOffset, s32 maxW, s32 maxH, u8** ppAnimData);
    virtual void VUpdate() override;
public:
    FP field_F4_scale_amount;
};
ALIVE_ASSERT_SIZEOF(Particle, 0xF8);

Particle* New_DestroyOrCreateObject_Particle_426F40(FP xpos, FP ypos, FP scale);
Particle* New_Orb_Particle_426AA0(FP xpos, FP ypos, FP velX, FP velY, FP scale, Layer layer, u8 r, u8 b, u8 g);
Particle* New_TintChant_Particle_426BE0(FP xpos, FP ypos, FP scale, Layer layer);
void New_Smoke_Particles_426C70(FP xpos, FP ypos, FP scale, s16 count, u8 r, u8 g, u8 b);
void New_RandomizedChant_Particle_45BC70(class BaseAliveGameObject* pObj);
Particle* New_TintShiny_Particle_426C30(FP xpos, FP ypos, FP scale, Layer layer);
void New_ShootingZFire_Particle_4269B0(FP xpos, FP ypos, FP scale);
void New_ShootingFire_Particle_426890(FP xpos, FP ypos, s8 direction, FP scale);
