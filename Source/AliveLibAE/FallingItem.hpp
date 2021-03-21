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
    Scale_short field_12_scale;
    __int16 field_14_fall_delay;
    __int16 field_16_max_falling_items;
    __int16 field_18_reset_id;
    __int16 field_20_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_FallingItem, 0x1C);

class FallingItem : public BaseAliveGameObject
{
public:
    EXPORT FallingItem* ctor_4272C0(Path_FallingItem* pTlv, int tlvInfo);

    EXPORT FallingItem* ctor_427560(__int16 xpos, __int16 ypos, __int16 scale, __int16 id, __int16 delayTime, __int16 numItems, __int16 resetId);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

    virtual void VOnThrowableHit(BaseGameObject*) override
    {
        // FallingItem__vnullsub_75
    }

private:
    EXPORT void dtor_427EB0();

    EXPORT FallingItem* vdtor_427530(signed int flags);

    EXPORT void vScreenChanged_428180();

    EXPORT void vUpdate_427780();

    EXPORT void DamageHitItems_427F40();

private:
    int field_118_tlvInfo;
    enum class State : __int16
    {
        eState_0_WaitForIdEnable = 0,
        eState_1_GoWaitForDelay = 1,
        eState_2_WaitForFallDelay = 2,
        eState_3_Falling = 3,
        eState_4_Smashed = 4,
    };
    State field_11C_state;
    __int16 field_11E_id;
    __int16 field_120_max_falling_items;
    __int16 field_122_remaining_falling_items;
    __int16 field_124_fall_delay;
    //__int16 field_126_pad;
    int field_128_delay_timer;
    __int16 field_12C_reset_id;
    __int16 field_12E_do_sound_in_state_falling;
    FP field_130_yPosStart;
    __int16 field_134_bHitGrinderOrMineCar;
    //__int16 field_136_pad;
    FP field_138_xpos;
    FP field_13C_ypos;
    int field_140_sound_channels;
    int field_144_created_gnFrame;
};
ALIVE_ASSERT_SIZEOF(FallingItem, 0x148);
