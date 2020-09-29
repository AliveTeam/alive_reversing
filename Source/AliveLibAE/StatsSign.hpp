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
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VScreenChanged() override;

    EXPORT StatsSign* ctor_47B600(Path_StatsSign* params, TlvItemInfoUnion a3);
    EXPORT void dtor_47B7E0();
    EXPORT BaseGameObject* vdtor_47B7B0(signed int flags);
    EXPORT void vUpdate_47B8D0();
    EXPORT void vRender_47B900(int** pOt);
    EXPORT void vScreenChanged_47BC40();
public:
    Alive::Font field_20_font1;
    Alive::Font field_58_font2;
    Alive::Font field_90_font3;
    Alive::Font field_C8_font4;
    int field_100_objectId;
    __int16 field_104_position_x;
    __int16 field_106_position_y;
    __int16 field_108_is_hidden;
    __int16 field_10A_muds_left_in_area;
};
ALIVE_ASSERT_SIZEOF(StatsSign, 0x10C);

ALIVE_VAR_EXTERN(__int16, sMudokonsInArea_5C1BC4);
ALIVE_VAR_EXTERN(char, sStatsSignCurrentArea_5C1A20);
