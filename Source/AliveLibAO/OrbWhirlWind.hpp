#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"

namespace AO {

class OrbWhirlWindParticle;
class BaseAliveGameObject;

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

    void VUpdate_48B990();

    virtual void VRender(PrimHeader** ppOt) override;

    void VRender_48BA90(PrimHeader** ppOt);

    void ToSpin_48BAD0(FP xpos, FP ypos, BaseAliveGameObject* pObj);

    void ToStop_48BBB0();

    s32 field_10_particle_spawn_counter;
    ParticlesState field_14_particles_state;
    s16 field_16_particleIdx;
    OrbWhirlWindParticle* field_18_particles[16];
    FP field_58_xpos;
    FP field_5C_ypos;
    FP field_60_scale;
};
ALIVE_ASSERT_SIZEOF(OrbWhirlWind, 0x64);

} // namespace AO
