#pragma once

#include "FunctionFwd.hpp"
#include "Path.hpp"
#include "BaseGameObject.hpp"
#include "Font.hpp"

struct Path_GasCountDown : public Path_TLV
{
    __int16 field_10_start_trigger_id;
    unsigned __int16 field_12_time;
    __int16 field_14_stop_trigger_id;
    __int16 field_16_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_GasCountDown, 0x18);

class GasCountDown : public BaseGameObject
{
public:
    EXPORT GasCountDown* ctor_417010(Path_GasCountDown* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VScreenChanged() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VUpdate() override;

private:
    EXPORT void dtor_417220();

    EXPORT GasCountDown* vdtor_4171F0(signed int flags);

    EXPORT void vScreenChanged_417700();

    EXPORT void vRender_4175A0(int **pOt);

    EXPORT void vUpdate_4172E0();

private:
    Font_Context field_20_font_context;
    Alive::Font field_30_font;
    int field_68_tlvInfo;
    __int16 field_6C_xpos;
    __int16 field_6E_ypos;
    __int16 field_70_start_trigger_id;
    __int16 field_72_stop_trigger_id;
    __int16 field_74_time_left;
    unsigned __int16 field_76_time;
};
ALIVE_ASSERT_SIZEOF(GasCountDown, 0x78);

ALIVE_VAR_EXTERN(int, sGasTimer_5C1BE8);
ALIVE_VAR_EXTERN(short, gGasOn_5C1C00);
