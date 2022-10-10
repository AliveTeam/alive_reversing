#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"

class Leaf final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    Leaf(FP xpos, FP ypos, FP xVel, FP yVel, FP scale);
    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    s16 field_F4_bHitSomething = 0;
};
