#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class Sparks final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Sparks(FP xpos, FP ypos, FP scale);
    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    s16 mSparkTimer = 0;
};

} // namespace AO
