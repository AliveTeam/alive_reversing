#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class GroundExplosion final : public BaseAnimatedWithPhysicsGameObject
{
public:
    virtual void VUpdate() override;

    GroundExplosion(FP xpos, FP ypos, FP scale);
private:
    void DealDamageRect(const PSX_RECT* pRect);

    FP mBombSpriteScale = {};
};

} // namespace AO
