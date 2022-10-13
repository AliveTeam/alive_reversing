#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class GroundExplosion final : public BaseAnimatedWithPhysicsGameObject
{
public:
    GroundExplosion(FP xpos, FP ypos, FP scale);

    void DealDamageRect(const PSX_RECT* pRect);

    virtual void VUpdate() override;

    FP mObjectScale = {};
};

} // namespace AO
