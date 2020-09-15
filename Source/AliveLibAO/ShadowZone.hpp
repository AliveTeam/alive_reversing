#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseGameObject.hpp"

START_NS_AO

struct Path_ShadowZone : public Path_TLV
{
    __int16 field_18_centre_w;
    __int16 field_1A_centre_h;
    __int16 field_1C_r;
    __int16 field_1E_g;
    __int16 field_20_b;
    __int16 field_22_id;
    __int16 field_24_scale;
    __int16 field_26_pad;
};
ALIVE_ASSERT_SIZEOF(Path_ShadowZone, 0x28);

class Map;

enum class LevelIds : __int16;

class ShadowZone : public BaseGameObject
{
public:
    EXPORT ShadowZone* ctor_435D30(Path_ShadowZone* pTlv, Map* pMap, int tlvInfo);

    EXPORT BaseGameObject* dtor_435F10();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT ShadowZone* Vdtor_436180(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_435FE0();

    EXPORT void GetColourAmount_435E40(FP* pOut, __int16 xpos, __int16 ypos);

    virtual void VUpdate() override;

    EXPORT void VUpdate_435F80();

    EXPORT static void CC ShadowZones_Calculate_Colour_435FF0(int xpos, int ypos, __int16 scale, short* r, short* g, short* b);

    int field_10_tlvInfo;
    LevelIds field_14_level;
    __int16 field_16_path;
    __int16 field_18_centre_x;
    __int16 field_1A_centre_y;
    __int16 field_1C_centre_mid_x;
    __int16 field_1E_centre_mid_y;
    __int16 field_20_mid_x;
    __int16 field_22_mid_y;
    __int16 field_24_id;
    __int16 field_26_scale;
    FP field_28_r;
    FP field_2C_g;
    FP field_30_b;
};
ALIVE_ASSERT_SIZEOF(ShadowZone, 0x34);

END_NS_AO

