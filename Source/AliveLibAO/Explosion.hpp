#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

START_NS_AO

class Explosion : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Explosion* ctor_458B80(FP xpos, FP ypos, FP scale);

    EXPORT void DealBlastDamage_459160(PSX_RECT* pRect);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    int field_D4[4];
    FP field_E4_scale;
};
ALIVE_ASSERT_SIZEOF(Explosion, 0xE8);

END_NS_AO

