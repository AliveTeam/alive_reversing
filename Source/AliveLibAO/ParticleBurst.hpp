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
    virtual void VRender(PrimHeader** ppOt) override;

    ParticleBurst_Item* field_E8_pRes = nullptr;
    s16 field_EC_count = 0;
    s32 field_F0_timer = 0;
    BurstType field_F4_type = BurstType::eFallingRocks_0;
};

} // namespace AO
