#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/Function.hpp"

enum class StartDirection : s16
{
    eRight_0 = 0,
    eLeft_1 = 1
};

struct Path_SlogSpawner final : public Path_TLV
{
    Scale_short field_10_scale;
    s16 field_12_max_slogs;
    s16 field_14_max_slogs_at_a_time;
    StartDirection field_16_start_direction;
    s16 field_18_slog_spawn_interval;
    s16 field_1A_spawner_switch_id;
    Choice_short field_1C_listen_to_sligs;
    s16 field_1E_chase_delay;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SlogSpawner, 0x20);

class SlogSpawner final : public BaseGameObject
{
public:
    SlogSpawner(Path_SlogSpawner* pTlv, s32 tlvInfo);

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    s32 field_20_tlvInfo = 0;
    s16 field_24_tlv_saved_slog_count = 0;
    FP field_28_xpos = {};
    FP field_2C_ypos = {};
    s32 field_30_spawn_timer = 0;
    Scale_short field_34_scale = Scale_short::eFull_0;
    s16 field_36_max_slogs = 0;
    s16 field_38_max_slogs_at_a_time = 0;
    StartDirection field_3A_start_direction = StartDirection::eRight_0;
    s16 field_3C_slog_spawn_interval = 0;
    s16 field_3E_spawner_switch_id = 0;
    Choice_short field_40_listen_to_sligs = Choice_short::eNo_0;
    s16 field_42_chase_delay = 0;
};
ALIVE_ASSERT_SIZEOF(SlogSpawner, 0x44);
