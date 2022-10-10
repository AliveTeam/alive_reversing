#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class Explosion final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Explosion(FP xpos, FP ypos, FP scale);

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    void DealBlastDamage(PSX_RECT* pRect);

    FP field_E4_explosion_size = {};
};

} // namespace AO
