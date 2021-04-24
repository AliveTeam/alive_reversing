#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class BaseBomb : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT BaseBomb* ctor_4173A0(FP xpos, FP ypos, int /*unused*/, FP scale);

    EXPORT void DealDamageRect_417A50(const PSX_RECT* pRect);

    virtual void VUpdate() override;

    EXPORT void VUpdate_417580();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    int field_D4[4];
    FP field_E4_scale;
};
ALIVE_ASSERT_SIZEOF(BaseBomb, 0xE8);

}

