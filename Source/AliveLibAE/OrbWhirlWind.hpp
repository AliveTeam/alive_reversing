#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/FixedPoint.hpp"
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

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    void ToSpin(FP xpos, FP ypos, FP scale, BaseGameObject* pObj);
    void ToStop();

private:
    s32 mParticleSpawnCounter = 0;
    ParticlesState mState = ParticlesState::eCreating;
    bool mIsMudokonSpirit = false;
    s16 mParticleIdx = 0;
    OrbWhirlWindParticle* mOrbParticles[16] = {};
    FP mXPos = {};
    FP mYPos = {};
    FP mScale = {};
};
