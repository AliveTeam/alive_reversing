#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class ZapSpark final : public BaseAnimatedWithPhysicsGameObject
{
public:
    ZapSpark(FP xpos, FP ypos, FP scale);
    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    s16 mSparkTimer = 0;
};

} // namespace AO
