#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class Leaf final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Leaf(FP xpos, FP ypos, FP xVel, FP yVel, FP scale);

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    s16 field_E4_bHitSomething = 0;
};

} // namespace AO
