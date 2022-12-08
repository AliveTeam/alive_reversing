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
    s16 mDrawFlipper = 0;
    s16 mSwitchId = 0;
    relive::Path_GasEmitter::GasColour mGasColour = relive::Path_GasEmitter::GasColour::eYellow;
    FP mEmitterXPos = {};
    FP mEmitterYPos = {};
};
