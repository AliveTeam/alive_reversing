#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "FixedPoint_common.hpp"

class GasEmitter final : public BaseGameObject
{
public:
    GasEmitter(relive::Path_GasEmitter* pTlv, const Guid& tlvId);
    ~GasEmitter();

    virtual void VStopAudio() override;
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

private:
    Guid mTlvId;
    s32 mEmitPower = 0;
    s16 field_28_draw_flipper = 0;
    s16 field_2A_switch_id = 0;
    relive::Path_GasEmitter::GasColour field_2C_gas_colour = relive::Path_GasEmitter::GasColour::eYellow;
    FP mEmitterXPos = {};
    FP mEmitterYPos = {};
    FP field_38_fp_not_used = {};
};
