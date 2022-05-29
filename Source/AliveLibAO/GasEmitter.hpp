#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
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

    EXPORT void VStopAudio_41D8D0();
    
    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_41D900();

    virtual void VUpdate() override;

    EXPORT void VUpdate_41D7D0();

    s32 field_10_tlvInfo;
    s32 field_14_emit_power;
    FP field_18_xpos;
    FP field_1C_ypos;
    FP field_20_fp_not_used;
};
ALIVE_ASSERT_SIZEOF(GasEmitter, 0x24);

} // namespace AO
