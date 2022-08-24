#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

enum class EReliveLevelIds : s16;

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
    s16 field_1C_r;
    s16 field_1E_g;
    s16 field_20_b;
    s16 field_22_id;
    ShadowZoneScale mScale;
    s16 field_26_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ShadowZone, 0x28);

class Map;

class ShadowZone final : public BaseGameObject
{
public:
    ShadowZone(relive::Path_ShadowZone* pTlv, Map* pMap, s32 tlvInfo);
    ~ShadowZone();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    void GetColourAmount(FP* pOut, s16 xpos, s16 ypos);
    static void ShadowZones_Calculate_Colour(s32 xpos, s32 ypos, Scale scale, s16* r, s16* g, s16* b);

    // Note: Completely inlined in AO
    s16 ApplysToScale(Scale scale);

    s32 field_10_tlvInfo = 0;
    EReliveLevelIds field_14_level = EReliveLevelIds::eNone;
    s16 field_16_path = 0;
    s32 field_18_centre_x = 0;
    s32 field_1A_centre_y = 0;
    s16 field_1C_centre_mid_x = 0;
    s16 field_1E_centre_mid_y = 0;
    s32 field_20_mid_x = 0;
    s32 field_22_mid_y = 0;
    s16 field_24_id = 0;
    relive::Path_ShadowZone::Scale mScale = relive::Path_ShadowZone::Scale::eBoth;
    FP field_28_r = {};
    FP field_2C_g = {};
    FP field_30_b = {};
};
ALIVE_ASSERT_SIZEOF(ShadowZone, 0x34);

ALIVE_VAR_EXTERN(DynamicArrayT<ShadowZone>*, sShadowZone_dArray_507B08);

} // namespace AO
