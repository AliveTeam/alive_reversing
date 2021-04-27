#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"

namespace AO {

struct Path_SlogSpawner : public Path_TLV
{
    s16 field_18_scale;
    s16 field_1A_num_slogs;
    s16 field_1C_num_at_a_time;
    s16 field_1E_direction;
    s16 field_20_ticks_between_slogs;
    s16 field_22_start_id;
};
ALIVE_ASSERT_SIZEOF(Path_SlogSpawner, 0x24);

class SlogSpawner : public BaseGameObject
{
public:
    EXPORT SlogSpawner* ctor_475DD0(Path_SlogSpawner* pTlv, s32 tlvInfo);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_475F90();

    BaseGameObject* VDestructor(s32 flags) override;

    virtual void VUpdate() override;

    EXPORT void VUpdate_475E30();

    s32 field_10_tlvInfo;
    s16 field_14_spawned_count;
    s16 field_16;
    FP field_18_xPos;
    FP field_1C_yPos;
    s32 field_20_spawn_timer;
    s16 field_24_scale;
    u16 field_26_num_slogs;
    u16 field_28_num_at_a_time;
    s16 field_2A_direction;
    u16 field_2C_ticks_between_slogs;
    u16 field_2E_start_id;
};
ALIVE_ASSERT_SIZEOF(SlogSpawner, 0x30);


} // namespace AO
