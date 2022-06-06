#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "../relive_lib/BaseGameObject.hpp"

enum class EReliveLevelIds : s16;

namespace AO {

struct Path_BeeNest final : public Path_TLV
{
    s16 field_18_id;
    s16 field_1A_swarm_size;
    s16 field_1C_chase_time;
    s16 field_1E_speed;
    s16 field_20_disabled_resources;
    s16 field_22_num_bees;
};
ALIVE_ASSERT_SIZEOF(Path_BeeNest, 0x24);

enum class BeeNestStates : u16
{
    eWaitForTrigger_0,
    eResetIfDead_1
};

class BeeSwarm;


class BeeNest final : public BaseGameObject
{
public:
    BeeNest(Path_BeeNest* pTlv, s32 tlvInfo);
    ~BeeNest();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    FP field_10_bee_x;
    FP field_14_bee_y;
    EReliveLevelIds field_18_level;
    s16 field_1A_path;
    s32 field_1C_tlvInfo;
    PSX_Point field_20_top_left;
    PSX_Point field_24_bottom_right;
    u16 field_28_switch_id;
    s16 field_2A_swarm_size;
    u16 field_2C_chase_ticks;
    BeeNestStates field_2E_state;
    FP field_30_speed;
    BeeSwarm* field_34_pBeeSwarm;
};
ALIVE_ASSERT_SIZEOF(BeeNest, 0x38);


} // namespace AO
