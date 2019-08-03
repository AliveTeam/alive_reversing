#pragma once

#include "BaseGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"

struct Path_SlogHut : public Path_TLV
{
    unsigned __int16 field_10_scale;
    __int16 field_12_switch_id;
    __int16 field_14_z_delay;
    __int16 field_16_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SlogHut, 0x18);

class SlogHut : public BaseGameObject
{
public:
    EXPORT SlogHut* ctor_4C4070(Path_SlogHut* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    EXPORT void dtor_4C4130();
    EXPORT SlogHut* vdtor_4C4100(signed int flags);
    EXPORT void vScreenChanged_4C4280();
    EXPORT void vUpdate_4C41B0();

private:
    FP field_20_xpos;
    FP field_24_ypos;
    FP field_28_scale;
    int field_2C_tlvInfo;
    __int16 field_30_switch_id;
    __int16 field_32_pad;
    int field_34_z_timer;
    __int16 field_38_z_delay;
    __int16 field_3A_pad;
};
ALIVE_ASSERT_SIZEOF(SlogHut, 0x3C);
