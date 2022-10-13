#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class AirExplosion final : public BaseAnimatedWithPhysicsGameObject
{
public:
    AirExplosion(FP xpos, FP ypos, FP scale);

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    void DealBlastDamage(PSX_RECT* pRect);

    FP mExplosionSize = {};
};

} // namespace AO
