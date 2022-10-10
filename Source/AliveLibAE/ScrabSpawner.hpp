#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "Scrab.hpp"

enum class ScrabSpawnerStates : s32
{
    eInactive_0 = 0,
    eScrabSpawned_1 = 1
};

struct ScrabSpawner_State final
{
    AETypes field_0_type;
    s16 field_2_pad;
    Guid field_4_tlvInfo;
    ScrabSpawnerStates field_8_state;
    Guid field_C_spawned_scrab_id;
};
//ALIVE_ASSERT_SIZEOF_ALWAYS(ScrabSpawner_State, 0x10);

class ScrabSpawner final : public BaseGameObject
{
public:
    ScrabSpawner(relive::Path_ScrabSpawner* pTlv, const Guid& tlvId);
    ~ScrabSpawner();

    static s32 CreateFromSaveState(const u8* pBuffer);
    
    virtual void VUpdate() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

private:
    Guid field_20_tlvInfo;
    s16 field_24_spawner_switch_id = 0;
    relive::Path_ScrabSpawner::SpawnDirection field_26_spawn_direction = relive::Path_ScrabSpawner::SpawnDirection::eNone;
    relive::Path_TLV field_28_tlv_data = {};
    ScrabSpawnerStates field_38_state = ScrabSpawnerStates::eInactive_0;
    Guid field_3C_spawned_scrab_id;
    s16 field_40_bFindSpawnedScrab = 0;
};
