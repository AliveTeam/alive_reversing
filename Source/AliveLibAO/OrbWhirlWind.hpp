#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"

START_NS_AO

class OrbWhirlWindParticle;
class BaseAliveGameObject;

class OrbWhirlWind : public BaseGameObject
{
public:
    enum class ParticlesState : __int16
    {
        eCreating = 0,
        eCreated = 1,
        eActive = 2
    };

    EXPORT OrbWhirlWind* ctor_48B870(FP xpos, FP ypos, FP scale);

    EXPORT BaseGameObject* dtor_48B910();


    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT OrbWhirlWind* Vdtor_48C4F0(signed int flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_48B990();

    virtual void VRender(int** ppOt) override;

    EXPORT void VRender_48BA90(int** ppOt);

    EXPORT void ToSpin_48BAD0(FP xpos, FP ypos, BaseAliveGameObject* pObj);

    EXPORT void ToStop_48BBB0();

    int field_10_particle_spawn_counter;
    ParticlesState field_14_particles_state;
    __int16 field_16_particleIdx;
    OrbWhirlWindParticle* field_18_particles[16];
    FP field_58_xpos;
    FP field_5C_ypos;
    FP field_60_scale;
};
ALIVE_ASSERT_SIZEOF(OrbWhirlWind, 0x64);

END_NS_AO

