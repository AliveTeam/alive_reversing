#pragma once

#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"
#include "OrbWhirlWindParticle.hpp"

class OrbWhirlWind final : public BaseGameObject
{
public:
    enum class ParticlesState : s16
    {
        eCreating = 0,
        eCreated = 1,
        eActive = 2
    };

    OrbWhirlWind(FP xpos, FP ypos, FP scale, bool bIsMudokonSpirit);
    ~OrbWhirlWind();

    void ToSpin(FP xpos, FP ypos, FP scale, BaseGameObject* pObj);
    void ToStop();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

private:
    s32 field_20_particle_spawn_counter;
    ParticlesState field_24_particles_state;
    s16 field_26_is_mudokon_spirit;
    s16 field_28_particleIdx;
    //s16 field_2A; // padding ?
    OrbWhirlWindParticle* field_2C_particles[16];
    FP field_6C_xpos;
    FP field_70_ypos;
    FP field_74_scale;
};
ALIVE_ASSERT_SIZEOF(OrbWhirlWind, 0x78);
