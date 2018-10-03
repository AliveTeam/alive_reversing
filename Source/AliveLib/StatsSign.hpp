#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Path.hpp"
#include "Font.hpp"

void StatsSign_ForceLink();

struct Path_StatsSign
{
    Path_TLV field_0_mBase;
    __int16 field_10_number_of_muds;
    __int16 field_12_zulag_number;
    int field_14_hidden;
};


class StatsSign : public BaseGameObject
{
public:
    EXPORT StatsSign * ctor_47B600(Path_StatsSign *params, int a3);
    EXPORT void dtor_47B7E0();

    EXPORT void vUpdate_47B8D0();
    EXPORT void vRender_47B900(int **pOt);
    EXPORT void vScreenChanged_47BC40();
public:
    Font field_20_font1;
    Font field_58_font2;
    Font field_90_font3;
    Font field_C8_font4;
    int field_100_objectId;
    __int16 field_104_position_x;
    __int16 field_106_position_y;
    __int16 field_108_is_hidden;
    __int16 field_10A_muds_left_in_area;
};
ALIVE_ASSERT_SIZEOF(StatsSign, 0x10C);