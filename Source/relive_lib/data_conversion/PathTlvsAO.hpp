#pragma once

#include "../../AliveLibAO/Path.hpp"

namespace AO {

    enum class ShadowZoneScale : s16
    {
        eBoth_0 = 0,
        eHalf_1 = 1,
        eFull_2 = 2,
    };

    struct Path_ShadowZone final : public Path_TLV
    {
        s16 field_18_centre_w;
        s16 field_1A_centre_h;
        s16 mRed;
        s16 mGreen;
        s16 mBlue;
        s16 field_22_id;
        ShadowZoneScale mScale;
        s16 field_26_pad;
    };
    ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ShadowZone, 0x28);
} // namespace AO
