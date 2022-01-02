#pragma once

#include "BaseGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/FunctionFwd.hpp"

struct Path_SlogSpawner final : public Path_TLV
{
    Scale_short field_10_scale;
    s16 field_12_max_slogs;
    s16 field_14_max_slogs_at_a_time;
    XDirection_short field_16_direction;
    s16 field_18_slog_spawn_delay;
    s16 field_1A_start_id;
    Choice_short field_1C_listen_to_sligs;
    s16 field_1E_chase_delay;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SlogSpawner, 0x20);

class SlogSpawner final : public BaseGameObject
{
public:
    EXPORT SlogSpawner* ctor_4C7FF0(Path_SlogSpawner* pTlv, s32 tlvInfo);

    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    EXPORT SlogSpawner* vdtor_4C8080(s32 flags);
    EXPORT void vScreenChanged_4C82A0();
    EXPORT void vUpdate_4C80D0();

private:
    s32 field_20_tlvInfo;
    s16 field_24_tlv_saved_slog_count;
    s16 field_26_padding;
    FP field_28_xpos;
    FP field_2C_ypos;
    s32 field_30_spawn_timer;
    Scale_short field_34_scale;
    s16 field_36_max_slogs;
    s16 field_38_max_slogs_at_a_time;
    XDirection_short field_3A_direction;
    s16 field_3C_slog_spawn_delay;
    s16 field_3E_start_id;
    Choice_short field_40_listen_to_sligs;
    s16 field_42_chase_delay;
};
ALIVE_ASSERT_SIZEOF(SlogSpawner, 0x44);
