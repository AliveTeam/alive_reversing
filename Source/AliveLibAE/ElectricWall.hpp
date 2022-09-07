#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/Function.hpp"

class ElectricWall final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    ElectricWall(relive::Path_ElectricWall* pTlv, const Guid& tlvId);
    ~ElectricWall();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    Guid field_F4_tlvInfo;
    s16 field_F8_switch_id = 0;
    relive::Path_ElectricWall::ElectricWallStartState field_FA_start_state = relive::Path_ElectricWall::ElectricWallStartState::eOff;
    s32 field_FC_sound_timer = 0;
};
ALIVE_ASSERT_SIZEOF(ElectricWall, 0x100);
