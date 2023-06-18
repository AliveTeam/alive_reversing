#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/SaveStateBase.hpp"

namespace relive
{
    struct Path_FlyingSligSpawner;
    class Path_TLV;
}

struct FlyingSligSpawnerSaveState final : public SaveStateBase
{
    FlyingSligSpawnerSaveState()
        : SaveStateBase(ReliveTypes::eFlyingSligSpawner, sizeof(*this))
    { }
    Guid field_4_tlvInfo;
    s32 field_8_bSpawned;
    Guid field_C_spawned_slig_obj_id;
};

class FlyingSligSpawner final : public BaseGameObject
{
public:
    static void CreateFromSaveState(SerializedObjectData& pBuffer);

    FlyingSligSpawner(relive::Path_FlyingSligSpawner* pTlv, const Guid& tlvId);
    ~FlyingSligSpawner();
    
    virtual void VUpdate() override;
    virtual void VGetSaveState(SerializedObjectData& pSaveBuffer) override;

private:
    Guid mTlvId;
    Guid field_24_spawned_slig_id;
    s16 field_28_spawner_switch_id = 0;
    relive::Path_TLV* field_2C_tlv_header = {};
    s32 field_3C_bSpawned = 0;
    s16 field_40_bFirstUpdate = 0;
};
