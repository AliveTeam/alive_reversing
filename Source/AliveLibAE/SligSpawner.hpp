#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "Psx.hpp"
#include "Path.hpp"
#include "Slig.hpp"

struct Path_SligSpawner final : public Path_TLV
{
    Scale_short scale;
    Path_Slig::StartState start_state;
    s16 pause_time;
    s16 pause_left_min;
    s16 pause_left_max;
    s16 pause_right_min;
    s16 pause_right_max;
    Path_Slig::ShootPossessedSligs shoot_possessed_sligs;
    s16 shoot_on_sight_delay;
    s16 number_of_times_to_shoot;
    s16 unknown; // maybe number of times to shoot after spawning?
    s16 code_1;
    s16 code_2;
    XDirection_short chase_abe_when_spotted;
    XDirection_short start_direction;
    s16 panic_timeout;
    s16 num_panic_sounds;
    s16 panic_sound_timeout;
    s16 stop_chase_delay;
    s16 time_to_wait_before_chase;
    s16 slig_bound_id;
    s16 alerted_listen_time;
    s16 percent_say_what;
    s16 percent_beat_mud;
    s16 talk_to_abe;
    s16 dont_shoot;
    s16 z_shoot_delay;
    s16 stay_awake;
    s16 disable_resources;
    s16 noise_wake_up_distance;
    s16 id;
    Choice_short unlimited_spawns;
};

enum class SpawnerStates : s16
{
    eInactive_0 = 0,
    eSligSpawned_1 = 1,
};

struct Slig_Spawner_State final
{
    AETypes field_0_type;
    s16 field_2_padding;
    s32 field_4_tlvInfo;
    SpawnerStates field_8_state;
    s16 field_A_padding;
    s32 field_C_spawned_slig_obj_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Slig_Spawner_State, 0x10);

class SligSpawner final : public BaseGameObject
{
public:
    SligSpawner(Path_Slig* pTlv, s32 tlvInfo);
    ~SligSpawner();

    static s32 CreateFromSaveState(const u8* pBuffer);

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

private:
    s32 field_20_tlv_info = 0;
    s16 field_24_slig_spawner_switch_id = 0;
    enum SpawnerFlags : s16
    {
        eBit1_DontDestroyTLV = 0x1,
        eBit2_UnlimitedSpawns = 0x2,
    };
    BitField16<SpawnerFlags> field_26_flags = {};
    Path_TLV field_28_tlv = {};
    SpawnerStates field_38_state = SpawnerStates::eInactive_0;
    s32 field_3C_spawned_slig_obj_id = 0;
    s16 field_40_bFindSpawnedSlig = 0;
};
ALIVE_ASSERT_SIZEOF(SligSpawner, 0x44);
