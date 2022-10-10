#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "FixedPoint_common.hpp"

class GasEmitter final : public BaseGameObject
{
public:
    GasEmitter(relive::Path_GasEmitter* pTlv, const Guid& tlvId);
    ~GasEmitter();
    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VStopAudio() override;

private:
    Guid field_20_tlvInfo;
    s32 field_24_emit_power = 0;
    s16 field_28_draw_flipper = 0;
    s16 field_2A_switch_id = 0;
    relive::Path_GasEmitter::GasColour field_2C_gas_colour = relive::Path_GasEmitter::GasColour::eYellow;
    FP field_30_xpos = {};
    FP field_34_ypos = {};
    FP field_38_fp_not_used = {};
};
