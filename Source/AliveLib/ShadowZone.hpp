#pragma once

#include "BaseGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"

enum class ShadowZoneScale : __int16
{
    eBoth_0 = 0,
    eHalf_1 = 1,
    eFull_2 = 2,
};

struct Path_ShadowZone : public Path_TLV
{
    __int16 field_10_centre_w;
    __int16 field_12_center_h;
    unsigned __int16 field_14_r;
    unsigned __int16 field_16_g;
    unsigned __int16 field_18_b;
    __int16 field_1A_id;
    ShadowZoneScale field_1C_scale;
    //__int16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ShadowZone, 0x20);

enum class LevelIds : __int16;

class ShadowZone : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    EXPORT ShadowZone* ctor_463900(Path_ShadowZone* pTlv, Path* pPath, DWORD tlvInfo);
    EXPORT static void CC ShadowZones_Calculate_Colour_463CE0(int xpos, int ypos, __int16 scale, short* r, short* g, short* b);
private:
    EXPORT void dtor_463BB0();
    EXPORT ShadowZone* vdtor_463A70(signed int flags);
    EXPORT void vScreenChanged_463CC0();
    EXPORT __int16 ApplysToScale_463C70(__int16 scale);
    EXPORT void vUpdate_463C40();
    EXPORT FP* GetColourAmount_463AA0(FP* pOut, __int16 xpos, __int16 ypos);
private:
    int field_20_tlvInfo;

    LevelIds  field_24_level;
    __int16 field_26_path;

    __int16 field_28_centre_x;
    __int16 field_2A_center_y;

    __int16 field_2C_centre_mid_x;
    __int16 field_2E_centre_mid_y;

    __int16 field_30_mid_x;
    __int16 field_32_mid_y;

    __int16 field_34_id;

    ShadowZoneScale field_36_scale;

    FP field_38_r;
    FP field_3C_g;
    FP field_40_b;
};
ALIVE_ASSERT_SIZEOF(ShadowZone, 0x44);
