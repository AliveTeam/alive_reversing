#pragma once

#include "../../AliveLibAE/Path.hpp"

enum class ShadowZoneScale : s16
{
    eBoth_0 = 0,
    eHalf_1 = 1,
    eFull_2 = 2,
};

struct Path_ShadowZone final : public Path_TLV
{
    s16 field_10_center_w;
    s16 field_12_center_h;
    u16 mRed;
    u16 mGreen;
    u16 mBlue;
    s16 field_1A_switch_id;
    ShadowZoneScale mScale;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ShadowZone, 0x20);
