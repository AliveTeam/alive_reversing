#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/Function.hpp"

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
    s16 field_16_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ElectricWall, 0x18);

class ElectricWall final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    ElectricWall(relive::Path_ElectricWall* pTlv, s32 tlvInfo);
    ~ElectricWall();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    s32 field_F4_tlvInfo = 0;
    s16 field_F8_switch_id = 0;
    relive::Path_ElectricWall::ElectricWallStartState field_FA_start_state = relive::Path_ElectricWall::ElectricWallStartState::eOff;
    s32 field_FC_sound_timer = 0;
};
ALIVE_ASSERT_SIZEOF(ElectricWall, 0x100);
