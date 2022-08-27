#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/Function.hpp"

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
    s16 mSwitchId;
    GasColour mColour;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_GasEmitter, 0x14);

class GasEmitter final : public BaseGameObject
{
public:
    GasEmitter(relive::Path_GasEmitter* pTlv, s32 tlvInfo);
    ~GasEmitter();
    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VStopAudio() override;

private:
    s32 field_20_tlvInfo = 0;
    s32 field_24_emit_power = 0;
    s16 field_28_draw_flipper = 0;
    s16 field_2A_switch_id = 0;
    GasColour field_2C_gas_colour = GasColour::Yellow_0;
    FP field_30_xpos = {};
    FP field_34_ypos = {};
    FP field_38_fp_not_used = {};
};
ALIVE_ASSERT_SIZEOF(GasEmitter, 0x3C);
