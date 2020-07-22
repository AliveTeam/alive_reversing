#include "stdafx_ao.h"
#include "Function.hpp"
#include "SlingMudokon.hpp"

void SlingMud_ForceLink() { }

START_NS_AO

using TSlingMudStateFunction = decltype(&SlingMudokon::State_0_Idle_46FCB0);

const TSlingMudStateFunction gSlingMudMotionTable_4CFCB0[] =
{
    &SlingMudokon::State_0_Idle_46FCB0,
    &SlingMudokon::State_1_Angry_46FCF0,
    &SlingMudokon::State_2_Speak_46FD70,
    &SlingMudokon::State_3_ShootStart_46FD90,
    &SlingMudokon::State_4_ShootEnd_46FEA0,
    &SlingMudokon::State_5_AngryToIdle_46FD50
};

void SlingMudokon::VUpdate_Real_46FBF0()
{
    NOT_IMPLEMENTED();
}

void SlingMudokon::VUpdate_46FBF0()
{
    const __int16 oldMotion = field_FC_current_motion;

    VUpdate_Real_46FBF0();

    if (oldMotion != field_FC_current_motion)
    {
        LOG_INFO("oldMotion = " << oldMotion << " newMotion = " << field_FC_current_motion);
    }
}

void SlingMudokon::State_0_Idle_46FCB0()
{
    NOT_IMPLEMENTED();
}

void SlingMudokon::State_1_Angry_46FCF0()
{
    NOT_IMPLEMENTED();
}

void SlingMudokon::State_2_Speak_46FD70()
{
    NOT_IMPLEMENTED();
}

void SlingMudokon::State_3_ShootStart_46FD90()
{
    NOT_IMPLEMENTED();
}

void SlingMudokon::State_4_ShootEnd_46FEA0()
{
    NOT_IMPLEMENTED();
}

void SlingMudokon::State_5_AngryToIdle_46FD50()
{
    NOT_IMPLEMENTED();
}

END_NS_AO
