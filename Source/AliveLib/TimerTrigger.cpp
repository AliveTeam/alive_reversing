#include "stdafx.h"
#include "TimerTrigger.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "Events.hpp"

TimerTrigger* TimerTrigger::ctor_4CDC20(Path_TimerTrigger* pTlv, int tlvInfo)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x5478E8);
    field_2C_tlvInfo = tlvInfo;
    field_4_typeId = Types::eTimerTrigger_136;
    field_20_id = pTlv->field_10_id;
    field_34_delay_time = pTlv->field_12_delay_time;
    field_24_ids[0] = pTlv->field_14_id1;
    field_24_ids[1] = pTlv->field_16_id2;
    field_24_ids[2] = pTlv->field_18_id3;
    field_24_ids[3] = pTlv->field_1A_id4;
    field_38_starting_switch_state = static_cast<short>(SwitchStates_Get_466020(field_20_id));
    field_22_state = TimerTriggerStates::eWaitForEnabled_0;
    return this;
}

BaseGameObject* TimerTrigger::VDestructor(signed int flags)
{
    return vdtor_4CDD00(flags);
}

void TimerTrigger::VUpdate()
{
    vUpdate_4CDDB0();
}

void TimerTrigger::VScreenChanged()
{
    vScreenChanged_4CDF00();
}

int TimerTrigger::VGetSaveState(BYTE* pSaveBuffer)
{
    return vGetSaveState_4CE030(reinterpret_cast<TimerTrigger_State*>(pSaveBuffer));
}

EXPORT signed int CC TimerTrigger::CreateFromSaveState_4CDF70(const BYTE* pData)
{
    auto pState = reinterpret_cast<const TimerTrigger_State*>(pData);

    Path_TimerTrigger* pTlv = static_cast<Path_TimerTrigger*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pState->field_4_tlvInfo));
    auto pTimerTrigger = alive_new<TimerTrigger>();
    if (pTimerTrigger)
    {
        pTimerTrigger->ctor_4CDC20(pTlv, pState->field_4_tlvInfo);
        pTimerTrigger->field_22_state = pState->field_C_state;
        pTimerTrigger->field_30_delay_timer = sGnFrame_5C1B84 + pState->field_8_delay_timer_base;
        pTimerTrigger->field_38_starting_switch_state = pState->field_E_starting_switch_state;

    }
    return sizeof(TimerTrigger_State);
}

TimerTrigger* TimerTrigger::vdtor_4CDD00(signed int flags)
{
    dtor_4CDD30();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void TimerTrigger::dtor_4CDD30()
{
    SetVTable(this, 0x5478E8);
    Path::TLV_Reset_4DB8E0(field_2C_tlvInfo, -1, 0, 0);
    BaseGameObject_dtor_4DBEC0();
}

void TimerTrigger::vUpdate_4CDDB0()
{
    switch (field_22_state)
    {
    case TimerTriggerStates::eWaitForEnabled_0:
        if (SwitchStates_Get_466020(field_20_id) != field_38_starting_switch_state)
        {
            field_22_state = TimerTriggerStates::eWaitForFirstTrigger_1;
            field_30_delay_timer = sGnFrame_5C1B84 + field_34_delay_time;
        }
        break;

    case TimerTriggerStates::eWaitForFirstTrigger_1:
        if (field_30_delay_timer <= static_cast<int>(sGnFrame_5C1B84))
        {
            ToggleAllIds_4CDEC0();
            field_22_state = TimerTriggerStates::eCheckForStartAgain_2;
        }
        break;

    case TimerTriggerStates::eCheckForStartAgain_2:
        if (SwitchStates_Get_466020(field_20_id) == field_38_starting_switch_state)
        {
            field_22_state = TimerTriggerStates::eWaitForSecondTrigger_3;
            field_30_delay_timer = sGnFrame_5C1B84 + field_34_delay_time;
        }
        break;

    case TimerTriggerStates::eWaitForSecondTrigger_3:
        if (field_30_delay_timer <= (signed int)sGnFrame_5C1B84)
        {
            ToggleAllIds_4CDEC0();
            field_22_state = TimerTriggerStates::eWaitForEnabled_0;
        }
        break;
    }

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}

void TimerTrigger::ToggleAllIds_4CDEC0()
{
    for (auto& id : field_24_ids)
    {
        if (id != 0)
        {
            SwitchStates_Do_Operation_465F00(id, SwitchOp::eToggle_2);
        }
    }
}

void TimerTrigger::vScreenChanged_4CDF00()
{
    if (field_22_state == TimerTriggerStates::eWaitForEnabled_0 || field_22_state == TimerTriggerStates::eCheckForStartAgain_2 ||
        gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId ||
        gMap_5C3030.sCurrentPathId_5C3032 != gMap_5C3030.field_C_5C303C_pathId ||
        gMap_5C3030.field_22 != gMap_5C3030.Get_Path_Unknown_480710())
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}

int TimerTrigger::vGetSaveState_4CE030(TimerTrigger_State* pState)
{
    pState->field_0_type = Types::eTimerTrigger_136;
    pState->field_4_tlvInfo = field_2C_tlvInfo;
    pState->field_C_state = field_22_state;
    pState->field_8_delay_timer_base = field_30_delay_timer - sGnFrame_5C1B84;
    pState->field_E_starting_switch_state = field_38_starting_switch_state;
    return sizeof(TimerTrigger_State);
}
