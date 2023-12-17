#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

struct ParticleBurst_Item;

enum class BurstType : s16
{
    eFallingRocks_0 = 0,
    eSticks_1 = 1,
    eBigPurpleSparks_2 = 2,
    eBigRedSparks_3 = 3,
    eMeat_4 = 4,
};

class ParticleBurst final : public BaseAnimatedWithPhysicsGameObject
{
public:
    ParticleBurst(FP xpos, FP ypos, s32 particleCount, FP scale, BurstType type);
    ~ParticleBurst();

    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;

    ParticleBurst_Item* mParticleItems = nullptr;
    u32 mAliveTimer = 0;
    BurstType mType = BurstType::eFallingRocks_0;
    s16 mParticleCount = 0;
};

} // namespace AO
