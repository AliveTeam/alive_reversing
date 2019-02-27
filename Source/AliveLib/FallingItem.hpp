#pragma once

#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"

struct FallingItem_Data
{
    int field_0;
    int field_4;
    __int16 field_8;
    __int16 field_A;
};
ALIVE_ASSERT_SIZEOF(FallingItem_Data, 0xC);

struct Path_FallingItem : public Path_TLV
{
    __int16 field_10_id;
    __int16 field_12_scale;
    __int16 field_14_delay_time;
    __int16 field_16_number_of_items;
    __int16 field_18_reset_id;
    // pad
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_FallingItem, 0x1C);

class FallingItem : public BaseAliveGameObject
{
public:
    EXPORT FallingItem* ctor_4272C0(Path_FallingItem* pTlv, int tlvInfo);
private:
    __int16 field_116_pad;
    int field_118_tlvInfo;
    __int16 field_11C_state;
    __int16 field_11E_id;
    __int16 field_120_num_items;
    __int16 field_122_num_items_remaining;
    __int16 field_124_delay_time;
    __int16 field_126_pad;
    int field_128_delay_timer;
    __int16 field_12C_reset_id;
    __int16 field_12E_do_sound_in_state_3;
    int field_130_yPosStart;
    __int16 field_134_bUnknown;
    __int16 field_136_pad;
    int field_138_xpos;
    int field_13C_ypos;
    int field_140_sound_channels;
    int field_144_created_gnFrame;
};
ALIVE_ASSERT_SIZEOF(FallingItem, 0x148);
