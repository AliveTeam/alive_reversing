#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "../AliveLibCommon/FunctionFwd.hpp"

class DeathBirdParticle final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;

    EXPORT DeathBirdParticle* ctor_43ECB0(FP xpos, FP ypos, s32 start, s16 playSound, FP scale);

private:
    EXPORT void Update_43EE70();
    EXPORT void dtor_43EE50();
    EXPORT DeathBirdParticle* vdtor_43EE20(s32 flags);

private:
    s8 field_F4_random;
    enum class States : s8
    {
        eAnimateDeathFlares_0 = 0,
        eTransformStarsToDoves_1 = 1
    };
    States field_F5_state;
    // 2 byte pad
    s32 field_F8_start;
    s16 field_FC_bPlaySound;
    // 2 byte pad
};
ALIVE_ASSERT_SIZEOF(DeathBirdParticle, 0x100);
