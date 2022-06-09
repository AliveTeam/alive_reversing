#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/Function.hpp"

struct Path_ZzzSpawner final : public Path_TLV
{
    Scale_short field_10_scale;
    s16 field_12_switch_id;
    s16 field_14_Zzz_interval;
    s16 field_16_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ZzzSpawner, 0x18);

class ZzzSpawner final : public BaseGameObject
{
public:
    ZzzSpawner(Path_ZzzSpawner* pTlv, s32 tlvInfo);
    ~ZzzSpawner();
    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    FP field_20_xpos = {};
    FP field_24_ypos = {};
    FP field_28_scale = {};
    s32 field_2C_tlvInfo = 0;
    s16 field_30_switch_id = 0;
    s32 field_34_Zzz_timer = 0;
    s16 field_38_Zzz_interval = 0;
};
ALIVE_ASSERT_SIZEOF(ZzzSpawner, 0x3C);
