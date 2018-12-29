#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FunctionFwd.hpp"

class DeathBirdParticle : public BaseAnimatedWithPhysicsGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;

    EXPORT DeathBirdParticle* ctor_43ECB0(FP xpos, FP ypos, int start, __int16 playSound, FP scale);
private:
    EXPORT void Update_43EE70();
    EXPORT void dtor_43EE50();
    EXPORT DeathBirdParticle* vdtor_43EE20(signed int flags);
private:
    int field_E4[4]; // never used
    char field_F4_random;
    char field_F5_state;
    // 2 byte pad
    int field_F8_start;
    __int16 field_FC_playSound;
    // 2 byte pad
};
ALIVE_ASSERT_SIZEOF(DeathBirdParticle, 0x100);
