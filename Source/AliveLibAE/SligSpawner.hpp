#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"
#include "Psx.hpp"
#include "Path.hpp"

struct Path_Slig;

struct Slig_Spawner_State
{
    Types field_0_type;
    __int16 field_2_padding;
    int field_4_tlvInfo;
    short field_8_state;
    short field_A_padding;
    int field_C_spawned_slig_obj_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Slig_Spawner_State, 0x10);

class SligSpawner : public BaseGameObject
{
public:
    EXPORT SligSpawner* ctor_409740(Path_Slig* pTlv, int tlvInfo);
    EXPORT static int CC CreateFromSaveState_409B10(const BYTE* pBuffer);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual int VGetSaveState(BYTE* pSaveBuffer) override;

private:
    EXPORT void dtor_409A70();
    EXPORT SligSpawner* vdtor_409800(signed int flags);
    EXPORT void vScreenChanged_409A30();
    EXPORT void vUpdate_409830();
    EXPORT int vGetSaveState_409BB0(Slig_Spawner_State* pState);

private:
    int field_20_tlv_info;
    __int16 field_24_slig_id;
    __int16 field_26_flags;
    Path_TLV field_28_tlv;
    __int16 field_38_state;
    __int16 field_3A_padding;
    int field_3C_spawned_slig_obj_id;
    __int16 field_40_bFindSpawnedSlig;
    __int16 field_42_padding;
};
ALIVE_ASSERT_SIZEOF(SligSpawner, 0x44);
