#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseGameObject.hpp"

namespace AO {

enum class ShadowZoneScale : s16
{
    eBoth_0 = 0,
    eHalf_1 = 1,
    eFull_2 = 2,
};

struct Path_ShadowZone : public Path_TLV
{
    s16 field_18_centre_w;
    s16 field_1A_centre_h;
    s16 field_1C_r;
    s16 field_1E_g;
    s16 field_20_b;
    s16 field_22_id;
    ShadowZoneScale field_24_scale;
    s16 field_26_pad;
};
ALIVE_ASSERT_SIZEOF(Path_ShadowZone, 0x28);

class Map;

enum class LevelIds : s16;

class ShadowZone : public BaseGameObject
{
public:
    EXPORT ShadowZone* ctor_435D30(Path_ShadowZone* pTlv, Map* pMap, s32 tlvInfo);

    EXPORT BaseGameObject* dtor_435F10();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT ShadowZone* Vdtor_436180(s32 flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_435FE0();

    EXPORT void GetColourAmount_435E40(FP* pOut, s16 xpos, s16 ypos);

    virtual void VUpdate() override;

    EXPORT void VUpdate_435F80();

    EXPORT static void CC ShadowZones_Calculate_Colour_435FF0(s32 xpos, s32 ypos, s16 scale, short* r, short* g, short* b);

    // Note: Completely inlined in AO
    s16 ApplysToScale(s16 scale);

    s32 field_10_tlvInfo;
    LevelIds field_14_level;
    s16 field_16_path;
    s16 field_18_centre_x;
    s16 field_1A_centre_y;
    s16 field_1C_centre_mid_x;
    s16 field_1E_centre_mid_y;
    s16 field_20_mid_x;
    s16 field_22_mid_y;
    s16 field_24_id;
    ShadowZoneScale field_26_scale;
    FP field_28_r;
    FP field_2C_g;
    FP field_30_b;
};
ALIVE_ASSERT_SIZEOF(ShadowZone, 0x34);

ALIVE_VAR_EXTERN(DynamicArrayT<ShadowZone>*, sShadowZone_dArray_507B08);

}

