#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

namespace AO {

class HoistParticle final : public BaseAnimatedWithPhysicsGameObject
{
public:
    HoistParticle(FP xpos, FP ypos, FP scale, AnimId animId);

    virtual void VUpdate() override;

    bool mHitGround = false;
};

class HoistRocksEffect final : public ::BaseGameObject
{
public:
    HoistRocksEffect(relive::Path_Hoist* pTlv, const Guid& tlvInfo);
    ~HoistRocksEffect();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    FP mTlvXPos = {};
    FP mTlvYPos = {};
    Guid mTlvId;
};

} // namespace AO
