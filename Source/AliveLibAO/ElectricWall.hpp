#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

namespace AO {

class ElectricWall final : public BaseAnimatedWithPhysicsGameObject
{
public:
    ElectricWall(relive::Path_ElectricWall* pTlv, const Guid& tlvId);
    ~ElectricWall();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    Guid mTlvInfo;
    s16 mSwitchId = 0;
    relive::Path_ElectricWall::ElectricWallStartState mStartState = relive::Path_ElectricWall::ElectricWallStartState::eOff;
    s32 mSoundTimer = 0;
};

} // namespace AO
