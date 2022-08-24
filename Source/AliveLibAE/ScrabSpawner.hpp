#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "Path.hpp"
#include "Scrab.hpp"

struct Path_ScrabSpawner final : public Path_Scrab
{
    s16 mSpawnerSwitchId;
    ScrabSpawnDirection mFacing;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ScrabSpawner, 0x30);

enum class ScrabSpawnerStates : s32
{
    eInactive_0 = 0,
    eScrabSpawned_1 = 1
};

struct ScrabSpawner_State final
{
    AETypes field_0_type;
    s16 field_2_pad;
    s32 field_4_tlvInfo;
    ScrabSpawnerStates field_8_state;
    s32 field_C_spawned_scrab_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(ScrabSpawner_State, 0x10);

class ScrabSpawner final : public BaseGameObject
{
public:
    ScrabSpawner(Path_ScrabSpawner* pTlv, s32 tlvInfo);
    ~ScrabSpawner();

    static s32 CreateFromSaveState(const u8* pBuffer);
    
    virtual void VUpdate() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

private:
    s32 field_20_tlvInfo = 0;
    s16 field_24_spawner_switch_id = 0;
    ScrabSpawnDirection field_26_spawn_direction = ScrabSpawnDirection::eNone_0;
    Path_TLV field_28_tlv_data = {};
    ScrabSpawnerStates field_38_state = ScrabSpawnerStates::eInactive_0;
    s32 field_3C_spawned_scrab_id = 0;
    s16 field_40_bFindSpawnedScrab = 0;
};
ALIVE_ASSERT_SIZEOF(ScrabSpawner, 0x44);
