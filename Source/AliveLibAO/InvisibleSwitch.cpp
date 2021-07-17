#include "stdafx_ao.h"
#include "Function.hpp"
#include "InvisibleSwitch.hpp"
#include "stdlib.hpp"
#include "Abe.hpp"
#include "SwitchStates.hpp"
#include "Alarm.hpp"
#include "Events.hpp"
#include "Game.hpp"

namespace AO {

void InvisibleSwitch::VScreenChanged()
{
    VScreenChanged_433700();
}

InvisibleSwitch* InvisibleSwitch::Vdtor_433740(s32 flags)
{
    dtor_433540();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* InvisibleSwitch::VDestructor(s32 flags)
{
    return Vdtor_433740(flags);
}

BaseGameObject* InvisibleSwitch::dtor_433540()
{
    SetVTable(this, 0x4BB438);
    gMap_507BA8.TLV_Reset_446870(field_14_tlvInfo, -1, 0, 0);
    return dtor_487DF0();
}

InvisibleSwitch* InvisibleSwitch::ctor_4334E0(Path_InvisibleSwitch* pTlv, s32 tlvInfo)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BB438);
    field_14_tlvInfo = tlvInfo;
    field_28_state = States::eWaitForTrigger_0;
    field_10_id = pTlv->field_18_id;
    field_12_action = pTlv->field_1A_action;
    field_1C_delay = pTlv->field_1C_delay;
    field_2C_scale = pTlv->field_20_scale;
    field_20_top_left = pTlv->field_10_top_left;
    field_24_bottom_right = pTlv->field_14_bottom_right;
    field_2A_set_off_alarm = pTlv->field_1E_set_off_alarm;
    return this;
}

void InvisibleSwitch::VScreenChanged_433700()
{
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level || gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path)
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }

    if (field_28_state != States::eWaitForDelayTimer_1)
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }
}

void InvisibleSwitch::VUpdate()
{
    VUpdate_4335A0();
}

void InvisibleSwitch::VUpdate_4335A0()
{
    switch (field_28_state)
    {
        case States::eWaitForTrigger_0:
        {
            // sControlledCharacter_50767C can be nullptr during the game ender
            // Within X bounds?
            const FP charXPos = sControlledCharacter_50767C->field_A8_xpos;
            if (sControlledCharacter_50767C && charXPos >= FP_FromInteger(field_20_top_left.field_0_x) && charXPos <= FP_FromInteger(field_24_bottom_right.field_0_x))
            {
                // Within Y bounds?
                const FP charYPos = sControlledCharacter_50767C->field_AC_ypos;
                if (charYPos >= FP_FromInteger(field_20_top_left.field_2_y) && charYPos <= FP_FromInteger(field_24_bottom_right.field_2_y))
                {
                    // TODO: ???
                    if (sControlledCharacter_50767C != sActiveHero_507678
                        || (sActiveHero_507678->field_FC_current_motion != eAbeMotions::Motion_157_DoorExit_42D780
                            && sActiveHero_507678->field_FC_current_motion != eAbeMotions::Motion_156_DoorEnter_42D370))
                    {
                        // Scale matches ?
                        if (field_2C_scale == 2
                            || (field_2C_scale == 0 && sControlledCharacter_50767C->field_BC_sprite_scale == FP_FromDouble(0.5))
                            || (field_2C_scale == 1 && sControlledCharacter_50767C->field_BC_sprite_scale == FP_FromInteger(1)))
                        {
                            field_28_state = States::eWaitForDelayTimer_1;
                            field_18_delay_timer = gnFrameCount_507670 + field_1C_delay;
                        }
                    }
                }
            }
            break;
        }

        case States::eWaitForDelayTimer_1:
            if (field_18_delay_timer <= static_cast<s32>(gnFrameCount_507670))
            {
                SwitchStates_Do_Operation_436A10(field_10_id, field_12_action);
                if (field_2A_set_off_alarm)
                {
                    auto pAlarm = ao_new<Alarm>();
                    if (pAlarm)
                    {
                        pAlarm->ctor_402570(150, 0, 30, Layer::eLayer_Above_FG1_39);
                    }
                }
                field_28_state = States::eWaitForTrigger_0;
            }
            break;
    }

    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

} // namespace AO
