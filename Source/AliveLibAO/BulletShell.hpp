#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class PathLine;

class BulletShell : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT BulletShell* ctor_462790(FP xpos, FP ypos, __int16 direction, FP scale);

    EXPORT BaseGameObject* dtor_4628B0();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT BulletShell* Vdtor_462A50(signed int flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_4628C0();

    int field_D4[4];
    PathLine* field_E4_pLine;
    int field_E8;
    __int16 field_EC_hitCount;
    __int16 field_EE;
    FP field_F0_speed;
};
ALIVE_ASSERT_SIZEOF(BulletShell, 0xF4);

}
