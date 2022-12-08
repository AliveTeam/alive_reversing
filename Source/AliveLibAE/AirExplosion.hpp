#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

class AirExplosion final : public BaseAnimatedWithPhysicsGameObject
{
public:
    AirExplosion(FP xpos, FP ypos, FP scale, bool bSmall);

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    void DealBlastDamage(PSX_RECT* pRect);

private:
    bool mSmallExplosion = false;
    FP mParticleScale = {};
    FP mExplosionSize = {};
};
