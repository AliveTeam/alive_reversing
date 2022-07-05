#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

class ZapSpark final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    ZapSpark(FP xpos, FP ypos, FP scale);
    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    s16 mSparkTimer = 0;
};
ALIVE_ASSERT_SIZEOF(ZapSpark, 0xFC);
