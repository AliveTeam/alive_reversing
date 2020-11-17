#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class Leaf : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Leaf* ctor_48B4F0(FP xpos, FP ypos, FP xVel, FP yVel, FP scale);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_48B860();

    virtual void VUpdate() override;

    EXPORT void VUpdate_48B650();

    BaseGameObject* VDestructor(signed int flags) override;

    int field_D4[4];
    __int16 field_E4_bHitSomething;
    __int16 field_E6;
};
ALIVE_ASSERT_SIZEOF(Leaf, 0xE8);

}

