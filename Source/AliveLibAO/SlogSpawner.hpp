#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"

START_NS_AO

struct Path_SlogSpawner : public Path_TLV
{
    __int16 field_18_scale;
    __int16 field_1A_num_slogs;
    __int16 field_1C_num_at_a_time;
    __int16 field_1E_direction;
    __int16 field_20_ticks_between_slogs;
    __int16 field_22_start_id;
};
ALIVE_ASSERT_SIZEOF(Path_SlogSpawner, 0x24);

class SlogSpawner : public BaseGameObject
{
public:
    EXPORT SlogSpawner* ctor_475DD0(Path_SlogSpawner* pTlv, int tlvInfo);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_475F90();

    BaseGameObject* VDestructor(signed int flags) override;

    virtual void VUpdate() override;

    EXPORT void VUpdate_475E30();

    int field_10_tlvInfo;
    __int16 field_14_spawned_count;
    __int16 field_16;
    FP field_18_xPos;
    FP field_1C_yPos;
    int field_20_spawn_timer;
    __int16 field_24_scale;
    unsigned __int16 field_26_num_slogs;
    unsigned __int16 field_28_num_at_a_time;
    __int16 field_2A_direction;
    unsigned __int16 field_2C_ticks_between_slogs;
    unsigned __int16 field_2E_start_id;
};
ALIVE_ASSERT_SIZEOF(SlogSpawner, 0x30);


END_NS_AO

