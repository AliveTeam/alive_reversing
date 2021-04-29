#pragma once

#include "BaseGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"

struct Path_TimerTrigger final : public Path_TLV
{
    s16 field_10_id;
    u16 field_12_trigger_delay;
    s16 field_14_id1;
    s16 field_16_id2;
    s16 field_18_id3;
    s16 field_1A_id4;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TimerTrigger, 0x1C);

enum class TimerTriggerStates : s16
{
    eWaitForEnabled_0 = 0,
    eWaitForFirstTrigger_1 = 1,
    eCheckForStartAgain_2 = 2,
    eWaitForSecondTrigger_3 = 3,
};

struct TimerTrigger_State final
{
    AETypes field_0_type;
    s32 field_4_tlvInfo;
    s32 field_8_delay_timer_base;
    TimerTriggerStates field_C_state;
    s16 field_E_starting_switch_state;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(TimerTrigger_State, 0x10);

class TimerTrigger final : public BaseGameObject
{
public:
    EXPORT TimerTrigger* ctor_4CDC20(Path_TimerTrigger* pTlv, s32 tlvInfo);

    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    EXPORT static s32 CC CreateFromSaveState_4CDF70(const u8* pData);

private:
    EXPORT TimerTrigger* vdtor_4CDD00(s32 flags);
    EXPORT void dtor_4CDD30();
    EXPORT void vUpdate_4CDDB0();
    EXPORT void ToggleAllIds_4CDEC0();
    EXPORT void vScreenChanged_4CDF00();
    EXPORT s32 vGetSaveState_4CE030(TimerTrigger_State* pState);

private:
    s16 field_20_id;
    TimerTriggerStates field_22_state;
    s16 field_24_ids[4];
    s32 field_2C_tlvInfo;
    s32 field_30_trigger_delay_timer;
    s32 field_34_trigger_delay;
    s16 field_38_starting_switch_state;
};
ALIVE_ASSERT_SIZEOF(TimerTrigger, 0x3C);
