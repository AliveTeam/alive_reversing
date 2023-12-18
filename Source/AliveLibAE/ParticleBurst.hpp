#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"

struct ParticleBurst_Item;

enum class BurstType
{
    eRocks,
    eSticks,
    eBigPurpleSparks,
    eBigRedSparks,
    eGreenSparks,
    eSmallPurpleSparks,
    eMeat,
};

class ParticleBurst final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    ParticleBurst(FP xpos, FP ypos, u32 particleCount, FP scale, BurstType type, s32 fineScale = 13, bool bFadeOut = false);
    ~ParticleBurst();

    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;

private:
    FP Random_Speed(FP scale);

private:
    ParticleBurst_Item* mParticleItems = nullptr;
    s16 mParticleCount = 0;
    u32 mAliveTimer = 0;
    BurstType mType = BurstType::eRocks;
    s16 mFineScale = 0;
    bool mFadeout = false;
};
