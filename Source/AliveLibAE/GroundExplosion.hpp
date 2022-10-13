#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "PlatformBase.hpp"

class GroundExplosion final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    virtual void VUpdate() override;

    GroundExplosion(FP x, FP y, FP scale);
private:
    void DealDamageRect(const PSX_RECT* pRect);

    FP mBombSpriteScale = {};
};
