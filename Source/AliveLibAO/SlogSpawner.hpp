#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_SlogSpawner final : public Path_TLV
{
    Scale_short field_18_scale;
    s16 field_1A_max_slogs;
    s16 field_1C_max_slogs_at_a_time;
    XDirection_short field_1E_direction;
    s16 field_20_slog_spawn_delay;
    s16 field_22_spawner_switch_id;
};
ALIVE_ASSERT_SIZEOF(Path_SlogSpawner, 0x24);

class SlogSpawner final : public BaseGameObject
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
    s16 field_16_padding;
    FP field_18_xPos;
    FP field_1C_yPos;
    s32 field_20_spawn_timer;
    Scale_short field_24_scale;
    u16 field_26_max_slogs;
    u16 field_28_max_slogs_at_a_time;
    XDirection_short field_2A_direction;
    u16 field_2C_slog_spawn_delay;
    u16 field_2E_spawner_switch_id;
};
ALIVE_ASSERT_SIZEOF(SlogSpawner, 0x30);


} // namespace AO
