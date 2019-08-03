#pragma once

#include "BaseGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"

struct Path_SlogSpawner : public Path_TLV
{
    __int16 field_10_scale;
    __int16 field_12_number_of_slogs;
    __int16 field_14_at_a_time;
    __int16 field_16_direction;
    __int16 field_18_ticks_between_slogs;
    __int16 field_1A_id;
    __int16 field_1C_listen_to_sligs;
    __int16 field_1E_jump_attack_delay;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SlogSpawner, 0x20);

class SlogSpawner : public BaseGameObject
{
public:
    EXPORT SlogSpawner* ctor_4C7FF0(Path_SlogSpawner* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
private:
    EXPORT SlogSpawner* vdtor_4C8080(signed int flags);
    EXPORT void vScreenChanged_4C82A0();
    EXPORT void vUpdate_4C80D0();

private:
    int field_20_tlvInfo;
    __int16 field_24_tlv_saved_slog_count;
    __int16 field_26;
    FP field_28_xpos;
    FP field_2C_ypos;
    int field_30_spawn_timer;
    __int16 field_34_scale;
    __int16 field_36_max_slogs;
    __int16 field_38_at_a_time;
    __int16 field_3A_direction;
    __int16 field_3C_spawn_time;
    __int16 field_3E_switch_id;
    __int16 field_40_listen_to_sligs;
    __int16 field_42_jump_delay;
};
ALIVE_ASSERT_SIZEOF(SlogSpawner, 0x44);
