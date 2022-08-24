#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"

namespace AO {

enum ElectricWallStartState : s16
{
    eOff_0 = 0,
    eOn_1 = 1,
};

struct Path_ElectricWall final : public Path_TLV
{
    Scale_short mScale;
    s16 mSwitchId;
    ElectricWallStartState mStartState;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ElectricWall, 0x20);


class ElectricWall final : public BaseAnimatedWithPhysicsGameObject
{
public:
    ElectricWall(relive::Path_ElectricWall* pTlv, s32 tlvInfo);
    ~ElectricWall();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    s32 mTlvInfo = 0;
    s16 mSwitchId = 0;
    relive::Path_ElectricWall::ElectricWallStartState mStartState = relive::Path_ElectricWall::ElectricWallStartState::eOff;
    s32 mSoundTimer = 0;
};
ALIVE_ASSERT_SIZEOF(ElectricWall, 0xF0);

} // namespace AO
