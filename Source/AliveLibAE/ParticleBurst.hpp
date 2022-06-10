#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
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
    u8** field_F4_ppRes;
    ParticleBurst_Item* field_F8_pRes;
    s16 field_FC_number_of_particles;
    s32 field_100_timer;
    BurstType field_104_type;
    s16 field_106_count;
};
ALIVE_ASSERT_SIZEOF(ParticleBurst, 0x108);
