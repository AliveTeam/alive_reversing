#pragma once

#include "BaseGameObject.hpp"
#include "Slurg.hpp"

struct Slurg_Spawner_Path_Data : public Slurg_Path_Data
{
    __int16 field_8_delay_between_slurgs;
    __int16 field_A_max_slurgs;
    __int16 field_C_switch_id;
    __int16 field_E_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Slurg_Spawner_Path_Data, 0x10);

struct Path_SlurgSpawner : public Path_Slurg
{
    Slurg_Spawner_Path_Data field_18_path_data;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SlurgSpawner, 0x28);

// NOTE: Apparently this object is never used - would kind of make sense as it reads
// the slurg spawned count from the TLV but never updates it.
// Also it hasn't got quiksave support.
class SlurgSpawner : public BaseGameObject
{
public:
    EXPORT SlurgSpawner* ctor_4C82E0(Path_SlurgSpawner* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    EXPORT void vUpdate_4C83C0();
    EXPORT void dtor_4C83A0();
    EXPORT SlurgSpawner* vdtor_4C8370(signed int flags);
    EXPORT void vScreenChanged_4C84A0();

private:
    int field_20_tlvInfo;
    Path_SlurgSpawner* field_24_slurg_tlv;
    Slurg_Spawner_Path_Data field_28_path_data;
    int field_38_padding;
    __int16 field_3C_padding;
    __int16 field_3E_delay_counter;
    BYTE field_40_spawned_count;
    char field_41_padding;
    __int16 field_42_padding;
};
ALIVE_ASSERT_SIZEOF(SlurgSpawner, 0x44);
