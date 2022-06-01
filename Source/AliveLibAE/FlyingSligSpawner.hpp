#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Path.hpp"
#include "BaseGameObject.hpp"

struct Path_FlyingSlig_Data final
{
    Scale_short field_0_scale;
    enum class SpawnDelayStates : s16
    {
        eMoveImmediately_0 = 0,
        eUseCustomSpawnMoveDelay_1 = 1,
    };
    SpawnDelayStates field_2_state;
    s16 field_4_spawn_move_delay;
    s16 field_6_patrol_pause_min;
    s16 field_8_patrol_pause_max;
    XDirection_short field_A_direction;
    s16 field_C_panic_delay;
    s16 field_E_give_up_chase_delay;
    s16 field_10_prechase_delay;
    s16 field_12_slig_bound_id;
    s16 field_14_alerted_listen_time;
    s16 field_16_spawner_switch_id;
    s16 field_18_grenade_delay;
    s16 field_1A_max_velocity;
    s16 field_1C_launch_switch_id;
    Choice_short field_1E_persistant;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_FlyingSlig_Data, 0x20);

struct Path_FlyingSlig final : public Path_TLV
{
    Path_FlyingSlig_Data field_10_data;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_FlyingSlig, 0x30);

struct Path_FlyingSligSpawner final : public Path_TLV
{
    Path_FlyingSlig_Data field_10;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_FlyingSligSpawner, 0x30);

struct FlyingSligSpawner_State final
{
    AETypes field_0_type;
    //s16 field_2_pad;
    s32 field_4_tlvInfo;
    s32 field_8_bSpawned;
    s32 field_C_spawned_slig_obj_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(FlyingSligSpawner_State, 0x10);

class FlyingSligSpawner final : public BaseGameObject
{
public:
    static s32 CreateFromSaveState(const u8* pBuffer);

    FlyingSligSpawner(Path_FlyingSligSpawner* pTlv, s32 tlvInfo);
    ~FlyingSligSpawner();
    
    virtual void VUpdate() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

private:
    void vUpdate_433E10();
    s32 vGetSaveState_43B730(FlyingSligSpawner_State* pSaveState);

private:
    s32 field_20_tlvInfo;
    s32 field_24_spawned_slig_id;
    s16 field_28_spawner_switch_id;
    s16 field_2A_pad;
    Path_TLV field_2C_tlv_header;
    s32 field_3C_bSpawned;
    s16 field_40_bFirstUpdate;
    s16 field_42_pad;
};
ALIVE_ASSERT_SIZEOF(FlyingSligSpawner, 0x44);
