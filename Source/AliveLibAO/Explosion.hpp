#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class Explosion final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Explosion(FP xpos, FP ypos, FP scale);

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    void DealBlastDamage(PSX_RECT* pRect);

    s32 field_D4_padding[4];
    FP field_E4_explosion_size;
};
ALIVE_ASSERT_SIZEOF(Explosion, 0xE8);

} // namespace AO
