#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

START_NS_AO

class Sparks : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Sparks* ctor_40A3A0(FP xpos, FP ypos, FP scale);
    EXPORT void VUpdate_40A4F0();
    EXPORT void VScreenChanged_40A640();

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    int field_D4_not_used[4];
    int field_E4_not_used1;
    __int16 field_E8_not_used2;
    __int16 field_EA_random;
};

END_NS_AO

