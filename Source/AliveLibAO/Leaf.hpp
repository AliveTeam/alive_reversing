#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

START_NS_AO

ALIVE_VAR(1, 0x4D148C, BYTE, sLeafRandIdx_4D148C, 8);

class Leaf : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Leaf* ctor_48B4F0(FP xpos, FP ypos, FP xVel, FP yVel, FP scale);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_48B860();

    virtual void VUpdate() override;

    EXPORT void VUpdate_48B650();

    BaseGameObject* VDestructor(signed int flags);

    int field_D4[4];
    __int16 field_E4_bHitSomething;
    __int16 field_E6;
};
ALIVE_ASSERT_SIZEOF(Leaf, 0xE8);

END_NS_AO

