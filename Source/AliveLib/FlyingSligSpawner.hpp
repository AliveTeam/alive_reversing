#pragma once

#include "FunctionFwd.hpp"
#include "Path.hpp"
#include "BaseGameObject.hpp"

struct Path_FlyingSlig_Data
{
    __int16 field_0_scale;
    __int16 field_2_state;
    __int16 field_4_hi_pause_time;
    __int16 field_6_patrol_pause_min;
    __int16 field_8_patrol_pause_max;
    __int16 field_A_direction;
    __int16 field_C_panic_delay;
    __int16 field_E_give_up_chase_delay;
    __int16 field_10_prechase_delay;
    __int16 field_12_slig_id;
    __int16 field_14_listen_time;
    __int16 field_16_trigger_id;
    __int16 field_18_grenade_delay;
    __int16 field_1A_max_velocity;
    __int16 field_1C_launch_id;
    __int16 field_1E_persistant;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_FlyingSlig_Data, 0x20);

struct Path_FlyingSlig : public Path_TLV
{
    Path_FlyingSlig_Data field_10_data;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_FlyingSlig, 0x30);

struct Path_FlyingSligSpawner : public Path_TLV
{
    Path_FlyingSlig_Data field_10;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_FlyingSligSpawner, 0x30);

struct FlyingSligSpawner_State
{
    Types field_0_type;
    //__int16 field_2_pad;
    int field_4_tlvInfo;
    int field_8_bSpawned;
    int field_C_spawned_slig_obj_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(FlyingSligSpawner_State, 0x10);

class FlyingSligSpawner : public BaseGameObject
{
public:
    EXPORT static signed int CC CreateFromSaveState_43B690(const BYTE* pBuffer);

    EXPORT FlyingSligSpawner* ctor_433D50(Path_FlyingSligSpawner* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual int VGetSaveState(BYTE* pSaveBuffer) override;

private:
    EXPORT void dtor_434030();
    EXPORT FlyingSligSpawner* vdtor_433DE0(signed int flags);
    EXPORT void vUpdate_433E10();
    EXPORT signed int vGetSaveState_43B730(FlyingSligSpawner_State* pSaveState);

private:
    int field_20_tlvInfo;
    int field_24_spawned_slig_id;
    __int16 field_28_trigger_id;
    __int16 field_2A_pad;
    Path_TLV field_2C_tlv_header;
    int field_3C_bSpawned;
    __int16 field_40_bFirstUpdate;
    __int16 field_42_pad;
};
ALIVE_ASSERT_SIZEOF(FlyingSligSpawner, 0x44);