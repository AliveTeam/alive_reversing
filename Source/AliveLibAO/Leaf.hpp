#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class Leaf final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Leaf(FP xpos, FP ypos, FP xVel, FP yVel, FP scale);

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    s16 field_E4_bHitSomething = 0;
};
ALIVE_ASSERT_SIZEOF(Leaf, 0xE8);

} // namespace AO
