#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"

namespace AO {

class ElectricWall final : public BaseAnimatedWithPhysicsGameObject
{
public:
    ElectricWall(relive::Path_ElectricWall* pTlv, const Guid& tlvId);
    ~ElectricWall();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    Guid mTlvInfo;
    s16 mSwitchId = 0;
    relive::Path_ElectricWall::ElectricWallStartState mStartState = relive::Path_ElectricWall::ElectricWallStartState::eOff;
    s32 mSoundTimer = 0;
};
ALIVE_ASSERT_SIZEOF(ElectricWall, 0xF0);

} // namespace AO
