#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"

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
    ParticleBurst(FP xpos, FP ypos, u32 numOfParticles, FP scale, BurstType type, s32 count);
    ~ParticleBurst();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

private:
    FP* Random_Speed(FP* random);

private:
    ParticleBurst_Item* field_F8_pRes = nullptr;
    s16 field_FC_number_of_particles = 0;
    s32 field_100_timer = 0;
    BurstType field_104_type = BurstType::eFallingRocks_0;
    s16 field_106_count = 0;
};
ALIVE_ASSERT_SIZEOF(ParticleBurst, 0x108);
