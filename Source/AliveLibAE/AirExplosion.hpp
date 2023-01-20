#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"

class AirExplosion final : public BaseAnimatedWithPhysicsGameObject
{
public:
    AirExplosion(FP xpos, FP ypos, FP explosion_size, bool bSmall);

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    void DealBlastDamage(PSX_RECT* pRect);

private:
    bool mSmallExplosion = false;
    FP mParticleScale = {};
    FP mExplosionSize = {};
};
