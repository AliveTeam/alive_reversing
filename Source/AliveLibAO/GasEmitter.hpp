#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/FixedPoint_common.hpp"

namespace relive
{
    struct Path_GasEmitter;
}

namespace AO {

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
    FP mEmitterXPos = {};
    FP mEmitterYPos = {};
    FP field_20_fp_not_used = {};
};

} // namespace AO
