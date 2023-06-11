#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

enum class ScrabSpawnerStates : s32
{
    eInactive_0 = 0,
    eScrabSpawned_1 = 1
};

struct ScrabSpawnerSaveState final
{
    ReliveTypes mType;
    s16 field_2_pad;
    Guid field_4_tlvInfo;
    ScrabSpawnerStates field_8_state;
    Guid field_C_spawned_scrab_id;
};

class ScrabSpawner final : public BaseGameObject
{
public:
    ScrabSpawner(relive::Path_ScrabSpawner* pTlv, const Guid& tlvId);
    ~ScrabSpawner();

    static void CreateFromSaveState(SerializedObjectData& pBuffer);
    
    virtual void VUpdate() override;
    virtual void VGetSaveState(SerializedObjectData& pSaveBuffer) override;

private:
    Guid mTlvId;
    s16 field_24_spawner_switch_id = 0;
    relive::Path_ScrabSpawner::SpawnDirection field_26_spawn_direction = relive::Path_ScrabSpawner::SpawnDirection::eNone;
    relive::Path_TLV field_28_tlv_data = {};
    ScrabSpawnerStates field_38_state = ScrabSpawnerStates::eInactive_0;
    Guid field_3C_spawned_scrab_id;
    s16 field_40_bFindSpawnedScrab = 0;
};
