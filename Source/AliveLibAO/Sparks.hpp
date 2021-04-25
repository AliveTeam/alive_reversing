#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class Sparks : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Sparks* ctor_40A3A0(FP xpos, FP ypos, FP scale);
    EXPORT void VUpdate_40A4F0();
    EXPORT void VScreenChanged_40A640();

    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    s32 field_D4_not_used[4];
    s32 field_E4_not_used1;
    s16 field_E8_not_used2;
    s16 field_EA_random;
};

}

