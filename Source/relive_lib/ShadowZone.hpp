#pragma once

#include "BaseGameObject.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "data_conversion/relive_tlvs.hpp"

enum class EReliveLevelIds : s16;

class Map;

class ShadowZone final : public BaseGameObject
{
public:
    static void MakeArray();
    static void FreeArray();

    ShadowZone(relive::Path_ShadowZone* pTlv, const Guid& tlvId);
    ~ShadowZone();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    void GetColourAmount(FP* pOut, s16 xpos, s16 ypos);
    static void ShadowZones_Calculate_Colour(s32 xpos, s32 ypos, Scale scale, s16* r, s16* g, s16* b);

    // Note: Completely inlined in AO
    s16 ApplysToScale(Scale scale);

    Guid mTlvId;
    EReliveLevelIds field_14_level = EReliveLevelIds::eNone;
    s16 field_16_path = 0;
    s32 field_18_centre_x = 0;
    s32 field_1A_centre_y = 0;
    s16 field_1C_centre_mid_x = 0;
    s16 field_1E_centre_mid_y = 0;
    s16 field_20_mid_x = 0;
    s16 field_22_mid_y = 0;
    s16 field_24_id = 0;
    relive::Path_ShadowZone::Scale mScale = relive::Path_ShadowZone::Scale::eBoth;
    FP field_28_r = {};
    FP field_2C_g = {};
    FP field_30_b = {};
};

extern DynamicArrayT<ShadowZone>* sShadowZoneArray;
