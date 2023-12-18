#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class AirExplosion final : public BaseAnimatedWithPhysicsGameObject
{
public:
    AirExplosion(FP xpos, FP ypos, FP explosionScale, bool bSmall);

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    void DealBlastDamage(PSX_RECT* pRect);

private:
    FP mExplosionScale = {};
    bool mSmallExplosion = false;
    FP mParticleBurstScale = {};
};

} // namespace AO
