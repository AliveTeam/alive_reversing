#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class Leaf final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Leaf(FP xpos, FP ypos, FP xVel, FP yVel, FP scale);

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    s32 field_D4_padding[4];
    s16 field_E4_bHitSomething;
    s16 field_E6_padding;
};
ALIVE_ASSERT_SIZEOF(Leaf, 0xE8);

} // namespace AO
