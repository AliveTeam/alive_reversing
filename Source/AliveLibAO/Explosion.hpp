#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class Explosion : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Explosion* ctor_458B80(FP xpos, FP ypos, FP scale);

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_459470();

    EXPORT void VUpdate_458D00();

    EXPORT void DealBlastDamage_459160(PSX_RECT* pRect);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    int field_D4[4];
    FP field_E4_scale;
};
ALIVE_ASSERT_SIZEOF(Explosion, 0xE8);

}

