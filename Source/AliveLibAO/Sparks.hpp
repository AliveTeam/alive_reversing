#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class Sparks final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Sparks(FP xpos, FP ypos, FP scale);
    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    s32 field_D4_not_used[4];
    s32 field_E4_not_used1;
    s16 field_E8_not_used2;
    s16 field_EA_random;
};

} // namespace AO
