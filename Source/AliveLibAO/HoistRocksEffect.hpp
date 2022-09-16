#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

namespace AO {

class HoistParticle final : public BaseAnimatedWithPhysicsGameObject
{
public:
    HoistParticle(FP xpos, FP ypos, FP scale, AnimId animId);

    virtual void VUpdate() override;

    s16 field_E4_bHitGround;
};
ALIVE_ASSERT_SIZEOF(HoistParticle, 0xE8);

class HoistRocksEffect final : public ::BaseGameObject
{
public:
    HoistRocksEffect(relive::Path_Hoist* pTlv, const Guid& tlvInfo);
    ~HoistRocksEffect();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    FP field_10_xpos = {};
    FP field_14_ypos = {};
    Guid field_18_tlvInfo;
};
ALIVE_ASSERT_SIZEOF(HoistRocksEffect, 0x2C);

} // namespace AO
