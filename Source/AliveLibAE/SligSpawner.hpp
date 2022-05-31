#pragma once

#include "BaseGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "Psx.hpp"
#include "Path.hpp"

struct Path_Slig;

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

    static s32 CreateFromSaveState_409B10(const u8* pBuffer);

    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

private:
    void vScreenChanged_409A30();
    void vUpdate_409830();
    s32 vGetSaveState_409BB0(Slig_Spawner_State* pState);

private:
    s32 field_20_tlv_info;
    s16 field_24_slig_spawner_switch_id;
    enum SpawnerFlags : s16
    {
        eBit1_DontDestroyTLV = 0x1,
        eBit2_UnlimitedSpawns = 0x2,
    };
    BitField16<SpawnerFlags> field_26_flags;
    Path_TLV field_28_tlv;
    SpawnerStates field_38_state;
    s16 field_3A_padding;
    s32 field_3C_spawned_slig_obj_id;
    s16 field_40_bFindSpawnedSlig;
    s16 field_42_padding;
};
ALIVE_ASSERT_SIZEOF(SligSpawner, 0x44);
