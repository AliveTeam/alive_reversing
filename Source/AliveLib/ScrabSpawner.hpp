#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"
#include "Path.hpp"
#include "Scrab.hpp"

struct Path_Scrab_Spawner : public Path_Scrab
{
    __int16 field_2C_switch_id;
    __int16 field_2E_scale;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Scrab_Spawner, 0x30);

struct ScrabSpawner_State
{
    Types field_0_type;
    __int16 field_2_pad;
    int field_4_tlvInfo;
    int field_8_state;
    int field_C_spawned_scrab_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(ScrabSpawner_State, 0x10);

class ScrabSpawner : public BaseGameObject
{
public:
    EXPORT ScrabSpawner* ctor_4AB450(Path_Scrab_Spawner* pTlv, int tlvInfo);
    EXPORT static int CC CreateFromSaveState_4ABEB0(const BYTE* pBuffer);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual int VGetSaveState(BYTE* pSaveBuffer) override;

private:
    EXPORT ScrabSpawner* vdtor_4AB4E0(signed int flags);
    EXPORT void dtor_4AB720();
    EXPORT int vGetSaveState_4ABF50(ScrabSpawner_State* pSaveState);
    EXPORT void vUpdate_4AB510();

private:
    int field_20_tlvInfo;
    __int16 field_24_switch_id;
    __int16 field_26_spawn_scale;
    Path_TLV field_28_tlv_data;
    int field_38_state;
    int field_3C_spawned_scrab_id;
    __int16 field_40_bFindSpawnedScrab;
    __int16 field_42;
};
ALIVE_ASSERT_SIZEOF(ScrabSpawner, 0x44);
