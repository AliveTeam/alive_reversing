#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

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
    ParticleBurst(FP xpos, FP ypos, s16 particleCount, FP scale, BurstType type);
    ~ParticleBurst();

    virtual void VUpdate() override;

    EXPORT void VUpdate_40D600();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_40D7F0(PrimHeader** ppOt);

    s32 field_D4_padding[4];
    u8** field_E4_ppRes;
    ParticleBurst_Item* field_E8_pRes;
    s16 field_EC_count;
    s16 field_EE_padding;
    s32 field_F0_timer;
    BurstType field_F4_type;
    s16 field_F6_padding;
};
ALIVE_ASSERT_SIZEOF(ParticleBurst, 0xF8);

} // namespace AO
