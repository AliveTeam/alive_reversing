#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"

namespace relive
{
    struct Path_FlyingSligSpawner;
    class Path_TLV;
}

struct FlyingSligSpawner_State final
{
    AETypes field_0_type;
    s16 field_2_pad;
    Guid field_4_tlvInfo;
    s32 field_8_bSpawned;
    Guid field_C_spawned_slig_obj_id;
};
//ALIVE_ASSERT_SIZEOF_ALWAYS(FlyingSligSpawner_State, 0x10);

class FlyingSligSpawner final : public BaseGameObject
{
public:
    static s32 CreateFromSaveState(const u8* pBuffer);

    FlyingSligSpawner(relive::Path_FlyingSligSpawner* pTlv, const Guid& tlvId);
    ~FlyingSligSpawner();
    
    virtual void VUpdate() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

private:
    Guid field_20_tlvInfo;
    Guid field_24_spawned_slig_id;
    s16 field_28_spawner_switch_id = 0;
    relive::Path_TLV* field_2C_tlv_header = {};
    s32 field_3C_bSpawned = 0;
    s16 field_40_bFirstUpdate = 0;
};
ALIVE_ASSERT_SIZEOF(FlyingSligSpawner, 0x44);
