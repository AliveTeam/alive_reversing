#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Font.hpp"
#include "Primitives.hpp"
#include "Path.hpp"

struct Path_ColourfulMeter : public Path_TLV
{
    s16 field_10_id;
    s16 field_12_number_of_meter_bars;
    s16 field_14_mines_alarm_countdown;
    Choice_short field_16_bStartFilled;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ColourfulMeter, 0x18);

constexpr short kMeterBarsXCount = 20;

class ColourfulMeter : public BaseGameObject
{
public:
    EXPORT ColourfulMeter* ctor_43CE40(Path_ColourfulMeter* pTlv, s32 tlvInfo);

    virtual BaseGameObject* VDestructor(s32 flags) override;

    virtual void VScreenChanged() override;

    virtual void VUpdate() override;

    virtual void VRender(PrimHeader** ppOt) override;

private:
    EXPORT ColourfulMeter* vdtor_43D050(s32 flags);

    EXPORT void dtor_43D080();

    EXPORT void vScreenChanged_43D7A0();

    EXPORT void vUpdate_43D140();

    EXPORT void vRender_43D2B0(PrimHeader** ppOt);

private:
    Font_Context field_20_font_context;
    Alive::Font field_30_font;
    s32 field_68_tlvInfo;
    s16 field_6C_text_x;
    s16 field_6E_text_y;
    s16 field_70_polys_to_render_count;
    s16 field_72_switch_id;
    s16 field_74_tlv_x;
    s16 field_76_tlv_y;
    s16 field_78_count;
    s16 field_7A_number_of_meter_bars;
    s16 field_7C_mines_alarm_countdown;
    s16 field_7E_starting_switch_state;
    s16 field_80;
    s16 field_82_bar_count;
    Choice_short field_84_bStartFilled;
    s16 field_86;
    Poly_G4 field_88_polyG4s[2][kMeterBarsXCount];
};
ALIVE_ASSERT_SIZEOF(ColourfulMeter, 0x6c8);

ALIVE_VAR_EXTERN(u8, gTotalMeterBars_5C1BFA);
ALIVE_VAR_EXTERN(WORD, gbDrawMeterCountDown_5C1BF8);
