#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class Leaf final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Leaf(FP xpos, FP ypos, FP xVel, FP yVel, FP scale);

    virtual void VScreenChanged() override;

    void VScreenChanged_48B860();

    virtual void VUpdate() override;

    void VUpdate_48B650();

    

    s32 field_D4_padding[4];
    s16 field_E4_bHitSomething;
    s16 field_E6_padding;
};
ALIVE_ASSERT_SIZEOF(Leaf, 0xE8);

} // namespace AO
