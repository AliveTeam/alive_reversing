#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class Leaf final : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Leaf* ctor_48B4F0(FP xpos, FP ypos, FP xVel, FP yVel, FP scale);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_48B860();

    virtual void VUpdate() override;

    EXPORT void VUpdate_48B650();

    BaseGameObject* VDestructor(s32 flags) override;

    s32 field_D4_padding[4];
    s16 field_E4_bHitSomething;
    s16 field_E6_padding;
};
ALIVE_ASSERT_SIZEOF(Leaf, 0xE8);

} // namespace AO
