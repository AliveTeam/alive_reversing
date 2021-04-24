#pragma once

#include "BaseGameObject.hpp"
#include "Slurg.hpp"

struct Slurg_Spawner_Path_Data
{
    s16 field_8_spawn_delay_between_slurgs;
    s16 field_A_max_slurgs;
    s16 field_C_switch_id;
    s16 field_E_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Slurg_Spawner_Path_Data, 0x8);

struct Path_SlurgSpawner : public Path_Slurg
{
    Slurg_Spawner_Path_Data field_18_spawner_data;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SlurgSpawner, 0x20);

// NOTE: Apparently this object is never used - would kind of make sense as it reads
// the slurg spawned count from the TLV but never updates it.
// Also it hasn't got quiksave support.
class SlurgSpawner : public BaseGameObject
{
public:
    EXPORT SlurgSpawner* ctor_4C82E0(Path_SlurgSpawner* pTlv, s32 tlvInfo);

    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    EXPORT void vUpdate_4C83C0();
    EXPORT void dtor_4C83A0();
    EXPORT SlurgSpawner* vdtor_4C8370(s32 flags);
    EXPORT void vScreenChanged_4C84A0();

private:
    s32 field_20_tlvInfo;
    Path_SlurgSpawner* field_24_slurg_tlv;
    Slurg_Spawner_Path_Data field_28_path_data;
    s32 field_38_padding;
    s16 field_3C_padding;
    s16 field_3E_delay_counter;
    BYTE field_40_spawned_count;
    char field_41_padding;
    s16 field_42_padding;
};
ALIVE_ASSERT_SIZEOF(SlurgSpawner, 0x3C);
