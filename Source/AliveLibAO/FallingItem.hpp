#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"

START_NS_AO

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
    __int16 field_18_id;
    __int16 field_1A_scale;
    __int16 field_1C_delay_time;
    __int16 field_1E_number_of_items;
    __int16 field_20_reset_id;
    __int16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF(Path_FallingItem, 0x24);

class FallingItem : public BaseAliveGameObject
{
public:
    EXPORT void DamageHitItems_41A6D0();


    EXPORT FallingItem* ctor_419F30(Path_FallingItem* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_41A660();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT FallingItem* Vdtor_41A7F0(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_41A7C0();

    virtual void VUpdate() override;

    EXPORT void VUpdate_41A120();

    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;

private:
    enum class State : __int16
    {
        eState_0_WaitForIdEnable = 0,
        eState_1_GoWaitForDelay = 1,
        eState_2_WaitForFallDelay = 2,
        eState_3_Falling = 3,
        eState_4_Smashed = 4,
    };

public:
    int field_10C_tlvInfo;
    State field_110_state;
    unsigned __int16 field_112_id;
    __int16 field_114_num_items;
    __int16 field_116_num_items_remaining;
    __int16 field_118_delay_time;
    __int16 field_11A;
    int field_11C_delay_timer;
    __int16 field_120_reset_id;
    __int16 field_122_do_sound_in_state_falling;
    FP field_124_yPosStart;
    FP field_128_xpos;
    FP field_12C_ypos;
    int field_130_sound_channels;
    int field_134_created_gnFrame;

};
ALIVE_ASSERT_SIZEOF(FallingItem, 0x138);

END_NS_AO

