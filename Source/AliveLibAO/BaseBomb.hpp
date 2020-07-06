#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

START_NS_AO

class BaseBomb : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT BaseBomb* ctor_4173A0(FP xpos, FP ypos, int /*unused*/, FP scale);

    EXPORT void DealDamageRect_417A50(const PSX_RECT* pRect);

    virtual void VUpdate() override;

    EXPORT void VUpdate_417580();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    int field_D4[4];
    FP field_E4_scale;
};
ALIVE_ASSERT_SIZEOF(BaseBomb, 0xE8);

END_NS_AO

