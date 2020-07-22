#include "stdafx_ao.h"
#include "Function.hpp"
#include "Slog.hpp"

void Slog_ForceLink() {}

START_NS_AO


using TSlogStateFunction = decltype(&Slog::State_0_Idle_4742E0);

const TSlogStateFunction sSlogMotionTable_4CFD30[] =
{
    &Slog::State_0_Idle_4742E0,
    &Slog::State_1_Walk_4743F0,
    &Slog::State_2_Run_4749A0,
    &Slog::State_3_TurnAround_474C70,
    &Slog::State_4_Fall_4750C0,
    &Slog::State_5_Unknown_474070,
    &Slog::State_6_MoveHeadUpwards_474220,
    &Slog::State_7_SlideTurn_474DB0,
    &Slog::State_8_StopRunning_474EC0,
    &Slog::State_9_StartWalking_474690,
    &Slog::State_10_EndWalking_4747D0,
    &Slog::State_11_Land_475AB0,
    &Slog::State_12_Unknown_475B50,
    &Slog::State_13_StartFastBarking_475B70,
    &Slog::State_14_EndFastBarking_475BB0,
    &Slog::State_15_Empty_475290,
    &Slog::State_16_Sleeping_4752E0,
    &Slog::State_17_FallAsleep_475510,
    &Slog::State_18_WakeUp_475460,
    &Slog::State_19_JumpForwards_475610,
    &Slog::State_20_JumpUpwards_475890,
    &Slog::State_21_Eating_475900,
    &Slog::State_22_Empty_475A90,
    &Slog::State_23_Scratch_475550,
    &Slog::State_24_Growl_475590,
};


void Slog::VUpdate_Real_4739C0()
{
    NOT_IMPLEMENTED();
}

void Slog::VUpdate_4739C0()
{
    const __int16 oldMotion = field_FC_current_motion;

    VUpdate_Real_4739C0();

    if (oldMotion != field_FC_current_motion)
    {
        LOG_INFO("oldMotion = " << oldMotion << " newMotion = " << field_FC_current_motion);
    }
}

void Slog::State_0_Idle_4742E0()
{
    NOT_IMPLEMENTED();
}

void Slog::State_1_Walk_4743F0()
{
    NOT_IMPLEMENTED();
}

void Slog::State_2_Run_4749A0()
{
    NOT_IMPLEMENTED();
}

void Slog::State_3_TurnAround_474C70()
{
    NOT_IMPLEMENTED();
}

void Slog::State_4_Fall_4750C0()
{
    NOT_IMPLEMENTED();
}

void Slog::State_5_Unknown_474070()
{
    NOT_IMPLEMENTED();
}

void Slog::State_6_MoveHeadUpwards_474220()
{
    NOT_IMPLEMENTED();
}

void Slog::State_7_SlideTurn_474DB0()
{
    NOT_IMPLEMENTED();
}

void Slog::State_8_StopRunning_474EC0()
{
    NOT_IMPLEMENTED();
}

void Slog::State_9_StartWalking_474690()
{
    NOT_IMPLEMENTED();
}

void Slog::State_10_EndWalking_4747D0()
{
    NOT_IMPLEMENTED();
}

void Slog::State_11_Land_475AB0()
{
    NOT_IMPLEMENTED();
}

void Slog::State_12_Unknown_475B50()
{
    NOT_IMPLEMENTED();
}

void Slog::State_13_StartFastBarking_475B70()
{
    NOT_IMPLEMENTED();
}

void Slog::State_14_EndFastBarking_475BB0()
{
    NOT_IMPLEMENTED();
}

void Slog::State_15_Empty_475290()
{
    NOT_IMPLEMENTED();
}

void Slog::State_16_Sleeping_4752E0()
{
    NOT_IMPLEMENTED();
}

void Slog::State_17_FallAsleep_475510()
{
    NOT_IMPLEMENTED();
}

void Slog::State_18_WakeUp_475460()
{
    NOT_IMPLEMENTED();
}

void Slog::State_19_JumpForwards_475610()
{
    NOT_IMPLEMENTED();
}

void Slog::State_20_JumpUpwards_475890()
{
    NOT_IMPLEMENTED();
}

void Slog::State_21_Eating_475900()
{
    NOT_IMPLEMENTED();
}

void Slog::State_22_Empty_475A90()
{
    NOT_IMPLEMENTED();
}

void Slog::State_23_Scratch_475550()
{
    NOT_IMPLEMENTED();
}

void Slog::State_24_Growl_475590()
{
    NOT_IMPLEMENTED();
}

END_NS_AO

