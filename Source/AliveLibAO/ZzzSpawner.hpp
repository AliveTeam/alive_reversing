#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "Layer.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_ZzzSpawner final : public Path_TLV
{
    Scale_short field_18_scale;
    s16 field_1A_switch_id;
    s16 field_1C_Zzz_delay;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF(Path_ZzzSpawner, 0x20);

class ZzzSpawner final : public BaseGameObject
{
public:
    ZzzSpawner(Path_ZzzSpawner* pTlv, s32 tlvInfo);
    ~ZzzSpawner();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    FP field_10_xpos;
    FP field_14_ypos;
    FP field_18_scale;
    s32 field_1C_tlvInfo;
    s16 field_20_switch_id;
    s16 field_22_padding;
    s32 field_24_timer;
    s16 field_28_Zzz_delay;
    s16 field_2A_padding;
};
ALIVE_ASSERT_SIZEOF(ZzzSpawner, 0x2C);

} // namespace AO
