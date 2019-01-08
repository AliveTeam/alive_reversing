#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "BitField.hpp"
#include "FunctionFwd.hpp"

struct Path_Grinder_Data
{
    __int16 field_10_scale;
    __int16 field_12_min_off_time;
    __int16 field_14_max_off_time;
    __int16 field_16_id;
    __int16 field_18_behavior;
    __int16 field_1A_speed;
    __int16 field_1C_start_state;
    __int16 field_1E_off_speed;
    __int16 field_20_min_off_time2;
    __int16 field_22_max_off_time2;
    __int16 field_24_start_position;
    __int16 field_26_direction;
};
ALIVE_ASSERT_SIZEOF(Path_Grinder_Data, 0x18);

struct Path_Grinder : public Path_TLV
{
    Path_Grinder_Data field_10_data;
};
ALIVE_ASSERT_SIZEOF(Path_Grinder, 0x28);

class Grinder : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Grinder* ctor_4200D0(Path_Grinder* pTlv, DWORD tlvInfo);

    EXPORT void vScreenChanged_4214B0();

private:
    int field_E4_not_used[4];
    __int16 field_F4_state;
    __int16 field_F6_width;
    __int16 field_F8_id;
    __int16 field_FA_direction;
    __int16 field_FC_min_off_time;
    __int16 field_FE_max_off_time;
    __int16 field_100_min_off_time2;
    __int16 field_102_max_off_time2;
    DWORD field_104_tlv;
    int field_108_off_timer;
    int field_10C_audio_channels_mask;
    FP field_110_xPos;
    FP field_114_yPos;
    FP field_118_speed;
    FP field_11C_speed2;
    FP field_120_off_speed;
    FP field_124_xyoff;
    enum Flags
    {
        eBit1 = 0x1,
        eBit2_StartOn = 0x2,
        eBit3_UseId = 0x4,
        eBit4_Toggle = 0x8,
        eBit5 = 0x10,
        eBit6_StartPos = 0x20,
    };
    BitField16<Flags> field_128_flags;
    __int16 field_12A_pad;
};
ALIVE_ASSERT_SIZEOF(Grinder, 0x12C);
