#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/FixedPoint_common.hpp"

class IBaseAliveGameObject;

namespace AO {

class OrbWhirlWindParticle;

class OrbWhirlWind final : public BaseGameObject
{
public:
    enum class ParticlesState : s16
    {
        eCreating = 0,
        eCreated = 1,
        eActive = 2
    };

    OrbWhirlWind(FP xpos, FP ypos, FP scale);
    ~OrbWhirlWind();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    void ToSpin(FP xpos, FP ypos, IBaseAliveGameObject* pObj);
    void ToStop();

private:
    s32 field_10_particle_spawn_counter = 0;
    ParticlesState field_14_particles_state = ParticlesState::eCreating;
    s16 field_16_particleIdx = 0;
    OrbWhirlWindParticle* field_18_particles[16] = {};
    FP field_58_xpos = {};
    FP field_5C_ypos = {};
    FP field_60_scale = {};
};

} // namespace AO
