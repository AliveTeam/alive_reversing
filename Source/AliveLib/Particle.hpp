#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"

class Particle : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Particle* ctor_4CC4C0(FP xpos, FP ypos, int animFrameTableOffset, int maxW, int maxH, BYTE** ppAnimData);
    EXPORT void vUpdate_4CC620();
    EXPORT BaseGameObject* vdtor_4CC5D0(signed int flags);

    virtual void VUpdate() override;
    virtual BaseGameObject* VDestructor(signed int flags) override;
private:
    int field_E4;
    int field_E8;
    int field_EC;
    int field_F0;
    int field_F4_scale_amount;
};
ALIVE_ASSERT_SIZEOF(Particle, 0xF8);
