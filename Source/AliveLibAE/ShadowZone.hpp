#pragma once

#include "BaseGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"

enum class ShadowZoneScale : s16
{
    eBoth_0 = 0,
    eHalf_1 = 1,
    eFull_2 = 2,
};

struct Path_ShadowZone : public Path_TLV
{
    s16 field_10_center_w;
    s16 field_12_center_h;
    u16 field_14_r;
    u16 field_16_g;
    u16 field_18_b;
    s16 field_1A_id;
    ShadowZoneScale field_1C_scale;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ShadowZone, 0x20);

enum class LevelIds : s16;

class ShadowZone : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    EXPORT ShadowZone* ctor_463900(Path_ShadowZone* pTlv, Path* pPath, DWORD tlvInfo);
    EXPORT static void CC ShadowZones_Calculate_Colour_463CE0(int xpos, int ypos, s16 scale, short* r, short* g, short* b);
private:
    EXPORT void dtor_463BB0();
    EXPORT ShadowZone* vdtor_463A70(signed int flags);
    EXPORT void vScreenChanged_463CC0();
    EXPORT s16 ApplysToScale_463C70(s16 scale);
    EXPORT void vUpdate_463C40();
    EXPORT FP* GetColourAmount_463AA0(FP* pOut, s16 xpos, s16 ypos);
private:
    int field_20_tlvInfo;

    LevelIds  field_24_level;
    s16 field_26_path;

    s16 field_28_centre_x;
    s16 field_2A_center_y;

    s16 field_2C_centre_mid_x;
    s16 field_2E_centre_mid_y;

    s16 field_30_mid_x;
    s16 field_32_mid_y;

    s16 field_34_id;

    ShadowZoneScale field_36_scale;

    FP field_38_r;
    FP field_3C_g;
    FP field_40_b;
};
ALIVE_ASSERT_SIZEOF(ShadowZone, 0x44);
