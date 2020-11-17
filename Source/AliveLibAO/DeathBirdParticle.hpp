#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class DeathBirdParticle : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT DeathBirdParticle* ctor_41D950(FP xpos, FP ypos, int start, __int16 playSound, FP scale);

    virtual void VUpdate() override;

    EXPORT void VUpdate_41DA60();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    int field_D4[4];
    char field_E4_random;
    char field_E5_state;
    __int16 field_E6;
    int field_E8_start;
    __int16 field_EC_playSound;
    __int16 field_EE;
};
ALIVE_ASSERT_SIZEOF(DeathBirdParticle, 0xF0);

}

