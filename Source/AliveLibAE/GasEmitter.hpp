#pragma once

#include "BaseGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"

enum class GasColour : s16
{
    Yellow_0,
    Red_1,
    Green_2,
    Blue_3,
    White_4
};

struct Path_GasEmitter final : public Path_TLV
{
    s16 field_10_switch_id;
    GasColour field_12_colour;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_GasEmitter, 0x14);

class GasEmitter final : public BaseGameObject
{
public:
    EXPORT GasEmitter* ctor_43CAA0(Path_GasEmitter* pTlv, s32 tlvInfo);

    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VStopAudio() override;

private:
    EXPORT void dtor_43CD20();
    EXPORT GasEmitter* vdtor_43CB40(s32 flags);
    EXPORT void vStopAudio_43CDE0();
    EXPORT void vScreenChanged_43CE20();
    EXPORT void vUpdate_43CB70();

private:
    s32 field_20_tlvInfo;
    s32 field_24_emit_power;
    s16 field_28_draw_flipper;
    s16 field_2A_switch_id;
    GasColour field_2C_gas_colour;
    s16 field_2E_pad;
    FP field_30_xpos;
    FP field_34_ypos;
    FP field_38_fp_not_used;
};
ALIVE_ASSERT_SIZEOF(GasEmitter, 0x3C);
