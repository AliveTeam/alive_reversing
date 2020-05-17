#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FunctionFwd.hpp"

class Leaf : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Leaf* ctor_4E3120(FP xpos, FP ypos, FP xVel, FP yVel, FP scale);
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
private:
    EXPORT void vUpdate_4E3330();
    EXPORT void vScreenChanged_4E35B0();
    EXPORT void dtor_4E3310();
    EXPORT Leaf* vdtor_4E32E0(signed int flags);
private:
    __int16 field_F4_bHitSomething;
    //__int16 field_F6_pad;
};
ALIVE_ASSERT_SIZEOF(Leaf, 0xF8);
