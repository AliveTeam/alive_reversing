#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
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
    s16 mStartInterval;
    s16 field_1C_fall_switch_id;
    MovementType mMovementType;
    s16 mBeesAmount;
    s16 mChaseTime;
    s16 mSpeed;
    Scale_short field_26_scale;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BeeSwarmHole, 0x28);

class BeeSwarmHole final : public BaseGameObject
{
public:
    BeeSwarmHole(relive::Path_BeeSwarmHole* pTlv, s32 tlvInfo);

    virtual void VUpdate() override;

    s32 field_10_tlvInfo = 0;
    PSX_RECT field_14_rect = {};
    s32 mStartIntervalTimer = 0;
    u16 mStartInterval = 0;
    relive::Path_BeeSwarmHole::MovementType mMovementType = relive::Path_BeeSwarmHole::MovementType::eHover;
    s16 field_26_bees_amount = 0;
    u16 field_28_chase_time = 0;
    u16 field_2A_speed = 0;
};
ALIVE_ASSERT_SIZEOF(BeeSwarmHole, 0x2C);


} // namespace AO
