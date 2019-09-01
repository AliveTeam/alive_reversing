#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Font.hpp"
#include "Primitives.hpp"
#include "Path.hpp"

struct Path_ColourfulMeter : public Path_TLV
{
    __int16 field_10_id;
    __int16 field_12_number_of_meter_bars;
    __int16 field_14_timer;
    __int16 field_16_bStartsFull;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ColourfulMeter, 0x18);

class ColourfulMeter : public BaseGameObject
{
public:
    EXPORT ColourfulMeter* ctor_43CE40(Path_ColourfulMeter* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VScreenChanged() override;

private:
    EXPORT ColourfulMeter* vdtor_43D050(signed int flags);

    EXPORT void dtor_43D080();

    EXPORT void vScreenChanged_43D7A0();

private:
    Font_Context field_20_font_context;
    Alive::Font field_30_font;
    int field_68_tlvInfo;
    __int16 field_6C;
    __int16 field_6E;
    __int16 field_70_polys_to_render_count;
    __int16 field_72_switch_id;
    __int16 field_74;
    __int16 field_76;
    __int16 field_78_count;
    __int16 field_7A_number_of_meter_bars;
    __int16 field_7C_timer;
    __int16 field_7E_starting_switch_state;
    __int16 field_80;
    __int16 field_82_bar_count;
    __int16 field_84_bStartsFull;
    __int16 field_86;
    Poly_G4 field_88_polyG4s[40];
};
ALIVE_ASSERT_SIZEOF(ColourfulMeter, 0x6c8);

ALIVE_VAR_EXTERN(BYTE, gTotalMeterBars_5C1BFA);
