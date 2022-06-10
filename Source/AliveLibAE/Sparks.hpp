#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

class Sparks final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    Sparks(FP xpos, FP ypos, FP scale);
    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    s16 field_FA_16_random;
};
ALIVE_ASSERT_SIZEOF(Sparks, 0xFC);
