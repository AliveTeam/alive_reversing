#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "../relive_lib/BaseGameObject.hpp"

namespace AO {

class GasEmitter final : public ::BaseGameObject
{
public:
    GasEmitter(relive::Path_GasEmitter* pTlv, const Guid& tlvId);
    ~GasEmitter();

    virtual void VStopAudio() override;
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    Guid field_10_tlvInfo;
    s32 field_14_emit_power = 0;
    FP field_18_xpos = {};
    FP field_1C_ypos = {};
    FP field_20_fp_not_used = {};
};
ALIVE_ASSERT_SIZEOF(GasEmitter, 0x24);

} // namespace AO
