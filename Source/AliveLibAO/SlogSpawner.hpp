#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

enum class StartDirection : s16
{
    eRight_0 = 0,
    eLeft_1 = 1
};

struct Path_SlogSpawner final : public Path_TLV
{
    Scale_short field_18_scale;
    s16 field_1A_max_slogs;
    s16 field_1C_max_slogs_at_a_time;
    StartDirection field_1E_start_direction;
    s16 field_20_slog_spawn_interval;
    s16 field_22_spawner_switch_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SlogSpawner, 0x24);

class SlogSpawner final : public BaseGameObject
{
public:
    SlogSpawner(Path_SlogSpawner* pTlv, s32 tlvInfo);

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    s32 field_10_tlvInfo;
    s16 field_14_spawned_count;
    FP field_18_xPos;
    FP field_1C_yPos;
    s32 field_20_spawn_timer;
    Scale_short field_24_scale;
    u16 field_26_max_slogs;
    u16 field_28_max_slogs_at_a_time;
    StartDirection field_2A_start_direction;
    u16 field_2C_slog_spawn_interval;
    u16 field_2E_spawner_switch_id;
};
ALIVE_ASSERT_SIZEOF(SlogSpawner, 0x30);


} // namespace AO
