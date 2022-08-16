#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "../AliveLibAO/Slig.hpp"

namespace AO {

struct Path_Slig;

struct Path_SligSpawner final : public Path_TLV
{
    Scale_short field_18_scale;
    Path_Slig::StartState field_1A_start_state;
    s16 field_1C_pause_time;
    s16 field_1E_pause_left_min;
    s16 field_20_pause_left_max;
    s16 field_22_pause_right_min;
    s16 field_24_pause_right_max;
    Path_Slig::ShootPossessedSligs field_26_shoot_possessed_sligs;
    s16 field_28_shoot_on_sight_delay;
    s16 field_2A_num_times_to_shoot;
    s16 field_2C_unknown; // TODO: Part of above field, check me?
    s16 field_2E_code1;
    s16 field_30_code2;
    Choice_short field_32_chase_abe;
    XDirection_short field_34_start_direction;
    s16 field_36_panic_timeout;
    s16 field_38_num_panic_sounds;
    s16 field_3A_panic_sound_timeout;
    s16 field_3C_stop_chase_delay;
    s16 field_3E_time_to_wait_before_chase;
    s16 field_40_slig_bound_id;
    s16 field_42_listen_time;
    s16 field_44_percent_say_what;
    s16 field_46_percent_beat_mud;
    s16 field_48_talk_to_abe;
    s16 field_4A_dont_shoot;
    s16 field_4C_z_shoot_delay;
    Choice_short field_4E_stay_awake;
    s16 field_50_disable_resources;
    s16 field_52_noise_wake_up_distance;
    s32 field_54_slig_spawner_switch_id;
};

class SligSpawner final : public BaseGameObject
{
public:
    SligSpawner(Path_Slig* pTlv, s32 tlvInfo);
    ~SligSpawner();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    

private:
    void VScreenChanged_402960();
    void VUpdate_4028A0();

private:
    s32 field_10_tlvInfo = 0;
    u16 field_14_slig_spawner_switch_id = 0;
    s16 field_16_flags = 0;
    Path_TLV field_18_tlv = {};
};
ALIVE_ASSERT_SIZEOF(SligSpawner, 0x30);

} // namespace AO
