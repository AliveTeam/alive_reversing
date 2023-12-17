#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"

struct ParticleBurst_Item;

enum class BurstType : s16
{
    eFallingRocks_0 = 0,
    eSticks_1 = 1,
    eBigPurpleSparks_2 = 2,
    eBigRedSparks_3 = 3,
    eInvalid_4 = 4,
    eGreenSparks_5 = 5,
    eSmallPurpleSparks_6 = 6,
};

class ParticleBurst final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    ParticleBurst(FP xpos, FP ypos, u32 particleCount, FP scale, BurstType type, s32 unknownCount);
    ~ParticleBurst();

    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;

private:
    FP* Random_Speed(FP* random);

private:
    ParticleBurst_Item* mParticleItems = nullptr;
    s16 mParticleCount = 0;
    u32 mAliveTimer = 0;
    BurstType mType = BurstType::eFallingRocks_0;
    s16 mUnknownCount = 0;
};
