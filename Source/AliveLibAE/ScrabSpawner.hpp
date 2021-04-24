#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"
#include "Path.hpp"
#include "Scrab.hpp"

struct Path_ScrabSpawner : public Path_Scrab
{
    s16 field_2C_switch_id;
    ScrabSpawnDirection field_2E_spawn_direction;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ScrabSpawner, 0x30);

struct ScrabSpawner_State
{
    AETypes field_0_type;
    s16 field_2_pad;
    int field_4_tlvInfo;
    int field_8_state;
    int field_C_spawned_scrab_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(ScrabSpawner_State, 0x10);

class ScrabSpawner : public BaseGameObject
{
public:
    EXPORT ScrabSpawner* ctor_4AB450(Path_ScrabSpawner* pTlv, int tlvInfo);
    EXPORT static int CC CreateFromSaveState_4ABEB0(const BYTE* pBuffer);

    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual int VGetSaveState(BYTE* pSaveBuffer) override;

private:
    EXPORT ScrabSpawner* vdtor_4AB4E0(s32 flags);
    EXPORT void dtor_4AB720();
    EXPORT int vGetSaveState_4ABF50(ScrabSpawner_State* pSaveState);
    EXPORT void vUpdate_4AB510();

private:
    int field_20_tlvInfo;
    s16 field_24_switch_id;
    ScrabSpawnDirection field_26_spawn_direction;
    Path_TLV field_28_tlv_data;
    int field_38_state;
    int field_3C_spawned_scrab_id;
    s16 field_40_bFindSpawnedScrab;
    s16 field_42_padding;
};
ALIVE_ASSERT_SIZEOF(ScrabSpawner, 0x44);
