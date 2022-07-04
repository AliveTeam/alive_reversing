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
    Scale_short field_18_scale;
    s16 field_1A_switch_id;
    ElectricWallStartState field_1C_start_state;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ElectricWall, 0x20);


class ElectricWall final : public BaseAnimatedWithPhysicsGameObject
{
public:
    ElectricWall(Path_ElectricWall* pTlv, s32 tlvInfo);
    ~ElectricWall();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    s32 field_E4_tlv = 0;
    s16 field_E8_switch_id = 0;
    ElectricWallStartState field_EA_start_state = ElectricWallStartState::eOff_0;
    s32 field_EC_sound_timer = 0;
};
ALIVE_ASSERT_SIZEOF(ElectricWall, 0xF0);

} // namespace AO
