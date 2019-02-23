#pragma once

#include "BaseGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"

struct Path_TimerTrigger : public Path_TLV
{
    __int16 field_10_id;
    unsigned __int16 field_12_delay_time;
    __int16 field_14_id1;
    __int16 field_16_id2;
    __int16 field_18_id3;
    __int16 field_1A_id4;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TimerTrigger, 0x1C);

enum class TimerTriggerStates : __int16
{
    eWaitForEnabled_0 = 0,
    eWaitForFirstTrigger_1 = 1,
    eCheckForStartAgain_2 = 2,
    eWaitForSecondTrigger_3 = 3,
};

struct TimerTrigger_State
{
    BaseGameObject::Types field_0_type;
    //__int16 field_2; // pad
    int field_4_tlvInfo;
    int field_8_delay_timer_base;
    TimerTriggerStates field_C_state;
    __int16 field_E_starting_switch_state;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(TimerTrigger_State, 0x10);

class TimerTrigger : public BaseGameObject
{
public:
    EXPORT TimerTrigger* ctor_4CDC20(Path_TimerTrigger* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual int VGetSaveState(BYTE* pSaveBuffer) override;

    EXPORT static signed int CC CreateFromSaveState_4CDF70(const BYTE* pData);

private:
    EXPORT TimerTrigger* vdtor_4CDD00(signed int flags);
    EXPORT void dtor_4CDD30();
    EXPORT void vUpdate_4CDDB0();
    EXPORT void ToggleAllIds_4CDEC0();
    EXPORT void vScreenChanged_4CDF00();
    EXPORT int vGetSaveState_4CE030(TimerTrigger_State* pState);

private:
    __int16 field_20_id;
    TimerTriggerStates field_22_state;
    __int16 field_24_ids[4];
    int field_2C_tlvInfo;
    int field_30_delay_timer;
    int field_34_delay_time;
    __int16 field_38_starting_switch_state;
    //__int16 field_3A; // pad
};
ALIVE_ASSERT_SIZEOF(TimerTrigger, 0x3C);
