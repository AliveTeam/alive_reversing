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

    EXPORT FallingItem* ctor_427560(__int16 xpos, __int16 ypos, __int16 scale, __int16 id, __int16 delayTime, __int16 numItems, __int16 resetId);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VUpdate() override
    {
        // TODO
    }

    virtual void VScreenChanged() override;

    virtual void vnull_4081A0(BaseGameObject*) override
    {
        // FallingItem__vnullsub_75
    }

private:
    EXPORT void dtor_427EB0();

    EXPORT FallingItem* vdtor_427530(signed int flags);

    EXPORT void vScreenChanged_428180();

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
    FP field_130_yPosStart;
    __int16 field_134_bUnknown;
    __int16 field_136_pad;
    FP field_138_xpos;
    FP field_13C_ypos;
    int field_140_sound_channels;
    int field_144_created_gnFrame;
};
ALIVE_ASSERT_SIZEOF(FallingItem, 0x148);
