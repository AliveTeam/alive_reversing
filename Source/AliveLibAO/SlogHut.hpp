#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseGameObject.hpp"

START_NS_AO

struct Path_SlogHut : public Path_TLV
{
    unsigned __int16 field_18_scale;
    __int16 field_1A_switch_id;
    __int16 field_1C_z_delay;
    __int16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF(Path_SlogHut, 0x20);

class SlogHut : public BaseGameObject
{
public:
    EXPORT SlogHut* ctor_472C80(Path_SlogHut* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_472CF0();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT SlogHut* Vdtor_472E30(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_472E20();

    FP field_10_xpos;
    FP field_14_ypos;
    FP field_18_scale;
    int field_1C_tlvInfo;
    __int16 field_20_switch_id;
    __int16 field_22;
    int field_24_timer;
    __int16 field_28_z_delay;
    __int16 field_2A;
};
ALIVE_ASSERT_SIZEOF(SlogHut, 0x2C);

END_NS_AO
