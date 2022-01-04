#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Psx.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_BeeSwarmHole final : public Path_TLV
{
    enum class MovementType : s16
    {
        eHover_0 = 0,
        eAttack_1 = 1,
        eFollowPath_2 = 2,
    };
    s16 field_18_what_to_spawn;
    s16 field_1A_interval;
    s16 field_1C_fall_switch_id;
    MovementType field_1E_movement_type;
    s16 field_20_bees_amount;
    s16 field_22_chase_time;
    s16 field_24_speed;
    Scale_short field_26_scale;
};
ALIVE_ASSERT_SIZEOF(Path_BeeSwarmHole, 0x28);

class BeeSwarmHole final : public BaseGameObject
{
public:
    EXPORT BeeSwarmHole* ctor_4782B0(Path_BeeSwarmHole* pTlv, s32 tlvInfo);

    virtual void VUpdate() override;

    EXPORT void VUpdate_478320();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    s32 field_10_tlvInfo;
    PSX_RECT field_14_rect;
    s32 field_1C_interval_timer;
    u16 field_20_interval;
    s16 field_22_fall_switch_id;
    Path_BeeSwarmHole::MovementType field_24_movement_type;
    s16 field_26_bees_amount;
    u16 field_28_chase_time;
    u16 field_2A_speed;
};
ALIVE_ASSERT_SIZEOF(BeeSwarmHole, 0x2C);


} // namespace AO
