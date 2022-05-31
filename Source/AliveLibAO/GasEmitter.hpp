#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseGameObject.hpp"

namespace AO {

struct Path_GasEmitter final : public Path_TLV
{
};

class GasEmitter final : public BaseGameObject
{
public:
    GasEmitter(Path_GasEmitter* pTlv, s32 tlvInfo);
    ~GasEmitter();

    virtual void VStopAudio() override;
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    s32 field_10_tlvInfo;
    s32 field_14_emit_power;
    FP field_18_xpos;
    FP field_1C_ypos;
    FP field_20_fp_not_used;
};
ALIVE_ASSERT_SIZEOF(GasEmitter, 0x24);

} // namespace AO
